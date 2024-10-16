;红染料

; 5301021	白丝
; 5301022	红染料
; 5301023	蓝染料
; 5301024	黄染料
; 5301025	红丝
; 5301026	蓝丝
; 5301027	黄丝
; 5301028	紫丝
; 5301029	绿丝
; 5301030	橙丝

proc 1
	strvar ret ret_1 ret_2 ret_3 ret_4
	GET_PACK_SPACE 1 ret
	GET_ITEM_NUM_INPAK 5301021 1 ret_1
	GET_ITEM_NUM_INPAK 5300162 1 ret_2
	GET_ITEM_NUM_INPAK 5301026 1 ret_3
	GET_ITEM_NUM_INPAK 5301027 1 ret_4
	
	if @ret >= 1
		if @ret_2 >= 1
			if @ret_1 >= 1
				DEC_ITEM_NUM_INPAK 5301021 1 0
				DEC_ITEM_NUM_INPAK 5300162 1 1
				if 0 = 0 and 1 = 0
					ADD_ITEM_NUM_INPAK 5301025 1 1 0
					
					add_sys_msg_start
					add_sys_msg 成功染制红丝！
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			elseif @ret_4 < 1 and @ret_3 < 1
					add_sys_msg_start
					add_sys_msg 缺少白丝材料，暂时无法染制红丝
					set_sys_msg_mode 3 0 0
					msg_info
			elseif @ret_4 >= 1
				DEC_ITEM_NUM_INPAK 5300162 1 2
				DEC_ITEM_NUM_INPAK 5301027 1 3
				if 2 = 0 and 3 = 0
					ADD_ITEM_NUM_INPAK 5301030 1 1 0
					
					add_sys_msg_start
					add_sys_msg 成功染制橙丝！
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			elseif @ret_3 < 1
					add_sys_msg_start
					add_sys_msg 缺少黄丝材料，暂时无法染制橙丝
					set_sys_msg_mode 3 0 0
					msg_info
			elseif @ret_3 >= 1
				DEC_ITEM_NUM_INPAK 5300162 1 4
				DEC_ITEM_NUM_INPAK 5301026 1 5
				if 4 = 0 and 5 = 0
					ADD_ITEM_NUM_INPAK 5301028 1 1 0
					
					add_sys_msg_start
					add_sys_msg 成功染制紫丝！
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg 缺少蓝丝材料，暂时无法染制紫丝
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 缺少红染料，暂时无法染制彩色丝线
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足！
		set_sys_msg_mode 1 0 0
		msg_info	
	endif
endproc 
