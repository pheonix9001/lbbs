luajit_dep = dependency('luajit')

sources = [
	'main.cpp',
	'global.cpp',
	'project.cpp',
	'option.cpp',
	'backends/ninja.cpp',
	'backends/backend.cpp'
	]

dependencies = [luajit_dep]

tachyon = executable('tachyon', sources,
	install : true,
	dependencies: dependencies)
