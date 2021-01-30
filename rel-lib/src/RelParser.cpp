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
    ParseResult result = ParseResult::NoExceptionOccurred;

    input_files = rel_model.GetListOfFiles();

    try {
        ReadAndParseSpecFiles(input_files);
        ReadAndParseDataFiles(input_files);
    }
    catch (FileIoException &e) {
        std::string msg(e.what());
        std::string log_message = "Error while opening file ";
        log_message.append(e.GetFilePath());
        l.LOG(LogLevel::ERROR, log_message);
        l.LOG(LogLevel::ERROR, e.what());

        result = ParseResult::ExceptionOccurred;
    }
    catch(ParseException &e) {
        Token t  = e.GetToken();
        l.LOG(LogLevel::ERROR, *t.GetFilename() + ": Line " + std::to_string(t.GetLineNumberOfToken()) + ", Pos " + std::to_string(t.GetPositionInLineOfToken()) + ":");
        l.LOG(LogLevel::ERROR, e.what());

        result = ParseResult::ExceptionOccurred;
    }

    return result;
}

void RelParser::LexFile(FileTokenData& file) {
    l.LOG(LogLevel::INFO, "Reading file " + file.filepath + " and creating tokens");
    Lexer lex(l);
    lex.Read(file);
}

void RelParser::ReadAndParseDataFiles(std::vector<FileTokenData> &input_files) {
    std::vector<FileTokenData>::iterator iter = input_files.begin();
    while( (iter = std::find_if(iter, input_files.end(), [](FileTokenData &d){return (d.GetDataTypeOfTokenList() == DataType::RequirementsData);})) != input_files.end() ) {
        LexFile(*iter);

        l.LOG(LogLevel::INFO, "Parsing of tokens from " + iter->filepath);
        rd_parser.ParseTokens(*iter);
        // delete the tokens, they are not needed anymore
        iter->token_list.clear();
        iter++;
    }

    // check all links
    rd_parser.CheckAllLinks();

    ParsingStatistic s = rd_parser.GetParsingStatistics();
    l.LOG(LogLevel::INFO, "# files parsed: " + std::to_string(s.number_of_files));
    l.LOG(LogLevel::INFO, "# type instances parsed: " + std::to_string(s.number_of_type_instances));
}

void RelParser::ReadAndParseSpecFiles(std::vector<FileTokenData> &input_files) {
    /* Lex and parse the specification files, to build up the spec */
    std::vector<FileTokenData>::iterator iter = input_files.begin();
    while( (iter = std::find_if(iter, input_files.end(), [](FileTokenData &d){return (d.GetDataTypeOfTokenList() == DataType::RequirementsSpecification);})) != input_files.end() ) {
        LexFile(*iter);

        l.LOG(LogLevel::INFO, "Parsing tokens from " + iter->filepath);
        rs_parser.ParseTokens(*iter);
        // delete the tokens, they are not needed anymore
        iter->token_list.clear();
        iter++;
    }

    // Ensure, that all enum types used exist
    rs_parser.CheckAllEnumTypes();
}

std::vector<RdFile> RelParser::GetDatabase()
{
    return rd_parser.GetDatabase();
}