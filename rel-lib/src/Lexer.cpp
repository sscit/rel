/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include <string>
#include <cmath>
#include "Lexer.h"


// ############ Implementation of class FileTokenData
FileTokenData::FileTokenData(DataType d) {
    data_type = d;
}

FileTokenData::FileTokenData(DataType d, FileReader &fr) : file_access(fr) {
    data_type = d;
}

DataType FileTokenData::GetDataTypeOfTokenList() const {
    return data_type;
}

FileReader& FileTokenData::GetFileReader() {
    return file_access;
}


// ############ Implementation of class Lexer
Lexer::Lexer(Logger &logger) : l(logger)  {
    // setup the map with known tokens
    token_table["type"] = TokenType::TYPE;
    token_table["enum"] = TokenType::ENUM;
    token_table["id"] = TokenType::ID;
    token_table["int"] = TokenType::INT;
    token_table["string"] = TokenType::STRING;
    token_table["link"] = TokenType::LINK;

    token_table["//"] = TokenType::LINE_COMMENT;
    token_table["/*"] = TokenType::COMMENT_BLOCK_START;
    token_table["*/"] = TokenType::COMMENT_BLOCK_END;

    token_table["{"] = TokenType::BRACKET_OPEN;
    token_table["}"] = TokenType::BRACKET_CLOSE;
    token_table[":"] = TokenType::COLON;
    token_table[","] = TokenType::COMMA;
    token_table["\""] = TokenType::QUOTATION_MARK;
    token_table["\\\""] = TokenType::QUOTATION_MARK_MASKED;
    token_table["\n"] = TokenType::END_OF_LINE;
    token_table["\r\n"] = TokenType::END_OF_LINE;

    current_line = 0;
    current_position_in_line = 0;
}

void Lexer::PrintTokenList(FileTokenData& d) {
    std::cout << "Nr of Tokens: " << d.token_list.size() << std::endl;

    for(unsigned int i=0; i<d.token_list.size(); ++i) {
        Token const &x = d.token_list[i];

        std::cout << "Token Type: " << x.GetTokenType() << ",\t string value: " << x.GetTokenValue() << std::endl;
    }
}

bool Lexer::IsInteger(std::string const &s) {
    int offset = 0;

    if(s.size() == 0)
        return false;

    if(s[0] == '-' && s.size() > 1)
        offset=1;

    return std::find_if(s.begin()+offset, s.end(),
                          [](unsigned char c) {
                            return !std::isdigit(c);
                        }) == s.end();
}

bool Lexer::IsIdentifier(std::string const &s) {
    unsigned char first_character = s[0];
    if( !(std::isalpha(first_character) != 0 || first_character == '_') )
        return false;

    return std::find_if(s.begin()+1, s.end(),
                        [](unsigned char c) {
                            return (!std::isalnum(c) && !(c == '_')) ;
                        }) == s.end();
}

bool Lexer::IsString(std::string const &s) {
    if(s.size() == 0)
        return false;

    return std::find_if(s.begin(), s.end(),
                        [](unsigned char c) {
                            return (IsDelimiter(c)) ;
                        }) == s.end();
}

void Lexer::AddTokenToList(std::string const &s, TokenType const &tt) {
    int current_position = current_position_in_line - s.size();
    current_position = std::abs(current_position);

    Token t(s, tt, filename, current_line, current_position);
    token_list->push_back(t); 

    std::string logmessage = "Token detected and stored: ";
    logmessage.append(Token::TokenTypeToString(tt));
    logmessage.append(" Line: " + std::to_string(t.GetLineNumberOfToken()) + " Pos: " + std::to_string(t.GetPositionInLineOfToken()));
    l.LOG(LogLevel::DBUG, logmessage);
}

bool Lexer::IsDelimiter(char const c) {
    if(c==' ' || c=='\r' || c=='\n')
        return true;

    else return false;
}

bool Lexer::IsOperator(std::string const& s) const {
    if(token_table.count(s) == 1)
        return true;
    else return false;
}

bool Lexer::IsOperator(SlidingWindow const& l) const {
    char buf[2];
    buf[0] = l.front();

    if(buf[0] == '{' || buf[0] == '}' || buf[0] == ':' || buf[0] == ',' || buf[0] == '\"')
        return true;

    buf[0] = l.front();
    buf[1] = l.back();

    if( (buf[0] == '/' && buf[1] == '/') ||
        (buf[0] == '/' && buf[1] == '*') ||
        (buf[0] == '*' && buf[1] == '/') ||
        (buf[0] == '\\' && buf[1] == '\"') )
        return true;

    return false;
}

bool Lexer::IsWhitespace(char const c) {
    if(c == ' ' || c == '\t')
        return true;

    else return false;
}

bool Lexer::IsLinebreak(const std::string& s) {
    if(s.size() == 1) {
        if(s.compare("\n") == 0)
            return true;
    }

    if(s.size() == 2) {
        if(s.compare("\r\n") == 0)
            return true;
    }

    return false;
}

void Lexer::CheckStringandAddToken(std::string &current_str, const char next_char) {
    if(token_table.count(current_str) == 1) {
        /* to avoid that a keyword is falsely identified a the beginning
         * of a string or identifier, e.g. *id*entifier, check the next character, too */
        if(current_str.compare( Token::TokenTypeToString(TokenType::ENUM).c_str() ) == 0) {
            if(!std::isalnum(next_char)) {
                AddTokenToList(current_str, token_table[current_str]);
                current_str.clear();
            }
        }
        else if(current_str.compare( Token::TokenTypeToString(TokenType::ID).c_str() ) == 0) {
            if(!std::isalnum(next_char)) {
                AddTokenToList(current_str, token_table[current_str]);
                current_str.clear();
            }
        }
        else if(current_str.compare( Token::TokenTypeToString(TokenType::INT).c_str() ) == 0) {
            if(!std::isalnum(next_char)) {
                AddTokenToList(current_str, token_table[current_str]);
                current_str.clear();
            }
        }
        else if(current_str.compare( Token::TokenTypeToString(TokenType::STRING).c_str() ) == 0) {
            if(!std::isalnum(next_char)) {
                AddTokenToList(current_str, token_table[current_str]);
                current_str.clear();
            }
        }
        else if(current_str.compare( Token::TokenTypeToString(TokenType::LINK).c_str() ) == 0) {
            if(!std::isalnum(next_char)) {
                AddTokenToList(current_str, token_table[current_str]);
                current_str.clear();
            }
        }
        else if(current_str.compare( Token::TokenTypeToString(TokenType::TYPE).c_str() ) == 0) {
            if(!std::isalnum(next_char)) {
                AddTokenToList(current_str, token_table[current_str]);
                current_str.clear();
            }
        }
        else {
            AddTokenToList(current_str, token_table[current_str]);
            current_str.clear();
        }
    }
    else if(current_str.compare("\\") == 0 && next_char == '\"') {
        /* quotation mark is masked, therefore do nothing here and
        wait until the next char is properly added to current_str */
    }
    else if(IsInteger(current_str)) {
        AddTokenToList(current_str, TokenType::INTEGER_VALUE);
        current_str.clear();
    }
    else if(IsIdentifier(current_str)) {
        AddTokenToList(current_str, TokenType::IDENTIFIER);
        current_str.clear();
    }
    else if(IsString(current_str)) {
        AddTokenToList(current_str, TokenType::STRING_VALUE);
        current_str.clear();
    }
}

void Lexer::Read(FileTokenData& data) {
    FileReader &file_reader = data.GetFileReader();

    file_reader.OpenFile(data.filepath.c_str());

    token_list = &data.token_list;
    filename = data.filepath;
    // Line and current position start from 0 index
    current_line = 0;
    current_position_in_line = 0;

    if(file_reader.IsFileOpen()) {
        std::string current_str;
        current_str.clear();
        // window moving across the input file, always keeps two chars
        SlidingWindow sliding_window;
        char c;

        // read first char in advance
        if(file_reader.GetChar(c)) {
            current_position_in_line++;
            sliding_window.push_back(c);
        }

        while(!sliding_window.empty()) {
            if(file_reader.GetChar(c)) {
                current_position_in_line++;
                sliding_window.push_back(c);
            }

            // discard whitespaces
            if(current_str.size()==0 && IsWhitespace(sliding_window.front())) {
                sliding_window.pop_front();
                continue;
            }

            // identify line break directly
            if(IsLinebreak(current_str)) {
                current_line++;
                current_position_in_line=1;
                AddTokenToList(current_str, token_table[current_str]);
                current_str.clear();
            }
            else if(IsDelimiter(sliding_window.front()) ||
                    IsOperator(sliding_window) ||
                    IsOperator(current_str)) {
                CheckStringandAddToken(current_str, sliding_window.front());
            }

            if(!IsWhitespace(sliding_window.front()))
                current_str.append(1, sliding_window.front());

            sliding_window.pop_front();
        }

        CheckStringandAddToken(current_str);
    }

    file_reader.Close();
}

Lexer::~Lexer() { }
