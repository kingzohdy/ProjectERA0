




proc 3
	
	get_map_val 0 8 1 10
	
	add 10 1
	set_map_val 0 8 @10 0
;	add_sys_msg_start
;		add_sys_msg $@10$--------------------------
;		set_sys_msg_mode 1 0 0
;		map_msg_info
	
	if 10 = 2
		call 100
		
		var 10 0
		set_map_val 0 8 @10 0
	endif
endproc 

proc 100
	effect 0 0 playEffct(21792,7732,90,"effect\\entity\\9570_16.ent")
	effect 0 0 playEffct(21599,12677,70,"effect\\entity\\9570_16.ent")
	effect 0 0 playEffct(19459,12280,90,"effect\\entity\\9570_16.ent")
	effect 0 0 playEffct(19609,16571,90,"effect\\entity\\9570_16.ent")
	effect 0 0 playEffct(19639,20281,90,"effect\\entity\\9570_16.ent")
	effect 0 0 playEffct(18327,24260,45,"effect\\entity\\9570_16.ent")
	effect 0 0 playEffct(21031,24260,135,"effect\\entity\\9570_16.ent")
	effect 0 0 playEffct(22924,26117,100,"effect\\entity\\9570_16.ent")
	effect 0 0 playEffct(16508,26117,80,"effect\\entity\\9570_16.ent")
	
endproc 