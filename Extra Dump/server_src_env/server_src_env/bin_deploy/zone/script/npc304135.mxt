;304135 ����

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����ϲ�û�о��Ե�����񣬲��ֻ����ǿ�ߺ��޷�������ʵ�����ߣ�
	task_stat 4513 1 0
	if 0 = 2
		option 201 #���������Ѳ����ң���ʼ���ƣ�
	endif
	task_stat 4514 1 0
	if 0 = 2
		option 301 #���������Ѳ����ң���ʼ���ƣ�
	endif
	task_stat 4515 1 0
	if 0 = 2
		option 401 #���������Ѳ����ң���ʼ���ƣ�
	endif
	task_stat 4516 1 0
	if 0 = 2
		option 501 #���������Ѳ����ң���ʼ���ƣ�
	endif
	task_stat 4517 1 0
	if 0 = 2
		option 601 #���������Ѳ����ң���ʼ���ƣ�
	endif
	
	task_add_option 4513 1 101
	task_add_option 4514 1 102
	task_add_option 4515 1 103
	task_add_option 4516 1 104
	task_add_option 4517 1 105
	
	talk
ENDPROC

PROC 101	
	task_do_option 4513 0
ENDPROC

PROC 102	
	task_do_option 4514 0
ENDPROC

PROC 103	
	task_do_option 4515 0
ENDPROC

PROC 104	
	task_do_option 4516 0
ENDPROC

PROC 105	
	task_do_option 4517 0
ENDPROC

proc 201;2
	call 701
	DIALOG 1
	add_talk #b#b#b#b#b#b#c186506�����ٵĲ���ѾƵ���
	add_talk #b#b#b#b#b����һ����ֻ�����¿�ʼ�#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ1���ƣ���Ͱ��#n
		option 704 #����Ͱװ����5���ƣ�
		option 202 #����Ͱװ����3���ƣ�
		; option 704 #����Ͱ��ľƵ�����Ͱ
		; option 704 #����Ͱ��ľƵ����Ͱ
		; option 704 #����Ͱ����
		; option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 202;4
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ�գ���Ͱ3����#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ1���ƣ���Ͱ��#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 704 #����Ͱ��ľƵ�����Ͱ
		option 203 #����Ͱ��ľƵ����Ͱ
		; option 704 #����Ͱ����
		; option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 203;2
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ3���ƣ���Ͱ��#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ1���ƣ���Ͱ��#n
		option 704 #����Ͱװ����5���ƣ�
		option 204 #����Ͱװ����3���ƣ�
		option 704 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		option 704 #����Ͱ����
		option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 204;4
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ3���ƣ���Ͱ3����#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ1���ƣ���Ͱ��#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 704 #����Ͱ��ľƵ�����Ͱ
		option 205 #����Ͱ��ľƵ����Ͱ
		option 704 #����Ͱ����
		option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 205;5
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ5���ƣ���Ͱ1����#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ1���ƣ���Ͱ��#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 704 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		option 206 #����Ͱ����
		option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 206;4
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ�գ���Ͱ1����#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ1���ƣ���Ͱ��#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 704 #����Ͱ��ľƵ�����Ͱ
		option 207 #����Ͱ��ľƵ����Ͱ
		option 704 #����Ͱ����
		option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 207;+
	call 703
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#bҪ����
	add_talk #b#b#b#b#b���ѡ��#c6d00d2�ҵ�����#n�������
	add_talk #b#b
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ1���ƣ���Ͱ��#n
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ1���ƣ���Ͱ��#n
	strvar rad
	rand rad 7
		switch rad 
			case 0
				option 208 #�ҵ����ˣ�
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 1
				option 704 #����Ͱװ����5���ƣ�
				option 208 #�ҵ����ˣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 2
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 208 #�ҵ����ˣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 3
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 208 #�ҵ����ˣ�
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 4
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 208 #�ҵ����ˣ�
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 5
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 208 #�ҵ����ˣ�
				option 704 #����Ͱ����
			endcase
			case 6
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
				option 208 #�ҵ����ˣ�
			endcase
		endswitch
		
	talk
endproc
proc 208;o
	task_fini 4513 0
endproc

proc 301;2
	call 701
	DIALOG 1
	add_talk #b#b#b#b#b#b#c186506�����ٵĲ���ѾƵ���
	add_talk #b#b#b#b#b����һ����ֻ�����¿�ʼ�#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ�գ���Ͱ1����#n
		option 704 #����Ͱװ����5���ƣ�
		option 302 #����Ͱװ����3���ƣ�
		; option 704 #����Ͱ��ľƵ�����Ͱ
		; option 704 #����Ͱ��ľƵ����Ͱ
		; option 704 #����Ͱ����
		; option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 302;4
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ�գ���Ͱ3����#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ�գ���Ͱ1����#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 704 #����Ͱ��ľƵ�����Ͱ
		option 303 #����Ͱ��ľƵ����Ͱ
		; option 704 #����Ͱ����
		; option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 303;2
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ3���ƣ���Ͱ��#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ�գ���Ͱ1����#n
		option 704 #����Ͱװ����5���ƣ�
		option 304 #����Ͱװ����3���ƣ�
		option 704 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		option 704 #����Ͱ����
		option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 304;4
	call 702
	DIALOG 1
		add_talk #b#b#b#b#b#c186506���ڣ���Ͱ3���ƣ���Ͱ3����#n
		add_talk #b#b
		add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ�գ���Ͱ1����#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 704 #����Ͱ��ľƵ�����Ͱ
		option 305 #����Ͱ��ľƵ����Ͱ
		option 704 #����Ͱ����
		option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 305;5
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ5���ƣ���Ͱ1����#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ�գ���Ͱ1����#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 704 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		option 306 #����Ͱ����
		option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 306;+
	call 703
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#bҪ����
	add_talk #b#b#b#b#b���ѡ��#c6d00d2�ҵ�����#n�������
	add_talk #b#b
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ�գ���Ͱ1����#n
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ�գ���Ͱ1����#n
	strvar rad
	rand rad 7
		switch rad 
			case 0
				option 307 #�ҵ����ˣ�
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 1
				option 704 #����Ͱװ����5���ƣ�
				option 307 #�ҵ����ˣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 2
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 307 #�ҵ����ˣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 3
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 307 #�ҵ����ˣ�
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 4
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 307 #�ҵ����ˣ�
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 5
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 307 #�ҵ����ˣ�
				option 704 #����Ͱ����
			endcase
			case 6
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
				option 307 #�ҵ����ˣ�
			endcase
		endswitch
			
	talk
endproc
proc 307;o
	task_fini 4514 0
endproc

proc 401;1
	call 701
	DIALOG 1
	add_talk #b#b#b#b#b#b#c186506�����ٵĲ���ѾƵ���
	add_talk #b#b#b#b#b����һ����ֻ�����¿�ʼ�#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ2���ƣ���Ͱ��#n
		option 402 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		; option 704 #����Ͱ��ľƵ�����Ͱ
		; option 704 #����Ͱ��ľƵ����Ͱ
		; option 704 #����Ͱ����
		; option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 402;3
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ5���ƣ���Ͱ��#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ2���ƣ���Ͱ��#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 403 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		; option 704 #����Ͱ����
		; option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 403;6
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ2���ƣ���Ͱ3����#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ2���ƣ���Ͱ��#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 704 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		option 704 #����Ͱ����
		option 404 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 404;+
	call 703
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#bҪ����
	add_talk #b#b#b#b#b���ѡ��#c6d00d2�ҵ�����#n�������
	add_talk #b#b
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ2���ƣ���Ͱ��#n
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ2���ƣ���Ͱ��#n
	strvar rad
	rand rad 7
		switch rad 
			case 0
				option 405 #�ҵ����ˣ�
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 1
				option 704 #����Ͱװ����5���ƣ�
				option 405 #�ҵ����ˣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 2
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 405 #�ҵ����ˣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 3
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 405 #�ҵ����ˣ�
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 4
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 405 #�ҵ����ˣ�
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 5
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 405 #�ҵ����ˣ�
				option 704 #����Ͱ����
			endcase
			case 6
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
				option 405 #�ҵ����ˣ�
			endcase
		endswitch
	talk
	
endproc
proc 405;o
	task_fini 4515 0
endproc

proc 501;1
	call 701
	DIALOG 1
	add_talk #b#b#b#b#b#b#c186506�����ٵĲ���ѾƵ���
	add_talk #b#b#b#b#b����һ����ֻ�����¿�ʼ�#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ�գ���Ͱ2����#n
		option 502 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		; option 704 #����Ͱ��ľƵ�����Ͱ
		; option 704 #����Ͱ��ľƵ����Ͱ
		; option 704 #����Ͱ����
		; option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 502;3
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ5���ƣ���Ͱ��#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ�գ���Ͱ2����#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 503 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		; option 704 #����Ͱ����
		; option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 503;6
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ2���ƣ���Ͱ3����#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ�գ���Ͱ2����#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 704 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		option 704 #����Ͱ����
		option 504 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 504;3
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ2���ƣ���Ͱ��
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ�գ���Ͱ2����#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 505 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		option 704 #����Ͱ����
		option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 505;+
	call 703
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#bҪ����
	add_talk #b#b#b#b#b���ѡ��#c6d00d2�ҵ�����#n�������
	add_talk #b#b
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ�գ���Ͱ2����#n
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ�գ���Ͱ2����#n
	strvar rad
	rand rad 7
		switch rad 
			case 0
				option 506 #�ҵ����ˣ�
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 1
				option 704 #����Ͱװ����5���ƣ�
				option 506 #�ҵ����ˣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 2
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 506 #�ҵ����ˣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 3
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 506 #�ҵ����ˣ�
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 4
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 506 #�ҵ����ˣ�
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 5
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 506 #�ҵ����ˣ�
				option 704 #����Ͱ����
			endcase
			case 6
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
				option 506 #�ҵ����ˣ�
			endcase
		endswitch
	talk
	
endproc
proc 506;o
	task_fini 4516 0
endproc

proc 601;1
	call 701
	DIALOG 1
	add_talk #b#b#b#b#b#b#c186506�����ٵĲ���ѾƵ���
	add_talk #b#b#b#b#b����һ����ֻ�����¿�ʼ�#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ4���ƣ���Ͱ��#n
		option 602 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		; option 704 #����Ͱ��ľƵ�����Ͱ
		; option 704 #����Ͱ��ľƵ����Ͱ
		; option 704 #����Ͱ����
		; option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 602;3
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ5���ƣ���Ͱ��#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ4���ƣ���Ͱ��#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 603 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		; option 704 #����Ͱ����
		; option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 603;6
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ2���ƣ���Ͱ3����#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ4���ƣ���Ͱ��#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 704 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		option 704 #����Ͱ����
		option 604 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 604;3
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ2���ƣ���Ͱ��#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ4���ƣ���Ͱ��#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 605 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		option 704 #����Ͱ����
		option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 605;1
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ�գ���Ͱ2����#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ4���ƣ���Ͱ��#n
		option 606 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 704 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		option 704 #����Ͱ����
		option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 606;3
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ5���ƣ���Ͱ2����#n
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ4���ƣ���Ͱ��#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 607 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		option 704 #����Ͱ����
		option 704 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 607;6
	call 702
	DIALOG 1
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ4���ƣ���Ͱ3����
	add_talk #b#b
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ4���ƣ���Ͱ��#n
		option 704 #����Ͱװ����5���ƣ�
		option 704 #����Ͱװ����3���ƣ�
		option 704 #����Ͱ��ľƵ�����Ͱ
		option 704 #����Ͱ��ľƵ����Ͱ
		option 704 #����Ͱ����
		option 608 #����Ͱ����
		; option 705 #�ҵ����ˣ�
	talk
endproc
proc 608;+
	call 703
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#bҪ����
	add_talk #b#b#b#b#b���ѡ��#c6d00d2�ҵ�����#n�������
	add_talk #b#b
	add_talk #b#b#b#b#b#c186506���ڣ���Ͱ4���ƣ���Ͱ��#n
	add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ4���ƣ���Ͱ��#n
	strvar rad
	rand rad 7
		switch rad 
			case 0
				option 609 #�ҵ����ˣ�
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 1
				option 704 #����Ͱװ����5���ƣ�
				option 609 #�ҵ����ˣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 2
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 609 #�ҵ����ˣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 3
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 609 #�ҵ����ˣ�
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 4
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 609 #�ҵ����ˣ�
				option 704 #����Ͱ����
				option 704 #����Ͱ����
			endcase
			case 5
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 609 #�ҵ����ˣ�
				option 704 #����Ͱ����
			endcase
			case 6
				option 704 #����Ͱװ����5���ƣ�
				option 704 #����Ͱװ����3���ƣ�
				option 704 #����Ͱ��ľƵ�����Ͱ
				option 704 #����Ͱ��ľƵ����Ͱ
				option 704 #����Ͱ����
				option 704 #����Ͱ����
				option 609 #�ҵ����ˣ�
			endcase
		endswitch
	talk
endproc
proc 609;o
	task_fini 4517 0
endproc

proc 701;x
	ADD_SYS_MSG_START
	ADD_SYS_MSG ׼����ʼ���ƣ������ٵĲ���ѾƵ���
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc
proc 702;x
	ADD_SYS_MSG_START
	ADD_SYS_MSG ���Ʋ�����ȷ������û�ﵽҪ�����������
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc
proc 703;x
	ADD_SYS_MSG_START
	ADD_SYS_MSG �Ѿ����Ҫ�󣬵�����ҵ����ˡ��㱨�����
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc
proc 704;!
	DIALOG 1
	task_stat 4513 1 0
	if 0 = 2
		add_talk #b#b#b#b#b#b#b#b#b#c186506������ս
		add_talk #b#b#b#b ���ڲ������ֻ�����µ���#n
		add_talk #b#b
		add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ1���ƣ���Ͱ��#n
			option 704 #����Ͱװ����5���ƣ�
			option 202 #����Ͱװ����3���ƣ�
		talk
	endif
	task_stat 4514 1 0
	if 0 = 2
		add_talk #b#b#b#b#b#b#b#b#b#c186506������ս
		add_talk #b#b#b#b ���ڲ������ֻ�����µ���#n
		add_talk #b#b
		add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ�գ���Ͱ1����#n
			option 704 #����Ͱװ����5���ƣ�
			option 302 #����Ͱװ����3���ƣ�
		talk
	endif
	task_stat 4515 1 0
	if 0 = 2
		add_talk #b#b#b#b#b#b#b#b#b#c186506������ս
		add_talk #b#b#b#b ���ڲ������ֻ�����µ���#n
		add_talk #b#b
		add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ2���ƣ���Ͱ��#n
			option 402 #����Ͱװ����5���ƣ�
			option 704 #����Ͱװ����3���ƣ�
		talk
	endif
	task_stat 4516 1 0
	if 0 = 2
		add_talk #b#b#b#b#b#b#b#b#b#c186506������ս
		add_talk #b#b#b#b ���ڲ������ֻ�����µ���#n
		add_talk #b#b
		add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ�գ���Ͱ2����#n
			option 502 #����Ͱװ����5���ƣ�
			option 704 #����Ͱװ����3���ƣ�
		talk
	endif
	task_stat 4517 1 0
	if 0 = 2
		add_talk #b#b#b#b#b#b#b#b#b#c186506������ս
		add_talk #b#b#b#b ���ڲ������ֻ�����µ���#n
		add_talk #b#b
		add_talk #b#b#b#b#b#c0181f9Ҫ�󣺽�Ͱ4���ƣ���Ͱ��#n
			option 602 #����Ͱװ����5���ƣ�
			option 704 #����Ͱװ����3���ƣ�
		talk
	endif
	
	ADD_SYS_MSG_START
	ADD_SYS_MSG ���ڵ��Ʋ������ֻ�ûص���һ�����µ���
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO

endproc
proc 705;x
	ADD_SYS_MSG_START
	ADD_SYS_MSG ���ź�����δ�ܴﵽҪ�������¿�ʼ���ƣ�
	SET_SYS_MSG_MODE 4 0 0
	MSG_INFO
endproc
