;�����ýű�
proc 1
	strvar num
	mon_get_player_num_in_rect 1 400 400 num
	
	add_sys_msg_start
	add_sys_msg �����:$@num$
	set_sys_msg_mode 4 0 0
	msg_info

	;NEW_NPC 1 284051 18716 9953 0 test.mac

	;new_mon_and_forced_move 261009 0 1 0 28749 14373 0 28635 13014

	add_sys_msg_start
	add_sys_msg ~�����:$@num$
	set_sys_msg_mode 4 0 0
	msg_info

	is_can_bianshen 0 0

	strvar x y
	GET_PLAYER_MAPINFO  1 0 0 x y

	effect 2 0 playUiEffect(0,0,2,"effect\\entity\\9576_16.ent")	

	effect 2 0 playEffct($@X$,$@Y$,"effect\\entity\\9576_18.ent")
endproc
;����ǿ���ƶ�
proc 2
	strvar x y
	GET_PLAYER_MAPINFO  1 0 0 x y
;	effect 2 0 playEffct($@x$,$@y$,"effect\\entity\\9569_16.ent")
	MON_FORCED_MOVE 1 0 @x @y 45 1000
endproc
;ɾ��
proc 3
	del_mon 1
endproc
;Ч��
proc 4
	strvar x y
	GET_PLAYER_MAPINFO  1 0 0 x y
	effect 0 0 playEffct($@x$,$@y$,"effect\\entity\\1000900_8.ent")
endproc
;����
proc 5
	strvar x y angle
	GET_PLAYER_MAPINFO  1 0 0 x y
	
	get_radian 1 @x @y 10187 19213 angle

	get_angle_pos 1 @x @y @angle 500 x y

	add_sys_msg_start
	add_sys_msg angle:$@angle$:$@x$:$@y$
	set_sys_msg_mode 4 0 0
	msg_info
	
	add angle 180
	mod angle 360
	effect 0 0 playEffct($@X$,$@Y$,$@angle$,"effect\\entity\\9570_16.ent")  

endproc
;ǿ�ƹ����ƶ��������
proc 6
	strvar x y
	GET_PLAYER_MAPINFO  1 0 0 x y
;	effect 2 0 playEffct($@x$,$@y$,"effect\\entity\\9569_16.ent")
;	var x 10187
;	var y 19213
	MON_FORCED_MOVE_pos 1 0 @x @y

endproc
;����ѹ����
proc 7
	strvar defid x y mid
	GET_MON_INFO 1 defid 0 0 0 0 x y
	NEW_MON @defid 1 @x @y 500 0 0 1 mid
	mon_push_press 1 @mid
endproc
;�����е
proc 8
	strvar x y
	GET_PLAYER_MAPINFO  1 0 0 x y
	NEW_MON_MACHINE 1 291000 0 1 @x @y 0
endproc
;��Χ����,�޲��
proc 9
	player_trigger_mon_press 1 0 1000 0 0 
endproc
;��Χ����,����Ы
proc 10
	player_trigger_mon_press 1 0 1000 233011 0 
endproc
;
proc 11
	MON_RANGE_ADD_PLAYER_HP 1 500 -100
endproc
;mon skill
proc 12
	mon_skill 10351 1
endproc
;del mon
proc 13
;	DELETE_MON_BY_DEFID 294031 1
	DELETE_MON_BY_DEFID 294032 1
;	DELETE_MON_BY_DEFID 294033 1
endproc
proc 14
	set_desig 1 125 
endproc
proc 15
	strvar px py
	GET_PLAYER_MAPINFO  1 0 0 px py
	;ɾ�컷
	;delete_mon_by_defid 284049 1

	add_sys_msg_start
	add_sys_msg �������뿪ʼ��ע�⿴�Ǹ����ڲ�ͬ�ģ�Ȼ��վ��ȥ��
	set_sys_msg_mode 3 0 0
	map_msg_info

	strvar r
	rand r 9
	strvar x
	strvar y
	strvar posX
	strvar posY
	strvar offset
	;��ɫ284050
	;��ɫ284049
	;NEW_LIFE_MON #����id #�������� #����x #����y #����뾶 #Ѳ��ID #�Ƿ�������� #���� #����ʱ�� #���ر�־ #��һֻ�����ʵ��ID
	strvar id 
	strvar t
	strvar i
	var i 0
	var t 10

	var x -1
	
	while x < 2
		var y -1
		while y < 2
			var posX @px
			var posY @py
			var offset 350
			mul offset @x
			add posX @offset
			var offset 350 
			mul offset @y
			add posY @offset

			var id 284049
			if i = @r
				var id 284050
				if i != 0
					var id 284096
					add id @i
					sub id 1
				endif
				NEW_LIFE_MON @id 1 @posX @posY 0 0 0 0 @t 1 0
			else
				if i != 0
					var id 284088
					add id @i
					sub id 1
				endif
				new_mon @id 1 @posX @posY 0 0 0 1 0
			endif	
			add i 1	
			add y 1
		endwhile
		
		add x 1
		
		;
	endwhile


endproc
proc 16
	add_sys_msg_start
	add_sys_msg ���ѽ���Σ�����ȣ���С�������ںڰ��еĵ���  
	set_sys_msg_mode 3 0 0
	map_msg_info
	
endproc

;Ч�ʲ���
proc 17
	strvar s e i
	get_time_sec s
	var i 0
	while i < 100000000
		is_same_week 0 100 1
		add i 1
	endwhile
	get_time_sec e

	sub e @s

	add_sys_msg_start
	add_sys_msg sec: $@e$  
	set_sys_msg_mode 3 0 0
	map_msg_info
	
endproc
;ɾ��Ʒ,����oplog��,DEC_ITEM_NUM_BYIDX
proc 18
	DEC_ITEM_NUM_BYIDX 5300998 1 2 1 0
endproc
;ɾ��Ʒ,����oplog��,DEC_ITEM_NUM_INPAK
proc 19
	DEC_ITEM_NUM_INPAK 5300998 1 2
endproc

proc 20
	add_sys_msg_start
	add_sys_msg Ƶ������1
	set_sys_msg_mode 1 0 0
	msg_info

		add_sys_msg_start
	add_sys_msg Ƶ������2
	set_sys_msg_mode 2 0 0
	msg_info

		add_sys_msg_start
	add_sys_msg Ƶ������3
	set_sys_msg_mode 3 0 0
	MAP_MSG_INFO
		add_sys_msg_start
	add_sys_msg Ƶ������4
	set_sys_msg_mode 4 0 0
	msg_info
		add_sys_msg_start
	add_sys_msg Ƶ������5
	set_sys_msg_mode 5 0 0
	msg_info
		add_sys_msg_start
	add_sys_msg Ƶ������6
	set_sys_msg_mode 6 0 0
	msg_info
endproc

proc 21
	dialog 3
	add_talk ss 
	NPC_COMMIT_OPT 22
	SET_DLG_COMMIT_NUM 1 10
	talk
endproc

proc 22
	PLAYER_CLOSE_DIALOG 0 0
endproc

proc 23
	;AUTO_MOVE #���ر�־ #��ͼID #����x #����y #��ʱ����ID #���� #״̬id #״̬�ȼ� #״̬����ʱ��
;	auto_move 1 1000 30000 30000 1460049 0 1095 1 3600
	auto_move 1 1000 30000 30000 1460049 0 0 0 0

;	NEW_OWNER_TMP_ITEM 1460049 9800 8800 0 0 0 184 1095 1 3600 1
;#��ƷID  #����x����  #����y���� #���� #ˢ��ʱ�� 
;
;    #����ʱ�� #Ѳ��·��id,#״̬id #״̬�ȼ� #״̬����ʱ�� #����ֵ
endproc

proc 24
;	NEW_OWNER_TMP_ITEM 1460073 26500 38700 0 0 0 313 1095 1 3600 1
	AUTO_MOVE 1 2100 3790 3377 1460073 0 1053 1 3600
endproc 

proc 25
	map_trans 1
endproc

proc 26
	;����
	add_sys_msg_start
	add_sys_msg ΰ��ġ�$_Player$��������ͨ��ʱ����п����������ͨ����ʱ��ǳ�������ս����������20��00��ʽ����  
	set_sys_msg_mode 1 0 0
	svr_msg_info
	;Ƶ��6��ʾ
	add_sys_msg_start
	add_sys_msg ��$_Player$�����ɹ�������ͨ��ʱ����п����ĵ�ͨ������á������߳ƺš�  
	set_sys_msg_mode 6 0 0
	svr_msg_info
endproc

proc 27
	set_world_city_enable_time 1 0
endproc

proc 28
	strvar open
	get_world_city_open_time 1 open

	get_now_time 0 0 0 1 0 0 0
	var 13 0
	if 1 = 5
		var 13 1
	endif

	add_sys_msg_start
	add_sys_msg $@open$kk$@13$kk$@1$
	set_sys_msg_mode 6 0 0
	msg_info
endproc

proc 29
	choose_with_timer 1 30 31 95 ѡ�����
endproc

proc 30
	add_sys_msg_start
	add_sys_msg ѡ����ȷ��
	set_sys_msg_mode 6 0 0
	msg_info
endproc

proc 31
	add_sys_msg_start
	add_sys_msg ѡ����ȡ��$_CityOwnerClanName$..
	set_sys_msg_mode 6 0 0
	msg_info
endproc

;���ƹ���,������ʱ��
proc 32
	strvar defid x y mid
	GET_MON_INFO 1 defid 0 0 0 0 x y
	NEW_LIFE_MON @defid 1 @x @y 500 0 0 0 30 1 mid
	mon_push_press 1 @mid
endproc

proc 33
	choose 1 30 31 ѡ�����
endproc

proc 34
	get_player_daily_flag 1 8 2
	
	add_sys_msg_start
	add_sys_msg $@1$
	add_sys_msg $@2$
	set_sys_msg_mode 6 0 0
	msg_info

	set_player_daily_flag 1 8 1
	add_sys_msg_start
	add_sys_msg $@1$
	set_sys_msg_mode 6 0 0
	msg_info

	get_player_daily_flag 1 0 2
	
	add_sys_msg_start
	add_sys_msg $@1$
	add_sys_msg $@2$
	set_sys_msg_mode 6 0 0
	msg_info

endproc

proc 35
	add_sys_msg_start
	add_sys_msg ����Ƶ������
	set_sys_msg_mode 14 0 0
	clan_msg_info 1 0
endproc

proc 36
;	MON_CALL_ENEMY_MON #����id #�������� #�ȼ� #�Ѷȣ�(4s 3a 2b 1c 0d)�� #����x #����y #����뾶 #Ѳ��ID #
;�Ƿ�������� #���� #����ʱ�� #���ر�־ #��һֻ�����ʵ��ID
;307012 �ɵ�
;
;�й� 284272
	strvar defid x y mid
	GET_MON_INFO 1 defid 0 0 0 0 x y

	MON_CALL_ENEMY_MON 284272 1 0 0 @x @y 0 0 0 0 0 1 0
endproc

proc 37
	open_ui 62


endproc

proc 38
	DYN_ACTIVE_STATUS_SET 0 19 1
endproc

proc 39
	DYN_ACTIVE_STATUS_SET 0 19 0
endproc

proc 40
	DYN_ACTIVE_STATUS_SET 0 19 2
endproc

proc 41
	SET_PLAYER_CURR 10566 1 
	add_sys_msg_start
	add_sys_msg $@1$
	add_sys_msg ee
	set_sys_msg_mode 6 0 0
	msg_info
endproc

proc 42
	strvar defid x y mid hp
	GET_MON_INFO 1 defid mid hp 0 0 x y
	sub hp 1
	mul hp -1
	mon_set_hp @hp @mid 1
endproc

proc 43
	task_fini 30034 1
endproc

proc 44
;	GET_PLAYER_ID 1 2
	strvar defid x y mid hp
	GET_MON_INFO 1 defid mid hp 0 0 x y
	TARGET_ADD_STATUS_dyn 10 3 @mid 3110 1 100 1
endproc

proc 45
;	GET_PLAYER_ID 1 2
	strvar defid x y mid hp
	GET_MON_INFO 1 defid mid hp 0 0 x y
	TARGET_ADD_STATUS_dyn 10 3 @mid 3110 2 100 1
endproc

proc 46

	count_down 1 0 10 ���Ե���ʱ
endproc

proc 47
	get_spy_target_clan_id 1 2
	if 1 = 0
		add_target_clan_stable 1 @2 -1 0
	endif
endproc

proc 48	
	strvar sec
	get_time_sec sec
	;timebegin11,��ʼ��ţʱ��
	var 11 0
	get_task_var 30022 4 32 1 11
	sub sec @11


	add_sys_msg_start
	add_sys_msg ��ţ��ʼʱ��$@11$,�����ѹ���$@sec$��
	set_sys_msg_mode 6 0 0
	msg_info

	if sec < 1800
		call 54
	endif
endproc


proc 49
	COW_FINAL_RANGE 1 10
endproc

proc 50
	COW_FINAL_RANGE 1 20
endproc

proc 51
	COW_FINAL_RANGE 1 30
endproc

proc 52
	COW_FINAL_BEGIN 1
endproc

proc 53
	COW_FINAL_END 1
endproc



proc 54
	mail_text #r#r#t�ף�ף����Ŷ���㱻����¼ȡ��Ŷ��ţ�к�æ�����Ŷ��ֻ��һС�����˲����ʸ�����Ŷ��ֻҪ������ܻ�ô������ҵ�Ŷ�������Ŷ���ף��ǵý���20��30��21��00���ʺ絺��ͨ���ƶ�ǰ���ǹ�������Ŷ����ʱ����Ŷ��

	sys_mail 1 5052207 1 0 0 ţ�к�æ������ѡ֪ͨ$ţ�к�æ��߻�С��
endproc

proc 55
	strvar num num2
	get_map_mon_num 2300 206900 1 num 0
	get_map_mon_num 2300 206901 1 12 0

	add_sys_msg_start
	add_sys_msg 2300,206900����$@num$,206901����$@12$
	set_sys_msg_mode 0 0 0
	msg_info

;	GET_MAP_MON_NUM #��ͼID #���ﶨ��ID #���ر�־ #�������� #�ҵ���һ�������ʵ���ɣ�
endproc

proc 56
	GET_MAP_INST 1000 0
		
endproc