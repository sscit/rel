/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "RelParser.h"

RelParser::RelParser(Logger &logger, FileEngine const &files) 
    : l(logger), rel_model(files), rs_parser(l), rd_parser(l, rs_parser) {}
RelParser::~RelParser() {}

void RelParser::ProcessRelModel()
{
    l.LOG(LogLevel::INFO, "Starting to read REL model");
    std::vector<FileTokenData> input_files;
    input_files = rel_model.GetListOfFiles();

    ReadAndLexInputFiles(input_files);

    ParseFiles(input_files);
}

std::vector<RdTypeInstance> RelParser::GetDatabase()
{
    return rd_parser.GetDatabase();
}

void RelParser::ReadAndLexInputFiles(std::vector<FileTokenData> &input_files) {
    for(unsigned int i=0; i<input_files.size(); i++) {
        l.LOG(LogLevel::INFO, "Reading file " + input_files[i].filepath + " and creating tokens");
        Lexer lex(l);
        lex.Read(input_files[i]);
    }
}

void RelParser::ParseFiles(std::vector<FileTokenData> &input_files) {
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
    }

    ParsingStatistic s = rd_parser.GetParsingStatistics();
    l.LOG(LogLevel::INFO, "# files parsed: " + std::to_string(s.number_of_files));
    l.LOG(LogLevel::INFO, "# type instances parsed: " + std::to_string(s.number_of_type_instances));
}