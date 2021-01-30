import librel_py as rel
import sys
import time
import os

def test_big_project():
    l = rel.Logger()
    #l.SetLogLevel(rel.LogLevel.INFO)

    fe = rel.FileEngine(l)
    fe.SetSearchRecursive(True)
    fe.SetDirectory(os.getcwd() + "/test/big")

    relparser = rel.RelParser(l, fe)
    ret = relparser.ProcessRelModel()

    assert ret == rel.ParseResult.NoExceptionOccurred

    data = relparser.GetDatabase()
    nr_of_type_instances = 0
    for files in data:
        nr_of_type_instances = nr_of_type_instances + len(files.type_instances)

    assert nr_of_type_instances == 4041

    # check that the data has been parsed properly
    for files in data:
        for d in files.type_instances:
            if d.type.Get() == "ComponentRequirement":
                assert len(d.attributes) == 7
                assert len(d.type.attributes) == 7
                # check that specification of this type is available
                assert d.type.attributes[0].Get() == "Identifier"
                assert d.type.attributes[1].Get() == "Text"
                assert d.type.attributes[2].Get() == "Parent_Requirement"
                assert d.type.attributes[3].Get() == "Owner"

                assert d.type.attributes[4].Get() == "Status"
                assert d.type.attributes[4].enum_definition.Get() == "Status"
                assert len(d.type.attributes[4].enum_definition.enum_elements) == 4
                assert d.type.attributes[4].enum_definition.enum_elements[0].Get() == "Draft"
                assert d.type.attributes[4].enum_definition.enum_elements[1].Get() == "New"
                assert d.type.attributes[4].enum_definition.enum_elements[2].Get() == "InProgress"
                assert d.type.attributes[4].enum_definition.enum_elements[3].Get() == "Accepted"

                assert d.type.attributes[5].Get() == "Is_security_relevant"
                assert d.type.attributes[5].enum_definition.Get() == "SecurityRelevance"

                assert d.type.attributes[6].Get() == "Is_implemented"
                assert d.type.attributes[6].enum_definition.Get() == "ImplementationStatus"


            if d.type.Get() == "Information":
                assert len(d.attributes) == 2
            
            if d.type.Get() == "SystemRequirement":
                assert len(d.attributes) == 6
                assert d.attributes[0].name.Get() == "Identifier"
                assert d.attributes[1].name.Get() == "Text"
                assert d.attributes[2].name.Get() == "Parent_Requirement"
                assert d.attributes[3].name.Get() == "Owner"
                assert d.attributes[4].name.Get() == "Status"
                assert d.attributes[5].name.Get() == "Is_security_relevant"

                # check one explicit data set, that it has been read correctly
                if d.attributes[0].string_value.Get() == "hdasjh736723":
                    assert d.attributes[1].string_value.Get() == "The smartphone shall use a POSIX based operating system"
                    assert len(d.attributes[2].link_value) == 1
                    assert d.attributes[2].link_value[0].Get() == "hdasjh736723"
                    assert d.attributes[3].enum_value.Get() == "ProductTeam"
                    assert d.attributes[4].enum_value.Get() == "Accepted"
                    assert d.attributes[5].enum_value.Get() == "No"
        
# main
test_big_project()
