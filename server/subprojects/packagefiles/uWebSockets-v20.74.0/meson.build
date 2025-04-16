project('uWebSockets', 'c', 'cpp', version: 'v20.74.0', meson_version: '>=1.1')

uSockets_dep = dependency('uSockets')

uWebSockets_dep = declare_dependency(
  include_directories: include_directories('src'),
  dependencies: [uSockets_dep],
)
