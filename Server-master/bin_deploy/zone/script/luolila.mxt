;��ţ����,������
;������
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ӭ����̩��ũ������Ȼ���������Ѿ�û����Щ��ũ��Ӧ�þ㱸�Ķ�������������Ȼ���ҵļң��Ҳ�����������
	
	
	task_add_option 1619 1 119
	task_add_option 1620 1 120
	task_add_option 1621 1 121
	task_add_option 1622 1 122
	task_add_option 2000 1 123
	TASK_STAT 12052 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)�͸�������������ѫ��
	endif
	TASK_STAT 12252 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)�㿴һ�£�����������⣬��Ͷ��ô��
	endif
	
	
	talk
endproc 
PROC 119
	task_do_option 1619 1
ENDPROC
PROC 120
	task_do_option 1620 1
ENDPROC
PROC 121
	task_do_option 1621 1
ENDPROC
PROC 122
	task_do_option 1622 1
ENDPROC
proc 123
	task_do_option 2000 1
endproc
PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b��ֻ��������Щ�����Ķ��ﲻҪ��Ϊ����Ĵ������ȥ��
	OPTION 250 #(����)֪���ˣ�
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300507 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
		dec_item_num_inpak 5300507 1 0
		task_fini 12052 1
	endif
ENDPROC
