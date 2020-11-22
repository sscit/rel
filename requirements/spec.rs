type Requirement
{
    Identifier : id,
    Text : string,
    State : RequirementState,
    ImplementationStatus : ImplStatus,
}

enum ImplStatus
{
    Not_Implemented,
    Implemented,
}

type Information
{
    Identifier : id,
    Text : string,
}

enum RequirementState 
{
    Draft,
    In_Discussion,
    Accepted,
    Rejected,
}

