/****************************************
 *  Universita' degli Studi di Udine    *
 *  Ingegneria Elettronica              *
 *  Tesi di Laurea Triennale            *
 *  Giacomo Andrioli - 133047           *
 *  File: ServerAMQP.cpp 	            *
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


#include "ServerAMQP.hpp"

/*
 *      CLASSE ServerAMQP 
 */

/* 
 * Descrizione: la funzione stampa a linea di comando un messaggio
 * Parametri:
 *      - message: stringa da stampare
*/
void AMQP::ServerAMQP::Debug(std::string message)
{
    if (_VERBOSE)
        std::cout << message << std::endl;
}

/* 
 * COSTRUTTORE
 * Parametri:
 *      - server_url: stringa dell'indirizzo tcp su cui il server ascolta le
 *          connessioni in entrata: "IP:PORTA"
 *      - verbose: abilita le stampe di debug sulla linea di comando
 * 
*/
AMQP::ServerAMQP::ServerAMQP(const std::string &server_url, bool verbose) 
    : listen_handler(verbose), url(server_url) 
{
    _VERBOSE = verbose;
    response_function = nullptr;
}


std::string AMQP::ServerAMQP::generate_address() 
{
    std::ostringstream addr;
    addr << "server" << address_counter++;
    return addr.str();
}


/*
 * Descrizione: la funzione imposta il puntatore alla funzione che viene chiamata 
 *     quando viene ricevuto un messaggio per ottenere il body della risposta.
 */
void AMQP::ServerAMQP::SetMessageHandler(std::string(*handler)(const proton::message&))
{
    if (handler == nullptr)
        throw std::invalid_argument("Invalid nullptr handler");
    
    response_function = handler;
}

void AMQP::ServerAMQP::on_message(proton::delivery& d, proton::message& msg)
{
    Debug("Message: " + proton::get<std::string>(msg.body()));

    // Accetta la delivery: il client viene informato che la consegna è
    // presa in carico dal server
    d.accept();

    // Indirizzo di risposta
    std::string reply_to = msg.reply_to();

    if (senders.find(reply_to) == senders.end()) {
        Debug("Error: no link for reply_to: " + reply_to);
    } 
    else {
        // Cerca il sender nell'elenco
        proton::sender sender = senders.find(reply_to)->second;

        // Messaggio di risposta
        proton::message reply;

        // Imposta il destinatario
        reply.to(reply_to);
        
        // Usa la funzione puntata da response_function per ottenere il body
        // da mandare in risposta
        reply.body((*this->response_function)(msg));
        
        // Correla il messaggio da inviare a quello ricevuto, 
        // in questo modo il client può sapere a quale messaggio appartiene la richiesta 
        reply.correlation_id(msg.correlation_id());

        // Invia il messaggio di risposta
        sender.send(reply);
    }
}


void AMQP::ServerAMQP::on_container_start(proton::container &c) 
{
    // Se non è stato specificata la funzione che fornisce il messaggio per 
    // le risposte ai client, lancia un'eccezione
    if (response_function == nullptr)
        throw std::logic_error("Invalid nullptr in messagge handler function pointer");
    
    Debug("Container started");

    // Ascolta le connessioni in entrata
    listener = c.listen(url, listen_handler);
}





void AMQP::ServerAMQP::on_sender_open(proton::sender &sender) 
{
    // Il client deve aver aperto il sender richiedendo un indirizzo dinamico
    if (sender.source().dynamic()) 
    {
        // Genera un indirizzo unico per il sender
        // Il client invierà messaggi al server usando questo indirizzo
        std::string address = generate_address();

        // Apertura del sender
        sender.open(proton::sender_options().source(proton::source_options().address(address)));

        // Salva il sender nell'elenco, associandolo all'indirizzo
        senders[address] = sender;

        Debug("Sender opened");
    }
}


// L'evento viene chiamato quando viene aperta una connessione
void AMQP::ServerAMQP::on_connection_open(proton::connection & c)
{
    // Se la connessione non era già stata aperta prima
    if(!c.reconnected())
    {
        // Apertura nuova connessione
        c.open();
        Debug("Connection opened");
        return;
    }

    // Non va aperta una nuova connessione nel caso di riconnessione
    Debug("Reconnection");
}

void AMQP::ServerAMQP::on_connection_close(proton::connection & c)
{
    // Chiude la connessione
    c.close();

    Debug("Connection closed");
}




// Eventi in overriding solo per debug a linea di comando
void AMQP::ServerAMQP::on_connection_error(proton::connection & c)
{
    Debug("Connection error" + c.error().what());
}

void AMQP::ServerAMQP::on_transport_open(proton::transport & t)
{
    Debug("Transport opened");
}

void AMQP::ServerAMQP::on_transport_error(proton::transport & t)
{
    Debug("Connection error" + t.error().what());
}

void AMQP::ServerAMQP::on_transport_close(proton::transport &)
{
    Debug("Transport close");
}

void AMQP::ServerAMQP::on_tracker_accept(proton::tracker &t)
{
    Debug("Tracker accept");
}






/*
 *      Classe listener_ready_handler 
 */

AMQP::ServerAMQP::listener_ready_handler::listener_ready_handler(bool verbose)
: verbose(verbose)
{}

void AMQP::ServerAMQP::listener_ready_handler::on_open(proton::listener& l)
{
    if (verbose)
        std::cout << "Server listening on " << l.port() << std::endl;
}


void AMQP::ServerAMQP::listener_ready_handler::on_error(proton::listener& l, const std::string& error_string)
{
    if (verbose)
        std::cout << error_string << std::endl;
}








