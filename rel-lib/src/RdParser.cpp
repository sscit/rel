/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "RdParser.h"

RdParser::RdParser(Logger &logger, RsParser const & s) : Parser(logger), specification(s) { }

ParsingStatistic RdParser::GetParsingStatistics() const {
    return statistic;
}

std::vector<RdTypeInstance> RdParser::GetDatabase()
{
    return database;
}

template<class T>
void RdParser::EnsureToken(FileTokenData const& tokens, unsigned int& index, TokenType const& tt, T const e) {
    while(tokens.token_list[index].GetTokenType() == TokenType::END_OF_LINE ||
          tokens.token_list[index].GetTokenType() == TokenType::LINE_COMMENT ||
          tokens.token_list[index].GetTokenType() == TokenType::COMMENT_BLOCK_START) {
        if(tokens.token_list[index].GetTokenType() == TokenType::LINE_COMMENT) {
            LineComment(tokens, index);
        }
        if(tokens.token_list[index].GetTokenType() == TokenType::COMMENT_BLOCK_START) {
            MultiLineComment(tokens, index);
        }
        index++;
    }

    if(tokens.token_list[index].GetTokenType() != tt)
        throw e;
}


void RdParser::CheckAllLinks() {
    l.LOG(LogLevel::DEBUG, "Checking that all links defined exist and point to valid ids");

    for(unsigned int i=0; i<database.size(); i++) {
        RdTypeInstance const &instance = database[i];

        for(unsigned int j=0; j<instance.attributes.size(); j++) {
            RdTypeInstanceAttribute const &element = instance.attributes[j];
            if(element.name.token_of_attribute.GetTokenType() == TokenType::LINK) {
                for(auto const &link : element.link_value) {
                    auto const search = unique_ids.find(link.name);
                    l.LOG(LogLevel::DEBUG, "search for " + link.name);
                    if(search == unique_ids.end()) {
                        throw RdTypeException(element.token_of_value, "Link points to id that does not exist");
                    }
                }
            }
        }
    }
}

RdString RdParser::ReadString(FileTokenData const& tokens, unsigned int &index) {
    RdString result;
    Token const& begin_string = tokens.token_list[index];
    EnsureToken(tokens, index, TokenType::QUOTATION_MARK, RdStringException(begin_string, "Invalid token, expected \""));
    index++;

    bool suffix_has_been_added=false;
    while(tokens.token_list[index].GetTokenType() != TokenType::QUOTATION_MARK
          && index < tokens.token_list.size()) {
        suffix_has_been_added = false;
        std::string suffix = "";
        if(tokens.token_list[index].GetTokenType() != TokenType::END_OF_LINE) {
            suffix = " ";
            suffix_has_been_added = true;
        }

        result.value.append(tokens.token_list[index].GetTokenValue() + suffix);
        index++;
    }

    if(suffix_has_been_added) {
        result.value.pop_back();
    }

    if(index == tokens.token_list.size())
        throw RdStringException(begin_string, "String termination not found");

    return result;
}

RdInteger RdParser::Integer(FileTokenData const& tokens, unsigned int &index) {
    RdInteger integer_value(std::stoi(tokens.token_list[index].GetTokenValue()));

    return integer_value;
}

bool RdParser::EnumValueExists(std::vector<RsRdIdentifier> const &enum_values, RsRdIdentifier &enum_value) const {
    std::vector<RsRdIdentifier>::const_iterator iter = enum_values.begin();
    bool found = false;
    for(;iter != enum_values.end(); ++iter) {
        if(iter->name.compare(enum_value.name) == 0) {
            found = true;
            enum_value = *iter;
            break;
        }
    }

    return found;
}

bool RdParser::HasAttributeValueCorrectType(RsTypeAttribute const& type_element, TokenType const current_token) {
    return (type_element.token_of_attribute.GetTokenType() == current_token);
}

void RdParser::ParseArrayOfLinks(FileTokenData const& tokens, unsigned int& index, RdTypeInstanceAttribute& type_instance_attribute)
{
    l.LOG(LogLevel::DEBUG, "Parsing Array of links");
    Token const &array_start = tokens.token_list[index];

    index++;
    while( (tokens.token_list.at(index).GetTokenType() != TokenType::ARRAY_END) && (index < tokens.token_list.size()) ) {
        EnsureToken(tokens, index, TokenType::IDENTIFIER, ArrayException(tokens.token_list.at(index), "Wrong token, expected identifier"));
        type_instance_attribute.link_value.push_back(Identifier(tokens, index));
        index++;

        EnsureToken(tokens, index, TokenType::COMMA, ArrayException(tokens.token_list.at(index), "Wrong token, expected ,"));
        index++;
    }

    if(index == tokens.token_list.size()) {
        throw ArrayException(array_start, "Array not finished, missing ]");
    }
    if(type_instance_attribute.link_value.size() == 0) {
        throw ArrayException(array_start, "Array empty");
    }
}

RdTypeInstance RdParser::TypeInstance(FileTokenData const& tokens, unsigned int& index) {
    Token const &type_token = tokens.token_list.at(index);
    RdTypeInstance type_instance;
    RsRdIdentifier type_name = Identifier(tokens, index++);
    RsType type_definition;
    l.LOG(LogLevel::DEBUG, "Parsing Type Instance of type " + type_name.name);

    try {
        type_definition = specification.GetType(type_name);
    }
    catch(TypeNotFoundException &e) {
        throw RdTypeException(type_token, "Type " + type_name.name + " not found in specification");
    }
    type_instance.type = type_definition;
    type_instance.file_origin = tokens.filepath;

    EnsureToken(tokens, index, TokenType::BRACKET_OPEN, RdTypeException(tokens.token_list.at(index), "Wrong token, expected {"));
    index++;

    unsigned int number_of_elements = 0;
    while( !IsNextToken(tokens, index, TokenType::BRACKET_CLOSE) ) {
        l.LOG(LogLevel::DEBUG, "Starting to parse new data attribute");
        EnsureToken(tokens, index, TokenType::IDENTIFIER, RdTypeException(tokens.token_list.at(index), "Wrong token, expected attribute"));
        RsRdIdentifier attribute = Identifier(tokens, index);
        if( number_of_elements >= type_definition.attributes.size() ||
            attribute.name.compare(type_definition.attributes[number_of_elements].name.name) != 0) {
            throw RdTypeException(tokens.token_list.at(index), "Attribute not defined in type specification");
        }
        l.LOG(LogLevel::DEBUG, "Parsing data attribute named " + attribute.name);

        RdTypeInstanceAttribute type_instance_attribute;
        type_instance_attribute.name = type_definition.attributes[number_of_elements];
        index++;

        EnsureToken(tokens, index, TokenType::COLON, RdTypeException(tokens.token_list.at(index), "Wrong token, expected :"));
        index++;
        type_instance_attribute.token_of_value = tokens.token_list.at(index);
        if(tokens.token_list.at(index).GetTokenType() == TokenType::STRING_VALUE) {
            if(HasAttributeValueCorrectType(type_instance_attribute.name, TokenType::STRING)) {
                type_instance_attribute.string_value.value = tokens.token_list.at(index).GetTokenValue();
                l.LOG(LogLevel::DEBUG, "attribute value of type string parsed");
            }
            else {
                throw RdTypeException(tokens.token_list.at(index), "Attribute value has wrong type, expected string value");
            }
        }
        else if(tokens.token_list.at(index).GetTokenType() == TokenType::QUOTATION_MARK) {
            if(HasAttributeValueCorrectType(type_instance_attribute.name, TokenType::STRING)) {
                type_instance_attribute.string_value = ReadString(tokens, index);
                l.LOG(LogLevel::DEBUG, "attribute value of type string parsed");
            }
            else {
                throw RdTypeException(tokens.token_list.at(index), "Attribute value has wrong type, expected string value");
            }
        }
        else if(tokens.token_list.at(index).GetTokenType() == TokenType::INTEGER_VALUE) {
            if(HasAttributeValueCorrectType(type_instance_attribute.name, TokenType::INT)) {
                type_instance_attribute.integer_value = Integer(tokens, index);
                l.LOG(LogLevel::DEBUG, "attribute value of type integer parsed");
            }
            else {
                throw RdTypeException(tokens.token_list.at(index), "Attribute value has wrong type, expected integer value");
            }
        }
        else if(tokens.token_list.at(index).GetTokenType() == TokenType::ARRAY_BEGIN) {
            /* An array is used. Ensure that the attribute is of type link. Only in this case,
               an array can be used */
            if(type_instance_attribute.name.token_of_attribute.GetTokenType() == TokenType::LINK) {
                l.LOG(LogLevel::DEBUG, "an array of links to unique ids has been identified");
                ParseArrayOfLinks(tokens, index, type_instance_attribute);
            }
            else {
                throw RdTypeException(tokens.token_list.at(index), "Unexpected token");
            }
        }
        else if(tokens.token_list.at(index).GetTokenType() == TokenType::IDENTIFIER) {
            l.LOG(LogLevel::DEBUG, "attribute value of type identifier identified");
            if(type_instance_attribute.name.token_of_attribute.GetTokenType() == TokenType::ENUM) {
                // Check that the enum value used exists
                type_instance_attribute.enum_value.name = tokens.token_list.at(index).GetTokenValue();
                if(!EnumValueExists(type_instance_attribute.name.enum_definition.enum_elements, type_instance_attribute.enum_value)) {
                    throw RdTypeException(tokens.token_list.at(index), "Enum value " + type_instance_attribute.enum_value.name + \
                                          " not found in specification of enum type " + \
                                          type_instance_attribute.name.enum_definition.name.name);
                }
                l.LOG(LogLevel::DEBUG, "attribute has enum value " + type_instance_attribute.enum_value.name + ", enum value exists");
            }
            else if(type_instance_attribute.name.token_of_attribute.GetTokenType() == TokenType::LINK) {
                type_instance_attribute.link_value.push_back(Identifier(tokens, index));
                l.LOG(LogLevel::DEBUG, "attribute of type link parsed. One link has been set.");
            }
            else if(type_instance_attribute.name.token_of_attribute.GetTokenType() == TokenType::ID) {
                type_instance_attribute.string_value.value = tokens.token_list.at(index).GetTokenValue();
                l.LOG(LogLevel::DEBUG, "attribute of type id parsed");
            }
            else
                throw RdTypeException(tokens.token_list.at(index), "Unexpected token");
        }
        else if(tokens.token_list.at(index).GetTokenType() == TokenType::ID) {
            l.LOG(LogLevel::DEBUG, "attribute value of type id identified");
            if(HasAttributeValueCorrectType(type_instance_attribute.name, TokenType::ID)) {
                type_instance_attribute.string_value.value = tokens.token_list.at(index).GetTokenValue();
                l.LOG(LogLevel::DEBUG, "attribute value of type id parsed");
            }
            else {
                throw RdTypeException(tokens.token_list.at(index), "Attribute value has wrong type, expected id");
            }
        }
        else {
            throw RdTypeException(tokens.token_list.at(index), "Unexpected token");
        }

        // check for unique id, if this attribute defined an id
        if(type_definition.attributes[number_of_elements].token_of_attribute.GetTokenType() == TokenType::ID) {
            if(unique_ids.find(type_instance_attribute.string_value.value) != unique_ids.end()) {
                throw RdTypeException(tokens.token_list.at(index), "ID already used");
            }
            else {
                AddUniqueIdToDatabase(type_instance_attribute.string_value, tokens.filepath);
            }
        }

        type_instance.attributes.push_back(type_instance_attribute);
        index++;
        EnsureToken(tokens, index, TokenType::COMMA, RdTypeException(tokens.token_list.at(index), "Wrong token, expected ,"));
        index++;
        number_of_elements++;
    }

    // Check error cases
    if(type_instance.attributes.size() == 0)
        throw RdTypeException(tokens.token_list.at(index), "No attributes defined in type instance");

    if(type_instance.attributes.size() < type_instance.type.attributes.size())
        throw RdTypeException(tokens.token_list.at(index), "Type instance does not match to type definition, missing attributes.");

    l.LOG(LogLevel::DEBUG, "Dataset " + type_instance.type.name.name + " (" + std::to_string(type_instance.attributes.size()) + " elements) has been created.");

    return type_instance;
}

void RdParser::CleanupUniqueIdDatabase(std::string const& uri) {
    // Clear old unique ids originating from this file out of the data base
    for( auto const & to : unique_id_origin) {
        if(to.uri.compare(uri) == 0) {
            unique_ids.erase(to.type_name); 
        }
    }

    unique_id_origin.remove_if(
        [&](TypeOrigin &to){
            return (to.uri.compare(uri) == 0);
        }
    );
}

void RdParser::AddUniqueIdToDatabase(RdString const& unique_id, std::string const& uri) {
    unique_ids.insert({unique_id.value, unique_id});

    TypeOrigin new_unique_id(uri);
    new_unique_id.type_name = unique_id.value;
    unique_id_origin.push_back(new_unique_id);
}

void RdParser::CleanupDatabase(std::string const& path) {
    // Clear old database entries originating from this file
    database.erase( std::remove_if(database.begin(), database.end(),
        [&](RdTypeInstance &ti){
            return (ti.file_origin.compare(path) == 0);
        }
    ), database.end() );
}

void RdParser::ParseTokens(FileTokenData const& tokens) {
    statistic.number_of_files++;
    CleanupUniqueIdDatabase(tokens.filepath);
    CleanupDatabase(tokens.filepath);

    for(unsigned int index=0; index<tokens.token_list.size(); ++index) {
        Token const& current_token = tokens.token_list[index];

        if(current_token.GetTokenType() == TokenType::LINE_COMMENT) {
            LineComment(tokens, index);
        }
        else if(current_token.GetTokenType() == TokenType::COMMENT_BLOCK_START) {
            MultiLineComment(tokens, index);
        }
        else if(current_token.GetTokenType() == TokenType::IDENTIFIER) {
            try {
                RdTypeInstance t = TypeInstance(tokens, index);
                database.push_back(t);
                statistic.number_of_type_instances++;
            }
            catch(std::out_of_range &e) {
                l.LOG(LogLevel::DEBUG, "Type instance not ready to be parsed completely, hit Array out of bounds.");
                throw RdTypeException(current_token, "Type instance incomplete");
            }
        }
        else if(current_token.GetTokenType() == TokenType::END_OF_LINE) { }
        else {
            throw WrongTokenException(current_token, "Unexpected token");
        }
    }
}

RdParser::~RdParser() { }
