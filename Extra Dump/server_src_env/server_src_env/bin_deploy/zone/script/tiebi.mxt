;NPC�ű� - ����

;NPC�ű�Ĭ��ִ���¼�
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�������ڸ����ˣ������ʱ�����ֹۣ�����ұ�������ѱ鲼ĩ��׷���ߵ������ߣ����Ǳ���ץ��ʱ�佫�����������
	add_talk #b#b
	add_talk #b#b#c186506�����ͨ���򿪿�����Ϸ��ġ��������֡���ť�һ����˻��ֽ�����������ѯ���ֹ�����ʣ��ʱ�䡣#n
	task_add_option 3680 1 100
	talk
endproc

proc 100
	task_do_option 3680 1
endproc



