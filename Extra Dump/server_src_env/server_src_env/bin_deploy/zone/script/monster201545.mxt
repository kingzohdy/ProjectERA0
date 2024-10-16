;祭祖神殿 ,先遣队2
;死亡事件
proc 2
	strvar x y
	GET_MON_INFO 1 0 0 0 0 0 x y
	;临时机械,需接口
	;new_tmp_machine ::tempmachine2::
	NEW_MON_MACHINE 1 291001 0 1 @x @y 0
	;机械2掉落位置x
	VAR 23 @x
	set_pworld_var 52 32 @23 1
	;机械2掉落位置y
	VAR 24 @y
	set_pworld_var 56 32 @24 1
	;机械2刷新标记,0未触发过,1触发过
	VAR 19 1
	set_pworld_var 36 32 @19 1
endproc
