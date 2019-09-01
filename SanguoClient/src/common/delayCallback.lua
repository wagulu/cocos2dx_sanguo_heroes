local Timer = _REQUIRE "src.utils.Timer"

local delayCallback = {};

function delayCallback:create(callback, delaySecond)
    local function onTimer(dt, data, timerId)
        Timer:kill(timerId);
        callback();
    end
    
    local timerId = Timer:start(onTimer, delaySecond);
    return timerId;
end

return delayCallback;
