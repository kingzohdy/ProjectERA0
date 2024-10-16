#CREATE DATABASE dirty;

CREATE TABLE if not exists `t_business` (
  `busid` int(11) not null auto_increment,
  `name` varchar(32) not null default '',
  `busname` varchar(40) default NULL,
  primary key(busid)
) ENGINE=MyISAM;

CREATE TABLE if not exists `t_dirty_bowl` (
  `id` int(11) not null auto_increment,
  `level` int(11) DEFAULT 1,
  `word` varchar(255) not null DEFAULT '',
  `type` char(1) not null DEFAULT 1,
  primary key(id),
  unique key key1(`word`, `type`)
) ENGINE=MyISAM;

CREATE TABLE if not exists `t_dirty_xxz` (
  `id` int(11) not null auto_increment,
  `level` int(11) DEFAULT 1,
  `word` varchar(255) not null DEFAULT '',
  `type` char(1) not null DEFAULT 1,
  primary key(id),
  unique key key1(`word`, `type`)
) ENGINE=MyISAM;

CREATE TABLE if not exists `t_role` (
  `roleid` int(11) NOT NULL auto_increment,
  `rolename` varchar(100) NOT NULL default '',
  PRIMARY KEY  (`roleid`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=gb2312; 

CREATE TABLE if not exists `t_user` (
  `userid` int(11) NOT NULL default '0',
  `username` varchar(100) NOT NULL default '',
  `roleid` int(11) NOT NULL default '0',
  `busid` int(11) NOT NULL default '0',
  PRIMARY KEY  (`userid`)
) ENGINE=MyISAM DEFAULT CHARSET=gb2312; 

insert into t_business values(1, "bowl", "´óÍë²Ë");
insert into t_business values(2, "xxz", "ÏÉÏÀ´«");

insert into t_dirty_bowl values(1, 1, "Áì½±|½±",1);
insert into t_dirty_bowl values(2, 2, "Âè|Âè",  1);
insert into t_dirty_bowl values(3, 1, "money",  2);
insert into t_dirty_bowl values(4, 10, "fuck",  2);
insert into t_dirty_bowl values(5, 2, "shit",   2);
insert into t_dirty_bowl values(6, 2, "cc", 	2);
insert into t_dirty_bowl values(7, 2, "dd", 	2);
insert into t_dirty_bowl values(8, 2, "ee", 	2);
insert into t_dirty_bowl values(9, 1, "falun", 	4);
insert into t_dirty_bowl values(10, 1, "[s|3]g[0-9a-zA-Z]{0,6}\.[0-9a-zA-Z]{2,6}\.", 	3);
insert into t_dirty_bowl values(11, 1, "µÇ[Â¼|Â½].*[Àñ|½±]", 							3);

insert into t_dirty_xxz values(1, 1, "Áì½±|½±",1);
insert into t_dirty_xxz values(2, 2, "Âè|Âè",  1);
insert into t_dirty_xxz values(3, 1, "money",  2);
insert into t_dirty_xxz values(4, 10, "fuck",  2);
insert into t_dirty_xxz values(5, 2, "shit",   2);
insert into t_dirty_xxz values(6, 2, "cc", 	2);
insert into t_dirty_xxz values(7, 2, "dd", 	2);
insert into t_dirty_xxz values(8, 2, "ee", 	2);
insert into t_dirty_xxz values(9, 1, "falun", 	4);
insert into t_dirty_xxz values(10, 1, "[s|3]g[0-9a-zA-Z]{0,6}\.[0-9a-zA-Z]{2,6}\.", 	3);
insert into t_dirty_xxz values(11, 1, "µÇ[Â¼|Â½].*[Àñ|½±]", 							3);


insert into t_role(rolename) values('Admin');
insert into t_role(rolename) values('User');

insert into t_user(userid,username,roleid,busid) values(4733,'kailu(Â¬¿­)',1,0);
