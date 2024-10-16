;;;;;圣诞魔棒



proc 1
;	GET_TARGET_INFO 1 1 20	
;	GET_ID_PLAYER_STATUS_INFO @20 1099 60 3 0 0 0
;	if 60 = 0
;		add_sys_msg_start
;		add_sys_msg 不可以对有状态的目标使用……
;		set_sys_msg_mode 3 0 0
;		msg_info
;
;	else
	;	GET_TARGET_INFO 1 1 20
	;	TARGET_ADD_STATUS 1 @20 3061 1 600 40
	;	if 40 != -1		
	;		dec_item_num_inpak 5012108 1 0
	;	endif
	
;	endif

endproc 