/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "RelParser.h"
#include <thread>

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
        result = ReadAndParseDataFiles(input_files);
    }
    catch (FileIoException &e) {
        result = LogFileIoException(e);
    }
    catch(ParseException &e) {
        result = LogParseException(e);
    }

    return result;
}

ParseResult RelParser::LogFileIoException(FileIoException &e) {
    std::string msg(e.what());
    std::string log_message = "Error while opening file ";
    log_message.append(e.GetFilePath());
    l.LOG(LogLevel::ERROR, log_message);
    l.LOG(LogLevel::ERROR, e.what());

    return ParseResult::ExceptionOccurred;
}

ParseResult RelParser::LogParseException(ParseException &e) {
    Token t  = e.GetToken();
    l.LOG(LogLevel::ERROR, *t.GetFilename() + ": Line " + std::to_string(t.GetLineNumberOfToken()) + ", Pos " + std::to_string(t.GetPositionInLineOfToken()) + ":");
    l.LOG(LogLevel::ERROR, e.what());

    return ParseResult::ExceptionOccurred;
}

void RelParser::LexFile(FileTokenData& file) {
    l.LOG(LogLevel::DBUG, "Reading file " + file.filepath + " and creating tokens");
    Lexer lex(l);
    lex.Read(file);
}

ParseResult RelParser::ReadAndParseDataFiles(std::vector<FileTokenData> &input_files) {
    l.LOG(LogLevel::INFO, "Processing data files...");
    /* Define the function that is spawned into a thread. There will be N
       threads, one for every data file. To report back if there are exceptions during
       parsing, an exception pointer is provided. */
    std::vector<std::exception_ptr> eps;
    auto thread_main = [&](FileTokenData * const input, unsigned int const file_index) {
        try {
            LexFile(*input);
            l.LOG(LogLevel::DBUG, "Parsing of tokens from " + input->filepath);
            rd_parser.ParseTokens(*input, file_index);
            // delete the tokens, they are not needed anymore
            input->token_list.clear();
        }
        catch(FileIoException &e) {
            eps.push_back(std::current_exception());
        }
        catch(ParseException &e) {
            eps.push_back(std::current_exception());
        }
    };

    std::vector<std::thread*> thread_list;
    std::vector<FileTokenData>::iterator iter = input_files.begin();
    unsigned int file_index = 1;
    ParseResult result = ParseResult::NoExceptionOccurred;

    while( (iter = std::find_if(iter, input_files.end(), [](FileTokenData &d){return (d.GetDataTypeOfTokenList() == DataType::RequirementsData);})) != input_files.end() ) {
        /* Provide the index of the file, to ensure that the resulting type instances are stored in the same
           order in the data base as the files have been got out of the sorted input files data structure */
        std::thread *th = new std::thread(thread_main, &(*iter), file_index);
        thread_list.push_back(th);
        iter++;
        file_index++;
    }

    for(auto t : thread_list) {
		t->join();
	}

    /* Check if there was an exception during data parsing. if yes, just output the first one */
    if(eps.size() > 0) {
        try {
            std::rethrow_exception(eps[0]);
        }
        catch (FileIoException &e) {
            result = LogFileIoException(e);
        }
        catch (ParseException &e) {
            result = LogParseException(e);
        }
    }

    for(auto t : thread_list) {
       delete t;
    }

    // check all links
    rd_parser.CheckAllLinks();

    const ParsingStatistic &s = rd_parser.GetParsingStatistics();
    l.LOG(LogLevel::INFO, "# files parsed: " + std::to_string(s.GetNrOfFiles()));
    l.LOG(LogLevel::INFO, "# type instances parsed: " + std::to_string(s.GetNrOfTypeInstances()));

    return result;
}

void RelParser::ReadAndParseSpecFiles(std::vector<FileTokenData> &input_files) {
    /* Lex and parse the specification files, to build up the spec */
    std::vector<FileTokenData>::iterator iter = input_files.begin();
    l.LOG(LogLevel::INFO, "Processing specification files...");
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

std::list<RdFile> RelParser::GetDatabase()
{
    return rd_parser.GetDatabase();
}