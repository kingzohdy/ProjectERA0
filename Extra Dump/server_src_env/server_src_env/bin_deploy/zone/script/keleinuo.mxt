

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������������ʲô��ʧ��ͳͳ���Լ��Ĵ��������ȣ��Ӵ�ѧϰ�Ĺ��������󾫣�ֱ��������ͬ�Ĵ��󣬲�Ҫ�ѹ�ʧ���õ����˼����ȥ�����ʧȥѧ�ԵĻ��ᣡ
	TASKCYCLE_ADD_OPTION 106 1 101

	
		task_add_option	2247 1 103
	talk
endproc 


PROC 101
	TASKCYCLE_DO_OPTION 106 1
ENDPROC

PROC 103
	task_do_option 2247 1
ENDPROC
