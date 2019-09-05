/****************************************
 *  Universita' degli Studi di Udine    *
 *  Ingegneria Elettronica              *
 *  Tesi di Laurea Triennale            *
 *  Giacomo Andrioli - 133047           *
 *  File: ServerAMQP.hpp 	            *
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

#ifndef SERVERAMQP_HPP
#define SERVERAMQP_HPP

#include <proton/connection.hpp>
#include <proton/connection_options.hpp>
#include <proton/container.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/message_id.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/link.hpp>
#include <proton/listen_handler.hpp>
#include <proton/listener.hpp>
#include <proton/sender.hpp>
#include <proton/sender_options.hpp>
#include <proton/source_options.hpp>
#include <proton/tracker.hpp>
#include <proton/transport.hpp>
#include <proton/value.hpp>

#include <iostream>
#include <map>
#include <string>
#include <sstream>



namespace AMQP
{

    class ServerAMQP : public proton::messaging_handler 
    {
        private:
            /*                  *
             *      ATTRIBUTI   *
             *                  */

            // Classe per la gestione dell'ascolto delle connessioni in entrata
            class listener_ready_handler : public proton::listen_handler 
            {
                public:
                    // Abilita il debug a linea di comando
                    bool verbose;

                    // Costruttore
                    listener_ready_handler(bool verbose = false);
                    
                    // Eventi in overriding
                    void on_open(proton::listener& l) override;
                    void on_error(proton::listener& l, const std::string& error_string) override;
            };

            // Mappa per associare un sender all'indirizzo di destinazione
            std::map<std::string, proton::sender> senders;

            // Counter per la generazione di indirizzi univoci per rispondere ai client
            long int address_counter;    

            // Oggetti per la gestione dell'ascolto delle connessioni in entrata
            proton::listener listener;
            listener_ready_handler listen_handler;
            
            // URL del server per l'ascolto delle richieste di connessione
            std::string url;

            // Abilita il debug a linea di comando
            bool _VERBOSE;
            
            // Puntatore a una funzione che verra' chiamata dal server
            // per ottenere il body del messaggio di risposta. La funzione riceve come parametro il 
            // messaggio di richiesta
            std::string (*response_function)(const proton::message&);


            
        public:
            /*                  *
             *      METODI      *
             *                  */

            // Costruttore
            ServerAMQP(const std::string& server_address, bool verbose = false);

            // Funzione per la generazione di un indirizzo univoco 
            std::string generate_address();
            
            // Funzione per il debug a linea di comando
            void Debug(std::string message);

            // Set del puntatore a funzione response_function
            void SetMessageHandler(std::string(*handler)(const proton::message&));

            // Funzioni evento in overriding
            void on_message(proton::delivery &d, proton::message &msg) override;
            void on_container_start(proton::container &c) override;
            void on_sender_open(proton::sender &sender) override;
            void on_connection_open(proton::connection & c) override;
            void on_connection_close(proton::connection & c) override;
            
            // Stampa errori a linea di comando nel caso si abbia verbose = true
            void on_connection_error(proton::connection & c) override;
            void on_transport_open(proton::transport & t) override;
            void on_transport_error(proton::transport & t) override;
            void on_transport_close(proton::transport &) override;
            void on_tracker_accept(proton::tracker &t) override;
    };
}


#endif