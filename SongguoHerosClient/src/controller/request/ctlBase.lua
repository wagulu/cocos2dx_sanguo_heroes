
_REQUIRE("pbHelper")
local model_user = _REQUIRE("model.model_user")


local ctlBase = {};

--ctlBase.__isRequesting = false;
--ctlBase.__cbRequest = nil;
--ctlBase.__responseResult = false;
--ctlBase.__responseData = nil;
--ctlBase.__responseCode = 0;
--ctlBase.__responseMsg = "";

function ctlBase:name()
    return "ctlBase";
end

function ctlBase:urlBase()
    return SERVER_URL;
end

function ctlBase:snid()
    return "snId=2";
end

function ctlBase:setShowLoading(isShow)
    self.bShowLoading_ = isShow
end

function ctlBase:sendRequest(callback, ...)
    if(self.__isRequesting) then
        release_print("ERROR, request is busy! request name = " .. self:name());
        return;
    end
    
    if(HTTP_OFFLINE) then
        self:delayCallbackOffline(callback);
        return;
    end

    if(GLOBAL_MY_APP) then
       if self.bShowLoading_ then
            GLOBAL_MY_APP:showLoading()
            cclog("showLoading....................")
        end
    end

    self.__isRequesting = true;
    self.__cbRequest = callback;

    self:onSendRequest(...);
    release_print("send http request, name = " .. self:name());
 
end

function ctlBase:delayCallbackOffline(callback)
    local function onTimer(dt, data, timerId)
        Timer:kill(timerId);

        self.__responseResult = true;
        self.__responseData = {};
        self.__responseCode = 0;
        self.__responseMsg = "";
        callback(self.__responseResult, self.__responseData, self.__responseCode, self.__responseMsg);
    end

    Timer:start(onTimer, 0.001);

end

function ctlBase:cbOnline(eventType, table)
    if(GLOBAL_MY_APP) then
        if self.bShowLoading_ then
             GLOBAL_MY_APP:hideLoading()
            cclog("hideLoading....................")
        end
    end

    self.__isRequesting = false;
    local cb = self.__cbRequest;
    self.__cbRequest = nil;

    self.__responseResult = false;
    self.__responseData = nil;
    self.__responseCode = 0;
    self.__responseMsg = "网络错误";

    if(table[1]) then
        self.__responseData = self:onDecodePb(table[3]);
        self.__responseResult, self.__responseCode, self.__responseMsg = self:validateRequestResponse(self.__responseData);
    end
    
    if(self.__responseResult) then
        self:readUserDataFromResponse(self.__responseData);
        self:onSuccess(self.__responseData);
    end

    if not(self.__responseResult) then
        self:onFail();
    end

    if(cb ~= nil) then
        cb(self.__responseResult, self.__responseData, self.__responseCode, self.__responseMsg);
    end

end

function ctlBase:onSendRequest()
--to be overrid
end

function ctlBase:onDecodePb(pb)
--to be overrid
end

function ctlBase:onSuccess(data)
--to be overrid
end

function ctlBase:onFail()
    --to be overrid
    PopMsg.getInstance():flashShow(self.__responseMsg);
end

function ctlBase:validateRequestResponse(httpData)
    if(httpData == nil) then
        release_print("ERROR, http response data error!");
        return false, 0, "http response data error";
    end

    if(httpData["api"] == nil) then
        release_print("ERROR, http response [api] error!");
        return false, 0, "http response [api] error!";
    end

    local errorCode = httpData["api"]["errorNo"];
    if(errorCode ~= 0) then
        local errorMsg = httpData["api"]["errorMessage"];
        local errorStr = "ERROR, http response fail! error code = " .. tostring(errorCode) .. ", msg = " .. tostring(errorMsg)
        release_print(errorStr);
        return false, errorCode,errorMsg;
    end

    release_print("http response is OK!");
    return true, 0, "";
end


function ctlBase:readUserDataFromResponse(httpData)
    model_user:readUserDataFromResponse(httpData);
end


return ctlBase;
