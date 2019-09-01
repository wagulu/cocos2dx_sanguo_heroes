
local model = {};
model.map = {};

--            'id'                    => $this->getId(),
--            'monsterId'             => $this->getMasterId(),
--            'level'                 => $this->getLevel(),
--            'quality'               => $this->getQuality(),
--            'star'                  => $this->getStar(),
--            'exp'                   => $this->getExp(),
--            "maxSoldier"            => $this->getMaxSoldier(),
--            "arrowSpeed"            => $this->getArrowSpeed(),
--            "HP"                    => $this->getHP(),
--            "MP"                    => $this->getMP(),
--            "attack"                => $this->getAttack(),
--            "power"                 => $this->getPower(),
--            "speed"                 => $this->getSpeed(),
--            "soldier"               => $this->getSoldier(),
--            "skills"                => $this->getMonsterSkills()->toArrayForBattle($this),
--            "formations"            => $this->getFormations()->toArray(),
--            "equipment"             => $this->getMonsterEquipments()->toArray($user),
--            "treasureItems"         => $this->getTreasureItems()->toArray(),

function model:setNode(node, count)
    self.map[node.id] = {};
    self.map[node.id].count = count;
    self.map[node.id].monster = node;
end

function model:getNode(id)
    return self.map[id];
end

function model:getMonster(id)
    if self.map[id] then
        return self.map[id].monster
    end
end

function model:getFrame(id)
    local node = self:getMonster(id);
    if(node) then
        return self:getFrameByQuality(node.quality)
    end
    return "";
end

function model:getFrameByQuality(quality)
    cclog("asset:".."frame/frame_"..pbCommon.ColorType[quality]..".png")
    return "frame/frame_"..pbCommon.ColorType[quality]..".png"
end

function model:haveMonster(id)
    return self.map[id] ~= nil;
end

function model:isMonsterActive(id)
    return self:haveMonster(id) and self.map[id].count > 0;
end

function model:initFromPb(pb)
    for key, value in pairs(pb.monsters) do
        self:setNode(value, 1);
    end
    for key, value in pairs(pb.notHaveMonsters) do
        self:setNode(value, 0);
    end
end


function model:activeMonster(id)
    local node = self:getNode(id);
    if(node) then
        if(node.count > 0) then
            release_print("ERROR, already have monster, id = " .. tostring(id));
        end
        node.count = 1;
    else
        release_print("ERROR, can't find monster, id = " .. tostring(id));
    end
end


function model:deactiveMonster(id)
    local node = self:getNode(id);
    if(node) then
        if not(node.count == 0) then
            release_print("ERROR, don't have monster, id = " .. tostring(id));
        end
        node.count = 0;
    else
        release_print("ERROR, can't find monster, id = " .. tostring(id));
    end
end

function model:updateMonster(monster)
    self.map[monster.id].monster = monster;
end

function model:updateMergeMonster(monster)
    self.map[monster.id].monster = monster;
    self.map[monster.id].count = 1;
end

function model:getAllActiveMonsters()
    local list = {};
    for key, value in pairs(self.map) do
        if(value.count > 0) then
            list[#(list) + 1] = value.monster;
        end
    end
    return list;
end

function model:getAllInactiveMonster()
    local list = {};
    for key, value in pairs(self.map) do
        if not(self:isMonsterActive(key)) then
            list[#(list) + 1] = value.monster;
        end
    end
    return list;
end

function model:getColor(id)
    local node = self:getMonster(id);
    if(node) then
        local name = pbCommon.ColorType[node.quality]
        local splits = string.split(name,"_")
        return pbCommon.Colors[tonumber(splits[1])],splits
    end
end


function model:getEquipColor(quality)
    local name = pbCommon.ColorType[quality]
    local splits = string.split(name,"_")
    return pbCommon.Colors[tonumber(splits[1])],splits
end

return model;
