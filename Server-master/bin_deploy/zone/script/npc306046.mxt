;306046 ϣ���ء�����


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����������˰䲼�ĺ�ɫ��ɱ�������˭������Ҫ��ϻ�ɱ���ְ��������ֶ������˵�Ǳ����вʵ����̫���ˡ�
	add_talk #b#b�㲻֪������ʱ����㷢����������Ĵ��ڵ��������˱�����һ�����εĵ�һֱʱ��׼�����㱳��ͱ��һ����
	add_talk #b#b
	add_talk #c005ebe#b#b��������������ˣ����飩���䵽�����ڣ�
		option 100 ��Ұ���֡����˱�����
		option 200 ��Ұ���֡����鱨����
		
	TASK_ADD_OPTION 4641 0 300
	
	talk
endproc 

proc 100
	choose 2 101 102 �Ƿ�ȷ����Ҫ��Ұ���֡����˱�������
endproc

proc 101
	apply_pworld 516 0 0
endproc

proc 200
	choose 2 201 102 �Ƿ�ȷ����Ҫ��Ұ���֡����鱨������
endproc

proc 201
	apply_pworld 516 1 0
endproc

proc 300
	task_do_option 4641 1
endproc
