;70����,70������ͨ���߼�����3
;70������ͨ���߼�����3
PROC 1
;	add_sys_msg_start
;	add_sys_msg �߼�����3,105
;	set_sys_msg_mode 3 0 0
;	map_msg_info
	;�߼�����3���,0δ������,1������
        GET_PWORLD_VAR 36 32 1 19
        If 19 = 0
;                add_sys_msg_start
;                add_sys_msg �߼�����3
;                set_sys_msg_mode 3 0 0
;                map_msg_info
                var 19 1
                set_pworld_var 36 32 @19 1
        endif
ENDPROC
