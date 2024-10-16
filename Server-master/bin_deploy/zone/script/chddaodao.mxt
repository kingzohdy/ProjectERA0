
;刀刀
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b $_player$，您已被零纪元官方抽中为幸运玩家，您将获得以下的奖励：
	add_talk #b#b星辉宝钻 99个
	add_talk #b#b幻化果实 99个
	add_talk #b#b极品宠物-公爵
	add_talk #b#b金券 3000
	add_talk #b#b请点击以下链接登陆到活动主页领取属于你的奖品。
	add_talk #b#b您的获奖验证码为：P41A，请牢记。
	
	;	get_task_var 30013 0 8 1 20
	;	if 20 = 1
		
			option 150 这个是零纪元的官网吗？
	;	endif
	talk
endproc
proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我当然不会告诉你，#c186506零纪元的官网是：http://www.ljy0.com #n
	add_talk #b#b嘿嘿，你若是知道了零纪元的官网，岂不是就看穿了我的骗术了嘛，我还怎么#c186506通过传播木马和用虚假中奖网站骗玩家上当，然后盗他们的号呢？#n

	add_talk #b#b<#049#049#049#049>
	add_talk #b#b我是盗号了怎么样，我是骗了不少人，即使你今天拆穿了我，明天还是会有不少人上当，嘿嘿，我可是有绝招的！

		option 151 什么绝招？

	talk
endproc 

proc 151
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我从来不告诉别人，我们的绝对是#c186506通过邮件、私聊等骗玩家说登陆某个网站就可以获得极品武器、极品宠物、星辉宝钻99个、幻化果实99个，金券数千……#n
	add_talk #b#b结果，结果你是知道的！
	add_talk #b#b<我决定要为民除害了！>
	add_talk #b#b怎么，想动手？告诉你，我可不怕，我有帮手！木马，好好招呼招呼他！
	task_stat 30013 1 20
	if 20 = 2
		option 152 #看招！
	endif
	talk
endproc 
proc 152
	task_fini 30013 1
	NEW_LIFE_MON 241009 1 35400 35200 0 0 0 0 10 0 0
endproc 









