

proc 1
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b����ͨ��
	add_talk #b
	add_talk #b#bΪ��һ������ŭ���Į�ΰ�������ȷ�������������ȶ���չ����������Ⱥ�������Ʋ���ȫ���ƶ����÷�չ���־����԰�Ŭ��˹����е����������������ش�Σ����������ͨ�����ش�ͨ�档
	add_talk #b#b��ͨ���Է���֮����ʩ�С�
	add_talk #b
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#bůȪ�� ÷��Ү����
	add_talk #b
	add_talk #b#b�����ش�Σ������������
	
	task_add_option 1591 1 101
	task_add_option 1592 1 102
	talk
endproc 


PROC 101
	task_do_option 1591 1
ENDPROC
PROC 102
	task_do_option 1592 1
ENDPROC
