#COPYTO --PRE77--192.168.0.77:/usr/local/tjw/myworld/tools/shop_db_edit/readme.txt
2013-1-10 patch
��������:
�������ݿ��Ӧ��������Ϣ
	bill_x ���ݿ�����ʾ����Ϣ
	world_x_x ���ݿ�����ʾ��Ӧ������Ϣ

���·���:
	��ѹ�󸲸�(��ɾ����Ŀ¼,��Ϊ�������ļ�)nginx/html/phpmyadmin33Ŀ¼

.............................................................................................
2013-1-9 �������

����php
	Դ�빹����php ��Ҫ������--enable-soap �� --with-mysqli

����mysql
	�������ݿ�phpmyadmin����:
		mysql < ./dep/create_tables.sql 

	�½��û�shopuser���޸��̳���ص����ݿ�,
		CREATE USER 'shopuser'@'192.168.0.77' IDENTIFIED BY 'shopuser';
	����ip
		192.168.0.77
	��Ҫ�޸ĳɲ���nginx���ڷ�����������ip

	����world_x_x���ݿ�������̳���ر�Ȩ��  SELECT, INSERT, UPDATE, REFERENCES, DELETE
		ShopDef
		ShopLimitDef
		ShopMsgDef
	

	����(1��10��):
		GRANT SELECT, INSERT, UPDATE, REFERENCES, DELETE ON `world_1_10`.`ShopMsgDef` TO 'shopuser'@'%';
		GRANT SELECT, INSERT, UPDATE, REFERENCES, DELETE ON `world_1_10`.`ShopLimitDef` TO 'shopuser'@'%';
		GRANT SELECT, INSERT, UPDATE, REFERENCES, DELETE ON `world_1_10`.`ShopDef` TO 'shopuser'@'%';

	����bill_x���ݿ�����Ľ��������ر�Ȩ�� SELECT
		GodCoinConsumeRecord
	
	����(3��):
		GRANT SELECT ON bill_3.GodCoinConsumeRecord TO 'shopuser'@'%';
	
	�������ݿ� `phpmyadmin` ��ص�Ȩ��:
		GRANT SELECT, INSERT, UPDATE, REFERENCES, DELETE ON `phpmyadmin`.* TO 'shopuser'@'%';

���� phpMyAdmin	
	�޸� ./phpmyadmin33/config.inc.php
	��
	$cfg['Servers'][$i]['host'] = '192.168.0.77';
	�е�ip 192.168.0.77�޸ĳ�
	mysql���ڷ���������ip

	�������ݱ����
		ʹ��shopuser��¼��,ѡ�����ݿ�ͱ�,���"׷��"->"����׷��"

nginx����
	��ֵ htpasswdSHOP �ļ��� nginx/conf Ŀ¼����

	����nginx.conf �ļ�
	location ~ ^/phpmyadmin33/.*\.php$ {
	    auth_basic            "SHOP";
            auth_basic_user_file  htpasswdSHOP;
            root           html/phpmyadmin33;
            fastcgi_pass   127.0.0.1:9000;
            fastcgi_index  index.php3;
            fastcgi_param  SCRIPT_FILENAME  /usr/local/nginx/html/$fastcgi_script_name;
            include        fastcgi_params;
        }

	Ĭ���û�����Ϊshopuser shopuser
	
����cs_service.wsdl
	�ļ�λ��: ./phpmyadmin33/cfg Ŀ¼��
	���ļ��� cs_service.wsdl.default �޸�Ϊ cs_service.wsdl
	�޸�
	<wsoap:address location="http://192.168.0.57:800/cgi-bin/cs_service"/>
	��
	http://192.168.0.57:800/cgi-bin/cs_service
	�޸ĳ�
	cs_service�����������ַ


www.0dyx.cn
