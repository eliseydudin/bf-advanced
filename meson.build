project('fkys', ['c', 'cpp'], default_options: ['debug=true'])

sources = ['src/main.c', 'src/fkys.c']
dependencies = [dependency('llvm')]

inc = include_directories('.')

executable('fkys', sources, dependencies: dependencies, include_directories: inc)
