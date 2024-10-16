;捕鹿工具  对怪物使用，清怪 给BUFF 完成任务 删物品


Proc 1
	
	STRVAR id1 id2 Ret
	task_stat 1739 1 Ret
	if Ret = 2
	
		GET_TARGET_INFO 3 1 id1
		GET_PLAYER_ID 1 id2
		;GET_TARGET_INFO 1 1 id2
			DESTROY_MON 1
			if 1 = -1
				return
			else
				TARGET_ADD_STATUS 1 @id2 1219 2 600 1

				DEC_ITEM_NUM_INPAK 5300488 1 1
				
				strvar m1
				get_task_var 1739 0 8 1 m1
				var m1 1
				set_task_var 1739 0 8 @m1 1
			endif
	else
		add_sys_msg_start
		add_sys_msg 无法使用物品，请放弃任务后重新接取!
		set_sys_msg_mode 1 0 0
		msg_info
	
	endif
endproc			
			
				
				