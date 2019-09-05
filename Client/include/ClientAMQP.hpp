/****************************************
 *  Universita' degli Studi di Udine    *
 *  Ingegneria Elettronica              *
 *  Tesi di Laurea Triennale            *
 *  Giacomo Andrioli - 133047           *
 *  File: ClientAMQP.hpp 	            *
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

#ifndef CLIENTAMQP_HPP
#define CLIENTAMQP_HPP

#include <proton/connection.hpp>
#include <proton/connection_options.hpp>
#include <proton/container.hpp>
#include <proton/listen_handler.hpp>
#include <proton/listener.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/message_id.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/link.hpp>
#include <proton/listen_handler.hpp>
#include <proton/listener.hpp>
#include <proton/receiver_options.hpp>
#include <proton/reconnect_options.hpp>
#include <proton/sender_options.hpp>
#include <proton/source_options.hpp>
#include <proton/tracker.hpp>
#include <proton/transport.hpp>
#include <proton/types.hpp>
#include <proton/value.hpp>
#include <proton/work_queue.hpp>

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <condition_variable>


namespace AMQP
{
    class ClientAMQP : public proton::messaging_handler {
        private:
            // Server URL
            std::string server_address;

            // Opzioni
            proton::connection_options c_opts;
            proton::reconnect_options rc_opts;
            
            // Gestione risorse condivise
            std::mutex locker;
            std::condition_variable sender_ready;
            std::condition_variable messages_ready;

            // Code
            std::queue<proton::message> msg_queue;      // NB: risorsa condivisa, da bloccare con locker
            proton::work_queue* w_queue;    // Coda delle operazioni da eseguire. NB: Thread safe         

            // Link endpoint
            proton::sender sender;      // In uscita
            proton::receiver receiver;  // In entrata

            unsigned int id_counter;    // Contatore per gli id dei messaggi
            
            // Flag per l'abilitazione del debug a linea di comando
            bool _VERBOSE;

            // Metodo per la stampa di debug a riga di comando. (Thread safe rispetto a OutputLocker)
            void Debug(std::string message);    
            
        public:
            std::mutex OutputLocker;    // Locker per l'output a linea di comando

            // Costruttori
            ClientAMQP (std::string server_address, bool verbose = false);

            // Metodi thread safe
            void send(std::string message);             // Inserire un messaggio nella coda di invio dall'esterno della classe        
            proton::message receive();                  // Lettura messaggi all'esterno della classe, modalità blocking
            bool receive(proton::message& message);     // Lettura messaggi all'esterno della classe, modalità non-blocking
            void close ();                              // Richiede la chiusura della connessione

        private:
            // Restituisce la coda di lavoro, se questa e' stata inizializzata, altrimenti aspetta
            // che venga inizializzata
            proton::work_queue* work_queue();   // NB: thread safe

            // Eventi
            void on_container_start(proton::container & c) override;
            void on_sender_open(proton::sender& s) override;
            void on_message(proton::delivery &d, proton::message &msg) override;
            void on_connection_open(proton::connection & c) override;
            void on_tracker_accept(proton::tracker &t) override;

            // Eventi in overriding solo per stamp di debug
            void on_connection_error(proton::connection & c) override;
            void on_transport_open(proton::transport & t) override;
            void on_connection_close(proton::connection & c) override;
            void on_transport_error(proton::transport & t) override;
            void on_transport_close(proton::transport & t) override;
            void on_receiver_open(proton::receiver & r) override;  
    };

} // end namespace AMQP

#endif