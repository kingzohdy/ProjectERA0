;302033

	
proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ǵ��д��ڿ�ɱ�Ƕ��������ָ����ˣ�Ϊ�˺�����������ҫ����ѡ���Ŵ��磡
	add_talk #b#b���������������ε��Ƶ���������ڰѶ�����Զ��̤�ڽ��£�
	add_talk #b#b����Ӵ�����İ����������ѵĿ��..
	add_talk #b#b���ڱ���¡���Ƴ���#cff4e00�Ծɻ���#n������û����û�����������˶�������ʵ���ֻ���ġ�#cff4e00�Ծɻ���#n�����񣡣�

	task_add_option 1601 1 101
	task_add_option 1602 1 102
	
	task_add_option 1701 1 103
	task_add_option 1702 1 104
	task_add_option 1700 1 105
	
	task_add_option 1749 1 106
	task_add_option 1600 1 107
		
	; OPTION 110 #(����)���̵� 
	task_stat 1749 1 20
	if 20 = 2
		option 150 ��Ҫ����ѽ��
	endif
	
	strvar LV
	GET_PLAYER_BASE_ATT 1 1 LV 1
	
	if @LV < 50
		option 401 #(����)��ս���������̵�
	endif
	if @LV >= 50 and @LV < 60
		option 402 #(����)��ս���������̵�
	endif
	if @LV >= 60 and @LV < 70
		option 403 #(����)��ս���������̵�
	endif
	if @LV >= 70 and @LV < 80
		option 404 #(����)��ս���������̵�
	endif
	if @LV >= 80
		option 405 #(����)��ս���������̵�
	endif
	
	option 240 #�Ծɻ���-�һ�����
	
	;option 200 �Ծɻ��¡�#c6d00d2׿Խ�� ��ҹ��Ӱ#n��
	; option 200 �Ծɻ���-׿Խ����ҹ��Ӱ������
	; option 220 �Ծɻ���-ʷʫ����ҹ��Ӱ������
	;option 210 �Ծɻ��¡�#c6d00d2׿Խ�� �������#n��
	; option 210 �Ծɻ���-׿Խ��������١�����
	; option 230 �Ծɻ���-׿Խ��������١�����
	
	talk
endproc

PROC 240
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����һ��ĸ��ȼ������Σ�
	
	option 241 #�Ծɻ���-�һ���64��������
	option 242 #�Ծɻ���-�һ���74��������
	talk
ENDPROC

PROC 241
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����һ����֡�64���������Σ�
	
	option 203 #�Ծɻ���-׿Խ����ҹ��Ӱ������
	option 223 #�Ծɻ���-ʷʫ�������Ӱ������
	talk
ENDPROC

PROC 242
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����һ����֡�74���������Σ�
	
	option 213 #�Ծɻ���-׿Խ��������١�����
	option 233 #�Ծɻ���-ʷʫ���ξ����١�����
	talk
ENDPROC

PROC 101
	task_do_option 1601 1
ENDPROC
PROC 102
	task_do_option 1602 1
ENDPROC
PROC 103
	task_do_option 1701 1
ENDPROC
PROC 104
	task_do_option 1702 1
ENDPROC
PROC 105
	task_do_option 1700 1
ENDPROC
PROC 106
	task_do_option 1749 1
ENDPROC
PROC 107
	task_do_option 1600 1
ENDPROC

proc 110
	 OPEN_NPC_SHOP
endproc

proc 2
	get_trig_task 1 2
	if 2 = 1602
		task_fini 1602 1
	endif
	if 2 = 1702
		task_fini 1702 1
	endif
	
	if 2 = 1749
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b�ͻ���װ�򣬺ñȽ���������,��ǿ����ʿҲ�벻��������װ����
			
			option 150 ��Ҫ����ѽ��
		talk
	endif
endproc
proc 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bΪ��֤����������������˵�����Ҿ�������С��Ŀ���鿼���㣬��������ڸо�����̫�˽�ǿ����ת�ǵĹ��̣���ȥ�˶����Ƕ��о��о������ɣ�
		
		option 151 ��׼�����ˣ������ʰɣ�
				
	talk
endproc 
proc 151
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bǿ��װ��������10%���Ե���ɫ���ǽ�ʲô��

		option 152 A��������
		option 152 B���ϼ���
		option 153 C��������
		option 152 D��ţX��

				
	talk
endproc 
proc 152
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c186506װ��ǿ��ÿ���ǻ�����1%-10%�Ļ������ԣ�����������ֵ10%ʱ��ǿ��������Ǿͻ�����ɫ���׳������ǡ�#n
		
		option 151 ���»ش�
	
	talk
endproc 
proc 153
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ǿ��ʯ���ܸ�װ�����ǵĲ�����ʲô��

		option 161 A������

		option 160 B��ת��
		option 161 C����
		option 161 D������
				
	talk
endproc 
proc 161
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c186506����ӵ��һ��ǿ������װ��ʱ��������ѽ���ת�Ƶ���һ����ͬ�Ĳ����ϣ�������̽�����ת�ǡ���#n
	add_talk #b#b#c186506����˵����ӵ��һ��+9��41������������԰���ת�Ƶ�û�м��ǵ�51����61����71�������ϡ�ת�Ƴɹ�����ĸ߼������ͻ���+9�����Ҳ���Ҫ�κ����ģ�#n
	add_talk #b#b#c186506ע��ֻ����ͬ������װ�����ſ��Խ���ת��Ӵ��#n

		option 151 ���»ش�
	
	talk
endproc

proc 160
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bת��ʱ��Ӧ����ô�żӹ��ǵ�װ�����Լ��߼�װ������ת��װ�����أ�
	
		option 170 A����߷�ǿ�����ģ��ұ߷Ŵ�ת�ǵ�
		option 171 B����߷Ŵ�ת�ǵģ��ұ߷�ǿ������
		option 171 C����߷�ǿ�����ģ��ұ߷��ǻԱ���
		option 171 D����߷Ŵ�ת�ǵģ��ұ߷��ǻԱ���
				
	talk
endproc 
proc 171
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c186506��ߵ�װ���Ḳ�ǵ��ұ�װ���ϵ��Ǽ������ԣ�ת�ǵ�ʱ��һ��Ҫ�����ģ�#n

		option 151 ���»ش�
	
	talk
endproc 

proc 170
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bȥ�Ļ�ÿ���ת�ǵ�װ���أ�
	
		option 181 A����������װ����ֱ�Ӽ��ǵ�
		option 181 B����������ҵ�̯λ�Ϲ���
		option 181 C���Լ����ǻԱ���ǿ��
		option 180 #D������˵������
				
	talk

endproc 
proc 181
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c186506����2��Ҳ��Ӵ��������в��󶨵������ǵ�װ������������ȥ������Ӵ�������һ��׬Ǯ�ĺ�;���أ�#n

		option 151 ���»ش�
	
	talk

endproc 

proc 180
	task_fini 1749 1
endproc 

proc 200
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b��������
;	add_talk �ﳬֵ������ 
	add_talk �ɻ��#c186506ȫ��#n��#c6d00d2׿Խ��#n����ҹ��Ӱ�����Σ�
	add_talk �����ν��м��ʻ�ø��õġ�#cff4e00��������#n����
	; add_talk #b#b
	add_talk �ﻻ������� 
	strvar num
	GET_ITEM_NUM_INPAK 2455007 1 num
	if num = 0
		add_talk ���Σ�׿Խ�ġ���ҹ��Ӱ�� #cF80B0B 0 #n/1
	else
		add_talk ���Σ�׿Խ�ġ���ҹ��Ӱ�� #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 500
		add_talk ���ң�������������#cF80B0B $@50$ #n/500
	else
		add_talk ���ң�������������#c186506 $@50$ #n/500			
	endif
;	add_talk #b#b
	add_talk �ﻻ����֪�� 
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ǿ���Ǽ�#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ϴ������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��ħ����#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��Ƕ��ʯ#n��
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b����÷��������Ʒ

		option 203 #ͬ�⻻��
		; option 210 #ȷ���һ��������
	
	talk
endproc
proc 203

	dialog 3
	add_talk #b#b#b#b#b#b#b#b#b��������
	add_talk �ɻ��#c186506ȫ��#n��#c6d00d2׿Խ��#n����ҹ��Ӱ�����Σ�
	add_talk �����ν��м��ʻ�ø��õġ�#cff4e00��������#n����
	add_talk �ﻻ������� 
	strvar num
	GET_ITEM_NUM_INPAK 2455007 1 num
	if num = 0
		add_talk ���Σ�׿Խ�ġ���ҹ��Ӱ�� #cF80B0B 0 #n/1
	else
		add_talk ���Σ�׿Խ�ġ���ҹ��Ӱ�� #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 500
		add_talk ���ң�������������#cF80B0B $@50$ #n/500
	else
		add_talk ���ң�������������#c186506 $@50$ #n/500			
	endif
;	add_talk #b#b
	add_talk �ﻻ����֪�� 
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ǿ���Ǽ�#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ϴ������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��ħ����#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��Ƕ��ʯ#n��
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b����÷��������Ʒ	
	add_talk #b#c186506�������Ҫ���������Σ�׿Խ�ġ���ҹ��Ӱ��#n
                                                    
		NPC_COMMIT_OPT 204
	
	talk
endproc 

proc 204
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg �������Ҫ������׿Խ�ġ���ҹ��Ӱ������
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	if m_ID = 2455007
	
		strvar Moy
		DEC_MONEY_TEST 0 500 76 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ��������������㣬�޷�����
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			return
		else
			DEC_MONEY 0 500 76 10
		;	DEC_ITEM_NUM_BYIDX 2455007 1 @m_Pos1 @m_Pos2 RetTask
			COMMIT_ARM_WASH_ADDATTR 10
			
			PLAYER_CLOSE_DIALOG 0 0
			ADD_SYS_MSG_START
			ADD_SYS_MSG ��ɹ�������׿Խ�ġ���ҹ��Ӱ�����Σ�
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			call 203
			; add_sys_msg_start
			; add_sys_msg $@m_ID$ $@10$
			; set_sys_msg_mode 1 0 0
			; msg_info
		endif
		
	else
		add_sys_msg_start
		add_sys_msg �������Ҫ������׿Խ�ġ���ҹ��Ӱ������
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc

; proc 201 
	; choose 2 202 301 ������ֻ���þ����εĸ������ԣ��Ƿ�ȷ�ϻ�����
; endproc 
proc 301
endproc
; proc 202
	; PKG_IS_UNLOCK 20 30
	; if 20 = 0 and 30 = 0
		; return
	; endif
	; strvar num
	; GET_ITEM_NUM_INPAK 2455007 1 num
	; if num > 0
		; strvar Moy
		; DEC_MONEY_TEST 0 1000 76 Moy
		; if Moy = -1
			; ADD_SYS_MSG_START
			; ADD_SYS_MSG ���������㲻�㣬�޷��һ�
			; SET_SYS_MSG_MODE 1 0 0
			; MSG_INFO
			; return
		; else
			; DEC_MONEY 0 1000 76 10
			
			; dec_item_num_inpak 2455007 1 0
			; ADD_BIND_ITEM_NUM_INPAK 2455007 1 1 0
		; endif
	; else
		; ADD_SYS_MSG_START
		; ADD_SYS_MSG ���ı�����û����ҹ��Ӱ���޷��һ�
		; SET_SYS_MSG_MODE 1 0 0
		; MSG_INFO
	; endif
; endproc 



proc 210
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b��������
;	add_talk �ﳬֵ������ 
	add_talk �ɻ��#c186506ȫ��#n��#c6d00d2׿Խ��#n��������١����Σ�
	add_talk �����ν��м��ʻ�ø��õġ�#cff4e00��������#n����
	; add_talk #b#b
	add_talk �ﻻ������� 
	strvar num
	GET_ITEM_NUM_INPAK 2455008 1 num
	if num = 0
		add_talk ���Σ�׿Խ�ġ�������١� #cF80B0B 0 #n/1
	else
		add_talk ���Σ�׿Խ�ġ�������١� #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 2000
		add_talk ���ң�������������#cF80B0B $@50$ #n/2000
	else
		add_talk ���ң�������������#c186506 $@50$ #n/2000			
	endif
;	add_talk #b#b
	add_talk �ﻻ����֪�� 
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ǿ���Ǽ�#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ϴ������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��ħ����#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��Ƕ��ʯ#n��
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b����÷��������Ʒ
	
	;	option 201 #ȷ���һ���ҹ��Ӱ
		option 213 #ͬ�⻻��
	
	talk
endproc
; proc 211 


	; choose 2 212 301 ������ֻ���þ����εĸ������ԣ��Ƿ�ȷ�ϻ�����
; endproc 

; proc 212

	; PKG_IS_UNLOCK 20 30
	; if 20 = 0 and 30 = 0
		; return
	; endif

	; strvar num
	; GET_ITEM_NUM_INPAK 2455008 1 num
	; if num > 0
		; strvar Moy
		; DEC_MONEY_TEST 0 3000 76 Moy
		; if Moy = -1
			; ADD_SYS_MSG_START
			; ADD_SYS_MSG ���������㲻�㣬�޷��һ�
			; SET_SYS_MSG_MODE 1 0 0
			; MSG_INFO
			; return
		; else
			; DEC_MONEY 0 3000 76 10
			; if 10 = 0
				
				; dec_item_num_inpak 2455008 1 0
				; ADD_BIND_ITEM_NUM_INPAK 2455008 1 1 0
			; endif
		; endif
	; else
		; ADD_SYS_MSG_START
		; ADD_SYS_MSG ���ı�����û�и�����٣��޷��һ�
		; SET_SYS_MSG_MODE 1 0 0
		; MSG_INFO
	
	; endif
; endproc 

proc 213
	dialog 3
	add_talk #b#b#b#b#b#b#b#b#b��������
	add_talk �ɻ��#c186506ȫ��#n��#c6d00d2׿Խ��#n��������١����Σ�
	add_talk �����ν��м��ʻ�ø��õġ�#cff4e00��������#n����
	add_talk �ﻻ������� 
	strvar num
	GET_ITEM_NUM_INPAK 2455008 1 num
	if num = 0
		add_talk ���Σ�׿Խ�ġ�������١� #cF80B0B 0 #n/1
	else
		add_talk ���Σ�׿Խ�ġ�������١� #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 2000
		add_talk ���ң�������������#cF80B0B $@50$ #n/2000
	else
		add_talk ���ң�������������#c186506 $@50$ #n/2000			
	endif
;	add_talk #b#b
	add_talk �ﻻ����֪�� 
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ǿ���Ǽ�#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ϴ������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��ħ����#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��Ƕ��ʯ#n��
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b����÷��������Ʒ
	add_talk #b#c186506�������Ҫ�������Σ�׿Խ�ġ�������١�#n
                                                    
		NPC_COMMIT_OPT 214
	
	talk
endproc

proc 214
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg �������Ҫ������׿Խ�ġ�������١�����
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	if m_ID = 2455008
	
		strvar Moy
		DEC_MONEY_TEST 0 2000 76 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ��������������㣬�޷�����
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			return
		else
			DEC_MONEY 0 2000 76 10
		;	DEC_ITEM_NUM_BYIDX 2455007 1 @m_Pos1 @m_Pos2 RetTask
			COMMIT_ARM_WASH_ADDATTR 1
			PLAYER_CLOSE_DIALOG 0 0
			
			PLAYER_CLOSE_DIALOG 0 0
			ADD_SYS_MSG_START
			ADD_SYS_MSG ��ɹ�������׿Խ�ġ�������١����Σ�
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			call 213
		endif
	else
		add_sys_msg_start
		add_sys_msg �������Ҫ������׿Խ�ġ�������١�����
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc

proc 401
	OPEN_SHOP_BY_ID 10020
	
	task_fini 2267 1
endproc
proc 402
	OPEN_SHOP_BY_ID 10021
	
	task_fini 2267 1
endproc
proc 403
	OPEN_SHOP_BY_ID 10022
	
	task_fini 2267 1
endproc
proc 404
	OPEN_SHOP_BY_ID 10023
	
	task_fini 2267 1
endproc
proc 405
	OPEN_SHOP_BY_ID 10024
	
	task_fini 2267 1
endproc

proc 220
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b�������� 
	add_talk �ɻ��#c186506ȫ��#n��#cff0080ʷʫ��#n�������Ӱ�����Σ�
	add_talk �����ν��м��ʻ�ø��õġ�#cff4e00��������#n����
	; add_talk #b#b
	add_talk �ﻻ������� 
	strvar num
	GET_ITEM_NUM_INPAK 2411603 1 num
	if num = 0
		add_talk ���Σ�ʷʫ�ġ������Ӱ�� #cF80B0B 0 #n/1
	else
		add_talk ���Σ�ʷʫ�ġ������Ӱ�� #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 500
		add_talk ���ң�������������#cF80B0B $@50$ #n/500
	else
		add_talk ���ң�������������#c186506 $@50$ #n/500			
	endif
	add_talk �ﻻ����֪�� 
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ǿ���Ǽ�#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ϴ������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��ħ����#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��Ƕ��ʯ#n��
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b����÷��������Ʒ

		option 223 #ͬ�⻻��
	
	talk
endproc
proc 223

	dialog 3
	add_talk #b#b#b#b#b#b#b#b#b�������� 
	add_talk �ɻ��#c186506ȫ��#n��#cff0080ʷʫ��#n�������Ӱ�����Σ�
	add_talk �����ν��м��ʻ�ø��õġ�#cff4e00��������#n����
	add_talk �ﻻ������� 
	strvar num
	GET_ITEM_NUM_INPAK 2411603 1 num
	if num = 0
		add_talk ���Σ�ʷʫ�ġ������Ӱ�� #cF80B0B 0 #n/1
	else
		add_talk ���Σ�ʷʫ�ġ������Ӱ�� #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 500
		add_talk ���ң�������������#cF80B0B $@50$ #n/500
	else
		add_talk ���ң�������������#c186506 $@50$ #n/500			
	endif
	add_talk �ﻻ����֪�� 
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ǿ���Ǽ�#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ϴ������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��ħ����#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��Ƕ��ʯ#n��
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b����÷��������Ʒ
	add_talk #b#c186506�������Ҫ���������Σ�ʷʫ�ġ������Ӱ��#n
                                                    
		NPC_COMMIT_OPT 224
	
	talk
endproc 

proc 224
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg �������Ҫ������ʷʫ�ġ������Ӱ�����Σ�
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	if m_ID = 2411603
	
		strvar Moy
		DEC_MONEY_TEST 0 500 76 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ��������������㣬�޷�����
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			return
		else
			DEC_MONEY 0 500 76 10
			COMMIT_ARM_WASH_ADDATTR 10
			PLAYER_CLOSE_DIALOG 0 0
			
			PLAYER_CLOSE_DIALOG 0 0
			ADD_SYS_MSG_START
			ADD_SYS_MSG ��ɹ�������ʷʫ�ġ������Ӱ�����Σ�
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			call 223
		endif
		
	else
		add_sys_msg_start
		add_sys_msg �������Ҫ������ʷʫ�ġ������Ӱ������
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc

proc 230
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b��������
	add_talk �ɻ��#c186506ȫ��#n��#cff0080ʷʫ��#n���ξ����١����Σ�
	add_talk �����ν��м��ʻ�ø��õġ�#cff4e00��������#n����
	; add_talk #b#b
	add_talk �ﻻ������� 
	strvar num
	GET_ITEM_NUM_INPAK 2411604 1 num
	if num = 0
		add_talk ���Σ�ʷʫ�ġ��ξ����١� #cF80B0B 0 #n/1
	else
		add_talk ���Σ�ʷʫ�ġ��ξ����١� #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 2000
		add_talk ���ң�������������#cF80B0B $@50$ #n/2000
	else
		add_talk ���ң�������������#c186506 $@50$ #n/2000			
	endif
	add_talk �ﻻ����֪�� 
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ǿ���Ǽ�#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ϴ������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��ħ����#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��Ƕ��ʯ#n��
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b����÷��������Ʒ
	
		option 233 #ͬ�⻻��
	
	talk
endproc

proc 233
	dialog 3
	add_talk #b#b#b#b#b#b#b#b#b��������
	add_talk �ɻ��#c186506ȫ��#n��#cff0080ʷʫ��#n���ξ����١����Σ�
	add_talk �����ν��м��ʻ�ø��õġ�#cff4e00��������#n����
	add_talk �ﻻ������� 
	strvar num
	GET_ITEM_NUM_INPAK 2411604 1 num
	if num = 0
		add_talk ���Σ�ʷʫ�ġ��ξ����١� #cF80B0B 0 #n/1
	else
		add_talk ���Σ�ʷʫ�ġ��ξ����١� #c186506 $@num$ #n/1
	endif
	GET_ID_PLAYER_INFO 0 23 50
	if 50 < 2000
		add_talk ���ң�������������#cF80B0B $@50$ #n/2000
	else
		add_talk ���ң�������������#c186506 $@50$ #n/2000			
	endif
	add_talk �ﻻ����֪�� 
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ǿ���Ǽ�#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00ϴ������#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��ħ����#n��
	add_talk �����ν�ȫ������װ���ġ�#cff4e00��Ƕ��ʯ#n��
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b����÷��������Ʒ
	add_talk #b#c186506�������Ҫ�������Σ�ʷʫ�ġ��ξ����١�#n
                                                    
		NPC_COMMIT_OPT 234
	
	talk
endproc

proc 234
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg �������Ҫ������ʷʫ�ġ��ξ����١�����
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	if m_ID = 2411604
	
		strvar Moy
		DEC_MONEY_TEST 0 2000 76 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ��������������㣬�޷�����
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			return
		else
			DEC_MONEY 0 2000 76 10
			COMMIT_ARM_WASH_ADDATTR 1
			PLAYER_CLOSE_DIALOG 0 0
			
			PLAYER_CLOSE_DIALOG 0 0
			ADD_SYS_MSG_START
			ADD_SYS_MSG ��ɹ�������ʷʫ�ġ��ξ����١����Σ�
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
			call 233
		endif
	else
		add_sys_msg_start
		add_sys_msg �������Ҫ������ʷʫ�ġ��ξ����١�����
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc
