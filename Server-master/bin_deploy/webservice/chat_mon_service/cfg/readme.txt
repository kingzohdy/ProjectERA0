#COPYTO --PRE77--192.168.0.77:/usr/local/tjw/myworld/src/chat_mon_service/readme.txt
2012-12-31 patch
service端:
	覆盖 ./bin/chat_mon_service
	覆盖 ./cfg/start_chat_mon_service.sh
	覆盖 ./cfg/stop_chat_mon_service.sh

	修改 ./cfg/chat_mon_service.xml
		<FcgiWebServerIPList>192.168.0.77</FcgiWebServerIPList>
		将192.168.0.77 修改成ngxin绑定的内网ip
		

	执行 cd cfg;./stop_chat_mon_service.sh; ./start_chat_mon_service.sh

.............................................................................

2012-12-27 patch

service端:
	覆盖 ./bin/chat_mon_service
	覆盖 ./cfg/start_chat_mon_service.sh
	覆盖 ./cfg/stop_chat_mon_service.sh

	修改 ./cfg/chat_mon_service.xml
		在
		<SpawnPort>8092 </SpawnPort>
		后面增加下面行:
		<SpawnNum>5 </SpawnNum>

	执行 cd cfg;./stop_chat_mon_service.sh; ./start_chat_mon_service.sh

	默认会开启5个进程

nginx修改:
	在http段中增加upstream
	 upstream acc {
                server  192.168.0.77:8092 weight=1;
                server  192.168.0.77:8093 weight=1;
                server  192.168.0.77:8094 weight=1;
                server  192.168.0.77:8095 weight=1;
                server  192.168.0.77:8096 weight=1;
        }
	ip地址要做相应修改



	将 location ~ ^/cgi-bin/chat_mon_service$ 段中的
	fastcgi_pass   192.168.0.77:8092; 
	修改成:
	fastcgi_pass   acc;

	重启nginx

.............................................................................

1.依赖spawn-fcgi-1.6.3,第一次部署需要安装,源码在 ../dep 目录下面
	第一次部署时需要新建数据库chat_mon_service,sql文件在../dep 目录下面

2.XML配置文件放在 ../cfg 目录下,安装时,所有的xml文件都有后缀.default,配置好后,去掉.default后缀
	chat_mon_service.xml :
		配置fastcgi绑定的ip地址(SpawnIP)和端口(SpawnPort),请使用内网ip
		配置chat_mon_service 数据库连接
		配置account_1 到 account_n 的数据库连接
		配置部署gmtools Web端nginx服务器地址(FcgiWebServerIPList),请使用内网ip,多个ip可以用逗号(,)分割

	htpasswdSOAP,htpasswdFCGI:
		nginx使用的web用户密码验证文件,第一次部署的时候复制nginx的conf目录下面


3.配置好xml文件后,使用../cfg目录下面的起停脚本
	start_chat_mon_service.sh
	stop_chat_mon_service.sh


4.配置web端nginx,配置文件为nginx.conf
        location ~ ^/cgi-bin/chat_mon_service$ {
		auth_basic            "SOAP";
		auth_basic_user_file  htpasswdSOAP;
            root           html;
            fastcgi_pass   192.168.0.77:8092;
            fastcgi_index  index.php;
           fastcgi_param  SCRIPT_FILENAME  /usr/local/nginx/html/$fastcgi_script_name;
            include        fastcgi_params;
        }
	其中
	fastcgi_pass   192.168.0.77:8092;
	的ip和端口修改成 chat_mon_service.xml中的ip和端口
	htpasswdSOAP 文件放在nginx的conf目录下,可以使用apache的htpasswd2命令生成或修改
	htpasswd2 -b htpasswdSOAP soapuser *新密码*
	密码修改后,需配置 soap.php 中的 SoapClient(...."login" => "soapuser","password"=> "新密码"...)

5.配置chat_mon_service.wsdl 在文件结尾处找到以下内容
	<service name="ChatMonService" >
		<documentation>LJY CS Service </documentation>
		<port name="ChatMonService" binding="s0:ChatMonServiceSoap">
			<wsoap:address location="http://192.168.0.57:800/cgi-bin/chat_mon_service"/>
		</port>
	</service>
	其中
	http://192.168.0.57:800/cgi-bin/chat_mon_service
	需要修改成
	http://nginx内网ip:nginx端口/cgi-bin/chat_mon_service

	修改完成后将文件chat_mon_service.wsdl复制到php相应目录cfg下面,覆盖同名文件

6.php使用chat_mon_service示例
	源码构建的php需要带参数--enable-soap
	./configure --其他参数 \
	--enable-soap 
	
	test.php:
	$client = new SoapClient("chat_mon_service.wsdl",
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

	$xml=$client->GetChatMonountPasswd(array('ChatMonName'=>'007'));


7. 负载均衡,配置nginx.conf
	chat_mon_service 默认开启5个实例,每个实例侦听的端口自增1
	在http段中增加upstream
	 upstream acc {
                server  192.168.0.77:8092 weight=1;
                server  192.168.0.77:8093 weight=1;
                server  192.168.0.77:8094 weight=1;
                server  192.168.0.77:8095 weight=1;
                server  192.168.0.77:8096 weight=1;
        }



	将 location ~ ^/cgi-bin/chat_mon_service$ 段中的
	fastcgi_pass   192.168.0.77:8092; 
	修改成:
	fastcgi_pass   acc;

	重启nginx



