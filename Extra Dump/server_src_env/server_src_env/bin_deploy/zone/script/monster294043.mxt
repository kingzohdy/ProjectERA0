;������� ,��̨
;����
proc 1
	;�Ƿ��̨�յ�
	strvar mid hp
	GET_MON_INFO 1 0 mid hp 0 0 0 0
	sub hp 1
	mul hp -1
	;����̨��Ѫ	
	mon_set_hp @hp @mid 1
	;��̳��ʵ��id
	VAR 14 @mid
	set_pworld_var 16 32 @14 1
	
	;�ϴ�tick��Ѫ������
	var 10 0
	set_mon_var 0 32 @10 1
endproc
;����
proc 2
	;��̳��ʵ��id
	VAR 14 0
	set_pworld_var 16 32 @14 1
endproc
;tick
proc 4
	return
	strvar mid percent
	GET_MON_INFO 1 0 mid 0 0 0 0 0
	get_mon_hp_percent @mid percent
	if percent >= 100
		get_pworld_var 8 32 1 12;
		add 12 1
		;�����̨��Ѫ�׶�
		set_pworld_var 8 32 @12 1;
		;ȫ��aoe
		mon_skill 10351 1
		;��boss��aoe
		return
	endif
	get_mon_var 0 32 1 10
	set_mon_var 0 32 @percent 1
	dev 10 20
	dev percent 20
	if 10 != @percent
		mul percent 20
		add_sys_msg_start
		add_sys_msg ����!����!!��̳�������������$@percent$%��һ����̳����������ﵽ100%��Ӱ-�ܿ�-���޽�����������!!���ٶ����س��ֵ����������!!!
		set_sys_msg_mode 4 0 0
		map_msg_info
	endif
endproc
