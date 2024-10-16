;牧牛预赛,预赛牧牛1
;牧牛
proc 1
	;是否提示过距离 
	var 10 0
	set_mon_var 0 32 @10 1
endproc
;死亡
proc 2
	SET_MON_TASK_STAT 30022 2 1
	add_sys_msg_start
	add_sys_msg 你的牧宠逃走了！
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
		add_sys_msg 你与牧宠的距离过远！
		set_sys_msg_mode 3 0 0
		mon_owner_msg_info
	endif
endproc
;强制移动到终点的事件 
proc 8
	strvar x y dis angle
	GET_MON_INFO 1 0 0 0 0 0 x y
	GET_POINT_DIST @x @y 12600 47000 dis

		
	if @dis <= 600
		
		
		strvar sec
		get_time_sec sec
		;timebegin11,开始赶牛时间
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

			mail_text #r#r#t亲，祝贺你哦！你被我们录取了哦！牛仔很忙活动决赛哦！只有一小部分人才有资格参与的哦！只要参与就能获得大量银币的哦！不绑的哦！亲，记得今晚20：30—21：00来彩虹岛，通过浩东前往星光大道参赛哦！不见不散哦！

			sys_mail 1 5052207 1 0 0 牛仔很忙决赛入选通知$牛仔很忙活动策划小组

			add_sys_msg_start
			add_sys_msg 本届初赛您的成绩是：$@sec$秒，恭喜您获得决赛资格，请在交任务后查看邮箱  
			set_sys_msg_mode 3 0 0
			mon_owner_msg_info
		else
			add_sys_msg_start
			add_sys_msg 本届初赛您的成绩是：$@sec$秒，很遗憾您未能晋级  
			set_sys_msg_mode 3 0 0
			mon_owner_msg_info
		endif


		SET_MON_TASK_STAT 30022 1 1
		
		DESTROY_MON 1
		return 
	endif

	var 10 0
	if @dis <= 5000
		;是否提示过距离
		get_mon_var 0 32 1 10
		if 10 = 0 
			var 10 1
			set_mon_var 0 32 @10 1
			add_sys_msg_start
			add_sys_msg 很快就到目的地了，加把劲吧
			set_sys_msg_mode 3 0 0
			mon_owner_msg_info
			
			var 10 2
		endif
	endif

	if 10 != 2
		add_sys_msg_start
		add_sys_msg 牧宠喘了口气
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
