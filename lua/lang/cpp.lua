local M = {}

local LINKER = require'lang/ccpp/linker'
local dep = require'lang/ccpp/dependency'
local cpp_COMPILER = require'lang/cpp/compiler'

-- Utility function
--
-- Takes in list of .cpp sources, generates `build` definitions,
-- and returns .o files
M.objects = function(sources, opts)
	local object_files = {}
	for _,file in ipairs(sources) do
		local out = string.gsub(file, '%.cpp$', '.o')
		cpp_COMPILER:generate(out, {srcdir..'/'..file}, opts)
		table.insert(object_files, out);
	end

	return object_files
end

M.executable = function(out, sources, opts)
	print("-- Creating executable "..out)
	local opts = opts or {}

	local basedep = dep:new()

	for _,dep in ipairs(opts.link_with or {}) do
		basedep:merge(dep)
	end
	local obj_files = M.objects(sources, { cflags = basedep.cflags })

	LINKER:generate(out, obj_files, {
		ldflags = basedep.ldflags
	})
end

return M
