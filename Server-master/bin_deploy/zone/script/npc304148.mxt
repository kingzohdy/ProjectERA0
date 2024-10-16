proc 1
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b亲爱的#Name#，如果你想知道关于宠物寄养与宠物进化的相关知识，就跟我聊聊吧。
		
		OPTION 300 #(买卖)打开商店
		option 400 #《宠物寄养说明书第一章》寄养
		option 401 #《宠物寄养说明书第二章》喂食与温饱度
		option 402 #《宠物寄养说明书第三章》玩耍与欢乐度
		option 403 #《宠物寄养说明书第四章》引诱
		option 404 #《宠物寄养说明书第五章》宠物进化
		talk
	
endproc

proc 300
    OPEN_NPC_SHOP
endproc

proc 400
	DIALOG 1
		add_talk 《宠物寄养说明书第一章》寄养
		add_talk #b#b寄养栏位分为#cC45ADE公会栏位#n与#cC45ADE租赁栏位#n两种
		add_talk #b#b使用公会栏位时，需要消耗一定的公会贡献度；使用租赁栏位时，则消耗一定的银币（可点击魔法宠物笼查看类型及消耗）
		add_talk #b#b宠物成长潜力越高，寄养时的消耗也就越多（包括栏位费用-公会贡献度或银币、寄养消耗的经验值）
		add_talk #b#b#cFF4e00注：无论是公会贡献度还是银币，都是每小时的价格，最终消耗还需要乘以寄养时间。
		add_talk #b#b宠物寄养成功后，每3分钟会获得一些潜力。#n
		
		option 1 #返回上一级
		
		talk
		
endproc
	
proc 401
	DIALOG 1
		add_talk 《宠物寄养说明书第二章》喂食与温饱度
		add_talk 目前的宠物食物有：
		add_talk #b#b#cC45ADE清水#n 可喂食成长潜力 1-6 的宠物 可在时间城莉莉的商店中购买
		add_talk #b#b#cC45ADE糖水#n 可喂食成长潜力 1-8 的宠物 可通过陪宠物玩耍从宠物馈赠中获得
		add_talk #b#b#cC45ADE香油#n 可喂食成长潜力 1-9 的宠物 可通过宠物打工、引诱其他人的宠物获得
		add_talk #b#b
		add_talk 温饱度与潜力的关系：
		add_talk #b#b温饱度#c4D85BE 1-29#n  潜力获得速度为#c4D85BE 1#n 倍
		add_talk #b#b温饱度#c4D85BE 30-59#n 潜力获得速度为#c4D85BE 1.6#n 倍
		add_talk #b#b温饱度#c4D85BE 60-79#n 潜力获得速度为#c4D85BE 2#n 倍
		add_talk #b#b温饱度#c4D85BE 8-100#n 潜力获得速度为#c4D85BE 3#n 倍
		
		option 1 #返回上一级
		
		talk
endproc

proc 402
	DIALOG 1
		add_talk 《宠物寄养说明书第三章》玩耍与欢乐度
		add_talk 目前的玩耍方式有2种：
		add_talk #b#b消耗宠物玩具 增加5点欢乐度 
		add_talk 宠物玩具的获得方式：
		add_talk #b#b可通过宠物打工获得，打工时间为90分钟，每次打工消耗25点欢乐度
		
		option 1 #返回上一级
		
		talk
endproc

proc 403
	DIALOG 1
		add_talk 《宠物寄养说明书第四章》引诱
		add_talk #b#b每只宠物在寄养的过程中最多可被引诱5次，每次寄养时引诱次数会被重置。引诱需要消耗银币，并且引诱次数越多，银币消耗越高！
		add_talk #b#b引诱有可能会失败，当引诱成功后，会获得“宠物劳动果实”，内含潜力、宠物食物、宠物玩具、进化果实。
		add_talk #b#b被寄养未超过1小时的宠物无法被引诱！
		
		option 1 #返回上一级
		
		talk
endproc	

proc 404
	DIALOG 1
		add_talk 《宠物寄养说明书第五章》宠物进化
		add_talk #b#b当潜力积攒到100%时，就可前往时间城莉莉处进行宠物进化，提升宠物成长潜质。
		add_talk #b#b当您≥50级，才会看到莉莉处的宠物进化选项
		add_talk #b#b宠物进化需要的进化果实可通过宠物打工或引诱别人寄养的宠物获得。
		
		option 1 #返回上一级
		
		talk
endproc	
	