# REL - Requirements Engineering Language
The REL framework includes a domain specific language to handle requirements in large software projects. It is used to create a requirements model, which consists of the specification and the actual requirements data.

The *Requirements Specification* (file extension _rs_) contains definitions of types and enumerations, which structure the requirements. For every project, it is necessary to define the requirements specification and adapt it to the project's needs.

The *Requirements Data* (file extension _rd_) contains the actual data. It consists of instances of the types defined in the specification, which carry the corresponding data.

## Example

Requirements Specification `spec.rs`:
```
type Requirement
{
    Unique_Id : id,
    Text : string,
    /* Defines the state of the requirement,
       whether it is already accepted by
       the product owner. */
    State : RequirementState,
}

type Information
{
    Unique_Id : id,
    Text : string,
}

enum RequirementState 
{
    Draft,
    In_Discussion,
    Accepted,
    Rejected,
}

```
Typically in the specification, a type *Requirement* is defined, which has a set of attributes, like the requirement text and a unique identifier. Additionally, enumerations can be used for further attributes and their discrete values.


Requirements Data `req_data.rd`:
```
Requirement
{
    Unique_Id : dsl1,
    Text : "The REL language shall support comments in both
    specification and data files.",
    State : Accepted,
}
```

Within the data part, the type *Requirement* is instantiated multiple times, each instance containing the text of the requirement, its unique identifier and the current state of the requirement.

# REL Framework

The following libraries and tools are part of the REL framework:

- [rel_lib](./rel-lib) - library containing the implementation of REL: Parser and model validation is located there. The library can be embedded into different tool environments.

- [rel_cli](./rel-cli) - command line interface for REL, to parse a requirements model and check its validity.

- [rel_ls](./rel-ls) - language server implementation for REL, which can be used in IDEs supporting the [Language Server Protocol (LSP)](https://microsoft.github.io/language-server-protocol/), to get syntax highlighting, online validation and more for REL within the IDE.

- [vscode-ext](./vscode-ext) - Extension for [Visual Studio Code](https://code.visualstudio.com/), using the language server implementation to support REL in this IDE.

- [rel_py](./relpy) - python3 integration of REL, so that developers can use python language to write project-specific data exporters and validators for their requirements model.

## Development Enviroment of REL Framework

The REL project uses [bazel](https://bazel.build) as build environment.

Branch `main` is protected and pushes directly to the branch will be rejected. Create a feature branch and pull request, to get changes into `main`.

Every PR that is supposed to go into `main` is validated in CI via [Github Action](../.github/workflows/main.jml). The action builds all libraries and binaries of the framework, runs the unit tests and validates [REL's requirements](./requirements) with [rel_cli](./rel-cli)

# Getting Started

## Getting Started Video

[![REL Getting Started](http://img.youtube.com/vi/Utb5rT9Ahug/0.jpg)](http://www.youtube.com/watch?v=Utb5rT9Ahug "REL Getting Started")

## Prerequisites

The following tools are required to build and run REL:

- Linux OS (REL framework has been tested with Ubuntu 20.04)
- [Bazel](https://bazel.build) build environment (>=3.7)
- C++ Compiler supporting C++17
- Python 3.8 (to write custom Python scripts using REL python module)
- Visual Studio Code (>=1.50) for IDE support via language server

## Validate REL framework requirements

To process [REL framework requirements](./requirements), execute the following steps:

1. Clone this repository from Github
2. Build [REL command line interface](./rel-cli):
    - on Linux OS `bazel build --config=default //rel-cli:rel_cli`
    - on macOS `bazel build --config=macos //rel-cli:rel_cli`
3. Run the binary with the following command: `bazel-bin/rel-cli/rel_cli -r -v ./requirements/`

# Further Documentation

- [Developers Guide](./doc/developers_guide.md): Describes typical use cases and how to make the most out of REL for software projects.

Additionally, the following projects give an impression of the language on a larger scale:
 - folder [test](./test), which contains an artificial test project, describing the requirements of a smartphone. 
 - Have a look at the [requirements](./requirements) of REL itself, of course they are written in REL, too (eat your own dogfood!).
