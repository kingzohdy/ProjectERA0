


proc 1
	dialog 1
	add_talk ������ע������˵���� 
	add_talk #b#bÿ��ÿ������޴θ��ҹ�ע����
	add_talk #b#bÿ�ι�ע���������úô�
;	add_talk #b#b#c186506ʹ�á������������������ӽ϶���������ý϶ཱ����ʹ�á��۽������ܡ����ӽ�����������ý��ٽ�����#n
	
	add_talk #b#bֻ�л᳤��Ȩ�޼���BOSS��BOSS����3��Сʱ
	add_talk #b#bֻ������06��00-24��00������᳤δ�ܼ�ʱ�������24��00�Զ����
	add_talk #b#b
	is_in_my_clan_city 50
	if 50 = 1
		GET_CLAN_BABY_FEED 50
		if 50 >= 0 and 50 <= 100
			add_talk #b#b��ǰƷ�ʣ���ͨ
		endif
		if 50 > 100 and 50 <= 2000
			add_talk #b#b��ǰƷ�ʣ�#c186506����#n
		endif
		if 50 > 2000 and 50 <= 5000
			add_talk #b#b��ǰƷ�ʣ�#c005ebe����#n
		endif
		if 50 > 5000
			add_talk #b#b��ǰƷ�ʣ�#c6d00d2�ܳ�
		endif
		add_talk #b#b#c6d00d2ÿ��һ����6��00����BOSSƷ��
		;	option 150 ʹ�á���������������ע����
			option 160 ʹ�á��۽������ܡ���ע����
			
		strvar ret
		GET_CLAN_BUILDING_LV ret 0 9
		if ret >= 3
			option 200 #����BOSS��ֻ�л᳤��Ȩ�޲�����
		endif
	endif
		
	talk
	
endproc 

proc 150
	; strvar ret
	; GET_CLAN_BUILDING_LV ret 0 9
	; if ret < 3
		
		; add_sys_msg_start
		; add_sys_msg ���ڹ���ȼ����ͣ�δ�ܴﵽ����BOSS���ɼƻ�����������������ȼ������ԣ�
		; set_sys_msg_mode 3 0 0
		; msg_info
		; return
	; endif

	; dialog 1
	; add_talk #b#bÿ��������޴εĸ��ҹ�ע������ÿ�ζ��ɻ�ý�����
	; add_talk #b#b
	; add_talk #b#b#c186506����������������
	; add_talk #b#b
	
		; option 151 ʹ�á���������������  1��
		; option 152 ʹ�á���������������  5��
		; option 153 ʹ�á���������������  10��
		; option 154 ʹ�á���������������  50��
		; option 155 ʹ�á���������������  100��
		; option 1 ����
	; talk
endproc

proc 159
	strvar ret
	task_stat 3018 1 ret
	if 1 = 0 and ret = 2
		task_fini 3018 1 
	endif
endproc 

proc 151
	strvar num
	GET_ITEM_NUM_INPAK 5060036 1 num
	if num >= 1
		DEC_ITEM_NUM_INPAK 5060036 1 30
		if 30 = 0
			CLAN_BABY_FEED 1 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
			;	mul 50 2
				dev 50 10000
				
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 156
	endif
	
	
	; add_sys_msg_start
	; add_sys_msg $@num$
	; set_sys_msg_mode 1 0 0
	; msg_info
	
endproc
proc 152
	strvar num
	GET_ITEM_NUM_INPAK 5060036 1 num
	if num >= 5
		DEC_ITEM_NUM_INPAK 5060036 5 30
		if 30 = 0
			CLAN_BABY_FEED 5 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
				mul 50 5
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 156
	endif
endproc 
proc 153
	strvar num
	GET_ITEM_NUM_INPAK 5060036 1 num
	if num >= 10
		DEC_ITEM_NUM_INPAK 5060036 10 30
		if 30 = 0
			CLAN_BABY_FEED 10 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
				mul 50 10
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 156
	endif
endproc 
proc 154
	strvar num
	GET_ITEM_NUM_INPAK 5060036 1 num
	if num >= 50
		DEC_ITEM_NUM_INPAK 5060036 50 30
		if 30 = 0
			CLAN_BABY_FEED 50 20
			if 20 = 0
			
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
				mul 50 50
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 156
	endif
endproc 
proc 155
	strvar num
	GET_ITEM_NUM_INPAK 5060036 1 num
	if num >= 100
		DEC_ITEM_NUM_INPAK 5060036 100 30
		if 30 = 0
			CLAN_BABY_FEED 100 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
				mul 50 100
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 156
	endif
endproc 
proc 156
	add_sys_msg_start
	add_sys_msg ��û���㹻�Ĵ���������
	set_sys_msg_mode 3 0 0
	msg_info
endproc 


proc 160
	strvar ret
	GET_CLAN_BUILDING_LV ret 0 9
	if ret < 3
		
		add_sys_msg_start
		add_sys_msg ���ڹ���ȼ����ͣ�δ�ܴﵽ����BOSS���ɼƻ�����������������ȼ������ԣ�
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	
	
	GET_NOW_TIME 0 0 0 20 15 0 0
	if 50 = 1 and 20 = 1 and 15 < 6
		add_sys_msg_start
		add_sys_msg ÿ���ռ���BOSS��������һ����6��00ǰ����ΪBOSS��ע����
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	GET_CLAN_BABY_STAT 50
	if 50 != 0
	
		add_sys_msg_start
		add_sys_msg ÿ���ռ���BOSS��������һ����6��00ǰ����ΪBOSS��ע����
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	
	dialog 1
	add_talk #b#bÿ��������޴εĸ��ҹ�ע������ÿ�ζ��ɻ�ý�����
	add_talk #b#b
	add_talk #b#b#c186506�۽������ܳ��������#L��Ȥ����@@[306042,4400]#n���
	add_talk #b#b

	
		option 161 ʹ�á��۽������ܡ���  1��
		option 162 ʹ�á��۽������ܡ���  5��
		option 163 ʹ�á��۽������ܡ���  10��
		option 164 ʹ�á��۽������ܡ���  50��
		option 165 ʹ�á��۽������ܡ���  100��
		option 1 ����
	talk
endproc

proc 161

	GET_NOW_TIME 0 0 0 20 15 0 0
	if 50 = 1 and 20 = 1 and 15 < 6
		add_sys_msg_start
		add_sys_msg ÿ���ռ���BOSS��������һ����6��00ǰ����ΪBOSS��ע����
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	GET_CLAN_BABY_STAT 50
	if 50 != 0
		return
	endif
	strvar num
	GET_ITEM_NUM_INPAK 5060037 1 num
	if num >= 1
		DEC_ITEM_NUM_INPAK 5060037 1 30
		if 30 = 0
			CLAN_BABY_FEED 1 20
			if 20 = 0
			
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
			;	mul 50 2
				
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 166
	endif	
endproc
proc 162

	GET_NOW_TIME 0 0 0 20 15 0 0
	if 50 = 1 and 20 = 1 and 15 < 6
		add_sys_msg_start
		add_sys_msg ÿ���ռ���BOSS��������һ����6��00ǰ����ΪBOSS��ע����
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	GET_CLAN_BABY_STAT 50
	if 50 != 0
	
		add_sys_msg_start
		add_sys_msg ÿ���ռ���BOSS��������һ����6��00ǰ����ΪBOSS��ע����
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	strvar num
	GET_ITEM_NUM_INPAK 5060037 1 num
	if num >= 5
		DEC_ITEM_NUM_INPAK 5060037 5 30
		if 30 = 0
			CLAN_BABY_FEED 5 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
			;	mul 50 2
				mul 50 5
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 166
	endif	
endproc
proc 163

	GET_NOW_TIME 0 0 0 20 15 0 0
	if 50 = 1 and 20 = 1 and 15 < 6
		add_sys_msg_start
		add_sys_msg ÿ���ռ���BOSS��������һ����6��00ǰ����ΪBOSS��ע����
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	GET_CLAN_BABY_STAT 50
	if 50 != 0
	
		add_sys_msg_start
		add_sys_msg ÿ���ռ���BOSS��������һ����6��00ǰ����ΪBOSS��ע����
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	strvar num
	GET_ITEM_NUM_INPAK 5060037 1 num
	if num >= 10
		DEC_ITEM_NUM_INPAK 5060037 10 30
		if 30 = 0
			CLAN_BABY_FEED 10 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
			;	mul 50 2
				mul 50 10
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 166
	endif	
endproc
proc 164

	GET_NOW_TIME 0 0 0 20 15 0 0
	if 50 = 1 and 20 = 1 and 15 < 6
		add_sys_msg_start
		add_sys_msg ÿ���ռ���BOSS��������һ����6��00ǰ����ΪBOSS��ע����
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	GET_CLAN_BABY_STAT 50
	if 50 != 0
	
		add_sys_msg_start
		add_sys_msg ÿ���ռ���BOSS��������һ����6��00ǰ����ΪBOSS��ע����
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	strvar num
	GET_ITEM_NUM_INPAK 5060037 1 num
	if num >= 50
		DEC_ITEM_NUM_INPAK 5060037 50 30
		if 30 = 0
			CLAN_BABY_FEED 50 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
			;	mul 50 2
				mul 50 50
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 166
	endif	
endproc
proc 165

	GET_NOW_TIME 0 0 0 20 15 0 0
	if 50 = 1 and 20 = 1 and 15 < 6
		add_sys_msg_start
		add_sys_msg ÿ���ռ���BOSS��������һ����6��00ǰ����ΪBOSS��ע����
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	GET_CLAN_BABY_STAT 50
	if 50 != 0
	
		add_sys_msg_start
		add_sys_msg ÿ���ռ���BOSS��������һ����6��00ǰ����ΪBOSS��ע����
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	strvar num
	GET_ITEM_NUM_INPAK 5060037 1 num
	if num >= 100
		DEC_ITEM_NUM_INPAK 5060037 100 30
		if 30 = 0
			CLAN_BABY_FEED 100 20
			if 20 = 0
				GET_ID_PLAYER_INFO 0 19 50
				dev 50 10000
			;	mul 50 2
				mul 50 100
				ADD_EXP 0 @50 1
			;	ADD_ID_PLAYER_CONTRI 0 0 10 0 0
				call 159
			endif
		endif
	else
		call 166
	endif	
endproc

proc 166
	add_sys_msg_start
	add_sys_msg ��û���㹻�ľ۽�������
	set_sys_msg_mode 3 0 0
	msg_info
endproc 

proc 200
	
	GET_CLAN_BABY_STAT 20
	if 20 = 0
		clan_baby_call 10
		if 10 = 0
			EFFECT 4 1 SetMessageTips(415)
			
			GET_ID_PLAYER_INFO 0 18 50
			
			add_sys_msg_start
			add_sys_msg Ӣ������Ļ᳤����������ǰ��������м���BOSS-����������Ҿ���ǰ������ɱ����÷ǳ����Ľ���Ӵ��
			set_sys_msg_mode 14 0 0
			CLAN_MSG_INFO 1 @50

		endif
		; GET_CUR_MAX_LEVEL 50 60
		; if 50 = 0 and 60 < 60
			; GET_CLAN_BABY_FEED 40
			; if 40 >= 0 and 40 <= 100
				; new_life_mon 208100 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 100 and 40 <= 2000
				; new_life_mon 208101 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 2000 and 40 <= 5000
				; new_life_mon 208102 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 5000
				; new_life_mon 208103 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
		; endif
		; if 50 = 0 and 60 >= 60 and 60 < 70 
			; GET_CLAN_BABY_FEED 40
			; if 40 >= 0 and 40 <= 100
				; new_life_mon 208104 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 100 and 40 <= 2000
				; new_life_mon 208105 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 2000 and 40 <= 5000
				; new_life_mon 208106 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 5000
				; new_life_mon 208107 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
		; endif
		; if 50 = 0 and 60 >= 70 and 60 < 80 
			; GET_CLAN_BABY_FEED 40
			; if 40 >= 0 and 40 <= 100
				; new_life_mon 208108 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 100 and 40 <= 2000
				; new_life_mon 208109 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 2000 and 40 <= 5000
				; new_life_mon 208110 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 5000
				; new_life_mon 208111 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
		; endif
		; if 50 = 0 and 60 >= 80
			; GET_CLAN_BABY_FEED 40
			; if 40 >= 0 and 40 <= 100
				; new_life_mon 208112 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 100 and 40 <= 2000
				; new_life_mon 208113 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 2000 and 40 <= 5000
				; new_life_mon 208114 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
			; if 40 > 5000
				; new_life_mon 208115 1 29600 10100 1 1 1 1 10800 1 1 
			; endif
		; endif
	else
		
		add_sys_msg_start
		add_sys_msg ��Ч������ǰ��ʷ�������˽�����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 










