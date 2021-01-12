# Visual Studio Code Extension for REL - Requirements Engineering Language

Extension for Visual Studio Code, which enables support for [REL](https://github.com/sscit/rel).

## Features

The extension includes fundamental syntax highlighting and coloring of REL. Additionally, it embeds the language server of REL into Visual Studio Code.

## Setup Language Server for REL

The extension expects the language server binary in folder `~/bin/rel-ls/` within your home directory. To change this path, you have to modify file [extension.js](./extension.js#L6].

Description how to build the language server for REL from source code is available on Github: [https://github.com/sscit/rel/tree/main/rel-ls](https://github.com/sscit/rel/tree/main/rel-ls)

## How to install extension in Visual Studio Code

- clone repository https://github.com/sscit/rel
- Build language server with bazel and copy binary into `~/bin/rel-ls/`
- download latest vsix package of this extension from Github and install it in Visual Studio Code via "Install from VSIX..." in the extensions menu.

## Release Notes

### v0.6.0

v0.6 of REL framework does not contain any changes on the Visual Studio Code Extension for REL. VSIX package of 0.5.0 can still be used.

### 0.5.0

Initial release of Visual Studio Code Extension for REL.

Link to release and VSIX: https://github.com/sscit/rel/releases/tag/0.5.0
