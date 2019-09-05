/****************************************
 *  Universita' degli Studi di Udine    *
 *  Ingegneria Elettronica              *
 *  Tesi di Laurea Triennale            *
 *  Giacomo Andrioli - 133047           *
 *  File: parser-utility.cpp            *
 ****************************************/

#include "Parser.hpp"


/* 
     * UTILITY (parser-utility.cpp)
     * 
     *  Classi enum:
     *      - Schema
     *      - Type
     *      - ControlType
     *      - SignalType
     *  Classi:
     *      - ParserException
     *      - Signal
     *      - Program
     *  Altro:
     *      - typedef
     *      - funzioni per timestamps 
     */


/* 
 * Timestamps utilities
*/

/*  Descrizione: la funzione ritorna una strina con il timestamp UTC in formato:
 *                      dd-mm-yyyy/hh:mm:ss
 *  - dd indica il giorno del mese espresso a due cifre
 *  - mm indica il mese espresso a due cifre
 *  - yyyy indica l’anno espresso a quattro cifre
 *  - hh indica l’ora espressa a due cifre
 *  - mm indica l’ora espressa a due cifre
 *  - ss indica l’ora espressa a due cifre
 *      
*/
std::string Parser::TimestampString()
{
    // Tempo attuale
    std::time_t now = time(nullptr);
    return TimestampToString(now);
}


/*  
 *  Descrizione: la funzione ritorna una strina con il timestamp UTC
 *      ottenuto da time, in formato:
 *                      dd-mm-yyyy/hh:mm:ss
 *      - dd indica il giorno del mese espresso a due cifre
 *      - mm indica il mese espresso a due cifre
 *      - yyyy indica l’anno espresso a quattro cifre
 *      - hh indica l’ora espressa a due cifre
 *      - mm indica l’ora espressa a due cifre
 *      - ss indica l’ora espressa a due cifre
 *  Argomento:
 *      - const std::time_t& time: tempo da convertire in stringa
 *      
*/
std::string Parser::TimestampToString (const std::time_t& time)
{
    std::tm* cal = std::gmtime(&time);
    std::stringstream stream;
    if (cal->tm_mday < 10)
        stream << "0";
    stream << cal->tm_mday << "-";
    if (cal->tm_mon < 10)
        stream << "0";
    stream << cal->tm_mon << "-" << 1900+cal->tm_year << "/";
    if (cal->tm_hour < 10)
        stream << "0";
    stream << cal->tm_hour << ":";
    if (cal->tm_min < 10)
        stream << "0";
    stream << cal->tm_min <<":"; 
    if (cal->tm_sec < 10)
        stream << "0";
    stream << cal->tm_sec;
    return stream.str();
}


time_t Parser::StringToTimestamp (const std::string &)
{
    // Unimplemented
    throw ParserException("Unimplemented function", "StringToTimestamp()");
}






/* 
 * ENUM CLASSES UTILITIES:
 *  - Schema
 *  - Type
 *  - ControlType
 *  - SignalType
 * 
 */




/*
 * 
 *  ENUM CLASS Schema
 * 
 */

/*
 *  Desctione: operatore di output
 */
std::ostream& Parser::operator<< (std::ostream& os, const Schema& s)
{
    return os << SchemaToString(s);
}

/*
 *  Descrizione: la funzione restituisce la stringa associata all'enum schema passato 
 *      come parametro.
 */
std::string Parser::SchemaToString(const Schema & schema)
{
    switch (schema)
    {
        case Schema::Action:
            return "action";
        case Schema::Sensor:
            return "sensor";
        case Schema::Catalogue:
            return "catalogue";
        case Schema::Error:
            return "error";
        case Schema::Undefined:
            return "undefined";
        default:
            throw ParserException("Unimplemented Schema case", "Parser::SchemaToString(...)");
            return "";
    }
}


/*
 *  Descrizione: restituisce l'enum Schema corrispondente alla stringa 's'
 */
Parser::Schema Parser::StringToSchema(const std::string& s)
{
    if (s == "action")
        return Schema::Action;
    if (s == "sensor")
        return Schema::Sensor;
    if (s == "catalogue")
        return Schema::Catalogue;
    if (s == "error")
        return Schema::Error;
    if (s == "undefined")
        return Schema::Undefined;
    throw ParserException("Invalid string for a Schema", "Parser::StringToSchema(...)");
    return Schema::Undefined;
}








/*
 * ENUM CLASS Type
 */

/*
 *  Descrizione: operatore di output
 */
std::ostream& Parser::operator<< (std::ostream& os, const Type& t)
{
    return os << TypeToString(t);
}

/*
 *  Descrizione: la funzione restituisce la stringa associata all'enum type passato 
 *      come parametro.
 */
std::string Parser::TypeToString(const Type& type)
{
    switch (type)
    {
        case Type::Request:
            return "request";
        case Type::Response:
            return "response";
        case Type::Undefined:
            return "undefined";
        default:
            throw ParserException("Unimplemented type case", "Parser::TypeToString(...)");
            return "";
    }
}



/*
 *  Descrizione: restituisce l'enum Type corrispondente alla stringa 's'
 */
Parser::Type Parser::StringToType(const std::string& s)
{
    if (s == "request")
        return Type::Request;
    if (s == "response")
        return Type::Response;
    if (s == "undefined")
        return Type::Undefined;
    throw ParserException("Invalid string for a Type", "Parser::StringToType(...)");
    return Type::Undefined;
}





/*
 * ENUM CLASS ControlType
 */

/*
 *  Descrizione: operatore di output
 */
std::ostream& Parser::operator<< (std::ostream& os, const Parser::ControlType& type)
{
    return os << ControlTypeToString(type);
}


/*
 *  Descrizione: la funzione restituisce la stringa associata all'enum type passato 
 *      come parametro.
 */
std::string Parser::ControlTypeToString(Parser::ControlType type)
{
    switch (type)
    {
        case ControlType::Undefined:
            return "undefined";
        case ControlType::Signal:
            return "signal";
        case ControlType::Program:
            return "program";
        default:
            throw ParserException("Unimplemented type case", "Parser::ControlTypeToString(...)");
            return "";
    }
}


/*
 *  Descrizione: restituisce l'enum ControlType corrispondente alla stringa 's'
 */
Parser::ControlType Parser::StringToControlType(std::string s)
{
    if (s == "signal")
        return ControlType::Signal;
    if (s == "program")
        return ControlType::Program;
    if (s == "undefined")
        return ControlType::Undefined;
    throw ParserException("Invalid string for a Type", "Parser::StringToType(...)");
    return ControlType::Undefined;
}








/*
 * ENUM CLASS SignalType
 */

/*
 *  Descrizione: operatore di output
 */
std::ostream& Parser::operator<< (std::ostream& os, const Parser::SignalType& type)
{
    return os << SignalTypeToString(type);
}


/*
 *  Descrizione: la funzione restituisce la stringa associata all'enum type passato 
 *      come parametro.
 */
std::string Parser::SignalTypeToString(Parser::SignalType type)
{
    switch (type)
    {
        case SignalType::Undefined:
            return "undefined";
        case SignalType::Impulse:
            return "impulse";
        case SignalType::Step:
            return "step";
        case SignalType::Rect:
            return "rect";
        case SignalType::Ramp:
            return "ramp";
        case SignalType::Triangle:
            return "triangle";
        default:
            throw ParserException("Unimplemented type case", "Parser::SignalTypeToString(...)");
            return "";
    }
}



/*
 *  Descrizione: restituisce l'enum SignalType corrispondente alla stringa 's'
 */
Parser::SignalType Parser::StringToSignalType(std::string s)
{
    if (s == "impulse")
        return SignalType::Impulse;
    if (s == "step")
        return SignalType::Step;
    if (s == "rect")
        return SignalType::Rect;
    if (s == "ramp")
        return SignalType::Ramp;
    if (s == "triangle")
        return SignalType::Triangle;
    if (s == "undefined")
        return SignalType::Undefined;
    throw ParserException("Invalid string for a Type", "Parser::StringToType(...)");
    return SignalType::Undefined;
}

















/* 
 * CLASS ParserException
 */

/*
 *  Descrizione: operatore di output
 */
std::ostream& Parser::operator<< (std::ostream & os, const Parser::ParserException& e)
{ 
    os  << "ParserException: " << e.message << std::endl 
        << "@ " << e.where << std::endl;
    if (e.error_code != "")
        os  << "Code: " << e.error_code << std::endl;
    return os;
}

/*
 *  Costruttore
 */
Parser::ParserException::ParserException(std::string what, std::string at, std::string code)
    : message(what), where(at), error_code(code) 
{}

/*
 *  Descrizione: selettore campo message
 */
const char* Parser::ParserException::what() const noexcept
{ 
    return message.c_str(); 
}

/*
 *  Descrizione: selettore campo where
 */
std::string Parser::ParserException::at() const
{ 
    return where;
}

/*
 *  Descrizione: selettore campo error_code
 */
std::string Parser::ParserException::code() const
{
    return error_code;
}













/* 
 * CLASS Signal
 */


/*
 *  Descrizione: operatore di output
 */
std::ostream& Parser::operator<< (std::ostream& os, const Parser::Signal& s)
{
    os  << "Signal type: " << s.sig_type << "; "
        << "Peak: " << s.sig_peak << "; "
        << "Delay: " << s.sig_delay << "; "
        << "Duration: " << s.sig_duration << "; "
        << "Offset: " << s.sig_offset << ". ";
    return os;
}

/*
 *  Costruttore senza argomenti
 */
Parser::Signal::Signal()
    : sig_type(SignalType::Undefined)
{
    sig_peak = 0;
    sig_duration = 0;
    sig_delay = 0;
    sig_offset = 0;
}


/*
 *  Costruttore: data contiene l'oggetto json che viene utilizzato per estrarre
 *      i parametri, secondo il protocollo. Non tutti i tipi di segnale specificano tutti gli argomenti.
 */
Parser::Signal::Signal(const Json::Value& data)
{
    Json::Value node;

    if (data == Json::nullValue)
        throw ParserException("nullValue argument 'data'", "Parser::Signal::Signal(Json::Value data)");
    

    // Parametro signal_type
    node = data["signal-type"];
    if (node == Json::nullValue)
        throw ParserException("Required key: signal-type", "Parser::Signal::Signal(Json::Value data)");
    sig_type = StringToSignalType(node.asString());


    // Parsing dei parametri del segnale: in base al SignalType, alcuni parametri non devono essere specificati
    switch (sig_type)
    {
        case SignalType::Undefined:
            throw ParserException("Undefined signal type not allowed", "Parser::Signal::Signal(Json::Value data)");
            break;
        
        case SignalType::Ramp:
        case SignalType::Triangle:

            // Parametro offset
            node = data["offset"];
            if (node == Json::nullValue)
                throw ParserException("Required key: offset", "Parser::Signal::Signal(Json::Value data)");
            if (!node.isNumeric())
                throw ParserException("Invalid offset value: must be numeric", "Parser::Signal::Signal(Json::Value data)");
            sig_offset = node.asDouble();
        

        case SignalType::Rect:
            // Parametro duration
            node = data["duration"];
            if (node == Json::nullValue)
                throw ParserException("Required key: duration", "Parser::Signal::Signal(Json::Value data)");
            if (!node.isNumeric())
                throw ParserException("Invalid duration value: must be numeric", "Parser::Signal::Signal(Json::Value data)");
            sig_duration = node.asDouble();
            if (sig_duration < 0)
                throw ParserException("Invalid value: duration < 0", "Parser::Signal::Signal(Json::Value data)");


        case SignalType::Impulse:
        case SignalType::Step:

            // Parametri comuni a tutti i tipi di segnali
            //// Parametro peak
            node = data["peak"];
            if (node == Json::nullValue)
                throw ParserException("Required key: peak", "Parser::Signal::Signal(Json::Value data)");
            if (!node.isNumeric())
                throw ParserException("Invalid peak value: must be numeric", "Parser::Signal::Signal(Json::Value data)");
            sig_peak = node.asDouble();

            //// Parametro delay
            node = data["delay"];
            if (node == Json::nullValue)
                throw ParserException("Required key: delay", "Parser::Signal::Signal(Json::Value data)");
            if (!node.isNumeric())
                throw ParserException("Invalid delay value: must be numeric", "Parser::Signal::Signal(Json::Value data)");
            sig_delay = node.asDouble();
            if (sig_delay < 0)
                throw ParserException("Invalid value: delay < 0", "Parser::Signal::Signal(Json::Value data)");
            break;
        
        default:
            throw ParserException("Unimplemented SignalType case", "Parser::Signal::Signal(Json::Value data)");
            break;
    }
}

/*
 *  Descrizione: selettore dell'attributo sig_type
 */
Parser::SignalType Parser::Signal::GetSignalType() const
{
    return sig_type;
}

/*
 *  Descrizione: selettore dell'attributo sig_peak
 */
double Parser::Signal::Peak() const
{
    return sig_peak;
}

/*
 *  Descrizione: selettore dell'attributo sig_duration
 */
double Parser::Signal::Duration() const
{
    return sig_duration;
}

/*
 *  Descrizione: selettore dell'attributo sig_delay
 */
double Parser::Signal::Delay() const
{
    return sig_delay;
}

/*
 *  Descrizione: selettore dell'attributo sig_offset
 */
double Parser::Signal::Offset() const
{
    return sig_offset;
}

/*
 *  Descrizione: setting del tipo di segnale indefinito, senza parametri
 */
void Parser::Signal::SetUndefined()
{
    sig_type = SignalType::Undefined;
}

/*
 *  Descrizione: setting del tipo di segnale impulso, con parametri peak e delay
 */
void Parser::Signal::SetImpulse(double peak, double delay)
{
    sig_type = SignalType::Impulse;

    sig_peak = peak;
    
    if (delay<0)
        throw ParserException("Invalid anticausal delay", "void Parser::Signal::SetImpulse(double value, double delay)");
    sig_delay = delay;
}


/*
 *  Descrizione: setting del tipo di segnale gradino, con parametri peak e delay
 */
void Parser::Signal::SetStep(double peak, double delay)
{
    sig_type = SignalType::Step;
    sig_peak = peak;
    
    if (delay<0)
        throw ParserException("Invalid anticausal delay", "void Parser::Signal::SetStep(double value, double delay)");
    sig_delay = delay;
}


/*
 *  Descrizione: setting del tipo di segnale rettangolare, con parametri peak, duration e delay
 */
void Parser::Signal::SetRect(double peak, double delay, double duration)
{
    sig_type = SignalType::Rect;
    sig_peak = peak;
    
    if (delay<0)
        throw ParserException("Invalid anticausal delay", "void Parser::Signal::SetRect(double value, double delay)");
    sig_delay = delay;

    if (duration<0)
        throw ParserException("Invalid negative duration", "void Parser::Signal::SetRect(double value, double delay)");
    sig_duration = duration;
}


/*
 *  Descrizione: setting del tipo di segnale rampa, con parametri peak, duration, offset e delay
 */
void Parser::Signal::SetRamp(double peak, double delay, double duration, double offset)
{
    sig_type = SignalType::Ramp;
    sig_peak = peak;
    
    if (delay<0)
        throw ParserException("Invalid anticausal delay", "void Parser::Signal::SetRamp(double value, double delay, double duration, double delta)");
    sig_delay = delay;

    if (duration<0)
        throw ParserException("Invalid negative duration", "void Parser::Signal::SetRamp(double value, double delay, double duration, double delta)");
    sig_duration = duration;

    sig_offset = offset;
}


/*
 *  Descrizione: setting del tipo di segnale triangolare, con parametri peak, duration, offset e delay
 */
void Parser::Signal::SetTriangle(double peak, double delay, double duration, double offset)
{
    sig_type = SignalType::Triangle;
    
    sig_peak = peak;
    
    if (delay<0)
        throw ParserException("Invalid anticausal delay", "void Parser::Signal::SetRamp(double value, double delay, double duration, double delta)");
    sig_delay = delay;

    if (duration<0)
        throw ParserException("Invalid negative duration", "void Parser::Signal::SetRamp(double value, double delay, double duration, double delta)");
    sig_duration = duration;

    sig_offset = offset;
}











/* 
 * CLASS Program
 */

/*
 *  Descrizione: operatore di output
 */
std::ostream& Parser::operator<<(std::ostream& os, const Program& p)
{
    os  << "Id: " << p.program_id << std::endl;
    
    if(p.parameter_list.size())
    {
        os  << "Parameter list:" << std::endl;
    
        for (unsigned int i = 0; i < p.parameter_list.size(); i++)
        {
            os  << p.parameter_list.at(i).first << " : " << p.parameter_list.at(i).second << std::endl;
        }
    }
    return os;
}

/*
 *  Costruttore: imposta program_id. Il parametro id ha valore di default 0U
 */
Parser::Program::Program(unsigned int id)
{
    program_id = id;
}

/*
 *  Costruttore: l'oggetto data contiene il json da cui vengono estratti gli attributi dell'oggetto
 */
Parser::Program::Program(const Json::Value& data)
{
    Json::Value node;

    if (data == Json::nullValue)
        throw ParserException("Invalid nullValue argument 'data'", "Parser::Program::Program(Json::Value data)");
    
    // Parametro program_id
    node = data["program-id"];
    if (node == Json::nullValue)
        throw ParserException("Required key: program-id", "Parser::Program::Program(Json::Value data)");
    if (!node.isInt())
        throw ParserException("Required key: program-id", "Parser::Program::Program(Json::Value data)");
    program_id = node.asInt();

    // Analisi dell'oggetto json 'parameters' da cui viene estratta la lista dei parametri
    node = data["parameters"];
    if (node == Json::nullValue)
        throw ParserException("Required key: parameters", "Parser::Program::Program(Json::Value data)");

    Json::Value::Members mems(node.getMemberNames());
    for (unsigned int i = 0; i < mems.size(); i++)
    {
        parameter_list.push_back(std::make_pair(mems.at(i), node[mems.at(i)].asString()));
    }


}

/*
 *  Descrizione: selettore campo program_id
 */
unsigned int Parser::Program::ProgramId()
{
    return program_id;
}

/*
 *  Descrizione: selettore campo program_id
 */
void Parser::Program::ProgramId(unsigned int id)
{
    program_id = id;
}

/*
 *  Descrizione: selettore campo parameter_list
 */
Parser::StringStringMapVector Parser::Program::ParameterList()
{
    return parameter_list;
}

/*
 *  Descrizione: selettore campo parameter_list
 */
void Parser::Program::ParameterList(const Parser::StringStringMapVector& v)
{
    parameter_list = v;
}



