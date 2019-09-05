/****************************************
 *  Universita' degli Studi di Udine    *
 *  Ingegneria Elettronica              *
 *  Tesi di Laurea Triennale            *
 *  Giacomo Andrioli - 133047           *
 *  File: parser-message.cpp            *
 ****************************************/

#include "Parser.hpp"

/* 
 * CLASS Message
 */

/*
 *  Descrizione: metodo statico che estrae dal messaggio json, contenuto nell'argomento 'message',
 *      il tipo di messaggio
 */
Parser::Type Parser::Message::GetMessageType(const std::string& message)
{
    Json::CharReaderBuilder builder;
    std::stringstream stream(message);      // Costruzione stream del messaggio
    std::string errors;

    Json::Value root;
    Json::Value node;

    // Parsing nell'oggetto json root
    bool success = Json::parseFromStream(builder, stream, &root, &errors);
    if(!success)
    {
        throw ParserException("Invalid raw JSON string", "Message::ParseHeader() -> Json::Reader::parse(...)");
    }

    // Estrazione parametro type
    node = root["type"];
    if(node == Json::nullValue)
    {
        throw ParserException("Required key: type", "Parser::Message::ParseHeader(const std::string&)");
    }

    return Parser::StringToType(node.asString());
}


/*
 *  Descrizione: operatore di output
 */
std::ostream& Parser::operator<< (std::ostream & os, const Message& m)
{ 
    return os << m.ToString() << std::endl;
}


/*
 *  Costruttore: senza argomenti 
 */
Parser::Message::Message()
    : version("1.0"), schema(Schema::Undefined), 
        type(Type::Undefined), timestamp(TimestampString()), body("")
{}

/*
 *  Costruttore: l'argomento stringa 'message' deve contenere il messaggio con cui si vuole inizializzare l'oggetto.
 *      Il parsing avviene tramite la funzione ParseHeader()
 */
Parser::Message::Message(const std::string& message)
    : version(""), schema(Schema::Undefined), type(Type::Undefined), timestamp(""), body("")
{
    ParseHeader(message);
}

/*
 *  Descrizione: Indica se e' stato chiamato il metodo ParseHeader() e quindi se l'oggetto 
 *      contiene dei dati validi
 */
bool Parser::Message::IsHeaderParsed()
{
    return schema != Schema::Undefined && type != Type::Undefined;
}

/*
 *  Descrizione: la funzione effettua il parsing dell'argomento stringa 'raw' per compilare gli
 *      attributi dell'oggetto.
 */
void Parser::Message::ParseHeader(const std::string& raw)
{
    Json::CharReaderBuilder builder;
    std::stringstream stream(raw);      // Costruzione stream del messaggio
    std::string errors;

    Json::Value root;
    Json::Value node;

    bool success;
    success = Json::parseFromStream(builder, stream, &root, &errors);

    if(!success)
    {
        throw ParserException("Invalid raw JSON string", "Message::ParseHeader() -> Json::Reader::parse(...)");
    }

    node = root["protocol-version"];
    if(node == Json::nullValue)
    {
        throw Parser::ParserException("Required key: protocol-version", "Parser::Message::ParseHeader(const std::string&)");
    }
    version = node.asString();

    node = root["schema"];
    if(node == Json::nullValue)
    {
        throw Parser::ParserException("Required key: Schema", "Parser::Message::ParseHeader(const std::string&)");
    }
    this->schema = Parser::StringToSchema(node.asString());

    node = root["type"];
    if(node == Json::nullValue)
    {
        throw ParserException("Required key: type", "Parser::Message::ParseHeader(const std::string&)");
    }
    type = Parser::StringToType(node.asString());

    if(root["body"] != Json::nullValue)
        body = root["body"];
    
    node = root["timestamp"];
    if(node == Json::nullValue)
    {
        throw ParserException("Required key: timestamp", "Parser::Message::ParseHeader(const std::string&)");
    }
    timestamp = node.asString();
}


/*
 *  Descrizione: restituisce la stringa contenente i dati dell'oggetto in formato json
 */
std::string Parser::Message::ToString() const
{
    Json::Value root;
    
    root["protocol-version"] = version != "" ? version : "1.0";
    root["schema"] = SchemaToString(schema);
    root["type"] = TypeToString(type);
    root["body"] = body;
    root["timestamp"] = timestamp != "" ? timestamp : TimestampString();
    
    return Json::writeString(Json::StreamWriterBuilder(), root);
}

/*
 *  Descrizione: selettore di lettura dell' attributo version
 */
std::string Parser::Message::Version() const
{
    return version;
}

/*
 *  Descrizione: selettore di scrittura dell'attributo version
 */
void Parser::Message::Version(const std::string& s)
{
    if (std::stod(s) <= 0)
        throw ParserException("Invalid version string", "Message::Version(const std::string&)");
    version = s;
}

/*
 *  Descrizione: selettore di lettura dell'attributo body
 */
std::string Parser::Message::Body() const
{
    return Json::writeString(Json::StreamWriterBuilder(), body);
}

/*
 *  Descrizione: selettore di scrittura attributo body
 */
void Parser::Message::Body(const std::string& body_string)
{
    Json::CharReaderBuilder builder;
    std::stringstream stream(body_string);
    std::string errors;
    bool success;

    success = Json::parseFromStream(builder, stream, &body, &errors);
    if(!success)
    {
        throw ParserException("Invalid raw JSON string", "void Parser::Message::Body(const std::string& str)");
    }
}


/*
 *  Descrizione: selettore di lettura attributo schema
 */
Parser::Schema Parser::Message::GetSchema() const
{
    return schema;
}

/*
 *  Descrizione: selettore di scrittura attributo schema
 */
void Parser::Message::SetSchema(const Schema& schema)
{
    this->schema = schema;
}

/*
 *  Descrizione: selettore di lettura attributo type
 */
Parser::Type Parser::Message::GetType() const
{
    return type;
}

/*
 *  Descrizione: selettore di scrittura attributo type
 */
void Parser::Message::SetType(const Type& type)
{
    this->type = type;
}

/*
 *  Descrizione: selettore di lettura attributo timestamp
 */
std::string Parser::Message::Timestamp() const
{
    return timestamp;
}

/*
 *  Descrizione: selettore di scrittura attributo timestamp
 */
void Parser::Message::Timestamp(const std::string& timestamp)
{
    this->timestamp = timestamp;
}



/*
 *  Class MessageBody 
*/

Parser::MessageBody::~MessageBody()
{}

void Parser::MessageBody::Parse(const Json::Value&)
{
    throw ParserException("Unimplemented interface", "void Parser::MessageBody::Parse(const Json::Value&)");
}

Json::Value Parser::MessageBody::ToJsonValue() const
{
    throw ParserException("Unimplemented interface", "Json::Value Parser::MessageBody::ToJsonValue() const");
}


