local model_static_library = _REQUIRE("model/static/model_static_library.lua")
local model_user = _REQUIRE("model/model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")

local helper_monster = _REQUIRE("model/helper/helper_monster.lua")

local helper = {};
helper.notifications = {};

NOTIFICATION_TYPE = {};
NOTIFICATION_TYPE.UNKNOWN = 0;
NOTIFICATION_TYPE.MONSTER_STAR_UP = 1;
NOTIFICATION_TYPE.MONSTER_QUALITY_UP = 2;
NOTIFICATION_TYPE.MONSTER_MERGE = 3;
NOTIFICATION_TYPE.EQUIPMENT_QUALITY_UP = 10;

function helper:createNotification(type, data)
    type = type or NOTIFICATION_TYPE.UNKNOWN;

    local n = {};
    n.type = type;
    n.data = data;
    return n;
end

function helper:getNotifications()
    return self.notifications;
end

function helper:hasSpecialNotification(type)
    for key, value in pairs(self.notifications) do
        if(value.type == type) then
            return true;
        end
    end
    return false;
end

function helper:addNotification(n)
    local found = false;

    for key, notification in pairs(self.notifications) do
        if(n.type == notification.type) then
            self.notifications[key] = n;
            found = true;
            break;
        end
    end
    
    if not(found) then
        self.notifications[#(self.notifications) + 1] = n;
    end
    
end

function helper:delNotification(n)
    local notifications = {};

    for key, notification in pairs(self.notifications) do
        if(n ~= notification) then
            notifications[#(notifications) + 1] = notification;
        end
    end
    
    self.notifications = notifications;
end

function helper:delAll()
    helper.notifications = {};
end

-- detect

function helper:detectAll()
    self:detectMonsterStarUp();
    self:detectMonsterQualityUp();
    self:detectMonsterMerge();
    self:detectMonsterEquipmentQualityUp();
end

function helper:detectMonsterStarUp()

    local team = model_user.battleTeam;

    for key, value in pairs(team) do
        local monsterId = value.monsterId;
        if(monsterId > 0) then
            local result = helper_monster:canMonsterStarUp(monsterId);
            if(result) then
                local data = {
                    monsterId = monsterId,
                };
                local notification = self:createNotification(NOTIFICATION_TYPE.MONSTER_STAR_UP, data);
                self:addNotification(notification);
                return;
            end
        end
    end

end

function helper:detectMonsterQualityUp()

    local team = model_user.battleTeam;

    for key, value in pairs(team) do
        local monsterId = value.monsterId;
        if(monsterId > 0) then
            local result = helper_monster:canMonsterQualityUp(monsterId);
            if(result.id == 0) then
                local data = {
                    monsterId = monsterId,
                };
                local notification = self:createNotification(NOTIFICATION_TYPE.MONSTER_QUALITY_UP, data);
                self:addNotification(notification);
                return;
            end
        end
    end

end

function helper:detectMonsterMerge()
    local inactiveMonsters = model_monsterStorage:getAllInactiveMonster();

    for key, value in pairs(inactiveMonsters) do
        local monsterId = value.id;
        local canMerge = helper_monster:canMergeMonster(monsterId);
        if(canMerge) then
            local data = {
                monsterId = monsterId,
            };
            local notification = self:createNotification(NOTIFICATION_TYPE.MONSTER_MERGE, data);
            self:addNotification(notification);
            return;
        end
    end

end

function helper:detectMonsterEquipmentQualityUp()
    local team = model_user.battleTeam;
    
    for key, value in pairs(team) do
        local monsterId = value.monsterId;
        local monster = model_monsterStorage:getMonster(monsterId);
        if(monster) then
            for key, value in pairs(monster.equipment) do
                local equipmentId = value.id;
                local canQualityUp = helper_monster:canEquipmentQualityUp(monsterId, equipmentId);
                if(canQualityUp) then
                    local data = {
                        monsterId = monsterId,
                        equipmentId = equipmentId,
                    };
                    local notification = self:createNotification(NOTIFICATION_TYPE.EQUIPMENT_QUALITY_UP, data);
                    self:addNotification(notification);
                    return;
                end
            end
        end
    end
    
end


return helper;
