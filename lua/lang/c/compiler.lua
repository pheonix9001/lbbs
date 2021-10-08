assert(project, "Define 'project' brefore calling 'require'")

-- Dependencies
require'lang/ccpp/options'
local dep = require'lang/ccpp/dependency'

local M = {}

local c_COMPILER = Rule.new('c_COMPILER', {
	command = "gcc $cflags -c -o $out $in",
	description = "Compiling $out..."
})

-- Utility function
--
-- Takes in list of .c sources, generates `build` definitions,
-- and returns .o files
M.objects = function(sources, opts)
	local object_files = {}
	for _,file in ipairs(sources) do
		local out = file:gsub('%.c$', '.o')
		c_COMPILER:generate(out, {srcdir..'/'..file}, opts)
		table.insert(object_files, out);
	end

	return object_files
end

local c_LINKER = Rule.new('c_LINKER', {
	command = "gcc $cflags $ldflags -o $out $in",
	description = "Linking $in to $out..."
})


M.executable = function(out, sources, opts)
	print("-- Creating executable "..out)
	local opts = opts or {}

	local basedep = dep:new()

	for _,dep in ipairs(opts.link_with or {}) do
		basedep:merge(dep)
	end
	local obj_files = M.objects(sources, { cflags = basedep.cflags })

	c_LINKER:generate(out, obj_files, {
		ldflags = basedep.ldflags
	})
end

M.lib = function(out, sources, opts)
end

return M
