;��ţԤ��,Ԥ����ţ1
;��ţ
proc 1
	;�Ƿ���ʾ������ 
	var 10 0
	set_mon_var 0 32 @10 1
endproc
;����
proc 2
	SET_MON_TASK_STAT 30022 2 1
	add_sys_msg_start
	add_sys_msg ������������ˣ�
	set_sys_msg_mode 3 0 0
	mon_owner_msg_info
endproc
;tick
proc 4
	MON_TASK_AREA_CHECK 30022 3000 300 1
	if 1 = 0
	del_mon 1
	return
	endif
	strvar dis
	mon_owner_dis 1 dis
	if dis >= 2000
		add_sys_msg_start
		add_sys_msg ��������ľ����Զ��
		set_sys_msg_mode 3 0 0
		mon_owner_msg_info
	endif
endproc
;ǿ���ƶ����յ���¼� 
proc 8
	strvar x y dis angle
	GET_MON_INFO 1 0 0 0 0 0 x y
	GET_POINT_DIST @x @y 12600 47000 dis

		
	if @dis <= 600
		
		
		strvar sec
		get_time_sec sec
		;timebegin11,��ʼ��ţʱ��
		var 11 0
		get_task_var 30022 4 32 1 11
;		add_sys_msg_start
;		add_sys_msg $@11$..$@sec$ 
;		set_sys_msg_mode 0 0 0
;		map_msg_info

		sub sec @11
;		add_sys_msg_start
;		add_sys_msg $@11$..$@sec$ 
;		set_sys_msg_mode 0 0 0
;		map_msg_info
		if sec < 240
;			add_sys_msg_start
;			add_sys_msg $@11$..$@sec$ 
;			set_sys_msg_mode 0 0 0
;			map_msg_info
;			add_item_num_inpak 5052207 1 0 10

			mail_text #r#r#t�ף�ף����Ŷ���㱻����¼ȡ��Ŷ��ţ�к�æ�����Ŷ��ֻ��һС�����˲����ʸ�����Ŷ��ֻҪ������ܻ�ô������ҵ�Ŷ�������Ŷ���ף��ǵý���20��30��21��00���ʺ絺��ͨ���ƶ�ǰ���ǹ�������Ŷ��������ɢŶ��

			sys_mail 1 5052207 1 0 0 ţ�к�æ������ѡ֪ͨ$ţ�к�æ��߻�С��

			add_sys_msg_start
			add_sys_msg ����������ĳɼ��ǣ�$@sec$�룬��ϲ����þ����ʸ����ڽ������鿴����  
			set_sys_msg_mode 3 0 0
			mon_owner_msg_info
		else
			add_sys_msg_start
			add_sys_msg ����������ĳɼ��ǣ�$@sec$�룬���ź���δ�ܽ���  
			set_sys_msg_mode 3 0 0
			mon_owner_msg_info
		endif


		SET_MON_TASK_STAT 30022 1 1
		
		DESTROY_MON 1
		return 
	endif

	var 10 0
	if @dis <= 5000
		;�Ƿ���ʾ������
		get_mon_var 0 32 1 10
		if 10 = 0 
			var 10 1
			set_mon_var 0 32 @10 1
			add_sys_msg_start
			add_sys_msg �ܿ�͵�Ŀ�ĵ��ˣ��ӰѾ���
			set_sys_msg_mode 3 0 0
			mon_owner_msg_info
			
			var 10 2
		endif
	endif

	if 10 != 2
		add_sys_msg_start
		add_sys_msg ���购�˿���
		set_sys_msg_mode 3 0 0
		mon_owner_msg_info
	endif

	get_radian 1 @x @y 12600 47000 angle
	get_angle_pos 1 @x @y @angle 500 x y

	add angle 180
	if angle > 360
		sub angle 360
	endif

;	effect 3 0 playEffct($@x$,$@y$,$@angle$,"effect\\entity\\9570_16.ent")
endproc
