# Meson
Meson is a modern, speedy, platform-independent, incremental compiler.
In Meson, we use a python-like syntax to define sources, dependencies, generators etc etc.
In this Section, we explain the basics of using Meson.

## Installing Meson
Installing Meson is simple, and requires no root/sudo/su privileges.
See [mesonbuild.com](https://mesonbuild.com/SimpleStart.html#installing-meson) for installation instructions for all popular platforms.
Below, we provide information on installing Meson on Linux machines.


### Installing Meson - Linux
The easiest way to install Meson on Linux is by using pip:
```bash
pip3 install meson ninja --user
```
Installing through this method ensures we have the right version of `meson`.
If you choose another installation method, please verify you have at least 
version 0.59.1.
> **Note**: The `--user` flag installs given packages in user-mode, meaning we do not need superuser privileges.

After installation, the required executables (called `meson` and `ninja`) exist in `~/.local/bin`.
> **Note**: Don't forget to add `~/.local/bin` to your `$PATH` environment variable.
> By doing that, you can execute simply `meson <command>` and `ninja <command>` from your terminal.



## Instantiating Meson
We instantiate this project (only required once) by executing:
```bash
mkdir build
cd build
meson ..
```
Meson initializes itself, generates build files, executables, libraries etc in the `build/` dir.

> **Note**: At this point we expect you to have `flex` and `bison` installed


## Building with Meson
To compile this framework, execute (from `build/`):
```bash
meson
ninja
```

`meson` generates the compilation files with the default configuration options.
All configuration options are available in [`meson-options.txt`](/meson_options.txt).
Print the current configuration using:
```bash
meson configure
```
The options printed at the bottom are defined by this project.

To set options **initially**, use `-Doption=value`, e.g:
```bash
meson -Dwith-assignment=2
```

**After initial configuration**, every time you wish to change compilation options, you should regenerate the compilation files using:
```bash
meson -Doption1=value1 -Doption2=value2 ... -DoptionN=valueN --reconfigure
```
After (re)building the compilation files, (re)compile using `ninja`.



## Configuring Project Options
Within this project, there are 2 configurations which we may set. 
The first, `with-builddir`, changes the build-directory (relative to the root-directory) to use when compiling.
We expected this option will never need to change.

The other configuration, `with-assignment`, switches the current-working assignment. 
This configuration defines which assignments are activated.
E.g., when starting assignment 2, you will need to change this configuration, with:
```bash
meson -Dwith-assignment=2 --reconfigure
```

After doing so, please (re)compile with:
```bash 
ninja clean; ninja
```

## Executable and Library Locations
Compiled executables and libraries can be found in `build/`. The exact locations for each binary:
* machinecode:
   - `build/src/machinecode/coco_compiler_machine_code`
  
  Use to generate x86-64 assembly from a C-minus input-program
* testutils:
   - `build/src/testutils/coco_compiler_tests`
  
  Use to test the compiler


## Adding Source Files
Suppose we are working on the `grammar` module, and we want to add an additional `example.h` file.
We don't have to do anything with Meson here to get it compiled.
Instead, we just need to include the header in a part of the project that does get compiled.
> **Note**: It is not allowed to add headers to `/src/<module>/src/main/public/` for any module.

Now suppose we are adding a matching `example.cpp` file for our header.
That file is **not** compiled by Meson, because Meson does not know about it.
We need to add it to the source files for the `grammar` module.
To do so, we update `/src/grammar/src/main/meson.build` to contain the following:
```python
libgrammar_files += files(
    'cpp/example.cpp', # <-------- This line was added to add our new cpp file.
    'cpp/types/types.cpp',
    'cpp/grammarbuilder/grammarbuilder.cpp',
    'cpp/passes/typepass/typepass.cpp',
    'cpp/generator.cpp',
    'cpp/debug/debug.cpp'
```
This is all we have to do to add a source file for the `grammar` module.
It works equivalently for all other modules.

Students are allowed to make these changes to the build system.


## Cleaning
To clean up object files etcetera, use:
```bash
ninja clean
```


## IDE Integration
Meson, being the 'new cool' player in the field, is often not supported by IDE's,
which only support older, more established compilation systems such as Cmake and Make.

This is a problem easily solved.
After configuring `meson` and compiling once using `ninja`, there is a file called `build/compile_commands.json`.
Many popular IDEs (IntelIJ Clion, VSCode etc) all can open this file as a project.
When doing this, the IDE will understand the Meson project and behave normally.
There are several tricks to make it work even better!

Check out:
1. [Clion](https://blog.jetbrains.com/clion/2021/01/working-with-meson-in-clion-using-compilation-db/)
2. [Meson with Visual Studio and XCode](https://mesonbuild.com/IDE-integration.html)
3. [Meson plugin for Visual Studio](https://marketplace.visualstudio.com/items?itemName=asabil.meson)

Out of these links, we only tested and extensively used the first one, since we develop in Clion.
Let us know how you got Meson to work with your favorite IDE!



## FAQ
1. **Q: I have multiple compilers on my system, and Meson picks the wrong one. How can I let Meson use compiler `X`?**  
   A: Specify the compiler to use with the `CXX` environment variable. Point `CXX` to your compiler, using an absolute path.

Missing a question here? Let us know. We will try to answer it, and we add it here if enough people have this question.