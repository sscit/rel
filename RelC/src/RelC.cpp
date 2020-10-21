//============================================================================
// Name        : RelC.cpp
// Author      : sscit.de
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>
#include <chrono>
#include "Lexer.h"
#include "FileEngine.h"
#include "ParseException.h"
#include "RsParser.h"
#include "RdParser.h"
#include "Logger.h"

using namespace std;

/* Singletons */
Logger *g_logger_object;

/* Method Definitions */
void ProcessCommandLine(int const argc, char const * const argv[], FileEngine &f, Logger &l)
{
    std::string root_dir(".");

    if(argc > 1)
    {
        for(int i=1; i<argc; ++i)
        {
            std::string argument(argv[i]);

            if(argument.compare("-r") == 0)
                f.SetSearchRecursive(true);
            else if(argument.compare("-v") == 0)
                l.SetLogLevel(LogLevel::INFO);
            else if(argument.compare("-vv") == 0)
                l.SetLogLevel(LogLevel::DEBUG);
            else
                f.SetStartDirectory(argument);
        }
    }
}

void ProcessInputFiles(Logger const &l, std::vector<FileTokenData> &input_files)
{
    for(unsigned int i=0; i<input_files.size(); i++)
    {
        l.LOG(LogLevel::INFO, "Reading file " + input_files[i].filepath + " and creating tokens");
        Lexer lex(l);
        lex.Read(input_files[i]);
    }
}

void ParseFiles(Logger const &l, std::vector<FileTokenData> &input_files)
{
    RsParser rs_parser(l);
    RdParser rd_parser(l, rs_parser);

    try
    {
        // Parse the specifications and build up the data structures
        std::vector<FileTokenData>::iterator iter = input_files.begin();
        while( (iter = std::find_if(iter, input_files.end(), [](FileTokenData &d){return (d.GetDataTypeOfTokenList() == DataType::RequirementsSpecification);})) != input_files.end() )
        {
            l.LOG(LogLevel::INFO, "Parsing tokens from " + iter->filepath);
            rs_parser.ParseTokens(*iter);
            iter++;
        }

        // Ensure, that all enum types used exist
        rs_parser.CheckAllEnumTypes();

        // Now parse the data and build up data structures
        iter = input_files.begin();
        while( (iter = std::find_if(iter, input_files.end(), [](FileTokenData &d){return (d.GetDataTypeOfTokenList() == DataType::RequirementsData);})) != input_files.end() )
        {
            l.LOG(LogLevel::INFO, "Parsing of tokens from " + iter->filepath);
            rd_parser.ParseTokens(*iter);
            iter++;
        }

        // check all links
        rd_parser.CheckAllLinks();
    }
    catch(ParseException &e)
    {
        Token t  = e.GetToken();
        l.LOG(LogLevel::ERROR, t.GetFilename() + ": Line " + std::to_string(t.GetLineNumberOfToken()) + ", Pos " + std::to_string(t.GetPositionInLineOfToken()) + ":");
        l.LOG(LogLevel::ERROR, e.what());
    }

    std::cout << "### summary ###" << std::endl;
    ParsingStatistic s = rd_parser.GetParsingStatistics();
    std::cout << "files: " << s.number_of_files << std::endl;
    std::cout << "type instances: " << s.number_of_type_instances << std::endl;
}

int main(int argc, char* argv[]) {

    auto t1 = std::chrono::high_resolution_clock::now();
    /* Initialize singletons */
    g_logger_object = new Logger();

    FileEngine input_file_handler;
    ProcessCommandLine(argc, argv, input_file_handler, *g_logger_object);

    std::vector<FileTokenData> input_files;
    input_files = input_file_handler.GetListOfFiles();
    ProcessInputFiles(*g_logger_object, input_files);

    ParseFiles(*g_logger_object, input_files);


    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << "duration " <<  duration;


    /* Cleanup singletons */
    delete g_logger_object;

    return 0;
}

/* code to measure runtime
auto t1 = std::chrono::high_resolution_clock::now();
auto t2 = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
std::cout << "duration " <<  duration;


    */
