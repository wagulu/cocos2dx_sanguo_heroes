
local storage = {};
storage.map = {};

--    optional int32                  inventoryTreasureItemId = 1;    //背包序列
--    optional int32                  treasureItemId = 2;             //珍宝id
--    optional int32                  level = 3;                      //珍宝等级
--    optional int32                  exp = 4;                        //珍宝经验
--    optional int32                  quality = 5;                    //珍宝品级
--    optional int32                  quippedType = 6;                //珍宝装备类型
--    optional int32                  useMonster = 7;                 //使用中的武将id 0为没有武将使用

function storage:setNode(node)
    self.map[node.inventoryTreasureItemId] = node;
end

function storage:getNode(inventoryTreasureItemId)
    return self.map[inventoryTreasureItemId];
end

function storage:getLevel(inventoryTreasureItemId)
    local node = self:getNode(inventoryTreasureItemId);
    if(node) then
        return node.level;
    end
    return 0;
end

function storage:getExp(inventoryTreasureItemId)
    local node = self:getNode(inventoryTreasureItemId);
    if(node) then
        return node.exp;
    end
    return 0;
end

function storage:getQuality(inventoryTreasureItemId)
    local node = self:getNode(inventoryTreasureItemId);
    if(node) then
        return node.quality;
    end
    return 0;
end

function storage:getEquipMonsterId(inventoryTreasureItemId)
    local node = self:getNode(inventoryTreasureItemId);
    if(node) then
        return node.useMonster;
    end
    return 0;
end

function storage:getEquipType(inventoryTreasureItemId)
    local node = self:getNode(inventoryTreasureItemId);
    if(node) then
        local prototype = static_model_treasureItem:getNode(node.treasureItemId);
        return prototype.quippedType;
    end
    return 0;
end

function storage:getLevelUpType(inventoryTreasureItemId)
    local node = self:getNode(inventoryTreasureItemId);
    if(node) then
        local prototype = static_model_treasureItem:getNode(node.treasureItemId);
        return prototype.levelUpType;
    end
    return 0;
end

function storage:getQualityUpType(inventoryTreasureItemId)
    local node = self:getNode(inventoryTreasureItemId);
    if(node) then
        local prototype = static_model_treasureItem:getNode(node.treasureItemId);
        return prototype.qualityUpType;
    end
    return 0;
end

function storage:initFromPb(pb)
    local list = pb.treasureItems;

    for key, value in pairs(list) do
        self:setNode(value);
    end
end

function storage:getItems()
    local list ={}
    for key, value in pairs(self.map) do
        list[#(list) + 1] = value;
    end
    return list
end

function storage:updateItem(item)
    self:setNode(item);
end

function storage:removeItem(inventoryTreasureItemId)
    self.map[inventoryTreasureItemId] = nil;
end

-- interface

function storage:collectItemWithTypeId(typeId, onlyUnequipped, excludeInventoryIdList)
    local list = {};
    
    for key, value in pairs(self.map) do
        local ignore = false;
    
        if(onlyUnequipped and value.useMonster > 0) then
            ignore = true;
        end

        if(value.treasureItemId ~= typeId) then
            ignore = true;
        end
        
        if(excludeInventoryIdList[value.inventoryTreasureItemId] ~= nil) then
            ignore = true;
        end
        
        if not(ignore) then
            list[#(list) + 1] = value;
        end
    end
    
    return list;
end

return storage;
