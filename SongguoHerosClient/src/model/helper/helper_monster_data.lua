
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local extension_equipment = _REQUIRE("model/extension/model_extension_equipment.lua")
local extension_treasure = _REQUIRE("model/extension/model_extension_treasure.lua")
local extension_relationship_treasure = _REQUIRE("model/extension/model_extension_relationship_treasure.lua")
local extension_equipment_enhancement = _REQUIRE("model/extension/model_extension_equipment_enhancement.lua")
local extension_treasure_enhancement = _REQUIRE("model/extension/model_extension_treasure_enhancement.lua")
local extension_breaktrough = _REQUIRE("model/extension/model_extension_breakthrough.lua")
local extension_destiny = _REQUIRE("model/extension/model_extension_destiny.lua")

local helper = {};
local const = {}
helper.const = const

const.STRENGTH_TO_HP_COEFFICIENT = 18
const.STRENGTH_TO_STK_COEFFICIENT = 1.5
const.STRENGTH_TO_ARMOR_COEFFICIENT = 0.5

const.INTELLIGENCE_TO_MP_COEFFICIENT = 0.5
const.INTELLIGENCE_TO_POWER_COEFFICIENT = 2
const.INTELLIGENCE_TO_RES_COEFFICIENT = 0.8

const.LEADERSHIP_TO_STK_COEFFICIENT = 0.3
const.LEADERSHIP_TO_ARMOR_COEFFICIENT = 0.3

const.ATK_HIT_RATE_COEFFICIENT = 10000
const.ATK_DODGE_RATE_COEFFICIENT = 6000
const.ATK_CSR_COEFFICIENT = 4000
const.ATK_ANTI_CSR_COEFFICIENT = 4000
const.ATK_VAMP_COEFFICIENT = 2000

const.ABILITY_HIT_RATE_COEFFICIENT = 10000
const.ABILITY_DODGE_RATE_COEFFICIENT = 6000
const.ABILITY_CSR_COEFFICIENT = 4000
const.ABILITY_ANTI_CSR_COEFFICIENT = 4000
const.ABILITY_VAMP_COEFFICIENT = 2000

const.PHYSICAL_CSDR_COEFFICIENT = 2000
const.PHYSICAL_RESILIENCE_COEFFICIENT = 4000
const.PHYSICAL_DR_CEILING_COEFFICIENT = 0.5

const.SPELL_CSDR_COEFFICIENT = 2000
const.SPELL_RESILIENCE_COEFFICIENT = 4000
const.SPELL_DR_CEILING_COEFFICIENT = 0.5

const.ARMOR_COEFFICIENT_1 = 20
const.ARMOR_COEFFICIENT_2 = 4000
const.RESISTANCE_COEFFICIENT_1 = 20
const.RESISTANCE_COEFFICIENT_2 = 4000

const.SOLDIER_COEFFICIENT_1 = 20
const.SOLDIER_COEFFICIENT_2 = 35
const.SOLDIER_COEFFICIENT_3 = 1


-- common start

function helper:round(value, prec)
	prec = prec or 0
	local result = value * math.pow(10, prec)
	return math.floor(result + 0.5) / math.pow(10, prec) 
end

function helper:calculateTotalValue(id, attribute, value)
    local baseValue = value;
    
    value, baseValue = extension_equipment:calculateAttribute(id, attribute, value, baseValue) 
    value, baseValue = extension_treasure:calculateAttribute(id, attribute, value, baseValue) 
    value, baseValue = extension_relationship_treasure:calculateAttribute(id, attribute, value, baseValue) 
    value, baseValue = extension_equipment_enhancement:calculateAttribute(id, attribute, value, baseValue) 
    value, baseValue = extension_treasure_enhancement:calculateAttribute(id, attribute, value, baseValue) 
    value, baseValue = extension_breaktrough:calculateAttribute(id, attribute, value, baseValue) 
    value, baseValue = extension_destiny:calculateAttribute(id, attribute, value, baseValue) 
    
    return value
end

-- level up

function helper:fightCapacity(id)
    --战斗力
    return 1000
end

function helper:fightCapacityNextQuality(id)
    --进阶后战斗力
    return 1200
end

function helper:calculateStrengthNextStar(id)
    --升星后武力
    local storage = model_monsterStorage:getMonster(id)
    storage.star = storage.star + 1
    local result = self:calculateStrength(id)
    storage.star = storage.star - 1
    return result
end

function helper:calculateIntelligenceNextStar(id)
    --升星后智力
    local storage = model_monsterStorage:getMonster(id)
    storage.star = storage.star + 1
    local result = self:calculateIntelligence(id)
    storage.star = storage.star - 1
    return result
end

function helper:calculateLeadershipNextStar(id)
    --升星后统率力
    local storage = model_monsterStorage:getMonster(id)
    storage.star = storage.star + 1
    local result = self:calculateLeadership(id)
    storage.star = storage.star - 1
    return result
end

-- base start

function helper:getBaseMonsterProp(id)
    local result = {}
    
    _, result["Strength"] = self:calculateStrength(id)
    _, result["Intelligence"] = self:calculateIntelligence(id)
    _, result["Leadership"] = self:calculateLeadership(id)
    _, result["HP"] = self:calculateHP(id)
    _, result["MP"] = self:calculateMP(id)
    _, result["Attack"] = self:calculateAttack(id)
    _, result["Power"] = self:calculatePower(id)
    _, result["Armor"] = self:calculateArmor(id)
    _, result["Resistance"] = self:calculateResistance(id)
    _, result["AtkHitRateValue"] = self:calculateAtkHitRateValue(id)
    _, result["AtkHitRate"] = self:calculateAtkHitRate(id)
    --_, result["AbilityHitRateValue"] = self:calculateAbilityHitRateValue(id)
    --_, result["AbilityHitRate"] = self:calculateAbilityHitRate(id)
    _, result["AtkDodgeRateValue"] = self:calculateAtkDodgeRateValue(id)
    _, result["AtkDodgeRate"] = self:calculateAtkDodgeRate(id)
    _, result["AbilityDodgeRateValue"] = self:calculateAbilityDodgeRateValue(id)
    _, result["AbilityDodgeRate"] = self:calculateAbilityDodgeRate(id)
    _, result["AtkCSV"] = self:calculateAtkCSV(id)
    _, result["AtkCSR"] = self:calculateAtkCSR(id)
    _, result["AbilityCSV"] = self:calculateAbilityCSV(id)
    _, result["AbilityCSR"] = self:calculateAbilityCSR(id)
    _, result["AtkAntiCSV"] = self:calculateAtkAntiCSV(id)
    _, result["AtkAntiCSR"] = self:calculateAtkAntiCSR(id)
    _, result["AbilityAntiCSV"] = self:calculateAbilityAntiCSV(id)
    _, result["AbilityAntiCSR"] = self:calculateAbilityAntiCSR(id)
    _, result["PhysicalCSDV"] = self:calculatePhysicalCSDV(id)
    _, result["PhysicalCSDR"] = self:calculatePhysicalCSDR(id)
    _, result["SpellCSDV"] = self:calculateSpellCSDV(id)
    _, result["SpellCSDR"] = self:calculateSpellCSDR(id)
    _, result["PhysicalResilienceValue"] = self:calculatePhysicalResilienceValue(id)
    _, result["PhysicalResilience"] = self:calculatePhysicalResilience(id)
    _, result["SpellResilienceValue"] = self:calculateSpellResilienceValue(id)
    _, result["SpellResilience"] = self:calculateSpellResilience(id)
    _, result["ArPR"] = self:calculateArPR(id)
    _, result["SRPR"] = self:calculateSRPR(id)
    _, result["HealingBonus"] = self:calculateHealingBonus(id)
    _, result["ArPV"] = self:calculateArPV(id)
    _, result["PhysicalDR"] = self:calculatePhysicalDR(id)
    _, result["SRPV"] = self:calculateSRPV(id)
    _, result["SpellDR"] = self:calculateSpellDR(id)
    _, result["AtkVamp"] = self:calculateAtkVamp(id)
    _, result["AtkVampRateValue"] = self:calculateAtkVampRateValue(id)
    _, result["AtkVampRate"] = self:calculateAtkVampRate(id)
    _, result["AbilityVamp"] = self:calculateAbilityVampRateValue(id)
    _, result["AbilityVampRateValue"] = self:calculateAbilityVampRateValue(id)
    _, result["AbilityVampRate"] = self:calculateAbilityVampRate(id)
    _, result["MaxSoldier"] = self:calculateMaxSoldier(id)
    _, result["HolyDamage"] = self:calculateHolyDamage(id)
    
    return result
end

function helper:getFinalMonsterProp(id)
    local result = {}

    result["Strength"] = self:calculateStrength(id)
    result["Intelligence"] = self:calculateIntelligence(id)
    result["Leadership"] = self:calculateLeadership(id)
    result["HP"] = self:calculateHP(id)
    result["MP"] = self:calculateMP(id)
    result["Attack"] = self:calculateAttack(id)
    result["Power"] = self:calculatePower(id)
    result["Armor"] = self:calculateArmor(id)
    result["Resistance"] = self:calculateResistance(id)
    result["AtkHitRateValue"] = self:calculateAtkHitRateValue(id)
    result["AtkHitRate"] = self:calculateAtkHitRate(id)
    --result["AbilityHitRateValue"] = self:calculateAbilityHitRateValue(id)
    --result["AbilityHitRate"] = self:calculateAbilityHitRate(id)
    result["AtkDodgeRateValue"] = self:calculateAtkDodgeRateValue(id)
    result["AtkDodgeRate"] = self:calculateAtkDodgeRate(id)
    result["AbilityDodgeRateValue"] = self:calculateAbilityDodgeRateValue(id)
    result["AbilityDodgeRate"] = self:calculateAbilityDodgeRate(id)
    result["AtkCSV"] = self:calculateAtkCSV(id)
    result["AtkCSR"] = self:calculateAtkCSR(id)
    result["AbilityCSV"] = self:calculateAbilityCSV(id)
    result["AbilityCSR"] = self:calculateAbilityCSR(id)
    result["AtkAntiCSV"] = self:calculateAtkAntiCSV(id)
    result["AtkAntiCSR"] = self:calculateAtkAntiCSR(id)
    result["AbilityAntiCSV"] = self:calculateAbilityAntiCSV(id)
    result["AbilityAntiCSR"] = self:calculateAbilityAntiCSR(id)
    result["PhysicalCSDV"] = self:calculatePhysicalCSDV(id)
    result["PhysicalCSDR"] = self:calculatePhysicalCSDR(id)
    result["SpellCSDV"] = self:calculateSpellCSDV(id)
    result["SpellCSDR"] = self:calculateSpellCSDR(id)
    result["PhysicalResilienceValue"] = self:calculatePhysicalResilienceValue(id)
    result["PhysicalResilience"] = self:calculatePhysicalResilience(id)
    result["SpellResilienceValue"] = self:calculateSpellResilienceValue(id)
    result["SpellResilience"] = self:calculateSpellResilience(id)
    result["ArPR"] = self:calculateArPR(id)
    result["SRPR"] = self:calculateSRPR(id)
    result["HealingBonus"] = self:calculateHealingBonus(id)
    result["ArPV"] = self:calculateArPV(id)
    result["PhysicalDR"] = self:calculatePhysicalDR(id)
    result["SRPV"] = self:calculateSRPV(id)
    result["SpellDR"] = self:calculateSpellDR(id)
    result["AtkVamp"] = self:calculateAtkVamp(id)
    result["AtkVampRateValue"] = self:calculateAtkVampRateValue(id)
    result["AtkVampRate"] = self:calculateAtkVampRate(id)
    result["AbilityVamp"] = self:calculateAbilityVampRateValue(id)
    result["AbilityVampRateValue"] = self:calculateAbilityVampRateValue(id)
    result["AbilityVampRate"] = self:calculateAbilityVampRate(id)
    result["MaxSoldier"] = self:calculateMaxSoldier(id)
    result["HolyDamage"] = self:calculateHolyDamage(id)

    return result
end

function helper:calculateStrength(id)
    --角色力量值=初始力量值+初始力量成长值*(角色等级-1)*(星级增量+品质增量)+其他系统附加
    local monster = static_model_monster:getNode(id)
    local storage = model_monsterStorage:getMonster(id)
    if storage then
        local qualityGain = static_model_monsterQuality:getGain(storage.quality)
        local starGain = static_model_monsterStar:getGain(storage.star)
        local baseStrength = monster.baseStrength + monster.strengthGain * (storage.level - 1) * (1 + qualityGain + starGain)
        return self:round(self:calculateTotalValue(id, "Strength", baseStrength), 2), self:round(baseStrength, 2)
    end
    return 0
end

function helper:calculateIntelligence(id)
    --总智力值=初始智力+智力成长*(角色等级-1)*(1+品质增量+星级增量)+其他系统附加
    local monster = static_model_monster:getNode(id)
    local storage = model_monsterStorage:getMonster(id)
    if storage then
        local qualityGain = static_model_monsterQuality:getGain(storage.quality)
        local starGain = static_model_monsterStar:getGain(storage.star)
        local baseIntelligence = monster.baseIntelligence + monster.intelligenceGain * (storage.level - 1) * (1 + qualityGain + starGain)
        return self:round(self:calculateTotalValue(id, "Intelligence", baseIntelligence), 2), self:round(baseIntelligence, 2)
    end
    return 0
end

function helper:calculateLeadership(id)
    --总统帅值=初始统帅+统帅成长*(角色等级-1)*(1+品质增量+星级增量)+其他系统附加
    local monster = static_model_monster:getNode(id)
    local storage = model_monsterStorage:getMonster(id)
    if storage then
        local qualityGain = static_model_monsterQuality:getGain(storage.quality)
        local starGain = static_model_monsterStar:getGain(storage.star)
        local baseLeadership = monster.baseLeadership + monster.leadershipGain * (storage.level - 1) * (1 + qualityGain + starGain);
        return self:round(self:calculateTotalValue(id, "Leadership", baseLeadership), 2), self:round(baseLeadership, 2)
    end
    return 0
end

function helper:calculateHP(id) 
    --力量值*力量转HP系数+初始HP
    local monster = static_model_monster:getNode(id)
    local baseHP = monster.baseHP + self:calculateStrength(id) * self.const.STRENGTH_TO_HP_COEFFICIENT    
    return self:round(self:calculateTotalValue(id, "HP", baseHP)), self:round(baseHP)
end

function helper:calculateMP(id)
    --智力值*智力转MP系数+初始MP
    local monster = static_model_monster:getNode(id)
    local baseMP = monster.baseMP + self:calculateIntelligence(id) * self.const.INTELLIGENCE_TO_MP_COEFFICIENT
    return self:round(self:calculateTotalValue(id, "MP", baseMP)), self:round(baseMP)
end

function helper:calculateAttack(id)
    --（力量值*力量转物攻系数+统帅值*统帅转物攻系数+初始物理攻击
    local monster = static_model_monster:getNode(id)
    local baseAttack = monster.baseAtk + self:calculateStrength(id) * self.const.STRENGTH_TO_STK_COEFFICIENT + self:calculateLeadership(id) * self.const.LEADERSHIP_TO_STK_COEFFICIENT
    return self:round(self:calculateTotalValue(id, "Attack", baseAttack)), self:round(baseAttack)
end

function helper:calculatePower(id)
    --智力值*智力转法攻系数+初始法术攻击
    local monster = static_model_monster:getNode(id)
    local basePower = monster.basePower + self:calculateIntelligence(id) * self.const.INTELLIGENCE_TO_POWER_COEFFICIENT
    return self:round(self:calculateTotalValue(id, "Power", basePower)), self:round(basePower)
end

function helper:calculateArmor(id)
    --（力量值*力量转物理护甲系数+统帅值*统帅转物理护甲系数+初始物理护甲）
    local monster = static_model_monster:getNode(id)
    local baseArmor = monster.baseAr + self:calculateStrength(id) * self.const.STRENGTH_TO_ARMOR_COEFFICIENT + self:calculateLeadership(id) * self.const.LEADERSHIP_TO_ARMOR_COEFFICIENT
    return self:round(self:calculateTotalValue(id, "Armor", baseArmor)), self:round(baseArmor)
end

function helper:calculateResistance(id)
    --法术防御=智力值*智力转法术防御系数+初始法术防御+其他系统增加的法术防御
    local monster = static_model_monster:getNode(id)
    local baseResistance = monster.baseRes + self:calculateIntelligence(id) * self.const.INTELLIGENCE_TO_RES_COEFFICIENT
    return self:round(self:calculateTotalValue(id, "Resistance", baseResistance)), self:round(baseResistance)
end


-- expand start

function helper:calculateAtkHitRateValue(id)
    local baseAtkHitRateValue = 0;
    return self:calculateTotalValue(id, "AtkHitRateValue", baseAtkHitRateValue), 0
end

function helper:calculateAtkHitRate(id, atkHitRateValue)
    --普攻命中值/（普攻命中值+普攻命中系数）+初始普攻命中率
    atkHitRateValue = atkHitRateValue or 0
    local expand = static_model_monsterExpand:getNode(id)
    local baseAtkHitRate = expand.baseAtkHitRate + atkHitRateValue / (atkHitRateValue + self.const.ATK_HIT_RATE_COEFFICIENT)
    return self:round(self:calculateTotalValue(id, "AtkHitRate", baseAtkHitRate), 4), self:round(baseAtkHitRate, 4)
end

function helper:calculateAbilityHitRateValue(id)
    local baseAbilityHitRateValue = 0
    return self:calculateTotalValue(id, "AbilityHitRateValue", baseAbilityHitRateValue), 0
end

function helper:calculateAbilityHitRate(id, abilityHitRateValue)
    --技能命中值/（技能命中值+技能命中系数）+配置
    abilityHitRateValue = abilityHitRateValue or 0
    local expand = static_model_monsterExpand:getNode(id)
    local baseAbilityHitRate = expand.baseAbilityHitRate + abilityHitRateValue / (abilityHitRateValue + self.const.ABILITY_HIT_RATE_COEFFICIENT)
    return self:round(self:calculateTotalValue(id, "AbilityHitRate", baseAbilityHitRate), 4), self:round(baseAbilityHitRate, 4)
end

function helper:calculateAtkDodgeRateValue(id)
    local baseAtkDodgeRateValue = 0;
    return self:calculateTotalValue(id, "AtkDodgeRateValue", baseAtkDodgeRateValue), 0
end

function helper:calculateAtkDodgeRate(id, atkDodgeRateValue) 
    --普攻闪避值/（普攻闪避值+普攻闪避系数）
    atkDodgeRateValue = atkDodgeRateValue or 0
    local expand = static_model_monsterExpand:getNode(id)
    local baseAtkDodgeRate = expand.baseAtkDodgeRate + atkDodgeRateValue / (atkDodgeRateValue + self.const.ATK_DODGE_RATE_COEFFICIENT)
    return self:round(self:calculateTotalValue(id, "AtkDodgeRate", baseAtkDodgeRate), 4), self:round(baseAtkDodgeRate, 4)
end

function helper:calculateAbilityDodgeRateValue(id)
    local baseAbilityDodgeRateValue = 0
    return self:calculateTotalValue(id, "AbilityDodgeRateValue", baseAbilityDodgeRateValue), 0
end

function helper:calculateAbilityDodgeRate(id, abilityDodgeRateValue) 
    --技能闪避值/（技能闪避值+技能闪避系数）+配置
    abilityDodgeRateValue = abilityDodgeRateValue or 0
    local expand = static_model_monsterExpand:getNode(id)
    local baseAbilityDodgeRate = expand.baseAbilityDodgeRate + abilityDodgeRateValue / (abilityDodgeRateValue + self.const.ABILITY_DODGE_RATE_COEFFICIENT)
    return self:round(self:calculateTotalValue(id, "AbilityDodgeRate", baseAbilityDodgeRate), 4), self:round(baseAbilityDodgeRate, 4)
end

function helper:calculateAtkCSV(id)
    local baseAtkCSV = 0
    return self:calculateTotalValue(id, "AtkCSV", baseAtkCSV), 0
end

function helper:calculateAtkCSR(id, atkCSV) 
    --普攻暴击值/（普攻暴击值+普攻暴击系数）+配置
    atkCSV = atkCSV or 0
    local expand = static_model_monsterExpand:getNode(id)
    local baseAtkCSR =expand.baseAtkCSR + atkCSV / (atkCSV + self.const.ATK_CSR_COEFFICIENT)
    return self:round(self:calculateTotalValue(id, "AtkCSR", baseAtkCSR), 4), self:round(baseAtkCSR, 4)
end

function helper:calculateAbilityCSV(id) 
    local baseAbilityCSV = 0
    return self:calculateTotalValue(id, "AbilityCSV", baseAbilityCSV), 0
end

function helper:calculateAbilityCSR(id, abilityCSV)
    --技能暴击率=技能暴击值/（技能暴击值+技能暴击系数）+配置+其他系统附加
    abilityCSV = abilityCSV or 0
    local expand = static_model_monsterExpand:getNode(id)
    local baseAbilityCSR = expand.baseAbilityCSR + abilityCSV / (abilityCSV + self.const.ABILITY_CSR_COEFFICIENT)
    return self:round(self:calculateTotalValue(id, "AbilityCSR", baseAbilityCSR), 4), self:round(baseAbilityCSR, 4)
end

function helper:calculateAtkAntiCSV(id)
    local baseAtkAntiCSV = 0;
    return self:calculateTotalValue(id, "AtkAntiCSV", baseAtkAntiCSV), 0
end

function helper:calculateAtkAntiCSR(id, atkAntiCSV)
    --普攻暴抗值/（普攻暴抗值+普攻爆抗系数）+配置
    atkAntiCSV = atkAntiCSV or 0
    local expand = static_model_monsterExpand:getNode(id)
    local baseAtkAntiCSR = expand.baseAtkAntiCSR + atkAntiCSV / (atkAntiCSV + self.const.ATK_ANTI_CSR_COEFFICIENT);
    return self:round(self:calculateTotalValue(id, "AtkAntiCSR", baseAtkAntiCSR), 4), self:round(baseAtkAntiCSR, 4)
end

function helper:calculateAbilityAntiCSV(id) 
    local baseAbilityAntiCSV = 0
    return self:calculateTotalValue(id, "AbilityAntiCSV", baseAbilityAntiCSV), self:round(baseAbilityAntiCSV)
end

function helper:calculateAbilityAntiCSR(id, abilityAntiCSV) 
    --技能暴抗值/（技能暴抗值+技能爆抗系数）+配置
    abilityAntiCSV = abilityAntiCSV or 0
    local expand = static_model_monsterExpand:getNode(id)
    local baseAbilityAntiCSR = expand.baseAbilityAntiCSR + abilityAntiCSV / (abilityAntiCSV + self.const.ABILITY_ANTI_CSR_COEFFICIENT)
    return self:round(self:calculateTotalValue(id, "AbilityAntiCSR", baseAbilityAntiCSR), 4), self:round(baseAbilityAntiCSR, 4)
end

function helper:calculatePhysicalCSDV(id) 
    local basePhysicalCSDV = 0
    return self:round(self:calculateTotalValue(id, "PhysicalCSDV", basePhysicalCSDV)), 0
end

function helper:calculatePhysicalCSDR(id, physicalCSDV) 
    --1+物理爆伤值/物理爆伤系数+配置
    physicalCSDV = physicalCSDV or 0
    local expand = static_model_monsterExpand:getNode(id)
    local basePhysicalCSDR = expand.basePhysicalCSDR + 1 + physicalCSDV / self.const.PHYSICAL_CSDR_COEFFICIENT
    return self:round(self:calculateTotalValue(id, "PhysicalCSDR", basePhysicalCSDR), 4), self:round(basePhysicalCSDR, 4)
end

function helper:calculateSpellCSDV(id) 
    local baseSpellCSDV = 0;
    return self:round(self:calculateTotalValue(id, "SpellCSDV", baseSpellCSDV)), 0
end

function helper:calculateSpellCSDR(id, spellCSDV) 
    --1+法术爆伤值/法术爆伤系数+配置
    spellCSDV = spellCSDV or 0
    local expand = static_model_monsterExpand:getNode(id)
    local baseSpellCSDR = expand.baseSpellCSDR + 1 + spellCSDV / self.const.SPELL_CSDR_COEFFICIENT
    return self:round(self:calculateTotalValue(id, "SpellCSDR", baseSpellCSDR), 4), self:round(baseSpellCSDR, 4)
end

function helper:calculatePhysicalResilienceValue(id) 
    local basePhysicalResilienceValue = 0;
    return self:round(self:calculateTotalValue(id, "PhysicalResilienceValue", basePhysicalResilienceValue)), 0
end

function helper:calculatePhysicalResilience(id, physicalResilienceValue) 
    --物理韧性值/物理韧性系数+配置+其他系统附加
    physicalResilienceValue = physicalResilienceValue or 0
    local expand = static_model_monsterExpand:getNode(id)
    local basePhysicalResilience = expand.basePhysicalResilience + physicalResilienceValue / self.const.PHYSICAL_RESILIENCE_COEFFICIENT
    return self:round(self:calculateTotalValue(id, "PhysicalResilience", basePhysicalResilience), 4), self:round(basePhysicalResilience, 4)
end

function helper:calculateSpellResilienceValue(id) 
    local baseSpellResilienceValue = 0
    return self:round(self:calculateTotalValue(id, "SpellResilienceValue", baseSpellResilienceValue)), 0
end

function helper:calculateSpellResilience(id, spellResilienceValue) 
    --法术韧性值/法术韧性系数+配置
    spellResilienceValue = spellResilienceValue or 0
    local expand = static_model_monsterExpand:getNode(id)
    local baseSpellResilience = expand.baseSpellResilience + spellResilienceValue / self.const.SPELL_RESILIENCE_COEFFICIENT
    return self:round(self:calculateTotalValue(id, "SpellResilience", baseSpellResilience), 4), self:round(baseSpellResilience, 4)
end

function helper:calculateArPR(id) 
    --1+配置
    local expand = static_model_monsterExpand:getNode(id)
    local baseArPR = expand.baseArPR + 1 + (self:calculateArPV(id) / (self:calculateArPV(id) + self.const.ARMOR_COEFFICIENT_2))
    return self:round(self:calculateTotalValue(id, "ArPR", baseArPR), 4), self:round(baseArPR, 4)
end

function helper:calculateSRPR(id) 
    --1+配置
    local expand = static_model_monsterExpand:getNode(id)
    local baseSRPR = expand.baseSRPR + 1 + (self:calculateSRPV(id) / (self:calculateSRPV(id) + self.const.RESISTANCE_COEFFICIENT_2))
    return self:round(self:calculateTotalValue(id, "SRPR", baseSRPR), 4), self:round(baseSRPR, 4)
end

function helper:calculateHealingBonus(id) 
    local baseHealingBonus = 0
    return self:round(self:calculateTotalValue(id, "HealingBonus", baseHealingBonus), 4), 0
end

function helper:calculateArPV(id) 
    local baseArPV = 0;
    return self:round(self:calculateTotalValue(id, "ArPV", baseArPV)), 0
end

function helper:calculatePhysicalDR(id) 
    --物理伤害减免率=min（（物理护甲值-物理护甲系数1）/（物理护甲-物理护甲系数1+物理护甲系数2），物伤减免上限系数）+初始物理伤害减免率+其他系统附加
    --物理护甲
    local expand = static_model_monsterExpand:getNode(id)
    local armor = self:calculateArmor(id) - self.const.ARMOR_COEFFICIENT_1
    local basePhysicalDR = expand.basePhysicalDR + math.min(armor / (armor + self.const.ARMOR_COEFFICIENT_2) , self.const.PHYSICAL_DR_CEILING_COEFFICIENT)
    return self:round(self:calculateTotalValue(id, "PhysicalDR", basePhysicalDR), 4), self:round(basePhysicalDR, 4)
end

function helper:calculateSRPV(id) 
    local baseSRPV = 0
    return self:round(self:calculateTotalValue(id, "SRPV", baseSRPV)), 0
end

function helper:calculateSpellDR(id) 
    --法术伤害减免率=min（（法术防御值-法术防御系数1）/（法术防御-法术防御系数1+法术防御系数2），法伤减免上限系数）+初始法术伤害减免率+其他系统附加
    --法术防御
    local expand = static_model_monsterExpand:getNode(id)
    local resistance = self:calculateResistance(id) - self.const.RESISTANCE_COEFFICIENT_1
    local baseSpellDR = expand.baseSpellDR + math.min(resistance / (resistance + self.const.RESISTANCE_COEFFICIENT_2), self.const.SPELL_DR_CEILING_COEFFICIENT)
    return self:round(self:calculateTotalValue(id, "SpellDR", baseSpellDR), 4), self:round(baseSpellDR, 4)
end

function helper:calculateAtkVamp(id) 
    local baseAtkVamp = 0
    return self:round(self:calculateTotalValue(id, "AtkVamp", baseAtkVamp)), 0
end

function helper:calculateAtkVampRateValue(id) 
    local baseAtkVampRateValue = 0
    return self:round(self:calculateTotalValue(id, "AtkVampRateValue", baseAtkVampRateValue)), 0
end

function helper:calculateAtkVampRate(id, atkVampRateValue) 
    --普攻吸血值/（普攻吸血值+普攻吸血系数）+配置
    atkVampRateValue = atkVampRateValue or 0
    local expand = static_model_monsterExpand:getNode(id)
    local baseAtkVampRate = expand.baseAtkVampRate + atkVampRateValue / (atkVampRateValue + self.const.ATK_VAMP_COEFFICIENT)
    return self:round(self:calculateTotalValue(id, "AtkVampRate", baseAtkVampRate), 4), self:round(baseAtkVampRate, 4)
end

function helper:calculateAbilityVampRateValue(id) 
    local baseAbilityVamp = 0
    return self:round(self:calculateTotalValue(id, "AbilityVamp", baseAbilityVamp)), 0
end

function helper:calculateAbilityVampRateValue(id) 
    local baseAbilityVampRateValue = 0
    return self:round(self:calculateTotalValue(id, "AbilityVampRateValue", baseAbilityVampRateValue)), 0
end

function helper:calculateAbilityVampRate(id, abilityVampRateValue) 
    --技能吸血值/（技能吸血值+技能吸血系数）+配置
    abilityVampRateValue = abilityVampRateValue or 0
    local expand = static_model_monsterExpand:getNode(id)
    local baseAbilityVampRate = expand.baseAbilityVampRate + abilityVampRateValue / (abilityVampRateValue + self.const.ABILITY_VAMP_COEFFICIENT)
    return self:round(self:calculateTotalValue(id, "AbilityVampRate", baseAbilityVampRate), 4), self:round(baseAbilityVampRate, 4)
end

function helper:calculateMaxSoldier(id) 
    --统帅值/带兵数系数1+带兵数系数2+角色等级/带兵数系数3
    local storage = model_monsterStorage:getMonster(id)
    local baseMaxSoldier = self:calculateLeadership(id) / self.const.SOLDIER_COEFFICIENT_1 + self.const.SOLDIER_COEFFICIENT_2 + storage.level / self.const.SOLDIER_COEFFICIENT_3
    return self:round(self:calculateTotalValue(id, "MaxSoldier", baseMaxSoldier)), self:round(baseMaxSoldier)
end

function helper:calculateHolyDamage(id) 
    local baseHolyDamage = 0
    return self:round(self:calculateTotalValue(id, "HolyDamage", baseHolyDamage)), 0
end

return helper

