;15级副本

PROC 1
	add_sys_msg_start
	add_sys_msg 进入战鼓村副本！	
	set_sys_msg_mode 4 0 0
	map_msg_info
    
	var 10 0
	set_pworld_var 0 32 @10 1
	
	VAR 11 0 				
        SET_PWORLD_VAR 4 32 @11 1
	
	VAR 12 0 				
        SET_PWORLD_VAR 8 32 @12 1

	VAR 13 0 				
        SET_PWORLD_VAR 32 32 @13 1
	VAR 14 0 				
        SET_PWORLD_VAR 16 32 @14 1

	VAR 15 0 				
        SET_PWORLD_VAR 64 32 @15 1
	VAR 16 0 				
        SET_PWORLD_VAR 12 32 @16 1

	VAR 17 0 				
        SET_PWORLD_VAR 20 32 @17 1

	VAR 18 0 				
        SET_PWORLD_VAR 24 32 @18 1

	VAR 19 0 				
        SET_PWORLD_VAR 28 32 @19 1

	VAR 20 0 				
        SET_PWORLD_VAR 36 32 @20 1

	
	NEW_MON 201001 4 9324 15632 500 0 0 1 2
	NEW_MON 284014 1 9960 12949 0 8 0 1 2
	NEW_MON 205002 1 13666 6484 0 79 0 1 2
	new_npc 1 333021 6870 1197 0 zhu30out.mac

ENDPROC
PROC 3

	
;	add_sys_msg_start
;	add_sys_msg 15=$@15$ 16=$@16$ 10=$@10$ 18=$@18$
;	set_sys_msg_mode 1 0 0
;	map_msg_info

	


	GET_PWORLD_VAR 64 32 1 15
	GET_PWORLD_VAR 4 32 1 11
	GET_PWORLD_VAR 12 32 1 16
	GET_PWORLD_VAR 0 32 1 10
	IF 15 = 1
	add 16 1
	set_pworld_var 12 32 @16 1
	GET_PWORLD_VAR 12 32 1 16
		if 16 = 1
		var 10 0
		set_PWORLD_VAR 0 32 @10 1
		add_sys_msg_start
		add_sys_msg 你破坏了圣战鼓，准备迎接赫巴贡的愤怒吧
		set_sys_msg_mode 4 0 0
		map_msg_info
		new_tmp_item 1400012 9746 17310 0 0 0 1
		NEW_TMP_ITEM 1420001 8968 17310 0 0 0 1
		endif
		if 16 = 3
		NEW_MON 201001 2 8431 16616 200 0 0 1 2
		NEW_MON 231020 7 8431 16616 200 0 0 1 2
		NEW_MON 284013 1 8431 16616 200 0 0 1 2
		endif
		if 16 = 7
		NEW_MON 201001 3 10445 16598 200 0 0 1 2
		NEW_MON 231020 6 10445 16598 200 0 0 1 2
		NEW_MON 284013 1 10445 16598 200 0 0 1 2
		endif
		if 16 = 10
		NEW_MON 201010 1 9364 17012 0 0 0 1 21
		set_pworld_var 42 32 @21 1
		get_pworld_var 42 32 1 21
		TARGET_ADD_STATUS 3 @21 1095 1 7200 2

		add_sys_msg_start
		add_sys_msg 赫巴贡酋长出现了！
		set_sys_msg_mode 4 0 0
		map_msg_info
		VAR 15 2
       	 	SET_PWORLD_VAR 64 32 @15 1
		var 11 3
		SET_PWORLD_VAR 4 32 @11 1
		endif
	ENDIF


	GET_PWORLD_VAR 24 32 1 18
	GET_PWORLD_VAR 4 32 1 11
	GET_PWORLD_VAR 20 32 1 17
	if 18 = 10 and 11 = 3
			add 17 1 
			SET_PWORLD_VAR 20 32 @17 1
			GET_PWORLD_VAR 20 32 1 17
			if 17 = 1
			new_npc 1 333003 9317 17506 0 zhu30out.mac
			endif
			if 17 = 3
				add_sys_msg_start
				add_sys_msg 副本己完成，可通过传送门离开
				set_sys_msg_mode 4 0 0
				map_msg_info
				var 11 4
			endif

	endif
	
	SET_PWORLD_VAR 24 32 @18 1
	SET_PWORLD_VAR 4 32 @11 1

ENDPROC

PROC 4

	
	
	GET_PWORLD_VAR 0 32 1 10
	ADD 10 1
	set_pworld_var 0 32 @10 1

	VAR 19 0 				
        SET_PWORLD_VAR 28 32 @19 1

	VAR 20 0 				
        SET_PWORLD_VAR 36 32 @20 1


	GET_PWORLD_VAR 24 32 1 18
	GET_PWORLD_VAR 28 32 1 19
	GET_PWORLD_VAR 36 32 1 20
	GET_MON_INFO 0 19 0 20 0 0 0 0 
	if 19 = 201010 and 20 = 0
		var 18 10
        	SET_PWORLD_VAR 24 32 @18 1
	endif

;	GET_PWORLD_VAR 8 32 1 12
;	GET_PWORLD_VAR 16 32 1 14
;	GET_PWORLD_VAR 4 32 1 11
;	GET_MON_INFO 0 12 0 14 0 0 0 0
;	IF 12 = 284014 AND 14 = 0 AND 11 = 0
;			NEW_MON 231001 1 8528 13082 0 0 0 1 2
;			NEW_MON 231001 1 10215 12933 0 0 0 1 2
;			NEW_MON 231001 1 9260 12330 0 0 0 1 2
			

;			VAR 11 1
;			SET_PWORLD_VAR 4 32 @11 1
;			刷第四区第二波怪	
;	ENDIF
	
	

ENDPROC

proc 5

	effect 2 0 playEffct(7000,3000,"effect\\entity\\9598_16.ent")

endproc