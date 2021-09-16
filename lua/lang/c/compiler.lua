M = {}

local c_COMPILER = Rule.new('c_COMPILER', {
	command = "gcc -c -o $out $in",
	description = "Compiling $out..."
})

local c_LINKER = Rule.new('c_LINKER', {
	command = "gcc -o $out $in",
	description = "Linking $in to $out..."
})

M.executable = function(out, sources, obj)
	print("-- Creating executable "..out)
	local obj = obj or {}
	local object_files = {}

	for _,file in ipairs(sources) do
		local out = file:gsub('%.c$', '.o')
		c_COMPILER:generate(out, {srcdir..'/'..file})
		table.insert(object_files, out);
	end

	c_LINKER:generate(out, object_files)
end

return M
