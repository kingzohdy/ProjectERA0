;�����е�����
proc 1

	strvar m
	get_pworld_var 24 32 1 m
	if m != 10
		dialog 1
		add_talk $_npc$:
		add_talk #b#b����һ��׼��������������ʱ���Կ�ʼ���롣
		
		option 150 #���ǿ�ʼ�ɣ�����׼������
		option 151 #���������롿˵��
		
		talk
	else
		dialog 1
		add_talk $_npc$:
		add_talk #b#b�������Ѿ�˳�������10�����롣
		add_talk #b#b������������ɽ���Ľ�����
		
		option 152 ��ȡ���뽱��
		
		talk
	endif
	
endproc

proc 150
	strvar startflag
	var startflag 1
	set_pworld_var 0 32 @startflag 1
	player_close_dialog1 302129 1
	
endproc 

proc 151
	dialog 1
	add_talk $_npc$:
	add_talk #c186506�ʱ�䣺
	add_talk #b#bÿ���κ�ʱ��#n
	add_talk #c005ebe��淨��
	add_talk #b#bʱ���ڰ�������ȷ�ĺ���˳��������ƺ���#n
	add_talk #b#b
	add_talk #c6d00d2�������
	add_talk #b#b1. ˳�����10����Ŀ��������ȡ���ҽ���
	add_talk #b#b2. �ɹ�����Խ�࣬��������Խ��#n
	talk
endproc 

proc 152
	; get award
	strvar m p
	strvar sex
	strvar level
	strvar job
	get_pworld_var 48 32 1 m
	get_player_base_att 1 sex level job

	; ((level-60)*50+500)*m
	sub @level 60
	mul @level 50
	add @level 500
	mul @level @m
	
	get_pworld_var 56 32 1 p
	if p = 0
		add_money 0 @level 20 1
		new_npc 1 333162 7668 8980 180 csm333162.mac
	endif
	del_npc 302129 1
	
	; flag already get the price
	var p 1
	set_pworld_var 56 32 @p 1
endproc 












