; 
; Я������\������ҹ��NPC

proc 1



	dialog 1
	add_talk $_npc$:
	add_talk #b#b��һ��ս��������ʱ�յ�ת�ơ������ı任��ս������ʷ�����������ϵ�������ȥ��ֱ�������ٴ����ִ�ء�
	task_add_option 2241 1 101
	task_add_option 2242 1 102
	task_add_option 2243 1 103
	task_add_option 2244 1 104
	task_add_option 2245 1 105

	get_pworld_var 0 32 1 10
	if 10 = 1
			add_talk #b#b׼���þ������һ��������ô��
			option 500 �ǵģ���ʦ
	endif
	talk
endproc
proc 101
    task_do_option 2241 1
endproc
proc 102
    task_do_option 2242 1
endproc

proc 103
    task_do_option 2243 1
endproc
proc 104
    task_do_option 2244 1
endproc
proc 105
    task_do_option 2245 1
endproc

proc 2
	
	get_trig_task 1 2
	get_pworld_var 0 32 1 10
	if 10 = 1
		dialog 1
		add_talk $_npc$:

		add_talk #b#b׼���þ������һ��������ô��
		option 500 �ǵģ���ʦ
		talk

		
	endif
endproc



proc 500
	dialog 1
	add_talk $_npc$:

	add_talk #b#b�����������֮�ش�̫���ˣ������ڵĺڰ������������ϻ����Ѿ��ޱ�ǿ��Ҳ����úͶ�������һ������������
	option 501 #���ɣ���ʦ��ʣ�µ��¾ͽ������ǰ�
	option 502 #���������Ƭ�̣��ҵĶ��ѻ�û׼����
	talk

endproc
proc 501
	
	var 10 2
	set_pworld_var 0 32 @10 1
	add_sys_msg_start
	add_sys_msg �¸ҵ�$_player$�����˿�˹̹��������ʽ
	set_sys_msg_mode 4 0 0
	map_msg_info

endproc














