; �մ�


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b˭˵���˲���ӵ���˼�������ֻ�������ڻ������Ǹ������㣿
	add_talk #b#b��ϣ����һ�죬��Ҳ�ܳ�Ϊ�������ˡ����˺��ˣ��������İ���С����ͻ�����Ͷ���ҵĻ�����
	
		task_add_option 2132 1 100
		
		strvar Ret
		task_stat 2127 1 Ret
		if Ret = 2
			option 150 ������������������
		endif
		
		strvar Ret1
		task_stat 2109 1 Ret1
		if Ret1 = 2
			option 180 ��������˵����������

		endif
		
	talk
endproc

PROC 100
	task_do_option 2132 1
ENDPROC


proc 2
;	get_trig_task 1 2
;	if 2 = 2128
;		dialog 1
;		add_talk $_npc$:
;		add_talk #b#b
;			option 150 ����ѡ�����֣�
;		talk
;	endif
endproc

proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ˣ�$_player$����û���أ�����ʲô����

		option 151 �մ�С����������������

	talk

endproc 
proc 151
	dialog 1
	add_talk $_npc$:
	add_talk #b#bŶ���ⲻ�ո����������������ȥ�����أ���������

		option 152 ����ʲô��
	talk

endproc 
proc 152
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������Ȼ����ɫ�ģ�������ˮ������ɫ�ģ������ǵ�����һƿ��īˮһ�������������ھ����أ������ǳ��أ����ǳ��أ����ǳ��أ�

		option 153 �������
	talk

endproc 
proc 153
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�������ˣ���˵��Щ�ˣ�˵��Ҳû�ã������úã�����ȥ���մ�ɡ�

		option 154 #��л
	talk

endproc 
proc 154
	ADD_ITEM_NUM_INPAK 5300903 1 1 0
	return
endproc 

proc 180
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�Ǹ��ǣ�����̫�����뺷�ˡ��������Ұ�����ʲôû�Թ�����ϧ����û�Թ����⣬���һ��ǻ��������ˣ�

		option 181 ������

	talk
endproc 
proc 181
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��Ȼ��Ҷ�˵����ҥ���������Ҿ�����϶������£��㿴������
	add_talk #b#b<�����ӻ����ó�һ���Ѿ�������������ɢ������ζ�Ļ�����������Ů�����ķǳ���ˬ>
	add_talk #b#b�㿴����ЩͼƬ����ô��ʵ������
		option 182 ���Ļ����ܽ��ҿ�����

	talk
endproc 

proc 182
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��Ӵ����������Ӵ������컯�յģ��������Ǵӻ������������ɣ����������������ҿɰ���С�������֪�������ζ������һ�𡭡�

		option 183 #�һᾡ��黹�ģ�

	talk
endproc 
proc 183
	
	ADD_ITEM_NUM_INPAK 5300896 1 1 0
endproc 