

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bû�г�û�д�
	add_talk #b#b�����ǵ�������ǰ
	add_talk #b#bû��ǹû����
	add_talk #b#b���˸�������
	add_talk #b#b��������������
	add_talk #b#bÿһ�����ض��������Լ���
	add_talk #b#b����˭Ҫ��ռȥ
	add_talk #b#b���Ǿͺ���ƴ����

	task_add_option 1625 1 118
	task_add_option 1626 1 119
	task_add_option 1627 1 120
	task_add_option 1628 1 121

	TASK_STAT 12048 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)���ǻ���֪ͨ������ϣ���õ�һЩ��Ϣ
	endif

	TASK_STAT 12248 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)�Ҹ�������˷糵ģ��
	endif
	talk
endproc 


PROC 118
	task_do_option 1625 1
ENDPROC
PROC 119
	task_do_option 1626 1
ENDPROC
PROC 120
	task_do_option 1627 1
ENDPROC
PROC 121
	task_do_option 1628 1
ENDPROC

PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b������ͺ��ʵĻ����������ǲ��������ҵ��鱨��

	OPTION 250 #(����)�õģ�
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300506 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300506 1 0
	task_fini 12048 1
	endif
ENDPROC




