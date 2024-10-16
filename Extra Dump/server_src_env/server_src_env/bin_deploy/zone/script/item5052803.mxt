;宝石包
proc 1
;	strvar Num
;	GET_PACK_SPACE 0 Num
;	if Num < 3
;		add_sys_msg_start
;		add_sys_msg 背包空间不足,请预留3个以上空格再开启！
;		set_sys_msg_mode 1 0 0
;		msg_info
;		return
;	else
;		strvar Rad1 Rad2 Rad3
;		var Rad1 0
;		var Rad2 3
;
;		while @Rad1 < @Rad2
;		add Rad1 1
;			rand Rad3 10
;			switch Rad3
;				case 0
;					ADD_ITEM_NUM_INPAK 5200000 1 0 0
;				endcase
;				case 1
;					ADD_ITEM_NUM_INPAK 5200000 1 0 0
;				endcase
;				case 2
;					ADD_ITEM_NUM_INPAK 5200010 1 0 0
;				endcase
;				case 3
;					ADD_ITEM_NUM_INPAK 5200010 1 0 0
;				
;				endcase
;				case 4
;					ADD_ITEM_NUM_INPAK 5200010 1 0 0
;				
;				endcase
;				case 5
;					ADD_ITEM_NUM_INPAK 5200010 1 0 0
;				
;				endcase
;				case 6
;					ADD_ITEM_NUM_INPAK 5200010 1 0 0
;				
;				endcase
;				case 7
;					ADD_ITEM_NUM_INPAK 5200010 1 0 0
;				
;				endcase
;				case 8
;					ADD_ITEM_NUM_INPAK 5200020 1 0 0
;				
;				endcase
;				case 9
;					ADD_ITEM_NUM_INPAK 5200020 1 0 0
;				
;				endcase
;
;			endswitch
;		endwhile
;		DEC_ITEM_NUM_INPAK 5052803 1 1
;	endif
;
endproc
