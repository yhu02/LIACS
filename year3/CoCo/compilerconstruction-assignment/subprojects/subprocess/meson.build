project('subprocess', 'cpp',  version : '0.4.0',  default_options : ['warning_level=0'])

inc = include_directories('src/cpp')


subprocess_files = []
subdir('src')

mpark_variant = subproject('mpark_variant')
ghcfilesystem = subproject('filesystem')

mpark_variant_dep = mpark_variant.get_variable('mpark_variant_dep')
ghcfilesystem_dep = ghcfilesystem.get_variable('ghcfilesystem_dep')
thread_dep = dependency('threads')

subprocess_depends = [mpark_variant_dep, ghcfilesystem_dep, thread_dep]


libsubprocess = shared_library(
    'subprocess',
    subprocess_files,
    include_directories : inc,
    dependencies : subprocess_depends,
    install : true)

subprocess_dep = declare_dependency(include_directories : inc, link_with : libsubprocess, dependencies: subprocess_depends)