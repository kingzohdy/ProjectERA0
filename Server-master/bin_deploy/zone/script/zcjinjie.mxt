

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���Ƿ�֪��#c6d00d2׿Խװ��#n���Խ��׳ɸ�������#cff0080ʷʫ��װ#n��
	add_talk #b#b���ҽ�Ϊ���ṩ΢���������񣬾����������΢���������ɺϽ���Ϊװ�����׵Ĳ��ϡ�
	; add_talk #b#b#c676464��50����60��ÿ���������2�Σ�
	add_talk #b#b
	add_talk #b#bÿ��΢��ÿ�춼������#c1865063��#n��ÿ������#c18650620��#n
	add_talk #b#b#b#c186506����δ�������������ۼƵ����ʹ��#n
			
		task_add_option 2251 1 101
		task_add_option 2252 1 102
		task_add_option 2253 1 103
		task_add_option 2254 1 104

;		strvar time
;		GET_TIME_SEC time
;		set_role_var 18 32 @time 1
		
		strvar ret time
		get_role_var 18 32 1 time 
		is_same_day @time ret
		if ret = -1
			var 10 0
			set_role_var 23 8 @10 1
			var 11 0
			set_role_var 24 8 @11 1
			var 12 0
			set_role_var 25 8 @12 1
			var 13 0
			set_role_var 26 8 @13 1
		endif
	
		; task_stat 2251 10 30
		; if 10 = 0 and 30 = 3
			; option 151 #���������һ�ź�ҫ���
		; endif
		; task_stat 2252 10 31
		; if 10 = 0 and 31 = 3
			; option 160 #���������һ�ų�ͭ���
		; endif
		; task_stat 2253 10 32
		; if 10 = 0 and 32 = 3
			; option 170 #���������һ�ż�����
		; endif
		; task_stat 2254 10 33
		; if 10 = 0 and 33 = 3
			; option 180 #���������һ�ź������
		; endif
		
		option 190 ��Ҫ����΢��
		
		option 150 ��װ�����ס��롾΢��������˵��
		
	talk
	
	
endproc 

PROC 101
	task_do_option 2251 1
ENDPROC
PROC 102
	task_do_option 2252 1
ENDPROC
PROC 103
	task_do_option 2253 1
ENDPROC
PROC 104
	task_do_option 2254 1
ENDPROC

proc 151
	exchg_item 2 1
	call 190
endproc
proc 160
	exchg_item 3 1
	call 190
endproc
proc 170
	exchg_item 4 1
	call 190
endproc
proc 180
	exchg_item 5 1
	call 190
endproc

proc 190
	DIALOG 1
	add_talk ������������
	
	strvar LV
	exchg_item_get 2 40
	exchg_item_get 3 50
	exchg_item_get 4 60
	exchg_item_get 5 70
	task_stat 2251 10 30
	task_stat 2252 10 31
	task_stat 2253 10 32
	task_stat 2254 10 33
	GET_PLAYER_BASE_ATT 1 1 LV 1
	
	if 30 = 3
		if 40 != -1
			add_talk ��ҫ΢����ʣ��#c186506$@40$#n/30��
		else
			add_talk ��ҫ΢����ʣ��#c1865060#n/30��
		endif
	else
		if @LV > 49
		add_talk ���#c341700װ������-��ҫ���#n����󣬼�������#c186506��ҫ΢��#n
		endif
	endif
	if 31 = 3
		if 50 != -1
			add_talk ��ͭ΢����ʣ��#c186506$@50$#n/30��
		else
			add_talk ��ͭ΢����ʣ��#c1865060#n/30��
		endif
	else
		if @LV > 59
		add_talk ���#c341700װ������-��ͭ���#n����󣬼�������#c186506��ͭ΢��#n
		endif
	endif
	if 32 = 3
		if 60 != -1
			add_talk ���΢����ʣ��#c186506$@60$#n/30��
		else
			add_talk ���΢����ʣ��#c1865060#n/30��
		endif
	else
		if @LV > 69
		add_talk ���#c341700װ������-������#n����󣬼�������#c186506���΢��#n
		endif
	endif
	if 33 = 3
		if 70 != -1
			add_talk ����΢����ʣ��#c186506$@70$#n/30��
		else
			add_talk ����΢����ʣ��#c1865060#n/30��
		endif
	else
		if @LV > 79
			add_talk ���#c341700װ������-�������#n����󣬼�������#c186506����΢��#n
		endif
	endif
	
	if 10 = 0 and 30 = 3
		option 151 #���������1�� - ��ҫ���
	endif
	if 10 = 0 and 31 = 3
		option 160 #���������1�� - ��ͭ���
	endif
	if 10 = 0 and 32 = 3
		option 170 #���������1�� - ������
	endif
	if 10 = 0 and 33 = 3
		option 180 #���������1�� - �������
	endif
	
	
		option 1 ����
	talk
endproc

proc 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����ԡ�װ�����ס���΢�����������в����׵ĵط���������Ӧ���Ҷ����ˡ�

	option 251 ��װ�����ס�����������
	option 252 ��΢���������ۻ�����˵��
	option 1 ���Ѿ������˽�
	talk
endproc



proc 251
	DIALOG 1
;	add_talk $_npc$:
	add_talk #b#b#c186506�ʣ�װ�������Ǳ�Ȼ�ɹ�����#n
	add_talk #b#b��Of Course�����ԻݼҼ������������֤��
	add_talk #b#b
	add_talk #b#b#c186506�ʣ�װ�����׺���֮ǰϴ������ɫ���ԡ�ǿ�����Ǽ�����Ƕ�ı�ʯ������#n
	add_talk #b#b�𣺵�Ȼ�ڣ�����֮�⻹�и�ħ���ԡ���ӡ���Զ��ᱣ����
	add_talk #b#b
	add_talk #b#b#c186506�ʣ�������Ҫ�ĺ�ҫ�Ͻ�#c676464��50��װ�����ײ��ϣ�#n��#c186506��ͭ�Ͻ�#n#c676464��60��װ�����ײ��ϣ�#n��#c186506��ֺϽ�#n#c676464��70��װ�����ײ��ϣ�#n��#c186506����Ͻ�#n#c676464��80��װ�����ײ��ϣ�#n#c186506����ã�#n
	add_talk #b#b��ͨ����֮�������Ӧ��#c0181f9����顱�ϳ�#n��
	add_talk #b#b
	add_talk #b#b#c186506�ʣ��ϳɡ��Ͻ���Ҫ�ġ���顱����ã�#n
	add_talk #b#b��ͨ��΢���ϳɣ��ͽ���5�ǽ���������BOSS���䣡�Լ���������ܶһ���������
	add_talk #b#b
	add_talk #b#b#c186506�ʣ��ϳɡ���顱��΢������ã�#n
	add_talk #b#b��Ī���ż�-��ҫ΢����������̳-��ͭ΢�����γ���Ѷ��-���΢����
	add_talk #b#b
	; add_talk #b#b#c186506�ʣ����е���ɫװ�������Խ��׳ɷ�װ��#n
	; add_talk #b#b��˵���������ҽ��ö��˲�С�Ľ��������µġ�װ������-����ƪ����ʧ�ˣ���Ȼ����Ŭ�����ȣ�Ǳ�����У���������δ�ҵ����ν��ױ�Ȼ�ɹ��ķ�����Ϊ�˱������ò����ײŵ��ֵ���ɫװ���𻵡����ϣ�����ֻ�����֮һ�ļ���Ҳ�����ǲ�Ը�����ģ�
	; add_talk #b#b�����ҽ��ö��˾�����ʱ���ṩ���Σ����Ρ���������ָ���������Ľ��׷���
	add_talk #b#b#c186506�ʣ����Ρ�����Ҳ���Խ�����#n
	add_talk #b#b�����Ρ���������ָ��������������һ�ֽ��׹��գ�����ͨ��#c186506����Ͻ�#n#c676464��60-69�����ײ��ϣ�#n��#c186506���ҺϽ�#n#c676464��70-79�����ײ��ϣ�#n��ʵ�ֽ��ס��������ʧ��֮���ҵ���Щ�Ͻ����顣
	
	option 150 лл������Ľ��
	talk
endproc 

proc 252
	DIALOG 1
	add_talk $_npc$:
	add_talk ��#bÿ��΢��ֻҪ�ɹ������������ɷֱ�����#c186506�ۻ���������#n�ķ���
	add_talk ��#b�ۻ������������񣬿ɰѵ���#c186506δ��������#n�ۻ������ʹ��
	add_talk ��#bÿ��΢������ܹ��ۻ�#c18650630��#n��������
	
	option 150 лл�����ϸ˵��
	talk
endproc

;;;-------------------------------------------------------------------------------
; proc 888
; ���ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ���
	; strvar time
	; get_role_var 23 8 1 10
	
	; add_sys_msg_start
	; add_sys_msg $@10$
	; set_sys_msg_mode 1 0 0
	; msg_info
	; strvar num
	; GET_PACK_SPACE 0 num
	; if num = 0
		; call 555
		; return
	; endif
	; if 10 < 3
		; GET_ITEM_NUM_INPAK 5060200 1 30	
		; if 30 >= 20
			; dec_item_num_inpak 5060200 20 15
			; if 15 = 0
				; add_item_num_inpak 5060100 1 1 0
				
				; GET_TIME_SEC time
				; set_role_var 18 32 @time 1			
				; add 10 1
				; set_role_var 23 8 @10 1		
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg ��ҫ΢������20�ţ��޷�����
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg ÿ��ֻ������3�κ�ҫ���
		; set_sys_msg_mode 3 0 0
		; msg_info
	; endif
; endproc 
; proc 889
;���ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ���
	; strvar num
	; GET_PACK_SPACE 0 num
	; if num = 0
		; call 555
		; return
	; endif
	; strvar time
	; get_role_var 24 8 1 11
	; if 11 < 3
		; GET_ITEM_NUM_INPAK 5060201 1 30	
		; if 30 >= 20
			; dec_item_num_inpak 5060201 20 15
			; if 15 = 0
				; add_item_num_inpak 5060101 1 1 0
				
				; GET_TIME_SEC time
				; set_role_var 18 32 @time 1			
				; add 11 1
				; set_role_var 24 8 @11 1		
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg ��ͭ΢������20�ţ��޷�����
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg ÿ��ֻ������3�γ�ͭ���
		; set_sys_msg_mode 3 0 0
		; msg_info
	; endif

; endproc 
; proc 890
;���ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ���
	; strvar num
	; GET_PACK_SPACE 0 num
	; if num = 0
		; call 555
		; return
	; endif
	; strvar time
	; get_role_var 25 8 1 12
	; if 12 < 3
		; GET_ITEM_NUM_INPAK 5060202 1 30	
		; if 30 >= 20
			; dec_item_num_inpak 5060202 20 15
			; if 15 = 0
				; add_item_num_inpak 5060102 1 1 0
				
				; GET_TIME_SEC time
				; set_role_var 18 32 @time 1			
				; add 12 1
				; set_role_var 25 8 @12 1		
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg ���΢������20�ţ��޷�����
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg ÿ��ֻ������3�μ�����
		; set_sys_msg_mode 3 0 0
		; msg_info
	; endif
; endproc 
; proc 891
;���ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ��ݱ���
	; strvar num
	; GET_PACK_SPACE 0 num
	; if num = 0
		; call 555
		; return
	; endif
	; strvar time
	; get_role_var 26 8 1 13
	; if 13 < 3
		; GET_ITEM_NUM_INPAK 5060203 1 30	
		; if 30 >= 20
			; dec_item_num_inpak 5060203 20 15
			; if 15 = 0
				; add_item_num_inpak 5060103 1 1 0
				
				; GET_TIME_SEC time
				; set_role_var 18 32 @time 1			
				; add 13 1
				; set_role_var 26 8 @13 1		
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg ����΢������20�ţ��޷�����
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg ÿ��ֻ������3�κ������
		; set_sys_msg_mode 3 0 0
		; msg_info
	; endif
; endproc
; proc 555

	; add_sys_msg_start
	; add_sys_msg �����������޷������Ʒ
	; set_sys_msg_mode 3 0 0
	; msg_info
; endproc 






