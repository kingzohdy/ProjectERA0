proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#�Կ������������ǻʼ�ѧԺһֱ����սǰ�ߵ���Ҫս�������ܵ����������˵ĸ߶ȹ�ע��
	add_talk #b#b#ֻҪ��#c186506�ﵽ60�������������ΪѧԺ�ĵ�ʦ#n�������������׵ĵ�ʦ�����ǽ�����Ծ��ԵĿ϶��뽱����
	add_talk #b#b#�����#c186506ֻ��30��59����Ҳ���Դ��������ҵ���ʦЭ���ɳ�#n��
	add_talk #b#b#��Ϊ�ʼ�ѧԺ�Ľ��ڣ����ڴ����ĵ�ϣ��ÿһλ��ʿ�ܹ�ӵ���Լ���һƬ��ա�

	strvar Ret Ret_1 MaxLV
	get_id_player_info 0 8 Ret
	GET_CUR_MAX_LEVEL Ret_1 MaxLV
	
	if @Ret > 59
		option 100 #��γ�Ϊ��ʦ��
	endif
	if @Ret_1 = 0 and @MaxLV > 59
		if @Ret > 29 and @Ret < 60
			option 300 #����ҵ���ʦ��
		endif
		option 200 #��ο��˱�ҵ��
	endif

	talk
endproc 

proc 100
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ҫ��Ϊһ����ʦ����������Ҫ�߱�#c186506�ﵽ60��#n�������ʵ����
	add_talk #b#b����Ϊ�˱�֤��ѧ��������ʦͬʱֻ������#c1865065��#nѧ����
	add_talk #b#b
	add_talk #b#b#c186506��ȷ������ʦ����ϵʱ˫�����ڳ���#n
	
	option 101 ��Ҫ����ѧ��
	option 102 �鿴ѧ���Ǽ���Ϣ
	option 103 ��ʦ��������
	option 1 �һ����ٶ��˽�
	
	talk
endproc 

proc 101
	SCHOOL_INVITE_UI 0
endproc	

proc 102
	GET_REG_INFO 0
endproc

proc 103
	DIALOG 1
	add_talk ��ʦ������
	add_talk #b#bֻҪѧ��#c186506�ɳ���10��#n�󣬴ﵽ#c18650645,50,54,56,58,60,63,65,70��#nʱ����ʦ���ɴ������յ�һ�ݽ�����
	add_talk #b#b��ѧ��#c186506�ﵽ65��#n�ɹ���ҵ�����ǽ����ʼ�һ���ҵ��������ʦ��ѧ����
	add_talk #b#b
	add_talk #b#b�����ѧ���ĸ�������á�#c186506���е���#n���Ŀ϶���ӵ�������������������ҷ��ġ�#c186506�ʼ�ѧԺ�����-�µ�#n�����ûʼ�ѧԺ#c186506ר����Ʒ#n��
	
		option 100 ���Ѿ��˽�

	talk
endproc

proc 200
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bֻҪѧ��#c186506�ﵽ65��#n������ѡ���Ƿ��ҵ��
	add_talk #b#b�ɹ����˱�ҵ�����ǽ����ʼ�һ���ҵ��������ʦ��ѧ����

		option 201 ���뿼�˱�ҵ
		option 1 �һ����ٶ��˽�

	talk
endproc
	
proc 201
	SCHOOL_FINI 0
endproc

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������#c18650630����59��#n֮�䣬���ǽ�������1λ��ʦ������������ĳɳ���ü���İ�����
	add_talk #b#b
	add_talk #b#b#c186506�����ͨ���·��ķ���Ѱ��ʦ��Ϣ������Ч��ݵ��ҵ���ʦ��#n

	option 301 ����Ѱ��ʦ��Ϣ��ѧ���Ǽǣ�
	option 102 �鿴ѧ���Ǽ���Ϣ
	option 302 ѧ����������
	option 1 �һ����ٶ��˽�

	talk
endproc
	
proc 301
	open_ui 86
endproc

proc 302
	DIALOG 1
	add_talk ѧ��������
	add_talk #b#bͨ����ʦ����ѧ��ѧ�����Ի���ɵ�ʦֱ�Ӵ��ڵľ���ֵ��
	add_talk #b#bֻҪѧ��#c186506����40����С����ߵȼ����10��#n��ÿ�컹����ȡ��#c186506��������#n��״̬��ô�������ֵ��
	add_talk #b#b��ѧ��#c186506�ﵽ65��#n�ɹ���ҵ�����ǽ����ʼ�һ���ҵ��������ʦ��ѧ����
	
	option 300 ���Ѿ��˽�
	
	talk
endproc
