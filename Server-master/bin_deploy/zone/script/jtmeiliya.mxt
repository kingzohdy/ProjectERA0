
;÷����
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ҫ��Ϊʲô����û�����ͣ�������Լ�ȥ���飬��������������ȫ������һ����Ρ���Ҳ�������ꡢ��ʮ�����Ҳ��ﵽ�����Σ������ȣ�����Ҫ�������ǣ���Ҫ�԰��ݶ�˹����ֳ����Ե��ҳϣ�
	task_add_option 1562 1 103
	task_add_option 1563 1 104
	task_add_option 1564 1 105
	task_add_option 1565 1 106
	task_add_option 1566 1 107
	task_add_option 1567 1 108
	strvar Ret
	task_stat 1565 1 Ret
	if Ret = 2
		option 150 ��ʼ����
	
	endif
	talk
endproc 

PROC 103
	task_do_option 1562 1
ENDPROC
PROC 104
	task_do_option 1563 1
ENDPROC
PROC 105
	task_do_option 1564 1
ENDPROC
PROC 106
	task_do_option 1565 1
ENDPROC
PROC 107
	task_do_option 1566 1
ENDPROC
PROC 108
	task_do_option 1567 1
ENDPROC

proc 2
	get_trig_task 1 2
 
	if 2 = 1567
		task_fini 1567 1
	endif 

endproc



proc 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����$_player$��
		option 151 ��
		option 300 #����
	talk
endproc
proc 300
	strvar Mid Hp
	GET_PLAYER_ID 1 Mid 
	EFFECT_ID 2 @Mid playMontion(%d,"8000_5")
	
	GET_ID_PLAYER_INFO 0 2 Hp
	if Hp > 1000
		ADD_PLAYER_HP 0 -1000 1
		return
	else
		sub @Hp 1
		mul @Hp -1
		ADD_PLAYER_HP 0 @Hp 1
		
	endif
endproc
proc 151
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bū���ܻ��������
                  
		option 300 #��
		option 152 ����
	talk
endproc
proc 152
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ը���ΪѪɫ׷���ߵ�һԱ��
                  
		option 153 Ը��
		option 300 #��Ը��
	talk
endproc
proc 153
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bѪ�Ǻ�ɫ����
 
		option 154 ��
		option 300 #����
	talk
endproc
proc 154
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����֮����ᱳ��������

		option 300 #��
		option 155 ����
	talk
endproc
proc 155
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ը��Ϊ��Ѫɫ�ƻ��׳�������

		option 156 Ը��
		option 300 #��Ը��
	talk
endproc
proc 156
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ը�⽫�����׸����ݶ�˹��

		option 157 Ը��
		option 300 #��Ը��
	talk
endproc
proc 157
	task_fini 1565 1
endproc















