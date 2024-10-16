#ifndef ZONE_ERR_H
#define ZONE_ERR_H

#include "comm/terr.h"
#include "comm/terrimpl.h"

extern const char* err_get(HERR hErr);

#define ZONE_GET_ERR(idx, lang)		err_get(idx)

#define ZONE_MOD_IDX		0	/* @@mod 0 mmog*/

#define ZONEERR_GM_BADPARAM	1
/* @@error 1 ����Ĳ���:%s */

#define ZONEERR_GM_SUCC			2
/* @@error 2 GMָ�� [%s %s]ִ�гɹ� */

#define ZONEERR_EXP_DEC			3
/* @@error 3 �㱻�۳���%d�㾭��ֵ */

#define ZONEERR_EXP_ADD			4
/* @@error 4 ������%d�㾭��ֵ */

#define ZONEERR_ACTION_NOT		5
/* @@error 5 ��ǰ״̬�²��������%s���� */

#define ZONEERR_SCRIPT_DEBUG	6
/* @@error 6 debug script:%s*/

#define ZONEERR_OP_LIMIT	7
/* @@error 7 �������ޣ�%d��ֻ�ܲ���һ�� */

#define ZONEERR_DIRTY 8
/* @@error 8 �Բ������������Ϣ�������޴ʻ�*/

#define ZONEERR_NOSPACE 9
/* @@error 9 �����޷����ɸ�����Ʒ����������*/

#define ZONEERR_ARM_VALIDEND 10
/* @@error 10 ��Ʒ%s������Ч�ڣ��ѱ�����*/

#define ZONEERR_AMM 11
/* @@error 11 ��ҩ�˲�����*/

#define ZONEERR_TEAM_OP 12
/* @@error 12 ��Ӵ�����Ϣ��ʧЧ*/

#define ZONEERR_CHAT_COMM 13
/* @@error 13 ��ķ���Ƶ�ʹ���*/

#define ZONEERR_CHAT_WORLD 14
/* @@error 14 ����Ƶ�ʳ����趨������Ƶ��%d�뷢��һ��*/

#define ZONEERR_CHAT_LEVEL 15
/* @@error 15 ��Ƶ������Ҫ����С�ȼ�Ϊ%d*/

#define ZONEERR_ARM_WASH 16
/* @@error 16 �����е�װ������ϴ��*/

#define ZONEERR_ARM_WASH1 17
/* @@error 17 ���ȷ�����Ҫϴ����װ��*/

#define ZONEERR_ARM_WASH2 18
/* @@error 18 ��Ҫ�츳ֵ�ﵽ150��!*/

#define ZONEERR_ARM_WASH3 19
/* @@error 19 ����ȫ������,�޷�ϴ��!*/

#define ZONEERR_ARM_WASH4 20
/* @@error 20 ��Ҳ���!*/

#define ZONEERR_ARM_STAR 21
/* @@error 21 �����е�װ������ǿ��*/

#define ZONEERR_ARM_STAR1 22
/* @@error 22 ���ȷ�����Ҫǿ����װ��*/

#define ZONEERR_ARM_STAR2 23
/* @@error 23 ʱװ����ǿ��*/

#define ZONEERR_ARM_STAR3 24
/* @@error 24 װ��ǿ���ѵ�����*/

#define ZONEERR_GEM_UP 25
/* @@error 25 �ϳɱ�ʯʧ��!*/

#define ZONEERR_GEM_UP1 26
/* @@error 26 �����ɹ�,����1��%s */

#define ZONEERR_GEM_INLAY 27
/* @@error 27 ������Ƕ*/

#define ZONEERR_GEM_INLAY1 28
/* @@error 28 װ���ȼ�������������Ƕ*/

#define ZONEERR_GEM_INLAY2 29
/* @@error 29 �޴˱�ʯ*/

#define ZONEERR_GEM_INLAY3 30
/* @@error 30 %s ��Ƕ%s �ɹ�*/

#define ZONEERR_GEM_INLAY4 31
/* @@error 31 ��Ƕʧ��*/

#define ZONEERR_GEM_PICK 32
/* @@error 32 ȱ�ٱ�ʯժȡ��*/

#define ZONEERR_ARM_MOVE 33
/* @@error 33 ת��Ʒ�ʵ�װ��ֻ�ܴӱ����л�ȡ*/

#define ZONEERR_ARM_MOVE1 34
/* @@error 34 ���ȷ�����Ҫת��Ʒ�ʵ�װ��*/

#define ZONEERR_ARM_MOVE2 35
/* @@error 35 ���ȷ���ת��Ʒ�ʵ�Ŀ��װ��*/

#define ZONEERR_ARM_MOVE3 36
/* @@error 36 ת��װ��Ʒ��ʧ��*/

#define ZONEERR_ARM_MOVE4 37
/* @@error 37 ʱװ���ܽ���ת�ǲ���*/

#define ZONEERR_ARM_MOVE5 38
/* @@error 38 װ�����Ͳ�����,Ʒ��ת��ʧ��*/

#define ZONEERR_ARM_MOVE6 39
/* @@error 39 װ���Ǽ�����,Ʒ��ת��ʧ��*/

#define ZONEERR_BIND1 40
/* @@error 40 ֻ�ܰ󶨱����е�װ��*/

#define ZONEERR_BIND2 41
/* @@error 41 ���ȷ�����Ҫ�󶨵�װ��*/

#define ZONEERR_BIND3 42
/* @@error 42 ���ȷ�����Ҫ�󶨵���Ʒ*/

#define ZONEERR_BIND4 43
/* @@error 43 ��װ�������ض����䣬���ܰ�*/

#define ZONEERR_BIND5 44
/* @@error 44 ����Ʒ�Ѿ��ǰ�״̬��*/

#define ZONEERR_BIND6 45
/* @@error 45 ����Ʒ�ɹ�*/

#define ZONEERR_UNBIND1 46
/* @@error 46 ֻ�ܽ�󱳰��е�װ��*/

#define ZONEERR_UNBIND2 47
/* @@error 47 ���ȷ�����Ҫ����װ��*/

#define ZONEERR_UNBIND3 48
/* @@error 48 �����߲�����*/

#define ZONEERR_UNBIND4 49
/* @@error 49 ���ʧ��*/

#define ZONEERR_UNBIND5 50
/* @@error 50 �����߲��ܶԸ�װ��ʹ�� */

#define ZONEERR_UNBIND6 51
/* @@error 51 ��󶨳ɹ�! */

#define ZONEERR_FENJIE1 52
/* @@error 52 �����ռ䲻��,��������! */

#define ZONEERR_NOMONEY 53
/* @@error 53 û���㹻������!*/

#define ZONEERR_FENJIE2 54
/* @@error 54 �ֽ�ʧ�ܣ�����ѡ��װ�����ֽܷ� */

#define ZONEERR_SAVE_SUCC 55
/* @@error 55 ����ɹ�! */

#define ZONEERR_SAVE_FAIL 56
/* @@error 56 ����ʧ��! */

#define ZONEERR_GEM_RESET1 57
/* @@error 57 ��ʯ����ʧ��! */

#define ZONEERR_GEM_POWER 58
/* @@error 58 ��ʯ����ʧ��!*/

#define ZONEERR_SPAR_SORB 59
/* @@error 59 ���ճɹ�!*/

#define ZONEERR_SPAR_SORB1 60
/* @@error 60 ����ʧ��!*/

#define ZONEERR_EXP1 61
/* @@error 61 ������ %llu����ֵ(��������,�������50 ��)!*/

#define ZONEERR_EXP2 62
/* @@error 62 ������ %llu����ֵ(%s��������,�������50 ��)!*/

#define ZONEERR_EXP3 63
/* @@error 63 ������ %llu����ֵ(%s%s%s)*/

#define ZONEERR_ATTR1 64
/* @@error 64 ������%d%s*/

#define ZONEERR_ATTR2 65
/* @@error 65 ��������%d%s*/

#define ZONEERR_BATTLE_SVR1 66
/* @@error 66 ���״̬�޷����и������Ͳ���*/

#define ZONEERR_BUSY 67
/* @@error 67 ������æ,���Ե�Ƭ��*/

#define ZONEERR_BATTLE_SVR2 68
/* @@error 68 �����ɹ�!*/

#define ZONEERR_CLAN1 69
/* @@error 69 �㻹û�м���һ������*/

#define ZONEERR_CLAN2 70
/* @@error 70 �����ڵĹ����Ѿ���ɢ��*/

#define ZONEERR_CLAN3 71
/* @@error 71 ���Ѿ��������������*/

#define ZONEERR_CLAN4 72
/* @@error 72 ����������������ڵĹ���,�뾡�촦��*/

#define ZONEERR_CLAN5 73
/* @@error 73 ������������Ĺ������,�뾡�촦��*/

#define ZONEERR_CLAN6 74
/* @@error 74 ������������Ĺ���ж�,�뾡�촦��*/

#define ZONEERR_CLAN7 75
/* @@error 75 �������Լ����˻�ж�*/

#define ZONEERR_CLAN8 76
/* @@error 76 ���Ѿ�������Է�����ͬ��*/

#define ZONEERR_CLAN9 77
/* @@error 77 ���Ѿ���Է��������*/

#define ZONEERR_CLAN10 78
/* @@error 78 ����Է����ᴦ�ڵж�״̬*/

#define ZONEERR_CLAN11 79
/* @@error 79 ����Է����ᴦ�ڵж�״̬*/

#define ZONEERR_CLAN26 80
/* @@error 80 ��û��Ȩ�޽��д�������*/

#define ZONEERR_CLAN12 81
/* @@error 81 ���Ѿ���GM������������*/

#define ZONEERR_CLAN13 82
/* @@error 82 ��ǰ����ȼ���,��������,�޷�����*/

#define ZONEERR_CLAN14 83
/* @@error 83 ����������Ϊ��*/

#define ZONEERR_CLAN15 84
/* @@error 84 �������������дʻ�Ƿ��ַ�(ֻ������Ӣ�Ļ�����),����������*/

#define ZONEERR_CLAN16 85
/* @@error 85 ��������Ѿ��й���ʹ����*/

#define ZONEERR_CLAN17 86
/* @@error 86 ���������Ҳ����ڻ�����*/

#define ZONEERR_CLAN18 87
/* @@error 87 ����Ҹո��˳��˹��ᣬ�޷��ٱ����룬��ȴ�%s������*/

#define ZONEERR_CLAN19 88
/* @@error 88 ��ǰ����ȼ���,��������,�޷�����ӳ�Ա*/

#define ZONEERR_CLAN20 89
/* @@error 89 �����������Ѿ������˹���*/

#define ZONEERR_CLAN21 90
/* @@error 90 �������������ڽ����������룬���Ժ�����*/

#define ZONEERR_CLAN22 91
/* @@error 91 �㲻�ܰ��Լ��������*/

#define ZONEERR_CLAN23 92
/* @@error 92 ��û��Ȩ�޽������������*/

#define ZONEERR_CLAN24 93
/* @@error 93 ���Ѿ������������*/

#define ZONEERR_CLAN25 94
/* @@error 94 �᳤�������˳�����*/

#define ZONEERR_CLAN27 96
/* @@error 96 �Է�������,���ܽ�����ת�õĹ���ְλ*/

#define ZONEERR_CLAN28 97
/* @@error 97 �Է��Ѳ��ڹ�����,���ܽ�����ת�õĹ���ְλ*/

#define ZONEERR_CLAN29 98
/* @@error 98 ���ܰѹ���ְλת�ø����Լ����ߵ���*/

#define ZONEERR_CLAN30 99
/* @@error 99 ��ǰ����ȼ���,������������,�޷�����*/

#define ZONEERR_CLAN31 100
/* @@error 100 ���Ѿ��������б�����*/

#define ZONEERR_CLAN32 101
/* @@error 101 %s �ܾ�������빫�������*/

#define ZONEERR_BATTLE_SVR_OP 102
/* @@error 102 ���״̬�޷����д˲���*/

#define ZONEERR_SVR_ERR 103
/* @@error 103 ���ڷ������ڲ����ϵ��¹���ϵͳ��ǰ������*/

#define ZONEERR_CLAN33 104
/* @@error 104 ָ���Ĳ�Ʒ������, �޷�����*/

#define ZONEERR_CLAN34 105
/* @@error 105  ͬʱ���쵽������, ��ȴ������������*/

#define ZONEERR_CLAN35 106
/* @@error 106  ��Դ������λ����, �޷�����*/

#define ZONEERR_CLAN36 107
/* @@error 107  �����ʽ𲻹�, �޷�����*/

#define ZONEERR_CLAN37 108
/* @@error 108  �Ƽ�������, �޷�ѧϰ*/

#define ZONEERR_CLAN38 109
/* @@error 109  �Ƽ����ĵȼ�����,�޷�ѧϰ*/

#define ZONEERR_CLAN39 110
/* @@error 110  �Ƽ�: %s �Ѿ���ѧϰ��, �����ظ�ѧϰ*/

#define ZONEERR_CLAN40 111
/* @@error 111  ����Ƽ���������, �޷�ѧϰ*/

#define ZONEERR_CLAN41 112
/* @@error 112  ǰ�ÿƼ���δѧϰ*/

#define ZONEERR_CLAN42 113
/* @@error 113  �����ʽ���, �޷�ѧϰ*/

#define ZONEERR_CLAN43 114
/* @@error 114  �Ƽ�: %s Lv.%d��δѧϰ*/

#define ZONEERR_CLAN44 115
/* @@error 115  �Ƽ�: %s�Ѿ�ѧϰ����*/

#define ZONEERR_CLAN45 116
/* @@error 116  ���ֻ��ͬʱѧϰ%d���Ƽ�*/

#define ZONEERR_CLAN46 117
/* @@error 117  ����Ƽ���û�����з���*/

#define ZONEERR_CLAN47 118
/* @@error 118  û���з��ڽ�����,�޷�ȡ���з�*/

#define ZONEERR_CLAN48 119
/* @@error 119  �Ƽ�������,�޷�ȡ��*/

#define ZONEERR_CLAN49 120
/* @@error 120  ����Ƽ���û�����з���*/

#define ZONEERR_CLAN50 121
/* @@error 121  û���з��ڽ�����,�޷������з�*/

#define ZONEERR_CLAN51 122
/* @@error 122  �Ƽ�������,�޷������з�*/

#define ZONEERR_CLAN52 123
/* @@error 123  û����Ʒ��������,�޷�ȡ������*/

#define ZONEERR_CLAN53 124
/* @@error 124  ��������,�޷�ȡ������*/

#define ZONEERR_CLAN54 125
/* @@error 125  ��Ʒ������,�޷�ȡ������*/

#define ZONEERR_CLAN55 126
/* @@error 126  û����Ʒ��������,�޷���������*/

#define ZONEERR_CLAN56 127
/* @@error 127  ��������,�޷���������*/

#define ZONEERR_CLAN57 128
/* @@error 128  ��Ʒ������,�޷���������*/

#define ZONEERR_CLAN58 129
/* @@error 129  ���켴�����, �������*/

#define ZONEERR_CLAN59 130
/* @@error 130  �����ʽ�������ɼ���, ����%d*/

#define ZONEERR_CLAN60 131
/* @@error 131  �һ�����������Դ�����е�����*/

#define ZONEERR_CLAN61 132
/* @@error 132  ���ṱ�׶Ȳ���!*/

#define ZONEERR_CLAN62 133
/* @@error 133  ������%d�㹫�ṱ�׶�*/

#define ZONEERR_CLAN63 134
/* @@error 134  ��������, �޷���������*/

#define ZONEERR_CLAN64 135
/* @@error 135  ��û��Ȩ�޲������������Ʒ*/

#define ZONEERR_CLAN65 136
/* @@error 136  ������Դ���Ŀռ䲻�㣬�޷��ύ��Դ%s*/

#define ZONEERR_CLAN66 137
/* @@error 137  �������Ʒ��Ҫ�Ĺ�����Դ���㣬�޷�����*/

#define ZONEERR_CLAN67 138
/* @@error 138  ��ǰ�ĿƼ�ˮƽ����������%s*/

#define ZONEERR_CLAN68 139
/* @@error 139  �����������¹���ʧ��*/

#define ZONEERR_CLAN69 140
/* @@error 140  �����ʽ����Թ������Ʒ*/

#define ZONEERR_CLAN70 141
/* @@error 141  ������Դ������λ����*/

#define ZONEERR_CLAN71 142
/* @@error 142  �㲻�ǹ����Ա, ���ܽ��д˲���*/

#define ZONEERR_CLAN72 143
/* @@error 143 �Ҳ�����е, ���ܽ��д˲���*/

#define ZONEERR_CLAN73 144
/* @@error 144 ���Ǳ������е, ���ܽ��д˲���*/

#define ZONEERR_CLAN74 145
/* @@error 145 ��е����ʧ��*/

#define ZONEERR_CLAN75 146
/* @@error 146 ���ǹ����Ա, �����ύ*/

#define ZONEERR_CLAN76 147
/* @@error 147 ����Ʒ�޷��ύ��������Դ����*/

#define ZONEERR_CLAN77 148
/* @@error 148 ������Դ[%s]����%d, �����ύ!*/

#define ZONEERR_CLAN78 149
/* @@error 149 �����й�����Դ[%s]�������ύ����%d��, �����ύ!*/

#define ZONEERR_CLAN79 150
/* @@error 150 ������Դ��������, �������ύ��*/

#define ZONEERR_CLAN80 151
/* @@error 151 ����ı���λ��*/

#define ZONEERR_CLAN81 152
/* @@error 152 ����Ʒ�޷��ύ��������Դ����*/

#define ZONEERR_CLAN82 153
/* @@error 153 ���򹫻���Դ�����ύ��[%s]��%d*/

#define ZONEERR_CLAN83 154
/* @@error 154 ��������Ѵ���*/

#define ZONEERR_CLAN84 155
/* @@error 155 ��Ĺ������ڻ�û�г���*/

#define ZONEERR_CLAN85 156
/* @@error 156 ��ɱ�����߻��۸��˻��ֶһ����������˻���50 ʱ�һ��Ľ�����������*/

#define ZONEERR_CLAN86 157
/* @@error 157 ����Ĵ��͵ص�*/

#define ZONEERR_CLAN87 158
/* @@error 158 ˼�����к�����Ҳ��ܽ��д˲���*/

#define ZONEERR_CLAN88 159
/* @@error 159 �޷����д˲���,�����Ѿ���ɢ!*/

#define ZONEERR_CLAN89 160
/* @@error 160 ������в�����*/

#define ZONEERR_CLAN90 161
/* @@error 161 ���%d����֣��㵱ǰ�ѻ��%d��50����*/

#define ZONEERR_COLLECT1 162
/* @@error 162 ȱ�ٲɼ���Ҫ�Ĺ���*/

#define ZONEERR_COLLECT2 163
/* @@error 163 �ɼ�װ���;ò�����*/

#define ZONEERR_COLLECT3 164
/* @@error 164 �ɼ�ʧ��!*/

#define ZONEERR_COLLECT4 165
/* @@error 165 ���βɼ���Ҫװ��%s*/

#define ZONEERR_COLLECT5 166
/* @@error 166 %s���;öȲ�����*/

#define ZONEERR_COLLECT6 167
/* @@error 167 ���βɼ���Ҫ���ռ���%s*/

#define ZONEERR_COLLECT7 168
/* @@error 168 ���βɼ���Ҫ��ؼ��ܴﵽ%d��*/

#define ZONEERR_COLLECT8 169
/* @@error 169 ���βɼ���Ҫ�����Ʒ%s����������*/

#define ZONEERR_COLLECT9 170
/* @@error 170 ���βɼ���Ҫ�����Ʒ%s*/

#define ZONEERR_COLLECT10 171
/* @@error 171 ������״̬�²��ɲɼ�*/

#define ZONEERR_COLLECT11 172
/* @@error 172 ��ǰ״̬���ܲɼ�*/

#define ZONEERR_COLLECT12 173
/* @@error 173 �ɼ�װ���;ò�����*/

#define ZONEERR_COLLECT13 174
/* @@error 174 �ɼ���Ҫ�����Ʒ������*/

#define ZONEERR_COLLECT14 175
/* @@error 175 ���ɼ���Ŀǰ�Ѿ����������ύ��Դ���ٽ��вɼ�*/

#define ZONEERR_COLLECT15 176
/* @@error 176 �ÿ����Ҫ�����˲ɼ�������ܲɼ�*/

#define ZONEERR_COLLECT16 177
/* @@error 177 �Ǿݵ�ռ�칫���Ա��Ȩռ��*/

#define ZONEERR_COLLECT17 178
/* @@error 178 �������Ὠ����Ŀ����Ч*/

#define ZONEERR_COLLECT18 179
/* @@error 179 �ɼ�Ŀ�����ڽ�����*/

#define ZONEERR_COLLECT19 180
/* @@error 180 ��һ���ɼ�Ŀ�����ڽ�����*/

#define ZONEERR_SKILL1 181
/* @@error 181 �˵�ͼ����ʹ�ü���*/

#define ZONEERR_SKILL2 182
/* @@error 182 �˵�ͼ����ʹ����˫����*/

#define ZONEERR_SKILL3 183
/* @@error 183 ��ս��״̬�²���ʹ�ô˼���*/

#define ZONEERR_SKILL4 184
/* @@error 184 ս��״̬�²���ʹ�ô˼���*/

#define ZONEERR_SKILL5 185
/* @@error 185 �д費��ʹ�ô˼���*/

#define ZONEERR_SKILL6 186
/* @@error 186 ����״̬����ʹ�ô˼���*/

#define ZONEERR_SKILL7 187
/* @@error 187 ��ǰ״̬����ʹ�ü���*/

#define ZONEERR_SKILL8 188
/* @@error 188 ʹ����ʱ��еʱ�޷�����*/

#define ZONEERR_SKILL9 189
/* @@error 189 ��̯״̬���޷�����*/

#define ZONEERR_SKILL10 190
/* @@error 190 ����״̬���޷�����*/

#define ZONEERR_SKILL11 191
/* @@error 191 ����״̬���޷�����*/

#define ZONEERR_SKILL12 192
/* @@error 192 ˼�����к�������޷�����*/

#define ZONEERR_SKILL13 193
/* @@error 193 ��ǰ״̬�޷�����*/

#define ZONEERR_SKILL14 194
/* @@error 194 ��˻�е״̬���޷�����*/

#define ZONEERR_SKILL15 195
/* @@error 195 �������״̬�޷�����*/

#define ZONEERR_SKILL16 196
/* @@error 196 ���״̬�޷�����*/

#define ZONEERR_SKILL17 197
/* @@error 197 ����%s ������*/

#define ZONEERR_SKILL18 198
/* @@error 198 ���ѳ��������˵�������*/

#define ZONEERR_SKILL19 199
/* @@error 199 ��ǰ״̬�޷����*/

#define ZONEERR_SKILL20 200
/* @@error 200 ��ʱû�и����*/

#define ZONEERR_SKILL21 201
/* @@error 201 �˵�ͼ�������*/

#define ZONEERR_SKILL22 202
/* @@error 202 ���ʧ��,������ֻ��ʱ��ǳ�������Ȩ�����*/

#define ZONEERR_SKILL23 203
/* @@error 203 ���ʧ��,������ֻ��ʱ���ռ�칫���Ա����Ȩ�����*/

#define ZONEERR_SKILL24 204
/* @@error 204 ��������Ч���ѵ��ڣ��޷�ʹ��*/

#define ZONEERR_SKILL25 205
/* @@error 205 ����״̬���޷����*/

#define ZONEERR_SKILL26 206
/* @@error 206 ���%s �ɹ�*/

#define ZONEERR_SKILL27 207
/* @@error 207 ���״̬�޷����д˲���*/

#define ZONEERR_SKILL28 208
/* @@error 208 ��ǰѪ��������*/

#define ZONEERR_SKILL29 209
/* @@error 209 ��%s�������׳���ζС��Ϻ�������ס�ջ��뿪��ɳ̲��*/

#define ZONEERR_ROLENAME_DUP 210
/* @@error 210 ��ɫ���ѱ�ʹ��*/

#define ZONEERR_PWORLD 211
/* @@error 211 �ʱ���ѹ������ܽ��뵽������*/

#define ZONEERR_PWORLD1 212
/* @@error 212 �޷���������!*/

#define ZONEERR_PWORLD2 213
/* @@error 213 ����%s ����������������%d��!*/

#define ZONEERR_PWORLD3 214
/* @@error 214 ��������ʧ�ܣ��޷����룬ֻ�жӳ��ſɴ�������*/

#define ZONEERR_EXG1 215
/* @@error 215 ��ǰ���ĵȼ��µĳ��ﲻ�ܽ���*/

#define ZONEERR_EXG2 216
/* @@error 216 �������״̬�²��ܽ���*/

#define ZONEERR_EXG3 217
/* @@error 217 �����ռ�����,����ʧ��*/

#define ZONEERR_EXG4 218
/* @@error 218 �Է���������,����ʧ��*/

#define ZONEERR_EXG5 219
/* @@error 219 ����������,����ʧ��*/

#define ZONEERR_EXG6 220
/* @@error 220 �Է�����������,����ʧ��*/

#define ZONEERR_EXG7 221
/* @@error 221 ����̫Զ,�޷�����*/

#define ZONEERR_EXG8 222
/* @@error 222 �Է��Ѿ�����*/

#define ZONEERR_EXG9 223
/* @@error 223 �Է������쳣��¼״̬,%d�����ڲ��ܽ��д˲���*/

#define ZONEERR_EXG10 224
/* @@error 224 �Է�û����֤��ȫ������*/

#define ZONEERR_EXG11 225
/* @@error 225 ͬ������Ҳ��ܽ���*/

#define ZONEERR_EXG12 226
/* @@error 226 �Է������˳���Ϸ,���ܽ���*/

#define ZONEERR_EXG13 227
/* @@error 227 ��ȫʱ�����޷�����,��ȫʱ�仹ʣ%ld��*/

#define ZONEERR_EXG14 228
/* @@error 228 �Է��ڰ�ȫʱ�����޷�����,��ȫʱ�仹ʣ%ld��*/

#define ZONEERR_EXG15 229
/* @@error 229 �Է��ڰ�̯״̬���޷�����*/

#define ZONEERR_EXG16 230
/* @@error 230 �Է�������״̬���޷�����*/

#define ZONEERR_EXG17 231
/* @@error 231 �Է����ڽ���״̬,�޷�����*/

#define ZONEERR_EXG18 232
/* @@error 232 ���Դ���15�뽻����ȴʱ�䣬���Ժ�����*/

#define ZONEERR_EXG19 233
/* @@error 233 %s�ܾ�����*/

#define ZONEERR_EXG20 234
/* @@error 234 ���������������%s ����ȡ��*/

#define ZONEERR_EXG21 235
/* @@error 235 ����̫Զ,����ȡ��*/

#define ZONEERR_EXG22 236
/* @@error 236 ���%s :�����������Ӧ��,�Ժ�����*/

#define ZONEERR_EXG23 237
/* @@error 237 �Է�������,�޷�����*/

#define ZONEERR_EXG24 238
/* @@error 238 �Ѿ����ڽ���״̬*/

#define ZONEERR_EXG25 239
/* @@error 239 ����ȡ��*/

#define ZONEERR_EXG26 240
/* @@error 240 ��������������*/

#define ZONEERR_EXG27 241
/* @@error 241 ���׳ɹ�*/

#define ZONEERR_EXG28 242
/* @@error 242 �Է�����,����ȡ��*/

#define ZONEERR_EXG29 243
/* @@error 243 ����%s����*/

#define ZONEERR_EXG30 244
/* @@error 244 �Է�����%s����*/

#define ZONEERR_EXG31 245
/* @@error 245 �Է�%s�ﵽ����*/

#define ZONEERR_EXG32 246
/* @@error 246 %s�ﵽ����*/

#define ZONEERR_FAIRY1 247
/* @@error 247 ������λ�ﵽ����*/

#define ZONEERR_FAIRY2 248
/* @@error 248 ս��״̬�в���װ������*/

#define ZONEERR_FAIRY3 249
/* @@error 249 ͬ���ľ���ֻ��ӵ��һ��*/

#define ZONEERR_FAIRY4 250
/* @@error 250 ������λ�ﵽ����*/

#define ZONEERR_FAIRY5 251
/* @@error 251 �ɹ����(%s)����*/

#define ZONEERR_FAIRY6 252
/* @@error 252 �˸����������ٻ�����*/

#define ZONEERR_FAIRY7 253
/* @@error 253 �˵�ͼ�������ٻ�����*/

#define ZONEERR_FAIRY8 254
/* @@error 254 (%s)�����Ѿ��ǳ���״̬*/

#define ZONEERR_FAIRY9 255
/* @@error 255 ��ǰ״̬�޷��ٻ�����*/

#define ZONEERR_FAIRY10 256
/* @@error 256 ����ǰ���ڰ�̯״̬���޷�ǿ��*/

#define ZONEERR_FAIRY11 257
/* @@error 257 ����ǰ���ڽ���״̬���޷�ǿ��*/

#define ZONEERR_FAIRY12 258
/* @@error 258 �þ����������Ʒ�ʣ��޷�ǿ��*/

#define ZONEERR_FAIRY13 259
/* @@error 259 ��ǰ����ȼ����㣬�޷�ǿ��*/

#define ZONEERR_FAIRY14 260
/* @@error 260 ��ǰ���鲻������Ԫ�ؾ���!*/

#define ZONEERR_FAIRY15 261
/* @@error 261 ��ӡ��Ҫ%d ����ֵ*/

#define ZONEERR_FAIRY16 262
/* @@error 262 ȱ�ٶ�Ӧ�ĵ���!*/

#define ZONEERR_FAIRY17 263
/* @@error 263 (%s)�����ٻ��ɹ�*/

#define ZONEERR_FAIRY18 264
/* @@error 264 ����ʧ��,(%s)�����Ѿ�����*/

#define ZONEERR_FAIRY19 265
/* @@error 265 ����ʧ��,��ɫ�ȼ�����%d��*/

#define ZONEERR_FAIRY20 266
/* @@error 266 (%s)��������������*/

#define ZONEERR_FAIRY21 267
/* @@error 267 ����(%s)������Ҫ%d %s*/

#define ZONEERR_FAIRY22 268
/* @@error 268 ����(%s)������Ҫ%d����*/

#define ZONEERR_FAIRY23 269
/* @@error 269 û�������еľ���*/

#define ZONEERR_FAIRY24 270
/* @@error 270 �������Ҳ������ٳ���,�޷�������������*/

#define ZONEERR_FAIRY25 271
/* @@error 271 �ɹ�������������,����ʱ�����%d����*/

#define ZONEERR_FAIRY26 272
/* @@error 272 (%s)����û�г���,�����ٻ�*/

#define ZONEERR_FAIRY27 273
/* @@error 273 (%s)�ջسɹ�*/

#define ZONEERR_FAIRY28 274
/* @@error 274 %s���������ɹ�,�ȼ�������%d��*/

#define ZONEERR_FAIRY29 275
/* @@error 275 (%s)��������������,����ע�뾭��*/

#define ZONEERR_FAIRY30 276
/* @@error 276 ע��������������*/

#define ZONEERR_FAIRY31 277
/* @@error 277 ����ֵ����*/

#define ZONEERR_FAIRY32 278
/* @@error 278 ���ϲ��㣬�޷�����*/

#define ZONEERR_FAIRY33 279
/* @@error 279 �����������㣬�޷�����*/

#define ZONEERR_FAIRY34 280
/* @@error 280 %s����ǿ���ɹ�,�Ǽ�������%d��*/

#define ZONEERR_FAIRY35 281
/* @@error 281 %s����ǿ��ʧ��,�Ǽ�������%d��*/

#define ZONEERR_FAIRY36 282
/* @@error 282 %s����ǿ��ʧ��*/

#define ZONEERR_GENUIS1 283
/* @@error 283 ����״̬�²��������츳!*/

#define ZONEERR_GENUIS2 284
/* @@error 284 ��ɫ�ȼ�������60���������츳ϵͳ!*/

#define ZONEERR_GENUIS3 285
/* @@error 285 �츳:%s �Ѿ�����߼���!*/

#define ZONEERR_GENUIS4 286
/* @@error 286 ��ɫ�ȼ����㣬�޷�����!*/

#define ZONEERR_GENUIS5 287
/* @@error 287 ���鲻�㣬�޷�����!*/

#define ZONEERR_GENUIS6 288
/* @@error 288 %s���㣬�޷�����!*/

#define ZONEERR_GENUIS7 289
/* @@error 289 ��Ҫ�츳ֵ�ﵽ%d�ſ�����!*/

#define ZONEERR_GENUIS8 290
/* @@error 290 %s ��������%s��%d��!*/

#define ZONEERR_GM1 291
/* @@error 291 �Ҳ���Ҫɾ������Ʒ*/

#define ZONEERR_GM2 292
/* @@error 292 �㱻����%d ��*/

#define ZONEERR_GM3 293
/* @@error 293 װ�ؽű�ʧ��*/

#define ZONEERR_GM4 294
/* @@error 294 װ�ؽű��ɹ�*/

#define ZONEERR_GM5 295
/* @@error 295 ����[%s]ѧ����%s*/

#define ZONEERR_GM6 296
/* @@error 296 �˸���Ҫ�����*/

#define ZONEERR_GM7 297
/* @@error 297 ����:%s �ѱ��ύɾ��.*/

#define ZONEERR_GM8 298
/* @@error 298 ��������ؽ�*/

#define ZONEERR_GM9 299
/* @@error 299 clan:%s, lv:%d, money:%d, stable:%d, active:%d, tech:%d, used-tech:%d, memnum:%d*/

#define ZONEERR_GM10 300
/* @@error 300 conf : %s | %d to %d*/

#define ZONEERR_GM11 301
/* @@error 301 conf : %s = %d*/

#define ZONEERR_GM12 302
/* @@error 302 ID    ʣ��ʱ��  �������:��������*/

#define ZONEERR_GM13 303
/* @@error 303 %08x  %d  %d:%s */

#define ZONEERR_GM14 304
/* @@error 304 �ܼ� %d ������*/

#define ZONEERR_GM15 305
/* @@error 305 �ɹ������һ������ ID = %08x */

#define ZONEERR_GM16 306
/* @@error 306 ID: �ƺ� */

#define ZONEERR_GM17 307
/* @@error 307 �ܼ�%u���ƺ� */

#define ZONEERR_GM18 308
/* @@error 308 ��ǰ������ʱ��:%s������������ʱ��:%s����ǰ����%u */

#define ZONEERR_GM19 309
/* @@error 309 ��û�����Ȩ�� */

#define ZONEERR_GM20 310
/* @@error 310 �Է������� */

#define ZONEERR_MAIL1 311
/* @@error 311 ����һ�����ʼ��������������뼰ʱ���� */

#define ZONEERR_MAIL2 312
/* @@error 312 �ʼ����ͳɹ�*/

#define ZONEERR_MAIL3 313
/* @@error 313 ��ȡ��Ʒʧ��,��������*/

#define ZONEERR_MAIL4 314
/* @@error 314 ��ȡ��Ʒʧ��,�Ҳ����ʼ��е���Ʒ*/

#define ZONEERR_MAIL5 315
/* @@error 315 ��ȡ��Ʒʧ��,��������*/

#define ZONEERR_MAIL6 316
/* @@error 316 ��ȡ%sʧ��,%s�Ѵ�����*/

#define ZONEERR_MAIL7 317
/* @@error 317 �����ռ䲻�㣬����������*/

#define ZONEERR_MAIL8 318
/* @@error 318 %s ���͸�����ʼ�������һ�ε�½ʱ���������ע�����*/

#define ZONEERR_RELATION1 319
/* @@error 319 �����Ӧ�����%s��Ե��,�Ѱ����Ǽ�Ϊ��ʱ����*/

#define ZONEERR_RELATION2 320
/* @@error 320 ��������Լ�Ϊ����*/

#define ZONEERR_RELATION3 321
/* @@error 321 Ŀ������ĺ�����,�޷����Ӻ���,�밴F���򿪺������鿴*/

#define ZONEERR_RELATION4 322
/* @@error 322 Ŀ��������޵�����,�޷����Ӻ���,�밴F�����޵������鿴*/

#define ZONEERR_RELATION5 323
/* @@error 323 �Է��Ѿ��������������*/

#define ZONEERR_RELATION6 324
/* @@error 324 ���������ﵽ����*/

#define ZONEERR_RELATION7 325
/* @@error 325 ���Ѿ���Է������Ϊ����*/

#define ZONEERR_RELATION8 326
/* @@error 326 %s�����߻����ڲ�ͬ������,���ܼ�Ϊ����*/

#define ZONEERR_RELATION9 327
/* @@error 327 %s����Ϊ����*/

#define ZONEERR_RELATION10 328
/* @@error 328 ��ܾ���%s Ϊ����*/

#define ZONEERR_RELATION11 329
/* @@error 329 %s �������볬ʱ�����ٴγ���*/

#define ZONEERR_RELATION12 330
/* @@error 330 %s �ܾ�����Ϊ����*/

#define ZONEERR_RELATION13 331
/* @@error 331 %sĿǰ�����߻����ڲ�ͬ������,���ܼ�Ϊ������*/

#define ZONEERR_RELATION14 332
/* @@error 332 ���ܰ��Լ���ӽ�������*/

#define ZONEERR_RELATION15 333
/* @@error 333 �Է��Ѿ��ں�������*/

#define ZONEERR_RELATION16 334
/* @@error 334 �������ﵽ����,��ɾ���������*/

#define ZONEERR_RELATION17 335
/* @@error 335 ��ǰ��ϵ���ܰ�%s��ӽ�������*/

#define ZONEERR_RELATION18 336
/* @@error 336 ���%s������ӽ�������*/

#define ZONEERR_RELATION19 337
/* @@error 337 ���ܰ��Լ������޵�����*/

#define ZONEERR_RELATION20 338
/* @@error 338 ����������ϵ��������޵�����*/

#define ZONEERR_RELATION21 339
/* @@error 339 �޵������Ѵﵽ����,��ɾ���������*/

#define ZONEERR_RELATION22 340
/* @@error 340 %sĿǰ������,�޷��鿴װ��*/

#define ZONEERR_RELATION23 341
/* @@error 341 ���%s �������Ϊ����*/

#define ZONEERR_RELATION24 342
/* @@error 342 �Ѿ����޵�������*/

#define ZONEERR_RELATION25 343
/* @@error 343 %s ��������޵�����*/

#define ZONEERR_RELATION26 344
/* @@error 344 ��� %s ������ͬ�ķ�!*/

#define ZONEERR_RELATION27 345
/* @@error 345 ͬ������Ҳſɼ�Ϊ����*/

#define ZONEERR_RELATION28 346
/* @@error 346 ͬ������Ҳſɼ��������*/

#define ZONEERR_SHOP1 347
/* @@error 347 û���㹻����Ʒ������!*/

#define ZONEERR_SHOP2 348
/* @@error 348 һ���Թ���Ķ���̫�࣬����������*/

#define ZONEERR_SHOP3 349
/* @@error 349 ������Ʒʧ��*/

#define ZONEERR_NOMONEY1 350
/* @@error 350 û���㹻�Ľ�ȯ*/

#define ZONEERR_SHOP4 351
/* @@error 351 ������Ʒ�ɹ�*/

#define ZONEERR_SHOP5 352
/* @@error 352 ������Ʒ�ɹ�*/

#define ZONEERR_NOMONEY2 353
/* @@error 353 û���㹻�Ľ��*/

#define ZONEERR_SHOP6 354
/* @@error 354 ����ʧ��*/

#define ZONEERR_SHOP7 355
/* @@error 355 �Է�������ĺ���,����ʧ��*/

#define ZONEERR_SHOP8 356
/* @@error 356 �ý�ɫ�ѱ��û�ɾ��,����ʧ��*/

#define ZONEERR_SHOP9 357
/* @@error 357 ����Ʒ��û���ϼ�ʱ��*/

#define ZONEERR_SHOP10 358
/* @@error 358 ����Ʒ�¼���*/

#define ZONEERR_SHOP11 359
/* @@error 359 ����������*/

#define ZONEERR_SHOP12 360
/* @@error 360 ����ʧ�ܣ����������˵㣬�´�����ϳ���*/

#define ZONEERR_SHOP13 361
/* @@error 361 ���ܽ����Լ��ĳ��۵�*/

#define ZONEERR_SHOP14 362
/* @@error 362 ���%s��Ҫ�ﵽ���ޣ�����ʧ��*/

#define ZONEERR_SHOP15 363
/* @@error 363 ���ܽ����Լ��ķ�����*/

#define ZONEERR_SHOP16 364
/* @@error 364 ������ҽ��׵����Ѵﵽ���ޣ����Ժ�����*/

#define ZONEERR_SHOP17 365
/* @@error 365 ����ʧ�ܣ��㷢���Ľ�ҵ��ѵ�������*/

#define ZONEERR_SHOP18 366
/* @@error 366 %s����, �޷�����*/

#define ZONEERR_SHOP19 367
/* @@error 367 �������Ӳ�����֧�����۵�������*/

#define ZONEERR_SHOP20 368
/* @@error 368 ����ʧ�ܣ��㷢�������ҵ��ѵ�������*/

#define ZONEERR_SHOP21 369
/* @@error 369 �������ҽ��׵����Ѵﵽ���ޣ����Ժ�����*/

#define ZONEERR_SHOP22 370
/* @@error 370 ��Ľ�ҽ��ﵽ���ޣ�����ʧ��!*/

#define ZONEERR_SHOP23 371
/* @@error 371 ������ҽ��ﵽ���ޣ�����ʧ��!*/

#define ZONEERR_SHOP24 372
/* @@error 372 ���׵�������*/

#define ZONEERR_SHOP25 373
/* @@error 373 û�ҵ�����Ϊ %s �ĺ���*/

#define ZONEERR_SHOP26 374
/* @@error 374 ��ҳ��۵�ʧ�ܣ������ͨ���ʼ������������*/

#define ZONEERR_SHOP27 375
/* @@error 375 ���׵������ɹ�!*/

#define ZONEERR_SHOP28 376
/* @@error 376 ���׵�����ʧ�ܣ�����ͨ���ʼ������������*/

#define ZONEERR_SHOP29 377
/* @@error 377 �������ҳ��۵��ɹ����밴[B]���򿪱������ա�*/

#define ZONEERR_SHOP30 378
/* @@error 378 �һ�ʧ��,��������ȷ��ֵ*/

#define ZONEERR_SHOP31 379
/* @@error 379 ��ѯ%s���ʧ��*/

#define ZONEERR_SHOP32 380
/* @@error 380 ��������%u���ҹ�����%u��ң��밴��V�������̳ǲ���*/

#define ZONEERR_SHOP33 381
/* @@error 381 �����۵�%u����Ѿ����۳ɹ���������ʼ�*/

#define ZONEERR_SHOP34 382
/* @@error 382 ����%s���۵��ɹ����밴[V]�����̳ǲ���*/

#define ZONEERR_SHOP35 383
/* @@error 383 ���Ľ�ҳ��۵����ڷ����ˣ���ͨ���ʼ�����*/

#define ZONEERR_SHOP36 384
/* @@error 384 ��������%u��ҹ�����%u���ң�����˰�����밴��B�����򿪱�������*/

#define ZONEERR_SHOP37 385
/* @@error 385 �����۵�%u�����Ѿ����۳ɹ���������ʼ�*/

#define ZONEERR_SHOP38 386
/* @@error 386 �������ҳ��۵����ڷ����ˣ���ͨ���ʼ�����*/

#define ZONEERR_SHOP39 387
/* @@error 387 ���㣬��ȡ���ʧ��*/

#define ZONEERR_SHOP40 388
/* @@error 388 ϵͳ������ȡ���ʧ��*/

#define ZONEERR_STALL1 389
/* @@error 389 ��λ������̯λ����ǰ���տ�����*/

#define ZONEERR_STALL2 390
/* @@error 390 �Ͻ���NPC��Χ��̯����ǰ���տ�����*/

#define ZONEERR_STALL3 391
/* @@error 391 ��ǰ���ĵȼ��µĳ��ﲻ�ܽ���*/

#define ZONEERR_STALL4 392
/* @@error 392 ����״̬�µĳ��ﲻ�ܽ���*/

#define ZONEERR_STALL5 393
/* @@error 393 ��ɫ�ȼ����ڵ���20�����ܰ�̯*/

#define ZONEERR_STALL6 394
/* @@error 394 �д�״̬�²��ܰ�̯*/

#define ZONEERR_STALL7 395
/* @@error 395 ����״̬�²��ܰ�̯*/

#define ZONEERR_STALL8 396
/* @@error 396 ���Ѿ��ڰ�̯��*/

#define ZONEERR_STALL9 397
/* @@error 397 ����״̬���޷��鿴̯λ*/

#define ZONEERR_STALL10 398
/* @@error 398 ����̫Զ�޷��鿴̯λ*/

#define ZONEERR_STALL11 399
/* @@error 399 ̯���Ѿ�����*/

#define ZONEERR_STALL12 400
/* @@error 400 ̯���Ѿ���̯*/

#define ZONEERR_STALL13 401
/* @@error 401 ��Ʒ�Ѿ��������¼�*/

#define ZONEERR_STALL14 402
/* @@error 402 ̯�����ڵ�������Ʒ*/

#define ZONEERR_STALL15 403
/* @@error 403 ��Ʒ�Ѿ�����,��������*/

#define ZONEERR_STALL16 404
/* @@error 404 ̯��%s�����ﵽЯ������*/

#define ZONEERR_STALL17 405
/* @@error 405 �����Ѿ��������¼�*/

#define ZONEERR_STALL18 406
/* @@error 406 ̯�����ڵ����˳���*/

#define ZONEERR_STALL19 407
/* @@error 407 ������λ����*/

#define ZONEERR_STALL20 408
/* @@error 408 ��̯״̬�²���������λ*/

#define ZONEERR_STALL21 409
/* @@error 409 ̯λ���Ѵﵽ����*/

#define ZONEERR_STALL22 410
/* @@error 410 û����Ӧ�ĵ���������̯λ��*/

#define ZONEERR_STALL23 411
/* @@error 411 ̯λ�����������޷��ϼܸ������Ʒ*/

#define ZONEERR_STALL24 412
/* @@error 412 ������Ʒ������Я���Ľ����ó����ܶ�����*/

#define ZONEERR_PAK1 413
/* @@error 413 ������%d��%s*/

#define ZONEERR_PAK2 414
/* @@error 414 �����ƶ���Ŀ����λ*/

#define ZONEERR_PAK3 415
/* @@error 415 ��Ʒ�����ƶ��ǵ����*/

#define ZONEERR_PAK4 416
/* @@error 416 �̳Ǳ������ܴ洢���񱳰���Ʒ*/

#define ZONEERR_PAK5 417
/* @@error 417 �����ƶ���Ʒ*/

#define ZONEERR_PAK6 418
/* @@error 418 �̳Ǳ������ܴ洢�ֿⱳ����Ʒ*/

#define ZONEERR_PAK7 419
/* @@error 419 ��̯״̬����������*/

#define ZONEERR_PAK8 420
/* @@error 420 ����״̬����������*/

#define ZONEERR_PAK9 421
/* @@error 421 �ֿ���Ʒ�Ѿ�����*/

#define ZONEERR_PAK10 422
/* @@error 422 ������Ʒ�Ѿ�����*/

#define ZONEERR_PAK11 423
/* @@error 423 ��������ְҵ�����*/

#define ZONEERR_PAK12 424
/* @@error 424 ��������Ҫ��С�ȼ�Ϊ%d��*/

#define ZONEERR_PAK13 425
/* @@error 425 �����Ա𲻷�*/

#define ZONEERR_PAK14 426
/* @@error 426 ���Ѿ�ӵ���˸�����*/

#define ZONEERR_PAK15 427
/* @@error 427 ������λ����!*/

#define ZONEERR_PAK16 428
/* @@error 428 ���Ѿ�����������������λ��*/

#define ZONEERR_ARM101 429
/* @@error 429 װ���ȼ���ϴ��ʯ����*/

#define ZONEERR_ARM102 430
/* @@error 430 װ��ϴ������İ󶨲��ϲ���*/

#define ZONEERR_ARM103 431
/* @@error 431 װ��ϴ������ķǰ󶨵Ĳ��ϲ���*/

#define ZONEERR_ARM104 432
/* @@error 432 %s���㣬�޷�����ϴ������*/

#define ZONEERR_ARM105 433
/* @@error 433 %s���㣬�޷�����ǿ������*/

#define ZONEERR_ARM106 434
/* @@error 434 װ��ǿ������İ󶨱�ʯ����*/

#define ZONEERR_ARM107 435
/* @@error 435 �ǰ�װ��ǿ������ķǰ󶨵ı�ʯ����*/

#define ZONEERR_ARM108 436
/* @@error 436 ��������ֻ������%d���ı�ʯ!*/

#define ZONEERR_ARM109 437
/* @@error 437 �ļ����ϱ�ʯ����ͨ����������!*/

#define ZONEERR_ARM110 438
/* @@error 438 ��ʯ:%s �Ѿ�����߼���ʯ!*/

#define ZONEERR_ARM111 439
/* @@error 439 ��Ƕʧ�ܣ�%s����*/

#define ZONEERR_ARM112 440
/* @@error 440 װ�����Ѿ����ˣ������ٴ����*/

#define ZONEERR_ARM113 441
/* @@error 441 ������ʯ��ʧ�ܣ�%s����*/

#define ZONEERR_ARM114 442
/* @@error 442 ���ź������ʧ���ˣ�Ҫ���������*/

#define ZONEERR_ARM115 443
/* @@error 443 ��ϲ���׳ɹ���*/

#define ZONEERR_ARM116 444
/* @@error 444 ��Ҫ%d %s����Ŀǰ���ֻ��Ҳ���*/

#define ZONEERR_ARM117 445
/* @@error 445 ת��Ʒ������ı�ʯ����*/

#define ZONEERR_ARM118 446
/* @@error 446 ת��Ʒ��ʧ��*/

#define ZONEERR_ARM119 447
/* @@error 447 �ܿ�ϧ���Ǽ�ת��δ�ܳɹ������ٴγ���*/

#define ZONEERR_ARM120 448
/* @@error 448 ��ϲ��װ���Ǽ�ת�Ƴɹ���*/

#define ZONEERR_ARM121 449
/* @@error 449 ��ȯ����,�����Խ��*/

#define ZONEERR_ARM122 450
/* @@error 450 ��Ҳ���,�����Խ��*/

#define ZONEERR_ARM123 451
/* @@error 451 ʱװ���ֽܷ�!*/

#define ZONEERR_ARM124 452
/* @@error 452 ���ɷֽ��������ȼ�20����װ��!*/

#define ZONEERR_ARM125 453
/* @@error 453 ʱװ��������!*/

#define ZONEERR_ARM126 454
/* @@error 454 ����������������ȼ�20����װ��!*/

#define ZONEERR_ARM127 455
/* @@error 455 ��Ҫ�츳ֵ�ﵽ100�����ʹ�þ�ʯ����!*/

#define ZONEERR_ARM128 456
/* @@error 456 �����ɹ�!*/

#define ZONEERR_NOENOUGH 457
/* @@error 457 %s ����!*/

#define ZONEERR_ARM129 458
/* @@error 458 ����%d�ı�ʯ���ܳ���!*/

#define ZONEERR_ARM130 459
/* @@error 459 �˱�ʯ���ܳ���!*/

#define ZONEERR_ARM131 460
/* @@error 460 ʹ�õĲ��ϲ�������װ��*/

#define ZONEERR_ARM132 461
/* @@error 461 ��ѡ�����������װ���ȼ�������ʯ*/

#define ZONEERR_ARM133 462
/* @@error 462 %s���㣬�޷�������������*/

#define ZONEERR_ARM134 463
/* @@error 463 ����ʯ����!*/

#define ZONEERR_ARM135 464
/* @@error 464 %s���㣬�޷����и߼�����*/

#define ZONEERR_NOMONEY3 465
/* @@error 465 û���㹻�İ���*/

#define ZONEERR_ARM136 466
/* @@error 466 ��ʯ�Ѿ�����ߵȼ���!*/

#define ZONEERR_ATTR101 467
/* @@error 467 ����ֵ����*/

#define ZONEERR_ATTR102 468
/* @@error 468 ����%d�����ֵ*/

#define ZONEERR_ATTR103 469
/* @@error 469 ����%d�����ֵ(������%d��) */

#define ZONEERR_ATTR104 470
/* @@error 470 ����%d�����ֵ */

#define ZONEERR_ATTR105 471
/* @@error 471 ����Я�����������ӣ����Բ����������� */

#define ZONEERR_ATTR106 472
/* @@error 472 ��ɫ20������ʹ�ð�ȫ������ */

#define ZONEERR_ATTR107 473
/* @@error 473 ��ĵȼ���������%d��*/

#define ZONEERR_ATTR108 474
/* @@error 474 ���ڵȼ���ӡ��Ӱ�죬��ľ����ȡЧ���½���%d��*/

#define ZONEERR_PWORLD101 475
/* @@error 475 %s �ȼ����ڸ�����С�ȼ�Ҫ��!*/

#define ZONEERR_PWORLD102 476
/* @@error 476 %s �ȼ����ڸ������ȼ�Ҫ��!*/

#define ZONEERR_PWORLD103 477
/* @@error 477 %s %s��ɴ����Ѵﵽÿ����ɴ�������!*/

#define ZONEERR_PWORLD104 478
/* @@error 478 %s���ڽ�����,�޷�������������*/

#define ZONEERR_PWORLD105 479
/* @@error 479 ս��״̬�޷�����*/

#define ZONEERR_PWORLD106 480
/* @@error 480 ����%s ����������������%d��!*/

#define ZONEERR_PWORLD107 481
/* @@error 481 ��Ա%s �����߻��ڿ��״̬!*/

#define ZONEERR_PWORLD108 482
/* @@error 482 ������ֻ�жӳ����ܿ����˸���!*/

#define ZONEERR_PWORLD109 483
/* @@error 483 ��������Ҫһ���յı��������ʹ�ø���Ʒ*/

#define ZONEERR_PWORLD110 484
/* @@error 484  4Сʱ��ֻ�����10��ս��*/

#define ZONEERR_PWORLD111 485
/* @@error 485 ��������,��޷�����,���´βμ�*/

#define ZONEERR_PWORLD112 486
/* @@error 486 ���� ������ ״̬! ���ܱ����Ŷ�!*/

#define ZONEERR_PWORLD113 487
/* @@error 487 �����в��ܱ����Ŷ�!*/

#define ZONEERR_PWORLD114 488
/* @@error 488 �˸��������ظ�����*/

#define ZONEERR_PWORLD115 489
/* @@error 489 ��ǰ�ȼ����㣬���ܽ���%s*/

#define ZONEERR_PWORLD116 490
/* @@error 490 δ������ʱ�䣬���ܽ���%s*/

#define ZONEERR_PWORLD117 491
/* @@error 491 %s��������Ѵﵽÿ��Ĵ�������*/

#define ZONEERR_PWORLD118 492
/* @@error 492 ��ĵȼ������ϣ��޷�����ս��*/

#define ZONEERR_PWORLD119 493
/* @@error 493 �㴦�� ���ƴ���״̬����ʱ���ܽ���ս��*/

#define ZONEERR_PWORLD120 494
/* @@error 494 %s��������Ѵﵽÿ��Ĵ�������*/

#define ZONEERR_PWORLD121 495
/* @@error 495 ˼�����к�����Ҳ��ܱ���ս��*/

#define ZONEERR_PWORLD122 496
/* @@error 496 �㲻�ڶ����У��޷����鱨��*/

#define ZONEERR_PWORLD123 497
/* @@error 497 ���ڶ����У���жӳ�����*/

#define ZONEERR_PWORLD124 498
/* @@error 498 �����ɶӳ�����!*/

#define ZONEERR_PWORLD125 499
/* @@error 499 %s �������ƴ���״̬,��ʱ���ܱ���ս��*/

#define ZONEERR_PWORLD126 500
/* @@error 500 %s �� ������ ״̬! ���ܱ����Ŷ�!*/

#define ZONEERR_PWORLD127 501
/* @@error 501 %s �ȼ������ϣ��޷�����ս��*/

#define ZONEERR_PWORLD128 502
/* @@error 502 %s ���ڿ��״̬��,���ܱ���*/

#define ZONEERR_PWORLD129 503
/* @@error 503 %s ���ڸ�����,���ܱ���*/

#define ZONEERR_PWORLD130 504
/* @@error 504 ˼�����к�����Ҳ��ܱ���ս��*/

#define ZONEERR_PWORLD131 505
/* @@error 505 %s��%s��������Ѵﵽÿ��Ĵ�������*/

#define ZONEERR_PWORLD132 506
/* @@error 506 %s�����߻��ڱ���ͼ,���ܽ��б���*/

#define ZONEERR_PAK101 507
/* @@error 507 ��ӵ����%s ���� */

#define ZONEERR_PAK102 508
/* @@error 508 ���Ѿ�ѧ���˴˼���*/

#define ZONEERR_PAK103 509
/* @@error 509 ��ǰ�޳������.�޷�ʹ��*/

#define ZONEERR_PAK104 510
/* @@error 510 �����ѵ���ߵȼ�,���ܻ�ȡ����*/

#define ZONEERR_PAK105 511
/* @@error 511 ����ȼ�����,���ܻ�ȡ����*/

#define ZONEERR_PAK106 512
/* @@error 512 �����ҳ϶�����������Ҫ������*/

#define ZONEERR_PAK107 513
/* @@error 513 ����״̬,�޷�ʰȡ��Ʒ*/

#define ZONEERR_PAK108 514
/* @@error 514 ����̫Զ�޷�ʰȡ*/

#define ZONEERR_PAK109 515
/* @@error 515 ����Ʒ���������У���ȴ���Ʒ��������ʱ�������ʰȡ*/

#define ZONEERR_PAK110 516
/* @@error 516 ��ǰ״̬����ʰȡ��Ʒ*/

#define ZONEERR_PAK111 517
/* @@error 517 ְҵ����*/

#define ZONEERR_PAK112 518
/* @@error 518 �ȼ�����*/

#define ZONEERR_PAK113 519
/* @@error 519 ������ѧ�� %s  ����*/

#define ZONEERR_PAK114 520
/* @@error 520 �Ա𲻷�*/

#define ZONEERR_PAK115 521
/* @@error 521 ������Ҳ��ܴ�����װ��*/

#define ZONEERR_PAK116 522
/* @@error 522 ������Ҳ��ܴ�����װ��*/

#define ZONEERR_PAK117 523
/* @@error 523 ɱ��ֵ�ﵽ%d ���ܴ�����װ��*/

#define ZONEERR_PAK118 524
/* @@error 524 ʹ��ʧ��,����Ʒʹ��Ȩ��Ϊʱ��ǳ���*/

#define ZONEERR_PAK119 525
/* @@error 525 ʹ��ʧ��,����Ʒʹ��Ȩ��Ϊʱ���ռ�칫���Ա*/

#define ZONEERR_PAK120 526
/* @@error 526 ��Ҫ�츳�ﵽ%d��*/

#define ZONEERR_PAK121 527
/* @@error 527 ��ǰ״̬�����л�����*/

#define ZONEERR_PAK122 528
/* @@error 528 ��û��ѧ��˼���,��������*/

#define ZONEERR_PAK123 529
/* @@error 529 ���ܵȼ�����*/

#define ZONEERR_PAK124 530
/* @@error 530 ���赲�޷�ʹ�õ���*/

#define ZONEERR_PAK125 531
/* @@error 531 Ŀ������Զ�޷�ʹ�õ���*/

#define ZONEERR_PAK126 532
/* @@error 532 ֻ�ܶ����ʹ��*/

#define ZONEERR_PAK127 533
/* @@error 533 �޷���Ŀ��ʹ��*/

#define ZONEERR_PAK128 534
/* @@error 534 Ŀ�����ߴ˲���*/

#define ZONEERR_PAK129 535
/* @@error 535 Ŀ���Ѿ��ж�Ӧ��״̬,�޷��ٶ���ʹ�ô���Ʒ*/

#define ZONEERR_PAK130 536
/* @@error 536 ���Ѿ�����Ӧ��״̬,�޷�ʹ�ô���Ʒ*/

#define ZONEERR_PAK131 537
/* @@error 537 �������������貹��*/

#define ZONEERR_PAK132 538
/* @@error 538 ���Ѿ�ѧ�������Լ��*/

#define ZONEERR_PAK133 539
/* @@error 539 �����Լ�Ѿ���������,��������*/

#define ZONEERR_PAK134 540
/* @@error 540 �����������ﵽ����*/

#define ZONEERR_PAK135 541
/* @@error 541 ������λ����������*/

#define ZONEERR_PAK136 542
/* @@error 542 û�о������*/

#define ZONEERR_PAK137 543
/* @@error 543 ����Ʒ�Ѿ���Ч,���Ժ�ʹ��*/

#define ZONEERR_PAK138 544
/* @@error 544 ���ߵ�buffЧ��Ŀǰ������Ч����ʱ����ʹ�øõ���*/

#define ZONEERR_PAK139 545
/* @@error 545 ״̬�����ѵ��������޷�������*/

#define ZONEERR_PAK140 546
/* @@error 546 ��̯״̬�²���ʹ�ô˵���*/

#define ZONEERR_PAK141 547
/* @@error 547 ��ǰ��������ʹ�ô˵���*/

#define ZONEERR_PAK142 548
/* @@error 548 ��ǰ����ʹ�ô˵���*/

#define ZONEERR_PAK143 549
/* @@error 549 ��̲��̫ӵ��*/

#define ZONEERR_PAK144 550
/* @@error 550 ֻ���ں�̲��ʹ��*/

#define ZONEERR_PAK145 551
/* @@error 551 ��Ŀǰû��ɱ��ֵ,����ʹ�ô˵���*/

#define ZONEERR_PAK146 552
/* @@error 552 �����ڴ˵�ͼ��ʹ�ø���Ʒ*/

#define ZONEERR_PAK147 553
/* @@error 553 ֻ����ָ��������ʹ�ø���Ʒ*/

#define ZONEERR_PAK148 554
/* @@error 554 ��ǰ״̬����ʹ�õ���*/

#define ZONEERR_PAK149 555
/* @@error 555 �˵��߲����ڿ��ս��ʹ��*/

#define ZONEERR_PAK150 556
/* @@error 556 �˸�������ʹ�ûָ��Ե���*/

#define ZONEERR_PAK151 557
/* @@error 557 ��Ʒʹ�����޵��ڣ��޷�ʹ��*/

#define ZONEERR_PAK152 558
/* @@error 558 ��ɫ���޸ı�����7��*/

#define ZONEERR_PAK153 559
/* @@error 559 ��Ʒÿ��ʹ�ô����Ѵ����ޣ��޷�ʹ��*/

#define ZONEERR_PAK154 560
/* @@error 560 �ȼ�������,����תְ!*/

#define ZONEERR_PAK155 561
/* @@error 561 ���鲻��,����תְ!*/

#define ZONEERR_PAK156 562
/* @@error 562 ս��״̬������ʹ�ô���Ʒ*/

#define ZONEERR_PAK157 563
/* @@error 563 ������е������ʹ�ô���Ʒ*/

#define ZONEERR_PAK158 564
/* @@error 564 ����Ʒ������ͨ*/

#define ZONEERR_PAK159 565
/* @@error 565 %s ����ʹ����*/

#define ZONEERR_PAK160 566
/* @@error 566 %s ʹ����Ч���ѵ�*/

#define ZONEERR_PAK161 567
/* @@error 567 ��Ʒ %s ������ͨ*/

#define ZONEERR_PAK162 568
/* @@error 568 ��Ʒ %s ��ί����Ʒ,������ͨ*/

#define ZONEERR_PAK163 569
/* @@error 569 ��ǰ��������ʹ�ã����ܶ���*/

#define ZONEERR_PAK164 570
/* @@error 570 �㶪����%s  */

#define ZONEERR_PAK165 571
/* @@error 571 ����̫Զ��*/

#define ZONEERR_PAK166 572
/* @@error 572 ���ڶ������޷���������*/

#define ZONEERR_PAK167 573
/* @@error 573 ������������*/

#define ZONEERR_PAK168 574
/* @@error 574 ����ͬһ��������*/

#define ZONEERR_PAK169 575
/* @@error 575 ����ͬһ���������޷�����Է�*/

#define ZONEERR_PAK170 576
/* @@error 576 �Է���ǰ״̬�޷�������*/

#define ZONEERR_PAK171 577
/* @@error 577 ���Ѿ����������˵���������*/

#define ZONEERR_PAK172 578
/* @@error 578 ��ǰ״̬�޷������������*/

#define ZONEERR_PAK173 579
/* @@error 579 ����ͬһ���������޷�����*/

#define ZONEERR_PAK174 580
/* @@error 580 �Է�û�����*/

#define ZONEERR_PAK175 581
/* @@error 581 �����Գ���*/

#define ZONEERR_PAK176 582
/* @@error 582 ��������̫Զ�ˣ��߽�һЩ��*/

#define ZONEERR_PAK177 583
/* @@error 583 ȱ����������*/

#define ZONEERR_PAK178 584
/* @@error 584 %s���㣬�޷�������������*/

#define ZONEERR_PAK179 585
/* @@error 585 �޴�����*/

#define ZONEERR_PAK180 586
/* @@error 586 �������Ѿ�������ߵȼ��������ٽ���ǿ��*/

#define ZONEERR_PAK181 587
/* @@error 587 ��ϲ�����%s�ɹ�����%d��*/

#define ZONEERR_PAK182 588
/* @@error 588 �ܿ�ϧ��%sδ�������ɹ����ٶȲ���*/

#define ZONEERR_PAK183 589
/* @@error 589 %s �ܾ�����ĳ�������*/

#define ZONEERR_PAK184 590
/* @@error 590 %s ��������ĳ�������*/

#define ZONEERR_PAK185 591
/* @@error 591 %s �ܾ�����ĳ�������*/

#define ZONEERR_PAK186 592
/* @@error 592 %s ��������ĳ�������*/

#define ZONEERR_PAK187 593
/* @@error 593 �㲻������ô��*/

#define ZONEERR_PAK188 594
/* @@error 594 %s �����뿪����������*/

#define ZONEERR_PAK189 595
/* @@error 595 �޴�����*/

#define ZONEERR_PAK190 596
/* @@error 596 %s����*/

#define ZONEERR_PAK191 597
/* @@error 597 �������������滻��������*/

#define ZONEERR_PAK192 598
/* @@error 598 û��װ����������,����װ����������*/

#define ZONEERR_PAK193 599
/* @@error 599 ��Ʒ�Ѿ�����*/

#define ZONEERR_PAK194 600
/* @@error 600 ���ܰѵ��߽�����ʧЧ����λ��*/

#define ZONEERR_PAK195 601
/* @@error 601 ��ȷ����Ҫɾ������λ��û����Ʒ*/

#define ZONEERR_PAK196 602
/* @@error 602 ���ܽ���Ʒ�ƶ�����Ҫɾ������λ��*/

#define ZONEERR_PAK197 603
/* @@error 603 ������λ����%d��*/

#define ZONEERR_PAK198 604
/* @@error 604 ������λ����%d��*/

#define ZONEERR_PAK199 605
/* @@error 605 �ɹ�������һ��������λ*/

#define ZONEERR_PAK200 606
/* @@error 606 ����������λʧ��*/

#define ZONEERR_PAK201 607
/* @@error 607 ����[%s]������µļ�����λ */

#define ZONEERR_PAK202 608
/* @@error 608 ���ĳ���%s���ڳ�ս״̬�����ܽ��м���������� */

#define ZONEERR_PAK203 609
/* @@error 609 ����%s�����������Ѵﵽ����*/

#define ZONEERR_PAK204 610
/* @@error 610 ��ǰ%s����*/

#define ZONEERR_PAK205 611
/* @@error 611 ����[%s]�����%d����*/

#define ZONEERR_PAK206 612
/* @@error 612 ����[%s]��������ֵ%d*/

#define ZONEERR_PAK207 613
/* @@error 613 %s�����ﵽ����*/

#define ZONEERR_PAK208 614
/* @@error 614 %s ����Ч���ѵ��ڣ��޷�ʹ��*/

#define ZONEERR_PAK209 615
/* @@error 615 װ��:%s,id:%d,����:%d*/

#define ZONEERR_PAK210 616
/* @@error 616 %s ʹ����Ч���ѵ�����ע��鿴*/

#define ZONEERR_PAK211 617
/* @@error 617 %s ʹ����Ч��С��10 ����*/

#define ZONEERR_PAK212 618
/* @@error 618 %s ʹ����Ч��С��3 Сʱ*/

#define ZONEERR_PAK213 619
/* @@error 619 %s ʹ����Ч��С��24 Сʱ*/

#define ZONEERR_PAK214 620
/* @@error 620 �㲻����ѧϰ����䷽*/

#define ZONEERR_PAK215 621
/* @@error 621 ��ļ��ܵȼ�����,�޷�ѧϰ*/

#define ZONEERR_PAK216 622
/* @@error 622 ���Ѿ�ѧ����%s*/

#define ZONEERR_PAK217 623
/* @@error 623 ��������,���ܽ��в�׽*/

#define ZONEERR_PAK218 624
/* @@error 624 ��׽%sʧ��*/

#define ZONEERR_PAK219 625
/* @@error 625 ��ǰ״̬���޷�ʹ��*/

#define ZONEERR_PAK220 626
/* @@error 626 �㲻�ڶ�����*/

#define ZONEERR_PAK221 627
/* @@error 627 ���������Ա�������߻򲻷�������*/

#define ZONEERR_PAK222 628
/* @@error 628 �ڸ������޷�ʹ��*/

#define ZONEERR_PAK223 629
/* @@error 629 δ֪��ʹ�÷�ʽ*/

#define ZONEERR_PAK224 630
/* @@error 630 �����ʹ�÷�ʽ*/

#define ZONEERR_PAK225 631
/* @@error 631 �Ѿ��ﵽ��¼��������*/

#define ZONEERR_PAK226 632
/* @@error 632 �����λ������*/

#define ZONEERR_PAK227 633
/* @@error 633 ��Ϊ�ȼ�����,���޷����͵�Ŀ���ͼ*/

#define ZONEERR_PAK228 634
/* @@error 634 �ӳ�������,�޷�����*/

#define ZONEERR_PAK229 635
/* @@error 635 �ӳ��ڸ�����,�޷�����*/

#define ZONEERR_PAK230 636
/* @@error 636 ���Ƕӳ�,����Ҫʹ�ô���Ʒ*/

#define ZONEERR_PAK231 637
/* @@error 637 ��ǰ״̬���޷�����*/

#define ZONEERR_PAK232 638
/* @@error 638 ��Ϊ�ȼ�����,���޷����͵�Ŀ���ͼ*/

#define ZONEERR_PAK233 639
/* @@error 639 �޷����͵��ӳ����ڵĵ�ͼ*/

#define ZONEERR_PAK234 640
/* @@error 640 �����������λ����*/

#define ZONEERR_PAK235 641
/* @@error 641 ����������λ����*/

#define ZONEERR_PAK236 642
/* @@error 642 ����������λʧ��,�����%d���۴�����*/

#define ZONEERR_PAK237 643
/* @@error 643 �����������λ�Ѿ�ȫ������*/

#define ZONEERR_PAK238 644
/* @@error 644 ����������λ�Ѿ�ȫ������*/

#define ZONEERR_PAK239 645
/* @@error 645 �밴˳����������λ*/

#define ZONEERR_PAK240 646
/* @@error 646 ����ֵ��������ʱ����Ҫ�ظ�*/

#define ZONEERR_PAK241 647
/* @@error 647 ������λ����%d��*/

#define ZONEERR_PAK242 648
/* @@error 648 ������λ����%d��*/

#define ZONEERR_PAK243 649
/* @@error 649 ħ��ֵ��������ʱ����Ҫ�ظ�*/

#define ZONEERR_PAK244 650
/* @@error 650 ���Ѿ�ӵ������ƺ�*/

#define ZONEERR_PAK245 651
/* @@error 651 ��ƺ������ﵽ����*/

#define ZONEERR_PAK246 652
/* @@error 652 û�����������ľ���*/

#define ZONEERR_PAK247 653
/* @@error 653 ����ȼ�����*/

#define ZONEERR_PAK248 654
/* @@error 654 ����Ʒ������ȴ��*/

#define ZONEERR_PAK249 655
/* @@error 655 ��ʹ������Ʒ %s*/

#define ZONEERR_PAK250 656
/* @@error 656 %s ����ʹ������Ʒ %s*/

#define ZONEERR_PAK251 657
/* @@error 657 ��� %s ʹ������Ʒ %s*/

#define ZONEERR_PAK252 658
/* @@error 658 ��Ų�����ô��%s*/

#define ZONEERR_PAK253 659
/* @@error 659 ������᲻�����������λ��װ��*/

#define ZONEERR_PAK254 660
/* @@error 660 ʱװ���ܿ�ӡ*/

#define ZONEERR_PAK255 661
/* @@error 661 ��ָ������Ʒ����һ��װ��*/

#define ZONEERR_PAK256 662
/* @@error 662 װ���ȼ����ܵ���%d��*/

#define ZONEERR_PAK257 663
/* @@error 663 ��ָ������Ʒ���Ǹ�ħ����*/

#define ZONEERR_PAK258 664
/* @@error 664 ��ָ������Ʒ����һ��װ��*/

#define ZONEERR_PAK259 665
/* @@error 665 �����Ʒ�ĵȼ�û�дﵽ�˸�ħҪ�����С�ȼ�(Lv.%d)*/

#define ZONEERR_PAK260 666
/* @@error 666 �����Ʒ�ĵȼ��Ѿ����ڴ˸�ħҪ������ȼ�(Lv.%d)*/

#define ZONEERR_PAK261 667
/* @@error 667 ����ر�ͼʹ�ô����Ѿ��ﵽ%d������*/

#define ZONEERR_PAK262 668
/* @@error 668 %s ��������%d ��*/

#define ZONEERR_PAK263 669
/* @@error 669 ��Ҫ�����츳�ﵽ%d��*/

#define ZONEERR_COMBAT1 669
/* @@error 669 �������޷��ڵ�ǰ��ͼ�����*/

#define ZONEERR_GAY1 670
/* @@error 670 ����Ҫӵ��һ�����飬���ұ����Ƕӳ�*/

#define ZONEERR_GAY2 671
/* @@error 671 ȱ����Ʒ��%s�����ʧ��*/

#define ZONEERR_GAY3 672
/* @@error 672 �����������ѷ��ͣ������ĵȴ���Ӧ*/

#define ZONEERR_GAY4 673
/* @@error 673 ��ǰ��Ұ��û�ж��ѣ�������뷢��ʧ��*/

#define ZONEERR_GAY5 674
/* @@error 674 %s��Ӧ���㷢�͵�������볬ʱ�����Ժ�����*/

#define ZONEERR_GAY6 675
/* @@error 675 %s�ܾ���������룬���ʧ��*/

#define ZONEERR_GAY7 676
/* @@error 676 �������뿪�˶��飬���ʧ��*/

#define ZONEERR_GAY8 677
/* @@error 677 �����������ʧЧ*/

#define ZONEERR_GAY9 678
/* @@error 678 ������ң�%sȱ����Ʒ��%s�����ʧ��*/

#define ZONEERR_GAY10 679
/* @@error 679 %s������������룬��Գɹ�*/

#define ZONEERR_GAY11 680
/* @@error 680 �������%s�����룬��Գɹ�*/

#define ZONEERR_MACHINE1 681
/* @@error 681 ��ǰ״̬�޷����*/

#define ZONEERR_MACHINE2 682
/* @@error 682 ��ǰ״̬�²�������г����е����*/

#define ZONEERR_MACHINE3 683
/* @@error 683 �˵�ͼ������˻�е*/

#define ZONEERR_MACHINE4 684
/* @@error 684 ��ǰ��ͼ������˴˻�е*/

#define ZONEERR_MACHINE5 685
/* @@error 685 ��ǰ��λ����*/

#define ZONEERR_MACHINE6 686
/* @@error 686 ��ǰ��е�����Ըı���λ*/

#define ZONEERR_MACHINE7 687
/* @@error 687 ��ǰ״̬�޷�������е*/

#define ZONEERR_MACHINE8 688
/* @@error 688 ��е�����ľ���̫Զ���޷�����*/

#define ZONEERR_MACHINE9 689
/* @@error 689 ���Ѿ����������˵Ļ�е����*/

#define ZONEERR_MACHINE10 690
/* @@error 690 �Է�û����˻�е*/

#define ZONEERR_MACHINE11 691
/* @@error 691 �ϳ�ʧ�ܣ��û�е������������*/

#define ZONEERR_MACHINE12 692
/* @@error 692 ���˻�е��������*/

#define ZONEERR_MACHINE13 693
/* @@error 693 ��е���������������޷�����*/

#define ZONEERR_MACHINE14 694
/* @@error 694 ����̫Զ��*/

#define ZONEERR_MACHINE15 695
/* @@error 695 %s �ܾ�����ĳ�������*/

#define ZONEERR_MACHINE16 696
/* @@error 696 %s ��������ĳ�������*/

#define ZONEERR_MACHINE17 697
/* @@error 697 ��ǰ״̬�޷����*/

#define ZONEERR_MACHINE18 698
/* @@error 698 ��ǰ״̬�޷�������е*/

#define ZONEERR_MACHINE19 699
/* @@error 699 %s�Ѿ��������Ļ�е������ͬ��ս*/

#define ZONEERR_MACHINE20 700
/* @@error 700 ��ǰ�������ܸ���*/

#define ZONEERR_MACHINE21 701
/* @@error 701 �������ڸ���,�޷����л���*/

#define ZONEERR_MACHINE22 702
/* @@error 702 ȱ�ٵ���:%s */

#define ZONEERR_MACHINE23 703
/* @@error 703 �������޻���,��Ҫ֧����Ӧ�����Ļ�����ͬ*/

#define ZONEERR_MACHINE24 704
/* @@error 704 %s�����뿪�����Ļ�е*/

#define ZONEERR_MACHINE25 705
/* @@error 705 %s�뿪�����Ļ�е*/

#define ZONEERR_MACHINE26 706
/* @@error 706 �˻�е���𻵣��޷�ʹ��*/

#define ZONEERR_MACHINE27 707
/* @@error 707 ����δʹ�òɼ����ɼ����޷���ȡ��Դ*/

#define ZONEERR_MACHINE28 708
/* @@error 708 ����û���빫�ᣬ�޷�ʹ�ø���Ʒ*/

#define ZONEERR_MACHINE29 709
/* @@error 709 ռ��ʧ�ܣ�%s�ѱ�ռ��*/

#define ZONEERR_MACHINE30 710
/* @@error 710 ����ʧ�ܣ��ɼ���ֻ����10��00-24��00֮�佨��*/

#define ZONEERR_MACHINE31 711
/* @@error 711 �õ�����Ҫ�����е�����ʹ��*/

#define ZONEERR_MACHINE32 712
/* @@error 712 ���Ļ�еĿǰ����ָ�����*/

#define ZONEERR_MACHINE33 713
/* @@error 713 ���Ļ�еĿǰ�����������*/

#define ZONEERR_WEATHER1 714
/* @@error 714 Ȩ�޲���,ֻ��ʱ��ǳ����ſɽ��д˲���*/

#define ZONEERR_WEATHER2 715
/* @@error 715 �ѳɹ���ʱ�����������Ϊ%s!*/

#define ZONEERR_VIP1 716
/* @@error 716 ���겻����,����ʧ��*/

#define ZONEERR_VIP2 717
/* @@error 717 ����%s���㣬���ι���ʧ��*/

#define ZONEERR_VIP3 718
/* @@error 718 ��ͨ %s ʧ����*/

#define ZONEERR_VAL1 719
/* @@error 719 �������������10w����*/

#define ZONEERR_MAIL101 720
/* @@error 720 ��û���㹻��%s�����ʼ�*/

#define ZONEERR_MAIL102 721
/* @@error 721 ���İ�����û���κ���Ʒ*/

#define ZONEERR_MAIL103 722
/* @@error 722 �����ʼ�����ĵ�����������*/

#define ZONEERR_MAIL104 723
/* @@error 723 ��û��ѡ�����ڷ����ʼ����ʼ�����*/

#define ZONEERR_MAIL105 724
/* @@error 724 ��ѡ������Ʒ�����ʼ�����*/

#define ZONEERR_MAIL106 725
/* @@error 725 �����ʼ�������ʼ�������������*/

#define ZONEERR_MAIL107 726
/* @@error 726 ��������æ���ʼ�����ʧ�ܣ����Ժ�����*/

#define ZONEERR_MAIL108 727
/* @@error 727 �Ҳ����ռ��ˣ��ʼ�����ʧ��*/

#define ZONEERR_MAIL109 728
/* @@error 728 �ʼ�ɾ��ʧ��*/

#define ZONEERR_MAIL110 729
/* @@error 729 �������ռ�������*/

#define ZONEERR_MAIL111 730
/* @@error 730 �������ʼ�����*/

#define ZONEERR_MAIL112 731
/* @@error 731 ���ܸ��Լ������ʼ�*/

#define ZONEERR_MAIL113 732
/* @@error 732 �ռ�������ĺ������У����ܷ����ʼ�*/

#define ZONEERR_MAIL114 733
/* @@error 733 �ռ���������޵������У����ܷ����ʼ�*/

#define ZONEERR_MAIL115 734
/* @@error 734 ���ڶԷ��������У����ܷ����ʼ�*/

#define ZONEERR_MAIL116 735
/* @@error 735 �����ռ��˵��޵У����ܷ����ʼ�*/

#define ZONEERR_MAIL117 736
/* @@error 736 �ռ��������������ʼ�����ʧ��*/

#define ZONEERR_MAIL118 737
/* @@error 737 ��������,����ʹ���ʼ�*/

#define ZONEERR_TRANS101 738
/* @@error 738 ֻ��ʱ����������Ա����ʹ�ô���·*/

#define ZONEERR_TRANS102 739
/* @@error 739 �ȼ��ﵽ%d����ʹ�ô���·*/

#define ZONEERR_TRANS103 740
/* @@error 740 �˵�ͼ̫Σ����,���ǵ�%d���Ժ���ȥ��*/

#define ZONEERR_TMP101 741
/* @@error 741 ��ǰ���߲������*/

#define ZONEERR_TMP102 742
/* @@error 742 ��ǰ��������ʱ��е*/

#define ZONEERR_TEAM101 743
/* @@error 743 �������Լʱ����̫��*/

#define ZONEERR_TEAM102 744
/* @@error 744 ����״̬�������*/

#define ZONEERR_TEAM103 745
/* @@error 745 �Է�������״̬*/

#define ZONEERR_TEAM104 746
/* @@error 746 Ŀ�����ж���,�޷������������*/

#define ZONEERR_TEAM105 747
/* @@error 747 Ŀ��������޵�������,�޷����,�밴F�����޵������鿴*/

#define ZONEERR_TEAM106 748
/* @@error 748 ������������*/

#define ZONEERR_TEAM107 749
/* @@error 749 �ӳ�������,���Ժ�����*/

#define ZONEERR_TEAM108 750
/* @@error 750 �Է��Ѿ�����*/

#define ZONEERR_TEAM109 751
/* @@error 751 �Է����״̬�������*/

#define ZONEERR_TEAM110 752
/* @@error 752 �㲻�Ƕӳ��޷�����%s�������*/

#define ZONEERR_TEAM111 753
/* @@error 753 Ŀ������ĺ�������,�޷����,�밴F���򿪺������鿴*/

#define ZONEERR_TEAM112 754
/* @@error 754 Ŀ��������޵�������,�޷����,�밴F�����޵������鿴*/

#define ZONEERR_TEAM113 755
/* @@error 755 ��ӳɹ�����Ϊ�ӳ�������ģʽΪ[%s] */

#define ZONEERR_TEAM114 756
/* @@error 756 ������飬�ӳ�Ϊ%s������ģʽΪ[%s] */

#define ZONEERR_TEAM115 757
/* @@error 757 %s �ܾ�������������*/

#define ZONEERR_TEAM116 758
/* @@error 758 %s �Ѿ�������������*/

#define ZONEERR_TEAM117 759
/* @@error 759 ��� %s �����������Ӧ��,���Ժ�����*/

#define ZONEERR_TEAM118 760
/* @@error 760 ת�õĶ�������*/

#define ZONEERR_TEAM119 761
/* @@error 761 ���Ѿ��Ƕӳ�*/

#define ZONEERR_TEAM120 762
/* @@error 762 ��ǰ����ͶƱ����,���Ժ�����*/

#define ZONEERR_TEAM121 763
/* @@error 763 ��������ֻ�ܷ���һ��ͶƱ����,���Ժ�����*/

#define ZONEERR_TEAM122 764
/* @@error 764 �޴���Լ*/

#define ZONEERR_TEAM123 765
/* @@error 765 ���Ѿ����͹����룬�ȴ��Է��ظ�*/

#define ZONEERR_TEAM124 766
/* @@error 766 ���Ѿ����͹����룬���Ժ�����*/

#define ZONEERR_TEAM125 767
/* @@error 767 ���ڶԷ���������*/

#define ZONEERR_TEAM126 768
/* @@error 768 ���ڶԷ��޵�������*/

#define ZONEERR_TEAM127 769
/* @@error 769 �㲻�Ƕӳ�*/

#define ZONEERR_TEAM128 770
/* @@error 770 ���뿪�˶���*/

#define ZONEERR_TEAM129 771
/* @@error 771 %s δ���������*/

#define ZONEERR_TEAM130 772
/* @@error 772 ��ͼ�ڴ�����ҪROLL���װ������ROLL����ٷ���ͶƱ*/

#define ZONEERR_TEAM131 773
/* @@error 773 ���ܿ����Լ�*/

#define ZONEERR_TEAM132 774
/* @@error 774 ��ͬ����Ҳ������*/

#define ZONEERR_TEAM133 775
/* @@error 775 ��Ա:%s Ŀǰ���ڲ�����Ӹ���״̬*/

#define ZONEERR_TEAM134 776
/* @@error 776 ��ǰ״̬����Ҫ��������*/

#define ZONEERR_TEAM135 777
/* @@error 777 ��Ӹ���ʧ��*/

#define ZONEERR_TEAM136 778
/* @@error 778 �ӳ����ڲ��ɸ���״̬*/

#define ZONEERR_TEAM137 779
/* @@error 779 �����Ա %s ����ʹ�ô���·*/

#define ZONEERR_TEAM138 780
/* @@error 780 �����Ա %s ���ܽ���˵�ͼ*/

#define ZONEERR_TEAM139 781
/* @@error 781 �����ٻ�ʧЧ*/

#define ZONEERR_TEAM140 782
/* @@error 782 �뿪�ٻ�����ս��������ʧ��*/

#define ZONEERR_TEAM141 783
/* @@error 783 �㲻�ڶ�����*/

#define ZONEERR_TEAM142 784
/* @@error 784 ��Ͷ����ٻ������߲���ͬһ��������*/

#define ZONEERR_TEAM143 785
/* @@error 785 �Ѿ������˶����ٻ�����Чʱ��*/

#define ZONEERR_TEAM144 786
/* @@error 786 ��ǰ״̬���޷�����*/

#define ZONEERR_TEAM145 787
/* @@error 787 ��Ϊ�ȼ�����,���޷����͵�Ŀ���ͼ*/

#define ZONEERR_RONGLU1 788
/* @@error 788 �뵽 ָ����ͼ ʹ�ô˵���*/

#define ZONEERR_RONGLU2 789
/* @@error 789 ��¯��������,�������·���*/

#define ZONEERR_RONGLU3 790
/* @@error 790 ����ʹ�ô���¯*/

#define ZONEERR_RONGLU4 791
/* @@error 791 ħ����¯�����������ɲɼ���ñ���*/

#define ZONEERR_RONGLU5 792
/* @@error 792 ��¯�ѷ��ã���������*/

#define ZONEERR_RONGLU6 793
/* @@error 793 ��ǰû�п���������¯*/

#define ZONEERR_RESPACT1 794
/* @@error 794 ����%s�е�����������%u��*/

#define ZONEERR_RESPACT2 795
/* @@error 795 ����%s�е�����������%u��(��������,�������50��)��*/

#define ZONEERR_RESPACT3 796
/* @@error 796 ����%s�е������Ѿ��ﵽ%s */

#define ZONEERR_SECSKILL1 797
/* @@error 797 ȱ���������*/

#define ZONEERR_SECSKILL2 798
/* @@error 798 ��ļ��ܵȼ�����,�޷�ѧϰ*/

#define ZONEERR_SECSKILL3 799
/* @@error 799 ���Ѿ�ѧ����*/

#define ZONEERR_NOHUOBI 800
/* @@error 800 ������Ҳ���*/

#define ZONEERR_SECSKILL4 801
/* @@error 801 %s������ȴ��*/

#define ZONEERR_SECSKILL5 802
/* @@error 802 ��δѧ������Ƽ�����רҵ*/

#define ZONEERR_SECSKILL6 803
/* @@error 803 �����Դ����ѧϰ*/

#define ZONEERR_SECSKILL7 804
/* @@error 804 �����Ʒ���ɱ��ֽ�*/

#define ZONEERR_SECSKILL8 805
/* @@error 805 ��û������Ƽ�����רҵ*/

#define ZONEERR_MISC1 806
/* @@error 806 �Է�����ָ�����д跶Χ*/

#define ZONEERR_MISC2 807
/* @@error 807 ���Ѿ����д�״̬��*/

#define ZONEERR_MISC3 808
/* @@error 808 �Է��Ѿ����д�״̬��*/

#define ZONEERR_MISC4 809
/* @@error 809 �Է���̯״̬�²����д�*/

#define ZONEERR_MISC5 810
/* @@error 810 �Է�����״̬�²����д�*/

#define ZONEERR_MISC6 811
/* @@error 811 �Է�����˫״̬��,�����д�*/

#define ZONEERR_MISC7 812
/* @@error 812 �Է��Ѿ�����*/

#define ZONEERR_MISC107 813
/* @@error 813 ����̫Զ,�޷��д�*/

#define ZONEERR_MISC8 814
/* @@error 814 �д������Ѿ�ȡ��*/

#define ZONEERR_MISC9 815
/* @@error 815 %s ͬ��������д�����,3���ʼ�д�*/

#define ZONEERR_MISC10 816
/* @@error 816 3���ʼ�д�*/

#define ZONEERR_MISC11 817
/* @@error 817 %s �ܾ�������д�����*/

#define ZONEERR_MISC12 818
/* @@error 818 �˵ز������д�*/

#define ZONEERR_MISC13 819
/* @@error 819 ��̯״̬�²����д�*/

#define ZONEERR_MISC14 820
/* @@error 820 ����״̬�²����д�*/

#define ZONEERR_MISC15 821
/* @@error 821 ����״̬�������д�*/

#define ZONEERR_MISC16 822
/* @@error 822 ��˫״̬�²����д�*/

#define ZONEERR_MISC17 823
/* @@error 823 �д�ȡ��*/

#define ZONEERR_MISC18 824
/* @@error 824 �д�ʧ��*/

#define ZONEERR_MISC19 825
/* @@error 825 �д�ʤ��*/

#define ZONEERR_MISC20 826
/* @@error 826 �д迪ʼ*/

#define ZONEERR_MISC21 827
/* @@error 827 �����뿪�д�����,5���ʧ��*/

#define ZONEERR_MISC22 828
/* @@error 828 �����ͻ������ڽ������Ʋ���,���Ժ�����*/

#define ZONEERR_MISC23 829
/* @@error 829 û�ж�Ӧ�������Ϣ������ѹ���*/

#define ZONEERR_MISC24 830
/* @@error 830 ���ݸ���ʧ��,�޷���ɴ˲���*/

#define ZONEERR_UNKNOWN 831
/* @@error 831 δ֪����*/

#define ZONEERR_MISC25 832
/* @@error 832 ��ǰ���ܳ齱*/

#define ZONEERR_MISC26 833
/* @@error 833 ����齱�����Ѿ�����*/

#define ZONEERR_MISC27 834
/* @@error 834 ��ϲ��!ͨ���齱���%d�� %s*/

#define ZONEERR_ARM201 835
/* @@error 835 ��װ����δ����,���ܽ��д˲���*/

#define ZONEERR_BOOTY201 836
/* @@error 836 ���%s���ϳ�ȡ��%d�����*/

#define ZONEERR_BUSY201 837
/* @@error 837 �������ڽ����в���ֱ�����*/

#define ZONEERR_BUSY202 838
/* @@error 838 δ���빫���޷�����*/

#define ZONEERR_BUSY203 839
/* @@error 839 �������ÿ����ɴ����Ѵ�����*/

#define ZONEERR_BUSY204 840
/* @@error 840 %s���������Ѿ��ڽ����У�ֻ��δ���еĻ������ֱ�����*/

#define ZONEERR_BUSY205 841
/* @@error 841 ����ֻ��ֱ����ɵ�һ��*/

#define ZONEERR_BUSY206 842
/* @@error 842 ���Ѿ��ڸ����У�������ò���*/

#define ZONEERR_BUSY207 843
/* @@error 843 ������ɴ����Ѵﵽ��������*/

#define ZONEERR_BUSY208 844
/* @@error 844 ���Ѿ��ں�̲��������ò���*/

#define ZONEERR_BUSY209 845
/* @@error 845 ���������ù���̲ʱ�䣬ֻ��δ�ù���̲ʱ��ſ���ֱ�����*/

#define ZONEERR_BUSY210 846
/* @@error 846 ���������������������*/

#define ZONEERR_BUSY211 847
/* @@error 847 ��%s�������*/

#define ZONEERR_CITY201 848
/* @@error 848 �����ʽ��㣬�޷�����*/

#define ZONEERR_CITY202 849
/* @@error 849 ָ���Ĳ��ϲ�����,�޷�����*/

#define ZONEERR_CITY203 850
/* @@error 850 ȱ�ٲ��� %s ���޷�����*/

#define ZONEERR_CITY204 851
/* @@error 851 �ܵ��ȼ���ӡӰ�죬���޷�������2������*/

#define ZONEERR_CITY205 852
/* @@error 852 �ܵ��ȼ���ӡӰ�죬���޷�������3������*/

#define ZONEERR_CITY206 853
/* @@error 853 �����ʽ��㣬�޷�����*/

#define ZONEERR_CITY207 854
/* @@error 854 �����ʽ��㣬�޷�����*/

#define ZONEERR_CITY208 855
/* @@error 855 ����ʧ�ܣ���ǰ����ֻ���ڷ�ս���ڼ�ſɽ���*/

#define ZONEERR_CITY209 856
/* @@error 856 ֻ�й���᳤�븱�᳤�д˲���Ȩ��*/

#define ZONEERR_CITY210 857
/* @@error 857 �������ѳ��ݻ��ӹ������ˣ�����������*/

#define ZONEERR_CITY211 858
/* @@error 858 ���������������񣬻��δ����*/

#define ZONEERR_CITY212 859
/* @@error 859 �����³���δ�������˵��񣬻��ʱ�ر�*/

#define ZONEERR_CITY213 860
/* @@error 860 �ɹ���������ʱ��ǳ�������Ľ֮��*/

#define ZONEERR_CITY214 861
/* @@error 861 ���������������һö������*/

#define ZONEERR_CITY215 862
/* @@error 862 ����δ�������˵����޷���ȡ��������*/

#define ZONEERR_CITY216 863
/* @@error 863 ��ȡʧ�ܣ�ֻ�������Լ��ĵ���ſ���ȡ*/

#define ZONEERR_CITY217 864
/* @@error 864 ����������ȡ���������棬����������*/

#define ZONEERR_CITY218 865
/* @@error 865 �ɹ���ȡ���յ�������ֳɣ�%d����*/

#define ZONEERR_CLAN201 866
/* @@error 866 ���Ѿ�������һ������*/

#define ZONEERR_CLAN202 867
/* @@error 867 ��ո��˳����ɢ�˹��ᣬ��ȴ�%s���ٴ���*/

#define ZONEERR_CLAN203 868
/* @@error 868 ������: %s �Ѿ�����*/

#define ZONEERR_CLAN204 869
/* @@error 869 �����������Ѿ�ʧЧ*/

#define ZONEERR_CLAN205 870
/* @@error 870 ������: %s ����������Ѿ�ʧЧ*/

#define ZONEERR_CLAN206 871
/* @@error 871 ������: %s �Ѿ������˱�Ĺ���*/

#define ZONEERR_CLAN207 872
/* @@error 872 ����[%s] �Ѿ�������, ȡ��%s */

#define ZONEERR_CLAN208 873
/* @@error 873 ����[%s] �������ѳ�ʱ, ��%s�б���ɾ��*/

#define ZONEERR_CLAN209 874
/* @@error 874 �����ʽ��Ѵ����ޣ�������������*/

#define ZONEERR_CLAN210 875
/* @@error 875 ���ᰲ�����Ѵ����ޣ�������������*/

#define ZONEERR_CLAN211 876
/* @@error 876 ����Ƽ����Ѵ����ޣ�������������*/

#define ZONEERR_CLAN212 877
/* @@error 877 %s ���ǹ��� %s �ĳ�Ա*/

#define ZONEERR_CLAN213 878
/* @@error 878 ��ĸ��˹��׶�������%d */

#define ZONEERR_CLAN214 879
/* @@error 879 ��ĸ��˹��׶��Ѵ����ֵ*/

#define ZONEERR_CLAN215 880
/* @@error 880 ���Ǳ�Ǹ��֪ͨ��, ���������ڹ�����������,����%d���ڱ�ϵͳ��ɢ*/

#define ZONEERR_CLAN216 881
/* @@error 881 ͬ������Ҳſ��������*/

#define ZONEERR_CLAN217 882
/* @@error 882 ���Ѿ�������һ������,�޷��ٴ�������*/

#define ZONEERR_CLAN218 883
/* @@error 883 ���������Ѵﵽ����,�޷��ٴ�������*/

#define ZONEERR_CLAN219 884
/* @@error 884 ��ո��˳����ɢ�˹��ᣬ��ȴ�%s���ٴ���*/

#define ZONEERR_CLAN220 885
/* @@error 885 ��ĵȼ�δ��%d��,���ڻ����ܴ�������*/

#define ZONEERR_CLAN221 886
/* @@error 886 ��������ȱ�ٵ��ߣ���Ҫ%d��%s */

#define ZONEERR_CLAN222 887
/* @@error 887 ��ȱ�ٴ���������Ҫ�ĵ��� */

#define ZONEERR_CLAN223 888
/* @@error 888 ���%s����%d,��������ʧ��*/

#define ZONEERR_CLAN224 889
/* @@error 889 ����[%s] ��ͬ���б�����, �����������µĹ���*/

#define ZONEERR_CLAN225 890
/* @@error 890 ����[%s] ��ͬ���б�����, �����������µĹ���*/

#define ZONEERR_CLAN226 891
/* @@error 891 ����ĵж��б�����, ������������������ս*/

#define ZONEERR_CLAN227 892
/* @@error 892 ��ɹ�������һ������*/

#define ZONEERR_CLAN228 893
/* @@error 893 ��ɫ %s �Ѿ����ڹ�����*/

#define ZONEERR_CLAN229 894
/* @@error 894 �㻹û�м���һ������*/

#define ZONEERR_CLAN230 895
/* @@error 895 ���Ѿ������˹����ɢ����ʱ����%s���ٴ�ȷ�Ͻ�ɢ*/

#define ZONEERR_CLAN231 896
/* @@error 896 �޸Ĺ������Ƶ��,����%d������޸�*/

#define ZONEERR_CLAN232 897
/* @@error 897 ��ո��˳��˹��ᣬ�޷��ټ��빫�ᣬ��ȴ�%s������*/

#define ZONEERR_CLAN233 898
/* @@error 898 ���������Ĺ����Ѳ�����*/

#define ZONEERR_CLAN234 899
/* @@error 899 ����빫�� %s �������Ѿ�ʧЧ*/

#define ZONEERR_CLAN235 900
/* @@error 900 ���ѽ��빫�� %s ��Ԥ����Ա�б���*/

#define ZONEERR_CLAN236 901
/* @@error 901 �޷��ҵ���ɫ%s */

#define ZONEERR_CLAN237 902
/* @@error 902 %s�Ѿ������������Ĺ���*/

#define ZONEERR_CLAN238 903
/* @@error 903 ��ո��˳�����,�޷��ټ��빫��,��ȴ�%s������*/

#define ZONEERR_CLAN239 904
/* @@error 904 %s�ո��˳��˹���,�޷��ټ��빫��,��ȴ�%s������*/

#define ZONEERR_CLAN240 905
/* @@error 905 ͬ���б�����*/

#define ZONEERR_CLAN241 906
/* @@error 906 �Է��������˴��������*/

#define ZONEERR_CLAN242 907
/* @@error 907 �Է�����æ*/

#define ZONEERR_CLAN243 908
/* @@error 908 ָ���Ĺ����Ѿ�������*/

#define ZONEERR_CLAN244 909
/* @@error 909 ����[%s] ��ͬ���б�����*/

#define ZONEERR_CLAN245 910
/* @@error 910 ���򹫻�[%s] ����ͬ������*/

#define ZONEERR_CLAN246 911
/* @@error 911 ��ָ���Ĺ�����սCD������ȴ��*/

#define ZONEERR_CLAN247 912
/* @@error 912 ��Ҫ%d���Ҳ���������������ս*/

#define ZONEERR_CLAN248 913
/* @@error 913 ָ���Ĺ����Ѿ�������*/

#define ZONEERR_CLAN249 914
/* @@error 914 ����[%s] �ĵж��б�����*/

#define ZONEERR_CLAN250 915
/* @@error 915 ָ���Ĺ��᲻��ͬ���б���*/

#define ZONEERR_CLAN251 916
/* @@error 916 %s �ܾ�������������*/

#define ZONEERR_CLAN252 917
/* @@error 917 %s û����Ӧ����������*/

#define ZONEERR_CLAN253 918
/* @@error 918 %s �ܾ������ͬ������*/

#define ZONEERR_CLAN254 919
/* @@error 919 %s û����Ӧ���ͬ������*/

#define ZONEERR_CLAN255 920
/* @@error 920 ��ѯ�������,��ָ����׼ȷ�Ĺ�����*/

#define ZONEERR_CLAN256 921
/* @@error 921 ����[%s] �Ѿ������ѻ������������ѻ�ж�,�����޷��ټ��������б�*/

#define ZONEERR_CLAN257 922
/* @@error 922 ����[%s] �Ѿ������������б���*/

#define ZONEERR_CLAN258 923
/* @@error 923 ����[%s] �Ѿ���������, �������б����Ƴ�*/

#define ZONEERR_CLAN259 924
/* @@error 924 ����Ľ�������[%s] �����%s�ܾ���*/

#define ZONEERR_CLAN260 925
/* @@error 925 û��ռ�������Ӧ���ʽ�*/

#define ZONEERR_CLAN261 926
/* @@error 926 ֻ��ռ�칫��᳤��Ȩ��ȡռ���ʽ�*/

#define ZONEERR_CLAN262 927
/* @@error 927 �㲻��ʱ��ǳ��������Ա*/

#define ZONEERR_CLAN263 928
/* @@error 928 ����ʧ�ܣ�ֻ����ᳬ��2���ɫ�ſɽ��д˲���*/

#define ZONEERR_CLAN264 929
/* @@error 929 �����ռ���ʽ�����ȡ,�޷��ظ���ȡ*/

#define ZONEERR_CLAN265 930
/* @@error 930 û��ռ�������Ӧ���ʽ�*/

#define ZONEERR_CLAN266 931
/* @@error 931 �𾴵�ʱ����������ѳɹ���ȡ�����ռ���ʽ�%d%s */

#define ZONEERR_CLAN267 932
/* @@error 932 �Ҳ���ָ���Ĺ����Ա*/

#define ZONEERR_CLAN268 933
/* @@error 933 �������ƺ������дʻ�*/

#define ZONEERR_CLAN269 934
/* @@error 934 �㽫�������õ�̫����*/

#define ZONEERR_CLAN270 935
/* @@error 935 ���ܸ����Լ����θ߻���ͬ�������ù���*/

#define ZONEERR_CLAN271 936
/* @@error 936 ��û��Ȩ�޽��������õñ��Լ���ͬ�����*/

#define ZONEERR_CLAN272 937
/* @@error 937 һ������ֻ����һ���᳤*/

#define ZONEERR_CLAN273 938
/* @@error 938 һ������ֻ����һ�����᳤*/

#define ZONEERR_CLAN274 939
/* @@error 939 һ��ս��ֻ����һ���ų�*/

#define ZONEERR_CLAN275 940
/* @@error 940 ������ɫ����̫��*/

#define ZONEERR_CLAN276 941
/* @@error 941 ������ɫ���óɹ�*/

#define ZONEERR_CLAN277 942
/* @@error 942 ��Ч�ļ��ܻ�ȼ�*/

#define ZONEERR_CLAN278 943
/* @@error 943 �����ʽ���,�޷��з��˼���*/

//#define ZONEERR_CLAN279 944
/* @@error 944 ���Ἴ������,�޷��о�*/

#define ZONEERR_CLAN279 944
/* @@error 944 ���Ἴ���޷��з�*/

#define ZONEERR_CLAN280 945
/* @@error 945 �˼�����Ч�ȼ����з��ȼ���,�޷������з�*/

#define ZONEERR_CLAN281 946
/* @@error 946 �˼����޷��з�*/

#define ZONEERR_CLAN282 947
/* @@error 947 ���ỹû���з��˼���*/

#define ZONEERR_CLAN283 948
/* @@error 948 �˼����޷�ѧϰ������*/

#define ZONEERR_CLAN284 949
/* @@error 949 ��Ч�ļ��ܻ�ȼ�*/

#define ZONEERR_CLAN285 950
/* @@error 950 ���׶Ȳ���,�޷�ѧϰ�������˼���*/

#define ZONEERR_CLAN286 951
/* @@error 951 �˼����޷��ֶ�ѧϰ������*/

#define ZONEERR_CLAN287 952
/* @@error 952 ���Ἴ������,�޷�ѧϰ*/

#define ZONEERR_CLAN288 953
/* @@error 953 δѧϰ�Ĺ��Ἴ���޷�����*/

#define ZONEERR_CLAN289 954
/* @@error 954 ��Ҫѧϰ�˼��ܵ�ǰ�õȼ���������*/

#define ZONEERR_CLAN290 955
/* @@error 955 ��Ч�ĵ�ͼλ��*/

#define ZONEERR_CLAN291 956
/* @@error 956 �㲻�ܸ��Լ�����ս��*/

#define ZONEERR_CLAN292 957
/* @@error 957 ֻ����ͨ��Ա���Լ���ս��*/

#define ZONEERR_CLAN293 958
/* @@error 958 ���ɰ�����ս�ų�Ա�ӵ��Լ�ս��*/

#define ZONEERR_CLAN294 959
/* @@error 959 �õ�ͼ����ʹ�ô���Ʒ*/

#define ZONEERR_CLAN295 960
/* @@error 960 �����ٻ������߲�����,�ٻ�ʧ��*/

#define ZONEERR_CLAN296 961
/* @@error 961 �㲻�ڹ�����*/

#define ZONEERR_CLAN297 962
/* @@error 962 ��͹����ٻ������߲���ͬһ��������*/

#define ZONEERR_CLAN298 963
/* @@error 963 �Ѿ������˹����ٻ�����Чʱ��*/

#define ZONEERR_CLAN299 964
/* @@error 964 ��ǰ״̬���޷�����*/

#define ZONEERR_CLAN300 965
/* @@error 965 ��Ϊ�ȼ�����,���޷����͵�Ŀ���ͼ*/

#define ZONEERR_CLAN301 966
/* @@error 966 �㲻�ǻ᳤���޷��ٻ�*/

#define ZONEERR_CLAN302 967
/* @@error 967 Boss���ٻ����޷��ٴ��ٻ�*/

#define ZONEERR_CLAN303 968
/* @@error 968 δ������ʱ�䣬�޷��ٻ�*/

#define ZONEERR_CLAN304 969
/* @@error 969 ���Ѿ����������*/

#define ZONEERR_CLAN305 970
/* @@error 970 Bossδ�������޷���ȡ*/

#define ZONEERR_CLAN306 971
/* @@error 971 �����Ʒ[%s]�������ѱ�%s������%d��*/

#define ZONEERR_CLAN307 972
/* @@error 972 ����Ƽ�[%s]���з��ѱ�%s������%d��*/

#define ZONEERR_CLAN308 973
/* @@error 973 �����ʽ�������ɼ���, ����%d */

#define ZONEERR_CLAN309 974
/* @@error 974 �з��������, ������� */

#define ZONEERR_CLAN310 975
/* @@error 975 ����ȼ��Ѵ�Lv.%d���޷�������*/

#define ZONEERR_CLAN311 976
/* @@error 976 ��Ҫ���еȼ��ﵽLv.%d, ��������*/

#define ZONEERR_CLAN312 977
/* @@error 977 ������������Lv%d */

#define ZONEERR_CLAN313 978
/* @@error 978 ��ǰ����ȼ���(Lv.%d), ������еȼ����ܳ���%d */

#define ZONEERR_CLAN314 979
/* @@error 979 ��ǰ����ȼ���(Lv.%d), ���������빫�����*/

#define ZONEERR_CLAN315 980
/* @@error 980 ������и�������ʧ��*/

#define ZONEERR_CLAN316 981
/* @@error 981 �Ҳ���ָ���Ľ������޷������ȼ�*/

#define ZONEERR_CLAN317 982
/* @@error 982 �˽������ڽ�����... */

#define ZONEERR_CLAN318 983
/* @@error 983 ��ǰ����ȼ���(Lv.%d), �˽����ȼ����ܳ���%d */

#define ZONEERR_CLAN319 984
/* @@error 984 ��ǰ����ȼ���(Lv.%d), �����ܽ���˽��� */

#define ZONEERR_CLAN320 985
/* @@error 985 �˽���������������*/

#define ZONEERR_CLAN321 986
/* @@error 986 �㻹û�м���һ������*/

#define ZONEERR_CLAN322 987
/* @@error 987 ��Ĺ������ڻ�û�г���*/

#define ZONEERR_CLAN323 988
/* @@error 988 ������и���������*/

#define ZONEERR_CLAN324 989
/* @@error 989 ���빫����и���ʧ��*/

#define ZONEERR_CLAN331 990
/* @@error 990 ����Ҫ%d�㰲���Ȳſ���%s */

#define ZONEERR_CLAN325 991
/* @@error 991 ����Ҫ%d�����ʽ�ſ���%s */

#define ZONEERR_CLAN326 992
/* @@error 992 ����Ҫ%d�����ĿƼ���ſ���%s */

#define ZONEERR_CLAN327 993
/* @@error 993 ������Ʒ[%s]����, �޷�%s */

#define ZONEERR_CLAN328 994
/* @@error 994 ������Ʒ����, �޷�%s */

#define ZONEERR_CLAN329 995
/* @@error 995 ����%s�Ѳ�����������*/

#define ZONEERR_CLAN330 996
/* @@error 996 ���Ĺ�����Ҫ��Ͷ��%d%s�Ƽ��㣬�ſ�����%s */

#define ZONEERR_CLAN332 997
/* @@error 997 ���Ĺ�����Ҫ��Ͷ��%d%s�Ƽ��㣬�ſ�����%s */

#define ZONEERR_CLAN333 998
/* @@error 998 ����Ҫѧϰһ��%s�Ƽ����ܽ���%s */

#define ZONEERR_CLAN334 999
/* @@error 999 ������еȼ�����Ҫ�ﵽLv.%d, ��������%s */

#define ZONEERR_CLAN335 1000
/* @@error 1000 ������еȼ�����Ҫ�ﵽLv.%d, ���ܽ���%s */

#define ZONEERR_DESIGNATION101 1001
/* @@error 1001 ������һ���ƺ�[%s] */

#define ZONEERR_DESIGNATION102 1002
/* @@error 1002 �㲻��ӵ�и���ĳƺ��� */

#define ZONEERR_DESIGNATION103 1003
/* @@error 1003 ���Ѿ����д˳ƺ��� */

#define ZONEERR_DESIGNATION104 1004
/* @@error 1004 ����ƺŲ����� */

#define ZONEERR_DESIGNATION105 1005
/* @@error 1005 �ƺ�[%s] �ѹ���Ч��,ϵͳ�Զ�ɾ��*/

#define ZONEERR_DESIGNATION106 1006
/* @@error 1006 �㻹û������ƺ�,�޷�ʹ��*/

#define ZONEERR_DESIGNATION107 1007
/* @@error 1007 ��ľ���ֵ����,�޷�ʹ������ƺ�*/

#define ZONEERR_MAP101 1008
/* @@error 1008 �㲻�ǹ����Ա*/

#define ZONEERR_MAP102 1009
/* @@error 1009 ��Ա %s ���ǹ����Ա*/

#define ZONEERR_MAP103 1010
/* @@error 1010 ���ṱ�׶Ȳ���*/

#define ZONEERR_MAP104 1011
/* @@error 1011 ��Ա %s ���ṱ�׶Ȳ���*/

#define ZONEERR_MAP105 1012
/* @@error 1012 �������״̬�����ɽ���˸���*/

#define ZONEERR_MAP106 1013
/* @@error 1013 �˸��������ظ�����*/

#define ZONEERR_MAP107 1014
/* @@error 1014 �˸���Ҫ��ͬһ����*/

#define ZONEERR_MAP108 1015
/* @@error 1015 ���빫�ᵱ���޷��μӴ˻*/

#define ZONEERR_MAP109 1016
/* @@error 1016 ���������Ѵ�����*/

#define ZONEERR_MAP110 1017
/* @@error 1017 ��ѹر�*/

#define ZONEERR_MAP111 1018
/* @@error 1018 ��Ѿ���ʼ���޷����л���������´�׼ʱ�μ�*/

#define ZONEERR_MAP112 1019
/* @@error 1019 �μӸ������������࣬���Ժ�����*/

#define ZONEERR_MAP113 1020
/* @@error 1020 �˸���Ҫ�����*/

#define ZONEERR_MAP114 1021
/* @@error 1021 �㲻�Ƕӳ�*/

#define ZONEERR_MAP115 1022
/* @@error 1022 ����δ��������������*/

#define ZONEERR_MAP116 1023
/* @@error 1023 �����л����ˣ�����ʧ��*/

#define ZONEERR_MAP117 1024
/* @@error 1024 �������óɹ�*/

#define ZONEERR_MAP118 1025
/* @@error 1025 ��������,��Ʒͨ���ʼ�����,ע�����*/

#define ZONEERR_MAP119 1026
/* @@error 1026 ��������*/

#define ZONEERR_MAP120 1027
/* @@error 1027 �븴����ٽ���*/

#define ZONEERR_MAP121 1028
/* @@error 1028 ����״̬�����Խ���ս��*/

#define ZONEERR_MAP122 1029
/* @@error 1029 ��̯״̬�����Խ���ս��*/

#define ZONEERR_MAP123 1030
/* @@error 1030 ��̯״̬�������л���ͼ*/

#define ZONEERR_MAP124 1031
/* @@error 1031 ս��״̬�������л���ͼ*/

#define ZONEERR_MAP125 1032
/* @@error 1032 ����״̬�������л���ͼ*/

#define ZONEERR_MAP126 1033
/* @@error 1033 ����״̬�²������л���ͼ*/

#define ZONEERR_MAP127 1034
/* @@error 1034 �Զ�����״̬�²������л���ͼ*/

#define ZONEERR_MAP128 1035
/* @@error 1035 ����״̬�²������л���ͼ*/

#define ZONEERR_MAP129 1036
/* @@error 1036 ����ֵ����,�޷���������*/

#define ZONEERR_MAP130 1037
/* @@error 1037 ���� %s ��ɴ����Ѵﵽÿ����ɵĴ�������*/

#define ZONEERR_MAP131 1038
/* @@error 1038 ���� %s ��ɴ����Ѵﵽÿ����ɵĴ�������*/

#define ZONEERR_MAP132 1039
/* @@error 1039 �˸���%d������ֻ�ܽ�һ��*/

#define ZONEERR_MAP133 1040
/* @@error 1040 ��Ա %s �˸���%d������ֻ�ܽ�һ��*/

#define ZONEERR_MAP134 1041
/* @@error 1041 ˼�����еĺ�����Ҳ��ܽ��븱��*/

#define ZONEERR_MAP135 1042
/* @@error 1042 %s  ��˼�����к������,���ܽ��븱��*/

#define ZONEERR_MAP136 1043
/* @@error 1043 ��Ա %s ÿ�տ���ɴ����Ѵ�����*/

#define ZONEERR_MAP137 1044
/* @@error 1044 ��Ա %s ÿ�ܿ���ɴ����Ѵ�����*/

#define ZONEERR_MAP138 1045
/* @@error 1045 �˸���Ҫ��ȼ�%d-%d, ��ĵȼ�����,���ܽ���˸���*/

#define ZONEERR_MAP139 1046
/* @@error 1046 ��Ա %s �ȼ�����, �˸���Ҫ��ȼ�%d-%d,���ܽ���˸���*/

#define ZONEERR_MAP140 1047
/* @@error 1047 �˸���Ҫ��ȼ�%d-%d, ��ĵȼ�����,���ܽ���˸���*/

#define ZONEERR_MAP141 1048
/* @@error 1048 ��Ա %s �ȼ�����, �˸���Ҫ��ȼ�%d-%d,���ܽ���˸���*/

#define ZONEERR_MAP142 1049
/* @@error 1049 %s������*/

#define ZONEERR_MAP143 1050
/* @@error 1050 �˸���Ҫ��ȼ�%d-%d, %s�ȼ�����*/

#define ZONEERR_MAP144 1051
/* @@error 1051 %s�Ѵﵽ %s����ÿ����ɵĴ�������*/

#define ZONEERR_MAP145 1052
/* @@error 1052 %s ���㲻��ͬһ�����ᣬ�˸���Ҫ��ͬһ����*/

#define ZONEERR_MAP146 1053
/* @@error 1053 %s���ṱ�׶Ȳ���*/

#define ZONEERR_MAP147 1054
/* @@error 1054 %s ���㲻��ͬһ��ͼ��*/

#define ZONEERR_MAP148 1055
/* @@error 1055 %s������Ұ��*/

#define ZONEERR_MAP149 1056
/* @@error 1056 ��������ʧ�ܣ��޷����룬ֻ�жӳ��ſɴ�������*/

#define ZONEERR_MAP150 1057
/* @@error 1057 ����˵�ͼ%d���Ӻ�ᴫ�͵�ʱ���*/

#define ZONEERR_MAP151 1058
/* @@error 1058 %s�����Ͻ����ͼ����*/

#define ZONEERR_MAP152 1059
/* @@error 1059 ��ͼ�������࣬���Ժ����*/

#define ZONEERR_MAP153 1060
/* @@error 1060 �����������ٻ�����*/

#define ZONEERR_MAP154 1061
/* @@error 1061 ����������ʹ�þ���,�˳��������Զ���ԭ*/

#define ZONEERR_MAP155 1062
/* @@error 1062 ս��״̬�޷�����*/

#define ZONEERR_MAP156 1063
/* @@error 1063 û�н��븱����Ҫ�ĵ���*/

#define ZONEERR_MAP157 1064
/* @@error 1064 �㲻�ڸ�����*/

#define ZONEERR_NPC101 1065
/* @@error 1065 ������Ʒ��Ҫ��Ӧ����Ƽ�*/

#define ZONEERR_NPC102 1066
/* @@error 1066 ֻ��ʱ���ռ�칫���Ա���Թ������Ʒ*/

#define ZONEERR_NPC103 1067
/* @@error 1067 ֻ������ռ����%d��ʱ��ǵĹ����Ա�ſ��Թ������Ʒ*/

#define ZONEERR_NPC104 1068
/* @@error 1068 ������Ʒ��Ҫ%s�����ﵽ%s */

#define ZONEERR_NPC105 1069
/* @@error 1069 ��Ҫ%d  %s����������*/

#define ZONEERR_NPC106 1070
/* @@error 1070 û����Ӧ�Ķһ�����*/

#define ZONEERR_NPC107 1071
/* @@error 1071 %s����*/

#define ZONEERR_NPC108 1072
/* @@error 1072 Ҫ�ع�����Ʒ�ѹ���*/

#define ZONEERR_NPC109 1073
/* @@error 1073 ����Ʒ���ܳ���*/

#define ZONEERR_NPC110 1074
/* @@error 1074 ���������ﵽ����,����Ʒ���ܳ���*/

#define ZONEERR_NPC111 1075
/* @@error 1075 ���װ��ո�����������Ҫ����*/

#define ZONEERR_NPC112 1076
/* @@error 1076 �������,��װ������ӡ,����ʱ��ǽ���ʦ���*/

#define ZONEERR_NPC113 1077
/* @@error 1077 �����ϵ�װ���;ö���������ʱ����Ҫά��*/

#define ZONEERR_NPC114 1078
/* @@error 1078 ���ṱ�׶Ȳ���*/

#define ZONEERR_NPC115 1079
/* @@error 1079 ��װ����������*/

#define ZONEERR_PET101 1080
/* @@error 1080 ���ĳ����ҳ϶ȹ��ͣ���ѱ������*/

#define ZONEERR_PET102 1081
/* @@error 1081 %s ���ٴﵽ%d���ſ��Գ�ս*/

#define ZONEERR_PET103 1082
/* @@error 1082 ���������ѵ�������*/

#define ZONEERR_PET104 1083
/* @@error 1083 ������е�����ﲻ�ɳ�ս*/

#define ZONEERR_PET105 1084
/* @@error 1084 ��ǰ�������ﲻ�ɳ�ս*/

#define ZONEERR_PET106 1085
/* @@error 1085 ��ǰ��ͼ���ﲻ�ɳ�ս*/

#define ZONEERR_PET107 1086
/* @@error 1086 ��ǰ״̬�����޷���ս*/

#define ZONEERR_PET108 1087
/* @@error 1087 %s ���ٴﵽ%d���ſ��Գ�ս*/

#define ZONEERR_PET109 1088
/* @@error 1088 ����%s������10�����ϣ����ܳ�ս*/

#define ZONEERR_PET110 1089
/* @@error 1089 ���ĳ����ҳ϶Ȳ�������ѱ������*/

#define ZONEERR_PET111 1090
/* @@error 1090 ���������ѵ�������*/

#define ZONEERR_PET112 1091
/* @@error 1091 �������״̬���ܳ�������*/

#define ZONEERR_PET113 1092
/* @@error 1092 �������ȷ�Ĳ���*/

#define ZONEERR_PET114 1093
/* @@error 1093 ��������,�޷���������*/

#define ZONEERR_PET115 1094
/* @@error 1094 ���ĳ���%s �ҳ϶�̫�ͣ��޷���ս*/

#define ZONEERR_PET116 1095
/* @@error 1095 ���ĳ���%s Խ��Խ������*/

#define ZONEERR_PET117 1096
/* @@error 1096 ���ĳ���%s �ǳ�������*/

#define ZONEERR_PET118 1097
/* @@error 1097 ���ĳ���%s ���Ʋ���*/

#define ZONEERR_PET119 1098
/* @@error 1098 ����20������ʹ�ü�����*/

#define ZONEERR_PET120 1099
/* @@error 1099 �˼����޷�ѧϰ*/

#define ZONEERR_PET121 1100
/* @@error 1100 ���ﲻ��ѧϰ�������*/

#define ZONEERR_PET122 1101
/* @@error 1101 ���ĳ��ﴦ�ڳ���״̬,���ܽ��м�����������*/

#define ZONEERR_PET123 1102
/* @@error 1102 �ü����Ѵﵽ�ȼ�����*/

#define ZONEERR_PET124 1103
/* @@error 1103 ȱ�ټ�����������:%s */

#define ZONEERR_PET125 1104
/* @@error 1104 ���������������дʻ�Ƿ��ַ�(ֻ������Ӣ�Ļ�����),���������� */

#define ZONEERR_PET126 1105
/* @@error 1105 ���ĳ��ﴦ�ڳ���״̬�����ܽ���ǿ������ */

#define ZONEERR_PET127 1106
/* @@error 1106 �Է�û����֤��ȫ������*/

#define ZONEERR_PET128 1107
/* @@error 1107 ͬ������Ҳ����ںϳ���*/

#define ZONEERR_PET129 1108
/* @@error 1108 �Է������˳���Ϸ�޷��ںϳ���*/

#define ZONEERR_PET130 1109
/* @@error 1109 �Է�������״̬���޷��ںϳ���*/

#define ZONEERR_PET131 1110
/* @@error 1110 ����̫Զ,�޷��ںϳ���*/

#define ZONEERR_PET132 1111
/* @@error 1111 ��ĳ���δ����,�޷��ںϳ���*/

#define ZONEERR_PET133 1112
/* @@error 1112 ��ĳ��ﲻ���Խ����ں�*/

#define ZONEERR_PET134 1113
/* @@error 1113 ��ĳ�������δ�ﵽ��������,�޷��ں�*/

#define ZONEERR_PET135 1114
/* @@error 1114 ��ĳ������ʲ���һ������.�޷��ں�*/

#define ZONEERR_PET136 1115
/* @@error 1115 ��ĳ���ȼ�С��50��,�޷��ں�*/

#define ZONEERR_PET137 1116
/* @@error 1116 �Է�����δ����,�޷��ںϳ���*/

#define ZONEERR_PET138 1117
/* @@error 1117 �Է��ĳ�������δ�ﵽ��������,�޷��ں�*/

#define ZONEERR_PET139 1118
/* @@error 1118 �Է��ĳ������ʲ���һ������,�޷��ں�*/

#define ZONEERR_PET140 1119
/* @@error 1119 �Է��ĳ���ȼ�С��50��,�޷��ں�*/

#define ZONEERR_PET141 1120
/* @@error 1120 ˫�����ﲻ��ͬ�����,�޷��ں�*/

#define ZONEERR_PET142 1121
/* @@error 1121 �������ں�״̬,�޷��ںϳ���*/

#define ZONEERR_PET143 1122
/* @@error 1122 ���ڴ����ں�����,�޷��ںϳ���*/

#define ZONEERR_PET144 1123
/* @@error 1123 �Է������ں�״̬,�޷��ںϳ���*/

#define ZONEERR_PET145 1124
/* @@error 1124 �Է����ڴ����ں�����,�޷��ںϳ���*/

#define ZONEERR_PET146 1125
/* @@error 1125 ���ĳ��ﴦ�ڳ�ս״̬�����ܽ��д˲���*/

#define ZONEERR_PET147 1126
/* @@error 1126 ǿ���ȼ��Ѿ��ﵽ���*/

#define ZONEERR_PET148 1127
/* @@error 1127 ����ǿ���Ѽ��������*/

#define ZONEERR_NOITEM 1128
/* @@error 1128 ȱ����Ӧ�ĵ���*/

#define ZONEERR_PET149 1129
/* @@error 1129 ���ĳ��ﴦ�ڳ���״̬,���ܽ��м������ò���*/

#define ZONEERR_PET150 1130
/* @@error 1130 ���ܸ񶼱�����,�޷����ü���*/

#define ZONEERR_PET151 1131
/* @@error 1131 ����������*/

#define ZONEERR_PET152 1132
/* @@error 1132 ���ĳ���:%s�ѽ���Ϊ%s����*/

#define ZONEERR_PET153 1133
/* @@error 1133 %s���㣬���ܽ��лû�����*/

#define ZONEERR_PET154 1134
/* @@error 1134 ��̯״̬�²��ܽ��лû�����*/

#define ZONEERR_PET155 1134
/* @@error 1134 ���ĳ��ﴦ�ڳ���״̬�����ܽ��лû�����*/

#define ZONEERR_PET156 1135
/* @@error 1135 ��������޷��û�*/

#define ZONEERR_PET157 1136
/* @@error 1136 ������һ��%s */

#define ZONEERR_PET158 1137
/* @@error 1137 ��ǰ״̬����ʹ�ü��� */

#define ZONEERR_PET159 1138
/* @@error 1138 ������͵ȼ�Ҫ��Ϊ%d�� */

#define ZONEERR_PET160 1139
/* @@error 1139 �����ҳ϶ȵ���%d�����ܺ��� */

#define ZONEERR_PET161 1140
/* @@error 1140 ����%s������10�����ϣ����ܳ�ս��*/

#define ZONEERR_PET162 1141
/* @@error 1141 �����ϵĳ����޷���ս*/

#define ZONEERR_PET163 1142
/* @@error 1142 ����%s�Ѿ�ѧ��%s�������ظ�ѧϰ*/

#define ZONEERR_PET164 1143
/* @@error 1143 ����%s�ҳ϶�����*/

#define ZONEERR_PET165 1144
/* @@error 1144 ����%s�ҳ϶Ȼָ���%d��*/

#define ZONEERR_PET166 1145
/* @@error 1145 ��̯״̬������˲���*/

#define ZONEERR_PET167 1146
/* @@error 1146 ����״̬������˲���*/

#define ZONEERR_PET168 1147
/* @@error 1147 ����״̬������˲���*/

#define ZONEERR_PET169 1148
/* @@error 1148 ����[%s]ѧ����%s */

#define ZONEERR_PET170 1149
/* @@error 1149 ���Ҳ��㣬�޷����г��＼������*/

#define ZONEERR_PET171 1150
/* @@error 1150 ����[%s]ѧ����%s%d��*/

#define ZONEERR_PET172 1151
/* @@error 1151 ����[%s]%s���ܵȼ��½���*/

#define ZONEERR_PET173 1152
/* @@error 1152 �������ܵȼ�ʧ��,���ܵȼ�����*/

#define ZONEERR_PET174 1153
/* @@error 1153 ǿ�����ϲ���,�޷�ǿ��*/

#define ZONEERR_PET175 1154
/* @@error 1154 ����[%s]�ļ�����λ������µ�����*/

#define ZONEERR_PET176 1155
/* @@error 1155 %s�ܾ�����ĳ����ں�����*/

#define ZONEERR_PET177 1156
/* @@error 1156 �����ں�ȡ��*/

#define ZONEERR_PET178 1157
/* @@error 1157 ������һ��%s */

#define ZONEERR_PET179 1158
/* @@error 1158 ���ֻ������%d�����ܸ�*/

#define ZONEERR_PET180 1159
/* @@error 1159 �˼��ܸ��Ѿ�������*/

#define ZONEERR_PET181 1160
/* @@error 1160 ���ܸ������ɹ�*/

#define ZONEERR_PET182 1161
/* @@error 1161 ���ĳ��ﴦ�ڳ�ս״̬�����ܽ��м������������*/

#define ZONEERR_PET183 1162
/* @@error 1162 ��û���������ܸ�*/

#define ZONEERR_PET184 1163
/* @@error 1163 ��û�������˼��ܸ�*/

#define ZONEERR_PET185 1164
/* @@error 1164 ���ܸ�����ɹ�*/

#define ZONEERR_PET186 1165
/* @@error 1165 ��ĳ���:%s ������%s ����*/

#define ZONEERR_PET187 1166
/* @@error 1166 %s ���㣬�޷����г��＼������*/

#define ZONEERR_PET188 1167
/* @@error 1167 ȱ��%s���޷����г��＼������*/

#define ZONEERR_PET189 1168
/* @@error 1168 ���＼�����óɹ�*/

#define ZONEERR_PET190 1169
/* @@error 1169 ����[%s]�ĵȼ���������%d��*/

#define ZONEERR_PET191 1170
/* @@error 1170 ����[%s]�����%d����*/

#define ZONEERR_PET192 1171
/* @@error 1171 ��ǰ�޳�������,�޷����г���������*/

#define ZONEERR_PET193 1172
/* @@error 1172 �Ѿ����ڳ������״̬*/

#define ZONEERR_PET194 1173
/* @@error 1173 ��ɫ��͵ȼ�Ҫ��Ϊ%d��*/

#define ZONEERR_PLAYER101 1174
/* @@error 1174 ���ۼ�����ʱ������%dСʱ*/

#define ZONEERR_PLAYER102 1175
/* @@error 1175 ���Ѿ�����ƣ����Ϸʱ�䣬������Ϸ���潫��Ϊ����ֵ��50����Ϊ�����Ľ������뾡��������Ϣ�����ʵ�������������ѧϰ����*/

#define ZONEERR_PLAYER103 1176
/* @@error 1176 ���ѽ��벻������Ϸʱ�䣬Ϊ�����Ľ�������������������Ϣ���粻���ߣ��������彫�ܵ��𺦣����������ѽ�Ϊ�㣬ֱ�������ۼ�����ʱ����5Сʱ�󣬲��ָܻ�����*/

#define ZONEERR_PLAYER104 1177
/* @@error 1177 �ý�ɫ����δ���׵ķ�����,���ȳ�����ɾ����ɫ*/

#define ZONEERR_PLAYER105 1178
/* @@error 1178 ��̯״̬�²�����ȡ����*/

#define ZONEERR_PLAYER106 1179
/* @@error 1179 ����״̬�²�����ȡ����*/

#define ZONEERR_PLAYER107 1180
/* @@error 1180 %s ��������������%s��������ͨ��NPC���͵��������ִ�*/

#define ZONEERR_PLAYER108 1181
/* @@error 1181 ����ʺű����ý���*/

#define ZONEERR_PLAYER109 1182
/* @@error 1182 ����ʺű����ԣ���ʣ %d ��*/

#define ZONEERR_PLAYER110 1183
/* @@error 1183 �㱻���ý���*/

#define ZONEERR_PLAYER111 1184
/* @@error 1184 �㱻���ԣ���ʣ %d ��*/

#define ZONEERR_PLAYER112 1185
/* @@error 1185 �������%s�����ߡ������ڻ����������������*/

#define ZONEERR_PLAYER113 1186
/* @@error 1186 ������Ķ��󲻴��ڻ��߲�����*/

#define ZONEERR_PLAYER114 1187
/* @@error 1187 ���� %s ��������*/

#define ZONEERR_PLAYER115 1188
/* @@error 1188 �������%s ������*/

#define ZONEERR_PLAYER116 1189
/* @@error 1189 ���վ���ֵ�Ѵﵽ����*/

#define ZONEERR_PLAYER117 1190
/* @@error 1190 ��ľ���ֵ�Ѵﵽ�ȼ��洢���ޣ����ڽ�ɫ�����ֶ�����*/

#define ZONEERR_PLAYER118 1191
/* @@error 1191 ��ľ���ֵ�Ѵﵽ�ȼ��洢����*/

#define ZONEERR_PLAYER119 1192
/* @@error 1192 ��������δ�����ˣ��������������ϵͳ��3Сʱ�����澭��������3-5Сʱ�����澭����룬5Сʱ�����澭��Ϊ��*/

#define ZONEERR_NOMORE_MONEY1 1193
/* @@error 1193 �޷���ø���İ���*/

#define ZONEERR_NOMORE_MONEY2 1194
/* @@error 1194 �޷���ø���Ľ�ȯ*/

#define ZONEERR_PLAYER120 1195
/* @@error 1195 ��̯״̬���޷�����*/

#define ZONEERR_PLAYER121 1196
/* @@error 1196 ����״̬���޷�����*/

#define ZONEERR_PLAYER122 1197
/* @@error 1197 ����״̬���޷�����*/

#define ZONEERR_PLAYER123 1198
/* @@error 1198 ��ǰ״̬���޷�����*/

#define ZONEERR_PLAYER124 1199
/* @@error 1199 ��˻�е״̬���޷�����*/

#define ZONEERR_PLAYER125 1200
/* @@error 1200 ���״̬��ս������״̬���ܽ��и�������*/

#define ZONEERR_PLAYER126 1201
/* @@error 1201 ������û�б�ϵͳ�޸Ĺ�*/

#define ZONEERR_PLAYER127 1202
/* @@error 1202 ����Ȩ�޸Ĺ�����*/

#define ZONEERR_PLAYER128 1203
/* @@error 1203 ��������ͬ,����Ҫ�޸�*/

#define ZONEERR_PLAYER129 1204
/* @@error 1204 �������ѱ�ʹ��*/

#define ZONEERR_PLAYER130 1205
/* @@error 1205 ��ɫ����ͬ,����Ҫ�޸�*/

#define ZONEERR_PLAYER131 1206
/* @@error 1206 ��ɫ���޸ı�����7��*/

#define ZONEERR_PLAYER132 1207
/* @@error 1207 ��ǰ״̬������Ծ*/

#define ZONEERR_PLAYER133 1208
/* @@error 1208 ��ɫ%s�����ߣ����ܹ۲�Է�*/

#define ZONEERR_PLAYER134 1209
/* @@error 1209 ���������ﵽ����*/

#define ZONEERR_PLAYER135 1210
/* @@error 1210 �������Ŀ�괦�ڲ��ܸ���λ�ã�����ʧ��*/

#define ZONEERR_PLAYER136 1211
/* @@error 1211 ��ͬ��·�����ܸ���*/

#define ZONEERR_DIRTY1 1212
/* @@error 1212 ���뺬�����дʻ�Ƿ��ַ�(ֻ������Ӣ�Ļ�����),����������*/

#define ZONEERR_PLAYER137 1213
/* @@error 1213 �ڱ������ܸ�������������*/

#define ZONEERR_PLAYER138 1214
/* @@error 1214 Ŀ���޷�����*/

#define ZONEERR_PLAYER139 1215
/* @@error 1215 �����ڸ���!*/

#define ZONEERR_PLAYER140 1216
/* @@error 1216 ս��������������*/

#define ZONEERR_PLAYER141 1217
/* @@error 1217 û��תְ��ص���Ʒ*/

#define ZONEERR_PLAYER142 1218
/* @@error 1218 ���Ѿ�תְ��*/

#define ZONEERR_PLAYER143 1219
/* @@error 1219 �ȼ�����,����תְ*/

#define ZONEERR_PLAYER144 1220
/* @@error 1220 ���鲻��,����תְ*/

#define ZONEERR_PLAYER145 1221
/* @@error 1221 תְʧ��*/

#define ZONEERR_PLAYER146 1222
/* @@error 1222 û�ж�Ӧ�Ľ���*/

#define ZONEERR_PLAYER147 1223
/* @@error 1223 %s�����ﵽ������*/

#define ZONEERR_PLAYER148 1224
/* @@error 1224 ý��������������*/

#define ZONEERR_PLAYER149 1225
/* @@error 1225 �˵����Ѿ�ʹ�ù�%d���ˣ���������*/

#define ZONEERR_PLAYER150 1226
/* @@error 1226 ע���ʺųɹ�*/

#define ZONEERR_PLAYER151 1227
/* @@error 1227 �����Ϣ��ɹ�*/

#define ZONEERR_PLAYER152 1228
/* @@error 1228 �����Ϣ�ʧ��*/

#define ZONEERR_PLAYER153 1229
/* @@error 1229 ʹ����һ��%s */

#define ZONEERR_PLAYER154 1230
/* @@error 1230 ���ɱ��ֵ���ߣ����п����ܵ�Ѳ�������Ĺ���*/

#define ZONEERR_PLAYER155 1231
/* @@error 1231 ɱ��ֵ����%d��Ŀǰɱ��ֵΪ%d��*/

#define ZONEERR_PLAYER156 1232
/* @@error 1232 �㻹������ȡ�˽���*/

#define ZONEERR_PLAYER157 1233
/* @@error 1233 ���Ѿ���ȡ���˽���*/

#define ZONEERR_PLAYER158 1234
/* @@error 1234 ��Ч�Ľ׶ν���*/

#define ZONEERR_PLAYER159 1235
/* @@error 1235 %s�������ֶ�����*/

#define ZONEERR_PLAYER160 1236
/* @@error 1236 ��Ҫתְ��������*/

#define ZONEERR_PLAYER161 1237
/* @@error 1237 ������Ҫ�츳ֵ�ﵽ%d��*/

#define ZONEERR_PLAYER162 1238
/* @@error 1238 �ȼ����ڷ�ӡ�У���������ӡ����%dСʱ*/

#define ZONEERR_PLAYER163 1239
/* @@error 1239 ���״̬��ս������״̬���ܽ��и�������*/

#define ZONEERR_PLAYER164 1240
/* @@error 1240 ���ܸ����Լ�*/

#define ZONEERR_PLAYER165 1241
/* @@error 1241 Ŀ���޷�����*/

#define ZONEERR_PLAYER166 1242
/* @@error 1242 �㲻�ڸ�����*/

#define ZONEERR_PLAYER167 1243
/* @@error 1243 �㱻%s  ɱ����*/

#define ZONEERR_PLAYER168 1244
/* @@error 1244 ��ɱ���� %s */

#define ZONEERR_PLAYER169 1245
/* @@error 1245 �ȼ�����*/

#define ZONEERR_PLAYER170 1246
/* @@error 1246 Ԥ�����������Ѿ���ȡ����*/

#define ZONEERR_PLAYER171 1247
/* @@error 1247 %d���Ľ������Ѿ���ȡ����*/

#define ZONEERR_PLAYER172 1248
/* @@error 1248 �����Ѿ���ȡ�����ν���*/

#define ZONEERR_PLAYER173 1249
/* @@error 1249 �����Ѿ���ȡ�����齱��*/

#define ZONEERR_PLAYER174 1250
/* @@error 1250 ��С�ȼ�Ҫ��Ϊ%d��*/

#define ZONEERR_PLAYER175 1251
/* @@error 1251 ���ȼ�Ҫ��Ϊ%d��*/

#define ZONEERR_PLAYER176 1252
/* @@error 1252 ��������ʱ��С��%d����*/

#define ZONEERR_PLAYER177 1253
/* @@error 1253 ֻ����ȡ����ȼ���Ӧ�ĸ���*/

#define ZONEERR_PLAYER178 1254
/* @@error 1254 �ȼ�Ϊ1������ά���ʽ���Ĺ����޷���ȡ����*/

#define ZONEERR_PLAYER179 1255
/* @@error 1255 ���Ĺ�������δ�����,��μӻ�ĳ�Ա̫��*/

#define ZONEERR_PLAYER180 1256
/* @@error 1256 ��Ҫ�ȼ��ﵽ%d������ȡ�˸���*/

#define ZONEERR_PLAYER181 1257
/* @@error 1257 ��Ҫ���ṱ�׶ȴﵽ%d������ȡ�˸���*/

#define ZONEERR_PLAYER182 1258
/* @@error 1258 ��Ҫ������еȼ��ﵽ%d������ȡ�˸���*/

#define ZONEERR_NOMORE_CONTRI 1259
/* @@error 1259 ��ĸ��˹��׶��Ѵ����ޣ�������������*/

#define ZONEERR_PLAYER183 1260
/* @@error 1260 ������������ʱ��С��%d����*/

#define ZONEERR_PLAYER184 1261
/* @@error 1261 ������������ʱ��С��%d����*/

#define ZONEERR_PLAYER185 1262
/* @@error 1262 ֻ������Чʱ������ȡ����*/

#define ZONEERR_PLAYER186 1263
/* @@error 1263 ֻ�������а��ϲ�����ȡ����*/

#define ZONEERR_PLAYER187 1264
/* @@error 1264 �Ѿ���ȡ������*/

#define ZONEERR_PLAYER188 1265
/* @@error 1265 û��������Ӧ�Ľ���*/

#define ZONEERR_PLAYER189 1266
/* @@error 1266 ������ȱ��GPS��λ�ǵ���*/

#define ZONEERR_PLAYER190 1267
/* @@error 1267 GPS��λ�ǳ���ʱЧ*/

#define ZONEERR_PLAYER191 1268
/* @@error 1268 ���Ѿ��ڱ���״̬��,����ʧ��*/

#define ZONEERR_PLAYER192 1269
/* @@error 1269 ���ڰ�̯״̬��,����ʧ��*/

#define ZONEERR_PLAYER193 1270
/* @@error 1270 �����Զ�����״̬��,����ʧ��*/

#define ZONEERR_PLAYER194 1271
/* @@error 1271 �������״̬��,����ʧ��*/

#define ZONEERR_PLAYER195 1272
/* @@error 1272 ��ӭ����ɳ̲������������ó�����չ�ԡ����*/

#define ZONEERR_PLAYER196 1273
/* @@error 1273 �����컹ʣ��%d���ӵ��չ�ԡ���㾭��ʱ��*/

#define ZONEERR_PLAYER196 1273
/* @@error 1273 �����컹ʣ��%d���ӵ��չ�ԡ���㾭��ʱ��*/

#define ZONEERR_PLAYER197 1274
/* @@error 1274 ��������չ�ԡ���㾭��ʱ��������*/

#define ZONEERR_PLAYER198 1275
/* @@error 1275 �����������뿪ɳ̲��Χ*/

#define ZONEERR_PLAYER199 1276
/* @@error 1276 ս��������ʼ����ʱ���ռ�칫���Ա�����ڴ˵�ͼ����*/

#define ZONEERR_PLAYER200 1277
/* @@error 1277 ս��ģʽ���л�Ϊ[����ģʽ] */

#define ZONEERR_PLAYER201 1278
/* @@error 1278 ý����������� */

#define ZONEERR_PLAYER202 1279
/* @@error 1279 ��Nѡ1�У���ѡ��һ������ */

#define ZONEERR_PLAYER203 1280
/* @@error 1280 �ڵ�ǰ��ͼ�޷�ʹ�� */

#define ZONEERR_PLAYER204 1281
/* @@error 1281 ������%d�㹫�ṱ�׶� */

#define ZONEERR_PWORLD201 1282
/* @@error 1282 ���ѱ�����Ϊ��е���ˣ�������������ж���Ӫ��Ӯȡ���齱��*/

#define ZONEERR_PWORLD202 1283
/* @@error 1283 ���ѱ�����Ϊ��е���ޣ�������������ж���Ӫ��Ӯȡ���齱��*/

#define ZONEERR_RANGE201 1284
/* @@error 1284 ��������ԭ���˽����ϵ,�޷��鿴*/

#define ZONEERR_RANGE202 1285
/* @@error 1285 �Է��������������,�������������*/

#define ZONEERR_RANGE203 1286
/* @@error 1286 ���᲻������*/

#define ZONEERR_RANGE204 1287
/* @@error 1287 Ŀ���޷��۲�*/

#define ZONEERR_RANGE205 1288
/* @@error 1288 Ŀ�������޷��۲�*/

#define ZONEERR_ROLL201 1289
/* @@error 1289 ��������������л�����ģʽ*/

#define ZONEERR_ROLL202 1290
/* @@error 1290 ֻ�жӳ������л�����ģʽ*/

#define ZONEERR_RONGLU201 1291
/* @@error 1291 �����Ѿ����*/

#define ZONEERR_RONGLU202 1292
/* @@error 1292 ȱ�ٵ���:%s */

#define ZONEERR_RONGLU203 1293
/* @@error 1293 ��¯�ѷ��ã��������� */

#define ZONEERR_RONGLU204 1294
/* @@error 1294 ��ǰû�п���������¯ */

#define ZONEERR_RONGLU205 1295
/* @@error 1295 ������������¯,������˲��� */

#define ZONEERR_RONGLU206 1296
/* @@error 1296 ��ǰ�Ѿ��ǽ���¯,������˲���*/

#define ZONEERR_RUNE201 1297
/* @@error 1297 �Ҳ���ָ���ļ���*/

#define ZONEERR_RUNE202 1298
/* @@error 1298 �������������޷���ô��*/

#define ZONEERR_RUNE203 1299
/* @@error 1299 ����Ҫ�ﵽ%d�����ſ�ִ�иò���*/

#define ZONEERR_RUNE204 1300
/* @@error 1300 �˼��ܷ�����δ����*/

#define ZONEERR_RUNE205 1301
/* @@error 1301 ���İ�������, ����Ҫ%d����*/

#define ZONEERR_RUNE206 1302
/* @@error 1302 ���ĸ��ѿ����������ü�������Ƕ������*/

#define ZONEERR_RUNE207 1303
/* @@error 1303 �Ҳ���ָ���ļ���*/

#define ZONEERR_RUNE208 1304
/* @@error 1304 �Ҳ���ָ������Ʒ*/

#define ZONEERR_RUNE209 1305
/* @@error 1305 ����Ҫ�ﵽ%d���󣬲ſ�ִ�иò���*/

#define ZONEERR_RUNE210 1306
/* @@error 1306 �ø���δ����������������ô��*/

#define ZONEERR_RUNE211 1307
/* @@error 1307 �ü���û�ж�Ӧ�ķ������ԣ���������ô��*/

#define ZONEERR_RUNE212 1308
/* @@error 1308 �����Ʒ���Ƿ���*/

#define ZONEERR_RUNE213 1309
/* @@error 1309 ���ܷ���δ����ȷ����*/

#define ZONEERR_RUNE214 1310
/* @@error 1310 ���ܡ�%s���ɹ���Ƕ�˷���*/

#define ZONEERR_RUNE215 1311
/* @@error 1311 ��ѡ������Ʒ���Ƿ�����������*/

#define ZONEERR_RUNE216 1312
/* @@error 1312 �ø���û�з��ģ�����������ô��*/

#define ZONEERR_RUNE217 1313
/* @@error 1313 ��Ҫ�����������߲�������*/

#define ZONEERR_RUNE218 1314
/* @@error 1314 ������������Ҫ�ĵ�����������*/

#define ZONEERR_RUNE219 1315
/* @@error 1315 �������ͬ�ķ���*/

#define ZONEERR_RUNE220 1316
/* @@error 1316 ֻ���������ĵ���*/

#define ZONEERR_RUNE221 1317
/* @@error 1317 %s �Ѿ�����ߵȼ�����*/

#define ZONEERR_RUNE222 1318
/* @@error 1318 δ֪�ķ�����ɫ*/

#define ZONEERR_RUNE223 1319
/* @@error 1319 �����޷�����*/

#define ZONEERR_RUNE224 1320
/* @@error 1320 ս��״̬���޷��л����ļ���*/

#define ZONEERR_RUNE225 1321
/* @@error 1321 �����л�������ȴ��,���Ժ�����*/

#define ZONEERR_RUNE226 1322
/* @@error 1322 ��Ч�ķ�������*/

#define ZONEERR_RUNE227 1323
/* @@error 1323 ��û��Ƕ��Ӧ�ķ���*/

#define ZONEERR_FIGHT_OP 1324
/* @@error 1324 ս��״̬���޷����д˲���*/

#define ZONEERR_RUNE228 1325
/* @@error 1325 ��Ӧ��ɫ�߼�������������*/

#define ZONEERR_RUNE229 1326
/* @@error 1326 û�п��Ա���ķ��Ķ�������*/

#define ZONEERR_RUNE230 1327
/* @@error 1327 ��Ҫ��Ʒ%s */

#define ZONEERR_SAFE201 1328
/* @@error 1328 �˺Ŵ����쳣��¼״̬,%d�����ڲ��ܽ��д˲��� */

#define ZONEERR_SAFE202 1329
/* @@error 1329 ������֤��ȫ������*/

#define ZONEERR_SAFE203 1330
/* @@error 1330 ��ȫ�����벻��Ϊ��*/

#define ZONEERR_SAFE204 1331
/* @@error 1331 ���Ѿ����ù���ȫ��������*/

#define ZONEERR_SAFE205 1332
/* @@error 1332 ��ȫ������,���μǰ�ȫ������*/

#define ZONEERR_SAFE206 1333
/* @@error 1333 ��û�����ð�ȫ������*/

#define ZONEERR_SAFE207 1334
/* @@error 1334 ��ȫ���Ѿ���֤����*/

#define ZONEERR_SAFE208 1335
/* @@error 1335 ��ȫ����֤�ɹ�*/

#define ZONEERR_SCRIPT201 1336
/* @@error 1336 ������λ�ﵽ����*/

#define ZONEERR_SCRIPT202 1337
/* @@error 1337 ͬ���ľ���ֻ��ӵ��һ��*/

#define ZONEERR_SKILL201 1338
/* @@error 1338 ��ӵ���˼��ܣ�%s */

#define ZONEERR_SKILL202 1339
/* @@error 1339 �˼�������ְҵ����*/

#define ZONEERR_SKILL203 1340
/* @@error 1340 �˼�����Ҫ��С�ȼ�Ϊ%d��*/

#define ZONEERR_SKILL204 1341
/* @@error 1341 �˼�����Ҫ��ѧϰ%d��%s */

#define ZONEERR_SKILL205 1342
/* @@error 1342 �˼�����Ҫ�����%s����*/

#define ZONEERR_SKILL206 1343
/* @@error 1343 ѧϰ�˼�����Ҫ:%s */

#define ZONEERR_SKILL207 1344
/* @@error 1344 �����������辭��ֵ����*/

#define ZONEERR_SKILL208 1345
/* @@error 1345 ���츳����ѧ�츳����,����ѧϰ���츳*/

#define ZONEERR_SKILL209 1346
/* @@error 1346 �츳�㲻��*/

#define ZONEERR_SKILL210 1347
/* @@error 1347 ǰ���Ѿ�ʹ�õ��츳�㲻��,����ѧϰ�ò��츳����*/

#define ZONEERR_SKILL211 1348
/* @@error 1348 �˴����ܽ���*/

#define ZONEERR_SKILL212 1349
/* @@error 1349 ���״̬���ܷ���*/

#define ZONEERR_SKILL213 1350
/* @@error 1350 ��ǰ���鲻��,����ѧϰ����*/

#define ZONEERR_SPAN201 1351
/* @@error 1351 ����״̬���ɿ������*/

#define ZONEERR_SPAN202 1352
/* @@error 1352 ��������״̬�²��ɿ��*/

#define ZONEERR_SPAN203 1353
/* @@error 1353 ����ʧ�ܣ�ͬһ�ʺ�ֻ����һ����ɫ�ɴ��ڿ��״̬*/

#define ZONEERR_SPAN204 1354
/* @@error 1354 ������������,���Ժ�����*/

#define ZONEERR_SPAN205 1355
/* @@error 1355 ����ʧ�ܣ�ͬһ�ʺ�ֻ����һ����ɫ�ɴ��ڿ��״̬*/

#define ZONEERR_STATUS201 1356
/* @@error 1356 Ŀ�겻���κ�״̬Ч��Ӱ��*/

#define ZONEERR_STATUS202 1357
/* @@error 1357 %s����������ʧ��*/

#define ZONEERR_STATUS203 1358
/* @@error 1358 ������%s */

#define ZONEERR_STATUS204 1359
/* @@error 1359 ����Ů���٣��������%s״̬*/

#define ZONEERR_STRONG201 1360
/* @@error 1360 ʹ����ʱ��еʱ�޷�����*/

#define ZONEERR_STRONG202 1361
/* @@error 1361 ���״̬���޷�����*/

#define ZONEERR_STRONG203 1362
/* @@error 1362 ���ʱ������12Сʱ,�޷���ȡ*/

#define ZONEERR_STRONG204 1363
/* @@error 1363 �޷���ȡ,ֻ��ռ�칫���Ա�ſɽ��д˲���*/

#define ZONEERR_STRONG205 1364
/* @@error 1364 �޷���ȡ,ռ��ݵ��ڶ��������ȡ*/

#define ZONEERR_STRONG206 1365
/* @@error 1365 Ȩ�޲����޷���ȡ,ֻ�й���᳤������ȡ*/

#define ZONEERR_STRONG207 1366
/* @@error 1366 ���Ĺ�������Ѿ���ȡ�˴˾ݵ�ռ���ʽ�*/

#define ZONEERR_NOMORE_MONEY3 1367
/* @@error 1367 �޷���ø��������*/

#define ZONEERR_STRONG208 1368
/* @@error 1368 ���ɹ���ȡ��%d���ҵ�ռ���ʽ�*/

#define ZONEERR_STRONG209 1369
/* @@error 1369 ���֮���в��ܻ�ô�Ч��*/

#define ZONEERR_STRONG210 1370
/* @@error 1370 �������Ѿ���ȡ���˾ݵ��%sЧ��*/

#define ZONEERR_STRONG211 1371
/* @@error 1371 %sЧ����ȡ�ɹ�*/

#define ZONEERR_STRONG212 1372
/* @@error 1372 �������Ѿ���ȡ���˾ݵ㹫�ṱ�׶�*/

#define ZONEERR_STRONG213 1373
/* @@error 1373 ����ʧ��,ֻ�й���򸱻᳤��Ȩ�޲��뾺��*/

#define ZONEERR_STRONG214 1374
/* @@error 1374 ����ʧ��,�޷�����ռ��ľݵ���о���*/

#define ZONEERR_STRONG215 1375
/* @@error 1375 ����ʧ��,��ǰʱ�䲻�ܾ���*/

#define ZONEERR_STRONG216 1376
/* @@error 1376 ���Ĺ����Ѿ��μ������ݵ㾺��,����ʧ��*/

#define ZONEERR_STRONG217 1377
/* @@error 1377 �����ύ�ɹ�,����12��00����ڴ˴���ѯ���ս��*/

#define ZONEERR_STRONG218 1378
/* @@error 1378  [%s]�������ù�������*/

#define ZONEERR_STRONG219 1379
/* @@error 1379 �ݵ�ս��δ����,�޷�����*/

#define ZONEERR_STRONG220 1380
/* @@error 1380 �����Ѳμ�%sս,���ɽ��뵱ǰ�ݵ�ս��*/

#define ZONEERR_STRONG221 1381
/* @@error 1381 ��û�й���*/

#define ZONEERR_STRONG222 1382
/* @@error 1382 �����ڹ�����Ȩ�޽��뵱ǰ�ݵ�սս��*/

#define ZONEERR_STRONG223 1383
/* @@error 1383 �ǿɲ�սս�ų�Ա*/

#define ZONEERR_STRONG224 1384
/* @@error 1384 �޷�����,ֻ��ռ�칫���Ա�ſɽ��д˲���*/

#define ZONEERR_STRONG225 1385
/* @@error 1385 Ȩ�޲����޷�����,ֻ�л᳤�򸱻᳤�д�Ȩ��*/

#define ZONEERR_STRONG226 1386
/* @@error 1386 �����ʽ���,����ʧ��*/

#define ZONEERR_STRONG227 1387
/* @@error 1387 �������������Ѹ���*/

#define ZONEERR_STRONG228 1388
/* @@error 1388 �Ƽ��ȼ�����,����ʧ��*/

#define ZONEERR_STRONG229 1389
/* @@error 1389 ���������Ѿ�����ɹ�*/

#define ZONEERR_STRONG230 1390
/* @@error 1390 ���������Ѿ�����ɹ�*/

#define ZONEERR_STRONG231 1391
/* @@error 1391 ս��״̬���޷�����*/

#define ZONEERR_STRONG232 1392
/* @@error 1392 ���ڸ������޷�����*/

#define ZONEERR_NO_POWER_OP 1393
/* @@error 1393 Ȩ�޲��㣬�޷����д˲���*/

#define ZONEERR_STRONG233 1394
/* @@error 1394 ���óɹ�*/

#define ZONEERR_STRONG234 1395
/* @@error 1395 ֻ�й���򸱻᳤��Ȩ�޿���ս����ҫ�*/

#define ZONEERR_STRONG235 1396
/* @@error 1396 ռ��ݵ��ڶ�����ܿ���ս����ҫ�*/

#define ZONEERR_STRONG236 1397
/* @@error 1397 �˾ݵ�����Ѿ�������ս����ҫ�*/

#define ZONEERR_STRONG237 1398
/* @@error 1398 %s �Ĺ�����������ԡ��ҫ��â,��ʱ�޷�����*/

#define ZONEERR_STRONG238 1399
/* @@error 1399 ��ʱ�εĹ��������ƺ��޷���ӦӢ���ף��*/

#define ZONEERR_SVR201 1400
/* @@error 1400 �㱻���� %d ��*/

#define ZONEERR_SVR202 1401
/* @@error 1401 �㱻���ý���*/

#define ZONEERR_SVR203 1402
/* @@error 1402 ��Ľ��Ա������*/

#define ZONEERR_SVR204 1403
/* @@error 1403 ����ʺŽ��Ա������*/

#define ZONEERR_SVR205 1404
/* @@error 1404 ����ʺű����� %d ��*/

#define ZONEERR_SVR206 1405
/* @@error 1405 ��ɫ%s�����ߣ����ܹ۲�Է�*/

#define ZONEERR_TASK201 1406
/* @@error 1406 δ��Գɹ����޷���ȡ������*/

#define ZONEERR_TASK202 1407
/* @@error 1407 ��ȡ��ͬ������,������ȡ����*/

#define ZONEERR_TASK203 1408
/* @@error 1408 ���໷������ÿ����ɵĴ����Ѵﵽ����*/

#define ZONEERR_TASK204 1409
/* @@error 1409 �˻�������Ҫ����С�ȼ�Ϊ%d��*/

#define ZONEERR_TASK205 1410
/* @@error 1410 �˻�������Ҫ�����ȼ�Ϊ%d��*/

#define ZONEERR_TASK206 1411
/* @@error 1411 �˻�������ÿ����ɵĴ����Ѵﵽ����*/

#define ZONEERR_TASK207 1412
/* @@error 1412 %s�ﵽ���ޣ��������˻�������*/

#define ZONEERR_TASK208 1413
/* @@error 1413 ������û����ص��������*/

#define ZONEERR_TASK209 1414
/* @@error 1414 �����Է���������*/

#define ZONEERR_TASK210 1415
/* @@error 1415 ���ѷ���%s����*/

#define ZONEERR_TASK211 1416
/* @@error 1416 ������ʧ�ܣ�������������з���������*/

#define ZONEERR_TASK212 1417
/* @@error 1417 ����������������ͨ���ʼ�����*/

#define ZONEERR_TASK213 1418
/* @@error 1418 ��ɱ�������Ҫ%s */

#define ZONEERR_TASK214 1419
/* @@error 1419 �������[%s] �����%d �� */

#define ZONEERR_TASK215 1420
/* @@error 1420 ���������[%s] */

#define ZONEERR_TASK216 1421
/* @@error 1421 ������״̬�²����������*/

#define ZONEERR_TASK217 1422
/* @@error 1422 �����޷������������������������ٽ�����*/

#define ZONEERR_TASK218 1423
/* @@error 1423 ���Ѿ��������%s, ���������Զ����*/

#define ZONEERR_TASK219 1424
/* @@error 1424 %s�����Զ���޷���������*/

#define ZONEERR_TASK220 1425
/* @@error 1425 %s ����æ���޷������������Ժ�����*/

#define ZONEERR_TASK221 1426
/* @@error 1426 %sδ�ﵽ��������ĵȼ�Ҫ��*/

#define ZONEERR_TASK222 1427
/* @@error 1427 %s�ѽ�ȡ�˸������޷���������*/

#define ZONEERR_TASK223 1428
/* @@error 1428 %s�Ѵﵽ�������ɴ�������*/

#define ZONEERR_TASK224 1429
/* @@error 1429 %s��ͬ�����Ա���޷���������*/

#define ZONEERR_TASK225 1430
/* @@error 1430 %sδ��ɹ�������ǰ�������޷���������*/

#define ZONEERR_TASK226 1431
/* @@error 1431 %s���ڲ��ɱ���״̬���޷����ܴ�����*/

#define ZONEERR_TASK227 1432
/* @@error 1432 %s�Ѿ���ɹ��������޷����ܴ�����*/

#define ZONEERR_TASK228 1433
/* @@error 1433 %sδ�ﵽ������Ҫ���޷���������*/

#define ZONEERR_TASK229 1434
/* @@error 1434 ������״̬�в��ɹ�������*/

#define ZONEERR_TASK230 1435
/* @@error 1435 ս���в��ɹ�������*/

#define ZONEERR_TASK231 1436
/* @@error 1436 ��������ܹ���*/

#define ZONEERR_TASK232 1437
/* @@error 1437 ������ʧ�ܣ����ܹ���*/

#define ZONEERR_TASK233 1438
/* @@error 1438 ��������ɣ����ܹ���*/

#define ZONEERR_TASK234 1439
/* @@error 1439 ���ǲ���һ�������У����ܹ�������*/

#define ZONEERR_TASK235 1440
/* @@error 1440 ��ȡ��ͬ������,������ȡ����*/

#define ZONEERR_TASK236 1441
/* @@error 1441 ���������ߵı�����������������*/

#define ZONEERR_TASK237 1442
/* @@error 1442 �������������Ļ��Ҵﵽ���ޣ�������ȡ����*/

#define ZONEERR_TASK238 1443
/* @@error 1443 ��������Ҫ��ӽ���*/

#define ZONEERR_TASK239 1444
/* @@error 1444 �����ܴ�����ʱ������е������״̬�£��޷��������Ժ�����*/

#define ZONEERR_TASK240 1445
/* @@error 1445 �Ѿ������������,�����ظ���ȡ*/

#define ZONEERR_TASK241 1446
/* @@error 1446 ���Խӵ������Ѿ��ﵽ����*/

#define ZONEERR_TASK242 1447
/* @@error 1447 ������������Ҳ���*/

#define ZONEERR_TASK243 1448
/* @@error 1448 ��ǰ״̬���ܽ�ȡ��������*/

#define ZONEERR_TASK244 1449
/* @@error 1449 ��ȡ��������ҪԤ��һ��������*/

#define ZONEERR_TASK245 1450
/* @@error 1450 ��������%s���޷���ȡ*/

#define ZONEERR_TASK246 1451
/* @@error 1451 ��ȡ������[%s] */

#define ZONEERR_TASK247 1452
/* @@error 1452 ��ϲ�㴥����[%s] ����������*/

#define ZONEERR_TASK248 1453
/* @@error 1453 ��Ĺ�����Դ���������� %s x %d */

#define ZONEERR_TASK249 1454
/* @@error 1454 Ŀǰ״̬������ɸ����������³�*/

#define ZONEERR_TASK250 1455
/* @@error 1455 %s�����ߣ��޷���������*/

#define ZONEERR_TASK251 1456
/* @@error 1456 ��Ա:%s ,�޷����ܹ��������*/

#define ZONEERR_TASK252 1457
/* @@error 1457 ��Ա:%s ,���ܹ��������ɹ�*/

#define ZONEERR_TASK253 1458
/* @@error 1458 ����������ʧЧ*/

#define ZONEERR_TASK254 1459
/* @@error 1459 �������%s���������[%s] */

#define ZONEERR_TASK255 1460
/* @@error 1460 %s�������㹲�������[%s] */

#define ZONEERR_TASK256 1461
/* @@error 1461 %s�ܾ����㹲������� */

#define ZONEERR_UNREAL201 1462
/* @@error 1462 ����%s  ������*/

#define ZONEERR_UNREAL202 1463
/* @@error 1463 ���Ѿ�����,�޷�����*/

#define ZONEERR_UNREAL203 1464
/* @@error 1464 ����%s  �Ѿ�����,�޷�����*/

#define ZONEERR_UNREAL204 1465
/* @@error 1465 ����%s  ���㲻��ͬһ��ͼ,�޷�����*/

#define ZONEERR_UNREAL205 1466
/* @@error 1466 �����ڰ�̯,�޷�����*/

#define ZONEERR_UNREAL206 1467
/* @@error 1467 ����%s  ���ڰ�̯,�޷�����*/

#define ZONEERR_UNREAL207 1468
/* @@error 1468 �����ڽ���,�޷�����*/

#define ZONEERR_UNREAL208 1469
/* @@error 1469 ����%s  ���ڽ���,�޷�����*/

#define ZONEERR_UNREAL209 1470
/* @@error 1470 �������д�,�޷�����*/

#define ZONEERR_UNREAL210 1471
/* @@error 1471 ����%s  �����д�,�޷�����*/

#define ZONEERR_UNREAL211 1472
/* @@error 1472 ������˼������,�޷�����*/

#define ZONEERR_UNREAL212 1473
/* @@error 1473 ����%s  ����˼������,�޷�����*/

#define ZONEERR_UNREAL213 1474
/* @@error 1474 ��������˻���״̬��,�޷�����*/

#define ZONEERR_UNREAL214 1475
/* @@error 1475 ����%s  ������˻���״̬��,�޷�����*/

#define ZONEERR_UNREAL215 1476
/* @@error 1476 ��û�ж���*/

#define ZONEERR_UNREAL216 1477
/* @@error 1477 �㲻�Ƕӳ�*/

#define ZONEERR_UNREAL217 1478
/* @@error 1478 �ӳ����ܵ���20��*/

#define ZONEERR_UNREAL218 1479
/* @@error 1479 ��������%d��*/

#define ZONEERR_UNREAL219 1480
/* @@error 1480 ����%s  ������*/

#define ZONEERR_UNREAL220 1481
/* @@error 1481 ������Ч��������%d��*/

#define ZONEERR_UNREAL221 1482
/* @@error 1482 ��û�б����%d��ļ�¼*/

#define ZONEERR_UNREAL222 1483
/* @@error 1483 ����ɱ����С��%d�� */

#define ZONEERR_UNREAL223 1484
/* @@error 1484 �ó�����δ����,�����ڴ�*/

#define ZONEERR_UNREAL224 1485
/* @@error 1485 ���Ѿ�����%d��,�����ڼ���֮�Ŵ��������ļ�¼,��һ�ν����Դ������ֱ�ӽ���þ�%d��*/

#define ZONEERR_UNREAL225 1486
/* @@error 1486 ���Ѿ�������ò�Ľ���*/

#define ZONEERR_UNREAL226 1487
/* @@error 1487 ����ɹ�*/

#define ZONEERR_UNREAL227 1488
/* @@error 1488 Ŀǰû�лþ�����*/

#define ZONEERR_UNREAL228 1489
/* @@error 1489 �ӳ�������*/

#define ZONEERR_UNREAL229 1490
/* @@error 1490 �ӳ����ڻþ���*/

#define ZONEERR_UNREAL230 1491
/* @@error 1491 ���Ƕӳ����޷�����*/

#define ZONEERR_UNREAL231 1492
/* @@error 1492 ���뱾��Boss��ͼ��Ҫ�ӳ��ȼ��ﵽ%d��*/

#define ZONEERR_MUD101 2000
/* @@error 2000 %s�����Ϊ����*/

#define ZONEERR_MUD102 2001
/* @@error 2001 %s �ܾ�����Ϊ����*/

#define ZONEERR_MUD103 2002
/* @@error 2002 %s �ܾ�����ĺ�������*/

#define ZONEERR_MUD104 2003
/* @@error 2003 �������%s �����߻򲻴���*/


#define ZONEERR_TEAM301 2100
/* @@error 2100 ����%s��ȡ��*/

#define ZONEERR_TEAM302 2101
/* @@error 2101 %sͬ�⿪��%s */

#define ZONEERR_TEAM303 2102
/* @@error 2102 %s�ܾ�����%s */

#define ZONEERR_TEAM304 2103
/* @@error 2103 ����ʧ��,%s ������*/

#define ZONEERR_TEAM305 2104
/* @@error 2104 %s ���ڲ��ɴ���״̬*/

#define ZONEERR_TEAM306 2105
/* @@error 2105 �ӳ�����ֵ���㣬�޷���ɴ���*/

#define ZONEERR_TEAM307 2106
/* @@error 2106 %s������ÿ����ɵĴ����Ѵﵽ����*/

#define ZONEERR_TEAM308 2107
/* @@error 2107 %s�ĵȼ�����������Ҫ��*/

#define ZONEERR_TEAM309 2108
/* @@error 2108 %s��ȡ��ͬ������,������ȡ������*/

#define ZONEERR_TEAM310 2109
/* @@error 2109 %s���������ߵı�����������������*/

#define ZONEERR_TEAM311 2110
/* @@error 2110 %s�����������Ļ��Ҵﵽ���ޣ�������ȡ����*/

#define ZONEERR_TEAM312 2111
/* @@error 2111 %s���ܴ�����ʱ������е������״̬�£��޷��������Ժ�����*/

#define ZONEERR_TEAM313 2112
/* @@error 2112 %s�Ѿ������������,�����ظ���ȡ*/

#define ZONEERR_TEAM314 2113
/* @@error 2113 %s���Խӵ������Ѿ��ﵽ����*/

#define ZONEERR_TEAM315 2114
/* @@error 2114 %s������������Ҳ���*/

#define ZONEERR_TEAM316 2115
/* @@error 2115 �����������ӻ�ɱ���ﾭ������*/

#define ZONEERR_TEAM317 2116
/* @@error 2116 ��ϲ���%s�����Ķ�Ա��%s�ɹ���ɱ����%d �׵����Ҿ��ޣ��������Ķ�Ա�ǻ���˴����ľ��齱��*/

#define ZONEERR_TEAM318 2117
/* @@error 2117 %s������ÿ����ɵĴ����Ѵﵽ����*/

#define ZONEERR_TEAM319 2118
/* @@error 2118 �������г�Ա����ɵھŲ���֮������,�����ٴν�����֮��*/

#define ZONEERR_BROAD301 2200
/* @@error 2200 ���%s������������%s�п�����%sx%d */

#define ZONEERR_BROAD302 2201
/* @@error 2201 %s */

#define ZONEERR_BROAD303 2202
/* @@error 2202 ��ɳ�ݵؾݵ㱻ռ�죬ʱ�������ս�������ܣ��ڶ��ܵ�����20��00����ʽ���� */

#define ZONEERR_BROAD304 2203
/* @@error 2203 ս���ѽ�����[%s]������ʱ��ǵĿ���Ȩ */

#define ZONEERR_BROAD305 2204
/* @@error 2204 ս����ǰ������[%s]������ǰ���ʱ��ǿ���Ȩ*/

#define ZONEERR_BROAD306 2205
/* @@error 2205 ս���ѽ��������ź���û���κι�����ʱ��ǿ���Ȩ������ٽ�����*/

#define ZONEERR_BROAD307 2206
/* @@error 2206 ս���ѽ�����[%s]������Ȼӵ��ʱ��ǿ���Ȩ*/

#define ZONEERR_BROAD308 2207
/* @@error 2207 ʱ��ǳ�������ս������ʽ������ǰ���п����Ĳμ�ս��*/

#define ZONEERR_BROAD309 2208
/* @@error 2208 �𾴵�ʱ��ǳ���[%s]�Ѿ�����*/

#define ZONEERR_BROAD310 2209
/* @@error 2209 �𾴵�ʱ��ǳ���[%s]�Ѿ�����*/

#define ZONEERR_BROAD311 2210
/* @@error 2210 %s�����������ޣ������ڶ೯����*/

#define ZONEERR_BROAD312 2211
/* @@error 2211 %s��������������������������*/

#define ZONEERR_BROAD313 2212
/* @@error 2212 %s�����������������𲻶�*/

#define ZONEERR_BROAD314 2213
/* @@error 2213 %s������С��һ�ӣ�����˿����*/

#define ZONEERR_BROAD315 2214
/* @@error 2214 %s�����ķ�˿�Ѿ��鲼���ϱ�*/

#define ZONEERR_BROAD316 2215
/* @@error 2215 %s�����ĳ���߿�����������ʱ��ǣ�*/

#define ZONEERR_BROAD317 2216
/* @@error 2216 %s�����������������ܵ�������߱鲼����*/

#define ZONEERR_BROAD318 2217
/* @@error 2217 %s�Ѵﵽ�˼��˰����������������ڳ���̥�ľ���*/

#define ZONEERR_BROAD319 2218
/* @@error 2218 %s�����⵽�����˵����������������˳�����*/

#define ZONEERR_BROAD320 2219
/* @@error 2219 %s�����ĵ������˳����������޷��ֱ�����*/

#define ZONEERR_BROAD321 2220
/* @@error 2220 %s�����ĵ������ڽ��Ծ����Σ��ѲҲ��̶�*/

#define ZONEERR_BROAD322 2221
/* @@error 2221 %s�����ĵ����Ѿ�����������û������Χ��*/

#define ZONEERR_BROAD323 2222
/* @@error 2222 %s�����Ѿ��쵽���˼����ߣ��������ܣ����˶�û�˰��ľ���*/

#define ZONEERR_BROAD324 2223
/* @@error 2223 �Ž�������������[%s]������ǿ���[%s]����*/

#define ZONEERR_BROAD325 2224
/* @@error 2224 %s����ɹ�ռ��%s��ȫ����Դ���ز���*/

#define ZONEERR_BROAD326 2225
/* @@error 2225 %s����ɹ�ռ��%s��ȫ����Դ���ز���*/

#define ZONEERR_BROAD327 2226
/* @@error 2226 %s�������ڽ���%s���ɼ�������10���Ӻ������*/

#define ZONEERR_BROAD328 2227
/* @@error 2227 %s��[#L%s@@(%d,%d,%d)#n] �����𻵣��ɼ������ռ��ĸ��ֵ��߶���ɢ���ڵ���*/

#define ZONEERR_BROAD330 2229
/* @@error 2229 %s������и��Ŭ������%s����%d�����������ܵ���һ����ٶ�*/

#define ZONEERR_BROAD331 2230
/* @@error 2230 [%s]�ĳ���[%s]�����һ������ǿ�������Ǻ��˵�ͷ����ɷ����*/

#define ZONEERR_BROAD332 2231
/* @@error 2231 %s �ĳ���%s �ѽ���Ϊ%s����*/

#define ZONEERR_BROAD333 2232
/* @@error 2232 %s���׼�ŭ��%s����һ�������ʮ���ǧ��*/

#define ZONEERR_BROAD334 2233
/* @@error 2233 ��ŭ�µ�%s�����˿ɵ������������µ�ǿ���Ѳ�������*/

#define ZONEERR_BROAD335 2234
/* @@error 2234 %s����������ɱ¾ʱ�䣬������צ�µ������Ѵﵽ100��*/

#define ZONEERR_BROAD336 2235
/* @@error 2235 ɱ¾����!��%sһ����%s�䵽ʬ���޴棬��Ϊ��200��������*/

#define ZONEERR_BROAD337 2236
/* @@error 2236 һצ��ɽ�ӣ�һǹ������!����%s��ǰһ�ж��Ǹ��ƣ���ɱ300���ˣ�С��һ��*/

#define ZONEERR_BROAD338 2237
/* @@error 2237 �޵�ɱ��!��Ψһ������%s�Ĵʣ�500�������������*/

#define ZONEERR_BROAD340 2239
/* @@error 2239 %s�����%s���������״̬����ҿ�ȥ����*/

#define ZONEERR_BROAD341 2240
/* @@error 2240 ǿ���%sͻ���˴��֮�ĵĵ�%d�㣬�����ĵ��Ľ���*/

#define ZONEERR_BROAD342 2241
/* @@error 2241 ��������,��������!%s ��%d��%d.%d��ˢ���˴��֮�ĵ�%d��ļ�¼*/

#define ZONEERR_BROAD343 2242
/* @@error 2242 �콵�ḣ�������*/

#define ZONEERR_BROAD344 2243
/* @@error 2243 %s��ͼ�������˹�%s����ɱ��������벻�����ջ��ϡ�����ǰ��*/

#define ZONEERR_BROAD345 2244
/* @@error 2244 ��Ҫ���棺%s�Ѽ���,���������ռ��*/

#define ZONEERR_BROAD346 2245
/* @@error 2245 %s������������ȥ��â,ս����ҫ��ݸ�һ����*/

#define ZONEERR_BROAD347 2246
/* @@error 2246 [ʱ��ǳ���] %s  �ѽ�ʱ��Ǳ�Ϊ%s */

#define ZONEERR_BROADMAP301 2300
/* @@error 2300 [%s]������Ѹ��֮�Ƴɹ�ռ����%s */

#define ZONEERR_BROADMAP302 2301
/* @@error 2301 [%s]���͹��ˣ��Ѿ���%s�����ֻ��� */

#define ZONEERR_BROADMAP303 2302
/* @@error 2302 [%s]�������ܵ�����ͷ�ɹ�������%s���� */

#define ZONEERR_BROADMAP304 2303
/* @@error 2303 [%s]������ǿ��������ɹ�ռ����%s */

#define ZONEERR_BROADMAP305 2304
/* @@error 2304 %s ����������,���˵�%d��ɱ*/

#define ZONEERR_BROADMAP306 2305
/* @@error 2305 %s ���ħһ��,�����%d��ɱ*/

#define ZONEERR_BROADMAP307 2306
/* @@error 2306 %s �ѳ���,�����%d��ɱ*/

#define ZONEERR_BROADMAP308 2307
/* @@error 2307 %s �������ܵ�,�����%d��ɱ*/

#define ZONEERR_BROADMAP309 2308
/* @@error 2308 %s  ����һ�����,�����%d��ɱ*/

#define ZONEERR_BROADMAP310 2309
/* @@error 2309 %s ���������ɵ������%d��ɱ*/

#define ZONEERR_BROADMAP311 2310
/* @@error 2310 [%s]���͹��ˣ���Ȼ��ɱ��%s���� %s��%s��*/

#define ZONEERR_BROADMAP312 2311
/* @@error 2311 %s �ɹ��ս���%s ��%d��ɱ! */

#define ZONEERR_BROADMAP313 2312
/* @@error 2312 һ����ұ��컯��Ϊ��е�������죬�������е������������ */

#define ZONEERR_BROADMAP314 2313
/* @@error 2313 һ����ұ��컯��Ϊ�˻�е�������죬�������е������������ */

#define ZONEERR_BROADMAP315 2314
/* @@error 2314 �μӻ�������٣����ջ�ر� */

#define ZONEERR_BROADMAP316 2315
/* @@error 2315 [%s]�����Ѿ���ɢ,[%s]�ڱ��ξݵ�ս�л�ʤ!30���ر�ս�� */

#define ZONEERR_BROADMAP317 2316
/* @@error 2316 [%s]���ṥռ��[%s]! */

#define ZONEERR_BROADMAP318 2317
/* @@error 2317 ע�⣺ս��ʱ�佫��5�����Ժ����! */

#define ZONEERR_BROADMAP319 2318
/* @@error 2318 ע�⣺ս��ʱ�佫��10�����Ժ����! */

#define ZONEERR_BROADMAP320 2319
/* @@error 2319 ע�⣺ս��ʱ�佫��20�����Ժ����! */

#define ZONEERR_BROADMAP321 2320
/* @@error 2320 %s�����ڱ��ξݵ�ս�л�ʤ!30���ر�ս��*/

#define ZONEERR_BROADMAP322 2321
/* @@error 2321 ս��ʱ���Ѿ�����,%s������%d����Դ�����������ʤ��.30���ر�ս��*/

#define ZONEERR_BROADMAP323 2322
/* @@error 2322 [%s]�ѷ�����ǰ�ݵ�ռ��Ȩ,[%s]�ڱ��ξݵ�ս�л�ʤ!30���ر�ս��*/

#define ZONEERR_BROADMAP324 2323
/* @@error 2323 ɱ��%d������!��ɱ�����ﵽ%d��,���Խ�����һ����*/

#define ZONEERR_BROADMAP325 2324
/* @@error 2324 ɱ��%d(�ܹ�%d)������! */

#define ZONEERR_BROADMAP326 2325
/* @@error 2325 %s��ɱ��%s */


#define ZONEERR_BROADCAMPMAP301 2400
/* @@error 2400 ��Ϊ������㣬������ʤ����Ψһ����*/

#define ZONEERR_BROADCAMPMAP302 2401
/* @@error 2401 ��Ϊ��е���˵����ǣ��������еж�����������ʤ��*/

#define ZONEERR_BROADCAMPMAP303 2402
/* @@error 2402 ��Ϊ��е���޵����ǣ�����һ�еж�����������ʤ��*/


#define ZONEERR_LINK301 2500
/* @@error 2500 #L%s#n  ������иŬ�������ڳɹ���%s ��Ƕ��@ITEMLINK@ ��*/

#define ZONEERR_LINK302 2501
/* @@error 2501 ������ȫ����@POSLINK@λ�ý�����*/

#define ZONEERR_LINK303 2502
/* @@error 2502 ���ϣ�ȫ����@POSLINK@λ�ü��ϣ�*/

#define ZONEERR_LINK304 2503
/* @@error 2503 %s����ռ��@POSLINK@%s����������������Դ*/

#define ZONEERR_LINK305 2504
/* @@error 2504 ������%d��@ITEMLINK@ */

#define ZONEERR_LINK306 2505
/* @@error 2505 �㶪����%d��@ITEMLINK@ */

#define ZONEERR_LINK307 2506
/* @@error 2506 %s�����%d��@ITEMLINK@ */

#define ZONEERR_LINK308 2507
/* @@error 2507 %s ����@ITEMLINK@ %d ��*/

#define ZONEERR_LINK309 2508
/* @@error 2508 #c00ff00��ϲ%sӮ����@ITEMLINK@# */

#define ZONEERR_LINK310 2509
/* @@error 2509 %s ������@ITEMLINK@ */

#define ZONEERR_LINK311 2510
/* @@error 2510 %s ���Ὺ����@POSLINK@�������ĵ�[ս����ҫ]�! */

#define ZONEERR_LINK312 2511
/* @@error 2511 %s ������@POSLINK@�������ĵ�[ս����ҫ]�,��ԡ��Խ����ϻ�þ���ֵ! */

#define ZONEERR_CLANBROAD201 2600
/* @@error 2600 [%s] �����Ѿ�������� */

#define ZONEERR_CLANBROAD202 2601
/* @@error 2601 [%s] �����Ѿ�������� */

#define ZONEERR_CLANBROAD203 2602
/* @@error 2602 [%s] �����Ѿ����� */

#define ZONEERR_CLANBROAD204 2603
/* @@error 2603 [%s] �Ѿ�ά����� */

#define ZONEERR_CLANBROAD205 2604
/* @@error 2604 [%s] �Ѿ���� */

#define ZONEERR_CLANBROAD206 2605
/* @@error 2605 �����ڵĹ����Ѿ�����ɢ�� */

#define ZONEERR_CLANBROAD207 2606
/* @@error 2606 ���Ĺ��᱾��ά������Ϊ��%d������ʣ��Ĺ����ʽ�%d��������֧��ά�����ã����ή���ȼ�%hd*/

#define ZONEERR_CLANBROAD208 2607
/* @@error 2607 ���Ĺ��᱾��ά������Ϊ��%d������ʣ��Ĺ����ʽ�%d��������֧��ά�����ã�������Խ�ͣ�ά������Խ�ߣ����ά���ʽ��������³���%s��ϵͳ����ɢ���� */

#define ZONEERR_CLANBROAD209 2608
/* @@error 2608 ���Ĺ��᱾��ά������Ϊ��%d������ʣ��Ĺ����ʽ�%d��������֧��ά�����ã�������Խ�ͣ�ά������Խ�ߣ����Ὣ��%s�󽵼��������ʱ���ڽ�ά���Ѳ��룬�򲻻ή��*/

#define ZONEERR_CLANBROAD210 2609
/* @@error 2609 ���Ĺ��᱾��ά������Ϊ��%d���۳���ǰ�����ʽ�Ϊ%d��������Խ�ͣ�ά������Խ��*/

#define ZONEERR_CLANBROAD211 2610
/* @@error 2610 �������%s�������սʱ�����������ж�״̬*/

#define ZONEERR_CLANBROAD212 2611
/* @@error 2611 %s����Ա��������սʱ�����*/

#define ZONEERR_CLANBROAD213 2612
/* @@error 2612 %s������%s�뱾���ᴦ����ս״̬, ����2Сʱ*/

#define ZONEERR_CLANBROAD214 2613
/* @@error 2613 ����᳤%s�Ѿ����߳���%d�죬����᳤��������30�죬��ô�᳤��ְλ�Զ�ת�ø����׶���߲�������ʱ�䲻���� 15��Ĺ����Ա*/

#define ZONEERR_CLANBROAD215 2614
/* @@error 2614 �᳤%s���������Ѿ�����30�죬�᳤ְλ�Զ�ת�ø�%s */

#define ZONEERR_CLANBROAD216 2615
/* @@error 2615 %sΪ����������%u�����ʽ�*/

#define ZONEERR_CLANBROAD217 2616
/* @@error 2616 �����ʽ�������%u */

#define ZONEERR_CLANBROAD218 2617
/* @@error 2617 %sΪ����������%d���ᰲ����*/

#define ZONEERR_CLANBROAD219 2618
/* @@error 2618 ���ᰲ����������%d */

#define ZONEERR_CLANBROAD220 2619
/* @@error 2619 %sΪ����������%d����Ƽ���*/

#define ZONEERR_CLANBROAD221 2620
/* @@error 2620 ����Ƽ���������%d */

#define ZONEERR_CLANBROAD222 2621
/* @@error 2621 GM �� %s ����˹���*/

#define ZONEERR_CLANBROAD223 2622
/* @@error 2622 %s �˳��˹���*/

#define ZONEERR_CLANBROAD224 2623
/* @@error 2623 ͬ�˹���: %s�Ѿ���GM���*/

#define ZONEERR_CLANBROAD225 2624
/* @@error 2624 �жԹ���: %s�Ѿ���GM���*/

#define ZONEERR_CLANBROAD226 2625
/* @@error 2625 ������⽻��ϵ�Ѿ���GM���*/

#define ZONEERR_CLANBROAD227 2626
/* @@error 2626 %s�Ļ᳤ְλ��GM��������%s */

#define ZONEERR_CLANBROAD228 2627
/* @@error 2627 %s �����˹���*/

#define ZONEERR_CLANBROAD229 2628
/* @@error 2628 ���������� %s ����*/

#define ZONEERR_CLANBROAD230 2629
/* @@error 2629 %s���������Ĺ��������ս����ս��������Сʱ*/

#define ZONEERR_CLANBROAD231 2630
/* @@error 2630 ���Ĺ����%s���������ս����ս��������Сʱ*/

#define ZONEERR_CLANBROAD232 2631
/* @@error 2631 %s �� %s ����˹���*/

#define ZONEERR_CLANBROAD233 2632
/* @@error 2632 %s �˳��˹���*/

#define ZONEERR_CLANBROAD234 2633
/* @@error 2633 �����ڵĹ����Ѿ���ɢ��*/

#define ZONEERR_CLANBROAD235 2634
/* @@error 2634 �᳤%s���������ɢ����ʱ��%s֮���ٴ�ȷ�Ͻ���ɢ����*/

#define ZONEERR_CLANBROAD236 2635
/* @@error 2635 %s�޸��˹���*/

#define ZONEERR_CLANBROAD237 2636
/* @@error 2636 %s�޸�������*/

#define ZONEERR_CLANBROAD238 2637
/* @@error 2637 %s��%s֮λ�ø���%s */

#define ZONEERR_CLANBROAD239 2638
/* @@error 2638 %s ������ %s ���ǳ� */

#define ZONEERR_CLANBROAD240 2639
/* @@error 2639 ���� %s �˳���ͬ�� */

#define ZONEERR_CLANBROAD241 2640
/* @@error 2640 ���� %s �˳���ͬ��*/

#define ZONEERR_CLANBROAD242 2641
/* @@error 2641 %s �ѽ� %s ����Ϊ %s */

#define ZONEERR_CLANBROAD243 2642
/* @@error 2642 ���Ἴ��%s�ȼ�%d�з��ɹ��������������ʽ�:%d */

#define ZONEERR_CLANBROAD244 2643
/* @@error 2643 �����ڵĹ����Ѿ���GM�����ɢ*/

#define ZONEERR_CLANBROAD245 2644
/* @@error 2644 ����Ľ�Ǯ����Դ��GM�޸�*/

#define ZONEERR_CLANBROAD246 2645
/* @@error 2645 ���ڻ���˲�������������BOSS��������Զ�������λǰ���������ʷ��������������ɱ*/

#define ZONEERR_CLANBROAD247 2646
/* @@error 2646 %s �òɼ�����ȡ��Դ��������Դ���Ķ����� [%s]��%d */

#define ZONEERR_CLANBROAD248 2647
/* @@error 2647  ��������: %s �������*/

#define ZONEERR_CLANBROAD249 2648
/* @@error 2648 ����Ƽ�: %s �з����*/

#define ZONEERR_CLANBROAD250 2649
/* @@error 2649 %s Ϊ���Ṻ����[%s]��%d */

#define ZONEERR_CLANBROAD251 2650
/* @@error 2650 ������Դ %s �Ѵﵽ���� */

#define ZONEERR_CLANBROAD252 2651
/* @@error 2651 %s �������Ľ��������Ĺ������*/

#define ZONEERR_CLANBROAD253 2652
/* @@error 2652 %s ����� %s  �������Ľ��������Ĺ������*/

#define ZONEERR_CLANBROAD254 2653
/* @@error 2653 ���ػ�������Ŧ-��ͨģʽ���ݸ�һ���䣬δ�μӵĳ�Ա�ɲμӡ��ػ�������Ŧ-����ģʽ��*/

#define ZONEERR_CLANBROAD255 2654
/* @@error 2654 �������ֻ�Ĺ������Ҫ������ɣ�����������#L������@@(%d,%d,%d)#n�������������еĻ�Ա��ù�����ֽ��� %d�㹱�׶ȣ�*/

#define ZONEERR_CLANBROAD256 2655
/* @@error 2655 �������[#L%s@@(%d,%d,%d)#n] ���⵽����������ȥ��˵�����*/

#define ZONEERR_CLANBROAD257 2656
/* @@error 2656 ���Ὠ��[%s] �� %s �ݻ�*/

#define ZONEERR_CLANBROAD258 2657
/* @@error 2657 ���Ὠ��[%s] �� %s ����� %s �ݻ�*/

#define ZONEERR_CLANBROAD259 2658
/* @@error 2658 %s �������Lv %d��������Lv %d */

#define ZONEERR_CLANBROAD260 2659
/* @@error 2659 %s ��������д�Lv %d��������Lv %d */

#define ZONEERR_CLANBROAD261 2660
/* @@error 2660 %s ��ʼ���� %s,��Ҫʱ��: %s */

#define ZONEERR_CLANBROAD262 2661
/* @@error 2661 %s ��ʼ���� %sLv.%d �ĵȼ�,��Ҫʱ��: %s */

#define ZONEERR_CLANBROAD263 2662
/* @@error 2662 %s ������� */

#define ZONEERR_CLANBROAD264 2663
/* @@error 2663 %sLv.%d ������� */

#define ZONEERR_CLANBROAD265 2664
/* @@error 2664 ����[#L%s@@(%d,%d,%d)#n] �ܵ������ƻ��������֧Ԯ*/

#define ZONEERR_CLANBROAD266 2665
/* @@error 2665 �ɹ����������ߣ��������ֻ�ݸ�һ����*/

#define ZONEERR_CLANBROAD267 2666
/* @@error 2666 %s%s�������������ֻ���ҵ�ʱ��ǵ�#L˹�ٷ�˹@@[303006,1000]#n���ٴ��͵�����ұ������*/

#define ZONEERR_CLANBROAD268 2667
/* @@error 2667 �����ڹ��Ὰ��%sʧ��,�˻������ʽ�%d */

#define ZONEERR_CLANBROAD269 2668
/* @@error 2668 �����ڹ��Ὰ��%s�ɹ�,�����й����Ա������20:30-21:30׼ʱ�μӾݵ�ս !#cff0000��ע�⣺�ݵ�ս��ս��ս�����ý���������������#n*/

#define ZONEERR_CLANBROAD270 2669
/* @@error 2669 �����ڹ��Ὰ��%s�ɹ�,��Ϊ�����ڹ�����Ψһ�μӾ��Ĵ˾ݵ㹫��,���Ĺ�����Ϊռ�칫��*/

#define ZONEERR_CLANBROAD271 2670
/* @@error 2670 �����ڹ����Ծ��Ľ����1������%s�ɹ�,ϵͳ�ж�Ϊ���ط�����,�����й����Ա������20��30׼ʱ�μӾݵ�ս!#cff0000��ע�⣺�ݵ�ս��ս��ս�����ý���������������#n*/

#define ZONEERR_CLANBROAD272 2671
/* @@error 2671 �����ڹ����Ծ��Ľ����2������%s�ɹ�,ϵͳ�ж�Ϊ��ս������,�����й����Ա������20��30׼ʱ�μӾݵ�ս!#cff0000��ע�⣺�ݵ�ս��ս��ս�����ý���������������#n*/

#define ZONEERR_CLANBROAD273 2672
/* @@error 2672 �Է����������%s���߽�ɢ�˹���,��ǰ�ݵ�����ռ��,���Ĺ�����Ϊռ�칫��*/

#define ZONEERR_CLANBROAD274 2673
/* @@error 2673 %sս�����Ѿ�����*/

#define ZONEERR_CLANBROAD275 2674
/* @@error 2674 %s������������ȥ��â,ս����ҫ��ݸ�һ����*/

#define ZONEERR_CLANBROAD276 2675
/* @@error 2675 �����ڹ����ѷ���%sռ��Ȩ*/

#define ZONEERR_CLANBROAD277 2676
/* @@error 2676 �㱻%s%s�߳��������*/

#define ZONEERR_CLANBROAD278 2677
/* @@error 2677 ����%s %s�ڹ�������б���ͻ��,��ȡ��ʱ�����ᷢ�ŵ�%d%s����*/

#define ZONEERR_KICK201 2700
/* @@error 2700 ������¼��ͼ�������࣬���Ժ��¼*/

#define ZONEERR_KICK202 2701
/* @@error 2701 �������,�����µ�¼*/

#define ZONEERR_KICK203 2702
/* @@error 2702 ϵͳʱ�䱻�޸�*/

#define ZONEERR_KICK204 2703
/* @@error 2703 ��Ľ�ɫ������ %d ��*/

#define ZONEERR_KICK205 2704
/* @@error 2704 ������ip��ַΪ%s���û�ʹ�������ʺŵ�¼*/

#define ZONEERR_KICK206 2705
/* @@error 2705 ����ʱ�����*/

#define ZONEERR_KICK207 2706
/* @@error 2706 ��ɫ�����ɹ�*/

#define ZONEERR_KICK208 2707
/* @@error 2707 ��Ľ�ɫ������ %d ��*/

#define ZONEERR_KICK209 2708
/* @@error 2708 ��Ľ�ɫ�����ö�����*/

#define ZONEERR_KICK210 2709
/* @@error 2709 ����ʺű����ö�����*/

#define ZONEERR_KICK211 2710
/* @@error 2710 ����ʺű����� %d ��*/

#define ZONEERR_KICK212 2711
/* @@error 2711 ��ɫ��������,����ͷ���ϵ*/

#define ZONEERR_KICK213 2712
/* @@error 2712 ��¼��IP��ַ������*/

#define ZONEERR_KICK214 2713
/* @@error 2713 ϵͳ�ر�*/


#define ZONEERR_SCRIPT301 2800
/* @@error 2800 �㿪�����ı���,���%s */

#define ZONEERR_SCRIPT302 2801
/* @@error 2801 �˺Ŵ����쳣��¼״̬,%d�����ڲ��ܽ��д˲���*/

#define ZONEERR_SCRIPT303 2802
/* @@error 2802 ÿ��ʹ�ô����Ѿ��ﵽ������*/

#define ZONEERR_SCRIPT304 2803
/* @@error 2803 %s�����ﵽ������,���ʧ��*/

#define ZONEERR_SCRIPT305 2804
/* @@error 2804 ����ɱ����С��%d��*/

#define ZONEERR_SCRIPT306 2805
/* @@error 2805 ��ͻ���˵ȼ��ļ���,���齱������*/

#define ZONEERR_SCRIPT307 2806
/* @@error 2807 ��ͻ���˵ȼ��ļ���,�������ı��ػ��˫������,�õ�%s��%s */

#define ZONEERR_SCRIPT308 2807
/* @@error 2807 %s ����̫Զ*/

#define ZONEERR_SCRIPT309 2808
/* @@error 2808 %s ����æ*/

#define ZONEERR_SCRIPT310 2809
/* @@error 2809 ���ô����Ѿ��ﵽ����*/

#define ZONEERR_SCRIPT311 2810
/* @@error 2810 ��Ҫ���ϲ���,���������Ŀ*/

#define ZONEERR_DIFF301 2900
/* @@error 2900 %s������*/

#define ZONEERR_DIFF302 2901
/* @@error 2901 �˸���%d������ֻ�ܽ�һ��*/

#define ZONEERR_DIFF303 2902
/* @@error 2902 ˼�����еĺ�����Ҳ��ܽ��븱��*/

#define ZONEERR_DIFF304 2903
/* @@error 2903 ÿ�տ���ɴ����Ѵ�����*/

#define ZONEERR_DIFF305 2904
/* @@error 2904 ÿ�ܿ���ɴ����Ѵ�����*/

#define ZONEERR_DIFF306 2905
/* @@error 2905 %s���ǹ����Ա*/

#define ZONEERR_DIFF307 2906
/* @@error 2906 ���ṱ�׶Ȳ���*/

#define ZONEERR_DIFF308 2907
/* @@error 2907 �ȼ������޷����븱��*/

#define ZONEERR_DIFF309 2908
/* @@error 2908 �ȼ������޷����븱��*/

#define ZONEERR_SAFE301 3000
/* @@error 3000 �˺Ŵ����쳣��¼״̬,%d�����ڲ��ܽ��д˲���*/

#define ZONEERR_SAFE302 3001
/* @@error 3001 ��ȫ��������֤ʧ��*/

#define ZONEERR_SAFE303 3002
/* @@error 3002 ��û�����ð�ȫ������*/

#define ZONEERR_SAFE304 3003
/* @@error 3003 ��ȫ��������֤ʧ��*/

#define ZONEERR_SAFE305 3004
/* @@error 3004 ��ȫ�����벻��Ϊ��*/

#define ZONEERR_SAFE306 3005
/* @@error 3005 �޸�����ɹ�*/


#define ZONEERR_SAFE309 3008
/* @@error 3008 ��ȫ�������Ѿ�ȡ��*/

#define ZONEERR_SAFE310 3009
/* @@error 3009 ��ȫ��������Ч*/

#define ZONEERR_SAFE311 3010
/* @@error 3010 ��ȫ������ֻ��%d����ҿ���*/

#define ZONEERR_TASK301 3100
/* @@error 3100 δ���ǰ������,������ȡ����*/


#define ZONEERR_PAK701 10000
/* @@error 10000 ���Ķ�λ����λ�ɹ�*/

#define ZONEERR_PAK702 10001
/* @@error 10001 ����Ʒ�޷���ĩ����ҫ������ʹ��*/

#define ZONEERR_PAK703 10002
/* @@error 10002 ÿ��������ȡ�Ѵﵽ����*/

#define ZONEERR_CLAN801 11000
/* @@error 11000 %s%s��%s�߳��������*/

#define ZONEERR_ARM901 12000
/* @@error 12000 ��ϲ%s �� %s�ĵ�%d�Ǵ�����������������ף���������*/

#define ZONEERR_ARM902 12001
/* @@error 12001 ����װ������ϴ��*/

#define ZONEERR_ARM903 12002
/* @@error 12002 ʱװ����ϴ��*/

#define ZONEERR_ARM904 12003
/* @@error 12003 ��ϴ�����ϲ������ڸ߼�ϴ��*/

#define ZONEERR_ARM905 12004
/* @@error 12004 ����װ��������Ƕ��ʯ*/

#define ZONEERR_ARM906 12005
/* @@error 12005 ����װ�����ɱ�ʯժȡ*/

#define ZONEERR_ARM907 12006
/* @@error 12006 ����װ������ת��*/

#define ZONEERR_ARM908 12007
/* @@error 12007 ����Ϊ������ʯ�ٳ���*/

#define ZONEERR_ARM909 12008
/* @@error 12008 ����װ����������*/

#define ZONEERR_ARM910 12009
/* @@error 12009 ʱװ��������*/

#define ZONEERR_ARM911 12010
/* @@error 12010 ��������ʯ�������ڸ߼�����*/

#define ZONEERR_ARM912 12011
/* @@error 12011 ����װ������ǿ��*/

#define ZONEERR_ARM913 12012
/* @@error 12012 ����װ������ת������*/

#define ZONEERR_ARM914 12013
/* @@error 12013 ��װ��û����Ҫ����ļ�������*/

#define ZONEERR_ARM915 12014
/* @@error 12014 ���Ҳ���,����ʧ��*/

#define ZONEERR_ARM916 12015
/* @@error 12015 ���Ҳ���,�޷�������������*/

#define ZONEERR_ARM917 12016
/* @@error 12016 ��������˭������,%s�������������������˵�����:@ITEMLINK@*/

#define ZONEERR_ARM918 12017
/* @@error 12017 ��ϲ����%s��%d�Ǵ������!*/


#define ZONEERR_PLAYER801 13000
/* @@error 13000 �㻹��δ�����Ľ�ҽ��׵������ܸ���*/

#define ZONEERR_PLAYER802 13001
/* @@error 13001 �㻹��δ���������ҽ��׵������ܸ���*/

#define ZONEERR_PLAYER803 13002
/* @@error 13002 ��ҫ����ʮ��ս��֮%s��Ӣ�µ�%s����!*/

#define ZONEERR_PLAYER804 13003
/* @@error 13003 ��ϲ���%s�����콵,������@ITEMLINK@ */

#define ZONEERR_PLAYER805 13004
/* @@error 13004 ����ֵ����, �޷����ܺ�̲���� */

#define ZONEERR_BATTLE100 14000
/* @@error 14000 [%s]�ɹ��ݻ���[%s]��Ҫ����������ⳡս����ʤ�� */

#define ZONEERR_BATTLE101 14001
/* @@error 14001 ս��ʱ���ѽ�����[%s]���ѵĻ��������ʤ��*/

#define ZONEERR_BATTLE102 14002
/* @@error 14002 [%s]%s��Ѹ�ײ����ڶ�֮�������ӿ�����ע������Դ�ᾧ*/

#define ZONEERR_BATTLE103 14003
/* @@error 14003 �㲻�ǹ����Ա,�޷�������ս��*/

#define ZONEERR_BATTLE104 14004
/* @@error 14004 ���Ѿ�������,�޷��ٱ�����ս��*/

#define ZONEERR_BATTLE105 14005
/* @@error 14005 ���ڹ���û�б�����ս�����������ܶ��Լ���*/

#define ZONEERR_BATTLE106 14006
/* @@error 14006 ���ڹ����Ѿ���������ս��*/

#define ZONEERR_BATTLE107 14007
/* @@error 14007 ���ڵĹ��ỹû�п�����ս��,���޷�������ս��*/

#define ZONEERR_BATTLE108 14008
/* @@error 14008 �㲻�ǹ����Ա,�޷�������ս��*/

#define ZONEERR_BATTLE109 14009
/* @@error 14009 ���Ҿ��޻���ڽ����У��޷�����*/

#define ZONEERR_BATTLE110 14010
/* @@error 14010 �ѱ���ĩ����ҫ���ã��޷�����������ս��*/

#define ZONEERR_BATTLE111 14011
/* @@error 14011 %s�ѱ���ĩ����ҫ���ã��޷�����ս������*/

#define ZONEERR_BATTLE112 14012
/* @@error 14012 ���Ȩ�޲���,�޷�������ս��*/

#define ZONEERR_BATTLE113 14013
/* @@error 14013 ս���ѽ���,�޷�����*/

#define ZONEERR_BATTLE114 14014
/* @@error 14014 δ�ҵ���Ӧ�Ĺ������*/

#define ZONEERR_BATTLE115 14015
/* @@error 14015 �õ�һ����Դ�ᾧ���ٶ��������ӿ�����*/

#define ZONEERR_BATTLE116 14016
/* @@error 14016 ȱ����Դ�ᾧ���޷��������ӿ�����*/

#define ZONEERR_BATTLE117 14017
/* @@error 14017 ���ѳ�Խ�ֽ׶�ս����ʵ�����������ȼ���ս�½׶�ս��*/


#define ZONEERR_CARDBOX100 15000
/* @@error 15000 %s���Ǹ��գ���%s�еõ���%s x%d*/

#define ZONEERR_ADD_MSG_SHOW 15001
/* @@error 15001 ��ϲ���:%s �õ�@ITEMLINK@,���Ǻ��˵�ͷ!*/

#define ZONEERR_NO_VIM 15002
/* @@error 15002 ���ľ���ֵ����*/

#define ZONEERR_CLAN_DART1 15003
/* @@error 15003 %s ���Ὺ����[��������]�,�ɹ����˽������ô�������ֵ!*/

#define ZONEERR_CLAN_DART2 15004
/* @@error 15004 %s ��������˷�æ������ó��,[��������]��ݸ�һ����!*/

#define ZONEERR_CLAN_DART3 15005
/* @@error 15005 %s �����%s �� %s ����ݻ�!*/

#define ZONEERR_STORE1 15006
/* @@error 15006 ���Ҳ���!*/

#define ZONEERR_STORE2 15007
/* @@error 15007 �������Ѵ���!*/

#define ZONEERR_STORE3 15008
/* @@error 15008 ��������Ͼ�˼�������Ϣ!*/

#define ZONEERR_STORE4 15009
/* @@error 15009 ս������Ͼ�˼�������Ϣ!*/

#define ZONEERR_STORE5 15010
/* @@error 15010 ��̯����Ͼ�˼�������Ϣ!*/

#define ZONEERR_STORE6 15011
/* @@error 15011 ��������Ͼ�˼�������Ϣ!*/

#define ZONEERR_STORE7 15012
/* @@error 15012 �д�����Ͼ�˼�������Ϣ!*/

#define ZONEERR_STORE8 15013
/* @@error 15013 ��������%d�����̳��������Ʒ����㡾�л������̿ɲ鿴!*/

#define ZONEERR_STORE9 15014
/* @@error 15014 δ�ѵ��е��̳��������Ʒ������ȷ������Ʒ������!*/

#define ZONEERR_CLAN_301 15100
/* @@error 15100 �����Ѿ��������������˻*/

#define ZONEERR_CLAN_302 15101
/* @@error 15101 ��ʱ�β��ܿ����������˻*/

#define ZONEERR_COLLECT_301 15201
/* @@error 15201 �޷���ø�����*/

#define ZONEERR_DIFF_301 15301
/* @@error 15301 ���Ѿ���ȡ����*/

#define ZONEERR_LOSTTOTEM_301 15401
/* @@error 15401 ÿ��ʹ�ô������Ϊ5��*/

#define ZONEERR_LOSTTOTEM_302 15402
/* @@error 15402 ������ʧ��֮������ĵ�ͼʹ��*/

#define ZONEERR_MAP_702 15501
/* @@error 15501 ���Ҿ��޻���ڽ����У����ɽ��븱��*/

#define ZONEERR_MAP_703 15502
/* @@error 15502 �õ�ͼ�������ֽ��б�������������ܻ����ܹ���*/

#define ZONEERR_MAP_704 15503
/* @@error 15503 ����δͻ�Ʊ��㲻����ȡͻ�ƽ���*/

#define ZONEERR_MAP_705 15504
/* @@error 15504 ���Ѿ���ȡ������Ľ���*/

#define ZONEERR_MISC_301 15601
/* @@error 15601 ����һ��Ŭ����������%d��%s����ȥ���׶�˹��ȡ���Ұ�*/

#define ZONEERR_MISC_302 15602
/* @@error 15602 ֻ�������⺣̲����������Ӵ*/

#define ZONEERR_MISC_303 15603
/* @@error 15603 ���������������������ſ���Ӵ���������⺣̲ɹ�չ�ԡ������ǳ������꣺309 167��*/

#define ZONEERR_MISC_304 15604
/* @@error 15604 С�����Ѿ����ֺ�ˮ���ˣ��ǵ�ι������Ҫ�ǵ���������Ӵ*/

#define ZONEERR_MISC_305 15605
/* @@error 15605 С����û����ô��θ��Ŷ��ι̫�������Ż���Ӵ*/

#define ZONEERR_MISC_306 15606
/* @@error 15606 %s��������*/

#define ZONEERR_NPC_301 15700
/* @@error 15700 ս���������޷�����*/

#define ZONEERR_PACKAGE_301 15800
/* @@error 15800 ��ǰ��ͼ����ʹ��*/

#define ZONEERR_PACKAGE_302 15801
/* @@error 15801 һ����˳������ǰ����*/

#define ZONEERR_PACKAGE_303 15802
/* @@error 15802  ��ײ�������ˣ����ٽ����У�*/

#define ZONEERR_PACKAGE_304 15803
/* @@error 15803  ������ı���������ð���ǣ�ͷ��Ŀѣ�����˼���ǰ�У�*/

#define ZONEERR_PACKAGE_305 15804
/* @@error 15804  ˳������һ�����䣬��һ����Ȼ�׻����������ţ�*/

#define ZONEERR_PACKAGE_306 15805
/* @@error 15805  ����һ�����鱦�䣡*/

#define ZONEERR_PACKAGE_307 15806
/* @@error 15806  ��������һͨ����ӭ��������*/

#define ZONEERR_PACKAGE_308 15807
/* @@error 15807  ���е������㣬��ʱ�����������*/

#define ZONEERR_PACKAGE_309 15808
/* @@error 15808  �㻹û�м��빫��,�޷�ʹ�ù����ʽ�ȯ! */

#define ZONEERR_PACKAGE_310 15809
/* @@error 15809  ��ǰ��ͼ������ʹ�ö��鴫�;�*/

#define ZONEERR_PACKAGE_311 15810
/* @@error 15810  ��ǰ��ͼ������ʹ�ö�λ��*/

#define ZONEERR_PACKAGE_312 15811
/* @@error 15811  δ���빫�ᣬ�޷�ʹ��*/

#define ZONEERR_PLAYER_301 15900
/* @@error 15900  ѧ���Ǽ�������������ٵǼ�! */

#define ZONEERR_PLAYER_302 15901
/* @@error 15901  �Ѿ��Ǽǵ�ѧ�������ڵǼ�! */

#define ZONEERR_PLAYER_303 15902
/* @@error 15902  �Ѵﵽ��Ʊ���޷��ٹ���ó����Ʒ����������������*/

#define ZONEERR_PLAYER_304 15903
/* @@error 15903  ó�����˵�������Ʒ�ļ۸������³�����Ʒ*/

#define ZONEERR_PLAYER_305 15904
/* @@error 15904  ó�����˵�������Ʒ�ļ۸������¹�����Ʒ*/

#define ZONEERR_PLAYER_306 15905
/* @@error 15905  ��Ʒ�����⣬����ѡ������Ʒ����ȴ�ó�������ϻ�*/

#define ZONEERR_PLAYER_307 15906
/* @@error 15906  �㻨����%d��Ʊ������%d��ó����Ʒ*/

#define ZONEERR_PLAYER_308 15907
/* @@error 15907  �������%d��ó����Ʒ���%lld��Ʊ*/

#define ZONEERR_PLAYER_309 15908
/* @@error 15908  ��Ŀǰ��ó�ױ�����Ʊ��̫�٣��̻�����Ϊ����ͨ�˱���ͨ������������ó�����˶һ���Ʊ*/

#define ZONEERR_PLAYER_310 15909
/* @@error 15909  �ɹ�����ó����Ʒ���%lld��Ʊ*/


#define ZONEERR_ARM801 16001
/* @@error 16001 ��ǰ�Ǽ��Ѿ�������ǿ��!*/

#define ZONEERR_ZHENGYI_DEC		17001
/* @@error 17001 ��������%d������� */

#define ZONEERR_ZHENGYI_ADD		17002
/* @@error 17002 ������%d������� */

#define ZONEERR_LIFE_BRAND_DEC		17003
/* @@error 17003 ��������%d���潱�� */

#define ZONEERR_LIFE_BRAND_ADD		17004
/* @@error 17004 ������%d���潱�� */

#define ZONEERR_CLAN_BATTLE_WIN		17005
/* @@error 17005 %s����������֮�ǿ��ս����ʤ�������Ϊ���ǻ����� */

#define ZONEERR_CLAN_BATTLE_FAIL		17006
/* @@error 17006 %s����������֮�ǹ�����ս����ս�ܣ����Ϊ���Ǽ��Ͱ� */

#define ZONEERR_NOTINWAITQUEUE		17007
/* @@error 17007 �㲻�ڵȴ������л򹫻᲻�ٵȴ�������,�޷�����ս�� */

#define ZONEERR_PWORLDOVER		17008
/* @@error 17008 ս���ѽ���,�޷�����ս�� */

#define ZONEERR_CLAN_IN_BATTLE		17009
/* @@error 17009 �����ѱ������ս��,�޷���ɢ */

#define ZONEERR_BAOXIANG200		17100
/* @@error 17100 ���������������еĵ�����ͨ���ʼ��ķ�ʽ���͸�������ע����� */

#define ZONEERR_TEAM800		17200
/* @@error 17200 ���������������ɽ���˶���*/

#define ZONEERR_TEAM801		17201
/* @@error 17201 ���������������������κ���ҽ������*/

#define ZONEERR_TASK801		17300
/* @@error 17300 �˻ÿ���������%d��,ÿ��һ�����������ô���,���Ժ�����! */

#define ZONEERR_TASK802		17301
/* @@error 17301 �ѽ��ܻ�������[%s]����鿴������� */

#define ZONEERR_TASK803		17302
/* @@error 17302 �õ���ֻ�ܴ�������ĵ�һ�������뵽npc����ȡ�������� */

#define ZONEERR_SHOP301		17400
/* @@error 17400 ����Ʒֻ��ͬʱ����һ��*/

#define ZONEERR_SHOP302		17401
/* @@error 17401 ��ӵ�д���Ʒ*/

#define ZONEERR_SHOP303		17402
/* @@error 17402 ���ɹ������˳ɳ����������ÿ�ա������ڿ���ȡһ�ν�ȯ������30��*/

#define ZONEERR_SHOP304		17403
/* @@error 17403 ����Ʒ�޷�����*/

#define ZONEERR_SPARTA301		17500
/* @@error 17500 ĩ����ҫ����δ����*/

#define ZONEERR_SPARTA302		17501
/* @@error 17501 δ������ʱ�䣬���ܱ���ĩ����ҫ����*/

#define ZONEERR_SPARTA303		17502
/* @@error 17502 ��Ҫ3�˶�����ܱ���ĩ����ҫ����*/

#define ZONEERR_SPARTA304		17503
/* @@error 17503 ĩ����ҫ���ö��鱨����Ҫ�ӳ����ܱ���*/

#define ZONEERR_SPARTA305		17504
/* @@error 17504 ����ʧ�ܣ�ĩ����ҫ����ֻ��3�˶�����ܱ���*/

#define ZONEERR_SPARTA306		17505
/* @@error 17505 ���ѱ���ս�����޷�����ĩ����ҫ���ñ���*/

#define ZONEERR_SPARTA307		17506
/* @@error 17506 �ѱ���ĩ����ҫ���ã������ظ�����*/

#define ZONEERR_SPARTA308		17507
/* @@error 17507 ������˼�����в��ܱ���ĩ����ҫ����*/

#define ZONEERR_SPARTA309		17508
/* @@error 17508 %s�������߻���״̬���޷�����ĩ����ҫ����*/

#define ZONEERR_SPARTA310		17509
/* @@error 17509 %s�ȼ�����60�������ܽ��о���������*/

#define ZONEERR_SPARTA311		17510
/* @@error 17510 ��Ա%s�ڱ���ս���У��޷�����ĩ����ҫ����*/

#define ZONEERR_SPARTA312		17511
/* @@error 17511 %s���ڸ�����˼�����У�����ʧ��*/

#define ZONEERR_SPARTA313		17512
/* @@error 17512 %s������ĩ���۷�׼����ͼ���۷徺��������,�޷�����ĩ����ҫ����*/

#define ZONEERR_STORE301		17600
/* @@error 17600 �ʲ�����֧�����޷�,�������Զ��յ�! */

#define ZONEERR_STORE302		17601
/* @@error 17601 �����Ѹ��ʲ�,����%dСʱ%d���Ӻ��Զ��յ�,�뼰ʱԤ���㹻���ʲ�! */

#define ZONEERR_STORE303	17602
/* @@error 17602 ��Ʒ���¼ܻ���������! */

#define ZONEERR_STORE304	17603
/* @@error 17603 ���ҽ�����Я������,�޷�ȡ���ö�ȵ��ʲ� ! */

#define ZONEERR_STORE305	17604
/* @@error 17604 �ʲ���������������,�޷�����ö�ȵ�����! */

#define ZONEERR_STORE306   17605
/* @@error 17605 ���ҽ�����Я������! */

#define ZONEERR_STORE307   17606
/* @@error 17606 �鿴�ĵ��̲�����! */

#define ZONEERR_SYNCRELATION301   17700
/* @@error 17700 ����û�е�ʦ��ҵ����ʱ������Ϊѧ�� */

#define ZONEERR_SYNCRELATION302   17701
/* @@error 17701 ����������5��ѧ�� */

#define ZONEERR_SYNCRELATION303   17702
/* @@error 17702 Ŀ����Ҳ�����Χ */

#define ZONEERR_SYNCRELATION304   17703
/* @@error 17703 �ȼ���30��60֮�����Ҳ��ܱ���Ϊѧ��*/

#define ZONEERR_SYNCRELATION305   17704
/* @@error 17704 Ŀ����Ҿ����Զ*/

#define ZONEERR_SYNCRELATION306   17705
/* @@error 17705 �������Զ*/

#define ZONEERR_SYNCRELATION307   17706
/* @@error 17706 �����յ�ѧ���Ѿ�������ʦ����ϵ��*/

#define ZONEERR_SYNCRELATION308   17707
/* @@error 17707 ���뷢�ͳɹ����ȴ��Է�Ӧ��*/

#define ZONEERR_SYNCRELATION309   17708
/* @@error 17708 ��ĵȼ��Ѿ����ʺϳ�Ϊѧ��*/

#define ZONEERR_SYNCRELATION310   17709
/* @@error 17709 ��ʦ�����Զ*/

#define ZONEERR_SYNCRELATION311   17710
/* @@error 17710 ���볬ʱ*/

#define ZONEERR_SYNCRELATION312   17711
/* @@error 17711 %s�ܾ�����������*/

#define ZONEERR_SYNCRELATION313   17712
/* @@error 17712 ����Ҫ����500�����ֵ���ܽ��ʦͽ��ϵ*/

#define ZONEERR_SYNCRELATION314   17713
/* @@error 17713 ��ѧ�������Ѿ��������ɫ����! */

#define ZONEERR_SYNCRELATION315   17714
/* @@error 17714 ����ǰ�ľ��鲻�� */

#define ZONEERR_SYNCRELATION316   17715
/* @@error 17715 �����ɫ����ɹ�������ѧ����ȡ����������ô��е��� */

#define ZONEERR_SYNCRELATION317   17716
/* @@error 17716 ������ȡ�����Ѵ�����*/

#define ZONEERR_SYNCRELATION318   17717
/* @@error 17717 ����δ��65����ʱ���ܱ�ҵ*/

#define ZONEERR_SYNCRELATION319   17718
/* @@error 17718 ���ĵ�ʦ���ڸ���*/

#define ZONEERR_SYNCRELATION320   17719
/* @@error 17719 ����û�е�ʦ��ҵ����ʱ������Ϊѧ��*/

#define ZONEERR_SYNCRELATION321   17720
/* @@error 17720 ���ĵ�ʦ�����Ѿ���ҵ��%d��ѧ��������ʱ���ܿ��˱�ҵ*/

#define ZONEERR_SYNCRELATION322   17721
/* @@error 17721 ���ĵ�ʦ�����ߣ���ʱ���ܿ��˱�ҵ*/


#define ZONEERR_ATTR801	18000
/* @@error 18000 ���潱���Ѿ��ﵽ���ޣ������һ����ߺ������ȡ */

#define ZONEERR_ATTR802	18001
/* @@error 18001 ÿ�����潱�»�ȡ�Ѵﵽ����*/

#define ZONEERR_STRONG239 18100
/* @@error 18100 %d���������ֻ��ռ��%d���ݵ�!*/

#define ZONEERR_STRONG240	18101
/* @@error 18101 ��%s�����Ὺ���ˡ��ػ�������Ŧ-����ģʽ������κι����Ա����֧Ԯ��ս����ȡ�������*/

#define ZONEERR_STRONG241	18102
/* @@error 18102 ��%s������ġ��ػ�������Ŧ-����ģʽ������ݸ�һ����*/

#define ZONEERR_STRONG242	18103
/* @@error 18103 ÿ����ռ��ݵ�������һ��ս�ţ�����û�ж���ս�ſɷ��䣡*/

#define ZONEERR_STRONG243	18104
/* @@error 18104 �ݵ����ڼ�ս�����У���ʱ���ܶԾݵ�ս�Ž��е�����*/

#define ZONEERR_STRONG244	18105
/*@@error 18105  �����Ѳμӹ���һģʽ�ġ��ػ�������Ŧ������޷��ٴβμ�!*/

#define ZONEERR_CLAN336 19000
/*@@error 19000 ����ȼ����㣬�޷��з��˼���*/


#define ZONEERR_CLANPET1 19100
/*@@error 19100 �����Ա[%s]�ѽ�����[%s]�����ڹ�����г�����԰��,������λ˧����Ů�Ƕ������*/

#define ZONEERR_CLANPET2 19101
/*@@error 19101 �װ������:Ŀǰ���ҽ���������%d�ʽ��׵����ڽ��ڽ���ƽ����,�Լ۱ȳ�ֵ,����ӻԾ����!*/

#define ZONEERR_DIANFENG1 19200
/*@@error 19200 ����ʧ�ܣ��μ�ĩ���۷���������Ҫ�ȼ����ڵ���65��*/

#define ZONEERR_DIANFENG2 19201
/* @@error 19201 ����Ʒ�޷���ĩ���۷徺��������ʹ��*/

#define ZONEERR_DIANFENG3 19202
/*@@error 19202 ���Ҳ���%d���޷�֧�������ѣ�����ʧ��*/

#define ZONEERR_DIANFENG4 19203
/*@@error 19203 ĩ���۷屨���ɹ������ڱ�����׼ʱ�μӱ�����*/

#define ZONEERR_DIANFENG5 19204
/*@@error 19204 �����Ҳ���%d���޷�����ھ�����*/

#define ZONEERR_DIANFENG6 19205
/*@@error 19205 ���Ѵ����ĩ���۷�����������ʱ�䣬����Ϊ�Զ�������*/

#define ZONEERR_DIANFENG7 19206
/*@@error 19206 ĩ���۷���������������20���Ӻ��������Ҫ�����μӵ���ʿ�뾡�챨����*/

#define ZONEERR_DIANFENG8 19207
/*@@error 19207 ���ڶԷ�ͻȻ���ߣ�ϵͳ���ٴ�Ϊ�����ж���ѡ��*/

#define ZONEERR_DIANFENG9 19208
/*@@error 19208 ���Ѿ�������*/

#define ZONEERR_DIANFENG10 19209
/*@@error 19209  ������ְҵ�鱨����������*/

#define ZONEERR_DIANFENG11 19210
/*@@error 19210  �и���ְҵ�鱨����������%d �ˣ��޷���ʼ�ھ�����*/

#define ZONEERR_DIANFENG12 19211
/*@@error 19211  ����ĩ���۷�ھ�������������*/

#define ZONEERR_DIANFENG13 19212
/*@@error 19212 ���Ѿ�������*/

#define ZONEERR_DIANFENG14 19213
/*@@error 19213 ��ϲ�����ĩ���۷����������뽱������ǰ��#L�������@@[303063,1000]#n����ȡ������*/

#define ZONEERR_DIANFENG15 19214
/*@@error 19214 %s���ĩ���۷��������ھ�!*/

#define ZONEERR_DIANFENG16 19215
/*@@error 19215 %s*/

#define ZONEERR_DIANFENG17 19216
/*@@error 19216  �˵�ͼ�� �����͵�ĩ���۷�׼������!*/

#define ZONEERR_DIANFENG18 19217
/*@@error 19217  �������޷�����!*/

#define ZONEERR_DIANFENG19 19218
/*@@error 19218 ���״̬�޷�����*/

#define ZONEERR_DIANFENG20 19219
/*@@error 19219 �������ڱ���������ʱ����������Ƶ����ϵͳ��Ĭ��Ϊ�Զ�����!*/

#define ZONEERR_DIANFENG21 19220
/*@@error 19220 ���ڶԷ�����ʱ������������Զ���������һ�ֱ�����*/

#define ZONEERR_DIANFENG22 19221
/*@@error 19221 ��ȡ��ҽ����Ѿ�ͨ���ʼ�����!*/

#define ZONEERR_DIANFENG23 19222
/*@@error 19222 ���״̬��ս������״̬���ܽ����۷崫�Ͳ���*/

#define ZONEERR_DIANFENG24 19223
/* @@error 19223 ĩ���۷�׼����ͼ�л��۷徺�������в��ܱ���ս��*/

#define ZONEERR_DIANFENG25 19224
/* @@error 19224 %s����ĩ���۷�׼����ͼ�л��۷徺��������, �޷�����ս��*/

#define ZONEERR_DIANFENG26 19225
/* @@error 19225 ĩ���۷����������ڼ�ս��,���¹����ѹر�*/

#define ZONEERR_DIANFENG27 19226
/* @@error 19226 ĩ���۷���������δ����,���¹�����ʱ�ر�*/

#define ZONEERR_DIANFENG28 19227
/* @@error 19227 ĩ���۷����������ڼ�ս��,���������ѹر�*/

#define ZONEERR_DIANFENG29 19228
/* @@error 19228 ĩ���۷���������δ����,����������ʱ�ر�*/

#define ZONEERR_DIANFENG30 19229
/* @@error 19229 ����ѡ���ְҵ��û���˱������������Ժ�������*/

#define ZONEERR_DIANFENG31 19230
/* @@error 19230 �˴���ħ���ų������˴��ͣ���ǰ��������ͼ���д���!*/

#define ZONEERR_DIANFENG32 19231
/* @@error 19231 %s ���ڲ��ɴ��͵�ͼ!*/

#define ZONEERR_DIANFENG33 19232
/* @@error 19232 ����ĩ���۷�׼����ͼ���۷徺��������,���ɽ��븱��*/

#define ZONEERR_DIANFENG34 19233
/*@@error 19233 ���ڶԷ����������������Զ���������һ�ֱ�����*/

#define ZONEERR_DIANFENG35 19234
/*@@error 19234 ���ս��㣬ֹͣ�ϳ�*/

#define ZONEERR_DIANFENG36 19235
/* @@error 19235 ����ս��״̬, �����Դ���*/

#define ZONEERR_DIANFENG37 19236
/* @@error 19236 ���ڶ���״̬, �����Դ���*/

#define ZONEERR_DIANFENG38 19237
/* @@error 19237 ����׼����ͼ���޷����д��ͣ�������������ٽ��д��ͣ�*/

#define ZONEERR_DIANFENG39 19238
/*@@error 19238 ���ڶԷ����������������Զ���������һ�ֱ�����*/

#define ZONEERR_DIANFENG40 19239
/*@@error 19239 ���ڶԷ�����������ϵͳ���ٴ�Ϊ�����ж���ѡ��*/

#define ZONEERR_DIANFENG41 19240
/*@@error 19240 �����Զ�����ĩ���۷����*/

#define ZONEERR_TANK1 19300
/*@@error 19300 �㱻�з���%s����*/

#define ZONEERR_TANK2 19301
/*@@error 19301 %s�ڷ�ɧ�Ĳ����£�һ�ڻ����˵з�%d��ս����*/

#define ZONEERR_TANK3 19302
/*@@error 19302 %s��ֱΪս��������һ�ڻ����˵з�%d��ս����*/

#define ZONEERR_TANK4 19303
/*@@error 19303 %sд�����µ��񻰣�һ�ڻ����˵з�%d��ս����*/

#define ZONEERR_TANK5 19304
/*@@error 19304 %s�ѳ�Խ�˵����ˣ�һ�ڻ����˵з�ȫ��ս������*/


#define ZONEERR_CLAN_PET1 19401
/*@@error 19401 ����%s����%d��Ǳ��ֵ*/

#define ZONEERR_CLAN_PET2 19402
/* @@error 19402 ����%s ����!���ܸ���!*/

#define ZONEERR_CLAN_PET3 19403
/* @@error 19403 ������������ֹʹ��!*/


#define ZONEERR_SHOP_LIMIT1 19501
/* @@error 19501 %d��֮��ֻ�ܹ���һ��%s*/

#define ZONEERR_SHOP_LIMIT2 19502
/* @@error 19502 �ý�ɫ�Ѿ�ӵ�иõ��ߣ��뾡��ʹ�ú���ܹ���*/

#define ZONEERR_SHOP_LIMIT3 19503
/* @@error 19503 %sһ��ֻ�ܹ���һ��*/

#define ZONEERR_SHOP_LIMIT4 19504
/* @@error 19504 %s��������*/


#define ZONEERR_PET195 19505
/* @@error 19505 �˵���ֻ�ܶԳ�ս����ʹ�ã��뽫����״̬�޸�Ϊ��ս��*/

#define ZONEERR_PET196 19506
/* @@error 19506 ֻ��%s�ĳ������ʹ��%s!*/


#define ZONEERR_WEDDING1 19507
/* @@error 19507 %s�������ĺ���,�޷���Է�����%s*/

#define ZONEERR_WEDDING2 19508
/* @@error 19508 ������%s�ĺ���,�޷���Է�����%s*/

#define ZONEERR_WEDDING3 19509
/* @@error 19509 ����%s�ľ���̫Զ,�޷���Է�����%s*/

#define ZONEERR_WEDDING4 19510
/* @@error 19510 �����������¹�ϵ,�޷�����Է�����%s*/

#define ZONEERR_WEDDING5 19511
/* @@error 19511 �������Ƿ��޹�ϵ,�޷�����Է�����%s*/

#define ZONEERR_WEDDING6 19512
/* @@error 19512 %s���ҵ������еİ���,�޷�����Է�����%s*/

#define ZONEERR_WEDDING7 19513
/* @@error 19513 ����ӵ�������еİ���,�޷�����Է�����%s*/

#define ZONEERR_WEDDING8 19514
/* @@error 19514 �Է�������!*/

#define ZONEERR_WEDDING9 19515
/* @@error 19515 ���ǵ�ǰΪ����,�޷����Է�����������޳�*/

#define ZONEERR_WEDDING10 19516
/* @@error 19516 ���ǵ�ǰΪ����,�޷����Է�����������޳�*/

#define ZONEERR_WEDDING11 19517
/* @@error 19517 ���ź�����Ȼ����,��ʱ�޷����ƻ����ײ�*/

#define ZONEERR_WEDDING12 19518
/* @@error 19518 ��ʱ�޷����ƻ����ײͣ���Ҫ˫�����ܶȴﵽ%d*/

#define ZONEERR_WEDDING13 19519
/* @@error 19519 %s�ƺ������ڳ�,��ʱ�޷����ƻ����ײ�*/

#define ZONEERR_WEDDING14 19520
/* @@error 19520 %s�ƺ���δ����,��ʱ�޷����ƻ����ײ�*/

#define ZONEERR_WEDDING15 19521
/* @@error 19521 �����Ѿ������˻����ײ�,�޷��ٶ��ƻ����ײ�*/

#define ZONEERR_WEDDING17 19523
/* @@error 19523 ���ź�����Ȼ����,��ʱ�޷���ʼ����*/

#define ZONEERR_WEDDING18 19524
/* @@error 19524 ��ʱ�޷���ʼ�ٰ������Ҫ˫�����ܶȴﵽ%d*/

#define ZONEERR_WEDDING19 19525
/* @@error 19525 %s�ƺ������ڳ�,��ʱ�޷���ʼ����*/

#define ZONEERR_WEDDING20 19526
/* @@error 19526 %s�ƺ���δ����,��ʱ�޷���ʼ����*/

#define ZONEERR_WEDDING21 19527
/* @@error 19527 ���ǵĻ����Ѿ���ʼ��,�޷������¿�ʼ����*/

#define ZONEERR_WEDDING22 19528
/* @@error 19528 %s�ƺ������ڳ�,��ʱ�޷����л������*/

#define ZONEERR_WEDDING23 19529
/* @@error 19529 ���ź��㲢û�а���,�޷���ʼ�������*/

#define ZONEERR_WEDDING24 19530
/* @@error 19530 �ƺ���һ���������ھ��л������,��������پ��л������*/

#define ZONEERR_WEDDING25 19531
/* @@error 19531 �����ھ��л���������,��ʱ�޷����л������*/

#define ZONEERR_WEDDING26 19532
/* @@error 19532 �ƺ���һ���������ھ��л���Ѳ��,��������پ��л���Ѳ��*/

#define ZONEERR_WEDDING27 19533
/* @@error 19533 �����ھ��л��������,��ʱ�޷����л�������*/

#define ZONEERR_WEDDING28 19534
/* @@error 19534 ������δЯ��%s , ��ʱ�޷������ϵ*/

#define ZONEERR_WEDDING29 19535
/* @@error 19535 %s����˷���,��������ο̹����ĵĸ���!*/

#define ZONEERR_WEDDING30 19536
/* @@error 19536 �Է�������%s !*/

#define ZONEERR_WEDDING31 19537
/* @@error 19537 �������Ѿ��з��޶�����,�޷��ٶ��ƻ����ײ�*/

#define ZONEERR_WEDDING32 19538
/* @@error 19538 ���ǻ�û���ƻ����ײ�,���ܿ�ʼ����*/

#define ZONEERR_WEDDING33 19539
/* @@error 19539 ���ź��㲢û�а���*/

#define ZONEERR_WEDDING34 19540
/* @@error 19540 %s�ƺ������ڳ�,��ʱ�޷����л�������*/

#define ZONEERR_WEDDING35 19541
/* @@error 19541 ��Ҫ���Ƽ�Լ�����ײͻ��ߺ��������ײͲſ��Խ��л������*/

#define ZONEERR_WEDDING36 19542
/* @@error 19542 ��Ҫ���ƺ��������ײͲſ��Խ��л�������*/

#define ZONEERR_WEDDING37 19543
/* @@error 19543 %sί��ܾ�����ı��*/

#define ZONEERR_WEDDING38 19544
/* @@error 19544 %s�ܾ������Եĳ�ŵ,�ƺ���δ����̤�������׼��*/

#define ZONEERR_WEDDING39 19545
/* @@error 19545 �����ھ��л���������,��ʱ�޷������ϵ*/

#define ZONEERR_WEDDING40 19546
/* @@error 19546 �����ھ��л��������,��ʱ�޷������ϵ*/

#define ZONEERR_WEDDING41 19547
/*@@error 19547  ����ʥ�顢ʥ����ʥ�ӵ���������:%s��%s��Ϊ����.����,���ɿ�������������*/

#define ZONEERR_WEDDING42 19548
/* @@error 19548 �������Ǿۼ�,���ϵۺ���������ǰ,��֤%s��%s���������ʥ�Ļ���*/

#define ZONEERR_WEDDING43 19549
/* @@error 19549 %s��%s�����⺣̲������¡�صĻ������!*/

#define ZONEERR_WEDDING44 19550
/* @@error 19550 %s��%s�����������ܳ����۵Ļ���ʱ���!*/

#define ZONEERR_WEDDING45 19551
/* @@error 19551 ��һƬ����Ц����,%s��%s�Ļ������Բ������!*/

#define ZONEERR_WEDDING46 19552
/* @@error 19552 �����Ŵ�ҵ�ף��,%s��%s�Ļ�������Բ������!*/

#define ZONEERR_WEDDING47 19553
/* @@error 19553 %s ������%s��׷��,�Ӵ˳�Ϊһ�������మ������!*/

#define ZONEERR_WEDDING48 19554
/* @@error 19554 ���Ѿ��а�����*/

#define ZONEERR_WEDDING49 19555
/* @@error 19555 ����ǰ���ڵĵ�ͼ��ʱ�޷�����*/

#define ZONEERR_WEDDING50 19556
/* @@error 19556 ����ʱ�޷����͵��Է����ڵĵ�ͼ*/

#define ZONEERR_WEDDING51 19557
/* @@error 19557 ���ź�����Ȼ����!*/

#define ZONEERR_WEDDING52 19558
/* @@error 19558 �����Ѿ����й��˻�����䣬�޷��پ��л������!*/

#define ZONEERR_WEDDING53 19559
/* @@error 19559 �����Ѿ����й��˻������Σ��޷��پ��л�������!*/

#define ZONEERR_WEDDING54 19560
/* @@error 19560 ����%s����˷��֣���������ο̹����ĵĸ���!*/

#define ZONEERR_WEDDING55 19561
/* @@error 19561 %sֻ�н������¹�ϵ����ܴ���!*/

#define ZONEERR_WEDDING56 19562
/* @@error 19562 %sֻ�н������޹�ϵ����ܴ���!*/

#define ZONEERR_WEDDING57 19563
/* @@error 19563 %sֻ�н������»���޹�ϵ����ܴ���!*/

#define ZONEERR_WEDDING58 19564
/* @@error 19564 ����%s�����ܶ�������%d��!*/

#define ZONEERR_WEDDING59 19565
/* @@error 19565 ����%s�����ܶȼ�����%d��!*/

#define ZONEERR_WEDDING60 19566
/* @@error 19566 ��ʱ�޷���Ϊ���£���Ҫ˫�����ܶȴﵽ%d*/

#define ZONEERR_WEDDING61 19567
/* @@error 19567 %s�ɹ�����������ײͣ�������������и��ײ͵Ļ���*/

#define ZONEERR_WEDDING62 19568
/* @@error 19568 %s�ɹ������˼�Լ�ײͣ�������������и��ײ͵Ļ���*/

#define ZONEERR_WEDDING63 19569
/* @@error 19569 %s�ɹ������˺����ײͣ�������������и��ײ͵Ļ���*/

#define ZONEERR_WEDDING64 19570
/* @@error 19570 �㽻���˼�������������ö%s����ʼ��һ���µĻ���*/

#define ZONEERR_WEDDING65 19571
/* @@error 19571 ��ʼ�����������ʽǰ����Ҫ%s ��ʾ���ƻ���ʱ��%s*/

#define ZONEERR_WEDDING66 19572
/* @@error 19572 ���%s�����ܶ���������ʱ����Ҫ�������ܶ�*/

#define ZONEERR_WEDDING67 19573
/* @@error 19573 %s���������ף��޷���Է�����%s*/

#define ZONEERR_WEDDING68 19574
/* @@error 19574 %s�Ѿ��ҵ������£��޷��ٽ������Ļ�Ӧ*/

#define ZONEERR_WEDDING69 19575
/* @@error 19575 %s�Ѿ��ҵ��˰��£��޷��ٽ������Ļ�Ӧ*/

#define ZONEERR_WEDDING70 19576
/* @@error 19576 ���Ѿ��ҵ������£��޷��ٽ���%s�ı��*/

#define ZONEERR_WEDDING71 19577
/* @@error 19577 ���Ѿ��ҵ��˰��£��޷��ٽ���%s�ı��*/

#define ZONEERR_WEDDING72 19578
/* @@error 19578 ��İ�������˼�����У���ʱ�޷�ʹ�øü��ܴ��͹�ȥ*/

#define ZONEERR_WEDDING73 19579
/* @@error 19579 ��İ��²�����ߣ���ʱ�޷�ʹ�øü���*/

#define ZONEERR_WEDDING74 19580
/* @@error 19580 ������������ս���У���ʱ�޷�ʹ�øü���*/

#define ZONEERR_WEDDING75 19581
/* @@error 19581 �ܳ���̫���������������������*/

#define ZONEERR_WEDDING76 19582
/* @@error 19582 ���ǵ�ǰΪ���£��޷����Է�������ĺ�������*/

#define ZONEERR_WEDDING77 19583
/* @@error 19583 ���ǵ�ǰΪ���£��޷����Է�������ĺ�������*/

#define ZONEERR_WEDDING78 19584
/* @@error 19584 ��İ����Ѳ�����������ʱ�޷�ʹ�øü���*/

#define ZONEERR_WEDDING79 19585
/* @@error 19585 ������˼�����У���ʱ�޷�ʹ�øü��ܴ��͹�ȥ*/

#define ZONEERR_WEDDING80 19586
/* @@error 19586 %s������ĺ��ѣ���ʱ�޷����ܶԷ��ı��*/

#define ZONEERR_WEDDING81 19587
/* @@error 19587 �㲢��%s�ĺ��ѣ���ʱ�޷����ܶԷ��ı��*/

#define ZONEERR_WEDDING82 19588
/* @@error 19588 ����%s�ľ���̫Զ����ʱ�޷����ܶԷ��ı��*/

#define ZONEERR_WEDDING83 19589
/* @@error 19589 ��ָ��˵���������Ƶ��ײ��嵥�ѷ�������*/

#define ZONEERR_WEDDING84 19590
/* @@error 19590 ����һ���µĻ�������Ҫ�ջ�%s��%s����ѽ�ָЯ���ڱ�����*/

#define ZONEERR_RONGYAN1 19600
/* @@error 19600 ̫��ϧ�ˣ���ѡ���boss�Ǹ�ð�ƻ�������ķ�ŭ����й�����ð�ƻ��ϰɣ�����������������*/

#define ZONEERR_RONGYAN2 19601
/* @@error 19601 �����ɢ�����Ҿ��޻ʧ��*/

#define ZONEERR_RONGYAN3 19602
/* @@error 19602 δ�������Ҿ��޴��ʱ���ڻ�ɱ���ʧ��*/

#define ZONEERR_RONGYAN4 19603
/* @@error 19603 ���ĵ���:%sx%d!*/

#define ZONEERR_RONGYAN5 19604
/* @@error 19604 ��װ����������!*/

#define ZONEERR_RONGYAN6 19605
/* @@error 19605 �㲻�ڴ˶�����,�޷����д˲���*/

#define ZONEERR_RONGYAN7 19606
/* @@error 19606 �㲻�ڹ��������,�޷����д˲���*/

#define ZONEERR_RONGYAN8 19607
/* @@error 19607 ��������Ѳ�����,�޷����д˲���*/

#define ZONEERR_RONGYAN9 19608
/* @@error 19608 ������в����Ŷ�״̬,�޷����д˲���*/

#define ZONEERR_RONGYAN10 19609
/* @@error 19609 �Ҳ�����Ӧ���ŶӶ���,�޷����д˲���*/

#define ZONEERR_RONGYAN11 19610
/* @@error 19610 ��û��Ȩ�޽��д˲���*/

#define ZONEERR_RONGYAN12 19611
/* @@error 19611 ��������,�޷�����*/

#define ZONEERR_RONGYAN13 19612
/* @@error 19612 δ֪�Ķ��е�λ����*/

#define ZONEERR_RONGYAN14 19613
/* @@error 19613 ����ǰ��֮������Ϊ:��%d��,���ܽ����%d��!*/

#define ZONEERR_RONGYAN15 19614
 /* @@error 19614 ��е�������Ĵ���,�뾡�첹������!*/

#define ZONEERR_RONGYAN16 19615
/* @@error 19615 %s��ʼȫ���ٻ��ռ����Ҿ��ޣ�%d���Ӻ󽫳�����%s�������ɱ����ȡ�޶�齱��*/

#define ZONEERR_RONGYAN17 19616
/* @@error 19616 ��%s�Ĵ����£��ɹ���ɱ%d���ռ����Ҿ��ޣ����в����߶�����˾޶��*/

#define ZONEERR_RONGYAN18 19617
/* @@error 19617 �����Ѿ��������ٻ��ռ����Ҿ�����*/

#define ZONEERR_RONGYAN19 19618
/* @@error 19618 ��ʱ�����ٻ��ռ����Ҿ��ޣ����Ժ��ٻ�*/

#define ZONEERR_RONGYAN20 19619
/* @@error 19619 �ռ����Ҿ����Ѿ�������%s������ʿ��ǰ����ɱ*/

#define ZONEERR_RONGYAN21 19620
/* @@error 19620 ֻ����12:00-24:00 ֮���ٻ��ռ����Ҿ���*/

#define ZONEERR_ARM_UNREAL1 19700
/* @@error 19700 �û����οո�������벻�����ױ���*/

#define ZONEERR_ARM_UNREAL2 19701
/* @@error 19701 Ŀ�걳�οո���ֻ����������ױ���*/

#define ZONEERR_ARM_UNREAL3 19702
/* @@error 19702 �û��ɹ�!*/

#define ZONEERR_ARM_UNREAL4 19703
/* @@error 19703 װ���ѹ��ڣ����ܽ��д˲���*/

#define ZONEERR_ARM_UNREAL5  19704
/* @@error 19704 �����е�װ�����ܻû�*/

#define ZONEERR_ARM_UNREAL6  19705
/* @@error 19705 ֻ�����뱳�ε���*/

#define ZONEERR_ARM_UNREAL7  19706
/* @@error 19706 �û�ʧ��!*/

#define ZONEERR_ARM_LEVELUP1  19750
/* @@error 19750 ����ʧ�ܣ���װ��������Ҫ%d��!*/

#define ZONEERR_PET_UNBIND 19800
/* @@error 19800 %s���㣬���ܽ��г��������*/

#define ZONEERR_SPAN_MAP1 19900
/* @@error 19900 ���ѽ�������ͼ���˵�ͼ��ս��ģʽ�Է�Ϊ��λ��*/

#define ZONEERR_SPAN_MAP2 19901
/* @@error 19901 %s���ڲ�ͬ�����������ߣ��������*/

#define ZONEERR_SPAN_STRONG1 20000
/* @@error 20000 ����ݵ�ս֮���ɻ᳤����*/

#define ZONEERR_SPAN_STRONG2 20001
/* @@error 20001 ���Ĺ��������ﵽ����!*/

#define ZONEERR_SPAN_STRONG3 20002
/* @@error 20002 ��Ĺ����Ѿ����Ĺ��˾ݵ���!*/

#define ZONEERR_SPAN_STRONG4 20003
/* @@error 20003 ͬʱֻ�ܾ���%d������ݵ�!*/

#define ZONEERR_SPAN_STRONG5 20004
/* @@error 20004 ���Ҳ���!����ʧ��!*/

#define ZONEERR_SPAN_STRONG6 20005
/* @@error 20005 ����δռ���κο���ݵ㣬�޷���ȡռ�콱��!*/

#define ZONEERR_SPAN_STRONG7 20006
/* @@error 20006 ռ�콱��ֻ�л᳤����ȡ!*/

#define ZONEERR_SPAN_STRONG8 20007
/* @@error 20007 �����ռ���ʽ�����ȡ,�޷��ظ���ȡ*/

#define ZONEERR_SPAN_STRONG9 20008
/* @@error 20008 ��ȡ�ɹ�!*/

#define ZONEERR_SPAN_STRONG10 20009
/* @@error 20009 �ǿɲ�սս�ų�Ա!*/

#define ZONEERR_CLAN_PET01 20030
/* @@error 20030 ���ڹ��ή��,�������ĳ����ѱ��ջ�!*/

#define ZONEERR_CLAN_PET02 20031
/* @@error 20031 �����Ѿ���߳ɳ��ȼ���!*/

#define ZONEERR_CLAN_PET03 20032
/* @@error 20032 ���ڳ�ս(����)�ĳ����޷�������,���Ƚ������ٻ�!*/

#define ZONEERR_CLAN_PET04 20033
/* @@error 20033 ������һ��,����λ���г�����ס,��ѡ��������λ!*/

#define ZONEERR_CLAN_PET05 20034
/* @@error 20034 ͬʱֻ�ܼ���һֻ����!*/

#define ZONEERR_CLAN_PET06 20035
/* @@error 20035 ���������λ����!*/

#define ZONEERR_CLAN_PET07 20036
/* @@error 20036 ���ṱ�׶Ȳ���,�޷�����!*/

#define ZONEERR_CLAN_PET08 20037
/* @@error 20037 ���Ҳ���,�޷�����*/

#define ZONEERR_CLAN_PET09 20038
/* @@error 20038 �߼�����ʱ������!*/

#define ZONEERR_CLAN_PET10 20039
/* @@error 20039 ����ֵ����,�޷�����!*/

#define ZONEERR_CLAN_PET11 20040
/* @@error 20040 ���ĳ��� %s �����ɹ�!*/

#define ZONEERR_CLAN_PET12 20041
/* @@error 20041 �ó����Ѵ��ڡ���ֹ������������״̬���޷�����ιʳ����ˣ�����գ�!*/

#define ZONEERR_CLAN_PET13 20042
/* @@error 20042 �����ռ䲻��!*/

#define ZONEERR_CLAN_PET14 20043
/* @@error 20043 Ϊ�˸�л������,%s �ó�һ�������͸���,���������������Ŷ!*/

#define ZONEERR_CLAN_PET15 20044
/* @@error 20044 ���ﻶ�ֶ�����%d��!*/

#define ZONEERR_CLAN_PET16 20045
/* @@error 20045 ȡ�������ɹ�,ʣ��%d����ֵ�ѷ�������,��ӭ���ٴι��ٳ�����԰!*/

#define ZONEERR_CLAN_PET17 20046
/* @@error 20046 ȡ���ɹ�!*/

#define ZONEERR_CLAN_PET18 20047
/* @@error 20047 �����Ͷ���ʵ�ѷ������ı�����!*/

#define ZONEERR_CLAN_PET19 20048
/* @@error 20048 ���ﻶ�ֶȽϵ�,���ܽ��й���,�����������ˣһ�����!*/

#define ZONEERR_CLAN_PET20 20049
/* @@error 20049 ʱ��̫����,������Ԥ����90�����ó������һ�ݹ���!*/

#define ZONEERR_CLAN_PET21 20050
/* @@error 20050 ������Ѿ�����,����ȡ����!*/

#define ZONEERR_CLAN_PET22 20051
/* @@error 20051 �������ڴ�!*/

#define ZONEERR_CLAN_PET23 20052
/* @@error 20052 ����û��60���Ӳ�������!*/

#define ZONEERR_CLAN_PET24 20053
/* @@error 20053 ��������ɣ����Ѿ���ʣ�޼���!*/

#define ZONEERR_CLAN_PET25 20054
/* @@error 20054 ���Ѿ����չ�����Ӵ����ֻ���������հ�!*/

#define ZONEERR_CLAN_PET26 20055
/* @@error 20055 ���Ҳ���,�޷�����!*/

#define ZONEERR_CLAN_PET27 20056
/* @@error 20056 ����%d����,���ճɹ�!*/

#define ZONEERR_CLAN_PET28 20057
/* @@error 20057 ����%d����,���ճɹ�!���ﻶ�ֽϵ�,���ս������!*/

#define ZONEERR_CLAN_PET29 20058
/* @@error 20058 ����%d����,����ʧ��!�����±���Խ��,Խ�������ճɹ�Ŷ!*/

#define ZONEERR_CLAN_PET30 20059
/* @@error 20059 �����Ѿ��ܱ���,�Բ��¸��ණ����,���Ժ�����ιʳ!*/

#define ZONEERR_CLAN_PET31 20060
/* @@error 20060 ιʳ�ɹ�,���� %s ʳ��1��!*/

#define ZONEERR_CLAN_PET32 20061
/* @@error 20061 �������ڼ�����,��ǰ���������ȡ�������ſɽ�����������*/

#define ZONEERR_CLAN_PET33 20062
/* @@error 20062 ������δ����!*/

#define ZONEERR_CLAN_PET34 20063
/* @@error 20063 ������������,�㲻������!*/

#define ZONEERR_CLAN_PET35 20064
/* @@error 20064 ����ɹ�!*/

#define ZONEERR_HOME1 20080
/* @@error 20080 ȱ�ٵ���:%s!*/

#define ZONEERR_CARERR_ALTER1 20200
/* @@error 20200 ְҵת���ɹ�*/

#define ZONEERR_CARERR_ALTER2 20201
/* @@error 20201 �����ڴ���������е�У���ʱ�޷�ת��ְҵ*/

#define ZONEERR_CARERR_ALTER3 20202
/* @@error 20202 ������ʹ�������Ƽ��У���ʱ�޷�ת��ְҵ*/

#define ZONEERR_CARERR_ALTER4 20203
/* @@error 20203 ������ϻ�������װ����ʱװ����ʱ�޷�ת��ְҵ*/

#define ZONEERR_CARERR_ALTER5 20204
/* @@error 20204 1Сʱ��ֻ��ת��ְҵ1�Σ���ʱ�޷�ת��ְҵ*/

#define ZONEERR_PET701 20205
/* @@error 20205  ���츳�鲻�ܸ��ó�ս�ĳ���ʹ�ã���ѡ���Ӧ�ĳ��*/

#define ZONEERR_PET702 20206
/* @@error 20206  �������Բ���ȫ������!*/

#define ZONEERR_PET703 20207
/* @@error 20207  ��װ��û�м�������!*/

#define ZONEERR_RUNE_1 20300
/* @@error 20300 �÷��ļ���ֻ��ͨ����Ƕ#%s��%s#����*/

#define ZONE_BOSS_BAOXING_1 20340
/* @@error 20340 �ȼ�����ﵽ%d�����ܿ�������*/

#define ZONEERR_SHIKONG_1 20350
/* @@error 20350 �ѳ������롾%s������ʱ�䣬�������Ŷӣ�*/

#define ZONEERR_SHIKONG_2 20351
/* @@error 20351 ��%s���Ŷ���ȡ����*/

#define ZONEERR_SHIKONG_3 20352
/* @@error 20352 �����ڵȴ������У��޷�����*/

#define ZONEERR_SHIKONG_4 20353
/* @@error 20353 ����״̬���ܽ���*/

#define ZONEERR_SHIKONG_5 20354
/* @@error 20354 ��̯״̬���ܽ���*/

#define ZONEERR_SHIKONG_6 20355
/* @@error 20355 ��е״̬�²��ܽ���*/

#define ZONEERR_SHIKONG_7 20356
/* @@error 20356 %s �ȼ������ϣ��޷�����*/

#define ZONEERR_SHIKONG_8 20357
/* @@error 20357 %s�ѱ���ĩ����ҫ���ã��޷����б���*/

#define ZONEERR_SHIKONG_9 20358
/* @@error 20358 %s Ϊ˼�����к�����ң����ܱ���*/

#define ZONEERR_SHIKONG_10 20359
/* @@error 20359 %s����ĩ���۷�׼����ͼ�л��۷徺��������, �޷�����*/

#define ZONEERR_SHIKONG_11 20360
/* @@error 20360 ��ĵȼ������ϣ��޷�����*/

#define ZONEERR_SHIKONG_12 20361
/* @@error 20361 �����ɹ�!*/

#define ZONEERR_SHIKONG_13 20362
/* @@error 20362 �ѱ���ĩ����ҫ���ã��޷����б���*/

#define ZONEERR_SHIKONG_14 20363
/* @@error 20363 %s���ڱ���ͼ������ʧ��!*/

#define ZONEERR_SHIKONG_15 20364
/* @@error 20364 ս��״̬���ܽ���*/


#define ZONEERR_HOME_01 20400
/* @@error 20400 ����δ���ѻ����Ѿ���ֲ��ֲ��*/

#define ZONEERR_HOME_02 20401
/* @@error 20401 �����Ѿ���ȡ����*/

#define ZONEERR_HOME_03 20402
/* @@error 20402 �����ռ䲻��*/

#define ZONEERR_HOME_04 20403
/* @@error 20403 �����Լ���԰����*/

#define ZONEERR_HOME_05 20404
/* @@error 20404 ���Ҳ���!*/

#define ZONEERR_HOME_06 20405
/* @@error 20405 %s �ѽ���԰����Ϊ����!*/

#define ZONEERR_HOME_07 20406
/* @@error 20406 ���Ѿ�ӵ����һ����԰!*/

#define ZONEERR_HOME_08 20407
/* @@error 20407 %s �Ѿ�ȡ������!*/

#define ZONEERR_HOME_09 20408
/* @@error 20408 ���ﱥʳ�ȴﵽ����!*/

#define ZONEERR_HOME_10 20409
/* @@error 20409 �������ȴﵽ����!*/

#define ZONEERR_HOME_11 20410
/* @@error 20410 �������ȴﵽ���� !*/

#define ZONEERR_HOME_12 20411
/* @@error 20411 ���｡���ȴﵽ����!*/

#define ZONEERR_HOME_13 20412
/* @@error 20412 ��ǰ�ȼ��˵ض�Ӧ�Ŀ������ﵽ����!*/

#define ZONEERR_HOME_14 20413
/* @@error 20413 ����ȼ�̫��,�޷��ջ�!*/

#define ZONEERR_HOME_15 20414
/* @@error 20414 ��ǰ�˵صȼ��������޷�����*/

#define ZONEERR_HOME_16 20415
/* @@error 20415 �˵صȼ�����,�޷���ֲ������*/

#define ZONEERR_SUBJECT_01 20500
/* @@error 20500 �������������������Ľ�����ͨ���ʼ��ķ�ʽ���͸�������ע����� */

#define ZONEERR_SUBJECT_02 20501
/* @@error 20501 ��ϲ���õ�%d��,���%d��%s*/

#define ZONEERR_SUBJECT_03 20502
/* @@error 20502 ��ϲ���%s�����������л�õ�һ��,����%d��%s*/

#define ZONEERR_SUBJECT_04 20503
/* @@error 20503 �Ѵ��ھ���ӳɵĵ���Ч������ʱ����ʹ�øõ���*/

#define ZONEERR_RUNE231 20510
/* @@error 20510 ��ɫ�ﵽ%d�����ɼ���÷��ļ���*/


#ifdef __cplusplus
extern "C"
{
#endif

	HERR err_init(const char* pszPath, const char* pszLang, int iFlags);
	void err_release(void);

#ifdef __cplusplus
}
#endif

#endif /* ZONE_ERR_H */


