;70����,3��boss
proc 1
	GET_PWORLD_VAR 40 32 1 20
        If 20 = 0
		dialog 1
		ADD_TALK $_npc$:
		ADD_TALK #b#b����İ���ˣ����������������ײ������ɪ�ĵ��̣���������е�ʲô���ͣ����ڣ������������ɪ���ţ�
		ADD_TALK #b#b�������룬�ǰ����Ǵ�ж�˿���ƴװ���أ�������������������ߺ��أ��������������������������Ƶ��ս���LW-1186�ٺ��ʲ����ˡ������ǽ�����ɪ���������ǵľ�����
		ADD_TALK #b#b�ҵĵ��̣�������������û��ѡ�񣬰��������ǿ���ѡ����ɪΪ����׼���Ĳ��Թ��ߡ�
		option 100 ��ս
		talk
		
	endif
endproc
proc 100
	;boss3�Ի����,0δ������,1������
	GET_PWORLD_VAR 40 32 1 20
        If 20 = 0
                var 20 1
                set_pworld_var 40 32 @20 1
        endif
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ɱ�������ֲ����顣
	talk
	DEL_NPC 317013 1
endproc
