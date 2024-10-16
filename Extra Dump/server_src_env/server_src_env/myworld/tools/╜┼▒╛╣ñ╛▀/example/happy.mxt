;---------------------------------------------------------------------
;  ר�����ƣ���QQ�������ҡ�
;  �ű����ƣ����տ���.MXT
;  �ű�˵������Ӧ�����¼������տ��֡�
;  �������ڣ�2003.9.9.
;---------------------------------------------------------------------
;
;��ʼִ�����տ����¼�����Ļ�𽥱��
PROC    0
    CLEAR_TALK 'A'
    CLEAR_ANI    
    FADE_IN 8
ENDPROC

;�ڰ��п�������
PROC    3
    PLAY_VOICE 0 wave\sound1.WAV
ENDPROC

;�ڰ��й�������
PROC    9
    PLAY_VOICE 0 wave\sound2.WAV
ENDPROC  

;��񻮹�
PROC    12
    PLAY_VOICE 0 wave\match.WAV
    ADD_RLEANI 400 300 0 event\matchfire.ran
ENDPROC

;��Ļ�������������������ɶԳ��������վ���ʵ���λ����
PROC    16 
    GIFT_FALL 5 10 600*2
    CHANGE_MAP �����ɶ�.map   
    NPC_POSITION 'V' 371 464
    NPC_POSITION 'I' 374 267 
    NPC_POSITION 200 70 565
    NPC_POSITION 201 710 565
    NPC_POSITION 202 174 427
    NPC_POSITION 203 606 427
    NPC_POSITION 204 119 494
    NPC_POSITION 205 661 494 
    NPC_POSITION 206 204 348
    NPC_POSITION 207 558 348
    NPC_POSITION 208 255 291
    NPC_POSITION 209 525 291
    NPC_POSITION 210 374 267
    FADE_OUT 32    
ENDPROC

;�ڱ�ʩ����ͷ�����ֶ���
PROC    18
    PLAY_VOICE 0 wave\festivity.WAV
    ADD_LOOPMAGIC 'V' 8 magic\����.gif
ENDPROC

;����ǰ�����տ��ֶ���
PROC    23
    STOP_MUSIC
    PLAY_MUSIC 0 wave\open_one.wav
    CLEAR_TALK 'A'
    COUNT_PLAYERS 8
    IF 8 = 1
        CLEAR_MAGIC 'V'
        CLEAR_TALK 'V'
        ADD_TALK 'V' �Ǻǣ�ף�Լ����տ��֣�
        ADD_LOOPMAGIC 'V' 3 magic\����.gif
    ELSEIF 8 = 2
        CLEAR_MAGIC 'I'
        CLEAR_TALK 'I'
        ADD_TALK 'I' ף$TO$���տ��֣�
        ADD_LOOPMAGIC 'I' 3 magic\����.gif
    ELSEIF 8 < 5
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        ADD_LOOPMAGIC 200 3 magic\����.gif
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_LOOPMAGIC 201 3 magic\����.gif
        CLEAR_TALK 202
        ADD_LOOPMAGIC 202 3 magic\����.gif
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 3 magic\����.gif
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 3 magic\����.gif
    ELSEIF 8 <= 8
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 ף$TO$���տ��֣�
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_TALK 202 ף$TO$���տ��֣�
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 3 magic\����.gif
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 3 magic\����.gif
        CLEAR_TALK 205
        ADD_LOOPMAGIC 205 3 magic\����.gif
        CLEAR_TALK 206
        ADD_LOOPMAGIC 206 3 magic\����.gif
        CLEAR_TALK 207
        ADD_LOOPMAGIC 207 3 magic\����.gif
    
   ELSE
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 ף$TO$���տ��֣�
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_TALK 202 ף$TO$���տ��֣�
        
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 3 magic\����.gif
        
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 3 magic\����.gif
        
        CLEAR_TALK 205
        ADD_LOOPMAGIC 205 3 magic\����.gif
        
        CLEAR_TALK 206
        ADD_LOOPMAGIC 206 3 magic\����.gif
        
        CLEAR_TALK 207
        ADD_LOOPMAGIC 207 3 magic\����.gif
    ENDIF
ENDPROC
;
;
PROC    25
    PLAY_VOICE 0 wave\applause.wav
ENDPROC
;
;����Ʈ��
PROC    34
    ADD_LOOPMAGIC 'A' 1 magic\����.gif
    CLEAR_MAGIC 'V'
ENDPROC

;���һ�����
PROC    80
    CLEAR_MAGIC 'A'
    COUNT_PLAYERS 8
    IF 8 = 1
        ADD_TALK 'V' ף�Լ����տ��֣�
        ADD_LOOPMAGIC 'V' 3 magic\ơ��.gif
        PLAY_VOICE 0 wave\applause.wav
    ELSEIF 8 = 2
        CLEAR_TALK 'I'
        ADD_TALK 'I' ף$TO$���տ��֣�
        ADD_LOOPMAGIC 'I' 3 magic\ơ��.gif
    ELSEIF 8 < 5
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        ADD_LOOPMAGIC 200 3 magic\ơ��.gif
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_LOOPMAGIC 201 3 magic\ơ��.gif
        CLEAR_TALK 202
        ADD_LOOPMAGIC 202 3 magic\ơ��.gif
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 4 magic\����.gif
        PLAY_VOICE 0 wave\applause.wav
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 4 magic\����.gif
    ELSEIF 8 <= 8
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 ף$TO$���տ��֣�
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_TALK 202 ף$TO$���տ��֣�
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 3 magic\ơ��.gif
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 3 magic\ơ��.gif
        CLEAR_TALK 205
        ADD_LOOPMAGIC 205 3 magic\ơ��.gif
        CLEAR_TALK 206
        ADD_LOOPMAGIC 206 4 magic\����.gif
        PLAY_VOICE 0 wave\applause.wav
        CLEAR_TALK 207
        ADD_LOOPMAGIC 207 4 magic\����.gif
   ELSE
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 ף$TO$���տ��֣�
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_TALK 202 ף$TO$���տ��֣�
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 3 magic\ơ��.gif
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 3 magic\ơ��.gif
        CLEAR_TALK 205
        ADD_LOOPMAGIC 205 3 magic\ơ��.gif
        CLEAR_TALK 206
        ADD_LOOPMAGIC 206 4 magic\����.gif
        PLAY_VOICE 0 wave\applause.wav
        CLEAR_TALK 207
        ADD_LOOPMAGIC 207 4 magic\����.gif
    ENDIF
ENDPROC


;���˿�����
PROC    88
    COUNT_PLAYERS 8
    IF 8 = 1
        CLEAR_TALK 'V'
        ADD_TALK 'V' ��~~~�øж���       �Լ����Լ�������PARTY��
        ADD_LOOPMAGIC 'V' 2 magic\����.gif
    ELSE
        CLEAR_TALK 'V'
        ADD_TALK 'V' ��~~~�øж���       лл�μ��ҵ�����PARTY��
        ADD_LOOPMAGIC 'V' 2 magic\����.gif
    ENDIF
ENDPROC
;

PROC    96
    COUNT_PLAYERS 8
    IF 8 = 1
        CLEAR_TALK 'V'
        ADD_TALK 'V' �ţ������Լ�Ϊ�Լ�׼�����������
    ELSE
        CLEAR_TALK 'V'
        ADD_TALK 'V' ���ҿ���Ϊ��׼�����������
    ENDIF
ENDPROC
;
;��������
PROC    99  
    CLEAR_MAGIC 'A'
    ADD_LOOPMAGIC 'I' 3 magic\��β��.gif
    COUNT_PLAYERS 8
    IF 8 = 1
        CLEAR_MAGIC 'V'
        ADD_LOOPMAGIC 'V' 1 magic\����.gif
    ELSEIF 8 = 2
        ADD_TALK 'I'  ף$TO$���տ��֣�
        ADD_LOOPMAGIC 'I' 1 magic\����.gif
        ADD_TALK 'I' ���տ��֣�
    ELSEIF 8 < 5
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200  ף$TO$���տ��֣�
        ADD_LOOPMAGIC 200 1 magic\����.gif
        ADD_TALK 200 ���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_LOOPMAGIC 201 1 magic\����.gif
        ADD_TALK 201 ���տ��֣�
        CLEAR_TALK 202
        CLEAR_MAGIC 202
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        CLEAR_TALK 204
        CLEAR_MAGIC 204
    ELSEIF 8 <= 8
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 ף$TO$���տ��֣�
        
        CLEAR_MAGIC 202
        ADD_LOOPMAGIC 202 1 magic\����.gif
        ADD_TALK 201 ���տ��֣�
        
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 1 magic\����.gif
        ADD_TALK 203 ���տ��֣�
        
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        
        CLEAR_TALK 207
        CLEAR_MAGIC 207
    
   ELSE
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 ף$TO$���տ��֣�
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_TALK 202 ף$TO$���տ��֣�
        
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 1 magic\����.gif
        ADD_TALK 203 ���տ��֣�
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 1 magic\����.gif
        ADD_TALK 204 ���տ��֣�
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        
        CLEAR_TALK 207
        CLEAR_MAGIC 207
    ENDIF
ENDPROC
;
;
PROC    100
    CLEAR_TALK 'V'
ENDPROC    
;
;
PROC    101
    COUNT_PLAYERS 8
    IF 8 < 5
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_LOOPMAGIC 202 1 magic\����.gif
        CLEAR_MAGIC 203
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 1 magic\����.gif
    
    ELSEIF 8 <= 8
       
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 1 magic\����.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 1 magic\����.gif
    
   ELSE
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 1 magic\����.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 1 magic\����.gif
    ENDIF
ENDPROC

PROC    118
    CLEAR_TALK 'V'
    ADD_TALK 'V' ��Ư��������
ENDPROC
;
PROC    128
    COUNT_PLAYERS 8
    IF 8 = 1
        CLEAR_TALK 'V'
        ADD_TALK 'V' ������������ֵ�һ��~~~��~~~~�Լ����Լ����տ��ָ�ɣ�
    ELSE
        CLEAR_TALK 'V'
        ADD_TALK 'V' �Һܸж�,������������ֵ�һ��~~~~~~~лл��ҵ�ף��������һ���������տ��ָ裡
    ENDIF
ENDPROC

PROC    132
    ADD_LOOPMAGIC 'V' 666 magic\�ܳ�.gif
ENDPROC

PROC    137
    STOP_VOICE
    CLEAR_TALK 'V'
ENDPROC
;

PROC    138
    ;STOP_MUSIC
    PLAY_MUSIC 0 wave\birthday.wav
    ADD_LOOPMAGIC 'A' 666 magic\����ҡҡ.gif
    CLEAR_MAGIC 'V'
    ADD_LOOPMAGIC 'V' 666 magic\�ܳ�.gif
ENDPROC
;
;
PROC    140
    NPC_POSITION 'V' 363 472
ENDPROC
;
PROC    146
    NPC_POSITION 'V' 355 480
ENDPROC
;
PROC    152
    NPC_POSITION 'V' 363 488
ENDPROC

PROC    158
    NPC_POSITION 'V' 371 496
ENDPROC

PROC    164
    NPC_POSITION 'V' 363 504
ENDPROC

PROC    170
    NPC_POSITION 'V' 355 512
ENDPROC


PROC    176
    NPC_POSITION 'V' 363 520
ENDPROC

PROC    182
    NPC_POSITION 'V' 371 528
ENDPROC

PROC    188
    NPC_POSITION 'V' 363 520
ENDPROC

PROC    194
    NPC_POSITION 'V' 355 512
ENDPROC

PROC    200
    NPC_POSITION 'V' 363 504
ENDPROC

PROC    206
    NPC_POSITION 'V' 371 496
ENDPROC

PROC    212
    NPC_POSITION 'V' 363 488
ENDPROC

PROC    218
    NPC_POSITION 'V' 355 480
ENDPROC

PROC    224
    NPC_POSITION 'V' 363 472
ENDPROC


PROC    230
    NPC_POSITION 'V' 371 464
ENDPROC

PROC    236
    NPC_POSITION 'V' 363 472
ENDPROC

PROC    242
    NPC_POSITION 'V' 371 464
ENDPROC

PROC    233
    ADD_GIFANI 360 150 30 event\misc\happy.gif
    ADD_RLEANI 420 200 40 event\misc\������.ran
    ADD_RLEANI 510 210 60 event\misc\������.ran
    ADD_RLEANI 320 180 60 event\misc\������.ran
ENDPROC

PROC    243
    STOP_VOICE
    CLEAR_TALK 'V'
    ADD_TALK 'V' ����,��Ҫ��ʼ��Ը��.
ENDPROC

PROC    246
    CLEAR_ANI    
    FADE_IN 4
ENDPROC


PROC    248
    CLEAR_TALK 'V'
ENDPROC

PROC    251
    ;ADD_GIFANI 380 300 50 event\misc\pray.gif
    ADD_GIFANI 380 300 5 event\misc\pray.gif
ENDPROC
;
PROC    252
    CLEAR_MAGIC 'A'  
ENDPROC
;
;
PROC    253
    CLEAR_MAGIC 'V'
    ADD_GIFANI 300 200 1 event\misc\����.gif
ENDPROC


PROC    260   
    FADE_OUT 4
    CLEAR_TALK 'V'
ENDPROC

PROC    265
    PLAY_MUSIC 0 wave\open_one.wav
ENDPROC

PROC    269
    PLAY_VOICE 0 wave\yell.WAV
ENDPROC

PROC    273
    CLEAR_TALK 'V'
    ADD_TALK 'V' ��ʼ�Ե�������
    
    COUNT_PLAYERS 8
    IF 8 = 1
    ELSEIF 8 = 2
    ELSEIF 8 < 5
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 ף$TO$���տ��֣�
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_LOOPMAGIC 202 3 magic\��.gif
        CLEAR_MAGIC 203
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 3 magic\��.gif
    
    ELSEIF 8 <= 8
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 ף$TO$���տ��֣�
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_LOOPMAGIC 202 3 magic\��.gif
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 3 magic\��.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 3 magic\��.gif
    
   ELSE
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 ף$TO$���տ��֣�
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_LOOPMAGIC 202 3 magic\��.gif
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 3 magic\��.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 3 magic\��.gif
    ENDIF
ENDPROC

PROC    278
    PLAY_VOICE 0 wave\fun.WAV
ENDPROC

PROC    279
    CLEAR_MAGIC 'A'
    ADD_MAGIC 'V' magic\��Ե���.gif 
ENDPROC

PROC    282
    PLAY_VOICE 0 wave\cake.wav
ENDPROC

;���һ��Ц
PROC    285
    PLAY_VOICE 0 wave\laugh.WAV
    ADD_LOOPMAGIC 'V' 2 magic\����.gif
ENDPROC

PROC    287
    CLEAR_TALK 'V'
    ADD_TALK 'V' ~~���ر������~~~~~~
    ADD_LOOPMAGIC 'V' 5 magic\��ͨ.gif
ENDPROC

;�ֵ���
PROC    304
    CLEAR_TALK 'V'
    CLEAR_MAGIC 'V'
    COUNT_PLAYERS 8
    IF 8 = 1
    ELSEIF 8 = 2
    ELSEIF 8 < 5
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        ADD_LOOPMAGIC 200 5 magic\��ͨ.gif
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_LOOPMAGIC 201 5 magic\��ͨ.gif
        CLEAR_TALK 202
        ADD_LOOPMAGIC 202 5 magic\��ͨ.gif
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 5 magic\����.gif
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 5 magic\��ͨ.gif
    
    ELSEIF 8 <= 8
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 ף$TO$���տ��֣�
        
        CLEAR_TALK 202
        CLEAR_MAGIC 202
        ADD_LOOPMAGIC 202 5 magic\��ͨ.gif
        
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 5 magic\��ͨ.gif
        
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\��ͨ.gif
        
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 5 magic\����.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 5 magic\��ͨ.gif
        
        CLEAR_TALK 207
        CLEAR_MAGIC 207
        ADD_LOOPMAGIC 207 5 magic\��ͨ.gif
        
    ELSE
        ADD_TALK 205 �Ե�������
        ADD_TALK 206 �Ե�������
        ADD_TALK 207 �Ե�������
        
        CLEAR_TALK 200
        CLEAR_MAGIC 200
        ADD_LOOPMAGIC 200 5 magic\��ͨ.gif
        CLEAR_TALK 201
        CLEAR_MAGIC 201
        ADD_TALK 201 �Ե�������
        CLEAR_TALK 202
        CLEAR_MAGIC 202
        ADD_LOOPMAGIC 202 5 magic\��ͨ.gif
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_TALK 203 �Ե�������
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\����.gif
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_TALK 205 �Ե�������
    ENDIF
    

ENDPROC

;���һ��Ц
PROC    307
    COUNT_PLAYERS 8
    IF 8 = 1
        CLEAR_TALK 'V'
        ADD_TALK 'V' �Լ���ʼ�����յ��⡫ 
    ELSE
        CLEAR_TALK 'V'
        ADD_TALK 'V' ��!���һ���������յ��⡫ 
    ENDIF
  
    COUNT_PLAYERS 8
    IF 8 < 5
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        ADD_LOOPMAGIC 200 5 magic\Ц.gif
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_LOOPMAGIC 201 5 magic\Ц.gif
        CLEAR_TALK 202
        CLEAR_MAGIC 202
        ADD_LOOPMAGIC 202 5 magic\Ц.gif
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 5 magic\Ц.gif
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\Ц.gif
    
    ELSEIF 8 <= 8
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 ף$TO$���տ��֣�
        
        CLEAR_TALK 202
        CLEAR_MAGIC 202
        ADD_LOOPMAGIC 202 5 magic\Ц.gif
        
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 5 magic\Ц.gif
        
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\����.gif
        
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 5 magic\Ц.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 5 magic\Ц.gif
        
        CLEAR_TALK 207
        CLEAR_MAGIC 207
        ADD_LOOPMAGIC 207 5 magic\Ц.gif
    
   ELSE
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 ף$TO$���տ��֣�
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_TALK 202 ף$TO$���տ��֣�
        
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 5 magic\Ц.gif
        
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\����.gif
        
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 5 magic\Ц.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 5 magic\Ц.gif
        
        CLEAR_TALK 207
        CLEAR_MAGIC 207
        ADD_LOOPMAGIC 207 5 magic\Ц.gif
    ENDIF
ENDPROC
;
PROC    316
    CLEAR_MAGIC 'A'
ENDPROC
;
PROC    317
    COUNT_PLAYERS 8
    IF 8 = 1
        CLEAR_TALK 'V'
        ADD_TALK 'V' �Լ��ٺ�һ���ɣ�
    ELSE
        CLEAR_TALK 'A'
        ADD_TALK 'V' ������~�ٴ�������
    ENDIF
ENDPROC

PROC    320
    COUNT_PLAYERS 8
    IF 8 < 3
        CLEAR_MAGIC 'A'
        ADD_LOOPMAGIC 'A' 5 magic\��β��.gif
    ELSEIF 8 < 5
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        ADD_LOOPMAGIC 200 5 magic\��β��.gif
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_LOOPMAGIC 201 5 magic\��β��.gif
        CLEAR_TALK 202
        CLEAR_MAGIC 202
        ADD_LOOPMAGIC 202 5 magic\��β��.gif
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 5 magic\�վ�.gif
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\�վ�.gif
    
    ELSEIF 8 <= 8
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 ף$TO$���տ��֣�
        
        CLEAR_TALK 202
        CLEAR_MAGIC 202
        ADD_LOOPMAGIC 202 5 magic\��β��.gif
        
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 5 magic\��β��.gif
        
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\��β��.gif
        
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 5 magic\��β��.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 5 magic\�վ�.gif
        
        CLEAR_TALK 207
        CLEAR_MAGIC 207
        ADD_LOOPMAGIC 207 5 magic\��β��.gif
    
   ELSE
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 ף$TO$���տ��֣�
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 ף$TO$���տ��֣�
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_TALK 202 ף$TO$���տ��֣�
        
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 5 magic\��β��.gif
        
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\��β��.gif
        
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 5 magic\��β��.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 5 magic\�վ�.gif
        
        CLEAR_TALK 207
        CLEAR_MAGIC 207
        ADD_LOOPMAGIC 207 5 magic\��β��.gif
    ENDIF
ENDPROC

PROC    325
    CLEAR_TALK 'A'
    CLEAR_MAGIC 'A'
    COUNT_PLAYERS 8
    IF 8 = 1
        ADD_TALK 'V' ���Լ�������Ƭ����ס������ֵ����ӣ�
    ELSE
        ADD_TALK 'V' ����һ������Ӱ����ס������ֵ����ӣ�
    ENDIF
ENDPROC

;��ҵ�����λ�ã�׼����Ӱ����
PROC    332
    CLEAR_TALK 'A'
    CLEAR_MAGIC 'A'
    NPC_POSITION 'V' 394 400
    NPC_POSITION 'I' 210 400
    NPC_POSITION 200 295 400
    NPC_POSITION 201 343 400
    NPC_POSITION 202 245 400
    NPC_POSITION 203 439 400
    NPC_POSITION 204 489 400
    NPC_POSITION 205 542 400
    NPC_POSITION 206 259 357
    NPC_POSITION 207 306 357
    NPC_POSITION 208 353 357
    NPC_POSITION 209 403 357
    NPC_POSITION 210 455 357
    NPC_POSITION 211 503 357
    NPC_POSITION 212 258 313
    NPC_POSITION 213 310 313
    NPC_POSITION 214 354 313
    NPC_POSITION 215 400 313
    NPC_POSITION 216 445 313
    NPC_POSITION 217 486 313
ENDPROC
;
;��ʼ��321
PROC    334
    CLEAR_MAGIC 'A'
    PLAY_VOICE 0 wave\di.wav
    ADD_LOOPMAGIC 'V' 6 magic\����3.gif
ENDPROC

PROC    339
    PLAY_VOICE 0 wave\di.wav
    ADD_LOOPMAGIC 'V' 6 magic\����2.gif
ENDPROC

PROC    344
    PLAY_VOICE 0 wave\di.wav
    ADD_LOOPMAGIC 'V' 6 magic\����1.gif
ENDPROC

;�����
PROC    348
    CLEAR_ANI
    ADD_RLEANI 400 300 1111 event\snap\birthrim.ran
    SHOW_TITLEMENU 0
ENDPROC

;����
PROC    349
    PLAY_VOICE 0 wave\photo.WAV
    SNAPSHOT 76 24 590 452
    ;SNAPSHOT 0 0 800 600
    SHOW_TITLEMENU 1
ENDPROC
;
PROC    350
    PLAY_VOICE 0 wave\festivity.WAV
ENDPROC

;����
PROC    358
    CLEAR_ANI
    CLEAR_MAGIC 'A'
    CLEAR_TALK 'A'
    MERGE_OUT 4
    RESTORE_MAP
    STOP_VOICE
    STOP_MUSIC
    BREAK_SCRIPT
ENDPROC
;
PROC    999
ENDPROC
;
