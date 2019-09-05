/****************************************
 *  Universita' degli Studi di Udine    *
 *  Ingegneria Elettronica              *
 *  Tesi di Laurea Triennale            *
 *  Giacomo Andrioli - 133047           *
 *  File: ClientAMQP.cpp 	            *
 ****************************************/

/* Licence inherited from the use of the open source QPID Proton library
 * available at: https://github.com/apache/qpid-proton
 * Copyright 2019 Giacomo Andrioli
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "ClientAMQP.hpp"


/*
 * Costruttore
 * Argomenti:
 * 		- server_address: indica l'url del server a cui connettersi. Un esempio di URL
 * 			e' la coppia ip:porta 127.0.0.1:5672
 * 		- verbose: abilita la stampa a linea di comando (cerr) per il debug. Utilizza il 
 * 			mutex OutputLocker per evitare problemi sulla risorsa condivisa. 
*/
AMQP::ClientAMQP::ClientAMQP (std::string server_address, bool verbose)
    : server_address(server_address)
{
	_VERBOSE = verbose;
    id_counter = 0;
	
	// Impostazione delle opzioni per la riconnessione automatica del client
	rc_opts.delay(proton::duration(500));
	rc_opts.delay_multiplier(2);
	rc_opts.max_delay(proton::duration(5000));
	rc_opts.max_attempts(0);

	c_opts.reconnect(rc_opts);
}


/*
 * Descrizione: La funzione stampa a linea di comando (cerr) il messaggio passato come argomento.
 * 		La funzione è thread safe grazie al mutex OutputLocker
 */
void AMQP::ClientAMQP::Debug(std::string message) 
{ 
    if(_VERBOSE) 
    {
        OutputLocker.lock();
        std::cerr << "CONTAINER: " << message << std::endl;
        OutputLocker.unlock();
    }
}


/*
 * Descrizione: la funzione inserisce l'argomento string 'message' nel body
 * 		di un messaggio AMQP e aggiunge alla coda di lavoro l'invio del messaggio
 */
void AMQP::ClientAMQP::send(std::string message) 
{
	// Preparazione del messaggio da inviare
	proton::message msg;
	msg.id(id_counter);	// id univoco
	msg.body(message);	// inserisce l'argomento nel body
	Debug("Reply_to address: " + receiver.source().address());
	
	// Indirizzo assegnato dinamicamente dal server all'apertura della connessione per le richieste del client
	msg.reply_to(receiver.source().address());	

	// Incremento contatore degli id
	id_counter++;

	// Inserisce nella coda di lavoro una funzione anonima (tramite espressione lamdba)
	// che invierà il messaggio appena possibile.
	// E' necessario l'uso del 'capturing by copy' [=] nella lambda expression,
	// in quanto msg ha scope locale
	work_queue()->add([=]() { sender.send(msg); });
}

/* 
 * Descrizione: La funzione restituisce il messaggio in testa alla coda di messaggi ricevuti.
 * 		Se la coda è vuota, blocca l'esecuzione in attesa di un messaggio. E' Thread safe, 
 * 		grazie all'utilizzo del mutex locker.
*/
proton::message AMQP::ClientAMQP::receive() 
{
	std::unique_lock<std::mutex> ulock(locker);		// Evita i conflitti sulle risorse condivise
	
	while (msg_queue.empty()) 
		messages_ready.wait(ulock);	// Sblocca l'esecuzione quando viene segnalato
									// l'arrivo di un messaggio tramite message_ready.notify_all()	
	
	// Estrae l'elemento di testa nella coda e lo restituisce
	proton::message msg = std::move(msg_queue.front());
	msg_queue.pop();
	return msg;
}

/* 
 * Descrizione: La funzione restituisce il messaggio in testa alla coda di messaggi ricevuti
 * 		nel messaggio passato come parametro per riferimento. Restituisce true se e' stato letto
 * 		un messaggio, false se la coda e' vuota.
 * 		Se la coda e' vuota, non blocca l'esecuzione in attesa di un messaggio. E' Thread safe, 
 * 		grazie all'utilizzo del mutex locker.
*/
bool AMQP::ClientAMQP::receive(proton::message& message)
{
	std::unique_lock<std::mutex> ulock(locker);		// Evita i conflitti sulle risorse condivise
	
	if (msg_queue.empty())
		return false;
	
	message = std::move(msg_queue.front());
	msg_queue.pop();
	return true;
}

/* 
 * Descrizione: la funzione inserisce la chiusura della connessione nella coda dei lavori
*/
void AMQP::ClientAMQP::close ()
{
	work_queue()->add([=]() { sender.connection().close(); });
}

/*
 * Descrizione: restituisce il puntatore alla coda di lavoro. Se questa non e' stata ancora
 * 		inizializzata dopo l'apertura di un sender, viene bloccata l'esecuzione in attesa dell'evento
 */
proton::work_queue* AMQP::ClientAMQP::work_queue()
{
	std::unique_lock<std::mutex> ulock(locker);
	while(!w_queue)
		sender_ready.wait(ulock);
	return w_queue;
}






/*
 * Evento: start del container a seguito di una chiamata proton::container::run 
*/
void AMQP::ClientAMQP::on_container_start(proton::container & c) 
{
	Debug("Container start");

	// Richiesta di apertura della connessione al server
	// Specificando le opzioni c_opts impostate nel costruttore
	c.connect(server_address, c_opts);
}

/* 
 * Evento: apertura di un sender
 * Descrizione:	inizializza la coda di lavoro e salva nell'attributo sender
 * 		il proton::sender che e' stato aperto
*/
void AMQP::ClientAMQP::on_sender_open(proton::sender& s)
{
	// Dalla documentazione della libreria: Sender and work_queue must be set atomically
	std::lock_guard<std::mutex> l(locker);
	sender = s;

	// Inizializza la coda di lavoro
	w_queue = &s.work_queue();

	// Notifica l'inizializzazione del sender
	sender_ready.notify_all();
}


/* 
 * Evento: arrivo di un messaggio
 * Descrizione:	inserisce il messaggio nella coda dei messaggi ricevuti
*/
void AMQP::ClientAMQP::on_message(proton::delivery &d, proton::message &msg)
{
	std::lock_guard<std::mutex> ulock(locker);

	msg_queue.push(msg);

	messages_ready.notify_all();

	Debug("Received message");
}

/* 
 * Evento: richiesta di connessione
 * Descrizione:	accetta la connessione, apre un sender e un receiver
*/
void AMQP::ClientAMQP::on_connection_open(proton::connection & c)
{
	if(!c.reconnected())
	{
		// Accetta la richiesta di connessione
		c.open();
		Debug("Connection opened");

		// Apertura del sender
		try {
			// Il sender viene aperto con target all'indirizzo del server 
			sender = c.open_sender(server_address);
		} catch(proton::error & e) {
			Debug("Sender open failed: " + std::string(e.what()));
		}

		// Apertura del receiver
		try {
			// L'opzione dynamic richiede al server di assegnare un indirizzo univoco per le richieste
			proton::receiver_options opts = proton::receiver_options().source(proton::source_options().dynamic(true));

			receiver = sender.connection().open_receiver("", opts);
		} catch(proton::error & e) {
			Debug("Receiver open failed: " + std::string(e.what()));
		}
	}
	else
	{
		// Non va aperta una connessione nel caso di una riconnessione
		Debug("Connection reopened");
	}
	
}

/*
 * Evento: il server ha accettato il messaggio inviato
 */
void AMQP::ClientAMQP::on_tracker_accept(proton::tracker &t)  
{
	// Settle del tracker, serve per segnalare che il messaggio e' stato 
	// consegnato correttamente e che può essere eliminato dai buffer 
	t.settle();
	Debug("Delivery confirmed, delivery settled");
}






// Events for debug only

void AMQP::ClientAMQP::on_connection_error(proton::connection & c) 
{
	Debug("Connection error: " + std::string(c.error().what()));
}

void AMQP::ClientAMQP::on_transport_open(proton::transport & t) 
{
	Debug("Transport open");
}

void AMQP::ClientAMQP::on_connection_close(proton::connection & c) 
{
	Debug("Connection closed");
} 

void AMQP::ClientAMQP::on_transport_error(proton::transport & t) 
{
	Debug("Transport error: " + std::string(t.error().what()));
}

void AMQP::ClientAMQP::on_transport_close(proton::transport & t)  
{
	Debug("Transport close");
}

void AMQP::ClientAMQP::on_receiver_open(proton::receiver & r)  
{
	Debug("Receiver Opened");
}







