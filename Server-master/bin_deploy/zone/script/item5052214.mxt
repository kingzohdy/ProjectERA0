;С����

proc 1
	YANG_HAI_BANG_START_CHECK 10
	if @10 = -2
		CHOOSE 1 103 102 ���С�����Ѿ��ں����ˣ����ȷ����������һֻ
	else if @10 = 0
		call 103
	endif
endproc 

proc 102
endproc

proc 103
	YANG_HAI_BANG_START 11 
	if @11 < 0
		return
	endif
		
	DEC_ITEM_NUM_INPAK 5052214 1 14
endproc
