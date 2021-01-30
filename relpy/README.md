# rel_py - Python3 integration of REL

The REL framework provides a Python3 module, which can be imported into Python3 scripts. It embeds [rel_lib](../rel-lib), which handels parsing and validation a REL model. Afterwards, the requirements data from the model can be accessed within Python3, to export it into different formats (e.g. [HTML](./html_export)) or to run project-specific validators.

A rudimentary [HTML export](./html_export) is part of the REL framework and can be used as example, how to use the Python3 module.

Python3 integration of REL is using [pybind11](https://github.com/pybind/pybind11) framework.

# How to build Python3 module for REL
```
bazel build //relpy:rel_py
``` 

# Run integration test of rel_py
```
bazel test //relpy/test:integration_test
```


# Getting Started

## Importing rel_py into own Python3 scripts

After building the Python3 module as described above, a Python3 dynamic library called `librel_py.so` is available. In order to import the library into your scripts, it either has to be copied into Python3's main module folders or Python3 is started within the directory, where the library is located. Please refer to the official [Python](https://www.python.org/) documentation for more information about how to import custom modules.

```
import librel_py as rel
```

## Creating Requirements Model

The module exposes three classes. `RelParser` is the main class, it encapsulates the whole REL library and is used to build up a requirements model from specification and data files. In order to create an object of `RelParser`, objects of the following two classes have to be provided:

- an object of class `Logger`, which defines the log level of the parser. Additionally, it allows sending the log output to stdout or storing it in a file.
- an object of class `FileEngine`, which defines the location of the files in the file system

### Logger

```
l = rel.Logger()
l.SetLogLevel(rel.LogLevel.INFO)
```

In this example, an object of class `Logger` is created. Log data is sent to stdout. If log data shall be written into file, a file path can be handed over to the constructor.
If necessary, custom log level can be set to ERROR, WARNING, INFO, and DBUG. Default is WARNING. Be aware, that DBUG produces loads of output, which slows down execution significiantly.

### FileEngine

```
fe = rel.FileEngine(l)
fe.SetSearchRecursive(True)
fe.SetStartDirectory("../../requirements")
```

Create object of class `FileEngine` and set the starting directory and whether it shall search for REL files recursively in all sub directories.


### Parsing the model
```
relparser = rel.RelParser(l, fe)
result = relparser.ProcessRelModel() 
```

To process the model, call `ProcessRelModel()`. If the model was created successfully, the method returns `rel.ParseResult.NoExceptionOccurred`. If there is an error in the model, e.g. syntax error or another violation, execution is stopped, an exception is raised and its evaluation is logged. Additionally, the method returns `rel.ParseResult.ExceptionOccurred`.

## Traversing the AST

```
data = relparser.GetDatabase()
```

The resulting data can be accessed via method `GetDatabase()`. It returns a list of [`RdFile`](../rel-lib/src/AST.h#L97) objects, which contain the data sets. Every object represents one file, consisting of another list of type instances.

Depending on your use case, you can then transfer this data into another file format or run custom validations.
