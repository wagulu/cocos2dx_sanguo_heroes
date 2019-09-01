
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")

local helper_shoplist_item = _REQUIRE("model/helper/helper_shoplist_item.lua")
local helper_drop = _REQUIRE("model/helper/helper_drop.lua")
local helper_highladderShop = _REQUIRE("model/helper/helper_highladderShop.lua")

local ctlHighladderExecBattleRequest = class("ctlHighladderExecBattleRequest", ctlBase)

function ctlHighladderExecBattleRequest:name()
    return "ctlHighladderExecBattleRequest";
end

function ctlHighladderExecBattleRequest:onSendRequest(_rank, _progress, _monsterId, _enemyMonsterId)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

        rank = _rank,
        progress = _progress,
        monsterId = _monsterId,
        enemyMonsterId = _enemyMonsterId,
    }

    local requestType = "HighladderExecBattleRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "highladder.pb?_a=execBattle&" .. self:snid(),
        self.name());


end

function ctlHighladderExecBattleRequest:onDecodePb(pb)
    local requestType = "HighladderExecBattleResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlHighladderExecBattleRequest:onSuccess(data)
--    helper_drop:getDrop(data.prize);
--    helper_highladderShop:updateItem(data.shop);
    for key, value in pairs(model_user.battleMyTeam) do
        if(value.id == data.monsterInfo.id) then
            local currentHP = value.currentHP;
            local currentMP = value.currentMP;
            model_user.battleMyTeam[key] = data.monsterInfo;
            model_user.battleMyTeam[key].currentHP = currentHP;
            model_user.battleMyTeam[key].currentMP = currentMP;
            break;
        end
    end

    for key, value in pairs(model_user.battleNpcTeam) do
        if(value.id == data.EnemyMonsterInfo.id) then
            local currentHP = value.currentHP;
            local currentMP = value.currentMP;
            model_user.battleNpcTeam[key] = data.EnemyMonsterInfo;
            model_user.battleNpcTeam[key].currentHP = currentHP;
            model_user.battleNpcTeam[key].currentMP = currentMP;
            break;
        end
    end
end


return ctlHighladderExecBattleRequest;


