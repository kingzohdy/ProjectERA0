;;����Ѳ������

proc 1 

	strvar Ret Rad1 Rad2
	is_in_my_clan_city  Ret
	if Ret = 0 
		rand Rad1 2
		if Rad1 = 0
			mon_speech 0 0 0 1 ��ɲ�����ң����������ʵʵ�Ĵ��ţ�
		endif
		if Rad1 = 1
			mon_speech 0 0 0 1 ����ñ����鷳��
		endif
	endif	
	if Ret = 1
		rand Rad2 3
		if Rad2 = 0
			mon_speech 0 0 0 1 �������������ǵ����Σ�
		endif
		if Rad2 = 1
			mon_speech 0 0 0 1 ��������۸��㣬�����������ң�
		endif
		if Rad2 = 2
			mon_speech 0 0 0 1 ע����Щ�����ˣ�������ʱ���ܻᵷ�ң�
		endif
	endif
endproc 