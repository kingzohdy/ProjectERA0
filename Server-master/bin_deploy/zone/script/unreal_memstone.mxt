;�ʺ�þ�-����ʯNPC

proc 1
	get_unreal_layer 1 10
	DIALOG 1
	add_talk $_npc$:
	var 20 @10
	MOD @20 10
	if @10 != 1 and @20 = 1
	add_talk #b#b�㵱ǰ����λ��-�þ���$@10$�㣬���"������ǰ���͵�",���Ա��浱ǰ�þ�λ��,�´������Դ������Ķ���,ֱ�Ӵ��͵���λ�á�
	option 100 #������ǰ���͵�
	else
	add_talk #b#b���Ǽ���֮��.Ҳ���ڻþ���ĳһ��,���ܰ��������...
	endif
	option 200 #����ʱ���
	talk	
endproc

proc 100
	save_unreal_layer 1
endproc

proc 200
	choose 0 201 202 ȷ������ʱ�����?
endproc

proc 201
	is_captain 20 0
	if @20 = 0
		TEAM_TRANS_INSAMEMAPINST 1000 40765 35339 0
	else
		TRANSMIT 1000 40765 35339 0
	endif
endproc

proc 202

endproc
