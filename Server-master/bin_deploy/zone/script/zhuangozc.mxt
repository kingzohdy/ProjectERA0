;תְ������ʿ��������

proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Զ��Ҫ��ͼ֪���Լ���ĩ�գ������İ�������ڻ��ڵ��¡�
		
;	strvar Ret num1
;	GET_ITEM_NUM_INPAK 5300925 1 num1
;	if num1 > 0
;		TASK_STAT 3305 1 Ret
;		if Ret = 2
;			option 700 �����뻻����
;		endif
;	endif
	
	option 120 #��ʱ���

	talk
	
endproc
proc 120

	transmit 1000 28100 8800 1
endproc 

	
; proc 700
	; DIALOG 1
	; add_talk $_npc$:
	; add_talk #b#b���ڽ���Ʈ������ʲô���˽���ط��׸��õ��أ�
	; add_talk #b#b��������ۣ���ƣ�����־���ֶ�����������ˮ����ض��ǡ�

		; option 701 #����

	; talk
	
; endproc
; proc 701
	
	; strvar ItmNum Num Rad
	; GET_ITEM_NUM_INPAK 5300925 1 ItmNum
	; if ItmNum > 0
			
		; GET_PACK_SPACE 0 Num
		; if Num > 0
			; DEC_ITEM_NUM_INPAK 5300925 1 1
			
			; rand Rad 100
			; if @Rad < 80
				; ADD_ITEM_NUM_INPAK 5300926 1 0 0
			; endif
			; if @Rad >= 80 and @Rad < 95
				; ADD_ITEM_NUM_INPAK 5300927 1 0 0
			; endif
			; if @Rad >= 95 and @Rad < 99
				; ADD_ITEM_NUM_INPAK 5300928 1 0 0
			; endif
			; if @Rad = 99
				; ADD_ITEM_NUM_INPAK 5300929 1 0 0
			; endif
		; else
			; add_sys_msg_start
			; add_sys_msg �����ռ䲻�㣡
			; set_sys_msg_mode 1 0 0
			; msg_info
		; endif
	; else
		; add_sys_msg_start
		; add_sys_msg ������û�п��Խ�������Ʒ��
		; set_sys_msg_mode 1 0 0
		; msg_info
	; endif
; endproc 