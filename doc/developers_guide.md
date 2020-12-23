# Developer's Guide

Requirements engineering is a key part of software development. Without requirements, a project's goals are hard to define and it is not clear, when the software is actually finished. Additionally, as requirements are often provided from customers towards the development organization, they are essential to ensure that all parties have the same understanding of the product. Without requirements, projects often end up as shown in the famous cartoon, [the swing and the tree](https://blog.codefor.cash/2017/08/30/software-development-risks-the-tree-swing-risk/).

In safety critical software development projects, requirements engineering has even more responsiblity. It it not sufficient to simply describe the product. A comprehensive tracing from requirements towards software and tests is necessary, too, to prove that all parts of the source code are necessary and there is no code, that shouldn't be there. Every piece of code may introduces a bug, resulting in a safety issue, and therefore argumentation is required, why it is necessary.

The REL project provides a framework to enable requirements engineers in defining a requirements model for their software project. It allows definition of a specification, which structures the actual requirements data, and extending the requirements text with additional attributes and links. Supported by the model, software engineers can then focus on writing their requirements plus all necessary meta data to fulfil the requirements process. Besides the underlying DSL, the REL framework also consists of a language server implementation, which can be used to get IDE support, while working with the text files. Additionally, REL is provided as python module, so that requirements engineers can easily write custom data exporters and validation scripts.

## Key Features of the REL Framework

- REL provides a domain specific language, which allows defining a requirements model with types, links, and enumerations. This model is then filled with requirements data, namely text and necessary meta data. The domain specific language resides in text files, which can be stored on the file system or in a repository, which is the fundament of all well-establised agile working models for software development
- [Core C++ Library](../rel-lib), implementing a parser for the REL DSL and fundamental model checking. It is implemented in C++17, without external dependencies to other libraries. Library can be embedded into any environment that supports this use case.
- [Command Line interface](../rel-cli), to validate a REL model by parsing all files that belong to it. It can be used in CI environment, to do fundamental model check with every commit
- [Language Server Implementation](../rel-ls) and [Visual Studio Code](../vscode-ext) plugin, to support developers with a powerful IDE, while working with REL
- [Python Integration](../rel-py), so that developers can write project-specific exporters and data validation.

In a nutshell, the REL framework shall enable requirements engineers to define a requirements model and the corresponding processes, which helps the whole team to focus on the actual work of writing valuable requirements. With the domain specific language as core element, well-known agile processes can be applied. The tooling provided in the framework is focusing on clear usecases, and always provides hooks for project-specific extensions. In an ideal world, requirements engineers use the REL framework to define tooling and processes upfront, so that developers can then focus on the creative work of writing requirements. 


## What is REL not about

- The REL framework does not provide any guidance on HOW to write the actual requirement. English or German, semi-formal, or using HTML within the requirement text - these are project-specific decisions and there are tons of publications and books available, that elaborate on this topic. All REL tools can deal with Unicode characters and line breaks within the requirements text, and therefore support all kinds of content.
- The REL framework does not provide any guidance about the processes, that embed requirements engineering into the software project. Processes are highly project-specific and for example depent on the safety classification of the project. As the data resides in text files, all agile software development processes can be applied and tailored. REL's python integration allows an easy extension with project specific exporter or validation scripts. Integration into CI and automatic tests can easily be developed.



## How to install and use REL in Visual Studio Code

Have a look at the [readme of the Visual Studio Code extension](../vscode-ext/README.md), on how to setup development environment for REL with Visual Studio Code.

