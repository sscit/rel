# rel_cli - Command Line Interface for REL

## Command Line Options

```
rel_cli [-r] [-v|-vv] [<PATH>]+
```
| Option        | Description   |
| ------------- |-------------|
| -r      | if specified, folder provided is processed recursively, to find all underlying specification or data files. |
| -v      | print LogLevel::INFO messages during execution   |
| -vv     | print LogLevel::DBUG messages during execution. This option has significant impact on runtime.   |
| \[\<PATH\>\]+     | Paths to folders, separated by spaces, where relevant files are located, that contain the REL model's data. |


# How to build rel_cli

```
bazel build //rel-cli:rel_cli
``` 

# Test Environment

Run unittests:
```
bazel test //rel-cli/test/unittest:RelCliUnitTest
``` 

Process big test data set:
```
bazel-bin/rel-cli/rel_cli -r -v ./test/big
``` 