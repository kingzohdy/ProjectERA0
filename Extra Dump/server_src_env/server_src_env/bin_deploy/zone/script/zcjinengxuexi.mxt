

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ҿ����������������ѪҺ�з��ڣ�

	task_add_option 2017 1 103
	task_add_option 2018 1 104
;	task_add_option 4204 1 105
	task_add_option 4205 1 106

	task_add_option 4224 1 109
	task_add_option 4301 1 112
;50��
	task_add_option 2201 1 114
	task_add_option 4400 1 115

	task_add_option 2015 1 119
	task_add_option 2016 1 120

;20~30�޸�
	task_add_option 4102 1 123
	task_add_option 1028 1 125
	task_add_option 1029 1 126
	task_add_option 4103 1 127
	task_add_option 4121 1 128
	task_add_option 4104 1 130
;;;;;;
	task_add_option 1101 1 131
	task_add_option 4322 1 132
	task_add_option 4425 1 133
	
;;;תְ
	task_add_option 3401 1 134
;;;;
	task_add_option 1077 1 135
	task_add_option 1081 1 136
;	task_add_option 3403 1 137
	
	
	
	option 12 #ѧϰ����
;	option 13 #���������Ƕ
;	option 14 #�������ժȡ
	option 200 #������ת��ְҵ���ư��ķ�������
;	
;	strvar LV
;	GET_PLAYER_ID 1 50
;	ID_PLAYER_BASE_ATT @50 1 1 LV 1 
;	if @LV >= 20
;		option 300 #�����츳
;	endif
	strvar Ret
	task_stat 1081 1 Ret	
	if Ret = 2
		
;		option 400 ʱ���̫���ˣ��ܷ�����һ�̣�
	endif
	
	
	strvar rum
	GET_ITEM_NUM_INPAK 5300965 1 rum
	task_stat 3025 1 Ret
	if Ret = 2 and rum = 0
		option 500 #�����溢�������ú����

	endif
	
	task_stat 1077 1 20
	if 20 = 2
		GET_MAP_NPC_ID 306000 40 1
		mon_speech @40 2 0 1 ��˵�㽫����������ִ��������Ȼ���ﲼ��Σ�գ�Ҳ�������ʱɥ����������Ҫ��ס���������ǵļ��磡
		
		task_fini 1077 0
	endif
	talk
endproc 

proc 12
	open_ui 1	
endproc
proc 13
	open_ui 25
endproc
proc 14
	open_ui 26
endproc

proc 500
	task_stat 3025 1 Ret
	if Ret = 2
		strvar rum
		GET_ITEM_NUM_INPAK 5300965 1 rum
		if rum = 0
			ADD_ITEM_NUM_INPAK 5300965 1 1 20
		endif
	endif
endproc 





proc 300
	strvar geni
	RESET_ALL_GENIUS geni
	if @geni = 0
		GET_PLAYER_ID 0 30
		effect_id 0 @30 playMontion(%d,"9554_16")
		add_sys_msg_start
		add_sys_msg �ɹ������츳��
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

PROC 103
	task_do_option 2017 1
ENDPROC
PROC 104
	task_do_option 2018 1
ENDPROC

PROC 106
	task_do_option 4205 1
ENDPROC
PROC 109
	task_do_option 4224 1
ENDPROC

PROC 112
	task_do_option 4301 1
ENDPROC

PROC 114
	task_do_option 2201 1
ENDPROC
PROC 115
	task_do_option 4400 1
ENDPROC

PROC 119
	task_do_option 2015 1
ENDPROC
PROC 120
	task_do_option 2016 1
ENDPROC
PROC 123
	task_do_option 4102 1
ENDPROC
PROC 125
	task_do_option 1028 1
ENDPROC
PROC 126
	task_do_option 1029 1
ENDPROC
PROC 127
	task_do_option 4103 1
ENDPROC
PROC 128
	task_do_option 4121 1
ENDPROC

PROC 130
	task_do_option 4104 1
ENDPROC
PROC 131
	task_do_option 1101 1
ENDPROC
PROC 132
	task_do_option 4322 1
ENDPROC
PROC 133
	task_do_option 4425 1
ENDPROC


PROC 134
	task_do_option 3401 1
ENDPROC
PROC 135
	task_do_option 1077 1
ENDPROC
PROC 136
	task_do_option 1081 1
ENDPROC
PROC 137
	task_do_option 3403 1
ENDPROC



proc 2
	get_trig_task 1 2
	if 2 = 1095
		task_fini 1095 1
	endif  

	if 2 = 1029
		task_fini 1029 1
	endif 

endproc

proc 400
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ȼ���ԣ������Ǹ��������ء�
	add_talk #b#b����������ֱ�ӽ����͵�Ŀ�ĵأ���;����ͣ��Ӵ��
	add_talk #b#b��ȷ��Ҫ����ֱ��೵��
		option 401 #��ֱ�ӽ����͵�Ŀ�ĵذɣ�
		option 402 #�����������Լ���ȥ�����ܹ��ʱ��ǣ�
	talk
endproc
proc 401
	
;	NEW_OWNER_TMP_ITEM 1460070 20900 17100 0 0 0 310 1095 1 3600 1
	AUTO_MOVE 0 1000 27000 24900 1460070 0 3081 1 3600
endproc 
proc 402
	return
endproc 

proc 200
	DIALOG 1
		add_talk #b#b#b#b#b#b#b#b#c186506��ת��׼����
		add_talk #b#b#b#b#b ȷ�������޴����κ�װ��#n
		add_talk #b#b#b#b#b#b#b#b#cF80B0B��ת��ע�⡷
		add_talk #b ת���󽫱����츳ֵ�����Զ��滻ְҵ�츳
		add_talk  ת�������ְҵ���ܣ����Զ�ϰ����ְҵ����
		add_talk #b#b ת���������ĩ���۷�������������#n
		add_talk #b#b
		add_talk #b#b#b#b#b#b#b#b#c005ebe��ת�����顷
		add_talk �ʵ�׼������ְҵ��װ����ת��ְҵ������������#n
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv < 70
			add_talk #b#b
			add_talk #b#b#c186506��������ɫ�ﵽ70����ѡ��ת��ְҵ������#n
		endif
		
		if lv >= 70
			if job != 17
				option 201 #�鿴���ػ��ߡ�ְҵ
			endif
			if job != 33
				option 202 #�鿴�������ߡ�ְҵ
			endif
			if job != 20
				option 203 #�鿴��ʥ���ߡ�ְҵ
			endif
			if job != 36
				option 204 #�鿴��Ԧ��ʦ��ְҵ
			endif
			if job != 19
				option 205 #�鿴����ɱ�ߡ�ְҵ
			endif
			if job != 35
				option 206 #�鿴�������ߡ�ְҵ
			endif
			if job != 18
				option 207 #�鿴�������ߡ�ְҵ
			endif
			if job != 34
				option 208 #�鿴������ʦ��ְҵ
			endif
		endif
		
	talk	
endproc

proc 201
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b������ð��ʱ���쵼�ߣ�ս��ʱ���ػ��ߡ�
		add_talk #b#b����ӵ����ߵķ������������ǿ�ı��ݣ����ڶ�ʱ���ڴ�������Լ��ķ������������÷�Χ�������䳬ǿ�ĵֿ��˺������������Ķ��ѡ�
		add_talk #c005ebe�ó�������������
		add_talk ְҵ�ص㣺������ֵ���߷�����#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 17
				option 211 #ת���ɡ��ػ��ߡ�ְҵ
			endif
		endif
		
		option 200 #����
		
	talk	
endproc

proc 202
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b�����ǿ񱩵Ķ�ʿ��������ɱ¾�ߡ�
		add_talk #b#b���Ƕ�ս���������ڳ��˵�����ͨ������ǣ�Ƽ��������ҵ��ˣ�Ȼ�����侪�˵�Ǳ��˲�伤��ǿ����˺�����������������˺���
		add_talk #c005ebe�ó����������ֽ������ֽ�
		add_talk ְҵ�ص㣺������˺���ǣ�Ƽ���#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 33
				option 212 #ת���ɡ������ߡ�ְҵ
			endif
		endif
		
		option 200 #����
		
	talk	
endproc

proc 203
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b�������Ŷӵľ������䣬����ʹ�Ļ���
		add_talk #b#b����ӵ�д����Ļָ����ܣ��Գ�ǿ����������������ߵĶ��ѣ����Ҹ�������ǿ���ף��������ֻҪ�������ڣ���������ս������Զ��ʤ���ߡ�
		add_talk #c005ebe�ó�������ʥ�ȡ�ʥ��
		add_talk ְҵ�ص㣺Ⱥ�����Ƽ��ܡ���������#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 20
				option 213 #ת���ɡ�ʥ���ߡ�ְҵ
			endif
		endif
		
		option 200 #����
		
	talk	
endproc

proc 204
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b�������ƿ��׵�Ĵ�ʦ�����Ͻ����ٲ��ߡ�
		add_talk #b#b����ӵ�в��׵�ħ���˺������������Ч�Ĵ�����������ǻ����Ŷ��бز����ٵ�Э���ߣ����ø�������ħ��������������ĥ���ˡ�
		add_talk #c005ebe�ó�������ʥ�ȡ�ʥ��
		add_talk ְҵ�ص㣺Զ��ħ���˺������似��#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 36
				option 214 #ת���ɡ�Ԧ��ʦ��ְҵ
			endif
		endif
		
		option 200 #����
		
	talk	
endproc

proc 205
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b������һ�����ĳ�У�����������ɱ�֡�
		add_talk #b#b����ʹ��˫��ǹ��Ϊ�Լ��������� ͨ��������ף���ȷ�����ǣ���ŵ��ˣ���ɱ�ڴ��ֲ����У��Ƕ�����Ҫ���ζ�����
		add_talk #c005ebe�ó�����������ǹ������ǹ
		add_talk ְҵ�ص㣺������ס�Զ�������˺�#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 19
				option 215 #ת���ɡ���ɱ�ߡ�ְҵ
			endif
		endif
		
		option 200 #����
		
	talk	
endproc

proc 206
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b�����Ǳ��Ƶ�ר�ң�ս������¾�ߡ�
		add_talk #b#b���Ǿ�ͨ���ֻ�ǹ��ǿ��Ļ���֧Ԯ��Ϊ�˶��ѵ���������ǿ��Ⱥ��ɱ���������˲��ݺ��ӣ���̼�ݻ�һ���������������˵�ӡ��
		add_talk #c005ebe�ó���������ǹ��׼��
		add_talk ְҵ�ص㣺Զ�̷�Χ�����˺�#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 35
				option 216 #ת���ɡ������ߡ�ְҵ
			endif
		endif
		
		option 200 #����
		
	talk	
endproc

proc 207
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b�������շǷ��ķ�Χħ������������ʹ�ߡ�
		add_talk #b#b���Ż������۵����Ƕ�ս�����Ŷ��ص���⣬ӵ����׿Խ�Ĵ�Χ����ħ��������ż���Ч��ʹ�����������ܡ�
		add_talk #c005ebe�ó�������ħ�ȡ�ħ��
		add_talk ְҵ�ص㣺Զ�̷�Χħ���˺������ټ���#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 18
				option 217 #ת���ɡ������ߡ�ְҵ
			endif
		endif
		
		option 200 #����
		
	talk	
endproc

proc 208
	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b����������ǵĴ����ʡ�
		add_talk #b#b����ǿ����ƻ��������������Ե�������裬���Ŷ��бز����ٵ����У���������ĵ���֮����ּ��õ��˻ҷ�����
		add_talk #c005ebe�ó�������ħ�ȡ�ħ��
		add_talk ְҵ�ص㣺Զ�̵���ħ���˺�#n
		
		call 210
		
		strvar lv job
		GET_PLAYER_BASE_ATT 0 0 lv job
		
		if lv >= 70
			if job != 34
				option 218 #ת���ɡ�����ʦ��ְҵ
			endif
		endif
		
		option 200 #����
		
	talk	
endproc

proc 211
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 221 220 ת��ְҵ��Ҫ$@lv$���Һ�1����ת��֮�顱\n�Ƿ�ȷ��ת���ɡ��ػ��ߡ���
	choose 2 221 220 ת��ְҵֻ��Ҫ֧��$@lv$����\n�Ƿ�ȷ��ת���ɡ��ػ��ߡ���
endproc
proc 212
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 222 220 ת��ְҵ��Ҫ$@lv$���Һ�1����ת��֮�顱\n�Ƿ�ȷ��ת���ɡ������ߡ���
	choose 2 222 220 ת��ְҵֻ��Ҫ֧��$@lv$����\n�Ƿ�ȷ��ת���ɡ������ߡ���
endproc
proc 213
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 223 220 ת��ְҵ��Ҫ$@lv$���Һ�1����ת��֮�顱\n�Ƿ�ȷ��ת���ɡ�ʥ���ߡ���
	choose 2 223 220 ת��ְҵֻ��Ҫ֧��$@lv$����\n�Ƿ�ȷ��ת���ɡ�ʥ���ߡ���
endproc
proc 214
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 224 220 ת��ְҵ��Ҫ$@lv$���Һ�1����ת��֮�顱\n�Ƿ�ȷ��ת���ɡ�Ԧ��ʦ����
	choose 2 224 220 ת��ְҵֻ��Ҫ֧��$@lv$����\n�Ƿ�ȷ��ת���ɡ�Ԧ��ʦ����
endproc
proc 215
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 225 220 ת��ְҵ��Ҫ$@lv$���Һ�1����ת��֮�顱\n�Ƿ�ȷ��ת���ɡ���ɱ�ߡ���
	choose 2 225 220 ת��ְҵֻ��Ҫ֧��$@lv$����\n�Ƿ�ȷ��ת���ɡ���ɱ�ߡ���
endproc
proc 216
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 226 220 ת��ְҵ��Ҫ$@lv$���Һ�1����ת��֮�顱\n�Ƿ�ȷ��ת���ɡ������ߡ���
	choose 2 226 220 ת��ְҵֻ��Ҫ֧��$@lv$����\n�Ƿ�ȷ��ת���ɡ������ߡ���
endproc
proc 217
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 227 220 ת��ְҵ��Ҫ$@lv$���Һ�1����ת��֮�顱\n�Ƿ�ȷ��ת���ɡ������ߡ���
	choose 2 227 220 ת��ְҵֻ��Ҫ֧��$@lv$����\n�Ƿ�ȷ��ת���ɡ������ߡ���
endproc
proc 218
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 0
	mul lv 100
	
	; choose 2 228 220 ת��ְҵ��Ҫ$@lv$���Һ�1����ת��֮�顱\n�Ƿ�ȷ��ת���ɡ�����ʦ����
	choose 2 228 220 ת��ְҵֻ��Ҫ֧��$@lv$����\n�Ƿ�ȷ��ת���ɡ�����ʦ����
endproc

proc 221
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 17
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg �����е����Ҳ���֧��ת��ְҵ�ķ��ã���ʱ�޷�ת��ְҵ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg ������������װ�����͵ġ��������ȫ������ʱ�޷�ת��ְҵ
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg �������Ҳ���ת��ְҵ��Ҫ�ġ�ת��֮�顿����ʱ�޷�ת��ְҵ
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif	
endproc

proc 222
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 33
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg �����е����Ҳ���֧��ת��ְҵ�ķ��ã���ʱ�޷�ת��ְҵ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg ������������װ�����͵ġ��������ȫ������ʱ�޷�ת��ְҵ
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg �������Ҳ���ת��ְҵ��Ҫ�ġ�ת��֮�顿����ʱ�޷�ת��ְҵ
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif	
endproc

proc 223
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 20
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg �����е����Ҳ���֧��ת��ְҵ�ķ��ã���ʱ�޷�ת��ְҵ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg ������������װ�����͵ġ��������ȫ������ʱ�޷�ת��ְҵ
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg �������Ҳ���ת��ְҵ��Ҫ�ġ�ת��֮�顿����ʱ�޷�ת��ְҵ
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif	
endproc

proc 224
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 36
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg �����е����Ҳ���֧��ת��ְҵ�ķ��ã���ʱ�޷�ת��ְҵ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg ������������װ�����͵ġ��������ȫ������ʱ�޷�ת��ְҵ
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg �������Ҳ���ת��ְҵ��Ҫ�ġ�ת��֮�顿����ʱ�޷�ת��ְҵ
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif	
endproc

proc 225
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 19
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg �����е����Ҳ���֧��ת��ְҵ�ķ��ã���ʱ�޷�ת��ְҵ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg ������������װ�����͵ġ��������ȫ������ʱ�޷�ת��ְҵ
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg �������Ҳ���ת��ְҵ��Ҫ�ġ�ת��֮�顿����ʱ�޷�ת��ְҵ
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif	
endproc

proc 226
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 35
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg �����е����Ҳ���֧��ת��ְҵ�ķ��ã���ʱ�޷�ת��ְҵ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg ������������װ�����͵ġ��������ȫ������ʱ�޷�ת��ְҵ
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg �������Ҳ���ת��ְҵ��Ҫ�ġ�ת��֮�顿����ʱ�޷�ת��ְҵ
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif	
endproc

proc 227
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 18
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg �����е����Ҳ���֧��ת��ְҵ�ķ��ã���ʱ�޷�ת��ְҵ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg ������������װ�����͵ġ��������ȫ������ʱ�޷�ת��ְҵ
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg �������Ҳ���ת��ְҵ��Ҫ�ġ�ת��֮�顿����ʱ�޷�ת��ְҵ
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif	
endproc

proc 228
	strvar num ret ret1 ret2 ret3 gezi
	GET_ITEM_NUM_INPAK 5030020 0 num
	GET_PACK_SPACE 0 gezi
	career_alter_check ret
	if ret = 0
		; if num >= 1
			if gezi > 0
				strvar lv job yinbi
				GET_PLAYER_BASE_ATT 0 0 lv job
				mul lv 100
				GET_ID_PLAYER_INFO 0 14  yinbi
				if yinbi >= @lv
					; DEC_ITEM_NUM_INPAK 5030020 1 ret1
					DEC_MONEY 0 @lv 20 ret2
					if ret2 = 0; and ret1 = 0
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5030070 1 1 ret3
						if ret3 = 0
							career_alter 0 34
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg �����е����Ҳ���֧��ת��ְҵ�ķ��ã���ʱ�޷�ת��ְҵ
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg ������������װ�����͵ġ��������ȫ������ʱ�޷�ת��ְҵ
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		; else
			; add_sys_msg_start
			; add_sys_msg �������Ҳ���ת��ְҵ��Ҫ�ġ�ת��֮�顿����ʱ�޷�ת��ְҵ
			; set_sys_msg_mode 3 0 0
			; msg_info
		; endif
	endif
endproc

proc 210
	strvar lv job yinbi
	GET_PLAYER_BASE_ATT 0 0 lv job
	
	add_talk #b#b
	if lv >= 70
		mul lv 100
		GET_ID_PLAYER_INFO 0 14  yinbi
		if yinbi >= @lv
			add_talk ת�����ã�#c186506$@lv$#n���ң��ȼ�Խ�ߣ�����Խ�ߣ�
		endif
		if yinbi < @lv
			add_talk ת�����ã�#cF80B0B$@lv$#n���ң��ȼ�Խ�ߣ�����Խ�ߣ�
		endif
		; add_talk ��Ҫ���ߣ�ת��ְҵ��Ҫ�ṩ��#c6d00d2ת��֮��#n��
	endif
	add_talk #b#b
	add_talk  #c005ebe�����ɹ�ת��ְҵ������1�����������ȫ������#n
	add_talk #b#b
	add_talk #b#b#b#b#c186506��ÿ��ת��ְҵ��Ҫ���1Сʱ��#n
endproc
