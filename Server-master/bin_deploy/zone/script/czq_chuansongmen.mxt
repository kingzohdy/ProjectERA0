

proc 1

	strvar npcid lv Rflag Maxlv
;	GET_TARGET_INFO 2 9 npcid
	GET_NPC_DEFID 1 npcid
	GET_PLAYER_BASE_ATT 1 1 lv 1
	GET_CUR_MAX_LEVEL Rflag Maxlv
	
	
	if npcid = 333090
		if lv >= 65
			TRANSMIT 3211 4349 23185 1
		else
			add_sys_msg_start
			add_sys_msg ǰ��ΪΣ��֮�أ�����ﵽ65������ǰ��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif             
	if npcid = 333091 
		if lv >= 70
			TRANSMIT 3211 3218 4470 1
		else
			add_sys_msg_start
			add_sys_msg ǰ��ΪΣ��֮�أ�����ﵽ70������ǰ��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif             
	if npcid = 333092 
		if lv >= 70
			TRANSMIT 3200 21126 21690 1
		else
			add_sys_msg_start
			add_sys_msg ǰ��ΪΣ��֮�أ�����ﵽ70������ǰ��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif        
	if npcid = 333093 
		if lv >= 65     
			TRANSMIT 3110 9647 18631 1
		else
			call 200
		endif
	endif             
	if npcid = 333094 
		if lv >= 67     
			TRANSMIT 3212 2762 16642 1
		else
			add_sys_msg_start
			add_sys_msg ǰ��ΪΣ��֮�أ�����ﵽ67������ǰ��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif             
	if npcid = 333095 
		; if lv >= 65     
			TRANSMIT 3211 22899 22128 1
		; else
			; call 200
		; endif
	endif             
	if npcid = 333096 
		if lv >= 67     
			TRANSMIT 3213 1284 6365 1
		else
			add_sys_msg_start
			add_sys_msg ǰ��ΪΣ��֮�أ�����ﵽ67������ǰ��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif             
	if npcid = 333097 
		; if lv >= 65     
			TRANSMIT 3212 18679 22417 1
		; else
			; call 200
		; endif
	endif             
	if npcid = 333098 
		if Rflag = 0 and Maxlv > 69
			TRANSMIT 3214 23891 21465 1
		else
			add_sys_msg_start
			add_sys_msg ǰ��ΪΣ��֮�أ�������69���ȼ���ӡ����ǰ��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		
			; add_sys_msg_start
			; add_sys_msg ��δ���ţ������ڴ�
			; set_sys_msg_mode 1 0 0
			; msg_info
			; return
		; if lv >= 65     
			; TRANSMIT 3214 24467 21441 1
		; endif   
	endif
	
	if npcid = 333099 
		; if lv >= 70
			TRANSMIT 3213 9458 6412 1
		; else
			; call 200
		; endif
	endif             
	if npcid = 333101 
		if Rflag = 0 and Maxlv > 74
			TRANSMIT 3215 10886 22385 1
		else
			add_sys_msg_start
			add_sys_msg ǰ��ΪΣ��֮�أ�������74���ȼ���ӡ����ǰ��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif             
	if npcid = 333102 
		; if lv >= 70
			TRANSMIT 3214 13500 16850 1
		; endif
	endif             
	if npcid = 333103 
		if Rflag = 0 and Maxlv > 79
			TRANSMIT 3216 1658 6404 1
		else
			add_sys_msg_start
			add_sys_msg ǰ��ΪΣ��֮�أ�������79���ȼ���ӡ����ǰ��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif             
	if npcid = 333104 
		; if lv >= 70
			TRANSMIT 3215 26200 5900 1 
		; else
			; call 200
		; endif
	endif 
	if npcid = 333105
		strvar Rflag Maxlv ret num ret1 ret3 num2 time time2
		GET_CUR_MAX_LEVEL Rflag Maxlv
		GET_VAILD_ITEM_NUM_INPAK 5060080 ret num
		get_event_stat ret1 102
		GET_ITEM_NUM_INPAK 5060080 ret3 num2
		get_event_exist_time time 102
		
		if Rflag = 0 and Maxlv > 79
			if ret1 = 1
				; if ret3 = 0 and num2 > 0
					; if ret = 0 and num > 0
						if time > 1800
							TRANSMIT 3217 1658 6404 1
						else
							var time2 30
							dev time 60
							sub time2 @time
							
							add_sys_msg_start
							add_sys_msg ͨ�����߲�Ĵ�������Ŭ���ƽ��У���ɹ��ƽ⻹��Ҫ$@time2$����
							set_sys_msg_mode 3 0 0
							msg_info
						endif
					; else
						; add_sys_msg_start
						; add_sys_msg ��ġ�����֤֮���ѹ��ڣ���Ҫ���һ��ġ�������ͳ�족��ȡ�µġ�����֤֮��
						; set_sys_msg_mode 3 0 0
						; msg_info
					; endif
				; else
					; add_sys_msg_start
					; add_sys_msg ����ͨ�����߲㣬��Ҫ���е�һ�㡰������ͳ�족����ġ�����֤֮��
					; set_sys_msg_mode 3 0 0
					; msg_info
				; endif
			else
				add_sys_msg_start
				add_sys_msg ͨ�����߲�Ĵ����ѹرգ���������Ҫ�����յ�19:00�����ٴ��ƽ����
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg ǰ��ΪΣ��֮�أ�������79���ȼ���ӡ����ǰ��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if npcid = 333106
			TRANSMIT 3216 10800 6400 1
	endif
	
endproc 

proc 100
	; add_sys_msg_start
	; add_sys_msg  �ɵúã��ɹ��ڳ���һ��������
	; set_sys_msg_mode 3 0 0
	; msg_info
endproc 

proc 200
	add_sys_msg_start
	add_sys_msg  ǰ��ΪΣ��֮�أ�������ߵȼ�ʵ������ǰ��
	set_sys_msg_mode 3 0 0
	msg_info
endproc 


