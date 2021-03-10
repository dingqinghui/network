local cjson = require "cjson"
local function_list = function_list or {}

-- cjson.encode will serialise a Lua value into a string containing the JSON representation.
-- cjson.encode+ supports the following types:
-- boolean
-- lightuserdata (NULL value only)
-- nil
-- number
-- string
-- table
-- for only number index table! you need note :
-- cjson.encode({ [3] = "data" })
-- Returns: '[null,null,"data"]'
function function_list.encode(value)
	return cjson.encode(value)
end

-- cjson.decode will deserialise any UTF-8 JSON string into a Lua value
function function_list.decode(text)
	return cjson.decode(text)
end

--------------------------------Get and/or set CJSON configuration------------------------------

-- Lua CJSON classifies a Lua table into one of three kinds when encoding a
-- JSON array. This is determined by the number of values missing from the
-- Lua array as follows:

-- Normal:: All values are available.
-- Sparse:: At least 1 value is missing.
-- Excessively sparse:: The number of values missing exceeds the configured
--   ratio.

-- Lua CJSON encodes sparse Lua arrays as JSON arrays using JSON +null+ for
-- the missing entries.
------------------------
-- "convert" must be a boolean. Default: false.
-- "ratio" must be a positive integer. Default: 2.
-- "safe" must be a positive integer. Default: 10.
function function_list.encode_sparse_array(convert, ratio, safe)
	return cjson.encode_sparse_array(convert, ratio, safe)
end


-- Once the maximum table depth has been exceeded Lua CJSON will generate
-- an error. This prevents a deeply nested or recursive data structure from
-- crashing the application.
-- "depth" must be a positive integer. Default: 1000.
function function_list.encode_max_depth(depth)
	return cjson.encode_max_depth(depth)
end

-- Lua CJSON will generate an error when parsing deeply nested JSON once
-- the maximum array/object depth has been exceeded. This check prevents
-- unnecessarily complicated JSON from slowing down the application, or
-- crashing the application due to lack of process stack space.
-- "depth" must be a positive integer. Default: 1000.
function function_list.decode_max_depth(depth)
	return cjson.decode_max_depth(depth)
end

-- The amount of significant digits returned by Lua CJSON when encoding
-- numbers can be changed to balance accuracy versus performance. For data
-- structures containing many numbers, setting
-- +cjson.encode_number_precision+ to a smaller integer, for example +3+,
-- can improve encoding performance by up to 50%.
-- "precision" must be an integer between 1 and 14. Default: 14.
function function_list.encode_number_precision(precision)
	return cjson.encode_number_precision(precision)
end

-- "keep" must be a boolean. Default: true.
-- +true+:: The buffer will grow to the largest size required and is not
--   freed until the Lua CJSON module is garbage collected. This is the
--   default setting.
-- +false+:: Free the encode buffer after each call to +cjson.encode+.
function function_list.encode_keep_buffer(keep)
	return cjson.encode_keep_buffer(keep)
end

-- "setting" must a boolean or "null". Default: false.
-- +true+:: Allow _invalid numbers_ to be encoded using the Javascript
--   compatible values +NaN+ and +Infinity+. This will generate
--   non-standard JSON, but these values are supported by some libraries.
-- +"null"+:: Encode _invalid numbers_ as a JSON +null+ value. This allows
--   infinity and NaN to be encoded into valid JSON.
-- +false+:: Throw an error when attempting to encode _invalid numbers_.
--   This is the default setting.

-- The current setting is always returned, and is only updated when an
-- argument is provided.
function function_list.encode_invalid_numbers(setting)
	return cjson.encode_invalid_numbers(setting)
end

-- "setting" must be a boolean. Default: true.
-- +true+:: Accept and decode _invalid numbers_. This is the default
--   setting.
-- +false+:: Throw an error when _invalid numbers_ are encountered.

-- The current setting is always returned, and is only updated when an
-- argument is provided.
function function_list.decode_invalid_numbers(setting)
	return cjson.decode_invalid_numbers(setting)
end

-- +cjson.new+ can be used to instantiate an independent copy of the Lua
-- CJSON module. The new module has a separate persistent encoding buffer,
-- and default settings.

function function_list.new()
	return cjson.new()
end

return function_list