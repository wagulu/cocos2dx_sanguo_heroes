local msgAgent = {};


local msgSample = _REQUIRE("message.custom.msgSample")
local msgGameOver = _REQUIRE("message.custom.msgGameOver")
local msgSignOut = _REQUIRE("message.custom.msgSignOut")

local LISTENER_MAP =
    {
        msgSample,
        msgGameOver,
        msgSignOut,
    }

function onMessageResponse(eventType, table)
    release_print("get message response, event = " .. eventType);

    local listener;
    for key, value in pairs(LISTENER_MAP) do
        if(value.MESSAGE_NAME == eventType) then
            listener = value;
            break;
        end
    end
    if(listener ~= nil) then
        local callback = listener.onResponse;
        if(callback ~= nil) then
            callback(listener, eventType, table)
        end
    else
        release_print("unknown message event = " .. eventType);
    end


end

--function msgAgent:initialize()
--    release_print("msgAgent:initialize");
--
--    local function onEventListener(event)
--        local eventType = event:getEventName();
--        release_print("get message response, event = %s", eventType);
--
--        local target = LISTENER_MAP[eventType];
--
--        local callback = target.onResponse;
--        if(callback ~= nil) then
--            callback(target, event)
--        else
--            release_print("unknown message event = %s", tostring(event));
--        end
--    end
--
--    for key, value in pairs(LISTENER_MAP) do
--        release_print("register message listener, key = %s, response = %s", key, value);
--        local listener = cc.EventListenerCustom:create(key, onEventListener)
--        --        release_print("listener = %s", tostring(listener));
--        local eventDispatcher = cc.Director:getInstance():getEventDispatcher();
--        --        release_print("eventDispatcher = %s", tostring(eventDispatcher));
--        --        release_print("self = %s", tostring(self));
--        eventDispatcher:addEventListenerWithFixedPriority(listener, 1);
--    --        release_print("add listener");
--    end
--
--    release_print("msgAgent:initialize over");
--
--end

return msgAgent;