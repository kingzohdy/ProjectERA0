
;����
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b $_player$�����ѱ����Ԫ�ٷ�����Ϊ������ң�����������µĽ�����
	add_talk #b#b�ǻԱ��� 99��
	add_talk #b#b�û���ʵ 99��
	add_talk #b#b��Ʒ����-����
	add_talk #b#b��ȯ 3000
	add_talk #b#b�����������ӵ�½�����ҳ��ȡ������Ľ�Ʒ��
	add_talk #b#b���Ļ���֤��Ϊ��P41A�����μǡ�
	
	;	get_task_var 30013 0 8 1 20
	;	if 20 = 1
		
			option 150 ��������Ԫ�Ĺ�����
	;	endif
	talk
endproc
proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ҵ�Ȼ��������㣬#c186506���Ԫ�Ĺ����ǣ�http://www.ljy0.com #n
	add_talk #b#b�ٺ٣�������֪�������Ԫ�Ĺ��������ǾͿ������ҵ�ƭ������һ���ô#c186506ͨ������ľ���������н���վƭ����ϵ���Ȼ������ǵĺ��أ�#n

	add_talk #b#b<#049#049#049#049>
	add_talk #b#b���ǵ�������ô��������ƭ�˲����ˣ���ʹ���������ң����컹�ǻ��в������ϵ����ٺ٣��ҿ����о��еģ�

		option 151 ʲô���У�

	talk
endproc 

proc 151
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�Ҵ��������߱��ˣ����ǵľ�����#c186506ͨ���ʼ���˽�ĵ�ƭ���˵��½ĳ����վ�Ϳ��Ի�ü�Ʒ��������Ʒ����ǻԱ���99�����û���ʵ99������ȯ��ǧ����#n
	add_talk #b#b������������֪���ģ�
	add_talk #b#b<�Ҿ���ҪΪ������ˣ�>
	add_talk #b#b��ô���붯�֣������㣬�ҿɲ��£����а��֣�ľ���ú��к��к�����
	task_stat 30013 1 20
	if 20 = 2
		option 152 #���У�
	endif
	talk
endproc 
proc 152
	task_fini 30013 1
	NEW_LIFE_MON 241009 1 35400 35200 0 0 0 0 10 0 0
endproc 









