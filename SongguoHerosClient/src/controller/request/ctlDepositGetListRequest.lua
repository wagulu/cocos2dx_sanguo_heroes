

_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")

DEPOSIT_TYPE = {};
DEPOSIT_TYPE.NEW = "new";
DEPOSIT_TYPE.HISTORY = "history";

DEPOSIT_CHANNEL = {};
DEPOSIT_CHANNEL.SYS = "sys";
DEPOSIT_CHANNEL.GUILD = "guild";


local helper_drop = _REQUIRE("model/helper/helper_drop.lua")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_areaStorage = _REQUIRE("model/model_areaStorage.lua")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_mailStorage = _REQUIRE("model/model_mailStorage.lua")

local ctlDepositGetListRequest = class("ctlDepositGetListRequest", ctlBase)

function ctlDepositGetListRequest:name()
    return "ctlDepositGetListRequest";
end

local lastChannel;

function ctlDepositGetListRequest:onSendRequest(_type, _channel)

    _type = _type or DEPOSIT_TYPE.NEW;
    _channel = _channel or DEPOSIT_CHANNEL.SYS;

    lastChannel = _channel;

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        type = _type,
        channel = _channel,
    }

    local requestType = "DepositGetListRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "deposit.pb?_a=getList&" .. self:snid(),
        self.name());


end

function ctlDepositGetListRequest:onDecodePb(pb)
    local requestType = "DepositGetListResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end


function ctlDepositGetListRequest:onSuccess(data)
    for key, value in pairs(data.deposits) do
        model_mailStorage:setNode(value, lastChannel);
    end
end



return ctlDepositGetListRequest;

