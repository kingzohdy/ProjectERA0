
;С�̸�
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ˣ�$_player$�����ǵ����һ�νӴ���Ϸ��ʲôʱ��ô�����ǵõ����Ǹ������������ô�����ǵ����һ�α�����ʱ������ô��
	add_talk #b#b��ӭ�μӡ�#cb496ff������Щ�꣬����һ����Ϸ������#n��������ȣ�#c186506����Ҫ����һ������#n����#c186506�ӳ�#n��ӵ����Ʒ��ʱ��֮Կ��#c186506����·��ġ�Ѱ�һ��ѣ��ص����ꡱ���ӣ���Ա#n��ӵ����Ʒ������֮�֣�#c186506��������#n��˫�����ɽ��л��
;	add_talk #b#b�����˽����ϸ�ģ�������ϸ��˵���빥��Ӵ����������ϸ���ܡ�ѡ��鿴�ɡ�
		
		option 150 ���ϸ����
		
		strvar ret
		var 50 0
		IS_GAY_GONG ret
		if ret != 1
			strvar lv
			GET_PLAYER_BASE_ATT 0 0 lv 1
			if lv >= 40
				option 151 #Ѱ�һ��ѣ��ص�����
			else
				var 50 1
			endif
		endif
		if ret = 1

			option 200 #��ѯ������ԵĻ���
			
		;	TASKCYCLE_ADD_OPTION 130 1 120
		endif
		TASKCYCLE_ADD_OPTION 130 1 120
	talk
	if 50 = 1
		add_sys_msg_start
		add_sys_msg 40�����ϵ���Ҳ��ܹ�����������ԣ���������޵ȼ�
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc


PROC 120	
	TASKCYCLE_DO_OPTION 130 1
ENDPROC
proc 200
	dialog 1
	add_talk $_npc$:
	add_talk #b#b������Ե�����ǣ�$_GayShou$ 
	add_talk #b#b#c0181f9ע�������Ч��Ϊһ�죻��Գɹ����޷������������Գɹ������ۻ����ڷ񣬶����Խ������񣻽�������ʱ������������Ұ�ڣ�����˫�������ö���ľ��齱����#n
	
		option 1 ����
	talk
endproc 
proc 151
	;strvar tem ret
	;GET_TEAM_PLAYER_ID ret 0 0 0 0 0 0 0 0
	;if ret = 0
	;	IS_CAPTAIN tem 1
	;	if tem = 0
			GAY_INVITE 10
			
	;	else
	;		add_sys_msg_start
	;		add_sys_msg ��ѡ��ֻ�жӳ����ܵ�Ӵ��
	;		set_sys_msg_mode 4 0 0
	;		MSG_INFO
	;	endif
	;else
	;	add_sys_msg_start
	;	add_sys_msg ����Ҫ���ҵ�һ��Ը������ص�����Ļ��Ѳ���Ӵ��
	;	set_sys_msg_mode 4 0 0
	;	MSG_INFO
	;endif
endproc 

proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��ӭ�μӡ�#cb496ff������Щ�꣬����һ����Ϸ������#n��������ܽ���һ���ֻ���Ҫ�أ�ϣ����Ϊ����
	add_talk #b#b#c0181f9��������Ҫ���#n
	add_talk #b#b#c0181f9�ӳ�����ӵ����Ʒ��ʱ��֮Կ#n
	add_talk #b#b#c0181f9��Ա����ӵ����Ʒ������֮��#n

	add_talk #b#b�����Ч��Ϊһ�죬#c186506��Ч�����޷����#n�������ضԴ���
	add_talk #b#b��Գɹ������۱���ԵĶ�Ա����񣬶ӳ��Կɽ������񣡶ӳ���������ʱ��������Զ�Ա������Ұ�ڣ���˫�����޷���ö��⽱����
	
		option 160 ���Ƕӳ��ܵõ�ʲô�ô�������ô����
		option 161 ���Ƕ�Ա�ܵõ�ʲô�ô���Ҫ��ʲô��
		option 1 ����
	talk
endproc
proc 160
	dialog 1
	add_talk $_npc$:
	add_talk ��Ϊ�ӳ������ܻ�õĺô���
	add_talk #b#b������Ƕӳ�������ӵ����Ʒ��ʱ��֮Կ������Գɹ�������Խ��ܡ��ص����ꡱϵ������
	add_talk #b#b�������ʱ�����#c186506���������#n�������#c186506����ֵ��5000#nʱ����#c186506������һ�ݾ��齱��#n������ÿ�ν�����#c186506������Ե��������Ұ��#nʱ����#n������һ�ݽ���#n��
	add_talk #b#b
	add_talk ������ԣ�
	add_talk #b#b�����#cb496ffѰ�һ��ѣ��ص�����#n�����ӣ���������Ұ�ڵĶ������г�Ա����������룬��Աͬ��󣬿۳�˫����Ʒ����Գɹ���
	add_talk #b#bע���ӳ�������ֻ���������һ�Σ������Ч��Ϊһ�죬#c186506��Ч�����޷����#n�������ضԴ���

		option 161 ���Ƕ�Ա�ܵõ�ʲô�ô���Ҫ��ʲô��
		
		IS_GAY_GONG ret
		if ret != 1
			option 151 #Ѱ�һ��ѣ��ص�����
		endif
		option 150 ����

	talk
endproc 


proc 161
	dialog 1
	add_talk $_npc$:
	add_talk ��Ϊ��Ա�����ܻ�õĺô���
	add_talk #b#b������Ƕ�Ա������ӵ����Ʒ������֮�֣�����Գɹ��󣬶ӳ����С��ص����ꡱϵ������ʱ����Ҳ���ý�����	
	add_talk #b#b����˵���ǣ�����Գɹ�����ֻ��Ҫ����Ӹ���ӳ����ɣ��ǵģ�#c186506��ʲô������Ҫ����ֻҪ�ӳ������ص����ꡱϵ������ʱ����������ߣ���ͻ��ý�����#n
	add_talk #b
	add_talk ��������ԣ�
	add_talk #b#bֻ�жӳ��ܹ�������ԣ��ڶӳ� ��Ұ�ڵĶ�Ա���յ��ӳ������������룬��Ա���ͬ����ԣ��۳�˫����Ʒ����Գɹ���
	add_talk #b#bע���ӳ�������ʱ����һ��Ҫ�������Ӵ������˫����û�ж��⽱����

		
		option 160 ���Ƕӳ��ܵõ�ʲô�ô�������ô����
		option 150 ����
	
	talk
endproc 











 




