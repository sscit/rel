ReqDef
{
    unique_id : ReqId1,
    text : "Full Char äöÜß汉ṕò²³",
    status : Accepted,
    parent_req : ReqId1,
}

ReqDef
{
    unique_id : ReqId2,
    text : "System Requirement",
    status : Accepted,
    parent_req : [ReqId1,],
}

/* An instance of the requirement */
ReqDef
{
    unique_id : ReqId3,
    text : "Subsystem Requirement",
    status : Accepted,
    // Requirement is derived from two other requirements
    parent_req : [ReqId1,ReqId2,],
}

ReqDef
{
    unique_id : ReqId4,
    text : "SubSubsystem Requirement",
    status : Accepted,
    parent_req : ReqId3,
}
