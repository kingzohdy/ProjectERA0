
;С��
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b���˶�˵�Ƽ������ѣ������ܽпƼ����������˶����ü�ϰ�Ƽ������ף�ȴ�����ɸ���Ϊ����֮����
	add_talk #b#bÿ��һ���Ҷ��������������տƼ����ϣ�ϣ����Ҳ��ÿ�ܶ������ײ��ϣ�����Ϊ�����ǺܳԿ������飬�����˶������Ƽ������ȣ����һ�����һ���ٱ��䡣����������ʲô���������˲�֪����

		task_add_option 30016 15 100
		task_add_option 30017 15 101
		task_add_option 30018 15 102
		
		TASKCYCLE_ADD_OPTION 131 1 120
	talk
endproc



PROC 100
	task_do_option 30016 1
ENDPROC
PROC 101
	task_do_option 30017 1
ENDPROC
PROC 102
	task_do_option 30018 1
ENDPROC

PROC 120	
	TASKCYCLE_DO_OPTION 131 1
ENDPROC









