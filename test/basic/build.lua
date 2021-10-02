project = {
	name = 'example',
	lang = 'cpp',
	desc = 'A simple test case'
}
local c = require'lang.c.compiler'
local lua_dep = c.dep:new('luajit')
local dep = c.dep:new()

c.executable('hello', {'src/main.c'}, {
	link_with = {lua_dep}
})
