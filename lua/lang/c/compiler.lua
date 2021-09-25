assert(project, "Define 'project' brefore calling 'require'")

M = {}

local c_COMPILER = Rule.new('c_COMPILER', {
	command = "gcc $cflags -c -o $out $in",
	description = "Compiling $out..."
})

local c_LINKER = Rule.new('c_LINKER', {
	command = "gcc -o $out $in",
	description = "Linking $in to $out..."
})

option('cflags', '""')
option('ldflags', '""')
option('c_buildtype', '"debug"')

M.Dep = {
	cflags = get_option('cflags')..(project.cflags or ''),
	ldflags = get_option('ldflags')..(project.ldflags or ''),

	new = function(o, name)
		local o = o or setmetatable(o, M.Dep)

		return o
	end
}

M.executable = function(out, sources, opts)
	print("-- Creating executable "..out)
	local opts = opts or {}

	local cflags = project.cflags or ''
	local ldflags = project.ldflags or ''

	cflags = cflags..' '..get_option('cflags')
	ldflags = ldflags..' '..get_option('ldflags')

	if(get_option('c_buildtype') == 'debug')
	then
		cflags = cflags..' -O2 -Wall -Werr -D_DEBUG_'
	elseif(get_option('c_buildtype') == 'release')
	then
		ldflags = ldflags..' -O3 -Wall -D_RELEASE_'
	end

	for _,dep in ipairs(opts.link_with or {}) do
		cflags = cflags..' '..dep.cflags
		ldflags = ldflags..' '..dep.ldflags
	end

	local object_files = {}
	for _,file in ipairs(sources) do
		local out = file:gsub('%.c$', '.o')
		c_COMPILER:generate(out, {srcdir..'/'..file}, {
			cflags = cflags
		})
		table.insert(object_files, out);
	end

	c_LINKER:generate(out, object_files, {
		cflags = cflags,
		ldflags = ldflags
	})
end

M.dep = function(name)
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

	return {
		cflags = cflags,
		ldflags = ldflags
	}
end

return M
