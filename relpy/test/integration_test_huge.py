import librel_py as rel
import sys
import time
import os

def test_huge_project():
    l = rel.Logger()
    #l.SetLogLevel(rel.LogLevel.INFO)

    fe = rel.FileEngine(l)
    fe.SetSearchRecursive(True)
    fe.SetDirectory(os.getcwd() + "/test/huge")

    relparser = rel.RelParser(l, fe)
    ret = relparser.ProcessRelModel()

    assert ret == rel.ParseResult.NoExceptionOccurred

    data = relparser.GetDatabase()
    nr_of_type_instances = 0
    for files in data:
        nr_of_type_instances = nr_of_type_instances + len(files.type_instances)

    assert nr_of_type_instances == 474629
        
# main
test_huge_project()
