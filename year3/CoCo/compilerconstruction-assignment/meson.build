project('coco-assignments', 'cpp', version : '1.0', default_options : ['warning_level=3', 'c_std=c11', 'cpp_std=c++17'])

# We shut up gcc here about annoying gcc-only 'warnings'.
if meson.get_compiler('cpp').get_id() == 'gcc'
    add_global_arguments('-Wno-deprecated-copy', language: ['c', 'cpp'])
endif

assignment = get_option('with-assignment')
add_global_arguments('-DASSIGNMENT=@0@'.format(assignment), language: ['c', 'cpp'])

build_system = build_machine.system() # Name of the system we build on.

gtest = subproject('gtest')
tclap = subproject('tclap-1.4.0-rc1_vendored')
ghcfilesystem = subproject('filesystem')
subprocess = subproject('subprocess')
pico_sha2 = subproject('pico_sha2_vendored')


if build_system == 'windows'
  subproject('win-flex-bison-binaries') # Install flex/bison on Windows (hopefully).
endif

# Generators
flex = find_program('flex', required: true)
flex_gen = generator(
    flex,
    output: ['flex.@BASENAME@.cpp', 'flex.@BASENAME@.h'],
    arguments: [ '--outfile=@OUTPUT0@', '--header-file=@OUTPUT1@', '@INPUT@'])

bison = find_program('bison', required: true)
args =  [ '-Wall', '-v', '--output=@OUTPUT0@', '--defines=@OUTPUT1@', '@INPUT@' ]
bison_gen = generator(
    bison,
    output: ['bison.@BASENAME@.cpp', 'bison.@BASENAME@.h'],
    arguments: args)

# Variables
## Global variables
builddir_name = get_option('with-builddir')
builddir_rpath_own = join_paths('$ORIGIN', '..', '..')

## Subproject variables
builddir = join_paths(meson.source_root(), builddir_name)

# binaries
subdir('src/general')
subdir('src/grammar')
subdir('src/intermediate-code')
subdir('src/machine-code')
subdir('src/testutils')