proc 1
	dialog 1
	add_talk $_npc$:
	strvar yue ri
	GET_NOW_TIME yue ri 0 0 0 0 0
	
	if ( yue = 11 and ri = 28 ) or ( yue = 11 and ri = 29 ) or ( yue = 11 and ri = 30 ) or ( yue = 12 and ri = 1 ) or ( yue = 12 and ri = 2 )
		add_talk #b#b˭�����й���Ҫ������ʱ�����ǵ�һ��С���⣬������˵����Ī��Ķ��ݡ�
		add_talk #b#bƽ�ն���˴�ҵİ��������Σ����С����һ���������������ң�ϣ��������и���ů�ĸж��ڡ�
	else
		add_talk #b#b��λ���ѣ�����������棬����Ƿ���������������ĺò��ϣ�ά�������ƽ�Ϳ����ˡ��ȵȣ��ȱ��߰����ͼ���СǮ�����ɡ�
		add_talk #b#b�����𼱣���Ϊһ�����������˺������������Ǯ�ģ��ٺ١���
		add_talk #b#b͵͵�����㣬��������в����������!�ٺ١����㶮��
	endif
		add_talk #b#b
	strvar jnum
	GOLD_RATE_GET_DAYNUM jnum
		
	if jnum > 5
		sub jnum 5
		add_talk #c0040FF���վ��״�����5/5 #n
		add_talk #c0040FF��������Ȩ���״�����$@jnum$/3 #n
	else
	
		add_talk #c0040FF���վ��״�����$@jnum$/5 #n
		add_talk #c0040FF��������Ȩ������״�����0/3 #n
	
	endif
	strvar time ret
	get_role_var 27 32 1 time 
	get_role_var 31 8 1 10
	is_same_day @time ret
	if ret = -1
		var 10 0
		set_role_var 31 8 @10 1
	endif
	if 10 = 0
		add_talk #c0040FFˮ��������Ȩ��Ѵ�����0/1 #n
	else
		add_talk #c0040FFˮ��������Ȩ��Ѵ�����1/1 #n
	endif
	
;	TASK_STAT 3115 1 20
	GET_PLAYER_TASK_SPRITE_FLAG 1 20
	if 20 > 0
	;  	task_add_option 3115 1 99
	endif
	
	;��һ�
	task_stat 4436 1 51
	if 51 = 2
		option 510 ����ο�ʡ�����������
	endif
	
	task_add_option 3116 1 98
	task_add_option 3117 1 97
	task_add_option 3148 1 96
	
	strvar RetTsk
	task_stat 3126 1 RetTsk
	
	if RetTsk = 2
		option 200 ����ʥ���Ϲ������������
	endif
	
	if ( yue = 11 and ri = 28 ) or ( yue = 11 and ri = 29 ) or ( yue = 11 and ri = 30 ) or ( yue = 12 and ri = 1 ) or ( yue = 12 and ri = 2 )
		option 300 ���˺��ġ��ж��ڡ����������
	endif
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300916 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 �����뻻����
		endif
	endif
	
;	option 100 ʩ��100����
;	option 300 ʩ��1000����
;	option 500 ʩ��3000����

	;task_add_option 3110 1 99
	
	strvar retm
	GOLD_RATE_GET_MONEY retm
	if retm != -1
		if retm >= 0 and retm <= 4998
			option 801 ����4998����
		else
			option 800 ����$@retm$����
		endif
	endif
	
	option 822 #����ˮ��������Ȩ��������1��
	
	talk

endproc 
proc 800
	PKG_IS_UNLOCK 10 20
	if 10 = 0 and 20 = 0
		return
	endif
	
	strvar jnum
	GOLD_RATE_GET_DAYNUM jnum
	GET_STAR_INFO 2 20
	if 20 = 1
	
		if jnum >= 8
			add_sys_msg_start
			add_sys_msg ���˺��Ľ�������ʣ�޼���ֻ���ƴ�����ľ���
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
	else
		if jnum >= 5
			add_sys_msg_start
			add_sys_msg ���˺��Ľ�������ʣ�޼���ֻ���ƴ�����ľ���
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
	endif
	strvar spret mret
	GET_PACK_SPACE 0 spret
	if spret > 0
		GOLD_RATE_DEC_MONEY mret 
		if mret = -1
			add_sys_msg_start
			add_sys_msg �������˺���æ����Ǯ���ƺ�û������Ĵ���
			set_sys_msg_mode 3 0 0
			msg_info
		 
			return
		elseif mret = -2
			add_sys_msg_start
			add_sys_msg �´����Զ�������ң�˵����������ջ��ڵ�����
			set_sys_msg_mode 3 0 0
			msg_info
			
			BUY_HETONG 1
			;���Ҳ����򿪿�ݹ���ʽ
			
			return
		else
			GET_ID_PLAYER_INFO 0 8 30
			rand 40 100
			if 40 = 0
			
				
				add_sys_msg_start
				if 30 < 60
					ADD_ITEM_NUM_INPAK 5200054 1 0 1
					add_sys_msg ���˺�����������һ�����߽�Ũ��ת��ʯ�����ƺ����˶�û����
				endif
				if 30 >= 60 and 30 < 70
					ADD_ITEM_NUM_INPAK 5200055 1 0 1
					add_sys_msg ���˺�����������һ�����߽׽���ת��ʯ�����ƺ����˶�û����
				endif
				if 30 >= 70 and 30 < 80
					ADD_ITEM_NUM_INPAK 5200056 1 0 1
					add_sys_msg ���˺�����������һ�����߽׹���ת��ʯ�����ƺ����˶�û����
				endif
				if 30 >= 80
					ADD_ITEM_NUM_INPAK 5200057 1 0 1
					add_sys_msg ���˺�����������һ�����߽׻�ѧת��ʯ�����ƺ����˶�û����
				endif
				set_sys_msg_mode 3 0 0
				msg_info
				
			else
				ADD_BIND_ITEM_NUM_INPAK 5052111 1 1 0
				add_sys_msg_start
				add_sys_msg ���˺�С��������ͳ�һ����������ҡ�����������
				set_sys_msg_mode 3 0 0
				msg_info
			endif
			
			GET_EVENT_STAT 36 99
			if @36 = 1
			;�������ڽ�����
				ADD_BIND_ITEM_NUM_INPAK 5052118 2 1 37
				if @37 = 0
					strvar npcmid
					GET_MAP_NPC_ID  304013 npcmid 1
					MON_SPEECH  @npcmid 0 0 1 ֵ������ѽ�֮�ʣ��������±��͵����ǻ�������л�񣬶�л����ô��ʱ��İ�����
				endif
			endif
			
			call 811
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ѷŲ��½��ң����˺�ֻ���ƴ�����ľ���
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
endproc 

proc 801
	PKG_IS_UNLOCK 10 20
	if 10 = 0 and 20 = 0
		return
	endif
	
	strvar jnum
	GOLD_RATE_GET_DAYNUM jnum
	GET_STAR_INFO 2 20
	if 20 = 1
	
		if jnum >= 8
			add_sys_msg_start
			add_sys_msg ���˺��Ľ�������ʣ�޼���ֻ���ƴ�����ľ���
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
	else
		if jnum >= 5
			add_sys_msg_start
			add_sys_msg ���˺��Ľ�������ʣ�޼���ֻ���ƴ�����ľ���
			set_sys_msg_mode 3 0 0
			msg_info
			return
		endif
	endif
	strvar spret mret
	GET_PACK_SPACE 0 spret
	if spret > 0
		; GOLD_RATE_DEC_MONEY mret
		dec_money 0 4998 20 mret
		if mret = -1
			add_sys_msg_start
			add_sys_msg �´����Զ�������ң�˵����������ջ��ڵ�����
			set_sys_msg_mode 3 0 0
			msg_info
			
			BUY_HETONG 1
			; ���Ҳ����򿪿�ݹ���ʽ
			
			return
		
		; if mret = -1
			; add_sys_msg_start
			; add_sys_msg �������˺���æ����Ǯ���ƺ�û������Ĵ���
			; set_sys_msg_mode 3 0 0
			; msg_info
		 
			; return
		; elseif mret = -2
			; add_sys_msg_start
			; add_sys_msg �´����Զ�������ң�˵����������ջ��ڵ�����
			; set_sys_msg_mode 3 0 0
			; msg_info
			
			; BUY_HETONG 1
			;���Ҳ����򿪿�ݹ���ʽ
			
			; return
		else
		GOLD_RATE_DAYNUM_ADD 1
			if 1 = 0
				GET_ID_PLAYER_INFO 0 8 30
				rand 40 100
				if 40 = 0
				
					
					add_sys_msg_start
					if 30 < 60
						ADD_ITEM_NUM_INPAK 5200054 1 0 1
						add_sys_msg ���˺�����������һ�����߽�Ũ��ת��ʯ�����ƺ����˶�û����
					endif
					if 30 >= 60 and 30 < 70
						ADD_ITEM_NUM_INPAK 5200055 1 0 1
						add_sys_msg ���˺�����������һ�����߽׽���ת��ʯ�����ƺ����˶�û����
					endif
					if 30 >= 70 and 30 < 80
						ADD_ITEM_NUM_INPAK 5200056 1 0 1
						add_sys_msg ���˺�����������һ�����߽׹���ת��ʯ�����ƺ����˶�û����
					endif
					if 30 >= 80
						ADD_ITEM_NUM_INPAK 5200057 1 0 1
						add_sys_msg ���˺�����������һ�����߽׻�ѧת��ʯ�����ƺ����˶�û����
					endif
					set_sys_msg_mode 3 0 0
					msg_info
				
				else
					ADD_BIND_ITEM_NUM_INPAK 5052111 1 1 0
					add_sys_msg_start
					add_sys_msg ���˺�С��������ͳ�һ����������ҡ�����������
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			
				GET_EVENT_STAT 36 99
				if @36 = 1
				;�������ڽ�����
					ADD_BIND_ITEM_NUM_INPAK 5052118 2 1 37
					if @37 = 0
						strvar npcmid
						GET_MAP_NPC_ID  304013 npcmid 1
						MON_SPEECH  @npcmid 0 0 1 ֵ������ѽ�֮�ʣ��������±��͵����ǻ�������л�񣬶�л����ô��ʱ��İ�����
					endif
				endif
			call 811
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ѷŲ��½��ң����˺�ֻ���ƴ�����ľ���
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
endproc

proc 822 

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
	
	if 20 = 1
		get_role_var 31 8 1 10
		if 10 < 1
			GET_ID_PLAYER_INFO 0 8 30
			rand 40 100
			if 40 = 0
				add_sys_msg_start
				if 30 < 60
					ADD_ITEM_NUM_INPAK 5200054 1 0 1
					add_sys_msg ���˺�����������һ�����߽�Ũ��ת��ʯ�����ƺ����˶�û����
				endif
				if 30 >= 60 and 30 < 70
					ADD_ITEM_NUM_INPAK 5200055 1 0 1
					add_sys_msg ���˺�����������һ�����߽׽���ת��ʯ�����ƺ����˶�û����
				endif
				if 30 >= 70 and 30 < 80
					ADD_ITEM_NUM_INPAK 5200056 1 0 1
					add_sys_msg ���˺�����������һ�����߽׹���ת��ʯ�����ƺ����˶�û����
				endif
				if 30 >= 80
					ADD_ITEM_NUM_INPAK 5200057 1 0 1
					add_sys_msg ���˺�����������һ�����߽׻�ѧת��ʯ�����ƺ����˶�û����
				endif
				
				set_sys_msg_mode 3 0 0
				msg_info
			else
				ADD_BIND_ITEM_NUM_INPAK 5052111 1 1 0
				add_sys_msg_start
				add_sys_msg ���˺�С��������ͳ�һ����������ҡ�����������
				set_sys_msg_mode 3 0 0
				msg_info
			endif
			
			GET_EVENT_STAT 36 99
			if @36 = 1
			;�������ڽ�����
				ADD_BIND_ITEM_NUM_INPAK 5052118 2 1 37
				if @37 = 0
					strvar npcmid
					GET_MAP_NPC_ID  304013 npcmid 1
					MON_SPEECH  @npcmid 0 0 1 ֵ������ѽ�֮�ʣ��������±��͵����ǻ�������л�񣬶�л����ô��ʱ��İ�����
				endif
			endif
			
			call 811
			GET_TIME_SEC time
			set_role_var 27 32 @time 1			
			add 10 1
			set_role_var 31 8 @10 1	
			
		else
			add_sys_msg_start
			add_sys_msg ��Ҫ�����Σ��������˼Һ��۸���
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ֪����ô��ɸ߸�˧�𣿰���O�����ԡ���
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ͻ�������������ͬ�Ǻ������һ�������ů������������ģ�лл�㣬$_player$���һ�������ĵģ����ҷ��˲�һ�������㣡
		option 201 #����ʥ������
	talk
endproc 
proc 201
	GET_ITEM_NUM_INPAK 5300844 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300844 1 30
		if 30 != -1
			task_fini 3126 1
		endif
	endif
endproc 


proc 100
	strvar Lock
        pkg_is_unlock 1 Lock
        if Lock = 0
		
                return
        endif
	strvar m
	var m 100
	DEC_MONEY_TEST 0 @m 20 1
	 if @1 = -1
                ADD_SYS_MSG_START
                TASK_STAT 3115 1 5
		if 1 = 0 and 5 = 0
			 ADD_SYS_MSG �´����Զ�������ң�˵������������ջ�
		else
			 ADD_SYS_MSG ���ϵ����Ҳ��㹻ʩ�ᡣ
		endif
                SET_SYS_MSG_MODE 1 0 0
                MSG_INFO
				
			BUY_HETONG 1
			;���Ҳ����򿪿�ݹ���ʽ
			
		return
        endif
	
	DEC_MONEY 0 @m 20 1
	
	;ͨ���¼���
	strvar num
	var num 21
	;�������10000Ϊ����
	strvar r
	;���¼�����31��ʼ
	;70�����¸���
	var 31 1000 

;	var 32 4000 
;	var 33 1000 
;	var 34 100 

	var 32 0 
	var 33 0 
	var 34 0 

	var 35 0
	var 36 0
	var 37 0

	var 38 400
	var 39 400
	var 40 400
	var 41 400
	var 42 400

	;���¼�proc ��61��ʼ
	var 61 101 
	var 62 102 
	var 63 103 
	var 64 104
	var 65 105 
	var 66 106 
	var 67 107

	var 68 108
	var 69 109
	var 70 110
	var 71 111
	var 72 112

	strvar Ret Sex Level Job
        get_player_base_att  Ret Sex Level Job
	
	;70�������
;	if @level >= 70
;		var 32 0 
;		var 33 0 
;		var 34 0 
;		;û������
;		TASK_STAT 3115 1 5
;		if 1 = 0 and 5 = 0
;
;			var 35 4000 
;			var 36 1000 
;			var 37 5
;		endif
;	endif

	strvar stat



	rand r 10000

		if r < @31
		call @61
		return
	endif
	sub r @31
	if r < @32
		call @62
		return
	endif
	sub r @32
	if r < @33
		call @63
		return
	endif
	sub r @33
	if r < @34
		call @64
		return
	endif
	sub r @34
	if r < @35
		call @65
		return
	endif
	sub r @35
	if r < @36
		call @66
		return
	endif
	sub r @36
	if r < @37
		call @67
		return
	endif
	sub r @37

		if r < @38
		call @68
		return
	endif
	sub r @38
	if r < @39
		call @69
		return
	endif
	sub r @39
	if r < @40
		call @70
		return
	endif
	sub r @40
	if r < @41
		call @71
		return
	endif
	sub r @41
	if r < @42
		call @72
		return
	endif
	sub r @42

	;proc ��
	strvar p
	var p 20
	mod r @num
	add p @r

;	dialog 1
	add_talk $_npc$:
;	add_talk #b#b100��$@r$.....$@p$..
;	talk
	;return
	if P > 40
		var p 20
	endif

	call @p

endproc

proc 108
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���������Իر��Ĵ�л��


	talk
	
	strvar id
	GET_PLAYER_ID 1 id
	
	target_add_status 1 @id 3004 1 1800 1
	
endproc

proc 109
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���������Իر��Ĵ�л��


	talk
	
	strvar id
	GET_PLAYER_ID 1 id
	
	target_add_status 1 @id 3004 2 1800 1
	
endproc

proc 110
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���������Իر��Ĵ�л��


	talk
	
	strvar id
	GET_PLAYER_ID 1 id
	
	target_add_status 1 @id 3004 3 1800 1
	
endproc

proc 111
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���������Իر��Ĵ�л��


	talk
	
	strvar id
	GET_PLAYER_ID 1 id
	
	target_add_status 1 @id 3004 4 1800 1
	
endproc

proc 112
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���������Իر��Ĵ�л��


	talk
	
	strvar id
	GET_PLAYER_ID 1 id
	
	target_add_status 1 @id 3004 5 1800 1
	
endproc

; proc 300
	; strvar Lock
        ; pkg_is_unlock 1 Lock
        ; if Lock = 0
		
                ; return
        ; endif
	; strvar m
	; var m 1000
	; DEC_MONEY_TEST 0 @m 20 1
	 ; if @1 = -1
                ; ADD_SYS_MSG_START
                ; TASK_STAT 3115 1 5
		; if 1 = 0 and 5 = 0
			 ; ADD_SYS_MSG �´����Զ�������ң�˵������������ջ�
		; else
			 ; ADD_SYS_MSG ���ϵ����Ҳ��㹻ʩ�ᡣ
		; endif
                ; SET_SYS_MSG_MODE 1 0 0
                ; MSG_INFO
			
				; BUY_HETONG 1
				;���Ҳ����򿪿�ݹ���ʽ	
				
		; return
		
			
        ; endif

	; DEC_MONEY 0 @m 20 1

	;ͨ���¼���
	; strvar num
	; var num 21
	;�������10000Ϊ����
	; strvar r
	;���¼�����31��ʼ
	;70�����¸���
	; var 401 1000
	; var 402 500
	; var 403 500
	; var 404 500
	; var 405 500

					;	var 406 1000
					;	var 407 4000
					;	var 408 300
	
	; var 406 0
	; var 407 0
	; var 408 0

	; var 409 0
	; var 410 0
	; var 411 0
	
	;���¼�proc ��701��ʼ
	; var 701 301 
	; var 702 302 
	; var 703 303 
	; var 704 304 
	; var 705 305 
	; var 706 306 
	; var 707 307 
	; var 708 308 
	; var 709 309 
	; var 710 310 
	; var 711 311 

	; strvar Ret Sex Level Job
        ; get_player_base_att  Ret Sex Level Job
	
					;70�������,�¼���
					;	if @level >= 70
					;		
					;
					;			var 406 0
					;			var 407 0
					;			var 408 0
					;		;û������
					;		TASK_STAT 3115 1 5
					;		if 1 = 0 and 5 = 0
					;			var 409 1000
					;			var 410 4000
					;			var 411 53
					;		endif
					;	endif

	; rand r 10000

		; if r < @401
		; call @701
		; return
	; endif
	; sub r @401

	; if r < @402
		; call @702
		; return
	; endif
	; sub r @402

	; if r < @403
		; call @703
		; return
	; endif
	; sub r @403

	; if r < @404
		; call @704
		; return
	; endif
	; sub r @404

	; if r < @405
		; call @705
		; return
	; endif
	; sub r @405

	; if r < @406
		; call @706
		; return
	; endif
	; sub r @406

	; if r < @407
		; call @707
		; return
	; endif
	; sub r @407

	; if r < @408
		; call @708
		; return
	; endif
	; sub r @408

	; if r < @409
		; call @709
		; return
	; endif
	; sub r @409

	; if r < @410
		; call @710
		; return
	; endif
	; sub r @410

	; if r < @411
		; call @711
		; return
	; endif
	; sub r @411

	
	;proc ��
	; strvar p
	; var p 20
	; mod r @num
	; add p @r

	; if P > 40
		; var p 20
	; endif

	; call @p
; endproc

proc 500
	strvar Lock
        pkg_is_unlock 1 Lock
        if Lock = 0
		
                return
        endif

	strvar m
	var m 3000

	DEC_MONEY_TEST 0 @m 20 1
	 if @1 = -1
                ADD_SYS_MSG_START
		TASK_STAT 3115 1 5
		if 1 = 0 and 5 = 0
			 ADD_SYS_MSG �´����Զ�������ң�˵������������ջ�
		else
			 ADD_SYS_MSG ���ϵ����Ҳ��㹻ʩ�ᡣ
		endif
               
                SET_SYS_MSG_MODE 1 0 0
                MSG_INFO
				
				BUY_HETONG 1
				;���Ҳ����򿪿�ݹ���ʽ
				
		return
        endif

	DEC_MONEY 0 @m 20 1

	;ͨ���¼���
	strvar num
	var num 21
	;�������10000Ϊ����
	strvar r
	;���¼�����401��ʼ
	;70�����¸���
	var 401 1
	var 402 1000
;	var 403 4000
;	var 404 1000
;	var 405 500

	var 403 0
	var 404 0
	var 405 0

	var 406 0
	var 407 0
	var 408 0

	;�Ѿ��ƺ�������
	var 1 0
	GET_PLAYER_ID 1 id
	HAS_ID_PLAYER_DESIGNATION @id 123 1
	if @1 = 1
		var 401 0
	endif
	
	;���¼�proc ��701��ʼ
	var 701 501 
	var 702 502 
	var 703 503 
	var 704 504 
	var 705 505 
	var 706 506 
	var 707 507 
	var 708 508 

	strvar Ret Sex Level Job
        get_player_base_att  Ret Sex Level Job
	
	;70�������,�¼���
	if @level >= 70

		var 403 0
		var 404 0
		var 405 0

		;û������
		TASK_STAT 3115 1 5
		if 1 = 0 and 5 = 0
			var 406 4000
			var 407 1000
			var 408 160
		endif
	endif

	rand r 10000

	if r < @401
		call @701
		return
	endif
	sub r @401

	if r < @402
		call @702
		return
	endif
	sub r @402

	if r < @403
		call @703
		return
	endif
	sub r @403

	if r < @404
		call @704
		return
	endif
	sub r @404

	if r < @405
		call @705
		return
	endif
	sub r @405

	if r < @406
		call @706
		return
	endif
	sub r @406

	if r < @407
		call @707
		return
	endif
	sub r @407

	if r < @408
		call @708
		return
	endif
	sub r @408

	

	
	;proc ��
	strvar p
	var p 20
	mod r @num
	add p @r

	if P > 40
		var p 20
	endif

	call @p
endproc



;ͨ���¼�
proc 20
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪���Ҳ��ݿ����Զ����Լ���Ҫ�ĵ��ߣ����ܣ�����ս��ʱ��ʹ�á�
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 21
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪����С��ͼ�ϵ����Ӧ��λ�ã���������Զ�������Ӧ�ĵط���
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 22
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪�����ﵽ����ʮ��ʱ������ǰ��ʱ��ǵġ��������Ա�����������Լ��Ĺ��ᡣ
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 23
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪����������ʱ��ǵġ��������Ա������ͨ���������ѯ�����ܣ�ѡ��Ҫ����Ĺ��ᡣ
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 24
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪���������ڡ���Ϸ���á��ġ���Ϣ���á������ø���ƫ�õ�ͷ����ʾ��Ϣ��
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 25
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪���������ڡ���Ϸ���á��ġ��������á������ø���ƫ�õĿ�ݼ���
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 26
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪����F11�����԰���Һͳ������������������
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 27
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪����F12�����԰�̯λ������Ͷ�������������������������
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 28
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪����TAB�����Զ�ѡ����Χ�Ĺ֣�
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 29
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪���������;���������⵼���޷���ɣ����Է�������������ȡ��
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 30
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪���������˴���������ͷ�����������ҵ��������ĳ��ﵰ��
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 31
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪���������˴���ɽ����Ұ�����������ҵ���Ұ�������ĳ��ﵰ��
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 32
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪����ɳ�Ƕ������������潱�º������������ƺ�����ʱ��ǵ�÷�ܻ�ͷƻ�ȡ�����װ����
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 33
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪�����ּ��껪���������ܿ�������ʿ֮�ꡱ���
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 34
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪�����޷籩ս��������ս����º������������ƺ�����ʱ��ǵ�÷�ܻ�ͷƻ�ȡ�����װ����
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 35
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪����\�����԰ѽ�������������
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 36
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪���������˴ӻƽ���ǲ����ո�����ҵ������ߵĳ��ﵰ��
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 37
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪��ѧ��һ��Ƽ�����רҵ�����������ܶ��򲻵��Ķ�����
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 38
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪���̻�Ļ���������Ի�ô����Ľ�Ǯ��
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ����Ǻ�

	talk
endproc

proc 39
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪����ʱ��ǵ�������Դ��͵��ʺ絺��׽�����ߡ�Ұ����������������
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ��Ǻ�

	talk
endproc

proc 40
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	add_talk #b#b������֪�����֮�Ŀ��Ի����������������������Լ��
	add_talk #b#b����
	add_talk #b#b�����𼱣���֪����С���ܻ������أ�ֻ�ǡ��Ǻ�

	talk
endproc

;100�����¼�
proc 101
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�Ҷ�û������ô���ĵ��ˣ������ǩ����������
	talk
	strvar id
	GET_PLAYER_ID 1 id
	;С����30����
	target_add_status 1 @id 3005 1 1800 1
endproc

proc 102
	dialog 1
	add_talk $_npc$:
		add_talk #b#b���������Ǯ�ķ��ϣ��Ҿ����ĸ�������ˡ�
	add_talk #b#b�����������Դ����Ÿ��־��飬�������Ϊ����������Ԫ�ؾ��飬���Ƿֱ��ǣ�
	add_talk #b#b���񡪡���Ԫ��
	add_talk #b#bɽ�񡪡���Ԫ��
	add_talk #b#b���񡪡���Ԫ��
	add_talk #b#bӰ�ӡ�����Ԫ��
	add_talk #b#b��ʹ������Ԫ��
	add_talk #b#b�������ٴ��������ʱ����Щ���齫��ʼ��ѡ�������ǵ����ߣ��ӹ��ڴˣ��������磡
	add_talk #b#b��ʱ��ֻ��˵�����ˣ���֪���ģ��Ǻǡ��㶮�ġ�

	talk
endproc

proc 103
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��˵�еķ��������ű��磡��Ψһû��ʵ��ľ��飬ֻҪ������֮������������Ϊƽ����
	add_talk #b#bȻ�������񱻷�ӡ�������ӡ�����Ļ�����ʧ�����������ϡ�����˵������ĳ������У�����˵������ĳ������̤���ĵط�����������˵��������������Ѿ����ݻ��ˡ�
	add_talk #b#b����˵�������ӡ�ŷ���Ļ����Դ��������ϣ�����㻹��֪�����࣬������Զ��������ң��Ǻǡ��㶮�ġ�

	talk
endproc

proc 104
	dialog 1
	add_talk $_npc$:
		add_talk #b#b�������ģ�����������λ����֮�ӣ���������ʵ��Ը�����ͰѼ��������ഫ������ϻ���͸�����
	add_talk #b#b��Ȼ�㲻ʧΪ�������⣬�������ڵ�ʵ��������������ʵ��Ը���� 70�����������Ұɣ�����������˰��ŵ��Ǹ��ˣ��һ��һ�ж������㡣

	talk
endproc

proc 105
	dialog 1
	add_talk $_npc$:
		add_talk #b#b��Ǯ�ǲ��࡭���Ҽ�Ȼ�յ���Ǯ�Ϳ϶������š�
	add_talk #b#b��������ҵ���������˯�Ѿõľ������ѿ�ʼ�����������Ĵ���ѡ�������ǵ����ߣ�����Щ����ѡ�ϵ��ˣ����ǳ�֮Ϊ����֮�ӡ�
	add_talk #b#b���з�ӡ����������������˵���������۲��ݵ�Ӧ�þ��Ƿ���Ļ����ˣ�������㣬�Ҹ���Ҳ��һ��СС�Ľ��ۡ�
	add_talk #b#b���ڽ�����Ρ��������������鱨�����Ƿ�Ӧ���ٸ�����Ŭ���ȼ۽������Ǻǡ��㶮�ġ�
	talk
endproc

proc 106
	dialog 1
	add_talk $_npc$:
	add_talk #b#bÿ���˶���Ϊ��ӡ�ŷ���Ļ�������ʧ�����������ϣ�����һֱ�и�������û�и����κ��ˡ�
	add_talk #b#b�ҵļ��������ഫ��һ������ϻ�ӣ���������ĵ��飬�����������������Ϣ�����ܿ϶����ϻ�Ӿ���װ���ŷ���Ļ�����
	add_talk #b#b��Ȼ�������Ѿ�һƶ��ϴ����ϻ�Ӵ�δ�뿪��߰벽����������˽����ϻ�ӵ����飬�������������֮���С���ܡ����⡭���Ͽ���û�е��������ڱ��أ��Ǻǡ��㶮�ġ�

	talk
endproc

proc 107
	dialog 1
	add_talk $_npc$:
	
	add_talk #b#b��������г��⣬����������������ķ��ϣ���Ϊ���������������������˺����ҾͰ��㵱������֮�Ӱɡ�
	add_talk #b#b�����㻹�ǵ��Ȱ��Ҹ�æ���ҵļ����ഫ����������ϻ�ӣ����п��ܷ�ӡ�ŷ��������㻹�ð���ʵ��Ը����˳��֤�������㹻ӵ�з����������

	task_add_option 3115 1 99
	SET_PLAYER_TASK_SPRITE_FLAG 1 1
	player_set_hide_task 1 3115
	;task_add_option 1001 1 99

	talk

	
endproc

proc 96
	task_do_option 3148 1
endproc

proc 97
	task_do_option 3117 1
endproc

proc 98
	task_do_option 3116 1
endproc

proc 99
	task_do_option 3115 1
endproc
;300����

; proc 301
	; dialog 1
	; add_talk $_npc$:
	; add_talk #b#bԸ�ϵ۱����㣬��л�����˽�������ˡ�
	; GET_PLAYER_ID 1 id
	;���˹��30����
	; target_add_status 1 @id 3005 2 1800 1
	; talk
; endproc

; proc 302
	; dialog 1
	; add_talk $_npc$:
	; add_talk #b#b�����������ϳ�ߵ�ף����
	; GET_PLAYER_ID 1 id
	; �����ƶ�10����
	; target_add_status 1 @id 3148 1 1800 1
	; talk
; endproc

; proc 303
	; dialog 1
	; add_talk $_npc$:
	; add_talk #b#b�����������ϳ�ߵ�ף����
	; GET_PLAYER_ID 1 id
	;��������30����
	; target_add_status 1 @id 3148 2 1800 1
	; talk
; endproc

; proc 304
	; dialog 1
	; add_talk $_npc$:
	; add_talk #b#b�����������ϳ�ߵ�ף����
	; GET_PLAYER_ID 1 id
	;��������30����
	; target_add_status 1 @id 3148 3 1800 1
	; talk
; endproc

proc 305
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�����������ϳ�ߵ�ף����
	GET_PLAYER_ID 1 id
	;���˱���30����
	target_add_status 1 @id 3148 4 1800 1
	talk
endproc

proc 306
	dialog 1
	add_talk $_npc$:
		add_talk #b#b������Ҳ�Ҳ���ᣬֱ��һ�δ����˵�׽Ū���ǹ��������вƲ�������ʣ�µ�һ������ϻ�ӡ������Ҽ���������ഫ�����ģ���Ȼ�����������������ϣ�������һ����ʾ��ڤڤ��ע���ҽ�����ĳЩת�ۡ�
	add_talk #b#b�������ϻ�ӵ����ܣ�����������һ�仰������֮�գ���罫�������ƺ��ڴ���ϻ����װ��ʲô�����˵Ķ���������ʣ�µ���Ϣ������ʱֻ��˵�����ˣ�����㻹���˽�Щʲô���Ǻǡ��㶮�ġ�

	talk
endproc

proc 307
	dialog 1
	add_talk $_npc$:
		add_talk #b#b��˵�еķ��������ű��磡��Ψһû��ʵ��ľ��飬ֻҪ������֮������������Ϊƽ����
	add_talk #b#bȻ�������񱻷�ӡ�������ӡ�����Ļ�����ʧ�����������ϡ�����˵������ĳ������У�����˵������ĳ������̤���ĵط�����������˵��������������Ѿ����ݻ��ˡ�
	add_talk #b#b����˵�������ӡ�ŷ���Ļ����Դ��������ϣ�����㻹��֪�����࣬������Զ��������ң��Ǻǡ��㶮�ġ�

	talk
endproc

proc 308
	dialog 1
	add_talk $_npc$:
		add_talk #b#b�������ģ�����������λ����֮�ӣ���������ʵ��Ը�����ͰѼ��������ഫ������ϻ���͸�����
	add_talk #b#b��Ȼ�㲻ʧΪ�������⣬�������ڵ�ʵ��������������ʵ��Ը���� 70�����������Ұɣ�����������˰��ŵ��Ǹ��ˣ��һ��һ�ж������㡣

	talk
endproc

proc 309
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�������ģ�����������λ����֮�ӣ���������ʵ��Ը�����ͰѼ��������ഫ������ϻ���͸�����
	add_talk #b#b���������ϻ���������ҵ���ߣ�������һ�����˵���ʾ������������Ҳû�á�����ô˵��Ҳ��������������������ģ�����˺��������ǲ������˰��ž�Ҫ���������ж࿶���ˡ�

	talk
endproc

proc 310
	dialog 1
	add_talk $_npc$:
	add_talk #b#bÿ���˶���Ϊ��ӡ�ŷ���Ļ�������ʧ�����������ϣ�����һֱ�и�������û�и����κ��ˡ�
	add_talk #b#b�ҵļ��������ഫ��һ������ϻ�ӣ���������ĵ��飬�����������������Ϣ�����ܿ϶����ϻ�Ӿ���װ���ŷ���Ļ�����
	add_talk #b#b��Ȼ�������Ѿ�һƶ��ϴ����ϻ�Ӵ�δ�뿪��߰벽����������˽����ϻ�ӵ����飬�������������֮���С���ܡ����⡭���Ͽ���û�е��������ڱ��أ��Ǻǡ��㶮�ġ�


	talk
endproc

proc 311
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������г��⣬����������������ķ��ϣ���Ϊ���������������������˺����ҾͰ��㵱������֮�Ӱɡ�
	add_talk #b#b�����㻹�ǵ��Ȱ��Ҹ�æ���ҵļ����ഫ����������ϻ�ӣ����п��ܷ�ӡ�ŷ��������㻹�ð���ʵ��Ը����˳��֤�������㹻ӵ�з����������

	task_add_option 3115 1 99
	SET_PLAYER_TASK_SPRITE_FLAG 1 1
	player_set_hide_task 1 3115
	talk
endproc

;500
proc 501
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���Ǻ��ˣ�	
	add_talk #b#b#
	add_talk #b#b#
	add_talk #b#b#W��� #c186506[�ƺţ�������]#n
	talk
	;�ƺ�������
	var 1 0
	GET_PLAYER_ID 1 id
	HAS_ID_PLAYER_DESIGNATION @id 123 1
	if @1 != 1
		add_designation 1 123
	endif
endproc

proc 502
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ֱ����ҵ���ķ緶���뵱����Ҳ���Ǹ����塣
	GET_PLAYER_ID 1 id
	;��Ǯ��30����
	target_add_status 1 @id 3005 3 1800 1
	talk
endproc

proc 503
	dialog 1
	add_talk $_npc$:
	add_talk #b#b������Ҳ�Ҳ���ᣬֱ��һ�δ����˵�׽Ū���ǹ��������вƲ�������ʣ�µ�һ������ϻ�ӡ������Ҽ���������ഫ�����ģ���Ȼ�����������������ϣ�������һ����ʾ��ڤڤ��ע���ҽ�����ĳЩת�ۡ�
	add_talk #b#b�������ϻ�ӵ����ܣ�����������һ�仰������֮�գ���罫�������ƺ��ڴ���ϻ����װ��ʲô�����˵Ķ���������ʣ�µ���Ϣ������ʱֻ��˵�����ˣ�����㻹���˽�Щʲô���Ǻǡ��㶮�ġ�
	talk
endproc

proc 504
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��˵�еķ��������ű��磡��Ψһû��ʵ��ľ��飬ֻҪ������֮������������Ϊƽ����
	add_talk #b#bȻ�������񱻷�ӡ�������ӡ�����Ļ�����ʧ�����������ϡ�����˵������ĳ������У�����˵������ĳ������̤���ĵط�����������˵��������������Ѿ����ݻ��ˡ�
	add_talk #b#b����˵�������ӡ�ŷ���Ļ����Դ��������ϣ�����㻹��֪�����࣬������Զ��������ң��Ǻǡ��㶮�ġ�
	talk
endproc

proc 505
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�������ģ�����������λ����֮�ӣ���������ʵ��Ը�����ͰѼ��������ഫ������ϻ���͸�����
	add_talk #b#b��Ȼ�㲻ʧΪ�������⣬�������ڵ�ʵ��������������ʵ��Ը���� 70�����������Ұɣ�����������˰��ŵ��Ǹ��ˣ��һ��һ�ж������㡣
	talk
endproc

proc 506
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�������ģ�����������λ����֮�ӣ���������ʵ��Ը�����ͰѼ��������ഫ������ϻ���͸�����
	add_talk #b#b���������ϻ���������ҵ���ߣ�������һ�����˵���ʾ������������Ҳû�á�����ô˵��Ҳ��������������������ģ�����˺��������ǲ������˰��ž�Ҫ���������ж࿶���ˡ�
	talk
endproc

proc 507
	dialog 1
	add_talk $_npc$:
add_talk #b#bÿ���˶���Ϊ��ӡ�ŷ���Ļ�������ʧ�����������ϣ�����һֱ�и�������û�и����κ��ˡ�
add_talk #b#b�ҵļ��������ഫ��һ������ϻ�ӣ���������ĵ��飬�����������������Ϣ�����ܿ϶����ϻ�Ӿ���װ���ŷ���Ļ�����
add_talk #b#b��Ȼ�������Ѿ�һƶ��ϴ����ϻ�Ӵ�δ�뿪��߰벽����������˽����ϻ�ӵ����飬�������������֮���С���ܡ����⡭���Ͽ���û�е��������ڱ��أ��Ǻǡ��㶮�ġ�

	talk
endproc

proc 508
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������г��⣬����������������ķ��ϣ���Ϊ���������������������˺����ҾͰ��㵱������֮�Ӱɡ�
	add_talk #b#b�����㻹�ǵ��Ȱ��Ҹ�æ���ҵļ����ഫ����������ϻ�ӣ����п��ܷ�ӡ�ŷ��������㻹�ð���ʵ��Ը����˳��֤�������㹻ӵ�з����������
	task_add_option 3115 1 99
	SET_PLAYER_TASK_SPRITE_FLAG 1 1
	player_set_hide_task 1 3115
	talk
endproc

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������Σ��������Σ����߽��Σ��������Σ�����׷�Σ�
		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300916 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300916 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300917 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300918 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300919 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300920 1 0 0
			endif
		else
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ������û�п��Խ�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 



; proc 810
	; rand 20 18
	; if 20 = 0
		; call 811
	; endif
	; if 20 = 1
		; call 812
	; endif
	; if 20 = 2
		; call 813
	; endif
	; if 20 = 3
		; call 814
	; endif
	; if 20 = 4
		; call 815
	; endif
	; if 20 = 5
		; call 816
	; endif
	; if 20 = 6
		; call 817
	; endif
	; if 20 = 7
		; call 818
	; endif
	; if 20 = 8
		; call 819
	; endif
	; if 20 = 9
		; call 820
	; endif
	; if 20 = 10
		; call 822
	; endif
	; if 20 = 11
		; call 811
	; endif
	; if 20 = 12
		; call 811
	; endif
	; if 20 = 13
		; call 811
	; endif
	; if 20 = 14
		; call 811
	; endif
	; if 20 = 15
		; call 811
	; endif
	; if 20 = 16
		; call 811
	; endif
	; if 20 = 17
		; call 811
	; endif
; endproc 
proc 811
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������ܣ��Ҿ�˵һ�飬��Ҫ��ϸ���ã�
	rand 20 18
	if 20 = 0
		add_talk #b#b������֪���Ҳ��ݿ��Է�����Ҫ�ĵ��ߣ����ܣ�����ս��ʱ��ʹ�á�
	endif
	if 20 = 1
		add_talk #b#b������֪����С��ͼ�ϵ����Ӧ��λ�ã���������Զ�������Ӧ�ĵط���
	endif
	if 20 = 2
		add_talk #b#b������֪�����ﵽ����ʮ��ʱ������ǰ��ʱ��ǵġ��������Ա�����������Լ��Ĺ��ᡣ
	endif
	if 20 = 3
		add_talk #b#b������֪����������ʱ��ǵġ��������Ա������ͨ���������ѯ�����ܣ�ѡ��Ҫ����Ĺ��ᡣ
	endif
	if 20 = 4
		add_talk #b#b������֪���������ڡ���Ϸ���á��ġ���Ϣ���á������ø���ƫ�õ�ͷ����ʾ��Ϣ��
	endif
	if 20 = 5
		add_talk #b#b������֪���������ڡ���Ϸ���á��ġ��������á������ø���ƫ�õĿ�ݼ���
	endif
	if 20 = 6
		add_talk #b#b������֪����F11�����԰���Һͳ��������������������F12�����԰�̯λ������Ͷ�������������������������
	endif
	if 20 = 7
		add_talk #b#b������֪����TAB�����Զ�ѡ����Χ����Ĺ��
	endif
	if 20 = 8
		add_talk #b#b������֪����\�����԰ѽ�������������
	endif
	if 20 = 9
		add_talk #b#b������֪��ѧ��һ��Ƽ�����רҵ�����������ܶ��򲻵��Ķ�����
	endif
	if 20 = 10
		add_talk #b#b������֪�����֮����������15�㣬���Ի��1��ת�̳齱���ᡣ
	endif
	if 20 = 11
		add_talk #b#b������֪��ʹ�á��ر�ͼ���ƺ����ҵ�����ı�ʯ�ʹ������ҡ�
	endif
	if 20 = 12
		add_talk #b#b������֪��ֻ�м����˹��ᣬ���ܲμӡ��������֡������ᱦ�ء����ػ�������Ŧ���������鱨ս��������ݵ�ս����ʱ�������ս����Щ���
	endif
	if 20 = 13
		add_talk #b#b������֪����ɱһ���������ɲ�׽���ĳ���󣬽��п������ϡ��ɲ�׽���ı�����
	endif
	if 20 = 14
		add_talk #b#b������֪��ѧϰ����רҵ�󣬿�����ʱ��ǵ������ʦ����ȡ���������š�������ɺ󽫻�á��ɿ󳵡���
	endif
	if 20 = 15
		add_talk #b#b������֪��ѧϰ����רҵ�󣬿�����ʱ��ǵ������ʦ����ȡ���������š�������ɺ󽫻�á����ͳ�����
	endif
	if 20 = 16
		add_talk #b#b������֪�����ر�ͼ��������Ԫ�ء������ּ��껪����Щ��п��Ի�ô������ң�ǧ��Ҫ����ˣ�
	endif
	if 20 = 17
		add_talk #b#b������֪�����زɼ����������ڵ�ͼ����⵽��Χ�Ŀ����͵㣡
	endif

	add_talk ����
	add_talk #b#b�����𼱣�
	add_talk #b#b#c8B8B8B<���˺�С��������ͳ�һ����������������>#n
	add_talk #b#b�������Ҫ����Ľ��ң��ٺ١����㶮�ġ�#n
	add_talk #b#b
	add_talk #c186506��ã��������*1#n
	
	strvar jnum
	GOLD_RATE_GET_DAYNUM jnum	
	
	if jnum > 5
		sub jnum 5
		add_talk #c0040FF���վ��״�����5/5 #n
		add_talk #c0040FF��������Ȩ���״�����$@jnum$/3 #n
	else
	
		add_talk #c0040FF���վ��״�����$@jnum$/5 #n
		add_talk #c0040FF��������Ȩ������״�����0/3 #n
	
	endif
	
	get_role_var 31 8 1 10
	if 10 = 0
		add_talk #c0040FFˮ��������Ȩ��Ѵ�����0/1 #n
	else
		add_talk #c0040FFˮ��������Ȩ��Ѵ�����1/1 #n
	endif
	
	strvar yue ri
	GET_NOW_TIME yue ri 0 0 0 0 0
	if ( yue = 11 and ri = 28 ) or ( yue = 11 and ri = 29 ) or ( yue = 11 and ri = 30 ) or ( yue = 12 and ri = 1 ) or ( yue = 12 and ri = 2 )
		option 300 ���˺��ġ��ж��ڡ����������
	endif
	
	GOLD_RATE_GET_MONEY 10
	if 10 != -1
		if 10 >= 0 and 10 <= 4998
			option 801 ����4998����
		else
			option 800 ����$@10$����
		endif
	endif
	
	option 822 #����ˮ��������Ȩ��������1��
	
	talk
endproc 

;��һ�
proc 510
	GET_ITEM_NUM_INPAK 5052308 1 10
	STRVAR flag
	var flag 0
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5052308 1 11
		if 11 = 0
			var flag 1
			task_fini 4436 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#bлл�㣬$_player$���뽫�ҵ�ף���������ͣ�˳������лл����
			add_talk #b#b����̫��л�ˣ��������Ǹ�Ư����������Ը�����Լ�ᣬ������̫���ˣ��ұ�֤������������������Ͷ��ڣ�
			option 511 #��������Ҿ�������
			talk
		endif
	endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg ���ı�����û�С�ο�����ʡ���
		set_sys_msg_mode 1 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

proc 300
	dialog 1
	add_talk #b#b#b#b#b#b#b���ж��ڡ������
	add_talk #b#b#b#b#b#b#b #c18650611��28��-12��2��#n
	add_talk #b#b
	; add_talk #b#b#b#b#b#b#b#b#c005ebe�������һ��
	; add_talk #b#b#b#b#b#b#b��˫�����顱״̬#n
	; add_talk #b#b#b#b#bÿ�����߼�����ȡ1��״̬
	; add_talk #b#b״̬Ч����30������ɱ�ֻ��˫������
	add_talk #b#b#b#b#b#b#b#b#c005ebe�������һ��
	add_talk #b#b#b#b#b#b ���ж�֮�ġ������#n
	add_talk  ÿ���ۻ�����1��2��4Сʱ�ɷֱ���ȡ1�����
	add_talk #b ����������л����á�������ʱ�ʯ��
	
	get_player_daily_flag 10 16 11
	get_player_daily_flag 20 17 21
	get_player_daily_flag 30 18 31
	get_player_daily_flag 40 19 41
	STRVAR time
	GET_DAYONLINE 0 time
	
	; if 10 = 0 and 11 = 0
		; option 301 #��ȡ��˫�����顱״̬
	; endif
	if 20 = 0 and 21 = 0
		option 302 #��ȡ���ж�֮�ġ���������ۻ�����1Сʱ��
	endif
	if 30 = 0 and 31 = 0
		if time > 3600
			option 303 #��ȡ���ж�֮�ġ���������ۻ�����2Сʱ��
		endif
	endif
	if 40 = 0 and 41 = 0
		if time > 7200
			option 304 #��ȡ���ж�֮�ġ���������ۻ�����4Сʱ��
		endif
	endif
		
	talk
endproc

proc 301
	STRVAR ret num 
	
	get_player_daily_flag ret 16 num
	if ret = 0 and num = 0
		get_player_id 1 10
		TARGET_ADD_STATUS 1 @10 1039 1 1800 20
		
		strvar Mid
		GET_PLAYER_ID 0 Mid
		EFFECT_ID 0 @Mid playMontion(%d,"201800_5@reset")
		
		add_sys_msg_start
		add_sys_msg ���˺����˻����䣬����ʩ����1����˫�����顰״̬
		set_sys_msg_mode 3 0 0
		msg_info
		if 20 = 0
			var num 1
			SET_PLAYER_DAILY_FLAG 0 16 @num
		endif
	elseif ret = 0 and num = 1
		add_sys_msg_start
		add_sys_msg ������Ѿ���ȡ���ˡ�˫�����顱״̬������̫̰��Ŷ
		set_sys_msg_mode 3 0 0
		msg_info
	else
	endif
endproc

proc 302
	STRVAR time ret num ret2
	GET_DAYONLINE 0 time
	
	if time > 3600
		get_player_daily_flag ret 17 num
		if ret = 0 and num = 0
			GET_PACK_SPACE 0 num
			if @num > 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052986 1 1 ret2
				
				strvar Mid
				GET_PLAYER_ID 0 Mid
				EFFECT_ID 0 @Mid playMontion(%d,"201800_5@reset")
				
				add_sys_msg_start
				add_sys_msg ���˺�������æ��������1�����ж�֮�ġ������
				set_sys_msg_mode 3 0 0
				msg_info
				if ret2 = 0
					var num 1
					SET_PLAYER_DAILY_FLAG 0 17 @num
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG ��ճ�1�񱳰�������ǰ����ȡ
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		else
			add_sys_msg_start
			add_sys_msg ���������ȡ���ۻ�����1Сʱ�ġ��ж�֮�ġ������������̫̰��Ŷ
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		var 10 3600
		sub 10 @time
		dev 10 60
		ADD_SYS_MSG_START
		ADD_SYS_MSG ���˺����ڳﱸ������У���$@10$���Ӻ���ǰ����ȡ
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc

proc 303
	STRVAR time ret num ret2
	GET_DAYONLINE 0 time
	
	if time > 7200
		get_player_daily_flag ret 18 num
		if ret = 0 and num = 0
			GET_PACK_SPACE 0 num
			if @num > 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052986 1 1 ret2
				
				strvar Mid
				GET_PLAYER_ID 0 Mid
				EFFECT_ID 0 @Mid playMontion(%d,"201800_5@reset")
				
				add_sys_msg_start
				add_sys_msg ���˺�������æ��������1�����ж�֮�ġ������
				set_sys_msg_mode 3 0 0
				msg_info
				if ret2 = 0
					var num 1
					SET_PLAYER_DAILY_FLAG 0 18 @num
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG ��ճ�1�񱳰�������ǰ����ȡ
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		else
			add_sys_msg_start
			add_sys_msg ���������ȡ���ۻ�����2Сʱ�ġ��ж�֮�ġ������������̫̰��Ŷ
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		var 10 7200
		sub 10 @time
		dev 10 60
		ADD_SYS_MSG_START
		ADD_SYS_MSG ���˺����ڳﱸ������У���$@10$���Ӻ���ǰ����ȡ
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc

proc 304
	STRVAR time ret num ret2
	GET_DAYONLINE 0 time
	
	if time > 14400
		get_player_daily_flag ret 19 num
		if ret = 0 and num = 0
			GET_PACK_SPACE 0 num
			if @num > 1
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052986 2 1 ret2
				
				strvar Mid
				GET_PLAYER_ID 0 Mid
				EFFECT_ID 0 @Mid playMontion(%d,"201800_5@reset")
				
				add_sys_msg_start
				add_sys_msg ���˺�������æ��������2�����ж�֮�ġ������
				set_sys_msg_mode 3 0 0
				msg_info
				if ret2 = 0
					var num 1
					SET_PLAYER_DAILY_FLAG 0 19 @num
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG ��ճ�2�񱳰�������ǰ����ȡ
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			endif
		else
			add_sys_msg_start
			add_sys_msg ���������ȡ���ۻ�����4Сʱ�ġ��ж�֮�ġ������������̫̰��Ŷ
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		var 10 14400
		sub 10 @time
		dev 10 60
		ADD_SYS_MSG_START
		ADD_SYS_MSG ���˺����ڳﱸ������У���$@10$���Ӻ���ǰ����ȡ
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc
