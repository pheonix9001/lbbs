assert(project, "Define 'project' brefore calling 'require'")

M = {}

local c_COMPILER = Rule.new('c_COMPILER', {
	command = "gcc $cflags -c -o $out $in",
	description = "Compiling $out..."
})

local c_LINKER = Rule.new('c_LINKER', {
	command = "gcc $cflags $ldflags -o $out $in",
	description = "Linking $in to $out..."
})

option('cflags', '""')
option('ldflags', '""')
option('c_buildtype', '"debug"')

local dep = require'lang/ccpp/dependency'

M.executable = function(out, sources, opts)
	print("-- Creating executable "..out)
	local opts = opts or {}

	local basedep = dep:new()

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
		-- cflags = basedep.cflags,
		ldflags = basedep.ldflags
	})
end

M.lib = function(out, sources, opts)
end

return M
