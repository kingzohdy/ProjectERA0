

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#bΪʲôҡǮ���᳤��·�м䣿
	add_talk #b#bΪʲôҡǮ���᳤��ˮ����ϣ�
	add_talk #b#bΪʲôҡǮ����˵����
	add_talk #b#b
;	add_talk #b#bԪ������������ô����

	strvar time ret
	get_role_var 32 32 1 time 
	get_role_var 36 8 1 10
	get_role_var 37 8 1 11
	get_role_var 38 8 1 12
	get_role_var 39 8 1 13
		
	get_role_var 49 8 1 14
	get_role_var 50 8 1 15

	is_same_day @time ret
	if ret = -1
	;����ͬһ���������0
		var 10 0
		set_role_var 36 8 @10 1
		var 11 0
		set_role_var 37 8 @11 1
		var 12 0
		set_role_var 38 8 @12 1
		var 13 0
		set_role_var 39 8 @13 1
		
		var 14 0
		set_role_var 49 8 @14 1
		var 15 0
		set_role_var 50 8 @15 1
	endif
	
	STRVAR chunjie
	GET_EVENT_STAT chunjie 302
	
	if chunjie = 1
		option 300 #�������ͺ���  �������ǻԡ�
	endif

		option 200 #��ȡÿ��˫����ʵ��ˮ�����£�
	;	option 200 #ÿ������״̬��ˮ�����£�5250102  
	;	option 210 #��ȡÿ�ն����ٻ���ˮ�����£� 
		option 220 #��ȡÿ�չ����ٻ������ǻ��£�

		strvar mon day h m
		get_now_time mon day 1 1 h m 1 
		;if mon = 6
			;6��
			;if day = 22 or day = 23 or day = 24
			;22~24��
				;if h >= 6 and h <= 24
				;	option 500 #����1Сʱ��ȡ������ͬ
				;	option 550 #����2Сʱ��ȡ��¯ƾ֤
					
					
				;endif
			;endif
		;endif	
		; task_add_option 3343 15 100
		; task_add_option 3344 15 101
		; task_add_option 3345 15 102
		; task_add_option 3346 15 103
		; task_add_option 3347 15 104
		; task_add_option 3348 15 105
		; task_add_option 3349 15 106

		; strvar ret
		; task_stat 3343 0 10
		; task_stat 3344 0 11
		; task_stat 3345 0 12 
		; task_stat 3346 0 13 
		; task_stat 3347 0 14 
		; task_stat 3348 0 15
		; task_stat 3349 0 16
		; if 10 = 2 or 11 = 2 or 12 = 2 or 13 = 2 or 14 = 2 or 15 = 2 or 16 = 2
			; option 150 #žžž
		; endif
	talk              
endproc  

proc 200
	strvar ret
	GET_PACK_SPACE 0 ret
	if ret = 0
	
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬�޷���ȡ����
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	GET_STAR_INFO 1 20
	;ˮ������
	if 20 = 1
		PKG_IS_UNLOCK 10 20
		if 10 = 0 and 20 = 0
			return
		endif
		get_role_var 36 8 1 10
		if 10 < 1 
			
			ADD_BIND_ITEM_NUM_INPAK 5250102 1 1 0
			
			GET_TIME_SEC time
			set_role_var 32 32 @time 1	
			
			add 10 1
			set_role_var 36 8 @10 1	
		else
			add_sys_msg_start
			add_sys_msg �����Ѿ���ȡ���ˣ����������ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		
		endif
	else
		add_sys_msg_start
		add_sys_msg ��û��ˮ��������Ȩ���޷���ȡ
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc  
proc 210
	; GET_STAR_INFO 1 20
	; if 20 = 1
		; PKG_IS_UNLOCK 10 20
		; if 10 = 0 and 20 = 0
			; return
		; endif
		; get_role_var 37 8 1 11
		; if 10 < 1 
			
			; ADD_BIND_ITEM_NUM_INPAK 5041003 1 1 0
			
			; GET_TIME_SEC time
			; set_role_var 32 32 @time 1	
			
			; add 11 1
			; set_role_var 36 8 @11 1	
		; else
			; add_sys_msg_start
			; add_sys_msg �����Ѿ���ȡ���ˣ����������ɣ�
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg ��û��ˮ��������Ȩ���޷���ȡ
		; set_sys_msg_mode 3 0 0
		; msg_info

	; endif
endproc
proc 220

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret = 0
	
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬�޷���ȡ����
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	GET_STAR_INFO 4 20
	;���ǻ���
	if 20 = 1
		PKG_IS_UNLOCK 10 20
		if 10 = 0 and 20 = 0
			return
		endif
		get_role_var 38 8 1 12
		if 12 < 1 
			
		;	ADD_BIND_ITEM_NUM_INPAK 5041023 1 1 0
			ADD_ITEM_NUM_INPAK 5041023 1 1 0
			GET_TIME_SEC time
			set_role_var 32 32 @time 1	
			
			add 12 1
			set_role_var 38 8 @12 1	
		else
			add_sys_msg_start
			add_sys_msg �����Ѿ���ȡ���ˣ����������ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ��û�����ǻ�����Ȩ���޷���ȡ
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc



proc 150
	strvar ret
	task_stat 3343 0 10
	task_stat 3344 0 11 
	task_stat 3345 0 12 
	task_stat 3346 0 13 
	task_stat 3347 0 14 
	task_stat 3348 0 15
	task_stat 3349 0 16 
	if 10 = 2 
		task_fini 3343 1
	endif
	if 11 = 2 
		task_fini 3344 1
	endif
	if 12 = 2 
		task_fini 3345 1
	endif
	if 13 = 2 
		task_fini 3346 1
	endif
	if 14 = 2 
		task_fini 3347 1
	endif
	if 15 = 2 
		task_fini 3348 1
	endif
	if 16 = 2 
		task_fini 3349 1
	endif
endproc

PROC 100
	task_do_option 3343 1
ENDPROC
PROC 101
	task_do_option 3344 1
ENDPROC
PROC 102
	task_do_option 3345 1
ENDPROC
PROC 103
	task_do_option 3346 1
ENDPROC
PROC 104
	task_do_option 3347 1
ENDPROC
PROC 105
	task_do_option 3348 1
ENDPROC
PROC 106
	task_do_option 3349 1
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 3343 
		call 200
	endif
	if 2 = 3344
		call 200
	endif
	if 2 = 3345 
		call 200
	endif
	if 2 = 3346
		call 200
	endif
	if 2 = 3347
		call 200
	endif
	if 2 = 3348
		call 200
	endif
	if 2 = 3349
		call 200
	endif
endproc

; proc 200
; proc 200
	; dialog 1
	; add_talk $_npc$:
	; add_talk #b#bΪʲôҡǮ���᳤��·�м䣿
	; add_talk #b#bΪʲôҡǮ���᳤��ˮ����ϣ�
	; add_talk #b#bΪʲôҡǮ����˵����
	; add_talk #b#b
	; add_talk #b#bԪ������������ô����
		; option 150 #žžž
	; talk
; endproc 

proc 500
	GET_DAYONLINE 1 10
	
	var 11 @10
	if 10 >= 3600
		dev 10 3600
		MOD 11 3600
		dev 11 60
	else
		var 10 0
		dev 11 60
	endif

	dialog 1
	add_talk $_npc$:
	add_talk #b#b����������ʱ��Ϊ��$@10$ʱ$@11$��

		option 501 #��ȡ������ͬ
	talk
endproc 

proc 501

	get_role_var 49 8 1 14

	if 14 = 1
	
		add_sys_msg_start
		add_sys_msg �������Ѿ���ȡ���ˣ�
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	strvar lv
	GET_PLAYER_BASE_ATT 1 1 lv 1
	if lv < 40
		
		add_sys_msg_start
		add_sys_msg ��ļ���̫�ͣ�����40����������ȡ������
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif

	GET_DAYONLINE 1 10
	if 10 < 3600
		dialog 1
		add_talk $_npc$:
		add_talk #b#b�Բ�������������ʱ�䲻��1Сʱ��������ȡ������
		option 1 ����
		talk
	
	else
		strvar num
		GET_PACK_SPACE 0 num
		if num > 0
			ADD_BIND_ITEM_NUM_INPAK 5060023 10 1 1
			
			var 14 1
			set_role_var 49 8 @14 11
			
			GET_TIME_SEC time
			set_role_var 32 32 @time 1	
		
		else
		
			add_sys_msg_start
			add_sys_msg �����������޷���ȡ����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc


proc 550
	GET_DAYONLINE 1 10
	
	var 11 @10
	if 10 >= 3600
		dev 10 3600
		MOD 11 3600
		dev 11 60
	else
		var 10 0
		dev 11 60
	endif

	dialog 1
	add_talk $_npc$:
	add_talk #b#b����������ʱ��Ϊ��$@10$ʱ$@11$��
		
		option 551 #��ȡ��¯ƾ֤
	talk
endproc
proc 551
		
	get_role_var 50 8 1 15
	if 15 = 1
		add_sys_msg_start
		add_sys_msg �������Ѿ���ȡ���ˣ�
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	strvar lv
	GET_PLAYER_BASE_ATT 1 1 lv 1
	
	if lv < 40
		
		add_sys_msg_start
		add_sys_msg ��ļ���̫�ͣ�����40����������ȡ������
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif

	GET_DAYONLINE 1 10
	if 10 < 7200
		dialog 1
		add_talk $_npc$:
		add_talk #b#b�Բ�������������ʱ�䲻��2Сʱ��������ȡ������
		option 1 ����
		talk
	else
			; strvar lv
		; GET_PLAYER_BASE_ATT 1 1 lv 1
		strvar num
		GET_PACK_SPACE 0 num
		if num > 0
		
			if lv >= 40 and lv < 50
				ADD_BIND_ITEM_NUM_INPAK 5052316 10 1 1
			endif
			if lv >= 50 and lv < 60
				ADD_BIND_ITEM_NUM_INPAK 5052317 20 1 1
			endif
			if lv >= 60 and lv < 70
				ADD_BIND_ITEM_NUM_INPAK 5052318 30 1 1
			endif
			if lv >= 70
				ADD_BIND_ITEM_NUM_INPAK 5052319 40 1 1
			endif
			
			var 15 1
			set_role_var 50 8 @15 1
				
			
			GET_TIME_SEC time
			set_role_var 32 32 @time 1	
		else
			
			add_sys_msg_start
			add_sys_msg �����������޷���ȡ����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc 

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#b#b �������ͺ����������ǻԡ�
	add_talk #b#b#b#b#b#b#b#c1865061��27��-2��10��#n
	add_talk #b#b
	add_talk #b#b#bÿ�����ȡ1�Ρ��ǻ���͸����
	add_talk #b#b#c005ebeÿ����ȡ���ͳ�3�����ǻ���͸������
	
	STRVAR chunjie
	GET_EVENT_STAT chunjie 302
	
	if chunjie = 1
		option 301 #��ϲ���ƣ��ǻ�����
	endif
	
	talk 
endproc


proc 301
	strvar num chunjie zhi
	GET_PACK_SPACE 0 num
	GET_EVENT_STAT chunjie 302
	GET_PLAYER_DAILY_FLAG 0 6 zhi
	
	if chunjie = 1
		if zhi = 0
				if @num < 1
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ճ�1�񱳰�������ǰ����ȡ
					SET_SYS_MSG_MODE 3 0 0
					MSG_INFO
				else
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052297 3 1 0
					
					set_player_daily_flag 0 6 1
					
					add_sys_msg_start
					add_sys_msg ҡǮ�����͵õ�����3�����ǻ���͸������
					set_sys_msg_mode 3 0 0
					msg_info
				endif
		else
			ADD_SYS_MSG_START
			ADD_SYS_MSG ���������ȡ��ҡǮ���ġ��ǻ���͸�������񣬲���̫̰��Ŷ
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		endif
	else
		add_sys_msg_start
		add_sys_msg ���ꡰ���ں��񡱻�Ѿ������ˣ���ʱ�޷���ȡ
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

