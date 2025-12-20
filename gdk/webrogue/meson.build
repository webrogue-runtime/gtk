gdk_webrogue_sources = files([
  'gdkvulkancontext-webrogue.c',
])

gdk_webrogue_public_headers = files([
])

install_headers(gdk_webrogue_public_headers, 'gdkwebrogue.h', subdir: 'gtk-4.0/gdk/webrogue/')

gdk_webrogue_deps = [
]

libgdk_webrogue = static_library('gdk-webrogue',
  sources: [ gdk_webrogue_sources, gdk_gen_headers ],
  include_directories: [ confinc, gdkinc, ],
  c_args: [
    '-DGTK_COMPILATION',
    '-DG_LOG_DOMAIN="Gdk"',
  ] + common_cflags,
  dependencies: [ gdk_deps, gdk_webrogue_deps ],
)

# Used to generate pkg-config Requires
webrogue_public_deps = []
