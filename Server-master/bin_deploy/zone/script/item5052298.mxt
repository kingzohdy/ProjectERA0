; 春节·宝石豪礼

proc 1
	strvar num ret zhi rad gei kou job
	GET_PACK_SPACE 0 num
	get_role_var 63 8 ret zhi
	
	if num < 1
		add_sys_msg_start
		add_sys_msg 背包需要1个格子才装得下哦！请先整理背包再开启
		set_sys_msg_mode 1 0 0
		msg_info
	elseif ret = 0 and zhi < 5
		DEC_ITEM_NUM_INPAK 5052298 1 kou
			
		if kou = 0	
			GET_PLAYER_BASE_ATT 0 0 0 job
			rand rad 10
				if rad = 0
					if job = 1 or job = 3 or job = 17 or job = 19 or job = 33 or job = 35
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5210003 1 1 gei
						if gei = 0
							add zhi 1
							set_role_var 63 8 @zhi ret
						endif
					elseif job = 2 or job = 4 or job = 18 or job = 20 or job = 34 or job = 36
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5210103 1 1 gei
						if gei = 0
							add zhi 1
							set_role_var 63 8 @zhi ret
						endif
					else
					endif
				endif
				if rad = 1
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5211003 1 1 gei
					if gei = 0
						add zhi 1
						set_role_var 63 8 @zhi ret
					endif
				endif
				if rad = 2
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5211103 1 1 gei
					if gei = 0
						add zhi 1
						set_role_var 63 8 @zhi ret
					endif
				endif
				if rad = 3
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5211203 1 1 gei
					if gei = 0
						add zhi 1
						set_role_var 63 8 @zhi ret
					endif
				endif
				if rad = 4
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5212003 1 1 gei
					if gei = 0
						add zhi 1
						set_role_var 63 8 @zhi ret
					endif
				endif
				if rad = 5
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5212103 1 1 gei
					if gei = 0
						add zhi 1
						set_role_var 63 8 @zhi ret
					endif
				endif
				if rad = 6
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5212203 1 1 gei
					if gei = 0
						add zhi 1
						set_role_var 63 8 @zhi ret
					endif
				endif
				if rad = 7
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5212403 1 1 gei
					if gei = 0
						add zhi 1
						set_role_var 63 8 @zhi ret
					endif
				endif
				if rad = 8
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213303 1 1 gei
					if gei = 0
						add zhi 1
						set_role_var 63 8 @zhi ret
					endif
				endif
				if rad = 9
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213403 1 1 gei
					if gei = 0
						add zhi 1
						set_role_var 63 8 @zhi ret
					endif
				endif
		endif
	elseif ret = 0 and zhi > 4
		DEC_ITEM_NUM_INPAK 5052298 1 kou
			
		if kou = 0
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012114 10 1 0
		endif
	else
	endif
endproc
