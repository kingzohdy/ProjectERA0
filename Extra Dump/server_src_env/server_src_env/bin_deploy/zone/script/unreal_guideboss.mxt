;�þ�-BOSS��ͼNPC��
proc 1
	is_captain 10 0
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��һ��δ֪�����������ǰ,��׼��������?
	;�жϽ�ɫ�Ƿ��Ƕӳ�
	if @10 = 0
		is_last_unreal_boss_map 0 20
		;�������һ��BOSS��ͼ
		if @20 = 0
		option 100 #ǰ����һ��
		endif
	endif
	option 300 #����ʱ���
	talk	
endproc 

proc 100
	is_captain 10 0
	if @10 = 0 
		UNREAL_ENTER 0 2
	else
		add_sys_msg_start
		add_sys_msg	��ϧ�ˣ��������Ѳ��Ƕӳ����޷������鴫�뵽��һ���ˡ�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
endproc

proc 300
	choose 0 301 302 ȷ������ʱ�����?
endproc

proc 301
	TRANSMIT 1000 40765 35339 0
endproc

proc 302

endproc


