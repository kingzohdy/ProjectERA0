#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs_cgi.h"

//�����ļ��е����ݸ���ɫ���ʼ���
int my_send_mail()
{
	int iErr = 0;
	int *pHandle = NULL;
	int iRegionID = 0;
	int iWorldID = 0;
	char cTo[200];//�����ɫ��������200֮��
	FILE *fp = NULL;
	
	memset(cTo,0,sizeof(cTo));
	
	//send_mail_to_role.txt ���ļ����ݸ�ʽΪ : ��id ��id ��ɫ��
	if(!(fp=fopen("./send_mail_to_role.txt", "r")))
	{
		printf("���ļ�./send_mail_to_role.txtʧ��\n");
		iErr = -1;
		goto final;
	}
	
    if(init_interface("../../src/comm/proto_ss.tdr",&pHandle,"../../src/cs_cgi/cs_cgi.xml"))
    {
		printf("��ʼ��ʧ�ܣ�\n");
		iErr = -1;
		goto final;
    }
	
	if(!pHandle)
	{
		printf("��Ч�ľ��\n");
		iErr = -1;
		goto final;
	}
	
	
	while(fscanf(fp,"%d%d%s",&iRegionID,&iWorldID,cTo)>0)
	{
		iErr = cs_send_sys_mail(pHandle,iRegionID,iWorldID,cTo,"���Ԫ����","����","����ڿ���,��Ϸ���֣�",NULL,0,0,2000);
		if(iErr < 0)
			printf("�ʼ�����ʧ��: ԭ��:%d  ��ID:%d  ��ID:%d  ��ɫ��:%s\n",iErr,iRegionID,iWorldID,cTo);
	}
		
	goto final;
	
final:
	if(pHandle)
	{
		destory_interface(pHandle);
	}
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}
	return iErr;
}

int main(int argc,char *argv[])
{
	(void)argc;
	return my_send_mail();
}
