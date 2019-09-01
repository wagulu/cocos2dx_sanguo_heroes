

_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")

local helper_drop = _REQUIRE("model/helper/helper_drop.lua")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_areaStorage = _REQUIRE("model/model_areaStorage.lua")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_mailStorage = _REQUIRE("model/model_mailStorage.lua")

local ctlDepositReceiveRequest = class("ctlDepositReceiveRequest", ctlBase)

function ctlDepositReceiveRequest:name()
    return "ctlDepositReceiveRequest";
end

local lastKey;
local lastChannel;

function ctlDepositReceiveRequest:onSendRequest(_key, _channel)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        key = _key,
        channel = _channel,
    }
    
    lastKey = _key;
    lastChannel = _channel;

    local requestType = "DepositReceiveRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "deposit.pb?_a=receive&" .. self:snid(),
        self.name());


end

function ctlDepositReceiveRequest:onDecodePb(pb)
    local requestType = "DepositReceiveResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end


function ctlDepositReceiveRequest:onSuccess(data)
    local prizes = data.prizes;

    for key, value in pairs(prizes) do
        helper_drop:getDrop(value);
    end
    
    local mail = model_mailStorage:getNode(lastKey, lastChannel);
    if(mail) then
        model_mailStorage:setRead(lastKey, lastChannel);

--        1，看过就已读，2，获取为已读，3，获取完删除
        if(mail.actionType == 1) then
        elseif (mail.actionType == 2) then
        elseif (mail.actionType == 3) then
            model_mailStorage:deleteNode(lastKey, lastChannel);
        end
    end
    
end


return ctlDepositReceiveRequest;

