;�ٻ���Ԫ�ؽű�

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����~�������ϵ������Ѿ���ʣ�޼��ˣ���������һ��������������������������ŵ�ͼ��ĳһ���������ҵõ�����
	add_talk #b#b
	
	strvar trs1
	strvar trs2
	
	GET_EVENT_VAR_BY_ID 63 12 32 1 trs1
	GET_EVENT_VAR_BY_ID 63 16 32 1 trs2
	switch @trs1
		case 0
			switch @trs2
				case 0 
					add_talk #b#b(����Ԫ����հ���۾���#c186506���Ϸ�#n����һ�ۡ�)
				endcase
				case 1
					add_talk #b#b(����Ԫ����հ���۾���#c186506����#n����һ�ۡ�)
				endcase
				case 2
					add_talk #b#b(����Ԫ����հ���۾���#c186506������#n����һ�ۡ�)
				endcase
				case 3
					add_talk #b#b(����Ԫ����հ���۾���#c186506������#n����һ�ۡ�)
				endcase
				case 4
					add_talk #b#b(����Ԫ����հ���۾���#c186506���Ϸ�#n����һ�ۡ�)
				endcase
				case 5
					add_talk #b#b(����Ԫ����հ���۾���#c186506������#n����һ�ۡ�)
				endcase
				case 6
					add_talk #b#b(����Ԫ����հ���۾���#c186506������#n����һ�ۡ�)
				endcase
				case 7
					add_talk #b#b(����Ԫ����հ���۾���#c186506���Ϸ�#n����һ�ۡ�)
				endcase
				case 8
					add_talk #b#b(����Ԫ����հ���۾���#c186506����#n����һ�ۡ�)
				endcase
				case 9
					add_talk #b#b(����Ԫ����հ���۾���#c186506�Ϸ�#n����һ�ۡ�)
				endcase
			endswitch
		endcase
		case 1
			switch @trs2
				case 0 
					add_talk #b#b(����Ԫ����հ���۾���#c186506�����в�#n����һ�ۡ�)
				endcase
				case 1
					add_talk #b#b(����Ԫ����հ���۾���#c186506������#n����һ�ۡ�)
				endcase
				case 2
					add_talk #b#b(����Ԫ����հ���۾���#c186506����#n����һ�ۡ�)
				endcase
				case 3
					add_talk #b#b(����Ԫ����հ���۾���#c186506����#n����һ�ۡ�)
				endcase
				case 4
					add_talk #b#b(����Ԫ����հ���۾���#c186506������#n����һ�ۡ�)
				endcase
				case 5
					add_talk #b#b(����Ԫ����հ���۾���#c186506����#n����һ�ۡ�)
				endcase
			endswitch
		endcase		
		case 2
			switch @trs2
				case 0 
					add_talk #b#b(����Ԫ����հ���۾���#c186506������#n����һ�ۡ�)
				endcase
				case 1
					add_talk #b#b(����Ԫ����հ���۾���#c186506���Ϸ�#n����һ�ۡ�)
				endcase
				case 2
					add_talk #b#b(����Ԫ����հ���۾���#c186506���Ϸ�#n����һ�ۡ�)
				endcase
				case 3
					add_talk #b#b(����Ԫ����հ���۾���#c186506���Ϸ�#n����һ�ۡ�)
				endcase
				case 4
					add_talk #b#b(����Ԫ����հ���۾���#c186506����#n����һ�ۡ�)
				endcase
				case 5
					add_talk #b#b(����Ԫ����հ���۾���#c186506�Ϸ�#n����һ�ۡ�)
				endcase
				case 6
					add_talk #b#b(����Ԫ����հ���۾���#c186506�����в�#n����һ�ۡ�)
				endcase
				case 7
					add_talk #b#b(����Ԫ����հ���۾���#c186506����#n����һ�ۡ�)
				endcase
				case 8
					add_talk #b#b(����Ԫ����հ���۾���#c186506����#n����һ�ۡ�)
				endcase
				case 9
					add_talk #b#b(����Ԫ����հ���۾���#c186506�Ϸ�#n����һ�ۡ�)
				endcase
				case 10
					add_talk #b#b(����Ԫ����հ���۾���#c186506���Ϸ�#n����һ�ۡ�)
				endcase
				case 11
					add_talk #b#b(����Ԫ����հ���۾���#c186506�����в�#n����һ�ۡ�)
				endcase	
			endswitch
		endcase
	endswitch
	add_talk #b#b��ͼ��λ��ʾ���ϱ����������Ҷ���		
	talk
endproc 


	
	
	
