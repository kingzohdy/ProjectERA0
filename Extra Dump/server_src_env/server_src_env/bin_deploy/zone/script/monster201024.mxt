;������� ,����롪��˹��.������
;����
proc 1
	strvar memid
	GET_MON_INFO 1 0 memid 0 0 0 0 0
	;TARGET_ADD_STATUS 3 @memid 3028 1 25920000 0 
	;TARGET_ADD_STATUS 3 @memid 3029 1 25920000 0 
	;TARGET_ADD_STATUS 3 @memid 3030 1 25920000 0 
	TARGET_ADD_STATUS 3 @memid 1095 1 25920000 0
	
	strvar mid
	GET_MON_INFO 1 0 mid 0 0 0 0 0
	;boss4��ʵ��id
	VAR 15 @mid
	set_pworld_var 20 32 @15 1
	var 10 0
	set_mon_var 0 32 @10 1
endproc
proc 2
	strvar mid
	GET_MON_INFO 1 0 mid 0 0 0 0 0
;	mon_speech @mid 2 2 1 ΰ������鰡������֮��������룬�ٻ�Ӱ.�ܿ�.���ޣ�
	;boss4��ʵ��id
	VAR 15 0
	set_pworld_var 20 32 @15 1
	;��������ؼ���
	get_pworld_var 8 32 1 12
	
	if 12 > 0 
		return
	endif	
	return
	call 10
endproc
proc 4
	return
	;��������ؼ���
	get_pworld_var 8 32 1 12
	
	if 12 > 0 
		return
	endif
	get_mon_var 0 32 1 10
	if 10 > 0
		return
	endif
	strvar percent
	GET_MON_HP_PERCENT percent 1
	if percent < 50
		var 10 1
		set_mon_var 0 32 @10 1
		call 10
	endif
endproc
proc 10
	add 12 3
	set_pworld_var 8 32 @12 1
	strvar mid
	GET_MON_INFO 1 0 mid 0 0 0 0 0
	mon_speech @mid 2 2 1 �������������ˣ�������ȥ���ǰ취���üӿ�����ٶȣ������ǣ�ΰ���Ӱ.�ܿ�.���޾Ϳ콵�٣���ʱ���׳���������������ˣ����޴��˻��������ǵġ�
endproc
