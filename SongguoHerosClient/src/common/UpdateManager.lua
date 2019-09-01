_REQUIRE "cocos.cocos2d.json"

UpdateManager = {}
UpdateManager.currentConfig = nil
UpdateManager.latestConfig = nil
UpdateManager.packageUpdating = nil
UpdateManager.onComplete = nil
UpdateManager.onError = nil
UpdateManager.onDownload = nil
UpdateManager.onUncompress = nil
UpdateManager.onProgress = nil

UpdateManager.ErrorCode = {
    LOAD_VERSION_FAIL = 0,
    NETWORK = 1,
    CREATE_FILE = 2,
    UNDOWNED = 3,
    UNCOMPRESS = 4,
}
UpdateManager.storagePath = cc.FileUtils:getInstance():getWritablePath() .. "sanguo/"

local checkVersionCallback = nil

function UpdateManager:init()
    local callbackMap = {
        onVersionLoaded = "UpdateManager_onVersionLoaded",
        onError = "UpdateManager_onError",
        onProgress = "UpdateManager_onProgress",
        onSuccess = "UpdateManager_onSuccess",
        onDownload = "UpdateManager_onDownload",
        onUncompress = "UpdateManager_onUncompress",
    }
    SanGuoUpdateLib.setLuaCallbacks(json.encode(callbackMap))
    
    -- 清空search path
    local searchPaths = cc.FileUtils:getInstance():getSearchPaths()
    cc.FileUtils:getInstance():setSearchPaths({})
    
    -- 包内版本号
    local versionConfig = cc.FileUtils:getInstance():getStringFromFile("version")
    release_print("internal version config: " .. versionConfig)
    local internalConfig = self:parseVersionConfig(versionConfig)
    self.currentConfig = internalConfig
    
    -- 更新目录下的版本号
    if cc.FileUtils:getInstance():isFileExist(self.storagePath .. "version") then
        versionConfig = cc.FileUtils:getInstance():getStringFromFile(self.storagePath .. "version")
        release_print("storage version config: " .. versionConfig)
        local storageConfig = self:parseVersionConfig(versionConfig)
        
        -- 更新目录下的版本号更大，则以更新目录为准
        if internalConfig.version < storageConfig.version then
            self.currentConfig = storageConfig
        -- 包内的版本号更大，则删除更新目录
        else
            cc.FileUtils:getInstance():removeDirectory(self.storagePath)
            self.currentConfig = internalConfig
        end
    end
    
    -- 更新目录不存在，则创建更新目录
    if not cc.FileUtils:getInstance():isDirectoryExist(self.storagePath) then
        cc.FileUtils:getInstance():createDirectory(self.storagePath)
    end
    
    -- 恢复search path
    cc.FileUtils:getInstance():setSearchPaths(searchPaths)
    local newPaths = cc.FileUtils:getInstance():getSearchPaths()
    for i = 1, #(newPaths) do
        release_print("latest search paths: " .. newPaths[i])
    end
end

function UpdateManager:setCallbacks(_onComplete, _onError, _onDownload, _onUncompress, _onProgress)
    self.onComplete = _onComplete
    self.onError = _onError
    self.onDownload = _onDownload
    self.onUncompress = _onUncompress
    self.onProgress = _onProgress
end

function UpdateManager:parseVersionConfig(versionConfig)
    local lines = string.split(versionConfig, "\n")
    local subVersions = string.split(lines[1], ".")
    local gitVer = lines[2]
    local urlPrefix = lines[3]
    
    local packages = {}
    for i = 4, #(lines) do
        if string.len(lines[i]) > 0 then
            local pkgData = string.split(lines[i], ",")

            local pkg = {
                version = tonumber(pkgData[1]),
                url = urlPrefix .. pkgData[2] .. ".zip",
                size = tonumber(pkgData[3]),
                md5 = pkgData[4],
                gitVer = pkgData[5],
            }
            table.insert(packages, pkg)
        end
    end

    local result = {
        fullVersion = lines[1],
        version = tonumber(subVersions[4]),
        gitVer = gitVer,
        url = urlPrefix,
        packages = packages,
    }

    return result
end

function UpdateManager:getPackageInfo(version)
    local packageInfo = nil
    if self.latestConfig then
        for i = 1, #(self.latestConfig.packages) do
            local pkg = self.latestConfig.packages[i]
            if pkg.version == version then
                packageInfo = pkg
                break
            end
        end
    end
    return packageInfo
end

function UpdateManager:loadVersion()
    local versionUrl = self.currentConfig.url .. "version" .. "?rnd=" .. os.time()
    SanGuoUpdateLib.loadVersion(versionUrl)
end

function UpdateManager:checkUpdate(versionConfig)
	self.latestConfig = self:parseVersionConfig(versionConfig)
	
	if self.currentConfig.version < self.latestConfig.version then
	    local downloads = {}
        local packages = self.latestConfig.packages
        for i = 1, #(packages) do
            local pkg = packages[i]
            if pkg.version > self.currentConfig.version then
                table.insert(downloads, pkg)
            end
        end
        local downloadsJson = json.encode(downloads)
        release_print("UpdateManager:checkUpdate, start download packages: " .. downloadsJson)
        
        SanGuoUpdateLib.loadPackages(downloadsJson, self.storagePath)
	else
	    release_print("UpdateManager:checkUpdate, already latest, no need to update")
	   
	    if self.onComplete then
	        self.onComplete()
	    end
	end
end

function UpdateManager_onVersionLoaded(versionConfig)
    UpdateManager:checkUpdate(versionConfig)
end

function UpdateManager_onError(errorCode)
    if UpdateManager.onError then
        UpdateManager.onError(errorCode)
    end
end

function UpdateManager_onProgress(percent)
    if UpdateManager.packageUpdating then
        if UpdateManager.onProgress then
            UpdateManager.onProgress(UpdateManager.packageUpdating, percent)
        end
    end
end

function UpdateManager_onSuccess()
    if cc.FileUtils:getInstance():isFileExist(UpdateManager.storagePath .. "version") then
        local versionConfig = cc.FileUtils:getInstance():getStringFromFile(UpdateManager.storagePath .. "version")
        release_print("update current config: " .. versionConfig)
        UpdateManager.currentConfig = UpdateManager:parseVersionConfig(versionConfig)
    end

    if UpdateManager.onComplete then
        UpdateManager.onComplete()
    end
end

function UpdateManager_onDownload(version)
	local package = UpdateManager:getPackageInfo(version)
	if package then
	   UpdateManager.packageUpdating = package
	   
	   if UpdateManager.onDownload then
	       UpdateManager.onDownload(package)
	   end
	else
	   UpdateManager.packageUpdating = nil
	end
end

function UpdateManager_onUncompress(version)
    local package = UpdateManager:getPackageInfo(version)
    if package then
        UpdateManager.packageUpdating = package
        
        if UpdateManager.onUncompress then
            UpdateManager.onUncompress(package)
        end
    else
        UpdateManager.packageUpdating = nil
    end
end

UpdateManager:init()