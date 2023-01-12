'''
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
'''

inc = include_directories('src/main/public')
libgrammar_depends = [libgeneral_dep, ghcfilesystem.get_variable('ghcfilesystem_dep')]

libgrammar_files = []
subdir('src/main') # This adds all main source files

libgrammar = shared_library('grammar', libgrammar_files, include_directories : inc, dependencies : libgrammar_depends, install : true)
libgrammar_dep = declare_dependency(include_directories : inc, link_with : libgrammar, dependencies : libgrammar_depends)