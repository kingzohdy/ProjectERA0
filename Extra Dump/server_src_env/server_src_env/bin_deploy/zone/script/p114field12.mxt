;70����,70������ͨ���߼�����6
;70������ͨ���߼�����6
PROC 1
	;�߼�����6���,0δ������,1������
        GET_PWORLD_VAR 76 32 1 29
        If 29 = 0
;                add_sys_msg_start
;                add_sys_msg �߼�����6
;                set_sys_msg_mode 3 0 0
;                map_msg_info
                var 29 1
                set_pworld_var 76 32 @29 1
        endif
ENDPROC
