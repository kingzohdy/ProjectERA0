;祭祖神殿 ,先遣队1
;死亡事件
proc 2
	strvar x y
	GET_MON_INFO 1 0 0 0 0 0 x y
	;临时机械,需接口
	;new_tmp_machine ::tempmachine1::
	NEW_MON_MACHINE 1 291000 0 1 @x @y 0
	;机械1掉落位置x
	VAR 21 @x
	set_pworld_var 44 32 @21 1
	;机械1掉落位置y
	VAR 22 @y
	set_pworld_var 48 32 @22 1
	;机械1刷新标记,0未触发过,1触发过
	VAR 18 1
	set_pworld_var 32 32 @18 1
endproc
