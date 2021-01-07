import librel_py as rel
import sys
import time
import os

def test_huge_project():
    l = rel.Logger()
    #l.SetLogLevel(rel.LogLevel.INFO)

    fe = rel.FileEngine(l)
    fe.SetSearchRecursive(True)
    fe.SetStartDirectory(os.getcwd() + "/test/huge")

    relparser = rel.RelParser(l, fe)
    ret = relparser.ProcessRelModel()

    assert ret == rel.ParseResult.NoExceptionOccurred

    data = relparser.GetDatabase()
    assert len(data) == 474629
        
# main
# library needs more speed improvement, until this test can be enabled
#test_huge_project()
