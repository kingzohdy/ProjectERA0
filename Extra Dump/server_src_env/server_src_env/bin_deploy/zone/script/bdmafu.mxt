


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ѽѽ������������$_player$������������ѡ�绨���졭���������ԣ��Ǳ������Ե�ô��
	add_talk #b#b�����������������ˣ�
	add_talk #b#b�����ƣ������ƣ���ɶ���һ��һ��ǧ�ﱦ��������Щ�����۸���ˣ�����˵�ֻҪ500�����һ���3����ɫ��ѡ�����ϻ��б�������˵�����

;		task_add_option 1351 1 100
		option 150 #��������
	talk
	
endproc 
proc 150
	OPEN_NPC_SHOP 
	strvar ret
	task_stat 1377 1 ret
	if ret = 2
		task_fini 1377 1
	endif
endproc
;PROC 100
;	task_do_option 1351 1
;ENDPROC
;
;proc 2
;	get_trig_task 1 2
;	if 2 = 1355
;		call 150
;		NEW_OWNER_TMP_ITEM 1460060 9800 8800 0 0 0 184 1095 1 3600 1
;	endif
;endproc 
	
	