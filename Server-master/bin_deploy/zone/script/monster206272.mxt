;70����,boss1�ű�
;70����1��boss,���ܰ��е
proc 1
	strvar memid
	GET_MON_INFO 1 0 memid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @memid 1095 1 25920000 0 
		
endproc
;�����ű�
proc 2
	;ɾ�赲
	delete_mon_by_defid 261039 1
	;���
	delete_mon_by_defid 206264 1
	delete_mon_by_defid 206266 1
	delete_mon_by_defid 206268 1
	;boss6flagspeech39,boss6����˫����ʱ˵��
	var 39 1
	set_pworld_var 116 32 @39 1
endproc
