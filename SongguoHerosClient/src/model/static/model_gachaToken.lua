
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_gachaToken", model_static)

function model:setNode(node)
    self.map[node.id] = node;
    model.count = model.count + 1
end

function model:getNormal(id)
    if id > model.count then
    	return 100
    end
    local node = self:getNode(id);
    if(node) then
        return node.normal;
    end
    return 0
end

function model:getRare(id)
    if id > model.count then
        return 200
    end
    local node = self:getNode(id);
    if(node) then
        return node.rare;
    end
    return 0
end

function model:getTenNormal(id)
    local token = 0
    local startIndex = id
    for start=startIndex,startIndex + 9 do
        token = token + self:getNormal(start)
    end
    return token
end

function model:getTenRare(id)
    local token = 0
    local startIndex = id
    for start=startIndex,startIndex + 9 do
        token = token + self:getRare(start)
    end
    return token
end

local function init()
    model.count = 0
    model:load("config/static/GachaToken.Json");
end

init();

return model;
