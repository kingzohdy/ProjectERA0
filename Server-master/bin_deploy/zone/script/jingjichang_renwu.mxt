proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ֻ�Ӵ���ʿ��ֻ����������ʿ���ܵõ���ҵ��𾴣�ͬʱ�ҽ������Ǵ�����������������Ψһ�����ľ���ʤ����ֻ��ʤ�߲���������������Ȼ����Ҳ����ѡ���ӱܣ�����ԶԶ��!
	
	TASK_ADD_OPTION 3306 0 150
	
	DIANFENG_IF_OPEN 30
	if @30 = 1
		OPTION 201 #ĩ���۷�����������
		OPTION 200 ĩ���۷�������������ȡ
		OPTION 203 ĩ���۷���������������
	endif
	
	talk
endproc 

proc 150
	TASK_DO_OPTION 3306 0
endproc

proc 200
	strvar flag
	;������ȡ������
	DIANFENG_AWARD_CHECK  11 flag
	if @11 = 1 and @flag = 3
		add_sys_msg_start
		add_sys_msg ĩ���۷徺����������ȡ��
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	elseif @11 = 1 and @flag = 1
		GET_ID_PLAYER_INFO 0 7 whichcareer	
		if @whichcareer = 17
		;�ػ���
			CHOOSE 1 202 1 ��ϲ����á��ػ�����ھ���������#c00ffff���*100��ĩ������*4��10����������гƺš���������Ȩ��#n
		elseif @whichcareer = 33
		;������
			CHOOSE 1 202 1 ��ϲ����á���������ھ���������#c00ffff���*100��ĩ������*4��10����������гƺš���������Ȩ��#n
		elseif @whichcareer = 18
		;������
			CHOOSE 1 202 1 ��ϲ����á����ܡ���ھ���������#c00ffff���*100��ĩ������*4��10����������гƺš���������Ȩ��#n
		elseif @whichcareer = 34
		;����ʦ
			CHOOSE 1 202 1 ��ϲ����á����꡿��ھ���������#c00ffff���*100��ĩ������*4��10����������гƺš���������Ȩ��#n
		elseif @whichcareer = 19
		;��ɱ��
			CHOOSE 1 202 1 ��ϲ����á���ɱ����ھ���������#c00ffff���*100��ĩ������*4��10����������гƺš���������Ȩ��#n
		elseif @whichcareer = 35
		;������
			CHOOSE 1 202 1 ��ϲ����á�������ھ���������#c00ffff���*100��ĩ������*4��10����������гƺš���������Ȩ��#n
		elseif @whichcareer = 20
		;ʥ����
			CHOOSE 1 202 1 ��ϲ����á�ʥ�ӡ���ھ���������#c00ffff���*100��ĩ������*4��10����������гƺš���������Ȩ��#n
		elseif @whichcareer = 36
		;Ԧ��ʦ
			CHOOSE 1 202 1 ��ϲ����á�Ԧ�ס���ھ���������#c00ffff���*100��ĩ������*4��10����������гƺš���������Ȩ��#n
		else
			return 
		endif
	elseif @11 = 1 and @flag = 2
		CHOOSE 1 202 1 ��л����ĩ���۷���������֧�֣�������#c00ffff���*100��ĩ������*4��10�������#n
	else
		add_sys_msg_start
		add_sys_msg �ǳ��ź�����δ����ĩ���۷徺����û�п���ȡ������
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
endproc

proc 201
	DIANFENG_FIRST_INFO 0
endproc

proc 202
	strvar retflag
	;������ȡ������
	DIANFENG_AWARD 11 retflag
	if @11 = 1 and @retflag = 1
	;�ھ���������ֱ�Ӹ���
		return
	elseif @11 = 1 and @retflag = 2
	;������,������ֱ�Ӹ�
		return 
	elseif @11 = 1 and @retflag = 3
		add_sys_msg_start
		add_sys_msg ĩ���۷徺����������ȡ��
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	else
		add_sys_msg_start
		add_sys_msg �ǳ��ź�����δ����ĩ���۷徺����û�п���ȡ������
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
endproc

proc 203
	strvar retflag
	;���ɵ��������
	DIANFENG_FRONT_CHAMPION 10 retflag
	if @10 = 1 and @retflag = 1
		CHOOSE 1 204 1 ������Ҫ֧��10000���ң��Ƿ������
	else
		add_sys_msg_start
		add_sys_msg ֻ��ĩ���۷��ְҵ��ھ����ܽ��е������죬�㻹û�����Ұ��������Ȩ����֤���˺��������Ұɣ�
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
endproc

proc 204
	strvar whichcareer PosX PosY
	DEC_MONEY_TEST 0 10000 20 30
	if @30 = 0
		DEC_MONEY 0 10000 20 40
		if @40 = 0
			GET_ID_PLAYER_INFO 0 7 whichcareer	
			
			if @whichcareer = 17
			;�ػ���
				var PosX 25482
				var PosY 20909
			elseif @whichcareer = 33
			;������
				var PosX 27660
				var PosY 20909
			elseif @whichcareer = 18
			;������
				var PosX 25482
				var PosY 18867
			elseif @whichcareer = 34
			;����ʦ
				var PosX 27660
				var PosY 18868
			elseif @whichcareer = 19
			;��ɱ��
				var PosX 27658
				var PosY 19859
			elseif @whichcareer = 35
			;������
				var PosX 25482
				var PosY 19858
			elseif @whichcareer = 20
			;ʥ����
				var PosX 25482
				var PosY 17813
			elseif @whichcareer = 36
			;Ԧ��ʦ
				var PosX 27662
				var PosY 17818
			else
				return 
			endif
			
			;���õ���
			DIANFENG_SET_EFFIGY 1 1000 @PosX @PosY @whichcareer
		endif
	else
		add_sys_msg_start
		add_sys_msg ������Ҳ��㣬�޷�����
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
endproc



