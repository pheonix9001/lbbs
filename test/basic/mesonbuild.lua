project('basic', 'cpp', {
	desc = 'A basic test'
})

compiler = Rule.new('hello', {
	this = 'is',
	really = 'cool'
})
print(compiler)
compiler:generate('a.c', {'b.c', 'c.c'})
