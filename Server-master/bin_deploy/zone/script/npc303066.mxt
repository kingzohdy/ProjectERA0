;�����Ű�

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ף�Ϊ�˸����Ǵ��������¸����ᣬ�ҿ��������ṩ������λ�ĳ���Ӵ��
	add_talk #b#b����һ����������⣬������Ʒ���ñ��˿�������������ܡ�#c186506�������#n���أ��������Ǿ�����ʱ�����ⷢ��������ס�
	add_talk #b#b�������¿�ͷ�ѣ�����ֻҪ�ڷ�Ŭ�������϶��г�����Ķ�����������һ��Ҳ�����ҹ������Ӵ���ף�����ʲô�أ�
	add_talk #b#b
	add_talk #b#c186506����ﵽ20������ڴ����ޡ����̡�������Ʒ#n
	
	strvar LV
	GET_PLAYER_BASE_ATT 1 1 LV 1
	
	if @LV >= 20
		option 101 #���򿪵��̡�
		option 102 ���ġ����̱��䡷
	endif
	
	talk
endproc

proc 101
	strvar ret unl
	PKG_IS_UNLOCK ret unl
	if ret = 0 and unl = 1
		OPEN_STORE_UI 0
	endif
endproc

proc 102
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#c005ebe�����̱��䡷
	add_talk #b#b#b#b#b#b#b#b#b#bĿ¼#n
	add_talk #b#bȫ����������Ҫ�죬���������¸�֮·��
	
		option 201 ��һ�¡��������
		option 202 �ڶ��¡������ʲ���
		option 203 �����¡����̻��ܡ�
		option 204 �����¡�����������
		option 205 �����¡�����������
		option 1 ���Ѿ����յ���Ҫ��
	
	talk
endproc

proc 201
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b#c005ebe ��һ��
	add_talk #b#b#b#b#b#b#b#b���������#n
	add_talk #b#b#b#b#b#b�����޳��ṩ�������
	add_talk #b#b#b�������ߺ�����Կɱ�������������
	add_talk #b#b#b#b#b#b#b#b��������
	add_talk #b#b#b#b������Ʒ���ۣ�������Ʒ���롣
	add_talk #b#b#b#b�������޷�ʱ�����̴�����ɡ�
	add_talk #b#b#b#b���ʲ�#c18650648Сʱ#n�����̴����յꡣ
	add_talk #b#b#b#b���ʲ��յ�󣬴����˻���Ʒ��
	
		option 102 ���� - ���������Ŀ¼
	
	talk
endproc

proc 202
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b#c005ebe �ڶ���
	add_talk #b#b#b#b#b#b#b#b�������ʲ���#n
	add_talk #b#b#b#b�ʲ��غ��ŵ��̵������뷢չ��
	add_talk #b���ҿ����д����ʲ���ά�ֵ��̳��ڵ�������
	add_talk #b�ɹ�������Ʒ�����룬���Զ���������ʲ���
	add_talk #b���ҿɸ���������������д��ʲ�ȡ���ʽ�
	add_talk #b#b
    add_talk #b#b#b#b#b#b#b ����̸��ʲ���
	add_talk �ʲ�����֧�����޷�ʱ�����̽����ڸ��ʲ�������
	add_talk #b#b#b���ʲ�#c18650648Сʱ#n�󣬵��̽������յꡣ
	add_talk #b#b#b���������յ�����ʼ��˻���Ʒ��
	add_talk #b#b#b#b#b#b#b ��������޷ѡ�
	add_talk #b�����ÿ���#c18650660����#n���ʲ��Զ��������޷ѡ�
	add_talk #b#b#b#b ���̵����޷����г����ʸ���
	add_talk #b#b#b#b#b#b#b ����Ʒ���ܷѡ�
	add_talk #b#b�۳���Ʒ #c1865065%������ #n��������Ʒ�ı��ܷѡ�
	
		option 301 ��Ρ���ȡ�ʲ���
		option 102 ���� - ���������Ŀ¼
	
	talk
endproc

proc 301
	DIALOG 1
	add_talk �ʲ���ȡ������
	add_talk #b#b����������ġ�#c186506�ʲ�#n����ť�����ɽ����ʲ���ȡ����ز�����
	add_talk #b#b
	add_talk ��ȡע�����
	add_talk #b#b��ȡ�ʲ�ʱ����ȷ�����ϻ��ʲ����㹻��ȡ�����ҡ�
	
		option 202 ���� - �ڶ��¡������ʲ���
	
	talk
endproc

proc 203
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b#c005ebe ������
	add_talk #b#b#b#b#b#b#b#b�����̻��ܡ�#n
	add_talk #b#b#b ���ܹغ��ŵ��̿��ϼ���Ʒ��������
	add_talk #b#b#b#b#b#bÿ����̸���8����ܡ�
    add_talk #b#b#b#b#b#b#b ��������ܡ�
	add_talk #b���ҿ�����������ܣ����ӵ�����Ʒ��������
	add_talk #b#b#b���̵Ļ���Խ�࣬���޷Ѷ��Խ�ߡ�
	add_talk #b#b#b#b#b#b#b ��ʹ��Ȩ�ޡ�
	add_talk #b ����Ļ��ܿ��ڵ�ǰ���޵ĵ�������ʹ�á�
	add_talk #b#b#b#b#b#b#b �������ܡ�
	add_talk #b���ҿ����в�����ܣ����ص������޷Ѷ�ȡ�
	
		option 302 ��Ρ�������ܡ�
		option 303 ��Ρ�������ܡ�
		option 102 ���� - ���������Ŀ¼
	
	talk
endproc

proc 302
	DIALOG 1
	add_talk �������䷽����
	add_talk #b#b����������ġ�#c186506�������#n����ť�����ɽ���������ܵ���ز�����
	add_talk #b#b
	add_talk ����ע�����
	add_talk #b#b�������ʱ��Ҫ�ṩ��#c186506���������ͬ#n������ȷ��������Я����Ӧ�����ġ�#c186506���������ͬ#n����
	add_talk #b#bʱ��ǵġ����������ṩ��#c186506���������ͬ#n���ĳ��ۡ�
	
		option 203 ���� - �����¡����̻��ܡ�
	
	talk
endproc

proc 303
	DIALOG 1
	add_talk ���ܲ��������
	add_talk #b#b����������ġ�#c186506�������#n����ť�����ɽ��в�����ܵ���ز�����
	add_talk #b#b
	add_talk ���ע�����
	add_talk #b#b�������ʱ����ȷ������Ļ��������ϼܵ���Ʒ��
	
		option 203 ���� - �����¡����̻��ܡ�
	
	talk
endproc

proc 204
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b#c005ebe ������
	add_talk #b#b#b#b#b#b#b#b������������#n
	add_talk #b#b#b#b#b�����غ��ŵ��̵Ĺ�ע�ȡ�
    add_talk #b#b#b#b#b#b#b ���������ơ�
	add_talk #b#b#b#b��������Խ�ߣ���������Խ�ߡ�
	add_talk #b#b#b#b��������Խ�ߣ����̿���Խ�ߡ�
	add_talk #b#b#b#b#b#b#b ��Ӯȡ������
	add_talk #b#b#b#b#b ��������Ӱ�����������
	add_talk #b#b#b#b#b ����ʱ��Ӱ�����������
	add_talk #b#b#b#b#b �����ܶ�Ӱ�����������
	
		option 102 ���� - ���������Ŀ¼
	
	talk
endproc

proc 205
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b#c005ebe ������
	add_talk #b#b#b#b#b#b#b#b������������#n
	add_talk #b#b#b#b#b�����غ��ŵ��̵�֪���ȡ�
    add_talk #b#b#b#b#b#b#b ����������
	add_talk #b#b ���ҿ��ڵ��̵Ĺ��Ƶ�����������̡�
	add_talk #b#b#b#b#b#b#b �������ơ�
	add_talk #b#b �˿Ϳ�ͨ�����ҵĹ��ֱ�ӽ�����̡�
	
		option 304 ��Ρ��������̡�
		option 102 ���� - ���������Ŀ¼
	
	talk
endproc

proc 304
	DIALOG 1
	add_talk ��������������
	add_talk #b#b����������ġ�#c186506��������#n����ť�������ڵ��̵Ĺ��Ƶ�����������̡�
	add_talk #b#b
	add_talk ����ע�����
	add_talk #b#b��������ʱ��Ҫ�õ���#c186506С����#n������ȷ��������Я���㹻�ġ�#c186506С����#n����
	
		option 205 ���� - �����¡�����������
	
	talk
endproc
