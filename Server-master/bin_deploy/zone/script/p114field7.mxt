;70����,70������ͨ���߼�����1
;70������ͨ���߼�����1
PROC 1
	;�߼�����1���,0δ������,1������
        GET_PWORLD_VAR 28 32 1 17
        If 17 = 0
;		add_sys_msg_start
;		add_sys_msg �߼�����1
;		set_sys_msg_mode 3 0 0
;		map_msg_info
                var 17 1
                set_pworld_var 28 32 @17 1
        endif
ENDPROC
