;ʳ��


proc 1

	DIALOG 1

	add_talk $_npc$:
	add_talk #b#b��ӭ���٣����ﶼ�������ʳ�¯��ʳ��������ѡ��
	add_talk #b#b����Ŷ�����ǵ�ʹ�õ�ȫ�ǰ�ȫʳ�ģ����ſ��Ǿ����߱ر���Ʒ�¡��ٺ٣�
	add_talk #b#b�����˷���ʳ�������򲻿ɵ�Ŷ���ף�

	
;	task_add_option 1081 1 100
		
	option 120 #(����)���̵�
	strvar RetTsk
	task_stat 3128 1 RetTsk
	if RetTsk = 2
		option 200 ����ʥ���Ϲ������������
	endif
	
	strvar Ret
	task_stat 3674 1 Ret
	if Ret = 2
		option 250 ���湫����������Ҫ����������
	endif

	
	task_stat 4147 1 50
	if 50 = 2
		option 300 #��������������㶩��������
	endif
	
	talk
endproc 	


;PROC 100
;	task_do_option 1081 1
;ENDPROC


;proc 2
;	get_trig_task 1 2
;	if 2 = 3110
;		task_fini 3110 1
;	
;		NEW_OWNER_TMP_ITEM 1460071 27100 24900 0 0 0 311 1095 1 3600 1
;	endif 
;endproc 


proc 120
	OPEN_NPC_SHOP 
endproc 
proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b лл�㣬$_player$��лл�㣬ʥ���Ϲ������ף�
		option 210 #����ʥ������
	talk
endproc 
proc 210
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
			task_fini 3128 1
		endif
	endif
endproc 
	
	
proc 250
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bлл�㣬������������Ĳ��������������ˡ�
		option 251 #������������
	talk
endproc 
proc 251
	GET_ITEM_NUM_INPAK 5300871 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300871 1 30
		if 30 != -1
			task_fini 3674 1
		endif
	endif
endproc


proc 300
	GET_ITEM_NUM_INPAK 5301003 0 11
	if 11 < 1
	
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	else
	
		dec_item_num_inpak 5301003 1 30
		if 30 != -1
			task_fini 4147 1
		endif
	endif
endproc

