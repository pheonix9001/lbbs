project('basic', 'cpp', {
	desc = 'A basic test'
})

option('man', '"cool"', false)

print(get_option('man'))
