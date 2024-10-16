/*
 *	decoder.cpp
 *  game script command decoder functions.
 *
 *  Jim, 2003.8.19.
 */
#include <stdio.h>
#include "script.h"
#include "decoder.h"
#include "mcdef.h"

/*added by kent 2004.7.26, to test logic expression script*/
#ifdef TEST_LOGIC_EXP
	extern FILE *testfp;
#endif

int my_script_decoder(unsigned long code)
{
    int result;
    int num;
    char *str;

    //result = GAME_CONTINUE;
    result = 0;
    switch(code)
    {
    case CMD_RESET_SCRIPT:
        //result = decode_cmd_reset_script();
        break;
    case CMD_EXEC_SCRIPT:
        //result = decode_cmd_exec_script();
        break;
    case CMD_BREAK_SCRIPT:
        //result = decode_cmd_break_script();
        break;
    case CMD_CLEAR_TALK:
        //result = decode_cmd_clear_talk();
		debug_log(0, 0, "CLEAR_TALK");

#ifdef TEST_LOGIC_EXP
		fputs("CLEAR_TALK\n", testfp);
		fputs("\n", testfp);
#endif

        break;
    case CMD_ADD_TALK:
        num = get_script_number(NULL);
        //str = get_usercmd_script_string();
        //debug_log(0, 0, "ADD_TALK %d %s", num, str);
		debug_log(0, 0, "ADD_TALK %d", num);

#ifdef TEST_LOGIC_EXP
		fputs("ADD_TALK\n", testfp);
		fputs("\n", testfp);
#endif
		
        break;
    case CMD_CLEAR_ANI:
        //result = decode_cmd_clear_ani();

#ifdef TEST_LOGIC_EXP
		fputs("CMD_CLEAR_ANI\n", testfp);
		fputs("\n", testfp);
#endif
        break;
    case CMD_ADD_RLEANI:
        //result = decode_cmd_add_rleani();

#ifdef TEST_LOGIC_EXP
		fputs("CMD_ADD_RLEANI\n", testfp);
		fputs("\n", testfp);
#endif
		
        break;
    case CMD_ADD_GIFANI:
        //result = decode_cmd_add_gifani();
        break;
    case CMD_CLEAR_MAGIC:
        //result = decode_cmd_clear_magic();

#ifdef TEST_LOGIC_EXP
		fputs("CMD_CLEAR_MAGIC\n", testfp);
		fputs("\n", testfp);
#endif
        break;
    case CMD_ADD_MAGIC:
        //result = decode_cmd_add_magic();
        break;
    case CMD_ADD_LOOPMAGIC:
        //result = decode_cmd_add_loopmagic();
        break;
    case CMD_NPC_POSITION:
        //result = decode_cmd_npc_position();
        break;
    case CMD_NPC_DIR:
        //result = decode_cmd_npc_dir();
        break;
    case CMD_NPC_SHOW:
        //result = decode_cmd_npc_show();
        break;
    case CMD_BALLOON_HIKEUP:
        //result = decode_cmd_balloon_hikeup();
        break;
    case CMD_HEART_WAVEUP:
        //result = decode_cmd_heart_waveup();
        break;
    case CMD_STAR_FALL:
        //result = decode_cmd_star_fall();
        break;
    case CMD_COLORTIE_DANCE:
        //result = decode_cmd_colortie_dance();
        break;
    case CMD_GIFT_FALL:
        //result = decode_cmd_gift_fall();
        break;
    case CMD_HEART_FALL:
        //result = decode_cmd_heart_fall();
        break;
    case CMD_LEAF_WAVEDOWN:
        //result = decode_cmd_leaf_wavedown();
        break;
    case CMD_PLAY_MUSIC:
        //result = decode_cmd_play_music();
        break;
    case CMD_STOP_MUSIC:
        //result = decode_cmd_stop_music();
        break;
    case CMD_PLAY_VOICE:
        //result = decode_cmd_play_voice();
        break;
    case CMD_STOP_VOICE:
        //result = decode_cmd_stop_voice();
        break;
    case CMD_CHANGE_MAP:
        //result = decode_cmd_change_map();
        break;
    case CMD_RESTORE_MAP:
        //result = decode_cmd_restore_map();
        break;
    case CMD_FADE_IN:
        //result = decode_cmd_fade_in();
        break;
    case CMD_FADE_OUT:
        //result = decode_cmd_fade_out();
        break;
    case CMD_MERGE_OUT:
        //result = decode_cmd_merge_out();
        break;
    case CMD_SNAPSHOT:
        //result = decode_cmd_snapshot();
        break;
    case CMD_SHOW_TITLEMENU:
        //result = decode_cmd_show_titlemenu();
        break;
    case CMD_COUNT_PLAYERS:
        //result = decode_cmd_count_players();
        break;
    case CMD_CHECK_MAGIC_TO:
        //result = decode_cmd_check_magic_to();
        break;
    case CMD_ADD_SPECIALNPC:
        //result = decode_cmd_add_specialnpc();
        break;
    case CMD_DEL_SPECIALNPC:
        //result = decode_cmd_del_specialnpc();
        break;
    default:
        break;
    }
    return result;
}


