;����֮�䣨��ǿ����

;���������¼�
PROC 1
	;0λ��ʾ��ʼˢ�����ı�־   ����ֵΪ1��ʾ��ʼ��սѡ�������
	set_pworld_var 0 32 0 1
	;ÿ��������ˢһ�ι�
	set_pworld_var 4 32 0 1
	;��¼�ķ���
	set_pworld_var 8 32 0 1
	
	DAQIANGDAO_FENSHU 0 0
ENDPROC

;�رո����¼�
proc 2
	
endproc

;������ʱ�¼���ÿ2�봥��һ�Σ�
proc 3
	strvar sta1 sta2
	
	GET_PWORLD_VAR 0 32 20 sta1
	if @sta1 = 1	
		GET_PWORLD_VAR 4 32 20 sta2
		if @sta2 >= 3 
			call 200
			set_pworld_var 4 32 0 1
		else
			add sta2 1
			set_pworld_var 4 32 @sta2 1
		endif
	endif
endproc

;�����й��������¼�
proc 4

endproc

;�����н�ɫ�����¼�
proc 5

endproc

;�����н�ɫ�����¼�
proc 6

endproc

;�����е���ʱ�����¼�
proc 7
	
endproc 

;�������ʼ�¼�
proc 8
endproc 


proc 200		
	add_sys_msg_start
	add_sys_msg ��ǰ��Ȼ�и���Ӱ����
	set_sys_msg_mode 3 0 0
	MAP_MSG_INFO 			
	
	strvar outx outy
	strvar outx1 outy1
	strvar outx2 outy2
	strvar outx3 outy3
	strvar outx4 outy4
	GET_MAP_RANDPOINT  12822 12737 800 12 outx outy 
	GET_MAP_RANDPOINT  12822 12737 800 13 outx1 outy1 
	GET_MAP_RANDPOINT  12822 12737 800 14 outx2 outy2 
	GET_MAP_RANDPOINT  12822 12737 800 15 outx3 outy3 
	GET_MAP_RANDPOINT  12822 12737 800 16 outx4 outy4 
	
	rand 17 5
	if @17 = 0
		NEW_LIFE_MON 230059 1 @outx @outy 0 0 0 0 4 0 0
	elseif @17 = 1
		NEW_LIFE_MON 230060 1 @outx1 @outy1 0 0 0 0 4 0 0
	elseif @17 = 2
		NEW_LIFE_MON 230061 1 @outx2 @outy2 0 0 0 0 4 0 0
	elseif @17 = 3
		NEW_LIFE_MON 230062 1 @outx3 @outy3 0 0 0 0 4 0 0
	elseif @17 = 4
		NEW_LIFE_MON 230063 1 @outx4 @outy4 0 0 0 0 4 0 0
	else
		return
	endif
endproc

