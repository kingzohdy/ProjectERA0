;������� ,����
;����
proc 1
	strvar mid
	GET_MON_INFO 1 0 mid 0 0 0 0 0
	;�׻�����
	VAR 10 6
	set_mon_var 0 32 @10 1
	delete_mon_by_defid 201033 1
endproc
;3��һ��
proc 4
	;�׻�����
	get_mon_var 0 32 1 10
	if 10 < 1
		del_mon 1
		;delete_mon_by_defid 201033 1
		return
	endif
	sub 10 1
	set_mon_var 0 32 @10 1
	strvar x y
	GET_MON_INFO 1 0 0 0 0 0 x y
;	new_mon 201033 1 @x @y 500 0 0 1 2
endproc
