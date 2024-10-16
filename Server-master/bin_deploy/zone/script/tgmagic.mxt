;魔法阵

proc 1
		dialog 1
		add_talk #b#b（你仿佛看到暗淡的气流在魔法阵中缓缓转动）.
;		task_stat 1311 0 20
;		get_task_var 1311 4 8 1 12
;		if 20 = 2 and 12 = 0
;			option 102 #召唤封印幻兽
;		endif
;		option 103 #再见
		talk	
endproc

;proc 102
;		task_stat 1311 10 11
;		if 11 = 2
;			GET_ITEM_NUM_INPAK 5300407 0 15
;			if 15 > 0 
;				
;				effect 0 2 playMontion(%d,"10053_4")
;				add_item_num_inpak 5300018 1 1 2
;				dec_item_num_inpak 5300407 1 0
;				CALL_MON 1 231022 500 60 15061 9520 20
;				
;				get_task_var 1311 1 32 1 11
;				var 11 @20
;				set_task_var 1311 1 32 @11 1
;				
;				get_task_var 1311 4 8 1 12
;				var 12 1
;				set_task_var 1311 4 8 @12 1
;			else
;				add_sys_msg_start
;				add_sys_msg 你身上没有麒麟玉！
;				set_sys_msg_mode 4 0 0
;				msg_info
;			endif
;		endif	
;endproc

proc 103
		return 
endproc
