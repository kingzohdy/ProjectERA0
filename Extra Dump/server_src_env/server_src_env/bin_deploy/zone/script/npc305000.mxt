; ����


proc 1
	DIALOG 1
	add_talk #b#b#c6d00d2��ͬһ�����ģ�Juntos num s��ritmo����
	add_talk #b#b#b#b��֧������Ŀ�е�ʤ��֮�ӡ�#n
	add_talk #b#b
	add_talk #b#b#cFF0080ֻҪ֧�ֵ���ӽ��������ɻ�÷��Ľ�ȯ#n
	
		option 100 A��-���Ҷ�
		option 110 B��-���Ҷ�
		option 120 C��-���Ҷ�
		option 130 D��-���Ҷ�
		option 140 E��-���Ҷ�
		option 150 F��-���Ҷ�
		option 160 G��-���Ҷ�
		option 170 H��-���Ҷ�
	
	talk
endproc

proc 100
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 0 16 ret zhi1
	get_global_var 2 16 ret zhi2
	get_global_var 4 16 ret zhi3
	get_global_var 6 16 ret zhi4
	call 200
	
		option 101 ֧��-�����ӡ�����$@zhi1$�˹���
		option 102 ֧��-����¡�ӡ�����$@zhi2$�˹���
		option 103 ֧��-ī����ӡ�����$@zhi3$�˹���
		option 104 ֧��-���޵��Ƕӡ�����$@zhi4$�˹���
		option 1 ����
	
	talk
endproc

proc 101
	choose 2 301 300 ���򡾰����ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 301
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 1
					set_role_var 74 8 @zhi 0
					
					get_global_var 0 16 0 num1
					add num1 1
					set_global_var 0 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾰����ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif			
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 102
	choose 2 302 300 ���򡾿���¡�ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 302
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 2
					set_role_var 74 8 @zhi 0
					
					get_global_var 2 16 0 num1
					add num1 1
					set_global_var 2 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾿���¡�ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif			
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 103
	choose 2 303 300 ����ī����ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 303
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 3
					set_role_var 74 8 @zhi 0
					
					get_global_var 4 16 0 num1
					add num1 1
					set_global_var 4 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ�����ī����ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif			
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 104
	choose 2 304 300 ���򡾿��޵��Ƕӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 304
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 4
					set_role_var 74 8 @zhi 0
					
					get_global_var 6 16 0 num1
					add num1 1
					set_global_var 6 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾿��޵��Ƕӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif			
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 110
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 8 16 ret zhi1
	get_global_var 10 16 ret zhi2
	get_global_var 12 16 ret zhi3
	get_global_var 14 16 ret zhi4
	call 200
	
		option 111 ֧��-�������ӡ�����$@zhi1$�˹���
		option 112 ֧��-�����ӡ�����$@zhi2$�˹���
		option 113 ֧��-�Ĵ����Ƕӡ�����$@zhi3$�˹���
		option 114 ֧��-�����ӡ�����$@zhi4$�˹���
		option 1 ����
	
	talk
endproc

proc 111
	choose 2 311 300 �����������ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 311
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 5
					set_role_var 74 8 @zhi 0
					
					get_global_var 8 16 0 num1
					add num1 1
					set_global_var 8 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ������������ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif			
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 112
	choose 2 312 300 ���������ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 312
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 6
					set_role_var 74 8 @zhi 0
					
					get_global_var 10 16 0 num1
					add num1 1
					set_global_var 10 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����������ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif			
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 113
	choose 2 313 300 ���򡾰Ĵ����Ƕӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 313
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 7
					set_role_var 74 8 @zhi 0
					
					get_global_var 12 16 0 num1
					add num1 1
					set_global_var 12 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾰Ĵ����Ƕӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif			
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 114
	choose 2 314 300 ���򡾺����ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 314
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 8
					set_role_var 74 8 @zhi 0
					
					get_global_var 14 16 0 num1
					add num1 1
					set_global_var 14 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾺����ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 120
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 16 16 ret zhi1
	get_global_var 18 16 ret zhi2
	get_global_var 20 16 ret zhi3
	get_global_var 22 16 ret zhi4
	call 200
	
		option 121 ֧��-���ױ��Ƕӡ�����$@zhi1$�˹���
		option 122 ֧��-���ص��߶ӡ�����$@zhi2$�˹���
		option 123 ֧��-�ձ��ӡ�����$@zhi3$�˹���
		option 124 ֧��-ϣ���ӡ�����$@zhi4$�˹���
		option 1 ����
	
	talk
endproc

proc 121
	choose 2 321 300 ���򡾸��ױ��Ƕӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 321
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 9
					set_role_var 74 8 @zhi 0
					
					get_global_var 16 16 0 num1
					add num1 1
					set_global_var 16 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾸��ױ��Ƕӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 122
	choose 2 322 300 ���򡾿��ص��߶ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 322
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 10
					set_role_var 74 8 @zhi 0
					
					get_global_var 18 16 0 num1
					add num1 1
					set_global_var 18 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾿��ص��߶ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 123
	choose 2 323 300 �����ձ��ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 323
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 11
					set_role_var 74 8 @zhi 0
					
					get_global_var 20 16 0 num1
					add num1 1
					set_global_var 20 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ������ձ��ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 124
	choose 2 324 300 ����ϣ���ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 324
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 12
					set_role_var 74 8 @zhi 0
					
					get_global_var 22 16 0 num1
					add num1 1
					set_global_var 22 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ�����ϣ���ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 130
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 24 16 ret zhi1
	get_global_var 26 16 ret zhi2
	get_global_var 28 16 ret zhi3
	get_global_var 30 16 ret zhi4
	call 200
	
		option 131 ֧��-������ӡ�����$@zhi1$�˹���
		option 132 ֧��-Ӣ�����ӡ�����$@zhi2$�˹���
		option 133 ֧��-��˹����Ӷӡ�����$@zhi3$�˹���
		option 134 ֧��-������ӡ�����$@zhi4$�˹���
		option 1 ����
	
	talk
endproc

proc 131
	choose 2 331 300 ����������ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 331
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 13
					set_role_var 74 8 @zhi 0
					
					get_global_var 24 16 0 num1
					add num1 1
					set_global_var 24 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ�����������ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 132
	choose 2 332 300 ����Ӣ�����ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 332
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 14
					set_role_var 74 8 @zhi 0
					
					get_global_var 26 16 0 num1
					add num1 1
					set_global_var 26 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ�����Ӣ�����ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 133
	choose 2 333 300 ���򡾸�˹����Ӷӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 333
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 15
					set_role_var 74 8 @zhi 0
					
					get_global_var 28 16 0 num1
					add num1 1
					set_global_var 28 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾸�˹����Ӷӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 134
	choose 2 334 300 ����������ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 334
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 16
					set_role_var 74 8 @zhi 0
					
					get_global_var 30 16 0 num1
					add num1 1
					set_global_var 30 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ�����������ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 140
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 32 16 ret zhi1
	get_global_var 34 16 ret zhi2
	get_global_var 36 16 ret zhi3
	get_global_var 38 16 ret zhi4
	call 200
	
		option 141 ֧��-��ʿ�ӡ�����$@zhi1$�˹���
		option 142 ֧��-��϶���ӡ�����$@zhi2$�˹���
		option 143 ֧��-�鶼��˹�ӡ�����$@zhi3$�˹���
		option 144 ֧��-�����ӡ�����$@zhi4$�˹���
		option 1 ����
	
	talk
endproc

proc 141
	choose 2 341 300 ������ʿ�ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 341
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 17
					set_role_var 74 8 @zhi 0
					
					get_global_var 32 16 0 num1
					add num1 1
					set_global_var 32 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ�������ʿ�ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 142
	choose 2 342 300 ���򡾶�϶���ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 342
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 18
					set_role_var 74 8 @zhi 0
					
					get_global_var 34 16 0 num1
					add num1 1
					set_global_var 34 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾶�϶���ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 143
	choose 2 343 300 ���򡾺鶼��˹�ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 343
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 19
					set_role_var 74 8 @zhi 0
					
					get_global_var 36 16 0 num1
					add num1 1
					set_global_var 36 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾺鶼��˹�ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 144
	choose 2 344 300 ���򡾷����ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 344
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 20
					set_role_var 74 8 @zhi 0
					
					get_global_var 38 16 0 num1
					add num1 1
					set_global_var 38 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾷����ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 150
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 40 16 ret zhi1
	get_global_var 42 16 ret zhi2
	get_global_var 44 16 ret zhi3
	get_global_var 46 16 ret zhi4
	call 200
	
		option 151 ֧��-����͢�ӡ�����$@zhi1$�˹���
		option 152 ֧��-�������Ƕӡ�����$@zhi2$�˹���
		option 153 ֧��-���ʶӡ�����$@zhi3$�˹���
		option 154 ֧��-���ڶӡ�����$@zhi4$�˹���
		option 1 ����
	
	talk
endproc

proc 151
	choose 2 351 300 ���򡾰���͢�ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 351
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 21
					set_role_var 74 8 @zhi 0
					
					get_global_var 40 16 0 num1
					add num1 1
					set_global_var 40 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾰���͢�ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 152
	choose 2 352 300 �����������Ƕӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 352
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 22
					set_role_var 74 8 @zhi 0
					
					get_global_var 42 16 0 num1
					add num1 1
					set_global_var 42 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ������������Ƕӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 153
	choose 2 353 300 �������ʶӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 353
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 23
					set_role_var 74 8 @zhi 0
					
					get_global_var 44 16 0 num1
					add num1 1
					set_global_var 44 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ��������ʶӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 154
	choose 2 354 300 ���򡾲��ڶӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 354
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 24
					set_role_var 74 8 @zhi 0
					
					get_global_var 46 16 0 num1
					add num1 1
					set_global_var 46 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾲��ڶӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 160
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 48 16 ret zhi1
	get_global_var 50 16 ret zhi2
	get_global_var 52 16 ret zhi3
	get_global_var 54 16 ret zhi4
	call 200
	
		option 161 ֧��-�¹��ӡ�����$@zhi1$�˹���
		option 162 ֧��-���ɶӡ�����$@zhi2$�˹���
		option 163 ֧��-�����ӡ�����$@zhi3$�˹���
		option 164 ֧��-�������ӡ�����$@zhi4$�˹���
		option 1 ����
	
	talk
endproc

proc 161
	choose 2 361 300 ���򡾵¹��ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 361
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 25
					set_role_var 74 8 @zhi 0
					
					get_global_var 48 16 0 num1
					add num1 1
					set_global_var 48 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾵¹��ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 162
	choose 2 362 300 ���򡾼��ɶӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 362
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 26
					set_role_var 74 8 @zhi 0
					
					get_global_var 50 16 0 num1
					add num1 1
					set_global_var 50 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾼��ɶӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 163
	choose 2 363 300 ���������ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 363
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 27
					set_role_var 74 8 @zhi 0
					
					get_global_var 52 16 0 num1
					add num1 1
					set_global_var 52 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����������ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 164
	choose 2 364 300 �����������ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 364
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 28
					set_role_var 74 8 @zhi 0
					
					get_global_var 54 16 0 num1
					add num1 1
					set_global_var 54 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ������������ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 170
	DIALOG 1
	strvar ret zhi1 zhi2 zhi3 zhi4
	get_global_var 56 16 ret zhi1
	get_global_var 58 16 ret zhi2
	get_global_var 60 16 ret zhi3
	get_global_var 62 16 ret zhi4
	call 200
	
		option 171 ֧��-����ʱ�ӡ�����$@zhi1$�˹���
		option 172 ֧��-���������Ƕӡ�����$@zhi2$�˹���
		option 173 ֧��-�����ӡ�����$@zhi3$�˹���
		option 174 ֧��-����˹�ӡ�����$@zhi4$�˹���
		option 1 ����
	
	talk
endproc

proc 171
	choose 2 371 300 ���򡾱���ʱ�ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 371
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 29
					set_role_var 74 8 @zhi 0
					
					get_global_var 56 16 0 num1
					add num1 1
					set_global_var 56 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾱���ʱ�ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 172
	choose 2 372 300 ���򡾰��������Ƕӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 372
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 30
					set_role_var 74 8 @zhi 0
					
					get_global_var 58 16 0 num1
					add num1 1
					set_global_var 58 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾰��������Ƕӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 173
	choose 2 373 300 ���򡾺����ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 373
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 31
					set_role_var 74 8 @zhi 0
					
					get_global_var 60 16 0 num1
					add num1 1
					set_global_var 60 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾺����ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 174
	choose 2 374 300 ���򡾶���˹�ӡ���Ҫ֧��5000����\n�Ƿ�ȷ�Ϲ���
endproc

proc 374
	strvar yue ri zhi ret1 ret2 num1
	GET_NOW_TIME yue ri 0 0 0 0 0
	if yue < 6 or ( yue = 6 and ri < 13)
		GET_ROLE_VAR 74 8 0 zhi
		if zhi > 0
			call 202
		else
			DEC_MONEY_TEST 0 5000 20 ret1
			if ret1 = 0
				DEC_MONEY 0 5000 20 ret2
				if ret2 = 0
					var zhi 32
					set_role_var 74 8 @zhi 0
					
					get_global_var 62 16 0 num1
					add num1 1
					set_global_var 62 16 @num1 0
					
					ADD_SYS_MSG_START
					ADD_SYS_MSG ��ϲ��ɹ����򡾶���˹�ӡ���ֻҪ����ӽ������ɻ�ý�����
					SET_SYS_MSG_MODE 4 0 0
					MSG_INFO
				else
					call 203
				endif
			else
				call 203
			endif
		endif
	else
		call 201
	endif
endproc

proc 200
	add_talk ����˵����
	add_talk #b#bÿ��#c186506ֻ�ܹ���1��#n��������Ҫ����#c005ebe5000����#n
	add_talk #b#b�����ֹ���ڵ�#cF80B0B6��12��24:00#n
	add_talk #b#b
	add_talk ����������
	add_talk #b#b��������Ϊ#c005ebe����׶�#n��ÿ�׶�ÿ��һ���ȯ
	add_talk #b#bֻҪ�������ӽ��������ɻ�÷��Ľ�ȯ
	add_talk #b#b������ӽ����Ľ�ȯ����ƽ���ָ��ö����
	add_talk #b#b���������ӵ���ң����ٿɻ��#cff4e00һ�ٽ�ȯ#n
	strvar zhi
	GET_ROLE_VAR 74 8 0 zhi
	if zhi = 1
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ������ӡ�#n
	endif
	if zhi = 2
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ�����¡�ӡ�#n
	endif
	if zhi = 3
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ�ī����ӡ�#n
	endif
	if zhi = 4
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ����޵��Ƕӡ�#n
	endif
	if zhi = 5
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ��������ӡ�#n
	endif
	if zhi = 6
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ������ӡ�#n
	endif
	if zhi = 7
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ��Ĵ����Ƕӡ�#n
	endif
	if zhi = 8
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ������ӡ�#n
	endif
	if zhi = 9
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ����ױ��Ƕӡ�#n
	endif
	if zhi = 10
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ����ص��߶ӡ�#n
	endif
	if zhi = 11
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ��ձ��ӡ�#n
	endif
	if zhi = 12
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ�ϣ���ӡ�#n
	endif
	if zhi = 13
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ�������ӡ�#n
	endif
	if zhi = 14
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ�Ӣ�����ӡ�#n
	endif
	if zhi = 15
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ���˹����Ӷӡ�#n
	endif
	if zhi = 16
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ�������ӡ�#n
	endif
	if zhi = 17
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ���ʿ�ӡ�#n
	endif
	if zhi = 18
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ���϶���ӡ�#n
	endif
	if zhi = 19
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ��鶼��˹�ӡ�#n
	endif
	if zhi = 20
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ������ӡ�#n
	endif
	if zhi = 21
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ�����͢�ӡ�#n
	endif
	if zhi = 22
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ��������Ƕӡ�#n
	endif
	if zhi = 23
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ����ʶӡ�#n
	endif
	if zhi = 24
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ����ڶӡ�#n
	endif
	if zhi = 25
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ��¹��ӡ�#n
	endif
	if zhi = 26
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ����ɶӡ�#n
	endif
	if zhi = 27
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ������ӡ�#n
	endif
	if zhi = 28
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ��������ӡ�#n
	endif
	if zhi = 29
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ�����ʱ�ӡ�#n
	endif
	if zhi = 30
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ����������Ƕӡ�#n
	endif
	if zhi = 31
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ������ӡ�#n
	endif
	if zhi = 32
		add_talk #b#b
		add_talk #c005ebe��. ���Ѿ������ˡ�����˹�ӡ�#n
	endif
endproc

proc 201
	ADD_SYS_MSG_START
	ADD_SYS_MSG �����ֹ�����ѹ���������籭�޷��ٹ���
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc

proc 202
	ADD_SYS_MSG_START
	ADD_SYS_MSG ���Ѿ�������֧�ֵ���ӣ�������籭�޷��ٹ���
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc

proc 203
	ADD_SYS_MSG_START
	ADD_SYS_MSG ����Я�������Ҳ��㣬��ʱ�޷�����
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO
endproc
