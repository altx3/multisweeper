project('multisweeper-server', 'cpp',
         version : '0.0.1',
         default_options : ['cpp_std=c++17'],
         meson_version: '>=1.1')

# Dependencies

uWebSockets_dep = dependency('uWebSockets')
nlohmann_json_dep = dependency('nlohmann_json')

# Multisweeper++

inc = include_directories('include')

src = [
    'src/http_handler.cpp',
    'src/lobby_manager.cpp',
    'src/lobby.cpp',
    'src/logger.cpp',
    'src/main.cpp',
    'src/player.cpp',
    'src/uuid.cpp',
    'src/websocket_handler.cpp'
]

executable('multisweeper-server', src,
            include_directories : inc,
            dependencies: [uWebSockets_dep, nlohmann_json_dep])
