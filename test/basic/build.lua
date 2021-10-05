project = {
	name = 'example',
	lang = 'cpp',
	desc = 'A simple test case'
}
local c = require'lang.c.compiler'
local dep = require'lang.ccpp.dependency'

local lua_dep = dep:new('luajit')

c.executable('hello', {'src/main.c'}, {
	link_with = {lua_dep}
})
