deps = [
	dependency('luajit')
]

shared_module('compiler', ['compiler.c'],
	dependencies: deps,
	name_prefix: '',
	install: true,
	install_dir: '/usr/lib/tachyon/lang/c',
	include_directories: incdirs,
)
