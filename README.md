# REL
Requirements Engineering Language - a domain specific language to handle requirements in large software projects. It is used to create a REL model, which consists of the model's specification and the actual requirements data.

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

For more examples, have a look at the [Developers Guide](./doc/developers_guide.md), for a more detailed documentation of the language.

Additionally, the following projects give an impression of the language on a larger scale:
 - folder [test](./test), which contains an artificial test project, describing the requirements of a smartphone. 
 - Have a look at the [requirements](./requirements) of REL itself, of course they are written in REL, too (eat your own dogfood!).


# Development Environment

Folder [rel-lib](./rel-lib) contains the C++ library that implements lexer and parser for REL. It also contains the [unit tests](./rel-lib/test) for the library. In [rel-cli](./rel-cli), a basic command line interface can be found for rel-lib, to execute REL on the command line and parse a REL model consisting of requirements specification and requirements data files.

In [test](./test), a test data set is located, representing an artifical project.

Folder [rel-ls](./rel-ls) contains the Language Server for REL, which can for example be used with [Visual Studio Code](https://code.visualstudio.com/), for syntax highlighting, online validation, etc. The corresponding Visual Studio Code extension is located in folder [vscode-ext](./vscode-ext).

Build environment to build and run tests is [Bazel](https://bazel.build)

# How to build REL and use it

REL can be built with GCC. It requires C++17 support.

Clone the repository. Build the [REL command line interface](./rel-cli), which can be used to parse the test project or REL's requirements.

```
bazel build --config=default //rel-cli:rel_cli
``` 

After successful compilation, run the binary with the following command.

```
bazel-bin/rel-cli/rel_cli -r -v ./test/
```

Compilation, integration and usage of Language Server for REL is described [here](./rel-ls/README.md)

# Test Environment

## Unit Tests

Run unit tests of REL library:

```
bazel test --config=default //rel-lib/test/unittest:RelLibUnitTest
```

Run all unit tests in the project:

```
bazel test --config=default ...
```


## Test Data Set
Folder [test](./test) contains an exemplary set of requirements for a smartphone device. The data set can be used to test the tools and also to get a feeling, how a large project with 1000s of requirements looks like. For the sake of simplicity, only a fraction of the requirements are properly formulated, the rest uses fake text.
