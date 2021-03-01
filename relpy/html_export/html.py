# Script creates a HTML export of the REL requirements data
import librel_py as rel

l = rel.Logger()
#l.SetLogLevel(rel.LogLevel.DBUG)

fe = rel.FileEngine(l)
fe.SetSearchRecursive(True)
fe.SetDirectory("./requirements")

relparser = rel.RelParser(l, fe)
if relparser.ProcessRelModel() == rel.ParseResult.NoExceptionOccurred:
    data = relparser.GetDatabase()

    print("<html><body>")
    print("<h1>REL Requirements</h1>")

    for one_file in data:
        print("<h2>" + one_file.filename + "</h2>")

        for type_instance in one_file.type_instances:
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
                    elif len(elem.link_value) > 0:
                        for link in elem.link_value:
                            print(link.Get() + " ")

                    print("</i></p>")

            print("<hr>")

    print("</body></html>")


