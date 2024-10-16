proc 1


	DIALOG 1
	add_talk #b#b为了维护零纪元大陆的秩序，自治联盟先后颁布了4则相互维系的条约，熟悉的人们把这些条约统称为“四律条约”。
	add_talk #b#b
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b——自治联盟【风纪委员会】授

		option 100 《保护条约》 ——关于【战斗模式】法则
		option 200 《安全条约》 ——关于【地图规则】法则
		option 300 《制约条约》 ——关于【杀气值】法则
		option 400 《惩治条约》 ——关于【红黑名】法则

	talk
endproc 

proc 100
	DIALOG 1
	add_talk #b#b#b#b#b#b★《保护条约》概述★
	add_talk #b#b
	add_talk #b#b为了保护每个人的立场，自治联盟颁布了一则《保护条约》。
	add_talk #b#b其中更提供了【战斗模式】的选择。
	add_talk #b#b根据各种情况选择适当的模式，既可确保攻击不再造成误伤，保护相应立场同伴的安全。
	add_talk #b#b
	add_talk #c186506更变模式
	add_talk #b#b角色头像右下角的“模式”按钮可更改模式
	add_talk #b#b键盘“Ctrl+A”快捷键组合，可切换模式#n
	add_talk #b#b
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b——自治联盟【风纪委员会】授
	
		option 101 【战斗模式】说明
		option 1 我想了解更多条约

	talk
endproc

proc 101
	DIALOG 1
	add_talk #c186506更变模式
	add_talk #b#b角色头像右下角的“模式”按钮可更改模式
	add_talk #b#b键盘“Ctrl+A”快捷键组合，可切换模式#n
	add_talk #b#b
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b【战斗模式】说明
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b#c186506和平模式#n
	add_talk #b#b#b#b#b#b无法主动攻击任何玩家
	add_talk #b#b#b#b 受到玩家攻击，不可进行反击
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b#c186506中立模式#n
	add_talk #b#b#b#b#b#b无法主动攻击任何玩家
	add_talk #b#b#b#b 受到玩家攻击，可以进行反击
	add_talk #b#b#b可以主动攻击闪名、红名、黑名玩家
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b#c186506自由模式#n
	add_talk #b#b#b#b#b#b可以主动攻击任何玩家
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b#c186506队伍模式#n
	add_talk #b#b#b 可以主动攻击除队友外的任何玩家
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b#c186506公会模式#n
	add_talk #b#b#b 可以主动攻击除会友外的任何玩家
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#c186506公会宣战模式#n
	add_talk #b#b#b#b 可以攻击宣战的敌对公会玩家
	add_talk #b可以攻击除会友外的闪名、红名、黑名玩家
	add_talk #b#b
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b——自治联盟【风纪委员会】授
	
		option 1 我想了解更多条约
	
	talk
endproc

proc 200
	DIALOG 1
	add_talk #b#b#b#b#b#b★《安全条约》概述★
	add_talk #b#b
	add_talk #b#b为了维护零纪元大陆各地区的和平，自治联盟颁布了绝对的《安全条约》。
	add_talk #b#b《安全条约》将大部分混乱区域列入管制范围，并制定了一系列的安全措施，尽可能确保大家拥有一个良好和平的环境。
	add_talk #b#b
	add_talk #c186506区域查看
	add_talk #b#b“小地图”所示“地图名字”的颜色，可判断该区域的安全程度#n
	add_talk #b#b
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b——自治联盟【风纪委员会】授
	
		option 201 【区域颜色】说明
		option 1 我想了解更多条约

	talk
endproc

proc 201
	DIALOG 1
	add_talk #c186506区域查看
	add_talk #b#b“小地图”所示“地图名字”的颜色，可判断该区域的安全程度#n
	add_talk #b#b
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b【区域颜色】说明
	add_talk #b#b
	add_talk #b#b#b#b#b#b#c186506绿色名字#n（安全区域）
	add_talk #b#b#b#b#b#b#b#b 例：时间城
	add_talk #b#b#b#b#b#b#b#b禁止自由战斗
	add_talk #b#b
	add_talk #b#b#b#b#b#b#cffe400黄色名字#n（中立区域）
	add_talk #b#b#b#b#b#b 例：野外大部份地图
	add_talk #b#b#b#b#b#b#b#b允许自由战斗
	add_talk #b#b#b 击杀玩家，将获得“杀气值”惩罚
	add_talk #b#b
	add_talk #b#b#b#b#b#b#cF80B0B红色名字#n（战斗区域）
	add_talk #b#b#b#b#b#b#b#b例：地宫#cF80B0B 2#n层
	add_talk #b#b#b#b#b#b#b#b允许自由战斗
	add_talk #b#b#b# 击杀玩家，不获得“杀气值”惩罚
	add_talk #b#b
	add_talk #b#b#b#b#b#b#cF80B0B公会城市#n（特殊区域）
	add_talk #b#b#b#b#b#b#b#b允许自由战斗
	add_talk  所属公会成员击杀玩家，不获得“杀气值”惩罚
	add_talk  非该公会成员击杀玩家，将获得“杀气值”惩罚
	add_talk #b#b
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b——自治联盟【风纪委员会】授
	
		option 1 我想了解更多条约
	
	talk
endproc

proc 300
	DIALOG 1
	add_talk #b#b#b#b#b#b★《制约条约》概述★
	add_talk #b#b
	add_talk #b#b为了维持秩序，自治联盟颁布了《制约条约》。
	add_talk #b#b条约中对恶意杀人者进行了严厉打压，违反者将留下【杀气值】作为案底。
	add_talk #b#b
	add_talk #c186506杀气查看
	add_talk #b#b指向角色面板右上角红色的“双刃”图标，即可查看当前的杀气值
	add_talk #b#b（快捷键C可以直接打开“角色面板）#n
	add_talk #b#b
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b——自治联盟【风纪委员会】授
	
		option 301 【杀气值】说明
		option 1 我想了解更多条约

	talk
endproc

proc 301
	DIALOG 1
	add_talk #c186506杀气查看
	add_talk #b#b指向角色面板右上角红色的“双刃”图标，即可查看当前的杀气值
	add_talk #b#b（快捷键C可以直接打开“角色面板）#n
	add_talk #b#b
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b【杀气值】说明
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b #cF80B0B获得“杀气值”#n
	add_talk #b#b#b#b#b#b#b（先手击杀玩家）
	add_talk #b#b 每次击杀玩家，获得#cF80B0B 10#n点“杀气值”
	add_talk #b#b#b#b “杀气值”≥#cF80B0B 30#n，为红名玩家
	add_talk #b#b#b#b“杀气值”≥#cF80B0B 200#n，为黑名玩家
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b #c186506减少“杀气值”#n
	add_talk #b#b#b#b#b#b（每分钟减少#c186506 1#n次）
	add_talk #b#b#b#b“杀气值”越低，每次减少越多
	add_talk #b#b#b#b“杀气值”越高，每次减少越少
	add_talk #b使用“祷文”类道具，可直接减少“杀气值”
	add_talk #b#b#b#b“杀气值”<#c186506 30#n，为白名玩家
	add_talk #b#b
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b——自治联盟【风纪委员会】授
	
		option 401 【名字颜色】说明
		option 1 我想了解更多条约
	
	talk
endproc

proc 400
	DIALOG 1
	add_talk #b#b#b#b#b#b★《惩治条约》概述★
	add_talk #b#b
	add_talk #b#b为了惩治那些恶意杀人者，自治联盟颁布了《惩治条约》。
	add_talk #b#b条约将对“杀气值”达到一定程度的违反者，进行相应程度的记名，并给予严厉的制约与惩罚。
	add_talk #b#b
	add_talk #c186506记名查看
	add_talk #b#b“人物名字”所示的颜色，可判断当前的记名#n
	add_talk #b#b
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b——自治联盟【风纪委员会】授
	
		option 401 【名字颜色】说明
		option 1 我想了解更多条约

	talk
endproc

proc 401
	DIALOG 1
	add_talk #c186506记名查看
	add_talk #b#b“人物名字”所示的颜色，可判断当前的记名#n
	add_talk #b#b
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b【名字颜色】说明
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b#cffffff白色名字#n
	add_talk #b#b#b#b#b#b#b （杀气值 <#c186506 30#n）
	add_talk #b#b#b#b#b#b#b★玩家击杀惩罚★
	add_talk #b#b#b#b#b#b凶手将获得“杀气值”
	;add_talk #b#b#b#b#b 掉落些许携带的“银币”
	add_talk #b#b#b#b#b低几率掉落背包的“物品”
	add_talk #b#b#b#b#b#b#b★怪物击杀惩罚★
	add_talk #b#b#b#b#b掉落穿戴装备的“耐久度”
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b#cff8e8e闪烁名字#n
	add_talk #b#b#b （杀气值 <#c186506 30#n，先手攻击玩家）
	add_talk #b#b#b#b#b#b#b★玩家击杀惩罚★
	add_talk #b#b#b#b#b#b#c005ebe凶手不获得“杀气值”#n
	;add_talk #b#b#b#b#b 掉落些许携带的“银币”
	add_talk #b#b#b#b#b低几率掉落背包的“物品”
	add_talk #b#b#b#b#b#b#b★怪物击杀惩罚★
	add_talk #b#b#b#b#b掉落穿戴装备的“耐久度”
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b#cF80B0B红色名字#n
	add_talk #b#b#b#b#b#b#b （杀气值≥#cF80B0B 30#n）
	add_talk #b#b#b#b#b#b#b★玩家击杀惩罚★
	add_talk #b#b#b#b#b#b凶手不获得“杀气值”
	;add_talk #b#b#b#b#b 掉落少量携带的“银币”
	add_talk #b#b#b #c005ebe小几率掉落背包或身上的“物品”#n
	add_talk #b#b#b#b#b#b#b★怪物击杀惩罚★
	add_talk #b#b#b#b#b掉落穿戴装备的“耐久度”
	;add_talk #b#b#b#b#b #c005ebe掉落些许携带的“银币”#n
	add_talk #b#b#b#b#b#b#b#b★其它惩罚★
	add_talk #b#b#b #c005ebe在野外将受到“巡逻卫兵”的攻击#n
	add_talk #b#b#b#b #c005ebe商店的购买价格为正常的#n#cF80B0B 200％#n
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b#c000000黑色名字#n
	add_talk #b#b#b#b#b#b#b（杀气值≥#cF80B0B 200#n）
	add_talk #b#b#b#b#b#b#b★玩家击杀惩罚★
	add_talk #b#b#b#b#b#b凶手不获得“杀气值”
	add_talk #b#b#b#b#b 掉落适量携带的“银币”
	add_talk #b#b#b 有几率掉落背包或身上的“物品”
	add_talk #b#b#b#b#b#c005ebe掉落些许人物的“经验值”#n
	add_talk #b#b#b#b#b#b#b★怪物击杀惩罚★
	add_talk #b#b#b#b#b掉落穿戴装备的“耐久度”
	;add_talk #b#b#b#b#b 掉落少量携带的“银币”
	add_talk #b#b#b#b#b#c005ebe掉落些许人物的“经验值”#n
	add_talk #b#b#b#b#b#c005ebe低几率掉落背包的“物品”#n
	add_talk #b#b#b#b#b#b#b#b★其它惩罚★
	add_talk #b#b#b 在野外将受到“巡逻卫兵”的攻击
	add_talk #b#b#b#b 商店的购买价格为正常的#cF80B0B 200％#n
	add_talk #b#c005ebe使用“传送”类道具，将被直接送到【监狱】#n
	add_talk #c005ebe死亡后“回复活点”复活，将被直接送到【监狱】#n
	add_talk #b#b
	add_talk #cff4e00友情提醒：若非勇气可嘉，别轻易尝试成为红黑名#n
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b——自治联盟【风纪委员会】授
	
		option 1 我想了解更多条约
	
	talk
endproc

