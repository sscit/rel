Requirement
{
    Identifier : parser1,
    Text : "For every parse error, the parser shall output the line numbers,
    where the error occured and a meaningful error message.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : parser2,
    Text : "The parser shall validate that all links defined in the model are valid. 
    If there are invalid links, the parser shall output an error.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : parser3,
    Text : "The parser shall validate that all enum values used in the data do exist in
    specification. If an enum value is used that does not exist, the parser shall output an error.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : parser4,
    Text : "The parser shall support different log levels via command line option.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : parser5,
    Text : "The parser shall validate that all type instances comply to the type definition
    from specification. If there is a mismatch between type instance and type definition,
    an error shall be raised.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : integ24,
    Text : "The parser shall produce a deterministic output. Same input shall always lead to same output.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : integ25,
    Text : "The parser shall preserve the order of type instances induced by the input data.",
    State : Accepted,
    ImplementationStatus : Implemented,
}