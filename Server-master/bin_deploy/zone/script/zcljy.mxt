proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�װ���������ã��������Ԫ����ɴ�ʹ��
	add_talk #b#bΪ�˸�л��Ҷ��������Ԫ�ĺ���֧�֣�ֻҪ�ҵ����ҡ����������㡱���͡���Ԫ�����۵ļ���ң�����������ȡ��������Ŷ��
	add_talk #b#b#c186506��Ȼ�������ԴӸ��ֹ������ϻ򸱱����ҵ����ǣ��ף�#n
	add_talk #b#b�����ɣ��¸ҵ�ð�ռң�
;	add_talk ���ҵ���
	
;	strvar ItmNum10 ItmNum11 ItmNum12 ItmNum13 ItmNum14
;	var ItmNum10 0
;	var ItmNum11 0
;	var ItmNum12 0
;	var ItmNum13 0
;	var ItmNum14 0
;	GET_ITEM_NUM_INPAK 5060015 1 ItmNum10
;	GET_ITEM_NUM_INPAK 5060016 1 ItmNum11
;	GET_ITEM_NUM_INPAK 5060017 1 ItmNum12
;	GET_ITEM_NUM_INPAK 5060018 1 ItmNum13
;	GET_ITEM_NUM_INPAK 5060019 1 ItmNum14
;	
;	add_talk ���ҡ���#c186506��$@ItmNum10$��#n��
;	add_talk ��������#c186506��$@ItmNum11$��#n��
;	add_talk ���㡱��#c186506��$@ItmNum12$��#n��
;	add_talk ���͡���#c186506��$@ItmNum13$��#n��
;	add_talk ��Ԫ����#c186506��$@ItmNum14$��#n��
	
	task_add_option 3120 1 101	
	
	option 150 #ʹ�õ�������Ҷһ��б�
	option 210 #ʹ�á��㡱���͡���Ԫ���һ��б�
	option 220 #ʹ�á��ҡ����������㡱���͡���Ԫ���һ��б�
;	option 500 ý��ҽ�
			  
	
	talk
endproc 
proc 101
    task_do_option 3120 1
endproc
proc 150
	strvar Lock
	pkg_is_unlock 1 Lock
	if Lock = 0
		return
	endif
	strvar GrabTime
	PLAYER_IS_GRABTIME 1 GrabTime
	if GrabTime = 1
		return
	endif
	
	dialog 1
	; add_talk $_npc$:
	; add_talk #b#b���ã���������Ҫ�һ�ʲô������
	; add_talk #b#b
	add_talk ������#c005ebe���䱦��#n�����л����������䱦����
	add_talk #b#b#c6d00d2��������#n����ϡ�����
	add_talk #b#b#c6d00d2С����#n����ϡ�г��
	
	add_talk #b#b
	; add_talk ���ҵ���
	
	strvar ItmNum10 ItmNum11 ItmNum12 ItmNum13 ItmNum14
	var ItmNum10 0
	var ItmNum11 0
	var ItmNum12 0
	var ItmNum13 0
	var ItmNum14 0
	GET_ITEM_NUM_INPAK 5060015 1 ItmNum10
	GET_ITEM_NUM_INPAK 5060016 1 ItmNum11
	GET_ITEM_NUM_INPAK 5060017 1 ItmNum12
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum13
	GET_ITEM_NUM_INPAK 5060019 1 ItmNum14
	
	add_talk ���ҡ������ҵ�#c186506$@ItmNum10$#n��
	add_talk �����������ҵ�#c186506$@ItmNum11$#n��
	add_talk ���㡱�����ҵ�#c186506$@ItmNum12$#n��
	add_talk ���͡������ҵ�#c186506$@ItmNum13$#n��
	add_talk ��Ԫ�������ҵ�#c186506$@ItmNum14$#n��
	
	option 200 #ʹ��һ�����ҡ��һ�һ�������䱦�䡱
	option 201 #ʹ��һ���������һ�һ�������䱦�䡱
	option 202 #ʹ���������㡱�һ��������ǻԱ��ꡱ
	option 203 #ʹ���������͡��һ���������ʯ��Ƭ��
	option 204 #ʹ��������Ԫ���һ��������û���ʵ��

	talk
endproc

proc 200
;��
	strvar ItmNum Num
	GET_ITEM_NUM_INPAK 5060015 1 ItmNum
	if ItmNum < 1
		add_sys_msg_start
		add_sys_msg ������û�д���Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	else
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5060015 1 1
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052806 1 1 0
		else
			add_sys_msg_start
			add_sys_msg ��Ԥ��1�����ϱ����ո��ٿ�����
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
	call 150
endproc
proc 201
;��
	strvar ItmNum Num
	GET_ITEM_NUM_INPAK 5060016 1 ItmNum
	if ItmNum < 1
		add_sys_msg_start
		add_sys_msg ������û�д���Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	else
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5060016 1 1
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052806 1 1 0
		else
			add_sys_msg_start
			add_sys_msg ��Ԥ��1�����ϱ����ո��ٿ�����
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
	call 150
endproc 
proc 202
;��
	strvar ItmNum Num
	GET_ITEM_NUM_INPAK 5060017 1 ItmNum
	if ItmNum < 3
		add_sys_msg_start
		add_sys_msg ��Я������Ʒ�������㣡
		set_sys_msg_mode 1 0 0
		msg_info
	else
	
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 3 2 Num
		
		if Num = -1 
			add_sys_msg_start
			add_sys_msg ����û���㹻�Ŀռ䣡
			set_sys_msg_mode 1 0 0
			msg_info
			
		else
		
			DEC_ITEM_NUM_INPAK 5060017 3 10
			if 10 = -1
				return
			endif
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 3 1 0		
		
		endif

	endif
	call 150
endproc
proc 203
;��
	strvar ItmNum Num
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum
	if ItmNum < 3
		add_sys_msg_start
		add_sys_msg ��Я������Ʒ�������㣡
		set_sys_msg_mode 1 0 0
		msg_info
	else
	
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060014 6 2 Num
	
		if Num = -1 
			add_sys_msg_start
			add_sys_msg ����û���㹻�Ŀռ䣡
			set_sys_msg_mode 1 0 0
			msg_info
		
		else
			DEC_ITEM_NUM_INPAK 5060018 3 11
			if 11 = -1
				return
			endif
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060014 6 1 0
		endif
	endif
	call 150
endproc 
proc 204
;Ԫ
	strvar ItmNum Num
	GET_ITEM_NUM_INPAK 5060019 1 ItmNum
	if ItmNum < 3
		add_sys_msg_start
		add_sys_msg ��Я������Ʒ�������㣡
		set_sys_msg_mode 1 0 0
		msg_info
	else
	
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233000 2 2 Num
	
		if Num = -1
			add_sys_msg_start
			add_sys_msg ����û���㹻�Ŀռ䣡
			set_sys_msg_mode 1 0 0
			msg_info
		else
		
			DEC_ITEM_NUM_INPAK 5060019 3 12
			if 12 = -1
				return
			endif
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233000 2 1 0
		endif
	endif
	call 150
endproc
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc 210
;���Ԫ
	strvar Lock
	pkg_is_unlock 1 Lock
	if Lock = 0
		return
	endif
	strvar GrabTime
	PLAYER_IS_GRABTIME 1 GrabTime
	if GrabTime = 1
		return
	endif
	dialog 1
	; add_talk $_npc$:
	; add_talk #b#bֻҪ�ҵ�#c186506���㡱���͡���Ԫ��#n�Ϳ��Զһ����䱦��Ŷ���ף�
	; add_talk #b#b
	add_talk ������#c005ebe���䱦��#n�����л����������䱦����
	add_talk #b#b#c6d00d2���ҡ�#n�����������һ���#c005ebe���䱦��#n����
	add_talk #b#b#c6d00d2������#n�����������һ���#c005ebe���䱦��#n����

	add_talk #b#b
	; add_talk ���ҵ���
	
	strvar ItmNum20 ItmNum21 ItmNum22 ItmNum23 ItmNum24
	var ItmNum20 0
	var ItmNum21 0
	var ItmNum22 0
	var ItmNum23 0
	var ItmNum24 0
	GET_ITEM_NUM_INPAK 5060015 1 ItmNum20
	GET_ITEM_NUM_INPAK 5060016 1 ItmNum21
	GET_ITEM_NUM_INPAK 5060017 1 ItmNum22
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum23
	GET_ITEM_NUM_INPAK 5060019 1 ItmNum24
	
	add_talk ���ҡ������ҵ�#c186506$@ItmNum20$#n��
	add_talk �����������ҵ�#c186506$@ItmNum21$#n��
	add_talk ���㡱�����ҵ�#c186506$@ItmNum22$#n��
	add_talk ���͡������ҵ�#c186506$@ItmNum23$#n��
	add_talk ��Ԫ�������ҵ�#c186506$@ItmNum24$#n��
	
	option 211 #�һ�һ�������䱦�䡱
	
	talk

endproc 
proc 211
	strvar ItmNum1 ItmNum2 ItmNum3 Num
	GET_ITEM_NUM_INPAK 5060017 1 ItmNum1
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum2
	GET_ITEM_NUM_INPAK 5060019 1 ItmNum3
	if ItmNum1 > 0 and ItmNum2 > 0 and ItmNum3 > 0
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5060017 1 1
			DEC_ITEM_NUM_INPAK 5060018 1 1
			DEC_ITEM_NUM_INPAK 5060019 1 1
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052805 1 1 0
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$��ͨ�������Ԫ����һ����һ�������䱦�䡱
			set_sys_msg_mode 18 0 0
			svr_msg_info
		else
			add_sys_msg_start
			add_sys_msg ��Ԥ��1�����ϱ����ո��ٿ�����
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ��Я������Ʒ�������㣡
		set_sys_msg_mode 1 0 0
		msg_info
	endif
	call 210
endproc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

proc 220
;�Ұ�
	strvar Lock
	pkg_is_unlock 1 Lock
	if Lock = 0
		return
	endif
	strvar GrabTime
	PLAYER_IS_GRABTIME 1 GrabTime
	if GrabTime = 1
		return
	endif
	dialog 1
	; add_talk $_npc$:
	; add_talk #b#bֻҪ�ҵ�#c186506���ҡ����������㡱���͡���Ԫ��#n�Ϳ��Զһ��ص䱦��Ŷ���ף�
	; add_talk #b#b
	add_talk ������#c005ebe�ص䱦��#n�����л����������䱦����
	add_talk #b#b#c6d00d2��ʿ֮��#n����ϡ�����
	add_talk #b#b#c6d00d2����#n����ϡ�г��

	add_talk #b#b
	; add_talk ���ҵ���
	
	strvar ItmNum30 ItmNum31 ItmNum32 ItmNum33 ItmNum34
	var ItmNum30 0
	var ItmNum31 0
	var ItmNum32 0
	var ItmNum33 0
	var ItmNum34 0
	GET_ITEM_NUM_INPAK 5060015 1 ItmNum30
	GET_ITEM_NUM_INPAK 5060016 1 ItmNum31
	GET_ITEM_NUM_INPAK 5060017 1 ItmNum32
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum33
	GET_ITEM_NUM_INPAK 5060019 1 ItmNum34
	
	add_talk ���ҡ������ҵ�#c186506$@ItmNum30$#n��
	add_talk �����������ҵ�#c186506$@ItmNum31$#n��
	add_talk ���㡱�����ҵ�#c186506$@ItmNum32$#n��
	add_talk ���͡������ҵ�#c186506$@ItmNum33$#n��
	add_talk ��Ԫ�������ҵ�#c186506$@ItmNum34$#n��	
	
	option 221 #�һ�һ�����ص䱦�䡱

	
	talk
endproc

proc 221
	strvar ItmNum1 ItmNum2 ItmNum3 ItmNum4 ItmNum5 Num
	GET_ITEM_NUM_INPAK 5060015 1 ItmNum1
	GET_ITEM_NUM_INPAK 5060016 1 ItmNum2
	GET_ITEM_NUM_INPAK 5060017 1 ItmNum3
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum4
	GET_ITEM_NUM_INPAK 5060019 1 ItmNum5
	if ItmNum1 > 0 and ItmNum2 > 0 and ItmNum3 > 0 and ItmNum4 > 0 and ItmNum5 > 0
		GET_PACK_SPACE 0 Num 
		if Num > 0
			DEC_ITEM_NUM_INPAK 5060015 1 1
			DEC_ITEM_NUM_INPAK 5060016 1 1
			DEC_ITEM_NUM_INPAK 5060017 1 1
			DEC_ITEM_NUM_INPAK 5060018 1 1
			DEC_ITEM_NUM_INPAK 5060019 1 1
			
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052807 1 1 0
			
			add_sys_msg_start
			add_sys_msg ��ϲ��ҡ�$_player$��ͨ�����Ұ����Ԫ����һ����һ�����ص䱦�䡱
			set_sys_msg_mode 18 0 0
			svr_msg_info
		else
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ��Я������Ʒ�������㣡
		set_sys_msg_mode 1 0 0
		msg_info
	endif
	call 220
endproc


proc 500
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�װ���������ã����ǰ�����
	add_talk #b#bΪ�˴�л��Ҷԡ����Ԫ���ĺ񰮺�֧�֣��������������ռ�����������е�6�ֱ��Σ���ֽè��С����ɫɫ��С��ܡ���ʿ��졢С���ˡ����С�����Ҷһ����ƽ������Ŷ������������-���ػ��и���ı���������һ����Ͽ��ж��ɣ�
	add_talk #b#b���������ռ�6�š��齱ȯ�������ҳ�ȡ���ࡢ���õ�����Ŷ��

		option 510 #��Ҫ��6�ֱ��ζһ��ƽ����
		option 520 #��������6�ֳ齱ȯ�����Ͻ��жһ��齱

	talk
endproc
proc 510
	strvar it1 it2 it3 it4 it5 it6
	GET_ITEM_NUM_INPAK 3650003 1 it1
	GET_ITEM_NUM_INPAK 3653004 1 it2
	GET_ITEM_NUM_INPAK 3653003 1 it3
	GET_ITEM_NUM_INPAK 3653001 1 it4
	GET_ITEM_NUM_INPAK 3653000 1 it5
	GET_ITEM_NUM_INPAK 3653002 1 it6
	
	if it1 > 0 and it2 > 0 and it3 > 0 and it4 > 0 and it5 > 0 and it6 > 0
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 3650003 1 1
			DEC_ITEM_NUM_INPAK 3653004 1 1
			DEC_ITEM_NUM_INPAK 3653003 1 1
			DEC_ITEM_NUM_INPAK 3653001 1 1
			DEC_ITEM_NUM_INPAK 3653000 1 1
			DEC_ITEM_NUM_INPAK 3653002 1 1
			
			ADD_BIND_ITEM_NUM_INPAK 5052910 1 1 0                   
		else                  
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
	
	else
		add_sys_msg_start
		add_sys_msg ��Я������Ʒ�������㣡
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 520
	strvar it1 it2 it3 it4 it5 it6
	GET_ITEM_NUM_INPAK 5054000 1 it1
	GET_ITEM_NUM_INPAK 5054001 1 it2
	GET_ITEM_NUM_INPAK 5054002 1 it3
	GET_ITEM_NUM_INPAK 5054003 1 it4
	GET_ITEM_NUM_INPAK 5054004 1 it5
	GET_ITEM_NUM_INPAK 5054005 1 it6
	
	if it1 > 0 and it2 > 0 and it3 > 0 and it4 > 0 and it5 > 0 and it6 > 0
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 5054000 1 1
			DEC_ITEM_NUM_INPAK 5054001 1 1
			DEC_ITEM_NUM_INPAK 5054002 1 1
			DEC_ITEM_NUM_INPAK 5054003 1 1
			DEC_ITEM_NUM_INPAK 5054004 1 1
			DEC_ITEM_NUM_INPAK 5054005 1 1
			                  
			ADD_BIND_ITEM_NUM_INPAK 5052911 1 1 0  
		else                  
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
		
	else
		add_sys_msg_start
		add_sys_msg ��Я������Ʒ�������㣡
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 







