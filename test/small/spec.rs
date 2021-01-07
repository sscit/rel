// Definition of Requirements Type
type Requirement
{
    unique_id : id,
    text : string,
    status : State,
    // Link to the id of the parent of this requirement
    parent_req : link,
}

/* Enum defines whether requirement
   is accepted or not */
enum State
{
    Accepted,
    Rejected,
}
