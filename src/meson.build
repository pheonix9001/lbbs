luajit_dep = dependency('luajit')

sources = [
	'main.cpp',
	'global.cpp',
	'option.cpp',
	'luah.cpp',
	'backends/ninja.cpp',
	'backends/backend.cpp',
	]

dependencies = [luajit_dep]

tachyon = executable('tachyon', sources,
	install : true,
	dependencies: dependencies)
