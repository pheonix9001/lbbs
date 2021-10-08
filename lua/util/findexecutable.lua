local M = function(exe)
	local ret = ""

	if(jit.os == "Windows") then
		local fd = io.popen("where "..exe, 'r')

		ret = fd:read("*a")
		fd:close()
	else
		local fd = io.popen("which "..exe, 'r')

		ret = fd:read("*a")
		fd:close()
	end

	string.gsub(ret, '^%s+', '')
	string.gsub(ret, '%s+$', '')
	string.gsub(ret, '[\n\r]+', ' ')
	print(ret)

	if(ret)
	then
		return ret
	end
end

return M
