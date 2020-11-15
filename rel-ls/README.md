# REL Language Server

Language Server for Requirements Engineering Language, implementing [Language Server Protocol (LSP)](https://microsoft.github.io/language-server-protocol/)

REL Language Server has been tested with [Visual Studio Code](https://code.visualstudio.com/)

## Supported Version and Implementation Status

Currently supported version of LSP: [3.15](https://microsoft.github.io/language-server-protocol/specifications/specification-3-15/)

## Implementation Status of Features

# How to build and use REL Language Server with Visual Studio Code

Build language server binary with the following bazel command:

```
bazel build --config=default  //rel-ls:rel_ls
``` 