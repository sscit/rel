when trying to compile it

juancarlosfuentesmichel@JCs-MB-Pro rel % /users/juancarlosfuentesmichel/bin/bazel build --cxxopt='-std=c++17' //rel-cli:rel_cli --sandbox_debug

 I get the following error:

  /usr/bin/sandbox-exec -f /private/var/tmp/_bazel_juancarlosfuentesmichel/e17defa056c0f48266499c65242dc0c9/sandbox/darwin-sandbox/253/sandbox.sb /var/tmp/_bazel_juancarlosfuentesmichel/install/e5700781a43090e74aa4012c7e525e2a/process-wrapper '--timeout=0' '--kill_delay=15' external/local_config_cc/cc_wrapper.sh -U_FORTIFY_SOURCE -fstack-protector -Wall -Wthread-safety -Wself-assign -fcolor-diagnostics -fno-omit-frame-pointer 
  '-std=c++0x'
   -MD -MF bazel-out/darwin-fastbuild/bin/rel-cli/_objs/rel_cli/rel_cli.pic.d '-frandom-seed=bazel-out/darwin-fastbuild/bin/rel-cli/_objs/rel_cli/rel_cli.pic.o' -fPIC -iquote . -iquote bazel-out/darwin-fastbuild/bin -iquote external/bazel_tools -iquote bazel-out/darwin-fastbuild/bin/external/bazel_tools -isystem rel-lib/src -isystem bazel-out/darwin-fastbuild/bin/rel-lib/src 
   '-std=c++17' 
   -no-canonical-prefixes -Wno-builtin-macro-redefined '-D__DATE__="redacted"' '-D__TIMESTAMP__="redacted"' '-D__TIME__="redacted"' -c rel-cli/src/rel_cli.cpp -o bazel-out/darwin-fastbuild/bin/rel-cli/_objs/rel_cli/rel_cli.pic.o)
In file included from rel-cli/src/rel_cli.cpp:7:
rel-lib/src/FileEngine.h:33:30: error: 'auto' not allowed in function prototype
    void CreateFileTokenData(auto const&entry, std::map<std::string, DataType>&,

    mark that the std version is set twice once with '-std=c++0x' and later with '-std=c++17' 

the error is the same when using 

juancarlosfuentesmichel@JCs-MB-Pro rel % /users/juancarlosfuentesmichel/bin/bazel build //rel-cli:rel_cli --sandbox_debug 

I assume the error is caused by the clang compiler using the c++0x standard instead of the c++17 (I assume)

fix:
how to tell bazel to use the std=c++17?


-----

  /usr/bin/sandbox-exec -f /private/var/tmp/_bazel_juancarlosfuentesmichel/e17defa056c0f48266499c65242dc0c9/sandbox/darwin-sandbox/291/sandbox.sb /var/tmp/_bazel_juancarlosfuentesmichel/install/e5700781a43090e74aa4012c7e525e2a/process-wrapper '--timeout=0' '--kill_delay=15' external/local_config_cc/cc_wrapper.sh -U_FORTIFY_SOURCE -fstack-protector -Wall -Wthread-safety -Wself-assign -fcolor-diagnostics -fno-omit-frame-pointer '-std=c++0x' -MD -MF bazel-out/darwin-fastbuild/bin/rel-lib/_objs/rel_lib/RelParser.pic.d '-frandom-seed=bazel-out/darwin-fastbuild/bin/rel-lib/_objs/rel_lib/RelParser.pic.o' -fPIC -iquote . -iquote bazel-out/darwin-fastbuild/bin -isystem rel-lib/src -isystem bazel-out/darwin-fastbuild/bin/rel-lib/src -no-canonical-prefixes -Wno-builtin-macro-redefined '-D__DATE__="redacted"' '-D__TIMESTAMP__="redacted"' '-D__TIME__="redacted"' -c rel-lib/src/RelParser.cpp -o bazel-out/darwin-fastbuild/bin/rel-lib/_objs/rel_lib/RelParser.pic.o) sandbox-exec failed: error executing command 