#COPYTO --PRE77--192.168.0.77:/usr/local/tjw/myworld/src/acc_service/readme.txt
2013-3-12 path
service��:
	���� ./bin/acc_service

	�޸� ./cfg/acc_service.xml
		��
		<SpawnNum>5 </SpawnNum> �޸ĳ�
		<SpawnNum>8 </SpawnNum>
		��
		<SpawnNum>8 </SpawnNum>
		������������3��:
			<CsServiceUrl>http://192.168.0.57:800/cgi-bin/cs_service</CsServiceUrl>
			<HttpBaseUserID>soapuser</HttpBaseUserID>
			<HttpBaseUserPasswd>soapuser#1234</HttpBaseUserPasswd>
		���޸� http://192.168.0.57:800/cgi-bin/cs_service Ϊ��Ӫ������cs_service url
		HttpBaseUserID �� HttpBaseUserPasswd Ϊ����������ַ��Ҫ���û���������

    ִ�� cd cfg;./stop_acc_service.sh; ./start_acc_service.sh

����web��nginx,�����ļ�Ϊnginx.conf
	acc_service Ĭ�Ͽ���8��ʵ��,ÿ��ʵ�������Ķ˿�����1,ǰ5��ʵ����Ϊ������,��3��Ϊ�������,
	��http��������upstream
	 upstream accmisc {
                server  192.168.0.77:8097 weight=1;
                server  192.168.0.77:8098 weight=1;
                server  192.168.0.77:8099 weight=1;
        }

	192.168.0.77 Ҫ����Ӧ���޸�


        location ~ ^/cgi-bin/acc_service_misc$ {
		auth_basic            "SOAP";
		auth_basic_user_file  htpasswdSOAP;
            root           html;
            fastcgi_pass   accmisc;
            fastcgi_index  index.php;
           fastcgi_param  SCRIPT_FILENAME  /usr/local/nginx/html/$fastcgi_script_name;
            include        fastcgi_params;
        }

	����nginx
.............................................................................
2012-12-31 patch
service��:
	���� ./bin/acc_service
	���� ./cfg/start_acc_service.sh
	���� ./cfg/stop_acc_service.sh

	�޸� ./cfg/acc_service.xml
		<FcgiWebServerIPList>192.168.0.77</FcgiWebServerIPList>
		��192.168.0.77 �޸ĳ�ngxin�󶨵�����ip
		

	ִ�� cd cfg;./stop_acc_service.sh; ./start_acc_service.sh

.............................................................................

2012-12-27 patch

service��:
	���� ./bin/acc_service
	���� ./cfg/start_acc_service.sh
	���� ./cfg/stop_acc_service.sh

	�޸� ./cfg/acc_service.xml
		��
		<SpawnPort>8092 </SpawnPort>
		��������������:
		<SpawnNum>5 </SpawnNum>

	ִ�� cd cfg;./stop_acc_service.sh; ./start_acc_service.sh

	Ĭ�ϻῪ��5������

nginx�޸�:
	��http��������upstream
	 upstream acc {
                server  192.168.0.77:8092 weight=1;
                server  192.168.0.77:8093 weight=1;
                server  192.168.0.77:8094 weight=1;
                server  192.168.0.77:8095 weight=1;
                server  192.168.0.77:8096 weight=1;
        }
	ip��ַҪ����Ӧ�޸�



	�� location ~ ^/cgi-bin/acc_service$ ���е�
	fastcgi_pass   192.168.0.77:8092; 
	�޸ĳ�:
	fastcgi_pass   acc;

	����nginx

.............................................................................

1.����spawn-fcgi-1.6.3,��һ�β�����Ҫ��װ,Դ���� ../dep Ŀ¼����
	��һ�β���ʱ��Ҫ�½����ݿ�acc_service,sql�ļ���../dep Ŀ¼����

2.XML�����ļ����� ../cfg Ŀ¼��,��װʱ,���е�xml�ļ����к�׺.default,���úú�,ȥ��.default��׺
	acc_service.xml :
		����fastcgi�󶨵�ip��ַ(SpawnIP)�Ͷ˿�(SpawnPort),��ʹ������ip
		����acc_service ���ݿ�����
		����account_1 �� account_n �����ݿ�����
		���ò���gmtools Web��nginx��������ַ(FcgiWebServerIPList),��ʹ������ip,���ip�����ö���(,)�ָ�

	htpasswdSOAP,htpasswdFCGI:
		nginxʹ�õ�web�û�������֤�ļ�,��һ�β����ʱ����nginx��confĿ¼����


3.���ú�xml�ļ���,ʹ��../cfgĿ¼�������ͣ�ű�
	start_acc_service.sh
	stop_acc_service.sh


4.����web��nginx,�����ļ�Ϊnginx.conf
        location ~ ^/cgi-bin/acc_service$ {
		auth_basic            "SOAP";
		auth_basic_user_file  htpasswdSOAP;
            root           html;
            fastcgi_pass   192.168.0.77:8092;
            fastcgi_index  index.php;
           fastcgi_param  SCRIPT_FILENAME  /usr/local/nginx/html/$fastcgi_script_name;
            include        fastcgi_params;
        }
	����
	fastcgi_pass   192.168.0.77:8092;
	��ip�Ͷ˿��޸ĳ� acc_service.xml�е�ip�Ͷ˿�
	htpasswdSOAP �ļ�����nginx��confĿ¼��,����ʹ��apache��htpasswd2�������ɻ��޸�
	htpasswd2 -b htpasswdSOAP soapuser *������*
	�����޸ĺ�,������ soap.php �е� SoapClient(...."login" => "soapuser","password"=> "������"...)

5.����acc_service.wsdl ���ļ���β���ҵ���������
	<service name="AccService" >
		<documentation>LJY CS Service </documentation>
		<port name="AccService" binding="s0:AccServiceSoap">
			<wsoap:address location="http://192.168.0.57:800/cgi-bin/acc_service"/>
		</port>
	</service>
	����
	http://192.168.0.57:800/cgi-bin/acc_service
	��Ҫ�޸ĳ�
	http://nginx����ip:nginx�˿�/cgi-bin/acc_service

	�޸���ɺ��ļ�acc_service.wsdl���Ƶ�php��ӦĿ¼cfg����,����ͬ���ļ�

6.phpʹ��acc_serviceʾ��
	Դ�빹����php��Ҫ������--enable-soap
	./configure --�������� \
	--enable-soap 
	
	test.php:
	$client = new SoapClient("acc_service.wsdl",
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

	$xml=$client->GetAccountPasswd(array('AccName'=>'007'));


7. ���ؾ���,����nginx.conf
	acc_service Ĭ�Ͽ���5��ʵ��,ÿ��ʵ�������Ķ˿�����1
	��http��������upstream
	 upstream acc {
                server  192.168.0.77:8092 weight=1;
                server  192.168.0.77:8093 weight=1;
                server  192.168.0.77:8094 weight=1;
                server  192.168.0.77:8095 weight=1;
                server  192.168.0.77:8096 weight=1;
        }



	�� location ~ ^/cgi-bin/acc_service$ ���е�
	fastcgi_pass   192.168.0.77:8092; 
	�޸ĳ�:
	fastcgi_pass   acc;

	����nginx


