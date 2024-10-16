;宝箱1号


proc 1

	DIALOG 1
	add_talk $_npc$：

	
	strvar ClanID PlayerID
	;记录的公会ID	
	get_pworld_var 0 32 1 ClanID	
	;得到对话人物的公会ID
	GET_ID_PLAYER_INFO 0 18 PlayerID
;	if @PlayerID = @ClanID
	IS_IN_MY_CLAN_CITY 20
	if 20 = 1
		add_talk #b#b你是谁？你叫什么？你多大了？你家哪里？你要干什么？……	
		
		strvar NumRet
		GET_PLAYER_DAILY_FLAG 1 0 NumRet
		if NumRet = 1
			add_talk #b#b
			add_talk #b#b#c469DCA小提示：你已经摸过这个宝箱了，去找其他的吧！#n
		endif
		if NumRet = 0
		
			option 150 #摸摸宝箱里有什么
		endif
		
	else
		add_talk #b#b看什么看，没见过这么帅的宝箱啊！
	
	endif
	talk
endproc 

proc 150
	strvar Ret Retm Con Val
	SET_PLAYER_DAILY_FLAG Ret 0 1
	if Ret = 0
		GET_ID_PLAYER_INFO 0 19 Con	
		
		if Con != -1
			var Val @Con
			dev Val 100
	
			ADD_EXP 0 @Val Retm
			if Retm = 0
				CLAN_TOUCH_ADD 1 1
			endif
		endif
	endif
	return
endproc 








