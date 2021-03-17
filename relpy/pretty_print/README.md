# REL Source Code Formatting Script

The python script reads a REL model and prints out the requirements data again into the same files. It can be used to
pretty-print a REL requirements data model, e.g. align coding conventions and formatting. Additionally, as it touches every type instance and attribute of the model, it can be easily adapted and add custom code to apply bulk changes.

Note that comments within the files are lost during the parsing and writing the data back, as comments are not represented within the model.

## Run Script with Bazel

```
bazel run //relpy/pretty_print:pprint
```