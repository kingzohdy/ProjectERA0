;��Բnpc

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��֪������ڵ�������
	add_talk #b#b��֪�������Ϊʲô��������
	add_talk #b#b��֪��������˭��������

	
	TASK_ADD_OPTION 4486 0 103
	;TASKCYCLE_ADD_OPTION 139 1 120

	option 150 #(����)���Ӳ����̵�
	;option 151 #����
	
	talk
	
endproc 
proc 103
	TASK_DO_OPTION 4486 0
endproc
PROC 120	
	TASKCYCLE_DO_OPTION 139 1
ENDPROC

proc 150
	OPEN_NPC_SHOP 
endproc


