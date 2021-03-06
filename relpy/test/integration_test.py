import librel_py as rel
import sys
import time
import os

def test_rel_py():
    l = rel.Logger()
    #l.SetLogLevel(rel.LogLevel.DBUG)

    fe = rel.FileEngine(l)
    fe.SetSearchRecursive(True)
    fe.SetDirectory(os.getcwd() + "/test/small")

    relparser = rel.RelParser(l, fe)
    ret = relparser.ProcessRelModel()

    assert ret == rel.ParseResult.NoExceptionOccurred

    data = relparser.GetDatabase()
    assert len(data[0].type_instances) == 4

    # check that the data structure looks as expected
    counter = 1
    for d in data[0].type_instances:
        assert len(d.attributes) == 4
        assert d.attributes[0].name.Get() == "unique_id"
        assert d.attributes[0].string_value.Get() == "ReqId" + str(counter)
        assert d.attributes[1].name.Get() == "text"

        if d.attributes[0].string_value.Get() == "ReqId1":
            assert d.attributes[1].string_value.Get() == "Full Char äöÜß汉ṕò²³"

        assert d.attributes[2].name.Get() == "status"
        assert d.attributes[2].enum_value.Get() == "Accepted"
        assert len(d.attributes[3].link_value) >= 1

        counter = counter + 1


# main
test_rel_py()
