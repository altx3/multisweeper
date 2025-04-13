project('multisweeper-server', 'cpp',
        version : '0.0.1',
        default_options : ['cpp_std=c++20', 'default_library=static'])

inc = include_directories('include')

src = [
    'src/main.cpp',
    'src/utils/logger.cpp'
]

executable('multisweeper-server', src,
           include_directories : inc)
