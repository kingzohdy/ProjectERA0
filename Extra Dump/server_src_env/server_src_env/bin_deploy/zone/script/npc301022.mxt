proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ѣ��о���ô���������û��˼��ô���ðɣ������ͨ�����뿪�����������Ǵ���������þ��齱���Ļ���Ŷ��
	option 300 #����ʱ���
	talk
endproc

proc 300
	choose 0 301 302 ȷ������ʱ�����?
endproc

proc 301
	transmit 1000 39900 36200 1
endproc

proc 302

endproc