; ��ܶ�


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b �����Ҫ����עĿ����Ҫʹ���Լ���Ϊһ���������������֮�䣻��������һ�Ųݣ������˭����һ�Ųݣ�����Ǹ��˵�Բ���
	option 110 #���̵�
	talk
endproc

PROC 110
	OPEN_NPC_SHOP 
ENDPROC


;proc 2
;	get_trig_task 1 2
;	if 2 = 1910
;		task_fini 1910 1
;	endif
;
;endproc
