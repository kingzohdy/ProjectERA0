;70����,70������ͨ���߼�����5
;70������ͨ���߼�����5
PROC 1
	;�߼�����5���,0δ������,1������
        GET_PWORLD_VAR 72 32 1 28
        If 28 = 0
;                add_sys_msg_start
;                add_sys_msg �߼�����5
;                set_sys_msg_mode 3 0 0
;                map_msg_info
                var 28 1
                set_pworld_var 72 32 @28 1
        endif
ENDPROC
