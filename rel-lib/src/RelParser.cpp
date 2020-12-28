/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "RelParser.h"

RelParser::RelParser(Logger &logger, FileEngine const &files) 
    : l(logger), rel_model(files), rs_parser(l), rd_parser(l, rs_parser) {}
RelParser::~RelParser() {}

unsigned int RelParser::ParseResultToInteger(ParseResult const p) {
    unsigned int ret = 1;

    if (p == ParseResult::ExceptionOccurred)
        ret = 1;
    else if (p== ParseResult::NoExceptionOccurred)
        ret = 0;

    return ret;
}

ParseResult RelParser::ProcessRelModel()
{
    l.LOG(LogLevel::INFO, "Starting to read REL model");
    std::vector<FileTokenData> input_files;
    ParseResult result;

    input_files = rel_model.GetListOfFiles();
    result = ReadAndLexInputFiles(input_files);

    if (result == ParseResult::NoExceptionOccurred) {
        result = ParseFiles(input_files);
    }

    return result;
}

std::vector<RdTypeInstance> RelParser::GetDatabase()
{
    return rd_parser.GetDatabase();
}

ParseResult RelParser::ReadAndLexInputFiles(std::vector<FileTokenData> &input_files) {
    ParseResult result = ParseResult::NoExceptionOccurred;

    for(unsigned int i=0; i<input_files.size(); i++) {
        try {
            l.LOG(LogLevel::INFO, "Reading file " + input_files[i].filepath + " and creating tokens");
            Lexer lex(l);
            lex.Read(input_files[i]);
        }
        catch (FileIoException &e) {
            std::string msg(e.what());
            std::string log_message = "Error while opening file ";
            log_message.append(e.GetFilePath());
            l.LOG(LogLevel::ERROR, log_message);
            l.LOG(LogLevel::ERROR, e.what());

            result = ParseResult::ExceptionOccurred;
        }
    }

    return result;
}

ParseResult RelParser::ParseFiles(std::vector<FileTokenData> &input_files) {
    ParseResult result = ParseResult::NoExceptionOccurred;

    try {
        // Parse the specifications and build up the data structures
        std::vector<FileTokenData>::iterator iter = input_files.begin();
        while( (iter = std::find_if(iter, input_files.end(), [](FileTokenData &d){return (d.GetDataTypeOfTokenList() == DataType::RequirementsSpecification);})) != input_files.end() ) {
            l.LOG(LogLevel::INFO, "Parsing tokens from " + iter->filepath);
            rs_parser.ParseTokens(*iter);
            iter++;
        }

        // Ensure, that all enum types used exist
        rs_parser.CheckAllEnumTypes();

        // Now parse the data and build up data structures
        iter = input_files.begin();
        while( (iter = std::find_if(iter, input_files.end(), [](FileTokenData &d){return (d.GetDataTypeOfTokenList() == DataType::RequirementsData);})) != input_files.end() ) {
            l.LOG(LogLevel::INFO, "Parsing of tokens from " + iter->filepath);
            rd_parser.ParseTokens(*iter);
            iter++;
        }

        // check all links
        rd_parser.CheckAllLinks();
    }
    catch(ParseException &e) {
        Token t  = e.GetToken();
        l.LOG(LogLevel::ERROR, t.GetFilename() + ": Line " + std::to_string(t.GetLineNumberOfToken()) + ", Pos " + std::to_string(t.GetPositionInLineOfToken()) + ":");
        l.LOG(LogLevel::ERROR, e.what());

        result = ParseResult::ExceptionOccurred;
    }

    ParsingStatistic s = rd_parser.GetParsingStatistics();
    l.LOG(LogLevel::INFO, "# files parsed: " + std::to_string(s.number_of_files));
    l.LOG(LogLevel::INFO, "# type instances parsed: " + std::to_string(s.number_of_type_instances));

    return result;
}