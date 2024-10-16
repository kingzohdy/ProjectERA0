;银币副本：文字密码

;创建副本事件
PROC 1
	; if or not start var
	strvar a
	var a 0
	set_pworld_var 0 32 @a 1
	
	; current kill monster number
	strvar f
	var f 0
	set_pworld_var 20 32 @f 1
	
	; current round, start from 0
	strvar g
	var g 0
	set_pworld_var 24 32 @g 1
	
	; current round left time
	strvar l
	var l 0
	set_pworld_var 44 32 @l 1
	
	; correct number
	strvar m
	var m 0
	set_pworld_var 48 32 @m 1
	
	; new npc
	new_npc 1 302129 7700 8300 180 npc302129.mac
	
	; clean mon flag
	strvar n
	var n 0
	set_pworld_var 52 32 @n 1
	
	; get award flag
	strvar p
	var p 0
	set_pworld_var 56 32 @p 1
	
	;csm
	new_npc 1 333165 7900 4200 180 csm333165.mac
ENDPROC

;关闭副本事件
proc 2
endproc

;副本定时事件（每2秒触发一次）
proc 3
	strvar a f g l m n p
	get_pworld_var 0 32 1 a
	if @a = 1
		get_pworld_var 24 32 1 g
		if @g = 10
			;发送消息给客户端停止倒数
			strvar id
			get_pworld_first_player_id 1 id
			word_pwd_ui @id 2 0 0 1
		
			;停止答题
			get_pworld_var 0 32 1 a
			var a 0
			set_pworld_var 0 32 @a 1
			
			;npc再次出现
			get_pworld_var 56 32 1 p
			if p = 0
				new_npc 1 302129 7700 8300 180 npc302129.mac
			endif
			
			return 
		endif
		
		del_npc 302129 1
		
		; pworld start
		get_pworld_var 20 32 1 f
		get_pworld_var 44 32 1 l
		
		if @l = 0
			var n 0
			set_pworld_var 52 32 @n 1
		endif
		
		;开始准备和开始答题提示
		if @l = 0 or @l = 10
			call 300
		endif
		
		; ready left 10 second
		if @l = 0
			add_sys_msg_start
			add_sys_msg 10秒后，场中将刷新词语，请您按照词语顺序击杀
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		
		add @l 2
		set_pworld_var 44 32 @l 1
		
		; 刷怪
		if @l = 12
			word_pwd_rand_new_mon 200203 4 8000 8600 7400 8800 7400 8000 8000 8000 400 1
		endif
		
		; time out or killed monster number = 4
		if @f = 4
			add_sys_msg_start
			add_sys_msg 恭喜你答对了，稍后你将获取答题奖励
			set_sys_msg_mode 4 0 0
			map_msg_info
			
			;current round++
			get_pworld_var 24 32 1 g
			add @g 1
			set_pworld_var 24 32 @g 1
			
			;reset time
			var l 0
			set_pworld_var 44 32 @l 1
			
			;correct number++
			get_pworld_var 48 32 1 m
			add @m 1
			set_pworld_var 48 32 @m 1
			
			; reset kill monster number
			var f 0
			set_pworld_var 20 32 @f 1
			
			;clean mon 
			destroy_mon_by_defid 1 200204
		endif
		
		; time out
		if @l = 50 and @f != 4
			add_sys_msg_start
			add_sys_msg 很遗憾这道题时间到了
			set_sys_msg_mode 4 0 0
			map_msg_info
		
			;current round++
			get_pworld_var 24 32 1 g
			add @g 1
			set_pworld_var 24 32 @g 1
			
			;reset time
			var l 0
			set_pworld_var 44 32 @l 1
			
			; reset kill monster number
			var f 0
			set_pworld_var 20 32 @f 1
			
			;clean mon
			var n 1
			set_pworld_var 52 32 @n 1
			destroy_mon_by_defid 1 200204
			destroy_mon_by_defid 1 200203
		endif
	endif	
endproc

;副本中怪物死亡事件
proc 4
	
	
endproc

;副本中角色进入事件
proc 5

endproc

;副本中角色死亡事件
proc 6

endproc

;副本中倒计时结束事件
proc 7
	
endproc 

;副本活动开始事件
proc 8
endproc 

; show ui
proc 300
	strvar round time
	strvar curround curtime
	strvar id
	
	get_pworld_first_player_id 1 id
	get_pworld_var 24 32 1 curround
	get_pworld_var 44 32 1 curtime
	
	var time 50
	sub @time @curtime
	add @curround 1
	
	if @curround <= 10
		if @time > 40
			word_pwd_ui @id 0 @curround @time 1
		else
			word_pwd_ui @id 1 @curround 39 1
		endif
	endif
	
endproc 