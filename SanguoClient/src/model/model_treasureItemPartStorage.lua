
local storage = {};
storage.map = {};

function storage:setNode(node)
    self.map[node.treasureItemPartId] = node;
end

function storage:getNode(id)
    return self.map[id];
end

function storage:getTreasureItemId(id)
    local node = self:getNode(id);
    if(node) then
        return node.treasureItemId;
    end
    return 0;
end

function storage:getCount(id)
    local node = self:getNode(id);
    if(node) then
        return node.count;
    end
    return 0;
end

function storage:initFromPb(pb)
    local list = pb.treasureItemParts;

    for key, value in pairs(list) do
        self:setNode(value);
    end
end


function storage:setCount(id, count)
    local node = self:getNode(id);
    if(node) then
        node.count = count;
    end
end

function storage:sumCount(id, count)
    local node = self:getNode(id);
    if(node) then
        node.count = node.count + count;
    end
end

function storage:subCount(id, count)
    local node = self:getNode(id);
    if(node) then
        node.count = node.count - count;
        if(node.count < 0) then
            node.count = 0;
            release_print("ERROR: treasure item part count less than 0, id = " .. tostring(id));
        end
    end
end

function storage:getItems()
    local list ={}
    for key, value in pairs(self.map) do
        if value.count ~= 0 then
            list[#(list) + 1] = value;
        end
        
    end
    return list
end

function storage:updateItem(item)
    self:setNode(item);
end

return storage;
