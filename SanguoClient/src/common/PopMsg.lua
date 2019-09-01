--[[

消息框

]]

local KNBtn = {}

PopMsg = {}

function PopMsg:new()
    local store = nil

    return function(self)
        if store then return store end

        local o = {}
        setmetatable(o , self)
        --		self._index=self

        store = o
        store.text = ""
        store.isAction = false    --是否有动画在执行
        local mask = nil

        --检查及初始化参数
        local function checkData(str , param)
            store.text = str or ""
            -- 如果提示文字 为空  则不执行本次提示
            if(store.text == "") then
                return 0 , param
            end
            -- 动画是否在执行中，如果执行中则不执行新动画 (该判断会导致在提示动画未执行完切换场景后，提示不出现，所以暂时注释掉)
            --			if(store.isAction) then
            --				return 0 , param
            --			end
            -- 初始化动画执行时间
            local time = 1.5
            if args ~= nil then
                time = args.time or 1.5
            end

            return time , param
        end

        -- 创建 对应UI布局
        local function createLayout(type , args)
            args = args or {}
            local content = nil
            local textField = cc.Label:createWithSystemFont(store.text , FONT , 20)
            if type == 0 then
                content = display.newSprite("ui/prompt_bg.png")
                content:setAnchorPoint( cc.p(0 , 0) )
                textField:setColor(cc.c3b( 0xff , 0xff , 0xff ))
                textField:setAnchorPoint( cc.p( 0 , 1 ) )

                if textField:getContentSize().width > 360 then
                    textField:setDimensions(360,70)

                    textField:setPosition( cc.p( ( content:getContentSize().width - textField:getContentSize().width ) / 2 , content:getContentSize().height / 1.5 + 3 ) )
                else
                    textField:setPosition( cc.p( ( content:getContentSize().width - textField:getContentSize().width ) / 2 , content:getContentSize().height / 1.5 - 8 ) )
                end

                content:addChild(textField)

            else  --带按钮的提示框
              
            end

            local tempX = ( display.width - content:getContentSize().width ) / 2
            local tempY = ( display.height + content:getContentSize().height / 2 ) / 2 - content:getContentSize().height + 100
            content:setPosition( cc.p( tempX , tempY ) )
            return content , textField
        end


        -- 执行动画效果
        local function createAction(target , time , type , args)
            target:setVisible( true )
            local scene = display.getRunningScene()
            if not args then args = {} end
            local showTime = time  -- 动画效果总时间
            local startEfTime = showTime / 5  -- 显示前渐变动画效果时间
            local endEfTime = showTime / 5  -- 消失时渐变动画时间
            local move_y = args.y or 100

            store.isAction = true
            if type == 0 then  -- 无遮罩，展示后消失
                scene:addChild(target)

                startEfTime = 0.2
                endEfTime = 0.2

                -- 渐变显示
                transition.fadeIn(target , {
                    time = startEfTime,
                })

                transition.moveBy(target , {
                    y = 20,
                    time = startEfTime,
                    onComplete = function()
                        transition.moveBy(target , {
                            delay = showTime,
                            y = 20,
                            time = endEfTime,
                            onComplete = function()
                                target:setVisible(false)
                                target:removeSelf()	-- 清除自己
                                store.isAction = false
                            end
                        })
                    end
                })

                -- 消失动画
                transition.fadeOut(target , {
                    time = endEfTime,
                    delay = showTime + startEfTime,
                })
            end
        end
        -- 闪现
        function store:flashShow(str , args)

            local time , args = checkData(str , args)

            if time == 0 then return end

            -- 生成显示对像
            local tempContent , textField = createLayout(0)
            -- 设置文字

            textField:setString(store.text)
            -- 判断是否是两行文字
            if string.find(store.text , "\n") then
                textField:setPositionY( textField:getPositionY() + 15 )
            end
            -- 执行动画
            createAction(tempContent , time , 0)
        end
        return o
    end
end

PopMsg.getInstance = PopMsg:new()

