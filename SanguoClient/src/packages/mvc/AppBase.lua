
local AppBase = class("AppBase")

function AppBase:ctor(configs)
    self.configs_ = {
        viewsRoot  = "app.views",
        modelsRoot = "app.models",
        defaultSceneName = "MainScene",
    }
    self.showLoadingRefCount = 0
    for k, v in pairs(configs or {}) do
        self.configs_[k] = v
    end

    if type(self.configs_.viewsRoot) ~= "table" then
        self.configs_.viewsRoot = {self.configs_.viewsRoot}
    end
    if type(self.configs_.modelsRoot) ~= "table" then
        self.configs_.modelsRoot = {self.configs_.modelsRoot}
    end

    if DEBUG > 1 then
        dump(self.configs_, "AppBase configs")
    end

    if CC_SHOW_FPS then
        cc.Director:getInstance():setDisplayStats(true)
    end
    self.isSwitch = false
    self.switchDlgs = {} --跳到提前界面需要移除的dlg
    self.allDlgs ={}
    -- event
    self:onCreate()
end

function AppBase:run(initSceneName,...)
    initSceneName = initSceneName or self.configs_.defaultSceneName
--    local arg ={...}
--    table.insert(arg,1,initSceneName)
--    self:enterScene("loading/SwitchSceneLoading",unpack(arg, 1, table.maxn(arg)))
    self:enterScene(initSceneName,...)
end

function AppBase:enterScene(sceneName, ...)
    if self.isSwitch then
        local arg ={...}
        table.insert(arg,1,nil)
        self:removeDlgs()
        return self:showDlg(sceneName,unpack(arg, 1, table.maxn(arg)))
    end
    local arg ={...}
    table.insert(arg,1,nil)
    local view = self:createView(sceneName,unpack(arg, 1, table.maxn(arg)))
    view:showWithScene()
    self.currentSceneName = sceneName
    view:setName(sceneName)
    return view
end

function AppBase:showLoading()
    local scene = cc.Director:getInstance():getRunningScene()
    if scene then
        self.showLoadingRefCount = self.showLoadingRefCount + 1
        local loading = scene:getChildByName("gloab_loading")
        if loading then
            loading:show()
            return loading
        end
        local loading = self:createView("loading/LoadingScene")
        scene:addChild(loading,9999)
        loading:setName("gloab_loading")
        loading:show()
        return loading
    end
end

function AppBase:hideLoading()
    local scene = cc.Director:getInstance():getRunningScene()
    if scene then
        local loading = scene:getChildByName("gloab_loading")
        self.showLoadingRefCount = self.showLoadingRefCount - 1
        if loading then
            if self.showLoadingRefCount == 0 then
                loading:hide()
            end
            
        end
    end
end

function AppBase:showDlg(sceneName,...)
    local args = {...} or {}
    
    local scene = cc.Director:getInstance():getRunningScene()
    if scene then
--        local dialog = scene:getChildByName("gloab_dialog")
--        if dialog then
--        	dialog:removeSelf()
--        	dialog = nil
--        end
        local dialog = self:createView(sceneName,...)
        scene:addChild(dialog)
        dialog:setName(sceneName)
        if self.isSwitch then
            self.switchDlgs[sceneName] = sceneName
        end
        self.allDlgs[sceneName] = sceneName
        return dialog
    end
    
end

function AppBase:setSwitch(bl)
    self.isSwitch = bl
    if bl then
        self.switchDlgs = {}
        self:setDlgVisible(false)
    else
        self:setDlgVisible(true)
        self:removeDlgs()
        self.selectIslandId = 0
        self.selectAreaId = 0
        self.selectStageType =  0
        self.selectStageId = 0
        NotificationCenter_postNotification({msg = "REFRESH_PAGE"});
    end
    
end

function AppBase:removeDlg(sceneName)
    local scene = cc.Director:getInstance():getRunningScene()
    if scene then
        local dialog = scene:getChildByName(sceneName)
        if dialog then
            dialog:removeFromParent(true)
            dialog = nil
        end
    end
end

function AppBase:removeDlgs()
    for key,value in pairs(self.switchDlgs) do
        self:removeDlg(key)
    end
    self.switchDlgs = {}
end

function AppBase:setDlgVisible(bl)
    local scene = cc.Director:getInstance():getRunningScene()
    if scene then
        for key,value in pairs(self.allDlgs) do
            local dialog = scene:getChildByName(key)
            if dialog then
                dialog:setVisible(bl)
            end
        end
        local currentScene = scene:getChildByName(self.currentSceneName)
        if currentScene then
            currentScene:setVisible(bl)
        end
        
    end
    
end

function AppBase:createView(name,...)
    for _, root in ipairs(self.configs_.viewsRoot) do
        local packageName = string.format("%s.%s", root, name)
        local status, view = xpcall(function()
                return require(packageName)
            end, function(msg)
            if not string.find(msg, string.format("'%s' not found:", packageName)) then
                cclog("load view error: ", msg)
            end
        end)
        local t = type(view)
        if status and (t == "table" or t == "userdata") then
            return view:create(self, name,...)
        end
    end
    error(string.format("AppBase:createView() - not found view \"%s\" in search paths \"%s\"",
        name, table.concat(self.configs_.viewsRoot, ",")), 0)
end

function AppBase:onCreate()
end

return AppBase
