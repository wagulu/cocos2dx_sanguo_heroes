
-- 0 - disable debug info, 1 - less debug info, 2 - verbose debug info
DEBUG = 2

-- use framework, will disable all deprecated API, false - use legacy API
CC_USE_FRAMEWORK = true

-- show FPS on screen
CC_SHOW_FPS = true

-- disable create unexpected global variable
CC_DISABLE_GLOBAL = false


SWITSCENE=function(scenName,...)
    if GLOBAL_MY_APP ==nil then
        GLOBAL_MY_APP= require("app.MyApp"):create()
    end
    --cc.Director:getInstance():purgeCachedData()
    --cc.Director:getInstance():getTextureCache():removeAllTextures()
   
    GLOBAL_MY_APP:run(scenName,...)
    cclog(cc.Director:getInstance():getTextureCache():getCachedTextureInfo())
end

--SHOWDLG=function(scenName)
--    if myApp ==nil then
--        myApp= require("app.MyApp"):create()
--    end
--    myApp:showDlg(scenName)
--end

local size=cc.Director:getInstance():getOpenGLView():getDesignResolutionSize();
-- for module display
CC_DESIGN_RESOLUTION = {
    width = 1136,
    height = 640,
    autoscale = "FIXED_WIDTH",
--    callback = function(framesize)
--        local ratio = framesize.width / framesize.height
--        if ratio <= 1.34 then
--            -- iPad 768*1024(1536*2048) is 4:3 screen
--            return {autoscale = "FIXED_WIDTH"}
--        end
--    end
}
