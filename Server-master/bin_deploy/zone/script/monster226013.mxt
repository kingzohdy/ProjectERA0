;������ż

proc 2
	get_pworld_var 5 8 1 32
	add 32 1
	set_pworld_var 5 8 @32 1
	;�����������������ܵ�����	
	
;	get_pworld_var 6 8 1 33
;	add 32 1
;	set_pworld_var 6 8 @23 1
	;��������

endproc 

proc 3
;·������
	get_pworld_var 5 8 1 32
	add 32 1
	set_pworld_var 5 8 @32 1
	;�����������������ܵ�����	
	get_pworld_var 7 8 1 34
	add 34 1
	set_pworld_var 7 8 @34 1
	;�ܵ���ż����
	
	DESTROY_MON 1

endproc 



