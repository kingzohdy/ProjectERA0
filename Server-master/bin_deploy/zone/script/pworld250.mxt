;���Ҹ�������������

;���������¼�
PROC 1
	; if or not start var
	strvar a
	var a 0
	set_pworld_var 0 32 @a 1
	
	; current kill monster number
	strvar f
	var f 0
	set_pworld_var 20 32 @f 1
	
	; current round, start from 0
	strvar g
	var g 0
	set_pworld_var 24 32 @g 1
	
	; current round left time
	strvar l
	var l 0
	set_pworld_var 44 32 @l 1
	
	; correct number
	strvar m
	var m 0
	set_pworld_var 48 32 @m 1
	
	; new npc
	new_npc 1 302129 7700 8300 180 npc302129.mac
	
	; clean mon flag
	strvar n
	var n 0
	set_pworld_var 52 32 @n 1
	
	; get award flag
	strvar p
	var p 0
	set_pworld_var 56 32 @p 1
	
	;csm
	new_npc 1 333165 7900 4200 180 csm333165.mac
ENDPROC

;�رո����¼�
proc 2
endproc

;������ʱ�¼���ÿ2�봥��һ�Σ�
proc 3
	strvar a f g l m n p
	get_pworld_var 0 32 1 a
	if @a = 1
		get_pworld_var 24 32 1 g
		if @g = 10
			;������Ϣ���ͻ���ֹͣ����
			strvar id
			get_pworld_first_player_id 1 id
			word_pwd_ui @id 2 0 0 1
		
			;ֹͣ����
			get_pworld_var 0 32 1 a
			var a 0
			set_pworld_var 0 32 @a 1
			
			;npc�ٴγ���
			get_pworld_var 56 32 1 p
			if p = 0
				new_npc 1 302129 7700 8300 180 npc302129.mac
			endif
			
			return 
		endif
		
		del_npc 302129 1
		
		; pworld start
		get_pworld_var 20 32 1 f
		get_pworld_var 44 32 1 l
		
		if @l = 0
			var n 0
			set_pworld_var 52 32 @n 1
		endif
		
		;��ʼ׼���Ϳ�ʼ������ʾ
		if @l = 0 or @l = 10
			call 300
		endif
		
		; ready left 10 second
		if @l = 0
			add_sys_msg_start
			add_sys_msg 10��󣬳��н�ˢ�´���������մ���˳���ɱ
			set_sys_msg_mode 4 0 0
			map_msg_info
		endif
		
		add @l 2
		set_pworld_var 44 32 @l 1
		
		; ˢ��
		if @l = 12
			word_pwd_rand_new_mon 200203 4 8000 8600 7400 8800 7400 8000 8000 8000 400 1
		endif
		
		; time out or killed monster number = 4
		if @f = 4
			add_sys_msg_start
			add_sys_msg ��ϲ�����ˣ��Ժ��㽫��ȡ���⽱��
			set_sys_msg_mode 4 0 0
			map_msg_info
			
			;current round++
			get_pworld_var 24 32 1 g
			add @g 1
			set_pworld_var 24 32 @g 1
			
			;reset time
			var l 0
			set_pworld_var 44 32 @l 1
			
			;correct number++
			get_pworld_var 48 32 1 m
			add @m 1
			set_pworld_var 48 32 @m 1
			
			; reset kill monster number
			var f 0
			set_pworld_var 20 32 @f 1
			
			;clean mon 
			destroy_mon_by_defid 1 200204
		endif
		
		; time out
		if @l = 50 and @f != 4
			add_sys_msg_start
			add_sys_msg ���ź������ʱ�䵽��
			set_sys_msg_mode 4 0 0
			map_msg_info
		
			;current round++
			get_pworld_var 24 32 1 g
			add @g 1
			set_pworld_var 24 32 @g 1
			
			;reset time
			var l 0
			set_pworld_var 44 32 @l 1
			
			; reset kill monster number
			var f 0
			set_pworld_var 20 32 @f 1
			
			;clean mon
			var n 1
			set_pworld_var 52 32 @n 1
			destroy_mon_by_defid 1 200204
			destroy_mon_by_defid 1 200203
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

; show ui
proc 300
	strvar round time
	strvar curround curtime
	strvar id
	
	get_pworld_first_player_id 1 id
	get_pworld_var 24 32 1 curround
	get_pworld_var 44 32 1 curtime
	
	var time 50
	sub @time @curtime
	add @curround 1
	
	if @curround <= 10
		if @time > 40
			word_pwd_ui @id 0 @curround @time 1
		else
			word_pwd_ui @id 1 @curround 39 1
		endif
	endif
	
endproc 