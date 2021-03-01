import librel_py as rel
import sys
import time
import os

def test_small_and_big_project():
    l = rel.Logger()
    #l.SetLogLevel(rel.LogLevel.INFO)

    fe = rel.FileEngine(l)
    fe.SetSearchRecursive(True)
    fe.SetDirectory(os.getcwd() + "/test/big")
    fe.SetDirectory(os.getcwd() + "/test/small")

    relparser = rel.RelParser(l, fe)
    ret = relparser.ProcessRelModel()

    assert ret == rel.ParseResult.NoExceptionOccurred

    data = relparser.GetDatabase()
    type_inst = 0
    for files in data:
        type_inst = type_inst + len(files.type_instances)

    assert type_inst == 4047

# main
test_small_and_big_project()
