
;С��
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ͣ����Ǿ���ô��Ϊʲô��ô��ѽ���Ҷ�����������Դ��ˣ���վԶ�㣬���ҳ�����Դ�������ʲô��
	add_talk #b#b����˵�����Ҹߵ�Ҫ�����尢�̣���ͷ�����ǰ��̣���ͷ���������壬�������廹�ǰ����أ��ҿ���������Դ���Ҫ���㱧����ȥ�����ɡ�

		task_add_option 3316 1 100
	
		option 150 ���ǳ���С��ţ�ʲô���ⶼ�ܴ�������
		
		strvar num
		GET_ITEM_NUM_INPAK 5052208 1 num
		task_stat 3316 1 20
		if 20 = 2 and num = 0
		;	option 155 #�Ҳ�С�İ����භ��Ū���ˣ����ٸ���һ����
		endif
			OPTION 110 #���̵�
	talk
endproc
PROC 100
	task_do_option 3316 1
ENDPROC
proc 110

	OPEN_NPC_SHOP 
endproc 

proc 155
	ADD_ITEM_NUM_INPAK 5052208 1 1 0

endproc 


proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ҿɲ���׻ش����⣬��ø�Ǯ������˵�������лƽ��ݣ���ƽ��ݵ�Ȼ��������ҵģ�����������ô���ܸ���Ǯ�أ�
	add_talk #b#b���������⣬�һش��㸶Ǯ�����ܻƽ��ݣ����ţ�����������

;	add_talk #b#b�ҵļ۸�ܹ�����#c1865061000����һ�����⣡#n

		option 151 ��ʲô������أ�
	talk
endproc 
proc 151
	
	dialog 1
	add_talk $_npc$:
;	add_talk #b#b�ҿɲ���׻ش����⣬��ø�Ǯ������˵�������лƽ��ݣ���ƽ��ݵ�Ȼ��������ҵģ�����������ô���ܸ���Ǯ�أ�
;	add_talk #b#b���������⣬�һش��㸶Ǯ�����ܻƽ��ݣ����ţ�����������
;	add_talk #b#b	
	add_talk #b#b�ҵļ۸�ܹ�����#c1865061000����һ�����⣡#n

		option 210 ΪʲôС��������·�أ�
		option 211 �԰���ʱ������Ϊʲô�������أ�
		option 212 ΪʲôҪ�����أ�
		option 213 Ϊʲô������µ��������ϵ��أ�
		option 214 ��Ϊʲô��ͣ���أ�
		option 215 Ϊʲô������أ�
		option 216 ����Ϊʲô��ʱ�֣���ʱ���أ�
		option 217 ��Ϊʲô��ͣ�غ��أ�
		option 218 ʲô�Ǻ�Х�أ�
		option 219 ��ΪʲôҪ���ɻ��أ�
		option 220 Ϊʲô���ϵķɻ������ϵķɻ�С�أ�
		option 221 ����Ϊʲô�ɲ�Զ��
		option 222 Ϊʲô������Ҳ���ʶ�أ�
		option 223 ΪʲôС��������������Ʊ�أ�
		option 224 Ϊʲô���к�ҹ�أ�
		option 225 �ְ�Ϊʲô���������ϵ���������
		option 226 ����èΪʲô��·�������ڵģ�
		option 227 ����¹������ô��ô����
		option 228 ��Ϊʲô����ֻ�����أ�
	
	talk
endproc 

proc 200
	ADD_SYS_MSG_START
	ADD_SYS_MSG Ҫ�ȸ�Ǯ������������Ӵ��С��ɲ��뱻�԰����ͣ�
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO

endproc 
proc 209
endproc 

proc 210
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 160 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 160
	endif
endproc 
proc 211
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 161 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 161
	endif
endproc 
proc 212
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 162 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 162
	endif
endproc 
proc 213
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 163 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 163
	endif
endproc 
proc 214
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 164 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 164
	endif
endproc 

proc 215
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 165 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 165
	endif
endproc 
proc 216
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 166 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 166
	endif
endproc 

proc 217
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 167 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 167
	endif
endproc 
proc 218
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 168 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 168
	endif
endproc 

proc 219
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 169 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 169
	endif
endproc 
proc 220
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 170 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 170
	endif
endproc 

proc 221
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 171 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 171
	endif
endproc 
proc 222
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 172 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 172
	endif
endproc 
proc 223
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 173 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 173
	endif
endproc 
proc 224
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 174 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 174
	endif
endproc 
proc 225
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 175 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 175
	endif
endproc 

proc 226
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 176 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 176
	endif
endproc 

proc 227
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 177 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 177
	endif
endproc 

proc 228
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 178 209 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 178
	endif
endproc 






proc 160
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		
		dialog 1
		add_talk $_npc$:
		add_talk #b#b��Ϊ��ֻ��һ���ȣ�����������
			
			option 151 ����
		talk
	endif
endproc 
proc 161
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b�Բ������Ұ���ҧʹ�ˣ������ˡ�

			option 151 ����
		talk
	endif
endproc 
proc 162
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b��ձ�����Ū���ˣ���ϴһϴ�ˡ�

			option 151 ����
		talk
	endif
endproc 
proc 163
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b��Ϊ���µ��е�������ţ����������ǵ����衣

			option 151 ����
		talk
	endif
endproc 
proc 164
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b׼�������ˡ�

			option 151 ����
		talk
	endif
endproc
proc 165
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b����Ƣ�����������ܡ�

			option 151 ����
		talk
	endif
endproc 
proc 166
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b����ʱ������Ļ�����ʱ���������úóԷ���

			option 151 ����
		talk
	endif
endproc 
proc 167
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b�е����ܵ�̫Զ���󺣽����ǻ�����

			option 151 ����
		talk
	endif
endproc 
proc 168
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#bƽʱ�أ�������ȥ���󺣣���Х�أ����Ǵ��������ǡ�

			option 151 ����
		talk
	endif
endproc 
proc 169
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b��Ϊ��û�г��

			option 151 ����
		talk
	endif
endproc 
proc 170
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b�����ϣ�Ҫ��С��һ���ŷɵÿ졣

			option 151 ����
		talk
	endif
endproc 
proc 171
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b���˳�ס�����ǵ����ӡ�

			option 151 ����
		talk
	endif
endproc 
proc 172
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b����û���������ǵ����֡�

			option 151 ����
		talk
	endif
endproc 
proc 173
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b���ǿ���������������ϡ�

			option 151 ����
		talk
	endif
endproc 
proc 174
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b����̫��Ҫ��Ϣ��

			option 151 ����
		talk
	endif
endproc 
proc 175
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b��Ϊ���Լ������ߡ�

			option 151 ����
		talk
	endif
endproc 
proc 176
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b�����ǵİְ�����ѧ�ġ�

			option 151 ����
		talk
	endif
endproc 
proc 177
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b��Ϊ��������������ϵ�Ҷ�ӡ�

			option 151 ����
		talk
	endif
endproc 

proc 178
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b����һ���������һ������������������װ�����ˡ�

			option 151 ����
		talk
	endif
endproc 

















