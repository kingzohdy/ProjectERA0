;����1��


proc 1

	DIALOG 1
	add_talk $_npc$��

	
	strvar ClanID PlayerID
	;��¼�Ĺ���ID	
	get_pworld_var 0 32 1 ClanID	
	;�õ��Ի�����Ĺ���ID
	GET_ID_PLAYER_INFO 0 18 PlayerID
;	if @PlayerID = @ClanID
	IS_IN_MY_CLAN_CITY 20
	if 20 = 1
		add_talk #b#b����˭�����ʲô�������ˣ���������Ҫ��ʲô������	
		
		strvar NumRet
		GET_PLAYER_DAILY_FLAG 1 0 NumRet
		if NumRet = 1
			add_talk #b#b
			add_talk #b#b#c469DCAС��ʾ�����Ѿ�������������ˣ�ȥ�������İɣ�#n
		endif
		if NumRet = 0
		
			option 150 #������������ʲô
		endif
		
	else
		add_talk #b#b��ʲô����û������ô˧�ı��䰡��
	
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








