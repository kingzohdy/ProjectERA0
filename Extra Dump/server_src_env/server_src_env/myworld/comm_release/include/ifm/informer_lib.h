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
EXPORT int ifm_post_request(const char* service_name,//���ɴ���NULL
                            uint64_t tradeid,
                            char* paras[],//���ɴ���NULL,�ڴ�ռ��ں�������֮ǰ��������
                            uint32_t para_count);

EXPORT int ifm_get_request(const char* service_name,//���ɴ���NULL
                           uint64_t* tradeid,//*tradeid=0,ȡ����һ��
                           char *paras[], //���ɴ���NULL,�ڴ�ռ��ں�������֮ǰ��������
                           uint32_t *para_count); //���ɴ���NULL

EXPORT int ifm_post_response(const char* service_name, //���ɴ���NULL
                      uint64_t tradeid,
                      char *paras[], //���ɴ���NULL,�ڴ�ռ��ں�������֮ǰ��������
                      uint32_t para_count);

EXPORT int ifm_get_response(const char* service_name, //���ɴ���NULL
                     uint64_t* tradeid,//*tradeid=0,ȡ����һ��
                     char *paras[], //���ɴ���NULL,�ڴ�ռ��ں�������֮ǰ��������
                     uint32_t *para_count); //���ɴ���NULL
#else
extern int ifm_post_request(const char* service_name,//���ɴ���NULL
                            uint64_t tradeid,
                            char* paras[],//���ɴ���NULL,�ڴ�ռ��ں�������֮ǰ��������
                            uint32_t para_count);

extern int ifm_get_request(const char* service_name,//���ɴ���NULL
                           uint64_t* tradeid,//*tradeid=0,ȡ����һ��
                           char *paras[], //���ɴ���NULL,�ڴ�ռ��ں�������֮ǰ��������
                           uint32_t *para_count); //���ɴ���NULL

extern int ifm_post_response(const char* service_name, //���ɴ���NULL
                      uint64_t tradeid,
                      char *paras[], //���ɴ���NULL,�ڴ�ռ��ں�������֮ǰ��������
                      uint32_t para_count);

extern int ifm_get_response(const char* service_name, //���ɴ���NULL
                     uint64_t* tradeid,//*tradeid=0,ȡ����һ��
                     char *paras[], //���ɴ���NULL,�ڴ�ռ��ں�������֮ǰ��������
                     uint32_t *para_count); //���ɴ���NULL
#endif

#ifdef __cplusplus
}
#endif

#endif

