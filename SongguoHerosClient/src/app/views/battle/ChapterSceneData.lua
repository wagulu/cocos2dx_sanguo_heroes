local model_area = _REQUIRE "model/static/model_area.lua"
local model_areaStorage = _REQUIRE("model/model_areaStorage.lua")
local ChapterSceneData={
    areaIndex=1
}

function ChapterSceneData:getStageData()
    if HTTP_OFFLINE then
        return {{id=1,status=3,isMaster=true}}--战斗点状态 0未开放 1未解锁 2未清关 3未完成 4已完成
    end
    return  model_user.stagesList
end

function ChapterSceneData:getAreaData()
    if HTTP_OFFLINE then
        return {{id=1,status=4},{id=2,status=4}}--//场景状态 0：未开放 1：未解锁 2：未清关 3：未完成 4：已完成
    end
    return  model_user.areasList
end


--    model_user.npc = data["NPC"];
--    model_user.battleTeam = data["battleTeam"];
--    model_user.treasures = data["treasures"];
--    model_user.userStage = data["userStage"];


function ChapterSceneData:getNpc()
    if HTTP_OFFLINE then
        return {}
    end
    return  model_user.selectStageConfirmData["NPC"]
end

function ChapterSceneData:getBattleTeam()
    if HTTP_OFFLINE then
        return {}
    end
    return  model_user.battleMyTeam; --.selectStageConfirmData["battleTeam"]
end

function ChapterSceneData:getTreasures()
    if HTTP_OFFLINE then
        return {}
    end
    return  model_user.selectStageConfirmData["treasures"]
end

function ChapterSceneData:getUserStage()
    if HTTP_OFFLINE then
        return {}
    end
    return  model_user.battleStage
end



function ChapterSceneData:getAreaLen()
    return  #(self:getAreaData())
end

function ChapterSceneData:getMaxLen()
    return  #(self:getStageData())
end

function ChapterSceneData:getMaxStar()
    
    return  model_user.stageTotalStar
end

function ChapterSceneData:isDisplayPoint()
    local stages = self:getStageData()
    return stages[#stages].status <= 2 
end

function ChapterSceneData:isSwitch()
    return GLOBAL_MY_APP.isSwitch == true and self.areaIndex == GLOBAL_MY_APP.selectAreaId
end


function ChapterSceneData:getAreaName()
    return model_area:getName(self.areaIndex)
end

function ChapterSceneData:getTotalStar()
    local data = self:getStageData()
    local count = 0 
    for i=1,#data do
        if data[i].isMaster == true then
        	count = count + 1
        end
    end
    return count * 3
end

function ChapterSceneData:starTreasuresList()--
    return model_user.stageStarTreasuresList
end

function ChapterSceneData:getCurrentIndex()--当前在哪个节点
    local index =1
    local stages = self:getStageData()
    for i=1,self:getMaxLen() do
        if stages[i].status>=2 then
        	index = math.max(index,i)
        end
    end
    return index
end

function ChapterSceneData:getStageInfo(index)--通过index 获取stageInfo数据
    local storageStages = {}
    if(model_user.selectStageType == pbCommon.StageType.NORMAL) then
        storageStages = model_areaStorage.stages;
    else
        storageStages = model_areaStorage.stagesElite;
    end
    if index<=#storageStages then
        return storageStages[index]
    end
    return {}
end

function ChapterSceneData:getAreaInfo(index)--通过index 获取章节数据
    if index<=self:getMaxLen() then
        return self:getAreaData()[index]
    end
    return {}
end



--function ChapterSceneData:isLockMaster()--是否解锁精英管卡
--    local stages = self:getStageData()
--    return stages[#stages].status > 2
--end

function ChapterSceneData:isAttackNode(itemInfo)--该解决是否被打过
    return itemInfo.status > 1
end

return ChapterSceneData