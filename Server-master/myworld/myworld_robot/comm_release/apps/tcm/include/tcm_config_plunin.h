
/**
*
* @file     tcm_config_plunin.h 
* @brief   ≈‰÷√SOΩ”ø⁄
* 
* @author vinsonzuo
* @version 1.0
* @date 2009-5-13
*
*
* All rights reserved.
*
*/


#ifndef __TCM_CONFIG_PLUGIN__
#define __TCM_CONFIG_PLUGIN__

#ifdef __cplusplus
extern "C" {
#endif

typedef int HAPIHANDLE;

typedef struct config_plunin
{
    char* (* getname)	(struct config_plunin *p);
    char* (* getversion)	(struct config_plunin *p);
    int (* init)	(struct config_plunin *p);
    int (* cleanup)		(struct config_plunin *p);
    int (* createconfig)	(struct config_plunin *p, int id, char* dir, char* error, int maxlen);
    void* data;
} config_plunin;


int config_plunin_init(struct config_plunin *p, HAPIHANDLE handle);


#ifdef __cplusplus
}
#endif

#endif /* __TCM_CONFIG_PLUGIN__ */


