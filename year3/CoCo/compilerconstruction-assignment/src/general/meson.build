'''
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
'''

inc = include_directories('src/main/public')

libgeneral_files = []
subdir('src/main') # This adds all main source files

libgeneral = library('general', libgeneral_files, include_directories : inc, install : true)
libgeneral_dep = declare_dependency(include_directories : inc, link_with : libgeneral)