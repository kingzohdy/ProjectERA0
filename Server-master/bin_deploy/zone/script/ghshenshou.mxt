


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#bΪ���������Դ���ض�ȱ�����⣬���˾����Թ���Ϊ��λʵʩ��BOSS���ɡ��ƻ����üƻ���Ҫ�������£�
	add_talk #b#b
	add_talk #b#b#c186506 1������ﵽ3���Զ�����BOSS 
	add_talk #b#b 2���ٻ���3��Сʱδ��ɱ��BOSS����ʧ
	add_talk #b#b 3��ֻ�л᳤����Ȩ�޼���BOSS
	add_talk #b#b 4��ÿ����6��-24��֮��ɼ���BOSS
	add_talk #b#b�ڴ�ʱ�����δ�ֶ�������BOSS�Զ�����
	add_talk #b#b#c6d00d2��ɱbossʱ�������Ὠ�������ʱ�䲻��24Сʱ���޷���þ��顢�����ʽ���#n
	
	
	is_in_my_clan_city 50
	if 50 = 1
	
		task_add_option 3018 1 100
	
		option 150 ��BOSS�����ɼƻ���ϸ����
;		option 200 #����BOSS
	endif
	talk
	
endproc 
PROC 100
	task_do_option 3018 1
ENDPROC

proc 150
	dialog 1
	add_talk BOSS���ʣ�
	add_talk #b#bBOSS��Ϊ��ͨ�����á����㡢�ܳ�
	add_talk #b#b���BOSS����Ϊ��ͨ��ι��ʳ���ʹBOSS�����ʽ��ף�BOSS����Խ�ߣ���ɱʱ�Ľ���Խ�ߣ�
	add_talk #b#b
	add_talk �������ƣ�
	add_talk #b#b����ﵽ3���Զ�����BOSS��BOSS������󣨰����᳤�ֶ����ʱ�䵽��֮��BOSS�Զ�����Զ�������һ�ܵ�BOSS��
	add_talk #b#b
	add_talk ��ע˵����
	add_talk #b#bÿ�������Աÿ����Խ���һ�ι�ע�����������ɻ�ô����ľ��顣
	add_talk #b#b�ڲ��������������£�Ҳ���Զ�BOSS��ע������ÿ�ζ���������������

		option 151 ��һҳ��������Դ������˵��������˵����
		option 1 ����
	talk
endproc

proc 151
	dialog 1
	add_talk ������Դ��
;	add_talk #b#bĿǰ�ܹ���BOSS��ע��������Ʒ�У�
;	add_talk #b#b��������������ʱδ�г�����
	add_talk #b#b�۽������ܳ��������#L��Ȥ����@@[306042,4400]#n���

	add_talk #b#b
	add_talk ����˵����
	add_talk #b#b#c186506ÿ����6��-24��֮��᳤���ֶ�����BOSS���ڴ�ʱ�����δ������BOSS�Զ���������BOSS������ڹ�������С��ƻ�ִ�й�-ʷ�������������ٻ���3��Сʱδ��ɱ��BOSS����ʧ��

	add_talk #b#b
	add_talk ����˵����
	add_talk #b#b��ɱBOSSʱ�������Ĺ����Ա�Ի��þ��齱����������˫������ӳɣ���BOSS����������BOSS���䣬�ɻ᳤������BOSS����Խ�ߣ�����Խ�ã�
;	add_talk #b#b
	add_talk #b#b

		option 150 ��һҳ��BOSSƷ�ʡ��������ơ���ע˵����
		option 1 ����
	talk

endproc


proc 200
	strvar Pos
	player_clan_pos_info 1 0 0 0 Pos
	if Pos = 8
		CLAN_BABY_CALL 1
		
	
	else
		add_sys_msg_start
		add_sys_msg ֻ�л᳤�ſ��Բ���
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
















