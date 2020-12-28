/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef PARSEEXCEPTION_H_
#define PARSEEXCEPTION_H_

#include <string>
#include <exception>
#include "Token.h"
#include "AST.h"


class FileIoException : public std::exception {
public:
    FileIoException(std::string const&, std::string const&);
    virtual ~FileIoException();

    virtual const char* what() const throw();
    std::string GetFilePath() const;

protected:
    std::string const filepath;
    std::string const error_message;
};


class TypeNotFoundException : public std::exception {
public:
    TypeNotFoundException();
    virtual ~TypeNotFoundException();

    virtual const char* what() const throw();
};

class ParseException : public std::exception {
public:
    ParseException();
    ParseException(Token const, std::string const);
    virtual ~ParseException();

    Token GetToken() const;

protected:
    Token const token;
    std::string const custom_message;
};

class WrongTokenException : public ParseException {
public:
    WrongTokenException(Token const, std::string const msg = "");
    ~WrongTokenException();

    virtual const char* what() const throw();
};

class CommentException : public ParseException {
public:
    CommentException(Token const, std::string const msg = "");
    ~CommentException();

    virtual const char* what() const throw();
};

class RsEnumException : public ParseException {
public:
    RsEnumException(Token const, std::string const msg = "");
    ~RsEnumException();

    virtual const char* what() const throw();
};

class RsTypeException : public ParseException {
public:
    RsTypeException(Token const, std::string const msg = "");
    ~RsTypeException();

    virtual const char* what() const throw();
};

class EnumUsedButNotDefinedException : public ParseException {
public:
    EnumUsedButNotDefinedException(Token const, std::string const msg = "");
    ~EnumUsedButNotDefinedException();

    virtual const char* what() const throw();
};

class RdStringException : public ParseException {
public:
    RdStringException(Token const, std::string const msg = "");
    ~RdStringException();

    virtual const char* what() const throw();
};

class RdTypeException : public ParseException {
public:
    RdTypeException(RdTypeInstance const);
    RdTypeException(Token const, std::string const msg = "");
    ~RdTypeException();

    virtual const char* what() const throw();

private:
    RdTypeInstance const rd_type;
};



#endif /* PARSEEXCEPTION_H_ */
