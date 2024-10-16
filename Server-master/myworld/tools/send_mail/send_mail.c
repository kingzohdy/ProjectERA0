#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs_cgi.h"

//根据文件中的内容给角色发邮件！
int my_send_mail()
{
	int iErr = 0;
	int *pHandle = NULL;
	int iRegionID = 0;
	int iWorldID = 0;
	char cTo[200];//假设角色名长度在200之内
	FILE *fp = NULL;
	
	memset(cTo,0,sizeof(cTo));
	
	//send_mail_to_role.txt 该文件内容格式为 : 区id 服id 角色名
	if(!(fp=fopen("./send_mail_to_role.txt", "r")))
	{
		printf("打开文件./send_mail_to_role.txt失败\n");
		iErr = -1;
		goto final;
	}
	
    if(init_interface("../../src/comm/proto_ss.tdr",&pHandle,"../../src/cs_cgi/cs_cgi.xml"))
    {
		printf("初始化失败！\n");
		iErr = -1;
		goto final;
    }
	
	if(!pHandle)
	{
		printf("无效的句柄\n");
		iErr = -1;
		goto final;
	}
	
	
	while(fscanf(fp,"%d%d%s",&iRegionID,&iWorldID,cTo)>0)
	{
		iErr = cs_send_sys_mail(pHandle,iRegionID,iWorldID,cTo,"零纪元官网","测试","端午节快乐,游戏快乐！",NULL,0,0,2000);
		if(iErr < 0)
			printf("邮件发送失败: 原因:%d  区ID:%d  服ID:%d  角色名:%s\n",iErr,iRegionID,iWorldID,cTo);
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
