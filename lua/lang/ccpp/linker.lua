-- Options
option('ld', nil)

local util = require'util'

local ld = get_option('ld') or project.ld or util.find('gcc') or util.find('clang')
print('-- Found linker '..ld)

return Rule.new('LINKER', {
	command = ld.." $cflags $ldflags -o $out $in",
	description = "Linking $in to $out..."
})

