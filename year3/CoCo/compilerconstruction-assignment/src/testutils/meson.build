'''
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
'''

inc = include_directories('src/main/public')

gtest_dep = gtest.get_variable('gtest_dep')
pico_sha2_dep = pico_sha2.get_variable('pico_sha2_dep')
ghcfilesystem_dep = ghcfilesystem.get_variable('ghcfilesystem_dep')
subprocess_dep = subprocess.get_variable('subprocess_dep')
libtestutils_depends = [gtest_dep, ghcfilesystem_dep, libgeneral_dep, pico_sha2_dep, subprocess_dep, libmachine_dep]


libtestutils_files = []
subdir('src/main') # This adds all main source files

libtestutils = library('testutils', libtestutils_files, dependencies: libtestutils_depends, include_directories : inc, install : true)
libtestutils_dep = declare_dependency(include_directories : inc, link_with : libtestutils, dependencies: libtestutils_depends)

lib_test_exe = executable(
    'coco_compiler_tests',
    'src/main/cpp/main.cpp',
    dependencies: libtestutils_depends,
    include_directories : inc,
    #build_rpath : build_rpath,
    #install_rpath : install_rpath,
    install : true)