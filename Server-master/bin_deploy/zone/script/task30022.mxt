;��ţԤ��,Ԥ����ţ����
;�����¼�
proc 1 
;	NEW_CONVOY_MON ::cow:: 20600 31800 200 0 0 0 1800 30022 1 2
	var 2 0
	SET_TASK_VAR 30022 0 32 @2 1
endproc
;�����¼�
proc 5
	COMMIT_TASK 30022 1
	TASK_STAT 30022 1 11
	;����δ���,����
	if 11 != 2
		return
	endif
	;жĥɱţ
	GET_TASK_VAR 30022 0 32 1 10
	if 10 != 0
		DEL_ID_MON @10 1
	endif	
endproc
