project('multisweeper-server', 'cpp',
         version : '0.0.1',
         default_options : ['cpp_std=c++17'],
         meson_version: '>=1.1')

# Dependencies

uWebSockets_dep = dependency('uWebSockets')

# Multisweeper++

inc = include_directories('include')

src = [
    'src/main.cpp',
    'src/utils/logger.cpp'
]

executable('multisweeper-server', src,
            include_directories : inc,
            dependencies: [uWebSockets_dep])
