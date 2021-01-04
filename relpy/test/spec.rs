type Requirement
{
    unique_id : id,
    text : string,
    status : State,
    parent_req : link,
}

enum State
{
    Accepted,
    Rejected,
}
