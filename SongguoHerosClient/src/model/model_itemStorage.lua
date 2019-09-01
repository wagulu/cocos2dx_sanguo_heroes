
local storage = {};
storage.map = {};

function storage:setNode(node)
    if(self.map[node.type] == nil) then
        self.map[node.type] = {};
    end
    
    self.map[node.type][node.itemId] = node;
end

function storage:getNode(id, type)
    if self.map[type] then
        return self.map[type][id];
    end

end

function storage:getCount(id, type)
    local node = self:getNode(id, type);
    if(node) then
        return node.count;
    end
    return 0;
end

--function storage:getType(id)
--    local node = self:getNode(id);
--    if(node) then
--        return node.type;
--    end
--    return 0;
--end

function storage:initFromPb(pb)
    local list = pb.items;

    for key, value in pairs(list) do
        self:setNode(value);
    end
end


function storage:setCount(id, type, count)
    local node = self:getNode(id, type);
    if(node) then
        node.count = count;
    end
end

function storage:sumCount(id, type, count)
    local node = self:getNode(id, type);
    if(node) then
        node.count = node.count + count;
    end
end

function storage:subCount(id, type, count)
    local node = self:getNode(id, type);
    if(node) then
        node.count = node.count - count;
        if(node.count < 0) then
            node.count = 0;
            release_print("ERROR: item count less than 0, id = " .. tostring(id));
        end
    end
end


function storage:updateItem(item)
    self:setNode(item);
end

return storage;
