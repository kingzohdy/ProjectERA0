;竞技场
	
proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b刺激！疯狂！血腥！暴力！
    add_talk #b#b这里是属于强者的世界，弱肉强食、优胜劣汰的完美体现；强者最后的天堂！
    add_talk #b#b只要你够强，只要你够血腥、暴力，这里将给你无上的荣耀！
    add_talk #b#b这里是：末世荣耀殿堂！
	add_talk #c0040FF末世荣誉殿堂开启时间：周日 9：00-23：00
	add_talk #c186506周日 19：30-23：00参加可额外获得殿堂荣誉奖励
	add_talk #c186506（每天最多只能获得300点殿堂荣誉）
;	task_add_option 1601 1 101

		option 110 #打开殿堂荣誉商店
		option 300 #末世功勋排行榜奖励
	talk


	
endproc

PROC 101
;	task_do_option 1601 1
ENDPROC

proc 110
	 OPEN_NPC_SHOP
endproc

proc 2
;	get_trig_task 1 2
;	if 2 = 1602
;		task_fini 1602 1
;	endif


endproc


proc 300
	dialog 1
	add_talk #cff4e00周一使用邮件的方式发放排行榜奖励,请注意查收！#n
	add_talk #b#b#c0040FF第1名奖励：#n
	add_talk #b#b珍珠末世奖章*4、赤晶提炼器*3
	add_talk #b#b侏儒意志药剂*5、漆黑之羽*1
	add_talk #b#b#c0040FF第2名至第20名奖励：#n
	add_talk #b#b珍珠末世奖章*4、蓝晶提炼器*3
	add_talk #b#b侏儒意志药剂*3、腥红之翼*1、银币
	add_talk #b#b#c0040FF第21名至第50名奖励：#n
	add_talk #b#b珍珠末世奖章*3、绿晶提炼器*5、
	add_talk #b#b侏儒意志药剂*3、银币
	add_talk #b#b#c0040FF第51名至第100名奖励：#n
	add_talk #b#b珍珠末世奖章*2、绿晶提炼器*3
	add_talk #b#b侏儒意志药剂*2、银币
	add_talk #b#b#c0040FF第101名至第400名奖励：#n
	add_talk #b#b侏儒意志药剂*1、绿晶提炼器*1、银币
	add_talk #b#b

		option 1 返回
	talk
endproc









