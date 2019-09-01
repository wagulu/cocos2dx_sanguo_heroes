-------------------
--  Global
-- 
-- 
-- 
Timer = _REQUIRE "src.utils.Timer"
_REQUIRE ("src.utils.NotificationCenter")

--global constants
IS_RELEASE_VERSION = SanGuoConfigLib.isReleaseVersion();
IS_DEBUG_VERSION = not(IS_RELEASE_VERSION);

HTTP_OFFLINE = IS_DEBUG_VERSION and false;
AUTO_TEST_ENABLED = IS_DEBUG_VERSION and false;
AUTO_UPDATE_ENABLED = IS_RELEASE_VERSION and true;
INTEGRATION_TEST = IS_DEBUG_VERSION and (AUTO_TEST_ENABLED or false);
PVE_QUICK_BATTLE = IS_DEBUG_VERSION and false;
START_WITH_BLANK_PAGE = IS_DEBUG_VERSION and false;

-- NOTICE!! 如果需要连接非staging的服务器环境，请在下面修改，并且打一个RELEASE_VERSION=0的包
--server url
SERVER_URL = "http://dev.api.zjfygames.com/";
--SERVER_URL = "http://dyss.dev.api.zjfygames.com/";
--SERVER_URL = "http://zhang.zhongfeng.dev.api.zjfygames.com/";
--SERVER_URL = "http://devServer.dev.api.zjfygames.com/";
--SERVER_URL = "http://deng.miao.dev.api.zjfygames.com/";
--SERVER_URL = "http://zhai.kaiyu.dev.api.zjfygames.com/"
--SERVER_URL = "http://shu.zelin.dev.api.zjfygames.com/"

-- NOTICE!! 请勿随意修改下面的URL
if (IS_RELEASE_VERSION) then
    SERVER_URL = "http://dev.api.zjfygames.com/";
end


CC_SCREEN_SIZE={
    width=cc.Director:getInstance():getVisibleSize().width,
    height=cc.Director:getInstance():getVisibleSize().height
}

SCREEN_TOP=function(node)
    node:setPositionY(CC_DESIGN_RESOLUTION.height+(CC_SCREEN_SIZE.height-CC_DESIGN_RESOLUTION.height)/2)
end

SCREEN_BOTTOM=function(node)
    node:setPositionY((CC_DESIGN_RESOLUTION.height-CC_SCREEN_SIZE.height)/2)
end

SCREEN_CENTER=function(node)
    node:setPositionY((CC_SCREEN_SIZE.height-CC_DESIGN_RESOLUTION.height)/2+CC_DESIGN_RESOLUTION.height/2)
    node:setPositionX((CC_SCREEN_SIZE.width-CC_DESIGN_RESOLUTION.width)/2 +CC_DESIGN_RESOLUTION.width/2)
end

SCREEN_DO_LAYOUT = function(node)
    node:setContentSize(cc.size(CC_SCREEN_SIZE.width,CC_SCREEN_SIZE.height));
    ccui.Helper:doLayout(node)
end

SCREEN_SCALE_BG = function(node)
    local scale = math.max((CC_SCREEN_SIZE.height / CC_DESIGN_RESOLUTION.height),(CC_SCREEN_SIZE.width / CC_DESIGN_RESOLUTION.width))
    node:setScale(scale)
end

LANG = function(str,...)
    return string.format(str,...)
end