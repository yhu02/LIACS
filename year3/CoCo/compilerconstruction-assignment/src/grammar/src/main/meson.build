libgrammar_files += files(
    'cpp/types/types.cpp',
    'cpp/grammarbuilder/grammarbuilder.cpp',
    'cpp/generator.cpp',
    'cpp/debug/debug.cpp'
)
libgrammar_files += flex_gen.process('flex_bison/compiler.l')
libgrammar_files += bison_gen.process('flex_bison/compiler.y')