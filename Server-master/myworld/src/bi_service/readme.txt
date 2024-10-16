#COPYTO --PRE77--192.168.0.77:/usr/local/tjw/myworld/src/bi_service/readme.txt

2013-3-8 path
service��:
	���� ./bin/bi_service

	�޸� ./cfg/bi_service.xml
		��
		<SpawnNum>2 </SpawnNum> �޸ĳ�:
		<SpawnNum>5 </SpawnNum>

		��
		<SpawnNum>2 </SpawnNum>
		������������3��:
			<CsServiceUrl>http://192.168.0.57:800/cgi-bin/cs_service</CsServiceUrl>
			<HttpBaseUserID>soapuser</HttpBaseUserID>
			<HttpBaseUserPasswd>soapuser#1234</HttpBaseUserPasswd>
		���޸� http://192.168.0.57:800/cgi-bin/cs_service Ϊ��Ӫ������cs_service url
		HttpBaseUserID �� HttpBaseUserPasswd Ϊ����������ַ��Ҫ���û���������

    ִ�� cd cfg;./stop_bi_service.sh; ./start_bi_service.sh

nginx�޸�:
	��http�޸�upstream
	 upstream bi {
                server  192.168.0.77:9090 weight=1;
                server  192.168.0.77:9091 weight=1;
		��������3��:
		server  192.168.0.77:9092 weight=1;
                server  192.168.0.77:9093 weight=1;
		server  192.168.0.77:9094 weight=1;

        }
	ip��ַҪ����Ӧ�޸�

.............................................................................
2013-2-19 patch
service��:
	���� ./bin/bi_service

	ִ�� cd cfg;./stop_bi_service.sh; ./start_bi_service.sh

.............................................................................

2013-1-5 patch
����service�˶�Ӧ���ݿ�:
	ִ�� mysql < ./dep/bi_service.sql

service��:
	���� ./bin/bi_service
	���� ./cfg/start_bi_service.sh
	���� ./cfg/stop_bi_service.sh

	�޸� ./cfg/bi_service.xml
		<FcgiWebServerIPList>192.168.0.77</FcgiWebServerIPList>
		��192.168.0.77 �޸ĳ�ngxin�󶨵�����ip
		

	ִ�� cd cfg;./stop_bi_service.sh; ./start_bi_service.sh

.............................................................................
2012-12-27 patch

service��:
	���� ./bin/bi_service
	���� ./cfg/start_bi_service.sh
	���� ./cfg/stop_bi_service.sh

	�޸� ./cfg/bi_service.xml
		��
		<SpawnPort>9090 </SpawnPort>
		��������������:
		<SpawnNum>2 </SpawnNum>

	ִ�� cd cfg;./stop_bi_service.sh; ./start_bi_service.sh

	Ĭ�ϻῪ��2������

nginx�޸�:
	��http��������upstream
	 upstream bi {
                server  192.168.0.77:9090 weight=1;
                server  192.168.0.77:9091 weight=1;
        }
	ip��ַҪ����Ӧ�޸�



	�� location ~ ^/cgi-bin/bi_service$ ���е�
	fastcgi_pass   192.168.0.77:9090; 
	�޸ĳ�:
	fastcgi_pass   bi;

	����nginx

.............................................................................

1.����spawn-fcgi-1.6.3,��һ�β�����Ҫ��װ,Դ���� ../dep Ŀ¼����
	��һ�β���ʱ��Ҫ�½����ݿ�bi_service,sql�ļ���../dep Ŀ¼����

2.XML�����ļ����� ../cfg Ŀ¼��,��װʱ,���е�xml�ļ����к�׺.default,���úú�,ȥ��.default��׺
	bi_service.xml :
		����fastcgi�󶨵�ip��ַ(SpawnIP)�Ͷ˿�(SpawnPort),��ʹ������ip
		����bi_service ���ݿ�����
		����account_1 �� account_n �����ݿ�����
		����bill_1 �� bill_n �����ݿ�����
		���ò���gmtools Web��nginx��������ַ(FcgiWebServerIPList),��ʹ������ip,���ip�����ö���(,)�ָ�

	htpasswdSOAP,htpasswdFCGI:
		nginxʹ�õ�web�û�������֤�ļ�,��һ�β����ʱ����nginx��confĿ¼����


3.���ú�xml�ļ���,ʹ��../cfgĿ¼�������ͣ�ű�
	start_bi_service.sh
	stop_bi_service.sh


4.����web��nginx,�����ļ�Ϊnginx.conf
        location ~ ^/cgi-bin/bi_service$ {
		auth_basic            "SOAP";
		auth_basic_user_file  htpasswdSOAP;
            root           html;
            fastcgi_pass   192.168.0.77:9090;
            fastcgi_index  index.php;
           fastcgi_param  SCRIPT_FILENAME  /usr/local/nginx/html/$fastcgi_script_name;
            include        fastcgi_params;
        }
	����
	fastcgi_pass   192.168.0.77:9090;
	��ip�Ͷ˿��޸ĳ� bi_service.xml�е�ip�Ͷ˿�
	htpasswdSOAP �ļ�����nginx��confĿ¼��,����ʹ��apache��htpasswd2�������ɻ��޸�
	htpasswd2 -b htpasswdSOAP soapuser *������*
	�����޸ĺ�,������ soap.php �е� SoapClient(...."login" => "soapuser","password"=> "������"...)

5.����bi_service.wsdl ���ļ���β���ҵ���������
	<service name="BiService" >
		<documentation>LJY CS Service </documentation>
		<port name="BiService" binding="s0:BiServiceSoap">
			<wsoap:address location="http://192.168.0.57:800/cgi-bin/bi_service"/>
		</port>
	</service>
	����
	http://192.168.0.57:800/cgi-bin/bi_service
	��Ҫ�޸ĳ�
	http://nginx����ip:nginx�˿�/cgi-bin/bi_service

	�޸���ɺ��ļ�bi_service.wsdl���Ƶ�php��ӦĿ¼cfg����,����ͬ���ļ�

6.phpʹ��bi_serviceʾ��
	Դ�빹����php��Ҫ������--enable-soap
	./configure --�������� \
	--enable-soap 
	
	test.php:
	$client = new SoapClient("bi_service.wsdl",
				 array(
					// Stuff for development. 
					'trace' => 1,
					'exceptions' => true,
					'cache_wsdl' => WSDL_CACHE_NONE,
					'features' => SOAP_SINGLE_ELEMENT_ARRAYS,
					'encoding' =>'utf8',
					'login' => 'soapuser',
					'password' => 'soapuser#1234'
					     )
				);

	$xml=$client->GetAccountStat(array('RegionID'=>0,'AccName'=>'001'));


7. ���ؾ���,����nginx.conf
	bi_service Ĭ�Ͽ���2��ʵ��,ÿ��ʵ�������Ķ˿�����1
	��http��������upstream
	 upstream bi {
                server  192.168.0.77:9090 weight=1;
                server  192.168.0.77:9091 weight=1;
        }



	�� location ~ ^/cgi-bin/bi_service$ ���е�
	fastcgi_pass   192.168.0.77:9090; 
	�޸ĳ�:
	fastcgi_pass   bi;

	����nginx

