/****************************************
 *  Universita' degli Studi di Udine    *
 *  Ingegneria Elettronica              *
 *  Tesi di Laurea Triennale            *
 *  Giacomo Andrioli - 133047           *
 *  File: driver_client.cpp             *
 ****************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <queue>

#include "ClientAMQP.hpp"
#include "json.h"
#include "Parser.hpp"


void PrintLineMenu();
void StampaMessaggio(const proton::message&);
void StampaDatiMessaggio(const proton::message&);


int main(int argc, char **argv) 
{
    // Percorso del file json di configurazione
    const std::string config_file_path("./config/client-config.json");

    // Stream di lettura file
    std::ifstream ifs;

    // Oggetti per il parsing del file json
    Json::Value root;
    Json::Value node;

    // Parametri da estrarre dal file di configurazione
    std::string ip;
    std::string port;

    std::string command;
    

    try {
        // Apertura file configurazione
        ifs.open(config_file_path);
        if(!ifs.is_open())
            throw std::runtime_error("Unable to open the config file");

        // Parsing del Json da file
        ifs >> root;
        std::cout << "Loading configuration:" << std::endl;

        // Estrazione indirizzo ip
        node = root["ServerIPAddress"];
        if(node == Json::nullValue)
        {
            throw std::logic_error("Required node in config-file: /ServerIPAddress");
        }
        ip = node.asString();
        std::cout << "  - ServerIPAddress: " << ip << std::endl;

        // Estrazione porta
        node = root["ServerPort"];
        if(node == Json::nullValue)
        {
           throw std::logic_error("Required node in config-file: /ServerPort");
        }
        port = node.asString();
        std::cout << "  - ServerPort: " << port << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }

    // Creazione del client
    AMQP::ClientAMQP client(ip + ":" + port, true);
    proton::container container(client);

    // Avvio del thread su cui viene eseguito il client
    // Si deve usare la capture by reference &
    std::thread client_thread([&]() { container.run(); });

    client.OutputLocker.lock();
    std::cout << "Started container thread" << std::endl;
    client.OutputLocker.unlock();

    try
    {
        bool result;
        unsigned int gp_counter;
        unsigned int message_count;
        proton::message p_msg;

        Parser::SensorRequest sensor_request;
        Parser::ActionRequest action_request;
        Parser::Request request;
        Parser::Program* program;
        Parser::StringStringMapVector parameters;

        // Ciclo per la lettura ed esecuzione dei comandi testuali
        do
        {   
            // Stampa il menu dei comandi
            client.OutputLocker.lock();
            PrintLineMenu();
            client.OutputLocker.unlock();

            // Legge il comando
            std::getline(std::cin, command);

            if (command != "exit")
            {
                if (command == "sensor")
                {
                    // Invia messaggio di sensor request al server

                    // Creazione del body della richiesta per i dati del sensore
                    sensor_request.Frequency(1);
                    sensor_request.Samples(10);
                    sensor_request.SensorId(0);
                    
                    // Attach del body ad una richiesta 
                    request.SetSensorRequest(sensor_request);

                    // request.ToString() restituisce una stringa in formato Json.
                    // Il messaggio Json viene messo nella coda di lavoro per venire spedito appena 
                    // possibile dal thread di ClientAMQP
                    client.send(request.ToString());

                    message_count++;

                    client.OutputLocker.lock();
                    std::cout << "La richiesta dei dati del sensore e' stata inviata" << std::endl;
                    std::cout << std::endl << "Premere invio per continuare..." << std::endl << std::endl;
                    client.OutputLocker.unlock();
                    getchar();
                }
                else if (command == "action")
                {
                    // Invia messaggio di action request al server
                    action_request.Subject("led");
                    action_request.Function("fixed-light");
                    action_request.SetControlType(Parser::ControlType::Program);
                    action_request.Expiration(0.0);
                    action_request.Feedback(true);

                    // Costruzione oggetto program
                    program = new Parser::Program(1);
                    parameters.push_back(std::make_pair("state", "true"));
                    program->ParameterList(parameters);

                    action_request.SetProgram(program);

                    // Attach di action_request al messaggio request
                    request.SetActionRequest(action_request);

                    // request.ToString() restituisce una stringa in formato Json.
                    // Il messaggio Json viene messo nella coda di lavoro per venire spedito appena 
                    // possibile dal thread di ServerAMQP
                    client.send(request.ToString());

                    message_count++;

                    // Pulizia oggetto program
                    delete program;
                    program = nullptr;
                    
                    client.OutputLocker.lock();
                    std::cout << "La richiesta dei dati del sensore e' stata inviata" << std::endl;
                    std::cout << std::endl << "Premere invio per continuare..." << std::endl << std::endl;
                    client.OutputLocker.unlock();
                    getchar();  //
                }
                else if (command == "show-data" || command == "show-messages")
                {
                    // Stampa dei messaggi ricevuti

                    // Ciclo per svuotare la coda di messaggi ricevuti
                    gp_counter = 0;
                    do
                    {
                        // Non-blocking thread safe receive
                        result = client.receive(p_msg); 
                        // Se la coda non è vuota
                        if (result)
                        {
                            gp_counter++;

                            // Stampa di messaggi o dati
                            client.OutputLocker.lock();
                            std::cout   << std::endl << "Message #" << gp_counter 
                                        << std::endl; 
                            if (command == "show-data")
                                StampaDatiMessaggio(p_msg);
                            else if (command == "show-messages")
                                StampaMessaggio(p_msg);
                            std::cout   << std::endl
                                        << std::endl;
                            client.OutputLocker.unlock();
                        }
                    }
                    while(result);

                    // Se la coda era vuota
                    if (!gp_counter)
                    {
                        // Stampa messaggio
                        client.OutputLocker.lock();
                        std::cout << std::endl << "Non sono presenti messaggi" << std::endl;
                        client.OutputLocker.unlock();
                    }

                    client.OutputLocker.lock();
                    std::cout << std::endl << "Premere un tasto per continuare..." << std::endl << std::endl;
                    client.OutputLocker.unlock();
                    getchar();
                }
                else
                {
                    // Comando non riconosciuto
                    client.OutputLocker.lock();
                    std::cout << "Comando non riconosciuto: '" << command << "'" << std::endl;
                    std::cout << std::endl << "Premere un tasto per continuare..." << std::endl << std::endl;
                    client.OutputLocker.unlock();
                    getchar();
                }
                
            }
        } 
        while (command != "exit");
        
        client.close();
        client_thread.join();

        return 0;
    }
    catch (const Parser::ParserException& e)
    {
        std::cerr << e << std::endl;
        client.close();
        client_thread.join();
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        client.close();
        client_thread.join();
        return 1;
    }
}

// Stampa i dati associati ad un messaggio
// Non e' thread safe
void StampaDatiMessaggio(const proton::message& message)
{
    Parser::Type type;
    Parser::Response response;
    Parser::SensorResponse sensor_response;
    Parser::ActionResponse action_response;
    Parser::SensorData data;
    std::vector<std::string> vector;
    
    std::string json_message(proton::get<std::string>(message.body()));

    // Controlla che il messaggio sia un tipo response
    type = Parser::Message::GetMessageType(json_message);
    if (type != Parser::Type::Response)
        throw Parser::ParserException("Invalid message type: not a Type::Response", "void StampaDatiSensore(const proton::message& message)");

    // Parsing del messaggio
    response.Parse(json_message);
    
    // In base al tipo di schema del messaggio, stampa il contenuto del body
    switch (response.GetSchema())
    {
        case Parser::Schema::Sensor:
            sensor_response = response.GetSensorResponse();

            std::cout   << std::endl
                        << "Sensor response: " << std::endl                        
                        << "    - Sensor id: " << sensor_response.SensorId() << std::endl
                        << "    - Samples: "  << sensor_response.Samples() << std::endl
                        << "    - Frequency: " << sensor_response.Frequency() << std::endl
                        << "Sensor data: " << std::endl;
            
            data = sensor_response.Data();

            for (unsigned int i = 0; i < data.size(); i++)
            {
                std::cout << "    - " << data.at(i).first << ": " << std::endl;
                
                vector = data.at(i).second;
                for (unsigned int j = 0; j < vector.size(); j++)
                    std::cout << "        -> " << vector.at(j) << std::endl;
            }
            break;


        case Parser::Schema::Action:
            action_response = response.GetActionResponse();

            std::cout   << std::endl
                        << "Action response: " << std::endl                        
                        << "    - Subject: " << action_response.Subject() << std::endl
                        << "    - Function: "  << action_response.Function() << std::endl
                        << "    - Control type: " << action_response.GetControlType() << std::endl
                        << "    - Success: " << action_response.Success() << std::endl
                        << "    - Expired: " << action_response.Expired() << std::endl
                        << "    - Request timestamp: " << action_response.RequestTimestamp() << std::endl;
            break;
        
        default:
            throw Parser::ParserException("Invalid schema", "void StampaDatiSensore(const proton::message& message)");
            break;
    }
}

// Stampa il contenuto di un messaggio JSON
// Non e' thread safe
void StampaMessaggio(const proton::message& message)
{
    std::cout   << std::endl
                << "--------------------------------------------------------" << std::endl
                << proton::get<std::string>(message.body()) << std::endl
                << "--------------------------------------------------------" << std::endl << std::endl;
}


// Stampa il menu dei comandi
// Non e' thread safe
void PrintLineMenu()
{
    std::cout   << std::endl
                << std::endl
                << "MENU" << std::endl << std::endl 
                << "Elenco comandi" << std::endl
                << "    - 'exit': " << std::endl
                << "        termina il programma" << std::endl
                << "    - 'sensor': " << std::endl
                << "        richiede i dati del sensore 'random-numbers'" << std::endl
                << "    - 'action': " << std::endl
                << "        richiede l'azionamento del soggetto 'led'" << std::endl
                << "    - 'show-data': " << std::endl
                << "        mostra i dati ricevuti" << std::endl
                << "    - 'show-messages': " << std::endl
                << "        mostra i messaggi ricevuti" << std::endl
                << std::endl;
}


    

