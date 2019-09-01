local msgSignOut = {};

msgSignOut.MESSAGE_NAME = "msgSignOut";
local model_user = _REQUIRE("model.model_user")

function msgSignOut:onResponse(eventType, table)
    --local str = "Custom event 1 received, "..event.data.." times"
    release_print("msgSignOut:response");
    if  model_user.isPvp then
        ctlHighladderGetListRequest:sendRequest(handler(self,self.cutArenaPanel));
    else
--        _REQUIRE("battle/ChapterSceneData").type=2
--        SWITSCENE("battle/ChapterScene",2)
            GLOBAL_MY_APP:removeDlgs()
          SWITSCENE("battle/VersusScene")

    end
   
end

function msgSignOut:cutArenaPanel(success, data, code, msg)
    if(success) then
        SWITSCENE("arena/ArenaScene",data)
    else
    end
end

return msgSignOut;