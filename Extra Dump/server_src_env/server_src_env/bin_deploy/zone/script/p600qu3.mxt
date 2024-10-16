;工厂区域提示


proc 1
;	strvar Ret
;	GET_CLAN_BUILDING_LV Ret 0 2
;	if Ret = 0
;		add_sys_msg_start
;		add_sys_msg 以下区域暂未开放，敬请期待！
;		set_sys_msg_mode 1 0 0
;		msg_info
;	endif
endproc
