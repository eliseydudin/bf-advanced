project('bfa', ['c', 'cpp'], default_options: ['debug=true'])

sources = ['src/main.c', 'src/bfa.c']
dependencies = [dependency('llvm')]

inc = include_directories('.')

executable('bfa', sources, dependencies: dependencies, include_directories: inc)
