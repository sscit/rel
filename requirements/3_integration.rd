Requirement
{
    Identifier : integ1,
    Text : "REL parser shall be executed in Linux environment.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : integ11,
    Text : "REL parser shall be executed in MacOS environment.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : integ2,
    Text : "REL parser shall be able to parse all relevant files (requirements specification
            and requirements data) in a directory and its subdirectories.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : integ22,
    Text : "It shall be possible to provide multiple directory paths to the parser, which are then used
            as starting point to parse all relevant files in the corresponding directory tree.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : integ23,
    Text : "All files shall be parsed in alphabetical order, sorted by their path.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : integ_py1,
    Text : "REL parser shall be integrated into python environment, so that
    custom validators and data exporters can be written in python.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : integ_py2,
    Text : "Python integration shall allow accessing both requirements specification and
    requirements data of the model.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : integ3,
    Text : "REL parser shall be integrated into IDE via language server protocol, 
    so that the editor and its code completion features can be used to support writing requirements.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : integ5,
    Text : "language server shall support validation of the DSL and report errors back to IDE.",
    State : Accepted,
    ImplementationStatus : Implemented,
}

Requirement
{
    Identifier : integ6,
    Text : "language server shall support auto completion.",
    State : Accepted,
    ImplementationStatus : Not_Implemented,
}