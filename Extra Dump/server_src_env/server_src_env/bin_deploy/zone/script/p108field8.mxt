;70����,70������ͨ���߼�����2
;70������ͨ���߼�����2
PROC 1
	;�߼�����2���,0δ������,1������
        GET_PWORLD_VAR 32 32 1 18
        If 18 = 0
;                add_sys_msg_start
;                add_sys_msg �߼�����2
;                set_sys_msg_mode 3 0 0
;                map_msg_info
                var 18 1
                set_pworld_var 32 32 @18 1
        endif
ENDPROC
