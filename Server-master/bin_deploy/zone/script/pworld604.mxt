;ĩ���۷帱��

;���������¼�
PROC 1
	;���ֻ�������¼�
	set_pworld_var 0 32 0 0
	;1���ӹ���
	set_pworld_var 4 32 0 0
	;���ӹ�����
	set_pworld_var 8 32 0 0
	;������һ�������˻�
	set_pworld_var 12 32 0 0 
	;�ӹ����㣬��ǰ�����ĸ��׶Σ�������µ�¼��ʱ�򣬸��ӹ�����
	set_pworld_var 16 32 0 0 
	;�������
	set_pworld_var 20 32 0 0
ENDPROC

;�رո����¼�
proc 2
endproc

;������ʱ�¼���ÿ2�봥��һ�Σ�
proc 3
	IS_PWORLD_OVER 30
	if @30 != 1
		get_pworld_var 4 32 1 11
		if @11 >= 30
			strvar flag rVar rNum
			var flag 0
			get_pworld_var 0 32 1 15
			;1���Ӻ�ÿ16s �������������ʾ��ÿ26s�����������
			if @15 = 8
				RAND rVar 100
				;75%�ļ���
				if @rVar > 24
					set_pworld_var 20 32 1 0
					RAND rNum 12
					set_pworld_var 12 32 @rNum 0
					call 100
				endif
				set_pworld_var 0 32 9 0
			elseif  @15 = 14
				get_pworld_var 20 32 1 flag
				if @flag = 1
					call 101
				endif
				set_pworld_var 20 32 0 0
				set_pworld_var 0 32 0 0
			else
				add 15 1
				set_pworld_var 0 32 @15 0
			endif
			
			;1���Ӻ�  30s����
			get_pworld_var 8 32 1 12
			if @12 = 15
				;��������5000��
				call 110
				set_pworld_var 16 32 1 0 
				set_pworld_var 8 32 16 0 
			elseif @12 = 31
				;��������15000��
				call 111
				set_pworld_var 16 32 2 0 
				set_pworld_var 8 32 32 0 
			elseif @12 = 46
				;��������30000��
				call 112
				set_pworld_var 16 32 3 0 
				set_pworld_var 8 32 47 0 
			elseif @12 = 61
				;��������60000��
				call 113
				set_pworld_var 16 32 4 0 
				set_pworld_var 8 32 62 0 
			elseif @12 = 76
				set_pworld_var 16 32 0 0
			else
				add 12 1
				set_pworld_var 8 32 @12 0
			endif
		else
			add 11 1
			set_pworld_var 4 32 @11 1
		endif
	endif	
endproc

;�����н�ɫ�����¼�
proc 5	
	strvar memvar
	get_player_id 10 memvar
	if 10 = -1
		return 
	endif	
	
	get_pworld_var 16 32 1 90
	if @90 = 1
		TARGET_ADD_STATUS  1 @memvar 1306 1 60 11
	elseif @90 = 2
		TARGET_ADD_STATUS  1 @memvar 1306 2 60 11
	elseif @90 = 3
		TARGET_ADD_STATUS  1 @memvar 1306 3 60 11
	elseif @90 = 4
		TARGET_ADD_STATUS  1 @memvar 1306 4 60 11
	else
	endif
endproc

;��ʾ
proc 100
	strvar rVar 
	get_pworld_var 12 32 1 rVar
	if @rVar < 6
		add_sys_msg_start
		add_sys_msg ս��ף������10S��������ɳ���ϣ�����������׼����
		set_sys_msg_mode 4 0 0
		MAP_MSG_INFO
	else 
		add_sys_msg_start
		add_sys_msg ĩ�����伴�����٣������ö��׼����
		set_sys_msg_mode 4 0 0
		MAP_MSG_INFO
	endif
endproc

;�м��������˻�����������¼�
proc 101
	strvar rNum PosX PosY
	strvar RandPos
	get_pworld_var 12 32 1 rNum
	
	RAND RandPos 4
	if @RandPos = 0
		var PosX 7000
		var PosY 6500
	elseif @RandPos = 1
		var PosX 7000
		var PosY 5700
	elseif @RandPos = 2
		var PosX 6200
		var PosY 5700
	else
		var PosX 6200
		var PosY 6500
	endif
	
	if @rNum = 0
		NEW_LIFE_MON 261401 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 1
		NEW_LIFE_MON 261402 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 2
		NEW_LIFE_MON 261403 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 3
		NEW_LIFE_MON 261404 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 4
		NEW_LIFE_MON 261405 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 5
		NEW_LIFE_MON 261406 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 6
		NEW_LIFE_MON 261407 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 7
		NEW_LIFE_MON 261408 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 8
		NEW_LIFE_MON 261409 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 9
		NEW_LIFE_MON 261410 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 10
		NEW_LIFE_MON 261411 1 @PosX @PosY 800 0 0 0 10 0 0
	elseif @rNum = 11
		NEW_LIFE_MON 261412 1 @PosX @PosY 800 0 0 0 10 0 0
	else
	endif
endproc

;���ӹ�����
proc 110
	add_sys_msg_start
	add_sys_msg �ڳ�ʱ���ս���У�ȫ��ս��ϸ���õ�����������С��������
	set_sys_msg_mode 1 0 0
	MAP_MSG_INFO

	;���۷帱�����������˼�״̬
	DIANFENG_ADD_STATUS 1306 1 60 90
endproc

proc 111
	add_sys_msg_start
	add_sys_msg �ڳ�ʱ���ս���У�ȫ��ս��ϸ���õ����������ýϸߵ�������
	set_sys_msg_mode 1 0 0
	MAP_MSG_INFO
	
	DIANFENG_ADD_STATUS 1306 2 60 90
endproc

proc 112
	add_sys_msg_start
	add_sys_msg �ڳ�ʱ���ս���У�ȫ��ս��ϸ���õ����������ô��������
	set_sys_msg_mode 1 0 0
	MAP_MSG_INFO
	
	DIANFENG_ADD_STATUS 1306 3 60 90
endproc

proc 113
	add_sys_msg_start
	add_sys_msg �ڳ�ʱ���ս���У�ȫ��ս��ϸ���õ����������óɱ���������
	set_sys_msg_mode 1 0 0
	MAP_MSG_INFO
	
	DIANFENG_ADD_STATUS 1306 4 300 90
endproc
