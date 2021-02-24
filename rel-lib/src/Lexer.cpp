/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include <string>
#include <cmath>
#include "Lexer.h"
#include "ParseException.h"

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

void FileTokenData::SetFileReader(FileReader& fr) {
    file_access = fr;
}

// ############ Implementation of class PositionInFile
PositionInFile::PositionInFile() { 
    Reset(); 
}

void PositionInFile::Reset() {
     current_line = 0;
     current_position_in_line = 0;
     last_token_start = -1;
}

void PositionInFile::Newline() {
     current_line++;
     current_position_in_line = 0;
     last_token_start = -1;
}

void PositionInFile::CountChar(char const c) {   
    if( (c != ' ' && last_token_start == -1) )
        last_token_start = current_position_in_line;
    current_position_in_line++;
}

// ############ Implementation of class Lexer
Lexer::Lexer(Logger &logger) : l(logger)  {
    // setup the map with known tokens
    keyword_table["type"] = TokenType::TYPE;
    keyword_table["enum"] = TokenType::ENUM;
    keyword_table["id"] = TokenType::ID;
    keyword_table["int"] = TokenType::INT;
    keyword_table["string"] = TokenType::STRING;
    keyword_table["link"] = TokenType::LINK;

    operator_table["//"] = TokenType::LINE_COMMENT;
    operator_table["/*"] = TokenType::COMMENT_BLOCK_START;
    operator_table["*/"] = TokenType::COMMENT_BLOCK_END;

    operator_table["{"] = TokenType::BRACKET_OPEN;
    operator_table["}"] = TokenType::BRACKET_CLOSE;
    operator_table[":"] = TokenType::COLON;
    operator_table[","] = TokenType::COMMA;
    operator_table["\""] = TokenType::QUOTATION_MARK;
    operator_table["\\\""] = TokenType::QUOTATION_MARK_MASKED;
    operator_table["\n"] = TokenType::END_OF_LINE;
    operator_table["\r\n"] = TokenType::END_OF_LINE;
    operator_table["["] = TokenType::ARRAY_BEGIN;
    operator_table["]"] = TokenType::ARRAY_END;

    pos_counter.Reset();
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
    l.LOG(LogLevel::DBUG, std::to_string(pos_counter.current_position_in_line) + " " + std::to_string(s.size()) + " " + std::to_string(pos_counter.last_token_start));

    Token t(s, tt, *filename, pos_counter.current_line, pos_counter.last_token_start);
    token_list->push_back(t); 
    pos_counter.last_token_start = -1;

    if(l.GetCurrentLogLevel() == LogLevel::DBUG) {
        std::string logmessage = "Token detected and stored: ";
        logmessage.append(Token::TokenTypeToString(tt));
        logmessage.append(" Line: " + std::to_string(t.GetLineNumberOfToken()) + " Pos: " + std::to_string(t.GetPositionInLineOfToken()));
        l.LOG(LogLevel::DBUG, logmessage);
    }
}

bool Lexer::IsDelimiter(char const c) {
    if(c==' ' || c=='\r' || c=='\n')
        return true;

    else return false;
}

bool Lexer::IsOperatorOrKeyword(std::string const& s) const {
    bool ret = false;

    // First check for keywords, then the operators
    // the longest keyword is "string" with 6 chars. do not compare longer strings
    if(s.size() <= 6 && keyword_table.count(s) == 1)
        ret = true;
    else if(s.size() <= 2 && operator_table.count(s) == 1)
        ret = true;

    return ret;
}

TokenType Lexer::GetTokenTypeOfOperatorOrKeyword(std::string const& s) {
    TokenType ret = TokenType::TYPE;

    if(keyword_table.count(s) == 1)
        ret = keyword_table[s];
    else if(operator_table.count(s) == 1)
        ret = operator_table[s];

    return ret;
}

bool Lexer::IsOperator(SlidingWindow const& l) const {
    char buf[2];
    buf[0] = l.front().c;

    if(buf[0] == '{' || buf[0] == '}' || buf[0] == ':' || buf[0] == ',' || buf[0] == '\"')
        return true;

    buf[0] = l.front().c;
    buf[1] = l.back().c;

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

void Lexer::CheckStringandAddToken(std::string &current_str, char const next_char) {
    if( IsOperatorOrKeyword(current_str) ) {
        /* to avoid that a keyword is falsely identified a the beginning
         * of a string or identifier, e.g. *id*entifier, check the next character, too */
        if(current_str.compare( Token::TokenTypeToString(TokenType::ENUM).c_str() ) == 0) {
            if(!std::isalnum(next_char)) {
                AddTokenToList(current_str, TokenType::ENUM);
                current_str.clear();
            }
        }
        else if(current_str.compare( Token::TokenTypeToString(TokenType::ID).c_str() ) == 0) {
            if(!std::isalnum(next_char)) {
                AddTokenToList(current_str, TokenType::ID);
                current_str.clear();
            }
        }
        else if(current_str.compare( Token::TokenTypeToString(TokenType::INT).c_str() ) == 0) {
            if(!std::isalnum(next_char)) {
                AddTokenToList(current_str, TokenType::INT);
                current_str.clear();
            }
        }
        else if(current_str.compare( Token::TokenTypeToString(TokenType::STRING).c_str() ) == 0) {
            if(!std::isalnum(next_char)) {
                AddTokenToList(current_str, TokenType::STRING);
                current_str.clear();
            }
        }
        else if(current_str.compare( Token::TokenTypeToString(TokenType::LINK).c_str() ) == 0) {
            if(!std::isalnum(next_char)) {
                AddTokenToList(current_str, TokenType::LINK);
                current_str.clear();
            }
        }
        else if(current_str.compare( Token::TokenTypeToString(TokenType::TYPE).c_str() ) == 0) {
            if(!std::isalnum(next_char)) {
                AddTokenToList(current_str, TokenType::TYPE);
                current_str.clear();
            }
        }
        else {
            AddTokenToList(current_str, GetTokenTypeOfOperatorOrKeyword(current_str));
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
    filename = &data.filepath;
    // Line and current position start from 0 index
    pos_counter.Reset();

    if(file_reader.IsFileOpen()) {
        std::string current_str;
        current_str.clear();
        // window moving across the input file, always keeps two chars
        SlidingWindow sliding_window;
        char c;
        unsigned int current_line = 0;
        unsigned int position_in_line = 0;

        // read first char in advance
        if(file_reader.GetChar(c)) {
            CharAndPos x;
            // store the exact location of the char together with the character
            x.c = c; x.pos = position_in_line;
            sliding_window.push_back(x);
            position_in_line++;
        }

        while(!sliding_window.empty()) {
            if(file_reader.GetChar(c)) {
                CharAndPos x;
                x.c = c; x.pos = position_in_line;
                sliding_window.push_back(x);
                position_in_line++;
            }

            // identify line break directly
            if(IsLinebreak(current_str)) {
                AddTokenToList(current_str, operator_table[current_str]);
                pos_counter.Newline();
                position_in_line=0;
                current_line++;
                current_str.clear();
            }
            else if(IsDelimiter(sliding_window.front().c) ||
                    IsOperator(sliding_window) ||
                    IsOperatorOrKeyword(current_str)) {
                CheckStringandAddToken(current_str, sliding_window.front().c);
            }

            // consider the char in the position counter only when it is used
            pos_counter.CountChar(sliding_window.front().c);
            if(!IsWhitespace(sliding_window.front().c)) {
                current_str.append(1, sliding_window.front().c);   
            }

            sliding_window.pop_front();
        }

        CheckStringandAddToken(current_str);
    }
    else {
        std::string error_msg(strerror(errno));
        throw FileIoException(data.filepath, error_msg);
    }

    file_reader.Close();
}

Lexer::~Lexer() { }
