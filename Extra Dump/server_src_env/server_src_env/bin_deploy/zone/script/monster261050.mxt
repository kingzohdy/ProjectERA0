;祭祖神殿 ,巡逻队头目2,专掉爆破机械车
;出生事件
proc 1
	;招小弟计数
	var 10 0
	set_mon_var 0 32 @10 1
endproc
proc 2
	strvar x y
	GET_MON_INFO 1 0 0 0 0 0 x y
	;临时机械,需接口
	;new_tmp_machine ::tempmachine3::
	;NEW_MON_MACHINE #返回值 #怪物机械DefID #能量 #数量 #坐标x #坐标y #实例id
	NEW_MON_MACHINE 1 291002 0 1 @x @y 0
	;机械3掉落位置x
	VAR 25 @x
	set_pworld_var 60 32 @25 1
	;机械3掉落位置y
	VAR 26 @y
	set_pworld_var 64 32 @26 1
	;机械3刷新标记,0未触发过,1触发过
	VAR 20 1
	set_pworld_var 40 32 @20 1
endproc
;定时事件
proc 4
	;招小弟计数
	get_mon_var 0 32 1 10
	if 10 > 0
		return
	endif
	
	IS_MON_FIGHTING 1 11
	if 11 = 0
		return
	endif
	
	var 10 1
	set_mon_var 0 32 @10 1
	
	get_mon_group 1 12
	strvar x y mid
	GET_MON_INFO 1 0 mid 0 0 0 x y
	new_mon 261046 1 @x @y 300 0 0 1 2
	SET_MON_GROUP 1 @2 @12
	
	LINK_MON 1 @12
	mon_speech @mid 2 2 1 发现入侵者！挡住他们！	
endproc
