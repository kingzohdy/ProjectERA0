function pack(...)
	return {...}
end

function enumsubstr(str, patten, start)
	start = start or 1
	return function()
		local t = pack(string.find(str, patten, start))
		if #t == 0 then
			return
		end
		start = t[2] + 1
		if #t == 2 then
			t[3] = string.sub(str, t[1], t[2])
		end
		return unpack(t, 3)
	end
end

util = 
	{
	["container"] = {},
	};

function util.GetMaxFontWidth( t_fonts )
	local t_fontsWidth = {};
	for _, szFontName in ipairs( t_fonts ) do
		table.insert( t_fontsWidth, GetFontTextWidth(  getglobal( szFontName ) ) );
	end

	return math.max( unpack( t_fontsWidth ) );
end

function CheckMaxPriceNum( edit, maxPrice )
	local price =  tonumber( edit:GetText() ); 
	if price == nil then
		return;
	end

	if price > maxPrice then
		edit:SetText( maxPrice );
	end
end

-- ����RichText��ǰ��ʾ��λ��
function util.UpdateRich( szRichName, nStartLine )
	local rich = getglobal( szRichName );
	rich:SetDispPos( rich:GetStartDispPos() );

	for i = 1, nStartLine do
		rich:ScrollDown();
	end
end

--����TIPS��ʾ��λ��
function util.AdjustTooltipsScreenPos( data )
	local szTipsName		= data["tips"];
	local Frame				= getglobal( szTipsName );
	local nFrameWidth		= Frame:GetWidth();
	local nFrameHeight		= Frame:GetHeight();
	local nCursorX			= GetCursorPosX();
	local nCursorY			= GetCursorPosY();
	local nScreenWidth		= GetScreenWidth();
	local nScreenHeight		= GetScreenHeight();
	local horizonalPriority	= data["horizonal"] or "left";
	-- top bottom
	local verticalPriority	= data["vertical"] or "top";
	local yAdjust			= data["yAdjust"] or 0; 

	-- ������ʾ��ʱ��������ʾ���
	-- ������ʾ��ʱ��������ʾ����

	-- ��X,Y��ά������ƫ��
	local nXOffset = 0;
	-- ����굱ǰ���ڵ�λ�ÿ������X����Ŀ�Ⱥ��ұ�ʣ��Ŀ�Ⱦ���������tips����������
	if ( nCursorX - nFrameWidth >= 0 ) and ( nCursorX + nFrameWidth <= nScreenWidth ) then
		if horizonalPriority == "left" then
			nXOffset = nCursorX - nFrameWidth;
		elseif horizonalPriority == "right" then
			nXOffset = nCursorX;
		end
		
	elseif nCursorX - nFrameWidth < 0 then	-- �����������
		nXOffset = 5;
	elseif	( nCursorX + nFrameWidth > nScreenWidth ) then	-- �����Ҽ������
		nXOffset = nCursorX - math.abs( nScreenWidth - nCursorX - nFrameWidth ) - 5;
	end

	local nYOffset = 0;
	-- �����������
	if ( nCursorY - nFrameHeight ) >= 0 and ( nCursorY + nFrameHeight ) <= nScreenHeight then
		if verticalPriority == "top" then
			nYOffset = nCursorY;
		elseif verticalPriority == "bottom" then
			nYOffset = nCursorY + nFrameHeight;
		end
		
	elseif ( nCursorY - nFrameHeight ) < 0 then	-- �����ϲ��������
		nYOffset = nFrameHeight;
		-- ����굱ǰ������λ�õ��Ϸ��߶ȱ�Ҫ��ʾ��tips�ĸ߶�С
	elseif ( nCursorY + nFrameHeight ) > nScreenHeight then	-- �����²��������
		nYOffset = nCursorY;
	end

	nYOffset = nYOffset + yAdjust;

	Frame:SetPoint( "BottomLeft", "PhotoFrame", "topleft", nXOffset, nYOffset );
end

--����TIPS��ʾ��λ��
function util.AdjustBigtipsScreenPos( data )
	local szTipsName		= data["tipsFrame"];
	local szRelFrameName	= data["button"];
	local Frame				= getglobal( szTipsName );
	local relFrame			= getglobal( szRelFrameName );
	local nFrameWidth		= Frame:GetWidth() / GetScreenScaleY();
	local nFrameHeight		= Frame:GetHeight() / GetScreenScaleY();
	local nCursorX			= GetCursorPosX();
	local nCursorY			= GetCursorPosY();
	local nScreenWidth		= GetScreenWidth();
	local nScreenHeight		= GetScreenHeight();
	local horizonalPriority	= data["horizonal"] or "left";
	local verticalPriority	= data["vertical"]	or "top";

	if Frame == nil or relFrame == nil then
		return;
	end
	
	-- ������ʾ��ʱ��������ʾ���
	-- ������ʾ��ʱ��������ʾ����

	-- ��X,Y��ά������ƫ��
	local nXOffset		= 0;
	local xRelPoint		= "Left";
	local xSelfPoint	= "Right";
	-- ����굱ǰ���ڵ�λ�ÿ������X����Ŀ�Ⱥ��ұ�ʣ��Ŀ�Ⱦ���������tips����������
	-- �����������ʾtips
	if horizonalPriority == "left" and ( relFrame:GetRealLeft() - nFrameWidth >= 0 ) then
		nXOffset	= 0;
		xRelPoint	= "Left";
		xSelfPoint	= "Right";
	-- ���ұ�������ʾtips
	elseif horizonalPriority == "right" and ( relFrame:GetRealRight() + nFrameWidth <= nScreenWidth ) then
		nXOffset	= 0;
		xRelPoint	= "Right";
		xSelfPoint	= "Left";
	elseif ( relFrame:GetRealLeft() - nFrameWidth >= 0 ) then
		nXOffset	= 0;
		xRelPoint	= "Left";
		xSelfPoint	= "Right";
	elseif ( relFrame:GetRealRight() + nFrameWidth <= nScreenWidth ) then
		nXOffset	= 0;
		xRelPoint	= "Right";
		xSelfPoint	= "Left";
	else
		nXOffset	= 0;
		xRelPoint	= "center";
		xSelfPoint	= "center";
	end

	local nYOffset		= 0;
	local yRelPoint		= "Top";
	local ySelfPoint	= "Bottom";
	-- �����������
	if verticalPriority == "top" and ( relFrame:GetRealTop() - nFrameHeight >= 0 ) then
		nYOffset	= 0;
		yRelPoint	= "Top";
		ySelfPoint	= "Bottom";
	elseif verticalPriority == "bottom" and ( relFrame:GetRealBottom() + nFrameHeight ) <= nScreenHeight then
		nYOffset	= 0;
		yRelPoint	= "Bottom";
		ySelfPoint	= "Top";
	elseif ( relFrame:GetRealTop() - nFrameHeight ) >= 0 then
		nYOffset	= 0;
		yRelPoint	= "Top";
		ySelfPoint	= "Bottom";
	elseif ( relFrame:GetRealBottom() + nFrameHeight ) <= nScreenHeight then
		nYOffset	= 0;
		yRelPoint	= "Bottom";
		ySelfPoint	= "Top";
		-- ����굱ǰ������λ�õ��Ϸ��߶ȱ�Ҫ��ʾ��tips�ĸ߶�С
	else
		nYOffset	= 0;
		yRelPoint	= "center";
		ySelfPoint	= "center";
	end
	
	local selfPoint = "center";
	local relPoint	= "center";
	if xSelfPoint == "center" or ySelfPoint == "center" then
		selfPoint = "center";
	else
		selfPoint = ySelfPoint..xSelfPoint;
	end

	if xRelPoint == "center" or yRelPoint == "center" then
		relPoint = "center";
	else
		relPoint = yRelPoint..xRelPoint;
	end

	Frame:SetPoint( selfPoint, szRelFrameName, relPoint, nXOffset, nYOffset );
end

function util.isXConflict( data )
	local nX	= data["x"];
	local frame = getglobal( data["frame"] );
	return frame:GetLeft() <= nX and nX <= frame:GetRight();
end

function util.isYConflict( data )
	local nY	= data["y"];
	local frame = getglobal( data["frame"] );
	return frame:GetTop() <= nY and nY <= frame:GetBottom();
end

function util.AdjustTooltipsByCursorWithConflictTest( data )
	local szTipsName		= data["tips"];
	local Frame				= getglobal( szTipsName );
	local nFrameWidth		= Frame:GetWidth() * GetScreenScaleY();
	local nFrameHeight		= Frame:GetHeight() * GetScreenScaleY();
	local nCursorX			= GetCursorPosX();
	local nCursorY			= GetCursorPosY();
	local nScreenWidth		= GetScreenWidth();
	local nScreenHeight		= GetScreenHeight();
	local horizonalPriority	= data["horizonal"] or "left";
	-- top bottom
	local verticalPriority	= data["vertical"] or "top";
	local yAdjust			= data["yAdjust"] or 0; 
	local xAdjust			= data["xAdjust"] or 0; 
	local conflictFrame		= getglobal( data["conflictFrame"] );
	assert( conflictFrame ~= nil, "���ܴ�����ײ���" );
	local verticalReferencePoint	= data["verticalReferencePoint"];
	local horizonalReferencePoint	= data["horizonalReferencePoint"];

	-- ������ʾ��ʱ��������ʾ���
	-- ������ʾ��ʱ��������ʾ����

	-- ��X,Y��ά������ƫ��
	local nXOffset = nCursorX;
	-- ����굱ǰ���ڵ�λ�ÿ������X����Ŀ�Ⱥ��ұ�ʣ��Ŀ�Ⱦ���������tips����������
	if horizonalReferencePoint == "horizonal" then
		if ( nScreenWidth - conflictFrame:GetRight() ) >= nFrameWidth and 
			( nScreenWidth - conflictFrame:GetLeft() ) >= nFrameWidth then
			if horizonalPriority == "left" then
				nXOffset = conflictFrame:GetLeft() - nFrameWidth;
			elseif horizonalPriority == "right" then
				nXOffset = conflictFrame:GetRight();
			end			
		elseif ( nScreenWidth - conflictFrame:GetRight() ) < nFrameWidth then	-- �����Ҽ������
			nXOffset = conflictFrame:GetLeft() - nFrameWidth;
		elseif	( nScreenWidth - conflictFrame:GetLeft() ) < nFrameWidth then	-- �����������
			nXOffset = conflictFrame:GetRight();
		end

	else
		if ( nCursorX - nFrameWidth >= 0 ) and ( nCursorX + nFrameWidth <= nScreenWidth ) then
			if horizonalPriority == "left" then
				nXOffset = nCursorX - nFrameWidth;
				print( "AdjustTooltipsByCursorWithConflictTest--260" );
			elseif horizonalPriority == "right" then
				nXOffset = nCursorX - nFrameWidth;
				print( "AdjustTooltipsByCursorWithConflictTest--263" );
			end
			
		elseif nCursorX - nFrameWidth < 0 then	-- �����������
			nXOffset = nCursorX;
			print( "AdjustTooltipsByCursorWithConflictTest--268" );
		elseif	( nCursorX + nFrameWidth > nScreenWidth ) then	-- �����Ҽ������
			nXOffset = nCursorX - nFrameWidth;
			print( "AdjustTooltipsByCursorWithConflictTest--271" );
		end
	end
	

	local nYOffset = nCursorY / GetScreenScaleY();
	if verticalReferencePoint == "vertical" then
		-- �����������
		if conflictFrame:GetTop() >= nFrameHeight and 
			( nScreenHeight - conflictFrame:GetBottom() ) >= nFrameHeight then
			if verticalPriority == "top" then
				nYOffset = conflictFrame:GetTop();
			elseif verticalPriority == "bottom" then
				nYOffset = conflictFrame:GetBottom();
			end
			
		elseif  conflictFrame:GetTop() < nFrameHeight then	-- �����ϲ��������
			nYOffset = conflictFrame:GetBottom();
			-- ����굱ǰ������λ�õ��Ϸ��߶ȱ�Ҫ��ʾ��tips�ĸ߶�С
		elseif ( nScreenHeight - conflictFrame:GetBottom() ) < nFrameHeight then	-- �����²��������
			nYOffset = conflictFrame:GetTop();
		end

	else
		-- �����������
		if ( nCursorY - nFrameHeight ) >= 0 and ( nCursorY + nFrameHeight ) <= nScreenHeight then
			if verticalPriority == "top" then
				nYOffset = nCursorY;
			elseif verticalPriority == "bottom" then
				nYOffset = nCursorY + nFrameHeight;
			end
			
		elseif ( nCursorY - nFrameHeight ) < 0 then	-- �����ϲ��������
			nYOffset = 0;
			-- ����굱ǰ������λ�õ��Ϸ��߶ȱ�Ҫ��ʾ��tips�ĸ߶�С
		elseif ( nCursorY + nFrameHeight ) > nScreenHeight then	-- �����²��������
			nYOffset = nCursorY - nFrameHeight;
		end
	end
	
	nXOffset = nXOffset + xAdjust;
	nYOffset = nYOffset + yAdjust;

	Frame:SetPoint( "topleft", "UIClient", "topleft", nXOffset / GetScreenScaleY(), nYOffset / GetScreenScaleY() );
end

function util.AdjustBigtipsScreenPosEx( data )
	local szTipsName		= data["tipsFrame"];
	local szRelFrameName	= data["button"];
	local Frame				= getglobal( szTipsName );
	local relFrame			= getglobal( szRelFrameName );
	local nFrameWidth		= Frame:GetWidth() / GetScreenScaleY();
	local nFrameHeight		= Frame:GetHeight() / GetScreenScaleY();
	local nCursorX			= GetCursorPosX();
	local nCursorY			= GetCursorPosY();
	local nScreenWidth		= GetScreenWidth();
	local nScreenHeight		= GetScreenHeight();
	local horizonalPriority	= data["horizonal"] or "left";

	if Frame == nil or relFrame == nil then
		return;
	end
	
	-- ������ʾ��ʱ��������ʾ���
	-- ������ʾ��ʱ��������ʾ����

	-- ��X,Y��ά������ƫ��
	local nXOffset		= 0;
	local xRelPoint		= "Left";
	local xSelfPoint	= "Right";
	-- ����굱ǰ���ڵ�λ�ÿ������X����Ŀ�Ⱥ��ұ�ʣ��Ŀ�Ⱦ���������tips����������
	-- �����������ʾtips
	if horizonalPriority == "left" and ( relFrame:GetRealLeft() - nFrameWidth >= 0 ) then
		nXOffset	= 0;
		xRelPoint	= "Left";
		xSelfPoint	= "Right";
	-- ���ұ�������ʾtips
	elseif horizonalPriority == "right" and ( relFrame:GetRealRight() + nFrameWidth <= nScreenWidth ) then
		nXOffset	= 0;
		xRelPoint	= "Right";
		xSelfPoint	= "Left";
	elseif ( relFrame:GetRealLeft() - nFrameWidth >= 0 ) then
		nXOffset	= 0;
		xRelPoint	= "Left";
		xSelfPoint	= "Right";
	elseif ( relFrame:GetRealRight() + nFrameWidth <= nScreenWidth ) then
		nXOffset	= 0;
		xRelPoint	= "Right";
		xSelfPoint	= "Left";
	else
		nXOffset	= 0;
		xRelPoint	= "center";
		xSelfPoint	= "center";
	end

	local nYOffset		= 0;
	local yRelPoint		= "Top";
	local ySelfPoint	= "Bottom";
	-- �����������
	if ( relFrame:GetRealTop() - nFrameHeight ) >= 0 then
		nYOffset	= 0;
		yRelPoint	= "Top";
		ySelfPoint	= "Bottom";
	elseif ( relFrame:GetRealBottom() + nFrameHeight ) <= nScreenHeight then
		nYOffset	= 0;
		yRelPoint	= "Bottom";
		ySelfPoint	= "Top";
		-- ����굱ǰ������λ�õ��Ϸ��߶ȱ�Ҫ��ʾ��tips�ĸ߶�С
	else
		nYOffset	= 0;
		yRelPoint	= "center";
		ySelfPoint	= "center";
	end
	
	local selfPoint = "center";
	local relPoint	= "center";
	if xSelfPoint == "center" or ySelfPoint == "center" then
		selfPoint = "center";
	else
		selfPoint = ySelfPoint..xSelfPoint;
	end

	if xRelPoint == "center" or yRelPoint == "center" then
		relPoint = "center";
	else
		relPoint = yRelPoint..xRelPoint;
	end

	Frame:SetPointEx( selfPoint, szRelFrameName, relPoint, nXOffset, nYOffset );
end

-- ����2��ʵ��֮��ľ���
-- ���صĳ�����mΪ��λ,�ҽ���ǳ��ȵ�ƽ�������ǳ������ܿ��ǣ����ⲻ������ƽ����
function util.CalActorsDistance( lhsActor, rhsActor )
	local dist	= ( lhsActor:getPlayerX() / 10 - rhsActor:getPlayerX() / 10 ) ^ 2 + ( lhsActor:getPlayerZ() / 10 - rhsActor:getPlayerZ() / 10 ) ^ 2;
	return dist / ( 100 ^ 2 );
end

function util.testNpcDialogDistance(actorId, ...)
	local t = {...}
	timer(function()
		while true do
			local hide = true
			for _, v in ipairs(t) do
				if v and v:IsShown() then
					hide = false
				end
			end
			if hide then
				return
			end
			local npc = ActorMgr:FindActor(actorId)
			if not npc then
				break
			end
			if npc:getResId() ~= NpcDialog:getLastNpcResId() then
				break
			end
			if util.CalActorsDistance(npc, ActorMgr:getMainPlayer()) > (NpcDialog:getMaxNpcTalkDist( npc ) / 100) ^ 2 then
				break
			end
			coroutine.yield(0.5)
		end
		for _, v in ipairs(t) do
			if v and v:IsShown() then
				v:Hide()
			end
		end
	end)
end


function util.CalPointsDistance( lhsPoint, rhsPoint )
	return ( lhsPoint["x"] - rhsPoint["x"] ) ^ 2 + ( lhsPoint["y"] - rhsPoint["y"] ) ^ 2;
end

-- ���㶯̬Բ�������
--[[
function util.ConvertToCirclePoint( data )
	local circlePoint = {};
	local xScale	= math.sin( math.rad( data["angle"] ) );
	local yScale	= math.cos( math.rad( data["angle"] ) );
	local xRadius	= 0;
	local yRadius	= 0;
	if xScale > 0 then
		xRadius = math.min( data["maxX"] - data["x"], xScale * data["oneMovePathLength"] );
	else
		xRadius = math.min( data["x"], xScale * data["oneMovePathLength"] );
	end
	
	if yScale > 0 then
		yRadius = math.min( data["maxY"] - data["y"], yScale * data["oneMovePathLength"] );
	else
		yRadius = math.min( data["y"], yScale * data["oneMovePathLength"] );
	end
	

	circlePoint["x"]	= data["x"] + xScale * xRadius; 
	circlePoint["y"]	= data["y"] + yScale * yRadius; 

	print( "circlePoint[x] = "..circlePoint["x"].."��circlePoint[y] = "..circlePoint["y"].."��xRadius = "..xRadius.."��yRadius = "..yRadius..
			"�� angle = "..data["angle"] );

	return circlePoint;
end
--]]
function util.ConvertToCirclePoint( data )
	local circlePoint = {};
	
	circlePoint["x"]	= data["x"] + math.sin( math.rad( data["angle"] ) ) * data["radius"]; 
	circlePoint["y"]	= data["y"] + math.cos( math.rad( data["angle"] ) ) * data["radius"]; 

	return circlePoint;
end

-- ������Ч
function util.PlayUIEffect( data )
	local modelView = getglobal( data["modelView"] );
	modelView:setPetModel( 0, 0, data["effect"] );
	modelView:setPetScale( 0, data["scale"], data["scale"], data["scale"] );
	modelView:setEntityPosition( 0, 0, 100, 0 );
end

local szExchangMoneyIconPath = "uires\\TuPianLei\\WuPin\\DuiHuanQuan\\";
function util.GetExchangMoneyIconPath( data )
	local iconID	= data["iconID"];
	local IconPath	= szExchangMoneyIconPath..iconID..".tga";
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end

	return IconPath;
end

-- ����ModelView
function util.SetModelView( t_modelViews )
	for name, data in pairs( t_modelViews ) do
		local modelViewer = getglobal( name );
		modelViewer:setPetModel( 0, data.animId, data.path );
		modelViewer:setPetScale( 0, data.scale, data.scale, data.scale );
		modelViewer:setEntityPosition( 0, 0, 40, 0 );
		modelViewer:Show();
	end
end

-- ��ʾ���е����пؼ�
function util.ShowIcons( t_showIcons )
	for _, name in ipairs( t_showIcons ) do
		local icon = getglobal( name );
		if not icon:IsShown() then
			icon:Show();
		end
	end
end

-- ���ر��е����пؼ�
function util.HideIcons( t_hideIcons )
	for _, name in ipairs( t_hideIcons ) do
		local icon = getglobal( name );

		if icon:IsShown() then
			icon:Hide();
		end
	end
end

-- ���ñ��е�����tga
function util.SetTgas( t_tgas )
	for name, data in pairs( t_tgas ) do
		local tex = getglobal( name );
		tex:SetTexture( data["path"] );
		tex:SetTexRelUV( 1, 1 );
	end
end

function util.SetMasks( t_masks )
	for name, data in pairs( t_masks ) do
		local tex = getglobal( name );
		tex:setMask( data["path"] );
	end
end

-- 
function util.ExtendFrameByRich( data )
	local frame = getglobal( data["frame"] );
	local rich	= getglobal( data["rich"] );

	local nWidth = 1;
	for i = 1, rich:GetTextLines() do
		--local nLineWidth = rich:getLineRealWidth( i - 1 );
		local nLineWidth = rich:getLineWidth( i - 1 );
		
		if nWidth < nLineWidth then
			nWidth = nLineWidth;
		end
	end
	
	nWidth = nWidth + data["xOffset"] + data["xAdjust"];
	local nHeight = rich:GetTotalHeight();
	nHeight = ( nHeight == 0 ) and 1 or nHeight;
	nHeight = nHeight + data["yOffset"] + data["yAdjust"];
	
	rich:resizeRect( nWidth, nHeight );
	rich:SetSize( nWidth / GetScreenScaleY(), nHeight / GetScreenScaleY() );
	frame:SetSize( nWidth / GetScreenScaleY(), math.ceil( ( nHeight + 6 ) / GetScreenScaleY() ) );
end

-- disable�������е�Btn
function util.DisableBtns( t_disableBtns )
	for _, name in ipairs( t_disableBtns ) do
		local btn = getglobal( name );
		btn:Disable();
	end
end

function util.EnableBtns( t_enableBtns )
	for _, name in ipairs( t_enableBtns ) do
		local btn = getglobal( name );
		btn:Enable();
	end
end

-- ���ñ������е�chkBtn
function util.SetChkState( t_chkBtns )
	for name, data in pairs( t_chkBtns ) do
		local chkBtn = getglobal( name );
		chkBtn:setCheckState( data["state"] );
	end
end

-- check�������е�Btn
function util.CheckBtns( t_btns )
	for _, name in ipairs( t_btns ) do
		local btn = getglobal( name );
		btn:Checked( );
	end
end

-- discheck�������е�Btn
function util.DischeckBtns( t_btns )
	for _, name in ipairs( t_btns ) do
		local btn = getglobal( name );
		btn:DisChecked( );
	end
end

-- ���ñ��е�����tga
function util.SetTextures( t_textures )
	for name, data in pairs( t_textures ) do
		local tex = getglobal( name );
		tex:SetTexture( data["path"] );
		local UV = data["UV"];
		tex:SetTexUV( UV["x"], UV["y"], UV["width"], UV["height"] );
	end
end

-- ���ñ��е�����font
function util.SetFonts( t_fonts )
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetText( data["value"] );
		if data["color"] ~= nil then
			font:SetTextColor( data["color"]["r"], data["color"]["g"], data["color"]["b"] );
		end
	end
end

-- ���ñ��е�����edit
function util.SetEdites( t_edites )
	for name, data in pairs( t_edites ) do
		local edit = getglobal( name );
		edit:SetText( data["value"] );
	end
end

-- ���ñ��е�����font����ɫ
function util.SetFontsColor( t_fonts )
	for name, data in pairs( t_fonts ) do
		local font = getglobal( name );
		font:SetTextColor( data["color"]["r"], data["color"]["g"], data["color"]["b"] );
	end
end

-- ���ñ��е�����rich
function util.SetRiches( t_riches )
	for name, data in pairs( t_riches ) do
		local rich = getglobal( name );
		rich:SetText( data["value"], data["color"]["r"], data["color"]["g"], data["color"]["b"] );
	end
end

-- ���ÿؼ��ߴ�
function util.SetIconsWidth( t_chgWidthIcons )
	for name, data in pairs( t_chgWidthIcons ) do
		local icon = getglobal( name );
		icon:SetWidth( data["width"] );
	end
end

function util.SetIconsHeight( t_chgHeightIcons )
	for name, data in pairs( t_chgHeightIcons ) do
		local icon = getglobal( name );
		icon:SetHeight( data["height"] );
	end
end

function util.SetIconsSize( t_chgSizeIcons )
	for name, data in pairs( t_chgSizeIcons ) do
		local icon = getglobal( name );
		icon:SetSize( data["width"], data["height"] );
	end
end

-- ��ÿ���ؼ��Ӷ�Ӧ�����ú���������Utility�����ռ���

-- ������Чʹ��{ relativeFrame = ?, richContent = ?, texturePath = { path = ?, UV = { x = , y = ,  width = , height = } }, textureUsing = { width = ?, height = ?, offsetX = ?, offsetY = ? }, effectBtnUsing = { offsetX = ?, offsetY = ? } }
--[[
function util.BeginGuideEffect( data )
	local frame				= getglobal( "EffectFrame");
	local frameBtn			= getglobal( "EffectFrameGuide");
	local frameBtnTex		= getglobal( "EffectFrameGuideTex");
	local frameBtnRich		= getglobal( "EffectFrameGuideRich");
	local frameTexture		= getglobal( "EffectFrame_Texture");
	local relativeframe		= getglobal( data["relativeFrame"] );
end
--]]

function util.closeParent(w)
	local l = getglobal(w:GetParent())
	if l then
		l:Hide()
	end
end

function util.ShowInCenter(w)
	w:SetPoint("center", w:GetParent() or "UIClient", "center", 0, 0)
end

function util.HightParent(w, h)
	local parent = getglobal(w:GetParent())
	if parent then
		if h or h == nil then
			parent:Highlight()
		else
			parent:DisHighlight()
		end
	end
end

function util.PlaySound(t)
	
end

-- ��ʱ����������ڷֱ��ʴ���1024*768��ʱ��Ľ������
function util.tostringEx(obj)
	local t = type(obj)
	if t == "table" then
		local ret = "table:\n"
		for i, v in pairs(obj) do
			ret = ret.."\t"..util.tostringEx(i)..":"..util.tostringEx(v).."\n"
		end
		return ret
	elseif t == "string" then
		return "\""..obj.."\"\n"
	else
		return tostring(obj).."\n"
	end
end

function util.printEx(obj)
	print(util.tostringEx(obj))
end

util["container"].selectItem = 
							function ( itemData )
								
							end

function util.registerCommand(cmd, action)
	CharPreprocessor = CharPreprocessor or {}
	if CharPreprocessor[cmd] and action then
		--error(string.format("ע��ʧ�ܣ��Ѿ����ڸ��¼�:%s�������ظ�ע��", cmd))
	else
		CharPreprocessor[cmd] = action
	end
end

function util.installGlobalMetatable( data )
	local szFileName	= data["file"] or "";
	local nLine			= data["line"] or "";

	setmetatable(_G, 
	{
		__index = 
		function (_, n)
		   print( "attempt to read undeclared variable "..n.."  in file: "..szFileName );
		end,
	});
end

function util.uninstallGlobalMetatable()
	setmetatable( _G, nil );
end

function util.CompleteShowAllTextOnRich( data )
	local rich		= getglobal( data["rich"] );
	local frame		= getglobal( data["frame"] );
	rich:clearHistory();
	rich:resizeRect( 400 * GetScreenScaleY(), 400 * GetScreenScaleY() );
	rich:SetText( data["text"], data["color"]["r"], data["color"]["g"], data["color"]["b"] );

	local nWidth = 1;
	for i = 1, rich:GetTextLines() do
		local nLineWidth = rich:getLineWidth( i - 1 );
		if nWidth < nLineWidth then
			nWidth = nLineWidth;
		end
	end

	local nHeight = rich:GetTotalHeight();
	nHeight = ( nHeight == 0 ) and 1 or nHeight;
	rich:resizeRect( math.ceil( nWidth + 4 * GetScreenScaleY() ), math.ceil( nHeight + 4 * GetScreenScaleY() ) );
	rich:SetSize( math.ceil( nWidth + 4 * GetScreenScaleY() ) / GetScreenScaleY(), math.ceil( nHeight + 4 * GetScreenScaleY() ) / GetScreenScaleY() );
	rich:SetText( data["text"], data["color"]["r"], data["color"]["g"], data["color"]["b"] );
	
	if frame == nil then
		return;
	end

	frame:resizeRect( math.ceil( nWidth + 12 * GetScreenScaleY() ), math.ceil( nHeight + 18 * GetScreenScaleY() ) );
	frame:SetSize( math.ceil( (frame:GetRealWidth()+ 2) / GetScreenScaleY() ),  math.ceil( (frame:GetRealHeight() + 2 ) / GetScreenScaleY() ) );
	rich:SetPoint( "TopLeft", frame:GetName(), "TopLeft", 6, 3 );
end

function util.CompleteSetRiches( t_riches )
	for name, data in pairs( t_riches ) do
		local rich = getglobal( name );
		util.CompleteShowAllTextOnRich( { ["rich"] = rich:GetName(), ["text"] = data["value"], ["color"] = data["color"] } );
	end
end

-- �����Ʒͼ��
function util.getItemIconPath( data )
	local nItemID	= data["itemID"];
	local ItemDef	= getItemDef( nItemID );
	local IconPath	= GetItemEquipIconPath()..ItemDef.IconID..".tga";
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end

	return IconPath;
end

local function getNextPlayerEnemy(_, actorId)
	local id = ActorMgr:getNextPlayerEnemy(actorId)
	if id == 0 then
		return nil
	end
	return id
end

function util.PlayerEnemys()
	return getNextPlayerEnemy, nil, 0
end

function util.StringHash(str)
	local sum = 0
	for i = 1, #str do
		sum = (sum * 33 + string.byte(str, i, i)) % 4294967296
	end
	return sum
end

local BlockPrepareKey

function util.WaitForBlockPrepare(time, key, text)
	util.BreakBlockPrepare()
	BlockPrepareSlipText:SetText(text or "")
	BlockPrepareKey = key
	StartBlockPrepareSlip(time)
	return block(BlockPrepareSlip)
end

function util.BreakBlockPrepare(key)
	if not BlockPrepareSlip:IsShown() then
		return
	end
	if key and key ~= BlockPrepareKey then
		return
	end
	BlockPrepareSlip:Hide()
end

function util.IsBlockPrepare()
	if not BlockPrepareSlip:IsShown() then
		return
	end
	return true, BlockPrepareKey
end