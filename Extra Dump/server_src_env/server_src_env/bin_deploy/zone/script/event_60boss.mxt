

proc 1

	; strvar Tim1 Tim2 Mid3
	; var Tim1 0
	; set_event_var 0 32 @Tim1 1
	
	; var 10 0
	; set_event_var 4 32 @10 1
	; var 20 0
	; set_event_var 8 32 @20 1
	
	; NEW_MAP_MON 207006 1 3400 9699 4057 0 510 0 0 3600 10 0		

;�¼�ID 47
endproc

proc 3
	; get_event_var_by_id 47 4 32 12 20
	; if 12 = 0 and 20 = 0
	;;;;;û�д�����BOSS
		; strvar MonNum MonNum2
		; GET_MAP_MON_NUM 3400 207001 1 MonNum 1
		; GET_MAP_MON_NUM 3400 207006 1 MonNum2 1
		; if MonNum = 0 and MonNum2 = 0
			;;;;;��ʱ120����
			; get_event_var 0 32 1 Tim1
			; add Tim1 1
			; set_event_var 0 32 @Tim1 1
			
			; if Tim1 = 1440
		;;;;	if Tim1 = 6
				; var Tim1 0
				; set_event_var 0 32 @Tim1 1
				
				; NEW_MAP_MON 207006 1 3400 9699 4057 0 510 0 0 3600 10 0		
			; endif
		; endif
		; if MonNum = 1 or MonNum2 = 1
			; var Tim1 0
			; set_event_var 0 32 @Tim1 1
		
		; endif
	; endif
	; if 12 = 0 and 20 = 1
;;;;	;���������BOSS�����뵹��ʱ�׶�
		; get_event_var 8 32 1 20 
		; add 20 1
		; if 20 = 1
			; add_sys_msg_start
			; add_sys_msg 10���Ӻ���װ���ɻ�׼ʱ�ִ�#Lʮ���ſ�@@(3400,14500,16100)#n�����λ��ʿ�����ַ�׼����
			; set_sys_msg_mode 1 0 0
			; svr_msg_info
		; endif
		; set_event_var 8 32 @20 1
		
		; if 20 = 30
			; add_sys_msg_start
			; add_sys_msg 5���Ӻ���װ���ɻ�׼ʱ�ִ�#Lʮ���ſ�@@(3400,14500,16100)#n�����λ��ʿ�����ַ�׼����
			; set_sys_msg_mode 1 0 0
			; svr_msg_info
		
		; endif
		; if 20 = 60
		
			; NEW_MAP_MON 207001 1 3400 14500 16100 0 0 0 0 0 0 0
			; add_sys_msg_start
			; add_sys_msg ��װ���ɻ��ѵִ�#Lʮ���ſ�@@(3400,14500,16100)#n�����Ӷ������Ͻ������Դ��
			; set_sys_msg_mode 1 0 0
			; svr_msg_info
			; get_map_val 0 8 1 50
			; var 50 1
			; SET_MAP_VAL 0 8 @50 1
			;;;;;;;;;;;;;;;;;;;
			;;;;��ʱ����0
			; var 20 0
			; set_event_var 8 32 @20 1
			
			;;;;;;��
			; var 20 0
			; set_event_var 4 32 @20 1
		; endif
	; endif
endproc
