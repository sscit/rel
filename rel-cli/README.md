# rel_cli - Command Line Interface for REL

## Command Line Options

```
rel_cli [-r] [-v|-vv] <PATH>
```
| Option        | Description   |
| ------------- |-------------|
| -r      | if specified, folder provided is processed recursively, to find all underlying specification or data files. |
| -v      | print INFO messages during execution   |
| -vv     | print DEBUG messages during execution. This option has significant impact on runtime.   |
| \<PATH\>     | Path to folder, where REL model is located. |


# How to build rel_cli

```
bazel build --config=default  //rel-cli:rel_cli
``` 

# Test Environment

Run unittests:
```
bazel test --config=default //rel-cli/test/unittest:RelCliUnitTest
``` 

Process test data set:
```
bazel-bin/rel-cli/rel_cli -r -v ./test/
``` 