
function playUiEffect(x,y,nScale,szEffectName)
	local modelView = getglobal("UiEffectFrameModelView");
	if modelView ~= nil then
		if nScale == nil or nScale == 0 then
			nScale = 1;
		end
		modelView:setPetModel(0,0,szEffectName);
		modelView:setPetScale(0,0.2*nScale,0.2*nScale,0.2*nScale);
		modelView:setEntityPosition(0,0,100,0);
	end
end
