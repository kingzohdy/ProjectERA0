local t_FadeTaskName =
	{
		{ 2009, "uires\\ui\\ZhangJieMingXianShi\\DiSanZhangFaLaoDeNingShi.tga", 606, 112},
		{ 1098, "uires\\ui\\ZhangJieMingXianShi\\DiErZhangXueYuanDeZuiE.tga" , 606, 112},
		{ 1031, "uires\\ui\\ZhangJieMingXianShi\\DiYiZhangHunLuanDeCunZhuang.tga", 606, 112 },
		{ 1049, "uires\\ui\\ZhangJieMingXianShi\\XuTianKongDeJiaYuan.tga", 515, 126 },
	};

function SetFadeTaskNameShow( nTaskId )
	for i = 1, table.getn( t_FadeTaskName ) do
		if t_FadeTaskName[i][1] == nTaskId then
			local FadeTexture = getglobal("FadeTaskNameFrame_LongSceneNameTex");
			FadeTexture:SetSize( t_FadeTaskName[i][3], t_FadeTaskName[i][4] );
			FadeTexture:SetTexture( t_FadeTaskName[i][2] );

			if FadeSceneNameFrame:IsShown() then
				FadeSceneNameFrame:Hide();
			end

			FadeTaskNameFrame:Show();
			break;
		end
	end
end