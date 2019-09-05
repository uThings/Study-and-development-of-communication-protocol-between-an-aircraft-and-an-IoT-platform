/****************************************
 *  Universita' degli Studi di Udine    *
 *  Ingegneria Elettronica              *
 *  Tesi di Laurea Triennale            *
 *  Giacomo Andrioli - 133047           *
 *  File: Parser.hpp                    *
 ****************************************/

#ifndef PARSER
#define PARSER

#include <map>
#include <vector>
#include <exception>
#include <string>
#include <iostream>
#include <ctime>

#include "json.h"

namespace Parser 
{
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

    typedef std::vector<std::pair<std::string,std::vector<std::string>>> SensorData;
    typedef std::vector<std::pair<std::string,std::string>> StringStringMapVector;

    enum class Schema       { Undefined, Sensor, Action, Catalogue, Error};
    std::ostream& operator<< (std::ostream& , const Schema&);
    std::string SchemaToString(const Schema &);
    Schema StringToSchema(const std::string &);
    
    enum class Type         { Undefined, Request, Response};
    std::ostream& operator<< (std::ostream& , const Type&);
    std::string TypeToString(const Type&);
    Type StringToType(const std::string&);
    
    enum class ControlType  { Undefined, Signal, Program};
    std::ostream& operator<< (std::ostream& , const ControlType&);
    std::string ControlTypeToString(ControlType);
    ControlType StringToControlType(std::string);
    
    enum class SignalType   { Undefined, Impulse, Step, Rect, Ramp, Triangle};
    std::ostream& operator<< (std::ostream& , const SignalType&);
    std::string SignalTypeToString(SignalType);
    SignalType StringToSignalType(std::string);

    // Timestamp utilities
    std::string TimestampString();
    std::string TimestampToString (const std::time_t&);
    time_t StringToTimestamp (const std::string &);

    
    class ParserException : public std::exception
    {
        friend std::ostream& operator<< (std::ostream & os, const ParserException& e);

        private:
            std::string message;
            std::string where;
            std::string error_code;
        
        public:
            ParserException(std::string what, std::string at = "", std::string code = "");
            const char* what() const noexcept;
            std::string at() const;         
            std::string code() const;         

    };
    std::ostream& operator<< (std::ostream & os, const Parser::ParserException& e);
    

    class Signal {
        friend std::ostream& operator<< (std::ostream& os, const Signal& s);
        
        private:
            SignalType sig_type;
            double sig_peak;
            double sig_delay;
            double sig_duration;
            double sig_offset;
        
        public:
            // Costruttori
            Signal();
            Signal(const Json::Value& data);

            // Selettori
            SignalType GetSignalType() const;
            double Peak() const;
            double Delay() const;
            double Duration() const;
            double Offset() const;

            void SetUndefined();
            void SetImpulse(double peak, double delay);
            void SetStep(double peak, double delay);
            void SetRect(double peak, double delay, double duration);            
            void SetRamp(double peak, double delay, double duration, double offset);           
            void SetTriangle(double peak, double delay, double duration, double offset);           
    };
    std::ostream& operator<< (std::ostream& os, const Parser::Signal& s);


    class Program {
        friend std::ostream& operator<<(std::ostream&, const Program&);
        private:
            unsigned int program_id;
            StringStringMapVector parameter_list; 
        
        public:
            // Costruttore
            Program(unsigned int id = 0);
            Program(const Json::Value& data);

            // Selettori
            unsigned int ProgramId();
            StringStringMapVector ParameterList();

            void ProgramId(unsigned int);
            void ParameterList(const StringStringMapVector&);
    };
    std::ostream& operator<<(std::ostream&, const Program&);


    /*
     * (parser-message.cpp)
     *      - class Message
     *      - class MessageBody 
     */
    class Message {
        friend std::ostream& operator<<(std::ostream & os, const Message & m);

        protected:
            std::string version;
            Schema schema;
            Type type;
            std::string timestamp;
            
            Json::Value body;
        
        public:
            // Metodi statici
            static Type GetMessageType(const std::string&);
            
            // Costruttori
            Message();
            Message(const std::string&);

            // Selettori
            std::string Version() const;
            std::string Body() const;
            Schema GetSchema() const;
            Type GetType() const;
            std::string Timestamp() const;
            
            void Version(const std::string&);
            void Body(const std::string&);
            void SetSchema(const Schema&);
            void SetType(const Type&);
            void Timestamp(const std::string&);

            // Metodi
            bool IsHeaderParsed();
            void ParseHeader(const std::string&);
            std::string ToString() const;
    };
    std::ostream& operator<<(std::ostream& os, const Message& m);

    /* 
     * Classe astratta per il body dei messaggi
    */
    class MessageBody 
    {
        public:
            virtual ~MessageBody();
            virtual void Parse(const Json::Value&);
            virtual Json::Value ToJsonValue() const;
    };


    /*
     *  REQUESTS (parser-request.cpp)
     * 
     *  Classi:
     *      - SensorRequest
     *      - ActionRequest
     *      - CatalogueRequest: UNIMPLEMENTED
     *      - Request
     * 
     */
    class SensorRequest : MessageBody
    {
        private:
            unsigned int sensor_id;
            unsigned int samples;
            float frequency;
        
        public:
            // Costruttori
            SensorRequest();
            SensorRequest(const Json::Value& body);
            SensorRequest(const std::string& body);

            // Selettori
            unsigned int SensorId();
            unsigned int Samples();
            float Frequency();

            void SensorId(unsigned int);
            void Samples(unsigned int);
            void Frequency(float);

            // Metodi
            void Parse(const Json::Value&) override;
            Json::Value ToJsonValue() const override;
    };

    
    class ActionRequest : MessageBody
    {
        private:
            std::string subject;
            std::string function;
            ControlType control_type;
            Signal* signal;
            Program* program;
			float expiration;
			bool feedback;

        public:
            // Costruttori
            ActionRequest();
            ActionRequest(const Json::Value& body); 
            ActionRequest(const std::string& body); 

            // Selettori
            std::string Subject() const;
            std::string Function() const;
            ControlType GetControlType() const;
            Signal* GetSignal();
            Program* GetProgram();
            float Expiration() const;
            bool Feedback() const;

            void Subject(const std::string&);
            void Function(const std::string&);
            void SetControlType(const ControlType&);
            void SetSignal(Signal*);
            void SetProgram(Program*);
            void Expiration(float);
            void Feedback(bool);

            // Metodi
            void Parse(const Json::Value&) override;
            Json::Value ToJsonValue() const override;
    };


    class CatalogueRequest : MessageBody {
        // UNIMPLEMENTED
        public:
            CatalogueRequest();
    };

    
	class Request : public Message {
        private:
            SensorRequest* sensor;
            ActionRequest* action;
            CatalogueRequest* catalogue;

            void CleanParsing();
        
        public:
            // Operatori 
            Request& operator=(const Request&);

            // Costruttori, distruttori
            Request();
            Request(const std::string& message);
            Request(const Request& request);
            ~Request();
            
            // Selettori
            SensorRequest& GetSensorRequest();
            ActionRequest& GetActionRequest();
            CatalogueRequest& GetCatalogueRequest();
            void SetType(const Type&);      // TODO
            
            void SetSensorRequest(const SensorRequest&);
            void SetActionRequest(const ActionRequest&);
            void SetCatalogueRequest(const CatalogueRequest&);

            // Metodi
            bool IsBodyParsed();
            void ParseFromBody();
            void Parse(const std::string&);   
	};



    /*
     *  RESPONSE (parser-response.cpp)
     * 
     *  Classi:
     *      - ErrorResponse
     *      - ActionResponse
     *      - SensorResponse
     *      - CatalogueResponse: UNIMPLEMENTED
     *      - Response
     * 
     */
    class ErrorResponse : MessageBody {
        private:
            std::string code;
            std::string error_type;
            std::string at;
            std::string what;
        
        public:
            // Costruttori
            ErrorResponse();
            ErrorResponse(const Json::Value& body);
            ErrorResponse(const std::string& body);

            // Selettori
            std::string Code() const;   
            std::string ErrorType() const;   
            std::string At() const;     
            std::string What() const;   

            // Metodi set
            void Code(const std::string&); 
            void ErrorType(const std::string&);  
            void At(const std::string&); 
            void What(const std::string&);

            // Metodi
            void Parse(const Json::Value&) override;
            Json::Value ToJsonValue() const override;
    };


    class ActionResponse : MessageBody
    {
        private:
            std::string subject;
            std::string function;
            ControlType control_type;
            bool success;
            bool expired;
            std::string req_timestamp;

        public:
            // Costruttori
            ActionResponse();               
            ActionResponse(const Json::Value& body);  
            ActionResponse(const std::string& body);    

            // Selettori
            std::string Subject() const;        
            std::string Function() const;       
            ControlType GetControlType() const;    
            bool Success() const;                   
            bool Expired() const;                   
            std::string RequestTimestamp() const;   

            // Metodi set
            void Subject(std::string);
            void Function(const std::string&); 
            void SetControlType(ControlType); 
            void Success(bool); 
            void Expired(bool); 
            void RequestTimestamp(const std::string&);  

            // Metodi
            void Parse(const Json::Value&) override;
            Json::Value ToJsonValue() const override;
    };


    class SensorResponse : MessageBody 
    {
        private:
            unsigned int sensor_id;
            unsigned int samples;
            float frequency;
            std::vector<std::pair<std::string,std::vector<std::string>>> data;
        public:
            // Costruttori
            SensorResponse();
            SensorResponse(unsigned int sensor_id);               
            SensorResponse(const Json::Value& body);           
            
            // Selettori
            unsigned int SensorId() const;        
            unsigned int Samples() const;               
            float Frequency() const;        
            std::vector<std::pair<std::string,std::vector<std::string>>> Data() const;            

            void SensorId(unsigned int);    
            void Samples(unsigned int);     
            void Frequency(float); 
            void Data(const std::vector<std::pair<std::string,std::vector<std::string>>>&);

            // Metodi
            void Parse(const Json::Value&) override;
            Json::Value ToJsonValue() const override;
    };


    class CatalogueResponse {
        // UNIMPLEMENTED
        public:
            CatalogueResponse();
    };


	class Response : public Message {
		private:
            SensorResponse* sensor;
            ActionResponse* action;
            CatalogueResponse* catalogue;
            ErrorResponse* error;

            void CleanParsing();
        
        public:
            // Operatori
            Response& operator=(const Response&);   

            // Costruttori, distruttori
            Response();                     
            Response(const std::string& message);   
            Response(const Response& response);       
            ~Response();   

            // Selettori
            SensorResponse& GetSensorResponse();
            ActionResponse& GetActionResponse(); 
            CatalogueResponse& GetCatalogueResponse(); 
            ErrorResponse& GetErrorResponse();
            void SetType(const Type&);

            void SetSensorResponse(const SensorResponse&);    
            void SetActionResponse(const ActionResponse&);   
            void SetCatalogueResponse(const CatalogueResponse&);  
            void SetErrorResponse(const ErrorResponse&);  

            // Metodi
            bool IsBodyParsed();
            void ParseFromBody();  
            void Parse(const std::string&);
	};

}   // end namespace Parser

#endif