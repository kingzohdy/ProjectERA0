 

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ӭ���١��������С������Ǹ���ֿ����Ļ����˰��������ṩ��Ʒ��Ǯ�ҵĴ洢����
	add_talk #b#b�����һ��ǽ�������ʳ��Ž�������Ϊ���ǳ��ⲻ���͵Ļ����ˡ�
	add_talk #b#b#c186506�ֿ�ɴ�ȡ������Ʒ#n
	task_add_option 4039 1 103


	OPTION 104 #�򿪲ֿ�
;	option 200 #�����ҽ�����
	option 130 #�޸İ�ȫ������
	option 131 #ȡ����ȫ������


	TASK_STAT 12009 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)����������
	endif
	TASK_STAT 12209 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)���Ǵ�����ĵ�ͼ
	endif
	talk
endproc 

PROC 103
	task_do_option 4039 1
ENDPROC
PROC 104
	OPEN_UI 0
ENDPROC
proc 200
	OPEN_UI 35
endproc 

proc 130
	OPEN_UI 55
endproc
proc 131
	open_ui 56
endproc
PROC 160
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b��·����չ��������ԵĻ�����ϣ��ʱ��ǵĸ�λ���ܹ����ܵ��������ĺô���

	OPTION 260 #(����)лл���֧�֣�
	TALK
ENDPROC
PROC 260
	GET_ITEM_NUM_INPAK 5300511 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300511 1 0
	task_fini 12209 1
	endif
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 1097
		task_fini 1097 1
	endif

endproc
