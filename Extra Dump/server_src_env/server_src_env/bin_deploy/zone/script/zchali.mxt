
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�٣����ѣ��������￴������������۵���Ʒ���Ǳ�ĵط���Ҳ���򵽵ģ���Ȼ������Ҳ��Ҫһ��������������С����������ɻ�ò���ʲô������������Ҳ���õ��ģ�ֻҪ���ڻ����ص������ﵽ���������ϾͿ���ඡ�
	add_talk #b#b
	add_talk #b#b#c186506��ɡ����ˡ��͡����Ƴͷ������ܻ�û������̻��������#n

	OPTION 101 #(����)�����̵�

	STRVAR sta sta_2 sta_3
	
	TASK_STAT 4600 1 sta
	if @sta = 2
		option 200 ��ʼ����
	endif
	TASK_STAT 4614 1 sta_2
	if @sta_2 = 2
		option 600 ���������Ѳ�����
	endif
	TASK_STAT 4615 1 sta_3
	if @sta_3 = 2
		option 700 ��������֮������
	endif

	task_stat 4601 1 10
	if 10 = 2
		option 900 #����Ҫʲô�����أ�
	endif
	task_stat 4602 1 11
	if 11 = 2
		option 910 #����Ҫʲô�����أ�
	endif
	task_stat 4603 1 12
	if 12 = 2
		option 920 #����Ҫʲô���ﵰ�أ�
	endif
	
	task_add_option 4618 1 102
	
	TASKCYCLE_ADD_OPTION 145 1 800
	
	talk
endproc
	
PROC 101
	OPEN_NPC_SHOP
ENDPROC

PROC 102
	TASK_DO_OPTION 4618 0
ENDPROC

proc 200
	STRVAR sta not not_1 num

	TASK_STAT 4600 1 sta
	if @sta = 2
	
		;��Դ���
		GET_TASK_VAR 4600 0 32 1 not

		if @not >= 3
			TASK_FINI 4600 1
			PLAYER_CLOSE_DIALOG1 0 0
		else
			GET_TASK_VAR 4600 4 32 1 not_1
			if @not_1 = 1
				;��Դ�����0
				SET_TASK_VAR 4600 0 32 0 1
				;�����0
				SET_TASK_VAR 4600 4 32 0 1
			endif
		
			;20����
			rand num 20
			switch @num
				case 0
					call 300
				endcase

				case 1
					call 301
				endcase

				case 2
					call 302
				endcase
						
				case 3
					call 303
				endcase
						
				case 4
					call 304
				endcase
						
				case 5
					call 305
				endcase
						
				case 6
					call 306
				endcase
						
				case 7
					call 307
				endcase
						
				case 8
					call 308
				endcase
						
				case 9
					call 309
				endcase

				case 10
					call 310
				endcase
						
				case 11
					call 311
				endcase
						
				case 12
					call 312
				endcase
						
				case 13
					call 313
				endcase
				
				case 14
					call 314
				endcase
				
				case 15
					call 315
				endcase
				
				case 16
					call 316
				endcase
				
				case 17
					call 317
				endcase
				
				case 18
					call 318
				endcase
			endswitch
		endif
	endif
endproc

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506��������Ƶ�������ڵͼ۽������������һ�Ҫ�����칤�ߵģ�����ô�죿#n
	
	STRVAR num
	rand num 4
	if @num = 0
		option 401 ���ϼ�֮���б��˲�ռ�Ǳ���
		option 400 ���С��ͼ���Ͻǵ�BUG�����ٱ���
		option 401 �Ͻ���ϵ����Ҫ�绰����չ�ɳ��ں�����ϵ
		option 401 �����ܲ����ٱ��˵�
	elseif @num = 1
		option 400 ���С��ͼ���Ͻǵ�BUG�����ٱ���
		option 401 �Ͻ���ϵ����Ҫ�绰����չ�ɳ��ں�����ϵ
		option 401 �����ܲ����ٱ��˵�
		option 401 ���ϼ�֮���б��˲�ռ�Ǳ���
	elseif @num = 2
		option 401 �Ͻ���ϵ����Ҫ�绰����չ�ɳ��ں�����ϵ
		option 401 �����ܲ����ٱ��˵�
		option 401 ���ϼ�֮���б��˲�ռ�Ǳ���
		option 400 ���С��ͼ���Ͻǵ�BUG�����ٱ���
	else
		option 401 �����ܲ����ٱ��˵�
		option 401 ���ϼ�֮���б��˲�ռ�Ǳ���
		option 400 ���С��ͼ���Ͻǵ�BUG�����ٱ���
		option 401 �Ͻ���ϵ����Ҫ�绰����չ�ɳ��ں�����ϵ
	endif
	talk
endproc

proc 301
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506������˸���˵��½ĳ�ǹٷ���վ�ܹ���ȡ������Ϸ���ߡ���ҡ����ҡ����顢��������ô����#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 402 ���������ܲ��ܶ��һЩ
	option 400 ���С��ͼ���Ͻǵ�BUG�����ٱ���
	option 402 ��ͼ����Ⱥ����֮���ѣ���Ա����һ��ȥ��
	option 402 ����ȥ��
	elseif @num = 1
	option 400 ���С��ͼ���Ͻǵ�BUG�����ٱ���
	option 402 ��ͼ����Ⱥ����֮���ѣ���Ա����һ��ȥ��
	option 402 ����ȥ��
	option 402 ���������ܲ��ܶ��һЩ
	elseif @num = 2
	option 402 ��ͼ����Ⱥ����֮���ѣ���Ա����һ��ȥ��
	option 402 ����ȥ��
	option 402 ���������ܲ��ܶ��һЩ
	option 400 ���С��ͼ���Ͻǵ�BUG�����ٱ���
	else
	option 402 ����ȥ��
	option 402 ���������ܲ��ܶ��һЩ
	option 400 ���С��ͼ���Ͻǵ�BUG�����ٱ���
	option 402 ��ͼ����Ⱥ����֮���ѣ���Ա����һ��ȥ��
	endif
	talk
endproc

proc 302
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506�����˺ű�������ô�죿#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 404 ���½�һ���ţ����������ϴ��������
	option 404 ���͵��ˣ�����ν
	option 400 �󶨰�ȫ���䣬��ʱ��֤�����ܱ���
	option 404 �跨ȥ�����˵ĺ�
	elseif @num = 1
	option 404 ���͵��ˣ�����ν
	option 400 �󶨰�ȫ���䣬��ʱ��֤�����ܱ���
	option 404 �跨ȥ�����˵ĺ�
	option 404 ���½�һ���ţ����������ϴ��������
	elseif @num = 2
	option 400 �󶨰�ȫ���䣬��ʱ��֤�����ܱ���
	option 404 �跨ȥ�����˵ĺ�
	option 404 ���½�һ���ţ����������ϴ��������
	option 404 ���͵��ˣ�����ν
	else
	option 404 �跨ȥ�����˵ĺ�
	option 404 ���½�һ���ţ����������ϴ��������
	option 404 ���͵��ˣ�����ν
	option 400 �󶨰�ȫ���䣬��ʱ��֤�����ܱ���
	endif
	talk
endproc

proc 303
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506������ֱ������˺������޷�ȡ�ػ򱻶�����ܱ�������ô�죿#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 406 һ��һ���ԣ�����һ���ǶԵ�
	option 406 ����Ҳ��ֵ����Ǯ����������һ����
	option 406 �����������Ҳû�취
	option 400 ����̳�����ⷴ������������GM����
	elseif @num = 1	
	option 406 ����Ҳ��ֵ����Ǯ����������һ����
	option 406 �����������Ҳû�취
	option 400 ����̳�����ⷴ������������GM����
	option 406 һ��һ���ԣ�����һ���ǶԵ�
	elseif @num = 2
	option 406 �����������Ҳû�취
	option 400 ����̳�����ⷴ������������GM����
	option 406 һ��һ���ԣ�����һ���ǶԵ�
	option 406 ����Ҳ��ֵ����Ǯ����������һ����
	else
	option 400 ����̳�����ⷴ������������GM����
	option 406 һ��һ���ԣ�����һ���ǶԵ�
	option 406 ����Ҳ��ֵ����Ǯ����������һ����
	option 406 �����������Ҳû�취
	endif
	talk
endproc

proc 304
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506���Ԫ�Ĺ������ĸ���#n	
	
	STRVAR num
	rand num 4
	if @num = 0
	option 400 www.ljy0.com
	option 408 www.ljy1.com
	option 408 www.ljy2.com
	option 408 www.ljy3.com
	elseif @num = 1
	option 408 www.ljy1.com
	option 408 www.ljy2.com
	option 408 www.ljy3.com
	option 400 www.ljy0.com
	elseif @num = 2
	option 408 www.ljy2.com
	option 408 www.ljy3.com
	option 400 www.ljy0.com
	option 408 www.ljy1.com
	else
	option 408 www.ljy3.com
	option 400 www.ljy0.com
	option 408 www.ljy1.com
	option 408 www.ljy2.com
	endif
	talk
endproc

proc 305
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506���Ԫ����������Щ���ݣ�#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 410 ��������
	option 400 ����������������»����Ϸ���ϡ�����
	option 410 ��cctv��ȫ���������Ϣ����������վ�������Ƶ
	option 410 �߻�������
	elseif @num = 1
	option 400 ����������������»����Ϸ���ϡ�����
	option 410 ��cctv��ȫ���������Ϣ����������վ�������Ƶ
	option 410 �߻�������
	option 410 ��������
	elseif @num = 2
	option 410 ��cctv��ȫ���������Ϣ����������վ�������Ƶ
	option 410 �߻�������
	option 410 ��������
	option 400 ����������������»����Ϸ���ϡ�����
	else
	option 410 �߻�������
	option 410 ��������
	option 400 ����������������»����Ϸ���ϡ�����
	option 410 ��cctv��ȫ���������Ϣ����������վ�������Ƶ
	endif
	talk
endproc

proc 306
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506��Ϸ������˵���͵㿨����ң�ֻҪ�ѺŸ������Ϳ��ԣ������ô���أ�#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 412 ֱ�Ӹ�������ô�õ��°����ѵ�һ��������ȻҪ�Ͻ�
	option 412 �����ѵĺ�һ�����
	option 412 ����һ��
	option 400 ����ID����̳�����ⷴ������ȥ�ٱ�����
	elseif @num = 1
	option 412 �����ѵĺ�һ�����
	option 412 ����һ��
	option 400 ����ID����̳�����ⷴ������ȥ�ٱ�����
	option 412 ֱ�Ӹ�������ô�õ��°����ѵ�һ��������ȻҪ�Ͻ�
	elseif @num = 2
	option 412 ����һ��
	option 400 ����ID����̳�����ⷴ������ȥ�ٱ�����
	option 412 ֱ�Ӹ�������ô�õ��°����ѵ�һ��������ȻҪ�Ͻ�
	option 412 �����ѵĺ�һ�����
	else
	option 400 ����ID����̳�����ⷴ������ȥ�ٱ�����
	option 412 ֱ�Ӹ�������ô�õ��°����ѵ�һ��������ȻҪ�Ͻ�
	option 412 �����ѵĺ�һ�����
	option 412 ����һ��
	endif
	talk
endproc

proc 307
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506���Ԫ���˺Ź����Ĺ�����ַ�ǣ�#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 400 http://passport.wanyouyl.com
	option 414 http://www.ljy0.com
	option 414 http://bbs.ljy0.com
	option 414 http://pay.wanyouyl.com
	elseif @num = 1
	option 414 http://www.ljy0.com
	option 414 http://bbs.ljy0.com
	option 414 http://pay.wanyouyl.com
	option 400 http://passport.wanyouyl.com
	elseif @num = 2
	option 414 http://bbs.ljy0.com
	option 414 http://pay.wanyouyl.com
	option 400 http://passport.wanyouyl.com
	option 414 http://www.ljy0.com
	else
	option 414 http://pay.wanyouyl.com
	option 400 http://passport.wanyouyl.com
	option 414 http://www.ljy0.com
	option 414 http://bbs.ljy0.com
	endif
	talk
endproc

proc 308
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506�����г��ĸ���;�����������˽⵽���Ԫ���µĻ��̬��#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 400 ���Ԫ������http://www.ljy0.com
	option 400 ���Ԫ�ٷ���̳��http://bbs.ljy0.com
	option 400 ��������ý����վ����17173�����ˡ����桢���ף�
	option 416 ��Ϸ������Ƶ�������չ�XX�������Ϣ
	elseif @num = 1
	option 400 ���Ԫ�ٷ���̳��http://bbs.ljy0.com
	option 400 ��������ý����վ����17173�����ˡ����桢���ף�
	option 416 ��Ϸ������Ƶ�������չ�XX�������Ϣ
	option 400 ���Ԫ������http://www.ljy0.com
	elseif @num = 2
	option 400 ��������ý����վ����17173�����ˡ����桢���ף�
	option 416 ��Ϸ������Ƶ�������չ�XX�������Ϣ
	option 400 ���Ԫ������http://www.ljy0.com
	option 400 ���Ԫ�ٷ���̳��http://bbs.ljy0.com
	else
	option 416 ��Ϸ������Ƶ�������չ�XX�������Ϣ
	option 400 ���Ԫ������http://www.ljy0.com
	option 400 ���Ԫ�ٷ���̳��http://bbs.ljy0.com
	option 400 ��������ý����վ����17173�����ˡ����桢���ף�
	endif
	talk
endproc

proc 309
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506��Ϸ���Լ�������ͻȻ������Ҫ��ҡ����ҡ�װ���������ô����#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 418 ֱ�Ӹ���
	option 418 �ѵú��ѿ��У���̫�ò������ӣ�������˵ֱ�Ӹ�
	option 400 ��ϸѯ�ʣ�ͨ��������ʽ���绰����ϵ��ȷ�����
	option 418 ֱ������
	elseif @num = 1
	option 418 �ѵú��ѿ��У���̫�ò������ӣ�������˵ֱ�Ӹ�
	option 400 ��ϸѯ�ʣ�ͨ��������ʽ���绰����ϵ��ȷ�����
	option 418 ֱ������
	option 418 ֱ�Ӹ���
	elseif @num = 2
	option 400 ��ϸѯ�ʣ�ͨ��������ʽ���绰����ϵ��ȷ�����
	option 418 ֱ������
	option 418 ֱ�Ӹ���
	option 418 �ѵú��ѿ��У���̫�ò������ӣ�������˵ֱ�Ӹ�
	else
	option 418 ֱ������
	option 418 ֱ�Ӹ���
	option 418 �ѵú��ѿ��У���̫�ò������ӣ�������˵ֱ�Ӹ�
	option 400 ��ϸѯ�ʣ�ͨ��������ʽ���绰����ϵ��ȷ�����
	endif
	talk
endproc

proc 310
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506��������Ϸ�������ѣ�Ȼ�������ȥXXX��վ��ȡ��ҡ�������������ʿ��������ô����#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 420 �ۣ���ô�õ��£���Ȼ�ǸϽ�ȥ���ˣ�
	option 420 ���˺������������������æȥ�죡
	option 400 ƭ�ӣ�����IDȥ��̳�����ⷴ�������ٱ���
	option 420 ����ת�������ѣ������ѽй���һ��ȥ��
	elseif @num = 1
	option 420 ���˺������������������æȥ�죡
	option 400 ƭ�ӣ�����IDȥ��̳�����ⷴ�������ٱ���
	option 420 ����ת�������ѣ������ѽй���һ��ȥ��
	option 420 �ۣ���ô�õ��£���Ȼ�ǸϽ�ȥ���ˣ�
	elseif @num = 2
	option 400 ƭ�ӣ�����IDȥ��̳�����ⷴ�������ٱ���
	option 420 ����ת�������ѣ������ѽй���һ��ȥ��
	option 420 �ۣ���ô�õ��£���Ȼ�ǸϽ�ȥ���ˣ�
	option 420 ���˺������������������æȥ�죡
	else
	option 420 ����ת�������ѣ������ѽй���һ��ȥ��
	option 420 �ۣ���ô�õ��£���Ȼ�ǸϽ�ȥ���ˣ�
	option 420 ���˺������������������æȥ�죡
	option 400 ƭ�ӣ�����IDȥ��̳�����ⷴ�������ٱ���
	endif
	talk
endproc

proc 311
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506���������ȣ�����˵������ȷ�ģ�#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 422 ���Լ�ȥ���������о���
	option 422 ����С�ţ�С�Ż����;���
	option 400 ��̬�����ľ���ֻ���Լ��ĵȼ��й�
	option 422 ʹ�ø����еġ����浶�������ס�����پ���
	elseif @num = 1
	option 422 ����С�ţ�С�Ż����;���
	option 400 ��̬�����ľ���ֻ���Լ��ĵȼ��й�
	option 422 ʹ�ø����еġ����浶�������ס�����پ���
	option 422 ���Լ�ȥ���������о���
	elseif @num = 2
	option 400 ��̬�����ľ���ֻ���Լ��ĵȼ��й�
	option 422 ʹ�ø����еġ����浶�������ס�����پ���
	option 422 ���Լ�ȥ���������о���
	option 422 ����С�ţ�С�Ż����;���
	else
	option 422 ʹ�ø����еġ����浶�������ס�����پ���
	option 422 ���Լ�ȥ���������о���
	option 422 ����С�ţ�С�Ż����;���
	option 400 ��̬�����ľ���ֻ���Լ��ĵȼ��й�
	endif
	talk
endproc

proc 312
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c18650664�����ν��׵Ĳ��Ͻ�ʲô���֣�#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 424 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 424 ���΢���������顢��ֺϽ�
	option 400 ������顢����Ͻ�
	option 424 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	elseif @num = 1	
	option 424 ���΢���������顢��ֺϽ�
	option 400 ������顢����Ͻ�
	option 424 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	option 424 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	elseif @num = 2	
	option 400 ������顢����Ͻ�
	option 424 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	option 424 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 424 ���΢���������顢��ֺϽ�
	else
	option 424 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	option 424 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 424 ���΢���������顢��ֺϽ�
	option 400 ������顢����Ͻ�
	endif
	talk
endproc

proc 313
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c18650669�������׵Ĳ��Ͻ�ʲô���֣�#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 400 ������顢����Ͻ�
	option 426 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	elseif @num = 1
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 400 ������顢����Ͻ�
	option 426 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	elseif @num = 2	
	option 400 ������顢����Ͻ�
	option 426 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	else
	option 426 ��ҫ΢������ҫ��顢��ҫ�Ͻ�
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 426 ��ͭ΢������ͭ��顢��ͭ�Ͻ�
	option 400 ������顢����Ͻ�
	endif
	talk
endproc

proc 314
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506���＼�����á�����ͨ����λNPC���У�#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 428 ���ﵼʦ-����
	option 400 ����ѵ��Ա-���¶�
	option 428 ����ѵ��ʦ-������
	option 428 �����ʦ-̩��
	elseif @num = 1
	option 400 ����ѵ��Ա-���¶�
	option 428 ����ѵ��ʦ-������
	option 428 �����ʦ-̩��
	option 428 ���ﵼʦ-����
	elseif @num = 2
	option 428 ����ѵ��ʦ-������
	option 428 �����ʦ-̩��
	option 428 ���ﵼʦ-����
	option 400 ����ѵ��Ա-���¶�
	else
	option 428 �����ʦ-̩��
	option 428 ���ﵼʦ-����
	option 400 ����ѵ��Ա-���¶�
	option 428 ����ѵ��ʦ-������
	endif
	talk
endproc

proc 315
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506�ܹ���ñ�ְҵ��װ����װ����¯�ر�������ȡ���ĸ�NPC��#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 430 ��ѧʿ-����
	option 430 ������·��-������
	option 430 �����ʦ-��ŵ��
	option 400 ����-��������
	elseif @num = 1
	option 430 ������·��-������
	option 430 �����ʦ-��ŵ��
	option 400 ����-��������
	option 430 ��ѧʿ-����
	elseif @num = 2
	option 430 �����ʦ-��ŵ��
	option 400 ����-��������
	option 430 ��ѧʿ-����
	option 430 ������·��-������
	else
	option 400 ����-��������
	option 430 ��ѧʿ-����
	option 430 ������·��-������
	option 430 �����ʦ-��ŵ��
	endif
	talk
endproc

proc 316
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506װ�����ף�����װ����Ϊ��װ�����ĸ�NPC��#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 400 װ����⡢���״�ʦ-�ݽܷ�
	option 432 װ������ʦ-˹�Ϸ���
	option 432 װ��ǿ����ʦ-�˶���
	option 432 �������Ա-˹�ٷ�˹
	elseif @num = 1
	option 432 װ������ʦ-˹�Ϸ���
	option 432 װ��ǿ����ʦ-�˶���
	option 432 �������Ա-˹�ٷ�˹
	option 400 װ����⡢���״�ʦ-�ݽܷ�
	elseif @num = 2
	option 432 װ��ǿ����ʦ-�˶���
	option 432 �������Ա-˹�ٷ�˹
	option 400 װ����⡢���״�ʦ-�ݽܷ�
	option 432 װ������ʦ-˹�Ϸ���
	else
	option 432 �������Ա-˹�ٷ�˹
	option 400 װ����⡢���״�ʦ-�ݽܷ�
	option 432 װ������ʦ-˹�Ϸ���
	option 432 װ��ǿ����ʦ-�˶���
	endif
	talk
endproc

proc 317
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506װ������ת�ƣ�����ͬ�ȼ�����λ����װ�ĸ�������ת�Ƶ���һ��װ���ϣ����ĸ�NPC��#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 400 װ������ʦ-˹�Ϸ���
	option 434 װ��ǿ����ʦ-�˶���
	option 434 װ����⡢���״�ʦ-�ݽܷ�
	option 434 �߼���������-÷��
	elseif @num = 1
	option 434 װ��ǿ����ʦ-�˶���
	option 434 װ����⡢���״�ʦ-�ݽܷ�
	option 434 �߼���������-÷��
	option 400 װ������ʦ-˹�Ϸ���
	elseif @num = 2
	option 434 װ����⡢���״�ʦ-�ݽܷ�
	option 434 �߼���������-÷��
	option 400 װ������ʦ-˹�Ϸ���
	option 434 װ��ǿ����ʦ-�˶���
	else
	option 434 �߼���������-÷��
	option 400 װ������ʦ-˹�Ϸ���
	option 434 װ��ǿ����ʦ-�˶���
	option 434 װ����⡢���״�ʦ-�ݽܷ�
	endif
	talk
endproc

proc 318
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�Ķ�������Ŀ������������������һ�⣬�����¿�ʼ���⣬���������������������
	add_talk #b#b
	add_talk #b#b#c186506���Ľ��븱���ķ�ʽ�ǣ�#n
	
	STRVAR num
	rand num 4
	if @num = 0
	option 436 �ܵ������ſڣ������˺�ֱ�ӽ�ȥ
	option 436 ���������������Ҫ����������Ҫ������������
	option 400 ͨ��С��ͼ�ϵġ�������ť����ʱ������Ľ�
	option 436 ȥ������
	elseif @num = 1
	option 436 ���������������Ҫ����������Ҫ������������
	option 400 ͨ��С��ͼ�ϵġ�������ť����ʱ������Ľ�
	option 436 ȥ������
	option 436 �ܵ������ſڣ������˺�ֱ�ӽ�ȥ
	elseif @num = 2
	option 400 ͨ��С��ͼ�ϵġ�������ť����ʱ������Ľ�
	option 436 ȥ������
	option 436 �ܵ������ſڣ������˺�ֱ�ӽ�ȥ
	option 436 ���������������Ҫ����������Ҫ������������
	else
	option 436 ȥ������
	option 436 �ܵ������ſڣ������˺�ֱ�ӽ�ȥ
	option 436 ���������������Ҫ����������Ҫ������������
	option 400 ͨ��С��ͼ�ϵġ�������ť����ʱ������Ľ�
	endif
	talk
endproc

proc 400
	STRVAR sta num
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		GET_TASK_VAR 4600 0 32 1 not
		add not 1
		SET_TASK_VAR 4600 0 32 @not 1
		
		if @not >= 3
			TASK_FINI 4600 1
			PLAYER_CLOSE_DIALOG1 0 0
		else
			call 200
		endif
	endif
endproc

proc 401
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9�ܶ�ƭ�������������ǵ�̰С����������������ʵ��������������������һ������ǧ�������������û����ѵ���ͣ�#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 402
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9����ƭ������Щ����������һ������ġ�ľ����վ������������ʱ���ͨ��ľ���ȡ�����˺ţ����������棡�Ը�����ƭ��������׼���ԪΨһ������www.ljy0.com�������Թ����ϲ�ѯ�����е����Ԫ�ٷ��Ƴ��Ļ��#n
		
		option 500 ���¿�ʼ����
	endif
		
	talk
endproc

proc 404
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9���ۺ�ʱ�εأ���Ҫ����Լ��ķ�ƭ������ʶ��������δȻ��ͨ�������ġ��˺Ź�����ť����½�������ģ����ݡ���ȫ��ʾ��������ݣ���д��ȫ���䣬��֤��ȫ���䣬���ܱ��������Լ�Զ����ţ�#n
	
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 406
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9���������������������뵽��̳�����ⷴ������������GM������ϣ������Զ����������������������ǵ�������������һ������ʹ���˺���Ҳ������������ԣ��������˺Ű�ȫ��Ϊ��Ҫ��ͨ�������ġ��˺Ź�����ť����½�������ģ����ܱ�������֤��ȫ����ɣ�#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 408
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9�����ƭ�����ţ���ؼ���һ��������׼������www.ljy0.com#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 410
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9����ƭ����ĳ���������Ƶ�������ġ���XXX���������ȥ�ٶ�XXX�����������99.99����ƭ�ӣ��벻Ҫ���Ź�����û�г��ֹ��ġ�����������Ҫ�˽�������ȫ��ĸ����������½���Ԫ������www.ljy0.com#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 412
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9���ڵ��ż����ǳ������Щ���ü򵥵���������ױ��ƽ⣬���ԣ��벻Ҫ���Լ����˺�͸¶��İ���ˣ�����Ҫ������Щ�ǹٷ��������н��ʼ������Ｐ������Ϣ���ٷ���Ա������������Ҫ������ĸ�����Ϣ��#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 414
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9�˺Ź����ܱ�������Ĺ�����ַ��http://passport.wanyouyl.com
		add_talk #b#b
	    add_talk #b#b#c0181f9���Ԫ������http://www.ljy0.com
		add_talk #b#b
	    add_talk #b#b#c0181f9���Ԫ�ٷ���̳��http://bbs.ljy0.com
		add_talk #b#b
	    add_talk #b#b#c0181f9���Ԫ�ٷ���ֵ���ĵ�ַ��http://pay.wanyouyl.com#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 416
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9�����Ԫ����еġ����������ȡ����Ʒ���ǰ󶨵ģ����ɽ��׵�Ŷ�����ԣ���Щ����������XX����Ĵ����ƭ��Ӵ���ر�����Щ����ȥ�ٶȶ���ȥ�����������»��Ϣ�ģ�����׼���ԪΨһ������www.ljy0.com#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc


proc 418
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9����ƭ��������������һ����Ϸ�˺ţ��ھ��������ߺ������вƲ�֮ǰ���ȼٽ�����˵�������������к��ѡ������Ա��Ǯ���Ȼ����ͬ�����˵ĲƲ�һ��ת�ƣ����ԣ���������ʵ�л������Ԫ�������������Ǯ����ǵ�ȷ�϶Է��Ƿ�����Ϊ���ѱ���Ӵ��#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 420
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9����ƭ��������Ϸ�з��������Ϣ����ȡ��ҡ������������ϡ����Ʒ����������½һ���ǹٷ���վ����վ�Ϲ���ľ����¼�������˺���Ϣ�������ƭ�����ţ����������н���Ϣ�����ס���ԪΨһ������www.ljy0.com#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 422
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9����������ɽ�������οռ�һ�������ڶ�̬��������̬�����ľ���ֻ���Լ��ĵȼ��йأ������ѵĵȼ��޹أ���С���ܹ��ø����еĹ����Ѷȣ��ȼ������ͣ��ø������̸����ס���Ѹ�٣�����������������õľ��飬��Ϊ��ֻȡ����������ĵȼ�������е�������
		add_talk #b#b#c0181f9���⣬ʹ�ø����еġ����浶�������ס�Ҳ��������������õľ�����٣�Ψһ�������õľ�����ٵ�;���ǣ��ж����뿪�ˣ��޹غ����ĵȼ���ְҵ��#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 424
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9���Ρ����Σ���������ָ���������Ľ��ײ��϶�������Ͻ���ͨ��ʧ��֮����������������ϳɡ�#n
		
		option 500 ���¿�ʼ����
	endif
		
	talk
endproc

proc 426
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9���Ρ����Σ���������ָ���������Ľ��ײ��϶�������Ͻ���ͨ��ʧ��֮����������������ϳɡ�#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 428
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9����ѵ��Ա-���¶࣬�����԰���������м������á�����������������������������ǿ����������ʱ����·����ɸ۸�����#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 430
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9����-��������������ʱ����Ϸ����������Ա˹�ٷ�˹��Զ������40����ʼ���Ϳ��������Ƕ����ܡ���¯�ر���������ɫ¯���м��ʻ�ñ�ְҵ��ǰ�ȼ��ε���װ����ɫ¯�ӱ�Ȼ���ñ�ְҵ����װ������ʯ����һ�����ʻ��÷�װ��
		add_talk #b#b#c0181f9����������������¯�ر�������ʱ���ǽ�ɫ��¯�������Ȼ���һ�Ž���¯ƾ֤�����ܵ�һ������ʱ���Ϳ����ڹ������󴦽��ס��̡�������ɫ��¯�����ɽ�ɫ��¯��#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 432
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9������ڡ��ݺϳС����鿴װ�����׵�������⣬�Լ��ϳ�װ������Ҫ�Ĳ��ϳ������ϳɷ�ʽ��
		add_talk #b#b#c0181f9װ���������ڡ�װ����⡢���״�ʦ-�ݽܷᡱ�����С�#n
	
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 434
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9װ������ת���ڡ�װ������ʦ-˹�Ϸ��桱�����У�ת������Ҫ��ת��ʯ���ɴӵع���Ĺ������ϻ�á�������װ���Կ���ת�Ƶ���ͬ��λ���ȼ��ķ�װ��Ӵ��ת������Ҫ�ĵ��߿����̳�-��ɫ����-�ɳ�ҳ���й���#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 436
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c0181f9��ϸ�о���Ļ���Ͻ�С��ͼ�ϵġ������ְ�ťӴ�����ǽ��븱���Ŀ�ݷ�ʽ������֮�⻹��ս��������ť����ҽ��װ�ť������ְ�ť����������Ϸ��ø����㣡#n
		
		option 500 ���¿�ʼ����
	endif
	
	talk
endproc

proc 500
	STRVAR sta
	TASK_STAT 4600 1 sta
	if @sta = 2
		
		;��Դ�����0
		SET_TASK_VAR 4600 0 32 0 1
		;1�Ļ�����ʾһ��Ҫ�Ѵ�Դ�����0
		SET_TASK_VAR 4600 4 32 1 1
		call 200
	endif
	
endproc

PROC 600
	ENTER_PWORLD 514 0 0
	;���븱��
ENDPROC

PROC 700
	ENTER_PWORLD 515 0 0
	;���븱��
ENDPROC

PROC 800
	TASKCYCLE_DO_OPTION 145 1
ENDPROC

proc 900
	strvar ret
	
	dialog 3
	add_talk $_npc$:
	get_task_var 4601 0 8 1 ret	
	if ret = 0
		add_talk #b#b����Ҫһ����ɫ#c186506Ұս��#n��12��սʿ�ؼף������Բ���
	endif
	if ret = 1
		add_talk #b#b����Ҫһ����ɫ#c186506������#n��12����ʦ�ؼף������Բ���
	endif
	if ret = 2
		add_talk #b#b����Ҫһ����ɫ#c186506��սװ#n��12��ǹ���ؼף������Բ���
	endif
	if ret = 3
		add_talk #b#b����Ҫһ����ɫ#c186506����װ#n��12����˾�ؼף������Բ���
	endif
	if ret = 4
		add_talk #b#b����Ҫһ����ɫ#c186506����ͷ��#n��13��սʿͷ���������Բ���
	endif
	if ret = 5
		add_talk #b#b����Ҫһ����ɫ#c186506����ͷ��#n��13����ʦͷ���������Բ���
	endif
	if ret = 6
		add_talk #b#b����Ҫһ����ɫ#c186506������ñ#n��13��ǹ��ͷ���������Բ���
	endif
	if ret = 7
		add_talk #b#b����Ҫһ����ɫ#c186506ʮ������#n��13����˾ͷ���������Բ���
	endif
	if ret = 8
		add_talk #b#b����Ҫһ����ɫ#c186506��װ���#n��14��սʿ���Σ������Բ���
	endif
	if ret = 9
		add_talk #b#b����Ҫһ����ɫ#c186506Ԫ�ػ���#n��14����ʦ���Σ������Բ���
	endif
	if ret = 10
		add_talk #b#b����Ҫһ����ɫ#c186506�ȳ�����#n��14��ǹ�����Σ������Բ���
	endif
	if ret = 11
		add_talk #b#b����Ҫһ����ɫ#c186506��ͽ���#n��14����˾���Σ������Բ���
	endif
	if ret = 12
		add_talk #b#b����Ҫһ����ɫ#c186506�������#n��15��սʿ��ף������Բ���
	endif
	if ret = 13
		add_talk #b#b����Ҫһ����ɫ#c186506���ļ��#n��15����ʦ��ף������Բ���
	endif
	if ret = 14
		add_talk #b#b����Ҫһ����ɫ#c186506Ұ��II��#n��15��ǹ�ּ�ף������Բ���
	endif
	if ret = 15
		add_talk #b#b����Ҫһ����ɫ#c186506ʮ�ּ��#n��15����˾��ף������Բ���
	endif
	if ret = 16
		add_talk #b#b����Ҫһ����ɫ#c186506սʿѥ#n��16��սʿЬ�ӣ������Բ���
	endif
	if ret = 17
		add_talk #b#b����Ҫһ����ɫ#c186506����ѥ#n��16����ʦЬ�ӣ������Բ���
	endif
	if ret = 18
		add_talk #b#b����Ҫһ����ɫ#c186506ð��ѥ#n��16��ǹ��Ь�ӣ������Բ���
	endif
	if ret = 19
		add_talk #b#b����Ҫһ����ɫ#c186506����ѥ#n��16����ʦЬ�ӣ������Բ���
	endif
	
		add_talk #b#b����Ҫ�ύ����Ʒ�����·��Ŀ���
		
		NPC_COMMIT_OPT 901

	talk
endproc  

proc 901
	strvar ID fie gri ret
	GET_COMMIT_ITEM_INFO 1 ID
	
	GET_COMMIT_ITEM_INFO 2 fie
	;λ��
	GET_COMMIT_ITEM_INFO 3 gri
	;����
	
	get_task_var 4601 0 8 1 ret 
	if ret = 0
		if ID = 2019902
			DEC_ITEM_NUM_BYIDX 2019902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�Ұս����������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 1
		if ID = 2029902
			DEC_ITEM_NUM_BYIDX 2029902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ������£�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 2
		if ID = 2039902
			DEC_ITEM_NUM_BYIDX 2039902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĿ�սװ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 3
		if ID = 2049902
			DEC_ITEM_NUM_BYIDX 2049902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĵ���װ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 4
		if ID = 2119902
			DEC_ITEM_NUM_BYIDX 2119902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵľ���ͷ����������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 5
		if ID = 2129902
			DEC_ITEM_NUM_BYIDX 2129902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵķ���ͷ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 6
		if ID = 2139902
			DEC_ITEM_NUM_BYIDX 2139902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�������ñ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 7
		if ID = 2149902
			DEC_ITEM_NUM_BYIDX 2149902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ʮ�����Σ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 8
		if ID = 2419902
			DEC_ITEM_NUM_BYIDX 2419902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ���װ��ߣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 9
		if ID = 2429902
			DEC_ITEM_NUM_BYIDX 2429902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�Ԫ�ػ��Σ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 10
		if ID = 2439902
			DEC_ITEM_NUM_BYIDX 2439902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ��ȳ����Σ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 11
		if ID = 2449902
			DEC_ITEM_NUM_BYIDX 2449902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĽ�ͽ��ߣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 12
		if ID = 2219902
			DEC_ITEM_NUM_BYIDX 2219902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵľ�����ף�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 13
		if ID = 2229902
			DEC_ITEM_NUM_BYIDX 2229902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵķ��ļ�ף�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 14
		if ID = 1279902
			DEC_ITEM_NUM_BYIDX 1279902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�Ұ��II�ͣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 15
		if ID = 2249902
			DEC_ITEM_NUM_BYIDX 2249902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ʮ�ּ�ף�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 16
		if ID = 2619902
			DEC_ITEM_NUM_BYIDX 2619902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�սʿѥ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 17
		if ID = 2629902
			DEC_ITEM_NUM_BYIDX 2629902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵķ���ѥ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 18
		if ID = 2639902
			DEC_ITEM_NUM_BYIDX 2639902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ð��ѥ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 19
		if ID = 2649902
			DEC_ITEM_NUM_BYIDX 2649902 1 @fie @gri 1
			task_fini 4601 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����ѥ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 910
	strvar ret
	
	dialog 3
	add_talk $_npc$:
	get_task_var 4602 0 8 1 ret	
	if ret = 0
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506��յ�#n��11��սʿ���������������Բ���
	endif
	if ret = 1
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506����#n��21��սʿ���������������Բ���
	endif
	if ret = 2
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506��ʿ��#n��31��սʿ���������������Բ���
	endif
	if ret = 3
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506Ѫ�ȷ�â#n��41��սʿ���������������Բ���
	endif
	if ret = 4
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506��������#n��51��սʿ���������������Բ���
	endif
	if ret = 5
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506������#n��17��սʿ���ֶ��ƣ������Բ���
	endif
	if ret = 6
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506�׽��#n��27��սʿ���ֶ��ƣ������Բ���
	endif
	if ret = 7
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506���Ƕ�#n��37��սʿ���ֶ��ƣ������Բ���
	endif
	if ret = 8
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506ʨ��֮��#n��47��սʿ���ֶ��ƣ������Բ���
	endif
	if ret = 9
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506��������#n��57��սʿ���ֶ��ƣ������Բ���
	endif
	if ret = 10
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506������#n��11����ʦ���������������Բ���
	endif
	if ret = 11
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506��ľ��#n��21����ʦ���������������Բ���
	endif
	if ret = 12
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506������#n��31����ʦ���������������Բ���
	endif
	if ret = 13
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506ս��֮��#n��41����ʦ���������������Բ���
	endif
	if ret = 14
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506����Ȩ��#n��51����ʦ���������������Բ���
	endif
	if ret = 15
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506������#n��17����ʦ���ַ����������Բ���
	endif
	if ret = 16
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506Ѫɫޱ#n��27����ʦ���ַ����������Բ���
	endif
	if ret = 17
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506���ӡ��#n��37����ʦ���ַ����������Բ���
	endif
	if ret = 18
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506������Ԩ#n��47����ʦ���ַ����������Բ���
	endif
	if ret = 19
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506�Ͻ�ָ��#n��57����ʦ���ַ����������Բ���
	endif
	if ret = 20
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506����II#n��11��ǹ�����������������Բ���
	endif
	if ret = 21
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506����III#n��21��ǹ�����������������Բ���
	endif
	if ret = 22
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506����I#n��31��ǹ�����������������Բ���
	endif
	if ret = 23
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506����II#n��41��ǹ�����������������Բ���
	endif
	if ret = 24
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506����III#n��51��ǹ�����������������Բ���
	endif
	if ret = 25
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506�ش�II#n��17��ǹ�ָ��������������Բ���
	endif
	if ret = 26
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506�ش�III#n��27��ǹ�ָ��������������Բ���
	endif
	if ret = 27
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506����I#n��37��ǹ�ָ��������������Բ���
	endif
	if ret = 28
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506����II#n��47��ǹ�ָ��������������Բ���
	endif
	if ret = 29
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506����III#n��57��ǹ�ָ��������������Բ���
	endif
	if ret = 30
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506������#n��11����˾���������������Բ���
	endif
	if ret = 31
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506��ʿ��#n��21����˾���������������Բ���
	endif
	if ret = 32
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506��Ӱ��#n��31����˾���������������Բ���
	endif
	if ret = 33
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506��֮��#n��41����˾���������������Բ���
	endif
	if ret = 34
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506����Ȩ��#n��51����˾���������������Բ���
	endif
	if ret = 35
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506�ǹ��#n��17����˾����ʥ���������Բ���
	endif
	if ret = 36
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506ʥ�ɵ�#n��27����˾����ʥ���������Բ���
	endif
	if ret = 37
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506ʥ����#n��37����˾����ʥ���������Բ���
	endif
	if ret = 38
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506�����Ϣ#n��47����˾����ʥ���������Բ���
	endif
	if ret = 39
		add_talk #b#b����Ҫһ�Ѱ�ɫ#c186506ʮ�ֽ���#n��57����˾����ʥ���������Բ���
	endif
	
		add_talk #b#b����Ҫ�ύ����Ʒ�����·��Ŀ���
		
		NPC_COMMIT_OPT 911

	talk
endproc  

proc 911
	strvar ID fie gri ret
	GET_COMMIT_ITEM_INFO 1 ID
	
	GET_COMMIT_ITEM_INFO 2 fie
	;λ��
	GET_COMMIT_ITEM_INFO 3 gri
	;����
	
	get_task_var 4602 0 8 1 ret 
	if ret = 0
		if ID = 1019902
			DEC_ITEM_NUM_BYIDX 1019902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĽ�յ���������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 1
		if ID = 1019903
			DEC_ITEM_NUM_BYIDX 1019903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵľ��𵶣�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 2
		if ID = 1019904
			DEC_ITEM_NUM_BYIDX 1019904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ���ʿ����������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 3
		if ID = 1019905
			DEC_ITEM_NUM_BYIDX 1019905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�Ѫ�ȷ�â��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 4
		if ID = 1019906
			DEC_ITEM_NUM_BYIDX 1019906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĸ������У�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 5
		if ID = 1069902
			DEC_ITEM_NUM_BYIDX 1069902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ������ܣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 6
		if ID = 1069903
			DEC_ITEM_NUM_BYIDX 1069903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵİ׽�ܣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 7
		if ID = 1069904
			DEC_ITEM_NUM_BYIDX 1069904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ����Ƕܣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 8
		if ID = 1069905
			DEC_ITEM_NUM_BYIDX 1069905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ʨ��֮�ܣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 9
		if ID = 1069906
			DEC_ITEM_NUM_BYIDX 1069906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ��������ӣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 10
		if ID = 1109902
			DEC_ITEM_NUM_BYIDX 1109902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĳ����ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 11
		if ID = 1109903
			DEC_ITEM_NUM_BYIDX 1109903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĺ�ľ�ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 12
		if ID = 1109904
			DEC_ITEM_NUM_BYIDX 1109904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ������ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 13
		if ID = 1109905
			DEC_ITEM_NUM_BYIDX 1109905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ս��֮�ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 14
		if ID = 1109906
			DEC_ITEM_NUM_BYIDX 1109906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĹ���Ȩ�ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 15
		if ID = 1189902
			DEC_ITEM_NUM_BYIDX 1189902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĶ����򣿲�����ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 16
		if ID = 1189903
			DEC_ITEM_NUM_BYIDX 1189903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�Ѫɫޱ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 17
		if ID = 1189904
			DEC_ITEM_NUM_BYIDX 1189904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ����ӡ�ǣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 18
		if ID = 1189905
			DEC_ITEM_NUM_BYIDX 1189905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵľ�����Ԩ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 19
		if ID = 1189906
			DEC_ITEM_NUM_BYIDX 1189906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĺϽ�ָ�ƣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 20
		if ID = 1209902
			DEC_ITEM_NUM_BYIDX 1209902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����II��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 21
		if ID = 1209903
			DEC_ITEM_NUM_BYIDX 1209903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����III��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 22
		if ID = 1209904
			DEC_ITEM_NUM_BYIDX 1209904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����I��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 23
		if ID = 1209905
			DEC_ITEM_NUM_BYIDX 1209905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����II��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 24
		if ID = 1209906
			DEC_ITEM_NUM_BYIDX 1209906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����III��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 25
		if ID = 1289902
			DEC_ITEM_NUM_BYIDX 1289902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ��ش�II��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 26
		if ID = 1289903
			DEC_ITEM_NUM_BYIDX 1289903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ��ش�III��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 27
		if ID = 1289904
			DEC_ITEM_NUM_BYIDX 1289904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����I��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 28
		if ID = 1289905
			DEC_ITEM_NUM_BYIDX 1289905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����II��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 29
		if ID = 1289906
			DEC_ITEM_NUM_BYIDX 1289906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����III��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 30
		if ID = 1509902
			DEC_ITEM_NUM_BYIDX 1509902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ������ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 31
		if ID = 1509903
			DEC_ITEM_NUM_BYIDX 1509903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ���ʿ�ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 32
		if ID = 1509904
			DEC_ITEM_NUM_BYIDX 1509904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĻ�Ӱ�ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 33
		if ID = 1509905
			DEC_ITEM_NUM_BYIDX 1509905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ���֮�ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 34
		if ID = 1509906
			DEC_ITEM_NUM_BYIDX 1509906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵķ���Ȩ�ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 35
		if ID = 1589902
			DEC_ITEM_NUM_BYIDX 1589902 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ��ǹ�磿������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 36
		if ID = 1589903
			DEC_ITEM_NUM_BYIDX 1589903 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ʥ�ɵ䣿������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
	endif
	endif
	if ret = 37
		if ID = 1589904
			DEC_ITEM_NUM_BYIDX 1589904 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ʥ���󣿲�����ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 38
		if ID = 1589905
			DEC_ITEM_NUM_BYIDX 1589905 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ������Ϣ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 39
		if ID = 1589906
			DEC_ITEM_NUM_BYIDX 1589906 1 @fie @gri 1
			task_fini 4602 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ʮ�ֽ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 920
	strvar ret
	
	dialog 3
	add_talk $_npc$:
	get_task_var 4603 0 8 1 ret	
	if ret = 0
		add_talk #b#b����Ҫһֻ#c186506������ﵰ#n�����Բ���
	endif
	if ret = 1
		add_talk #b#b����Ҫһֻ#c186506Ұ����ﵰ#n�����Բ���
	endif
	if ret = 2
		add_talk #b#b����Ҫһֻ#c186506�����˳��ﵰ#n�����Բ���
	endif
	if ret = 3
		add_talk #b#b����Ҫһֻ#c186506����������ﵰ#n�����Բ���
	endif
	if ret = 4
		add_talk #b#b����Ҫһֻ#c186506ͺ�ճ��ﵰ#n�����Բ���
	endif
	if ret = 5
		add_talk #b#b����Ҫһֻ#c186506�Ĺ��������ﵰ#n�����Բ���
	endif
	if ret = 6
		add_talk #b#b����Ҫһֻ#c186506���ݻ����ﵰ#n�����Բ���
	endif
	if ret = 7
		add_talk #b#b����Ҫһֻ#c186506Ы�ӳ��ﵰ#n�����Բ���
	endif
	
		add_talk #b#b����Ҫ�ύ����Ʒ�����·��Ŀ���
		
		NPC_COMMIT_OPT 921

	talk
endproc  

proc 921
	strvar ID fie gri ret
	GET_COMMIT_ITEM_INFO 1 ID
	
	GET_COMMIT_ITEM_INFO 2 fie
	;λ��
	GET_COMMIT_ITEM_INFO 3 gri
	;����
	
	get_task_var 4603 0 8 1 ret 
	if ret = 0
		if ID = 5610100
			DEC_ITEM_NUM_BYIDX 5610100 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ�������������ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 1
		if ID = 5610101
			DEC_ITEM_NUM_BYIDX 5610101 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ�������Ұ����ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 2
		if ID = 5610102
			DEC_ITEM_NUM_BYIDX 5610102 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������������˳��ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 3
		if ID = 5611116
			DEC_ITEM_NUM_BYIDX 5611116 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ�����������������ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 4
		if ID = 5610104
			DEC_ITEM_NUM_BYIDX 5610104 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ�������ͺ�ճ��ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 5
		if ID = 5610106
			DEC_ITEM_NUM_BYIDX 5610106 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ��������Ĺ��������ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 6
		if ID = 5610107
			DEC_ITEM_NUM_BYIDX 5610107 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǽ��ݻ����ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 7
		if ID = 5610105
			DEC_ITEM_NUM_BYIDX 5610105 1 @fie @gri 1
			task_fini 4603 1
			call 999
		else
			add_sys_msg_start
			add_sys_msg ��ȷ�������Ы�ӳ��ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 999
	add_sys_msg_start
;	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 1 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc
