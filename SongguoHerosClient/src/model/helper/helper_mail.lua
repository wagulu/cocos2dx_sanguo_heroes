
local model_user = _REQUIRE("model/model_user")
_REQUIRE("pb/pbCommon.lua")

local model_mailStorage = _REQUIRE("model/model_mailStorage.lua")
local delayCallback = _REQUIRE("common/delayCallback.lua");

local helper = {};

function helper:initMailList(successCallback)
    self:getList(
        function(sucess,data)
            if sucess then
            	self:getList(
                    function(sucess,data)
                        if sucess then
                        	successCallback(sucess,data)
                        end
                    end,DEPOSIT_TYPE.NEW,DEPOSIT_CHANNEL.GUILD)
            end
        end,DEPOSIT_TYPE.NEW,DEPOSIT_CHANNEL.SYS)
end

function helper:getList(successCallback,type,chanel)
    model_mailStorage:deletChanel(chanel)
    ctlDepositGetListRequest:sendRequest(function(sucess,data)
        successCallback(sucess,data)
    end,type,chanel);
end



return helper;
