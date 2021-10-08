assert(project, "Define 'project' brefore calling 'require'")
-- Options
option('cxx', nil)

-- Dependencies
require'lang/ccpp/options'

local util = require'util'

local cxx = get_option('cxx') or project.cxx or util.find('c++') or util.find('gcc') or util.find('clang')
print('-- Found c++ compiler '..cxx)

local cpp_COMPILER = Rule.new('cpp_COMPILER', {
	command = cxx.." $cflags -c -o $out $in",
	description = "Compiling $out..."
})


return cpp_COMPILER
