;70副本,70副本右通道逻辑区域1
;70副本右通道逻辑区域1
PROC 1
	;逻辑区域1标记,0未触发过,1触发过
        GET_PWORLD_VAR 28 32 1 17
        If 17 = 0
;		add_sys_msg_start
;		add_sys_msg 逻辑区域1
;		set_sys_msg_mode 3 0 0
;		map_msg_info
                var 17 1
                set_pworld_var 28 32 @17 1
        endif
ENDPROC
