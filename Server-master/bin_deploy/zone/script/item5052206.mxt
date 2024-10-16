;²¶Ð·¹¤¾ß
proc 1
	DESTROY_MON 0
	ADD_ITEM_NUM_INPAK 5052200 1 1 0
endproc

proc 2
	get_mon_info 1 0 0 10 0 0 0 0
	ADD_ITEM_NUM_INPAK 5052200 1 2 0
	if 0 = -1
		set_script_int_ret -1
	endif
	if 10 = 0
		set_script_int_ret -1
	endif
endproc
