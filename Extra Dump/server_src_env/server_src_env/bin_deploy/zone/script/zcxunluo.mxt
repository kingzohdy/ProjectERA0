;Ѳ������


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�衭���衭
	add_talk #b#b#b#b�޷��ֿ�������
	add_talk #b#b�衭
	add_talk #b#b#b#b�Ų������㣬�����������ˡ�
	add_talk #b#b���衭
		
	task_stat 4148 1 20
	if 20 = 2
		option 150 #������������ȡ���ɼ�����������
	endif
	talk
endproc 	

proc 150
	task_stat 4148 1 20
	if 20 = 2
		add_item_num_inpak 5301002 1 0 0
	endif
	
endproc
