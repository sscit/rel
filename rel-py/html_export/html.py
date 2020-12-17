# Script creates a HTML export of the REL requirements data
import librel_py as rel

l = rel.Logger()
#l.SetLogLevel(rel.LogLevel.DEBUG)

fe = rel.FileEngine()
fe.SetSearchRecursive(True)
fe.SetStartDirectory("../../requirements")

relparser = rel.RelParser(l, fe)
if relparser.ProcessRelModel() == rel.ParseResult.NoExceptionOccurred:
    data = relparser.GetDatabase()

    print("<html><body>")
    print("<h1>REL Requirements</h1>")

    for type_instance in data:
        print (type_instance.file_origin)

        for elem in type_instance.type_elements_data:
           if elem.name.name.name == "Text":
                   print("<p>" + elem.string_value.value + "</p>")

        for elem in type_instance.type_elements_data:
           if elem.name.name.name != "Text":
                   print("<p><i>" + elem.name.name.name + ": ")
                   if elem.string_value.value != "":
                       print(elem.string_value.value)
                   elif elem.integer_value.value != 0:
                       print(str(elem.integer_value.value))
                   elif str(elem.enum_value.name) != "":
                       print(str(elem.enum_value.name))

                   print("</i></p>")

        print("<hr>")

    print("</body></html>")


