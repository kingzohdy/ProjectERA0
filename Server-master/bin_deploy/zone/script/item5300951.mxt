


proc 1

	; strvar taret
	; task_stat 3681 1 taret
	
	; if taret = 4
		; add_sys_msg_start
		; add_sys_msg ������ʧ�ܣ��޷�ʹ����Ʒ���������������½�ȡ
		; set_sys_msg_mode 4 0 0
		; msg_info
		; return
	; endif
	; if taret = 1
		; add_sys_msg_start
		; add_sys_msg �鱨�Ѿ���ȡ�ɹ������������
		; set_sys_msg_mode 4 0 0
		; msg_info
		; return
	; endif
	; strvar mapret
	; IS_IN_CLAN_SPY_TARGET_MAP mapret
	; if mapret != 1
		; add_sys_msg_start
		; add_sys_msg ����ط�����û��ʲô�鱨��ȥ���ȥ�Ĺ�����п����ɣ�
		; set_sys_msg_mode 4 0 0
		; msg_info
		; return
	; endif
	
	; strvar npcid rad
	; GET_TARGET_INFO 2 9 npcid
	;if npcid = 284301 or npcid = 284302 or npcid = 284303 or npcid = 284304 or npcid = 284305 or npcid = 284306 or npcid = 284307 or npcid = 284308 or npcid = 284309 or npcid = 284310

	
	; if npcid = 284301	
	;;	��ʾ��������һ�γɹ�1��ʧ��2
		; get_task_var 3681 12 8 1 22 
	;;	ʹ��1��
		; get_task_var 3681 0 8 1 10
		; if 10 = 0
			; rand rad 10
			; if rad < 10
	;;		�ɹ�
				; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; add_sys_msg_start
				; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
				; set_sys_msg_mode 4 0 0
				; msg_info
	;;			�ɹ�
				; var 22 1
				; set_task_var 3681 12 8 @22 1 
			; else
	;;		ʧ��
				; call 100
	;;			ʧ��
				; var 22 2
				; set_task_var 3681 12 8 @22 1 
	;;			ʧ�ܺ��ۻ�ʧ�ܴ���+1��ֻ��1�Σ�
				; get_task_var 3681 10 8 1 20
				; add 20 1
				; set_task_var 3681 10 8 @20 1 
			; endif
			; var 10 1
			; set_task_var 3681 0 8 @10 1
			; call 120
			; return
		; endif
		; if 10 = 1
			; get_task_var 3681 10 8 1 20
			; if 20 <= 5 and 22 = 1
				; call 101
			; endif
			; if 20 <= 5 and 22 = 2
				; call 100
			; endif
			; if 20 > 5
	;;		ʧ�ܴ�������5��ʱ���ظ�1���鱨��ֻ��1�Σ�
				; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; add_sys_msg_start
				; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
				; set_sys_msg_mode 4 0 0
				; msg_info
				; var 10 2
				; set_task_var 3681 0 8 @10 1
			; endif
		; endif
	; endif
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	2222
	; if npcid = 284302
		; get_task_var 3681 13 8 1 23 
		; get_task_var 3681 1 8 1 11
		; if 11 = 0
			; rand rad 10
			; if rad < 10
	;;		�ɹ�
				; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; add_sys_msg_start
				; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
				; set_sys_msg_mode 4 0 0
				; msg_info
	;;			�ɹ�
				; var 23 1
				; set_task_var 3681 13 8 @23 1	
			; else
	;;		ʧ��
				; call 100
	;;			ʧ��
				; var 23 2
				; set_task_var 3681 13 8 @23 1	
				
	;;			ʧ�ܺ��ۻ�ʧ�ܴ���+1��ֻ��1�Σ�
				; get_task_var 3681 10 8 1 20
				; add 20 1
				; set_task_var 3681 10 8 @20 1
			; endif
			; var 11 1
			; set_task_var 3681 1 8 @11 1
			; call 120
			; return
		; endif
		; if 11 = 1
			; get_task_var 3681 10 8 1 20
			; if 20 <= 5 and 23 = 1
				; call 101
			; endif
			; if 20 <= 5 and 23 = 2
				; call 100
			; endif
			; if 20 > 5
	;;		ʧ�ܴ�������5��ʱ���ظ�1���鱨��ֻ��1�Σ�
				; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; add_sys_msg_start
				; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
				; set_sys_msg_mode 4 0 0
				; msg_info
				; var 11 2
				; set_task_var 3681 1 8 @11 1
			; endif
		; endif
	; endif	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	33333
	; if npcid = 284303
		; get_task_var 3681 14 8 1 24	
		; get_task_var 3681 2 8 1 12
		; if 12 = 0
			; rand rad 10
			; if rad < 10
	;;		�ɹ�
				; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; add_sys_msg_start
				; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
				; set_sys_msg_mode 4 0 0
				; msg_info
	;;			�ɹ�
				; var 24 1
				; set_task_var 3681 14 8 @24 1	
			; else
	;;		ʧ��
				; call 100
	;;			ʧ��
				; var 24 2
				; set_task_var 3681 14 8 @24 1	
	;;			ʧ�ܺ��ۻ�ʧ�ܴ���+1��ֻ��1�Σ�
				; get_task_var 3681 10 8 1 20
				; add 20 1
				; set_task_var 3681 10 8 @20 1
			; endif
			; var 12 1
			; set_task_var 3681 2 8 @12 1
			; call 120
			; return
		; endif
		; if 12 = 1
			; get_task_var 3681 10 8 1 20
			; if 20 <= 5 and 24 = 1
				; call 101
			; endif
			; if 20 <= 5 and 24 = 2
				; call 100
			; endif
			; if 20 > 5
	;;		ʧ�ܴ�������5��ʱ���ظ�1���鱨��ֻ��1�Σ�
				; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; add_sys_msg_start
				; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
				; set_sys_msg_mode 4 0 0
				; msg_info
				; var 12 2
				; set_task_var 3681 2 8 @12 1
			; endif
		; endif
	; endif	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;4444
	; if npcid = 284304
		; get_task_var 3681 15 8 1 25
		; get_task_var 3681 3 8 1 13
		; if 13 = 0
			; rand rad 10
			; if rad < 10
	;;		�ɹ�
				; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; add_sys_msg_start
				; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
				; set_sys_msg_mode 4 0 0
				; msg_info
	;;			�ɹ�
				; var 25 1
				; set_task_var 3681 15 8 @25 1
			; else
	;;		ʧ��
				; call 100
	;;			ʧ��
				; var 25 2
				; set_task_var 3681 15 8 @25 1
	;;			ʧ�ܺ��ۻ�ʧ�ܴ���+1��ֻ��1�Σ�
				; get_task_var 3681 10 8 1 20
				; add 20 1
				; set_task_var 3681 10 8 @20 1
			; endif
			; var 13 1
			; set_task_var 3681 3 8 @13 1
			; call 120
			; return
		; endif
		; if 13 = 1
			; get_task_var 3681 10 8 1 20
			; if 20 <= 5 and 25 = 1
				; call 101
			; endif
			; if 20 <= 5 and 25 = 2
				; call 100
			; endif
			; if 20 > 5
	;;		ʧ�ܴ�������5��ʱ���ظ�1���鱨��ֻ��1�Σ�
				; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; add_sys_msg_start
				; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
				; set_sys_msg_mode 4 0 0
				; msg_info
				; var 13 2
				; set_task_var 3681 3 8 @13 1
			; endif
		; endif
	; endif	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	5555
	; if npcid = 284305
		
		; get_task_var 3681 16 8 1 26 
		; get_task_var 3681 4 8 1 14
		; if 14 = 0
			; rand rad 10
			; if rad < 10
		;;	�ɹ�
				; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; add_sys_msg_start
				; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
				; set_sys_msg_mode 4 0 0
				; msg_info
		;;		�ɹ�
				; var 26 1
				; set_task_var 3681 16 8 @26 1	
			; else
		;;	ʧ��
				; call 100
		;;		ʧ��
				; var 26 2
				; set_task_var 3681 16 8 @26 1	
		;;		ʧ�ܺ��ۻ�ʧ�ܴ���+1��ֻ��1�Σ�
				; get_task_var 3681 10 8 1 20
				; add 20 1
				; set_task_var 3681 10 8 @20 1
			; endif
			; var 14 1
			; set_task_var 3681 4 8 @14 1
			; call 120
			; return
		; endif
		; if 14 = 1
			; get_task_var 3681 10 8 1 20
			; if 20 <= 5 and 26 = 1
				; call 101
			; endif
			; if 20 <= 5 and 26 = 2
				; call 100
			; endif
			; if 20 > 5
		;;	ʧ�ܴ�������5��ʱ���ظ�1���鱨��ֻ��1�Σ�
				; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; add_sys_msg_start
				; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
				; set_sys_msg_mode 4 0 0
				; msg_info
				
				; var 14 2
				; set_task_var 3681 4 8 @14 1
			; endif
		; endif
	; endif		
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;6666
	; if npcid = 284306
		
		; get_task_var 3681 17 8 1 27 	
		; get_task_var 3681 5 8 1 15
		; if 15 = 0
			; rand rad 10
			; if rad < 10
		;;	�ɹ�
				; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; add_sys_msg_start
				; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
				; set_sys_msg_mode 4 0 0
				; msg_info
		;;		�ɹ�
				; var 27 1
				; set_task_var 3681 17 8 @27 1	
			; else
		;;	ʧ��
				; call 100
		;;		ʧ��
				; var 27 2
				; set_task_var 3681 17 8 @27 1	
		;;		ʧ�ܺ��ۻ�ʧ�ܴ���+1��ֻ��1�Σ�
				; get_task_var 3681 10 8 1 20
				; add 20 1
				; set_task_var 3681 10 8 @20 1
			; endif
			; var 15 1
			; set_task_var 3681 5 8 @15 1
			; call 120
			; return
		; endif
		; if 15 = 1
			; get_task_var 3681 10 8 1 20
			; if 20 <= 5 and 27 = 1
				; call 101
			; endif
			; if 20 <= 5 and 27 = 2
				; call 100
			; endif
			; if 20 > 5
		;;	ʧ�ܴ�������5��ʱ���ظ�1���鱨��ֻ��1�Σ�
				; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; var 15 2
				; set_task_var 3681 5 8 @15 1
			; endif
		; endif
	; endif	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;7777
	; if npcid = 284307
		; get_task_var 3681 18 8 1 28 
		; get_task_var 3681 6 8 1 16
		; if 16 = 0
			; rand rad 10
			; if rad < 10
		;;	�ɹ�
				; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; add_sys_msg_start
				; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
				; set_sys_msg_mode 4 0 0
				; msg_info
		;;		�ɹ�
				; var 28 1
				; set_task_var 3681 18 8 @28 1
			; else
		;;	ʧ��
				; call 100
		;;		ʧ��
				; var 28 2
				; set_task_var 3681 18 8 @28 1
		;;		ʧ�ܺ��ۻ�ʧ�ܴ���+1��ֻ��1�Σ�
				; get_task_var 3681 10 8 1 20
				; add 20 1
				; set_task_var 3681 10 8 @20 1
			; endif
			; var 16 1
			; set_task_var 3681 6 8 @16 1
			; call 120
			; return
		; endif
		; if 16 = 1
			; get_task_var 3681 10 8 1 20
			; if 20 <= 5 and 28 = 1
				; call 101
			; endif
			; if 20 <= 5 and 28 = 2
				; call 100
			; endif
			; if 20 > 5
		;;	ʧ�ܴ�������5��ʱ���ظ�1���鱨��ֻ��1�Σ�
				; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; add_sys_msg_start
				; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
				; set_sys_msg_mode 4 0 0
				; msg_info
				; var 16 2
				; set_task_var 3681 6 8 @16 1
			; endif
		; endif
	; endif		
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; if npcid = 284308
		; get_task_var 3681 7 8 1 17
		; if 17 = 0
		;;	�ɹ�
			; ADD_ITEM_NUM_INPAK 5300950 1 1 0
			; add_sys_msg_start
			; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
			; set_sys_msg_mode 4 0 0
			; msg_info

			; var 17 1
			; set_task_var 3681 7 8 @17 1
			; call 120
			; return
		; endif
		; if 17 = 1
			; call 101
		; endif
	; endif		
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; if npcid = 284309
		; get_task_var 3681 8 8 1 18
		; if 18 = 0
			; ADD_ITEM_NUM_INPAK 5300950 1 1 0
			; add_sys_msg_start
			; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
			; set_sys_msg_mode 4 0 0
			; msg_info
		
			; var 18 1
			; set_task_var 3681 8 8 @18 1
			; call 120
			; return
		; endif
		; if 18 = 1
			; call 101
		; endif
	; endif	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; if npcid = 284310
		; get_task_var 3681 9 8 1 19
		; if 19 = 0
			; ADD_ITEM_NUM_INPAK 5300950 1 1 0
				; add_sys_msg_start
				; add_sys_msg ��ϲ�����ɹ���ȡ�鱨һ�ݣ�
				; set_sys_msg_mode 4 0 0
				; msg_info
		
			; var 19 1
			; set_task_var 3681 9 8 @19 1
			; call 120
			; return
		; endif
		; if 19 = 1
			; call 101
		; endif
	; endif	
	

endproc  
proc 120
;	strvar num
;	GET_ITEM_NUM_INPAK 5300950 1 num
;	if num = 5
;		SET_PLAYER_SPY_TASK_STAGE_DONE 1
;		dec_item_num_inpak 5300951 1 0
;	;	dec_item_num_inpak 5300950 5 0
;		ADD_BIND_ITEM_NUM_INPAK 5300953 1 1 10
;	endif
endproc 

proc 2
	strvar npcid
	GET_TARGET_INFO 2 9 npcid
	if npcid != 284301 and npcid != 284302 and npcid != 284303 and npcid != 284304 and npcid != 284305 and npcid != 284306 and npcid != 284307 and npcid != 284308 and npcid != 284309 and npcid != 284310
	
		add_sys_msg_start
		add_sys_msg ��ǰ���з��Ĺ�����У��Եз�������鱨��ʹ��!
		set_sys_msg_mode 4 0 0
		msg_info

		set_script_int_ret -1

	endif
endproc 

proc 100
	add_sys_msg_start
	add_sys_msg û�з����鱨,��������Ŀ��ɣ�
	set_sys_msg_mode 4 0 0
	msg_info
endproc
proc 101
	add_sys_msg_start
	add_sys_msg ��Ŀ���鱨�Ѿ���ȡ,���������鱨,��������Ŀ��ɣ�
	set_sys_msg_mode 4 0 0
	msg_info
endproc
