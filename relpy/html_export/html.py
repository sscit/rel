# Script creates a HTML export of the REL requirements data
import librel_py as rel

l = rel.Logger()
#l.SetLogLevel(rel.LogLevel.DEBUG)

fe = rel.FileEngine(l)
fe.SetSearchRecursive(True)
fe.SetStartDirectory("./requirements")

relparser = rel.RelParser(l, fe)
if relparser.ProcessRelModel() == rel.ParseResult.NoExceptionOccurred:
    data = relparser.GetDatabase()

    print("<html><body>")
    print("<h1>REL Requirements</h1>")

    file_origin = ""

    for type_instance in data:
        if type_instance.file_origin != file_origin:
            file_origin = type_instance.file_origin
            print("<h2>" + file_origin + "</h2>")

        for elem in type_instance.attributes:
           if elem.name.Get() == "Text":
                   print("<p>" + elem.string_value.Get() + "</p>")

        for elem in type_instance.attributes:
           if elem.name.Get() != "Text":
                   print("<p><i>" + elem.name.Get() + ": ")
                   if elem.string_value.Get() != "":
                       print(elem.string_value.Get())
                   elif elem.integer_value.IsValid():
                       print(str(elem.integer_value.Get()))
                   elif elem.enum_value.Get() != "":
                       print(elem.enum_value.Get())
                   elif elem.link_value.Get() != "":
                       print(elem.link_value.Get())

                   print("</i></p>")

        print("<hr>")

    print("</body></html>")


