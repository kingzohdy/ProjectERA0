;npc�����

proc 1
	task_stat 4447 1 20
	 if 20 = 2
		 task_fini 4447 1
	 endif
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����������Ǵ���������$_player$����˵��������յ���ѡ�����������ʿ������δ����Ӣ���أ��ҳ�����ҲҪ����һ����
    add_talk #b#b��Ը�����˵˵��Ĺ�����

	
		
		TASK_ADD_OPTION 4485 0 100
		TASK_ADD_OPTION 4481 0 101
		TASK_ADD_OPTION 4483 0 102
		
		TASKCYCLE_ADD_OPTION 138 1 120
	talk
endproc 

proc 100
	TASK_DO_OPTION 4485 0
endproc
proc 101
	TASK_DO_OPTION 4481 0
endproc
proc 102
	TASK_DO_OPTION 4483 0
endproc
PROC 120	
	TASKCYCLE_DO_OPTION 138 1
ENDPROC
