#COPYTO --PRE77--192.168.0.77:/usr/local/tjw/myworld/src/cs_service/readme.txt
2013-7-15 path
	������������,GB18030 Ϊ�ַ���ת��ʱ�õ������ַ���,������EUC-KR,̨�己������ BIG5
	û�д�����Ĭ��Ϊ GB18030
	<ConvertEncoding>GB18030 </ConvertEncoding>

2013-1-14 patch
service��:
	���� ./bin/cs_service
	���� ./cfg/proto_ss.tdr

	�޸� ./cfg/cs_service.xml ��������:
	<RestrictInfo type="CsRestrictInfo">
                <RestrictIpNum>2 </RestrictIpNum>
                <RestrictIP>192.168.0.77</RestrictIP>
		<RestrictIP>192.168.0.79</RestrictIP>
                <AllowOperationNum>1 </AllowOperationNum>
                <AllowOperation type="OperationInfo">
                        <Name>SetAccSafelockReq</Name>
                </AllowOperation>
        </RestrictInfo>
	���� <RestrictIpNum>2 </RestrictIpNum> 2 Ϊ�������ؾ�������
	���� 192.168.0.77 �� 192.168.0.79 �޸�Ϊ����������ip

	ִ�� cd cfg;./stop_cs_service.sh;./start_cs_service.sh
.......................................................................

2013-1-11 patch
service��:
	���� ./bin/cs_service
	���� ./cfg/proto_ss.tdr

	ִ�� cd cfg;./stop_cs_service.sh;./start_cs_service.sh

web ��:
	�鿴 ./web/dep/webtool_readme.txt
	
.......................................................................

2012-12-31 patch
service��:
	���� ./bin/cs_service
	���� ./cfg/start_cs_service.sh
	���� ./cfg/stop_cs_service.sh

	�޸� ./cfg/cs_service.xml
		<FcgiWebServerIPList>192.168.0.77</FcgiWebServerIPList>
		��192.168.0.77 �޸ĳ�ngxin�󶨵�����ip
		

	ִ�� cd cfg;./stop_cs_service.sh; ./start_cs_service.sh

.......................................................................

1.����spawn-fcgi-1.6.3,��һ�β�����Ҫ��װ,Դ���� ../dep Ŀ¼����

2.XML�����ļ����� ../cfg Ŀ¼��,��װʱ,���е�xml�ļ����к�׺.default,���úú�,ȥ��.default��׺
	cs_service.xml :
		����fastcgi�󶨵�ip��ַ(SpawnIP)�Ͷ˿�(SpawnPort),��ʹ������ip
		���ò�ѯ�������(FindHandleNum)�Ͳ�ѯ�����ʱʱ��(FindHandleTimeout)
		���ò���gmtools Web��nginx��������ַ(FcgiWebServerIPList),��ʹ������ip,���ip�����ö���(,)�ָ�

	cs_cgi.xml:
		���ø�������cs_conn��ip�Ͷ˿�
		���ø�����account_cs_conn��ip�Ͷ˿�

	htpasswdSOAP,htpasswdFCGI:
		nginxʹ�õ�web�û�������֤�ļ�,��һ�β����ʱ����nginx��confĿ¼����


3.���ú�xml�ļ���,ʹ��../cfgĿ¼�������ͣ�ű�
	start_cs_service.sh
	stop_cs_service.sh


4.����web��nginx,�����ļ�Ϊnginx.conf
        location ~ ^/cgi-bin/cs_service$ {
		auth_basic            "SOAP";
		auth_basic_user_file  htpasswdSOAP;
            root           html;
            fastcgi_pass   192.168.0.77:8091;
            fastcgi_index  index.php;
           fastcgi_param  SCRIPT_FILENAME  /usr/local/nginx/html/$fastcgi_script_name;
            include        fastcgi_params;
        }
	����
	fastcgi_pass   192.168.0.77:8091;
	��ip�Ͷ˿��޸ĳ� cs_service.xml�е�ip�Ͷ˿�
	htpasswdSOAP �ļ�����nginx��confĿ¼��,����ʹ��apache��htpasswd2�������ɻ��޸�
	htpasswd2 -b htpasswdSOAP soapuser *������*
	�����޸ĺ�,������../web/gmtools/gamecontrol/soap.php �е� SoapClient(...."login" => "soapuser","password"=> "������"...)

5.����cs_service.wsdl ���ļ���β���ҵ���������
	<service name="CsService" >
		<documentation>LJY CS Service </documentation>
		<port name="CsService" binding="s0:CsServiceSoap">
			<wsoap:address location="http://192.168.0.57:800/cgi-bin/cs_service"/>
		</port>
	</service>
	����
	http://192.168.0.57:800/cgi-bin/cs_service
	��Ҫ�޸ĳ�
	http://nginx����ip:nginx�˿�/cgi-bin/cs_service

	�޸���ɺ��ļ�cs_service.wsdl���Ƶ� ../web/gmtools/cfg����,����ͬ���ļ�

6.web �˵����ú�ʹ�����Ķ� ../web/dep/Ŀ¼�µ��ļ� webtool_readme.txt
	��һ�β�����Ҫ�������ݿ�gmtools,ʹ��../web/dep/db/�����sql�������ݿ��
	�޸�default.conn.inc.php �������ݿ�����,������ɺ�,���ļ����ĳ�conn.inc.php
	��../web/gmtools����Ŀ¼���Ƶ�nginx���ڻ����Ķ�ӦĿ¼��

7.web �˷������� ie8.0 ���ϰ汾�� chrome 19 ���ϰ汾

8.web ������������֤(nginx):
	auth_basic            "FCGI";
	auth_basic_user_file  htpasswdFCGI;

	Ĭ���ʺ�����:wanyouadmin admin#0912
	�޸ķ�ʽ:htpasswd2 -b htpasswdFCGI wanyouadmin admin#0912

9.web ��php��

9.����nginx
	
	
