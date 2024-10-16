
#ifndef _OG_SHM_H
#define _OG_SHM_H

#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_SHM_PER_ZONE	300

#define ZONE_CONFIG_SHM_BYID    1
#define ZONE_CONFIG_SHM_BYINST  2

#define ZONE_CONFIG_KEY_FILE    "/tmp/config_shm_tab.key"

#define ZONE_CONFIGTAB_SHM_KEY  18008

////修改下面的的宏要同步修改脚本(zoneshm)中对应的值
//#define ZONE_CONFIG_SHM_KEY     11000
#define ZONE_CONNECT_SHM_KEY    11001
#define ZONE_OBJECT_SHM_KEY	    11002
#define ZONE_MAP_SHM_KEY        11003
#define ZONE_MISC_SHM_KEY       11004
#define ZONE_CONFIG_SHM_TABLE   11005
// #define CHANNEL_ZONE_CONNECT_SHM_KEY	11030
// #define CHANNEL_ZONE_CTRL_SHM_KEY	11031
// #define CHANNEL_ZONE_DB_SHM_KEY	11032


#endif

