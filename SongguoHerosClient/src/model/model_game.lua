local model = {};

model.autoQuickBattle = false;

function model:getAutoQuickBattle()
    return self.autoQuickBattle;
end

function model:setAutoQuickBattle(autoQuickBattle)
    self.autoQuickBattle = autoQuickBattle;
end

return model;