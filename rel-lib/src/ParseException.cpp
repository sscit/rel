/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "ParseException.h"

FileIoException::FileIoException(std::string const &fp, std::string const &em) : filepath(fp), error_message(em) { }

FileIoException::~FileIoException() { }

const char* FileIoException::what() const throw() {
    return error_message.c_str();
}

std::string FileIoException::GetFilePath() const {
    return filepath;
}


TypeNotFoundException::TypeNotFoundException() { }

TypeNotFoundException::~TypeNotFoundException() { }

const char* TypeNotFoundException::what() const throw() {
    return "type was not found in data structure";
}


ParseException::ParseException() { }

ParseException::ParseException(Token const t, std::string const msg = "") : token(t), custom_message(msg) { }

ParseException::~ParseException() { }

Token ParseException::GetToken() const {
    return token;
}


WrongTokenException::WrongTokenException(Token const t, std::string const msg) : ParseException(t, msg) { }

WrongTokenException::~WrongTokenException() { }

const char* WrongTokenException::what() const throw() {
    return custom_message.c_str();
}


CommentException::CommentException(Token const t, std::string const msg) : ParseException(t, msg) { }

CommentException::~CommentException() { }

const char* CommentException::what() const throw() {
    return custom_message.c_str();
}


RsEnumException::RsEnumException(Token const t, std::string const msg) : ParseException(t, msg) { }

RsEnumException::~RsEnumException() { }

const char* RsEnumException::what() const throw() {
    return custom_message.c_str();
}


RsTypeException::RsTypeException(Token const t, std::string const msg) : ParseException(t, msg) { }

RsTypeException::~RsTypeException() {}

const char* RsTypeException::what() const throw() {
    return custom_message.c_str();
}

EnumUsedButNotDefinedException::EnumUsedButNotDefinedException(Token const t, std::string const msg) 
    : ParseException(t, msg) { }

EnumUsedButNotDefinedException::~EnumUsedButNotDefinedException() {}

const char* EnumUsedButNotDefinedException::what() const throw() {
    return custom_message.c_str();
}


RdStringException::RdStringException(Token const t, std::string const msg) : ParseException(t, msg) { }

RdStringException::~RdStringException() { }

const char* RdStringException::what() const throw() {
    return custom_message.c_str();
}


RdTypeException::RdTypeException(Token const t, std::string const msg) : ParseException(t, msg) { }

RdTypeException::RdTypeException(RdTypeInstance const t) : ParseException(), rd_type(t) { }

RdTypeException::~RdTypeException() { }

const char* RdTypeException::what() const throw() {
    return custom_message.c_str();
}

