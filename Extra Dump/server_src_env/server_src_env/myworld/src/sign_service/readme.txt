#COPYTO --PRE77--192.168.0.77:/usr/local/tjw/myworld/src/sign_service/readme.txt

.............................................................................

1.����spawn-fcgi-1.6.3,��һ�β�����Ҫ��װ,Դ���� ../dep Ŀ¼����
	��һ�β���ʱ��Ҫ�½����ݿ�sign_service,sql�ļ���../dep Ŀ¼����

2.XML�����ļ����� ../cfg Ŀ¼��,��װʱ,���е�xml�ļ����к�׺.default,���úú�,ȥ��.default��׺
	sign_service.xml :
		����fastcgi�󶨵�ip��ַ(SpawnIP)�Ͷ˿�(SpawnPort),��ʹ������ip
		���ò���gmtools Web��nginx��������ַ(FcgiWebServerIPList),��ʹ������ip,���ip�����ö���(,)�ָ�

	htpasswdSOAP,htpasswdFCGI:
		nginxʹ�õ�web�û�������֤�ļ�,��һ�β����ʱ����nginx��confĿ¼����


3.���ú�xml�ļ���,ʹ��../cfgĿ¼�������ͣ�ű�
	start_sign_service.sh
	stop_sign_service.sh


4.����web��nginx,�����ļ�Ϊnginx.conf
        location ~ ^/cgi-bin/sign_service$ {
		auth_basic            "SOAP";
		auth_basic_user_file  htpasswdSOAP;
            root           html;
            fastcgi_pass   192.168.0.58:8010;
            fastcgi_index  index.php;
           fastcgi_param  SCRIPT_FILENAME  /usr/local/nginx/html/$fastcgi_script_name;
            include        fastcgi_params;
        }
	����
	fastcgi_pass   192.168.0.77:8010;
	��ip�Ͷ˿��޸ĳ� sign_service.xml�е�ip�Ͷ˿�
	htpasswdSOAP �ļ�����nginx��confĿ¼��,����ʹ��apache��htpasswd2�������ɻ��޸�
	htpasswd2 -b htpasswdSOAP soapuser *������*
	�����޸ĺ�,������ soap.php �е� SoapClient(...."login" => "soapuser","password"=> "������"...)

5.����sign_service.wsdl ���ļ���β���ҵ���������
	<service name="SignService" >
		<documentation>LJY CS Service </documentation>
		<port name="SignService" binding="s0:SignServiceSoap">
			<wsoap:address location="http://192.168.0.57:800/cgi-bin/sign_service"/>
		</port>
	</service>
	����
	http://192.168.0.58:800/cgi-bin/sign_service
	��Ҫ�޸ĳ�
	http://nginx����ip:nginx�˿�/cgi-bin/sign_service

	�޸���ɺ��ļ�sign_service.wsdl���Ƶ�php��ӦĿ¼cfg����,����ͬ���ļ�

6.phpʹ��sign_serviceʾ��
	Դ�빹����php��Ҫ������--enable-soap
	./configure --�������� \
	--enable-soap 
	
	test.php:
	$client = new SoapClient("sign_service.wsdl",
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

	$xml=$client->GetSign(array('UserName'=>'007'....));


7. ���ؾ���,����nginx.conf
	sign_service Ĭ�Ͽ���5��ʵ��,ÿ��ʵ�������Ķ˿�����1
	��http��������upstream
	 upstream sign {
                server  192.168.0.77:8010 weight=1;
                server  192.168.0.77:8011 weight=1;
                server  192.168.0.77:8012 weight=1;
                server  192.168.0.77:8013 weight=1;
                server  192.168.0.77:8014 weight=1;
        }



	�� location ~ ^/cgi-bin/sign_service$ ���е�
	fastcgi_pass   192.168.0.77:8010; 
	�޸ĳ�:
	fastcgi_pass   sign;

	����nginx

8. auth_svr����������sign_service��Ӧ��ip,��������б�����������webǩ��



www.0dyx.cn