;70副本,70副本右通道逻辑区域2
;70副本右通道逻辑区域2
PROC 1
	;逻辑区域2标记,0未触发过,1触发过
        GET_PWORLD_VAR 32 32 1 18
        If 18 = 0
;                add_sys_msg_start
;                add_sys_msg 逻辑区域2
;                set_sys_msg_mode 3 0 0
;                map_msg_info
                var 18 1
                set_pworld_var 32 32 @18 1
        endif
ENDPROC
