;°×µ° Áúµ°¶¨Ê±
proc 1
	get_pworld_var 8 32 1 20
    var 10 0
	set_mon_var 0 32 @10 1
endproc
proc 4
	
	get_mon_var 0 32 1 10
	add 10 1
	set_mon_var 0 32 @10 1
	
	;Áúµ°ÆÆÁÑ£¬²úÉúÑ¸ÃÍÁú
	if 10 = 30
	DESTROY_MON 1
	get_mon_info 1 283000 0 0 0 0 100 101
	new_mon 283010 1 @100 @101 5 0 0 0 1
	endif
	
	
endproc

;Áúµ°ËÀÍö
;proc 2
;	get_pworld_var 8 32 1 20
;	var 20 1
;	set_pworld_var 8 32 @20 1
;endproc

