

proc 1

	DIALOG 1
	strvar ClanRet
	is_in_my_clan_city  ClanRet
	if ClanRet = 0
		add_talk $_npc$:
		add_talk #b#b����ҵ�ô����ҿɲ����ױ�����ġ���
	
	else	
	
		strvar Ret
		GET_CLAN_BUILDING_LV Ret 0 1	
		add_talk $_npc$:
		add_talk #b#b���ã�����Ϊ�����ռ���Դ�İ����ǣ�
		add_talk #b#b
		add_talk #b#b#c186506���������Դ���ʽ���Ծ������ң��һ�������л��ģ�����Ҳ�����һЩ�ô�Ŷ#n
		add_talk #b#b#c186506#n
		add_talk #b#b
		add_talk #b#b#c8a8a8aС��ʾ��������Դ�������빫��ȼ�����������ᴢ�����Դ�ﵽ���޺󣬻��ǿ����ύ��Դ��ù��׶ȵ�#n
		if Ret = 100 or Ret = 0
;		add_talk #b#b#c186506��ǰ�ɼ����ĵȼ�Ϊ��0��#n		
		endif
		if Ret = 101 or Ret = 1
;		add_talk #b#b#c186506��ǰ�ɼ����ĵȼ�Ϊ��1��#n
		endif
		if Ret = 102 or Ret = 2
;		add_talk #b#b#c186506��ǰ�ɼ����ĵȼ�Ϊ��2��#n
		endif
		strvar CRet
		GET_CLAN_USED_TECH_POINT CRet 0 2
		
		if Ret > 0 and Ret != 100
		
;			option 120 #����ɼ���
			if CRet > 0
;				option 121 #�ɼ�������			
			endif
			if Ret < 2
			
;				option 119 #�����ɼ�����
			endif
			
		endif
;		option 200 #�ɼ�����ȡ��Դ
		option 201 #�򹫻������Դ
		
		
		option 298 #�򹫻�����ʽ�
		option 400 #�ʽ������
		
	endif
		
	talk
endproc 
proc 200
	LAYDOWN_CLAN_ORE 0 0
endproc


proc 201
	
	PLAYER_CLOSE_DIALOG 0 0
	DIALOG 3
	add_talk $_npc$:
	add_talk #b#b��Ϊ�����һԱ�������κ���������������Դ����Ȼ��Ҳ�����ô��ġ���
	add_talk �ɾ�����Դ��
	add_talk #b#b���ࣺԭ�ʿ���Ӣ�����ܿ�����󡢲Ժ��
	add_talk #b#b���ࣺԭ���͡��͹��͡�����͡������͡�������


	add_talk #b#b
		SET_DLG_COMMIT_NUM 5 10
		NPC_COMMIT_OPT 202
	talk
endproc
proc 298
		PLAYER_CLOSE_DIALOG 0 0
	PKG_IS_UNLOCK 20 30
	if 20 = 0 and 30 = 0
		
		return
	endif
	DIALOG 1
	add_talk $_npc$:
	add_talk �����ʽ����
	add_talk #b#b����1000 #c186506����#n����������1000�ʽ𣬸��˻��10�㹱�׶ȣ�
	add_talk #b#b����10000 #c186506����#n����������10000�ʽ𣬸��˻��100�㹱�׶ȣ�
	add_talk #b#b����50000 #c186506����#n����������50000�ʽ𣬸��˻��500�㹱�׶ȣ�
	add_talk #b#b
;	add_talk #b#b#c186506<��ʾ������������ʱ�����Զ��۳�����>#n
	
		option 290 ����1000�����ʽ�
		option 291 ����10000�����ʽ�
		option 292 ����50000�����ʽ�
	talk
endproc 
proc 290
	choose 2 310 305 ȷ������1000������Ϊ�����ʽ�
endproc
proc 291
	choose 2 320 305 ȷ������10000������Ϊ�����ʽ�
endproc
proc 292
	choose 2 330 305 ȷ������50000������Ϊ�����ʽ�
endproc

proc 300
	; strvar ret
	; get_id_player_info 0 21 ret
	; get_id_player_info 0 22 30
	; if 30 < 1000 and @ret >= 1000
		; choose 1 310 299 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	; elseif ret < 1000
		; choose 1 310 299 �������㣬����������ֱ���������ң��Ƿ����������
	; else
		; call 310
	; endif
endproc 

proc 299
	return
endproc

proc 310
	PKG_IS_UNLOCK 20 30
	if 20 = 0 and 30 = 0
		return
	endif
	DEC_MONEY_TEST 0 1000 20 11
	if 11 = -1
		add_sys_msg_start
		add_sys_msg ��û����ô��Ǯ��
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	DEC_MONEY 0 1000 20 10
	if 10 = 0
		ADD_ID_CLAN_MONEY 12 0 1000 0 0
	;	ADD_ID_PLAYER_CONTRI_TEST 0 0 10 0	
		if 12 = 0
			ADD_ID_PLAYER_CONTRI 0 0 10 0 0
			RECORD_ID_PLAYER_CLAN_DONATE 0 0 1000 1000 10
			
			strvar clanID
			GET_ID_PLAYER_INFO 0 18 clanID
			add_sys_msg_start
			add_sys_msg �����Ա$_player$�򹫻������1000�ʽ𣬸�л���Թ���İ�����
			set_sys_msg_mode 14 0 0
			CLAN_MSG_INFO 1 @clanID
		endif
	endif

endproc 
proc 301
	; strvar ret
	; get_id_player_info 0 21 ret
	; get_id_player_info 0 22 30
	; if 30 < 10000 and @ret >= 10000
		; choose 1 320 299 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	; elseif ret < 10000
		; choose 1 320 299 ���������㣬����������ֱ���������ң��Ƿ����������
	; else
		; call 320
	; endif
endproc 

proc 320
	PKG_IS_UNLOCK 20 30
	if 20 = 0 and 30 = 0
		
		return
	endif
	DEC_MONEY_TEST 0 10000 20 11
	if 11 = -1
		add_sys_msg_start
		add_sys_msg ��û����ô��Ǯ��
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	DEC_MONEY 0 10000 20 10
	if 10 = 0
		ADD_ID_CLAN_MONEY 13 0 10000 0 0
		if 13 = 0
		;	ADD_ID_PLAYER_CONTRI_TEST 0 0 20 0	
			ADD_ID_PLAYER_CONTRI 0 0 100 0 0
			RECORD_ID_PLAYER_CLAN_DONATE 0 0 10000 10000 100
			strvar clanID
			GET_ID_PLAYER_INFO 0 18 clanID
			add_sys_msg_start
			add_sys_msg �����Ա$_player$�򹫻������10000�ʽ𣬸�л���Թ���İ�����
			set_sys_msg_mode 14 0 0
			CLAN_MSG_INFO 1 @clanID
			
		endif
	endif

endproc 
proc 302
	; strvar ret
	; get_id_player_info 0 21 ret
	; get_id_player_info 0 22 30
	; if 30 < 50000 and @ret >= 50000
		; choose 1 330 299 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	; elseif ret < 50000
		; choose 1 330 299 �������㣬����������ֱ���������ң��Ƿ����������
	; else
		; call 330
	; endif
endproc 

proc 330
	PKG_IS_UNLOCK 20 30
	if 20 = 0 and 30 = 0
		
		return
	endif
	DEC_MONEY_TEST 0 50000 20 11
	if 11 = -1
		add_sys_msg_start
		add_sys_msg ��û����ô��Ǯ��
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	DEC_MONEY 0 50000 20 10
	if 10 = 0
		ADD_ID_CLAN_MONEY 14 0 50000 0 0
	;	ADD_ID_PLAYER_CONTRI_TEST 0 0 20 0	
		if 14 = 0
			ADD_ID_PLAYER_CONTRI 0 0 500 0 0
			RECORD_ID_PLAYER_CLAN_DONATE 0 0 50000 50000 500
			
			strvar clanID
			GET_ID_PLAYER_INFO 0 18 clanID
			add_sys_msg_start
			add_sys_msg �����Ա$_player$�򹫻������50000�ʽ𣬸�л���Թ���İ�����
			set_sys_msg_mode 14 0 0
			CLAN_MSG_INFO 1 @clanID
		endif
	endif

endproc 


proc 400
	open_ui 69
endproc 

proc 202
;	strvar Num 
;	GET_MUL_COMMIT_ITEM_INFO Ret 0 3 Num 
;	if Ret = 0
;		COMMIT_CLAN_ITEM 1 0 @Num		
;	endif
	COMMIT_MUL_CLAN_ITEM 0
	PLAYER_CLOSE_DIALOG 0 0
endproc


proc 119
	open_ui 22
endproc	

proc 120

	TRANSMIT 4000 17100 10800 1
endproc
proc 121
	open_ui 46
endproc 
proc 122
	OPEN_NPC_SHOP 
endproc

