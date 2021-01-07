// #### Type Definitions
// represents a requirement on system level
type SystemRequirement
{
    // unique identifier
    Identifier : id,
    // Description of the requirement
    Text : string,
    /* Link to parent requirement, which is detailed by this requirement
       If there is no parent, the link is set to itself
    */
    Parent_Requirement : link,
    // Development team, that owns the requirement
    Owner : Owner,
    // State of the requirement in the process
    Status : Status,
    // relevant for security evaluation?
    Is_security_relevant : SecurityRelevance,
}

// represents a requirement on component level
type ComponentRequirement
{
    // unique identifier
    Identifier : id,
    // Description of the requirement
    Text : string,
    /* Link to parent requirement, which is detailed by this requirement
       If there is no parent, the link is set to itself
    */
    Parent_Requirement : link,
    // Development team, that owns the requirement
    Owner : Owner,
    // State of the requirement in the process
    Status : Status,
    // relevant for security evaluation?
    Is_security_relevant : SecurityRelevance,
    // already implemented?
    Is_implemented : ImplementationStatus,
}


// represents an information, to make the requirements more readable
type Information
{
    Identifier : id,
    Text : string,
}


// #### Enum Definitions
enum Status
{
    Draft,
    New,
    InProgress,
    Accepted,
}

enum Owner 
{
    ProductTeam,
    KernelTeam,
    GUITeam,
    AppsTeam,
}

enum SecurityRelevance
{
    Yes,
    No,
}

enum ImplementationStatus
{
    Yes,
    No,
}