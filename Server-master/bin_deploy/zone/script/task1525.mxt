
proc 1
;接受任务
			var 10 0
			set_task_var 1525 0 8 @10 1
			var 11 0
			set_task_var 1525 1 8 @11 1
			var 12 0
			set_task_var 1525 2 8 @12 1
			strvar Ret1 Ret2 Ret3
			var Ret1 0
			set_task_var 1525 4 32 @Ret1 1
			var Ret2 0
			set_task_var 1525 8 32 @Ret2 1
			var Ret3 0
			set_task_var 1525 12 32 @Ret3 1
			
endproc

proc 3
;提交任务
		GET_ITEM_NUM_INPAK 5300351 0 50
		GET_ITEM_NUM_INPAK 5300352 0 51
		GET_ITEM_NUM_INPAK 5300373 0 52
		GET_ITEM_NUM_INPAK 5300372 0 53

		dec_item_num_inpak 5300351 @50 0
		dec_item_num_inpak 5300352 @51 0
		dec_item_num_inpak 5300373 @52 0
		dec_item_num_inpak 5300372 @53 0
endproc 

proc 4
	strvar Ret1 Ret2 Ret3
	get_task_var 1525 4 32 1 Ret1
	get_task_var 1525 8 32 1 Ret2
	get_task_var 1525 12 32 1 Ret3
	DEL_ID_MON @Ret1 1
	DEL_ID_MON @Ret2 1
	DEL_ID_MON @Ret3 1

endproc 

proc 5
;放弃任务
		GET_ITEM_NUM_INPAK 5300351 0 50
		GET_ITEM_NUM_INPAK 5300352 0 51
		GET_ITEM_NUM_INPAK 5300373 0 52
		GET_ITEM_NUM_INPAK 5300372 0 53

		dec_item_num_inpak 5300351 @50 0
		dec_item_num_inpak 5300352 @51 0
		dec_item_num_inpak 5300373 @52 0
		dec_item_num_inpak 5300372 @53 0

endproc

proc 6 
;定时事件
		get_task_var 1525 0 8 1 10
		
		if 10 = 1
				get_task_var 1525 1 8 1 11
				add 11 1
				set_task_var 1525 1 8 @11 1

				if 11 = 1 
					add_sys_msg_start
					add_sys_msg 一只巨型变异蜥蜴禁不住蝎果酱的诱惑现身，在它离开之前尽快消灭它吧！
					set_sys_msg_mode 0 0 0
					msg_info
				endif
				if 11 = 2
					strvar Ret1
					CALL_MON 1 233005 500 120 4300 3100 Ret1
					set_task_var 1525 4 32 @Ret1 1
				endif
				if 11 = 16
					add_sys_msg_start
					add_sys_msg 一只巨型变异蜥蜴禁不住蝎果酱的诱惑现身，在它离开之前尽快消灭它吧！
					set_sys_msg_mode 0 0 0
					msg_info
				endif
				if 11 = 17
				
					strvar Ret2
					CALL_MON 1 233005 500 120 4300 3100 Ret2
					set_task_var 1525 8 32 @Ret2 1
				endif
				if 11 = 26
					add_sys_msg_start
					add_sys_msg 一只巨型变异蜥蜴禁不住蝎果酱的诱惑现身，在它离开之前尽快消灭它吧！
					set_sys_msg_mode 0 0 0
					msg_info
				
				endif
				if 11 = 27
				
					strvar Ret3
					CALL_MON 1 233005 500 120 4300 3100 Ret3
					set_task_var 1525 12 32 @Ret3 1
				endif
;				if 11 = 31
;					add_sys_msg_start
;					add_sys_msg 一只巨型变异蜥蜴禁不住蝎果酱的诱惑现身，在它离开之前尽快消灭它吧！
;					set_sys_msg_mode 0 0 0
;					msg_info
;				endif
;				
;				if 11 = 32 
;					CALL_MON 1 233005 500 120 4300 3100 1
;				endif 
;				if 11 = 41
;					add_sys_msg_start
;					add_sys_msg 一只巨型变异蜥蜴禁不住蝎果酱的诱惑现身，在它离开之前尽快消灭它吧！
;					set_sys_msg_mode 0 0 0
;					msg_info
;				endif
				if 11 = 42
;					CALL_MON 1 233005 500 120 4300 3100 1
					
					get_task_var 1525 0 8 1 10
					var 10 0
					set_task_var 1525 0 8 @10 1
				endif

		endif
endproc









