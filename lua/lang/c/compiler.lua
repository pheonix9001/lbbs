assert(project, "Define 'project' brefore calling 'require'")

M = {}

local c_COMPILER = Rule.new('c_COMPILER', {
	command = "gcc $cflags -c -o $out $in",
	description = "Compiling $out..."
})
print("loaded compiler.lua")

local c_LINKER = Rule.new('c_LINKER', {
	command = "gcc $cflags $ldflags -o $out $in",
	description = "Linking $in to $out..."
})

option('cflags', '""')
option('ldflags', '""')
option('c_buildtype', '"debug"')

M.dep = {
	cflags = get_option('cflags')..(project.cflags or ''),
	ldflags = get_option('ldflags')..(project.ldflags or ''),

	new = function(o, name)
		local o = o or setmetatable(o, M.dep)
		o.__index = M.dep

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
	end,

	merge = function(self, dep)
		self.cflags = self.cflags..' '..dep.cflags
		self.ldflags = self.ldflags..' '..dep.ldflags
	end
}

M.executable = function(out, sources, opts)
	print("-- Creating executable "..out)
	local opts = opts or {}

	local basedep = M.Dep:new()

	for _,dep in ipairs(opts.link_with or {}) do
		basedep:merge(dep)
	end

	local object_files = {}
	for _,file in ipairs(sources) do
		local out = file:gsub('%.c$', '.o')
		c_COMPILER:generate(out, {srcdir..'/'..file}, {
			cflags = basedep.cflags
		})
		table.insert(object_files, out);
	end

	c_LINKER:generate(out, object_files, {
		cflags = basedep.cflags,
		ldflags = basedep.ldflags
	})
end

M.lib = function(out, sources, opts)
end

return M
