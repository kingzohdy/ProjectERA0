;���֮��

PROC 1					;���������¼�
	strvar a
	var a 0
	set_pworld_var 0 32 @a 1
	;aɱ�ּ���
	var 10 0
	set_pworld_var 4 32 @10 1
	;����
	strvar c
	var c 0
	set_pworld_var 8 32 @c 1
	;��ʱ
	strvar d
	var d 0
	set_pworld_var 12 32 @d 1
	;��������id
	strvar b
	BCT_GET_CURR_LAYER b 0
	;var b @10
	set_pworld_var 16 32 @b 1
	;ˢ����
	strvar e
	var e 0
	set_pworld_var 20 32 @e 1
	;ˢ�¹���ʵ��id
	strvar f
	var f 0
	set_pworld_var 24 32 @f 1
	;ˢ���Ӽ�ʱ
	strvar g
	var g 0
	set_pworld_var 28 32 @g 1
	;ת���¼�id
	strvar h
	var h 0
	set_pworld_var 32 32 @h 1
	;�����
	strvar i
	var i 0
	set_pworld_var 36 32 @i 1
	;����ʵ��id
	
	strvar j
	var j 0
	set_pworld_var 100 8 @j 1
	;ɾnpc����

	strvar k
	var k 0
	set_pworld_var 102 8 @k 1
	;�๦�ܻ����˰���
	
ENDPROC

PROC 3
	get_pworld_var 24 32 1 f
	get_pworld_var 20 32 1 e
	;BCT_GET_CURR_LAYER 10 0
	;set_pworld_var 4 32 @10 1
	get_pworld_var 4 32 1 10
	get_pworld_var 8 32 1 c
	;add_sys_msg_start
	;add_sys_msg f=$@f$,10=$@10$,e=$@e$
	;set_sys_msg_mode 3 0 0
	;map_msg_info

	if @10 = 5
		get_pworld_var 102 8 1 k
		if @k = 1
			add f 1
			set_pworld_var 24 32 @f 1
			if @f = 5
			add_sys_msg_start
			add_sys_msg 5�㵽�ˣ��Ͻ����ö๦�ܻ����˰ɣ�
			set_sys_msg_mode 3 0 0
			map_msg_info
			var f 0
			set_pworld_var 24 32 @f 1
			endif
		endif
	endif
		
		


	if @10 = 3 or @10 = 14 or @10 = 24 or @10 = 34 or @10 = 54
		add f 1
		set_pworld_var 24 32 @f 1
		if f = 20
			rand h 4
			;add_sys_msg_start
			;add_sys_msg h=$@h$
			;set_sys_msg_mode 3 0 0
			;map_msg_info
			switch h
				case 0
				NEW_TMP_ITEM 1440000 5400 5400 0 0 30 1
				endcase
				case 1	
				NEW_TMP_ITEM 1440000 7400 5400 0 0 30 1
				endcase
				case 2	
				NEW_TMP_ITEM 1440000 7400 7400 0 0 30 1
				endcase
				case 3	
				NEW_TMP_ITEM 1440000 5400 7400 0 0 30 1
				endcase
			endswitch
			add_sys_msg_start
			add_sys_msg ƽ̨�߳���һ������Ʒ��30�����ʧ
			set_sys_msg_mode 3 0 0
			map_msg_info
			var f 0
			set_pworld_var 24 32 @f 1
		endif
	endif

	

	get_pworld_var 100 8 1 j
		if @j = 1
			add f 1
			set_pworld_var 24 32 @f 1
			if f = 2
				DEL_NPC 304047 1
				var j 0
				set_pworld_var 100 8 @j 1
			endif
		endif
	;�յ����ſ�ʼ��ʱ����ʱ�����ɾ��npc
			

ENDPROC

PROC 4
	GET_MON_INFO 0 d 0 0 0 0 1 1
	if @d != 232004 and @d != 230014 and @d != 233032 and @d != 204003 and @d != 204027  and @d != 284015 and @d != 284016 and @d != 204027 and @d != 204108 
	get_pworld_var 0 32 1 a
	add a 1
	set_pworld_var 0 32 @a 1
	endif

	get_pworld_var 4 32 1 10
	if @10 = 1 or @10 = 3 or @10 = 7 or @10 = 11 or @10 = 14 or @10 = 21 or @10 = 24 or @10 = 31 or @10 = 34 or @10 = 41 or @10 = 42 or @10 = 44 or @10 = 47 or @10 = 51 or @10 = 52 or @10 = 53 or @10 = 57
		if a = 3
		add_sys_msg_start
		add_sys_msg ף���㣬��ͨ���˵��ĵ�$@10$��Ŀ���
		set_sys_msg_mode 3 0 0
		map_msg_info
		BCT_PASS 0
		;���֮�ģ�����
		if @10 > 15
			call 300
		else
			bct_ui 0 0
		endif
		MAP_DEL_TMP_ITEM 1 0
		add_player_hp 0 99999 1
		add_player_mp 0 99999 1
		ADD_FIGHT_PET_HP 0 99999 1
		
		
		var a 0
		set_pworld_var 0 32 @a 1
		endif
	endif

	if @10 = 2 or @10 = 12 or @10 = 22 or @10 = 33 or @10 = 43 or @10 = 54
		if a = 8
		add_sys_msg_start
		add_sys_msg ף���㣬��ͨ���˵��ĵ�$@10$��Ŀ���
		set_sys_msg_mode 3 0 0
		map_msg_info
		BCT_PASS 0
		;���֮�ģ�����
		if @10 > 15
			call 300
		else
			bct_ui 0 0
		endif
		MAP_DEL_TMP_ITEM 1 0
		add_player_hp 0 99999 1
		add_player_mp 0 99999 1
		ADD_FIGHT_PET_HP 0 99999 1

		
		var a 0
		set_pworld_var 0 32 @a 1
		endif
	endif

	if @10 = 4 or @10 = 13 or @10 = 17 or @10 = 23 or @10 = 27 or @10 = 32 or @10 = 37
		if a = 2
		add_sys_msg_start
		add_sys_msg ף���㣬��ͨ���˵��ĵ�$@10$��Ŀ���
		set_sys_msg_mode 3 0 0
		map_msg_info
		get_pworld_var 36 32 1 i
		target_add_status 1 @i 1206 1 0 1
		target_add_status 1 @i 1207 1 0 1
		target_add_status 1 @i 1208 1 0 1
		target_add_status 1 @i 1209 1 0 1
		BCT_PASS 0
		;���֮�ģ�����
		if @10 > 15
			call 300
		else
			bct_ui 0 0
		endif
		MAP_DEL_TMP_ITEM 1 0
		add_player_hp 0 99999 1
		add_player_mp 0 99999 1
		ADD_FIGHT_PET_HP 0 99999 1

		
		var a 0
		set_pworld_var 0 32 @a 1
		endif
	endif
	
	if @10 = 8 or @10 = 18 or @10 = 28 or @10 = 38 or @10 = 49 or @10 = 59
		if a = 15
		add_sys_msg_start
		add_sys_msg ף���㣬��ͨ���˵��ĵ�$@10$��Ŀ���
		set_sys_msg_mode 3 0 0
		map_msg_info
		BCT_PASS 0
		;���֮�ģ�����
		if @10 > 15
			call 300
		else
			bct_ui 0 0
		endif
		MAP_DEL_TMP_ITEM 1 0
		add_player_hp 0 99999 1
		add_player_mp 0 99999 1
		ADD_FIGHT_PET_HP 0 99999 1

		
		var a 0
		set_pworld_var 0 32 @a 1
		endif
	endif

	if @10 = 9 or @10 = 19 or @10 = 29 or @10 = 39 or @10 = 48 or @10 = 58
		if a = 10
		add_sys_msg_start
		add_sys_msg ף���㣬��ͨ���˵��ĵ�$@10$��Ŀ���
		set_sys_msg_mode 3 0 0
		map_msg_info
		BCT_PASS 0
		;���֮�ģ�����
		if @10 > 15
			call 300
		else
			bct_ui 0 0
		endif
		MAP_DEL_TMP_ITEM 1 0
		add_player_hp 0 99999 1
		add_player_mp 0 99999 1
		ADD_FIGHT_PET_HP 0 99999 1

		
		var a 0
		set_pworld_var 0 32 @a 1
		endif
	endif

	

	if @10 = 10 or @10 = 15 or @10 = 20 or @10 = 25 or @10 = 30 or @10 = 40 or @10 = 5 or @10 = 45 or @10 = 50 or @10 = 55 or @10 = 60
		BCT_PASS 0
		add_sys_msg_start
		add_sys_msg ף���㣬��ͨ���˵��ĵ�$@10$��Ŀ���
		set_sys_msg_mode 3 0 0
		map_msg_info
		MAP_DEL_TMP_ITEM 1 0
		add_player_hp 0 99999 1
		add_player_mp 0 99999 1
		ADD_FIGHT_PET_HP 0 99999 1
		var a 0
		set_pworld_var 0 32 @a 1
				new_mon 293102 1 6000 6000 0 0 0 0 0
				new_mon 293102 1 6400 6000 0 0 0 0 0
				new_mon 293102 1 6000 6400 0 0 0 0 0
				new_mon 293102 1 6400 6400 0 0 0 0 0
				new_mon 293102 1 6000 6800 0 0 0 0 0
				new_mon 293102 1 6800 6000 0 0 0 0 0
				new_mon 293102 1 6400 6800 0 0 0 0 0
				new_mon 293102 1 6800 6400 0 0 0 0 0
				new_mon 293102 1 6800 6800 0 0 0 0 0
	endif

	if @10 = 35 and d = 207206
		BCT_PASS 0
		add_sys_msg_start
		add_sys_msg ף���㣬��ͨ���˵��ĵ�$@10$��Ŀ���
		set_sys_msg_mode 3 0 0
		map_msg_info
		MAP_DEL_TMP_ITEM 1 0
		add_player_hp 0 99999 1
		add_player_mp 0 99999 1
		ADD_FIGHT_PET_HP 0 99999 1
		var a 0
		set_pworld_var 0 32 @a 1
				new_mon 293102 1 6000 6000 0 0 0 0 0
				new_mon 293102 1 6400 6000 0 0 0 0 0
				new_mon 293102 1 6000 6400 0 0 0 0 0
				new_mon 293102 1 6400 6400 0 0 0 0 0
				new_mon 293102 1 6000 6800 0 0 0 0 0
				new_mon 293102 1 6800 6000 0 0 0 0 0
				new_mon 293102 1 6400 6800 0 0 0 0 0
				new_mon 293102 1 6800 6400 0 0 0 0 0
				new_mon 293102 1 6800 6800 0 0 0 0 0
	endif

	
	GET_MON_INFO 0 d 0 0 0 0 1 1
	if @d = 232005 or @d = 284006 or @d = 233014 or @d = 204004 or @d = 204057 or @d = 205027
		add_sys_msg_start
		add_sys_msg ף���㣬��ͨ���˵��ĵ�$@10$��Ŀ���
		set_sys_msg_mode 3 0 0
		map_msg_info
		BCT_PASS 0
		;���֮�ģ�����
		if @10 > 15
			call 300
		else
			bct_ui 0 0
		endif
		MAP_DEL_TMP_ITEM 1 0
		add_player_hp 0 99999 1
		add_player_mp 0 99999 1
		ADD_FIGHT_PET_HP 0 99999 1
		var a 0
		set_pworld_var 0 32 @a 1
	endif
	
			
ENDPROC

PROC 5
	get_pworld_var 0 32 a 1
	var a 0
	set_pworld_var 0 32 @a 1

	var f 0
	set_pworld_var 24 32 @f 1
	get_player_id 1 i
	set_pworld_var 36 32 @i 1
	BCT_GET_CURR_LAYER 10 0
	set_pworld_var 4 32 @10 1
	var g @10
	add g 20
	set_pworld_var 28 32 @g 1
	SET_PWORLD_MAP_CURR
	;add_sys_msg_start
	;add_sys_msg g=$@g$
	;set_sys_msg_mode 3 0 0
	;map_msg_info
	call @g
	var c 0
	set_pworld_var 8 32 @c 1

ENDPROC

proc 6
	;add_sys_msg_start
	;add_sys_msg what the fuck
	;set_sys_msg_mode 3 0 0
	;map_msg_info
	bct_ui 1 0
	pworld_over
endproc

proc 7
	bct_ui 1 0
	pworld_over
endproc

proc 21
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�1��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 1
		var b 2
		set_pworld_var 16 32 @b 1
		task_stat 3172 1 2
		task_stat 3173 1 3
		if @2 = 2 or @3 = 2 or @2 = 1 or @3 = 1
		new_npc 1 304047 6415 4300 0 zhbuluoke.mac
		endif

		if @2 = 3 and @3 = 0
		new_npc 1 304047 6415 4300 0 zhbuluoke.mac
		endif
			
		BCT_CALL_MON 3 241000 0 300 6412 6396 0
	endif
endproc

proc 22
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�2��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 2
		var b 3
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 8 218000 0 800 6412 6396 0
		NEW_TMP_ITEM 1420002 6400 5000 0 0 0 1
	endif
endproc

proc 23
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�3��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 3
		var f 0
		var b 4
		set_pworld_var 24 32 @f 1
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 200009 0 500 6412 6396 e
		set_pworld_var 20 32 @e 1
		add_sys_msg_start
		add_sys_msg ��ס�����ű���ϣ��
		set_sys_msg_mode 4 0 0
		map_msg_info
	endif
endproc

proc 24
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�4��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 4
		var b 5
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 2 231021 0 300 6412 6396 0
		new_tmp_item 1400108 5700 5200 0 0 0 1
		new_tmp_item 1400109 7100 5200 0 0 0 1
		add_sys_msg_start
		add_sys_msg ʰȡ��ߵ��߿ɻ�ÿ���״̬��ʰȡ�ұߵ��߿ɻ�ü���״̬
		set_sys_msg_mode 4 0 0
		map_msg_info
	endif
endproc

proc 25
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�5��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 5
		var b 6
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 207200 0 300 6412 6396 e
		set_pworld_var 20 32 @e 1
		get_pworld_var 20 32 1 e
		TARGET_ADD_STATUS 3 @e 1095 1 7200 2
		task_stat 3342 1 2
		if @2 = 2
			add_sys_msg_start
			add_sys_msg 5�㵽�ˣ��Ͻ����ö๦�ܻ����˰ɣ�
			set_sys_msg_mode 3 0 0
			map_msg_info
			var k 1
			set_pworld_var 102 8 @k 1
		endif
		range_add_status 0 1206 1 0 1
		range_add_status 0 1207 1 0 1
	endif
endproc

proc 26
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�6��
	set_sys_msg_mode 3 0 0
	map_msg_info
	
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 6
		var b 7
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 232005 0 0 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1236 1 7200 2
		set_pworld_var 20 32 @e 1
		BCT_CALL_MON 4 232004 0 800 6412 6396 0
		add_sys_msg_start
		add_sys_msg �Ųȹ⻷��Ұ�������죬��ɱ���켴�ɹ���
		set_sys_msg_mode 4 0 0
		map_msg_info
	endif
endproc

proc 27
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�7��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 7
		var b 8
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 204009 0 500 6412 6396 0
	endif
endproc

proc 28
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�8��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 8
		var b 9
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 15 201001 0 1000 6412 6396 0
		NEW_TMP_ITEM 1460008 6400 5000 0 0 0 1
	endif
endproc

proc 29
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�9��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 9
		var b 10
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 10 231020 0 600 6412 6396 0
		;new_dyn_mon 1 204027 1 5400 5400 0 0 0 0 0 30 1
		;new_dyn_mon 1 204027 1 7400 5400 0 0 0 0 0 30 1
		;new_dyn_mon 1 204027 1 7400 7400 0 0 0 0 0 30 1
		;new_dyn_mon 1 204027 1 5400 7400 0 0 0 0 0 30 1
		BCT_CALL_MON 1 204027 0 0 5400 5400 0
		BCT_CALL_MON 1 204027 0 0 7400 7400 0
		BCT_CALL_MON 1 204027 0 0 5400 7400 0
		BCT_CALL_MON 1 204027 0 0 7400 5400 0


		;NEW_MON 204027 1 5400 5400 0 0 0 1 2
		;NEW_MON 204027 1 7400 5400 0 0 0 1 2
		;NEW_MON 204027 1 7400 7400 0 0 0 1 2
		;NEW_MON 204027 1 5400 7400 0 0 0 1 2
		add_sys_msg_start
		add_sys_msg ƽ̨�߲����Ŀŵ��ף��������￿�����׿ɿ��ٽ����ɱ
		set_sys_msg_mode 4 0 0
		map_msg_info
	endif
endproc

proc 30
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�10��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 10
		var b 11
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 207201 0 300 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1095 1 7200 2
		set_pworld_var 20 32 @e 1

		new_npc 3 304047 6415 4300 0 zhbuluoke.mac
		task_stat 3401 1 2
	;	task_stat 3171 1 3
	;	task_stat 3172 1 4
		if @2 = 2
			mon_speech @3 1 2 1 �����ˣ��������ڵ�ˮƽ������ʵ����̫Σ���ˣ���ʲô��Ҫ�Ұ�æ����
			add_sys_msg_start
			add_sys_msg �ۣ����޿˽��ڣ���ȥ����Ҫǩ��������Ҫ֤��ȥ�ɣ�
			set_sys_msg_mode 3 0 0
			map_msg_info
		endif
	endif
endproc

proc 31
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�11��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 11
		var b 12
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 213006 0 500 6412 6396 0
	endif
endproc

proc 32
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�12��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 12
		var b 13
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 8 213017 0 800 6412 6396 0
		new_tmp_item 1400003 6400 5000 0 0 0 1
	endif
endproc

proc 33
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�13��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 13
		var b 14
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 2 213007 0 300 6412 6396 0
		new_tmp_item 1400108 5700 5200 0 0 0 1
		new_tmp_item 1400109 7100 5200 0 0 0 1
	endif
endproc

proc 34
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�14��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 14
		var b 15
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 205003 0 500 6412 6396 0
		add_sys_msg_start
		add_sys_msg ��ס�����ű���ϣ��
		set_sys_msg_mode 4 0 0
		map_msg_info
		range_add_status 0 1206 1 0 1
		range_add_status 0 1207 1 0 1
	endif
	
endproc

proc 35
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�15��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 15
		var b 16
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 207202 0 0 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1095 1 7200 2
		set_pworld_var 20 32 @e 1
		new_tmp_item 1400106 5700 5200 0 0 0 1
		new_tmp_item 1400107 7100 5200 0 0 0 1
		add_sys_msg_start
		add_sys_msg ʰȡ�������֮Ȫ�ɻ�������ظ�״̬��ʰȡ�ұ�ħ��֮Ȫ�ɻ��ħ���ظ�״̬
		set_sys_msg_mode 4 0 0
		map_msg_info

		new_npc 3 304047 6415 4300 0 zhbuluoke.mac

		task_stat 3401 1 2
	;	task_stat 3171 1 3
	;	task_stat 3172 1 4
		if @2 = 2
			mon_speech @3 1 2 1 �����ˣ��������ڵ�ˮƽ������ʵ����̫Σ���ˣ���ʲô��Ҫ�Ұ�æ����
			add_sys_msg_start
			add_sys_msg �ۣ����޿˽��ڣ���ȥ����Ҫǩ��������Ҫ֤��ȥ�ɣ�
			set_sys_msg_mode 3 0 0
			map_msg_info
		endif

	;	if @3 = 3 and @4 = 0
	;	new_npc 1 304047 6415 4300 0 zhbuluoke.mac
	;	endif

	endif

endproc

proc 36
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�16��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 16
		var b 17
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 284006 0 0 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1236 1 7200 2
		BCT_CALL_MON 4 230014 0 800 6412 6396 0
		set_pworld_var 20 32 @e 1
		range_add_status 0 1208 1 0 1
		range_add_status 0 1209 1 0 1
	endif

endproc

proc 37
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�17��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 17
		var b 18
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 2 284007 0 300 6412 6396 e
		set_pworld_var 20 32 @e 1
	endif
endproc

proc 38
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�18��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 18
		var b 19
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 15 284009 0 1000 6412 6396 0
		new_tmp_item 1460006 6400 5200 0 0 0 1
	endif
endproc

proc 39
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�19��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 19
		var b 20
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 10 284008 0 600 6412 6396 0
		BCT_CALL_MON 1 204027 0 0 5400 5400 0
		BCT_CALL_MON 1 204027 0 0 7400 7400 0
		BCT_CALL_MON 1 204027 0 0 5400 7400 0
		BCT_CALL_MON 1 204027 0 0 7400 5400 0

		;NEW_MON 204027 1 5400 5400 0 0 0 1 2
		;NEW_MON 204027 1 7400 5400 0 0 0 1 2
		;NEW_MON 204027 1 7400 7400 0 0 0 1 2
		;NEW_MON 204027 1 5400 7400 0 0 0 1 2
	endif
endproc

proc 40
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�20��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 20
		var b 21
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 207203 0 0 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1095 1 7200 2
		set_pworld_var 20 32 @e 1
		new_tmp_item 1400106 5700 5200 0 0 0 1
		new_tmp_item 1400107 7100 5200 0 0 0 1
		range_add_status 0 1208 1 0 1
		range_add_status 0 1209 1 0 1

		new_npc 3 304047 6415 4300 0 zhbuluoke.mac

		task_stat 3401 1 2
		if @2 = 2 
			mon_speech @3 1 2 1 �����ˣ��������ڵ�ˮƽ������ʵ����̫Σ���ˣ���ʲô��Ҫ�Ұ�æ����
			add_sys_msg_start
			add_sys_msg �ۣ����޿˽��ڣ���ȥ����Ҫǩ��������Ҫ֤��ȥ�ɣ�
			set_sys_msg_mode 3 0 0
			map_msg_info
		endif
		
	endif
endproc

proc 41
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�21��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 21
		var b 22
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 230004 0 500 6412 6396 0
		range_add_status 0 1208 1 0 1
		range_add_status 0 1209 1 0 1
	endif
endproc

proc 42
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�22��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 22
		var b 23
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 8 213000 0 800 6412 6396 0
		NEW_TMP_ITEM 1420006 6400 5000 0 0 0 1
	endif
endproc

proc 43
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�23��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 23
		var b 24
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 2 226001 0 300 6412 6396 0
		new_tmp_item 1400108 5700 5200 0 0 0 1
		new_tmp_item 1400109 7100 5200 0 0 0 1
	endif
endproc

proc 44
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�24��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 24
		var b 25
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 200019 0 500 6412 6396 0
		add_sys_msg_start
		add_sys_msg ��ס�����ű���ϣ��
		set_sys_msg_mode 4 0 0
		map_msg_info
		range_add_status 0 1206 1 0 1
		range_add_status 0 1207 1 0 1
	endif
endproc

proc 45
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�25��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 25
		var b 26
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 207204 0 300 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1095 1 7200 2
		set_pworld_var 20 32 @e 1
		range_add_status 0 1208 1 0 1
		range_add_status 0 1209 1 0 1
	endif
endproc

proc 46
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�26��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 26
		var b 27
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 233014 0 0 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1236 1 7200 2
		BCT_CALL_MON 3 233032 0 300 6412 6396 0
		set_pworld_var 20 32 @e 1
	endif
endproc

proc 47
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�27��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 27
		var b 28
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 2 226006 0 300 6412 6396 0
	endif
endproc

proc 48
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�28��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 28
		var b 29
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 15 226011 0 1000 6412 6396 0
		new_tmp_item 1460015 6400 5200 0 0 0 1
	endif
endproc

proc 49
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�29��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 29
		var b 30
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 10 204035 0 600 6412 6396 0
		BCT_CALL_MON 1 204027 0 0 5400 5400 0
		BCT_CALL_MON 1 204027 0 0 7400 7400 0
		BCT_CALL_MON 1 204027 0 0 5400 7400 0
		BCT_CALL_MON 1 204027 0 0 7400 5400 0

		;NEW_MON 204027 1 5400 5400 0 0 0 1 2
		;NEW_MON 204027 1 7400 5400 0 0 0 1 2
		;NEW_MON 204027 1 7400 7400 0 0 0 1 2
		;NEW_MON 204027 1 5400 7400 0 0 0 1 2
	endif
endproc

proc 50
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�30��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 30
		var b 31
		set_pworld_var 16 32 @b 1
		

		BCT_CALL_MON 1 207205 0 300 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1095 1 7200 2
		set_pworld_var 20 32 @e 1
	endif
endproc

proc 51
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�31��
	set_sys_msg_mode 3 0 0
	map_msg_info
	
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 31
		var b 32
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 226012 0 500 6412 6396 0
	endif
endproc

proc 52
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�32��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 32
		var b 33
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 261003 0 300 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1236 1 7200 2
		BCT_CALL_MON 1 261003 0 300 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1236 1 7200 2
		new_tmp_item 1400108 5700 5200 0 0 0 1
		new_tmp_item 1400109 7100 5200 0 0 0 1
	endif
endproc

proc 53
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�33��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 33
		var b 34
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 8 200005 0 1000 6412 6396 0
		NEW_TMP_ITEM 1400007 6400 5000 0 0 0 1
		range_add_status 0 1206 1 0 1
		range_add_status 0 1207 1 0 1
	endif
endproc

proc 54
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�34��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 34
		var b 35
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 200004 0 500 6412 6396 0
		add_sys_msg_start
		add_sys_msg ��ס�����ű���ϣ��
		set_sys_msg_mode 4 0 0
		map_msg_info
	endif
endproc

proc 55
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�35��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 35
		var b 36
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 207206 0 0 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1095 1 7200 2
		set_pworld_var 20 32 @e 1
	endif
endproc

proc 56
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�36��
	set_sys_msg_mode 3 0 0
	map_msg_info
	DEL_NPC 304047 1
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 36
		var b 37
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 204004 0 0 6412 6396 e
		BCT_CALL_MON 4 204003 0 800 6412 6396 0
		TARGET_ADD_STATUS 3 @e 1236 1 7200 2
		set_pworld_var 20 32 @e 1
	endif
endproc

proc 57
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�37��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 37
		var b 38
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 2 200020 0 300 6412 6396 0
	endif
endproc

proc 58
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�38��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 38
		var b 39
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 15 272000 0 1000 6412 6396 0
		new_tmp_item 1460007 6400 5200 0 0 0 1
	endif
endproc

proc 59
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�39��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 39
		var b 40
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 10 205012 0 600 6412 6396 0
		BCT_CALL_MON 1 204027 0 0 5400 5400 0
		BCT_CALL_MON 1 204027 0 0 7400 7400 0
		BCT_CALL_MON 1 204027 0 0 5400 7400 0
		BCT_CALL_MON 1 204027 0 0 7400 5400 0

		;NEW_MON 204027 1 5400 5400 0 0 0 1 2
		;NEW_MON 204027 1 7400 5400 0 0 0 1 2
		;NEW_MON 204027 1 7400 7400 0 0 0 1 2
		;NEW_MON 204027 1 5400 7400 0 0 0 1 2
	endif
endproc

proc 60
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�40��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 40
		var b 41
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 207207 0 0 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1095 1 7200 2
		set_pworld_var 20 32 @e 1
	endif
endproc

proc 61
	
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�41��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 41
		var b 42
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 253004 0 600 6412 6396 0

	endif
endproc

proc 62
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�42��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 42
		var b 43
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 253005 0 600 6412 6396 0
	endif
endproc

proc 63
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�43��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 43
		var b 44
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 8 261009 0 500 6412 6396 0
		NEW_TMP_ITEM 1400010 6400 5000 0 0 0 1
	endif
endproc

proc 64
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�44��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 44
		var b 45
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 261010 0 500 6412 6396 0
	endif
endproc

proc 65
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�45��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 45
		var b 46
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 207208 0 300 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1095 1 7200 2
		set_pworld_var 20 32 @e 1
	endif
endproc

proc 66
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�46��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 46
		var b 47
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 204057 0 0 6412 6396 e
		BCT_CALL_MON 4 201006 0 500 6412 6396 0
		TARGET_ADD_STATUS 3 @e 1236 1 7200 2
		set_pworld_var 20 32 @e 1
	endif
endproc

proc 67
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�47��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 47
		var b 48
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 261021 0 300 6412 6396 0
		new_tmp_item 1400108 5700 5200 0 0 0 1
		new_tmp_item 1400109 7100 5200 0 0 0 1
	endif
endproc

proc 68
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�48��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 48
		var b 49
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 10 261020 0 600 6412 6396 0
		NEW_TMP_ITEM 1460016 6400 5000 0 0 0 1
		range_add_status 0 1206 1 0 1
		range_add_status 0 1207 1 0 1
	endif
endproc

proc 69
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�49��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 49
		var b 50
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 15 261022 0 600 6412 6396 0
		BCT_CALL_MON 1 204027 0 0 5400 5400 0
		BCT_CALL_MON 1 204027 0 0 7400 7400 0
		BCT_CALL_MON 1 204027 0 0 5400 7400 0
		BCT_CALL_MON 1 204027 0 0 7400 5400 0

	endif
endproc

proc 70
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�50��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 50
		var b 51
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 207209 0 300 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1095 1 7200 2
		set_pworld_var 20 32 @e 1
	endif
endproc

proc 71
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�51��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 51
		var b 52
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 205034 0 300 6412 6396 0
	endif
endproc

proc 72
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�52��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 52
		var b 53
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 284070 0 300 6412 6396 0
	endif
endproc

proc 73
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�53��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 53
		var b 54
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 261031 0 300 6412 6396 0
	endif
endproc

proc 74
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�54��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 54
		var b 55
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 8 205028 0 300 6412 6396 0
		NEW_TMP_ITEM 1420010 6400 5000 0 0 0 1
	endif
endproc

proc 75
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�55��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 55
		var b 56
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 207210 0 300 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1095 1 7200 2
		set_pworld_var 20 32 @e 1
	endif
endproc

proc 76
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�56��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 56
		var b 57
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 205027 0 0 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1236 1 7200 2
		set_pworld_var 20 32 @e 1
		BCT_CALL_MON 4 205028 0 300 6412 6396 0
	endif
endproc

proc 77
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�57��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 57
		var b 58
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 3 204070 0 300 6412 6396 0
		new_tmp_item 1400108 5700 5200 0 0 0 1
		new_tmp_item 1400109 7100 5200 0 0 0 1
	endif
endproc

proc 78
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�58��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 58
		var b 59
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 10 204075 0 600 6412 6396 0
		NEW_TMP_ITEM 1460017 6400 5000 0 0 0 1
		range_add_status 0 1206 1 0 1
		range_add_status 0 1207 1 0 1
	endif
endproc

proc 79
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�59��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 59
		var b 60
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 15 204076 0 600 6412 6396 0
		BCT_CALL_MON 1 204027 0 0 5400 5400 0
		BCT_CALL_MON 1 204027 0 0 7400 7400 0
		BCT_CALL_MON 1 204027 0 0 5400 7400 0
		BCT_CALL_MON 1 204027 0 0 7400 5400 0

	endif
endproc

proc 80
	add_sys_msg_start
	add_sys_msg ��ӭ�������֮�ĵ�60��
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 60
		var b 61
		set_pworld_var 16 32 @b 1
		BCT_CALL_MON 1 207211 0 300 6412 6396 e
		TARGET_ADD_STATUS 3 @e 1095 1 7200 2
		set_pworld_var 20 32 @e 1
	endif
endproc

proc 81
	add_sys_msg_start
	add_sys_msg ���֮�ĵ�61����δ���ţ����С��ͼ�Ա�����Ϣ�뿪
	set_sys_msg_mode 3 0 0
	map_msg_info
	;BCT_GET_CURR_LAYER 10 0
	get_pworld_var 16 32 1 b
	if b <= 61
		var b 62
		set_pworld_var 16 32 @b 1
		;BCT_CALL_MON 3 213005 0 300 6412 6396 0
	endif
endproc 

;���֮�ģ����������󣬲�������15 �� ��������5�ı����Ĳ�
proc 300
	bct_baoxiang_if_open 1
	if @1 = 1
		new_mon 293900 1 6400 6400 0 0 0 0 0
	else 
		bct_ui 0 0
	endif
endproc 