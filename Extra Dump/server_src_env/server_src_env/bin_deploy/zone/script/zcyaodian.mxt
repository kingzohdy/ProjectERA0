

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ʵ��ʼ�Ҳ�������ʳ��ģ���Ϊ�Ҳ���˵��������
	add_talk #b#b��һ��Ҫ��һ�ԣ��Ҳ�Ը���������ƭ�����ߣ������ǵع��ͣ�ȴ˵�ǻ����ͣ���������һ��û����������
	add_talk #b#b��������������֤ʵ�����˴��3�����ң����Ըо�Ӣ���ˣ�����ǿ׳�ˣ����һ·�����
	add_talk #b#b������ǿ���Ƽ���λð��;�е���ʿ��������۵�ʳ����˺�֤���������ˣ�ǿ׳ͦ�Σ�
	add_talk #b#b#c186506���ۻظ�������ֵ���롰ħ��ֵ����ʳ��#n	
	
	;��һ�
	task_stat 4434 1 51
	if 51 = 2
		option 510 ����ο�ʡ�����������
	endif
	
	task_add_option 1092 1 101
	task_add_option 1093 1 102
	task_add_option 1301 1 103
	task_add_option 1081 1 104
;	task_add_option 3004 1 105
	
;	task_add_option 1086 1 106
;	task_add_option 1087 1 107
;	task_add_option 1088 1 108
	
		OPTION 110 #(����)���̵�

	TASK_STAT 3004 1 20
	if 20 = 2
;		option 300 ʱ���̫���ˣ��ܷ�����һ�̣�
	endif
	talk
endproc 

PROC 101
	task_do_option 1092 1
ENDPROC
PROC 102
	task_do_option 1093 1
ENDPROC
PROC 103
	task_do_option 1301 1
ENDPROC
PROC 104
	task_do_option 1081 1
ENDPROC
;PROC 105
;	task_do_option 3004 1
;ENDPROC

PROC 106
	task_do_option 1086 1
ENDPROC
PROC 107
	task_do_option 1087 1
ENDPROC
PROC 108
	task_do_option 1088 1
ENDPROC



PROC 110
	OPEN_NPC_SHOP
ENDPROC

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ȼ���ԣ������Ǹ��������ء�
	add_talk #b#b����������ֱ�ӽ����͵�Ŀ�ĵأ���;����ͣ��Ӵ��
	add_talk #b#b��ȷ��Ҫ����ֱ��೵��
		option 301 #��ֱ�ӽ����͵�Ŀ�ĵذɣ�
		option 302 #�����������Լ���ȥ�����ܹ��ʱ��ǣ�
	talk
endproc
proc 301
;	NEW_OWNER_TMP_ITEM 1460071 27100 24900 0 0 0 311 1095 1 3600 1	
	AUTO_MOVE 0 1000 29400 26700 1460071 0 3081 1 3600
endproc 
proc 302
	return
endproc 
proc 2
	get_trig_task 1 2
	if 2 = 1093
		task_fini 1093 1
	endif 
;

	if 2 = 1301
		task_fini 1301 1
	endif 
	if 2 = 3110
;		DIALOG 1
;		add_talk $_npc$:
;		add_talk #b#b��ʵ��ʼ�Ҳ�������ʳ��ģ���Ϊ�Ҳ���˵��������
;		add_talk #b#b��һ��Ҫ��һ�ԣ��Ҳ�Ը���������ƭ�����ߣ������ǵع��ͣ�ȴ˵�ǻ����ͣ���������һ��û����������
;		add_talk #b#b��������������֤ʵ�����˴��3�����ң����Ըо�Ӣ���ˣ�����ǿ׳�ˣ����һ·�����
;		add_talk #b#b������ǿ���Ƽ���λð��;�е���ʿ��������۵�ʳ����˺�֤���������ˣ�ǿ׳ͦ�Σ�
;		add_talk #b#b#c186506���ۻظ�������ֵ���롰ħ��ֵ����ʳ��#n	
;				
;			task_fini 3110 1
;			option 300 ʱ���̫���ˣ��ܷ�����һ�̣�
		talk

	endif 
	
	
endproc

;��һ�
proc 510
	GET_ITEM_NUM_INPAK 5301004 1 10
	STRVAR flag
	var flag 0
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5301004 1 11
		if 11 = 0
			var flag 1
			task_fini 4434 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#bлл�㣬$_player$���뽫�ҵ�ף���������ͣ�˳������лл����
			add_talk #b#b�һ���ܶ�Ĳ��ϣ��Ӷ����ֵĳ��ף������Ҹ��ĸ�ҥ���������õĻ�Ц������һ����ζ���ȣ��װ��ģ�һ����ף���հɣ�
			option 511 #û����
			talk
		endif
	endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg ���ı�����û�С���Ʒ�С���
		set_sys_msg_mode 1 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc