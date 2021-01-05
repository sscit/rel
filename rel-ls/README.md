# REL Language Server

Language Server for Requirements Engineering Language, implementing [Language Server Protocol (LSP)](https://microsoft.github.io/language-server-protocol/)

REL Language Server has been tested with [Visual Studio Code](https://code.visualstudio.com/)

## Supported Version and Implementation Status

Currently supported version of LSP: [3.15](https://microsoft.github.io/language-server-protocol/specifications/specification-3-15/)

## Implementation Status of Features

| Feature         | Status      |
| -------------   |-------------|
| Diagnostics     | implemented |
| Auto Completion | Planned     |


# How to build REL Language Server

Build language server binary with the following bazel command:

```
bazel build //rel-ls:rel_ls
``` 

# Test Environment

Unit Tests:

```
bazel test //rel-ls/test/unittest:RelLsUnitTest
``` 