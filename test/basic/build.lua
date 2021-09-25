project = {
	name = 'example',
	lang = 'cpp',
	desc = 'A simple test case'
}

local c = require'lang/c/compiler'

local lua_dep = c.dep('luajit')

local dep = c.Dep:new()
print(dep.cflags)

c.executable('hello', {'src/main.c'}, {
	link_with = {lua_dep}
})
