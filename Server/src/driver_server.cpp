/****************************************
 *  Universita' degli Studi di Udine    *
 *  Ingegneria Elettronica              *
 *  Tesi di Laurea Triennale            *
 *  Giacomo Andrioli - 133047           *
 *  File: driver_server.hpp             *
 ****************************************/

#include <iostream>
#include <fstream>
#include <string>

#include "json.h"
#include "ServerAMQP.hpp"
#include "Parser.hpp"


// Funzione per la gestione dei responsi
std::string message_handler(const proton::message& message);
void LedControl(bool on);
std::vector<std::string> GetSensorData(unsigned int samples, float frequency);
std::string GetSensorName();



int main(int argc, char **argv) 
{
    // Percorso del file json di configurazione
    const std::string config_file_path("./config/server-config.json");

    // Stream di lettura file
    std::ifstream ifs;

    // Oggetti per il parsing del file json
    Json::Value root;
    Json::Value node;

    // Parametri da estrarre dal file di configurazione
    std::string ip;
    std::string port;
    
    // Apertura file configurazione
    ifs.open(config_file_path);

    if (ifs.is_open())
    {
        // Caricamento della configurazione predefinita
        try 
        {
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
        catch (std::exception & e)
        {
            std::cout << e.what()  << std::endl;
            return 1;
        }
    }
    else
    {
        std::cout << "Unable to open configuration file" << std::endl;
        return 1;
    }

    // Avvio del server
    try 
    {
        AMQP::ServerAMQP server(ip + ":" + port, true);

        // Impostazione con puntatore della funzione che verrà chiamata dal server 
        //  per ottenere il responso da mandare ai client
        server.SetMessageHandler(message_handler);
        
        // Avvia il server
        proton::container(server).run();

        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}


/*
 * Descrizione: La funzione viene chiamata dal server per ottenere il body del messaggio da inviare
 * risposta ad un client.
 * Parametri:   - request: è il messaggio arrivato dal client
 * Return: stringa con il body della risposta
 */

std::string message_handler(const proton::message& request_msg)
{
    std::string message(proton::get<std::string>(request_msg.body()));
    Parser::Response response;
    Parser::Request request;
    Parser::Type type;

    Parser::SensorRequest sensor_request;
    Parser::SensorResponse sensor_response;

    Parser::ActionRequest action_request;
    Parser::ActionResponse action_response;
    
    Parser::Program program;
    Parser::StringStringMapVector parameters;
    std::vector<std::pair<std::string,std::vector<std::string>>> data_vec;
    Parser::ErrorResponse error;

    try 
    {
        // Controllo del tipo di messaggio
        type = Parser::Message::GetMessageType(message);

        switch (type)
        {
            case Parser::Type::Request:
                request.Parse(message);

                switch (request.GetSchema())
                {
                    case Parser::Schema::Sensor:
                        sensor_request = request.GetSensorRequest();

                        // Controllo richiesta
                        if (sensor_request.SensorId() != 0)
                            throw Parser::ParserException("Invalid sensor-id", "std::string message_handler(const proton::message&), case Parser::Schema::Sensor");

                        sensor_response.Samples(sensor_request.Samples());    
                        sensor_response.Frequency(sensor_request.Frequency());                   
                        sensor_response.SensorId(sensor_request.SensorId());
                        
                        data_vec.push_back(make_pair(GetSensorName(), GetSensorData(sensor_request.Samples(), sensor_request.Frequency())));

                        sensor_response.Data(data_vec);

                        response.SetSensorResponse(sensor_response);
                        break;





                    case Parser::Schema::Action:
                        action_request = request.GetActionRequest();
                        
                        // Controllo richiesta
                        if (action_request.Subject() != "led")
                            throw Parser::ParserException("Invalid subject", "std::string message_handler(const proton::message&), case Parser::Schema::Sensor");
                        if (action_request.Function() != "fixed-light")
                            throw Parser::ParserException("Invalid function", "std::string message_handler(const proton::message&), case Parser::Schema::Sensor");
                        if (action_request.GetControlType() != Parser::ControlType::Program)
                            throw Parser::ParserException("Invalid control-type", "std::string message_handler(const proton::message&), case Parser::Schema::Action");

                        program = *action_request.GetProgram();

                        if (program.ProgramId() != 1)
                            throw Parser::ParserException("Invalid control-type", "std::string message_handler(const proton::message&), case Parser::Schema::Action");

                        parameters = program.ParameterList();
                        if (parameters.size() != 1 || parameters.at(0).first != "state")
                             throw Parser::ParserException("Invalid invalid program", "std::string message_handler(const proton::message&), case Parser::Schema::Action");

                        // Azionamento
                        if (parameters.at(0).second == "true")
                            LedControl(true);
                        else if (parameters.at(0).first == "false")
                            LedControl(false);
                        else
                             throw Parser::ParserException("Invalid parameter 'state' value", "std::string message_handler(const proton::message&), case Parser::Schema::Action");
                        
                        // Dati del responso
                        action_response.Subject(action_request.Subject());
                        action_response.Function(action_request.Function());
                        action_response.SetControlType(action_request.GetControlType());
                        action_response.Expired(false);                        
                        action_response.Success(true);
                        action_response.RequestTimestamp(request.Timestamp());                        

                        // Setting del body
                        response.SetActionResponse(action_response);
                        break;
                        
                    default:
                        break;
                }
                break;
            
            case Parser::Type::Undefined:
            default:
                throw Parser::ParserException("The message is not a request", "std::string message_handler(const proton::message& request)");
        }
    }
    catch(const Parser::ParserException& e)
    {
        std::cerr << "ECCEZIONE" << e << std::endl;
        error.Code("errorxyz");
        error.ErrorType("parsing");
        error.What(e.what());
        error.At(e.at());
        Parser::Response eresponse;
        eresponse.SetErrorResponse(error);

        return eresponse.ToString();
    }

    return response.ToString();
}


std::vector<std::string> GetSensorData(unsigned int samples, float frequency)
{
    std::vector<std::string> data;

    for (unsigned int i = 0; i < samples; i++)
        data.push_back(std::to_string(rand() % 100 + 1));
    
    return data;
}

std::string GetSensorName()
{
    return "random-numbers";
}

void LedControl(bool on)
{
    std::string state = on ? "ON" : "OFF";
    std::cerr << "---------- LED: " << state << " ----------" << std::endl;
}

