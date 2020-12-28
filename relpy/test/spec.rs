type Requirement
{
    unique_id : id,
    text : string,
    status : State,
}

enum State
{
    Accepted,
    Rejected,
}
