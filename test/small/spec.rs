// Definition of Requirements Type
type ReqDef
{
    unique_id : id,
    text : string,
    status : ConfirmationState,
    // Link to the id of the parent of this requirement
    parent_req : link,
}

/* Enum defines whether requirement
   is accepted or not */
enum ConfirmationState
{
    Accepted,
    Rejected,
}
