

proc 1
	DIALOG 1
	add_talk $_npc$:
;	add_talk #b#b�������˰�������Щ�Է��򿪲ɱ�ʯ���˳�������ʯ���ˡ���������ϣ���������ҵ����ǵı�ʯ��
;	add_talk #b#b��������ʯ�󱻳�������ʯ�����󡱣�����һ���ǳ��������;��ֻ�������������ܹ����б�ʯ������ϣ����Щ��Ϣ�������á�
	add_talk #b#b��˵Σ�յ�ʧ��֮����Ѱ�ҵ���Ⱦ�ĸ�������,������ĸ�������,�ܹ�������������Ʒ�ʵı�ʯ���г���,������ʯ���Ǽ�.
	add_talk #b#b�����ð�����ҵ�������Ⱦ�ĸ�������,������������,���ñ�ʯħ�󾻻���,ÿ����һ����Ⱦ�ĸ�������,��Ҫ����һ�ű�ʯ��Ƭ,�����ھ���,ÿ��ÿ�����ֻ�ܾ���50��.
	add_talk #b#b#c0a6400��ҵ�һ�γɹ�������Ⱦ�ĸ���������,ÿ��ɾ����ķݶ��������ۻ�,����������ֻ������10��,��ô�ڶ�����Ծ����ķݶ�Ϊ:��һ��ʣ���40��,���ϵڶ����50��,����90��!����ۻ�500��,�������ֲ������ۻ�
	
	; exchg_item_get 5213411 40
	; if 40 != -1
		; add_talk #b#b���������ɾ���������$@40$
	; endif

	
	strvar time
	get_role_var 41 32 1 time
	is_same_day @time ret
	if ret = -1
		var 15 0
		set_role_var 40 8 @15 1
		
	endif
	task_add_option 3403 1 100
	; task_add_option 4160 1 102
	; task_add_option 4161 1 103
	; task_add_option 4162 1 104
	; task_add_option 4163 1 105
	; task_add_option 4164 1 106
	; task_add_option 4165 1 107
	; task_add_option 4166 1 108
	; task_add_option 4167 1 109
	; task_add_option 4168 1 110
	; task_add_option 4169 1 111
	; task_add_option 4170 1 112
	; task_add_option 4171 1 113
	; task_add_option 4172 1 114
	; task_add_option 4173 1 115
	; task_add_option 4174 1 116
	; task_add_option 4175 1 117
	; task_add_option 4176 1 118
	; task_add_option 4177 1 119
	; task_add_option 4178 1 120
	; task_add_option 4179 1 121
	
	
	TASKCYCLE_ADD_OPTION 140 1 101



	OPTION 200 #(����)���̵�

;	OPTION 201 #��ʯ����

;	OPTION 202 #��ʯ��Ƕ

;	OPTION 203 #��ʯժȡ

	strvar Ret
	task_stat 3677 1 Ret
	if Ret = 2
		option 300 ���湫������������Ҫ����������

	endif
	option 500 ��ѯ�ɾ�������
	option 400 ����10����Ⱦ�ĸ�������	
	
	task_stat 4492 1 20
	if 20 = 2
		option 250 #��ʯ����ϣ��������һ������
	endif
	;option 410 ����20����Ⱦ�ĸ�������
	;option 420 ����50����Ⱦ�ĸ�������

	talk
endproc 

PROC 100
	task_do_option 3403 1
ENDPROC

proc 200
	OPEN_NPC_SHOP
ENDPROC 
proc 201
	open_ui 14
ENDPROC 
proc 202
	open_ui 15
ENDPROC 
proc 203
	open_ui 16
ENDPROC 
proc 204
	open_npc_shop
endproc 
 PROC 101	
	TASKCYCLE_DO_OPTION 140 1
 ENDPROC

PROC 102
	task_do_option 4160 1
ENDPROC
PROC 103
	task_do_option 4161 1
ENDPROC
PROC 104
	task_do_option 4162 1
ENDPROC
PROC 105
	task_do_option 4163 1
ENDPROC
PROC 106
	task_do_option 4164 1
ENDPROC
PROC 107
	task_do_option 4165 1
ENDPROC
PROC 108
	task_do_option 4166 1
ENDPROC
PROC 109
	task_do_option 4167 1
ENDPROC
PROC 110
	task_do_option 4168 1
ENDPROC
PROC 111
	task_do_option 4169 1
ENDPROC
PROC 112
	task_do_option 4170 1
ENDPROC
PROC 113
	task_do_option 4171 1
ENDPROC
PROC 114
	task_do_option 4172 1
ENDPROC
PROC 115
	task_do_option 4173 1
ENDPROC
PROC 116
	task_do_option 4174 1
ENDPROC
PROC 117
	task_do_option 4175 1
ENDPROC
PROC 118
	task_do_option 4176 1
ENDPROC
PROC 119
	task_do_option 4177 1
ENDPROC
PROC 120
	task_do_option 4178 1
ENDPROC
PROC 121
	task_do_option 4179 1
ENDPROC



proc 250
	dialog 3
	add_talk $_npc$:
	add_talk #b#b��������ĸ����з���#c186506��˼��#n��#c186506������#n
	add_talk #b#b�����û�����ӵĻ�����ȥ�����Ƕ������ɣ�
	
	NPC_COMMIT_OPT 251
	
	talk
endproc 
proc 251
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg �㻹û���ύ��Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	
	GET_COMMIT_ITEM_INFO 2 m_Pos1
	GET_COMMIT_ITEM_INFO 3 m_Pos2
	GET_COMMIT_ITEM_INFO 4 m_Col
	task_stat 4492 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4492 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4492 1
					call 502
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	
	endif
endproc 
proc 502
	add_sys_msg_start
	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 



proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b̫��л�ˣ����������ĥ��û��ĥ�Ʊ�ʯ�ء�
		option 301 #������������
	talk
endproc 
proc 301 
	GET_ITEM_NUM_INPAK 5300874 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300874 1 30
		if 30 != -1
			task_fini 3677 1
		endif
	endif
endproc 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc 400
	choose 1 402 403 �Ƿ�����1����ʯ��Ƭ,����10����Ⱦ�ĸ�������?
endproc 

proc 402
	exchg_item 1 1
endproc
proc 500
	DIALOG 1
	add_talk �ɾ���������
	
	exchg_item_get 1 40
	if 40 != -1
		add_talk #b#b��Ⱦ�ĸ���������ʣ��#c186506$@40$#n��		
	else
		add_talk #b#b��Ⱦ�ĸ���������ʣ��#c1865060#n��
	endif	
	add_talk #b
	add_talk #b#b��ô�����Ⱦ�ĸ�������:ʧ��֮���������
	add_talk #b#b��ô������Ⱦ�ĸ�������:�ڱ�ʯ���˴�,ÿ�ξ�����Ҫ����10����Ⱦ�ĸ���������1����ʯ��Ƭ,���Եõ�10����������
	add_talk #b#b��������:���Ը�����������Ʒ�ʵı�ʯ����,������ʯ������
		
	option 1 ����
	talk
endproc
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; proc 888
;;;����
;;;����
;;;����

	; get_role_var 40 8 1 15 
	
	; if 15 >= 50
		
		; add_sys_msg_start
		; add_sys_msg ÿ��ÿ�����ֻ�ܾ���50��,�����쾻�������Ѿ��ﵽ50��!
		; set_sys_msg_mode 3 0 0
		; msg_info
		; return
	; endif

	; strvar num
	; GET_PACK_SPACE 0 num
	; if num = 0
		; call 555
		; return
	; endif
	
	; GET_ITEM_NUM_INPAK 5060014 1 30	
	; GET_ITEM_NUM_INPAK 5213410 1 40	
	; if 30 >= 10 
		; if 40 >= 10
			
			; dec_item_num_inpak 5060014 10 50
			; dec_item_num_inpak 5213410 10 55
			; if 50 = 0 and 55 = 0
				; ADD_BIND_ITEM_NUM_INPAK 5213411 10 1 0
				
				; strvar time
				; GET_TIME_SEC time
				; set_role_var 41 32 @time 1
				
				; add 15 10
				; set_role_var 40 8 @15 1
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg ��Ⱦ�ĸ����������㣡
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg ��ʯ��Ƭ���㣡
		; set_sys_msg_mode 3 0 0
		; msg_info
	; endif
; endproc
proc 403
endproc 


; proc 410
	
	; choose 1 412 403 ����20����Ⱦ�ĸ���������Ҫ����20�ݱ�ʯ��Ƭ,�Ƿ�ȷ��?
; endproc
; proc 412
	; get_role_var 40 8 1 15 
	; if 15 >= 50
		
		; add_sys_msg_start
		; add_sys_msg ÿ��ÿ�����ֻ�ܾ���50��,�����쾻�������Ѿ��ﵽ50��!
		; set_sys_msg_mode 3 0 0
		; msg_info
		; return
	; endif

	; strvar num
	; GET_PACK_SPACE 0 num
	; if num = 0
		; call 555
		; return
	; endif
	
	; GET_ITEM_NUM_INPAK 5060014 1 30	
	; GET_ITEM_NUM_INPAK 5213410 1 40	
	; if 30 >= 20 
		; if 40 >= 20
			
			; dec_item_num_inpak 5060014 20 50
			; dec_item_num_inpak 5213410 20 55
			; if 50 = 0 and 55 = 0
				; ADD_BIND_ITEM_NUM_INPAK 5213411 20 1 0
				
				; strvar time
				; GET_TIME_SEC time
				; set_role_var 41 32 @time 1
				
				; add 15 20
				; set_role_var 40 8 @15 1
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg ��Ⱦ�ĸ����������㣡
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg ��ʯ��Ƭ���㣡
		; set_sys_msg_mode 3 0 0
		; msg_info
	; endif


; endproc 
; proc 420

	; choose 1 422 403 ����50����Ⱦ�ĸ���������Ҫ����50�ݱ�ʯ��Ƭ,�Ƿ�ȷ��?

; endproc 
; proc 422
	; get_role_var 40 8 1 15 
	; if 15 >= 50
		
		; add_sys_msg_start
		; add_sys_msg ÿ��ÿ�����ֻ�ܾ���50��,�����쾻�������Ѿ��ﵽ50��!
		; set_sys_msg_mode 3 0 0
		; msg_info
		; return
	; endif

	; strvar num
	; GET_PACK_SPACE 0 num
	; if num = 0
		; call 555
		; return
	; endif
	
	; GET_ITEM_NUM_INPAK 5060014 1 30	
	; GET_ITEM_NUM_INPAK 5213410 1 40	
	; if 30 >= 50 
		; if 40 >= 50
			
			; dec_item_num_inpak 5060014 50 50
			; dec_item_num_inpak 5213410 50 55
			; if 50 = 0 and 55 = 0
				; ADD_BIND_ITEM_NUM_INPAK 5213411 50 1 0
				
				; strvar time
				; GET_TIME_SEC time
				; set_role_var 41 32 @time 1
				
				; add 15 50
				; set_role_var 40 8 @15 1
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg ��Ⱦ�ĸ����������㣡
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg ��ʯ��Ƭ���㣡
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



















