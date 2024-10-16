;5301003


proc 1
;	strvar Num1 Num2
;	GET_ITEM_NUM_INPAK 5300862 1 Num1
;	GET_ITEM_NUM_INPAK 5300863 1 Num2
;	
;	if Num1 >= 5
;
;		if Num2 >= 1
;			strvar Ret Ret1
;			dec_item_num_inpak 5300862 5 Ret
;			dec_item_num_inpak 5300863 1 Ret1
;			if Ret = 0 and Ret1 = 0
;				add_item_num_inpak 5300864 1 1 0		
;			endif
;		else
;			add_sys_msg_start
;			add_sys_msg 你身上的提炼剂不足，无法合成！
;			set_sys_msg_mode 3 0 0
;			msg_info
;		
;		endif
;	else
;		add_sys_msg_start
;		add_sys_msg 你身上的霜磷花蕊数量不足，无法合成！
;		set_sys_msg_mode 3 0 0
;		msg_info
;	endif

endproc
