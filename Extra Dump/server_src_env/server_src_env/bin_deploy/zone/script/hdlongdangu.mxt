

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#bһ���춼���л��㶼�������������Ҫ��Ҫ���������μӻ����
	add_talk #b#b
	add_talk #c186506�ر���ʾ��
	add_talk #b#b[45-64]���μ������ȣ��ɻ�ô�������ֵ
	add_talk #b#b��������Խ�࣬������õľ���Խ��
	add_talk #b#b���ѵȼ�Խ�ͣ������Ѷ�Խ��
	add_talk #b#b������ֻ����˵ȼ��йأ������ѵȼ��޹�#n
	add_talk #b#b
	add_talk #b#b#cff0080[65-80]��ֻ�����������л��10%����
	add_talk #b#b���[������-����]����ɻ��[��ʯ������]#n


	
	task_add_option 3313 1 100
	task_add_option 3314 1 101
	task_add_option 3311 1 102
	task_add_option 3312 1 103
	task_add_option 3102 1 105
	
	
	task_add_option 3315 1 104
	
	OPTION 201 #����������
	
;	option 202 #(����)�����̵�
;	option 230 #������֮��
;	option 240 #����
;    option 241 #��ʾ
	
	talk
endproc 

proc 202
	OPEN_NPC_SHOP 
endproc


proc 230
	OPEN_UI 36
endproc
proc 240
	strvar X Y
	GET_PLAYER_MAPINFO 0 0 0 X Y
	effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9577_16.ent")
	effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9577_18.ent")	

;	effect 0 0 playEffct(31000,31800,"effect\\entity\\9544_16.ent")	

endproc

PROC 100
	task_do_option 3313 1
ENDPROC
PROC 101
	task_do_option 3314 1
ENDPROC
PROC 102
	task_do_option 3311 1
ENDPROC
PROC 103
	task_do_option 3312 1
ENDPROC
PROC 104
	task_do_option 3315 1
ENDPROC
PROC 105
	task_do_option 3102 1
ENDPROC


proc 201

;	ENTER_PWORLD 211 0 0
;		OPEN_PWORLD_UI 211
		OPEN_PWORLD_UI 13
endproc









