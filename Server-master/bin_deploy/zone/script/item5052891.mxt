



proc 1
	strvar Rac Ret Num
	GET_PACK_SPACE 0 Num
	if Num < 2
		add_sys_msg_start
		add_sys_msg 宝箱会随机开出物品，请预留两个背包空格再开启！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
;	strvar Dret num1 num2 Ret1
;	GET_ITEM_NUM_INPAK 5103001 1 num1
;	GET_ITEM_NUM_INPAK 5101001 1 num2
;	if num1 < 5
;		add_sys_msg_start
;		add_sys_msg 秘境精华数量不足
;		set_sys_msg_mode 4 0 0
;		msg_info
;		return
;	endif
	strvar Dret rad lv
	get_player_id 1 20
	ID_PLAYER_BASE_ATT @20 1 1 lv 1 
	;30~50级

	if lv >= 30 and lv <= 50
		dec_item_num_inpak 5052891 1 Dret
		if Dret = 0
			var 40 0
			while 40 < 2
			add 40 1
		
				rand rad 100
				if rad < 16
					ADD_BIND_ITEM_NUM_INPAK 5109501 5 1 0
				endif
				if rad >= 16 and rad < 32
					ADD_BIND_ITEM_NUM_INPAK 5109502 5 1 0
				endif
				if rad >= 32 and rad < 48
					ADD_BIND_ITEM_NUM_INPAK 5101001 10 1 0
				endif
				if rad >= 48 and rad < 64
					ADD_BIND_ITEM_NUM_INPAK 5102001 10 1 0
				endif
				if rad >= 64 and rad < 80
					ADD_BIND_ITEM_NUM_INPAK 5103001 5 1 0
				endif
				if rad >= 80 and rad < 96
					ADD_BIND_ITEM_NUM_INPAK 5103041 2 1 0
				endif
				if rad >= 96 and rad < 98
					ADD_BIND_ITEM_NUM_INPAK 5109201 1 1 0
				endif
				if rad >= 98 and rad < 100
					ADD_BIND_ITEM_NUM_INPAK 5103021 1 1 0
				endif
			endwhile
		endif
	endif		
	if lv > 50 and lv <= 70
		dec_item_num_inpak 5052891 1 Dret
		if Dret = 0		
			var 40 0
			while 40 < 2
			add 40 1

				rand rad 100
				if rad < 16
					ADD_BIND_ITEM_NUM_INPAK 5109503 5 1 0
				endif
				if rad >= 16 and rad < 32
					ADD_BIND_ITEM_NUM_INPAK 5109504 5 1 0
				endif
				if rad >= 32 and rad < 48
					ADD_BIND_ITEM_NUM_INPAK 5101002 10 1 0
				endif
				if rad >= 48 and rad < 64
					ADD_BIND_ITEM_NUM_INPAK 5102002 10 1 0
				endif
				if rad >= 64 and rad < 80
					ADD_BIND_ITEM_NUM_INPAK 5103002 5 1 0
				endif
				if rad >= 80 and rad < 96
					ADD_BIND_ITEM_NUM_INPAK 5103042 2 1 0
				endif
				if rad >= 96 and rad < 98
					ADD_BIND_ITEM_NUM_INPAK 5109202 1 1 0
				endif
				if rad >= 98 and rad < 100
					ADD_BIND_ITEM_NUM_INPAK 5103022 1 1 0
				endif
			endwhile
		endif
	endif
	if lv >= 71 and lv <= 120
		dec_item_num_inpak 5052891 1 Dret
		if Dret = 0
			var 40 0
			while 40 < 2
			add 40 1
				rand rad 100
				if rad < 32
					ADD_BIND_ITEM_NUM_INPAK 5109505 5 1 0
				endif
			;	if rad >= 16 and rad < 32
			;		ADD_ITEM_NUM_INPAK 5109506 5 1 0
			;	endif
				if rad >= 32 and rad < 48
					ADD_BIND_ITEM_NUM_INPAK 5101003 10 1 0
				endif
				if rad >= 48 and rad < 64
					ADD_BIND_ITEM_NUM_INPAK 5102003 10 1 0
				endif
				if rad >= 64 and rad < 80
					ADD_BIND_ITEM_NUM_INPAK 5103003 5 1 0
				endif
				if rad >= 80 and rad < 96
					ADD_BIND_ITEM_NUM_INPAK 5103043 2 1 0
				endif
				if rad >= 96 and rad < 98
					ADD_BIND_ITEM_NUM_INPAK 5109203 1 1 0
				endif
				if rad >= 98 and rad < 100
					ADD_BIND_ITEM_NUM_INPAK 5103023 1 1 0
				endif
			endwhile
		endif
	endif	
endproc 

proc 2
	strvar Rac Ret Num
	GET_PACK_SPACE 0 Num
	if Num < 2
		add_sys_msg_start
		add_sys_msg 宝箱会随机开出物品，请预留两个背包空格再开启！
		set_sys_msg_mode 4 0 0
		msg_info
		
		SET_SCRIPT_INT_RET -1
		return
	endif
endproc 









