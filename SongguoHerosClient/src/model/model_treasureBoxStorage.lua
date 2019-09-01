
local storage = {};
storage.map = {};

function storage:setNode(node)
    self.map[node.treasureBoxId] = node;
end

function storage:getNode(id)
    return self.map[id];
end

function storage:getTreasureBoxGroup(id)
    local node = self:getNode(id);
    if(node) then
        return node.groupId;
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
    local list = pb.treasureBoxes;

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
            release_print("ERROR: treasure box count less than 0, id = " .. tostring(id));
        end
    end
end

function storage:getBoxes()
    local list ={}
    for key, value in pairs(self.map) do
        if value.count ~= 0 then
            list[#(list) + 1] = value;
        end

    end
    return list;
end

function storage:updateItem(item)
    self:setNode(item);
end

return storage;
