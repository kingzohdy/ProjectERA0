#COPYTO --PRE77--192.168.0.77:/usr/local/tjw/myworld/tools/shop_db_edit/readme.txt
2013-1-10 patch
更新内容:
增加数据库对应的区服信息
	bill_x 数据库旁显示区信息
	world_x_x 数据库旁显示对应区服信息

更新方法:
	解压后覆盖(不删除旧目录,因为有配置文件)nginx/html/phpmyadmin33目录

.............................................................................................
2013-1-9 部署相关

配置php
	源码构建的php 需要带参数--enable-soap 和 --with-mysqli

配置mysql
	创建数据库phpmyadmin及表:
		mysql < ./dep/create_tables.sql 

	新建用户shopuser来修改商城相关的数据库,
		CREATE USER 'shopuser'@'192.168.0.77' IDENTIFIED BY 'shopuser';
	其中ip
		192.168.0.77
	需要修改成部署nginx所在服务器的内网ip

	设置world_x_x数据库下面的商城相关表权限  SELECT, INSERT, UPDATE, REFERENCES, DELETE
		ShopDef
		ShopLimitDef
		ShopMsgDef
	

	例子(1区10服):
		GRANT SELECT, INSERT, UPDATE, REFERENCES, DELETE ON `world_1_10`.`ShopMsgDef` TO 'shopuser'@'%';
		GRANT SELECT, INSERT, UPDATE, REFERENCES, DELETE ON `world_1_10`.`ShopLimitDef` TO 'shopuser'@'%';
		GRANT SELECT, INSERT, UPDATE, REFERENCES, DELETE ON `world_1_10`.`ShopDef` TO 'shopuser'@'%';

	设置bill_x数据库下面的金币消耗相关表权限 SELECT
		GodCoinConsumeRecord
	
	例子(3区):
		GRANT SELECT ON bill_3.GodCoinConsumeRecord TO 'shopuser'@'%';
	
	设置数据库 `phpmyadmin` 相关的权限:
		GRANT SELECT, INSERT, UPDATE, REFERENCES, DELETE ON `phpmyadmin`.* TO 'shopuser'@'%';

配置 phpMyAdmin	
	修改 ./phpmyadmin33/config.inc.php
	将
	$cfg['Servers'][$i]['host'] = '192.168.0.77';
	中的ip 192.168.0.77修改成
	mysql所在服务器内网ip

	设置数据表跟踪
		使用shopuser登录后,选择数据库和表,点击"追踪"->"创建追踪"

nginx配置
	赋值 htpasswdSHOP 文件到 nginx/conf 目录下面

	配置nginx.conf 文件
	location ~ ^/phpmyadmin33/.*\.php$ {
	    auth_basic            "SHOP";
            auth_basic_user_file  htpasswdSHOP;
            root           html/phpmyadmin33;
            fastcgi_pass   127.0.0.1:9000;
            fastcgi_index  index.php3;
            fastcgi_param  SCRIPT_FILENAME  /usr/local/nginx/html/$fastcgi_script_name;
            include        fastcgi_params;
        }

	默认用户密码为shopuser shopuser
	
配置cs_service.wsdl
	文件位置: ./phpmyadmin33/cfg 目录下
	将文件名 cs_service.wsdl.default 修改为 cs_service.wsdl
	修改
	<wsoap:address location="http://192.168.0.57:800/cgi-bin/cs_service"/>
	将
	http://192.168.0.57:800/cgi-bin/cs_service
	修改成
	cs_service部署的内网地址


www.0dyx.cn
