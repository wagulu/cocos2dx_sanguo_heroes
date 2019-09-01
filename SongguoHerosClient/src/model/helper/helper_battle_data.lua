_REQUIRE "cocos.cocos2d.json"
_REQUIRE("pb.pbCommon")

local model_User = _REQUIRE("model.model_user")
local model_monsterSkill = _REQUIRE("model.static.model_monsterSkill")
local model_monster = _REQUIRE("model.static.model_monster")
local helper = {};
local isDuel = false;

function helper:getSelectedHeros(battleTeam, idList)
    local pbHeros = {}
    for i = 1, #(idList) do
        for key, value in pairs(battleTeam) do
            if value.id == idList[i] then
                table.insert(pbHeros, value)
                break
            end
        end
    end
    return pbHeros
end

function helper:createBase(basePB)
    local nodeBase = {
        arrowSpeed = basePB.arrowSpeed,
        speed = basePB.speed,
        strikeCD = basePB.strikeCD,
        DR = basePB.DR,
        HP = basePB.HP,
        MP = basePB.MP,
        attack = basePB.attack,
        power = basePB.power,
        atkVamp = basePB.atkVamp,
        abilityVamp = basePB.abilityVamp,
        holyDamage = basePB.holyDamage,
        atkHitRate = basePB.atkHitRate,
        abilityHitRate = basePB.abilityHitRate,
        atkDodgeRate = basePB.atkDodgeRate,
        abilityDodgeRate = basePB.abilityDodgeRate,
        atkCSR = basePB.atkCSR,
        abilityCSR = basePB.abilityCSR,
        atkAntiCSR = basePB.atkAntiCSR,
        abilityAntiCSR = basePB.abilityAntiCSR,
        physicalCSDR = basePB.physicalCSDR,
        spellCSDR = basePB.spellCSDR,
        physicalResilience = basePB.physicalResilience,
        spellResilience = basePB.spellResilience,
        ArPR = basePB.ArPR,
        SRPR = basePB.SRPR,
        physicalDR = basePB.physicalDR,
        spellDR = basePB.spellDR,
        atkVampRate = basePB.atkVampRate,
        abilityVampRate = basePB.abilityVampRate,
        healingBonus = basePB.healingBonus
    };
    return nodeBase;
end

function helper:createPropertyMin()
    local nodeBase = {
        arrowSpeed = 0,
        speed = 0,
        strikeCD = 0.4,
        DR = 0,
        HP = 0,
        MP = 0,
        attack = 0,
        power = 0,
        atkVamp = 0,
        abilityVamp = 0,
        holyDamage = 0,
        atkHitRate = 0,
        abilityHitRate = 0,
        atkDodgeRate = 0,
        abilityDodgeRate = 0,
        atkCSR = 0,
        abilityCSR = 0,
        atkAntiCSR = 0,
        abilityAntiCSR = 0,
        physicalCSDR = 0,
        spellCSDR = 0,
        physicalResilience = 0,
        spellResilience = 0,
        ArPR = 0,
        SRPR = 0,
        physicalDR = 0,
        spellDR = 0,
        atkVampRate = 0,
        abilityVampRate = 0,
        healingBonus = 0,
        physicalShield = 0,
        spellShield = 0,
        shield = 0,
        atkHolyDamage = 0,
        abilityHolyDamage = 0
    };
    return nodeBase;
end

function helper:createPropertyMax()
    local nodeBase = {
        arrowSpeed = 999,
        speed = 999,
        strikeCD = 9.99,
        DR = 1.0,
        HP = 99999999,
        MP = 99999,
        attack = 99999,
        power = 99999,
        atkVamp = 99999,
        abilityVamp = 99999,
        holyDamage = 99999,
        atkHitRate = 9.0,
        abilityHitRate = 9.0,
        atkDodgeRate = 9.0,
        abilityDodgeRate = 9.0,
        atkCSR = 9.0,
        abilityCSR = 9.0,
        atkAntiCSR = 9.0,
        abilityAntiCSR = 9.0,
        physicalCSDR = 9.0,
        spellCSDR = 9.0,
        physicalResilience = 9.0,
        spellResilience = 9.0,
        ArPR = 9.0,
        SRPR = 9.0,
        physicalDR = 9.0,
        spellDR = 9.0,
        atkVampRate = 9.0,
        abilityVampRate = 9.0,
        healingBonus = 9.0,
        physicalShield = 99999999,
        spellShield = 99999999,
        shield = 99999999,
        atkHolyDamage = 99999,
        abilityHolyDamage = 99999
    };
    return nodeBase;
end

function helper:createSkills(skillsPB)
    local nodeSkillList = {};

    for i = 1, #(skillsPB) do
        local oneSkillPB = skillsPB[i];
        if oneSkillPB.isUnlock then
            local nodeSkill = {
                config = oneSkillPB.mark,
                id = oneSkillPB.id,
                name = model_monsterSkill:getName(oneSkillPB.id),
                level = oneSkillPB.level,
                mpCost = oneSkillPB.MPCost,
                energyCost = oneSkillPB.energyCost,
                energyStart = oneSkillPB.energyStart,
                energySpeed = model_monsterSkill:getEnerySpeed(oneSkillPB.id); --20
            };

            if oneSkillPB.soldier and oneSkillPB.soldier.id > 0 then
                nodeSkill.soldier = helper:createSoldier(oneSkillPB.soldier)
            end

            table.insert(nodeSkillList, nodeSkill);
        end
    end
    
    return nodeSkillList;
end

function helper:createSoldier(soldierPB)
    local soldierNum = soldierPB.num
    if isDuel then
        --单挑模式使用真实的兵数量
        --soldierNum = 100 
    end

    local nodeSoldier = {
        id = soldierPB.id,
        soldierType = soldierPB.type,
        num = soldierNum,
        asset = soldierPB.asset,
        orderSpeed = soldierPB.orderSpeed,
        damageType = soldierPB.damageType,
        base = self:createBase(soldierPB)
    };
    return nodeSoldier;
end

function helper:createHero(monstersPB, order)
    local position = monstersPB.position
    if isDuel then
        position = "f" .. order;
    end
    
    local ai = monstersPB.AI
    if model_user.isPvp then
        ai = model_monster:getAI(monstersPB.id)
    end

    local nodeHero = {
        id = monstersPB.id,
        pos = position,
        strategy = ai,
        asset = monstersPB.asset,
        maxSoldier = monstersPB.maxSoldier,
        standOrder = 1,--TODO, lrui, multi hero
        level = monstersPB.level,
        ERHPBase = monstersPB.ERHPBase,
        ERKillBase = monstersPB.ERKillBase,
        ERLoseBase = monstersPB.ERLoseBase,
        abilityCD = monstersPB.abilityCD,
        spellCostReduction = monstersPB.spellCostReduction,
        damageType = monstersPB.damageType,
        currentHP = monstersPB.currentHP,
        currentMP = monstersPB.currentMP,
        star = monstersPB.star,
        maxStar = 5,
        base = self:createBase(monstersPB),
        skills = self:createSkills(monstersPB.skills)
    };
    --TO-DO add hero type
    return nodeHero;
end

function helper:createTeam(pbMonsters)
    local heros = {}
    for i = 1, #(pbMonsters) do
        table.insert(heros, self:createHero(pbMonsters[i], i))
    end

    local nodeTeam = {
        heros = heros,
        soldier = self:createSoldier(pbMonsters[1].soldier),
        formation = self:getUsingFormation(pbMonsters[1].formations)
    }
    return nodeTeam;
end

function helper:createDialogue()
	local nodeDialogue = {}
	
	if model_user.battleWinTimes == 0 then
	    local dramas = model_user.selectStageConfirmData.dramas
	    if dramas then
	        local nodeEnter = {}
	   
	        for i = 1, #(dramas) do
                local drama = dramas[i]
                local oneDrama = {}
                if drama.side == 1 then
                    oneDrama.side = "left"
                else
                    oneDrama.side = "right"
                end
                oneDrama.role = drama.asset
                oneDrama.name = drama.name
                oneDrama.content = drama.content
               
                table.insert(nodeEnter, oneDrama)
	        end
	        
	        if table.maxn(nodeEnter) > 0 then
                nodeDialogue.enter = nodeEnter
	        end
	    end
	end
	
    return nodeDialogue
end

function helper:getUsingFormation(formationsPB)
    local id = nil;

    if isDuel then
        id = "d1"
    else
        for i = 1, #(formationsPB) do
            local oneFormationPB = formationsPB[i];
            if (id == nil or oneFormationPB.isUse) then
                id = static_model_formations:getAsset(oneFormationPB.formationId)
                break
            end
        end
    end

    return id;
end

function helper:getBattleType()
    local battleType;
    if model_user.isPvp then
        battleType = "pvp";
    elseif model_user.selectStageBattleMode == pbCommon.BattleMode.DUEL then
        battleType = "duel";
    elseif model_user.selectStageBattleMode == pbCommon.BattleMode.NORMAL then
        battleType = "pve";
    end
    return battleType;
end

function helper:getEnterShow(leftMonsterPB, rightMonsterPB)
    local enterShowName = "enterShow_";
    if not model_user.isPvp and model_user.selectStageBattleMode == pbCommon.BattleMode.DUEL then
        enterShowName = enterShowName .. "duel";
    else
        if string.find(leftMonsterPB.position, "f") then
            enterShowName = enterShowName .. "f"
        elseif string.find(leftMonsterPB.position, "b") then
            enterShowName = enterShowName .. "b"
        end
        
        if string.find(rightMonsterPB.position, "f") then
            enterShowName = enterShowName .. "f"
        elseif string.find(rightMonsterPB.position, "b") then
            enterShowName = enterShowName .. "b"
        end
    end
    return enterShowName;
end

function helper:getXOffset()
    local xOffset = 3
    if isDuel then xOffset = 22 end
    return xOffset
end

function helper:prepareJSON()
    if HTTP_OFFLINE then
         local path = cc.FileUtils:getInstance():getWritablePath() .. "fight.json"
        if not cc.FileUtils:getInstance():isFileExist(path) then
            path = "config/fight.json"
         end
        local fight = cc.FileUtils:getInstance():getStringFromFile(path)
        return fight
    end

    local battleType = self:getBattleType()
    if battleType == "duel" then 
        isDuel = true
    else
        isDuel = false
    end

    local pbSelfHeros = self:getSelectedHeros(model_User.battleMyTeam, model_User.battleSelectMonsterIdList)
    local pbEnemyHeros = self:getSelectedHeros(model_User.battleNpcTeam, model_User.battleSelectNpcIdList)

    local table = {}
    if model_user.isPvp == true then
        table = {
            myTeam = self:createTeam(pbSelfHeros),
            enemyTeam = self:createTeam(pbEnemyHeros),
            propertyMin = self:createPropertyMin(),
            propertyMax = self:createPropertyMax(),
            battleType = battleType,
            xOffset = self:getXOffset(),
            CSDRFloorCoefficient = 1.5,
            totalTime = 120,
            enterShow = self:getEnterShow(pbSelfHeros[1], pbEnemyHeros[1]),
            background = "2",
            leftSoldierAssetStyle = "1",
            rightSoldierAssetStyle = "2",
            leftSoldierIcon = "icon_ci.png",
            rightSoldierIcon = "icon_qi.png"
        };
    else
        table = {
            dialogue = self:createDialogue(),
            myTeam = self:createTeam(pbSelfHeros),
            enemyTeam = self:createTeam(pbEnemyHeros),
            propertyMin = self:createPropertyMin(),
            propertyMax = self:createPropertyMax(),
            battleType = battleType,
            xOffset = self:getXOffset(),
            CSDRFloorCoefficient = 1.5,
            totalTime = 120,
            enterShow = self:getEnterShow(pbSelfHeros[1], pbEnemyHeros[1]),
            background = "2",
            leftSoldierAssetStyle = "1",
            rightSoldierAssetStyle = "2",
            leftSoldierIcon = "icon_ci.png",
            rightSoldierIcon = "icon_qi.png"
        };
    end
   
    return json.encode(table);
end

return helper;
