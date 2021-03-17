# Script reads a REL model and prints out the requirements data again.
# It can be used as template to apply pretty-print to a rel model
# and to add own hooks, for bulk changes in an existing model.
import librel_py as rel

def indent(level):
    ret = ""
    for l in range(level):
        ret = ret + "    "

    return ret

def get_attribute_value(attr):
    ret = ""

    if attr.string_value.Get() != "":
        if attr.name.IsTypeId():
            ret = attr.string_value.Get()
        else:
            ret = "\"" + attr.string_value.Get() + "\""
    elif attr.integer_value.IsValid():
        ret = str(attr.integer_value.Get())
    elif attr.enum_value.Get() != "":
        ret = attr.enum_value.Get()
    elif len(attr.link_value) > 0:
        ret = "["
        for link in attr.link_value:
            ret = ret + link.Get() + ","
        ret = ret + "]"

    return ret

def print_type_instance(ti):
# apply own formatting style of requirements data or
# custom modifications ("bulk-change") of the data in this method
    ti_string = ti.type.Get() + " {\n"
    for attr in ti.attributes:
        ti_string = ti_string + indent(1) + attr.name.Get() + " : " + get_attribute_value(attr) + ",\n"

    ti_string = ti_string + "}\n\n"

    return ti_string


## main
l = rel.Logger()
#l.SetLogLevel(rel.LogLevel.DBUG)

fe = rel.FileEngine(l)
fe.SetSearchRecursive(True)
fe.SetDirectory("./requirements")

relparser = rel.RelParser(l, fe)
if relparser.ProcessRelModel() == rel.ParseResult.NoExceptionOccurred:
    data = relparser.GetDatabase()

    # get the requirements data of every file and write it back
    for one_file in data:
        with open(one_file.filename, 'w') as file_out:
            for type_instance in one_file.type_instances:
               file_out.write( print_type_instance(type_instance) )
