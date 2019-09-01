

local object = {};

function object:getDateFromTimestamp(time)
    local d = os.date("*t",time);
    return d;
end

function object:getDataStringFromeTimestamp(time)
    local date = self:getDateFromTimestamp(time)
    local month,day,hour,min,sec
    month = date.month
    day = date.day
    hour = date.hour
    min = date.min
    sec = date.sec
    if date.month < 10 then
        month = "0" + date.month
    end 
    if date.day < 10 then
        day = "0" + date.day
    end 
    if date.hour < 10 then
        hour = "0" + date.hour
    end 
    if date.min < 10 then
        min = "0" + date.min
    end 
    if date.sec < 10 then
        sec = "0" + date.sec
    end
    return date.year.."-"..month.."-"..day.." "..hour..":"..min..":"..sec
end

function object:getTimeString(time)
    local day =  math.floor(time / 60 / 60 / 24);
    time = time - day * 24 * 60 * 60;
    local hour =  math.floor(time / 60 / 60);
    time = time -hour * 60 * 60;
    local minute =  math.floor(time / 60);
    local totalHour = (day*24 + hour)
    if totalHour >0 then
        return LANG("%s小时前",totalHour)
    else
        return LANG("%s分钟前",math.max(1,minute))
    end
    
end

function object:isSameDay(time1, time2)
    local d1 = self:getDateFromTimestamp(time1);
    local d2 = self:getDateFromTimestamp(time2);
    return d1.year == d2.year and d1.month == d2.month and d1.day == d2.day;
end

function object:getLocalTimestamp()
    return os.time();
end

function object:convertSecondToDayHourMinuteSecond(time)
    local day =  math.floor(time / 60 / 60 / 24);
    time = time - day * 24 * 60 * 60;
    local hour =  math.floor(time / 60 / 60);
    time = time -hour * 60 * 60;
    local minute =  math.floor(time / 60);
    time = time - minute * 60;
    local second = math.floor(time);
    if day < 10 then
        day = "0" .. day
    end
    if hour < 10 then
        hour = "0" .. hour
    end
    if minute < 10 then
        minute = "0" .. minute
    end
    if second < 10 then
        second = "0" .. second
    end
    return day, hour, minute, second;
end

return object;
