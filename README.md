# libHeader

libHeader is a self-contained C++ cross-platform library for generating Objective-C headers from the raw strings embedded in the binary.

You are responsible for finding those strings and passing them to libHeader. libHeader turns the ugly typestrings, attributes and selectors into beautiful objective-c declarations that would make (objc inventor here) cry tears of joy.

## using libHeader in your project

it is extremely easy. See headerTest.cpp for the very short set of example invocations.

### Compiling with your project

There are like two files. You can submodule the repo, can just copy them in (license allows this without any headache), they require no fancy compilation stuff and should "just work"

