# rel
Requirements Engineering Language


# how to build

```
bazel build --copt="-std=c++17" --copt="-fconcepts" //RelC:relc
``` 


# how to run

```
bazel-bin/RelC/relc -r -vv ./requirements/
```
