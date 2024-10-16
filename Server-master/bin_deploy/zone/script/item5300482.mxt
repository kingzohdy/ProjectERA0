;蛇牙项链  给BUFF 删物品 完成任务 

Proc 1
	STRVAR id x y m1
	get_task_var 1717 0 8 1 m1
;		GET_MON_INFO 1 0 0 0 0 0 x y
;		GET_PLAYER_MAPINFO 1 1 1 x y
		GET_PLAYER_ID 1 id
		
;		effect 0 0 playEffct($@x$,$@y$,"effect\\entity\\5620004_11.ent")
		
		effect_id 0 @id playMontion(%d,"9552_16")
		
		TARGET_ADD_STATUS 1 @id 1229 1 60 1
			if 1 = -1
				return
			else 
				DEC_ITEM_NUM_INPAK 5300482 1 1
				var @m1 1
				set_task_var 1717 0 8 @m1 1
				if 1 = -1
					return
				endif
				add_sys_msg_start
				add_sys_msg 能量吸收成功，赶紧以最快的速度回到拉坎身边，千万不要停！
				set_sys_msg_mode 3 0 0
				msg_info
				
			ENDIF

	;	DESTROY_MON 10
endproc			
			
                           
				