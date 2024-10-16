#COPYTO --PRE77--192.168.0.77:/usr/local/tjw/myworld/src/sign_service/readme.txt

.............................................................................

1.依赖spawn-fcgi-1.6.3,第一次部署需要安装,源码在 ../dep 目录下面
	第一次部署时需要新建数据库sign_service,sql文件在../dep 目录下面

2.XML配置文件放在 ../cfg 目录下,安装时,所有的xml文件都有后缀.default,配置好后,去掉.default后缀
	sign_service.xml :
		配置fastcgi绑定的ip地址(SpawnIP)和端口(SpawnPort),请使用内网ip
		配置部署gmtools Web端nginx服务器地址(FcgiWebServerIPList),请使用内网ip,多个ip可以用逗号(,)分割

	htpasswdSOAP,htpasswdFCGI:
		nginx使用的web用户密码验证文件,第一次部署的时候复制nginx的conf目录下面


3.配置好xml文件后,使用../cfg目录下面的起停脚本
	start_sign_service.sh
	stop_sign_service.sh


4.配置web端nginx,配置文件为nginx.conf
        location ~ ^/cgi-bin/sign_service$ {
		auth_basic            "SOAP";
		auth_basic_user_file  htpasswdSOAP;
            root           html;
            fastcgi_pass   192.168.0.58:8010;
            fastcgi_index  index.php;
           fastcgi_param  SCRIPT_FILENAME  /usr/local/nginx/html/$fastcgi_script_name;
            include        fastcgi_params;
        }
	其中
	fastcgi_pass   192.168.0.77:8010;
	的ip和端口修改成 sign_service.xml中的ip和端口
	htpasswdSOAP 文件放在nginx的conf目录下,可以使用apache的htpasswd2命令生成或修改
	htpasswd2 -b htpasswdSOAP soapuser *新密码*
	密码修改后,需配置 soap.php 中的 SoapClient(...."login" => "soapuser","password"=> "新密码"...)

5.配置sign_service.wsdl 在文件结尾处找到以下内容
	<service name="SignService" >
		<documentation>LJY CS Service </documentation>
		<port name="SignService" binding="s0:SignServiceSoap">
			<wsoap:address location="http://192.168.0.57:800/cgi-bin/sign_service"/>
		</port>
	</service>
	其中
	http://192.168.0.58:800/cgi-bin/sign_service
	需要修改成
	http://nginx内网ip:nginx端口/cgi-bin/sign_service

	修改完成后将文件sign_service.wsdl复制到php相应目录cfg下面,覆盖同名文件

6.php使用sign_service示例
	源码构建的php需要带参数--enable-soap
	./configure --其他参数 \
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


7. 负载均衡,配置nginx.conf
	sign_service 默认开启5个实例,每个实例侦听的端口自增1
	在http段中增加upstream
	 upstream sign {
                server  192.168.0.77:8010 weight=1;
                server  192.168.0.77:8011 weight=1;
                server  192.168.0.77:8012 weight=1;
                server  192.168.0.77:8013 weight=1;
                server  192.168.0.77:8014 weight=1;
        }



	将 location ~ ^/cgi-bin/sign_service$ 段中的
	fastcgi_pass   192.168.0.77:8010; 
	修改成:
	fastcgi_pass   sign;

	重启nginx

8. auth_svr有配置限制sign_service对应的ip,如果不在列表内则不能生成web签名



www.0dyx.cn