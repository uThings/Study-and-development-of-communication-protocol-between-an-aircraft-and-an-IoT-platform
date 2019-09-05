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
#include <chrono>
#include <cmath>

#include "json.h"
#include "Parser.hpp"



double TestActionRequest();
double TestSensorRequest();
double TestActionResponse();
double TestSensorResponse();
double TestErrorResponse();


int main(int argc, char **argv) {
    using namespace std;
    unsigned int N = 10000;
    double sum;
    vector<pair<string, double(*)()>> fn_v;
    
    fn_v.push_back(make_pair("action request", TestActionRequest));
    fn_v.push_back(make_pair("sensor request", TestSensorRequest));
    fn_v.push_back(make_pair("action response", TestActionResponse));
    fn_v.push_back(make_pair("error response", TestErrorResponse));
    fn_v.push_back(make_pair("sensor response 1000 double samples", TestSensorResponse));
    
    for (unsigned j = 0; j < fn_v.size(); j++) {
        cerr<<"Test "<<fn_v.at(j).first<<" - tempo medio su "<<N<<"prove:    # ";
        sum = 0;
        for (unsigned int i = 0U; i < N; i++)
        {
            std::cerr << i;
            sum += (*fn_v.at(j).second)();
            for (unsigned k = 0; k < ((unsigned)log10(i)) + 1U; k++)
                std::cerr << '\b';
        }
        cerr << '\b' << '\b' << sum / N << "microseconds" << std::endl;
    }
}
















double TestSensorRequest()
{   
    using namespace std::chrono;
    high_resolution_clock::time_point t_start, t_end;

    t_start = high_resolution_clock::now();
    ////////////////////////////////////////////////////////////////////////////////

    Parser::Request request;

    Parser::SensorRequest sensor_request;
    sensor_request.Frequency(1);
    sensor_request.Samples(10);
    sensor_request.SensorId(0); 
    request.SetSensorRequest(sensor_request);

    std::string str(request.ToString());

    ////////////////////////////////////////////////////////////////////////////////
    t_end = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t_end - t_start);
    
    return time_span.count()*1e6;
}

double TestActionRequest()
{   
    using namespace std::chrono;
    high_resolution_clock::time_point t_start, t_end;

    t_start = high_resolution_clock::now();
    ////////////////////////////////////////////////////////////////////////////////

    Parser::Request request;
    Parser::ActionRequest action_request;
    Parser::Program* program;
    Parser::StringStringMapVector parameters;

    action_request.Subject("led");
    action_request.Function("fixed-light");
    action_request.SetControlType(Parser::ControlType::Program);
    action_request.Expiration(0.0);
    action_request.Feedback(true);

    program = new Parser::Program(1);
    parameters.push_back(std::make_pair("state", "true"));
    program->ParameterList(parameters);

    action_request.SetProgram(program);

    request.SetActionRequest(action_request);

    std::string str(request.ToString());

    ////////////////////////////////////////////////////////////////////////////////
    t_end = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t_end - t_start);
    
    return time_span.count()*1e6;
}


double TestActionResponse()
{
    using namespace std::chrono;
    high_resolution_clock::time_point t_start, t_end;

    std::string subject = "ledled";
    std::string function = "lightlight";
    std::string ts = "12-34-5678/90:12:34";
    Parser::ControlType ct(Parser::ControlType::Program);

    t_start = high_resolution_clock::now();
    ////////////////////////////////////////////////////////////////////////////////
    Parser::Response response;
    Parser::ActionResponse action_response;

    action_response.Subject(subject);
    action_response.Function(function);
    action_response.SetControlType(ct);
    action_response.Expired(false);                        
    action_response.Success(true);
    action_response.RequestTimestamp(ts);                        
    response.SetActionResponse(action_response);

    std::string str(response.ToString());
    

    ////////////////////////////////////////////////////////////////////////////////
    t_end = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t_end - t_start);
    
    return time_span.count()*1e6;
}


double TestSensorResponse()
{
    using namespace std::chrono;
    high_resolution_clock::time_point t_start, t_end;

    unsigned int samples = 1000;
    float freq = 1e-1;
    std::vector<double> data_vec;
    for (unsigned int k = 0; k < samples; k++)
        data_vec.push_back(1234.12341234 * rand());


    t_start = high_resolution_clock::now();
    ////////////////////////////////////////////////////////////////////////////////
    Parser::Response response;
    Parser::SensorResponse sensor_response;
    Parser::SensorData data;
    std::vector<std::string> vec;

    sensor_response.Samples(samples);    
    sensor_response.Frequency(freq);                   
    sensor_response.SensorId(samples);

    for (unsigned int i = 0; i < samples; i++)
    {
        vec.push_back(std::to_string(data_vec.at(i)));
    }

    data.push_back(std::make_pair("test", vec));
    sensor_response.Data(data);

    response.SetSensorResponse(sensor_response);

    std::string str(response.ToString());

    ////////////////////////////////////////////////////////////////////////////////
    t_end = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t_end - t_start);
    
    return time_span.count()*1e6;
}

double TestErrorResponse()
{
    using namespace std::chrono;
    high_resolution_clock::time_point t_start, t_end;

    std::string code = "errorcodeerrorcode";
    std::string error_type = "typetypetype";
    std::string what("whatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhatwhat");
    std::string at = "double TestErrorResponse()double TestErrorResponse()double TestErrorResponse()";

    t_start = high_resolution_clock::now();
    ////////////////////////////////////////////////////////////////////////////////
    Parser::Response response;
    Parser::ErrorResponse error_response;

    error_response.Code(code);
    error_response.ErrorType(error_type);
    error_response.What(what);
    error_response.At(at);
    response.SetErrorResponse(error_response);

    std::string str(response.ToString());
    
    ////////////////////////////////////////////////////////////////////////////////
    t_end = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t_end - t_start);
    
    return time_span.count()*1e6;
}

double TestSkeleton()
{   
    using namespace std::chrono;
    high_resolution_clock::time_point t_start, t_end;
    t_start = high_resolution_clock::now();

    // Code to bench goes here

    t_end = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t_end - t_start);
    return time_span.count()*1e6;
}