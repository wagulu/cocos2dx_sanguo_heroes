_REQUIRE("common/UpdateManager.lua")

local UpdateScene = class("UpdateScene", cc.load("mvc").ViewBase)
UpdateScene.RESOURCE_FILENAME = "Scene/LoginScene.csb"
UpdateScene.LoadingBar = nil
UpdateScene.Text_Progress = nil

function UpdateScene:onCreate()
    local panel_login = self:getResourceNode():getChildByName("panel_login")
    local Panel_Center = panel_login:getChildByName("Panel_Center")    
    local Panel_Function = Panel_Center:getChildByName("Panel_Function") 
    local Panel_Bottom = panel_login:getChildByName("Panel_Bottom") 
    local Text_loading = Panel_Center:getChildByName("Text_loading") 
    local Image_1 = Panel_Center:getChildByName("Image_1")
    local Text_Progress = Panel_Bottom:getChildByName("Text_Progress")
    
    self.LoadingBar = Panel_Bottom:getChildByName("LoadingBar")
    self.Text_Progress = Panel_Bottom:getChildByName("Text_Progress")

    Panel_Function:hide()
    Image_1:hide()
    Panel_Bottom:show()
    Text_loading:hide()
    Panel_Bottom:getChildByName("Text_Tip"):hide()

    local Panel_Bg = panel_login:getChildByName("Panel_Bg")
    SCREEN_SCALE_BG(Panel_Bg)
    
    self:showGameVersion()
    
    self:checkUpdate()
end

function UpdateScene:showGameVersion()
    local versionConfig = UpdateManager.currentConfig
    if versionConfig then
        local panel_login = self:getResourceNode():getChildByName("panel_login")
        local textField = cc.Label:createWithSystemFont("版本号：" .. versionConfig.fullVersion, FONT, 20)
        textField:setAnchorPoint(cc.p(1, 0))
        textField:setPositionX(CC_DESIGN_RESOLUTION.width+(CC_SCREEN_SIZE.width-CC_DESIGN_RESOLUTION.width)/2)
        panel_login:addChild(textField)
    end
end

function UpdateScene:checkUpdate()
    self.LoadingBar:setPercent(0)
    self.Text_Progress:setString("正在检查更新...")
    
    local function onComplete()
        SWITSCENE("splash/SplashScene")
    end
    
    local function onError(errorCode)
        if errorCode == UpdateManager.ErrorCode.LOAD_VERSION_FAIL then
            self.Text_Progress:setString("检查更新失败...")
        elseif errorCode == UpdateManager.ErrorCode.CREATE_FILE then
            self.Text_Progress:setString("保存更新包失败...")
        elseif errorCode == UpdateManager.ErrorCode.NETWORK then
            self.Text_Progress:setString("网络异常...")
        elseif errorCode == UpdateManager.ErrorCode.UNCOMPRESS then
            self.Text_Progress:setString("解压更新包失败...")
        elseif errorCode == UpdateManager.ErrorCode.UNDOWNED then
            self.Text_Progress:setString("下载更新包失败...")
        else
            self.Text_Progress:setString("未知错误...")
        end
    end
    
    local function onDownload(package)
        self.Text_Progress:setString("正在下载更新包" .. package.version)
    end
    
    local function onUncompress(package)
        self.Text_Progress:setString("正在解压更新包" .. package.version)
    end
    
    local function parseSizeAsMB(size)
        size = size / 1024
        local integer = math.floor(size)
        local decimal = math.floor((size - integer) * 100)
        return integer .. "." .. decimal
    end
    
    local function onProgress(package, percent)
        local total = math.ceil(package.size / 1024)
        local current = math.ceil(percent * total / 100)
        
        if total >= 1024 then
            total = parseSizeAsMB(total)
            current = parseSizeAsMB(current)
            self.Text_Progress:setString(current .. "MB/" .. total .. "MB")
        else
            self.Text_Progress:setString(current .. "KB/" .. total .. "KB")
        end
    end
    
    UpdateManager:setCallbacks(
        onComplete, 
        onError,
        onDownload,
        onUncompress,
        onProgress
    )
    UpdateManager:loadVersion()
end

return UpdateScene