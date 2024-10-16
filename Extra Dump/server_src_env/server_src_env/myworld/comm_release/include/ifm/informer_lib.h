#ifndef __INFORMERLIB_H__
#define __INFORMERLIB_H__

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef WIN32
typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;

#define EXPORT __declspec(dllexport)
EXPORT int ifm_post_request(const char* service_name,//不可传入NULL
                            uint64_t tradeid,
                            char* paras[],//不可传入NULL,内存空间在函数调用之前必须分配好
                            uint32_t para_count);

EXPORT int ifm_get_request(const char* service_name,//不可传入NULL
                           uint64_t* tradeid,//*tradeid=0,取任意一条
                           char *paras[], //不可传入NULL,内存空间在函数调用之前必须分配好
                           uint32_t *para_count); //不可传入NULL

EXPORT int ifm_post_response(const char* service_name, //不可传入NULL
                      uint64_t tradeid,
                      char *paras[], //不可传入NULL,内存空间在函数调用之前必须分配好
                      uint32_t para_count);

EXPORT int ifm_get_response(const char* service_name, //不可传入NULL
                     uint64_t* tradeid,//*tradeid=0,取任意一条
                     char *paras[], //不可传入NULL,内存空间在函数调用之前必须分配好
                     uint32_t *para_count); //不可传入NULL
#else
extern int ifm_post_request(const char* service_name,//不可传入NULL
                            uint64_t tradeid,
                            char* paras[],//不可传入NULL,内存空间在函数调用之前必须分配好
                            uint32_t para_count);

extern int ifm_get_request(const char* service_name,//不可传入NULL
                           uint64_t* tradeid,//*tradeid=0,取任意一条
                           char *paras[], //不可传入NULL,内存空间在函数调用之前必须分配好
                           uint32_t *para_count); //不可传入NULL

extern int ifm_post_response(const char* service_name, //不可传入NULL
                      uint64_t tradeid,
                      char *paras[], //不可传入NULL,内存空间在函数调用之前必须分配好
                      uint32_t para_count);

extern int ifm_get_response(const char* service_name, //不可传入NULL
                     uint64_t* tradeid,//*tradeid=0,取任意一条
                     char *paras[], //不可传入NULL,内存空间在函数调用之前必须分配好
                     uint32_t *para_count); //不可传入NULL
#endif

#ifdef __cplusplus
}
#endif

#endif

