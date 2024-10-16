;70副本,70副本右通道逻辑区域5
;70副本右通道逻辑区域5
PROC 1
	;逻辑区域5标记,0未触发过,1触发过
        GET_PWORLD_VAR 72 32 1 28
        If 28 = 0
;                add_sys_msg_start
;                add_sys_msg 逻辑区域5
;                set_sys_msg_mode 3 0 0
;                map_msg_info
                var 28 1
                set_pworld_var 72 32 @28 1
        endif
ENDPROC
