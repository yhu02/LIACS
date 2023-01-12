libintermediate_depends = [libgeneral_dep, libgrammar_dep, ghcfilesystem.get_variable('ghcfilesystem_dep')]
inc = include_directories('src/main/public')

libintermediate_depends = [libgeneral_dep, libgrammar_dep]

libintermediate_files = []
subdir('src/main') # This adds all main source files

libintermediate = shared_library('intermediate-code', libintermediate_files, include_directories : inc, dependencies : libintermediate_depends, install : true)
libintermediate_dep = declare_dependency(include_directories : inc, link_with : libintermediate, dependencies : libintermediate_depends)
