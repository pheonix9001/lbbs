project('msn-lua', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

luajit_dep = dependency('luajit')

dependencies = [luajit_dep]
executable('msn-lua',
           'src/main.cpp',
           install : true,
					 dependencies: dependencies)
