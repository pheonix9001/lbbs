luajit_dep = dependency('luajit')

sources = [
	'main.cpp',
	'global.cpp',
	'option.cpp',
	'backends/ninja.cpp',
	'backends/backend.cpp',
	]

dependencies = [luajit_dep]

lbbs = executable('lbbs', sources,
	install : true,
	dependencies: dependencies)
