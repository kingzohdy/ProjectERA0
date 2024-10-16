

proc 1
	DIALOG 1
	
	strvar ClanRet
	is_in_my_clan_city  ClanRet
	if ClanRet = 0
		add_talk $_npc$:
		add_talk #b#b小子，想偷东西吗？快离我远点，否则我就叫了！
	
	else
	
		add_talk $_npc$:
		add_talk #b#b你好，我将负责记录公会物资的收入支出，我想应该没人比我需要谨慎行事了。
		add_talk #b#b
		add_talk #b#b#c186506看到旁边的那个胖子了吗？我这里的道具卖的可比他那#n#cff00ff便宜2~5倍#n#c186506呢，不过得需要公会自己制造出来才可以换取的到。
		add_talk #b#b
		add_talk #b#b#c8a8a8a小提示：制造出来的道具默认是锁定的，需要会长解锁后才可换取#n
		add_talk #b#b#c8a8a8a锁定状态：只有会长可换取#n
		add_talk #b#b#c8a8a8a限制状态：官员以上官阶可换取#n
		add_talk #b#b#c8a8a8a开放状态：所有公会成员都可换取#n
	
		option 120 #公会资源中心
		option 140 #领取记录
;		option 121 #提交矿车
;		option 122 #提交资源
	endif	
	talk
endproc 

proc 120

	open_ui 47
endproc 
proc 121
	GIVEBACK_CLAN_MACHINE 0 0
endproc
proc 122
	LAYDOWN_CLAN_ORE 0 0
endproc 

proc 140
	open_ui 72
endproc