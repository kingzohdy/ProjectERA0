
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ڰ�����ڴ���˰�����޴�ĸ���Ҫ���������ڷḻ����Դ��ʹ������������׳��
	add_talk #b#bΪ�˹����Ǽ᲻�ɴݵ�Ҫ��������������֯����ʷ������ǿ������ݡ�
	add_talk #b#b���ڸ���Ҫ���ѱ��������Ű�Χ��ս��һ��������
	add_talk #b#b
	add_talk #b #c18650680������ڴ˴���������Ҫ���������ͼ��#n

	
	task_add_option 1076 1 100
	task_add_option 1077 1 101
	task_add_option 3104 1 102

	strvar Ret
	
	task_stat 1077 1 Ret
	if Ret = 2
;		option 300 ʱ���̫���ˣ��ܷ�����һ�̣�
	endif
	
	get_player_base_att 1 2 3 4
	if 3 >= 20
;		option 202 #��������ҹ��ڣ�����200���ң�
;		option 200 #��������Ŭ��˹������200���ң�
;		option 201 #������ʮ���ſ󶴣�����200���ң�
;		option 204 #�������ƽ�󾮣�����200���ң�
;		option 203 #����Խ��ͨ��������200���ң�
	;	transmit 1000 28154 6672 1
	
;	else
;		add_sys_msg_start
;		add_sys_msg ʵ�ڱ�Ǹ,���ĵȼ�δ�ﵽ10��,��������ʱ��������...
;		set_sys_msg_mode 3 0 0
;		msg_info

	endif
	
	option 400 ����ǰ������Ҫ���������ͼ��
	
	talk
endproc 

proc 400
	strvar lv
    GET_PLAYER_BASE_ATT 0 0 lv 0
	if lv >= 80
		choose 2 401 402 �Ƿ�ȷ��ǰ������Ҫ���������ͼ���� 
	else
		add_sys_msg_start
		add_sys_msg ����Ҫ����Σ�յش�������80������ǰ��
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 401
	strvar baoming ret
	cancel_span_queue_check baoming ret
	if @ret = 0 
		if @baoming = 1
			choose 2 403 402 ǰ������Ҫ����������ǰս���Ŷ�\n�Ƿ�ȷ��ǰ������Ҫ���������ͼ����
		elseif @baoming = 2
			choose 2 403 402 ǰ������Ҫ����������ǰĩ����ҫ�����Ŷ�\n�Ƿ�ȷ��ǰ������Ҫ���������ͼ����
		else
			call 404
		endif
	endif
endproc

proc 402
endproc

proc 403
	cancel_span_queue_check baoming ret
	if @ret = 0
		if @baoming = 1 or @baoming = 2
			cancel_span_queue @baoming 10
			if @10 = 0
				call 404
			endif
		else
			call 404
		endif
	endif
endproc

proc 404	
	get_world_camp_in_span_map 10 11
	if @10 = 0
		strvar PosX PosY
		if @11 = 1
			var PosX 6174
			var PosY 42436
		elseif @11 = 2 
			var PosX 43890
			var PosY 4912
		elseif @11 = 3
			var PosX 6226
			var PosY 6747
		elseif @11 = 4 
			var PosX 44075
			var PosY 43710
		elseif @11 = 5 
			var PosX 25481
			var PosY 48739
		elseif @11 = 6 
			var PosX 25329
			var PosY 2115
		elseif @11 = 7 
			var PosX 4672
			var PosY 25590
		elseif @11 = 8 
			var PosX 47433
			var PosY 25637
		else
			return 
		endif
			
		span_map_transmit 4500 @PosX @PosY 1
	endif
endproc

proc 100
    task_do_option 1076 1
endproc
proc 101
    task_do_option 1077 1
endproc
proc 102
    task_do_option 3104 1
endproc
proc 2
	get_trig_task 1 2

endproc 
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
;	NEW_OWNER_TMP_ITEM 1460069 26600 11000 0 0 0 309 1095 1 3600 1
	AUTO_MOVE 0 1000 20900 17100 1460069 0 3081 1 3600
endproc
proc 302
	return
endproc 
proc 200
;ɳĮ�ع����
	DEC_MONEY 0 200 20 190
	if 190 = 0
		transmit 2300 20400 5500 1
	else
		add_sys_msg_start
		add_sys_msg ûǮҲ�������ȥô����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
proc 202
;��ҹ���
	DEC_MONEY 0 200 20 190
	if 190 = 0
		transmit 2200 21500 22400 1
	else
		add_sys_msg_start
		add_sys_msg ûǮҲ�������ȥô����
		set_sys_msg_mode 3 0 0
		msg_info
	endif

endproc
proc 201
;�����ع�
	DEC_MONEY 0 200 20 190
	if 190 = 0
		transmit 2400 17400 22500 1
	else
		add_sys_msg_start
		add_sys_msg ûǮҲ�������ȥô����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 204
;������
	DEC_MONEY 0 200 20 190
	if 190 = 0
		transmit 2400 3800 16000 1
	else
		add_sys_msg_start
		add_sys_msg ûǮҲ�������ȥô����
		set_sys_msg_mode 3 0 0
		msg_info
	endif

endproc

proc 203
;������
	DEC_MONEY 0 200 20 190
	if 190 = 0
		transmit 2500 22300 36000 1
	else
		add_sys_msg_start
		add_sys_msg ûǮҲ�������ȥô����
		set_sys_msg_mode 3 0 0
		msg_info
	endif

endproc


