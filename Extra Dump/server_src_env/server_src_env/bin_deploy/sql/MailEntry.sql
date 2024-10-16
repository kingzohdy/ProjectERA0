CREATE TABLE IF NOT EXISTS MailEntry (
 WID BIGINT UNSIGNED  NOT NULL  ,
 Recv VARCHAR(30)    ,
 Send VARCHAR(30)    ,
 Time INT    ,
 Title VARCHAR(32)    ,
 Flags INT    ,
 Detail BLOB    ,
PRIMARY KEY(WID))  ENGINE=InnoDB DEFAULT CHARSET=latin1;
create index MailEntry_Idx_0 on MailEntry ( Recv);
create index MailEntry_Idx_1 on MailEntry ( Time);
