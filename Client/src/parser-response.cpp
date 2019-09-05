/****************************************
 *  Universita' degli Studi di Udine    *
 *  Ingegneria Elettronica              *
 *  Tesi di Laurea Triennale            *
 *  Giacomo Andrioli - 133047           *
 *  File: parser-response.cpp           *
 ****************************************/

#include "Parser.hpp"


/*
 * CLASS ErrorResponse
 */

/*
 *  Costruttore
 */
Parser::ErrorResponse::ErrorResponse()
    : code(""), error_type(""), at(""), what("")
{}


/*
 *  Costruttore: utilizza l'oggetto json 'body' per compilare gli attributi dell'oggetto
 */
Parser::ErrorResponse::ErrorResponse(const Json::Value& body)
{
    Parse(body);
}

/*
 *  Costruttore: converte la stringa 'body' in un oggetto json e lo utilizza per compilare 
 *      gli attributi dell'oggetto
 */
Parser::ErrorResponse::ErrorResponse(const std::string& body)
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
 *  Descrizione: selettore di lettura dell'attributo code
 */
std::string Parser::ErrorResponse::Code() const
{
    return code;
}

/*
 *  Descrizione: selettore di lettura dell'attributo error_type
 */
std::string Parser::ErrorResponse::ErrorType() const
{
    return error_type;
}

/*
 *  Descrizione: selettore di lettura dell'attributo at
 */
std::string Parser::ErrorResponse::At() const
{
    return at;
}

/*
 *  Descrizione: selettore di lettura dell'attributo what
 */
std::string Parser::ErrorResponse::What() const
{
    return what;
}

/*
 *  Descrizione: selettore di lettura dell'attributo code
 */
void Parser::ErrorResponse::Code(const std::string& code)
{
    this->code = code;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo type
 */
void Parser::ErrorResponse::ErrorType(const std::string& type)
{
    error_type = type;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo at
 */
void Parser::ErrorResponse::At(const std::string& where)
{
    at = where;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo what
 */
void Parser::ErrorResponse::What(const std::string& what)
{
    this->what = what;
}

/*
 *  Descrizione: la funzione estra i dati dall'oggetto json per compilare gli attributi dell'oggetto
 */
void Parser::ErrorResponse::Parse(const Json::Value& body)
{
    Json::Value info;
    Json::Value node;

    info = body["info"];
    if (info == Json::nullValue)
        throw Parser::ParserException("Required info key", "void Parser::ErrorResponse::Parse(const Json::Value& body)");
    
    node = info["code"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: code", "void Parser::ErrorResponse::Parse(const Json::Value& body)");
    code = node.asString();

    node = info["error-type"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: error-type", "void Parser::ErrorResponse::Parse(const Json::Value& body)");
    error_type = node.asString();

    node = info["at"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: at", "void Parser::ErrorResponse::Parse(const Json::Value& body)");
    at = node.asString();

    node = info["what"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: what", "void Parser::ErrorResponse::Parse(const Json::Value& body)");
    what = node.asString();
}

/*
 *  Descrizione: converte i dati degli attributi dell'oggetto in un oggetto json, secondo il protocollo
 */
Json::Value Parser::ErrorResponse::ToJsonValue() const
{
    Json::Value root;

    root["info"]["code"] = code;    
    root["info"]["error-type"] = error_type;    
    root["info"]["at"] = at;    
    root["info"]["what"] = what;

    return root;
}









/* 
 * CLASS SensorResponse
 */

/*
 *  Costruttore
 */
Parser::SensorResponse::SensorResponse()
    : data()
{
    sensor_id = 0;
    samples = 0;
    frequency = 0;
}

/*
 *  Costruttore: l'argomento sensor_id ha valore di default 0U
 */
Parser::SensorResponse::SensorResponse(unsigned int sensor_id)
    : data()
{
    this->sensor_id = sensor_id;
    samples = 0;
    frequency = 0;
}

/*
 *  Costruttore: utilizza l'oggetto json 'body' per compilare gli attributi dell'oggetto
 */
Parser::SensorResponse::SensorResponse(const Json::Value& body)
{
    Parse(body);
}

/*
 *  Descrizione: selettore di lettura dell'attributo sensor_id
 */
unsigned int Parser::SensorResponse::SensorId() const
{
    return sensor_id;
}   

/*
 *  Descrizione: selettore di lettura dell'attributo samples
 */
unsigned int Parser::SensorResponse::Samples() const
{
    return samples;
}          

/*
 *  Descrizione: selettore di lettura dell'attributo frequency
 */
float Parser::SensorResponse::Frequency() const
{
    return frequency;
}

/*
 *  Descrizione: selettore di lettura dell'attributo data
 */
Parser::SensorData Parser::SensorResponse::Data() const
{
    return data;
}


/*
 *  Descrizione: selettore di scrittura dell'attributo sensor_id
 */
void Parser::SensorResponse::SensorId(unsigned int sensor_id)
{
    this->sensor_id = sensor_id;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo samples
 */
void Parser::SensorResponse::Samples(unsigned int samples)
{
    if (samples == 0)
        throw ParserException("Invalid zero samples", "void Parser::SensorResponse::Samples(unsigned int sample)");
    this->samples = samples;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo frequency
 */
void Parser::SensorResponse::Frequency(float frequency)
{
    if (frequency <= 0)
        throw ParserException("Invalid non positive frequency", "void Parser::SensorResponse::Frequency(float)");
    this->frequency = frequency;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo data
 */
void Parser::SensorResponse::Data(const SensorData& data)
{
    if (data.size() == 0)
        throw ParserException("Invalid empty data set", "void Parser::SensorResponse::Data(const std::vector<std::pair<std::string,std::vector<std::string>>>& data)");
    this->data = data;
}

/*
 *  Descrizione: la funzione estra i dati dall'oggetto json per compilare gli attributi dell'oggetto
 */
void Parser::SensorResponse::Parse(const Json::Value& body)
{
    Json::Value info;
    Json::Value node;
    Json::Value data;

    info = body["info"];
    if (info == Json::nullValue)
        throw Parser::ParserException("Required info key", "void Parser::SensorResponse::Parse(const Json::Value& body)");

    node = info["sensor-id"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: sensor-id", "void Parser::SensorResponse::Parse(const Json::Value& body)");
    if (!node.isUInt())
        throw Parser::ParserException("body/sensor-id: invalid type", "void Parser::SensorResponse::Parse(const Json::Value& body)");
    sensor_id = node.asUInt();

    node = info["samples"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: samples", "void Parser::SensorResponse::Parse(const Json::Value& body)");
    if (!node.isUInt())
        throw Parser::ParserException("body/info/samples: invalid type", "void Parser::SensorResponse::Parse(const Json::Value& body)");
    samples = node.asUInt();

    node = info["frequency"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: frequency", "void Parser::SensorResponse::Parse(const Json::Value& body)");
    if (!node.isDouble())
        throw Parser::ParserException("body/info/frequency: invalid type", "void Parser::SensorResponse::Parse(const Json::Value& body)");
    frequency = node.asFloat();


    data = body["data"];
    if (data == Json::nullValue)
        throw Parser::ParserException("Required data key", "void Parser::SensorResponse::Parse(const Json::Value& body)");

    Json::Value::Members mems(data.getMemberNames());
    std::vector<std::string> vec;
    std::string name;
    for (unsigned int i = 0; i < mems.size(); i++)
    {
        name = mems.at(i);
        node = data[name];

        if (node.size() != samples)
            throw Parser::ParserException("body/data/" + name + ": invalid number of samples", "void Parser::SensorResponse::Parse(const Json::Value& body)");
        if (!node.isArray())
            throw Parser::ParserException("body/data/" + name + "is not an array", "void Parser::SensorResponse::Parse(const Json::Value& body)");
        
        for (unsigned int k = 0; k < node.size(); k++)
            vec.push_back(node[k].asString());

        this->data.push_back(std::make_pair(name, vec));
        vec.clear();
    }
}


/*
 *  Descrizione: converte i dati degli attributi dell'oggetto in un oggetto json, secondo il protocollo
 */
Json::Value Parser::SensorResponse::ToJsonValue() const
{
    Json::Value root;
    Json::Value obj;

    std::string name;
    std::vector<std::string> vec;

    root["info"]["sensor-id"] = sensor_id;
    root["info"]["samples"] = samples;
    root["info"]["frequency"] = frequency;
    
    for (unsigned int i = 0; i < data.size(); i++)
    {
        name = data.at(i).first;
        vec = data.at(i).second;
        for (unsigned int j = 0; j < vec.size(); j++)
        {
            obj[name][j] = vec.at(j);
        }
        root["data"] = obj;
    }

    return root;
}









/* 
 * CLASS ActionResponse
 */

/*
 *  Costruttore
 */
Parser::ActionResponse::ActionResponse()
    : subject(""), function(""), control_type(ControlType::Undefined), req_timestamp("")
{
    success = false;
    expired = false;
}

/*
 *  Costruttore: utilizza l'oggetto json 'body' per compilare gli attributi dell'oggetto
 */
Parser::ActionResponse::ActionResponse(const Json::Value& body)
{
    Parse(body);
}

/*
 *  Costruttore: converte la stringa 'body' in un oggetto json e lo utilizza per compilare 
 *      gli attributi dell'oggetto
 */
Parser::ActionResponse::ActionResponse(const std::string& body)
{
    Json::Value node;
    Json::CharReaderBuilder builder;
    std::stringstream stream(body);
    std::string errors;

    bool success = Json::parseFromStream(builder, stream, &node, &errors);
    if(!success)
    {
        throw ParserException("Invalid raw body JSON string", "Parser::ActionResponse::ActionResponse(const std::string& body)");
    }

    Parse(node);
}

/*
 *  Descrizione: selettore di lettura dell'attributo subject
 */
std::string Parser::ActionResponse::Subject() const
{
    return subject;
}

/*
 *  Descrizione: selettore di lettura dell'attributo function
 */
std::string Parser::ActionResponse::Function() const
{
    return function;
}

/*
 *  Descrizione: selettore di lettura dell'attributo control_type
 */
Parser::ControlType Parser::ActionResponse::GetControlType() const
{
    return control_type;
}

/*
 *  Descrizione: selettore di lettura dell'attributo success
 */
bool Parser::ActionResponse::Success() const
{
    return success;
}

/*
 *  Descrizione: selettore di lettura dell'attributo expired
 */
bool Parser::ActionResponse::Expired() const
{
    return expired;
}

/*
 *  Descrizione: selettore di lettura dell'attributo req_timestamp
 */
std::string Parser::ActionResponse::RequestTimestamp() const
{
    return req_timestamp;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo subject
 */
void Parser::ActionResponse::Subject(std::string subject)
{
    if (subject == "")
        throw ParserException("Invalid empty subject", "void Parser::ActionResponse::Subject(std::string subject)");
    this->subject = subject;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo function
 */
void Parser::ActionResponse::Function(const std::string& function)
{
    if (function == "")
        throw ParserException("Invalid empty function", "void Parser::ActionResponse::Function(std::string function)");
    this->function = function;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo control_type
 */
void Parser::ActionResponse::SetControlType(ControlType control_type)
{
    if (control_type == ControlType::Undefined)
        throw ParserException("Invalid undefined control_type", "void Parser::ActionResponse::SetControlType(ControlType control_type)");
    this->control_type = control_type;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo success
 */
void Parser::ActionResponse::Success(bool success)
{
    this->success = success;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo expired
 */
void Parser::ActionResponse::Expired(bool expired)
{
    this->expired = expired;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo req_timestamp
 */
void Parser::ActionResponse::RequestTimestamp(const std::string& ts)
{
    req_timestamp = ts;
}


/*
 *  Descrizione: la funzione estra i dati dall'oggetto json per compilare gli attributi dell'oggetto
 */
void Parser::ActionResponse::Parse(const Json::Value& body)
{
    Json::Value info;
    Json::Value node;
    Json::Value data;


    info = body["info"];
    if (info == Json::nullValue)
        throw Parser::ParserException("Required info key: sensor-id", "void Parser::ActionResponse::Parse(const Json::Value& body)");
    
    node = info["subject"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: function", "void Parser::ActionResponse::Parse(const Json::Value& body)");
    subject = node.asString();

    node = info["function"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: samples", "void Parser::ActionResponse::Parse(const Json::Value& body)");
    function = node.asString();

    node = info["control-type"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required info key: control-type", "void Parser::ActionResponse::Parse(const Json::Value& body)");
    control_type = StringToControlType(node.asString());

    data = body["data"];
    if (data == Json::nullValue)
        throw Parser::ParserException("Required body key: data", "void Parser::ActionResponse::Parse(const Json::Value& body)");
    
    node = data["success"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required data key: success", "void Parser::ActionResponse::Parse(const Json::Value& body)");
    if (!node.isBool())
        throw Parser::ParserException("Invalid data key: success must be bool", "void Parser::ActionResponse::Parse(const Json::Value& body)");
    success = node.asBool();

    node = data["expired"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required data key: expired", "void Parser::ActionResponse::Parse(const Json::Value& body)");
    if (!node.isBool())
        throw Parser::ParserException("Invalid data key: expired must be bool", "void Parser::ActionResponse::Parse(const Json::Value& body)");
    expired = node.asBool();

    node = data["request-timestamp"];
    if (node == Json::nullValue)
        throw Parser::ParserException("Required data key: required-timestamp", "void Parser::ActionResponse::Parse(const Json::Value& body)");
    req_timestamp = node.asString();
}


/*
 *  Descrizione: converte i dati degli attributi dell'oggetto in un oggetto json, secondo il protocollo
 */
Json::Value Parser::ActionResponse::ToJsonValue() const
{
    Json::Value root;

    root["info"]["subject"] = subject;
    root["info"]["function"] = function;
    root["info"]["control-type"] = ControlTypeToString(control_type);

    root["data"]["success"] = success;
    root["data"]["expired"] = expired;
    root["data"]["request-timestamp"] = req_timestamp;

    return root;
}






/* 
 * CLASS CatalogueResponse
 */
// UNIMPLEMENTED







/* 
 * CLASS Response
 */

/*
 *  Descrizione: La funzione elimina i dati dinamici puntati dagli attributi base: sensor, action, 
 *      catalogue
 */
void Parser::Response::CleanParsing()
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

    if (error != nullptr)
    {
        delete error;
        error = nullptr;
    }
}

/*
 *  Operatore di copia
 *  Descrizione: effettua la copia profonda dei dati
 */
Parser::Response& Parser::Response::operator=(const Response& res)
{
    // Pulizia oggetti
    CleanParsing();

    // Attributi derivati dalla classe Message
    version = res.version;
    schema = res.schema;
    type = res.type;
    timestamp = res.timestamp;
    body = res.body;

    // Copia profonda
    if (res.sensor != nullptr)
    {
        sensor = new Parser::SensorResponse(*res.sensor);
    }
    else
    {
        sensor = nullptr;
    }
    if (res.action != nullptr)
    {
        action = new Parser::ActionResponse(*res.action);
    }
    else
    {
        action = nullptr;
    }
    
    if (res.catalogue != nullptr)
    {
        catalogue = new Parser::CatalogueResponse(*res.catalogue);
    }
    else
    {
        catalogue = nullptr;
    }
    
    if (res.error != nullptr)
    {
        error = new Parser::ErrorResponse(*res.error);
    }
    else
    {
        error = nullptr;
    }

    return *this;
}

/*
 *  Costruttore
 */
Parser::Response::Response()
    : Message()
{
    type = Parser::Type::Response;
    timestamp = TimestampString();

    sensor = nullptr;
    action = nullptr;
    catalogue = nullptr;
    error = nullptr;
}

/*
 *  Costruttore: la stringa 'message' deve contenere il messaggio json che contiene i dati per compilare i dati
 *      dell'oggetto
 */
Parser::Response::Response(const std::string& message)
    : Message(message)
{
    if (type != Type::Response)
        throw Parser::ParserException("Invalid message type", "Parser::Response::Response(const std::string& message)");

    switch(schema)
    {
        case Parser::Schema::Action:
        case Parser::Schema::Sensor:
        case Parser::Schema::Catalogue:
        case Parser::Schema::Error:
            ParseFromBody();
            break;

        case Parser::Schema::Undefined:
            throw Parser::ParserException("Invalid request schema: undefined", "Parser::Response::Response(const std::string& message)");
            break;
        default:
            throw Parser::ParserException("Unimplemented schema", "Parser::Response::Response(const std::string& message)");
            break;
    }
}

/*
 *  Costruttore di copia: effettua la copia prodonda dell'oggetto sorgente
 */
Parser::Response::Response(const Response& res)
{
    // Attributi derivati dalla classe Message
    version = res.version;
    schema = res.schema;
    type = res.type;
    timestamp = res.timestamp;
    body = res.body;

    // Copia profonda
    if (res.sensor != nullptr)
    {
        sensor = new Parser::SensorResponse(*res.sensor);
    }
    if (res.action != nullptr)
    {
        action = new Parser::ActionResponse(*res.action);
    }
    if (res.catalogue != nullptr)
    {
        catalogue = new Parser::CatalogueResponse(*res.catalogue);
    }
    if (res.error != nullptr)
    {
        error = new Parser::ErrorResponse(*res.error);
    }
}

/*
 *  Distruttore
 */
Parser::Response::~Response()
{
    delete sensor;
    delete action;
    delete catalogue;
    delete error;
}


/*
 *  Descrizione: selettore di lettura dell'attributo sensor
 */
Parser::SensorResponse& Parser::Response::GetSensorResponse()
{
    return *sensor;
}

/*
 *  Descrizione: selettore di lettura dell'attributo action
 */
Parser::ActionResponse& Parser::Response::GetActionResponse()
{
    return *action;
}

/*
 *  Descrizione: selettore di lettura dell'attributo catalogue
 */
Parser::CatalogueResponse& Parser::Response::GetCatalogueResponse()
{
    return *catalogue;
}

/*
 *  Descrizione: selettore di lettura dell'attributo response
 */
Parser::ErrorResponse& Parser::Response::GetErrorResponse()
{
    return *error;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo sensor
 */
void Parser::Response::SetSensorResponse(const SensorResponse& response)
{
    if (!IsBodyParsed())
        CleanParsing();     // Elimina dati precedenti
    
    schema = Schema::Sensor;
    body = SensorResponse(response).ToJsonValue();      // Setting dell'attributo body
    ParseFromBody();        // Parsing del body per compilare action con i dati
}

/*
 *  Descrizione: selettore di scrittura dell'attributo action
 */
void Parser::Response::SetActionResponse(const ActionResponse& response)
{
    if (!IsBodyParsed())
        CleanParsing();     // Elimina dati precedenti

    schema = Schema::Action;
    body = ActionResponse(response).ToJsonValue();      // Setting dell'attributo body
    ParseFromBody();         // Parsing del body per compilare action con i dati
}

/*
 *  Descrizione: selettore di scrittura dell'attributo catalogue
 */
void Parser::Response::SetCatalogueResponse(const CatalogueResponse& response)
{
    // UNIMPLEMENTED
    throw ParserException("Unimplemented", "void Parser::Response::SetCatalogueResponse(CatalogueResponse* response)");
}


/*
 *  Descrizione: selettore di scrittura dell'attributo error
 */
void Parser::Response::SetErrorResponse(const ErrorResponse& response)
{
    if (!IsBodyParsed())
        CleanParsing();     // Elimina dati precedenti
    
    schema = Schema::Error;
    body = ErrorResponse(response).ToJsonValue();   // Setting dell'attributo body
    ParseFromBody();         // Parsing del body per compilare action con i dati
}   

/*
 *  Descrizione: selettore di scrittura dell'attributo type. 
 *      Overloading del metodo della classe Message per evitare errori
 */
void Parser::Response::SetType(const Type& type)
{
    if (type != Type::Response)
        throw ParserException("Invalid type specified for a Response", "void Parser::Response::SetType(const Type& type)");
}

/*
 *  Descrizione: indica se è stato effettuato il parsing dei dati del body. Se true, è puntato
 *      un oggetto SensorRequest, ActionRequest o CatalogueRequest 
 */
bool Parser::Response::IsBodyParsed()
{
    return action != nullptr || action != nullptr || catalogue != nullptr || error != nullptr;
}

/*
 *  Descrizione: compila gli attributi dell'oggetto eseguendo il parsing dei dati contenuti nell'attributo body
 */
void Parser::Response::ParseFromBody()
{
    if (type != Type::Response)
        throw Parser::ParserException("Invalid message type " + TypeToString(type), "void Parser::Response::ParseFromBody()");

    if (IsBodyParsed())
        CleanParsing();     // Elimina eventuali dati precedenti
    
    // In base allo schema del messaggio, costruisce uno degli attributi base
    // con il costruttore della relativa classe
    switch(schema)
    {
        case Parser::Schema::Sensor:
            sensor = new Parser::SensorResponse(body);
            break;
        
        case Parser::Schema::Action:
            action = new ActionResponse(body);
            break;
        
        case Parser::Schema::Catalogue:
            // UNIMPLEMENTED
            throw Parser::ParserException("Unimplemented schema: catalogue", "void Parser::Response::ParseFromBody()");
            break;
        
        case Parser::Schema::Error:
            error = new ErrorResponse(body);
            break;
        
        case Parser::Schema::Undefined:
            throw Parser::ParserException("Unimplemented schema: undefined", "void Parser::Response::ParseFromBody()");
            break;
        
        default:
            throw Parser::ParserException("Unimplemented schema", "void Parser::Response::ParseFromBody()");
            break;
    }
}


/*
 *  Descrizione: compila gli attributi dell'oggetto in base alla stringa 'message',
 *      Questa deve contenere il messaggio in formato json secondo lo standard
 */
void Parser::Response::Parse(const std::string& message)
{
    if(IsBodyParsed())
        CleanParsing();
    
    ParseHeader(message);

    ParseFromBody();
}


