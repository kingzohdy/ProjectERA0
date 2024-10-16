;试炼人偶

proc 2
	get_pworld_var 5 8 1 32
	add 32 1
	set_pworld_var 5 8 @32 1
	;总数：死亡次数、跑掉个数	
	
;	get_pworld_var 6 8 1 33
;	add 32 1
;	set_pworld_var 6 8 @23 1
	;死亡次数

endproc 

proc 3
;路径结束
	get_pworld_var 5 8 1 32
	add 32 1
	set_pworld_var 5 8 @32 1
	;总数：死亡次数、跑掉个数	
	get_pworld_var 7 8 1 34
	add 34 1
	set_pworld_var 7 8 @34 1
	;跑掉人偶个数
	
	DESTROY_MON 1

endproc 



