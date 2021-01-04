Requirement
{
    unique_id : ReqId1,
    text : "System Requirement",
    status : Accepted,
    parent_req : ReqId1,
}

Requirement
{
    unique_id : ReqId2,
    text : "System Requirement",
    status : Accepted,
    parent_req : [ReqId1,],
}


Requirement
{
    unique_id : ReqId3,
    text : "Subsystem Requirement",
    status : Accepted,
    parent_req : [ReqId1,ReqId2,],
}

Requirement
{
    unique_id : ReqId4,
    text : "SubSubsystem Requirement",
    status : Accepted,
    parent_req : ReqId3,
}
