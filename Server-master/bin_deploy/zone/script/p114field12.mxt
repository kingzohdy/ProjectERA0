;70副本,70副本右通道逻辑区域6
;70副本右通道逻辑区域6
PROC 1
	;逻辑区域6标记,0未触发过,1触发过
        GET_PWORLD_VAR 76 32 1 29
        If 29 = 0
;                add_sys_msg_start
;                add_sys_msg 逻辑区域6
;                set_sys_msg_mode 3 0 0
;                map_msg_info
                var 29 1
                set_pworld_var 76 32 @29 1
        endif
ENDPROC
