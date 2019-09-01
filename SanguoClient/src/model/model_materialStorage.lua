
local storage = {};
storage.map = {};

function storage:setNode(node)
    self.map[node.id] = node;
end

function storage:getNode(id)
    return self.map[id];
end

function storage:getCount(id)
    local node = self:getNode(id);
    if(node) then
        return node.count;
    end
    return 0;
end

function storage:initFromPb(pb)
    local list = pb.materials;
    
    for key, value in pairs(list) do
        self:setNode(value);
    end
end


function storage:setCount(id, count)
    local node = self:getNode(id);
    if(node) then
        node.count = count;
    else
        local newNode = {};
        newNode.id = id;
        newNode.count = count;
        self.map[id] = newNode;
    end
end

function storage:sumCount(id, count)
    local node = self:getNode(id);
    if(node) then
        node.count = node.count + count;
    else
        local newNode = {};
        newNode.id = id;
        newNode.count = count;
        self.map[id] = newNode;
    end
end

function storage:subCount(id, count)
    local node = self:getNode(id);
    if(node) then
        node.count = node.count - count;
        if(node.count < 0) then
            node.count = 0;
            release_print("ERROR: material count less than 0, id = " .. tostring(id));
        end
    else
        local newNode = {};
        newNode.id = id;
        newNode.count = count;
        self.map[id] = newNode;
    end
end


return storage;
