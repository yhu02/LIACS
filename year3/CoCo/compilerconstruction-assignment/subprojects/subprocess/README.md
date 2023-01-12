# subprocess
cross platform subprocess library for c++ similar to design of python
subprocess.
This library expands on [this](https://github.com/benman64/subprocess) open-source library.

See their [subprocess documentation](https://benman64.github.io/subprocess/index.html)
for further documentation.


# supports

- python-like style of subprocess. With very nice syntax for c++20.
- Connect output of process A to input of process B. However not pretty API for
  this.
- Environment utilities to make it easy to get/set environment variables. as
  easy as `subprocess::cenv["MY_VAR"] = "value"`.
- subprocess::EnvGuard that will save the environment and reload it when scope
  block ends, making it easy to have a temporary environment. Obviously this is
  not thread safe as environment variables are per-process, not per-thread.
- Get a copy of environment so you can modify a std::map as you please for use
  in a thread safe manner of environment and pass it along to subprocesses.
- cross-platform `find_program`
- `find_program` has special handling of "python3" on windows making it easy to
  find python3 executable. It searches the path for python and inspects it's
  version so that `find_program("python3")` is cross-platform.
- Supports connecting process stdin, stdout, stderr to C++ streams making
  redirection convenient. stdin can be connected with a std::string too.
- Supports timeouts on commands, for example 
  `subprocess::run({'sleep', '10'}, subprocess::RunBuilder().timeout(5))`

## Shakey elements

- The os error level exceptions is still changing. I'm thinking of having an
  OSError subclass to abstract the OS differences.

# requirements

- c++17
- linked with support for threading, filesystem

# Integration

##  Adhoc

1. copy files in src/cpp to your project.
2. add the top folder as include.
3. make sure cpp files are compiled.
4. add `#include <subprocess.hpp>` to start using in source files.

## Meson
To use the latest released subprocess wrap version, create `<root-project>/subprojects/subprocess.wrap`, containing:
```
[wrap-git]
url = https://github.com/Sebastiaan-Alvarez-Rodriguez/subprocess
revision = v0.4.0
```

To use older releases, check the instructions in the release notes [here](https://github.com/Sebastiaan-Alvarez-Rodriguez/subprojects/releases).

# Changelog

# 0.4.0

- Minimised build
- Changed build system to Meson