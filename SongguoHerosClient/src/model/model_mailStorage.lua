
local storage = {};
storage.map = {};

function storage:setNode(node, channel)
    if(self.map[channel] == nil) then
        self.map[channel] = {};
    end

    self.map[channel][node.key] = node;
end

function storage:getNode(key, channel)
    if self.map[channel] then
        return self.map[channel][key];
    end

end

function storage:deleteNode(key, channel)
    if self.map[channel] then
        self.map[channel][key] = nil;
    end
end

function storage:setRead(key, channel)
    if self.map[channel] then
        self.map[channel][key].status = 1;
    end
end

function storage:isRead(key, channel)
    if self.map[channel] then
        return self.map[channel][key].status == 1;
    end
    
    return true;
end

function storage:deletChanel(channel)
    if self.map[channel] then
        self.map[channel] = nil
    end

    return true;
end

function storage:collectMail(channel)
    local list = {};

    local map = self.map[channel];
    if(map) then
        for key, value in pairs(map) do
            if(value) then
                list[#(list) + 1] = value;
            end
        end
    end
    
    return list;
end


return storage;
