;
;script filename: npc3
;
PROC 1
    DIALOG 0
    ADD_TALK $_npc$��
    ADD_TALK ��ã�����$_npc$���Ǹ����������ġ�
    ADD_TALK �����ǵ���������ʲô����ְҵ���С�
    NEXT 20
    TALK
    
ENDPROC
;
;
PROC 20
    DIALOG 1
    ADD_TALK $_npc$��
    ADD_TALK ���Ǹոյ��������İɣ�
    OPTION 21 �ǵġ�
    OPTION 22 ���ǡ�
    TALK
ENDPROC
;
;
;���ѡ��������
PROC 21
    DIALOG 0
    ADD_TALK $_npc$��
    ADD_TALK �Ҿ�֪���������������������ġ���һ�������
    ADD_TALK �����Ϊʲô�һ�֪������ʵ������Ϊ�Ҹղſ�
    ADD_TALK ���ܶ�İ���ˣ����Ƕ�˵�������ģ������Ҳ���
    ADD_TALK Ҳ�ǵġ�
    NEXT 23
    TALK
    
ENDPROC
;
;
;���ѡ����������
PROC 22
    DIALOG 1
    ADD_TALK $_npc$��
    ADD_TALK Ŷ��Ϊʲô����ǰû�п����㣿
    ADD_TALK ��������Ҷ���ʶ�ġ���û��ƭ�Ұɣ�
    OPTION 28 û��ƭ�㣬����Ĳ��������ġ�
    OPTION 29 �������������㿴�����ˡ�
    TALK
ENDPROC
;
;
PROC 23
    DIALOG 1
    ADD_TALK $_npc$��
    ADD_TALK �㲻������Һ����°ɣ�
    OPTION 24 �����������ܽ�̸��
    OPTION 25 �е�����Ŷ��
    OPTION 26 �뿪��
    TALK
ENDPROC
;
;
;ѡ�ܽ�̸
PROC 24
    DIALOG 0
    ADD_TALK $_npc$��
    ADD_TALK �Ǻǣ��佱��Ҳ����ʹ���µĴ�����Ѿ�����
    ADD_TALK NN������ˣ�����������лл�㡣
    NEXT 27
    TALK
    
ENDPROC
;
;ѡ������
PROC 25
    DIALOG 3
    ADD_TALK $_npc$��
    ADD_TALK �����ģ����ء�
    TALK
ENDPROC
;
;ѡ�뿪
PROC 26
    DIALOG 3
    ADD_TALK $_npc$��
    ADD_TALK �ټ����ټ���
    TALK
ENDPROC
;
;
PROC 27
    DIALOG 3
    ADD_TALK $_npc$��
    ADD_TALK лл�㡣
    TALK
ENDPROC
;
;
;OPTION 28 û��ƭ�㣬����Ĳ��������ġ�
PROC 28
    DIALOG 3
    ADD_TALK $_npc$��
    ADD_TALK Ŷ��û��ƭ�Ҿͺã�����ͷ����ʲô�˶��С�
    ADD_TALK ���ˣ����ˣ�����$_player$���Ҽ������ˡ�
    TALK
ENDPROC
;
;
;OPTION 29 �������������㿴�����ˡ�
PROC 29
    DIALOG 0
    ADD_TALK $_npc$��
    ADD_TALK �ǣ���Ϊ����˭�������ˣ��Ҽ����˶��ˡ�
    ADD_TALK ��û�п���ʲô��
    NEXT 30
    TALK
    
ENDPROC
;
;
PROC 30
    DIALOG 0
    ADD_TALK $_npc$��
    ADD_TALK ���������һ���֪��������֡�
    NEXT 31
    TALK
    
ENDPROC
;
;
PROC 31
    DIALOG 0
    ADD_TALK $_npc$��
    ADD_TALK ���Ҿ�����ţ�ġ�
    NEXT 32
    TALK
ENDPROC
;
;
PROC 32
    DIALOG 1
    ADD_TALK $_npc$��
    ADD_TALK ��˵��֪��������֣������֤һ����
    OPTION 33 ���֤������˵�ҽ�ʲô��
    OPTION 34 û�й��򣬲����֤
    TALK
ENDPROC
;
;
;OPTION 33 ���֤������˵�ҽ�ʲô��
PROC 33
    DIALOG 3
    ADD_TALK $_npc$��
    ADD_TALK �㰡��������ֽ� $_player$ ��
    TALK
ENDPROC
;
;
;OPTION 34 û�й��򣬲����֤
PROC 34
    DIALOG 3
    ADD_TALK $_npc$��
    ADD_TALK û�й�ϵ���´����������ҡ�
    TALK
ENDPROC
;
;
;

