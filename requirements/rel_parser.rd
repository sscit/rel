Requirement
{
    Identifier : parser1,
    Text : "For every parse error, the parser shall output the line numbers,
    where the error occured and a meaningful error message.",
    State : Accepted,
}

Requirement
{
    Identifier : parser2,
    Text : "The parser shall validate that all links defined in the model are valid. 
    If there are invalid links, the parser shall output an error.",
    State : Accepted,
}

Requirement
{
    Identifier : parser3,
    Text : "The parser shall validate that all enum values used in the data do exist in
    specification. If an enum value is used that does not exist, the parser shall output an error.",
    State : Accepted,
}

Requirement
{
    Identifier : parser4,
    Text : "The parser shall support different log levels via command line option.",
    State : Accepted,
}

Requirement
{
    Identifier : parser5,
    Text : "The parser shall validate that all type instances comply to the type definition
    from specification. If there is a mismatch between type instance and type definition,
    an error shall be raised.",
    State : Accepted,
}