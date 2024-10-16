;假混乱元素脚本

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b（滋~）我身上的能量已经所剩无几了，但是另外一个能量充沛的我依旧隐藏在这张地图的某一处，你能找得到它吗？
	add_talk #b#b
	
	strvar trs1
	strvar trs2
	
	GET_EVENT_VAR_BY_ID 63 12 32 1 trs1
	GET_EVENT_VAR_BY_ID 63 16 32 1 trs2
	switch @trs1
		case 0
			switch @trs2
				case 0 
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506西南方#n看了一眼。)
				endcase
				case 1
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506西方#n看了一眼。)
				endcase
				case 2
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506西北方#n看了一眼。)
				endcase
				case 3
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506西北方#n看了一眼。)
				endcase
				case 4
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506西南方#n看了一眼。)
				endcase
				case 5
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506东北方#n看了一眼。)
				endcase
				case 6
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506东北方#n看了一眼。)
				endcase
				case 7
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506东南方#n看了一眼。)
				endcase
				case 8
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506东方#n看了一眼。)
				endcase
				case 9
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506南方#n看了一眼。)
				endcase
			endswitch
		endcase
		case 1
			switch @trs2
				case 0 
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506区域中部#n看了一眼。)
				endcase
				case 1
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506东北方#n看了一眼。)
				endcase
				case 2
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506东方#n看了一眼。)
				endcase
				case 3
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506东方#n看了一眼。)
				endcase
				case 4
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506东北方#n看了一眼。)
				endcase
				case 5
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506西方#n看了一眼。)
				endcase
			endswitch
		endcase		
		case 2
			switch @trs2
				case 0 
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506西北方#n看了一眼。)
				endcase
				case 1
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506西南方#n看了一眼。)
				endcase
				case 2
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506西南方#n看了一眼。)
				endcase
				case 3
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506西南方#n看了一眼。)
				endcase
				case 4
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506西方#n看了一眼。)
				endcase
				case 5
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506南方#n看了一眼。)
				endcase
				case 6
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506区域中部#n看了一眼。)
				endcase
				case 7
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506北方#n看了一眼。)
				endcase
				case 8
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506东方#n看了一眼。)
				endcase
				case 9
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506南方#n看了一眼。)
				endcase
				case 10
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506东南方#n看了一眼。)
				endcase
				case 11
					add_talk #b#b(混乱元素虚空般的眼睛往#c186506区域中部#n看了一眼。)
				endcase	
			endswitch
		endcase
	endswitch
	add_talk #b#b地图方位提示：上北下南左西右东。		
	talk
endproc 


	
	
	
