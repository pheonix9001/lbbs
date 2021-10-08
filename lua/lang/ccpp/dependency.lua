local M = {}

-- Modules
require'lang/ccpp/options'

M.cflags = get_option('cflags')..(project.cflags or '')
M.ldflags = get_option('ldflags')..(project.ldflags or '')

M.new = function(o, name)
	local o = o or setmetatable(o, M)
	o.__index = M

	if(name)
	then
		local f = io.popen('pkg-config --cflags '..name, 'r')
		local cflags = f:read('*a')
		local f = io.popen('pkg-config --libs '..name, 'r')
		local ldflags = f:read('*a')

		cflags = string.gsub(cflags, '^%s+', '')
		cflags = string.gsub(cflags, '%s+$', '')
		cflags = string.gsub(cflags, '[\n\r]+', ' ')

		ldflags = string.gsub(ldflags, '^%s+', '')
		ldflags = string.gsub(ldflags, '%s+$', '')
		ldflags = string.gsub(ldflags, '[\n\r]+', ' ')

		o.cflags = cflags
		o.ldflags = ldflags

		return o
	end

	if(get_option('c_buildtype') == 'debug')
	then
		o.cflags = o.cflags..' -O2 -Wall -Werror -D_DEBUG_'
	elseif(get_option('c_buildtype') == 'release')
	then
		o.cflags = o.cflags..' -O3 -Wall -Werror -D_RELEASE_'
	end

	return o
end

M.merge = function(self, dep)
	self.cflags = self.cflags..' '..dep.cflags
	self.ldflags = self.ldflags..' '..dep.ldflags
end

return M
