project = {
	name = 'example',
	lang = 'cpp',
	desc = 'A simple test case'
}
local cpp = require'lang.cpp'
local dep = require'lang.ccpp.dependency'

local lua_dep = dep:new('luajit')

cpp.executable('hello', {'src/main.cpp'}, {
	link_with = {lua_dep}
})
