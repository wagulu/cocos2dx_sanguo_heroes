
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlPlayerBuySkillUpPointRequest = class("ctlPlayerBuySkillUpPointRequest", ctlBase)

function ctlPlayerBuySkillUpPointRequest:name()
    return "ctlPlayerBuySkillUpPointRequest";
end

function ctlPlayerBuySkillUpPointRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "PlayerBuySkillUpPointRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "player.pb?_a=buySkillUpPoint&" .. self:snid(),
        self.name());


end

function ctlPlayerBuySkillUpPointRequest:onDecodePb(pb)
    local requestType = "PlayerBuySkillUpPointResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlPlayerBuySkillUpPointRequest:onSuccess(data)
    
end


return ctlPlayerBuySkillUpPointRequest;


