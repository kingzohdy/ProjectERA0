
proc 2

endproc 
proc 12
	strvar hp
	GET_MON_INFO 1 1 10 hp 0 0 20 30
	if hp != 0
		
		effect 0 0 playEffct($@20$,$@30$,"effect\\entity\\204011_18.ent")
		MON_SPEECH @10 1 1 1 ��!��Ȼ����18:00,�ֵܺ��һظ�����,���Ǹ��ҵ���,�´��һ�������ġ���
	endif

endproc 