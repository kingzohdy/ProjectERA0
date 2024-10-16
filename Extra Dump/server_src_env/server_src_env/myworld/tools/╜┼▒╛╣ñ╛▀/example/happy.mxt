;---------------------------------------------------------------------
;  专案名称：《QQ秀聊天室》
;  脚本名称：生日快乐.MXT
;  脚本说明：对应动感事件“生日快乐”
;  制作日期：2003.9.9.
;---------------------------------------------------------------------
;
;开始执行生日快乐事件，屏幕逐渐变黑
PROC    0
    CLEAR_TALK 'A'
    CLEAR_ANI    
    FADE_IN 8
ENDPROC

;黑暗中开门声音
PROC    3
    PLAY_VOICE 0 wave\sound1.WAV
ENDPROC

;黑暗中关门声音
PROC    9
    PLAY_VOICE 0 wave\sound2.WAV
ENDPROC  

;火柴划过
PROC    12
    PLAY_VOICE 0 wave\match.WAV
    ADD_RLEANI 400 300 0 event\matchfire.ran
ENDPROC

;屏幕亮了起来，出现生日派对场景，大家站在适当的位置上
PROC    16 
    GIFT_FALL 5 10 600*2
    CHANGE_MAP 生日派对.map   
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

;在被施法者头顶出现动画
PROC    18
    PLAY_VOICE 0 wave\festivity.WAV
    ADD_LOOPMAGIC 'V' 8 magic\荒唐.gif
ENDPROC

;调出前景生日快乐动画
PROC    23
    STOP_MUSIC
    PLAY_MUSIC 0 wave\open_one.wav
    CLEAR_TALK 'A'
    COUNT_PLAYERS 8
    IF 8 = 1
        CLEAR_MAGIC 'V'
        CLEAR_TALK 'V'
        ADD_TALK 'V' 呵呵，祝自己生日快乐！
        ADD_LOOPMAGIC 'V' 3 magic\鼓掌.gif
    ELSEIF 8 = 2
        CLEAR_MAGIC 'I'
        CLEAR_TALK 'I'
        ADD_TALK 'I' 祝$TO$生日快乐！
        ADD_LOOPMAGIC 'I' 3 magic\鼓掌.gif
    ELSEIF 8 < 5
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        ADD_LOOPMAGIC 200 3 magic\鼓掌.gif
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_LOOPMAGIC 201 3 magic\鼓掌.gif
        CLEAR_TALK 202
        ADD_LOOPMAGIC 202 3 magic\鼓掌.gif
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 3 magic\鼓掌.gif
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 3 magic\鼓掌.gif
    ELSEIF 8 <= 8
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 祝$TO$生日快乐！
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_TALK 202 祝$TO$生日快乐！
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 3 magic\鼓掌.gif
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 3 magic\鼓掌.gif
        CLEAR_TALK 205
        ADD_LOOPMAGIC 205 3 magic\鼓掌.gif
        CLEAR_TALK 206
        ADD_LOOPMAGIC 206 3 magic\鼓掌.gif
        CLEAR_TALK 207
        ADD_LOOPMAGIC 207 3 magic\鼓掌.gif
    
   ELSE
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 祝$TO$生日快乐！
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_TALK 202 祝$TO$生日快乐！
        
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 3 magic\鼓掌.gif
        
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 3 magic\鼓掌.gif
        
        CLEAR_TALK 205
        ADD_LOOPMAGIC 205 3 magic\鼓掌.gif
        
        CLEAR_TALK 206
        ADD_LOOPMAGIC 206 3 magic\鼓掌.gif
        
        CLEAR_TALK 207
        ADD_LOOPMAGIC 207 3 magic\鼓掌.gif
    ENDIF
ENDPROC
;
;
PROC    25
    PLAY_VOICE 0 wave\applause.wav
ENDPROC
;
;气球飘起
PROC    34
    ADD_LOOPMAGIC 'A' 1 magic\礼物.gif
    CLEAR_MAGIC 'V'
ENDPROC

;大家一起鼓掌
PROC    80
    CLEAR_MAGIC 'A'
    COUNT_PLAYERS 8
    IF 8 = 1
        ADD_TALK 'V' 祝自己生日快乐！
        ADD_LOOPMAGIC 'V' 3 magic\啤酒.gif
        PLAY_VOICE 0 wave\applause.wav
    ELSEIF 8 = 2
        CLEAR_TALK 'I'
        ADD_TALK 'I' 祝$TO$生日快乐！
        ADD_LOOPMAGIC 'I' 3 magic\啤酒.gif
    ELSEIF 8 < 5
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        ADD_LOOPMAGIC 200 3 magic\啤酒.gif
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_LOOPMAGIC 201 3 magic\啤酒.gif
        CLEAR_TALK 202
        ADD_LOOPMAGIC 202 3 magic\啤酒.gif
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 4 magic\鼓掌.gif
        PLAY_VOICE 0 wave\applause.wav
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 4 magic\鼓掌.gif
    ELSEIF 8 <= 8
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 祝$TO$生日快乐！
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_TALK 202 祝$TO$生日快乐！
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 3 magic\啤酒.gif
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 3 magic\啤酒.gif
        CLEAR_TALK 205
        ADD_LOOPMAGIC 205 3 magic\啤酒.gif
        CLEAR_TALK 206
        ADD_LOOPMAGIC 206 4 magic\鼓掌.gif
        PLAY_VOICE 0 wave\applause.wav
        CLEAR_TALK 207
        ADD_LOOPMAGIC 207 4 magic\鼓掌.gif
   ELSE
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 祝$TO$生日快乐！
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_TALK 202 祝$TO$生日快乐！
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 3 magic\啤酒.gif
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 3 magic\啤酒.gif
        CLEAR_TALK 205
        ADD_LOOPMAGIC 205 3 magic\啤酒.gif
        CLEAR_TALK 206
        ADD_LOOPMAGIC 206 4 magic\鼓掌.gif
        PLAY_VOICE 0 wave\applause.wav
        CLEAR_TALK 207
        ADD_LOOPMAGIC 207 4 magic\鼓掌.gif
    ENDIF
ENDPROC


;主人开场白
PROC    88
    COUNT_PLAYERS 8
    IF 8 = 1
        CLEAR_TALK 'V'
        ADD_TALK 'V' 呜~~~好感动，       自己给自己开生日PARTY！
        ADD_LOOPMAGIC 'V' 2 magic\呜呜.gif
    ELSE
        CLEAR_TALK 'V'
        ADD_TALK 'V' 呜~~~好感动，       谢谢参加我的生日PARTY！
        ADD_LOOPMAGIC 'V' 2 magic\呜呜.gif
    ENDIF
ENDPROC
;

PROC    96
    COUNT_PLAYERS 8
    IF 8 = 1
        CLEAR_TALK 'V'
        ADD_TALK 'V' 嗯，看看自己为自己准备的生日礼物！
    ELSE
        CLEAR_TALK 'V'
        ADD_TALK 'V' 让我看看为我准备的生日礼物！
    ENDIF
ENDPROC
;
;来宾讲话
PROC    99  
    CLEAR_MAGIC 'A'
    ADD_LOOPMAGIC 'I' 3 magic\鸡尾酒.gif
    COUNT_PLAYERS 8
    IF 8 = 1
        CLEAR_MAGIC 'V'
        ADD_LOOPMAGIC 'V' 1 magic\礼物.gif
    ELSEIF 8 = 2
        ADD_TALK 'I'  祝$TO$生日快乐！
        ADD_LOOPMAGIC 'I' 1 magic\礼物.gif
        ADD_TALK 'I' 生日快乐！
    ELSEIF 8 < 5
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200  祝$TO$生日快乐！
        ADD_LOOPMAGIC 200 1 magic\礼物.gif
        ADD_TALK 200 生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_LOOPMAGIC 201 1 magic\礼物.gif
        ADD_TALK 201 生日快乐！
        CLEAR_TALK 202
        CLEAR_MAGIC 202
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        CLEAR_TALK 204
        CLEAR_MAGIC 204
    ELSEIF 8 <= 8
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 祝$TO$生日快乐！
        
        CLEAR_MAGIC 202
        ADD_LOOPMAGIC 202 1 magic\礼物.gif
        ADD_TALK 201 生日快乐！
        
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 1 magic\礼物.gif
        ADD_TALK 203 生日快乐！
        
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
        ADD_TALK 200 祝$TO$生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 祝$TO$生日快乐！
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_TALK 202 祝$TO$生日快乐！
        
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 1 magic\礼物.gif
        ADD_TALK 203 生日快乐！
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 1 magic\礼物.gif
        ADD_TALK 204 生日快乐！
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
        ADD_LOOPMAGIC 202 1 magic\礼物.gif
        CLEAR_MAGIC 203
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 1 magic\礼物.gif
    
    ELSEIF 8 <= 8
       
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 1 magic\礼物.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 1 magic\礼物.gif
    
   ELSE
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 1 magic\礼物.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 1 magic\礼物.gif
    ENDIF
ENDPROC

PROC    118
    CLEAR_TALK 'V'
    ADD_TALK 'V' 好漂亮的礼物
ENDPROC
;
PROC    128
    COUNT_PLAYERS 8
    IF 8 = 1
        CLEAR_TALK 'V'
        ADD_TALK 'V' 今天是我最快乐的一天~~~嗯~~~~自己给自己生日快乐歌吧！
    ELSE
        CLEAR_TALK 'V'
        ADD_TALK 'V' 我很感动,今天是我最快乐的一天~~~~~~~谢谢大家的祝福，我们一起来唱生日快乐歌！
    ENDIF
ENDPROC

PROC    132
    ADD_LOOPMAGIC 'V' 666 magic\跑车.gif
ENDPROC

PROC    137
    STOP_VOICE
    CLEAR_TALK 'V'
ENDPROC
;

PROC    138
    ;STOP_MUSIC
    PLAY_MUSIC 0 wave\birthday.wav
    ADD_LOOPMAGIC 'A' 666 magic\玉手摇摇.gif
    CLEAR_MAGIC 'V'
    ADD_LOOPMAGIC 'V' 666 magic\跑车.gif
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
    ADD_RLEANI 420 200 40 event\misc\大星星.ran
    ADD_RLEANI 510 210 60 event\misc\大星星.ran
    ADD_RLEANI 320 180 60 event\misc\大星星.ran
ENDPROC

PROC    243
    STOP_VOICE
    CLEAR_TALK 'V'
    ADD_TALK 'V' 好了,我要开始许愿了.
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
    ADD_GIFANI 300 200 1 event\misc\想象.gif
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
    ADD_TALK 'V' 开始吃蛋糕啦！
    
    COUNT_PLAYERS 8
    IF 8 = 1
    ELSEIF 8 = 2
    ELSEIF 8 < 5
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 祝$TO$生日快乐！
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_LOOPMAGIC 202 3 magic\错.gif
        CLEAR_MAGIC 203
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 3 magic\布.gif
    
    ELSEIF 8 <= 8
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 祝$TO$生日快乐！
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_LOOPMAGIC 202 3 magic\错.gif
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 3 magic\布.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 3 magic\错.gif
    
   ELSE
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 祝$TO$生日快乐！
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_LOOPMAGIC 202 3 magic\错.gif
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 3 magic\布.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 3 magic\错.gif
    ENDIF
ENDPROC

PROC    278
    PLAY_VOICE 0 wave\fun.WAV
ENDPROC

PROC    279
    CLEAR_MAGIC 'A'
    ADD_MAGIC 'V' magic\狂吃蛋糕.gif 
ENDPROC

PROC    282
    PLAY_VOICE 0 wave\cake.wav
ENDPROC

;大家一起笑
PROC    285
    PLAY_VOICE 0 wave\laugh.WAV
    ADD_LOOPMAGIC 'V' 2 magic\呜呜.gif
ENDPROC

PROC    287
    CLEAR_TALK 'V'
    ADD_TALK 'V' ~~好特别的礼物~~~~~~
    ADD_LOOPMAGIC 'V' 5 magic\普通.gif
ENDPROC

;分蛋糕
PROC    304
    CLEAR_TALK 'V'
    CLEAR_MAGIC 'V'
    COUNT_PLAYERS 8
    IF 8 = 1
    ELSEIF 8 = 2
    ELSEIF 8 < 5
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        ADD_LOOPMAGIC 200 5 magic\普通.gif
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_LOOPMAGIC 201 5 magic\普通.gif
        CLEAR_TALK 202
        ADD_LOOPMAGIC 202 5 magic\普通.gif
        CLEAR_TALK 203
        ADD_LOOPMAGIC 203 5 magic\剪刀.gif
        CLEAR_TALK 204
        ADD_LOOPMAGIC 204 5 magic\普通.gif
    
    ELSEIF 8 <= 8
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 祝$TO$生日快乐！
        
        CLEAR_TALK 202
        CLEAR_MAGIC 202
        ADD_LOOPMAGIC 202 5 magic\普通.gif
        
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 5 magic\普通.gif
        
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\普通.gif
        
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 5 magic\剪刀.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 5 magic\普通.gif
        
        CLEAR_TALK 207
        CLEAR_MAGIC 207
        ADD_LOOPMAGIC 207 5 magic\普通.gif
        
    ELSE
        ADD_TALK 205 吃蛋糕啦～
        ADD_TALK 206 吃蛋糕啦～
        ADD_TALK 207 吃蛋糕啦～
        
        CLEAR_TALK 200
        CLEAR_MAGIC 200
        ADD_LOOPMAGIC 200 5 magic\普通.gif
        CLEAR_TALK 201
        CLEAR_MAGIC 201
        ADD_TALK 201 吃蛋糕啦～
        CLEAR_TALK 202
        CLEAR_MAGIC 202
        ADD_LOOPMAGIC 202 5 magic\普通.gif
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_TALK 203 吃蛋糕啦～
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\剪刀.gif
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_TALK 205 吃蛋糕啦～
    ENDIF
    

ENDPROC

;大家一起笑
PROC    307
    COUNT_PLAYERS 8
    IF 8 = 1
        CLEAR_TALK 'V'
        ADD_TALK 'V' 自己开始吃生日蛋糕～ 
    ELSE
        CLEAR_TALK 'V'
        ADD_TALK 'V' 来!大家一起来吃生日蛋糕～ 
    ENDIF
  
    COUNT_PLAYERS 8
    IF 8 < 5
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        ADD_LOOPMAGIC 200 5 magic\笑.gif
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_LOOPMAGIC 201 5 magic\笑.gif
        CLEAR_TALK 202
        CLEAR_MAGIC 202
        ADD_LOOPMAGIC 202 5 magic\笑.gif
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 5 magic\笑.gif
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\笑.gif
    
    ELSEIF 8 <= 8
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 祝$TO$生日快乐！
        
        CLEAR_TALK 202
        CLEAR_MAGIC 202
        ADD_LOOPMAGIC 202 5 magic\笑.gif
        
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 5 magic\笑.gif
        
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\剪刀.gif
        
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 5 magic\笑.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 5 magic\笑.gif
        
        CLEAR_TALK 207
        CLEAR_MAGIC 207
        ADD_LOOPMAGIC 207 5 magic\笑.gif
    
   ELSE
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 祝$TO$生日快乐！
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_TALK 202 祝$TO$生日快乐！
        
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 5 magic\笑.gif
        
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\剪刀.gif
        
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 5 magic\笑.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 5 magic\笑.gif
        
        CLEAR_TALK 207
        CLEAR_MAGIC 207
        ADD_LOOPMAGIC 207 5 magic\笑.gif
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
        ADD_TALK 'V' 自己再喝一杯吧！
    ELSE
        CLEAR_TALK 'A'
        ADD_TALK 'V' 朋友们~再次碰杯！
    ENDIF
ENDPROC

PROC    320
    COUNT_PLAYERS 8
    IF 8 < 3
        CLEAR_MAGIC 'A'
        ADD_LOOPMAGIC 'A' 5 magic\鸡尾酒.gif
    ELSEIF 8 < 5
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        ADD_LOOPMAGIC 200 5 magic\鸡尾酒.gif
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_LOOPMAGIC 201 5 magic\鸡尾酒.gif
        CLEAR_TALK 202
        CLEAR_MAGIC 202
        ADD_LOOPMAGIC 202 5 magic\鸡尾酒.gif
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 5 magic\烧酒.gif
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\烧酒.gif
    
    ELSEIF 8 <= 8
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 祝$TO$生日快乐！
        
        CLEAR_TALK 202
        CLEAR_MAGIC 202
        ADD_LOOPMAGIC 202 5 magic\鸡尾酒.gif
        
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 5 magic\鸡尾酒.gif
        
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\鸡尾酒.gif
        
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 5 magic\鸡尾酒.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 5 magic\烧酒.gif
        
        CLEAR_TALK 207
        CLEAR_MAGIC 207
        ADD_LOOPMAGIC 207 5 magic\鸡尾酒.gif
    
   ELSE
        CLEAR_MAGIC 200
        CLEAR_TALK 200
        ADD_TALK 200 祝$TO$生日快乐！
        CLEAR_MAGIC 201
        CLEAR_TALK 201
        ADD_TALK 201 祝$TO$生日快乐！
        CLEAR_MAGIC 202
        CLEAR_TALK 202
        ADD_TALK 202 祝$TO$生日快乐！
        
        CLEAR_TALK 203
        CLEAR_MAGIC 203
        ADD_LOOPMAGIC 203 5 magic\鸡尾酒.gif
        
        CLEAR_TALK 204
        CLEAR_MAGIC 204
        ADD_LOOPMAGIC 204 5 magic\鸡尾酒.gif
        
        CLEAR_TALK 205
        CLEAR_MAGIC 205
        ADD_LOOPMAGIC 205 5 magic\鸡尾酒.gif
        
        CLEAR_TALK 206
        CLEAR_MAGIC 206
        ADD_LOOPMAGIC 206 5 magic\烧酒.gif
        
        CLEAR_TALK 207
        CLEAR_MAGIC 207
        ADD_LOOPMAGIC 207 5 magic\鸡尾酒.gif
    ENDIF
ENDPROC

PROC    325
    CLEAR_TALK 'A'
    CLEAR_MAGIC 'A'
    COUNT_PLAYERS 8
    IF 8 = 1
        ADD_TALK 'V' 给自己照张相片，记住这个快乐的日子！
    ELSE
        ADD_TALK 'V' 我们一起来合影，记住这个快乐的日子！
    ENDIF
ENDPROC

;大家调整好位置，准备合影照相
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
;开始数321
PROC    334
    CLEAR_MAGIC 'A'
    PLAY_VOICE 0 wave\di.wav
    ADD_LOOPMAGIC 'V' 6 magic\数字3.gif
ENDPROC

PROC    339
    PLAY_VOICE 0 wave\di.wav
    ADD_LOOPMAGIC 'V' 6 magic\数字2.gif
ENDPROC

PROC    344
    PLAY_VOICE 0 wave\di.wav
    ADD_LOOPMAGIC 'V' 6 magic\数字1.gif
ENDPROC

;贴相框
PROC    348
    CLEAR_ANI
    ADD_RLEANI 400 300 1111 event\snap\birthrim.ran
    SHOW_TITLEMENU 0
ENDPROC

;照相
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

;结束
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
