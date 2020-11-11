/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef LEXER_H_
#define LEXER_H_

#include <map>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

#include "Token.h"
#include "SlidingWindow.h"
#include "Logger.h"
#include "FileReader.h"

enum class DataType {
    RequirementsSpecification,
    RequirementsData,
    Unknown
};

/* Class represents the input data, on the one
 * hand the data source that shall be read, on the other
 * hand as token list generated by the Lexer
 */
class FileTokenData {
public:
    FileTokenData(DataType);
    // Constructor to supply an own file reader
    FileTokenData(DataType, FileReader&);

    DataType GetDataTypeOfTokenList() const;
    FileReader &GetFileReader();

    std::string filepath;
    std::vector<Token> token_list;

private:
    DataType data_type;
    FileReader file_access;
};

class SlidingWindow;

class Lexer {
public:
    Lexer(Logger &);
    virtual ~Lexer();

    void Read(FileTokenData&);
    static void PrintTokenList(FileTokenData&);

protected:
    // returns true if string represents a signed integer, false otherwise
    bool IsInteger(std::string const&);
    // returns true if string represents an identifier, false otherwise
    bool IsIdentifier(std::string const&);
    // returns true if it is a string without end of line or other special chars
    bool IsString(std::string const&);
    void AddTokenToList(std::string const &s, TokenType const &tt);
    // returns true, if character is considered as delimiter
    static bool IsDelimiter(char const c);
    // returns true, if string represents line break
    static bool IsLinebreak(std::string const&);
    // returns true, if the characters in the data structure form an operator
    bool IsOperator(SlidingWindow const &);
    bool IsOperator(std::string const&);
    // returns true, if character is a whitespace
    static bool IsWhitespace(char const);
    // check string whether it contains a token
    void CheckStringandAddToken(std::string&, const char next_char = 0);

    std::map<std::string, TokenType> token_table;
    std::vector<Token> *token_list;
    Logger &l;

    std::string filename;
    unsigned int current_line;
    unsigned int current_position_in_line;
};

#endif /* LEXER_H_ */
