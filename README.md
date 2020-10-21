# rel
Requirements Engineering Language


# how to build

```
bazel build --copt="-std=c++17" --copt="-fconcepts" //RelC:relc
``` 


# how to run

```
bazel-bin/RelC/relc -r -v ./test/
```


# Test Environment
Folder [test](./test) contains an exemplary set of requirements for a smartphone device. The data set can be used to test the tools and also to get a felling, how a large project with 1000s of requirements looks like. For the sake of simplicity, only a fraction of the requirements are properly formulated, the rest uses fake text.
