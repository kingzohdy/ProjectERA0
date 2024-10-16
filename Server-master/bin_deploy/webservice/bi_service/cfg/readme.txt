#COPYTO --PRE77--192.168.0.77:/usr/local/tjw/myworld/src/bi_service/readme.txt

2013-3-8 path
service端:
	覆盖 ./bin/bi_service

	修改 ./cfg/bi_service.xml
		将
		<SpawnNum>2 </SpawnNum> 修改成:
		<SpawnNum>5 </SpawnNum>

		在
		<SpawnNum>2 </SpawnNum>
		后面增加下面3行:
			<CsServiceUrl>http://192.168.0.57:800/cgi-bin/cs_service</CsServiceUrl>
			<HttpBaseUserID>soapuser</HttpBaseUserID>
			<HttpBaseUserPasswd>soapuser#1234</HttpBaseUserPasswd>
		并修改 http://192.168.0.57:800/cgi-bin/cs_service 为运营环境的cs_service url
		HttpBaseUserID 和 HttpBaseUserPasswd 为访问上述地址需要的用户名和密码

    执行 cd cfg;./stop_bi_service.sh; ./start_bi_service.sh

nginx修改:
	在http修改upstream
	 upstream bi {
                server  192.168.0.77:9090 weight=1;
                server  192.168.0.77:9091 weight=1;
		下面增加3行:
		server  192.168.0.77:9092 weight=1;
                server  192.168.0.77:9093 weight=1;
		server  192.168.0.77:9094 weight=1;

        }
	ip地址要做相应修改

.............................................................................
2013-2-19 patch
service端:
	覆盖 ./bin/bi_service

	执行 cd cfg;./stop_bi_service.sh; ./start_bi_service.sh

.............................................................................

2013-1-5 patch
创建service端对应数据库:
	执行 mysql < ./dep/bi_service.sql

service端:
	覆盖 ./bin/bi_service
	覆盖 ./cfg/start_bi_service.sh
	覆盖 ./cfg/stop_bi_service.sh

	修改 ./cfg/bi_service.xml
		<FcgiWebServerIPList>192.168.0.77</FcgiWebServerIPList>
		将192.168.0.77 修改成ngxin绑定的内网ip
		

	执行 cd cfg;./stop_bi_service.sh; ./start_bi_service.sh

.............................................................................
2012-12-27 patch

service端:
	覆盖 ./bin/bi_service
	覆盖 ./cfg/start_bi_service.sh
	覆盖 ./cfg/stop_bi_service.sh

	修改 ./cfg/bi_service.xml
		在
		<SpawnPort>9090 </SpawnPort>
		后面增加下面行:
		<SpawnNum>2 </SpawnNum>

	执行 cd cfg;./stop_bi_service.sh; ./start_bi_service.sh

	默认会开启2个进程

nginx修改:
	在http段中增加upstream
	 upstream bi {
                server  192.168.0.77:9090 weight=1;
                server  192.168.0.77:9091 weight=1;
        }
	ip地址要做相应修改



	将 location ~ ^/cgi-bin/bi_service$ 段中的
	fastcgi_pass   192.168.0.77:9090; 
	修改成:
	fastcgi_pass   bi;

	重启nginx

.............................................................................

1.依赖spawn-fcgi-1.6.3,第一次部署需要安装,源码在 ../dep 目录下面
	第一次部署时需要新建数据库bi_service,sql文件在../dep 目录下面

2.XML配置文件放在 ../cfg 目录下,安装时,所有的xml文件都有后缀.default,配置好后,去掉.default后缀
	bi_service.xml :
		配置fastcgi绑定的ip地址(SpawnIP)和端口(SpawnPort),请使用内网ip
		配置bi_service 数据库连接
		配置account_1 到 account_n 的数据库连接
		配置bill_1 到 bill_n 的数据库连接
		配置部署gmtools Web端nginx服务器地址(FcgiWebServerIPList),请使用内网ip,多个ip可以用逗号(,)分割

	htpasswdSOAP,htpasswdFCGI:
		nginx使用的web用户密码验证文件,第一次部署的时候复制nginx的conf目录下面


3.配置好xml文件后,使用../cfg目录下面的起停脚本
	start_bi_service.sh
	stop_bi_service.sh


4.配置web端nginx,配置文件为nginx.conf
        location ~ ^/cgi-bin/bi_service$ {
		auth_basic            "SOAP";
		auth_basic_user_file  htpasswdSOAP;
            root           html;
            fastcgi_pass   192.168.0.77:9090;
            fastcgi_index  index.php;
           fastcgi_param  SCRIPT_FILENAME  /usr/local/nginx/html/$fastcgi_script_name;
            include        fastcgi_params;
        }
	其中
	fastcgi_pass   192.168.0.77:9090;
	的ip和端口修改成 bi_service.xml中的ip和端口
	htpasswdSOAP 文件放在nginx的conf目录下,可以使用apache的htpasswd2命令生成或修改
	htpasswd2 -b htpasswdSOAP soapuser *新密码*
	密码修改后,需配置 soap.php 中的 SoapClient(...."login" => "soapuser","password"=> "新密码"...)

5.配置bi_service.wsdl 在文件结尾处找到以下内容
	<service name="BiService" >
		<documentation>LJY CS Service </documentation>
		<port name="BiService" binding="s0:BiServiceSoap">
			<wsoap:address location="http://192.168.0.57:800/cgi-bin/bi_service"/>
		</port>
	</service>
	其中
	http://192.168.0.57:800/cgi-bin/bi_service
	需要修改成
	http://nginx内网ip:nginx端口/cgi-bin/bi_service

	修改完成后将文件bi_service.wsdl复制到php相应目录cfg下面,覆盖同名文件

6.php使用bi_service示例
	源码构建的php需要带参数--enable-soap
	./configure --其他参数 \
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


7. 负载均衡,配置nginx.conf
	bi_service 默认开启2个实例,每个实例侦听的端口自增1
	在http段中增加upstream
	 upstream bi {
                server  192.168.0.77:9090 weight=1;
                server  192.168.0.77:9091 weight=1;
        }



	将 location ~ ^/cgi-bin/bi_service$ 段中的
	fastcgi_pass   192.168.0.77:9090; 
	修改成:
	fastcgi_pass   bi;

	重启nginx

