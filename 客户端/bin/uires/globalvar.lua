UIGV=
{
	BUFF_MAX_DISPLAY = 16,
	DEBUFF_MAX_DISPLAY = 8,
	MONSTER_DEBUFF_MAX_DISPLAY = 8,
	
	item1num = 5,
	item2num = 20,
	HP_NUM = 0.2,
	
	bagopen = false,
	characteropen = false,
	XPRate = 0,
	XPReset = false,

	SkillPanelOpen = false;

	ItemTipColor	= {
		r	= 255,
		g	= 255,
		b	=255
	};

	ChatFrameOpen = true;
	QuestOpen = true;
	FileOpen = true;

	ScrollFrameMaxValueSet = false;

	HiLightColor = { r=204; g=204; b=204;};

	--任务相关
	FOR_TASK_ACTOR_IDENTITY	= "npc";
	FOR_ACCEPT_TASK_CLIENT_ID = -1; --当前被选中的任务ID

	QUESTS_DISPLAYED = 12;  --最大显示10个任务
	MAX_QUESTS = 20;	--最多可以接20个任务
	MAX_OBJECTIVES = 10;	--任务最多显示的任务需求物品
	MAX_TARGETS = 10;	--任务最多需要杀的怪物

	QuestSelected = 0;
	
	NPCOPTION_Btnoption = { 
		btn1={type=""}, 
		btn2={type=""},
		btn3={type=""}, 
		btn4={type=""},
		btn5={type=""}, };
		
	TASK_RewardID = {0, 0, 0, 0};
	TASK_SelReward = -1;

	--guild

	FOR_GUILD_LIST_SELECTED_IDENTITY	= "";
	
	--training
	TRAINING_CurrPage = 1;
	TRAINING_ListID = 0;
	TRAINING_ItemID = {0,0,0,0,0,0,0,0,0,0};
	TRAINING_ItemInfo = 
		{	{id=0}, 
			{id=0}, 
			{id=0}, 
			{id=0}, 
			{id=0},
			{id=0}, 
			{id=0},
			{id=0}};
	TRAINING_ItemCount = 0;
	TRAINING_SelItem = 0;
	
	--shotcut skill
	SS_BtnIndex = { 
		MagicShortcut1=1,
		MagicShortcut2=2,
		MagicShortcut3=3,
		MagicShortcut4=4,
		MagicShortcut5=5,
		MagicShortcut6=6,
		MagicShortcut7=7,
		MagicShortcut8=8,
		MagicShortcut9=9,
		Magic_04=10,
		Magic_05=11,
		Magic_06=12,
		Magic_07=13,
		Magic_08=14,
		Magic_09=15,
		Magic_10=16
	};
	
	SS_SkillCount = 0;
	SS_SkillInfo = 
	{
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0},
		{name="", skilllevel=0, desc="", skillid=0}
	};
		
	
	
	
	--聊天相关
	GCHAT_Command	="/s ";

};
