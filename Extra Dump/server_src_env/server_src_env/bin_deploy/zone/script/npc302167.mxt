;����npc

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�������ƣ���ϰ�ף����ж��硣
	add_talk #b#b��ѽڣ����Բ��ϣ����۾��ɡ�
	add_talk #b#b�򻧼��в����ա���ʲô���㲻������ӣ�����ͷ������ָ����һ����
	
;	TASK_ADD_OPTION 4486 0 103
	TASKCYCLE_ADD_OPTION 139 1 120

	option 130 �����ӵĲ��ϴ�����ã�
	option 125 �����������Ӱɣ�
	
	GET_PLAYER_DAILY_FLAG 0 15 10
	if 10 = 0
		option 134 ��ȡ��Ҷ
	endif
	
	talk
	
endproc 


proc 103
	TASK_DO_OPTION 4486 0
endproc

PROC 120	
	TASKCYCLE_DO_OPTION 139 1
ENDPROC

proc 125
	dialog 1
	add_talk $_npc$:
	add_talk #b#b#c186506��Ŵ�ס�#n��������ר���Ѷȵ����߸�����#c186506��ҹ�򡢻ƽ�󾮡�ɭ�ֽ�硢������ء�̫�����#n����������#c186506��ɽ���������ȡ����οռ�#n�����BOSS���м��ʵ��䡣
	add_talk #b##b#c186506���춹��#n����#L��Բ@@[302168,1000]#n���̵��о��г��ۣ���#c186506����Ȼ�����ڡ�#n����Ҫ��#c186506�̳�#n�й���
	add_talk #b#b����Ҷ����ֻ��Ҫ����ʱ����������ͺ��ˡ�
	option 131 ������˼��(����Ҷ+���춹)
	option 132 ����������(����Ҷ+��Ŵ��+������)
	option 133 ����������(����Ҷ+����Ȼ������)
	option 1 ����
	talk
endproc

PROC 130
	dialog 1
	add_talk $_npc$:
	add_talk #b#b#c186506��˼��#n������Ҷ+���춹
	add_talk #b#b#c0181f9������#n������Ҷ+��Ŵ��+������
	add_talk #b#b#c6d00d2������#n������Ҷ+����Ȼ������
	add_talk #b#b
	add_talk #b#b�㲻֪��������Ū�����ϣ���������Ӵ��
	add_talk #b#b#c186506��Ŵ�ס�#n��������ר���Ѷȵ����߸�����#c186506��ҹ�򡢻ƽ�󾮡�ɭ�ֽ�硢������ء�̫�����#n����������#c186506��ɽ���������ȡ����οռ�#n�����BOSS���м��ʵ��䡣
	add_talk #b##b#c186506���춹��#n����#L��Բ@@[302168,1000]#n���̵��о��г��ۣ���#c186506����Ȼ�����ڡ�#n����Ҫ��#c186506�̳�#n�й���
	add_talk #b#b����Ҷ����ֻ��Ҫ����ʱ����������ͺ��ˡ�
	
	option 160 #��лָ�㣬�����ھ�ȥ�Ҳ���
	talk
ENDPROC


PROC 131
	dialog 1
	add_talk $_npc$:
	add_talk #b#b#c186506��˼�տ���Ϊ��˲��ָ�������#c0181f9�����տ���Ϊ��˲��ָ�ħ����#ccd00d2������������޾��ı���#n

    add_talk #b#b����#c186506��˼��#n������ϣ�#c186506����Ҷ+���춹#n

	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060043 1 12
	
	add_talk #b#b��ǰӵ��#cff4e00��Ҷ��$@10$��#n
	add_talk #b#b��ǰӵ��#cff4e00�춹��$@12$��#n

	option 170 ������˼�� 1��
	option 172 ������˼�� 10��

	option 132 ����������
	option 133 ����������

	option 1 ����
		
	talk
ENDPROC

PROC 132
	dialog 1
	add_talk $_npc$:
	add_talk #b#b#c186506��˼�տ���Ϊ��˲��ָ�������#c0181f9�����տ���Ϊ��˲��ָ�ħ����#ccd00d2������������޾��ı���#n

    add_talk #b#b����#c0181f9������#n������ϣ�#c0181f9����Ҷ+��Ŵ��+������#n

	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060042 1 11
	GET_ITEM_NUM_INPAK 5060044 1 12
	
	
	add_talk #b#b��ǰӵ��#cff4e00��Ҷ��$@10$��#n
	add_talk #b#b��ǰӵ��#cff4e00Ŵ�ף�$@11$��#n
	add_talk #b#b��ǰӵ��#cff4e00���⣺$@12$��#n

	option 180 ���������� 1��
	option 182 ���������� 10��

	option 131 ������˼��
	option 133 ����������

	option 1 ����
		
	talk
ENDPROC

PROC 133
	dialog 1
	add_talk $_npc$:
	add_talk #b#b#c186506��˼�տ���Ϊ��˲��ָ�������#c0181f9�����տ���Ϊ��˲��ָ�ħ����#ccd00d2������������޾��ı���#n

    add_talk #b#b����#ccd00d2������#n������ϣ�#ccd00d2����Ҷ+����Ȼ������#n

	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060045 1 11
	
	add_talk #b#b��ǰӵ��#cff4e00��Ҷ��$@10$��#n
	add_talk #b#b��ǰӵ��#cff4e00��Ȼ�����ڣ�$@11$��#n

	option 190 ���������� 1��
	option 192 ���������� 10��

	option 131 ������˼��
	option 132 ����������

	option 1 ����
		
	talk
ENDPROC


proc 134	
	GET_DAYONLINE 1 10
	
	var 11 @10
	if 10 >= 3600
		dev 10 3600
		MOD 11 3600
		dev 11 60
	else
		var 10 0
		dev 11 60
	endif
	
	dialog 1
	add_talk $_npc$:
	add_talk #b#bÿ������6����������ʱ�䣬��������ʱ����ۼơ�
	add_talk #b#b
	add_talk #b#b����������ʱ��Ϊ��$@10$ʱ$@11$��
	add_talk #b#b
	add_talk #b#b�ۼ�#c186506����1Сʱ#n����ȡ��Ҷ��15��
	
    option 150 #��ȡ
	option 1 #����
	talk
endproc

;��ȡ
proc 150	
	GET_DAYONLINE 1 10
	
	GET_PLAYER_DAILY_FLAG 0 15 20
	;0��ʾδ����
	if 20 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif

	if 10 < 3600
		dialog 1
		add_talk $_npc$:
		add_talk #b#b�Բ�������������ʱ�䲻�㣱Сʱ��������ȡ������
		option 134 ����
		talk
	else
		GET_PACK_SPACE 0 11
		if 11 < 1
			add_sys_msg_start
			add_sys_msg ���ò��¸��ණ���ˣ���ȥ�ֿ����Ա�Ƕ�����һ�±����ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		else
			strvar Ret 
			SET_PLAYER_DAILY_FLAG Ret 15 1
			if Ret = 0				
				ADD_ITEM_NUM_INPAK 5060041 15 1 14
			endif
		endif
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc

;5012123  ��˼�� 1��
proc 170
	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060043 1 30
	if @10 < 1
		add_sys_msg_start
		add_sys_msg ȱ����Ҷ����ǰ������[ʱ���]��ȡ��Ҳ��ͨ������������ã�
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
	
	if @30 < 1
		add_sys_msg_start
		add_sys_msg ȱ�ٺ춹����ǰ����Բ[ʱ���]���̵��й���
		set_sys_msg_mode 4 0 0
		msg_info
		return 
    endif
	
	if @10 >= 1 and @30 >= 1
		GET_PACK_SPACE 0 11
		if 11 < 1
			add_sys_msg_start
			add_sys_msg ���ò��¸��ණ���ˣ���ȥ�ֿ����Ա�Ƕ�����һ�±����ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		else
			ADD_BIND_ITEM_NUM_INPAK 5012123 1 1 13
			DEC_ITEM_NUM_INPAK 5060041 1 0
			DEC_ITEM_NUM_INPAK 5060043 1 0
			add_sys_msg_start
			add_sys_msg ����һ����˼�سɹ���
			set_sys_msg_mode 4 0 0
			msg_info
			call 131
		endif
	else
		call 131
		add_sys_msg_start
		add_sys_msg ���ϲ��㣡���ϵġ������ӵĲ��ϴ�����ã���ѡ��ɲ鿴���ϳ���
		set_sys_msg_mode 4 0 0
		msg_info
	endif
	
endproc

;5012123  ��˼�� 10��
proc 172 
	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060043 1 30
	if @10 < 1
		add_sys_msg_start
		add_sys_msg ȱ����Ҷ����ǰ������[ʱ���]��ȡ��Ҳ��ͨ������������ã�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @30 < 1
		add_sys_msg_start
		add_sys_msg ȱ�ٺ춹����ǰ����Բ[ʱ���]���̵��й���
		set_sys_msg_mode 4 0 0
		msg_info
		return
    endif
	
	if @10 >= 10 and @30 >= 10
	    GET_PACK_SPACE 0 11
		if 11 < 1
			add_sys_msg_start
			add_sys_msg ���ò��¸��ණ���ˣ���ȥ�ֿ����Ա�Ƕ�����һ�±����ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		else
			ADD_BIND_ITEM_NUM_INPAK 5012123 10 1 13
			DEC_ITEM_NUM_INPAK 5060041 10 0
			DEC_ITEM_NUM_INPAK 5060043 10 0
			add_sys_msg_start
			add_sys_msg ����ʮ����˼�سɹ���
			set_sys_msg_mode 4 0 0
			msg_info
			call 131
		endif
	else
		call 131
		add_sys_msg_start
		add_sys_msg ���ϲ��㣡���ϵġ������ӵĲ��ϴ�����ã���ѡ��ɲ鿴���ϳ���
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc


;5012124   ������ 1��
proc 180
	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060042 1 20
	GET_ITEM_NUM_INPAK 5060044 1 30
	if @10 < 1
		add_sys_msg_start
		add_sys_msg ȱ����Ҷ����ǰ������[ʱ���]��ȡ��Ҳ��ͨ������������ã�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @20 < 1 
		add_sys_msg_start
		add_sys_msg ȱ��Ŵ�ף���ǰ�����ѡ�ר���Ѷȵ����߸��������鸱����ã�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @30 < 1
		add_sys_msg_start
		add_sys_msg ȱ�����⣡��ǰ����Բ[ʱ���]���̵��й���
		set_sys_msg_mode 4 0 0
		msg_info
		return
    endif
	
	if @10 >= 1 and @20 >= 1 and @30 >= 1
		GET_PACK_SPACE 0 11
		if 11 < 1
			add_sys_msg_start
			add_sys_msg ���ò��¸��ණ���ˣ���ȥ�ֿ����Ա�Ƕ�����һ�±����ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		else
			ADD_BIND_ITEM_NUM_INPAK 5012124 1 1 13
			DEC_ITEM_NUM_INPAK 5060041 1 0
			DEC_ITEM_NUM_INPAK 5060042 1 0
			DEC_ITEM_NUM_INPAK 5060044 1 0
			add_sys_msg_start
			add_sys_msg ����һ�������سɹ���
			set_sys_msg_mode 4 0 0
			msg_info
			call 132
		endif
	else
		call 132
		add_sys_msg_start
		add_sys_msg ���ϲ��㣡���ϵġ������ӵĲ��ϴ�����ã���ѡ��ɲ鿴���ϳ���
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

;5012124   ������ 10��
proc 182
	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060042 1 20
	GET_ITEM_NUM_INPAK 5060044 1 30
	if @10 < 1
		add_sys_msg_start
		add_sys_msg ȱ����Ҷ����ǰ������[ʱ���]��ȡ��Ҳ��ͨ������������ã�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @20 < 1 
		add_sys_msg_start
		add_sys_msg ȱ��Ŵ�ף���ǰ�����ѡ�ר���Ѷȵ����߸��������鸱����ã�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @30 < 1
		add_sys_msg_start
		add_sys_msg ȱ�����⣡��ǰ����Բ[ʱ���]���̵��й���
		set_sys_msg_mode 4 0 0
		msg_info
		return
    endif
	
	if @10 >= 10 and @20 >= 10 and @30 >= 10
		GET_PACK_SPACE 0 11
		if 11 < 1
			add_sys_msg_start
			add_sys_msg ���ò��¸��ණ���ˣ���ȥ�ֿ����Ա�Ƕ�����һ�±����ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		else
			ADD_BIND_ITEM_NUM_INPAK 5012124 10 1 13
			DEC_ITEM_NUM_INPAK 5060041 10 0
			DEC_ITEM_NUM_INPAK 5060042 10 0
			DEC_ITEM_NUM_INPAK 5060044 10 0
			add_sys_msg_start
			add_sys_msg ����ʮ�������سɹ���
			set_sys_msg_mode 4 0 0
			msg_info
			call 132
		endif
	else
		call 132
		add_sys_msg_start
		add_sys_msg ���ϲ��㣡���ϵġ������ӵĲ��ϴ�����ã���ѡ��ɲ鿴���ϳ���
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc


;5230043   ������ 1��
proc 190 
	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060045 1 20

	if @10 < 1
		add_sys_msg_start
		add_sys_msg ȱ����Ҷ����ǰ������[ʱ���]��ȡ��Ҳ��ͨ������������ã�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @20 < 1 
		add_sys_msg_start
		add_sys_msg ȱ����Ȼ�����ڣ����ڽ���̳��й���
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	strvar levelnum sex job
	GET_PLAYER_BASE_ATT 0 sex levelnum job
	if @10 >= 1 and @20 >= 1
		GET_PACK_SPACE 0 11
		if 11 < 1
			add_sys_msg_start
			add_sys_msg ���ò��¸��ණ���ˣ���ȥ�ֿ����Ա�Ƕ�����һ�±����ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		else
			if @levelnum >= 10 and @levelnum <= 59
				ADD_ITEM_NUM_INPAK 5230043 1 1 13
			elseif @levelnum >= 60 and @levelnum <= 69
				ADD_ITEM_NUM_INPAK 5230044 1 1 13
			elseif @levelnum >= 70 and @levelnum <= 120
				ADD_ITEM_NUM_INPAK 5230045 1 1 13
			else
				call 133
				add_sys_msg_start
				add_sys_msg ����������ʧ�ܣ�
				set_sys_msg_mode 4 0 0
				msg_info
				return
			endif
			DEC_ITEM_NUM_INPAK 5060041 1 0
			DEC_ITEM_NUM_INPAK 5060045 1 0
			add_sys_msg_start
			add_sys_msg ����һ�������سɹ���
			set_sys_msg_mode 4 0 0
			msg_info
			call 133
		endif
	else
		call 133
		add_sys_msg_start
		add_sys_msg ���ϲ��㣡���ϵġ������ӵĲ��ϴ�����ã���ѡ��ɲ鿴���ϳ���
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

;5230043   ������ 10��
proc 192 
	GET_ITEM_NUM_INPAK 5060041 1 10
	GET_ITEM_NUM_INPAK 5060045 1 20

	if @10 < 1
		add_sys_msg_start
		add_sys_msg ȱ����Ҷ����ǰ������[ʱ���]��ȡ��Ҳ��ͨ������������ã�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	if @20 < 1 
		add_sys_msg_start
		add_sys_msg ȱ����Ȼ�����ڣ����ڽ���̳��й���
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	strvar levelnum sex job
	GET_PLAYER_BASE_ATT 0 sex levelnum job
	if @10 >= 10 and @20 >= 10
		GET_PACK_SPACE 0 11
		if 11 < 1
			add_sys_msg_start
			add_sys_msg ���ò��¸��ණ���ˣ���ȥ�ֿ����Ա�Ƕ�����һ�±����ɣ�
			set_sys_msg_mode 3 0 0
			msg_info
		else
			if @levelnum >= 10 and @levelnum <= 59
				ADD_ITEM_NUM_INPAK 5230043 10 1 13
			elseif @levelnum >= 60 and @levelnum <= 69
				ADD_ITEM_NUM_INPAK 5230044 10 1 13
			elseif @levelnum >= 70 and @levelnum <= 120
				ADD_ITEM_NUM_INPAK 5230045 10 1 13
			else
				call 133
				add_sys_msg_start
				add_sys_msg ����������ʧ�ܣ�
				set_sys_msg_mode 4 0 0
				msg_info
				return
			endif
			DEC_ITEM_NUM_INPAK 5060041 10 0
			DEC_ITEM_NUM_INPAK 5060045 10 0
			add_sys_msg_start
			add_sys_msg ����ʮ�������سɹ���
			set_sys_msg_mode 4 0 0
			msg_info
			call 133
		endif
	else
		call 133
		add_sys_msg_start
		add_sys_msg ���ϲ��㣡���ϵġ������ӵĲ��ϴ�����ã���ѡ��ɲ鿴���ϳ���
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

