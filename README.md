# REL
Requirements Engineering Language - a domain specific language to handle requirements in large software projects.

The language consists of two parts:

The *Requirements Specification* (file extension _rs_) contains definitions of types and enumerations, which structure the requirements. For every project, it is necessary to define the requirements specification and adapt it to the project's needs.

The *Requirements Data* (file extension _rd_) contains the actual data. It consists of instances of the types defined in the specification, which carry the corresponding data.

For example, typically in the specification, a type *Requirement* is defined, which has a set of attributes, like the requirement text and a unique identifier. Within the data part, the type *Requirement* is instantiated multiple times, each instance containing the text of the requirement and its unique identifier.

To get a quick impression, have a look in folder [test](./test), which contains an artificial test project, describing the requirements of a smartphone. Or read the [requirements](./requirements) of REL itself, of course they are written in REL, too (eat your own dogfood!).


# how to build

Clone the repository. Build the C++ parser, which can be used to parse the test project or REL's requirements.

REL can be built with GCC. It requires C++17 support.

```
bazel build --copt="-std=c++17" --copt="-fconcepts" //RelC:relc
``` 

After successful compilation, run the binary with the following command.

```
bazel-bin/RelC/relc -r -v ./test/
```


# Test Environment
Folder [test](./test) contains an exemplary set of requirements for a smartphone device. The data set can be used to test the tools and also to get a feeling, how a large project with 1000s of requirements looks like. For the sake of simplicity, only a fraction of the requirements are properly formulated, the rest uses fake text.
