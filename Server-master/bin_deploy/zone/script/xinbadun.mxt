;���ִ帱�ӳ��Ͷ�

proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b�������ǵĹ��������ǲ�Ϊ����ս����˭��Ϊ��ս���أ�
	
	task_add_option 1053 1 101
;	task_add_option 1052 1 102
	task_add_option 1057 1 103
	
;	option 200 С��������
	talk
	
endproc
proc 101
    task_do_option 1053 1
endproc

;proc 102
;    task_do_option 1052 1
;endproc
proc 103
    task_do_option 1057 1
endproc

proc 2
	get_trig_task 1 2

	if 2 = 1057
		task_fini 1057 1
	;	NEW_OWNER_TMP_ITEM 1460065 14900 26200 0 0 0 306 1095 1 3600 1
	endif
endproc 
proc 200
	dialog 1
	add_talk $_npc$:
	add_talk #b#b����ȡ������ô��
		option 201 #���Ǻû���
		option 202 ����ȡ������
	talk
endproc 
proc 201
	strvar num
	GET_ITEM_NUM_INPAK 5052801 1 num
	if num = 0
		ADD_ITEM_NUM_INPAK 5052801 1 1 0
	endif
endproc
proc 202
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���������������ǲ��õ��ߵġ���������
		option 200 ������
		option 204 #�Ҽ������
	talk
endproc




