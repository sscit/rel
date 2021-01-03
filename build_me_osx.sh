clear

#local path to bazel
BAZEL='/users/juancarlosfuentesmichel/bin/bazel'

# clean the environment
$BAZEL clean --expunge

# build as a c++ only project
#$BAZEL build --config=macos -c dbg --action_env=BAZEL_USE_CPP_ONLY_TOOLCHAIN=1 //rel-cli:rel_cli --sandbox_debug --verbose_failures   

# build as required c/c++
$BAZEL build --config=macos -c dbg //rel-cli:rel_cli --sandbox_debug --verbose_failures
