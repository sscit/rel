type Requirement
{
    Identifier : id,
    Text : string,
    State : RequirementState,
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

