
NotificationCenter_observers = {}

function NotificationCenter_addObserver(target,msg,backFunc)
    local msgObj = {target = target,func = backFunc}
    if NotificationCenter_observers[msg] == nil then
        NotificationCenter_observers[msg] = {}
    end
    if NotificationCenter_isExist(target,msg,backFunc) then
        return
    end
    table.insert(NotificationCenter_observers[msg], msgObj)
end

function NotificationCenter_postNotification(data)
    local msgs = NotificationCenter_observers[data.msg]
    if msgs == nil then
        return
    end
    for k,l in pairs(msgs) do
        local func = l["func"]
        local target = l["target"]
        func(target,data)
    end
end

function NotificationCenter_isExist(target,msg,func)
    local msgs = NotificationCenter_observers[msg]
    for k,v in pairs(msgs) do
        if v["func"] == func and v["target"] == target then
            return true
        end
    end
    return false
end
--[[
* [removeObserverByName 删除某个消息的所有事件]
* @param  {[type]} msgNa
* @return {[type]}
]]
function NotificationCenter_removeObserverByName(msgNa)
    NotificationCenter_observers[msgNa] = nil
end
--[[
* [removeObserverByTarget 删除某个对象的所有事件]
* @param  {[type]} target
* @return {[type]}
]]
function NotificationCenter_removeObserverByTarget(target)
     for i,j in pairs(NotificationCenter_observers) do
            for k,l in pairs(j) do
                local tempTarget = l["target"]
                if tempTarget == target then
                    table.remove(j, k)
                end
            end
     end
end
--[[
* [removeObserverFromTargetByName 删除某个对象的某个事件]
* @param  {[type]} target
* @param  {[type]} msg
* @return {[type]}
]]
function NotificationCenter_removeObserverFromTargetByName(target,msg)
    local msgs = NotificationCenter_observers[msg]
    if(msgs ~= nil) then
        for k,v in pairs(msgs) do
            if v["target"] == target then
                table.remove(msgs, k)
            end
        end
    end
end

function NotificationCenter_removeAllMessage( ... )
    NotificationCenter_observers = {}
end
