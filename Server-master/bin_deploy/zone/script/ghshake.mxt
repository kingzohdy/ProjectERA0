

proc 1


	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ˣ�����������еľݵ������٣��ҿ��԰�������ҵ��
	add_talk #b#bսǰ׼�����ǵģ���û�п������ľ���������֯�ģ� ��ֻ���ھ�����ʤ�������ܽ��������ľݵ�ռ��Ȩ��
	add_talk #b#b����ս���������ң�ս����ÿ�ܶ�����20 ��30�ֿ����������ͨ���ҽ���ս�������ۻ��Ǵƣ��Ϳ� ��һ����
	add_talk #b#bʤ��������ʲô������ˮ��󣿵�Ȼ�Ǹ���ڵ��ˣ� ����ռ��������ݵ��Ժ����еĽ����������ҷ��ţ��� �ԸϿ��¸��¸�Ұ�


	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	strvar op1 op2 op3 op4 op5 op6
	
	STRONG_IS_OPEN 1 2202 op1
	STRONG_IS_OPEN 1 2200 op2
	STRONG_IS_OPEN 1 2300 op3
	STRONG_IS_OPEN 1 2400 op4
	STRONG_IS_OPEN 1 2500 op5
	STRONG_IS_OPEN 1 3404 op6

	strong_is_mine 1 2202 10
	strong_is_mine 1 2200 20
	strong_is_mine 1 2300 30
	strong_is_mine 1 2400 40
	strong_is_mine 1 2500 50
	strong_is_mine 1 3404 60	
		if map = 2202 and op1 = 1
			option 200 ����
			option 210 ����ݵ�ս��
			; option 220 ��ȡռ�콱��
			if 10 = 1
				option 400 �����ݵ�
			endif
		endif
		if map = 2200 and op2 = 1
			option 200 ����
			option 210 ����ݵ�ս��
			; option 220 ��ȡռ�콱��
			if 20 = 1
				option 400 �����ݵ�
			endif
		endif
		if map = 2300 and op3 = 1
			option 200 ����
			option 210 ����ݵ�ս��
			; option 220 ��ȡռ�콱��
			if 30 = 1
				option 400 �����ݵ�
			endif
		endif
		if map = 2400 and op4 = 1
			option 200 ����
			option 210 ����ݵ�ս��
			; option 220 ��ȡռ�콱��
			if 40 = 1
				option 400 �����ݵ�
			endif
		
		endif
		if map = 2500 and op5 = 1
			option 200 ����
			option 210 ����ݵ�ս��
			; option 220 ��ȡռ�콱��
			if 50 = 1
				option 400 �����ݵ�
			endif
		
		endif
		if map = 3404 and op6 = 1
			option 200 ����
			option 210 ����ݵ�ս��
			; option 220 ��ȡռ�콱��
			if 60 = 1
				option 400 �����ݵ�
			endif
		endif

		option 500 ���ᴫ��
	talk
endproc 
proc 200
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		strong_sale_info 1 2202
	endif
	if map = 2200
		strong_sale_info 1 2200
	endif
	if map = 2300
		strong_sale_info 1 2300
	endif
	if map = 2400
		strong_sale_info 1 2400
	endif
	if map = 2500
		strong_sale_info 1 2500
	endif
	if map = 3404
		strong_sale_info 1 3404
	endif


endproc
proc 210
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		IS_FIRST_STRONG 20 2202
		if 20 = 1
			choose 2 211 301 ���뵱ǰ�ݵ�ս���󣬱��ղ����ٽ��������ݵ�ս�����Ƿ�ȷ��ǰ����ǰս����
		else
			call 211
		endif
	endif
	if map = 2200
		IS_FIRST_STRONG 21 2200
		if 21 = 1
			choose 2 211 301 ���뵱ǰ�ݵ�ս���󣬱��ղ����ٽ��������ݵ�ս�����Ƿ�ȷ��ǰ����ǰս����
		else
			call 211
		endif
	endif
	if map = 2300
		IS_FIRST_STRONG 22 2300
		if 22 = 1
			choose 2 211 301 ���뵱ǰ�ݵ�ս���󣬱��ղ����ٽ��������ݵ�ս�����Ƿ�ȷ��ǰ����ǰս����
		else
			call 211
		endif
	endif
	if map = 2400
		IS_FIRST_STRONG 23 2400
		if 23 = 1
			choose 2 211 301 ���뵱ǰ�ݵ�ս���󣬱��ղ����ٽ��������ݵ�ս�����Ƿ�ȷ��ǰ����ǰս����
		else
			call 211
		endif
	endif
	if map = 2500
		IS_FIRST_STRONG 24 2500
		if 24 = 1
			choose 2 211 301 ���뵱ǰ�ݵ�ս���󣬱��ղ����ٽ��������ݵ�ս�����Ƿ�ȷ��ǰ����ǰս����
		else
			call 211
		endif
	endif
	if map = 3404
		IS_FIRST_STRONG 25 3404
		if 25 = 1
			choose 2 211 301 ���뵱ǰ�ݵ�ս���󣬱��ղ����ٽ��������ݵ�ս�����Ƿ�ȷ��ǰ����ǰս����
		else
			call 211
		endif
	endif
endproc
proc 301
endproc
proc 211
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		STRONG_ENTER 1 2202
	endif
	if map = 2200
		strong_enter 1 2200
	endif
	if map = 2300
		strong_enter 1 2300
	endif
	if map = 2400
		strong_enter 1 2400
	endif
	if map = 2500
		strong_enter 1 2500
	endif
	if map = 3404
		strong_enter 1 3404
	endif

endproc


proc 220
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������ɻ�ľ���̰�����ᣬ������֮һ���������� ��������еĽ���һ��ֻ����ȡһ�Σ�ע�⣡���ư��� ��

		option 300 ��ȡռ���ʽ�
	;	option 310 ��ȡ����
		option 320 ��ȡ����״̬
		option 330 ��ȡ���ṱ�׶�

	talk
endproc

proc 300
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		STRONG_PLAYER_AWARD 1 2202 1
	endif
	if map = 2200
		STRONG_PLAYER_AWARD 1 2200 1
	endif
	if map = 2300
		STRONG_PLAYER_AWARD 1 2300 1
	endif
	if map = 2400
		STRONG_PLAYER_AWARD 1 2400 1
	endif
	if map = 2500
		STRONG_PLAYER_AWARD 1 2500 1
	endif
	if map = 3404
		STRONG_PLAYER_AWARD 1 3404 1
	endif

endproc

proc 310
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		STRONG_PLAYER_AWARD 1 2202 2
	endif
	if map = 2200
		STRONG_PLAYER_AWARD 1 2200 2
	endif
	if map = 2300
		STRONG_PLAYER_AWARD 1 2300 2
	endif
	if map = 2400
		STRONG_PLAYER_AWARD 1 2400 2
	endif
	if map = 2500
		STRONG_PLAYER_AWARD 1 2500 2
	endif
	if map = 3404
		STRONG_PLAYER_AWARD 1 3404 2
	endif

endproc

proc 320
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		STRONG_PLAYER_AWARD 1 2202 3
	endif
	if map = 2200
		STRONG_PLAYER_AWARD 1 2200 3
	endif
	if map = 2300
		STRONG_PLAYER_AWARD 1 2300 3
	endif
	if map = 2400
		STRONG_PLAYER_AWARD 1 2400 3
	endif
	if map = 2500
		STRONG_PLAYER_AWARD 1 2500 3
	endif
	if map = 3404
		STRONG_PLAYER_AWARD 1 3404 3
	endif

endproc

proc 330
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		STRONG_PLAYER_AWARD 1 2202 4
	endif
	if map = 2200
		STRONG_PLAYER_AWARD 1 2200 4
	endif
	if map = 2300
		STRONG_PLAYER_AWARD 1 2300 4
	endif
	if map = 2400
		STRONG_PLAYER_AWARD 1 2400 4
	endif
	if map = 2500
		STRONG_PLAYER_AWARD 1 2500 4
	endif
	if map = 3404
		STRONG_PLAYER_AWARD 1 3404 4
	endif

endproc

proc 400
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		choose 2 401 402 �Ƿ�ȷ�Ϸ���[����]�ݵ�ռ��Ȩ 
	endif
	if map = 2200
		choose 2 401 402 �Ƿ�ȷ�Ϸ���[����ѩԭ]�ݵ�ռ��Ȩ 
	endif
	if map = 2300
		choose 2 401 402 �Ƿ�ȷ�Ϸ���[����ɳĮ]�ݵ�ռ��Ȩ 
	endif
	if map = 2400
		choose 2 401 402 �Ƿ�ȷ�Ϸ���[���ջ�Ұ]�ݵ�ռ��Ȩ 
	endif
	if map = 2500
		choose 2 401 402 �Ƿ�ȷ�Ϸ���[Խ������]�ݵ�ռ��Ȩ 
	endif
	if map = 3404
		choose 2 401 402 �Ƿ�ȷ�Ϸ���[��ɳ�ݵ�]�ݵ�ռ��Ȩ 
	endif	
endproc 
proc 402
	return
endproc 
proc 401
	strvar map
	GET_PLAYER_MAPINFO 1 map 0 0 0
	if map = 2202
		STRONG_CANCEL 1 2202 
	endif
	if map = 2200
		STRONG_CANCEL 1 2200
	endif
	if map = 2300
		STRONG_CANCEL 1 2300
	endif
	if map = 2400
		STRONG_CANCEL 1 2400
	endif
	if map = 2500
		STRONG_CANCEL 1 2500
	endif
	if map = 3404
		STRONG_CANCEL 1 3404
	endif	
endproc


proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����ԪԪ��ǰ���Ǵο�����ս�У��������������ᣬ���Ǵ�����������һ��һ�δ�ս���п������飬�Ӵ˱�����һ�����С�
	add_talk #b#bʱ�����գ���������ʷ�Ľ��棬�ܹ����������������Ĺ��ᣬ��Ȼ��Ϊ��ǿ��������������Ϊÿλ��ʿ���еġ��߶���������
	add_talk #b#b��ǿ�ߵ������ݲ��¶��ߣ�Ϊ�˳�Ϊ��ǿ�е���ǿ�����ݻ����˾�����������������ʵ�����֣������ֻ����һ��������ϰ���֮λ��
	add_talk #b#b
	add_talk #b#b#c186506ע�ͣ��߶�����Ϊ��ŷ����Ϊ��ӭ�ӡ�����Ļƻ衱֮ս���¶�Ϊ����Ӣ�۰��õĹ���
	
		option 501 �����˽⡰����������
		option 510 �����˽⡰����������
		option 520 �����˽⡰����������
		option 530 �����˽⡰����������
	
	talk
endproc


proc 501
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������Щռ���˾ݵ�Ĺ��ᣬ���ǽ����֮Ϊ�����������������������ֻ��ͬʱ��������������Ҳ����Ϊ����ǿ��������
	add_talk #b#b	
	add_talk #b#b�� ռ������ݵ㣺#c186506����������#n
	add_talk #b#b�� ռ������ѩԭ�ݵ㣺#c186506ѩԭ������#n
	add_talk #b#b�� ռ������ɳĮ�ݵ㣺#c186506ɳĮ������#n
	add_talk #b#b�� ռ�����ջ�Ұ�ݵ㣺#c186506��Ұ������#n
	add_talk #b#b�� ռ��Խ�����־ݵ㣺#c186506���֡�����#n
	add_talk #b#b�� ռ����ɳ�ݵؾݵ㣺#c186506��ɳ������
	
	;	option 300 �����˽⡰����������
		option 510 �����˽⡰����������
		option 520 �����˽⡰����������
		option 530 �����˽⡰����������
	
	talk
endproc
proc 510
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bֻ��ռ����һ���ݵ����ϵĹ�������ʸ񱻳�Ϊ������������Ȼ�����Ǳȡ���������ǿ�Ĺ���
	add_talk #b#b	
	add_talk #b#b�� ռ��2���ݵ㣺#c005ebe˫������#n
	add_talk #b#b�� ռ��3���ݵ㣺#c005ebe��������#n
	add_talk #b#b�� ռ��4���ݵ㣺#c005ebe��������#n
	add_talk #b#b�� ռ��5���ݵ㣺#c005ebe��������#n
	add_talk #b#b�� ռ��6���ݵ㣺#c005ebeȫ������

		option 501 �����˽⡰����������
	;	option 310 �����˽⡰����������
		option 520 �����˽⡰����������
		option 530 �����˽⡰����������
	
	talk
endproc 

proc 520
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ϊ��������ֻ��һ��Ҫ��˭��ռ��ʱ���˭���ǳ���������Ҳ��ζ�ų�����������ʣ�������ǿ����ľ���
	add_talk #b#b
	add_talk #b#b�� ռ��ʱ��ǣ�#c6d00d2ʱ�䡤����#n
	add_talk #b#b�� ռ��ʱ��Ǻ�1���ݵ㣺#c6d00d2����������#n
	add_talk #b#b�� ռ��ʱ��Ǻ�2���ݵ㣺#c6d00d2˫��������#n
	add_talk #b#b�� ռ��ʱ��Ǻ�3���ݵ㣺#c6d00d2����������#n
	add_talk #b#b�� ռ��ʱ��Ǻ�4���ݵ㣺#c6d00d2�ľ�������#n
	add_talk #b#b�� ռ��ʱ��Ǻ�5���ݵ㣺#c6d00d2�徳������

		option 501 �����˽⡰����������
		option 510 �����˽⡰����������
	;	option 320 �����˽⡰����������
		option 530 �����˽⡰����������
	
	
	talk
endproc 
proc 530
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ֻ��һ������������ʱ��������оݵ�Ĺ��ᣬ�����ʸ���ϰ����ı�����
	add_talk #b#b
	add_talk #b#b�� ռ��ʱ��Ǻ�6���ݵ㣺#cff4e00���硤����

		option 501 �����˽⡰����������
		option 510 �����˽⡰����������
		option 520 �����˽⡰����������
	;	option 330 �����˽⡰����������
	
	talk
endproc 






