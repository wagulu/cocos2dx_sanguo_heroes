
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_dropWay", model_static)


function model:setNode(node)
    
    if(self.map[node.itemType] == nil) then
        self.map[node.itemType] = {};
    end
    if(self.map[node.itemType][node.itemId] == nil) then
        self.map[node.itemType][node.itemId] = {};
    end
    local group = self.map[node.itemType][node.itemId];
    group[#(group) + 1] = node;
end

function model:getItems(itemId ,itemType)
    if self.map[itemType] and self.map[itemType][itemId] then
        local items = self.map[itemType][itemId]
        table.sort(items,function(node1,node2)
            return node1.orders < node2.orders
        end)
        return items
    end
    return {}
end


--interface




local function init()
    model:load("config/static/Dropway.Json");
end

init();

return model;
