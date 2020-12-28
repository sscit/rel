# Developer's Guide

## Introduction

Requirements engineering is a key part of software development. Without requirements, a project's goals are hard to define and it is not clear, when the software is actually finished. Additionally, as requirements are often provided from customers towards the development organization, they are essential to ensure that all parties have the same understanding of the product. Without requirements, projects often end up as shown in the famous cartoon, [the swing and the tree](https://blog.codefor.cash/2017/08/30/software-development-risks-the-tree-swing-risk/).

In safety critical software development projects, requirements engineering has even more responsiblity. It is not sufficient to simply describe the product. A comprehensive tracing from requirements towards software and tests is necessary, too, to prove that all parts of the source code are necessary and there is no code, that shouldn't be there. Every piece of code may introduces a bug, resulting in a safety issue, and therefore argumentation is required, why it is necessary.

The REL project provides the so called "REL framework", which enables requirements engineers to define a requirements model for their software project. It allows definition of a specification, which structures the actual requirements data, and extends the requirements text with additional attributes and links. Supported by the model, software engineers can then focus on writing their requirements plus all necessary meta data. Besides the underlying DSL, the REL framework also consists of a language server implementation, which can be used to get IDE support, while working with the text files. Additionally, python integration is available, so that requirements engineers can easily write custom data exporters and validation scripts.

## Key Features of the REL Framework

- [Domain Specific Language](../rel-lib/grammar), which allows defining a requirements model with types, links, and enumerations. This model is then filled with requirements data, namely text and necessary meta data. The domain specific language resides in text files, which can be stored on the file system or in a repository, which is the fundament of all well-establised agile working models for software development
- [Core C++ Library](../rel-lib), implementing a parser for the DSL and fundamental model checking. It is implemented in C++17, without external dependencies to other libraries. The library can be embedded into any environment that is capable of integrating C++ libraries.
- [Command Line Interface](../rel-cli), to validate a requirements model by parsing all files that belong to it. It can be used in CI environment, to do fundamental model checking with every commit.
- [Language Server Implementation](../rel-ls) and [Visual Studio Code](../vscode-ext) plugin, to support developers with a powerful IDE, while working with the DSL.
- [Python Integration](../relpy), so that requirements engineers can write project-specific exporters and data validation scripts.

In a nutshell, the REL framework shall enable requirements engineers to define a requirements model and the corresponding processes, which helps the whole team to focus on the actual work of writing valuable requirements. With the domain specific language as core element, well-known agile processes can be applied. The tooling provided in the framework is focusing on clear usecases, and always provides hooks for project-specific extensions. In an ideal world, requirements engineers use the REL framework to define tooling and processes upfront, so that developers can then focus on the creative work of writing requirements. 


## Not in Scope of the REL Framework

- The REL framework does not provide any guidance on **how** to write the actual requirement. English or German, semi-formal, or using HTML within the requirement text - these are project-specific decisions and there are tons of publications and books available, that elaborate on this topic. All tools within the framework can deal with unicode characters and line breaks within the requirements text, and therefore support all kinds of content.
- The REL framework does not provide any guidance about the processes, that embed requirements engineering into the software project. Processes are highly project-specific and for example tightly coupled to the safety classification of the final product. As the data resides in text files, all agile software development processes can be applied and tailored. REL's python integration allows an easy extension with project specific exporter or validation scripts. Integration into CI and automated tests can easily be developed.


## Getting Started

### Installation

To get more information about compiling and using the individual tools of the REL framework, please have a look at the corresponding README.md files.

Installation instructions of the Visual Studio Code extension can be found [here](../vscode-ext/README.md).

### Domain Specific Language Overview

The grammer of REL's domain specific language is defined [here](../rel-lib/grammar/grammar.g).

#### Requirements Specification

Within the requirements specification (file ending _.rs_), the requirements model is defined. It is possible to define custom types and enumerations.

```
type TypeName {
	attribute_name : attribute_type,
	attribute_name_2 : EnumType,
	...
}

enum EnumType {
	EnumValue1,
	EnumValue2,
	....
}
```

`attribute_type` can either be an enum type name or one of the following keywords:

| Keyword        | Description  |
| ------------- |-------------|
| `id`      | A unique identifier. The parser ensures that the string used as identifier within the data is unique among the whole project.  |
| `int`      | A signed integer value.      |
| `string` | A string value, encapsulated in quotation marks.      |
| `link` | Unique id of the requirement, the link points to. The parser ensures that all identifiers used within the link attribute exist in the model. |

#### Requirements Data

Requirements data files (file extension _.rd_) contain type instances of the `type`s defined in specification.

```
TypeName {
	attribute_name : value,
	attribute_name_2 : value,
	...
}
```

Depending on the `type`'s specification, `value` is either a signed integer, an identifier defining a unique id, a string, encapsulated in quotation marks, or an enum value.

See [REL's requirements](../requirements) or the [test project](../test) for more syntax examples.

### Designing the Requirements Specification

Designing and writing the requirements specification is the starting point, if the REL framework is used in a software development project. The following design guidelines help requirements engineers to cover all aspects in their project. It is also recommended to study the examples available in the project, namely [REL's own requirements](../requirements) and the [test project](../test).

At the beginning, the main type definitions are specified, which cover the majority of requirements data within the project. Requirements engineers should always think about the necessary attributes, which are required and make sense to support the processes. The more attributes, the more effort developers have to spend while creating type instances and filling them with data. Additionally, more attributes most likely lead to more efforts in terms of custom validations, that are executed in CI and are not covered by REL's basic validation (which only takes care about syntax and validity of type instances and their specification).

### Data Structuring

All data is stored in text files. This means that some kind of file and directory structure has to be defined, so that developers know where to put their content. It is important to define a suitable structure at the beginning, so that people can start working. Some aspects to consider:

- the REL parser processes all files in an alphabetical order. This behavior is important to know, because on the one hand, it ensures deterministic execution. On the other hand, sequence of files located in the repository has an impact, e.g. on data export. The sequence files and the data inside is parsed is reflected within the output artefacts, e.g. within a generated HTML file. This behavior is intended, because requirements are always categorized (at least by their files) or are maybe accompanied with explanatory paragraphs, which only make sense if the order is preserved.
- Requirements specification can be distributed among multiple files or can be kept in a single file. The parser always processes all specification files found within the directory tree first.
- It makes sense to split the actual requirements data into multiple files, grouped depending on their scope. A "category", if necessary, can either be implied by the file name or another approach would be to define a text field or enumeration, that defines the category.

### Custom Validation and Export Formats

Every project has its own needs in terms of custom validation and export formats. Therefore [rel-py](../rel-py) embeds REL into python, so that requirements engineers can write own scripts that operate on the model. For sample code and more explanation about the python integration, refer to the [README of rel-py](../rel-py/README.md).

For example, a custom valiator could check that a set of attributes is set consistently, or that special metrics applied to the requirement text (e.g. contains valid semi-formal formulas) are fulfilled. The REL parser itself already ensures the following:
- syntax of the language in all files is valid
- type instance matches to type definition within specification
- attributes defined as "id" are uniquely set among the whole model
- Enum attributes used in type instances exist
- the whole model can be read without errors

For all other use cases, own validators can be written in python.


The same applies for export formats: Excel, HTML, CSV, PDF, ... depending on the stakeholders and the size of the project, there are different needs. With custom python scripts, the requirements model can be transferred in every format desired.

### CI Integration

[rel-cli](../rel-cli) can be used to parse the model on command line. It checks syntax and compliance of the data to the specification. If rel-cli is integrated into CI, it can check every pull request, to ensure that the model is always consistent.

To validate the requirements in more detail, custom validators can be written in python, by using [rel-py](../rel-py). The resulting scripts can then be executed as part of the pull request validation, too.
