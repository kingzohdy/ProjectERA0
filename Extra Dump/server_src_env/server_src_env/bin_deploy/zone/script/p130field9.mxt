;70副本,70副本右通道逻辑区域3
;70副本右通道逻辑区域3
PROC 1
;	add_sys_msg_start
;	add_sys_msg 逻辑区域3,105
;	set_sys_msg_mode 3 0 0
;	map_msg_info
	;逻辑区域3标记,0未触发过,1触发过
        GET_PWORLD_VAR 36 32 1 19
        If 19 = 0
;                add_sys_msg_start
;                add_sys_msg 逻辑区域3
;                set_sys_msg_mode 3 0 0
;                map_msg_info
                var 19 1
                set_pworld_var 36 32 @19 1
        endif
ENDPROC
