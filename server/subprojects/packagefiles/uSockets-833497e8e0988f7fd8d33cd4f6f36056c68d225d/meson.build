project('uSockets', 'c', 'cpp', version: '833497e8e0988f7fd8d33cd4f6f36056c68d225d', meson_version: '>=1.1')

libuv_dep = dependency('libuv', required: true)
zlib_dep  = dependency('zlib',  required: true)

uSockets_src = [
  'src/bsd.c',
  'src/context.c',
  'src/loop.c',
  'src/quic.c',           # QUIC support
  'src/socket.c',
  'src/udp.c',
  'src/crypto/openssl.c', # OpenSSL support
  'src/crypto/sni_tree.cpp',
  'src/eventing/asio.cpp',
  'src/eventing/epoll_kqueue.c',
  'src/eventing/gcd.c',
  'src/eventing/libuv.c', # libuv backend
  'src/io_uring/io_context.c',
  'src/io_uring/io_loop.c',
  'src/io_uring/io_socket.c'
]

ssl_opt       = get_option('ssl')
boringssl_dep = dependency('boringssl', required: ssl_opt == 'boringssl')
openssl_dep   = dependency('openssl',   required: ssl_opt == 'openssl')
wolfssl_dep   = dependency('wolfssl',   required: ssl_opt == 'wolfssl')
auto_dep      = dependency('', required: false)
foreach dep: [wolfssl_dep, openssl_dep, boringssl_dep]
  if dep.found()
    auto_dep = dep
  endif
endforeach

ssl_dep = {
  'auto':      auto_dep,
  'boringssl': boringssl_dep,
  'openssl':   openssl_dep,
  'wolfssl':   wolfssl_dep,
  'none':      [],
}[ssl_opt]

c_args = []
if ssl_dep == []
  c_args += ['-DLIBUS_NO_SSL']
endif
# default_library=static
#   library() static_library()
# default_library=shared
#   library() shared_library()
# default_library=both
#   library() both_library()

uSockets_lib = library(
  'uSockets',
  uSockets_src,
  include_directories: include_directories('src'),
  c_args: c_args,
  dependencies: [libuv_dep, zlib_dep, ssl_dep],)

uSockets_dep = declare_dependency(
  link_with: uSockets_lib,
  include_directories: include_directories('src', 'src/crypto', 'src/crypto', 'src/eventing', 'src/internal', 'src/internal/eventing', 'src/internal/networking'),
  dependencies: [libuv_dep, zlib_dep, ssl_dep],
)
