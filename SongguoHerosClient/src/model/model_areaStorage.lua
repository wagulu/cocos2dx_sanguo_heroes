
local static_model_gacha = _REQUIRE("model/static/model_gacha.lua")
local dateHelper = _REQUIRE("common/dateHelper.lua")
local model_user = _REQUIRE("model/model_user.lua")
local static_model_stage = _REQUIRE("model/static/model_stage.lua")
local static_model_eliteStage = _REQUIRE("model/static/model_eliteStage.lua")

local storage = {};
storage.islands = {};
storage.areas = {};
storage.stages = {};
storage.stagesElite = {};
--storage.islandsMap = {};
--storage.areasMap = {};
--storage.stagesMap = {};
--storage.stagesEliteMap = {};


-- init

function storage:initWithUser(data)

    local userIslands = data.userIslands;
    local userAreas = data.userAreas;
    local userStages = data.userStages;
    local userEliteStages = data.userEliteStages;

    self.islands = userIslands;
    self.areas = userAreas;
    self.stages = userStages;
    self.stagesElite = userEliteStages;
    
--    for key, island in pairs(userIslands) do
--        self.islandsMap[island.islandId] = island;
--    end
--
--    for key, area in pairs(userAreas) do
--        self.areasMap[area.areaId] = area;
--    end
--
--    for key, stage in pairs(userStages) do
--        self.stagesMap[stage.id] = stage;
--    end
--
--    for key, stage in pairs(userEliteStages) do
--        self.stagesEliteMap[stage.id] = stage;
--    end

end

--function storage:updateIsland(island)
--    self.islands[island.islandId] = island;
--    
----    for key, value in pairs(self.islands) do
----        if(value.islandId == island.islandId) then
----            self.islands[key] = island;
----            break;
----        end
----    end
--end
--
--function storage:updateArea(area)
--    self.areas[area.areaId] = area;
--    
----    for key, value in pairs(self.areas) do
----        if(value.areaId == area.areaId) then
----            self.areas[key] = area;
----            break;
----        end
----    end
--end
--
---- update
--
--function storage:updateStage(stage, starTreasuresList, totalStar, stageType)
--    local area = self:getAreaWithStage(stage);
--
--    if(stageType == pbCommon.StageType.NORMAL) then
--        self.stages[stage.id] = stage;
--        area.stageTotalStar = totalStar;
--        area.stageStarTreasures = starTreasuresList;
--    else
--        self.stagesElite[stage.id] = stage;
--        area.eliteStageTotalStar = totalStar;
--        area.eliteStageStarTreasures = starTreasuresList;
--    end
--    
--end

-- update

function storage:updateStage(stageId, stageType, starInfo)
    local stage = nil;
    if(stageType == pbCommon.StageType.NORMAL) then
        stage = self.stages[stageId];
    else
        stage = self.stagesElite[stageId];
    end

    stage.starInfo = starInfo;

end

function storage:updateWithBattleResult(data)
    local refresh = data.refresh;
    
    for key, value in pairs(refresh.island) do
        self.islands[value.id].status = value.status;
    end

    for key, value in pairs(refresh.area) do
        self.areas[value.id].status = value.status;
        self.areas[value.id].stageTotalStar = value.stageTotalStar;
        self.areas[value.id].eliteStageTotalStar = value.eliteStageTotalStar;
    end

    for key, value in pairs(refresh.stage) do
        self.stages[value.id].status = value.status;
        self.stages[value.id].starInfo = value.starInfo;
    end

    for key, value in pairs(refresh.eliteStage) do
        self.stagesElite[value.id].status = value.status;
        self.stagesElite[value.id].starInfo = value.starInfo;
    end

end

-- helper

function storage:getAreaWithStage(stage, stageType)
    local stageModel;
    if(stageType == pbCommon.StageType.NORMAL) then
        stageModel = static_model_stage:getNode(stage.id);
    else
        stageModel = static_model_eliteStage:getNode(stage.id);
    end
    
    if stageModel == nil then
        return nil
        --release_print("something wrong " .. stage .. " " .. stageType)
    end

    return self:getAreaWithId(stageModel.areaId);
end

function storage:getStage(stageId, stageType)
    local stageModel;
    if(stageType == pbCommon.StageType.NORMAL) then
        stageModel = static_model_stage:getNode(stageId);
    else
        stageModel = static_model_eliteStage:getNode(stageId);
    end

    return stageModel
end

function storage:getExp(stageId, stageType)
    local stageModel = self:getStage(stageId,stageType)
    return stageModel.energy * 35
end

function storage:getAreaWithId(areaId)
    return self.areas[areaId];
end

function storage:getCurrentAreaId(stageType)
    local storageStages;
    if(stageType == pbCommon.StageType.NORMAL) then
        storageStages = self.stages;
    else
        storageStages = self.stagesElite;
    end
    
    for i=1, #storageStages do
        local stage = storageStages[i]
        if stage.status == 1  then
            local area = self:getAreaWithStage(storageStages[i-1], stageType);
            return area.areaId
        end
    end
    return 0
end

function storage:fillBattleDataToUser(islandId, areaId, stageType)

--    model.islandsList = {};
--    model.areasList = {};
--    model.stagesList = {};
--    model.stageStarTreasuresList = {};
--    model.stageTotalStar = 0;


    model_user.islandsList = self.islands;

    if(islandId ~= nil) then
        model_user.areasList = self:collectAreas(islandId);
    end

    if(areaId ~= nil and stageType ~= nil) then
    
        model_user.stagesList = self:collectStages(areaId, stageType);
        model_user.masterPointList = self:collectMasterPoint(areaId)

        local area = self.areas[areaId];
        if(stageType == pbCommon.StageType.NORMAL) then
            model_user.stageTotalStar = area.stageTotalStar;
            model_user.stageStarTreasuresList = area.stageStarTreasures;
        else
            model_user.stageTotalStar = area.eliteStageTotalStar;
            model_user.stageStarTreasuresList = area.eliteStageStarTreasures;
        end
    end

end

function storage:collectMasterPoint(areaId) --当前地区对应普通关卡大点数组，为了显示精英大点对应的位置使用
    local arr = {} --普通关卡大点数组，为了显示精英关卡使用
    local storages = self:collectStages(areaId, pbCommon.StageType.NORMAL);
    for i =1 , #storages do 
        if storages[i].isMaster == true then
            arr[#arr + 1] = i
        end
    end
    return arr
end

function storage:isLockNextArea(areaId)
    local storages = self:collectStages(areaId, pbCommon.StageType.NORMAL);
    return storages[#storages].status > 2
end

function storage:collectAreas(islandId)
    local areas = {};

    for key, area in pairs(self.areas) do
        if(area.islandId == islandId) then
            areas[#(areas) + 1] = area;
        end
    end
    
    return areas;
end

function storage:collectStages(areaId, stageType)
    local stages = {};
    
    local storageStages;
    if(stageType == pbCommon.StageType.NORMAL) then
        storageStages = self.stages;
    else
        storageStages = self.stagesElite;
    end
    
    for key, stage in pairs(storageStages) do
        local area = self:getAreaWithStage(stage, stageType);
        
        if(area and area.areaId == areaId) then
            stages[#(stages) + 1] = stage;
        end
    end
    
    return stages;
end

function storage:isLockStage(stageId,stageType)
    local storageStages;
    if(stageType == pbCommon.StageType.NORMAL) then
        storageStages = self.stages;
    else
        storageStages = self.stagesElite;
    end
    return storageStages[stageId].status > 2
end

return storage;

