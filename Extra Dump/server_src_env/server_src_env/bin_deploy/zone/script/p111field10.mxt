;70����,70������ͨ���߼�����4
;70������ͨ���߼�����4
PROC 1
;	add_sys_msg_start
;	add_sys_msg �߼�����4,105
;	set_sys_msg_mode 3 0 0
;	map_msg_info
	;�߼�����4���,0δ������,1������
        GET_PWORLD_VAR 68 32 1 27
        If 27 = 0
;		add_sys_msg_start
;		add_sys_msg �߼�����4
;		set_sys_msg_mode 3 0 0
;		map_msg_info
                var 27 1
                set_pworld_var 68 32 @27 1
        endif
ENDPROC
