/****************************************
 *  Universita' degli Studi di Udine    *
 *  Ingegneria Elettronica              *
 *  Tesi di Laurea Triennale            *
 *  Giacomo Andrioli - 133047           *
 *  File: parser-request.cpp            *
 ****************************************/

#include "Parser.hpp"



/* 
 * CLASS SensorRequest
 */

/*
 *  Descrizione: operatore di output
 */
std::ostream& operator<<(std::ostream& os, const Parser::Request& req)
{
    return os << req.ToString() << std::endl;
}

/*
 *  Costruttore
 */
Parser::SensorRequest::SensorRequest()
{
    sensor_id = 0;
    samples = 0;
    frequency = 1;
}

/*
 *  Costruttore: utilizza l'oggetto json 'body' per compilare gli attributi dell'oggetto
 */
Parser::SensorRequest::SensorRequest(const Json::Value& body)
{
    Parse(body);
}

/*
 *  Costruttore: converte la stringa 'body' in un oggetto json e lo utilizza per compilare 
 *      gli attributi dell'oggetto
 */
Parser::SensorRequest::SensorRequest(const std::string& body)
{
    Json::Value node;
    Json::CharReaderBuilder builder;
    std::stringstream stream(body);
    std::string errors;

    bool success = Json::parseFromStream(builder, stream, &node, &errors);
    if(!success)
    {
        throw ParserException("Invalid raw body JSON string", "Parser::SensorRequest::SensorRequest(std::string body)");
    }

    Parse(node);
}

/*
 *  Descrizione: selettore di lettura dell'attributo sensor_id
 */
unsigned int Parser::SensorRequest::SensorId()
{
    return sensor_id;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo sensor_id
 */
void Parser::SensorRequest::SensorId(unsigned int id)
{
    sensor_id = id;
}

/*
 *  Descrizione: selettore di lettura dell'attributo samples
 */
unsigned int Parser::SensorRequest::Samples()
{
    return samples;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo samples
 */
void Parser::SensorRequest::Samples(unsigned int s)
{
    if (s == 0)
        throw ParserException("Invalid zero samples", "void Parser::SensorRequest::Samples(unsigned int s)");
    samples = s;
}

/*
 *  Descrizione: selettore di lettura dell'attributo frequency
 */
float Parser::SensorRequest::Frequency()
{
    return frequency;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo frequency
 */
void Parser::SensorRequest::Frequency(float freq)
{
    if (freq < 0)
        throw Parser::ParserException("Invalid negative frequency", "void Parser::SensorRequest::Frequency(float freq)");
    frequency = freq;
}

/*
 *  Descrizione: la funzione estra i dati dall'oggetto json per compilare gli attributi dell'oggetto
 */
void Parser::SensorRequest::Parse(const Json::Value& body)
{
    Json::Value info;
    Json::Value node;

    info = body["info"];
    if (info == Json::nullValue)
        throw Parser::ParserException("Required info key", "Parser::SensorRequest::SensorRequest(Json::Value body)");

    node = info["sensor-id"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: sensor-id", "Parser::SensorRequest::SensorRequest(Json::Value body)");
    if (!node.isUInt())
        throw Parser::ParserException("body/sensor-id: invalid type", "Parser::SensorRequest::SensorRequest(Json::Value body)");
    sensor_id = node.asUInt();

    node = info["samples"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: samples", "Parser::SensorRequest::SensorRequest(Json::Value body)");
    if (!node.isUInt())
        throw Parser::ParserException("body/info/samples: invalid type", "Parser::SensorRequest::SensorRequest(Json::Value body)");
    samples = node.asUInt();

    node = info["frequency"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: frequency", "Parser::SensorRequest::SensorRequest(Json::Value body)");
    if (!node.isDouble())
        throw Parser::ParserException("body/info/frequency: invalid type", "Parser::SensorRequest::SensorRequest(Json::Value body)");
    frequency = node.asFloat();
}

/*
 *  Descrizione: converte i dati degli attributi dell'oggetto in un oggetto json, secondo il protocollo
 */
Json::Value Parser::SensorRequest::ToJsonValue() const
{
    Json::Value root;

    root["info"]["sensor-id"] = sensor_id;
    root["info"]["samples"] = samples;
    root["info"]["frequency"] = frequency;

    return root;
}











/* 
 * CLASS ActionRequest
 */


/*
 *  Costruttore
 */
Parser::ActionRequest::ActionRequest()
    : subject(""), function(""), control_type(ControlType::Undefined)
{
    signal = nullptr;
    program = nullptr;
}

/*
 *  Costruttore: utilizza l'oggetto json 'body' per compilare gli attributi dell'oggetto
 */
Parser::ActionRequest::ActionRequest(const Json::Value& body)
    : subject(""), function(""), control_type(ControlType::Undefined)
{
    Parse(body);
}

/*
 *  Costruttore: converte la stringa 'body' in un oggetto json e lo utilizza per compilare 
 *      gli attributi dell'oggetto
 */
Parser::ActionRequest::ActionRequest(const std::string& body)
{
    Json::Value node;
    Json::CharReaderBuilder builder;
    std::stringstream stream(body);
    std::string errors;

    bool success = Json::parseFromStream(builder, stream, &node, &errors);
    if(!success)
    {
        throw ParserException("Invalid raw body JSON string", "Parser::ActionRequest::ActionRequest(std::string body)");
    }

    Parse(node);
}

/*
 *  Descrizione: selettore di lettura dell'attributo subject
 */
std::string Parser::ActionRequest::Subject() const
{
    return subject;
}

/*
 *  Descrizione: selettore di lettura dell'attributo function
 */
std::string Parser::ActionRequest::Function() const
{
    return function;
}

/*
 *  Descrizione: selettore di lettura dell'attributo control_type
 */
Parser::ControlType Parser::ActionRequest::GetControlType() const
{
    return control_type;
}

/*
 *  Descrizione: selettore di lettura dell'attributo signal
 */
Parser::Signal* Parser::ActionRequest::GetSignal()
{
    return signal;
}

/*
 *  Descrizione: selettore di lettura dell'attributo program
 */
Parser::Program* Parser::ActionRequest::GetProgram()
{
    return program;
}

/*
 *  Descrizione: selettore di lettura dell'attributo expiration
 */
float Parser::ActionRequest::Expiration() const
{
    return expiration;
}

/*
 *  Descrizione: selettore di lettura dell'attributo feedback
 */
bool Parser::ActionRequest::Feedback() const
{
    return feedback;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo subject
 */
void Parser::ActionRequest::Subject(const std::string& sub)
{
    if(sub == "")
        throw ParserException("Invalid empty subject string", "void Parser::ActionRequest::Subject(const std::string&)");
    subject = sub;
}


/*
 *  Descrizione: selettore di scrittura dell'attributo function
 */
void Parser::ActionRequest::Function(const std::string& func)
{
    if(func == "")
        throw ParserException("Invalid empty function string", "void Parser::ActionRequest::Function(const std::string&)");
    function = func;
}


/*
 *  Descrizione: selettore di scrittura dell'attributo control_type
 */
void Parser::ActionRequest::SetControlType(const ControlType& ct)
{
    if(ct == ControlType::Undefined)
        throw ParserException("Invalid undefined control type", "void Parser::ActionRequest::SetControlType(const ControlType&)");
    control_type = ct;
}


/*
 *  Descrizione: selettore di scrittura dell'attributo signal
 */
void Parser::ActionRequest::SetSignal(Signal* sig)
{
    if(sig == nullptr)
        throw ParserException("Invalid signal nullptr", "void Parser::ActionRequest::SetSignal(Signal*)");
    
    signal = sig;

    delete program;
    program = nullptr;
}


/*
 *  Descrizione: selettore di scrittura dell'attributo program
 */
void Parser::ActionRequest::SetProgram(Program* prog) 
{
    if(prog == nullptr)
        throw ParserException("Invalid signal nullptr", "void Parser::ActionRequest::SetProgram(Program*)");
    
    program = prog;

    delete signal;
    signal = nullptr;
}


/*
 *  Descrizione: selettore di scrittura dell'attributo expiration
 */
void Parser::ActionRequest::Expiration(float exp)
{
    if (exp < 0)
        throw ParserException("Invalid parameter: exp < 0", "void Parser::ActionRequest::Expiration(float exp)");
    expiration = exp;
}


/*
 *  Descrizione: selettore di scrittura dell'attributo feedback
 */
void Parser::ActionRequest::Feedback(bool feedback)
{
    this->feedback = feedback;
}

/*
 *  Descrizione: la funzione estra i dati dall'oggetto json per compilare gli attributi dell'oggetto
 */
void Parser::ActionRequest::Parse(const Json::Value& body)
{
    Json::Value info;
    Json::Value node;
    Json::Value security;


    // Required nodes
    info = body["info"];
    if (info == Json::nullValue)
        throw Parser::ParserException("Required info key: sensor-id", "Parser::SensorRequest::SensorRequest(Json::Value body)");
    
    node = info["subject"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: function", "void Parser::ActionRequest::Parse(Json::Value body)");
    subject = node.asString();

    node = info["function"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: samples", "void Parser::ActionRequest::Parse(Json::Value body)");
    function = node.asString();

    node = info["control-type"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: control-type", "void Parser::ActionRequest::Parse(Json::Value body)");
    control_type = StringToControlType(node.asString());

    node = body["data"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required body key: data", "void Parser::ActionRequest::Parse(Json::Value body)");
    
    switch(control_type)
    {
        case ControlType::Undefined:
            throw Parser::ParserException("Invalid control-type: undefined", "void Parser::ActionRequest::Parse(Json::Value body)");
            break;
        
        case ControlType::Signal:
            signal = new Signal(node);
            break;
        
        case ControlType::Program:
            program = new Program(node);
            break;
        
        default:
            throw Parser::ParserException("Unimplemented control-type case", "void Parser::ActionRequest::Parse(Json::Value body)");
    }

    security = node["security"];
    if (security == Json::nullValue)
        throw Parser::ParserException("Required body key: security", "void Parser::ActionRequest::Parse(Json::Value body)");
    
    node = security["expiration"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required body/data/security key: expiration", "void Parser::ActionRequest::Parse(Json::Value body)");
    if (!node.isNumeric())
        throw Parser::ParserException("Invalid body/data/security value: expiration not numeric", "void Parser::ActionRequest::Parse(Json::Value body)");
    expiration = node.asFloat();

    node = security["feedback"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required body/data/security key: feedback", "void Parser::ActionRequest::Parse(Json::Value body)");
    if (!node.isBool())
        throw Parser::ParserException("Invalid body/data/security value: feedback not bool", "void Parser::ActionRequest::Parse(Json::Value body)");
    expiration = node.asBool();
}

/*
 *  Descrizione: converte i dati degli attributi dell'oggetto in un oggetto json, secondo il protocollo
 */
Json::Value Parser::ActionRequest::ToJsonValue() const
{
    Parser::StringStringMapVector list;
    Json::Value root;

    root["info"]["subject"] = subject;
    root["info"]["function"] = function;
    root["info"]["control-type"] = ControlTypeToString(control_type);

    root["data"]["security"]["expiration"] = expiration;
    root["data"]["security"]["feedback"] = feedback;
    
    switch (control_type)
    {
        case ControlType::Program:
            root["data"]["program-id"] = program->ProgramId();

            list = program->ParameterList();

            for (unsigned int i = 0; i < list.size(); i++)
                root["data"]["parameters"][list.at(i).first] = list.at(i).second;

            break;

        case ControlType::Signal:
            root["data"]["signal-type"] = SignalTypeToString(signal->GetSignalType());
            root["data"]["peak"] = signal->Peak();
            root["data"]["timing"]["delay"] = signal->Delay();
            root["data"]["timing"]["duration"] = signal->Duration();
            root["data"]["offset"] = signal->Offset();
            
            break;

        default:
            throw ParserException("Unimplemented control-type", "Json::Value Parser::ActionResponse::ToJsonValue() const");
    }

    return root;
}





/* 
 * CLASS CatalogueRequest
 */
// UNIMPLEMENTED






/* 
 * CLASS Request
 */

/*
 *  Operatore di copia
 *  Descrizione: effettua la copia profonda dei dati
 */
Parser::Request& Parser::Request::operator= (const Parser::Request& req)
{
    // Pulizia oggetti request
    CleanParsing();

    // Attributi derivati dalla classe Message
    version = req.version;
    schema = req.schema;
    type = req.type;
    timestamp = req.timestamp;
    body = req.body;

    // Copia profonda
    if (req.sensor != nullptr)
    {
        sensor = new Parser::SensorRequest(*req.sensor);
    }
    if (req.action != nullptr)
    {
        action = new Parser::ActionRequest(*req.action);
    }
    if (req.catalogue != nullptr)
    {
        catalogue = new Parser::CatalogueRequest(*req.catalogue);
    }

    return *this;
}

/*
 *  Costruttore
 */
Parser::Request::Request()
    : Message()
{
    version = "1.0";
    type = Parser::Type::Request;
    timestamp = TimestampString();

    sensor = nullptr;    
    action = nullptr;    
    catalogue = nullptr;
}

/*
 *  Costruttore: la stringa 'message' deve contenere il messaggio json che contiene i dati per compilare i dati
 *      dell'oggetto
 */
Parser::Request::Request(const std::string& message)
    : Message(message)
{
    if (type != Type::Request)
        throw Parser::ParserException("Invalid message type", "Parser::Request::Request(std::string message)");

    switch(schema)
    {
        case Parser::Schema::Action:
        case Parser::Schema::Sensor:
        case Parser::Schema::Catalogue:
            ParseFromBody();
            break;
        case Parser::Schema::Error:
            throw Parser::ParserException("Invalid request schema: error", "Parser::Request::Request(std::string message)");
            break;
        case Parser::Schema::Undefined:
            throw Parser::ParserException("Invalid request schema: undefined", "Parser::Request::Request(std::string message)");
            break;
        default:
            throw Parser::ParserException("Unimplemented schema", "Parser::Request::Request(std::string message)");
            break;
    }
}

/*
 *  Costruttore di copia: effettua la copia prodonda dell'oggetto sorgente
 */
Parser::Request::Request(const Parser::Request& req)
{
    // Attributi derivati dalla classe Message
    version = req.version;
    schema = req.schema;
    type = req.type;
    timestamp = req.timestamp;
    body = req.body;

    // Copia profonda
    if (req.sensor != nullptr)
    {
        sensor = new Parser::SensorRequest(*req.sensor);
    }
    if (req.action != nullptr)
    {
        action = new Parser::ActionRequest(*req.action);
    }
    if (req.catalogue != nullptr)
    {
        catalogue = new Parser::CatalogueRequest(*req.catalogue);
    }
}


/*
 *  Distruttore
 */
Parser::Request::~Request()
{
    delete sensor;    
    delete action;    
    delete catalogue;    
}


/*
 *  Descrizione: selettore di lettura dell'attributo sensor
 */
Parser::SensorRequest& Parser::Request::GetSensorRequest()
{
    return *sensor;
}

/*
 *  Descrizione: selettore di lettura dell'attributo action
 */
Parser::ActionRequest& Parser::Request::GetActionRequest()
{
    return *action;
}

/*
 *  Descrizione: selettore di lettura dell'attributo catalogue
 */
Parser::CatalogueRequest& Parser::Request::GetCatalogueRequest()
{
    return *catalogue;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo sensor
 */
void Parser::Request::SetSensorRequest(const SensorRequest& request)
{
    if (IsBodyParsed())
        CleanParsing();     // Elimina dati precedenti
    
    schema = Schema::Sensor;
    body = SensorRequest(request).ToJsonValue();   // Setting dell'attributo body
    ParseFromBody();    // Parsing del body per compilare sensor con i dati
}

/*
 *  Descrizione: selettore di scrittura dell'attributo action
 */
void Parser::Request::SetActionRequest(const ActionRequest& request)
{
    if (IsBodyParsed())
        CleanParsing();     // Elimina dati precedenti
    
    schema = Schema::Action;    
    body = ActionRequest(request).ToJsonValue();   // Setting dell'attributo body
    ParseFromBody();    // Parsing del body per compilare action con i dati
}

/*
 *  Descrizione: selettore di scrittura dell'attributo catalogue
 */
void Parser::Request::SetCatalogueRequest(const CatalogueRequest& request)
{
    // UNIMPLEMENTED
    throw ParserException("Unimplemented", "void Parser::Request::SetCatalogueRequest(const CatalogueRequest& cat)");
}

/*
 *  Descrizione: selettore di scrittura dell'attributo type
 *      Overloading del metodo della classe Message per evitare errori
 */
void Parser::Request::SetType(const Type& type)
{
    if (type != Type::Request)
        throw ParserException("Invalid type specified for a Response", "void Parser::Response::SetType(const Type& type)");
}

/*
 *  Descrizione: indica se è stato effettuato il parsing dei dati del body. Se true, è puntato
 *      un oggetto SensorRequest, ActionRequest o CatalogueRequest 
 */
bool Parser::Request::IsBodyParsed()
{
    return sensor != nullptr || action != nullptr || catalogue != nullptr;
}


/*
 *  Descrizione: compila gli attributi dell'oggetto eseguendo il parsing dei dati contenuti nell'attributo body
 */
void Parser::Request::ParseFromBody()
{
    if(type != Type::Request)
        throw Parser::ParserException("Invalid message type" + TypeToString(type), "Parser::Request::ParseBody(std::string message)");
    
    if (IsBodyParsed())
        CleanParsing();     // Elimina eventuali dati precedenti

    // In base allo schema del messaggio, costruisce uno degli attributi base
    // con il costruttore della relativa classe
    switch(schema)
    {
        case Parser::Schema::Sensor:
            sensor = new Parser::SensorRequest(body);
            break;
        
        case Parser::Schema::Action:
            action = new ActionRequest(body);
            break;
        
        case Parser::Schema::Catalogue:
            // UNIMPLEMENTED
            throw Parser::ParserException("Unimplemented schema: catalogue", "Parser::Request::ParseBody(std::string message)");
            break;
        
        case Parser::Schema::Error:
            throw Parser::ParserException("Invalid schema: error", "Parser::Request::ParseBody(std::string message)");
            break;
        
        case Parser::Schema::Undefined:
            throw Parser::ParserException("Unimplemented schema: undefined", "Parser::Request::ParseBody(std::string message)");
            break;
        
        default:
            throw Parser::ParserException("Unimplemented schema", "Parser::Request::ParseBody(std::string message)");
            break;
    }
}

/*
 *  Descrizione: compila gli attributi dell'oggetto in base alla stringa 'message',
 *      Questa deve contenere il messaggio in formato json secondo lo standard
 */
void Parser::Request::Parse(const std::string& message)
{
    if(IsBodyParsed())
        CleanParsing();     // Eventuale pulizia di vecchi dati
    
    ParseHeader(message);   // Metodo ereditato da Message per compilare gli attributi ereditati.
    
    // Compila gli attributi base in base all'oggetto json dell'attributo body. L'attributo body è
    // ereditato da Message, quindi è compilato da ParseHeader(message)
    ParseFromBody();    
}

/*
 *  Descrizione: La funzione elimina i dati dinamici puntati dagli attributi base: sensor, action, 
 *      catalogue
 */
void Parser::Request::CleanParsing()
{
    if (sensor != nullptr)
    {
        delete sensor;
        sensor = nullptr;
    }
    if (action != nullptr)
    {
        delete action;
        action = nullptr;
    }
    if (catalogue != nullptr)
    {
        delete catalogue;
        catalogue = nullptr;
    }
}


