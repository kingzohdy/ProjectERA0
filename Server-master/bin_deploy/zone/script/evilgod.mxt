;а��,��Ԩ������˹���
;���ʼ
;PROC 1
	; strvar ret
	; var ret 0
	; SET_EVENT_VAR 0 32 @ret 1 
;	��ʾ����
	; var 11 1
	; set_event_var 4 32 @11 1
	; strvar procnum i
	; var procnum 100
	; var i 0
	; while i < 1
		; var procnum 100
		; add procnum @i
		; call @procnum;��ȡ��������
		; GET_EVENT_VAR 0 32 1 ret
		; add procnum 100
		; if ret > 0
			; call @procnum;��ʾ
			; return
		; endif
		; var ret 0
		; SET_EVENT_VAR 0 32 @ret 1
		; add i 1
	; endwhile
	;��ȡ����
	; strvar first day
	; GET_WORLD_FIRST_START_TIME 1 first
	; get_time_sec day
	; sub day @first
	;��������
	; dev day 86400
	; mod day 1
	; var procnum 300
	; add procnum @day
	;��boss
	; call @procnum
	; sub procnum 100
	;��ʾ
	; call @procnum
; ENDPROC
;10��һ��
; PROC 3
;	��ʾ����
	; get_event_var 4 32 1 11
	; if 11 < 1 
		; return
	; endif
	; if 11 > 30
		; var 11 0
		; SET_EVENT_VAR 4 32 @11 1
		; return
	; else
		; add 11 1
		; SET_EVENT_VAR 4 32 @11 1
	; endif
	; var 20 @11
	; mod 20 6
	; if 20 != 0
		; return
	; endif
	; strvar ret
	; var ret 0
	; SET_EVENT_VAR 0 32 @ret 1
	; strvar procnum i
	; var procnum 100
	; var i 0
	; while i < 1
		; var procnum 100
		; add procnum @i
		; call @procnum;��ȡ��������
		; GET_EVENT_VAR 0 32 1 ret
		; add procnum 100
		; if ret > 0
			; call @procnum;��ʾ
			; return
		; endif
		; var ret 0
		; SET_EVENT_VAR 0 32 @ret 1
		; add i 1
	; endwhile
; ENDPROC
;�жϵ�ͼ�Ϲ��������

;;;;;;;;;;;;;;;;;;2.28ע�͵�ˢ��˹���
 proc 1
	; strvar num
	; var num 0
	; GET_MAP_MON_NUM 3200 200030 1 num 0
	;--SET_EVENT_VAR 0 32 @num 1 
	; if num = 0
		; call 200
		; call 300
	; endif
 endproc
;��ʾ
; proc 200
	; add_sys_msg_start
	; add_sys_msg #L�γ���Ѷ��@@(3210,13500,13200)#n�ͻȻ��������Ԩ������˹�������Ϣ�������ڴ˾�����Щ�ӽ�������
	; set_sys_msg_mode 6 0 0
	; svr_msg_info	
; endproc
;��boss
; proc 300
	; NEW_MAP_MON 200030 1 3210 13500 13200 0 0 0 0 0 1 0
; endproc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;�жϵ�ͼ�Ϲ��������
;proc 101
;	strvar num
;	var num 0
;	GET_MAP_MON_NUM ::map2:: 200030 1 num 0
;	SET_EVENT_VAR 0 32 @num 1 
;endproc
;;��ʾ
;proc 201
;	add_sys_msg_start
;	add_sys_msg ::map2name::ͻȻ��������Ԩ������˹�������Ϣ�������ڴ˾�����Щ�ӽ�������
;	set_sys_msg_mode 3 0 0
;	svr_msg_info	
;endproc
;;��boss
;proc 301
;	NEW_MAP_MON 200030 1 ::map2:: ::map2x:: ::map2y:: 0 0 0 0 0 1 0
;endproc
;;�жϵ�ͼ�Ϲ��������
;proc 102
;	strvar num
;	var num 0
;	GET_MAP_MON_NUM ::map3:: 200030 1 num 0
;	SET_EVENT_VAR 0 32 @num 1 
;endproc
;;��ʾ
;proc 202
;	add_sys_msg_start
;	add_sys_msg ::map3name::ͻȻ��������Ԩ������˹�������Ϣ�������ڴ˾�����Щ�ӽ�������
;	set_sys_msg_mode 3 0 0
;	svr_msg_info	
;endproc
;;��boss
;proc 302
;	NEW_MAP_MON 200030 1 ::map3:: ::map3x:: ::map3y:: 0 0 0 0 0 1 0
;endproc
;;�жϵ�ͼ�Ϲ��������
;proc 103
;	strvar num
;	var num 0
;	GET_MAP_MON_NUM ::map4:: 200030 1 num 0
;	SET_EVENT_VAR 0 32 @num 1 
;endproc
;;��ʾ
;proc 203
;	add_sys_msg_start
;	add_sys_msg ::map4name::ͻȻ��������Ԩ������˹�������Ϣ�������ڴ˾�����Щ�ӽ�������
;	set_sys_msg_mode 3 0 0
;	svr_msg_info	
;endproc
;;��boss
;proc 303
;	NEW_MAP_MON 200030 1 ::map4:: ::map4x:: ::map4y:: 0 0 0 0 0 1 0
;endproc
;;�жϵ�ͼ�Ϲ��������
;proc 104
;	strvar num
;	var num 0
;	GET_MAP_MON_NUM ::map5:: 200030 1 num 0
;	SET_EVENT_VAR 0 32 @num 1 
;endproc
;;��ʾ
;proc 204
;	add_sys_msg_start
;	add_sys_msg ::map5name::ͻȻ��������Ԩ������˹�������Ϣ�������ڴ˾�����Щ�ӽ�������
;	set_sys_msg_mode 3 0 0
;	svr_msg_info	
;endproc
;;��boss
;proc 304
;	NEW_MAP_MON 200030 1 ::map5:: ::map5x:: ::map5y:: 0 0 0 0 0 1 0
;endproc
;;�жϵ�ͼ�Ϲ��������
;proc 105
;	strvar num
;	var num 0
;	GET_MAP_MON_NUM ::map6:: 200030 1 num 0
;	SET_EVENT_VAR 0 32 @num 1 
;endproc
;;��ʾ
;proc 205
;	add_sys_msg_start
;	add_sys_msg ::map6name::ͻȻ��������Ԩ������˹�������Ϣ�������ڴ˾�����Щ�ӽ�������
;	set_sys_msg_mode 3 0 0
;	svr_msg_info	
;endproc
;;��boss
;proc 305
;	NEW_MAP_MON 200030 1 ::map6:: ::map6x:: ::map6y:: 0 0 0 0 0 1 0
;endproc
;;�жϵ�ͼ�Ϲ��������
;proc 106
;	strvar num
;	var num 0
;	GET_MAP_MON_NUM ::map7:: 200030 1 num 0
;	SET_EVENT_VAR 0 32 @num 1 
;endproc
;;��ʾ
;proc 206
;	add_sys_msg_start
;	add_sys_msg ::map7name::ͻȻ��������Ԩ������˹�������Ϣ�������ڴ˾�����Щ�ӽ�������
;	set_sys_msg_mode 3 0 0
;	svr_msg_info	
;endproc
;;��boss
;proc 306
;	NEW_MAP_MON 200030 1 ::map7:: ::map7x:: ::map7y:: 0 0 0 0 0 1 0
;endproc
;;�жϵ�ͼ�Ϲ��������
;proc 107
;	strvar num
;	var num 0
;	GET_MAP_MON_NUM ::map8:: 200030 1 num 0
;	SET_EVENT_VAR 0 32 @num 1 
;endproc
;;��ʾ
;proc 207
;	add_sys_msg_start
;	add_sys_msg ::map8name::ͻȻ��������Ԩ������˹�������Ϣ�������ڴ˾�����Щ�ӽ�������
;	set_sys_msg_mode 3 0 0
;	svr_msg_info	
;endproc
;;��boss
;proc 307
;	NEW_MAP_MON 200030 1 ::map8:: ::map8x:: ::map8y:: 0 0 0 0 0 1 0
;endproc
;;�жϵ�ͼ�Ϲ��������
;proc 108
;	strvar num
;	var num 0
;	GET_MAP_MON_NUM ::map9:: 200030 1 num 0
;	SET_EVENT_VAR 0 32 @num 1 
;endproc
;;��ʾ
;proc 208
;	add_sys_msg_start
;	add_sys_msg ::map9name::ͻȻ��������Ԩ������˹�������Ϣ�������ڴ˾�����Щ�ӽ�������
;	set_sys_msg_mode 3 0 0
;	svr_msg_info	
;endproc
;;��boss
;proc 308
;	NEW_MAP_MON 200030 1 ::map9:: ::map9x:: ::map9y:: 0 0 0 0 0 1 0
;endproc
;;�жϵ�ͼ�Ϲ��������
;proc 109
;	strvar num
;	var num 0
;	GET_MAP_MON_NUM ::map10:: 200030 1 num 0
;	SET_EVENT_VAR 0 32 @num 1 
;endproc
;;��ʾ
;proc 209
;	add_sys_msg_start
;	add_sys_msg ::map10name::ͻȻ��������Ԩ������˹�������Ϣ�������ڴ˾�����Щ�ӽ�������
;	set_sys_msg_mode 3 0 0
;	svr_msg_info	
;endproc
;;��boss
;proc 309
;	NEW_MAP_MON 200030 1 ::map10:: ::map10x:: ::map10y:: 0 0 0 0 0 1 0
;endproc
;;�жϵ�ͼ�Ϲ��������
;proc 110
;	strvar num
;	var num 0
;	GET_MAP_MON_NUM ::map11:: 200030 1 num 0
;	SET_EVENT_VAR 0 32 @num 1 
;endproc
;;��ʾ
;proc 210
;	add_sys_msg_start
;	add_sys_msg ::map11name::ͻȻ��������Ԩ������˹�������Ϣ�������ڴ˾�����Щ�ӽ�������
;	set_sys_msg_mode 3 0 0
;	svr_msg_info	
;endproc
;;��boss
;proc 310
;	NEW_MAP_MON 200030 1 ::map11:: ::map11x:: ::map11y:: 0 0 0 0 0 1 0
;endproc
;;�жϵ�ͼ�Ϲ��������
;proc 111
;	strvar num
;	var num 0
;	GET_MAP_MON_NUM ::map12:: 200030 1 num 0
;	SET_EVENT_VAR 0 32 @num 1 
;endproc
;;��ʾ
;proc 211
;	add_sys_msg_start
;	add_sys_msg ::map12name::ͻȻ��������Ԩ������˹�������Ϣ�������ڴ˾�����Щ�ӽ�������
;	set_sys_msg_mode 3 0 0
;	svr_msg_info	
;endproc
;;��boss
;proc 311
;	NEW_MAP_MON 200030 1 ::map12:: ::map12x:: ::map12y:: 0 0 0 0 0 1 0
;endproc
;;�жϵ�ͼ�Ϲ��������
;proc 112
;	strvar num
;	var num 0
;	GET_MAP_MON_NUM ::map13:: 200030 1 num 0
;	SET_EVENT_VAR 0 32 @num 1 
;endproc
;;��ʾ
;proc 212
;	add_sys_msg_start
;	add_sys_msg ::map13name::ͻȻ��������Ԩ������˹�������Ϣ�������ڴ˾�����Щ�ӽ�������
;	set_sys_msg_mode 3 0 0
;	svr_msg_info	
;endproc
;;��boss
;proc 312
;	NEW_MAP_MON 200030 1 ::map13:: ::map13x:: ::map13y:: 0 0 0 0 0 1 0
;endproc
;;�жϵ�ͼ�Ϲ��������
;proc 113
;	strvar num
;	var num 0
;	GET_MAP_MON_NUM ::map14:: 200030 1 num 0
;	SET_EVENT_VAR 0 32 @num 1 
;endproc
;;��ʾ
;proc 213
;	add_sys_msg_start
;	add_sys_msg ::map14name::ͻȻ��������Ԩ������˹�������Ϣ�������ڴ˾�����Щ�ӽ�������
;	set_sys_msg_mode 3 0 0
;	svr_msg_info	
;endproc
;;��boss
;proc 313
;	NEW_MAP_MON 200030 1 ::map14:: ::map14x:: ::map14y:: 0 0 0 0 0 1 0
;endproc




