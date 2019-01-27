-- Create the world id

INSERT INTO lin2db.dbo.server (id, name, ip, inner_ip, ageLimit, pk_flag, kind, port)
VALUES (1, 'Test', '127.0.0.1', '127.0.0.1', 0, 1, 0, 7777);
  --        name        ip (of your server)
GO

-- Create root user and make in build master aka game master aka GM
-- WARNING: the default password is also 'root'. First thing you need to do 
-- after install is change it.

INSERT INTO lin2db.dbo.ssn(ssn,name,email,job,phone,zip,addr_main,addr_etc,account_num)
VALUES ('777','Admin','admin@somewhere.com',0,'telphone','123456','','',1);

INSERT INTO lin2db.dbo.user_account(account, pay_stat)
VALUES ('root', 1);

INSERT INTO lin2db.dbo.user_info(account,ssn,kind)
VALUES ('root','777', 99);

INSERT INTO lin2db.dbo.user_auth(account,password,quiz1,quiz2,answer1,answer2)
VALUES ('root', CONVERT(binary, 0xb1be70e9a83f19192cb593935ec4e2e2), '', '', CONVERT(binary, ''), CONVERT(binary, ''));

INSERT INTO lin2world.dbo.builder_account(account_name, account_id, default_builder)
VALUES ('root', 1, 1);
GO

-- insert into petition.dbo.NCDBA7 values (1,'connection string',10000,30,'petition');
   --                                        read notes about PetitionD!!!
-- GO
