local ClanPworlTotalTime = 0
local t_ClanPworld = {
					[1] = [[
					
#c461e0a	����ƣ���������#c554623(����)
#c461e0a	�NPC��#L#cff3c00˹�ٷ�˹@@[303006,1000]#n#c554623(ʱ���)
#c461e0a	���ͼ������ұ����
#c461e0a	����ȼ�����10��]],
					[2] = [[#c554623	���ɱ"������"��ø��˻��֣�



#c554623	����˻��ֿɶһ�<���˻��ֽ���>

#c554623	����ɹ������Ҫ�󣬷���<������ֽ���>
#c0000ff	��ȡ��ʽ������ʱ��еĻ�Ա�Զ���ȡ



#c554623	 ���ɱ#L#cff3c00"������"#n,����ȡ<�����߱��佱��>
#c0000ff	��ʾ:ʹ�û�е�ɶ�"������"��ɾ޴��˺�
#c0a6400	 <�����߱��佱��>
#c461e0a	 ����ȼ���10:����ʯ��ϴ��ʯ
#c461e0a	 ����ȼ���61:�ǻԱ���

#c554623	 ������,������ʣ���"���˻���"]],
					[3] = [[	
#c554623	����˻���=#c0a640050#c554623,������������
#c554623	����˻���>#cff000050#c554623,�����������
#c0000ff	(�һ����˻��ֽ���,����"�������")
]],
					}

local t_LevelUp = {
				[130]		= [[#cd44800�����				
#c6f5220    [10��]��ɳ����ѧ���񣬾Ϳ���Я���������ζ�������ǿ��ĳ���
#c6f5220    ��ʼ��������[��ѧ]��ʶ����
#cd44800����ͼ��
#c6f5220    ʱ������������ļ�԰��������ȥ����һ�����֮�ǵĻԻͣ��������ԣ�
#c6f5220    ���빫��[#L�������Ա-˹�ٷ�˹@@[303006,1000]#n]
#c6f5220    ѧϰ����[#L����ѵ����ʦ-������@@[306000,1000]#n]
#c6f5220    �����Ե�ϸ�������������ð���ó̡�]],

				[150]		= [[#cd44800������-ս�Ĵ塤
#c6f5220    BOSS���հ͹�����
#c6f5220    ������������������-[����]�������졢�ȼ���15
#c6f5220    ������ڣ�#L��Ӱ[ϸ������]@@[303005,2100]#n
#c6f5220    �Ƽ��ȼ���18~20��
#c6f5220    �����Ѷȣ���
#c6f5220    �����������������顢��ɫƷ��װ��
#cd44800�������������ա�
#c6f5220    ʱ��ǵļ����������ջ�ӭ���Ĺ�ˣ���������ֻҪ��Щ�����ұ����������ʵĺ��շ��񣬴�����ʱ��Ǹ������С�
#c6f5220    ����NPC��������������-#Lϸ������@@[333056,2100]#n
#cd44800����ͼ��
#c6f5220    �����Լ�����ϸ���������ð���ó̡�]],


				[250]		= [[
#cd44800����������ҹ�ȡ�
#c6f5220    BOSS�������ܿˡ���Ѫ�����ǿ���˹���ڰ�ħ��ʦ��˹̹��
#c6f5220    ������ڣ���ҹָ��-#L������[����ѩԭ]@@[302028,2200]#n
#c6f5220    �Ƽ��ȼ���29~35��
#c6f5220    ����������30����װ����ɫ����ר���ѶȲ�����ɫװ��]],


				[300]	=[[#cd44800�����ᡤ
#c6f5220    [30��]�����Խ����Լ��Ĺ��ᡣ������Ѿ����빫�ᣬ�ɽ��ܹ���ר������
#c6f5220    NPC���������Ա-#L˹�ٷ�˹[ʱ���]@@[303006,1000]#n
#cd44800�����
#c6f5220    [30��]������ŵ��Ӯ��һ���˶��ᣬ��ͻ���һֻ�����ƶ��ٶȵ�����
#c6f5220    ��ʼ��������[��ѧ]�˶���
#cd44800�����淨-�����ȡ�
#c6f5220    �������ƣ�1��/ÿ��
#c6f5220    ���ڣ������ȿ�����-#L���ɵ�[ʱ���]@@[302046,1000]#n
#c6f5220    ��������������顢װ��ǿ�����ߡ�����û�����
#cd44800�����淨-��ɽ����
#c6f5220    �������ƣ�1��/ÿ��
#c6f5220    ���ڣ���ɽ��ָ�ӹ�-#L����ѷ[ʱ���]@@[303007,1000]#n
#c6f5220    ��������������ҡ����＼���顢װ��ǿ������
#cd44800�����淨-���ˡ�
#c6f5220    [30��]�ڻ������̻�-#L����[ʱ���]@@[302041,1000]#n������������������˿ɻ�ô������顢���ҡ�
#cd44800����ͼ��
#c6f5220    [30��]������ǰ������ɳĮ��ʼð���ó̡�]],


				[400]	=[[#cd44800�����淨-��ɳ�Ƕ���
#c6f5220    �������ƣ�������
#c6f5220    ���ڣ�#L��ɳ�Ƕ���-�ޱ�ѷ[ʱ���]@@[302047,1000]#n
#c6f5220    �����������PVPװ�������潱�£���ʤ����
#cd44800�����淨�����ּ��껪��
#c6f5220    ����ʱ�䣺������������ʾ
#c6f5220    ���ڣ����ּ��껪-#L��Ƥ��[ʱ���]@@[302045,1000]#n
#c6f5220    �����������ҩˮ������û����ߡ�����	

#cd44800����ͼ��
#c6f5220    [40��]������ǰ�����ջ�Ұ��ʼð���ó̡�
#c6f5220    [45��]���Ե�ʮ���ſ���ȡ�������񣬻�õͼ��ر�ͼ��]],



				[450]	=[[
#cd44800���¸���-�ƽ�󾮡�
#c6f5220    �Ƽ��ȼ���45~55��
#c6f5220    ���ڣ�#L��¡��[���ջ�Ұ]@@[302035,2400]#n
#c6f5220    �������50����װ����ɫ����ר���ѶȲ�����ɫװ��
#cd44800����ͼ��
#c6f5220    [45��]���Ե�ʮ���ſ���ȡ�������񣬿ɻ�õͼ��ر�ͼ��]],

				[500]	=[[#cd44800��ְҵ���
#c6f5220    [50��]ְҵ�����Ǹ�ְҵ���е���������ڿ�����ŵ����ѯ�й�ְҵ�������Ϣ�����������������ȥ��ȡ��
#c6f5220    ��������NPC����������-#L��ŵ��[ʱ���]@@[302001,1000]#n
#c6f5220    �Ƽ��ȼ���50��
#cd44800��תְ��
#c6f5220    ��͵ȼ���[55��]
#c6f5220    �������ƣ�Ӣ��֮·
#c6f5220    ������NPC������ѵ����ʦ-#L������[ʱ���]@@[306000,1000]#n
#c6f5220    תְ��ÿλӢ�۳ɳ��еıؾ�֮·��תְ������������õ��ʵķ�Ծ��
#cd44800����ͼ��
#c6f5220    [50��]������ǰ��Խ�����ֿ�ʼð���ó̡�
#c6f5220    [55��]���Ե�Խ������ȡ�������񣬿ɻ�òر�ͼ��]],

				[580]	=[[#cd44800�����顤
#c6f5220    [58��]ӵ�о����ɻ�þ���ǿ������������ٻ����鸽�壡ʱ��Ǵ������ٰ������׵½�ָ������λ�õ�һֻ���顪������
#c6f5220    �Ƽ��ȼ���60��
#c6f5220    ��ʼ��������[����]��;����и�Լ��
#c6f5220    ��������NPC��#Lʱ��Ǵ�������-�������׵�@@[306013,1000]#n[ʱ���]  
]],


				[600]	=[[#cd44800�����淨-���޷籩ս��
#c6f5220    �������ƣ�������
#c6f5220    ���ڣ��򿪻���ֱ��������ߵ������ͼ�걨���μ�
#c6f5220    �����������PVPװ�������潱�£�ս�����]],


				[700]	=[[#cd44800������-̫����
#c6f5220    �������ƣ�1��/ÿ��
#c6f5220    ���ڣ�#L���϶�-��˹[ʱ���]@@[304061,1000]#n
#c6f5220    �������װ��ǿ����ʯ������ǿ��ʯ����չ��������ϡ����
				]],

				[130]	=[[#cd44800��ŭ���Į��
#c6f5220    30��������Ե�ŭ���Į����ð�գ�������ӷḻ��ʵ�����
#cd44800�������ȸ�����
#c6f5220    30���������ͨ��ʱ��ǵ�#L���ɵ�@@[302046,1000]#n�������ȸ�������ս������ø���ľ���͸��õ�װ����
#cd44800�����ڻ��ͻ��
#c6f5220    30���������ͨ��ʱ��ǵ�#L����@@[302041,1000]#n�μ����ڻ��ͻ����ø���Ľ�Ǯ�;��顣
#cd44800����ɳ�Ƕ�����
#c6f5220    30���������ͨ��ʱ��ǵ�#L�ޱ�ѷ@@[302047,1000]#n�����μӿ�ɳ�Ƕ������˾�����PVP�����ø���ľ���͸��õ�װ�����ſ�����
#cd44800����Ŭ��˹��
#c6f5220    30��������Ե���Ŭ��˹������ս������ø���ľ��顣
#cd44800�����顤
#c6f5220    [10��]��ɾ����ѧ���񣬿ɻ��һֻӵ��ǿ���ܵġ����񡱾���
#c6f5220    ��ʼ��������[��ѧ]��;����и�Լ��
#c6f5220    ������NPC��#L�޲���[ʱ���]@@[302003,1000]#n]],


				[130]	=[[#cd44800�����ջ�Ұ��
#c6f5220    40��������Ե����ջ�Ұ����ð�գ�������ӷḻ��ʵ�����
#cd44800���ƽ�󾮸�����
#c6f5220    40���������ͨ��#L���@@[302026,2400]#n���ƽ�󾮸�������ս��������������£��������¿���ʱ��ǵ�#L÷��@@[302033,1000]#n���һ��ڶ���ְҵ��װ��
#cd44800��ʮ���ſ󶴡�
#c6f5220    40��������Ե�ʮ���ſ󶴽���ս������ø���ľ��顣]],


				[131]	=[[#cd44800���������֡�
#c6f5220    50��������Ե��������ּ���ð�գ�������ӷḻ��ʵ�����
#cd44800����������
#c6f5220    50��������Ե�����������ս������ø���ľ��顣]],



				[130]	= [[#cd44800��ʱ��ǡ�
#c6f5220    ����Դ��͵�ʱ��ǣ���#L������@@[304001,2000]#n ���ɽ��д��͡�
#cd44800�����顤
#c6f5220    15�����Ե�ʱ���xxxxxxxxxxxxx��
#cd44800�����
#c6f5220    15�����Ե�ʱ���xxxxxxxxxxxxx
#cd44800���ʺ絺��
#c6f5220    15�����Ե�ʱ���xxxxxxxxxxxxx]],
				};
			
--����������˻���	
local AwardVal = 0
--�������𹫻����
local AwardClanVal =0
local AwardClanMaxVal = 0
local ShowBoss = 0

function setLevelUpTips( nlevel )
	if t_LevelUp[ nlevel ] ~= nil then
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_XINSHOU_BTN_LEVEL, frame = "LevelUpTipFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\!.tga", level = nlevel } );
	end
end

function setClanPworldTips()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_PWORLD ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_CLAN_PWORLD, frame = "ClanPworldTipFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\12.tga", index = 0 } );
	end
end

function setClanPworldTipsFrameInfo( clanContri, clanMaxVal, clanMoney, clanVal, val, endTime, playerMoney, playerExp, showBoss )
	local mainplayer = ActorMgr:getMainPlayer();
	local levelupdef = getLevelUP( mainplayer:getLv() );
	local maxClanMoney = 1500 * 10/(510-50*10);
	local maxPlayerExp = levelupdef.ContributeExp / 200;
	ClanPworldTipFrameClanMoney:SetText( "�����ʽ�"..clanMoney.."/"..maxClanMoney );
	ClanPworldTipFramePersonalExp:SetText( "���˾��飺"..playerExp.."/"..maxPlayerExp );
	AwardVal = val;
	AwardClanVal = clanVal;
	AwardClanMaxVal = clanMaxVal;
	ShowBoss = showBoss
	local ClanViewer		= GameClanManager:getClanViewer();
	local ClanInfo			= ClanViewer.m_clanInfo;
	local clanLevelDef		= getClanLevelDef( ClanInfo.m_nClanLevel);
	local PersonalGetValFontString = getglobal("ClanPworldTipFramePersonalGetVal")
	if clanVal >= clanMaxVal then
		local PersonalGetValTextFontString = getglobal("ClanPworldTipFramePersonalGetValText");
		PersonalGetValTextFontString:SetTextColor( 128,128,128 );
		PersonalGetValFontString:SetTextColor( 128,128,128 );
		local szInfo = clanLevelDef.ClanValContri
		PersonalGetValFontString:SetText( szInfo );
	else
		PersonalGetValFontString:SetText( clanLevelDef.ClanValContri );
		PersonalGetValFontString:SetTextColor( 70,30,10 );	
	end
	ClanPworlTotalTime = endTime;
	SetEndTime();
end
function SetClockNum( num )
	if num >9 then
		return tostring( num );
	else
		return "0"..num;
	end
end
function SetEndTime()
	if ClanPworlTotalTime == 0 then
		return;
	end
	local timer = ClanPworlTotalTime - getServerTime().sec;
	if timer <= 0 then
		ClanPworldGameEnd();
		return ;
	end
	local nHour = math.floor( timer/3600 )
	local nMinute =  math.floor( ( timer - nHour*3600 ) / 60 )
	local nSecond = math.ceil( timer - nHour*3600 - nMinute*60 )
	local ClockTime = "";
	if nHour > 0 then 
		ClockTime = SetClockNum( nHour )..":"..SetClockNum( nMinute )..":"..SetClockNum( nSecond )
	else 
		ClockTime = SetClockNum( nMinute )..":"..SetClockNum( nSecond );
	end
	local EndTimeFontString	= getglobal("ClanPworldTipFrameEndTime")
	EndTimeFontString:SetText( ClockTime );
end
function ClanPworldTipFrame_onUpdate()	
	local ClanViewer		= GameClanManager:getClanViewer();
	local ClanInfo			= ClanViewer.m_clanInfo;
	local clanLevelDef	= getClanLevelDef( ClanInfo.m_nClanLevel);
	local PersonalGetValFontString = getglobal("ClanPworldTipFramePersonalGetVal")
	if AwardClanVal >= AwardClanMaxVal then
		local PersonalGetValTextFontString = getglobal("ClanPworldTipFramePersonalGetValText");
		PersonalGetValTextFontString:SetTextColor( 128,128,128 );
		PersonalGetValFontString:SetTextColor( 128,128,128 );
		PersonalGetValFontString:SetText( clanLevelDef.ClanValContri );
	else
		PersonalGetValFontString:SetText( clanLevelDef.ClanValContri );
		PersonalGetValFontString:SetTextColor( 70,30,10 );	
	end
	if IsClanPworldBegin() then
		ClanPworldTipFrameClanPworldStateRunning:SetGray( false );
		ClanPworldTipFrameClanPworldStateStateFont:SetText( "������" );
		ClanPworldTipFrameGetScore:Enable();
		ClanPworldTipFrameEnterClanPworldIsland:Enable();
		SetEndTime();
	end
end

function LevelUpTipFrame_ScrollBar_OnValueChanged()
	local slider = getglobal("LevelUpTipFrameScrollBar");
	local rich	= getglobal( "LevelUpTipFrameSecondRich" );
	rich:SetDispPos( rich:GetStartDispPos() );
	for i = 1, slider:GetValue() do
		rich:ScrollDown();
	end
end
function ClanPworldTipFrame_onHide()
	SetClanPworldFrameShow( false )
end
function ClanPworldTipFrame_onShow()
	local InfoRich = getglobal( "ClanPworldTipFrameSecondRichInfo" );
	InfoRich:SetLinkTextColor( 10, 100, 0);
	InfoRich:SetText( t_ClanPworld[1],111,82,32 );
	local GiftRich = getglobal( "ClanPworldTipFrameSecondRichGift" );
	GiftRich:SetLinkTextColor( 10, 100, 0);
	GiftRich:SetText( t_ClanPworld[3],111,82,32 );
	local RoleRich = getglobal( "ClanPworldTipFrameSecondRichRole" );
	RoleRich:SetLinkTextColor( 10, 100, 0);
	RoleRich:SetText( t_ClanPworld[2],111,82,32 );	
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	if IsClanPworldBegin() then
		ClanPworldTipFrameClanPworldStateRunning:SetGray( false );
		ClanPworldTipFrameClanPworldStateStateFont:SetText( "������" );
		ClanPworldTipFrameGetScore:Enable();
		ClanPworldTipFrameEnterClanPworldIsland:Enable();
		SetEndTime();
	end
	SetClanPworldFrameShow( true );
end
function ShowClanPworldFrame()
	ClanPworldTipFrameClanPworldStateRunning:SetGray( true );
	ClanPworldTipFrameClanPworldStateStateFont:SetText( "δ����" );
	ClanPworldTipFrameGetScore:Disable();
	ClanPworldTipFrameEnterClanPworldIsland:Disable();
	ClanPworldTipFrameEndTime:SetText( "" );
	ClanPworldTipFrame:Show();
end
function HideClanPworldFrame()
	local ClanPworldTipFrame = getglobal( "ClanPworldTipFrame" );
	ClanPworldTipFrame:Hide();
end
function ClanPworldTipFrameAwardBtn_onClick()
	if AwardVal > 0 then
		GameClanManager:clan_Opt_PworldAward();
	else
		ShowMidTips( "�޶һ���������ĸ��˻���" );
	end
end
function ClanPworldIslandShowInfoFrameExChange_onEnter()
	SetGameTooltips( this:GetName(), "����һ�<���˻��ֽ���>\n���˻���=#c00ff0050#cffffbe,������������\n���˻���>#cff000050#cffffbe,�����������" );
end
function ClanPworldIslandShowInfoFramekulou_onClick()
	if not GameClanManager:IsInClanPworldIsland() then
		ShowMidTips( "����ǰ���ڹ���ұ����" );
		return;
	end
	
	MapShower:GetMaxMapContal():linkMoveto( CLAN_EVERYDAY_PWORLD_MAP_ID_LUA, 16700, 19000 );
end
function ClanPworldIslandShowInfoFramekulou_onEnter()
	SetGameTooltips( this:GetName(), "���ǰ���������ߡ�����λ��\n#c00ff00����Ҫ����ɹ������Ҫ��" );
end
function SecondRichRole_OnClick()
	if not GameClanManager:IsInClanPworldIsland() then
		ShowMidTips( "����ǰ���ڹ���ұ����" );
		return;
	end
	
	MapShower:GetMaxMapContal():linkMoveto( CLAN_EVERYDAY_PWORLD_MAP_ID_LUA, 16700, 19000 );
end
function ClanPworldTipFrameEnterClanPworldIsland_onClick()
	--[[
	if GameClanManager:IsInClanPworldIsland() then
		ShowMidTips( "����ǰ���ڹ���ұ����" );
		return;
	end
	--]]
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:getLv() < 10 then
		ShowMidTips( "����ȼ�δ�ﵽ�Ҫ����ʱ�޷��μ�" );
		return;
	end
	local nCurMapID;
	local MaxMapControl	= MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() == 0 then
		nCurMapID = MapShower:getRealMapId();
	else
		nCurMapID = MaxMapControl:getMapID();
	end
	if nCurMapID >= MIN_COPY_ID then
		ShowMidTips( "��ǰ�������ܴ���" );
		return;
	end
	GameClanManager:clan_Opt_EnterClanPworldIsland();
end

function ShowLevelUpTipFrame( nlevel )
	if t_LevelUp[ nlevel ] ~= nil then
		LevelUpTipFrameFirstLineFont:SetText("��ϲ������".. nlevel .."�����������鵽��������");
		local rich = getglobal("LevelUpTipFrameSecondRich");
		local slider  = getglobal( "LevelUpTipFrameScrollBar" );  
		local sliderThumbTex = getglobal( "LevelUpTipFrameScrollBar".."ThumbTexture" );
		rich:SetLinkTextColor(10,110,0);
		rich:SetText( t_LevelUp[ nlevel ],111,82,32 );
		local nViewLine	= rich:GetViewLines();
		local nAllLine	= rich:GetTextLines();
		rich:SetDispPos(rich:GetStartDispPos());
		slider:SetValue(0);
		slider:Hide();
		sliderThumbTex:Hide();
		if nAllLine - nViewLine > 0 then
			local nMaxValue = nAllLine - nViewLine; 
			slider:Show();
			slider:SetMaxValue( nMaxValue );
			slider:SetMinValue(0);
			slider:SetValueStep(1);
		end
		LevelUpTipFrame:Show();
	end
end
function ClanPworldIslandShowInfoFrame_onUpdate()
	local szColor = "#c888888";
	if AwardVal == 50 then
		szColor = "#c00ff00";
	elseif AwardVal > 50 then
		szColor = "#cff0000";
	end
	ClanPworldIslandShowInfoFramePersonalVal:SetText( szColor..AwardVal.."#cffffbe/50", 255, 120, 120 )
	local szInfo = ( AwardClanVal >= AwardClanMaxVal ) and "#c888888"..AwardClanVal.."#cffffbe/"..AwardClanMaxVal or "#cff0000"..AwardClanVal.."#cffffbe/"..AwardClanMaxVal
	ClanPworldIslandShowInfoFrameClanVal:SetText( szInfo, 255, 120, 120 )
	if AwardVal >= 50 then
		local tex = getglobal( "ClanPworldIslandShowInfoFrameExChange_Texture" )
		tex:SetUVAnimation( 48, true );
		tex:Show();
	else
		local tex = getglobal( "ClanPworldIslandShowInfoFrameExChange_Texture" )
		tex:Hide();
	end

	if ShowBoss > 0 then
		local tex = getglobal( "ClanPworldIslandShowInfoFramekulou_Texture" )
		tex:SetUVAnimation( 48, true );
		tex:Show();
	else
		local tex = getglobal( "ClanPworldIslandShowInfoFramekulou_Texture" )
		tex:Hide();
	end
end
function LevelUpTipFrame_OnShow()
	this:SetPoint( "center", "UIClient", "center", 0, 0 );
end

local t_Message =
{

				[1]   =  { "���Ὠ��",
			      [[
#cd44800������1����
		#c6f5220���Ὠ������Ա��������50�ˡ�
		#c6f5220��������Է�չ�����Լ��Ĺ����ˣ���������Ѷ���������ɡ�
#cd44800�����ݡ�
		#c6f5220�����������#L�������Ա-˹�ٷ�˹@@[303006,1000]#n��ȡ�����ḻ���ճ�����
		#c6f5220������#L�������Ա-˹�ٷ�˹@@[303006,1000]#n��ȡ���ỷ�����񣬸����������͵Ĳ�ͬ���Ի�ù���Ĳ�ͬ����ֵ��

#cd44800��С��ʾ��
		#c6f5220��������Լ���������ױ���ע����ô����߹�������İ����Ⱥ�������,�����������ʿɹ������ٶ�����
		#c6f5220���˹���ӵ�еĻ��������⣬�������Լ��༭�����ٽ׵����ƣ��᳤���Զ�ÿ�������Ա������Ȥ���ǳƵ�����ע
		
		#c6f5220ע��������������10�ˣ�����7��󣬽���ϵͳ���գ����պ���չ���������Ϣ]],
						},
			
				[2]   =  { "��������",
			      [[
#cd44800������2����
		#c6f5220���ᵽ��2������Ա����������80�ˡ�
		#c6f5220���������ȥ�����Ƽ������ˣ��Ƽ����ĵ���2�����и�����õĿƼ�����ѧϰ��

#cd44800��С��ʾ��
		#c6f5220�����Լ����ᱻ�����ױ���ע����ô����߹�������İ����Ⱥͳ�Ա�������ʰ�
	
		#c6f5220ע��������������10�ˣ�����7��󣬽���ϵͳ���գ����պ���չ���������Ϣ]],
	
						},	
						
				[3]   =  { "��������",
			      [[
#cd44800������3����
		#c6f5220���ᵽ��3������Ա����������120�ˡ�
		#c6f5220�������µĿƼ������о������ڹ���Ƽ����鿴��
		#c6f5220�����Զ�����BOSS���ɼƻ������ڿ���ǰ��������ж�BOSS��ע����

#cd44800��С��ʾ��
		#c6f5220�����Լ����ᱻ�����ױ���ע����ô����߹�������İ����Ⱥͳ�Ա�������ʰ�
	
		#c6f5220ע��������������10�ˣ�����7��󣬽���ϵͳ���գ����պ���չ���������Ϣ]],
	
						},
								
				[4]   =  { "��������",
			      [[
#cd44800������4����
		#c6f5220���ᵽ��4������Ա����������160�ˡ�
		#c6f5220�������µĿƼ������о������ڹ���Ƽ����鿴��

#cd44800��С��ʾ��
		#c6f5220�����Լ����ᱻ�����ױ���ע����ô����߹�������İ����Ⱥͳ�Ա�������ʰ�
	
		#c6f5220ע��������������10�ˣ�����7��󣬽���ϵͳ���գ����պ���չ���������Ϣ]],
	
						},
				[5]   =  { "��������",
			      [[
#cd44800������5����
		#c6f5220���ᵽ��5������Ա����������200�ˡ�
		#c6f5220�������µĿƼ������о������ڹ���Ƽ����鿴��

#cd44800��С��ʾ��
		#c6f5220�����Լ����ᱻ�����ױ���ע����ô����߹�������İ����Ⱥͳ�Ա�������ʰ�
	
		#c6f5220ע��������������10�ˣ�����7��󣬽���ϵͳ���գ����պ���չ���������Ϣ]],
	
						},
														
						
						
----[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[ ----�������

				[10]		= {"������н���",
								[[
#cd44800���������1����
		#c6f5220���Ĺ������г�Աӵ���˽��뱾����ר�����е�Ȩ����
		#c6f5220#L�������Ա-˹�ٷ�˹@@[303006,1000]#n���ͽ��빫����е�ͼ��
		#c6f5220#L������������-����˹����@@[304051,60000]#n������Ƽ�����
		#c6f5220Ȼ�����#L������������-����˹����@@[304051,60000]#n�����蹤��
#cd44800��������     
		#c6f5220�Ƽ�����#L�Ƽ��з�-���@@[304052,60000]#n��ѧϰ���ֹ���Ƽ���Ψһ;����
		#c6f5220����#L��������-�޲�@@[12800,16200]#n��ѧϰ�˶�Ӧ�ĿƼ������������������ĵ��߰ɡ�
		#c6f5220��Դ���ģ�����������Ƽ����ߡ���е���ɼ���Դ�Ĵ�Ŵ���
#cd44800����ͼ��
		#c6f5220���Ĺ���������ڱ������Ա��ר����ͼŶ�����������ɳ��룬�μ���Ȥ������ͻ��
		#c6f5220�����Ҳ������ʱ������������飬���������ķ羰��
#cd44800��С��ʾ��
		#c6f5220��ͼ����Ƿ�յģ���Ҫ����Ƽ����ĺ�ſɵ�����������]],
								},
							
				[11]		= {"�����������",
								[[
#cd44800���������2����

		#c6f5220#L������������-����˹����@@[304051,60000]#n�����蹤��
#cd44800��������     
		#c6f5220�Ƽ�����#L�Ƽ��з�-���@@[304052,60000]#n��ѧϰ����Ƽ�
		#c6f5220��������#L��������Ա@@[304055,60000]#n����ѧϰ����Ƽ��󣬼��ɽ��蹤������������ɼ������ĵ������Լ���е����Ʒ��

#cd44800����ͼ��
		#c6f5220�����������ʱ������������飬���������ķ羰Ŷ��]],
							},			
								
							
------[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[---�Ƽ�����
			   [20]   =  { "�Ƽ�����",
		      [[
#cd44800���Ƽ�����1����
		#c6f5220�Ƽ����Ľ���ɹ�������#L�Ƽ��з�-���@@[304053,60000]#n��ѧϰ���¿Ƽ�����
		#c6f5220�ɼ��Ƽ����ɼ��Ƽ�������ͨ�ɼ��������
		#c6f5220�������ߣ��ɼ���û�����ˣ�ûѪ���ˣ�ѧϰ��Ӧ�ĿƼ������Ӧ�ĵ��ߣ��ͺ�-������������е����-�ָ���еѪ��
		#c6f5220�����Ƽ�����ʴ����40MM���Ͳ �ȵ�
#cd44800��С��ʾ��
		#c6f5220�Ƽ����Զ��ͬʱѧϰ��ʡʱʡ��	]],

						},	
---[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[---�ɼ�����				
			 [30]   =  { "�ɼ�����",
			      [[
#cd44800���ɼ�����1����
		#c6f5220����ɼ�������������4�ֲ�ͬ�Ļ����Ͳɼ��������ڲɼ����ֲ�ͬ����Դ
		#c6f5220����ɼ��������Լ�ʻ�ɼ�������ɼ�����Ҳ����ֻ��ǰ�����������������Ŀ�Ҫ����Ŷ��
					 <�κι����Ա������ɼ������ɽ���˵�ͼ>
					 
		#c6f5220#L�ɼ����Ĺ���@@[304053,60000]#n�����ɼ�����]],
						},			

			 [31]   =  { "�ɼ���������",
			      [[
#cd44800���ɼ�����2����
		#c6f5220��ѧϰ���ߵĲɼ��Ƽ�����ǿ�ɼ����ĸ������ԡ�
		#c6f5220����ɼ�������������4�ֲ�ͬ�Ļ����Ͳɼ��������ڲɼ����ֲ�ͬ����Դ
		#c6f5220����ɼ��������Լ�ʻ�ɼ�������ɼ�����Ҳ����ֻ��ǰ�����������������Ŀ�Ҫ����Ŷ��
					  <�κι����Ա������ɼ������ɽ���˵�ͼ>
								  
		#c6f5220#L�ɼ����Ĺ���@@[304053,60000]#n�����ɼ�����]],
						},			
							
				
----[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[----����

			 [40]   =  { "����",
			      [[
#cd44800��������
		#c6f5220������Ĺ����������һЩ��е�������Ƽ������ˣ�����Ҫ��ѧ��Ƽ�
		#c6f5220�ɼ������Ϳ�ɼ����������ʯ�ɼ������
		#c6f5220�����Ƽ����ͺ�����е���֡���ʴ����40MM���Ͳ]],
						},			


			 [180]   =  { "���ʱ��",
			      [[
#cd44800����ô��ý�����
		#c6f5220���ࣺÿ���10����1�ξ��齱����
		#c6f5220��е�����ɱ���ࡢ��ɱ��е���˿ɻ�ý���
		#c6f5220��е���ˣ���ɱ���ࡢ��ɱ��е����ɻ�ý���		
#cd44800����Ӫʤ������
		#c6f5220����ʤ���������ʱ������ֻҪ��1�˴�������ʤ��
		#c6f5220��е����ʤ���������ʱ��û����������һ�е�������� > ��е����������Ϊʤ��
		#c6f5220��е����ʤ���������ʱ��û����������һ�е�������� > ��е����������Ϊʤ��
#cd44800��С��ʾ��
		#c6f5220����������Ȼ��С���������������Ĳ��������
		#c6f5220׼��ʱ��ʱ�����Զ����ƫƧ�Ľ��䣬��Ϊ�㲻֪����ʼ��˭�ǵ��ˣ�˭�����ѣ�
		
						]],
						},	
						
						
			 [200]   =  { "����ɳĮս��",
			      [[
#cd44800��Ŀ�ꡤ
		#c6f5220��ɱ�з���Ӫָ�ӹټ��ɻ��ʤ��#n
#cd44800���淨��		
		#c6f5220˫����ָ�ӹ�ÿ������ͻ��ܵ��з��ɻ��Ŀ�Ϯ��ռ������ĸ�������ʹ����ָ�ӹٱ����Ϯ�˺�#n
#cd44800��͵Ϯ��			
		#c6f5220˫�������з�������ÿ�οɳ���1�ˣ��ɵ��ط�����͵Ϯ����������ÿ��10��Ż�ˢ��һ��
#cd44800������㡤	
		#c6f5220������������̬������ռ�죬ռ��󣬿������Լ�����ĸ���㸴��#n
#cd44800��С��ʾ��			
		#c6f5220Ҫʱ��ע�⼺��ָ�ӹٵ�Ѫ������ʾ#n]],
						},

			[210]   =  { "���Ҫ��ս��",
			      [[
#cd44800��Ŀ�ꡤ
		#c6f5220�����ӿ������������з�Ҫ��λ�ã��ɴݻٵط�Ҫ���������ʤ��#n
#cd44800���淨��		
		#c6f5220��Դ�ᾧ���ڵ�ͼ�п�Ѱ�ҵ���Դ�ᾧ���ɼ��������ӿ��������ύ��Դ��#n
		#c6f5220���ӿ��������ڵ�ͼ����λ�ã���ͬ��Ӫ�ύ��Դ�ᾧ�󣬻���Է���Ӫǰ�����ƶ����з�Ҫ��λ�ú󣬿ɻ���з�Ҫ����#n
#cd44800���ر���ʾ��	
		#c6f5220��ɫ����������Я������Դ�ᾧ����ʧ��ע���ɱ�з���ӪЯ����Դ�ᾧ��ҡ�#n
#cd44800��ע�⡤			
		#c6f5220Ҫʱ��ע�����ӿ�����λ�ã���ʱ��ɱ�з���Ӫ���������ṩ��Դ�ᾧ�������ӿ������ƶ���#n]],
						},


			 [300]   =  { "����Ԫ��",
			      [[
		#c6f5220����һ����������������Ԫ�س�����#c0181f9������ѩԭ��#n��#n	
		
#c6f5220��Я�����������ġ��ҵ�����Ԫ�أ���֮�Ի����ռ�����!#n
#c6f5220�ռ�����������#L�������׵�[ʱ���]@@[306013,1000]#n���ύ������ҽ�����#n
		
#c6f5220����Ԫ�ػ���ÿ���11:00-16:00��18:00-23:00������֡�#n
#c6f5220ÿ�γ��ֳ���8���ӡ�#n
#c6f5220�����ռ��������������Ŀ���ͨ�������ˡ����á�#n]],["path"] = "uires\\TuPianLei\\PiaoYiTiShiTuBiao\\HunLuanYuanSu.tga",
						},						

			 [301]   =  { "����Ԫ��",
			      [[
		#c6f5220����һ����������������Ԫ�س�����#c0181f9��ʮ���ſ󶴡�#n��#n	
		
#c6f5220��Я�����������ġ��ҵ�����Ԫ�أ���֮�Ի����ռ�����!#n
#c6f5220�ռ�����������#L�������׵�[ʱ���]@@[306013,1000]#n���ύ������ҽ�����#n
		
#c6f5220����Ԫ�ػ���ÿ���11:00-16:00��18:00-23:00������֡�#n
#c6f5220ÿ�γ��ֳ���8���ӡ�#n
#c6f5220�����ռ��������������Ŀ���ͨ�������ˡ����á�#n]],["path"] = "uires\\TuPianLei\\PiaoYiTiShiTuBiao\\HunLuanYuanSu.tga",
						},	
						
			 [302]   =  { "����Ԫ��",
			      [[
		#c6f5220����һ����������������Ԫ�س�����#c0181f9����Ŭ��˹��#n��#n	

#c6f5220��Я�����������ġ��ҵ�����Ԫ�أ���֮�Ի����ռ�����!#n
#c6f5220�ռ�����������#L�������׵�[ʱ���]@@[306013,1000]#n���ύ������ҽ�����#n
		
#c6f5220����Ԫ�ػ���ÿ���11:00-16:00��18:00-23:00������֡�#n
#c6f5220ÿ�γ��ֳ���8���ӡ�#n
#c6f5220�����ռ��������������Ŀ���ͨ�������ˡ����á�#n]],["path"] = "uires\\TuPianLei\\PiaoYiTiShiTuBiao\\HunLuanYuanSu.tga",
						},	

						
			 [310]   =  { "��Ӱ֮ս",
			      [[
    #c6f5220������֣���ʿ�ǣ���ǰ�壡��#n	
    #c6f5220���ʼ������������#c0181f9��Ӱ֮��#n����Ӱ������#c0181f9ʥ��֮��#n��10����δ��ɱ#c0181f9��Ӱ֮��#n����Ϊ#c0181f9��Ӱ����#n��#c0181f9ʥ��֮��#n����Ϊ#c0181f9ʥ������#n��������и��˺��Ĺ���������
	#c6f5220��������ɱ����һֻ�֣���Ӱ����ˢ��һֻ#c0181f9ʥ��֮��#n����Ӱ��ɱ����һֻ�֣���������ˢ��һֻ#c0181f9��Ӱ֮��#n��#n
    #c6f5220�������ʣ����������ٵ�һ����ʤ�������뾭�������ҽ���������#Yȫ����ɱ������������ǰ60%����ҿɻ�����ҽ���#n��#n
    #c6f5220���ÿ���12��00-12:10 ��16:30-16:40����������10���ӡ�ͨ��#Gޱޱ��[ʱ���]#n������#n]],

						},	
			[320]    =  { "����֮��",
			      [[			
    #c6f5220������ֻʣ��һ��������Ŧʱ����Ŧ�����Ĺ���ʤ�������ȱ��ݻ���Ŧ�Ĺ���Ϊ��3����
    #c6f5220������ʣ����Ŧ����1��������ŦѪ�������������Σ�Ѫ�����Ĺ���ʤ����
#GĿ��#n
    #cff0080��ɱ�з������Ա���ݻٵз�������Ŧ#n
#G͵Ϯ#n
     #c6f5220ÿ����Ӫ�����з�������ÿ�οɳ���1�ˣ��ɵ�˳ʱ�뷽��Ĺ������͵Ϯ���ɳ���������Ӫ����ѷ���������ѷ���������һ����Ҫ15��
#GС��ʾ#n
    #c6f5220Ҫʱ��ע���ѷ�������Ŧ��Ѫ��]],
						},	
			 [410]   =  { "תְ����ȫ����",
[[#c6f5220תְ���񡪡��������ɸǶ������ξ��ĸ�������Ϊ5�����裺#n
#c6f5220#cFF0080ע�����²����谴˳����У��������޷����е��������������������½�ȡ#n
		
#Y����һ��#n
    ��#L������@@[306020,4402]#n�Ի�����þ�
    �˻þ�����ʿ������#L��Ʊ@@[303023,1000]#n���룩

#Y�����������һ�أ�#n
    �ҵ�#L���ɸǶٵ�����@@(50900,4300,20900)#n
    ���㿿���������壬�������ͻ����������
    �����Ի�#G��ȡ���������������#n
    ʹ��#G���񱳰��е�ذ��#n�������
    ��ϲ�㣬�������һ����

#Y�����������ڶ��أ�#n
    ��������#L���ж�@@(50900,5500,9700)#n�Ի�������ѡ��
    ѡ��һ��#Gǩ����Լ#n���Ƽ���
      ǩ����Լ����ж����ɻ�е
      ����������ͻ�͹��ж����壬ͬʱ�������ǿ������
      �ڹ��ж��İ����£���һ����ͻ�����һЩ
    ѡ�������ǩ��
      ���ɸǶٻ������ܹ��ж�
      Ȼ������Ҫ��������������ս

#Y���������������أ�#n
    �������ѡ����ʲô���㶼Ҫ#L ����@@(50900,5400,7900)#n��ս
    #G���ܻ�����I�š�������II�ż���˵�еĻ���������������#n

#Y�����ģ������Ĺأ�#n
    �������ɸǶ�#G�ػ�#L����@@(50900,19900,8500)#n#Gʹ��Ѫ������#n
      #cFF4e00������ְҵ��սʿ����ʦ��ǹ�֣�
        1������Ҫ��ɱ���й���
        2������Ỻ����Ѫ
        3������Ѫ����������3��
      #c6d00d2������ְҵ�Ǽ�˾��
        1��������Զ���Ѫ
        2��������ʹ��ʥ�����������Ѫ
        3��ˢ��ʱ���Ȼ�ɱ����
        4�����������Ѫ����3��

#Y�����壺#n
��#L���ɸǶٵ����@@(50900,20200,20700)#n�Ի��뿪�þ����������

��ϲ�㣬������һ�������ʹ��������ˣ�������ѧʶ֮��תְȥ�ɣ��ǵ�һ��Ҫѡ��ְҵӴ��

						]],
						},	
						
						
			 [415]   =  { "����BOSS���ɼƻ�-����BOSS",
			      [[
#c6f5220�������ɼƻ���BOSS-��������ڹ�������м�����Ҿ���ǰ������ɱ����÷ǳ����Ľ���Ӵ��#n	
#c6f5220BOSS�����3��Сʱδ��ɱ���Զ���ʧ��#n
#c6f5220�����ϵͳ�Զ��������ܵ�BOSS��#n
		
#c6f5220��ɱʱ��BOSS�����ı������Ա�Կɻ�ô������齱���������Զ���ù����ʽ�#n

						]],
						},	


			[1000]   =  { "ʱ�������ս",
			      [[
#cd44800����顤
		#c6f5220 ʱ�������ս�Ѽ���������ܣ��ڶ��ܵ�����20��00����ʽ������#n
		#c6f5220 ʤ�����ᣬ����Ϊʱ��ǵ����ˣ����������Ȩ���������ϸ��Ϣ��ͨ��ʱ����ڵ�����˹����ѯ��#n
#cd44800��ս������		
		#c6f5220 ս��ʱ�䣺����ս����ʱ�䣺ÿ������20��00������ս����ʱ�䣺ÿ������21��30��#n
		#c6f5220 ս����ͼ������ս�������п����ĵ�ͼ���У���ͨ��ʱ���������˹�����롣#n
		#c6f5220 ս������#n
		#c6f5220 		 1.ֻ�оݵ�ռ�칫������п����ĵ�ͼ�����Ὠ������Ȩ��#n
		#c6f5220 		 2.��ս����ͼ�У�����ռ�졢�ݻٻ�ɹ��ػ��Ƿ��������ɻ��ս�����֡�#n
		#c6f5220 ʤ��������#n
		#c6f5220 		 ��ս������ʱ�����ݸ�����ս�������ж�ʤ��������������1���ᣬ�������ʤ�������������#n
		#c6f5220 		 1.����ط��������������������ͬ������ط�ʤ����#n
		#c6f5220 		 2.�����������֮�����ƽ�֣����շ����ﵽʱ�䣬��ǰ�߻�ʤ��#n
		#c6f5220 		 ��Ҫ��ʾ�����п������У�����3���ն˿���������Ȩ�ɻ�ô���ս�����֣�#n
#cd44800��ռ��ʱ��ǹ��ά����			
		#c6f52201.ռ�칫��᳤ÿ�����ȡһ��ռ���ʽ𣨴������ң���
		2.���е�ǰ�����Ա����ʱ���NPC��������Ʒ��ά��װ����������һ���ۿ��Żݡ�
		3.���е�ǰ�����Ա����[������������]���纽�ߴ����У�ӵ�и���ĵ�ͼ���͵㡣
		4.ʱ��ǳ�����ռ�칫��᳤������ʱ������ĳ�������˵���
		5.��ǰռ�칫��Ĺ��������⻯��
		6.�ر�ע�⣺����Ϊ��Ҫ��Ȩ�뽱����������ռ��ʱ��ǵ�ʱ��Խ�ã�ӵ�е���Ȩ�뽱��Խ�ࣩ��#n]],
						},	
			[2001] = { "һ���ݵ㿪��",
			      [[
#cd44800�ݵ��飺
	1���ݵ㣺[����]��[����ѩԭ]��[����ɳĮ]�ݵ��ѿ��������й�����ڶ�Ӧ��ͼ��[�ݵ�������]ɳ�˴����о��Ĳ�����ݵ㡣
�ݵ����
	���ģ�
		����ʱ�䣺ÿ��һ�����������ģ��ڶ������繫����������Ͽ����ݵ�����ս��
		���Ļ��ѣ�ʹ�ù����ʽ���о��ġ�
		����Ŀ�ģ�����ʤ�����ᣬ����òμӾݵ�ս��Ȩ����
	ս����
		ս��ʱ�䣺ÿ�ܶ���������20��30-21��30��Ϊ�ݵ�ս����ʱ�䡣
		ս������ս���Թ���Ϊ��λ��1��1��ʽ�ڷ�յ�ս�������н��С�
		�����ڹ����ĸ���Դ����㣬ռ����Դ��󼺷�����������Դ��
		ս�������ڣ�PK�޳ͷ���������PKֵ��
	ս��������ս��ʱ���ڣ����Ȼ��20000��Դ�㹫��Ϊʤ�����ᡣ
		��ս��ʱ�����ʱ��˫�������δ���20000��Դ�㣬����Դ�������һ�����ս����ʤ����
		����ʤ������Ϊ��ǰ�ݵ��ռ�칫�ᣬ����ռ����Ȩ
		ע����˫�������Դ��ͬ�����ж����ط�����ʤ����
�ݵ㽱����
	�᳤ÿ�տ���ȡһ��ռ���ʽ������������ң�
	�����Աÿ�տ���ȡһ�ι��ṱ�׶Ƚ���
	�����Աÿ�տ���ȡһ��"��֮��ҫ"Ч������
	�ھݵ����ڵ�ͼָ���ص��������Թ������ĵȵȽ���
	ע���ݵ�ȼ�Խ�ߣ�����ȡ�Ľ���Խ���#n]],
						},
			[2002] = { "�����ݵ㿪��",
			      [[
#cd44800�ݵ��飺
	2���ݵ㣺[���ջ�Ұ]��[Խ������]�ݵ��ѿ��������й�����ڶ�Ӧ��ͼ��[�ݵ�������]ɳ�˴����о��Ĳ�����ݵ㡣
�ݵ����
	���ģ�
		����ʱ�䣺ÿ��һ�����������ģ��ڶ������繫����������Ͽ����ݵ�����ս��
		���Ļ��ѣ�ʹ�ù����ʽ���о��ġ�
		����Ŀ�ģ�����ʤ�����ᣬ����òμӾݵ�ս��Ȩ����
	ս����
		ս��ʱ�䣺ÿ�ܶ���������20��30-21��30��Ϊ�ݵ�ս����ʱ�䡣
		ս������ս���Թ���Ϊ��λ��1��1��ʽ�ڷ�յ�ս�������н��С�
		�����ڹ����ĸ���Դ����㣬ռ����Դ��󼺷�����������Դ��
		ս�������ڣ�PK�޳ͷ���������PKֵ��
	ս��������ս��ʱ���ڣ����Ȼ��20000��Դ�㹫��Ϊʤ�����ᡣ
		��ս��ʱ�����ʱ��˫�������δ���20000��Դ�㣬����Դ�������һ�����ս����ʤ����
		����ʤ������Ϊ��ǰ�ݵ��ռ�칫�ᣬ����ռ����Ȩ
		ע����˫�������Դ��ͬ�����ж����ط�����ʤ����
�ݵ㽱����
	�᳤ÿ�տ���ȡһ��ռ���ʽ������������ң�
	�����Աÿ�տ���ȡһ�ι��ṱ�׶Ƚ���
	�����Աÿ�տ���ȡһ��"��֮��ҫ"Ч������
	�ھݵ����ڵ�ͼָ���ص��������Թ������ĵȵȽ���
	ע���ݵ�ȼ�Խ�ߣ�����ȡ�Ľ���Խ���#n]],
						},
			[2003] = { "�����ݵ㿪��",
			      [[
#cd44800�ݵ��飺
	3���ݵ㣺[��ɳ�ݵ�]�ݵ��ѿ��������й�����ڶ�Ӧ��ͼ��[�ݵ�������]ɳ�˴����о��Ĳ�����ݵ㡣
�ݵ����
	���ģ�
		����ʱ�䣺ÿ��һ�����������ģ��ڶ������繫����������Ͽ����ݵ�����ս��
		���Ļ��ѣ�ʹ�ù����ʽ���о��ġ�
		����Ŀ�ģ�����ʤ�����ᣬ����òμӾݵ�ս��Ȩ����
	ս����
		ս��ʱ�䣺ÿ�ܶ���������20��30-21��30��Ϊ�ݵ�ս����ʱ�䡣
		ս������ս���Թ���Ϊ��λ��1��1��ʽ�ڷ�յ�ս�������н��С�
		�����ڹ����ĸ���Դ����㣬ռ����Դ��󼺷�����������Դ��
		ս�������ڣ�PK�޳ͷ���������PKֵ��
	ս��������ս��ʱ���ڣ����Ȼ��20000��Դ�㹫��Ϊʤ�����ᡣ
		��ս��ʱ�����ʱ��˫�������δ���20000��Դ�㣬����Դ�������һ�����ս����ʤ����
		����ʤ������Ϊ��ǰ�ݵ��ռ�칫�ᣬ����ռ����Ȩ
		ע����˫�������Դ��ͬ�����ж����ط�����ʤ����
�ݵ㽱����
	�᳤ÿ�տ���ȡһ��ռ���ʽ������������ң�
	�����Աÿ�տ���ȡһ�ι��ṱ�׶Ƚ���
	�����Աÿ�տ���ȡһ��"Ѫ֮��ҫ"Ч������
	�ھݵ����ڵ�ͼָ���ص��������Թ������ĵȵȽ���
	ע���ݵ�ȼ�Խ�ߣ�����ȡ�Ľ���Խ���#n]],
						},
			
--			-- 44���ȼ���ӡ��ʾ
--			[10044] = { "�����ӡ��44����",
--						[[
--						
--	��������ǰ�ѽ��44����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����
--						]]
--						},
--			-- 44���ȼ���ӡ��ʾ
--			[10049] = { "�����ӡ��49����",
--						[[
--						
--	��������ǰ�ѽ��49����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����
--						]]
--						},
--			-- 44���ȼ���ӡ��ʾ
--			[10054] = { "�����ӡ��54����",
--						[[
--						
--	��������ǰ�ѽ��54����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����
--						]]
--						},
--			-- 44���ȼ���ӡ��ʾ
--			[10059] = { "�����ӡ��59����",
--						[[
--						
--	��������ǰ�ѽ��59����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����
--						]]
--						},
--			-- 44���ȼ���ӡ��ʾ
--			[10064] = { "�����ӡ��64����",
--						[[
--						
--	��������ǰ�ѽ��64����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����
--						]]
--						},
--			-- 44���ȼ���ӡ��ʾ
--			[10069] = { "�����ӡ��69����",
--						[[
--						
--	��������ǰ�ѽ��69����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����
--						]]
--						},
--			-- 44���ȼ���ӡ��ʾ
--			[10074] = { "�����ӡ��74����",
--						[[
--						
--	��������ǰ�ѽ��74����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����
--						]]
--						},
--						-- 49���ȼ���ӡ��ʾ
			[10061] = { "�����ӡ��61����",
						[[
	��������ǰ�ѽ��61����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10062] = { "�����ӡ��62����",
						[[
	��������ǰ�ѽ��62����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10063] = { "�����ӡ��63����",
						[[
	��������ǰ�ѽ��63����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10064] = { "�����ӡ��64����",
						[[
	��������ǰ�ѽ��64����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10065] = { "�����ӡ��65����",
						[[
	��������ǰ�ѽ��65����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10066] = { "�����ӡ��66����",
						[[
	��������ǰ�ѽ��66����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10067] = { "�����ӡ��67����",
						[[
	��������ǰ�ѽ��67����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10068] = { "�����ӡ��68����",
						[[
	��������ǰ�ѽ��68����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10069] = { "�����ӡ��69����",
						[[
	��������ǰ�ѽ��69����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10070] = { "�����ӡ��70����",
						[[
	��������ǰ�ѽ��70����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10071] = { "�����ӡ��71����",
						[[
	��������ǰ�ѽ��71����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10072] = { "�����ӡ��72����",
						[[
	��������ǰ�ѽ��72����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10073] = { "�����ӡ��73����",
						[[
	��������ǰ�ѽ��73����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10074] = { "�����ӡ��74����",
						[[
	��������ǰ�ѽ��74����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10075] = { "�����ӡ��75����",
						[[
	��������ǰ�ѽ��75����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10076] = { "�����ӡ��76����",
						[[
	��������ǰ�ѽ��76����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10077] = { "�����ӡ��77����",
						[[
	��������ǰ�ѽ��77����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10078] = { "�����ӡ��78����",
						[[
	��������ǰ�ѽ��78����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
			[10079] = { "�����ӡ��79����",
						[[
	��������ǰ�ѽ��79����ӡ������ǿ��Լ��������ȼ���������ࡢ���ḻ����Ϸ����

						]]
						},
--
}

function SetMessageTips( nIdx )
	if t_Message[ nIdx ] ~= nil then
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_XINSHOW_BTN_MSG, frame = "LevelUpTipFrame", 
												path =t_Message[nIdx].path ~= nil and t_Message[nIdx].path or "uires\\TuPianLei\\PiaoYiTiShiTuBiao\\!.tga", index = nIdx } );
	end
end

function CloseMessageTips( nIdx )
	if t_Message[ nIdx ] ~= nil then
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		t_centerXinShouBtnControl:eraseMsgBtnByIndex( { itemDefId = 0, Type = CENTER_XINSHOW_BTN_MSG, frame = "LevelUpTipFrame", 
												path =t_Message[nIdx].path ~= nil and t_Message[nIdx].path or "uires\\TuPianLei\\PiaoYiTiShiTuBiao\\!.tga", index = nIdx } );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
end
     
function ShowLevelUpMsgTip( nIdx )
	if t_Message[ nIdx ] ~= nil then
		LevelUpTipFrameFirstLineFont:SetText( t_Message[nIdx][1] );
		local rich = getglobal("LevelUpTipFrameSecondRich");
		local slider  = getglobal( "LevelUpTipFrameScrollBar" );  
		local sliderThumbTex = getglobal( "LevelUpTipFrameScrollBar".."ThumbTexture" );
		rich:SetLinkTextColor(10,110,0);
		rich:SetText( t_Message[nIdx][2],111,82,32 );
		local nViewLine	= rich:GetViewLines();
		local nAllLine	= rich:GetTextLines();
		rich:SetDispPos(rich:GetStartDispPos());
		slider:SetValue(0);
		slider:Hide();
		sliderThumbTex:Hide();
		if nAllLine - nViewLine > 0 then
			local nMaxValue = nAllLine - nViewLine; 
			slider:Show();
			slider:SetMaxValue( nMaxValue );
			slider:SetMinValue(0);
			slider:SetValueStep(1);
		end
		LevelUpTipFrame:Show();
	end
end

function WolrdCityBattleOpen()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_XINSHOW_BTN_MSG, frame = "LevelUpTipFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\!.tga", index = 1000 } )
end

local isClosed = true;

function SetClanOpenTouchTips()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_OPEN_TOUCH ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_CLAN_OPEN_TOUCH, frame = "ClanOpenTouchTipFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\12.tga", index = 0 } );
	end
	isClosed = false;
end

function ClanOpenTouchGameEnd()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_OPEN_TOUCH );	
	if nIdx > 0 then
		t_centerXinShouBtnControl:eraseByIndex(  nIdx );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
	if ClanOpenTouchTipFrame:IsShown() then
		ClanOpenTouchTipFrame:Hide();
	end
	isClosed = true;
end

local szClanOpenTouchAwaardDesc = [[
					
#c461e0aÿ�������������飺%d

#c461e0a�ڱ��м��ʻ�ã�
]]

function ClanOpenTouchTipFrame_OnLoad()
	local szText = [[
					
#c5546231������̽����Ѱ�ұ���
2��ע��̽������ʾ��Ϣ���ֱ���
3�����ֱ���ʱ�����ھ�����̽����8�װ뾶��Χ���ڱ�
4���ڳ��������������������
5��̽�������������




	1����������֯��Ա����������Խ�࣬�����Ķ��⽱��Խ��
	2��ע�Ᵽ��̽�����İ�ȫ����������ػ��ߺ�������Ա���ƻ�#n]]

	InfoRich = getglobal( "ClanOpenTouchTipFrameSecondRichRole" )
	InfoRich:SetText( szText,111,82,32 );

	szText = string.format( szClanOpenTouchAwaardDesc,0 );
	InfoRich = getglobal( "ClanOpenTouchTipFrameSecondRichGift" )
	InfoRich:SetText( szText,111,82,32 );

	local tAwardItem = { 5233000,5233500,5050510,5060014,5510002 }
	for i=1,5 do
		local itemDef = getItemDef( tAwardItem[i] );
		local btn = getglobal( "ClanOpenTouchTipFrameAwardItemBtn" .. i );
		local ItemBackTex = getglobal( btn:GetName().."BackTexture");
		local ItemBoxTex = getglobal( btn:GetName().."BoxTexture");
		local UVAnimationTex = getglobal( btn:GetName().."UVAnimationTex");
		local ItemTex = getglobal( btn:GetName().."IconTexture");
		ItemBackTex:Show();
		ItemBoxTex:Show();
		local IconPath = GetItemEquipIconPath()..itemDef.IconID..".tga";
		if not IsInExistence( IconPath ) then
			IconPath = DEFAULT_ITEM_PATH;
		end
		ItemTex:SetTexture( IconPath );
		SetEquipImportantEffectUV( itemDef, UVAnimationTex:GetName() );
		btn:SetClientUserData( 0,tAwardItem[i] )
	end

	this:RegisterEvent( "GE_ENTER_MAP" );
end

function ShowClanOpenTouchTipFrame()		
	ClanOpenTouchTipFrame:Show();
end

function ClanOpenTouchTipFrame_OnShow()
	local sText;
	if not isClosed then
	sText = [[
					
#c461e0a����ƣ�����#c554623(����)
#c461e0a�NPC����������#c554623(�������)
#c461e0a���ͼ���������
#c461e0a����ȼ���������
#c461e0a�״̬��#n   #cffffbe������#n]];
	ClanOpenTouchTipFrameRunning:SetGray( false );
	ClanOpenTouchTipFrameGoBtn:Enable();
	else
	sText = [[
					
#c461e0a����ƣ�����#c554623(����)
#c461e0a�NPC����������#c554623(�������)
#c461e0a���ͼ���������
#c461e0a����ȼ���������
#c461e0a�״̬��#n   #cffffbeδ����#n]];
	ClanOpenTouchTipFrameRunning:SetGray( true );
	ClanOpenTouchTipFrameGoBtn:Disable();
	--#L��������@@[304056,60000]
	end
	local InfoRich = getglobal( "ClanOpenTouchTipFrameSecondRichInfo" );
	InfoRich:SetText( sText,111,82,32 );

	this:SetPoint( "center", "UIClient", "center", 0, 0 );
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer == nil then return end
	local levelupdef = getLevelUP( mainplayer:getLv() );
	local nExp = math.floor( levelupdef.ContributeExp / 100 )
	szText = string.format( szClanOpenTouchAwaardDesc,nExp );
	InfoRich = getglobal( "ClanOpenTouchTipFrameSecondRichGift" )
	InfoRich:SetText( szText,111,82,32 );	
end

function ClanOpenTouchTipFrame_OnHide()
end

function ClanOpenTouchTipFrameAwardItemBtn_OnEnter()
	local itemId = this:GetClientUserData( 0 )
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, getItemDef(itemId) )
end

local bClanCityMapAutoMove = false;
function ClanOpenTouchTipFrameGoBtn_OnClick()
	
	local ClanViewer = GameClanManager:getClanViewer();
	if MapShower:getRealMapId() == CLAN_CITY_MAP_ID and GameClanManager:getCurClanCityId() == ClanViewer.m_clanInfo.m_uClanId then
		-- �߹�ȥ
		MapShower:GetMaxMapContal():linkMoveto( MapShower:getRealMapId(), 12700, 29200 );
	else		
		local nCurMapID;
		local MaxMapControl	= MapShower:GetMaxMapContal();
		if MaxMapControl:getMapID() == 0 then
			nCurMapID = MapShower:getRealMapId();
		else
			nCurMapID = MaxMapControl:getMapID();
		end
		if nCurMapID >= MIN_COPY_ID then
			ShowMidTips( "��ǰ�������ܴ���" );
			return;
		end
		 --����
		 GameClanManager:clan_Opt_EnterCity( ClanViewer.m_clanInfo.m_uClanId )
		 bClanCityMapAutoMove = true;
	end
end

function ClanOpenTouchTipFrame_OnEvent()
	if arg1 == "GE_ENTER_MAP" then
		if bClanCityMapAutoMove then
			bClanCityMapAutoMove = false;
			MapShower:GetMaxMapContal():linkMoveto( MapShower:getRealMapId(), 12700, 29200 );
		end
	end
end
-------------------------------------------------����-�鱨����ս--------------------------------------------
local IntellingenceInfo = {
	[1] = [[#c461e0a	����ƣ��鱨����ս#c554623(����)
#c461e0a	�NPC���鱨��#c554623(�������)
#c461e0a	���ͼ���������
#c461e0a	����ȼ���20��]],
	[2] = [[#c461e0a	��������ֵ������	
#c461e0a	�������ṱ�ס�������]],
	[3] = [[#c461e0a	���ȡ�鱨����	
#c461e0a	1.ǰ��Ŀ�깫����У����鱨�����ϻ�ȡ3���鱨��
#c461e0a	2.��ɱ�������ص���ң�Ҳ�л����ȡ�鱨��
#c461e0a	3.������������Ѿ��ռ����鱨����ʱ��Ǹ�������Ҳ����ʧ�ܡ�

#c461e0a	���鱨��������
#c461e0a	1.�����������Ա�����ж��5�ݶ�ʧ���鱨��
#c461e0a	2.������������һ�ɱ��������Ѿ��ռ����鱨��
#c461e0a	3.��ɱǰ����ȡ�鱨����ҿ��Ի�ý�����]],
}
local MAX_CITY_CHOICE_NUM = 8;
local MAX_CITYPOS_CHOICE_NUM = 8;
function setClanSPYTips()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_SPY ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_CLAN_SPY, frame = "ClanIntellingenceFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\12.tga", index = 0 } );
	end
end
function ClanIntellingenceFrame_onShow()
	ClanIntellingenceFrameSecondRichInfo:SetText( IntellingenceInfo[1], 255, 255, 255 );
	ClanIntellingenceFrameSecondRichGift:SetText( IntellingenceInfo[2], 255, 255, 255 );
	ClanIntellingenceFrameSecondRichRole:SetText( IntellingenceInfo[3], 255, 255, 255 );
end
function ClanIntellingenceFrameEnterBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer();
	if mainplayer:getLv() < CLAN_SPY_LEVEL_LIMIT_MIN then
		ShowMidTips( "����ȼ�δ�ﵽ�Ҫ����ʱ�޷��μ�" );
		return;
	end
	local nCurMapID;
	local MaxMapControl	= MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() == 0 then
		nCurMapID = MapShower:getRealMapId();
	else
		nCurMapID = MaxMapControl:getMapID();
	end
	if nCurMapID >= MIN_COPY_ID then
		ShowMidTips( "����ǰ���ڸ����л����ڹ������,�޷�����" );
		return;
	end
	GameClanManager:Clan_Opt_GoToClanIntellingenceNPC();
end	
--�������ѡ�����
local t_AvailableCityList = {
	[1] = {},
	[2] = {},
	[3] = {},
	[4] = {},
	[5] = {},
	[6] = {},
	[7] = {},
	[8] = {},
}
local SPY_MAX_CLAN_CITY = 8;
local Chose_City_ID = 0;
--����ص�ѡ�����
local SPY_MAX_CLAN_POS = 8;
local Chose_Pos_ID = 0;
local CLAN_SPY_TASK_ID = 3681;
function ChoseBtn_onClick()
	if this:GetParent() == "ChoseClanCityFrame" then
		for i = 1, MAX_CITY_CHOICE_NUM do
			local btn = getglobal( this:GetParent().."Choice"..i )
			btn:DisChecked();
		end
		Chose_City_ID = this:GetClientUserData( 0 );
	elseif this:GetParent() == "ChoseCityPosFrame" then
		for i = 1, MAX_CITYPOS_CHOICE_NUM do
			local btn = getglobal( this:GetParent().."Choice"..i )
			btn:DisChecked();
		end
		Chose_Pos_ID = this:GetClientID();
	end
	this:Checked();
end
function OpenChoseClanCityFrame( npcId )
	ChoseClanCityFrame:SetClientUserData( 0, npcId );
	ChoseClanCityFrame:Show();
end
function ChoseClanCityFrame_OnShow()
	if ChoseCityPosFrame:IsShown() then
		ChoseCityPosFrame:Hide();
	end
	local npcid = this:GetClientUserData( 0 );
	util.testNpcDialogDistance( npcid, ChoseClanCityFrame );
	for i = 1 , SPY_MAX_CLAN_CITY do
		local btn = getglobal( "ChoseClanCityFrameChoice"..i );
		btn:SetClientUserData( 0, 0 );
		btn:DisChecked();
		local btnShow = getglobal( btn:GetName().."Text" );
		btnShow:SetText( "" );
	end
	local nClanTotalNum	= GameClanManager:getClanTotalNum();
	--���ÿ�Ǳ��Ĺ����б�
	local t_CanAttackClanList = {};
	local SetClanNum = 0;
	while nClanTotalNum > 0 do
		local OneClanInfo = GameClanManager:getOneClanInfoByIndex( nClanTotalNum - 1 );
		local clanInfo = OneClanInfo.m_ClanInfo;
		if clanInfo.m_Flag % 2 == 1 then
			local MyClanViewer = GameClanManager:getClanViewer();
			local MyClanInfo = MyClanViewer.m_clanInfo;
			if MyClanInfo.m_uClanId ~= clanInfo.m_uClanId then
				SetClanNum = SetClanNum + 1;
				table.insert( t_CanAttackClanList, clanInfo );
				if SetClanNum > 8 then return end
			end
		end
		nClanTotalNum = nClanTotalNum -1;
	end
	--���շ��ٶ�Ϊ�������򣬴Ӹߵ���
	table.sort( t_CanAttackClanList, function ( a, b )
				return a.m_nActiveValue > b.m_nActiveValue;
	end )
	for i = 1 , SetClanNum do
		local clanInfo = t_CanAttackClanList[i];
		local btn = getglobal( "ChoseClanCityFrameChoice"..i );
		btn:SetClientUserData( 0, clanInfo.m_uClanId );
		local btnShow = getglobal( btn:GetName().."Text" );
		btnShow:SetText( clanInfo.m_szClanName );
	end
	--Ĭ��ѡ���һ��
	local btn = getglobal( "ChoseClanCityFrameChoice"..1 )
	btn:Checked();
	Chose_City_ID = btn:GetClientUserData( 0 );
end
function ChoseClanCityFrame_OnUpdate()
	if not Quest:isQuestRun( CLAN_SPY_TASK_ID ) then
		this:Hide();
	end
end
function ChoseClanCityFrameOKBtn_onClick()
	if Chose_City_ID > 0 then
		ChoseClanCityFrame:Hide();
		ChoseCityPosFrame:Show();
	else
		ShowMidTips( "��ѡ�񹫻����" );
	end
end
function ChoseClanCityFrameCancleBtn_onClick()
	ChoseClanCityFrame:Hide();
end
function ChoseCityPosFrame_OnShow()
	if ChoseClanCityFrame:IsShown() then
		ChoseClanCityFrame:Hide();
	end
	local npcid = ChoseClanCityFrame:GetClientUserData( 0 );
	util.testNpcDialogDistance( npcid, ChoseCityPosFrame );
	Chose_Pos_ID = 0;
	for i = 1, SPY_MAX_CLAN_POS do
		local btn = getglobal( this:GetName().."Choice"..i );	
		btn:DisChecked();
		local ClanSpyTransDef = GameClanManager:getClanSpyTransDef( i );
		local btnShow = getglobal( this:GetName().."Choice"..i.."Text" );
		btnShow:SetText( ClanSpyTransDef.Name );
	end
	local btn = getglobal( this:GetName().."Choice1" )
	btn:Checked();
	Chose_Pos_ID = btn:GetClientID();
end
function ChoseCityPosFrame_OnUpdate()
	if not Quest:isQuestRun( CLAN_SPY_TASK_ID ) then
		this:Hide();
	end
end
function ChoseCityPosFrameOKBtn_onClick()
	if Chose_Pos_ID > 0 and Chose_Pos_ID < 9 then
		ChoseCityPosFrame:Hide();
		GameClanManager:Clan_Opt_GoToClanCityPos( Chose_City_ID, Chose_Pos_ID );
	else
		ShowMidTips( "��ѡ�񹫻���еص�" );
	end
end
function ChoseCityPosFrameCancleBtn_onClick()
	ChoseCityPosFrame:Hide();
end
-----------------------------------------------����-�ػ�������Ŧ----------------------------------------------------
local ClanMachinePworldInfo = {
	[1] = [[#c461e0a	����ƣ��ػ�������Ŧ#c554623(����)
#c461e0a	�NPC������˹����#L#c00a400(�������)@@[303006,1000]#n#c554623
#c461e0a	���ͼ��������Ŧ����
#c461e0a	����ģʽ��%s���Ὺ��]],
	[2] = [[#c461e0a	ÿ������#G(������Ĺ���ɻ��)#n
	
	
	
#c461e0a	�б�����



#c00a400	����ɱ�б������˿ɻ�þ���ֵ��]],
	[3] = [[#c461e0a	��ɹ��ػ�������з�30��Ϯ��
#c461e0a	���ػ�ʧ�ܣ�������Ŧ���з��ݻ�

#c461e0a	����ͨģʽ���ѷ������Ա�ɲμ�
#c461e0a	������ģʽ�����й����Ա�ɲμ�

#c461e0a	ȡʤ���ţ�
#c461e0a	1.ʹ��������е�ɶԵз���ɿɹ��˺�
#c461e0a	2.[������Ŧ����]�ṩ�ڵ�����
#c461e0a	3.[������Ŧ����]�ṩ��еǿ��
#c461e0a	4.[������Ŧ����]�ṩ������ά��





#c0000ff	������ʾ��
#c0000ff			����ֻ�ܲμ�1��[�ػ�������Ŧ]
]],
}
--��ʱС������ݼ�¼
local newStarTime = {}
local CurWaveNum = {}
local CurWaveTimer = {}
local CurRaidTimer = {}

function SetClanMachinePworld()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if t_centerXinShouBtnControl:getIndexByType( CENTER_CLAN_MACHINE_PWORLD ) == 0 then
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_CLAN_MACHINE_PWORLD, frame = "ClanMachinePworldFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\12.tga", index = 0 } );
	end
end
function ClanMachinePworldFrame_OnShow()
	local mainplayer = ActorMgr:getMainPlayer()
	if not mainplayer then return end;
	local clanName = "����"
	if mainplayer:getUnionMachinePworldClanID() ~= 0 then
		clanName = "#c00a400"..GameClanManager:clanGetClanNameByID( mainplayer:getUnionMachinePworldClanID() ).."#n#c461e0a"
	end
	local szInfo = string.format( ClanMachinePworldInfo[1], clanName )
	ClanMachinePworldFrameSecondRichInfo:SetText( szInfo, 255, 255, 255 );
	ClanMachinePworldFrameSecondRichGift:SetText( ClanMachinePworldInfo[2], 255, 255, 255 );
	ClanMachinePworldFrameSecondRichRole:SetText( ClanMachinePworldInfo[3], 255, 255, 255 );
end
function SetClanMachineBtnState()
	--�������Լ�������ػ�������������Ƿ���������ģʽ
	local mainplayer = ActorMgr:getMainPlayer()
	--�й��Ὺ�����ػ���Ŧ-��ͨģʽ�������Ӧ��ť
	if not ClanMachinePworldFrameEnterBtn:IsEnable() and mainplayer:getNormalMachinePworldClanID() ~= 0 then
		ClanMachinePworldFrameEnterBtn:Enable()
	end
	if ClanMachinePworldFrameEnterBtn:IsEnable() and mainplayer:getNormalMachinePworldClanID() == 0 then
		ClanMachinePworldFrameEnterBtn:Disable()
	end
	--�й��Ὺ�����ػ���Ŧ-����ģʽ�������Ӧ��ť
	if not ClanMachinePworldFrameEnterUnionBtn:IsEnable() and mainplayer:getUnionMachinePworldClanID() ~= 0 then
		ClanMachinePworldFrameEnterUnionBtn:Enable()
	end
	if ClanMachinePworldFrameEnterUnionBtn:IsEnable() and mainplayer:getUnionMachinePworldClanID() == 0 then
		ClanMachinePworldFrameEnterUnionBtn:Disable()
	end
end
function ClanMachinePworldFrame_OnUpdate()
	local mainplayer = ActorMgr:getMainPlayer()
	local clanName = "����"
	if mainplayer:getUnionMachinePworldClanID() ~= 0 then
		clanName = "#c00a400"..GameClanManager:clanGetClanNameByID( mainplayer:getUnionMachinePworldClanID() ).."#n#c461e0a"
	end
	local szInfo = string.format( ClanMachinePworldInfo[1], clanName )
	ClanMachinePworldFrameSecondRichInfo:SetText( szInfo, 255, 255, 255 );
end
--��Ϊ�µ�����ģʽ�������ϵ���ʾ��Ϣ��ʹ�������ĺ�������ʾ����
function ClanMachinePworldTimeShow( nPworldId )
	local MainPlayer = ActorMgr:getMainPlayer()
	if MainPlayer and MainPlayer:getMapRealId() == nPworldId * 100 then
		ClanMachinePworldTimeShowFrameNormalAttack:SetText( "��"..CurWaveNum[nPworldId].."����"..CurWaveTimer[nPworldId].."��" );
		ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "��Ϯ��"..CurRaidTimer[nPworldId].."��" );
	end
	--ClanMachinePworldFrameWavesInfo:SetText("�з�Ϯ����"..CurWaveNum[nPworldId].."/30��");
end
--��Ϊ�µ�����ģʽ�������ϵ���ʾ��Ϣʹ�ñ�������ʾ����
function SetClanMachineWavesInfo( nflag, nWave )
	--����ģʽ
	if nflag == 1 then
		ClanMachinePworldFrameUnionWavesInfo:SetText("����ģʽ��"..nWave.."/30��");
	else
		ClanMachinePworldFrameWavesInfo:SetText("��ͨģʽ��"..nWave.."/30��");
	end
end
function ClanMachinePworldFrameEnterBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getNormalMachinePworldClanID() == 0 then
		ShowMidTips( "������δ�������ػ�������Ŧ-��ͨģʽ������ʱ�޷��μ�" )
		return
	end
	local nCurMapID;
	local MaxMapControl	= MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() == 0 then
		nCurMapID = MapShower:getRealMapId();
	else
		nCurMapID = MaxMapControl:getMapID();
	end
	if nCurMapID >= MIN_COPY_ID then
		ShowMidTips( "����ǰ���ڸ����л����ڹ������,�޷�����" );
		return;
	end
	GameClanManager:Clan_Opt_EnterClanMachinePworld( mainplayer:getClanID() );
end
function ClanMachinePworldFrameEnterUnionBtn_OnClick()
	local mainplayer = ActorMgr:getMainPlayer()
	if mainplayer:getUnionMachinePworldClanID() == 0 then
		ShowMidTips( "��ǰ�޹��Ὺ�����ػ�������Ŧ-����ģʽ������ʱ�޷��μ�" )
		return
	end
	local nCurMapID;
	local MaxMapControl	= MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() == 0 then
		nCurMapID = MapShower:getRealMapId();
	else
		nCurMapID = MaxMapControl:getMapID();
	end
	if nCurMapID >= MIN_COPY_ID then
		ShowMidTips( "����ǰ���ڸ����л����ڹ������,�޷�����" );
		return;
	end
	GameClanManager:Clan_Opt_EnterClanMachinePworld( mainplayer:getUnionMachinePworldClanID() );
end
local t_ClanMachinePworldAwardItemIDList = { 5510003, 5510006, 5510004, 5511046, 5234000, 5051101, 5510000, }
function ClanMachinePworldFrameAwardItemBtn_OnShow()
	local itemId				= t_ClanMachinePworldAwardItemIDList[this:GetClientID()]
	if itemId == nil then return end
	local itemdef				= getItemDef( itemId );
	if itemdef == nil then return end
	local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
	-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	local IconTexture = getglobal( this:GetName().."IconTexture" )
	IconTexture:SetTexture(IconPath)
end
function ClanMachinePworldFrameAwardItemBtn_OnEnter()
	local itemId				= t_ClanMachinePworldAwardItemIDList[this:GetClientID()]
	if itemId == nil then return end
	local itemdef				= getItemDef( itemId );
	if itemdef == nil then return end
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemdef )
end
---------------------------------------------�ػ�������Ŧ-��ʱС���--------------------------------------------------
local CLAN_MACHINE_PWORLD_MAP_ID = 60200 --������Ŧ��ͼID
local THE_DREAM_PWORLD_MAP_ID = 21900 --���οռ��ͼID
local ITERATION_PWORLD_MAP_ID = 25000 --���ֻ����ID
local nIterationType = 0;
local nIterationNum = 0;
function ClanMachinePworldTimeShowFrame_OnUpdate()
	local MainPlayer = ActorMgr:getMainPlayer()
	local idx = MainPlayer and MainPlayer:getMapRealId() / 100 or 0;
	if idx == 0 or newStarTime[idx] == nil then return end;

	if getServerTime().sec - newStarTime[idx] >= 1 then
		CurWaveTimer[idx] = CurWaveTimer[idx] - 1
		if CurWaveTimer[idx] < 0 then
			CurWaveTimer[idx] = 0;
		end
		CurRaidTimer[idx] = CurRaidTimer[idx] - 1
		if CurRaidTimer[idx] < 0 then
			CurRaidTimer[idx] = 0;
		end
		if idx == ITERATION_PWORLD_MAP_ID / 100 then
			if nIterationType ~= 2 then
				ClanMachinePworldTimeShowFrameNormalAttack:SetText( "��"..nIterationNum.."�⣺"..CurWaveTimer[idx].."��" );
				if nIterationType == 0 then
					ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "����׼���С�����" );
				elseif nIterationType == 1 then
					ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "����⡤����" );
				end
			end
		else
			ClanMachinePworldTimeShowFrameNormalAttack:SetText( "��"..CurWaveNum[idx].."����"..CurWaveTimer[idx].."��" );
			if idx == CLAN_MACHINE_PWORLD_MAP_ID / 100 then
				ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "��Ϯ��"..CurRaidTimer[idx].."��" );
			elseif idx == THE_DREAM_PWORLD_MAP_ID /100 then
				ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "������Ϯ������" );
			end
		end
		newStarTime[idx] = getServerTime().sec;
	end
end
------------------------------------------------�ݵ�ս�·�Ư������-----------------------------------------------------
function StrongPointActive( lv )
	local nIdx = 2000 + lv;
	if t_Message[ nIdx ] ~= nil then
		local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
		t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CENTER_XINSHOW_BTN_MSG, frame = "LevelUpTipFrame", 
												path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\!.tga", index = nIdx } );
	end
end
--------------------------------------------------���οռ�--------------------------------------------------------------
function TheDreamTimeShow( nPworldId )
	local MainPlayer = ActorMgr:getMainPlayer()
	if MainPlayer and MainPlayer:getMapRealId() == nPworldId * 100 then
		ClanMachinePworldTimeShowFrameNormalAttack:SetText( "��"..CurWaveNum[nPworldId].."����"..CurWaveTimer[nPworldId].."��" );
		ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "������Ϯ������" );
	end
end
--------------------------------------------------���ֻ--------------------------------------------------------------
local isStarted = false;
function isIterationStarted()
	return isStarted;
end

function setIterationStarted(bStart)
	isStarted = bStart;
end

function UpdateIterationTimeShow( type, leftTime, num )
	local MainPlayer = ActorMgr:getMainPlayer()
	local index = MainPlayer and MainPlayer:getMapRealId() / 100 or 0;
	nIterationType = type;
	nIterationNum = num;
	CurWaveTimer[index] = leftTime;
	CurRaidTimer[index] = CurWaveTimer[index];
	newStarTime[index] = getServerTime().sec;
	if MainPlayer then --and MainPlayer:getMapRealId() == nPworldId * 100 then
		if type ~= 2 then
			ClanMachinePworldTimeShowFrameNormalAttack:SetText( "��"..num.."�⣺"..CurWaveTimer[index].."��" );
			if type == 0 then
				ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "����׼���С�����" );
			elseif type == 1 then
				ClanMachinePworldTimeShowFrameRaiderAattack:SetText( "����⡤����" );
			end
		end
	end
	isStarted = true
end

function UpdatePworldWaveInfo( nWaveNum, nWaveTimer, nRaidTimer, nPworldId )
	if nWaveNum <= 0 or nWaveTimer < 0 then return end
	CurWaveNum[nPworldId] = nWaveNum
	CurWaveTimer[nPworldId] = nWaveTimer
	CurRaidTimer[nPworldId] = nRaidTimer >= 0 and nRaidTimer or 0
	newStarTime[nPworldId] = getServerTime().sec;
	if nPworldId == CLAN_MACHINE_PWORLD_MAP_ID / 100 then
		ClanMachinePworldTimeShow( nPworldId );
	elseif nPworldId == THE_DREAM_PWORLD_MAP_ID /100 then
		TheDreamTimeShow( nPworldId );
	end
end

------------------------------------------------�������� ����-----------------------------------------------------------
local t_ClanDartDeliveryInfo = {
					[1] = [[
#c461e0a����ƣ���������#c554623(����)
#c461e0a�NPC��#L����@@[302041,1000]#n#c554623(ʱ���)
#c461e0a����ȼ�����55��]],
					[2] = [[
					
#c461e0a����ɡ�#c0a6400���ͻ���#n#c461e0a�����񣬿ɶ����ô�������ֵ

#c461e0a�����͵Ļ�������Խ�ߣ���õĽ���Խ��


#c0a6400��ܰ��ʾ��
#c0a6400		ֻҪ�ڻ�ڼ��ȡ�����ͻ��������񣬻
#c0a6400���������ͬ���ɻ�û������
						]],
					 			}
local AWARD_ITEM_ID = 5510000
local nDartTime 	= 0;			--�����ʱ��
local CLAN_DART_ACTIVE_ID = 55
function SetClanDartInfo( nEndTime )
	nDartTime = nEndTime;
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	if nEndTime > 0 then 	
		if t_centerXinShouBtnControl:getIndexByType( CLAN_DART_DELIVERY ) == 0 then
			t_centerXinShouBtnControl:ShowExtraBtn( { itemDefId = 0, Type = CLAN_DART_DELIVERY, frame = "ClanDartDeliveryFrame", 
													path ="uires\\TuPianLei\\PiaoYiTiShiTuBiao\\12.tga", index = 0,nActiveID = CLAN_DART_ACTIVE_ID } );
		end
	--	t_centerXinShouBtnControl:updateCenterBtns();
	else 
		CloseCenterClanDartButton();     -- �ر�����������ť
	end
end
-- �ر�����������ť
function CloseCenterClanDartButton()
	local t_centerXinShouBtnControl = GetCenterXinShouBtnControl();
	local nIdx = t_centerXinShouBtnControl:getIndexByType( CLAN_DART_DELIVERY );
	if nIdx > 0 then	
		t_centerXinShouBtnControl:eraseByIndex( nIdx );
		t_centerXinShouBtnControl:updateCenterBtns();
	end
	if ClanDartDeliveryFrame:IsShown() then 
		ClanDartDeliveryFrame:Hide();
	end
end

function ClanDartDeliveryFrame_OnShow()
	local curTime 	= getServerTime().sec;
	local nTme		= nDartTime - curTime; 
	local font		= getglobal("ClanDartDeliveryFrameEndTimeFont");
	
	if nTme > 0 then 
		local timeFont = SetDartEndTimeText( nTme );
		font:SetText( "ʣ��ʱ�䣺"..timeFont );
	else 	
		font:SetText( "ʣ��ʱ�䣺00:00" );
	end
end

function SetDartEndTimeText( nTime )
	local szText = "";
	local hour 		=  math.floor( nTime / 3600 );
	local minute 	=  math.floor( nTime / 60 ) ;
	local second 	= nTime;
	if hour > 0 then	
		minute = math.floor( ( nTime - hour * 3600 )/60 );
		second = nTime - hour * 3600 - minute* 60;
		local hourFont = SetClockNum( hour );
		local minFont  = SetClockNum( minute ); 
		local secFont  = SetClockNum( second );	
		szText = szText..hourFont..":"..minFont..":"..secFont;
	elseif minute > 0 then 
		second = nTime - minute* 60;
		local minFont  = SetClockNum( minute ); 
		local secFont  = SetClockNum( second );	
		szText = szText..minFont..":"..secFont;
	else 
		local secFont  = SetClockNum( second );	
		szText = szText.."00:"..secFont;
	end
	return szText;
end

function ClanDartDeliveryFrame_OnLoad()
	this:setUpdateTime(1);
	this:RegisterEvent("GE_CLAN_DISABLEFRAME");
	ClanDartDeliveryFrameLeftRichInfo:SetLinkTextColor( 10, 100, 0);
	ClanDartDeliveryFrameLeftRichInfo:SetText( t_ClanDartDeliveryInfo[1],255,255,255 )
	ClanDartDeliveryFrameRightRichInfo:SetText( t_ClanDartDeliveryInfo[2],255,255,255 )
	local itemdef = getItemDef( AWARD_ITEM_ID );
	local IconPath = GetItemEquipIconPath()..itemdef.IconID..".tga";
	-- ���û�ҵ���Ʒ��ͼ����ʹ��Ĭ��ͼ����ʾ
	if not IsInExistence( IconPath ) then
		IconPath = DEFAULT_ITEM_PATH;
	end
	ClanDartDeliveryFrameAwardItemBtnIconTexture:SetTexture(IconPath)
end

function ClanDartDeliveryFrame_OnEvent()
	if arg1 == "GE_CLAN_DISABLEFRAME" then
		CloseCenterClanDartButton();
		nDartTime = 0;
	end
end

function ClanDartDeliveryFrame_OnUpdate()
	local curTime 	= getServerTime().sec;
	local nTme		= nDartTime - curTime; 
	local font		= getglobal("ClanDartDeliveryFrameEndTimeFont");
	if nTme > 0 then 
		local timeFont = SetDartEndTimeText( nTme );
		font:SetText( "ʣ��ʱ�䣺"..timeFont );
	else 	
		font:SetText( "ʣ��ʱ�䣺00:00" );
	end
end

-- ���ϲμ�
function ClanDartDeliveryFrameOkBtn_OnClick()
	if CanNotTransportOtherMap() then 
		return;
	end
	
	local frame = getglobal("ClanDartDeliveryFrame");
	GamePromotion:RequestTransportToNPC( frame:GetClientUserData( 0 ) );
	if ClanDartDeliveryFrame:IsShown() then 
		ClanDartDeliveryFrame:Hide();
	end
--	CloseCenterClanDartButton();
end
function CanNotTransportOtherMap()
	local mainPlayer = ActorMgr:getMainPlayer();
	if  mainPlayer == nil then
		return true;
	end
	if mainPlayer:isInStall() then
		ShowMidTips("��̯״̬�У���ʱ�޷����ͣ�");
		return true;
	end
	if mainPlayer:isInFight() then
		ShowMidTips("ս��״̬�У���ʱ�޷����ͣ�");
		return true;
	end
	if TradeFrame:IsShown() then
		ShowMidTips( "����״̬�У���ʱ�޷�����" );
		return true;
	end
	if mainPlayer:isDead() then 
		ShowMidTips( "����״̬�У���ʱ�޷�����" );
		return true;
	end 
	local 	rider		= mainPlayer:getRider();
	local	riderId		= rider:getRideItem().RideID;
	local	riderLv		= rider:getRideItem().nLevel;
	local	rideDef 	= getRideDef( riderId, riderLv );
	if mainPlayer:isInRide() and rideDef ~= nil and rideDef.RideOnNum >= 1 then
		ShowMidTips("��������״̬�£���ʱ�޷����ͣ�");
		return true;
	end
	if mainPlayer:isOnMonsterMachine() then 
		ShowMidTips("ʹ�û�е�У���ʱ�޷�����");
		return true;
	end
	local nCurMapID;
	local MaxMapControl	= MapShower:GetMaxMapContal();
	if MaxMapControl:getMapID() == 0 then
		nCurMapID = MapShower:getRealMapId();
	else
		nCurMapID = MaxMapControl:getMapID();
	end
	if nCurMapID >= MIN_COPY_ID then
		ShowMidTips( "��ǰ�������ܴ���" );
		return true;
	end
	return false;
end
function ClanDartDeliveryFrameAwardItemBtn_OnEnter()
	local itemdef = getItemDef( AWARD_ITEM_ID );
	if itemdef == nil then return end
	ShowItemTips( LUA_ITEMTIPS_TYPE_NORMAL, LUA_ITEMTIPS_ITEMDEF, itemdef )
end