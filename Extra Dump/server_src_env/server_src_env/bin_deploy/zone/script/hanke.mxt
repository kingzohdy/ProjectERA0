

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���Ǻ���


	TASK_STAT 12054 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)��������
	endif

	TASK_STAT 12254 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)�㿴һ�£�����������⣬��Ͷ��ô��
	endif
	talk
endproc 


PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$       
    	ADD_TALK  ����������Ľ��ӣ�ֻ�е�����ĵ������ܷ�������Ľ��ӣ���������

	OPTION 250 #(����)�����ˣ�
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300508 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300508 1 0
	task_fini 12054 1
	endif
ENDPROC



