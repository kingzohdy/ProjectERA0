proc 1
	
	strvar tim
	var tim 0
	rand tim 1710

	;������
	;var tim 1709

	set_event_var 0 32 @tim 1

	strvar loc
	var loc 0
	set_event_var 4 32 @loc 1
	
	;��ʱ��ɾ��npc
	strvar tim2
	var tim2 0
	set_event_var 8 32 @tim2 1
	
	strvar trs1
	var trs1 0
	set_event_var 12 32 @trs1 1
	
	strvar trs2
	var trs2 0
	set_event_var 16 32 @trs2 1
endproc 

proc 3
	
	get_event_var 0 32 1 tim 
	get_event_var 4 32 1 loc
	
	GET_TOP_BY_LEVEL 1 1 60
	
	if loc = 0 and 60 >= 55
		add tim 1
		set_event_var 0 32 @tim 1 
		
		;���������籨��
		;add_sys_msg_start
		;add_sys_msg $@tim$
		;set_sys_msg_mode 1 0 0
		;SVR_MSG_INFO

		if tim >= 1710
			;ˢnpc
			call 120
			
			var loc 1
			set_event_var 4 32 @loc 1
		endif
	endif
	if loc = 1
		get_event_var 8 32 1 tim2 
		add tim2 1
		set_event_var 8 32 @tim2 1
		
		get_event_var 12 32 1 trs1
		
		switch trs1
			case 0
				switch tim2
					case 13
						add_sys_msg_start
						add_sys_msg ����һ����������������Ԫ�س���������ѩԭ����Я����������ǰ���ռ�����!
						set_sys_msg_mode 6 0 0
						SVR_MSG_INFO
					endcase
					case 37
						add_sys_msg_start
						add_sys_msg ����������ѩԭ�Ļ���Ԫ�ؽ���2���Ӻ���ʧ�����λ�ӽ��ж�!
						set_sys_msg_mode 6 0 0
						SVR_MSG_INFO
					endcase
					case 49
						add_sys_msg_start
						add_sys_msg ����Ԫ�ص������Ѿ��ľ������λ�´μ���Ŭ����
						set_sys_msg_mode 6 0 0
						SVR_MSG_INFO
						
						EFFECT 1 0 CloseMessageTips(300)
						
						var loc 2
						set_event_var 4 32 @loc 1
					endcase
				endswitch
			endcase
			case 1
				switch tim2
					case 13
						add_sys_msg_start
						add_sys_msg ����һ����������������Ԫ�س�����ʮ���ſ󶴣���Я����������ǰ���ռ�����!
						set_sys_msg_mode 6 0 0
						SVR_MSG_INFO
					endcase
					case 37
						add_sys_msg_start
						add_sys_msg ������ʮ���ſ󶴵Ļ���Ԫ�ؽ���2���Ӻ���ʧ�����λ�ӽ��ж�!
						set_sys_msg_mode 6 0 0
						SVR_MSG_INFO
					endcase
					case 49
						add_sys_msg_start
						add_sys_msg ����Ԫ�ص������Ѿ��ľ������λ�´μ���Ŭ����
						set_sys_msg_mode 6 0 0
						SVR_MSG_INFO
						
						EFFECT 1 0 CloseMessageTips(301)
						
						var loc 2
						set_event_var 4 32 @loc 1
					endcase
				endswitch
			endcase
			case 2
				switch tim2
					case 13
						add_sys_msg_start
						add_sys_msg ����һ����������������Ԫ�س����ڰ�Ŭ��˹����Я����������ǰ���ռ�����!
						set_sys_msg_mode 6 0 0
						SVR_MSG_INFO
					endcase
					case 37
						add_sys_msg_start
						add_sys_msg �����ڰ�Ŭ��˹���Ļ���Ԫ�ؽ���2���Ӻ���ʧ�����λ�ӽ��ж�!
						set_sys_msg_mode 6 0 0
						SVR_MSG_INFO
					endcase
					case 49
						add_sys_msg_start
						add_sys_msg ����Ԫ�ص������Ѿ��ľ������λ�´μ���Ŭ����
						set_sys_msg_mode 6 0 0
						SVR_MSG_INFO
						
						EFFECT 1 0 CloseMessageTips(302)
						
						var loc 2
						set_event_var 4 32 @loc 1
					endcase
				endswitch
			endcase
		endswitch
	endif
endproc 

proc 100

	get_event_var 12 32 1 trs1
	get_event_var 16 32 1 trs2
    ;NEW_MAP_LIFE_NPC 1 @map 302169 @x @y 0 900 hdhunluan.mac
	
	switch trs1
		case 0
			add_sys_msg_start
			add_sys_msg ����һ����������������Ԫ�س���������ѩԭ����Я����������ǰ���ռ�����!
			;$@map$ $@x$ $@y$���ӵ���һ�е�ĩβ���������ڲ��Բ鿴npc���ֵĵص㣩
			set_sys_msg_mode 6 0 0
			SVR_MSG_INFO
			
			EFFECT 1 0 SetMessageTips(300)
			NEW_MAP_LIFE_NPC 1 2200 302132 16100 14200 0 480 hdhunluan2.mac
			
			NEW_MAP_LIFE_NPC 1 2200 302169 @x @y 0 480 hdhunluan.mac
		endcase
		case 1
			add_sys_msg_start
			add_sys_msg ����һ����������������Ԫ�س�����ʮ���ſ󶴣���Я����������ǰ���ռ�����!
			;$@map$ $@x$ $@y$���ӵ���һ�е�ĩβ���������ڲ��Բ鿴npc���ֵĵص㣩
			set_sys_msg_mode 6 0 0
			SVR_MSG_INFO
			
			EFFECT 1 0 SetMessageTips(301)
			NEW_MAP_LIFE_NPC 1 3400 302132 5500 2400 0 480 hdhunluan2.mac
			
			NEW_MAP_LIFE_NPC 1 3400 302169 @x @y 0 480 hdhunluan.mac
		endcase
		case 2
			add_sys_msg_start
			add_sys_msg ����һ����������������Ԫ�س����ڰ�Ŭ��˹����Я����������ǰ���ռ�����!
			;$@map$ $@x$ $@y$���ӵ���һ�е�ĩβ���������ڲ��Բ鿴npc���ֵĵص㣩
			set_sys_msg_mode 6 0 0
			SVR_MSG_INFO
			
			EFFECT 1 0 SetMessageTips(302)
			NEW_MAP_LIFE_NPC 1 3300 302132 1000 7800 0 480 hdhunluan2.mac
			
			NEW_MAP_LIFE_NPC 1 3300 302169 @x @y 0 480 hdhunluan.mac
		endcase
	endswitch
endproc 

;npcˢ��λ�ã�
;2202������27100,42900����25500,36900����6800,12700����41500,32100��
;2300����ɳĮ��4800,3200����20500,2800����13900,2800��
;2400���ջ�Ұ��19300,21800����4600,21800����11800,2300����19800,3300��

;�µص㣺
;2200����ѩԭ��46 49����47 119����66 181����66 214����55 67����204 231����229 208����141 208����229 115����154 85��
;3400ʮ���ſ󶴣�106 119����117 149����146 79����150 116����141 150����29 105��
;3300��Ŭ��˹��39 100����13 45����9 13����35 30����19 57����43 21����74 80����54 114����112 76����75 10����119 19����76 62��

proc 120

	strvar x y rad1 rad2
	var rad1 0
	var rad2 0
	
	rand rad1 3
	
	if rad1 = 0
		rand rad2 10
		if rad2 = 0
			set_call_var x 4600
			set_call_var y 4900
		endif
		if rad2 = 1
			set_call_var x 4700
			set_call_var y 11900
		endif
		if rad2 = 2
			set_call_var x 6600
			set_call_var y 18100
		endif
		if rad2 = 3
			set_call_var x 6600
			set_call_var y 21400
		endif
		if rad2 = 4
			set_call_var x 5500
			set_call_var y 6700
		endif
		if rad2 = 5
			set_call_var x 20400
			set_call_var y 23100
		endif
		if rad2 = 6
			set_call_var x 22900
			set_call_var y 20800
		endif
		if rad2 = 7
			set_call_var x 22800
			set_call_var y 7900
		endif
		if rad2 = 8
			set_call_var x 22900
			set_call_var y 11500
		endif
		if rad2 = 9
			set_call_var x 15400
			set_call_var y 8500
		endif
		
		;set_call_var map 2200
		get_event_var 12 32 1 trs1
		var trs1 @rad1
		set_event_var 12 32 @trs1 1
		
		get_event_var 16 32 1 trs2
		var trs2 @rad2
		set_event_var 16 32 @trs2 1
		
		
		call 100
	endif
	if rad1 = 1
		rand rad2 6
		if rad2 = 0
			set_call_var x 10600
			set_call_var y 11900
		endif
		if rad2 = 1
			set_call_var x 11700
			set_call_var y 14900
		endif
		if rad2 = 2
			set_call_var x 14600
			set_call_var y 7900
		endif
		if rad2 = 3
			set_call_var x 15000
			set_call_var y 11600
		endif
		if rad2 = 4
			set_call_var x 14100
			set_call_var y 15000
		endif
		if rad2 = 5
			set_call_var x 2900
			set_call_var y 10500
		endif
		
		;set_call_var map 3400
		get_event_var 12 32 1 trs1
		var trs1 @rad1
		set_event_var 12 32 @trs1 1
		
		get_event_var 16 32 1 trs2
		var trs2 @rad2
		set_event_var 16 32 @trs2 1
		
		call 100
	endif
	if rad1 = 2
		rand rad2 12
		if rad2 = 0
			set_call_var x 3900
			set_call_var y 10000
		endif
		if rad2 = 1
			set_call_var x 1300
			set_call_var y 4500
		endif
		if rad2 = 2
			set_call_var x 900
			set_call_var y 1300
		endif
		if rad2 = 3
			set_call_var x 3500
			set_call_var y 3000
		endif
		if rad2 = 4
			set_call_var x 1900
			set_call_var y 5700
		endif
		if rad2 = 5
			set_call_var x 4300
			set_call_var y 2100
		endif
		if rad2 = 6
			set_call_var x 7400
			set_call_var y 8000
		endif
		if rad2 = 7
			set_call_var x 5400
			set_call_var y 11400
		endif
		if rad2 = 8
			set_call_var x 11200
			set_call_var y 7600
		endif
		if rad2 = 9
			set_call_var x 7500
			set_call_var y 1000
		endif
		if rad2 = 10
			set_call_var x 11900
			set_call_var y 1900
		endif
		if rad2 = 11
			set_call_var x 7600
			set_call_var y 6200
		endif
		
		;set_call_var map 3300
		get_event_var 12 32 1 trs1
		var trs1 @rad1
		set_event_var 12 32 @trs1 1
		
		get_event_var 16 32 1 trs2
		var trs2 @rad2
		set_event_var 16 32 @trs2 1

		call 100
	endif

endproc