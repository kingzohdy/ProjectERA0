
;���촺�ڻ
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ã��������õĴ���ʱ�ڣ�������Ӧ�����ضȹ��ģ��������ʿ��Ҫ����һ�������ޣ�����������ף����
		
		task_add_option 3020 1 100
		task_add_option 3021 1 101
		task_add_option 3022 1 102
		task_add_option 3023 1 103
		task_add_option 3024 1 104
		task_add_option 3025 1 105
		task_add_option 3026 1 106
		
	
		option 150 ���ܰ���ʲôæ��
		option 151 #û����Ȥ
	
	talk
endproc 

PROC 100
	task_do_option 3020 1
ENDPROC
PROC 101
	task_do_option 3021 1
ENDPROC
PROC 102
	task_do_option 3022 1
ENDPROC
PROC 103
	task_do_option 3023 1
ENDPROC
PROC 104
	task_do_option 3024 1
ENDPROC
PROC 105
	task_do_option 3025 1
ENDPROC
PROC 106
	task_do_option 3026 1
ENDPROC

proc 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ÿ���#c186506 1��27��-2��10��#n�����޶����ӵ�������Ҷ����ǵ�������д����ƻ������ʱ����Ҫ��λ��ʿ�İ�����һ���������ޣ����õ���һ���ף��������Ҹ�������񽫻�õ�����������
	add_talk #b#b1��27��-2��10���ڼ�ɲμ����»�����ң�
	add_talk #b#b#c186506�1�����ÿ�����췢�������޻�ɱ����
	add_talk #b#b�2�����ÿ�����췢������ҹ�ȡ��ƽ�󾮡�ɭ�ֽ�������һ����ɱ������ͨ�Ѷȼ����ϣ�
	add_talk #b#b�3��10:00-20:00����ɱ������ѩԭ������ɳĮ�����ջ�Ұ��Խ�����֡�Խ�������ֵ����ޣ����޻�ÿ���Сʱˢ��һ��
	
		option 1 ����
		option 151 #��֪����
	
	talk

endproc
