;����


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������ʵ�ļ��⣬�������������꣡������Щ���Һ�ˮ��͸��װ������˳����˰����¶ȡ�
	add_talk #b#bΪ�˲������������ź���������ڴ˻ӻ��ɣ�

	option 120 #(����)���̵�
		strvar RetTsk
	task_stat 3129 1 RetTsk
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
proc 120
	OPEN_NPC_SHOP 
endproc 
proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b ������������Ҳ��ʥ�����ﰡ������������
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
			task_fini 3129 1
		endif
	endif
endproc 

proc 250
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������ͻ�ʱ�仹��һ�������׼ʱ����
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






