_REQUIRE "src.global"

model_user = _REQUIRE("model.model_user")
model_static =_REQUIRE("model.static.model_static")
helper_pve = _REQUIRE("model.helper.helper_pve")
model_static_library =_REQUIRE("model.static.model_static_library")

_REQUIRE "src.controller.ctlGlobal"
_REQUIRE("controller.ctlAgent")
_REQUIRE("message.msgAgent")
_REQUIRE("pbHelper")
_REQUIRE("common/PopMsg")

LOADING = {};
LOADING.startApp = 0;

local function main()


    --start integration test
    if(INTEGRATION_TEST) then
        _REQUIRE("common/IntegrationTest.lua")
    else
        if not(START_WITH_BLANK_PAGE) then
            SWITSCENE("login/LoginScene")
        else
            SWITSCENE("MainScene")
        end
    end


end



main();

