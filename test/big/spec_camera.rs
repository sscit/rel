type Camera.Requirement
{
    // unique identifier
    Identifier : id,
    // Description of the requirement
    Text : string,
    Supplier : Camera.Supplier,
}



// #### Enum Definitions
enum Camera.Supplier
{
    CompanyA,
    CompanyB,
    CompanyC,
}
