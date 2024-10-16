#COPYTO --PRE77--192.168.0.77:/usr/local/tjw/myworld/src/cs_service/readme.txt
2013-7-15 path
	配置中增加行,GB18030 为字符集转码时用的中文字符集,韩文用EUC-KR,台湾繁体字用 BIG5
	没有此行则默认为 GB18030
	<ConvertEncoding>GB18030 </ConvertEncoding>

2013-1-14 patch
service端:
	覆盖 ./bin/cs_service
	覆盖 ./cfg/proto_ss.tdr

	修改 ./cfg/cs_service.xml 增加内容:
	<RestrictInfo type="CsRestrictInfo">
                <RestrictIpNum>2 </RestrictIpNum>
                <RestrictIP>192.168.0.77</RestrictIP>
		<RestrictIP>192.168.0.79</RestrictIP>
                <AllowOperationNum>1 </AllowOperationNum>
                <AllowOperation type="OperationInfo">
                        <Name>SetAccSafelockReq</Name>
                </AllowOperation>
        </RestrictInfo>
	其中 <RestrictIpNum>2 </RestrictIpNum> 2 为官网负载均衡数量
	其中 192.168.0.77 和 192.168.0.79 修改为官网的内网ip

	执行 cd cfg;./stop_cs_service.sh;./start_cs_service.sh
.......................................................................

2013-1-11 patch
service端:
	覆盖 ./bin/cs_service
	覆盖 ./cfg/proto_ss.tdr

	执行 cd cfg;./stop_cs_service.sh;./start_cs_service.sh

web 端:
	查看 ./web/dep/webtool_readme.txt
	
.......................................................................

2012-12-31 patch
service端:
	覆盖 ./bin/cs_service
	覆盖 ./cfg/start_cs_service.sh
	覆盖 ./cfg/stop_cs_service.sh

	修改 ./cfg/cs_service.xml
		<FcgiWebServerIPList>192.168.0.77</FcgiWebServerIPList>
		将192.168.0.77 修改成ngxin绑定的内网ip
		

	执行 cd cfg;./stop_cs_service.sh; ./start_cs_service.sh

.......................................................................

1.依赖spawn-fcgi-1.6.3,第一次部署需要安装,源码在 ../dep 目录下面

2.XML配置文件放在 ../cfg 目录下,安装时,所有的xml文件都有后缀.default,配置好后,去掉.default后缀
	cs_service.xml :
		配置fastcgi绑定的ip地址(SpawnIP)和端口(SpawnPort),请使用内网ip
		配置查询句柄数量(FindHandleNum)和查询句柄超时时间(FindHandleTimeout)
		配置部署gmtools Web端nginx服务器地址(FcgiWebServerIPList),请使用内网ip,多个ip可以用逗号(,)分割

	cs_cgi.xml:
		配置各服务器cs_conn的ip和端口
		配置各大区account_cs_conn的ip和端口

	htpasswdSOAP,htpasswdFCGI:
		nginx使用的web用户密码验证文件,第一次部署的时候复制nginx的conf目录下面


3.配置好xml文件后,使用../cfg目录下面的起停脚本
	start_cs_service.sh
	stop_cs_service.sh


4.配置web端nginx,配置文件为nginx.conf
        location ~ ^/cgi-bin/cs_service$ {
		auth_basic            "SOAP";
		auth_basic_user_file  htpasswdSOAP;
            root           html;
            fastcgi_pass   192.168.0.77:8091;
            fastcgi_index  index.php;
           fastcgi_param  SCRIPT_FILENAME  /usr/local/nginx/html/$fastcgi_script_name;
            include        fastcgi_params;
        }
	其中
	fastcgi_pass   192.168.0.77:8091;
	的ip和端口修改成 cs_service.xml中的ip和端口
	htpasswdSOAP 文件放在nginx的conf目录下,可以使用apache的htpasswd2命令生成或修改
	htpasswd2 -b htpasswdSOAP soapuser *新密码*
	密码修改后,需配置../web/gmtools/gamecontrol/soap.php 中的 SoapClient(...."login" => "soapuser","password"=> "新密码"...)

5.配置cs_service.wsdl 在文件结尾处找到以下内容
	<service name="CsService" >
		<documentation>LJY CS Service </documentation>
		<port name="CsService" binding="s0:CsServiceSoap">
			<wsoap:address location="http://192.168.0.57:800/cgi-bin/cs_service"/>
		</port>
	</service>
	其中
	http://192.168.0.57:800/cgi-bin/cs_service
	需要修改成
	http://nginx内网ip:nginx端口/cgi-bin/cs_service

	修改完成后将文件cs_service.wsdl复制到 ../web/gmtools/cfg下面,覆盖同名文件

6.web 端的配置和使用请阅读 ../web/dep/目录下的文件 webtool_readme.txt
	第一次部署需要创建数据库gmtools,使用../web/dep/db/下面的sql创建数据库表
	修改default.conn.inc.php 配置数据库链接,配置完成后,将文件名改成conn.inc.php
	将../web/gmtools整个目录复制到nginx所在机器的对应目录下

7.web 端访问请用 ie8.0 以上版本或 chrome 19 以上版本

8.web 端配置密码验证(nginx):
	auth_basic            "FCGI";
	auth_basic_user_file  htpasswdFCGI;

	默认帐号密码:wanyouadmin admin#0912
	修改方式:htpasswd2 -b htpasswdFCGI wanyouadmin admin#0912

9.web 端php在

9.重启nginx
	
	
