/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#ifndef PARSEEXCEPTION_H_
#define PARSEEXCEPTION_H_

#include <exception>
#include <string>

#include "AST.h"
#include "Token.h"

class FileIoException : public std::exception {
   public:
    FileIoException(std::string const&, std::string const&);
    virtual ~FileIoException();

    /* Req: parser1 */
    virtual const char* what() const throw();
    /* Req: parser1 */
    std::string GetFilePath() const;

   protected:
    std::string const filepath;
    std::string const error_message;
};

class TypeNotFoundException : public std::exception {
   public:
    TypeNotFoundException();
    virtual ~TypeNotFoundException();

    /* Req: parser1 */
    virtual const char* what() const throw();
};

class ParseException : public std::exception {
   public:
    ParseException();
    ParseException(Token const, std::string const);
    virtual ~ParseException();

    /* Req: parser1 */
    Token GetToken() const;

   protected:
    Token const token;
    std::string const custom_message;
};

class WrongTokenException : public ParseException {
   public:
    WrongTokenException(Token const, std::string const msg = "");
    ~WrongTokenException();

    /* Req: parser1 */
    virtual const char* what() const throw();
};

class CommentException : public ParseException {
   public:
    CommentException(Token const, std::string const msg = "");
    ~CommentException();

    /* Req: parser1 */
    virtual const char* what() const throw();
};

class RsEnumException : public ParseException {
   public:
    RsEnumException(Token const, std::string const msg = "");
    ~RsEnumException();

    /* Req: parser1 */
    virtual const char* what() const throw();
};

class RsTypeException : public ParseException {
   public:
    RsTypeException(Token const, std::string const msg = "");
    ~RsTypeException();

    /* Req: parser1 */
    virtual const char* what() const throw();
};

class EnumUsedButNotDefinedException : public ParseException {
   public:
    EnumUsedButNotDefinedException(Token const, std::string const msg = "");
    ~EnumUsedButNotDefinedException();

    /* Req: parser1 */
    virtual const char* what() const throw();
};

class RdStringException : public ParseException {
   public:
    RdStringException(Token const, std::string const msg = "");
    ~RdStringException();

    /* Req: parser1 */
    virtual const char* what() const throw();
};

class ArrayException : public ParseException {
   public:
    ArrayException(Token const, std::string const msg = "");
    ~ArrayException();

    /* Req: parser1 */
    virtual const char* what() const throw();
};

class RdTypeException : public ParseException {
   public:
    RdTypeException(RdTypeInstance const);
    RdTypeException(Token const, std::string const msg = "");
    ~RdTypeException();

    /* Req: parser1 */
    virtual const char* what() const throw();

   private:
    RdTypeInstance const rd_type;
};

#endif /* PARSEEXCEPTION_H_ */
