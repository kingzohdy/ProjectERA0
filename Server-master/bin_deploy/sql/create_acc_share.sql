CREATE TABLE IF NOT EXISTS AccShare (
 Uin INT  NOT NULL  ,
 LastLogout INT    ,
 OfflineExp INT    ,
 TakeMudExp BIGINT    ,
 GrabTime INT    ,
PRIMARY KEY(Uin))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
