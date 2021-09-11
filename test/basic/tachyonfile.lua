project('basic', 'cpp', {
	desc = 'A basic test'
})

c = require'lang.c.compiler'
option('man', 'cool')

print(get_option('man'))
