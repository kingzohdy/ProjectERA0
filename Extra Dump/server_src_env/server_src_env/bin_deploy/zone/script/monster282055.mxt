;70����,��5��������
proc 4
	;�߼�����boss1������,0δ������,1��һ�ű���,2��һ�ű���,3�����ű���
	get_pworld_var 80 32 1 30
	if 30 < 3
		return
	endif
	strvar x y
	GET_MON_INFO 1 0 0 0 0 0 x y
	del_mon 1
	new_mon 282049 1 @x @y 0 0 0 1 0
endproc
