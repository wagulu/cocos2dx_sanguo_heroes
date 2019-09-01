local print = release_print
local tconcat = table.concat
local tinsert = table.insert
local srep = string.rep
local type = type
local pairs = pairs
local tostring = tostring
local next = next

function print_r(root)
    local cache = {  [root] = "." }
    local function _dump(t,space,name)
        local temp = {}
        for k,v in pairs(t) do
            local key = tostring(k)
            if cache[v] then
                tinsert(temp,"+" .. key .. " {" .. cache[v].."}")
            elseif type(v) == "table" then
                local new_key = name .. "." .. key
                cache[v] = new_key
                tinsert(temp,"+" .. key .. _dump(v,space .. (next(t,k) and "|" or " " ).. srep(" ",#key),new_key))
            else
                tinsert(temp,"+" .. key .. " [" .. tostring(v).."]")
            end
        end
        return tconcat(temp,"\n"..space)
    end
    print(_dump(root, "",""))
end


function print_data_dump(data, len)
    len = len or string.len(data);

    local str = "dump data, len = " .. tostring(len) .. "\n";

    for i = 1, len do
        local charCode = string.byte(data, i);
        local charCodeX = string.format("%X", charCode);
        if(#(charCodeX) == 1) then
            charCodeX = "0" .. charCodeX;
        end
        str = str .. charCodeX .. " ";

        if(i % 2 == 0) then
            str = str .. "\t";
        end
        if(i % 24 == 0) then
            str = str .. "\n";
        end
    end
    
    print(str);

end
