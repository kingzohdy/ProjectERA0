;召集令



proc 1
;	get_player_mapinfo 0 1 2 3 4
;	if 2 = 3302
;			effect 0 0 playEffct(4942,14841,"effect\\entity\\9513_16.ent")
;			dec_item_num_inpak 5300830 1 0			
;
;			strvar Mid1 Mid2 Mid3 
;			PLAYER_CALL_BUB 213034 240 Mid1
;			PLAYER_CALL_BUB 213034 240 Mid2
;			PLAYER_CALL_BUB 213034 240 Mid3
;			
;			set_task_var 1569 0 32 @Mid1 10
;			set_task_var 1569 4 32 @Mid2 11
;			set_task_var 1569 8 32 @Mid3 12
;			
;			add_sys_msg_start
;			add_sys_msg 信徒，展现你们忠心的时刻到了！
;			set_sys_msg_mode 3 0 0
;			msg_info
;			
;	  else
;			add_sys_msg_start
;			add_sys_msg 此物品需要在阿努比斯祭坛使用！
;			set_sys_msg_mode 3 0 0
;			msg_info
;	  endif

endproc


