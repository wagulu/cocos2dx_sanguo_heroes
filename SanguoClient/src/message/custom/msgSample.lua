
--module("controller/ctlListAreaRequest.lua")

local msgSample = {};

msgSample.MESSAGE_NAME = "msgSample";

function msgSample:onResponse(event)
    release_print("msgSample:response");

end


return msgSample;