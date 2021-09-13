project('basic', 'cpp', {
	desc = 'A basic test'
})

local c = require"lang/c/compiler"

c.executable('hello', {'src/main.c'})
