project('basic', 'cpp', {
	desc = 'A basic test'
})

local sources = {'src/main.c'}

local c_compiler = Rule.new('c_COMPILER', {
	command = "gcc -c $in -o $out",
	desc = "Compiling $in..."
})

local c_LINKER = Rule.new('c_LINKER', {
	command = "gcc $in -o $out",
	desc = "Linking files..."
})

local outs = {}

-- generate build statements
for _,file in ipairs(sources) do
	local out,rep = string.gsub(file, '.c$', '.o')
	table.insert(outs, out)
	c_compiler:generate(out, {'../'..file})
end

c_LINKER:generate('hello', outs)
