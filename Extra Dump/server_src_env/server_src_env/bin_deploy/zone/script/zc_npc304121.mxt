proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b������ʵ�������������������ܹ������еó���Ը���ֺα�����ʵ����ʧ����ںޣ�
	add_talk #b#b��һ�Σ�������ζ�Ҫ���ֵ�ס�ڰ�����Ϯ����Ϊ��ѧ�о�С����ȡ�������ʱ�䳷�ˡ�
	add_talk #b#b
	add_talk #b#b#cFF0080���������ȥ��ɽ���������Э�����������#n
	add_talk #c186506�ر���ʾ��
	add_talk #b#b��������Խ�࣬������õľ���Խ��
	add_talk #b#b���ѵȼ�Խ�ͣ������Ѷ�Խ��
	add_talk #b#b������ֻ����˵ȼ��йأ������ѵȼ��޹�
	
	OPTION 201 #���븱��
	
	task_add_option 3317 1 101
	
	talk
endproc 

PROC 101
	task_do_option 3317 1
ENDPROC

proc 201
	OPEN_PWORLD_UI 14
endproc