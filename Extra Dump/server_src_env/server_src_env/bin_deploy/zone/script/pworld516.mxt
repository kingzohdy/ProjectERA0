
PROC 1	
	;boss �Ƿ�����
	set_pworld_var 0 32 0 0
	;����ʱ2����
	set_pworld_var 4 32 0 0
ENDPROC

;�ر��¼�
proc 2
	
endproc

;2��һ��
PROC 3
	get_pworld_var 0 32 10 11
	if @10 = 0 and @11 = 1
		get_pworld_var 4 32 12 13
		add 13 1
		set_pworld_var 4 32 @13 12
		
		DELETE_MON_BY_DEFID 207213 0
		DELETE_MON_BY_DEFID 207214 0
	endif
	
	get_pworld_var 4 32 14 15
	;2���ӹرո���
	if @14 = 0 and @15 >= 60
		PWORLD_OVER	
		COMPEL_CLOSE_PWORLD 1
	endif
ENDPROC

PROC 4
	
ENDPROC
