
local model = {};
model.map = {};

function model:setNode(node)
    self.map[node.monsterPartId] = node;
end

function model:getNode(id)
    return self.map[id];
end

function model:getCount(id)
    local node = self:getNode(id);
    if(node) then
        return node.count;
    end
    return 0;
end

function model:initFromPb(pb)
    local list = pb.monsterParts;

    for key, value in pairs(list) do
        self:setNode(value);
    end
end


function model:setCount(id, count)
    local node = self:getNode(id);
    if(node) then
        node.count = count;
    else
        local newNode = {};
        newNode.monsterPartId = id;
        newNode.count = count;
        self.map[id] = newNode;
    end
end

function model:sumCount(id, count)
    local node = self:getNode(id);
    if(node) then
        node.count = node.count + count;
    else
        local newNode = {};
        newNode.monsterPartId = id;
        newNode.count = count;
        self.map[id] = newNode;
    end
end

function model:subCount(id, count)
    local node = self:getNode(id);
    if(node) then
        node.count = node.count - count;
        if(node.count < 0) then
            node.count = 0;
            release_print("ERROR: material count less than 0, id = " .. tostring(id));
        end
    else
        local newNode = {};
        newNode.monsterPartId = id;
        newNode.count = count;
        self.map[id] = newNode;
    end
end


return model;
