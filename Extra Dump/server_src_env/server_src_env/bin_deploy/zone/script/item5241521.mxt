




proc 1


	; DEC_ITEM_NUM_INPAK 5241521 1 100
	; if 100 = 0

		;GET_PLAYER_ID 1 Mid
		;EFFECT_ID 0 @Mid playMontion(%d,"352116@reset")
		;strvar x y
		; GET_PLAYER_MAPINFO 1 1 1 x y
		; effect 0 0 playEffct($@x$,$@y$,0,"effect\\entity\\352116.ent",15)

	strvar mid map
	PLAYER_CALL_BUB 302166 300 mid
	; endif
	
	GET_PLAYER_MAPINFO 1 map 1 1 1 
	if map = 2000 or map = 2001 or map = 2002 or map = 2003 or map = 2005 or map = 2006 or map = 2007 or map = 2004
		
			rand 20 3
			if 20 = 0
				mon_speech @mid 1 2 1 �ۣ��Ҹղſ�����һ���ӣ���֪��������������
			endif
			if 20 = 1
				mon_speech @mid 1 2 1 ���ǻ᲻�᲻С�ĵ�������
			endif
			if 20 = 2
				mon_speech @mid 1 2 1 ������Ǵ�˵�е�������������һ�㶼���ף�
			endif
	endif
	if map = 2100 or map = 2101 or map = 2102 or map = 2103 or map = 2105 or map = 2106 or map = 2107 or map = 2108
		;����
		
		rand 20 3
		if 20 = 0
			mon_speech @mid 1 2 1 ��˵�����кܶ�Ұ�ˣ������Ұ����
		endif
		if 20 = 1
			mon_speech @mid 1 2 1 ��˵Ұ�˻��С���ӣ��������
		endif
		if 20 = 2
			mon_speech @mid 1 2 1 ���ǵ�����ȥ�����ɣ�˳�㲶���㡢Ϻʲô�ģ�
		endif
	endif
	
	if map = 2200
	;ѩԭ
		rand 20 4
		if 20 = 0
			mon_speech @mid 1 2 1 ��������䰡��
		endif
		if 20 = 1
			mon_speech @mid 1 2 1 �ö�ѩ����ʱ������겻��ѩ�أ�
		endif
		if 20 = 2
			mon_speech @mid 1 2 1 ������Ӧ�ô����������ģ�����ѩ����Ư������
		endif
		if 20 = 3
			mon_speech @mid 1 2 1 ����ȥ�ݷ���������Ѫ�����ɣ����ǻỶӭ���ǲ���
		endif
	endif
	
	if map = 2300 
	;ɳĮ
		rand 20 4
		if 20 = 0
			mon_speech @mid 1 2 1 �㿴�������ǻƽ��أ�
		endif
		if 20 = 1
			mon_speech @mid 1 2 1 ����ȥůȪ�򿴿��ɣ�˳��������Ȫ��
		endif
		if 20 = 2
			mon_speech @mid 1 2 1 ůȪ����ô��û����Ȫ�أ�
		endif
		if 20 = 3
			mon_speech @mid 1 2 1 ��Ŭ��˹�����Ǹ����ĺ��񹷵ľ�����
		endif
	endif
	
	if map = 2400
	;����
			
		rand 20 6
		if 20 = 0
			mon_speech @mid 1 2 1 ���ƺ���������Ĳ�̫һ������ô˵�أ��е������
		endif
		if 20 = 1
			mon_speech @mid 1 2 1 �㿴��Щ�ݣ����ǿݻƵģ����쵽����
		endif
		if 20 = 2
			mon_speech @mid 1 2 1 ʱ���һ�굽ͷ���Ǵ��죬û���ļ���û�뵽�����������ģ�
		endif
		if 20 = 3
			mon_speech @mid 1 2 1 ��Щ������������н����������ڿ������ɣ�
		endif
		if 20 = 4
			mon_speech @mid 1 2 1 ���ջ�Ұ��Ϧ�����᲻���ʱ��ǻ����أ����������������ɣ�
		endif
		if 20 = 5
			mon_speech @mid 1 2 1 �ƺ�������أ�������ȥ�����ɣ��㲻������¾�����ʲô��
		endif
	endif
	
	
	
	if map = 2500
	;Խ��
			rand 20 5
			if 20 = 0
				mon_speech @mid 1 2 1 �����������ʱ��ǲ�˶��١�
			endif
			if 20 = 1
				mon_speech @mid 1 2 1 �ۣ��öද������ǲ��ǿ��Ե��������ߣ�
			endif
			if 20 = 2
				mon_speech @mid 1 2 1 ������������ǲ����ȳԵ�ʲô�أ�
			endif
			if 20 = 3
				mon_speech @mid 1 2 1 �ף��Ҹղ��ƺ�����һ��С���ӡ����塭
			endif
			if 20 = 4
				mon_speech @mid 1 2 1 ��������������������Ϊʲô�Ұ�������겻�������أ�
			endif
	endif
	if map = 1000
	
	
		mon_speech @mid 1 2 1 �е�ʧ���أ�û�п���������Ҳû�п�������ˡ�
	endif
endproc 













