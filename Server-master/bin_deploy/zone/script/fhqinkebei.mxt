; Ƥ��ϣ

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b$_player$���㿴���Ұְ���������
	add_talk #b#b<һ�����۵����������������>
	add_talk #b#b�������èè��ʱ��˯���ˣ������������Ƕ������ˣ������ҵĲ����ޣ�Ҳ�����ˡ���
		task_add_option 1936 1 100
		task_add_option 1937 1 101

	talk
endproc

PROC 100
	task_do_option 1936 1
ENDPROC
PROC 101
	task_do_option 1937 1
ENDPROC



;proc 2
;	get_trig_task 1 2
;	if 2 = 1910
;		task_fini 1910 1
;	endif
;
;endproc
