/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "AST.h"

bool RsTypeAttribute::IsTypeId() const {
    if(token_of_attribute.GetTokenType() == TokenType::ID) {
        return true;
    }
    else return false;
}

