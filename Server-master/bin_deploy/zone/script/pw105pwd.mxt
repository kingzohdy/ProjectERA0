;70����,���������npc
proc 1
	get_pworld_var 12 32 1 13
	if 13 != 0
		return
	endif
	DIALOG 1
;	add_talk $_npc$:
	add_talk #b#b#c676464<����ϸ�о���������������ĸ߿Ƽ����������ƺ�û��һ˿�����������������׿׵İ��ݶ�û�У�������һ��Īչʱ����������ӡ��һ��С��>#n
	add_talk #b#bϸ�ھ����ɰܣ�ע�������е�ÿһ��ϸ�ڣ���ͻ��ҵ��ɹ���Կ�ס�
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b#b#b#b#b����ޱޱ��
	option 101 #��ʼ��������
	talk
endproc
proc 101
	;�����׶�,��Ҫ����tickʱҪ������proc
	
	get_pworld_var 12 32 1 13
	if 13 = 0
		var 13 1
		set_pworld_var 12 32 @13 1 
		var 10 45
		new_mon_by_dir 261041 1 20638 29712 0 0 0 @10 1 2
	endif
	call 1
endproc
