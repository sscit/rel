/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "RdParser.h"

IdentifierPosition::IdentifierPosition(std::string const& name,
        unsigned int const ln, unsigned short const pil, unsigned short const l) :
        identifier_name(name), line_number(ln), position_in_line(pil), length(l) { }

RdParser::RdParser(Logger &logger, RsParser const & s) : Parser(logger), specification(s) {
    current_file_to_add = 1;
}

std::list<RdFile> const& RdParser::GetDatabase() const {
    return database;
}

template<class T>
void RdParser::EnsureToken(FileTokenData const& tokens, std::list<Token>::const_iterator& iter, TokenType const& tt, T const e) {
    while(iter->GetTokenType() == TokenType::END_OF_LINE ||
          iter->GetTokenType() == TokenType::LINE_COMMENT ||
          iter->GetTokenType() == TokenType::COMMENT_BLOCK_START) {
        if(iter->GetTokenType() == TokenType::LINE_COMMENT) {
            LineComment(tokens, iter);
        }
        if(iter->GetTokenType() == TokenType::COMMENT_BLOCK_START) {
            MultiLineComment(tokens, iter);
        }
        iter++;
    }

    if(iter->GetTokenType() != tt)
        throw e;
}


void RdParser::CheckAllLinks() {
    l.LOG(LogLevel::DBUG, "Checking that all links defined exist and point to valid ids");
    std::lock_guard<std::mutex> db_lock(mtx);
    for(auto const& files : database) {
        for(auto const& instance : files.type_instances) {
            for(unsigned int j=0; j<instance.attributes.size(); j++) {
                RdTypeInstanceAttribute const &element = instance.attributes[j];
                if(element.name->token_of_attribute.GetTokenType() == TokenType::LINK) {
                    for(auto const &link : element.link_value) {
                        auto const search = unique_ids.find(link.name);
                        l.LOG(LogLevel::DBUG, "search for " + link.name);
                        if(search == unique_ids.end()) {
                            throw RdTypeException(element.token_of_value, "Link points to id that does not exist");
                        }
                    }
                }
            }
        }
    }
}

RdString RdParser::ReadString(FileTokenData const& tokens, std::list<Token>::const_iterator& iter) {
    RdString result;
    Token const& begin_string = *iter;
    EnsureToken(tokens, iter, TokenType::QUOTATION_MARK, RdStringException(begin_string, "Invalid token, expected \""));
    iter++;

    bool suffix_has_been_added=false;
    while(iter->GetTokenType() != TokenType::QUOTATION_MARK
          && iter != tokens.token_list.end()) {
        suffix_has_been_added = false;
        std::string suffix = "";
        if(iter->GetTokenType() != TokenType::END_OF_LINE) {
            suffix = " ";
            suffix_has_been_added = true;
        }

        result.value.append(*(iter->GetTokenValue)() + suffix);
        iter++;
    }

    if(suffix_has_been_added) {
        result.value.pop_back();
    }

    if(iter == tokens.token_list.end())
        throw RdStringException(begin_string, "String termination not found");

    return result;
}

RdInteger RdParser::Integer(FileTokenData const& tokens, std::list<Token>::const_iterator& iter) {
    RdInteger integer_value(std::stoi(*(iter->GetTokenValue())));

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

void RdParser::ParseArrayOfLinks(FileTokenData const& tokens, std::list<Token>::const_iterator& iter, RdTypeInstanceAttribute& type_instance_attribute)
{
    l.LOG(LogLevel::DBUG, "Parsing Array of links");
    Token const &array_start = *iter;

    iter++;
    while( (iter->GetTokenType() != TokenType::ARRAY_END) && (iter != tokens.token_list.end()) ) {
        EnsureToken(tokens, iter, TokenType::IDENTIFIER, ArrayException(SafeDeref(tokens,iter), "Wrong token, expected identifier"));
        type_instance_attribute.link_value.push_back(Identifier(tokens, iter));
        StoreLinkLocationInFile(tokens.filepath,type_instance_attribute.link_value.back(), *iter);
        iter++;

        EnsureToken(tokens, iter, TokenType::COMMA, ArrayException(SafeDeref(tokens,iter), "Wrong token, expected ,"));
        iter++;
    }

    if(iter == tokens.token_list.end()) {
        throw ArrayException(array_start, "Array not finished, missing ]");
    }
    if(type_instance_attribute.link_value.size() == 0) {
        throw ArrayException(array_start, "Array empty");
    }
}

bool RdParser::GetTargetOfLink(std::string const& filename, IdentifierPosition const& link, 
                               IdentifierPosition& target, std::string& target_path) {
    bool ret = false;

    std::vector<IdentifierPosition>& links_in_file = links[filename];

    for(IdentifierPosition const& p : links_in_file) {
        if(link.line_number == p.line_number) {
            if(link.position_in_line >= p.position_in_line && 
               link.position_in_line < (p.position_in_line + p.length)
            ) {
                /* At the position provided, a link is located. Now return the position
                 * of the link target */
                ret = true;
                std::string const& id = p.identifier_name;
                target = unique_ids[id];
                for(TypeOrigin const& t : unique_id_origin) {
                    if(t.type_name.compare(id) == 0) {
                        target_path = t.path;
                        break;
                    }
                }
            }
        }
    }

    return ret;
}


RdTypeInstance RdParser::TypeInstance(FileTokenData const& tokens, std::list<Token>::const_iterator& iter) {
    Token const &type_token = *iter;
    RdTypeInstance type_instance;
    RsRdIdentifier type_name = Identifier(tokens, iter);
    iter++;
    RsType const * type_definition;
    l.LOG(LogLevel::DBUG, "Parsing Type Instance of type " + type_name.name);

    try {
        type_definition = specification.GetTypeDefinition(type_name);
    }
    catch(TypeNotFoundException &e) {
        throw RdTypeException(type_token, "Type " + type_name.name + " not found in specification");
    }
    type_instance.type = type_definition;

    EnsureToken(tokens, iter, TokenType::BRACKET_OPEN, RdTypeException(SafeDeref(tokens,iter), "Wrong token, expected {"));
    iter++;

    unsigned int number_of_elements = 0;
    while( !IsNextToken(tokens, iter, TokenType::BRACKET_CLOSE) ) {
        l.LOG(LogLevel::DBUG, "Starting to parse new data attribute");
        EnsureToken(tokens, iter, TokenType::IDENTIFIER, RdTypeException(SafeDeref(tokens,iter), "Wrong token, expected attribute"));
        RsRdIdentifier attribute = Identifier(tokens, iter);
        if( number_of_elements >= type_definition->attributes.size() ||
            attribute.name.compare(type_definition->attributes[number_of_elements].name.name) != 0) {
            throw RdTypeException(SafeDeref(tokens,iter), "Attribute not defined in type specification");
        }
        l.LOG(LogLevel::DBUG, "Parsing data attribute named " + attribute.name);

        RdTypeInstanceAttribute type_instance_attribute;
        type_instance_attribute.name = &type_definition->attributes[number_of_elements];
        iter++;

        EnsureToken(tokens, iter, TokenType::COLON, RdTypeException(SafeDeref(tokens,iter), "Wrong token, expected :"));
        iter++;
        type_instance_attribute.token_of_value = *iter;
        if(iter->GetTokenType() == TokenType::STRING_VALUE) {
            if(HasAttributeValueCorrectType(*type_instance_attribute.name, TokenType::STRING)) {
                type_instance_attribute.string_value.value = *(iter->GetTokenValue());
                l.LOG(LogLevel::DBUG, "attribute value of type string parsed");
            }
            else {
                throw RdTypeException(SafeDeref(tokens,iter), "Attribute value has wrong type, expected string value");
            }
        }
        else if(iter->GetTokenType() == TokenType::QUOTATION_MARK) {
            if(HasAttributeValueCorrectType(*type_instance_attribute.name, TokenType::STRING)) {
                type_instance_attribute.string_value = ReadString(tokens, iter);
                l.LOG(LogLevel::DBUG, "attribute value of type string parsed");
            }
            else {
                throw RdTypeException(SafeDeref(tokens,iter), "Attribute value has wrong type, expected string value");
            }
        }
        else if(iter->GetTokenType() == TokenType::INTEGER_VALUE) {
            if(HasAttributeValueCorrectType(*type_instance_attribute.name, TokenType::INT)) {
                type_instance_attribute.integer_value = Integer(tokens, iter);
                l.LOG(LogLevel::DBUG, "attribute value of type integer parsed");
            }
            else {
                throw RdTypeException(SafeDeref(tokens,iter), "Attribute value has wrong type, expected integer value");
            }
        }
        else if(iter->GetTokenType() == TokenType::ARRAY_BEGIN) {
            /* An array is used. Ensure that the attribute is of type link. Only in this case,
               an array can be used */
            if(type_instance_attribute.name->token_of_attribute.GetTokenType() == TokenType::LINK) {
                l.LOG(LogLevel::DBUG, "an array of links to unique ids has been identified");
                ParseArrayOfLinks(tokens, iter, type_instance_attribute);
            }
            else {
                throw RdTypeException(SafeDeref(tokens,iter), "Unexpected token");
            }
        }
        else if(iter->GetTokenType() == TokenType::IDENTIFIER) {
            l.LOG(LogLevel::DBUG, "attribute value of type identifier identified");
            if(type_instance_attribute.name->token_of_attribute.GetTokenType() == TokenType::ENUM) {
                // Check that the enum value used exists
                type_instance_attribute.enum_value.name = *(iter->GetTokenValue());
                if(!EnumValueExists(type_instance_attribute.name->enum_definition.enum_elements, type_instance_attribute.enum_value)) {
                    throw RdTypeException(SafeDeref(tokens,iter), "Enum value " + type_instance_attribute.enum_value.name + \
                                          " not found in specification of enum type " + \
                                          type_instance_attribute.name->enum_definition.name.name);
                }
                l.LOG(LogLevel::DBUG, "attribute has enum value " + type_instance_attribute.enum_value.name + ", enum value exists");
            }
            else if(type_instance_attribute.name->token_of_attribute.GetTokenType() == TokenType::LINK) {
                type_instance_attribute.link_value.push_back(Identifier(tokens, iter));
                l.LOG(LogLevel::DBUG, "attribute value of type link parsed. One link has been set.");
                StoreLinkLocationInFile(tokens.filepath,type_instance_attribute.link_value.back(), *iter);
            }
            else if(type_instance_attribute.name->token_of_attribute.GetTokenType() == TokenType::ID) {
                type_instance_attribute.string_value.value = *(iter->GetTokenValue());
                l.LOG(LogLevel::DBUG, "attribute of type id parsed");
                // ensure uniqueness of ID
                if(unique_ids.find(type_instance_attribute.string_value.value) != unique_ids.end()) {
                    throw RdTypeException(SafeDeref(tokens,iter), "ID already used");
                }
                else {
                    AddUniqueIdToDatabase(type_instance_attribute.string_value, tokens.filepath, *iter);
                }
            }
            else
                throw RdTypeException(SafeDeref(tokens,iter), "Unexpected token");
        }
        else {
            throw RdTypeException(SafeDeref(tokens,iter), "Unexpected token");
        }

        type_instance.attributes.push_back(type_instance_attribute);
        iter++;
        EnsureToken(tokens, iter, TokenType::COMMA, RdTypeException(SafeDeref(tokens,iter), "Wrong token, expected ,"));
        iter++;
        number_of_elements++;
    }

    // Check error cases
    if(type_instance.attributes.size() == 0)
        throw RdTypeException(SafeDeref(tokens,iter), "No attributes defined in type instance");

    if(type_instance.attributes.size() < type_instance.type->attributes.size())
        throw RdTypeException(SafeDeref(tokens,iter), "Type instance does not match to type definition, missing attributes.");

    l.LOG(LogLevel::DBUG, "Dataset " + type_instance.type->name.name + " (" + std::to_string(type_instance.attributes.size()) + " elements) has been created.");

    return type_instance;
}

void RdParser::StoreLinkLocationInFile(std::string const& filename, RsRdIdentifier const& link, Token const& t) {
    // Add the link's position in file to the data structure
    std::lock_guard<std::mutex> db_lock(mtx);
    std::vector<IdentifierPosition>& link_ds = links[filename];
    IdentifierPosition pos(link.Get(), t.GetLineNumberOfToken(), t.GetPositionInLineOfToken(), t.GetTokenValue()->size());
    link_ds.push_back(pos);
}

void RdParser::CleanupUniqueIdDatabase(std::string const& path) {
    // Clear old unique ids originating from this file out of the data base
    std::lock_guard<std::mutex> db_lock(mtx);
    for( auto const & to : unique_id_origin) {
        if(to.path.compare(path) == 0) {
            unique_ids.erase(to.type_name); 
        }
    }

    unique_id_origin.remove_if(
        [&](TypeOrigin &to){
            return (to.path.compare(path) == 0);
        }
    );
}

void RdParser::AddUniqueIdToDatabase(RdString const& unique_id, std::string const& uri, Token const& token) {
    std::lock_guard<std::mutex> db_lock(mtx);
    IdentifierPosition pos(unique_id.Get(), token.GetLineNumberOfToken(), token.GetPositionInLineOfToken(), token.GetTokenValue()->size());
    unique_ids.insert({unique_id.value, pos});

    TypeOrigin new_unique_id(uri);
    new_unique_id.type_name = unique_id.value;
    unique_id_origin.push_back(new_unique_id);
}

void RdParser::CleanupDatabase(std::string const& path) {
    // Clear old database entries originating from this file
    std::lock_guard<std::mutex> db_lock(mtx);
    database.erase( std::remove_if(database.begin(), database.end(),
        [&](RdFile &f){
            return (f.filename.compare(path) == 0);
        }
    ), database.end() );
}

void RdParser::ParseTokens(FileTokenData const& tokens, unsigned int const file_index) {
    CleanupUniqueIdDatabase(tokens.filepath);
    CleanupDatabase(tokens.filepath);
    links[tokens.filepath].clear();

    RdFile new_types;
    new_types.filename = tokens.filepath;

    std::list<Token>::const_iterator iter = tokens.token_list.begin();
    while(iter != tokens.token_list.end()) {
        Token const& current_token = *iter;

        if(current_token.GetTokenType() == TokenType::LINE_COMMENT) {
            LineComment(tokens, iter);
        }
        else if(current_token.GetTokenType() == TokenType::COMMENT_BLOCK_START) {
            MultiLineComment(tokens, iter);
        }
        else if(current_token.GetTokenType() == TokenType::IDENTIFIER) {
            try {
                RdTypeInstance t = TypeInstance(tokens, iter);
                new_types.type_instances.push_back(t);
            }
            catch(std::out_of_range &e) {
                l.LOG(LogLevel::DBUG, "Type instance not ready to be parsed completely, hit Array out of bounds.");
                throw RdTypeException(current_token, "Type instance incomplete");
            }
        }
        else if(current_token.GetTokenType() == TokenType::END_OF_LINE) { }
        else {
            throw WrongTokenException(current_token, "Unexpected token");
        }

        if(iter != tokens.token_list.end())
            iter++;
    }

    AddToDatabase(new_types, file_index);
}

void RdParser::AddToDatabase(RdFile const& new_types, unsigned int const file_index) {
    std::unique_lock<std::mutex> db_lock(mtx);
    db_access.wait(db_lock, [&]{return file_index <= current_file_to_add;});

    l.LOG(LogLevel::INFO, "Storing AST Elements of file " + new_types.filename + " in database.");
    database.push_back(new_types);
    current_file_to_add++;
    db_lock.unlock();
    db_access.notify_all();
}

RdParser::~RdParser() { }
