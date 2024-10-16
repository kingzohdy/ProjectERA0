CREATE TABLE IF NOT EXISTS Account (
 Uin INT  NOT NULL AUTO_INCREMENT ,
 UinFlag INT    ,
 Pass9Account VARCHAR(51)    ,
 PasswdHash BLOB    ,
 BirthDay VARCHAR(9)    ,
 SilenceEndTime INT    ,
 FreezeEndTime INT    ,
 KEY(Uin),
PRIMARY KEY(Uin),
UNIQUE (Pass9Account))  ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1000;
create index Account_Idx_0 on Account ( Pass9Account);
