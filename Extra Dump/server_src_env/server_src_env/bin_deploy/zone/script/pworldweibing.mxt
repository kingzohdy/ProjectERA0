;Ѳ������


proc 1
	
	strvar Rad1 Ret
	GET_ID_PLAYER_INFO 0 17 Ret
	if Ret = 0 or Ret = 1 or Ret = 3 or Ret = 4 or Ret = 5
		rand Rad1 4
		if Rad1 = 0
			mon_speech 0 0 0 1 �ǹܲ��õ�����ÿ�춼ҪѲ��վ��ʲô�ģ�ѹ���ô󰡣�
		elseif Rad1 = 1
			mon_speech 0 0 0 1 �㿴�����������ô��
		elseif Rad1 = 2
			mon_speech 0 0 0 1 �������������Ѱ��·��
		else
			mon_speech 0 0 0 1 ��Ҫ��Ϊ�����й䣬��ʵ������Ŀ�ĵģ�
		endif
	endif
		
		
	if Ret = 2
		strvar Rad2
		rand Rad2 4
		if Rad2 = 0
			mon_speech 0 0 0 1 �����ң������Ƿ��
		elseif Rad2 = 1
			mon_speech 0 0 0 1 ������Ҫ����ס���������ǹܵĺ���㶮�ģ�
		elseif Rad2 = 2
			mon_speech 0 0 0 1 ��������������ң����Ҳ��������㹥���ң�
		else
			mon_speech 0 0 0 1 �������������
		endif
	endif
	
endproc 	