proc 1
	strvar num
	GET_RONGLU_TASK_NUM 10 140 num
	dialog 1
	add_talk $_npc$:
	add_talk #b#b����һ���۱��裬�����ʱ��������ڵ��������ջ��˺ö�ö���ɫװ����
	; add_talk #b#b����һ���۱��裬�����ʱ��������ڵ��������ջ��˺ö�ö�ϡ�б�ʯ��
	; add_talk #b#b����һ���۱��裬�����ʱ��������ڵ��������ջ��˺ö�ö������ϡ�
	add_talk #b#bŶ�����ԣ�������һ��#cFF0080ħ����¯#n����

	if 10 = 0 
	;	add_talk #b#b#c186506����-��¯�ر�����ÿ������6��
	;	add_talk #b#b#c186506ӵ����������Ȩ�Ľ�ɫ�ɶ������3��
	;	add_talk #b#b
		if num <= 6
			add_talk #b#b#c186506<��ǰ�����$@num$/6������>
			add_talk #b#b<�����¶������0/3������>
			
		else
		
			add_talk #b#b#c186506<��ǰ�����6/6������>
			sub num 6
			add_talk #b#b<�����¶������$@num$/3������>
		endif
		
		add_talk #b#b<������ӽ�������>
		
	endif

		;;;;;;;;;;;;
		task_add_option	3250 1 103
		task_add_option	3251 1 104
		task_add_option	3252 1 105
		task_add_option	3253 1 106
		task_add_option	3254 1 107
		
		
		option 150 #������¯Ʒ��
		
		task_stat 3250 1 20
		task_stat 3251 1 21
		task_stat 3252 1 22
		task_stat 3253 1 23
		task_stat 3254 1 24
		; strvar num1 num2 num3 num4 num5 
		; GET_ITEM_NUM_INPAK 5052316 1 num1
		; GET_ITEM_NUM_INPAK 5052317 1 num2
		; GET_ITEM_NUM_INPAK 5052318 1 num3
		; GET_ITEM_NUM_INPAK 5052319 1 num4
		; GET_ITEM_NUM_INPAK 5052328 1 num5	
		
		; if 20 = 2
			; option 250 #��ȡ����¯��ӵ��ƾ֤$@mum1$/10��
		; elseif 21 = 2
			; option 250 #��ȡ����¯��ӵ��ƾ֤$@mum2$/10��
		
		; elseif 22 = 2
			; option 250 #��ȡ����¯��ӵ��ƾ֤$@mum3$/10��
		
		; elseif 23 = 2
			; option 250 #��ȡ����¯��ӵ��ƾ֤$@mum4$/10��
		
		; elseif 24 = 2
			; option 250 #��ȡ����¯��ӵ��ƾ֤$@mum5$/10��
		; else
			option 250 #�һ�����¯
	;	endif
		
		
		option 260 #�һ�����¯ƾ֤
		
		if 20 = 2 or 21 = 2 or 22 = 2 or 23 = 2 or 24 = 2
			option 200 ʱ�մ���
		endif
	talk
	
	
endproc 



;;;;;;;;;;;;;;;;;
PROC 103
	task_do_option 3250 1
ENDPROC
PROC 104
	task_do_option 3251 1
ENDPROC
PROC 105
	task_do_option 3252 1
ENDPROC
PROC 106
	task_do_option 3253 1
ENDPROC
PROC 107
	task_do_option 3254 1
ENDPROC

proc 250
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ѣ�Ѱ����·�Ǻ������ģ����ó������Ŭ������ʵ�����м�������¯�����������࣬�������һЩ������¯ƾ֤���ҵ����ܽ������һ�á���
	add_talk #b#b
		task_stat 3250 1 20
		task_stat 3251 1 21
		task_stat 3252 1 22
		task_stat 3253 1 23
		task_stat 3254 1 24
		
		strvar num1 num2 num3 num4 num5 a
		GET_ITEM_NUM_INPAK 5052316 1 num1
		GET_ITEM_NUM_INPAK 5052317 1 num2
		GET_ITEM_NUM_INPAK 5052318 1 num3
		GET_ITEM_NUM_INPAK 5052319 1 num4
		GET_ITEM_NUM_INPAK 5052328 1 num5	
	if 20 = 2
		add_talk #b#b��ǰ���ȡ������Ϊ��#c186506 40������-��¯�ر�����#n
		add_talk #b#b��ȡ40~49����������-����¯��Ҫ#c186506 10 #n����40������¯ƾ֤��
		add_talk #b#b
		add_talk #b#b��������#c186506 $@num1$ #n����40������¯ƾ֤��
		var a @num1
		dev @a 10
		add_talk #b#b��ǰ���Ի�ȡ#c186506 $@a$ #n������¯
	endif
	if 21 = 2
		add_talk #b#b��ǰ���ȡ������Ϊ��#c186506 50������-��¯�ر�����#n
		add_talk #b#b��ȡ50~59����������-����¯��Ҫ#c186506 20 #n����50������¯ƾ֤��
		add_talk #b#b
		add_talk #b#b��������#c186506 $@num2$ #n����50������¯ƾ֤��
		var a @num2
		dev @a 20
		add_talk #b#b��ǰ���Ի�ȡ#c186506 $@a$ #n������¯
		
	endif
	if 22 = 2
		add_talk #b#b��ǰ���ȡ������Ϊ��#c186506 60������-��¯�ر�����#n
		add_talk #b#b��ȡ60~69����������-����¯��Ҫ#c186506 30 #n����60������¯ƾ֤��
		add_talk #b#b
		add_talk #b#b��������#c186506 $@num3$ #n����60������¯ƾ֤��
		var a @num3
		dev @a 30
		add_talk #b#b��ǰ���Ի�ȡ#c186506 $@a$ #n������¯
	endif
	if 23 = 2
		add_talk #b#b��ǰ���ȡ������Ϊ��#c186506 70������-��¯�ر�����#n
		add_talk #b#b��ȡ70~79����������-����¯��Ҫ#c186506 40 #n����70������¯ƾ֤��
		add_talk #b#b
		add_talk #b#b��������#c186506 $@num4$ #n����70������¯ƾ֤��
		var a @num4
		dev @a 40
		add_talk #b#b��ǰ���Ի�ȡ#c186506 $@a$ #n������¯
	endif
	if 24 = 2
		add_talk #b#b��ǰ���ȡ������Ϊ��#c186506 80������-��¯�ر�����#n
		add_talk #b#b��ȡ80�����ϱ�������-����¯��Ҫ#c186506 50 #n����80������¯ƾ֤��
		add_talk #b#b
		add_talk #b#b��������#c186506 $@num5$ #n����80������¯ƾ֤��
		var a @num5
		dev @a 50
		add_talk #b#b��ǰ���Ի�ȡ#c186506 $@a$ #n������¯
	endif
		
		option 255 #ȷ����ȡ����¯
		option 1 ����
	talk
endproc
proc 255

		task_stat 3250 1 20
		task_stat 3251 1 21
		task_stat 3252 1 22
		task_stat 3253 1 23
		task_stat 3254 1 24
		if 20 = 1 or 21 = 1 or 22 = 1 or 23 = 1 or 24 = 1
			
			ADD_SYS_MSG_START
			ADD_SYS_MSG ���Ѿ�����˱���-��¯�ر������޷���ȡ����¯
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
			return
		endif
		strvar num1 num2 num3 num4 num5
		GET_ITEM_NUM_INPAK 5052316 1 num1
		GET_ITEM_NUM_INPAK 5052317 1 num2
		GET_ITEM_NUM_INPAK 5052318 1 num3
		GET_ITEM_NUM_INPAK 5052319 1 num4
		GET_ITEM_NUM_INPAK 5052328 1 num5	
		if 20 = 2 

			; GET_PLAYER_BASE_ATT 1 1 30 1 
			; if 30 >= 40 and 30 < 50
			
			if num1 >= 10
				ronglu_chg_gold 15 5052315
				if 15 = 0
					DEC_ITEM_NUM_INPAK 5052316 10 20
				else
				;	call 257
				endif
			else
				call 256
			endif
		elseif 21 = 2
			if num2 >= 20
				ronglu_chg_gold 15 5052315
				if 15 = 0
					DEC_ITEM_NUM_INPAK 5052317 20 20
				else
				;	call 257
				endif
			else
				call 256
			endif
		elseif 22 = 2
			;if 30 >= 60 and 30 < 70
			if num3 >= 30
				ronglu_chg_gold 15 5052315
				if 15 = 0
					DEC_ITEM_NUM_INPAK 5052318 30 20
				else
				;	call 257
				endif
			else
				call 256
			endif
		elseif 23 = 2
			if num4 >= 40
				ronglu_chg_gold 15 5052315
				if 15 = 0
					DEC_ITEM_NUM_INPAK 5052319 40 20
				else
				;	call 257
				endif
			else
				call 256
			endif
		
		elseif 24 = 2
			if num5 >= 50
				ronglu_chg_gold 15 5052315
				if 15 = 0
					DEC_ITEM_NUM_INPAK 5052328 50 20
				else
				;	call 257
				endif
			else
				call 256
			endif
		else
			ADD_SYS_MSG_START
			ADD_SYS_MSG �㻹û�н�ȡ����-��¯�ر�����
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		endif
endproc
proc 256
	ADD_SYS_MSG_START
	ADD_SYS_MSG ������ȼ�ƥ��Ľ���¯ƾ֤�������޷��һ�����¯
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc 
proc 257
	ADD_SYS_MSG_START
	ADD_SYS_MSG ������û�п��Ի�ȡ����¯
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc 


proc 260
	dialog 1
;	add_talk $_npc$:
;	add_talk #b#b
	add_talk #cFF0080�һ�����#n
	add_talk #b#b
	add_talk #c186506 5��#n40������¯ƾ֤#cFF0080�һ�#n#c186506 1��#n 50������¯ƾ֤
	add_talk #c186506 5��#n50������¯ƾ֤#cFF0080�һ�#n#c186506 1��#n 60������¯ƾ֤
	add_talk #c186506 5��#n60������¯ƾ֤#cFF0080�һ�#n#c186506 1��#n 70������¯ƾ֤
	add_talk #c186506 5��#n70������¯ƾ֤#cFF0080�һ�#n#c186506 1��#n 80������¯ƾ֤
;	add_talk #c186506 5��#n50������¯ƾ֤#cFF0080�һ�#n#c186506 1��#n 60������¯ƾ֤
	
	
	
		option 261 �һ�50������¯ƾ֤
		option 262 �һ�60������¯ƾ֤
		option 263 �һ�70������¯ƾ֤
		option 264 �һ�80������¯ƾ֤
		option 1 ����
		
	talk
endproc

proc 261
	strvar num1 num2 num3 num4 num5
	GET_ITEM_NUM_INPAK 5052316 1 num1
	if num1 >= 5
		
		DEC_ITEM_NUM_INPAK 5052316 5 20
		ADD_BIND_ITEM_NUM_INPAK 5052317 1 1 0
		
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG �һ�����Ҫ��40������¯ƾ֤����5�ţ��һ�ʧ��
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc
proc 262

	GET_ITEM_NUM_INPAK 5052317 1 num2
	if num2 >= 5
	
		DEC_ITEM_NUM_INPAK 5052317 5 20
		ADD_BIND_ITEM_NUM_INPAK 5052318 1 1 0
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG �һ�����Ҫ��50������¯ƾ֤����5�ţ��һ�ʧ��
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	
	endif

endproc
proc 263
	GET_ITEM_NUM_INPAK 5052318 1 num3
	if num3 >= 5
	
		DEC_ITEM_NUM_INPAK 5052318 5 20
		ADD_BIND_ITEM_NUM_INPAK 5052319 1 1 0
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG �һ�����Ҫ��60������¯ƾ֤����5�ţ��һ�ʧ��
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc 
proc 264

	GET_ITEM_NUM_INPAK 5052319 1 num4
	if num4 >= 5
	
		DEC_ITEM_NUM_INPAK 5052319 5 20
		ADD_BIND_ITEM_NUM_INPAK 5052328 1 1 0
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG �һ�����Ҫ��70������¯ƾ֤����5�ţ��һ�ʧ��
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	
	endif
endproc


proc 200
	dialog 1
	add_talk $_npc$:
	
	add_talk #b#b�ҿ��԰����͵������ͼ�������ҿ�Ҫ��ȡһ��������Ŷ��
	add_talk #b#b��������һ�ֲ��û�Ǯ�ķ��������Դ��Լ�������еĹ�����Ŧ���Ĺ��׶ȴ��͵������ͼ
;	add_talk ���ͻ��ѣ����������Զ��������ң���
;	add_talk #b#b#c186506���ջ�Ұ��250������
;	add_talk #b#bԽ�����֣�350������
;	add_talk #b#b���ֵأ�500������
;	add_talk #b#b����ѹȣ�800������
;	add_talk #b#b��ɳ�ݵأ�810������#n
	add_talk #b#b
	add_talk #c0181f9�㵱ǰ������ص�Ϊ��
	task_stat 3250 1 20
	if 20 = 2
		add_talk #b#b���ջ�Ұ����Ҫ����#c186506 250 #n#c0181f9����
		option 300 #��Ҫȥɱа�����ڱ�	
		option 301 #��Ҫȥɱ��Ӹ�Ⱦ��
	endif
	task_stat 3251 1 30
	if 30 = 2
		add_talk #b#bԽ�����֣���Ҫ����#c186506 350 #n#c0181f9����
		option 310 #��Ҫȥɱ�꼪սʿ
		option 311 #��Ҫȥɱ�����Ա�
	endif
	task_stat 3252 1 40
	if 40 = 2
		add_talk #b#b���ֵأ���Ҫ����#c186506 500 #n#c0181f9����
		option 320 #��Ҫȥɱ��³��ħ��
		option 321 #��Ҫȥɱ����ʿ��
	endif
	task_stat 3253 1 50
	if 50 = 2
		add_talk #b#b����ѹȣ���Ҫ����#c186506 800 #n#c0181f9����
		option 330 #��Ҫȥɱ�����
		option 331 #��Ҫȥɱ��˪�ƻ���
	endif
	task_stat 3254 1 60
	if 60 = 2
		add_talk #b#b������ԭ����Ҫ����#c186506 3000 #n#c0181f9����
		option 340 #��Ҫȥɱ����Ⱦ�Ĵ���
		option 341 #��Ҫȥɱ����Ⱦ�Ŀ�
	endif
	
		option 1 ����
		
	talk
endproc 

proc 300
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 250 and @ret >= 250
		choose 1 210 221 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 250
		choose 1 210 221 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 210
	endif
endproc 
proc 301
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 250 and @ret >= 250
		choose 1 220 221 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 250
		choose 1 220 221 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 220
	endif
endproc 
;;;;;;;;;;;;;;
proc 310
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 350 and @ret >= 350
		choose 1 210 221 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 350
		choose 1 210 221 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 210
	endif
endproc 
proc 311
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 350 and @ret >= 350
		choose 1 220 221 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 350
		choose 1 220 221 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 220
	endif
endproc 
;;;;;;;;;;;;;;
proc 320
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 500 and @ret >= 500
		choose 1 210 221 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 500
		choose 1 210 221 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 210
	endif
endproc 
proc 321
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 500 and @ret >= 500
		choose 1 220 221 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 500
		choose 1 220 221 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 220
	endif
endproc 
;;;;;;;;;;;;;;
proc 330
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 800 and @ret >= 800
		choose 1 210 221 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 800
		choose 1 210 221 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 210
	endif
endproc 
proc 331
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 800 and @ret >= 800
		choose 1 220 221 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 800
		choose 1 220 221 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 220
	endif
endproc 
;;;;;;;;;;;;;;
proc 340
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 3000 and @ret >= 3000
		choose 1 210 221 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 3000
		choose 1 210 221 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 210
	endif
endproc 
proc 341
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 3000 and @ret >= 3000
		choose 1 220 221 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 3000
		choose 1 220 221 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 220
	endif
endproc 
;;;;;;;;;;;;;;


proc 221
endproc
proc 210
	strvar Moy
	task_stat 3250 1 20
	if 20 = 2
		DEC_MONEY_TEST 0 250 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 250 21 10
			if 10 = 0
				TRANSMIT 2400 6100 15600 1
			endif
		endif
		
	endif
	task_stat 3251 1 30
	if 30 = 2
		DEC_MONEY_TEST 0 350 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 350 21 10
			if 10 = 0
				TRANSMIT 2500 13200 28200 1 
			endif
		endif
	endif
	task_stat 3252 1 40
	if 40 = 2
		DEC_MONEY_TEST 0 500 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 500 21 10
			if 10 = 0
				TRANSMIT 2104 18700 14400 1 
			endif
		endif
	endif
	task_stat 3253 1 50
	if 50 = 2
		DEC_MONEY_TEST 0 800 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 800 21 10
			if 10 = 0
				TRANSMIT 2201 9700 9000 1 
			endif
		endif
	endif
	task_stat 3254 1 60
	if 60 = 2
		DEC_MONEY_TEST 0 3000 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 3000 21 10
			if 10 = 0
				TRANSMIT 3402 15900 21300 1
			endif
		endif
	endif
endproc  


proc 220
	strvar Moy
	task_stat 3250 1 20
	if 20 = 2
		DEC_MONEY_TEST 0 250 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 250 21 10
			if 10 = 0
				TRANSMIT 2400 16900 16600 1
			endif
		endif
		
	endif
	task_stat 3251 1 30
	if 30 = 2
		DEC_MONEY_TEST 0 350 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 350 21 10
			if 10 = 0
				TRANSMIT 2500 28100 30000 1 
			endif
		endif
	endif
	task_stat 3252 1 40
	if 40 = 2
		DEC_MONEY_TEST 0 500 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 500 21 10
			if 10 = 0
				TRANSMIT 2104 23600 11200 1 
			endif
		endif
	endif
	task_stat 3253 1 50
	if 50 = 2
		DEC_MONEY_TEST 0 800 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 800 21 10
			if 10 = 0
				TRANSMIT 2201 23100 33100 1 
			endif
		endif
	endif
	task_stat 3254 1 60
	if 60 = 2
		DEC_MONEY_TEST 0 3000 21 Moy
		if Moy = -1
			ADD_SYS_MSG_START
			ADD_SYS_MSG ������Ҳ��㣬�޷����ͣ�
			SET_SYS_MSG_MODE 4 0 0
			MSG_INFO
		else
			DEC_MONEY 0 3000 21 10
			if 10 = 0
				TRANSMIT 3402 28000 6800 1
			endif
		endif
	endif
endproc  


proc 2
	get_trig_task 1 2
	if 2 = 3250 
		call 1
	endif
	if 2 = 3251
		call 1
	endif
	if 2 = 3252
		call 1
	endif
	if 2 = 3253
		call 1
	endif
	if 2 = 3254
		call 1
	endif
	
endproc

proc 150
	open_ui 75
	
endproc 








