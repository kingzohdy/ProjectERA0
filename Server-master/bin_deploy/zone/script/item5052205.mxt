;道具-米尔斯的杀手锏
proc 1
	strvar defid mid
	get_mon_info 1 defid mid 0 0 0 0 0
	;抓螃蟹
	if @defid = 206402
		DESTROY_MON 0
		ADD_ITEM_NUM_INPAK 5052200 1 1 0
	;抓小龙虾
	elseif @defid = 206403
		DESTROY_MON 0
		ADD_ITEM_NUM_INPAK 5052201 1 1 0
	else
		return
	endif
endproc

proc 2
	strvar defid
	get_mon_info 1 defid 0 10 0 0 0 0
	if @defid != 206402 and @defid != 206403
		set_script_int_ret -1
	endif
	if @defid = 206402
		ADD_ITEM_NUM_INPAK 5052200 1 2 0
		if 0 = -1
			set_script_int_ret -1
		endif
	else
		ADD_ITEM_NUM_INPAK 5052201 1 2 0
		if 0 = -1
			set_script_int_ret -1
		endif
	endif
	if 10 = 0
		set_script_int_ret -1
	endif
endproc