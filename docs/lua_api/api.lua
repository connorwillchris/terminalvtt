--[[
LICENSE HERE
]]

---@diagnostic disable-next-line
core = {}

--[[Print to the console.]]
---@param ... string
---@return nil
core.print = function(...) end
print = core.print

--[[Roll dice. Specified as a string, like `d4` or `2d8` ect...]]
---@param dice string
---@return number
---@diagnostic disable-next-line
core.roll = function(dice) end
