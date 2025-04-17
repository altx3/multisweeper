project('uSockets', 'c', 'cpp', version: '833497e8e0988f7fd8d33cd4f6f36056c68d225d', meson_version: '>=1.1')

zlib_dep = dependency('zlib',  required: true)

uSockets_src = [
  'src/bsd.c',
  'src/context.c',
  'src/loop.c',
  'src/quic.c',           # QUIC support
  'src/socket.c',
  'src/udp.c',
  'src/crypto/openssl.c', # OpenSSL support
  'src/eventing/epoll_kqueue.c',
  'src/eventing/gcd.c',
  'src/eventing/libuv.c', # libuv backend
  'src/io_uring/io_context.c',
  'src/io_uring/io_loop.c',
  'src/io_uring/io_socket.c'
]

c_args = []

# SSL configuration

ssl_opt = get_option('ssl')
boringssl_dep = dependency('boringssl', required: ssl_opt == 'boringssl')
openssl_dep = dependency('openssl',   required: ssl_opt == 'openssl')
wolfssl_dep = dependency('wolfssl',   required: ssl_opt == 'wolfssl')
auto_dep = dependency('', required: false)
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
  'none':      dependency('', required: false),
}[ssl_opt]

if ssl_opt == 'boringssl' or ssl_opt == 'openssl' or auto_dep.name() == 'openssl' or auto_dep.name() == 'boringssl'
  uSockets_src += ['src/crypto/sni_tree.cpp']
endif

if ssl_opt == 'boringssl' or auto_dep.name() == 'boringssl'
  c_args += ['-Iboringssl/include', '-pthread', '-DLIBUS_USE_OPENSSL']
endif

if ssl_opt == 'openssl' or auto_dep.name() == 'openssl
  c_args += ['-DLIBUS_USE_OPENSSL']
endif

if ssl_opt == 'wolfssl' or auto_dep.name() == 'wolfssl
  c_args += ['-DLIBUS_USE_WOLFSSL', '-I/usr/local/include']
endif

if not ssl_dep.found()
  c_args += ['-DLIBUS_NO_SSL']
endif

# io_uring configuration

io_uring_dep = dependency('', required: false)
if get_option('io_uring')
  io_uring_dep = dependency('io_uring', required: true)
  c_args += ['-DLIBUS_USE_IO_URING']
endif

# libuv configuration

libuv_dep = dependency('', required: false)
if get_option('libuv')
  libuv_dep = dependency('libuv', required: true)
  c_args += ['-DLIBUS_USE_LIBUV']
endif

# ASIO configuration

asio_dep = dependency('', required: false)
if get_option('asio')
  asio_dep = dependency('asio', required: true)
  c_args += ['-DLIBUS_USE_ASIO']
endif

# GCD configuration

gcd_dep = dependency('', required: false)
if get_option('gcd')
  gcd_dep = dependency('gcd', required: true)
  c_args += ['-DLIBUS_USE_GCD']
endif

# asan configuration

if get_option('asan')
  c_args += ['--fsanitize=address', '-g']
endif

# QUIC configuration

quic_dep = dependency('', required: false)
if get_option('quic')
  quic_dep = dependency('quic', required: true)
  c_args += ['-DLIBUS_USE_QUIC', '-pthread','-std=c11', '-Isrc', '-Ilsquic/include']
endif

uSockets_lib = library(
  'uSockets',
  uSockets_src,
  include_directories: include_directories('src'),
  c_args: c_args,
  dependencies: [zlib_dep, ssl_dep, io_uring_dep, libuv_dep, asio_dep, gcd_dep, quic_dep],)

uSockets_dep = declare_dependency(
  link_with: uSockets_lib,
  include_directories: include_directories('src', 'src/crypto', 'src/crypto', 'src/eventing', 'src/internal', 'src/internal/eventing', 'src/internal/networking'),
  dependencies: [zlib_dep, ssl_dep, io_uring_dep, libuv_dep, asio_dep, gcd_dep, quic_dep],)
