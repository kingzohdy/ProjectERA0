;;;�м�̽����

proc 1

	strvar Locw Rad Num1 Num2 Num3 Num4 Num5
	
	GET_NPC_NUM 307015 1 Num1
	GET_NPC_NUM 307016 1 Num2
	GET_NPC_NUM 307017 1 Num3
	GET_NPC_NUM 307018 1 Num4
	GET_NPC_NUM 307019 1 Num5
	
	get_pworld_var 100 8 1 Locw	
	
	if Locw = 1
	
		strvar px py
		GET_PLAYER_MAPINFO 0 0 0 px py
		rand Rad 50	
		;�ڳ�����
		if Rad > 0 and Rad < 3
			strvar LocMon ClanID
			var LocMon 10
			set_pworld_var 101 8 @LocMon 1
			set_pworld_var 12 32 @px 1
			set_pworld_var 16 32 @py 1
			
			GET_MON_NUM_RADIUS 0 @px @py 3000 284272 50 60
			if 50 = 0 and 60 = 0
				effect 0 0 playEffct($@px$,$@py$,"effect\\entity\\1000900_8.ent")
				;��¼�ڳ������x yֵ
				add_sys_msg_start
				add_sys_msg �㾪���˱����ػ��ߣ�
				set_sys_msg_mode 3 0 0
				msg_info	
				
				CALL_MON 1 284272 0 60 @px @py 1
				;�õ�����ID
				get_pworld_var 0 32 1 ClanID	
				add_sys_msg_start
				add_sys_msg �����治�ã���$_player$�������˱����ػ��ߣ��ػ��߻���ֹ����ڱ������߹�������úܶ࣡
				set_sys_msg_mode 14 0 0
				CLAN_MSG_INFO 1 @ClanID
			else
				call 100
			endif
		endif

		;ʮ��֮һ�����ڳ�����
		if Rad = 0
		strvar dir radp
			rand dir 180
			rand radp 50
			add px @radp
			add py @radp
			if Num1 = 0
				NEW_LIFE_NPC 1 307015 @px @py @dir 21500 xbxiang_1.mac
				effect 0 0 playEffct($@px$,$@py$,"effect\\entity\\102200_8.ent")
				call 300
			elseif Num2 = 0

				NEW_LIFE_NPC 1 307016 @px @py @dir 21500 xbxiang_2.mac
				effect 0 0 playEffct($@px$,$@py$,"effect\\entity\\102200_8.ent")
				call 300
			
			elseif Num3 = 0
				NEW_LIFE_NPC 1 307017 @px @py @dir 21500 xbxiang_3.mac
				effect 0 0 playEffct($@px$,$@py$,"effect\\entity\\102200_8.ent")
				call 300
			
			elseif Num4 = 0
				NEW_LIFE_NPC 1 307018 @px @py @dir 21500 xbxiang_4.mac
				effect 0 0 playEffct($@px$,$@py$,"effect\\entity\\102200_8.ent")
				call 300
			
			elseif Num5 = 0
				NEW_LIFE_NPC 1 307019 @px @py @dir 21500 xbxiang_5.mac
				effect 0 0 playEffct($@px$,$@py$,"effect\\entity\\102200_8.ent")
				call 300
			else
				call 100
			endif
		endif
		
		;�ڳ���Ʒ
		if Rad >= 3 and Rad < 50
				;��������ȼ�����
		;	strvar Locg
		;	get_pworld_var 64 8 1 Locg
		;	if Locg = 10
		;		call 200
		;	endif
		;	if Locg = 20
		;		call 201
		;	endif
		;	if Locg = 30
		;		call 202
		;	endif
			strvar lv
			GET_PLAYER_BASE_ATT 0 0 LV 0
			if lv < 40
				call 200
			endif
			if lv >= 40 and lv < 60
				call 201
			endif
			if lv >= 60 and lv < 70
				call 202
			endif
			if lv >= 70 and lv < 80
				call 205
			endif
			if lv >= 80 and lv < 100
				call 203
			endif
			if lv >= 100 and lv <= 120
				call 204
			endif
		
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ı����Ѿ��ڿ��ˣ������ط��ɣ�
		set_sys_msg_mode 1 0 0
		msg_info
	endif

endproc  

proc 2
		strvar Rets Mapc Plos
	PLAYER_CLAN_POS_INFO 0 0 0 0 Plos
	if Plos = 0
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg ��ȥ���������ɣ���ֻ�ܲμ��Լ�����Ļ�ſ����ڱ���
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif
	DYN_ACTIVE_STATUS_GET  1 19 Rets
	if Rets = 0
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg ���ᱦ�ػδ����������ʹ����Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif
	
	strvar ret
	CLAN_JOIN_TIME_NO_SAMEDAY ret
	if ret = 0
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg �����û��Ϥ��������أ����빫�ᵱ���޷��ھ򹫻ᱦ�أ�
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	
	
	
	GET_PLAYER_MAPINFO 1 Mapc 0 0 0 
	if Mapc != 60000
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg �ھ���ֻ���ڹ�����е�ͼʹ�ã�
		set_sys_msg_mode 1 0 0
		msg_info
		return
	else
		strvar ClanID PlayerID
		;��¼�Ĺ���ID	
	;	get_pworld_var 0 32 1 ClanID	
		;�õ�����Ĺ���ID
	;	GET_ID_PLAYER_INFO 0 18 PlayerID
	;	if ClanID != @PlayerID
	IS_IN_MY_CLAN_CITY 20
	if 20 != 1
		SET_SCRIPT_INT_RET -1
			add_sys_msg_start
			add_sys_msg ��ǰ��ͼ�������Լ����ڹ���ĳ��У������ھ򱦲أ�
			set_sys_msg_mode 1 0 0
			msg_info
			return
		endif
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar midx midy px py M
	;��ȡ���ֱ���ʱ�ɵ���λ��
	get_pworld_var 4 32 1 midx 
	get_pworld_var 8 32 1 midy 
	
	;ʹ����Ʒʱ�����λ��
	GET_ID_PLAYER_INFO 0 3 px
	GET_ID_PLAYER_INFO 0 4 py
	;��þ���
	GET_POINT_DIST @midx @midy @px @py M
	
	strvar Locw
	get_pworld_var 100 8 1 Locw
	if Locw = 1
		if M > 800
			SET_SCRIPT_INT_RET -1
			add_sys_msg_start
			add_sys_msg �����ڵ�λ�þ��뱦��̽�����е�Զ�������׷��ֱ��أ�
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
	else
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg ̽������û�з��ֱ��أ��������ڲ������ص�Ŷ��
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif	
endproc 

proc 100
	add_sys_msg_start
	add_sys_msg ���ʲôҲû���ڵ�������Ŭ����
	set_sys_msg_mode 1 0 0
	msg_info
endproc 
proc 101

	add_sys_msg_start
	add_sys_msg ������Ķ���̫���ˣ�������һ�±����ɣ�
	set_sys_msg_mode 1 0 0
	msg_info
endproc 
proc 300
	;�õ�����ID
	get_pworld_var 0 32 1 ClanID	
	add_sys_msg_start
	add_sys_msg ������ã���$_player$����Ȼ�ڵ��˴��䣬��ҿ�ȥ������������ʲô��
	set_sys_msg_mode 14 0 0
	CLAN_MSG_INFO 1 @ClanID
	
endproc 

proc 200
;1~40��
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num = 0
		call 101
		return
	endif
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar Radn
	rand Radn 100
	;if Radn = 0
	;	ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 0
		;�û���ʵ
	;endif
	;if Radn = 1
	;	ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 0
		;�����ʵ
	;endif
	;if Radn = 2
	;	ADD_ITEM_NUM_INPAK 5050510 1 1 0
	;�����Լ
	;endif
	if Radn >= 0 and Radn < 3
		ADD_ITEM_NUM_INPAK 5060014 1 1 0
		;��ʯ��Ƭ
	endif
	if Radn >= 3 and Radn < 6
		ADD_ITEM_NUM_INPAK 5109201 1 1 0
		;���ұ���
	endif
;	if Radn >= 6 and Radn < 8
;		ADD_ITEM_NUM_INPAK 5219400 1 1 0
;		;��覱�ʯ������
;	endif
;	if Radn >= 8 and Radn < 10
;		ADD_ITEM_NUM_INPAK 5062000 1 1 0
;		;���ϲر�ͼ
;	endif
	
	if Radn >= 6 and Radn < 12
		ADD_ITEM_NUM_INPAK 5060064 1 1 0
		;���潱��*3
	endif

	if Radn >= 12 and Radn < 41
		ADD_ITEM_NUM_INPAK 5103001 1 1 0
		;�ؾ�����
	endif
	if Radn >= 41 and Radn < 71
		ADD_ITEM_NUM_INPAK 5101001 2 1 0
		;ԭ�ʿ�
	endif
	if Radn >= 71 and Radn < 100
		ADD_ITEM_NUM_INPAK 5102001 2 1 0
		;ԭ����
	endif
	
endproc 

proc 201
; 40 ~60��
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num = 0
		call 101
		return
	endif
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar Radn
	rand Radn 1000
	if Radn >= 0 and Radn < 3
		ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 0
		;�û���ʵ
	endif
	if Radn >= 3 and Radn < 5
		ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 0
		;�����ʵ
	endif
	;if Radn = 2
	;	ADD_ITEM_NUM_INPAK 5050510 1 1 0
	;�����Լ
	;endif
	if Radn >= 5 and Radn < 30
		ADD_ITEM_NUM_INPAK 5060014 1 1 0
		;��ʯ��Ƭ
	endif
	if Radn >= 30 and Radn < 40
		ADD_ITEM_NUM_INPAK 5109201 1 1 0
		;���ұ���
	endif
	if Radn >= 40 and Radn < 50
		ADD_ITEM_NUM_INPAK 5109202 1 1 0
		;�ۻ���
	endif
;	if Radn >= 50 and Radn < 60
;		ADD_ITEM_NUM_INPAK 5219400 1 1 0
;		;��覱�ʯ������
;	endif
;	if Radn >= 60 and Radn < 80
;		ADD_ITEM_NUM_INPAK 5062000 1 1 0
;		;���ϲر�ͼ
;	endif

	if Radn >= 50 and Radn < 110
		ADD_ITEM_NUM_INPAK 5060065 1 1 0
		;���潱��*5
	endif

	if Radn >= 110 and Radn < 200
		ADD_ITEM_NUM_INPAK 5103001 1 1 0
		;�ؾ�����
	endif
	if Radn >= 200 and Radn < 410
		ADD_ITEM_NUM_INPAK 5103002 1 1 0
		;��������
	endif
	if Radn >= 410 and Radn < 510
		ADD_ITEM_NUM_INPAK 5101001 2 1 0
		;ԭ�ʿ�
	endif
	if Radn >= 510 and Radn < 710
		ADD_ITEM_NUM_INPAK 5101002 2 1 0
		;��Ӣ��
	endif
	if Radn >= 710 and Radn < 810
		ADD_ITEM_NUM_INPAK 5102001 2 1 0
		;ԭ����
	endif
	if Radn >= 810 and Radn < 1000
		ADD_ITEM_NUM_INPAK 5102002 2 1 0
		;�͹���
	endif
endproc 

proc 202
;60-70
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num = 0
		call 101
		return
	endif
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar Radn
	rand Radn 1000
	if Radn >= 0 and Radn < 3
		ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 0
		;�û���ʵ
	endif
	if Radn >= 3 and Radn < 5
		ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 0
		;�����ʵ
	endif
	;if Radn = 2
	;	ADD_ITEM_NUM_INPAK 5050510 1 1 0
	;�����Լ
	;endif
	if Radn >= 5 and Radn < 30
		ADD_ITEM_NUM_INPAK 5060014 1 1 0
		;��ʯ��Ƭ
	endif
	if Radn >= 30 and Radn < 40
		ADD_ITEM_NUM_INPAK 5109201 1 1 0
		;���ұ���
	endif
	if Radn >= 40 and Radn < 50
		ADD_ITEM_NUM_INPAK 5109202 1 1 0
		;�ۻ���
	endif
;	if Radn >= 50 and Radn < 60
;	;	ADD_ITEM_NUM_INPAK 5109203 1 1 0
;		ADD_ITEM_NUM_INPAK 5103001 1 1 0
;		;�����ĳ��ؾ�����
;	endif
;	if Radn >= 60 and Radn < 80
;		ADD_ITEM_NUM_INPAK 5219400 1 1 0
;		;��覱�ʯ������
;	endif
;	if Radn >= 80 and Radn < 110
;		ADD_ITEM_NUM_INPAK 5062000 1 1 0
;		;���ϲر�ͼ
;	endif
	
	if Radn >= 50 and Radn < 110
		ADD_ITEM_NUM_INPAK 5060066 1 1 0
		;���潱��*10
	endif

	if Radn >= 110 and Radn < 160
		ADD_ITEM_NUM_INPAK 5103001 1 1 0
		;�ؾ�����
	endif
	if Radn >= 160 and Radn < 220
		ADD_ITEM_NUM_INPAK 5103002 1 1 0
		;��������
	endif
	if Radn >= 220 and Radn < 410
		ADD_ITEM_NUM_INPAK 5103003 1 1 0
		;�䴨����
	endif
	if Radn >= 410 and Radn < 450
		ADD_ITEM_NUM_INPAK 5101001 2 1 0
		;ԭ�ʿ�
	endif
	if Radn >= 450 and Radn < 510
		ADD_ITEM_NUM_INPAK 5101002 2 1 0
		;��Ӣ��
	endif
	if Radn >= 510 and Radn < 710
		ADD_ITEM_NUM_INPAK 5101003 2 1 0
		;���ܿ�
	endif
	if Radn >= 710 and Radn < 750
		ADD_ITEM_NUM_INPAK 5102001 2 1 0
		;ԭ����
	endif
	if Radn >= 750 and Radn < 810
		ADD_ITEM_NUM_INPAK 5102002 2 1 0
		;�͹���
	endif
	if Radn >= 810 and Radn < 1000
		ADD_ITEM_NUM_INPAK 5102003 2 1 0
		;�����
	endif
endproc

 
proc 203
	;80-100
	strvar Num rad
	GET_PACK_SPACE 0 Num
		
	if Num = 0
		call 101
		return
	endif
	rand rad 100
	if rad = 0
		ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 0
		;�û���ʵ
	endif
	if rad = 1
		ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 0
		;�����ʵ
	endif
;	if rad = 2
;		ADD_ITEM_NUM_INPAK 5050510 1 1 0
;		;�����Լ
;	endif
	if rad = 2
		ADD_ITEM_NUM_INPAK 5060014 1 1 0
		;��ʯ��Ƭ
	endif
	if rad = 3
		ADD_ITEM_NUM_INPAK 5109202 1 1 0
		;�ۻ���
	endif
	if rad = 4
		ADD_ITEM_NUM_INPAK 5109203 1 1 0
	endif
	if rad = 5
		ADD_ITEM_NUM_INPAK 5109204 1 1 0
	endif 
;	if rad >= 6 and rad < 9
;		ADD_ITEM_NUM_INPAK 5219400 1 1 0
;		;��覱�ʯ������
;	endif
;	if rad >= 9 and rad < 16
;		ADD_ITEM_NUM_INPAK 5062000 1 1 0
;		;���ϲر�ͼ
;	endif
	if rad >= 6 and rad < 12
		ADD_ITEM_NUM_INPAK 5060066 1 1 0
		;���潱��*10
	endif

	if rad >= 12 and rad < 20
		ADD_ITEM_NUM_INPAK 5103002 1 1 0
		;��������
	endif
	if rad >= 20 and rad < 26
		ADD_ITEM_NUM_INPAK 5103003 1 1 0
	endif
	if rad >= 26 and rad < 44
		ADD_ITEM_NUM_INPAK 5103004 1 1 0
	endif
	if rad >= 44 and rad < 48
		ADD_ITEM_NUM_INPAK 5101002 2 1 0
		;��Ӣ��
	endif
	if rad >= 48 and rad < 54
		ADD_ITEM_NUM_INPAK 5101003 2 1 0
	endif
	if rad >= 54 and rad < 72
		ADD_ITEM_NUM_INPAK 5101004 2 1 0
	endif
	if rad >= 72 and rad < 76
		ADD_ITEM_NUM_INPAK 5102002 2 1 0
		;�͹���
	endif
	if rad >= 76 and rad < 82
		ADD_ITEM_NUM_INPAK 5102003 2 1 0
	endif
	if rad >= 82 and rad < 100
		ADD_ITEM_NUM_INPAK 5101004 2 1 0
	endif
endproc 

proc 204
	;100-120
	strvar Num rad
	GET_PACK_SPACE 0 Num
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		SET_SCRIPT_INT_RET -1
		add_sys_msg_start
		add_sys_msg ��ô�߼���̽������û�з��ɹ����������ڲ������ص�Ŷ��
		set_sys_msg_mode 3 0 0
		msg_info
		return
		;��ʱ������
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
	if Num = 0
		call 101
		return
	endif
	rand rad 100
	if rad = 0
		ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 0
		;�û���ʵ
	endif
	if rad = 1
		ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 0
		;�����ʵ
	endif
	if rad = 2
		ADD_ITEM_NUM_INPAK 5050510 1 1 0
		;�����Լ
	endif
	if rad = 3
		ADD_ITEM_NUM_INPAK 5060014 1 1 0
		;��ʯ��Ƭ
	endif
	if rad = 4
		ADD_ITEM_NUM_INPAK 5109203 1 1 0
	endif
	if rad = 5
		ADD_ITEM_NUM_INPAK 5109204 1 1 0
	endif
	if rad = 6
		ADD_ITEM_NUM_INPAK 5109205 1 1 0
	endif 
	if rad >= 7 and rad < 9
		ADD_ITEM_NUM_INPAK 5219400 1 1 0
		;��覱�ʯ������
	endif
	if rad >= 9 and rad < 16
		ADD_ITEM_NUM_INPAK 5062000 1 1 0
		;���ϲر�ͼ
	endif
	if rad >= 16 and rad < 20
		ADD_ITEM_NUM_INPAK 5103003 1 1 0
	endif
	if rad >= 20 and rad < 26
		ADD_ITEM_NUM_INPAK 5103004 1 1 0
	endif
	if rad >= 26 and rad < 44
		ADD_ITEM_NUM_INPAK 5103005 1 1 0
	endif
	if rad >= 44 and rad < 48
		ADD_ITEM_NUM_INPAK 5101003 2 1 0
	endif
	if rad >= 48 and rad < 54
		ADD_ITEM_NUM_INPAK 5101004 2 1 0
	endif
	if rad >= 54 and rad < 72
		ADD_ITEM_NUM_INPAK 5101005 2 1 0
	endif
	if rad >= 72 and rad < 76
		ADD_ITEM_NUM_INPAK 5102003 2 1 0
	endif
	if rad >= 76 and rad < 82
		ADD_ITEM_NUM_INPAK 5102004 2 1 0
	endif
	if rad >= 82 and rad < 100
		ADD_ITEM_NUM_INPAK 5102005 2 1 0
	endif
endproc 



proc 205
;70-80
	strvar Num
	GET_PACK_SPACE 0 Num
	if Num = 0
		call 101
		return
	endif
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar Radn
	rand Radn 100
	if Radn = 0
		ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 0
		;�û���ʵ
	endif
	if Radn = 1
		ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 0
		;�����ʵ
	endif
	;if Radn = 2
	;	ADD_ITEM_NUM_INPAK 5050510 1 1 0
	;�����Լ
	;endif
	if Radn = 2
		ADD_ITEM_NUM_INPAK 5060014 1 1 0
		;��ʯ��Ƭ
	endif
	if Radn = 3
		ADD_ITEM_NUM_INPAK 5109201 1 1 0
		;���ұ���
	endif
	if Radn = 4
		ADD_ITEM_NUM_INPAK 5109202 1 1 0
		;�ۻ���
	endif
	if Radn = 5
		ADD_ITEM_NUM_INPAK 5109203 1 1 0
		;����
	endif
;	if Radn >= 6 and Radn < 8
;		ADD_ITEM_NUM_INPAK 5219400 1 1 0
;		;��覱�ʯ������
;	endif
;	if Radn >= 8 and Radn < 12
;		ADD_ITEM_NUM_INPAK 5062000 1 1 0
;		;���ϲر�ͼ
;	endif
	
	if Radn >= 6 and Radn < 12
		ADD_ITEM_NUM_INPAK 5060066 1 1 0
		;���潱��*10
	endif		
	
	if Radn >= 12 and Radn < 16
		ADD_ITEM_NUM_INPAK 5103001 1 1 0
		;�ؾ�����
	endif
	
	if Radn >= 16 and Radn < 22
		ADD_ITEM_NUM_INPAK 5103002 1 1 0
		;��������
	endif
	if Radn >= 22 and Radn < 41
		ADD_ITEM_NUM_INPAK 5103003 1 1 0
	endif
	if Radn >= 41 and Radn < 45
		ADD_ITEM_NUM_INPAK 5101001 2 1 0
		;ԭ�ʿ�
	endif
	if Radn >= 45 and Radn < 51
		ADD_ITEM_NUM_INPAK 5101002 2 1 0
	endif
	if Radn >= 51 and Radn < 71
		ADD_ITEM_NUM_INPAK 5101003 2 1 0
		;���ܿ�
	endif
	if Radn >= 71 and Radn < 75
		ADD_ITEM_NUM_INPAK 5102001 2 1 0
		;ԭ����
	endif
	if Radn >= 75 and Radn < 81
		ADD_ITEM_NUM_INPAK 5102002 2 1 0
		;�͹���
	endif
	if Radn >= 81 and Radn < 100
		ADD_ITEM_NUM_INPAK 5102003 2 1 0
	endif

endproc


