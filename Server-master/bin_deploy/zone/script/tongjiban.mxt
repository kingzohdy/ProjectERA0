

proc 1
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b����ͨ��
	add_talk #b
	add_talk #b#bΪ��һ����������ɳĮ�ΰ�������ȷ�������������ȶ���չ����������Ⱥ�������Ʋ���ȫ���ƶ����÷�չ���־����԰�Ŭ��˹����е����������������ش�Σ����������ͨ�����ش�ͨ�档
	add_talk #b#b��ͨ���Է���֮����ʩ�С�
	add_talk #b
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#bůȪ�� ÷��Ү����
	add_talk #b
	add_talk #b#b�����ش�Σ������������

	task_add_option 1591 1 101
	task_add_option 1592 1 102
	
	
	
	task_add_option 1698 1 103
	task_add_option 1699 1 104
	talk
endproc 


PROC 101
	task_do_option 1591 1
ENDPROC
PROC 102
	task_do_option 1592 1
ENDPROC
PROC 103
	task_do_option 1698 1
ENDPROC
PROC 104
	task_do_option 1699 1
ENDPROC
