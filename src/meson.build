luajit_dep = dependency('luajit')

sources = [
	'main.cpp',
	'global.cpp',
	'project.cpp',
	'backends/ninja.cpp',
	'backends/backend.cpp'
	]

dependencies = [luajit_dep]

msn_lua = executable('msn-lua', sources,
	install : true,
	dependencies: dependencies)
