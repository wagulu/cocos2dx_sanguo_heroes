cc.FileUtils:getInstance():setPopupNotify(false)
cc.FileUtils:getInstance():addSearchPath("src/")
cc.FileUtils:getInstance():addSearchPath("src/pb")
cc.FileUtils:getInstance():addSearchPath("src/app/views")
cc.FileUtils:getInstance():addSearchPath("res/")
cc.FileUtils:getInstance():addSearchPath("res/ui/")
cc.FileUtils:getInstance():addSearchPath("res/items/")

local function addUpdateFolderToSearchPath()
    local storagePath = cc.FileUtils:getInstance():getWritablePath() .. "sanguo/"
    -- 将更新目录添加到searchPath中
    local newPaths = {}
    --cc.FileUtils:getInstance():setSearchPaths({})
    local currentPaths = cc.FileUtils:getInstance():getSearchPaths()
    --release_print("root path:" .. currentPaths[1])
    -- 过滤掉重复的searchPath
    local pathExistMap = {}
    local hasEmptyPath = false
    for i = 1, #(currentPaths) do
        local p = currentPaths[i]
        if string.len(p) == 0 and not hasEmptyPath then
            hasEmptyPath = true
            table.insert(newPaths, p)
        elseif pathExistMap[p] == nil then
            pathExistMap[p] = true
            table.insert(newPaths, p)
        end
    end
    currentPaths = newPaths
    newPaths = {}
    -- 生成更新目录的searchPath
    for i = 1, #(currentPaths) do
        local p = currentPaths[i]
        local pos = string.find(p, "src/")
        if pos and (pos == 1 or string.sub(p, pos - 1, pos - 1) == "/")  then
            table.insert(newPaths, storagePath .. string.sub(p, pos))
        end
        pos = string.find(p, "res/")
        if pos and (pos == 1 or string.sub(p, pos - 1, pos - 1) == "/") then
            table.insert(newPaths, storagePath .. string.sub(p, pos))
        end
    end
    -- 将更新目录添加到searchPath的最前面
    for i = 1, #(currentPaths) do
        table.insert(newPaths, currentPaths[i])
    end
    -- 再次过滤重复的searchPath
    pathExistMap = {}
    currentPaths = newPaths
    newPaths = {}
    for i = 1, #(currentPaths) do
        local p = currentPaths[i]
        if pathExistMap[p] == nil then
            pathExistMap[p] = true
            table.insert(newPaths, p)
        end
    end
    -- 重新设置处理过的searchPath
    cc.FileUtils:getInstance():setSearchPaths(newPaths)
    -- 打印新的searchPath，测试用
    newPaths = cc.FileUtils:getInstance():getSearchPaths()
    for i = 1, #(newPaths) do
        release_print("new search paths: " .. newPaths[i])
    end
end
addUpdateFolderToSearchPath()

function _REQUIRE(filename)
    filename=string.gsub(filename,"%/",".")
    filename=string.gsub(filename,"%//",".")
    filename=string.gsub(filename,".lua","")
    filename=string.gsub(filename,"src.","")
    return require(filename)
end

_REQUIRE "config"
_REQUIRE "cocos.init"
_REQUIRE "global"

-- global cclog
cclog = function(...)
    release_print(string.format(...))
end

-- for CCLuaEngine traceback
function __G__TRACKBACK__(msg)
    cclog("----------------------------------------")
    cclog("======== LUA ERROR: " .. tostring(msg) .. "\n")
    cclog(debug.traceback())
    cclog("----------------------------------------")
    return msg
end

function initMain()
    local function onTimer()
        _REQUIRE("_main.lua");
    end

    local delayCallback = _REQUIRE("common/delayCallback.lua");
    delayCallback:create(onTimer, 0.1);
end

local function main()
    if IS_RELEASE_VERSION then
        if AUTO_UPDATE_ENABLED then
            SWITSCENE("update/UpdateScene")
        else
            SWITSCENE("splash/SplashScene")
        end
        
    else
        initMain()
    end
end

local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    release_print(msg)
end