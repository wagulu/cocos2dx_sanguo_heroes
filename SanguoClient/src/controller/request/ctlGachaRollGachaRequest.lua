
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")
local helper_drop = _REQUIRE("model/helper/helper_drop.lua")
local helper_pve = _REQUIRE("model/helper/helper_pve.lua")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local model_gachaStorage = _REQUIRE("model/model_gachaStorage.lua")

local ctlGachaRollGachaRequest = class("ctlGachaRollGachaRequest", ctlBase)

function ctlGachaRollGachaRequest:name()
    return "ctlGachaRollGachaRequest";
end

function ctlGachaRollGachaRequest:onSendRequest(_gachaId)
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        gachaId = _gachaId,
    }

    local requestType = "GachaRollGachaRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "Gacha.pb?_a=rollGacha&" .. self:snid(),
        self.name());

end

function ctlGachaRollGachaRequest:onDecodePb(pb)
    local requestType = "GachaRollGachaResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlGachaRollGachaRequest:onSuccess(data)
    for key, value in pairs(data.prizes) do
        helper_drop:getDrop(value);
    end
    
    model_itemStorage:updateItem(data.item);
    
    model_gachaStorage:updateItem(data.gachaInfo);
end


return ctlGachaRollGachaRequest;
