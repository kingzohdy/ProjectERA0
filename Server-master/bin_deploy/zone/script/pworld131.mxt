;������� ,��������
;�����¼�
PROC 1	
	;���ø�������
	call 90
	;���ֹ�
	call 91
	;�����赲
	call 92
	add_sys_msg_start
	add_sys_msg ������ʼ
	set_sys_msg_mode 4 0 0
	map_msg_info	
ENDPROC

;�ر��¼�
proc 2
	add_sys_msg_start
	add_sys_msg ��������
	set_sys_msg_mode 4 0 0
	map_msg_info
endproc

;2��һ��
PROC 3
	;��ͼ���ܿ���
	get_pworld_var 4 32 1 11
	;��ʱ
	get_pworld_var 0 32 1 10
	add 10 1
	set_pworld_var 0 32 @10 1
	;1����
	mod 10 30
	if 10 = 0 and 11 = 1
		call 103   ;����������ʾ
	endif

	get_pworld_var 8 32 1 12
	;ˢ����
	;��������μ�ʱ
	get_pworld_var 12 32 1 13
		
	if 12 = 5 
		strvar ret num
		var ret 0
		
		GET_MON_NUM_BY_DEFID 205280 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205281 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205282 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205288 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205287 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205289 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205290 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205291 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205292 num
		add ret @num
		
		strvar time
		var time @13
		add 13 1
		set_pworld_var 12 32 @13 1
		
		;���������
		get_pworld_var 24 32 1 16
		strvar num 
		var num 2
		if 16 = 10
			var num 0
		endif

		if ret <= @num or time > 60
			;ˢ�����
			if num = 0 
				;��10��Ҫ��С��ȫ����
				if ret <= 0
					call 202 
				endif				
			else
				call 202
			endif			
		endif

	endif
	if 12 = 4
		call 201
		add 12 1
		;����ˢ�ֽ׶�
		set_pworld_var 8 32 @12 1
	endif
	
	;��̨��Ѫ��
	if 12 = 6
		call 204
		add 12 1
		
		set_pworld_var 8 32 @12 1
	endif
	
	;boss5�Ƿ��λ��
	get_pworld_var 68 32 1 27
	if 27 > 0
		call 301
		VAR 27 0
		set_pworld_var 68 32 @27 1
	endif
	
	;close28,�رյ���ʱ
	var 28 0
	get_pworld_var 72 32 1 28
	if 28 > 0
		add 28 1
		;close28,�رյ���ʱ		
		set_pworld_var 72 32 @28 1
	endif
	if 28 > 4
		add_sys_msg_start
		add_sys_msg �����ѹر�,��Ӵ������뿪 
		set_sys_msg_mode 1 0 0
		map_msg_info
		new_npc 1 333045 14729 13113 0 pw105out.mac
		pworld_over
	endif	
ENDPROC

PROC 4
	strvar x y id hp mid
	GET_MON_INFO 0 id 0 hp 0 0 x y
	
	;�ű�����
	If id = 205316
		call 101
	endif
	;boss4����
	if id = 205283 and hp < 1
		call 203
		
	endif
	;boss5����
	if id = 201554 and hp < 1
		call 301
	endif
ENDPROC
;���ø�������
proc 90
	;���ø���������
	;10��ʱ�� 
	var 10 0
	set_pworld_var 0 32 @10 1
	;��ͼ���ܿ���
	var 11 0
	set_pworld_var 4 32 @11 1
	;��������ؼ���
	VAR 12 0
	set_pworld_var 8 32 @12 1
	;��������μ�ʱ,����*���/2 = 10*30/2
	VAR 13 0
	set_pworld_var 12 32 @13 1
	;��̳��ʵ��id
	VAR 14 0
	set_pworld_var 16 32 @14 1
	;boss4��ʵ��id
	VAR 15 0
	set_pworld_var 20 32 @15 1
	;���������
	VAR 16 0
	set_pworld_var 24 32 @16 1
	;�߼�����1���,0δ������,1������
	VAR 17 0
	set_pworld_var 28 32 @17 1
	;��е1ˢ�±��,0δ������,1������
	VAR 18 0
	set_pworld_var 32 32 @18 1
	;��е2ˢ�±��,0δ������,1������
	VAR 19 0
	set_pworld_var 36 32 @19 1
	;��е3ˢ�±��,0δ������,1������
	VAR 20 0
	set_pworld_var 40 32 @20 1
	;��е1����λ��x
	VAR 21 0
	set_pworld_var 44 32 @21 1
	;��е1����λ��y
	VAR 22 0
	set_pworld_var 48 32 @22 1
	;��е2����λ��x
	VAR 23 0
	set_pworld_var 52 32 @23 1
	;��е2����λ��y
	VAR 24 0
	set_pworld_var 56 32 @24 1
	;��е3����λ��x
	VAR 25 0
	set_pworld_var 60 32 @25 1
	;��е3����λ��y
	VAR 26 0
	set_pworld_var 64 32 @26 1
	;boss5�Ƿ��λ��
	VAR 27 0
	set_pworld_var 68 32 @27 1
	;close28,�رյ���ʱ
	var 28 0
	set_pworld_var 72 32 @28 1
	;shadow29,��Ӱ������ʾ
	var 29 0
	set_pworld_var 76 32 @29 1
	;count30,�������
	var 30 0
	set_pworld_var 80 32 @30 1
	;level31,���Ƚ׶�
	var 31 1
	set_pworld_var 84 32 @31 1
	;shadow32,boss��Ӱ������ʾ
	var 32 0
	set_pworld_var 88 32 @32 1
	;pworld33,�Ƿ�pworldover��,1����û����,0������
	var 33 1
	set_pworld_var 92 32 @33 1
endproc

;���ֹ�
proc 91

	;�����ֵ�Ѳ�߶�
	new_mon 205276 1 11586 13476 0 203 0 1 2
	SET_MON_GROUP 1 @2 22
	new_mon 205274 1 11536 13476 0 204 0 1 2
	SET_MON_GROUP 1 @2 22
	new_mon 205275 1 11486 13476 0 205 0 1 2
	SET_MON_GROUP 1 @2 22

	;�����ֵ�Ѳ�߶�
	new_mon 205276 1 13442 16279 0 209 0 1 2
	SET_MON_GROUP 1 @2 24
	new_mon 205274 1 13392 16279 0 210 0 1 2
	SET_MON_GROUP 1 @2 24
	new_mon 205275 1 13342 16279 0 211 0 1 2
	SET_MON_GROUP 1 @2 24
	
	;�����ֵ�Ѳ�߶�
	new_mon 205276 1 10598 18712 0 215 0 1 2
	SET_MON_GROUP 1 @2 26
	new_mon 205274 1 10548 18712 0 216 0 1 2
	SET_MON_GROUP 1 @2 26
	new_mon 205275 1 10498 18712 0 217 0 1 2
	SET_MON_GROUP 1 @2 26

	;�����ֵ�Ѳ�߶�
	new_mon 205276 1 11702 19716 0 218 0 1 2
	SET_MON_GROUP 1 @2 27
	new_mon 205274 1 11652 19716 0 219 0 1 2
	SET_MON_GROUP 1 @2 27
	new_mon 205275 1 11602 19716 0 220 0 1 2
	SET_MON_GROUP 1 @2 27

	;�����ֵ�Ѳ�߶�
	new_mon 205276 1 15265 18895 0 224 0 1 2
	SET_MON_GROUP 1 @2 29
	new_mon 205274 1 15215 18895 0 225 0 1 2
	SET_MON_GROUP 1 @2 29
	new_mon 205275 1 15165 18895 0 226 0 1 2
	SET_MON_GROUP 1 @2 29
	
	;�����ֵ�Ѳ�߶�
	new_mon 205276 1 18846 17368 0 230 0 1 2
	SET_MON_GROUP 1 @2 31
	new_mon 205274 1 18796 17368 0 231 0 1 2
	SET_MON_GROUP 1 @2 31
	new_mon 205275 1 18746 17368 0 232 0 1 2
	SET_MON_GROUP 1 @2 31
	
	;����վ�ڵĹ�
	NEW_MON_BY_DIR	205273	1	10784	19165	0	0	0	35	1	2	;	�ڰ�ħʦ
	NEW_MON_BY_DIR	205273	1	8494	13862	0	0	0	139	1	2	;	�ڰ�ħʦ
	NEW_MON_BY_DIR	205273	1	8491	13598	0	0	0	139	1	2	;	�ڰ�ħʦ
	NEW_MON_BY_DIR	205273	1	14292	18929	0	0	0	41	1	2	;	�ڰ�ħʦ
	NEW_MON_BY_DIR	205271	1	14013	18923	0	0	0	31	1	2	;	�ڰ�ħ��
	NEW_MON_BY_DIR	205271	1	21087	15690	0	0	0	39	1	2	;	�ڰ�ħ��
	NEW_MON_BY_DIR	205271	1	20708	13913	0	0	0	31	1	2	;	�ڰ�ħ��
	NEW_MON_BY_DIR	205271	1	20880	15293	0	0	0	33	1	2	;	�ڰ�ħ��
	NEW_MON_BY_DIR	205270	1	8766	19036	0	0	0	40	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	8779	18824	0	0	0	41	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	13998	19144	0	0	0	24	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	14026	18719	0	0	0	35	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	15303	19467	0	0	0	27	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	22476	13990	0	0	0	45	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	20590	14276	0	0	0	36	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205272	1	9031	18968	0	0	0	51	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	205272	1	16743	19221	0	0	0	5	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	205272	1	15708	19136	0	0	0	27	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	205272	1	17849	19169	0	0	0	5	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	205272	1	9008	18749	0	0	0	47	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	205270	1	10483	18831	0	0	0	40	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	10421	19161	0	0	0	27	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205271	1	10600	18979	0	0	0	42	1	2	;	�ڰ�ħ��
	NEW_MON_BY_DIR	205273	1	10802	18896	0	0	0	38	1	2	;	�ڰ�ħʦ
	NEW_MON_BY_DIR	205271	1	10745	13276	0	0	0	120	1	2	;	�ڰ�ħ��
	NEW_MON_BY_DIR	205271	1	10753	13400	0	0	0	114	1	2	;	�ڰ�ħ��
	NEW_MON_BY_DIR	205271	1	9124	13828	0	0	0	121	1	2	;	�ڰ�ħ��
	NEW_MON_BY_DIR	205271	1	10747	13536	0	0	0	129	1	2	;	�ڰ�ħ��
	NEW_MON_BY_DIR	205271	1	9129	13498	0	0	0	126	1	2	;	�ڰ�ħ��
	NEW_MON_BY_DIR	205270	1	15502	19191	0	0	0	23	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	15684	18933	0	0	0	28	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	16358	19004	0	0	0	7	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	16743	19000	0	0	0	4	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	18254	18893	0	0	0	5	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	17941	18906	0	0	0	1	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205272	1	15550	19388	0	0	0	23	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	205272	1	16325	19210	0	0	0	20	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	205273	1	16517	19310	0	0	0	7	1	2	;	�ڰ�ħʦ
	NEW_MON_BY_DIR	205273	1	18120	19126	0	0	0	9	1	2	;	�ڰ�ħʦ
	NEW_MON_BY_DIR	205273	1	20738	14352	0	0	0	43	1	2	;	�ڰ�ħʦ
	NEW_MON_BY_DIR	205273	1	22938	14060	0	0	0	37	1	2	;	�ڰ�ħʦ
	NEW_MON_BY_DIR	205271	1	20775	14085	0	0	0	42	1	2	;	�ڰ�ħ��
	NEW_MON_BY_DIR	205270	1	20829	15691	0	0	0	40	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	20646	15401	0	0	0	41	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	22448	14275	0	0	0	40	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	20497	13963	0	0	0	42	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205272	1	21011	15473	0	0	0	50	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	205272	1	20753	15139	0	0	0	43	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	205272	1	22694	14382	0	0	0	47	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	205272	1	20971	13938	0	0	0	48	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	205272	1	22675	14188	0	0	0	49	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	205272	1	22682	13996	0	0	0	49	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	205273	1	20956	14174	0	0	0	41	1	2	;	�ڰ�ħʦ
	NEW_MON_BY_DIR	205273	1	22830	14194	0	0	0	39	1	2	;	�ڰ�ħʦ
	NEW_MON_BY_DIR	205270	1	17675	18913	0	0	0	0	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	16520	19084	0	0	0	0	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	8777	13993	0	0	0	135	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	8795	13725	0	0	0	135	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	8772	13425	0	0	0	135	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	10922	13491	0	0	0	135	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	10923	13323	0	0	0	135	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205272	1	10481	13437	0	0	0	135	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	205277	1	8088	13786	0	0	0	135	1	2	;	������
	NEW_MON_BY_DIR	205278	1	16535	19552	0	0	0	0	1	2	;	���ɶ�
	NEW_MON_BY_DIR	205279	1	22045	13644	0	0	0	76	1	2	;	������˹
	NEW_MON_BY_DIR	205310	1	7431	13821	0	0	0	1	1	2	;	ˮ�ƽ��
	NEW_MON_BY_DIR	205311	1	16483	20058	0	0	0	0	1	2	;	������
	NEW_MON_BY_DIR	205312	1	22050	13185	0	0	0	0	1	2	;	��Ӱ���
	NEW_MON_BY_DIR	205270	1	10708	16747	0	0	0	80	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	11173	16627	0	0	0	100	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205270	1	11595	16305	0	0	0	100	1	2	;	�ڰ�սʿ
	NEW_MON_BY_DIR	205271	1	10892	16506	0	0	0	93	1	2	;	�ڰ�ħ��
	NEW_MON_BY_DIR	205271	1	11319	16334	0	0	0	100	1	2	;	�ڰ�ħ��
	NEW_MON_BY_DIR	261058	1	14795	14792	0	0	0	0	1	2	;	��Ӱ����
	NEW_MON_BY_DIR	261058	1	15369	14189	0	0	0	45	1	2	;	��Ӱ����
	NEW_MON_BY_DIR	261058	1	14790	13674	0	0	0	0	1	2	;	��Ӱ����
	NEW_MON_BY_DIR	261058	1	14223	14207	0	0	0	45	1	2	;	��Ӱ����
	NEW_MON_BY_DIR	261059	1	14808	14228	0	0	0	0	1	2	;	�ڰ�����
	NEW_MON_BY_DIR	294031	1	14801	14276	0	0	0	0	1	2	;	��Ӱ��Ļ


	

endproc
;�����赲
proc 92
	var 10 0		;���泯�����ֵ
	new_mon_by_dir 261039 1 14700 12627 0 0 0 @10 1 2
	var 10 135		;���泯�����ֵ
	new_mon_by_dir 205316 1 6842 18794 0 0 0 @10 1 2
	new_mon_by_dir 294009 1 7006 18794 0 0 0 @10 1 2
endproc
;�ű���
proc 101
	;�ſڴ��ƺڰ�֮���¼�
	;ɾ�赲
	delete_mon_by_defid 294009 1
	;���ڱ�
	strvar m1 m2 dir
	var dir 45
	new_mon_by_dir 205298 1 7451 18913 0 0 0 @dir 1 m1
	new_mon_by_dir 205302 1 7446 19079 0 0 0 @dir 1 m2

	mon_speech @m2 2 2 1 "�ε�..����������,ִ�л������!!!"
	;��ͼ���ܿ���
	var 11 1
	set_pworld_var 4 32 @11 1
	;�㲥	
	add_sys_msg_start
	add_sys_msg ���ǵ��ж������˺ڰ����ļƻ��������ܽܿ�.���޵ķ�ŭ
	set_sys_msg_mode 4 0 0
	map_msg_info	
	new_npc 1 302120 7939 20085 180 p100spy1.mac	
	add_sys_msg_start
	add_sys_msg ����̽��Ա�����ǣ�������������̫����������鱨 
	set_sys_msg_mode 1 0 0
	map_msg_info
endproc
;ȡ��������
proc 102
	;���һ�����ŵ����,���·Ÿ���  
	strvar mid x y
	GET_RAND_PWORLD_PLAYER_ID 1 mid
	if 1 != 0
		return
	endif
	;���ڲ�����
	return
	GET_ID_PLAYER_INFO @mid 3 x
	GET_ID_PLAYER_INFO @mid 4 y
	new_mon 201032 1 @x @y 0 0 0 1 0
endproc

;��ʾ,��������
proc 103
;30���ж�һ�θ�����ǰ����,������ʾ:����������:��ǰ�׶�1/2,�ƻ����1/3��.
;һֱ��:����������:�׶�1/3,�ƻ����3/3�� ,
;���ؽ׶ο�����,��ʾ:����������:2/3.�ֵ��׼���1/10����ʧ������:�׼��߳ɹ��׼�0/10��
;���ʧ��,��ʾ����ʧ��,ɾ�����й���,ˢ��������,����ʱ��������
;����ɹ��ֵ�10����,���׼����׼�����<10����ˢ������BOSS,��ʾ:����������3/3,��ɱ������ 0/1.��
;��ɱ�ɹ�����ʾ:�� ��������3/3,��ɱ������ 1/1,������ɡ�
	;level31,���Ƚ׶�
	var 31 0
	get_pworld_var 84 32 1 31
	if 31 < 1
		return
	endif
	if 31 = 1
		strvar ret num
		var ret 0
		GET_MON_NUM_BY_DEFID 205310 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205311 num
		add ret @num
		GET_MON_NUM_BY_DEFID 205312 num
		add ret @num
		
		sub ret 3
		mul ret -1
		add_sys_msg_start
		add_sys_msg ��������1/3,�ƻ����$@ret$/3 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	;���������
	get_pworld_var 24 32 1 16
	if 31 = 2 and 16 > 0 and 16 < 11
		;count30,�������
		var 30 0
		get_pworld_var 80 32 1 30
		add_sys_msg_start
		add_sys_msg ��������2/3.�ֵ��׼���$@16$/10����ʧ������:�׼��߳ɹ��׼�$@30$/10 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 31 = 3
		add_sys_msg_start
		add_sys_msg ��������3/3,��ɱ������0/1 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 31 = 4
		add_sys_msg_start
		add_sys_msg ��������3/3,��ɱ������1/1,������� 
		set_sys_msg_mode 1 0 0
		map_msg_info
		var 31 0
		;level31,���Ƚ׶�
		var 31 31
		set_pworld_var 84 32 @31 1
	endif
endproc

;�ɼ��￪����ȫ��,����2�׶�
proc 201
	;ɾnpc
	DEL_NPC 306023 1	
	;��boss
;	new_mon 205283 1 14851 14163 0 0 0 1 2
;	mon_speech @2 2 2 1 ����������,���Ҵ������ٻ�ΰ���Ӱ.�ܿ�.���޵Ľ���,�����ҵ�ŭ���!
	
	NEW_MON_BY_DIR 333049 1 11131 13657 0 0 0 45 1 0
	NEW_MON_BY_DIR 333049 1 18465 14930 0 0 0 45 1 0
	
	;level31,���Ƚ׶�
	var 31 2
	set_pworld_var 84 32 @31 1
endproc
;�����,��
proc 202
	;���������
	get_pworld_var 24 32 1 16
	add 16 1
	set_pworld_var 24 32 @16 1
	if 16 < 11
		add_sys_msg_start
		add_sys_msg ��$@16$�������߹����ˣ���ס��̳����ֹ���ǵ��׼��� 
		set_sys_msg_mode 1 0 0
		map_msg_info
;		���	13740	14011	�ұ�	15843	14219
		effect 0 0 playEffct(13740,14011,"effect\\entity\\9571_16.ent")
		effect 0 0 playEffct(15843,14219,"effect\\entity\\9569_16.ent")
	endif
	
	if 16 = 1	
		new_mon 205280 1 11362 13656 0 1040 0 0 0
		new_mon 205280 1 11274 13818 0 1041 0 0 0
		new_mon 205280 1 11275 13517 0 1042 0 0 0
		new_mon 205280 1 18210 14852 0 1050 0 0 0
		new_mon 205280 1 18310 15022 0 1051 0 0 0
		new_mon 205280 1 18409 14817 0 1052 0 0 0
	endif
	if 16 = 2
		new_mon 205289 1 11362 13656 0 1040 0 0 0
		new_mon 205289 1 11274 13818 0 1041 0 0 0
		new_mon 205289 1 11275 13517 0 1042 0 0 0
		new_mon 205289 1 18210 14852 0 1050 0 0 0
		new_mon 205289 1 18310 15022 0 1051 0 0 0
		new_mon 205289 1 18409 14817 0 1052 0 0 0
	endif
	if 16 = 3
		new_mon 205280 1 11362 13656 0 1040 0 0 0
		new_mon 205280 1 11274 13818 0 1041 0 0 0
		new_mon 205280 1 11275 13517 0 1042 0 0 0
		new_mon 205288 1 11230 13682 0 1043 0 0 0
		new_mon 205280 1 18210 14852 0 1050 0 0 0
		new_mon 205280 1 18310 15022 0 1051 0 0 0
		new_mon 205280 1 18409 14817 0 1052 0 0 0
		new_mon 205288 1 18154 15044 0 1053 0 0 0
	endif
	if 16 = 4
		new_mon 205290 1 11362 13656 0 1040 0 0 0
		new_mon 205290 1 11274 13818 0 1041 0 0 0
		new_mon 205288 1 11275 13517 0 1042 0 0 0
		new_mon 205288 1 11230 13682 0 1043 0 0 0
		new_mon 205290 1 18210 14852 0 1050 0 0 0
		new_mon 205290 1 18310 15022 0 1051 0 0 0
		new_mon 205288 1 18409 14817 0 1052 0 0 0
		new_mon 205288 1 18154 15044 0 1053 0 0 0
	endif
	if 16 = 5
		new_mon 205280 1 11362 13656 0 1040 0 0 0
		new_mon 205280 1 11274 13818 0 1041 0 0 0
		new_mon 205280 1 11275 13517 0 1042 0 0 0
		new_mon 205288 1 11230 13682 0 1043 0 0 0
		new_mon 205281 1 11420 13505 0 1044 0 0 0
		new_mon 205280 1 18210 14852 0 1050 0 0 0
		new_mon 205280 1 18310 15022 0 1051 0 0 0
		new_mon 205280 1 18409 14817 0 1052 0 0 0
		new_mon 205288 1 18154 15044 0 1053 0 0 0
		new_mon 205281 1 18311 14723 0 1054 0 0 0
	endif
	if 16 = 6	
		new_mon 205291 1 11362 13656 0 1040 0 0 0
		new_mon 205291 1 11274 13818 0 1041 0 0 0
		new_mon 205280 1 11275 13517 0 1042 0 0 0
		new_mon 205280 1 11230 13682 0 1043 0 0 0
		new_mon 205288 1 11420 13505 0 1044 0 0 0
		new_mon 205291 1 18210 14852 0 1050 0 0 0
		new_mon 205291 1 18310 15022 0 1051 0 0 0
		new_mon 205280 1 18409 14817 0 1052 0 0 0
		new_mon 205280 1 18154 15044 0 1053 0 0 0
		new_mon 205288 1 18311 14723 0 1054 0 0 0
	endif
	if 16 = 7
		new_mon 205280 1 11362 13656 0 1040 0 0 0
		new_mon 205280 1 11274 13818 0 1041 0 0 0
		new_mon 205280 1 11275 13517 0 1042 0 0 0
		new_mon 205288 1 11230 13682 0 1043 0 0 0
		new_mon 205288 1 11420 13505 0 1044 0 0 0
		new_mon 205280 1 18210 14852 0 1050 0 0 0
		new_mon 205280 1 18310 15022 0 1051 0 0 0
		new_mon 205280 1 18409 14817 0 1052 0 0 0
		new_mon 205288 1 18154 15044 0 1053 0 0 0
		new_mon 205288 1 18311 14723 0 1054 0 0 0
	endif
	if 16 = 8
		new_mon 205292 1 11362 13656 0 1040 0 0 0
		new_mon 205292 1 11274 13818 0 1041 0 0 0
		new_mon 205292 1 11275 13517 0 1042 0 0 0
		new_mon 205280 1 11230 13682 0 1043 0 0 0
		new_mon 205280 1 11420 13505 0 1044 0 0 0
		new_mon 205292 1 18210 14852 0 1050 0 0 0
		new_mon 205292 1 18310 15022 0 1051 0 0 0
		new_mon 205292 1 18409 14817 0 1052 0 0 0
		new_mon 205280 1 18154 15044 0 1053 0 0 0
		new_mon 205280 1 18311 14723 0 1054 0 0 0
	endif
	if 16 = 9
		new_mon 205280 1 11362 13656 0 1040 0 0 0
		new_mon 205280 1 11274 13818 0 1041 0 0 0
		new_mon 205280 1 11275 13517 0 1042 0 0 0
		new_mon 205288 1 11230 13682 0 1043 0 0 0
		new_mon 205282 1 11420 13505 0 1044 0 0 0
		new_mon 205280 1 18210 14852 0 1050 0 0 0
		new_mon 205280 1 18310 15022 0 1051 0 0 0
		new_mon 205280 1 18409 14817 0 1052 0 0 0
		new_mon 205288 1 18154 15044 0 1053 0 0 0
		new_mon 205282 1 18311 14723 0 1054 0 0 0
	endif
	if 16 = 10
		
		new_mon 205289 1 11362 13656 0 1040 0 0 0
		new_mon 205291 1 11274 13818 0 1041 0 0 0
		new_mon 205292 1 11275 13517 0 1042 0 0 0
		new_mon 205289 1 18210 14852 0 1050 0 0 0
		new_mon 205291 1 18310 15022 0 1051 0 0 0
		new_mon 205292 1 18409 14817 0 1052 0 0 0
		
	endif
	if 16 = 11
		;		14754,15108
		effect 0 0 playEffct(14754,15108,"effect\\entity\\9570_16.ent")
		NEW_MON_BY_DIR 333049 1 14687 15884 0 0 0 0 1 0
		new_mon 205287 1 14687 15884 0 1060 0 0 0
		
		var 13 0
		set_pworld_var 12 32 @13 1
		;level31,���Ƚ׶�
		var 31 3
		set_pworld_var 84 32 @31 1
	endif
	
	if 16 = 1
		add_sys_msg_start
		add_sys_msg ��ֹʧ����ͽ����ڰ���̳�� 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 2
		add_sys_msg_start
		add_sys_msg ��ֹ������ͽ����ڰ���̳��С�ģ������ܵ��ر�죡 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 3
		add_sys_msg_start
		add_sys_msg ��ֹʧ����ͽ����ڰ���̳��С�����ǵĵֿ��� 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 4
		add_sys_msg_start
		add_sys_msg ��ֹ�ᶨ��ͽ����ڰ���̳��ȫ���������ǣ� 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 5
		add_sys_msg_start
		add_sys_msg ��ֹʧ����ͽ����ڰ���̳��С�ĳ�����ͽ��Ϯ���� 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 6
		add_sys_msg_start
		add_sys_msg ��ֹ������ͽ����ڰ���̳��С�������Ա�ͬ���ھ��� 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 7
		add_sys_msg_start
		add_sys_msg ��ֹʧ����ͽ����ڰ���̳�� 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 8
		add_sys_msg_start
		add_sys_msg ��ֹѳ����ͽ����ڰ���̳��һ��ҪС�ģ����Ƿǳ�ǿ�� 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 9
		add_sys_msg_start
		add_sys_msg ��ֹʧ����ͽ����ڰ���̳��С������ͽ��Ϯ���� 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	if 16 = 10
		add_sys_msg_start
		add_sys_msg ��ֹ������ͽ,������ͽ,ѳ����ͽ�����̳,С�����ǵ���������!  
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	
	if 16 = 11
		add_sys_msg_start
		add_sys_msg �ɹ��ճ�����������ס��̳��ȫ��һ���� 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	;2���Ӽ�ʱ
	VAR 13 0
	set_pworld_var 12 32 @13 1
endproc

;ˢ�����,��
proc 802
	
	;���������
	get_pworld_var 24 32 1 16
	add 16 1
	set_pworld_var 24 32 @16 1
	new_mon 205280 1 11362 13656 0 240 0 1 0
	new_mon 205280 1 11274 13818 0 241 0 1 0
	new_mon 205280 1 11275 13517 0 242 0 1 0
	new_mon 205280 1 18210 14852 0 250 0 1 0
	new_mon 205280 1 18310 15022 0 251 0 1 0
	new_mon 205280 1 18409 14817 0 252 0 1 0
	NEW_TMP_ITEM 1420004 14802 14172 0 0 0 1
	NEW_TMP_ITEM 1400004 15254 14555 0 0 0 1
	NEW_TMP_ITEM 1400101 14345 14602 0 0 0 1
	;3,4
	if 16 >= 3
		new_mon 205288 1 11230 13682 0 243 0 1 0
		new_mon 205288 1 18154 15044 0 253 0 1 0
	endif
	if 16 = 5
		new_mon 205281 1 11420 13505 0 244 0 1 0
		new_mon 205281 1 18311 14723 0 254 0 1 0
	endif
	if 16 > 5 and 16 < 9
		new_mon 205288 1 11420 13505 0 244 0 1 0
		new_mon 205288 1 18311 14723 0 254 0 1 0
	endif
	if 16 >= 9
		new_mon 205288 1 11420 13505 0 244 0 1 0
		new_mon 205282 1 18311 14723 0 254 0 1 0
	endif
	;10��
	if 16 = 10
		new_mon 205287 1 14687 15884 0 260 0 1 0
;		new_mon 205280 1 11046 13628 0 ::patrol36:: 0 1 0
;		new_mon 205280 1 15026 17338 0 ::patrol37:: 0 1 0
;		new_mon 205280 1 19099 14424 0 ::patrol38:: 0 1 0
;		
;		;һ����ӳ����
;		new_mon 205282 1 19099 14424 0 ::patrol38:: 0 1 0
;		;ɾ����,������
;		DELETE_MON_BY_DEFID 205283 1
;		new_mon 205287 1 14851 14163 0 0 0 1 2
	endif
;	add 16 1
;	;��10��Ԥ��
;	if 16 = 10
;		add_sys_msg_start
;		add_sys_msg ����ӳ�����ĵ�$@16$����������꽫��30������
;		set_sys_msg_mode 4 0 0
;		map_msg_info
;	endif
;	var 17 @16
;	mod 16 3
;	;3,6,9��Ԥ��
;	if 16 = 0
;		add_sys_msg_start
;		add_sys_msg ���С�ӳ�����ĵ�$@17$����������꽫��30������
;		set_sys_msg_mode 4 0 0
;		map_msg_info	
;	endif
;
;	sub 16 1
;	mod 16 3
;	;3,6,9��
;	if 16 = 0
;		
;		;һ��С�ӳ����
;		new_mon 205281 1 15026 17338 0 ::patrol38:: 0 1 0
;	endif
endproc
;boss4����
proc 203
	;������
	return
	;ֹͣ�����
	;��������ؼ���
	var 12 0
	set_pworld_var 8 32 @12 1
	;ɾ�赲
	delete_mon_by_defid 261039 1
	;�ŵ�Ӱ
	PLAYER_MOVIE_START 1 0
	
	new_npc 1 302121 14729 13069 218 p100spy2.mac	
endproc

;��̨��Ѫ��,��
proc 204
	;ɾboss
	DELETE_MON_BY_DEFID 205283 1
	;ɾ��̨
	DELETE_MON_BY_DEFID 294043 1
	;ɾ������
	DELETE_MON_BY_DEFID 333049 1
	;��������ؼ���
	VAR 12 0
	set_pworld_var 8 32 @12 1
	;��������μ�ʱ,����*���/2 = 10*30/2
	VAR 13 0
	set_pworld_var 12 32 @13 1
	;��boss6	
;	new_mon 263007 1 14851 14163 0 0 0 1 2
	;count30,�������
	var 30 0
	get_pworld_var 80 32 1 30
	if 30 > 10
		var 30 10
	endif
	add_sys_msg_start
	add_sys_msg $@30$�������߳ɹ�����ڰ���̳�׼����ж�ʧ�� 
	set_sys_msg_mode 1 0 0
	map_msg_info
	;����ʱ
	;close28,�رյ���ʱ
	var 28 1
	set_pworld_var 72 32 @28 1
endproc
;��̨��Ѫ��,��
proc 205
	;ɾboss
	DELETE_MON_BY_DEFID 205283 1
	;ɾ��̨
	DELETE_MON_BY_DEFID 294043 1
	;��������ؼ���
	VAR 12 0
	set_pworld_var 8 32 @12 1
	;��������μ�ʱ,����*���/2 = 10*30/2
	VAR 13 60
	set_pworld_var 12 32 @13 1
	;��npc
	;����̨
	;��3���ɼ��￪��
	;npc����롪��˹��.������
	NEW_NPC 1 306023 14851 14163 0 pw100boss4npc.mac
	;��̨����
	new_mon 294043 1 14773 14014 0 0 0 1 2
	;�вɼ��￪��
	new_mon 201534 1 15571 13459 0 0 0 1 2
	new_mon 201535 1 14039 13457 0 0 0 1 2
	new_mon 201536 1 15563 14985 0 0 0 1 2
endproc
;3�׶�����
proc 301
;	add_sys_msg_start
;	add_sys_msg ����
;	set_sys_msg_mode 4 0 0
;	map_msg_info	
	VAR 17 0
	set_pworld_var 28 32 @17 1
	delete_mon_by_defid 201556 1
	delete_mon_by_defid 201557 1
	delete_mon_by_defid 201558 1
	;��е1ˢ�±��,0δ������,1������
	get_pworld_var 32 32 1 18
	;��е2ˢ�±��,0δ������,1������
	get_pworld_var 36 32 1 19
	;��е3ˢ�±��,0δ������,1������
	get_pworld_var 40 32 1 20
	strvar x y
	if 18 > 0
		;��е1����λ��x
		get_pworld_var 44 32 1 x
		;��е1����λ��y
		get_pworld_var 48 32 1 y
		NEW_MON_MACHINE 1 201556 0 1 @x @y 0
	endif
	
	if 19 > 0 
		;��е2����λ��x
		get_pworld_var 52 32 1 x
		;��е2����λ��y
		get_pworld_var 56 32 1 y
		NEW_MON_MACHINE 1 201557 0 1 @x @y 0
	endif
	if 20 > 0 
		;��е3����λ��x
		get_pworld_var 60 32 1 x
		;��е3����λ��y
		get_pworld_var 64 32 1 y
		NEW_MON_MACHINE 1 201558 0 1 @x @y 0
	endif
	
endproc
proc 302
	NEW_NPC 1 333045 17687 5000 0 jzsdgozc.mac
endproc
;���ֵ���,��ɾ��
PROC 10
	pworld_again
ENDPROC
proc 11
        OUT_PWORLD_BY_SINGLE
endproc
PROC 12
	pworld_over
ENDPROC
proc 13
	delete_mon_by_defid 294009 1
	delete_mon_by_defid 261039 1
endproc
proc 14 
	;��������ؼ���
	VAR 12 3
	set_pworld_var 8 32 @12 1
	;��������μ�ʱ,����*���/2 = 10*30/2
	VAR 13 0
	set_pworld_var 12 32 @13 1
endproc
proc 15
	;10��ʱ��
	var 10 0
	var 11 0
	var 12 0
	var 13 0
	get_pworld_var 0 32 1 10
	;��ͼ���ܿ���
	get_pworld_var 4 32 1 11
	;��������ؼ���
	get_pworld_var 8 32 1 12
	;��������μ�ʱ,����*���/2 = 10*30/2
	get_pworld_var 12 32 1 13
	add_sys_msg_start
	add_sys_msg ��ʱ��$@10$ ��ͼ���ܿ���$@11$ ��������ؼ���$@12$ ��������μ�ʱ$@13$ 
	set_sys_msg_mode 4 0 0
	map_msg_info
endproc
proc 16
	;����̨��Ѫ
	;��̳��ʵ��id
	get_pworld_var 16 32 1 14
	mon_set_hp 20000 @14 1
endproc
proc 17
	;count30,�������
	var 30 0
	get_pworld_var 80 32 1 30
	add 30 1
	set_pworld_var 80 32 @30 1
	delete_mon_by_defid 261059 1
	delete_mon_by_defid 261060 1
	delete_mon_by_defid 261061 1
	delete_mon_by_defid 261062 1
	delete_mon_by_defid 261063 1
	delete_mon_by_defid 261064 1
	delete_mon_by_defid 261065 1
	delete_mon_by_defid 261066 1
	delete_mon_by_defid 261067 1
	delete_mon_by_defid 261068 1
	delete_mon_by_defid 261069 1
	if 30 < 10
		add_sys_msg_start
		add_sys_msg $@30$�������߳ɹ�����ڰ���̳�׼��� 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	get_pworld_var 8 32 1 12
	if 30 >= 10 and 12 != 6
		;��������ؼ���
		var 12 6
		set_pworld_var 8 32 @12 1
	endif
	strvar id 
	var id 261059
	if 30 > 0 and 30 <= 10
		add id @30
		new_mon @id 1 14808 14228 0 0 0 1 0
	endif
	
	
endproc