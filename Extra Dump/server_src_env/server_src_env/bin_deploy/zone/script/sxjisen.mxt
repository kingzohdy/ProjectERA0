;;;
;l;
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b������Ż��������ǣ�ʹ���ǹ�����Ԯ����֪������û���������˵�Ԯ�֣����ǻ���ô������Ȼ�������������޷�����������ŵ���Ϯ�������⻹�ý��У�������$_player$��

		task_add_option 1851 1 100
		task_add_option 1852 1 101
		task_add_option 1853 1 102
		task_add_option 1864 1 103
		
		option 200 #(����)���̵�
	talk

endproc

proc 200
	OPEN_NPC_SHOP 
endproc

PROC 100
	task_do_option 1851 1
ENDPROC
PROC 101
	task_do_option 1852 1
ENDPROC
PROC 102
	task_do_option 1853 1
ENDPROC

PROC 103
	task_do_option 1864 1
ENDPROC

