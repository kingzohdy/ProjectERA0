 

proc 3
;提交任务
			GET_PLAYER_MAPINFO 60 61 62 63 64
			
;			NEW_OWNER_TMP_ITEM 1460003 @63 @64 0 0 120 0 0 0 0 0
			APPLY_TMP_ITEM 1460003 120 0
			add_sys_msg_start
			add_sys_msg 你的终极武器巨神兵已经制作完成，快试试吧！
			set_sys_msg_mode 3 0 0
			msg_info
endproc 








