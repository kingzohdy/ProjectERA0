CREATE TABLE IF NOT EXISTS ljy_member_profile (
  uid mediumint(8) unsigned NOT NULL,
  username char(32) NOT NULL DEFAULT '',
  realname varchar(255) NOT NULL DEFAULT '',
  gender tinyint(1) NOT NULL DEFAULT '0',
  birthday varchar(9) NOT NULL DEFAULT '0',
  telephone varchar(255) NOT NULL DEFAULT '',
  mobile varchar(255) NOT NULL DEFAULT '',
  idcard varchar(255) NOT NULL DEFAULT '',
  address varchar(255) NOT NULL DEFAULT '',
  zipcode varchar(255) NOT NULL DEFAULT '',
  qq varchar(255) NOT NULL DEFAULT '',
  authstr varchar(20) NOT NULL DEFAULT '',
  isactive tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (uid)
) ENGINE=MyISAM;
