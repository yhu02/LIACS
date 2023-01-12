libmachinecode_depends = [libgeneral_dep, libgrammar_dep, libintermediate_dep, ghcfilesystem.get_variable('ghcfilesystem_dep')]
inc = include_directories('src/main/public')

libmachine_depends = [libgeneral_dep, libgrammar_dep, libintermediate_dep]

libmachine_files = []
subdir('src/main') # This adds all main source files

libmachine = shared_library('machine-code', libmachine_files, include_directories : inc, dependencies : libmachine_depends, install : true)
libmachine_dep = declare_dependency(include_directories : inc, link_with : libmachine, dependencies : libmachine_depends)

libmachine_exe = executable(
    'coco_compiler_machine_code',
    'src/main/cpp/entrypoint/main.cpp',
    dependencies: libmachine_depends + [tclap.get_variable('tclap_dep'), libmachine_dep],
    # build_rpath : prebuiltdir,
    # install_rpath : prebuiltdir,
    include_directories : inc,
    install : true)
