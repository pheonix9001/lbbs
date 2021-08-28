luajit_dep = dependency('luajit')

sources = [
	'main.cpp',
	'global.cpp',
	'project.cpp'
	]

dependencies = [luajit_dep]

msn_lua = executable('msn-lua', sources,
	install : true,
	dependencies: dependencies)
