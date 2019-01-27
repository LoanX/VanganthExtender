USE [lin2db]
GO

CREATE TABLE [dbo].[acc_history] (
  [date] smalldatetime NULL,
  [acc_cnt] int NULL,
  [acc_total] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[account_quiz] (
  [account_id] int NOT NULL,
  [answer1] binary(32) NULL,
  [answer2] binary(32) NULL,
  [quiz1] nvarchar(255) COLLATE Korean_Wansung_CI_AS NULL,
  [quiz2] nvarchar(255) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[age_cnt] (
  [date] smalldatetime NULL,
  [birth_year] varchar(4) COLLATE Korean_Wansung_CI_AS NULL,
  [sex] varchar(2) COLLATE Korean_Wansung_CI_AS NULL,
  [ssn_cnt] int NULL,
  [acc_cnt] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[ban_pay] (
  [account] char(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [telnum] varchar(30) COLLATE Korean_Wansung_CI_AS NULL,
  [method] int NOT NULL,
  [flag] tinyint NOT NULL,
  [status] tinyint NOT NULL,
  [ban_memo_id] int NULL,
  [able_memo_id] int NULL,
  [ban_date] datetime NULL,
  [able_date] datetime NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[ban_ssn] (
  [idx] int IDENTITY(1, 1) NOT NULL,
  [ssn] char(13) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [reason] tinyint NOT NULL,
  [author_id] varchar(30) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [op_date] datetime NOT NULL,
  [memo_id] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[block_history] (
  [idx] int IDENTITY(1, 1) NOT NULL,
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [block_date] datetime NOT NULL,
  [duration] int DEFAULT 0 NOT NULL,
  [release_date] datetime NULL,
  [status] tinyint NOT NULL,
  [block_kind] tinyint NOT NULL,
  [block_reason] int NOT NULL,
  [blocked_pay_id] char(16) COLLATE Korean_Wansung_CI_AS NULL,
  [remain_day] int NULL,
  [recomp_flag] smallint DEFAULT 0 NOT NULL,
  [recomped_pay_id] char(16) COLLATE Korean_Wansung_CI_AS NULL,
  [track_date] char(8) COLLATE Korean_Wansung_CI_AS NULL,
  [track_serial] smallint NULL,
  [flag] tinyint NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[block_msg] (
  [uid] int NULL,
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [msg] varchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [reason] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[block_reason_code] (
  [block_reason] int NOT NULL,
  [block_desc] varchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [flag] tinyint NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[cdkey_user] (
  [date] smalldatetime NULL,
  [gender] smallint NULL,
  [age] tinyint NULL,
  [cdkind] smallint NULL,
  [zip] varchar(4) COLLATE Korean_Wansung_CI_AS NULL,
  [acc_cnt] int NULL,
  [use_time] bigint NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[change_ssn_history] (
  [login_id] varchar(48) COLLATE Korean_Wansung_CI_AS NULL,
  [net_address] nchar(15) COLLATE Korean_Wansung_CI_AS NULL,
  [hostname] nchar(48) COLLATE Korean_Wansung_CI_AS NULL,
  [pgmname] nchar(48) COLLATE Korean_Wansung_CI_AS NULL,
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NULL,
  [old_ssn] varchar(13) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [new_ssn] varchar(13) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [old_real_name] varchar(20) COLLATE Korean_Wansung_CI_AS NULL,
  [new_real_name] varchar(20) COLLATE Korean_Wansung_CI_AS NULL,
  [apply_date] datetime NOT NULL,
  [manager] varchar(20) COLLATE Korean_Wansung_CI_AS NULL,
  [requestor] tinyint NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[check_sms] (
  [idx] int IDENTITY(1, 1) NOT NULL,
  [mobile] varchar(15) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [key_num] varchar(10) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [reg_date] datetime DEFAULT getdate() NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[client_error_log] (
  [logdate] datetime DEFAULT getdate() NULL,
  [error_str] varchar(500) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[gm_illegal_login] (
  [account] varchar(15) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [try_date] datetime DEFAULT getdate() NOT NULL,
  [ip] varchar(15) COLLATE Korean_Wansung_CI_AS NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[history_conuser] (
  [date] smalldatetime NULL,
  [age] tinyint NULL,
  [stat] smallint NULL,
  [acc_cnt] int NULL,
  [ssn_cnt] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[history_times] (
  [date] smalldatetime NULL,
  [age] tinyint NULL,
  [stat] smallint NULL,
  [use_time] bigint NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[item_code] (
  [item_id] smallint NOT NULL,
  [name] varchar(20) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [duration] int NULL,
  [active_date] datetime NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[key_email] (
  [idx] int IDENTITY(1, 1) NOT NULL,
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [email] varchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [key_num] varchar(10) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [reg_date] datetime DEFAULT getdate() NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[local_cnt] (
  [date] smalldatetime NULL,
  [zip] varchar(6) COLLATE Korean_Wansung_CI_AS NULL,
  [ssn_cnt] int NULL,
  [acc_cnt] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[Notice] (
  [id] int IDENTITY(1, 1) NOT NULL,
  [notice] nvarchar(1000) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[pay_master] (
  [pay_id] char(16) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [play_start] datetime NULL,
  [play_end] datetime NULL,
  [real_end_date] datetime NULL,
  [method] int NOT NULL,
  [item_id] int NOT NULL,
  [real_amount] int DEFAULT 0 NOT NULL,
  [pay_date] datetime DEFAULT getdate() NOT NULL,
  [loc] tinyint DEFAULT 0 NOT NULL,
  [origin_pay_id] char(16) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[pay_method_code] (
  [method] int NOT NULL,
  [name] varchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [feature] int NOT NULL,
  [amount] char(10) COLLATE Korean_Wansung_CI_AS NULL,
  [duration] int NULL,
  [active_date] datetime NULL,
  [feeperday] money NULL,
  [feeperminute] money NULL,
  [feepersec] money NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[server] (
  [id] int NOT NULL,
  [name] varchar(50) COLLATE Cyrillic_General_CI_AS NOT NULL,
  [ip] varchar(15) COLLATE Cyrillic_General_CI_AS NOT NULL,
  [inner_ip] varchar(15) COLLATE Cyrillic_General_CI_AS NOT NULL,
  [ageLimit] int NOT NULL,
  [pk_flag] tinyint DEFAULT 1 NOT NULL,
  [kind] int DEFAULT 0 NOT NULL,
  [port] int DEFAULT 7777 NOT NULL,
  [region] int DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[ssn] (
  [ssn] char(13) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [name] varchar(15) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [email] varchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [newsletter] tinyint DEFAULT 0 NOT NULL,
  [job] int NOT NULL,
  [phone] varchar(16) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [mobile] varchar(20) COLLATE Korean_Wansung_CI_AS NULL,
  [reg_date] datetime DEFAULT getdate() NOT NULL,
  [zip] varchar(6) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [addr_main] varchar(255) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [addr_etc] varchar(255) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [account_num] tinyint DEFAULT 0 NOT NULL,
  [status_flag] int DEFAULT 0 NOT NULL,
  [final_news_date] datetime NULL,
  [master] varchar(14) COLLATE Korean_Wansung_CI_AS NULL,
  [valid_email_date] datetime NULL,
  [final_master_date] datetime DEFAULT getdate() NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[ssn_history] (
  [date] smalldatetime NULL,
  [ssn_cnt] int NULL,
  [ssn_total] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[statis_code] (
  [pay_stat] int NULL,
  [good_code] int NULL,
  [age_type] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[temp_123] (
  [t_stat] varchar(2) COLLATE Korean_Wansung_CI_AS NULL,
  [t_usedate] varchar(10) COLLATE Korean_Wansung_CI_AS NULL,
  [t_usetime] varchar(10) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[temp_pwd] (
  [account] varchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [oldpwd] binary(16) NOT NULL,
  [newpwd] binary(16) NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[temp_stat] (
  [t_date] char(10) COLLATE Korean_Wansung_CI_AS NULL,
  [t_count] varchar(10) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[test_applicant] (
  [idx] int NOT NULL,
  [ssn] char(13) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [name] varchar(15) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [email] varchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [reg_date] datetime NOT NULL,
  [kind] int NOT NULL,
  [status] tinyint NOT NULL,
  [cpu] smallint NOT NULL,
  [ram] smallint NOT NULL,
  [vga] smallint NOT NULL,
  [os] smallint NOT NULL,
  [mobile] varchar(15) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [zip] varchar(6) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [addr_main] varchar(255) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [addr_etc] varchar(255) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [career] varchar(500) COLLATE Korean_Wansung_CI_AS NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[test_flag] (
  [flag] char(3) COLLATE Korean_Wansung_CI_AS NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[Untitled - 1] (
  [RowNumber] int IDENTITY(1, 1) NOT NULL,
  [EventClass] int NULL,
  [TextData] ntext COLLATE Korean_Wansung_CI_AS NULL,
  [NTUserName] nvarchar(128) COLLATE Korean_Wansung_CI_AS NULL,
  [ClientProcessID] int NULL,
  [ApplicationName] nvarchar(128) COLLATE Korean_Wansung_CI_AS NULL,
  [LoginName] nvarchar(128) COLLATE Korean_Wansung_CI_AS NULL,
  [SPID] int NULL,
  [Duration] bigint NULL,
  [StartTime] datetime NULL,
  [Reads] bigint NULL,
  [Writes] bigint NULL,
  [CPU] int NULL
)
ON [PRIMARY]
TEXTIMAGE_ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_account] (
  [uid] int IDENTITY(1, 1) NOT NULL,
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [pay_stat] int DEFAULT 0 NOT NULL,
  [login_flag] int DEFAULT 0 NOT NULL,
  [warn_flag] int DEFAULT 0 NOT NULL,
  [block_flag] int DEFAULT 0 NOT NULL,
  [block_flag2] int DEFAULT 0 NOT NULL,
  [block_end_date] datetime NULL,
  [last_login] datetime NULL,
  [last_logout] datetime NULL,
  [subscription_flag] int DEFAULT 0 NOT NULL,
  [last_world] tinyint NULL,
  [last_game] int NULL,
  [last_ip] varchar(15) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_auth] (
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [password] binary(16) NOT NULL,
  [quiz1] varchar(255) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [quiz2] varchar(255) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [answer1] binary(32) NOT NULL,
  [answer2] binary(32) NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_block_daily] (
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [serial] int NULL,
  [block_date] datetime DEFAULT getdate() NOT NULL,
  [duration] int DEFAULT 0 NOT NULL,
  [reason] int NOT NULL,
  [flag] tinyint NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_char] (
  [uid] int NOT NULL,
  [world_id] int NOT NULL,
  [char_id] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_count] (
  [record_time] datetime DEFAULT getdate() NOT NULL,
  [server_id] tinyint NOT NULL,
  [world_user] smallint NOT NULL,
  [limit_user] smallint NOT NULL,
  [auth_user] int NOT NULL,
  [wait_user] int NOT NULL,
  [dayofweek] tinyint DEFAULT datepart(weekday,getdate()) NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_data] (
  [uid] int NOT NULL,
  [user_data] int DEFAULT 0 NOT NULL,
  [user_char_num] int DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_guardian] (
  [idx] int NOT NULL,
  [ssn] char(13) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [g_name] varchar(25) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [g_ssn] char(13) COLLATE Korean_Wansung_CI_AS NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_info] (
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [create_date] datetime DEFAULT getdate() NOT NULL,
  [ssn] varchar(13) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [status_flag] tinyint DEFAULT 0 NOT NULL,
  [kind] int DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_pay] (
  [pay_id] char(16) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [start_date] datetime DEFAULT getdate() NOT NULL,
  [method] int NOT NULL,
  [duration] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_pay_recomp] (
  [pay_id] char(16) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [duration] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_pay_reserve] (
  [pay_id] char(16) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [pay_date] datetime NOT NULL,
  [method] int NOT NULL,
  [duration] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_time] (
  [uid] int NOT NULL,
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [present_time] int NOT NULL,
  [next_time] int NULL,
  [total_time] int NOT NULL,
  [op_date] datetime NOT NULL,
  [flag] tinyint NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[worldstatus] (
  [idx] int NOT NULL,
  [server] varchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [status] tinyint DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

ALTER TABLE [dbo].[account_quiz]
ADD CONSTRAINT [PK_account_password] 
PRIMARY KEY CLUSTERED ([account_id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[ban_ssn]
ADD CONSTRAINT [PK_ban_ssn] 
PRIMARY KEY CLUSTERED ([ssn])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_block_account] ON [dbo].[block_history]
  ([account])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_blocked_pay_id] ON [dbo].[block_history]
  ([blocked_pay_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[block_history]
ADD CONSTRAINT [PK_block_history] 
PRIMARY KEY CLUSTERED ([idx])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_account] ON [dbo].[block_msg]
  ([account])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE CLUSTERED INDEX [IX_block_msg] ON [dbo].[block_msg]
  ([uid])
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_block_reason] ON [dbo].[block_reason_code]
  ([block_reason])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX1_change_ssn_history] ON [dbo].[change_ssn_history]
  ([account])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [ix1_check_sms] ON [dbo].[check_sms]
  ([mobile])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE CLUSTERED INDEX [PK_item_id] ON [dbo].[item_code]
  ([item_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [ix1_key_email] ON [dbo].[key_email]
  ([email])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE CLUSTERED INDEX [IX1_pay_master] ON [dbo].[pay_master]
  ([account])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[pay_master]
ADD CONSTRAINT [PK_pay_master] 
PRIMARY KEY NONCLUSTERED ([pay_id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[ssn]
ADD CONSTRAINT [PK_ssn] 
PRIMARY KEY CLUSTERED ([ssn])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[test_applicant]
ADD CONSTRAINT [PK_test_applicant] 
PRIMARY KEY CLUSTERED ([ssn])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[Untitled - 1]
ADD PRIMARY KEY CLUSTERED ([RowNumber])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_account]
ADD CONSTRAINT [PK_user_account_] 
PRIMARY KEY CLUSTERED ([account])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_auth]
ADD CONSTRAINT [PK_user_auth] 
PRIMARY KEY CLUSTERED ([account])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_serial] ON [dbo].[user_block_daily]
  ([serial])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_block_daily]
ADD CONSTRAINT [PK_user_block_daily] 
PRIMARY KEY CLUSTERED ([account])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_dayofweek] ON [dbo].[user_count]
  ([dayofweek])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_record] ON [dbo].[user_count]
  ([record_time])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_serverid] ON [dbo].[user_count]
  ([server_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_info]
ADD CONSTRAINT [PK_user_info] 
PRIMARY KEY CLUSTERED ([account])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_pay_id] ON [dbo].[user_pay]
  ([pay_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_start_date] ON [dbo].[user_pay]
  ([start_date])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_pay]
ADD CONSTRAINT [PK_user_pay] 
PRIMARY KEY CLUSTERED ([account])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE UNIQUE NONCLUSTERED INDEX [idx_pay_id] ON [dbo].[user_pay_recomp]
  ([pay_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_pay_recomp]
ADD CONSTRAINT [PK_user_pay_recomp] 
PRIMARY KEY CLUSTERED ([account])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_reserve_payid] ON [dbo].[user_pay_reserve]
  ([pay_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_pay_reserve]
ADD CONSTRAINT [PK_user_pay_reserve] 
PRIMARY KEY CLUSTERED ([account])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_account] ON [dbo].[user_time]
  ([account])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_time]
ADD CONSTRAINT [PK_user_time] 
PRIMARY KEY CLUSTERED ([uid])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[worldstatus]
ADD CONSTRAINT [PK_worldstatus] 
PRIMARY KEY CLUSTERED ([idx])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE PROCEDURE [dbo].[ap_GPwd]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[ap_GStat]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[ap_GUserTime]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[ap_SLog]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[ap_SUserTime]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_BlockAdd]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_BlockRemove]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_CheckPayAbility]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_CheckTelnumAbility]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_GetAccountInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_GetUserPay]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_outsideraccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserDuration]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserDurationByArsCypass]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserDurationByArsInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserDurationByCard]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserDurationByDeposit]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserDurationByHp]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserDurationByHpBack]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserDurationByiBanking]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserDurationOrTimeByCoupon]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserTime]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserTimeByArsCypass]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserTimeByArsInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserTimeByCard]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserTimeByHp]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_SetUserTimeByHpBack]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[l2p_TempCreateAccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_addnewcreateaccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_changemasteraccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_changepassword]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_changepassword2]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_changequiz]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_changeuserinfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_checkanswer]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_checkmasteraccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_checkssn]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateAccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateDummyAccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteAccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_getquiz]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_getquiz2]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_getuserinfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoginAccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LogoutAccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_newcreateaccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lp_CreateAccount_temp]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lp_del_account]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lp_del_account2]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lp_l2staticUsetime]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lp_l2statisticAgeGender]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lp_L2statisticConn]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lp_L2statisticConn_HH]

AS
SELECT 0
GO

-- Created By darkangel
-- 2003.06.19
-- Get Account's Password

ALTER PROCEDURE [dbo].[ap_GPwd]  @account varchar(14), @pwd binary(16) output
AS
SELECT @pwd=password FROM user_auth with (nolock) WHERE account=@account
GO

ALTER PROCEDURE [dbo].[ap_GStat]
@account varchar(14), 
@uid int OUTPUT, 
@payStat int OUTPUT, 
@loginFlag int OUTPUT, 
@warnFlag int OUTPUT, 
@blockFlag int OUTPUT, 
@blockFlag2 int OUTPUT, 
@subFlag int OUTPUT, 
@lastworld tinyint OUTPUT,
@block_end_date datetime OUTPUT
 AS
SELECT @uid=uid, 
	 @payStat=pay_stat,
              @loginFlag = login_flag, 
              @warnFlag = warn_flag, 
              @blockFlag = block_flag, 
              @blockFlag2 = block_flag2, 
              @subFlag = subscription_flag , 
              @lastworld=last_world, 
              @block_end_date=block_end_date 
               FROM user_account WITH (nolock)
WHERE account=@account
GO

ALTER PROCEDURE [dbo].[ap_GUserTime]  @uid int, @userTime int OUTPUT
AS
SELECT @userTime=total_time FROM user_time WITH (nolock)
WHERE uid = @uid
GO

ALTER PROCEDURE [dbo].[ap_SLog] 
@uid int, @lastlogin datetime, @lastlogout datetime, @LastGame int, @LastWorld tinyint, @LastIP varchar(15)
AS
UPDATE user_account 
SET last_login = @lastlogin, last_logout=@lastlogout, last_world=@lastWorld, last_game=@lastGame, last_ip=@lastIP
WHERE uid=@uid
GO

ALTER PROCEDURE [dbo].[ap_SUserTime] @useTime int, @uid int
AS
UPDATE user_time SET total_time = total_time - @useTime, present_time = present_time - @useTime
WHERE uid = @uid
GO

ALTER PROCEDURE dbo.dp_BlockAdd
	@in_account VARCHAR(14),
	@in_block_reason INT,	--block_reason_code ??? ??
	@in_block_flag_switch TINYINT,	--block_flag? ???? ?? 1, block_flag2? ???? ?? 2
	@in_duration INT,	--?????? ?? ??, 0?? ??? ??
	@in_track_date CHAR(8),	--??????? ???? NULL ?? ??0? ???
	@in_track_serial SMALLINT	--??????? ???? NULL ?? 0
AS
SET NOCOUNT ON


--?? ??
DECLARE @retcode INT
DECLARE @retmsg VARCHAR(1024)
DECLARE @block_flag INT	--?? ??? block_flag
DECLARE @new_block_flag INT	--?? ??? block_flag
DECLARE @block_kind TINYINT
DECLARE @idx_block_history INT
--//?? ??


--???? ??
IF LEN(@in_track_date) = 0
	SET @in_track_date = NULL
IF @in_track_serial = 0
	SET @in_track_serial = NULL


--??? ?? ??
IF @in_account IS NULL OR LEN(@in_account) = 0
BEGIN
	SET @retcode = -101
	SET @retmsg = '@in_account ? ??? ?? ???? ?????.'
	GOTO END_NO_ROLLBACK
END
ELSE IF @in_block_reason IS NULL OR @in_block_reason = 0
BEGIN
	SET @retcode = -102
	SET @retmsg = '@in_block_reason ? ??? ?? ???? ?????.'
	GOTO END_NO_ROLLBACK
END
ELSE IF @in_block_flag_switch NOT IN (1, 2)
BEGIN
	SET @retcode = -103
	SET @retmsg = '@in_block_flag_switch ? ??? ?? ???? ?????.'
	GOTO END_NO_ROLLBACK
END
ELSE IF @in_duration IS NULL OR @in_duration < 0
BEGIN
	SET @retcode = -104
	SET @retmsg = '@in_duration ? ??? ?? ???? ?????.'
	GOTO END_NO_ROLLBACK
END
ELSE IF @in_track_date IS NOT NULL AND LEN(@in_track_date) <> 8
BEGIN
	SET @retcode = -105
	SET @retmsg = '@in_track_date ? ??? ?? ???? ?????.'
	GOTO END_NO_ROLLBACK
END
ELSE IF @in_track_serial IS NOT NULL AND @in_track_serial <= 0
BEGIN
	SET @retcode = -106
	SET @retmsg = '@in_track_serial ? ??? ?? ???? ?????.'
	GOTO END_NO_ROLLBACK
END
ELSE IF @in_track_date IS NOT NULL AND @in_track_serial IS NULL
BEGIN
	SET @retcode = -107
	SET @retmsg = '@in_track_date, @in_track_serial ? ??? ?? ???? ?????.'
	GOTO END_NO_ROLLBACK
END
ELSE IF @in_track_date IS NULL AND @in_track_serial IS NOT NULL
BEGIN
	SET @retcode = -108
	SET @retmsg = '@in_track_date, @in_track_serial ? ??? ?? ???? ?????.'
	GOTO END_NO_ROLLBACK
END
ELSE IF @in_track_date IS NOT NULL AND @in_block_reason NOT IN (1, 2, 4)
BEGIN
	SET @retcode = -109
	SET @retmsg = '@in_block_reason ? ???? ??? ????.'
	GOTO END_NO_ROLLBACK
END
ELSE IF @in_track_date IS NOT NULL AND @in_block_flag_switch <> 2
BEGIN
	SET @retcode = -111
	SET @retmsg = '@in_block_flag_switch ? ???????.'
	GOTO END_NO_ROLLBACK
END
--//??? ?? ??


--??? ??? ??, ?? ??? ?? ??? ???? ??? ??
IF @in_duration = 0
BEGIN
	IF @in_track_date IS NULL	
	--????? ?? ??
	BEGIN
		IF (SELECT COUNT(*) FROM block_history WHERE 
				account = @in_account AND status = 1 AND block_reason = @in_block_reason) > 0
		BEGIN
			SET @retcode = -12
			SET @retmsg = '?? ??? ?? ??? ?? ??? ???? ????.'
			GOTO END_NO_ROLLBACK
		END
	END
	ELSE
	--????? ??, ?? ??? ?? ?? ?? ???? ??? ??
	BEGIN
		IF (SELECT COUNT(*) FROM block_history WHERE 
				account = @in_account AND status = 1
				AND track_date = @in_track_date AND track_serial = @in_track_serial) > 0
		BEGIN
			SET @retcode = -11
			SET @retmsg = '?? ??? ?? ?????? ??? ?? ??? ???? ????.'
			GOTO END_NO_ROLLBACK
		END
	END
END


--??? ????? ??
IF (SELECT COUNT(*) FROM user_account WHERE account = @in_account) = 0
BEGIN
	SET @retcode = -17
	SET @retmsg = '?? ??? ???? ????.'
	GOTO END_NO_ROLLBACK
END

--@new_block_flag ???; ????? ????? block_flag? ??? ???.
IF @in_block_flag_switch = 1
	SELECT @block_flag = block_flag FROM user_account WHERE account = @in_account
ELSE IF @in_block_flag_switch = 2
	SELECT @block_flag = block_flag2 FROM user_account WHERE account = @in_account
SET @new_block_flag = @block_flag | @in_block_reason
IF @@ERROR <> 0 GOTO END_NO_ROLLBACK


--@block_kind ???
IF @in_duration > 0
	SET @block_kind = 1
ELSE
	SET @block_kind = 0
IF @@ERROR <> 0 GOTO END_NO_ROLLBACK


BEGIN TRAN	-------------------------------------------------------------------


--block_history ??
INSERT INTO block_history 
	(account, block_date, status, block_reason, duration, block_kind, flag, track_date, track_serial)
VALUES
	(@in_account, GETDATE(), 1, @in_block_reason, @in_duration, @block_kind, @in_block_flag_switch, @in_track_date, @in_track_serial)
SET @idx_block_history = @@IDENTITY
IF @@ERROR <> 0 GOTO END_ROLLBACK


--?????? ?? user_block_daily ? ??
IF @in_duration > 0
BEGIN
	--todo: ??? ????? ?? ??, ? ?? block_flag? ??? ????? ??? ??
	INSERT INTO user_block_daily
		(account, serial, block_date, duration, reason, flag)
	VALUES
		(@in_account, @idx_block_history, GETDATE(), @in_duration, @in_block_reason, @in_block_flag_switch)
END
IF @@ERROR <> 0 GOTO END_ROLLBACK


--@new_block_flag ??
IF @in_block_flag_switch = 1
	UPDATE user_account SET block_flag = @new_block_flag WHERE account = @in_account
ELSE IF @in_block_flag_switch = 2
	UPDATE user_account SET block_flag2 = @new_block_flag WHERE account = @in_account
IF @@ERROR <> 0 GOTO END_ROLLBACK


SET @retcode = 1
SET @retmsg = '?? ???????.'


--todo: ?? ?? ??


COMMIT TRAN	-------------------------------------------------------------------
SELECT @retcode AS retcode, @retmsg AS retmsg
RETURN


END_ROLLBACK:
	ROLLBACK TRAN
END_NO_ROLLBACK:
	IF @retcode IS NULL
	BEGIN
		SET @retcode = -999
		SET @retmsg = '?? ?? ??? ??????.'
	END
	SELECT @retcode AS retcode, @retmsg AS retmsg
GO

ALTER PROCEDURE dbo.dp_BlockRemove
	@idx_block_history INT
AS
SET NOCOUNT ON


--?? ??
DECLARE @retcode INT
DECLARE @retmsg VARCHAR(1024)

DECLARE @block_flag INT	--?? ??? block_flag
DECLARE @new_block_flag INT	--?? ??? block_flag

DECLARE @account VARCHAR(14)
DECLARE @status TINYINT
DECLARE @block_reason INT
DECLARE @block_kind TINYINT
DECLARE @block_flag_switch TINYINT
--//?? ??


SELECT 
	@account = account, @status = status, @block_reason = block_reason, 
	@block_kind = block_kind, @block_flag_switch = flag
FROM block_history WHERE idx = @idx_block_history
IF @@ERROR <> 0
BEGIN
	SET @retcode = -7
	SET @retmsg = '?? ?? ??? ???? ???? ??? ??????.'
	GOTO END_NO_ROLLBACK
END


--??? ?? ??
IF @account IS NULL OR LEN(@account) = 0
BEGIN
	SET @retcode = -101
	SET @retmsg = '?? ??? ????.'
	GOTO END_NO_ROLLBACK
END
ELSE IF @status <> 1
BEGIN
	SET @retcode = -102
	SET @retmsg = '?? ?? ?? ???? ??? ????.'
	GOTO END_NO_ROLLBACK
END
--//??? ?? ??


--@new_block_flag ???
IF @block_flag_switch = 1
	SELECT @block_flag = block_flag FROM user_account WHERE account = @account
ELSE IF @block_flag_switch = 2
	SELECT @block_flag = block_flag2 FROM user_account WHERE account = @account
SET @new_block_flag = @block_flag & (~@block_reason)
IF @@ERROR <> 0
BEGIN
	SET @retcode = -17
	SET @retmsg = '?? ?? ??? ???? ???? ??? ??????.'
	GOTO END_NO_ROLLBACK
END
--//@new_block_flag ???


BEGIN TRAN	-------------------------------------------------------------------


--??? ??? ??, ?? ??? ? ? ??? ????? ?? ??? block_flag? ??????.
IF @block_kind = 0
BEGIN
	IF (SELECT COUNT(*) FROM block_history WHERE 
			account = @account AND status = 1 AND block_reason = @block_reason
			AND flag = @block_flag_switch AND idx <> @idx_block_history) = 0
	BEGIN
		IF @block_flag_switch = 1
			UPDATE user_account SET block_flag = @new_block_flag WHERE account = @account
		ELSE IF @block_flag_switch = 2
			UPDATE user_account SET block_flag2 = @new_block_flag WHERE account = @account
		IF @@ERROR <> 0 GOTO END_ROLLBACK		
	END
END


--block_history ????
UPDATE block_history SET
	release_date = GETDATE(), status = 0
WHERE idx = @idx_block_history
IF @@ERROR <> 0 GOTO END_ROLLBACK


--todo: ??? ????? ?? ??, ? ?? block_flag? ??? ????? ??? ??


SET @retcode = 1
SET @retmsg = '?? ???? ?????.'


--todo: ?? ?? ??


COMMIT TRAN	-------------------------------------------------------------------
SELECT @retcode AS retcode, @retmsg AS retmsg
RETURN


END_ROLLBACK:
	ROLLBACK TRAN
END_NO_ROLLBACK:
	IF @retcode IS NULL
	BEGIN
		SET @retcode = -999
		SET @retmsg = '?? ?? ??? ??????.'
	END
	SELECT @retcode AS retcode, @retmsg AS retmsg
GO

/*
	@ret_val description by  JuYoung 2003-07-25
			-4 : ????? ?? ?????.(ssn ssn error)
			-3 : ?? ??? ???????.(user_info ssn error)
			-2 : ??? ?? ?? ????.
			-1 : db error
			 1 : ?? ?? 
			 2 : ?? 11? 55?~12? 5?? ?? ?? ? ????.
			 3 : ?? 11? 30?~12? 30?? ?? ?? ? ????.
			 4 : ??? ???? ?????, ??? ??? ??? ?? ?????.5??? ?? ??? ??? ????. (pay_lock error)
			 5 : ??? ???? ??? ? ????. ????? ????? ????.(user_account block_flag, block_flag2)
			 6 : ????? ?? ?? ?? ?????. ????? ????? ????. (ban_pay account)
			 7 :?? ???? ?? ????, ?? ?? ??? ?? ?? ??, ?? ? ??? ????.(user_pay_reserve)
			 8 :?? ??? ???? ?? 7? ??? ??? ?????. ?? ?? ?? 7? ?? ?? ????? ????.
			 9 :????? ?? ?????, ?? ?? ? ????.
			*  result : 1 ?? ??, ???? ??? ?? ???
*/
ALTER PROCEDURE [dbo].[dp_CheckPayAbility] 
	@account varchar(14),
	@ret_val integer output
AS
set nocount on
declare @buffer varchar(25), @pay_stat integer, @block_flag integer, @block_flag2 integer, @warn_flag integer, @ssn char(13), @cur_pay_stat integer
declare @endofmonth datetime, @fromlimithours datetime, @tolimithours datetime, @fromnighthours datetime, @tonighthours datetime

select @ssn=ssn from user_info with (nolock) where account=@account
if @@rowcount = 0
begin
	set @ret_val = -3
	return
end
else if len(@ssn) <> 13
begin
	set @ret_val = -4
	return
end
if (@@error <> 0) goto do_return

select @pay_stat=pay_stat, @warn_flag=warn_flag&2, @block_flag=block_flag, @block_flag2=block_flag2 from user_account with (nolock) where account=@account
if @@rowcount = 0
begin
	set @ret_val = -2
	return
end
if (@@error <> 0) goto do_return

set @endofmonth = dateadd(day, -1, dateadd(month, 1, convert(char(8), getdate(), 120) + '01'))
set @fromlimithours =  dateadd(minute, 1410, @endofmonth)
set @tolimithours = dateadd(minute, 1470, @endofmonth)
set @fromnighthours = dateadd(minute, 1435, getdate())
set @tonighthours = dateadd(minute, 1445, getdate())

if (@fromnighthours >= getdate() and @tonighthours <= getdate())
begin
	set @ret_val = 2
	return
end
if (@@error <> 0) goto do_return

if (@fromlimithours >= getdate() and @tolimithours <= getdate())
begin
	set @ret_val = 3
	return
end
if (@@error <> 0) goto do_return

select @buffer=account from dbo.pay_lock with (nolock) where account=@account and method/1000 <> 3
if @@rowcount > 0
begin
	set @ret_val = 4
	return
end
if (@@error <> 0) goto do_return

if @block_flag <> 0 or @block_flag2 <> 0
begin
	set @ret_val = 5
	return
end
if (@@error <> 0) goto do_return

select @buffer=account from ban_pay with (nolock) where account=@account and flag=1 and status=1
if @@rowcount > 0
begin
	set @ret_val = 6
	return
end
if (@@error <> 0) goto do_return

if @warn_flag <> 0 
begin
	set @ret_val = 9
	return
end 
if (@@error <> 0) goto do_return

set  @cur_pay_stat = (@pay_stat/100)%10

if @pay_stat <> 0
begin
	select @buffer=account from user_pay_reserve with (nolock) where account=@account
	if @@rowcount > 0
	begin
		set @ret_val = 7
		return
	end
	else
	begin
		select @buffer=account from user_pay with (nolock) where account=@account
		if @@rowcount = 0
		begin
			set @ret_val = -3
			return
		end
		if @cur_pay_stat = 1
		begin
			select @buffer=account from user_pay with (nolock) where account=@account and  ((method/100)%10)=1 and datediff(day, getdate(), dateadd(day, duration, start_date)) < 7
			if @@rowcount = 0
			begin
				set @ret_val = 8
				return
			end
		end
	end
end
if (@@error <> 0) goto do_return

set @ret_val = 1
return


do_return:
	set @ret_val = -1
	return
GO

/*
@ret_val description by  JuYoung 2003-08-05
	              -1 : db error
		1 : ?? ??? ?? ??
		2 : ???, ARS ??(method error)
		3 : ?? ??? ????
		telnum format : 000 - 0000 - 0000
*/
ALTER PROCEDURE [dbo].[dp_CheckTelnumAbility] 
	@method integer,
	@telnum varchar(30),
	@ret_val integer output
AS
set nocount on
declare @buffer varchar(25), @check_method integer

set @check_method=@method/1000
if @check_method <> 5 and @check_method <> 7 and @check_method <> 8
begin
	set @ret_val = 2
	return
end
if (@@error <> 0) goto do_return

select @buffer=telnum from ban_pay with (nolock) where telnum=@telnum and flag=2 and status=1
if @@rowcount > 0
begin
	set @ret_val = 3
	return
end
if (@@error <> 0) goto do_return

set @ret_val = 1
return

do_return:
	set @ret_val = -1
	return
GO

ALTER PROCEDURE [dbo].[dp_GetAccountInfo] 
	@account VARCHAR(14)
AS

SELECT
		c.name,
		c.ssn,
		b.uid, 
		c.phone, 
		c.mobile, 
		c.job,
		c.newsletter, 
		CONVERT(CHAR(16), a.create_date, 20) As create_date,
		a.kind, 
		b.login_flag, 
		b.pay_stat,
		b.block_flag, 
		b.block_flag2, 
		b.warn_flag, 
		CONVERT(CHAR(16), b.last_login, 20) As last_login,
		CONVERT(CHAR(16), b.last_logout, 20) As last_logout,
		(c.addr_main+' ' +c.addr_etc) As address,
		c.zip, 
		c.email,  
		a.status_flag, 
		c.account_num
FROM
		user_info a (nolock) 
		INNER JOIN user_account b (nolock) ON a.account = b.account
		INNER JOIN ssn c (nolock) ON a.ssn = c.ssn
WHERE
	a.account = @account
GO

ALTER PROCEDURE [dbo].[dp_GetUserPay] 
	@account VARCHAR(14)
AS

SELECT
	method, start_date
FROM
	user_pay (nolock)
WHERE
	account = @account
GO

/*
	Descriptions : ???2  ??????

	Retrun Code
		@retcode = 1  '??
		@retcode = -1 'DB Error
		@retcode = 2  '?? ???? ??
		
	Created by Woo Se Won, 2003-07-15
	
	- pay_stat = 1002 ? ??
*/	

ALTER PROCEDURE [dbo].[dp_outsideraccount] 
	@account varchar(14),
	@passwd  varchar(255),
	@answer varchar(255),
	@kind	int,
	@retcode smallint output
AS

SET  NOCOUNT ON

DECLARE @sqlstr varchar(1024),@quiz varchar(255),@ssn char(13),@cflag int
SET @quiz = '?? ?? ???? ????'
SET @ssn = '7209112148611' -- ???
SET @cflag = 8 -- ???? ??

SELECT account FROM [lin2db].[dbo].[user_info] with(nolock) WHERE account = @account
IF @@ROWCOUNT <> 0 
BEGIN
	SET @retcode = 2
	RETURN 
END
 

SET @sqlstr = 'INSERT INTO [lin2db].[dbo].[user_auth] (account, password, quiz1, quiz2, answer1, answer2) values (''' +  @account + ''',  ' + @passwd + ',  ''' +  @quiz + ''', ''' +  @quiz + ''',' + @answer + ', ' + @answer + ')'
	
BEGIN TRAN
	UPDATE [lin2db].[dbo].[ssn]  SET status_flag = status_flag | @cflag WHERE ssn = @ssn

	EXEC (@sqlstr)
	IF @@ERROR <> 0 GOTO DO_ROLLBACK
	
	INSERT INTO [lin2db].[dbo].[user_info] (account, ssn, kind) VALUES
	(@account, @ssn, @kind)
	IF @@ERROR <> 0 GOTO DO_ROLLBACK
	
	INSERT INTO [lin2db].[dbo].[user_account] (account, pay_stat) VALUES
	(@account, 1002)
	IF @@ERROR <> 0 GOTO DO_ROLLBACK

COMMIT TRAN
SET @retcode = 1
RETURN


DO_ROLLBACK:
ROLLBACK TRAN
SET @retcode = -1
RETURN
GO

/*
	@ret_val description by  JuYoung 2003-07-23
			-5:no account
			-4:method error
			-3:np_getNewPayID fail
			-1:db error
			 1: success
			 2:duplicate pay_id(already payment)
			 3:duplicate id(already payment)
			 4:expire reserve
			 5 : amount error
			 6 : no real name(???? ????)
*/
ALTER PROCEDURE [dbo].[dp_SetUserDuration] 
	@account varchar(14),
	@method integer,
	@item_id integer,
	@ret_val integer output
AS
set nocount on
declare @buffer varchar(25), @pay_id varchar(16), @duration integer, @amount char(10), @uid integer, @pay_stat integer, @block_flag integer, @block_flag2 integer, @warn_flag integer

select @buffer=pay_id from user_pay_reserve with (nolock) where account=@account
if @@rowcount <> 0
begin
	set @ret_val = 4
	return 
end

select @duration=duration, @amount=amount from pay_method_code where method=@method and  (method/100)%10=1
if @@rowcount = 0
begin
	set @ret_val = -4
	return 0
end

exec dbo.dp_getnewpayid @pay_id output
if len(@pay_id) <> 16 
begin
	set @ret_val = -3
	return 
end

select @uid=uid, @pay_stat=isnull(pay_stat, 0), @block_flag=isnull(block_flag, 0), @block_flag2=isnull(block_flag2,0), @warn_flag=(warn_flag&2) from user_account with (nolock) where account=@account
if @@rowcount = 0
begin
	set @ret_val = -5
	return
end
else if @warn_flag <> 0
begin
	set @ret_val = 6
	return
end
else
begin
	if @pay_stat = 0 and @block_flag=0 and @block_flag=0
		goto current_pay
	else
	begin	
		goto reserve_pay
	end
end

current_pay:
begin tran
	insert into user_pay (pay_id, account, start_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, play_start, play_end, real_end_date, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, getdate(),  dateadd(day, @duration-1, getdate()),  dateadd(day, @duration-1, getdate()), @method, @item_id, @amount, getdate(), 1)
	if (@@error <> 0) goto do_rollback
	update user_account set pay_stat=@method where account=@account
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return 

reserve_pay:
begin tran
	insert into user_pay_reserve (pay_id, account, pay_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, @method, @item_id, @amount, getdate(), 2)
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return		

do_rollback:
	rollback tran
	set @ret_val = -1
	return
GO

/*
	@ret_val descripted by YounHeaYean
	-1:db error  
	-3:pay_id error 
	-4:method error
	-5:no account
	1: ?? 
 	2: already pay_id 
	3:already id 
	4:expire reserve, 
	5 : amount error
	6 : ?? ?? ?? ??
	* ?? : 3? ?? ??? ??, ?? ?? ???.

*/

ALTER procedure [dbo].[dp_SetUserDurationByArsCypass]
	@pay_id char(16),
	@account varchar(14),
	@method integer,
	@item_id integer,
	@amount integer,
	@tel_num varchar(20),
	@ctid varchar(40),
	@key_id varchar(4),
	@status tinyint,
	@ret_val integer output
	
as
set nocount on

declare @buffer varchar(50)
declare @check_amount integer
declare @check_method integer
declare @duration integer
declare @block_flag integer
declare @block_flag2 integer
declare @warn_flag integer
declare @pay_stat integer


select @buffer=pay_id from pay_master with (nolock) where pay_id = @pay_id
if @@rowcount  > 0
	begin
		set @ret_val = 2
		return
	end

select @buffer=ctid from dbo.ars_cypass_success with (nolock) where ctid = @ctid
if @@rowcount > 0
	begin
		set @ret_val = 3
		return
	end 

select @buffer=account from user_pay_reserve with (nolock) where account = @account
if @@rowcount > 0
	begin
		set @ret_val = 4
		return
	end

select @check_amount=amount, @check_method=(method/100)%10, @duration=duration from pay_method_code with (nolock) where method=@method
if @check_amount <> @amount
	begin
		set @ret_val = 5
		return
	end
else if @check_method <> 1
	begin
		set @ret_val = -4
	end


select @block_flag=isnull(block_flag,0), @block_flag2=isnull(block_flag2,0), @warn_flag=warn_flag&2, @pay_stat=pay_stat from user_account with (nolock) where account=@account
if @@rowcount = 0 
	begin
		set @ret_val = -5
		return
	end
else if @warn_flag <> 0
	begin
		set @ret_val = 6
		return
	end
else
	begin
		if @pay_stat=0 and @block_flag=0 and @block_flag2=0
			goto current_pay
		else
			goto reserve_pay
	end



current_pay:
	begin tran
		insert into user_pay(pay_id,account,start_date,method,duration) values(@pay_id, @account, getdate(), @method, @duration)
		if @@error <> 0 goto do_rollback

		insert into pay_master(pay_id,account,play_start,play_end,real_end_date,method,item_id,real_amount,pay_date,loc) 
			values(@pay_id,@account,getdate(),dateadd(day,@duration-1,getdate()),dateadd(day,@duration-1,getdate()),@method,@item_id,@amount,getdate(),1)
		if @@error <> 0 goto do_rollback
		
		update user_account set pay_stat=@method where account=@account
		if @@error <> 0 goto do_rollback

		insert into dbo.ars_cypass_success(pay_id, account, method, item_id, amount, op_date, pay_kind, status, tel_num, ctid, key_id)
			values(@pay_id, @account, @method, @item_id, @amount,  getdate(),  1, @status, @tel_num,@ctid, @key_id)
		if @@error <> 0 goto do_rollback	

	commit tran
	
	set @ret_val = 1
	return

reserve_pay:
	begin tran
		insert into user_pay_reserve(pay_id,account,pay_date,method,duration) values(@pay_id,@account,getdate(),@method,@duration)
		if @@error <> 0 goto do_rollback
		
		insert into pay_master(pay_id,account,method,item_id,real_amount,pay_date,loc) values(@pay_id,@account,@method,@item_id,@amount,getdate(),2)
		if @@error <> 0 goto do_rollback
		
		insert into dbo.ars_cypass_success(pay_id, account, method, item_id, amount, op_date, pay_kind, status, tel_num, ctid, key_id)
		values(@pay_id, @account, @method, @item_id, @amount,  getdate(),  1, @status, @tel_num,@ctid, @key_id)
		if @@error <> 0 goto do_rollback	

	commit tran
	
	set @ret_val = 1
	return

do_rollback:
	rollback tran
	set @ret_val = -1
GO

/*
	@ret_val descripted by YounHeaYean
	-1:db error  
	-3:pay_id error 
	-4:method error
	-5:no account
	1: ?? 
 	2: already pay_id 
	3:already id 
	4:expire reserve, 
	5 : amount error
	6 : ?? ?? ?? ??
	* ?? : 3? ?? ??? ??, ?? ?? ???.

*/

ALTER procedure [dbo].[dp_SetUserDurationByArsInfo]
	@pay_id char(16),
	@account varchar(14),
	@method smallint,
	@item_id smallint,
	@amount integer,
	@tel_num varchar(20),
	@key_id varchar(6),
	@status tinyint,
	@ret_val integer output	
as
set nocount on

declare @buffer varchar(50)
declare @check_amount integer
declare @check_method integer
declare @duration integer
declare @block_flag integer
declare @block_flag2 integer
declare @warn_flag integer
declare @pay_stat integer


select @buffer=pay_id from pay_master with (nolock) where pay_id = @pay_id
if @@rowcount  > 0
	begin
		set @ret_val = 2
		return
	end

select @buffer=pay_id from dbo.ars_info_success with (nolock) where pay_id = @pay_id
if @@rowcount > 0
	begin
		set @ret_val = 3
		return
	end

select @buffer=account from user_pay_reserve with (nolock) where account = @account
if @@rowcount > 0
	begin
		set @ret_val = 4
		return
	end

select @check_amount=amount, @check_method=(method/100)%10, @duration=duration from pay_method_code with (nolock) where method=@method
if @check_amount <> @amount
	begin
		set @ret_val = 5
		return
	end
else if @check_method <> 1
	begin
		set @ret_val = -4
	end


select @block_flag=isnull(block_flag,0), @block_flag2=isnull(block_flag2,0), @warn_flag=warn_flag&2, @pay_stat=pay_stat from user_account with (nolock) where account=@account
if @@rowcount = 0 
	begin
		set @ret_val = -5
		return
	end
else if @warn_flag <> 0
	begin
		set @ret_val = 6
		return
	end
else
	begin
		if @pay_stat=0 and @block_flag=0 and @block_flag2=0
			goto current_pay
		else
			goto reserve_pay
	end



current_pay:
	begin tran
		insert into user_pay(pay_id,account,start_date,method,duration) values(@pay_id, @account, getdate(), @method, @duration)
		if @@error <> 0 goto do_rollback

		insert into pay_master(pay_id,account,play_start,play_end,real_end_date,method,item_id,real_amount,pay_date,loc) 
			values(@pay_id,@account,getdate(),dateadd(day,@duration-1,getdate()),dateadd(day,@duration-1,getdate()),@method,@item_id,@amount,getdate(),1)
		if @@error <> 0 goto do_rollback
		
		update user_account set pay_stat=@method where account=@account
		if @@error <> 0 goto do_rollback

		insert into dbo.ars_info_success(pay_id, account, method, item_id, amount, op_date, pay_kind, status, tel_num, key_id)
			values(@pay_id, @account, @method,  @item_id, @amount, getdate(), 1, @status, @tel_num, @key_id)
		if @@error <> 0 goto do_rollback	

	commit tran
	
	set @ret_val = 1
	return

reserve_pay:
	begin tran
		insert into user_pay_reserve(pay_id,account,pay_date,method,duration) values(@pay_id,@account,getdate(),@method,@duration)
		if @@error <> 0 goto do_rollback
		
		insert into pay_master(pay_id,account,method,item_id,real_amount,pay_date,loc) values(@pay_id,@account,@method,@item_id,@amount,getdate(),2)
		if @@error <> 0 goto do_rollback

		insert into dbo.ars_info_success(pay_id, account, method, item_id, amount, op_date, pay_kind, status, tel_num, key_id)
			values(@pay_id, @account, @method,  @item_id, @amount, getdate(), 1, @status, @tel_num, @key_id)
		if @@error <> 0 goto do_rollback	

	commit tran
	
	set @ret_val = 1
	return

do_rollback:
	rollback tran
	set @ret_val = -1
GO

/*
	@ret_val descripted by JuYoung 2003-08-07
	-1:db error  
	-3:pay_id error 
	-4:method error
	-5:no account
	1: ?? 
 	2: already pay_id 
	3:already id 
	4:expire reserve, 
	5 : amount error
	6 : ?? ?? ?? ??
	* ?? : 3? ?? ??? ??, ?? ?? ???.

*/
ALTER PROCEDURE [dbo].[dp_SetUserDurationByCard] 
	@account varchar(14),
	@pay_id char(16),
	@tid varchar(40),
	@tel_num varchar(20),
	@email varchar(50),
	@amount integer,
	@method integer,
	@item_id integer,
	@auth_code varchar(8),
	@auth_time varchar(14),
	@card_code varchar(3),
	@card_name varchar(20),
	@result_code varchar(4),
	@ret_val integer output
AS
set nocount on
declare @buffer varchar(50), @check_amount integer, @check_method integer, @duration integer, @block_flag integer, @block_flag2 integer, @warn_flag integer, @pay_stat integer

select @buffer=pay_id from pay_master with (nolock) where pay_id=@pay_id
if @@rowcount > 0
begin
	set @ret_val = 2
	return
end

select @buffer=tid from dbo.card_success with (nolock) where tid=@tid
if @@rowcount > 0 
begin
	set @ret_val = 3
	return
end

select @buffer=account from user_pay_reserve with (nolock) where account=@account
if @@rowcount > 0
begin
	set @ret_val = 4
	return
end

select @check_amount=amount, @check_method=(method/100)%10, @duration=duration from pay_method_code with (nolock) where method=@method
if @check_amount <> @amount
begin
	set @ret_val = 5
	return
end
else if @check_method <> 1
begin
	set @ret_val = -4
	return
end

select @block_flag=isnull(block_flag,0), @block_flag2=isnull(block_flag2,0), @warn_flag=warn_flag&2, @pay_stat=pay_stat from user_account with (nolock) where account=@account
if @@rowcount = 0
begin
	set @ret_val = -5
	return
end
else if @warn_flag <> 0
begin
	set @ret_val = 6
	return
end
else
begin
	if @pay_stat=0 and @block_flag=0 and @block_flag2=0
		goto current_pay
	else
		goto reserve_pay
end

current_pay:
begin tran
	insert into user_pay (pay_id, account, start_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, play_start, play_end, real_end_date, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, getdate(),  dateadd(day, @duration-1, getdate()),  dateadd(day, @duration-1, getdate()), @method, @item_id, @amount, getdate(), 1)
	if (@@error <> 0) goto do_rollback
	update user_account set pay_stat=@method where account=@account
	if (@@error <> 0) goto do_rollback
	insert into dbo.card_success (pay_id, tid, account, email, tel_num, amount, item_id, auth_code, auth_time, card_code, card_name, result_code, result_msg, additional_info,op_date, status_flag, pay_kind) 
	values (@pay_id, @tid, @account, @email, @tel_num, @amount, @item_id, @auth_code, @auth_time, @card_code, @card_name, @result_code, '??', convert(char(12), getdate(), 112), getdate(), 'D1', @check_method)
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return 

reserve_pay:
begin tran
	insert into user_pay_reserve (pay_id, account, pay_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, @method, @item_id, @amount, getdate(), 2)
	if (@@error <> 0) goto do_rollback
	insert into dbo.card_success (pay_id, tid, account, email, tel_num, amount, item_id, auth_code, auth_time, card_code, card_name, result_code, result_msg, additional_info,op_date, status_flag, pay_kind) 
	values (@pay_id, @tid, @account, @email, @tel_num, @amount, @item_id, @auth_code, @auth_time, @card_code, @card_name, @result_code, '??', convert(char(12), getdate(), 112), getdate(), 'D1', @check_method)
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return		

do_rollback:
	rollback tran
	set @ret_val = -1
GO

/*
	@ret_val descripted by JuYoung
	-1:db error  
	-3:pay_id error 
	-4:method error
	-5:no account
	1: ?? 
 	2: already pay_id 
	3:already id 
	4:expire reserve, 
	5 : amount error
	6 : ?? ?? ?? ??
	* ?? : 3? ?? ??? ??, ?? ?? ???.
*/
ALTER PROCEDURE [dbo].[dp_SetUserDurationByDeposit] 
	@pay_id char(16),
	@account varchar(14),
	@amount integer,
	@ret_val integer output
AS
set nocount on
declare @buffer varchar(50), @item_id integer, @method integer, @tel_num varchar(20), @duration integer, @block_flag integer, @block_flag2 integer, @warn_flag integer, @pay_stat integer

select @buffer=pay_id from pay_master with (nolock) where pay_id = @pay_id
if @@rowcount > 0
begin
	set @ret_val = 2
	return
end

select @buffer=account from user_pay_reserve with (nolock) where account = @account
if @@rowcount > 0
begin
	set @ret_val = 4
	return
end

select @buffer=account from ban_pay with (nolock) where account=@account and flag=1 and status=1
if @@rowcount > 0
begin
	set @ret_val = 6
	return
end

select  @method=method, @item_id=item_id, @tel_num=tel_num  from dbo.bank_deposit with (nolock) where pay_id = @pay_id and status=0
if @@rowcount = 0
begin
	set @ret_val = 3
	return
end


select @duration=duration  from pay_method_code with (nolock) where method=@method
if @@rowcount = 0
begin
	set @ret_val = -4
	return
end

select @block_flag=isnull(block_flag,0), @block_flag2=isnull(block_flag2,0), @warn_flag=warn_flag&2, @pay_stat=pay_stat from user_account with (nolock) where account=@account
if @@rowcount = 0 
begin
	set @ret_val = -5
	return
end
else if @warn_flag <> 0
begin
	set @ret_val = 6
	return
end
else
	begin
		if @pay_stat=0 and @block_flag=0 and @block_flag2=0
			goto current_pay
		else
			goto reserve_pay
	end

current_pay:
begin tran
	insert into user_pay (pay_id, account, start_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, play_start, play_end, real_end_date, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, getdate(),  dateadd(day, @duration-1, getdate()),  dateadd(day, @duration-1, getdate()), @method, @item_id, @amount, getdate(), 1)
	if (@@error <> 0) goto do_rollback
	update user_account set pay_stat=@method where account=@account
	if (@@error <> 0) goto do_rollback
	update dbo.bank_deposit set op_date=getdate(), status=1 where pay_id=@pay_id
	if (@@error <> 0) goto do_rollback
	update dbo.bank_book set flag=1, state=4, tel_num=@tel_num where pay_id=@pay_id
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return
 
reserve_pay:
begin tran
	insert into user_pay_reserve (pay_id, account, pay_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, @method, @item_id, @amount, getdate(), 2)
	if (@@error <> 0) goto do_rollback
	update dbo.bank_deposit set op_date=getdate(), status=1 where pay_id=@pay_id
	if (@@error <> 0) goto do_rollback
	update dbo.bank_book set flag=1, state=4, tel_num=@tel_num where pay_id=@pay_id
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return

do_rollback:
	rollback tran
	set @ret_val = -1
	return
GO

/*
	@ret_val descripted by myhong 2003-08-08
	-1:db error  
	-3:pay_id error 
	-4:method error
	-5:no account
	1: ?? 
 	2: already pay_id 
	3:already id 
	4:expire reserve, 
	5 : amount error
	6 : ?? ?? ?? ??
	* ?? : 3? ?? ??? ??, ?? ?? ???.

*/
ALTER PROCEDURE [dbo].[dp_SetUserDurationByHp]
	@pay_id char(16),
	@account varchar(14),
	@method integer,
	@item_id integer,
	@tel_num varchar(20),
	@tid varchar(20),
	@status tinyint, 
	@ret_val integer output
AS
set nocount on
declare @buffer varchar(50), @check_amount integer, @check_method integer, @duration integer, @block_flag integer, @block_flag2 integer, @warn_flag integer, @pay_stat integer

select @buffer=pay_id from pay_master with (nolock) where pay_id=@pay_id
if @@rowcount > 0
begin
	set @ret_val = 2
	return
end

select @buffer=tid from dbo.hp_success with (nolock) where tid=@tid
if @@rowcount > 0 
begin
	set @ret_val = 3
	return
end

select @buffer=account from user_pay_reserve with (nolock) where account=@account
if @@rowcount > 0
begin
	set @ret_val = 4
	return
end

select @check_amount=amount, @check_method=(method/100)%10, @duration=duration from pay_method_code with (nolock) where method=@method
/*
???? ?? amount? input parameter? ??
if @check_amount <> @amount
begin
	set @ret_val = 5
	return
end
*/
if @check_method <> 1
begin
	set @ret_val = -4
	return
end

select @block_flag=isnull(block_flag,0), @block_flag2=isnull(block_flag2,0), @warn_flag=warn_flag&2, @pay_stat=pay_stat from user_account with (nolock) where account=@account
if @@rowcount = 0
begin
	set @ret_val = -5
	return
end
else if @warn_flag <> 0
begin
	set @ret_val = 6
	return
end
else
begin
	if @pay_stat=0 and @block_flag=0 and @block_flag2=0
		goto current_pay
	else
		goto reserve_pay
end

current_pay:
begin tran
	insert into user_pay (pay_id, account, start_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, play_start, play_end, real_end_date, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, getdate(),  dateadd(day, @duration-1, getdate()),  dateadd(day, @duration-1, getdate()), @method, @item_id, @check_amount, getdate(), 1)
	if (@@error <> 0) goto do_rollback
	update user_account set pay_stat=@method where account=@account
	if (@@error <> 0) goto do_rollback
	insert into dbo.hp_success (pay_id, account, method, item_id, amount, op_date, tel_num, pay_kind, status, tid)
	values (@pay_id, @account, @method, @item_id, @check_amount,  getdate(), @tel_num, 1, @status, @tid)
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return 

reserve_pay:
begin tran
	insert into user_pay_reserve (pay_id, account, pay_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, @method, @item_id, @check_amount, getdate(), 2)
	if (@@error <> 0) goto do_rollback
	insert into dbo.hp_success (pay_id, account, method, item_id, amount, op_date, tel_num, pay_kind, status, tid)
	values (@pay_id, @account, @method, @item_id, @check_amount,  getdate(), @tel_num, 1, @status, @tid)
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return		

do_rollback:
	rollback tran
	set @ret_val = -1
GO

/*
	@ret_val descripted by myhong 2003-08-26
	-1:db error  
	-3:pay_id error 
	-4:method error
	-5:no account
	1: ?? 
 	2: already pay_id 
	3:already id 
	4:expire reserve, 
	5 : amount error
	6 : ?? ?? ?? ??
	* ?? : 3? ?? ??? ??, ?? ?? ???.

*/
ALTER PROCEDURE [dbo].[dp_SetUserDurationByHpBack]
	@account varchar(14),
	@method integer,
	@item_id integer,
	@tel_num varchar(20),
	@tid varchar(20),
	@status tinyint, 
	@ret_val integer output
AS
set nocount on
declare @buffer varchar(50), @check_amount integer, @check_method integer, @duration integer, @block_flag integer, @block_flag2 integer, @warn_flag integer, @pay_stat integer, @pay_id char(16)

exec dbo.dp_getnewpayid @pay_id output
if len(@pay_id) <> 16 
begin
	set @ret_val = -3
	return 
end

select @buffer=pay_id from pay_master with (nolock) where pay_id=@pay_id
if @@rowcount > 0
begin
	set @ret_val = 2
	return
end

select @buffer=tid from dbo.hp_success with (nolock) where tid=@tid
if @@rowcount > 0 
begin
	set @ret_val = 3
	return
end

select @buffer=account from user_pay_reserve with (nolock) where account=@account
if @@rowcount > 0
begin
	set @ret_val = 4
	return
end

select @check_amount=amount, @check_method=(method/100)%10, @duration=duration from pay_method_code with (nolock) where method=@method
/*
???? ?? amount? input parameter? ??
if @check_amount <> @amount
begin
	set @ret_val = 5
	return
end
*/
if @check_method <> 1
begin
	set @ret_val = -4
	return
end

select @block_flag=isnull(block_flag,0), @block_flag2=isnull(block_flag2,0), @warn_flag=warn_flag&2, @pay_stat=pay_stat from user_account with (nolock) where account=@account
if @@rowcount = 0
begin
	set @ret_val = -5
	return
end
else if @warn_flag <> 0
begin
	set @ret_val = 6
	return
end
else
begin
	if @pay_stat=0 and @block_flag=0 and @block_flag2=0
		goto current_pay
	else
		goto reserve_pay
end

current_pay:
begin tran
	insert into user_pay (pay_id, account, start_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, play_start, play_end, real_end_date, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, getdate(),  dateadd(day, @duration-1, getdate()),  dateadd(day, @duration-1, getdate()), @method, @item_id, @check_amount, getdate(), 1)
	if (@@error <> 0) goto do_rollback
	update user_account set pay_stat=@method where account=@account
	if (@@error <> 0) goto do_rollback
	insert into dbo.hp_success (pay_id, account, method, item_id, amount, op_date, tel_num, pay_kind, status, tid)
	values (@pay_id, @account, @method, @item_id, @check_amount,  getdate(), @tel_num, 1, @status, @tid)
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return 

reserve_pay:
begin tran
	insert into user_pay_reserve (pay_id, account, pay_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, @method, @item_id, @check_amount, getdate(), 2)
	if (@@error <> 0) goto do_rollback
	insert into dbo.hp_success (pay_id, account, method, item_id, amount, op_date, tel_num, pay_kind, status, tid)
	values (@pay_id, @account, @method, @item_id, @check_amount,  getdate(), @tel_num, 1, @status, @tid)
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return		

do_rollback:
	rollback tran
	set @ret_val = -1
GO

/*
	@ret_val descripted by Woo, Cloud 2003-08-22
	-1:db error  
	-4:method error
	-5:no account
	1: ?? 
 	2: already pay_id 
	3:already id 
	4:expire reserve, 
	5 : amount error
	6 : ?? ?? ?? ??
	* ?? : 3? ?? ??? ??, ?? ?? ???.

*/
ALTER PROCEDURE [dbo].[dp_SetUserDurationByiBanking] 
	@pay_id char(16),
	@account varchar(14),
	@method integer,
	@item_id smallint,
	@amount char(10),
	@bank_name varchar(30),
	@user_name varchar(30),
	@email varchar(50),
	@pay_date char(14),
	@status tinyint,
	@ret_val integer output
AS
set nocount on
declare @buffer varchar(50), @check_amount integer, @check_method integer, @duration integer, @block_flag integer, @block_flag2 integer, @warn_flag integer, @pay_stat integer, @pay_kind tinyint

select @buffer=pay_id from pay_master with (nolock) where pay_id=@pay_id
if @@rowcount > 0
begin
	set @ret_val = 2
	return
end

select @buffer=pay_id from dbo.iBanking_success with (nolock) where pay_id=@pay_id
if @@rowcount > 0 
begin
	set @ret_val = 3
	return
end

select @buffer=account from user_pay_reserve with (nolock) where account=@account
if @@rowcount > 0
begin
	set @ret_val = 4
	return
end

select @check_amount=amount, @check_method=(method/100)%10, @duration=duration from pay_method_code with (nolock) where method=@method
if @check_amount = @amount --test ? <>?? ????
begin
	set @ret_val = 5
	return
end
else if @check_method <> 1
begin
	set @ret_val = -4
	return
end

select @block_flag=isnull(block_flag,0), @block_flag2=isnull(block_flag2,0), @warn_flag=warn_flag&2, @pay_stat=pay_stat from user_account with (nolock) where account=@account
if @@rowcount = 0
begin
	set @ret_val = -5
	return
end
else if @warn_flag <> 0
begin
	set @ret_val = 6
	return
end
else
begin
	if @pay_stat=0 and @block_flag=0 and @block_flag2=0
		goto current_pay
	else
		goto reserve_pay
end

current_pay:
begin tran
	insert into user_pay (pay_id, account, start_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, play_start, play_end, real_end_date, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, getdate(),  dateadd(day, @duration-1, getdate()),  dateadd(day, @duration-1, getdate()), @method, @item_id, @amount, getdate(), 1)
	if (@@error <> 0) goto do_rollback
	update user_account set pay_stat=@method where account=@account
	if (@@error <> 0) goto do_rollback
	insert into dbo.ibanking_success (pay_id, account, method, item_id, amount, bank_name, user_name, email, pay_date,  op_date, pay_kind, status) 
	values (@pay_id, @account, @method, @item_id, @amount, @bank_name, @user_name, @email, @pay_date, getdate(), 1, @status)
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return 

reserve_pay:
begin tran
	insert into user_pay_reserve (pay_id, account, pay_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, @method, @item_id, @amount, getdate(), 2)
	if (@@error <> 0) goto do_rollback
	insert into dbo.ibanking_success (pay_id, account, method, item_id, amount, bank_name, user_name, email, pay_date,  op_date, pay_kind, status) 
	values (@pay_id, @account, @method, @item_id, @amount, @bank_name, @user_name, @email, @pay_date, getdate(), 1, @status)
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return		

do_rollback:
	rollback tran
	set @ret_val = -1
GO

/*
	Coupon ?? : 	???-999,
			??? payid

	@ret_val descripted by YounHeaYean

	-1 :db error  
	-3 :pay_id error 
	-4 :method error
	-5 :no account
	-6 : invalid coupon_no
	-7 : expire coupon
	pay_id: ?? 
 	2 : already pay_id 
	3 : already id 
	4 : expire reserve, 
	5 : amount error
	6 : ?? ?? ?? ??
	* ?? : 3? ?? ??? ??, ?? ?? ???.

*/

ALTER procedure [dbo].[dp_SetUserDurationOrTimeByCoupon]
	@account varchar(14),
	@coupon_no varchar(16),
	@ret_val varchar(16) output
as
set nocount on

declare @pay_id char(16)
declare @method char(4)
declare @item_id char(3)
declare @buffer varchar(50)
declare @check_amount integer
declare @check_method integer
declare @duration integer
declare @amount integer
declare @uid integer
declare @block_flag integer
declare @block_flag2 integer
declare @warn_flag integer
declare @pay_stat integer
declare @old_pay_stat integer

exec dbo.dp_getnewpayid @pay_id output

if len(@pay_id) <> 16
begin
	set @ret_val = -3 
	return
end

select coupon_key from dbo.coupon_success where coupon_key = @coupon_no and reg_start <= getdate() and reg_end >= getdate() and account is null
if @@rowcount  < 1
	begin
		set @ret_val = -7
		return
	end

select @method = method from dbo.coupon_success where coupon_key = @coupon_no
if @@rowcount  < 1
	begin
		set @ret_val = -6
		return
	end

select @buffer=pay_id from pay_master with (nolock) where pay_id = @pay_id
if @@rowcount  > 0
	begin
		set @ret_val = 2
		return
	end

select @buffer=account from user_pay_reserve with (nolock) where account = @account
if @@rowcount > 0
	begin
		set @ret_val = 4
		return
	end

select @amount=amount, @check_method=(method/100)%10, @duration=duration, @item_id=right(method,3) from pay_method_code with (nolock) where method=@method
if @@rowcount < 1
	begin
		set @ret_val = -4
		return
	end


select @uid=uid, @block_flag=isnull(block_flag,0), @block_flag2=isnull(block_flag2,0), @warn_flag=warn_flag&2, @pay_stat=pay_stat from user_account with (nolock) where account=@account
if @@rowcount = 0 
	begin
		set @ret_val = -5
		return
	end
else if @warn_flag <> 0
	begin
		set @ret_val = 6
		return
	end
else
	begin
		if @pay_stat=0 and @block_flag=0 and @block_flag2=0
			if @check_method = 1
				goto current_amount_pay
			else if @check_method = 2
				goto current_time_pay
			else
				begin
					set @ret_val = -4
					return
				end
		else
			set @old_pay_stat = (@pay_stat/100)%10
			if @check_method = 1
				goto reserve_amount_pay
			else if @check_method = 2
				goto reserve_time_pay
			else
				begin
					set @ret_val = -4
					return
				end
	end


current_amount_pay:
	begin tran		
		insert into user_pay(pay_id,account,start_date,method,duration) values(@pay_id, @account, getdate(), @method, @duration)
		if @@error <> 0 goto do_rollback

		insert into pay_master(pay_id,account,play_start,play_end,real_end_date,method,item_id,real_amount,pay_date,loc) 
			values(@pay_id,@account,getdate(),dateadd(day,@duration-1,getdate()),dateadd(day,@duration-1,getdate()),@method,@item_id,@amount,getdate(),1)
		if @@error <> 0 goto do_rollback
		
		update user_account set pay_stat=@method where account=@account
		if @@error <> 0 goto do_rollback

		update dbo.coupon_success set account=@account, reg_date = getdate(), pay_id = @pay_id
			where coupon_key = @coupon_no
		if @@error <> 0 goto do_rollback	

	commit tran
	
	set @ret_val = @pay_id
	return


reserve_amount_pay:
	begin tran
		insert into user_pay_reserve(pay_id,account,pay_date,method,duration) values(@pay_id,@account,getdate(),@method,@duration)
		if @@error <> 0 goto do_rollback
		
		insert into pay_master(pay_id,account,method,item_id,real_amount,pay_date,loc) values(@pay_id,@account,@method,@item_id,@amount,getdate(),2)
		if @@error <> 0 goto do_rollback
		
		update dbo.coupon_success set account=@account, reg_date = getdate(), pay_id = @pay_id
			where coupon_key = @coupon_no
		if @@error <> 0 goto do_rollback	

	commit tran
	
	set @ret_val = @pay_id
	return


current_time_pay:
	begin tran			
		delete user_time where account =@account and flag=1
		if (@@error <> 0) goto do_rollback

		insert into user_pay(pay_id,account,start_date,method,duration) values(@pay_id, @account, getdate(), @method, @duration)
		if @@error <> 0 goto do_rollback

		insert into user_time (uid, account, present_time, total_time, op_date, flag) values (@uid, @account, @duration, @duration, getdate(), 1)
		if (@@error <> 0) goto do_rollback
	
		insert into pay_master(pay_id,account,play_start,play_end,real_end_date,method,item_id,real_amount,pay_date,loc) 
			values(@pay_id,@account,getdate(),dateadd(day,@duration-1,getdate()),dateadd(day,@duration-1,getdate()),@method,@item_id,@amount,getdate(),1)
		if @@error <> 0 goto do_rollback
		
		update user_account set pay_stat=@method where account=@account
		if @@error <> 0 goto do_rollback

		update dbo.coupon_success set account=@account, reg_date = getdate(), pay_id = @pay_id
			where coupon_key = @coupon_no
		if @@error <> 0 goto do_rollback	

	commit tran
	
	set @ret_val = @pay_id
	return


reserve_time_pay:
	begin tran
		insert into user_pay_reserve(pay_id,account,pay_date,method,duration) values(@pay_id,@account,getdate(),@method,@duration)
		if @@error <> 0 goto do_rollback
		
		insert into pay_master(pay_id,account,method,item_id,real_amount,pay_date,loc) values(@pay_id,@account,@method,@item_id,@amount,getdate(),2)
		if @@error <> 0 goto do_rollback

		if @old_pay_stat <> 2
			begin
				delete user_time where account=@account and flag=1
				if (@@error <> 0) goto do_rollback
				
				insert into user_time (uid, account, present_time, total_time, op_date, flag) values (@uid, @account, @duration, @duration, getdate(), 1)
				if (@@error <> 0) goto do_rollback
			end
		else
			begin
				update user_time set total_time=total_time+@duration where account=@account and flag=1
				if (@@error <> 0) goto do_rollback
			end

		update dbo.coupon_success set account=@account, reg_date = getdate(), pay_id = @pay_id
			where coupon_key = @coupon_no
		if @@error <> 0 goto do_rollback	

	commit tran
	
	set @ret_val = @pay_id
	return


do_rollback:
	rollback tran
	set @ret_val = -1
GO

/*
	@ret_val description by  JuYoung 2003-07-23
			-5:no account
			-4:method error
			-3:np_getNewPayID fail
			-1:db error
			 1: success
			 2:already pay_id
			 3:already id
			 4:expire reserve
			 5 : amount error
			 6 : no real name(???? ????)
*/
ALTER PROCEDURE [dbo].[dp_SetUserTime] 
	@account varchar(14),
	@method integer,
	@item_id integer,
	@ret_val integer output
AS
set nocount on
declare @buffer varchar(25), @pay_id varchar(16), @duration integer, @amount char(10), @uid integer, @pay_stat integer, @old_pay_stat integer, @block_flag integer, @block_flag2 integer, @warn_flag integer

select @buffer=pay_id from user_pay_reserve with (nolock) where account=@account
if @@rowcount <> 0
begin
	set @ret_val = 4
	return 
end

select @duration=duration, @amount=amount from pay_method_code where method=@method and  (method/100)%10=2
if @@rowcount = 0
begin
	set @ret_val = -4
	return 0
end

exec dbo.dp_getnewpayid @pay_id output
if len(@pay_id) <> 16 
begin
	set @ret_val = -3
	return 
end

select @uid=uid, @pay_stat=isnull(pay_stat, 0), @block_flag=isnull(block_flag, 0), @block_flag2=isnull(block_flag2,0), @warn_flag=(warn_flag&2) from user_account with (nolock) where account=@account
if @@rowcount = 0
begin
	set @ret_val = -5
	return
end
else if @warn_flag <> 0
begin
	set @ret_val = 6
	return
end
else
begin
	if @pay_stat = 0 and @block_flag=0 and @block_flag=0
		goto current_pay
	else
	begin	
		set @old_pay_stat = (@pay_stat/100)%10
		goto reserve_pay
	end
end

current_pay:
begin tran
	delete user_time where account=@account and flag=1
	if (@@error <> 0) goto do_rollback
	insert into user_pay (pay_id, account, start_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into user_time (uid, account, present_time, total_time, op_date, flag) values (@uid, @account, @duration, @duration, getdate(), 1)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, play_start, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, getdate(), @method, @item_id, @amount, getdate(), 1)
	if (@@error <> 0) goto do_rollback
	update user_account set pay_stat=@method where account=@account
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return 

reserve_pay:
begin tran
	insert into user_pay_reserve (pay_id, account, pay_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, @method, @item_id, @amount, getdate(), 2)
	if (@@error <> 0) goto do_rollback
	if @old_pay_stat <> 2
	begin
		delete user_time where account=@account and flag=1
		if (@@error <> 0) goto do_rollback
		insert into user_time (uid, account, present_time, total_time, op_date, flag) values (@uid, @account, @duration, @duration, getdate(), 1)
		if (@@error <> 0) goto do_rollback
	end
	else
	begin
		update user_time set total_time=total_time+@duration where account=@account and flag=1
		if (@@error <> 0) goto do_rollback
	end
	commit tran
	set @ret_val = 1
	return		

do_rollback:
	rollback tran
	set @ret_val = -1
	return
GO

/*
	@ret_val descripted by YounHeaYean
	-1:db error  
	-3:pay_id error 
	-4:method error
	-5:no account
	1: ?? 
 	2: already pay_id 
	3:already id 
	4:expire reserve, 
	5 : amount error
	6 : ?? ?? ?? ??
	* ?? : 3? ?? ??? ??, ?? ?? ???.

*/
ALTER procedure [dbo].[dp_SetUserTimeByArsCypass]
	@pay_id char(16),
	@account varchar(14),
	@method integer,
	@item_id smallint,
	@amount integer,
	@tel_num varchar(20),
	@ctid varchar(40),
	@key_id varchar(4),
	@status tinyint,
	@ret_val integer output
as
set nocount on

declare @buffer varchar(50) 
declare @check_amount integer
declare @check_method integer
declare @duration integer
declare @uid integer
declare @block_flag integer 
declare @block_flag2 integer 
declare @warn_flag integer
declare @pay_stat integer
declare @old_pay_stat integer

select @buffer=pay_id from pay_master with (nolock) where pay_id=@pay_id
if @@rowcount > 0
begin
	set @ret_val = 2
	return
end

select @buffer=ctid from dbo.ars_cypass_success with (nolock) where ctid=@ctid
if @@rowcount > 0 
begin
	set @ret_val = 3
	return
end

select @buffer=account from user_pay_reserve with (nolock) where account=@account
if @@rowcount > 0
begin
	set @ret_val = 4
	return
end

select @check_amount=amount, @check_method=(method/100)%10, @duration=duration from pay_method_code with (nolock) where method=@method
if @check_amount <> @amount
begin
	set @ret_val = 5
	return
end
else if @check_method <> 2
begin
	set @ret_val = -4
	return
end

select @uid=uid, @block_flag=isnull(block_flag,0), @block_flag2=isnull(block_flag2,0), @warn_flag=warn_flag&2, @pay_stat=pay_stat from user_account with (nolock) where account=@account
if @@rowcount = 0
	begin
		set @ret_val = -5
		return
	end
else if @warn_flag <> 0
	begin
		set @ret_val = 6
		return
	end
else
	begin
		if @pay_stat=0 and @block_flag=0 and @block_flag2=0
			goto current_pay
		else
		begin
			set @old_pay_stat = (@pay_stat/100)%10
			goto reserve_pay
		end
	end

current_pay:
begin tran
	delete user_time where account=@account and flag=1
	if (@@error <> 0) goto do_rollback

	insert into user_pay (pay_id, account, start_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback

	insert into user_time (uid, account, present_time, total_time, op_date, flag) values (@uid, @account, @duration, @duration, getdate(), 1)
	if (@@error <> 0) goto do_rollback

	insert into pay_master (pay_id, account, play_start, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, getdate(), @method, @item_id, @amount, getdate(), 1)
	if (@@error <> 0) goto do_rollback

	update user_account set pay_stat=@method where account=@account
	if (@@error <> 0) goto do_rollback

	insert into dbo.ars_cypass_success(pay_id, account, method, item_id, amount, op_date, pay_kind, status, tel_num, ctid, key_id)
	values(@pay_id, @account, @method, @item_id, @amount,  getdate(),  1, @status, @tel_num,@ctid, @key_id)
	if (@@error <> 0) goto do_rollback

	commit tran

	set @ret_val = 1
	return 

reserve_pay:
begin tran
	insert into user_pay_reserve (pay_id, account, pay_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback

	insert into pay_master (pay_id, account, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, @method, @item_id, @amount, getdate(), 2)
	if (@@error <> 0) goto do_rollback

	if @old_pay_stat <> 2
		begin
			delete user_time where account=@account and flag=1
			if (@@error <> 0) goto do_rollback
			
			insert into user_time (uid, account, present_time, total_time, op_date, flag) values (@uid, @account, @duration, @duration, getdate(), 1)
			if (@@error <> 0) goto do_rollback
		end

	else
		begin
			update user_time set total_time=total_time+@duration where account=@account and flag=1
			if (@@error <> 0) goto do_rollback
		end

	insert into dbo.ars_cypass_success(pay_id, account, method, item_id, amount, op_date, pay_kind, status, tel_num, ctid, key_id)
	values(@pay_id, @account, @method, @item_id, @amount,  getdate(),  1, @status, @tel_num,@ctid, @key_id)
	if (@@error <> 0) goto do_rollback

	commit tran

	set @ret_val = 1
	return		

do_rollback:
	rollback tran
	set @ret_val = -1
GO

/*
	@ret_val descripted by YounHeaYean
	-1:db error  
	-3:pay_id error 
	-4:method error
	-5:no account
	1: ?? 
 	2: already pay_id 
	3:already id 
	4:expire reserve, 
	5 : amount error
	6 : ?? ?? ?? ??
	* ?? : 3? ?? ??? ??, ?? ?? ???.

*/
ALTER procedure [dbo].[dp_SetUserTimeByArsInfo]
	@pay_id char(16),
	@account varchar(14),
	@method integer,
	@item_id smallint,
	@amount integer,
	@tel_num varchar(20),
	@key_id varchar(6),
	@status tinyint,
	@ret_val integer output
as
set nocount on

declare @buffer varchar(50) 
declare @check_amount integer
declare @check_method integer
declare @duration integer
declare @uid integer
declare @block_flag integer 
declare @block_flag2 integer 
declare @warn_flag integer
declare @pay_stat integer
declare @old_pay_stat integer

select @buffer=pay_id from pay_master with (nolock) where pay_id=@pay_id
if @@rowcount > 0
begin
	set @ret_val = 2
	return
end

select @buffer = pay_id from dbo.ars_info_success with (nolock) where pay_id = @pay_id
if @@rowcount > 0
begin
	set @ret_val = 3
	return
end

select @buffer=account from user_pay_reserve with (nolock) where account=@account
if @@rowcount > 0
begin
	set @ret_val = 4
	return
end

select @check_amount=amount, @check_method=(method/100)%10, @duration=duration from pay_method_code with (nolock) where method=@method
if @check_amount <> @amount
begin
	set @ret_val = 5
	return
end
else if @check_method <> 2
begin
	set @ret_val = -4
	return
end

select @uid=uid, @block_flag=isnull(block_flag,0), @block_flag2=isnull(block_flag2,0), @warn_flag=warn_flag&2, @pay_stat=pay_stat from user_account with (nolock) where account=@account
if @@rowcount = 0
	begin
		set @ret_val = -5
		return
	end
else if @warn_flag <> 0
	begin
		set @ret_val = 6
		return
	end
else
	begin
		if @pay_stat=0 and @block_flag=0 and @block_flag2=0
			goto current_pay
		else
		begin
			set @old_pay_stat = (@pay_stat/100)%10
			goto reserve_pay
		end
	end

current_pay:
begin tran
	delete user_time where account=@account and flag=1
	if (@@error <> 0) goto do_rollback

	insert into user_pay (pay_id, account, start_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback

	insert into user_time (uid, account, present_time, total_time, op_date, flag) values (@uid, @account, @duration, @duration, getdate(), 1)
	if (@@error <> 0) goto do_rollback

	insert into pay_master (pay_id, account, play_start, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, getdate(), @method, @item_id, @amount, getdate(), 1)
	if (@@error <> 0) goto do_rollback

	update user_account set pay_stat=@method where account=@account
	if (@@error <> 0) goto do_rollback

	insert into dbo.ars_info_success(pay_id, account, method, item_id, amount, op_date, pay_kind, status, tel_num, key_id)
	values(@pay_id, @account, @method, @item_id, @amount,  getdate(),  1, @status, @tel_num, @key_id)
	if (@@error <> 0) goto do_rollback

	commit tran

	set @ret_val = 1
	return 

reserve_pay:
begin tran
	insert into user_pay_reserve (pay_id, account, pay_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback

	insert into pay_master (pay_id, account, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, @method, @item_id, @amount, getdate(), 2)
	if (@@error <> 0) goto do_rollback

	if @old_pay_stat <> 2
		begin
			delete user_time where account=@account and flag=1
			if (@@error <> 0) goto do_rollback
			
			insert into user_time (uid, account, present_time, total_time, op_date, flag) values (@uid, @account, @duration, @duration, getdate(), 1)
			if (@@error <> 0) goto do_rollback
		end

	else
		begin
			update user_time set total_time=total_time+@duration where account=@account and flag=1
			if (@@error <> 0) goto do_rollback
		end

	insert into dbo.ars_info_success(pay_id, account, method, item_id, amount, op_date, pay_kind, status, tel_num, key_id)
		values(@pay_id, @account, @method, @item_id, @amount,  getdate(),  1, @status, @tel_num, @key_id)
	if (@@error <> 0) goto do_rollback

	commit tran

	set @ret_val = 1
	return		

do_rollback:
	rollback tran
	print @@error
	set @ret_val = -1
GO

/*
	@ret_val descripted by JuYoung 2003-08-07
	-1:db error  
	-3:pay_id error 
	-4:method error
	-5:no account
	1: ?? 
 	2: already pay_id 
	3:already id 
	4:expire reserve, 
	5 : amount error
	6 : ?? ?? ?? ??
	* ?? : 3? ?? ??? ??, ?? ?? ???.

*/
ALTER PROCEDURE [dbo].[dp_SetUserTimeByCard] 
	@account varchar(14),
	@pay_id char(16),
	@tid varchar(40),
	@tel_num varchar(20),
	@email varchar(50),
	@amount integer,
	@method integer,
	@item_id smallint,
	@auth_code varchar(8),
	@auth_time varchar(14),
	@card_code varchar(3),
	@card_name varchar(20),
	@result_code varchar(4),
	@ret_val integer output
AS
set nocount on
declare @buffer varchar(50), @check_amount integer, @check_method integer, @duration integer, @uid integer, @block_flag integer, @block_flag2 integer, @warn_flag integer, @pay_stat integer
declare @old_pay_stat integer
select @buffer=pay_id from pay_master with (nolock) where pay_id=@pay_id
if @@rowcount > 0
begin
	set @ret_val = 2
	return
end

select @buffer=tid from dbo.card_success with (nolock) where tid=@tid
if @@rowcount > 0 
begin
	set @ret_val = 3
	return
end

select @buffer=account from user_pay_reserve with (nolock) where account=@account
if @@rowcount > 0
begin
	set @ret_val = 4
	return
end

select @check_amount=amount, @check_method=(method/100)%10, @duration=duration from pay_method_code with (nolock) where method=@method
if @check_amount <> @amount
begin
	set @ret_val = 5
	return
end
else if @check_method <> 2
begin
	set @ret_val = -4
	return
end

select @uid=uid, @block_flag=isnull(block_flag,0), @block_flag2=isnull(block_flag2,0), @warn_flag=warn_flag&2, @pay_stat=pay_stat from user_account with (nolock) where account=@account
if @@rowcount = 0
begin
	set @ret_val = -5
	return
end
else if @warn_flag <> 0
begin
	set @ret_val = 6
	return
end
else
begin
	if @pay_stat=0 and @block_flag=0 and @block_flag2=0
		goto current_pay
	else
	begin
		set @old_pay_stat = (@pay_stat/100)%10
		goto reserve_pay
	end
end

current_pay:
begin tran
	delete user_time where account=@account and flag=1
	if (@@error <> 0) goto do_rollback
	insert into user_pay (pay_id, account, start_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into user_time (uid, account, present_time, total_time, op_date, flag) values (@uid, @account, @duration, @duration, getdate(), 1)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, play_start, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, getdate(), @method, @item_id, @amount, getdate(), 1)
	if (@@error <> 0) goto do_rollback
	update user_account set pay_stat=@method where account=@account
	if (@@error <> 0) goto do_rollback
	insert into dbo.card_success (pay_id, tid, account, email, tel_num, amount, item_id, auth_code, auth_time, card_code, card_name, result_code, result_msg, additional_info,op_date, status_flag, pay_kind) 
	values (@pay_id, @tid, @account, @email, @tel_num, @amount, @item_id, @auth_code, @auth_time, @card_code, @card_name, @result_code, '??', convert(char(12), getdate(), 112), getdate(), 'D1', @check_method)
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return 

reserve_pay:
begin tran
	insert into user_pay_reserve (pay_id, account, pay_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, @method, @item_id, @amount, getdate(), 2)
	if (@@error <> 0) goto do_rollback
	if @old_pay_stat <> 2
	begin
		delete user_time where account=@account and flag=1
		if (@@error <> 0) goto do_rollback
		insert into user_time (uid, account, present_time, total_time, op_date, flag) values (@uid, @account, @duration, @duration, getdate(), 1)
		if (@@error <> 0) goto do_rollback
	end
	else
	begin
		update user_time set total_time=total_time+@duration where account=@account and flag=1
		if (@@error <> 0) goto do_rollback
	end
	insert into dbo.card_success (pay_id, tid, account, email, tel_num, amount, item_id, auth_code, auth_time, card_code, card_name, result_code, result_msg, additional_info,op_date, status_flag, pay_kind) 
	values (@pay_id, @tid, @account, @email, @tel_num, @amount, @item_id, @auth_code, @auth_time, @card_code, @card_name, @result_code, '??', convert(char(12), getdate(), 112), getdate(), 'D1', @check_method)
	if (@@error <> 0) goto do_rollback

	commit tran
	set @ret_val = 1
	return		

do_rollback:
	rollback tran
	set @ret_val = -1
GO

/*
	@ret_val descripted by myhong 2003-08-20
	-1:db error  
	-3:pay_id error 
	-4:method error
	-5:no account
	1: ?? 
 	2: already pay_id 
	3:already id 
	4:expire reserve, 
	5 : amount error
	6 : ?? ?? ?? ??
	* ?? : 3? ?? ??? ??, ?? ?? ???.

	@email varchar(50),
	@auth_code varchar(8),
	@auth_time varchar(14),
	@card_code varchar(3),
	@card_name varchar(20),
	@result_code varchar(4),

*/
ALTER PROCEDURE [dbo].[dp_SetUserTimeByHp] 
	@pay_id char(16),
	@account varchar(14),
	@method integer,
	@item_id integer,
	@tel_num varchar(20),
	@tid varchar(20),
	@status tinyint, 
	@ret_val integer output
AS
set nocount on
declare @buffer varchar(50), @check_amount integer, @check_method integer, @duration integer, @uid integer, @block_flag integer, @block_flag2 integer, @warn_flag integer, @pay_stat integer
declare @old_pay_stat integer
select @buffer=pay_id from pay_master with (nolock) where pay_id=@pay_id
if @@rowcount > 0
begin
	set @ret_val = 2
	return
end

select @buffer=tid from dbo.hp_success with (nolock) where tid=@tid
if @@rowcount > 0 
begin
	set @ret_val = 3
	return
end

select @buffer=account from user_pay_reserve with (nolock) where account=@account
if @@rowcount > 0
begin
	set @ret_val = 4
	return
end

select @check_amount=amount, @check_method=(method/100)%10, @duration=duration from pay_method_code with (nolock) where method=@method
/*
???? ?? amount? input parameter? ??
if @check_amount <> @amount
begin
	set @ret_val = 5
	return
end
*/
if @check_method <> 2
begin
	set @ret_val = -4
	return
end

select @uid=uid, @block_flag=isnull(block_flag,0), @block_flag2=isnull(block_flag2,0), @warn_flag=warn_flag&2, @pay_stat=pay_stat from user_account with (nolock) where account=@account
if @@rowcount = 0
begin
	set @ret_val = -5
	return
end
else if @warn_flag <> 0
begin
	set @ret_val = 6
	return
end
else
begin
	if @pay_stat=0 and @block_flag=0 and @block_flag2=0
		goto current_pay
	else
	begin
		set @old_pay_stat = (@pay_stat/100)%10
		goto reserve_pay
	end
end

current_pay:
begin tran
	delete user_time where account=@account and flag=1
	if (@@error <> 0) goto do_rollback
	insert into user_pay (pay_id, account, start_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into user_time (uid, account, present_time, total_time, op_date, flag) values (@uid, @account, @duration, @duration, getdate(), 1)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, play_start, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, getdate(), @method, @item_id, @check_amount, getdate(), 1)
	if (@@error <> 0) goto do_rollback
	update user_account set pay_stat=@method where account=@account
	if (@@error <> 0) goto do_rollback
	insert into dbo.hp_success (pay_id, account, method, item_id, amount, op_date, tel_num, pay_kind, status, tid)
	values (@pay_id, @account, @method, @item_id, @check_amount,  getdate(), @tel_num, 1, @status, @tid)
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return 

reserve_pay:
begin tran
	insert into user_pay_reserve (pay_id, account, pay_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, @method, @item_id, @check_amount, getdate(), 2)
	if (@@error <> 0) goto do_rollback
	if @old_pay_stat <> 2
	begin
		delete user_time where account=@account and flag=1
		if (@@error <> 0) goto do_rollback
		insert into user_time (uid, account, present_time, total_time, op_date, flag) values (@uid, @account, @duration, @duration, getdate(), 1)
		if (@@error <> 0) goto do_rollback
	end
	else
	begin
		update user_time set total_time=total_time+@duration where account=@account and flag=1

		if (@@error <> 0) goto do_rollback
	end
	insert into dbo.hp_success (pay_id, account, method, item_id, amount, op_date, tel_num, pay_kind, status, tid)
	values (@pay_id, @account, @method, @item_id, @check_amount,  getdate(), @tel_num, 1, @status, @tid)
	if (@@error <> 0) goto do_rollback

	commit tran
	set @ret_val = 1
	return		

do_rollback:
	rollback tran
	set @ret_val = -1
GO

/*
	@ret_val descripted by myhong 2003-08-26
	-1:db error  
	-3:pay_id error 
	-4:method error
	-5:no account
	1: ?? 
 	2: already pay_id 
	3:already id 
	4:expire reserve, 
	5 : amount error
	6 : ?? ?? ?? ??
	* ?? : 3? ?? ??? ??, ?? ?? ???.

	@email varchar(50),
	@auth_code varchar(8),
	@auth_time varchar(14),
	@card_code varchar(3),
	@card_name varchar(20),
	@result_code varchar(4),

*/
ALTER PROCEDURE [dbo].[dp_SetUserTimeByHpBack] 
	@account varchar(14),
	@method integer,
	@item_id integer,
	@tel_num varchar(20),
	@tid varchar(20),
	@status tinyint, 
	@ret_val integer output
AS
set nocount on
declare @buffer varchar(50), @check_amount integer, @check_method integer, @duration integer, @uid integer, @block_flag integer, @block_flag2 integer, @warn_flag integer, @pay_stat integer, @pay_id char(16)

declare @old_pay_stat integer

exec dbo.dp_getnewpayid @pay_id output
if len(@pay_id) <> 16 
begin
	set @ret_val = -3
	return 
end

select @buffer=pay_id from pay_master with (nolock) where pay_id=@pay_id
if @@rowcount > 0
begin
	set @ret_val = 2
	return
end

select @buffer=tid from dbo.hp_success with (nolock) where tid=@tid
if @@rowcount > 0 
begin
	set @ret_val = 3
	return
end

select @buffer=account from user_pay_reserve with (nolock) where account=@account
if @@rowcount > 0
begin
	set @ret_val = 4
	return
end

select @check_amount=amount, @check_method=(method/100)%10, @duration=duration from pay_method_code with (nolock) where method=@method
/*
???? ?? amount? input parameter? ??
if @check_amount <> @amount
begin
	set @ret_val = 5
	return
end
*/
if @check_method <> 2
begin
	set @ret_val = -4
	return
end

select @uid=uid, @block_flag=isnull(block_flag,0), @block_flag2=isnull(block_flag2,0), @warn_flag=warn_flag&2, @pay_stat=pay_stat from user_account with (nolock) where account=@account
if @@rowcount = 0
begin
	set @ret_val = -5
	return
end
else if @warn_flag <> 0
begin
	set @ret_val = 6
	return
end
else
begin
	if @pay_stat=0 and @block_flag=0 and @block_flag2=0
		goto current_pay
	else
	begin
		set @old_pay_stat = (@pay_stat/100)%10
		goto reserve_pay
	end
end

current_pay:
begin tran
	delete user_time where account=@account and flag=1
	if (@@error <> 0) goto do_rollback
	insert into user_pay (pay_id, account, start_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into user_time (uid, account, present_time, total_time, op_date, flag) values (@uid, @account, @duration, @duration, getdate(), 1)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, play_start, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, getdate(), @method, @item_id, @check_amount, getdate(), 1)
	if (@@error <> 0) goto do_rollback
	update user_account set pay_stat=@method where account=@account
	if (@@error <> 0) goto do_rollback
	insert into dbo.hp_success (pay_id, account, method, item_id, amount, op_date, tel_num, pay_kind, status, tid)
	values (@pay_id, @account, @method, @item_id, @check_amount,  getdate(), @tel_num, 1, @status, @tid)
	if (@@error <> 0) goto do_rollback
	commit tran
	set @ret_val = 1
	return 

reserve_pay:
begin tran
	insert into user_pay_reserve (pay_id, account, pay_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
	if (@@error <> 0) goto do_rollback
	insert into pay_master (pay_id, account, method, item_id, real_amount, pay_date, loc) values (@pay_id, @account, @method, @item_id, @check_amount, getdate(), 2)
	if (@@error <> 0) goto do_rollback
	if @old_pay_stat <> 2
	begin
		delete user_time where account=@account and flag=1
		if (@@error <> 0) goto do_rollback
		insert into user_time (uid, account, present_time, total_time, op_date, flag) values (@uid, @account, @duration, @duration, getdate(), 1)
		if (@@error <> 0) goto do_rollback
	end
	else
	begin
		update user_time set total_time=total_time+@duration where account=@account and flag=1

		if (@@error <> 0) goto do_rollback
	end
	insert into dbo.hp_success (pay_id, account, method, item_id, amount, op_date, tel_num, pay_kind, status, tid)
	values (@pay_id, @account, @method, @item_id, @check_amount,  getdate(), @tel_num, 1, @status, @tid)
	if (@@error <> 0) goto do_rollback

	commit tran
	set @ret_val = 1
	return		

do_rollback:
	rollback tran
	set @ret_val = -1
GO

ALTER PROCEDURE [dbo].[l2p_TempCreateAccount]
@account varchar(14),
@ssn varchar(13)
AS
-- ?? ??? ???? ?? ?????. 
-- ?? ??? ???? ?? ??????? ???? ???? ????. 
-- ????????? ????? ???. 

DECLARE @account_num int

SELECT account FROM user_info WHERE account = @account
IF @@ROWCOUNT <> 0 
BEGIN
	print 'Already Exist'
	RETURN
END

SELECT @account_num= account_num FROM ssn WHERE ssn =@ssn
If @@rowcount  =  0
begin
	set @account_num = 1
end
else
begin
	set @account_num = @account_num + 1
end

BEGIN TRAN	
	IF @account_num = 1
		Insert ssn ( ssn, name, email, newsletter, job, phone, mobile, reg_date, zip, addr_main, addr_etc, account_num, status_flag )
		values (@ssn, '?????', 'newjisu@ncsoft.net',0,0,'02-1234-1234','011-1234-1234',getdate(),'','','',@account_num,0)		
	ELSE
		UPDATE ssn SET account_num = @account_num WHERE ssn =  @ssn
	IF @@ERROR <> 0 GOTO DO_ROLLBACK
	INSERT INTO user_account (account, pay_stat) VALUES (@account, 0)
	IF @@ERROR <> 0 GOTO DO_ROLLBACK
	Insert user_auth ( account, password, quiz1, quiz2, answer1, answer2 ) 
	values ( @account, 0xB53AA65D7C98EF3F0A93B5B578E2C4C4, '?? ?? ???? ??? ?????', '?? ?? ???? ??? ?????', 0x93A5EFCC45DA1D96A33A1C1CD14B6D6D, 0x93A5EFCC45DA1D96A33A1C1CD14B6D6D)
	IF @@ERROR <> 0 GOTO DO_ROLLBACK
	Insert user_info ( account, create_date, ssn, status_flag, kind )
	values ( @account, getdate(),@ssn, 0, 99  )
	IF @@ERROR <> 0 GOTO DO_ROLLBACK	
	
	
	--update user_account set pay_stat=101, login_flag=0 where account = @account
commit TRAN
RETURN 1

DO_ROLLBACK:
ROLLBACK TRAN
RETURN 0
GO

/*
	Descriptions : ???2 ????????

	Retrun Code
		@retcode = 1  '??
		@retcode = -1 'DB Error
		@retcode = -2 '???????
		@retcode = -3 '?????? ??
		@retcode = 2  '?? ???? ??
		
	Created by Woo Se Won, 2003-07-05
	
	- pay_stat = 1002 ? ??
*/	

ALTER PROCEDURE [dbo].[lin_addnewcreateaccount] 
	@master varchar(14),
	@name varchar(20),
	@ssn char(13),
	@account varchar(14),
	@passwd  varchar(512),
	@quiz1 varchar(255),
	@answer1 varchar(512),
	@quiz2 varchar(255),
	@answer2 varchar(512),
	@kind int,
	@anum tinyint,
	@retcode smallint output
AS

SET  NOCOUNT ON

DECLARE @sqlstr varchar(2048), @wstat int, @num tinyint

/* ssn ?? ? ?????? */
IF  EXISTS(SELECT *  FROM [lin2db].[dbo].[user_info] with(nolock) WHERE account = @account)
BEGIN
	SET @retcode = 2
	RETURN 
END

/* warn_flag check & account_num check*/
--SELECT top 1 @wstat= status_flag & 1, @num=account_num , @ssn = ssn FROM [lin2db].[dbo].[ssn] with(nolock) WHERE  ssn = 
--(SELECT ssn FROM [lin2db].[dbo].[user_info] with(nolock) WHERE account = @master ) and master = @master
SELECT top 1 @wstat= status_flag & 1, @num=account_num  FROM [lin2db].[dbo].[ssn] with(nolock) WHERE  ssn = @ssn and [name] = @name and master = @master
IF @@rowcount = 0
BEGIN
	SET @retcode = -2
	RETURN 
END

/* compare account_num*/
IF @num >= @anum
BEGIN
	SET @retcode = -3
	RETURN
END

/* warn_flag setting*/
IF @wstat = 0
	SET @wstat = 2 
ELSE
	SET @wstat = 0

/* insert account infomation */
SET @sqlstr = 'INSERT INTO [lin2db].[dbo].[user_auth] (account, password, quiz1, quiz2, answer1, answer2) values (''' +  @account + ''',  ' + @passwd + ',  ''' +  @quiz1 + ''', ''' +  @quiz2 + ''',' + @answer1 + ', ' + @answer2 + ')'
	
BEGIN TRAN
	UPDATE [lin2db].[dbo].[ssn]  SET account_num = account_num + 1 WHERE ssn = @ssn
	IF @@ERROR <> 0 GOTO DO_ROLLBACK
	
	EXEC (@sqlstr)
	IF @@ERROR <> 0 GOTO DO_ROLLBACK
	
	INSERT INTO [lin2db].[dbo].[user_info] (account, ssn, kind) VALUES
	(@account, @ssn, @kind)
	IF @@ERROR <> 0 GOTO DO_ROLLBACK
	
	INSERT INTO [lin2db].[dbo].[user_account] (account, pay_stat, warn_flag) VALUES
	(@account, 1002,@wstat)
	IF @@ERROR <> 0 GOTO DO_ROLLBACK

COMMIT TRAN
SET @retcode = 1
RETURN


DO_ROLLBACK:
ROLLBACK TRAN
SET @retcode = -1
RETURN
GO

/*
	Descriptions : ???2 ???????

	Retrun Code
		@retcode = 1  ??
		@retcode = -1 ??????
		@retcode = -2 ????
		@retcode = -3 1?????
		@retcode = -4 ??

	Created by Woo Se Won, 2003-07-05
	
*/	

ALTER PROCEDURE [dbo].[lin_changemasteraccount] 
	@account varchar(14),
	@master varchar(14),
	@retcode smallint output
AS

DECLARE @diffdate int, @ssn char(13), @check int
SET @check = 0

SET  NOCOUNT ON

IF EXISTS ( SELECT * FROM [lin2db].[dbo].[ssn] join [lin2db].[dbo].[user_info] on ssn.ssn = user_info.ssn WHERE user_info.account = @account and ssn.master = @master) goto DO_ERR

SELECT @diffdate = datediff(d,ssn.final_master_date , getdate() ) , @ssn = ssn.ssn FROM [lin2db].[dbo].[ssn] join [lin2db].[dbo].[user_info] on ssn.ssn = user_info.ssn WHERE user_info.account = @master
IF (@@ROWCOUNT = 0) goto DO_ERR1
IF(@diffdate = 0) goto DO_ERR2

BEGIN TRAN
	UPDATE [lin2db].[dbo].[ssn] SET master = @master , final_master_date = getdate() WHERE ssn = @ssn
	IF @@ERROR <> 0 goto DO_ERR3
COMMIT TRAN
SET @retcode = 1
RETURN

DO_ERR:
SET @retcode = -1
RETURN

DO_ERR1:
SET @retcode = -2
RETURN

DO_ERR2:
SET @retcode = -3 
RETURN

DO_ERR3:
ROLLBACK TRAN
SET @retcode = -4
RETURN
GO

/*
	Descriptions : ???2 ??? ? ??? ??????
	Retrun Code
		@retcode = 1  '??
		@retcode = -1 '??
		@retcode = -2 '???

	Created by Woo Se Won, 2003-07-05

*/	

ALTER PROCEDURE [dbo].[lin_changepassword] 
	@account varchar(14),
	@passwd  varchar(255),
	@answer1 varchar(255),
	@answer2 varchar(255),
	@retcode smallint output
AS

DECLARE @sqlstr varchar(1024)

SET  NOCOUNT ON

set @sqlstr = 'UPDATE [lin2db].[dbo].[user_auth] SET password =' + @passwd + ' WHERE account = ''' + @account + ''' and answer1 = ' + @answer1 + ' and answer2 = ' + @answer2
BEGIN TRAN
	EXEC (@sqlstr)
	IF @@ROWCOUNT = 0 goto DO_ROLLBACK1
	IF @@ERROR <> 0 goto DO_ROLLBACK	

	/*???? ?? flag ?*/
	UPDATE [lin2db].[dbo].[user_account] SET login_flag = login_flag & 2147483646 WHERE account = @account
	IF @@ERROR <> 0 goto DO_ROLLBACK


COMMIT TRAN
SET @retcode = 1
RETURN


DO_ROLLBACK:
ROLLBACK TRAN
SET @retcode = -1
RETURN

DO_ROLLBACK1:
ROLLBACK TRAN
SET @retcode = -2
RETURN
GO

/*
	Descriptions : ???2  ??????
	Retrun Code
		@retcode = 1  '??
		@retcode = -1 '??
		@retcode = -2 'user_auth ??? ???
		@retcode = -3 'ssn ??? ???
		@retcode = -4 '???? ??
		@retcode = -5 '???
		@retcode = -6 '?????

	Created by Woo Se Won, 2003-07-05

*/	

ALTER PROCEDURE [dbo].[lin_changepassword2] 
	@account varchar(14),
	@answer1 varchar(255),
	@answer2 varchar(255),
	@ssn char(13),
	@oldpasswd varchar(255),
	@newpasswd varchar(255),
	@retcode smallint output
AS

DECLARE @sqlstr varchar(1024)

SET  NOCOUNT ON


-- // ? ??
set @sqlstr = 'SELECT top 1 * FROM  [lin2db].[dbo].[user_auth]  WHERE account = ''' + @account + ''' and answer1 = ' + @answer1 + ' and answer2 = ' + @answer2
	EXEC (@sqlstr)
	IF @@ROWCOUNT = 0 goto DO_ROLLBACK4
	IF @@ERROR <> 0 goto DO_ROLLBACK5	


-- // ?? ???? ?? 
set @sqlstr = 'SELECT top 1 * FROM  [lin2db].[dbo].[user_auth]  (nolock) WHERE account = ''' + @account + ''' and password = ' + @oldpasswd 
	EXEC (@sqlstr)
	IF @@ROWCOUNT = 0 goto DO_ROLLBACK3
	IF @@ERROR <> 0 goto DO_ROLLBACK5	


-- //?????? ?? 
IF NOT EXISTS(SELECT * FROM [lin2db].[dbo].[user_info] WHERE account = @account and ssn = @ssn) goto DO_ROLLBACK2


-- //??? ????? ?? 
set @sqlstr = 'UPDATE [lin2db].[dbo].[user_auth] SET password =' + @newpasswd + ' WHERE account = ''' + @account + ''''
BEGIN TRAN
	EXEC (@sqlstr)
	IF @@ROWCOUNT = 0 goto DO_ROLLBACK1
	IF @@ERROR <> 0 goto DO_ROLLBACK	

	/*???? ?? flag ?*/
	UPDATE [lin2db].[dbo].[user_account] SET login_flag = login_flag & 2147483646 WHERE account = @account
	IF @@ERROR <> 0 goto DO_ROLLBACK

COMMIT TRAN
SET @retcode = 1
RETURN


DO_ROLLBACK:
ROLLBACK TRAN
SET @retcode = -1
RETURN


DO_ROLLBACK1:
ROLLBACK TRAN
SET @retcode = -2
RETURN

DO_ROLLBACK2:
SET @retcode = -3
RETURN

DO_ROLLBACK3:
SET @retcode = -4
RETURN

DO_ROLLBACK4:
SET @retcode = -5
RETURN

DO_ROLLBACK5:
SET @retcode = -6
RETURN
GO

/*
	Descriptions : ???2 ??, ? ??
	Retrun Code
		@retcode = 1  '??
		@retcode = -1 '??
		@retcode = -2 '????
		@retcode = -3 '????????
		@retcode = -4 '?????

	Created by Woo Se Won, 2003-07-05

*/	

ALTER PROCEDURE [dbo].[lin_changequiz] 
	@account varchar(14),
	@quiz1 varchar(255),
	@answer1 varchar(255),
	@quiz2 varchar(255),
	@answer2 varchar(255),
	@oldpasswd varchar(255),
	@newpasswd varchar(255),
	@retcode smallint output
AS

DECLARE @sqlstr varchar(2048)

SET  NOCOUNT ON
-- // ?????? ??
set @sqlstr = 'SELECT top 1 * FROM  [lin2db].[dbo].[user_auth]  (nolock) WHERE account = ''' + @account + ''' and password = ' + @oldpasswd 
	EXEC (@sqlstr)
	IF @@ROWCOUNT = 0 goto DO_ROLLBACK2
	IF @@ERROR <> 0 goto DO_ROLLBACK3	


set @sqlstr = 'UPDATE [lin2db].[dbo].[user_auth] SET  quiz1 = ''' + @quiz1 + ''' , quiz2 = ''' + @quiz2 + ''' , answer1 = ' + @answer1 + ' , answer2 = ' + @answer2 + ' , password = ' + @newpasswd +'  WHERE account = ''' + @account + ''''
BEGIN TRAN
	/*login_flag ???? ?*/
	UPDATE [lin2db].[dbo].[user_account] SET login_flag = login_flag & 2147483645 WHERE account = @account
	IF @@ERROR <> 0 goto DO_ROLLBACK

	EXEC (@sqlstr)
	IF @@ROWCOUNT = 0 goto DO_ROLLBACK1
	IF @@ERROR <> 0 goto DO_ROLLBACK

COMMIT TRAN
SET @retcode = 1
RETURN


DO_ROLLBACK:
ROLLBACK TRAN
SET @retcode = -1
RETURN

DO_ROLLBACK1:
ROLLBACK TRAN
SET @retcode = -2
RETURN

DO_ROLLBACK2:
SET @retcode = -3
RETURN

DO_ROLLBACK3:
SET @retcode = -4
RETURN
GO

/*
	Descriptions : ???2 ?????? ??
	Retrun Code
		@retcode = 1  '??
		@retcode = -1 '??
		@retcode = -2 'Master ?? ??

	Created by Woo Se Won, 2003-07-06

*/	

ALTER PROCEDURE [dbo].[lin_changeuserinfo] 
	@account varchar(14),
	@email varchar(50),
	@news tinyint,
	@phone varchar(20),
	@mobile varchar(20),
	@zip char(6),
	@addr1 varchar(255),
	@addr2 varchar(255),
	@retcode smallint output
AS

SET  NOCOUNT ON

-- // Master ?? ??
IF NOT EXISTS ( SELECT * FROM [lin2db].[dbo].[ssn] join [lin2db].[dbo].[user_info] on ssn.ssn = user_info.ssn WHERE user_info.account = @account and ssn.master = @account) goto DO_ERR


BEGIN TRAN
	/*news = 1*/
	IF @news = 1
		BEGIN
			UPDATE [lin2db].[dbo].[ssn] 
			SET email = @email, newsletter = @news, phone = @phone, mobile = @mobile, zip = @zip, addr_main = @addr1, addr_etc = @addr2, final_news_date = getdate() 
			WHERE ssn = (SELECT ssn FROM [lin2db].[dbo].[user_info] WHERE account = @account)
		END
	ELSE
		BEGIN
			UPDATE [lin2db].[dbo].[ssn] 
			SET email = @email, newsletter = @news, phone = @phone, mobile = @mobile, zip = @zip, addr_main = @addr1, addr_etc = @addr2 
			WHERE ssn = (SELECT ssn FROM [lin2db].[dbo].[user_info] WHERE account = @account)
		END
	IF @@ERROR <> 0 goto DO_ROLLBACK

COMMIT TRAN
SET @retcode = 1
RETURN


DO_ROLLBACK:
ROLLBACK TRAN
SET @retcode = -1
RETURN

DO_ERR:
SET @retcode = -2
RETURN
GO

/*
	Descriptions : ???2 ??? ? ??
	Retrun Code
		@retcode = 1  '??
		@retcode = -1 '??
		@retcode = -2 '???

	Created by Woo Se Won, 2003-07-07

*/	

ALTER PROCEDURE [dbo].[lin_checkanswer] 
	@account varchar(14),
	@answer1 varchar(255),
	@answer2 varchar(255),
	@retcode smallint output
AS

DECLARE @sqlstr varchar(1024)

SET  NOCOUNT ON

set @sqlstr = 'SELECT top 1 * FROM  [lin2db].[dbo].[user_auth]  (nolock) WHERE account = ''' + @account + ''' and answer1 = ' + @answer1 + ' and answer2 = ' + @answer2 

	EXEC (@sqlstr)

--select * from  [lin2db].[dbo].[user_auth]  WHERE account = @account and answer1 = @answer1 and answer2 = @answer2

	IF @@ROWCOUNT = 0 goto DO_ERR1
	IF @@ERROR <> 0 goto DO_ERR	

SET @retcode = 1
RETURN

DO_ERR:
SET @retcode = -1
RETURN

DO_ERR1:
SET @retcode = -2
RETURN
GO

/*
	Descriptions : ???2 ?????????

	Retrun Code
		@retcode = 1  '??
		@retcode = -1 '??

	Created by Woo Se Won, 2003-07-05
	
*/	

ALTER PROCEDURE [dbo].[lin_checkmasteraccount] 
	@account varchar(14),
	@retcode smallint output
AS

SET  NOCOUNT ON

IF NOT EXISTS ( select * from ssn join user_info on ssn.ssn = user_info.ssn where user_info.account = @account and user_info.account = ssn.master ) goto DO_ERR
SET @retcode = 1
RETURN

DO_ERR:
SET @retcode = -1
RETURN
GO

/*
	?    ? : ?????? ??? ???? ?? ??

	??? : 2003-06-18
	??? : woo se won
	
	??? : @ret >= 0 (??) ????
		  @ret = -1 (???????)
		  @ret = -2 (??? ? ?? ??????)
*/
ALTER PROCEDURE [dbo].[lin_checkssn]
	@ssn char(13),
	@ret int output
 AS

SET NOCOUNT ON

declare @num tinyint


SELECT ssn FROM [lin2db].[dbo].[ban_ssn] with(nolock) WHERE ssn = @ssn
IF @@rowcount <> 0  goto goto_back1

SELECT  @num=account_num FROM [lin2db].[dbo].[ssn] with(nolock) WHERE ssn  = @ssn
IF @@rowcount = 0  goto goto_back2

set @ret = @num
return

goto_back1:
set @ret = -2
return

goto_back2:
set @ret = -1
return
GO

/********************************************
lin_CreateAccount
	create account sp
INPUT
	@account	NCHAR(13),
	@pwd		NVARCHAR(50),
	@quiz1 		NVARCHAR(255),
	@quiz2 		NVARCHAR(255),
	@answer1 	NVARCHAR(255),
	@answer2 	NVARCHAR(255),
	@real_name 	NVARCHAR(20),
	@address 	NVARCHAR(50),
	@phone 		NVARCHAR(50),
	@email 		NVARCHAR(50)
OUTPUT
	@account_id,	@@IDENTITY
return
made by
	carrot
date
	2002-02-9
********************************************/
ALTER PROCEDURE [DBO].[lin_CreateAccount]
(
@account	NCHAR(13),
@pwd		NVARCHAR(50),
@quiz1 	NVARCHAR(255),
@quiz2 	NVARCHAR(255),
@answer1 	NVARCHAR(255),
@answer2 	NVARCHAR(255),
@real_name 	NVARCHAR(20),
@address 	NVARCHAR(50),
@phone 	NVARCHAR(50),
@email 	NVARCHAR(50)
)
AS

SET NOCOUNT ON

DECLARE @account_id	INT

	INSERT INTO user_account (account)
	VALUES (@account)
	
	SELECT @account_id = @@IDENTITY
	
	INSERT INTO user_auth (account, password, answer1, answer2, quiz1, quiz2)
	VALUES (@account, CONVERT(BINARY(50), @pwd),CONVERT(BINARY(50), @answer1), CONVERT(BINARY(50), @answer2), @quiz1, @quiz2)
	
	SELECT @account_id as Account_id
GO

ALTER PROCEDURE dbo.lin_CreateDummyAccount
(
	@beginIndex	INT,
	@endIndex	INT
)
AS

SET NOCOUNT ON

DECLARE @nowIndex INT
SELECT @nowIndex = @beginIndex
DECLARE @nowAccount VARCHAR(11)

WHILE @nowIndex <= @endIndex
BEGIN
	IF @nowIndex < 10
	BEGIN
		SELECT @nowAccount = 'dummy0000' + CONVERT(VARCHAR, @nowIndex)
	END
	ELSE IF @nowIndex < 100
	BEGIN
		SELECT @nowAccount = 'dummy000' + CONVERT(VARCHAR, @nowIndex)
	END
	ELSE IF @nowIndex < 1000
	BEGIN
		SELECT @nowAccount = 'dummy00' + CONVERT(VARCHAR, @nowIndex)
	END
	ELSE IF @nowIndex < 10000
	BEGIN
		SELECT @nowAccount = 'dummy0' + CONVERT(VARCHAR, @nowIndex)
	END
	ELSE IF @nowIndex < 100000
	BEGIN
		SELECT @nowAccount = 'dummy' + CONVERT(VARCHAR, @nowIndex)
	END
	ELSE
	BEGIN
		BREAK
	END

	INSERT INTO user_account (account) VALUES (@nowAccount)
	INSERT INTO user_auth(account, password, quiz1, quiz2, answer1, answer2)
	VALUES (@nowAccount, 0x700061007300730077006F0072006400, 'dummy?', 'dummy!',
	0x700061007300730077006F0072006400, 0x700061007300730077006F0072006400)

	SELECT @nowIndex = @nowIndex + 1
END
GO

/********************************************
lin_DeleteAccount
	delete account sp
INPUT
	@account	NCHAR(13),
OUTPUT
return
made by
	young
date
	2002-11-14
********************************************/
ALTER PROCEDURE [DBO].[lin_DeleteAccount]
(
@account	NCHAR(13)
)

as

set nocount on

delete from user_auth where account = @account

delete from user_account where account = @account
GO

/*
	Descriptions : ???2 ?? ????

	Created by Woo Se Won, 2003-07-05
	
*/	

ALTER PROCEDURE [dbo].[lin_getquiz] 
	@account varchar(14),
	@ssn char(13)
AS

SET  NOCOUNT ON
SELECT quiz1, quiz2 FROM [lin2db].[dbo].[user_auth] (nolock) WHERE account = 
(SELECT account FROM [lin2db].[dbo].[user_info] (nolock) WHERE account = @account and ssn = @ssn)

RETURN
GO

/*
	Descriptions : ???2 ?? ????

	Created by Woo Se Won, 2003-07-05
	
*/	

ALTER PROCEDURE [dbo].[lin_getquiz2] 
	@account varchar(14)
AS

SET  NOCOUNT ON
SELECT quiz1, quiz2 FROM [lin2db].[dbo].[user_auth] (nolock) WHERE account = @account

RETURN
GO

/*
	Descriptions : ??????

	Created by Woo Se Won, 2003-07-06
	
*/	

ALTER PROCEDURE [dbo].[lin_getuserinfo] 
	@account varchar(14)
AS

SET  NOCOUNT ON

SELECT email,newsletter,zip,addr_main, addr_etc, phone, mobile, master, status_flag & 4 AS email_flag, status_flag & 1 AS  ssn_flag, name, ssn.ssn
FROM [lin2db].[dbo].[ssn] (nolock)
WHERE ssn = (SELECT ssn FROM [lin2db].[dbo].[user_info] (nolock) WHERE account = @account)

RETURN
GO

/********************************************
lin_LoginAccount
	login account sp
INPUT
	@account	NCHAR(13),
OUTPUT
return
made by
	young
date
	2002-10-16
********************************************/
ALTER PROCEDURE [DBO].[lin_LoginAccount]
(
@account	NCHAR(13)
)
AS

SET NOCOUNT ON

update user_account set last_login = getdate() where account = @account
GO

/********************************************
lin_LogoutAccount
	logout account sp
INPUT
	@account	NCHAR(13),
OUTPUT
return
made by
	young
date
	2002-10-16
********************************************/
ALTER PROCEDURE [DBO].[lin_LogoutAccount]
(
@account	NCHAR(13)
)
AS

SET NOCOUNT ON

update user_account set last_logout = getdate() where account = @account
GO

/*
	Descriptions : ???2 ???????? (1? 3??)

	Retrun Code
		@retcode = 1  '??
		@retcode = -1 'DB Error
		@retcode = 3  '?? ???? ??
		@retcode = 4  '?? ???? ??????
		
	Created by Woo Se Won, 2003-06-19
	
	- pay_stat = 1002 ? ??
*/	

ALTER PROCEDURE [dbo].[lin_newcreateaccount] 
	@name varchar(20),
	@ssn char(13),
	@account varchar(14),
	@passwd  varchar(255),
	@quiz1 varchar(255),
	@answer1 varchar(255),
	@quiz2 varchar(255),
	@answer2 varchar(255),
	@email varchar(50),
	@news tinyint,
	@job int,
	@phone varchar(13),
	@mobile varchar(13),
	@zip char(6),
	@addr1 varchar(255),
	@addr2 varchar(255),
	@kind int,
	@status int,
	@retcode smallint output
AS

SET  NOCOUNT ON

DECLARE @sqlstr varchar(1024), @wstat int

--warn_stat setting
SELECT @wstat = (@status & 1)
IF @wstat = 0
	SET @wstat = 2 
ELSE
	SET @wstat = 0

SELECT account FROM [lin2db].[dbo].[user_info] with(nolock) WHERE account = @account
IF @@ROWCOUNT <> 0 
BEGIN
	SET @retcode = 3
	RETURN 
END
 
SELECT ssn FROM [lin2db].[dbo].[ssn] with(nolock) WHERE ssn = @ssn
IF @@ROWCOUNT <> 0 
BEGIN
	SET @retcode = 4
	RETURN 
END

SET @sqlstr = 'INSERT INTO [lin2db].[dbo].[user_auth] (account, password, quiz1, quiz2, answer1, answer2) values (''' +  @account + ''',  ' + @passwd + ',  ''' +  @quiz1 + ''', ''' +  @quiz2 + ''',' + @answer1 + ', ' + @answer2 + ')'
	
BEGIN TRAN
	INSERT INTO [lin2db].[dbo].[ssn] (name,ssn,email,newsletter,job,phone,mobile,zip,addr_main,addr_etc,account_num,status_flag,master) VALUES
	(@name, @ssn, @email, @news, @job, @phone, @mobile, @zip, @addr1, @addr2,1,@status,@account)
	IF @@ERROR <> 0 GOTO DO_ROLLBACK
	
	EXEC (@sqlstr)
	IF @@ERROR <> 0 GOTO DO_ROLLBACK
	
	INSERT INTO [lin2db].[dbo].[user_info] (account, ssn, kind) VALUES
	(@account, @ssn, @kind)
	IF @@ERROR <> 0 GOTO DO_ROLLBACK
	
	INSERT INTO [lin2db].[dbo].[user_account] (account, pay_stat, warn_flag) VALUES
	(@account, 1002,@wstat)
	IF @@ERROR <> 0 GOTO DO_ROLLBACK

COMMIT TRAN
SET @retcode = 1
RETURN


DO_ROLLBACK:
ROLLBACK TRAN
SET @retcode = -1
RETURN
GO

ALTER PROCEDURE [dbo].[lp_CreateAccount_temp]
@account varchar(14)
AS


DECLARE @account_num int

SELECT account FROM user_info WHERE account = @account
IF @@ROWCOUNT <> 0 
BEGIN
	print 'Already Exist'
	RETURN
END

SELECT @account_num= account_num FROM ssn WHERE ssn = '0000000000000'
If @@rowcount  =  0
begin
	set @account_num = 1
end
else
begin
	set @account_num = @account_num + 1
end

BEGIN TRAN	
	IF @account_num = 1
		Insert ssn ( ssn, name, email, newsletter, job, phone, mobile, reg_date, zip, addr_main, addr_etc, account_num, status_flag )
		values ('0000000000000', 'test', 'test@ncsoft.net',0,0,'','',getdate(),'','','',@account_num,0)		
	ELSE
		UPDATE ssn SET account_num = @account_num WHERE ssn =  '8306151674238'
	INSERT INTO user_account (account, pay_stat) VALUES (@account, 0)
	Insert user_auth ( account, password, quiz1, quiz2, answer1, answer2 ) 
	values ( @account, 0xB5BAE690B15763B2871E3838F56F4949, @account, @account, 0xB5BAE690B15763B2871E3838F56F4949CDF39ED9C91E1BFBCE577171BC264949 , 0xB5BAE690B15763B2871E3838F56F4949CDF39ED9C91E1BFBCE577171BC264949 )
	Insert user_info ( account, create_date, ssn, status_flag, kind )
	values ( @account, getdate(), '0000000000000', 0, 99  )
	
	
	--update user_account set pay_stat=101, login_flag=0 where account = @account
commit TRAN
print 'success'
RETURN 1

DO_ROLLBACK:
ROLLBACK TRAN
print 'db rollback'
RETURN 0
GO

ALTER PROCEDURE dbo.lp_del_account
@account varchar(14)
 AS
	delete ssn where ssn = ( select ssn from user_info where account = @account )
	delete user_info where account = @account
	delete user_auth where account = @account
	delete user_account where account = @account
GO

ALTER PROCEDURE dbo.lp_del_account2
@account varchar(14)
 AS
	delete user_info where account = @account
	delete user_auth where account = @account
	delete user_account where account = @account
GO

ALTER procedure [dbo].[lp_l2staticUsetime]
(@strDate varchar(10), @per varchar(2), @mtype varchar(10))
as

set nocount on

declare @before varchar(20)
declare @strsql varchar(5000)
declare @avg varchar(3)
declare @where varchar(1000)

-- ???? ?? ???? (??,??,??,???)
if @per = 'mm'	--??
	begin	
	set @before = convert(varchar(10),dateadd(yy, -1, cast(@strDate as datetime)),120)		
	set @avg = '30'
	set @where =  'where e.date between ''' + @before + ''' and dateadd(' + @per + ',-1,''' + @strdate + ''') ' 
	end
else if @per ='ww'	--??
	begin
	set @before = convert(varchar(10),dateadd(mm, -1, cast(@strDate as datetime)),120)	
	set @where =  'where datepart(' + @per + ',e.date) between datepart(' + @per + ', ''' + @before + ''') and datepart(' + @per + ',''' + @strdate + ''')-1 ' 
	set @avg = '7'
	end
else 			--??	@per='dd'	
	begin
	set @before = convert(varchar(10),dateadd(dd, -7, cast(@strDate as datetime)),120)	
	set @where = ' where e.date between ''' + @before + ''' and  ''' + @strdate + ''''
	set @avg = '1'	
	end

-- ???? ?? ??? ?? (??, ????, ?????)

if @mtype = 'goods'
	begin
 	set @strsql = 'select case e.stat when ''1002'' then ''obt'' else cast(e.stat as varchar(5)) end stat, convert(char(10), min(e.date), 120) std_date, sum(isnull(d.use_time,0))/'+@avg+ ' total '  + 
		        ' from (select * from ' +
		        ' (select distinct(date) date from history_times) as a ' +
		        ' cross join ' +
		        ' (select  distinct(stat) stat  from history_times) as b)as e ' +
		        ' left join (select stat, date, sum(use_time) use_time from history_times group by stat, date) as d ' +	
		        ' on d.date = e.date and d.stat = e.stat ' +
		        @where +
	         	        ' group by e.stat, datepart(' + @per + ', e.date) order by stat, std_date ' 		
	end 

else if @mtype = 'agegoods'
	begin
	set @strsql = ' select case e.stat when ''1002'' then ''obt'' else cast(e.stat as varchar(5)) end stat, convert(char(10), max(e.date), 120) std_date, sum(isnull(f.use_time, 0)) /'+@avg+ ' total '  + 
		        ' from (select * from  (select distinct(date) date from history_times) as a ' 	+
		        ' cross join  (select distinct(cast(age as varchar(3))+''-''+case cast(stat as varchar(4)) when ''1002'' then ''obt'' else cast(stat as varchar(4)) end)  stat from history_times) as b )  as e ' +
		        ' left outer join (select cast(age as varchar(3))+''-''+case cast(stat as varchar(4)) when ''1002'' then ''obt'' else cast(stat as varchar(4)) end stat, date, sum(use_time) use_time from history_times group by age, stat, date ) as f ' +
		        ' on e.date=f.date and e.stat = f.stat ' +
		        @where +
		        ' and left(e.stat,2)  < 41 group by e.stat, datepart(' + @per + ', e.date) ' +
		        ' UNION ' +
		        ' select  min(e.stat) stat, convert(char(10), max(e.date), 120) std_date, sum(isnull(use_time,0))/'+@avg+ ' total '  + 
		        ' from (select * from  (select distinct(date) date from history_times) as a ' 	+
		        ' cross join  (select distinct(cast(age as varchar(3))+''-''+case cast(stat as varchar(4)) when ''1002'' then ''obt'' else cast(stat as varchar(4)) end) stat from history_times) as b )  as e ' +
		        ' left outer join (select cast(age as varchar(3))+''-''+case cast(stat as varchar(4)) when ''1002'' then ''obt'' else cast(stat as varchar(4)) end stat, date, sum(use_time) use_time from history_times group by age, stat, date ) as f ' +
		        ' on e.date=f.date and e.stat = f.stat ' +
		        @where +
		        ' and left(e.stat,2) > 40 group by datepart(' + @per + ', e.date) ' +
		        ' order by stat,  std_date' 
	end
else
	begin
	select 'error' as message
	end

print @strsql

exec(@strsql)
GO

ALTER procedure [dbo].[lp_l2statisticAgeGender]
(@strDate varchar(10), @per varchar(2), @acc_ssn varchar(3))
as

set nocount on

declare @before varchar(20)
declare @strsql varchar(5000)
declare @avg varchar(3)
declare @column varchar(10)

-- ???? ?? ??? ?? (??, ????, ?????)
if @acc_ssn = 'acc' 
	begin
	set @column = 'acc_cnt'
	end
else
	begin
	set @column = 'ssn_cnt'
	end

if @per = 'dd'	--??
	begin

	set @before = convert(varchar(10),dateadd(dd, -7, cast(@strDate as datetime)),120)	
	set @strsql = ' select  e.stat stat, convert(char(10), min(e.date), 120) std_date, sum(isnull(f.' + @column + ', 0)) total '  + 
	        ' from (select * from  (select distinct(date) date from age_cnt) as a ' 	+
	        ' cross join  (select distinct(cast(birth_year as varchar(3))+''::''+case cast(sex as varchar(3)) when ''1'' then ''?'' when ''2'' then ''?'' when ''3'' then ''?'' when ''4'' then ''?'' end) stat from age_cnt) as b )  as e ' +
	        ' left outer join (select cast(birth_year as varchar(3))+''::''+case cast(sex as varchar(3)) when ''1'' then ''?'' when ''2'' then ''?'' when ''3'' then ''?'' when ''4'' then ''?'' end stat, date, sum(' + @column + ') ' + @column + ' from age_cnt group by birth_year, sex, date ) as f ' +
	        ' on e.date=f.date and e.stat = f.stat ' +
	        ' where e.date between ''' + @before  + ''' and ''' + @strdate  + ''' and left(e.stat,2)  < 41 group by e.stat, datepart(' + @per + ', e.date) ' +
	        ' UNION ' +
	        ' select  min(e.stat) stat, convert(char(10), min(e.date), 120) std_date, sum(isnull(' + @column + ',0)) total '  + 
	        ' from (select * from  (select distinct(date) date from age_cnt) as a ' 	+
	        ' cross join  (select distinct(cast(birth_year as varchar(3))+''::''+case cast(sex as varchar(3)) when ''1'' then ''?'' when ''2'' then ''?'' when ''3'' then ''?'' when ''4'' then ''?'' end) stat from age_cnt) as b )  as e ' +
	        ' left outer join (select cast(birth_year as varchar(3))+''::''+case cast(sex as varchar(3)) when ''1'' then ''?'' when ''2'' then ''?'' when ''3'' then ''?'' when ''4'' then ''?'' end stat, date, sum(' + @column + ') ' + @column + ' from age_cnt group by birth_year, sex, date ) as f ' +
	        ' on e.date=f.date and e.stat = f.stat ' +
	        ' where e.date between ''' + @before  + ''' and ''' + @strdate  + ''' and left(e.stat,2) > 40 group by right(e.stat,1), datepart(' + @per + ', e.date)  ' +
	        ' order by stat asc, std_date' 
	end

else if @per ='ww'	--??
	begin	
	set @strsql = ' select  e.stat stat, convert(char(10), min(e.date), 120) std_date, sum(isnull(f.' + @column + ', 0)) total '  + 
	        ' from (select * from  (select distinct(date) date from age_cnt) as a ' 	+
	        ' cross join  (select distinct(cast(birth_year as varchar(3))+''::''+case cast(sex as varchar(3)) when ''1'' then ''?'' when ''2'' then ''?'' when ''3'' then ''?'' when ''4'' then ''?'' end) stat from age_cnt) as b )  as e ' +
	        ' left outer join (select cast(birth_year as varchar(3))+''::''+case cast(sex as varchar(3)) when ''1'' then ''?'' when ''2'' then ''?'' when ''3'' then ''?'' when ''4'' then ''?'' end stat, date, sum(' + @column + ') ' + @column + ' from age_cnt group by birth_year, sex, date ) as f ' +
	        ' on e.date=f.date and e.stat = f.stat ' +
	        ' where e.date >= convert(varchar(10), dateadd(mm, -1, cast('''+@strDate+''' as datetime)),120) ' +
	        ' and e.date < (select max(date) from age_cnt where datepart(dw,date) = 1) and datepart(dw,e.date) = 7 ' +
	        ' and left(e.stat,2)  < 41 group by e.stat,datepart(wk, e.date) ' +
'                    


'+

	        ' UNION ' +
	        ' select  min(e.stat) stat, convert(char(10), min(e.date), 120) std_date, sum(isnull(' + @column + ',0)) total '  + 
	        ' from (select * from  (select distinct(date) date from age_cnt) as a ' 	+
	        ' cross join  (select distinct(cast(birth_year as varchar(3))+''::''+case cast(sex as varchar(3)) when ''1'' then ''?'' when ''2'' then ''?'' when ''3'' then ''?'' when ''4'' then ''?'' end) stat from age_cnt) as b )  as e ' +
	        ' left outer join (select cast(birth_year as varchar(3))+''::''+case cast(sex as varchar(3)) when ''1'' then ''?'' when ''2'' then ''?'' when ''3'' then ''?'' when ''4'' then ''?'' end stat, date, sum(' + @column + ') ' + @column + ' from age_cnt group by birth_year, sex, date ) as f ' +
	        ' on e.date=f.date and e.stat = f.stat ' +
	        ' where e.date >= convert(varchar(10), dateadd(mm, -1, cast('''+@strDate+''' as datetime)),120) ' +
	        ' and e.date < (select max(date) from age_cnt where datepart(dw,date) = 1) and datepart(dw,e.date) = 7 ' +
	        ' and left(e.stat,2)  > 40 group by right(e.stat,1), datepart(wk, e.date) ' +
	        ' order by stat asc,  std_date' 
	end


if @per = 'mm'	--??
	begin	
	set @strsql = ' select  e.stat stat, convert(char(10), min(e.date), 120) std_date, sum(isnull(f.' + @column + ', 0)) total '  + 
	        ' from (select * from  (select distinct(date) date from age_cnt) as a ' 	+
	        ' cross join  (select distinct(cast(birth_year as varchar(3))+''::''+case cast(sex as varchar(3)) when ''1'' then ''?'' when ''2'' then ''?'' when ''3'' then ''?'' when ''4'' then ''?'' end) stat from age_cnt) as b )  as e ' +
	        ' left outer join (select cast(birth_year as varchar(3))+''::''+case cast(sex as varchar(3)) when ''1'' then ''?'' when ''2'' then ''?'' when ''3'' then ''?'' when ''4'' then ''?'' end stat, date, sum(' + @column + ') ' + @column + ' from age_cnt group by birth_year, sex, date ) as f ' +
	        ' on e.date=f.date and e.stat = f.stat ' +
	        ' where e.date in (select convert(char(10),max(date),120) from age_cnt where date < (select convert(char(8),'''+@strDate+''',120) + ''01'') group by datepart(mm,date)) ' +
	        ' and left(e.stat,2)  < 41 group by e.stat, datepart(' + @per + ', e.date) ' +
	        ' UNION ' +
	        ' select  min(e.stat) stat, convert(char(10), min(e.date), 120) std_date, sum(isnull(' + @column + ',0)) total '  + 
	        ' from (select * from  (select distinct(date) date from age_cnt) as a ' 	+
	        ' cross join  (select distinct(cast(birth_year as varchar(3))+''::''+case cast(sex as varchar(3)) when ''1'' then ''?'' when ''2'' then ''?'' when ''3'' then ''?'' when ''4'' then ''?'' end) stat from age_cnt) as b )  as e ' +
	        ' left outer join (select cast(birth_year as varchar(3))+''::''+case cast(sex as varchar(3)) when ''1'' then ''?'' when ''2'' then ''?'' when ''3'' then ''?'' when ''4'' then ''?'' end stat, date, sum(' + @column + ') ' + @column + ' from age_cnt group by birth_year, sex, date ) as f ' +
	        ' on e.date=f.date and e.stat = f.stat ' +
	        ' where e.date in (select convert(char(10),max(date),120) from age_cnt where date < (select convert(char(8),'''+@strDate+''',120) + ''01'') group by datepart(mm,date)) ' +
	        ' and left(e.stat,2)  > 40 group by right(e.stat,1), datepart(' + @per + ', e.date)  ' +
	        ' order by stat asc,  std_date' 
	end


print @strsql
exec(@strsql)
GO

ALTER procedure  [dbo].[lp_L2statisticConn]
(@strDate char(10), @per varchar(2), @mtype varchar(10), @object varchar(5))
as

set nocount on

declare @before varchar(20)
declare @strsql varchar(5000)
declare @column varchar(10)
declare @table varchar(50)

-- ???? ?? ?? (???, ????)
if @object = 'acnt'	
	set @column = 'acc_cnt'	
else	
	set @column = 'ssn_cnt'
	

-- ???? ?? ???? (??,??,??,???)

if @per = 'mm'	--??
	begin	
	set @before = convert(varchar(10),dateadd(yy, -1, cast(@strDate as datetime)),120)		
	set @table = 'month_conuser'
	end
else if @per ='ww'	--??
	begin
	set @before = convert(varchar(10),dateadd(mm, -1, cast(@strDate as datetime)),120)	
	set @table = 'week_conuser'
	end
else if @per='dd'		--??
	begin
	set @before = convert(varchar(10),dateadd(dd, -7, cast(@strDate as datetime)),120)	
	set @table = 'total_conuser'
	end

-- ???? ?? ??? ?? (??, ????, ?????)

if @mtype = 'total'
	begin
 	set @strsql = 'select convert(char(10), min(date), 120) as std_date, sum(isnull(' + @column + ', 0)) as total_acc '  + 
		        ' from ' + @table + ' where date between ''' + @before  + ''' and ''' + @strdate  +
		        ''' group by datepart(' + @per + ', date) order by std_date ' 	
	end 
else if @mtype = 'age'
	begin
 	set @strsql = 'select c.stat stat, convert(char(10), min(c.date), 120) std_date, sum(isnull(d.total,0)) total '  + 
		        ' from (select * from ' +
		        ' (select distinct(date) date from ' + @table + ' ) as a ' +
		        ' cross join ' +
		        ' (select  distinct(age) stat  from  ' + @table + ' ) as b)as c ' +
		        ' left join (select age stat, date, sum(' + @column + ') total from  ' + @table + '  group by age, date) as d ' +	
		        ' on d.date = c.date and d.stat = c.stat ' +
		        ' where c.date between ''' + @before  + ''' and ''' + @strdate  + ''' and left(c.stat,2)  < 41 group by c.stat, datepart(' + @per + ', c.date) '  +
		        ' UNION ' +
		        'select min(c.stat) stat, convert(char(10), min(c.date), 120) std_date, sum(isnull(d.total,0)) total '  + 
		        ' from (select * from ' +
		        ' (select distinct(date) date from  ' + @table + ' ) as a ' +
		        ' cross join ' +
		        ' (select  distinct(age) stat  from  ' + @table + ' ) as b)as c ' +
		        ' left join (select age stat, date, sum(' + @column + ') total from  ' + @table + ' group by age, date) as d ' +	
		        ' on d.date = c.date and d.stat = c.stat ' +
		        ' where c.date between ''' + @before  + ''' and ''' + @strdate  + ''' and left(c.stat,2)  > 40 group by datepart(' + @per + ', c.date) '  +
		        ' order by stat,  std_date' 
	end

else if @mtype = 'agepay'
	begin
	set @strsql = ' select  e.stat stat, convert(char(10), min(e.date), 120) std_date, sum(isnull(f.' + @column + ', 0)) total '  + 
		        ' from (select * from  (select distinct(date) date from  ' + @table + ' ) as a ' 	+
		        ' cross join  (select distinct(cast(age as varchar(3))+''-''+case cast(stat as varchar(4)) when ''1002'' then ''obt'' else cast(stat as varchar(4)) end) stat from  ' + @table + ' ) as b )  as e ' +
		        ' left outer join (select cast(age as varchar(3))+''-''+case cast(stat as varchar(4)) when ''1002'' then ''obt'' else cast(stat as varchar(4)) end stat, date, sum(' + @column + ') ' + @column + ' from  ' + @table + '  group by age, stat, date ) as f ' +
		        ' on e.date=f.date and e.stat = f.stat ' +
		        ' where e.date between ''' + @before  + ''' and ''' + @strdate  + ''' and left(e.stat,2)  < 41 group by e.stat, datepart(' + @per + ', e.date) ' +
		        ' UNION ' +
		        ' select  min(e.stat) stat, convert(char(10), min(e.date), 120) std_date, sum(isnull(' + @column + ',0)) total '  + 
		        ' from (select * from  (select distinct(date) date from  ' + @table + ' ) as a ' 	+
		        ' cross join  (select distinct(cast(age as varchar(3))+''-''+case cast(stat as varchar(4)) when ''1002'' then ''obt'' else cast(stat as varchar(4)) end) stat from  ' + @table + ' ) as b )  as e ' +
		        ' left outer join (select cast(age as varchar(3))+''-''+case cast(stat as varchar(4)) when ''1002'' then ''obt'' else cast(stat as varchar(4)) end stat, date, sum(' + @column + ') ' + @column + ' from  ' + @table + '  group by age, stat, date ) as f ' +
		        ' on e.date=f.date and e.stat = f.stat ' +
		        ' where e.date between ''' + @before  + ''' and ''' + @strdate  + ''' and left(e.stat,2) > 40 group by datepart(' + @per + ', e.date)  ' +
		        ' order by stat,  std_date' 
	end
else
	begin
	select 'error' as message
	end

print @strsql

exec(@strsql)
GO

ALTER procedure  [dbo].[lp_L2statisticConn_HH]
(@strDate char(10), @per varchar(2), @mtype varchar(10), @object varchar(5))
as

set nocount on

declare @before varchar(10)
declare @strsql varchar(5000)
declare @column varchar(10)


set @before = convert(varchar(10),dateadd(dd, -1, cast(@strDate as datetime)),120)

-- ???? ?? ?? (???, ????)
if @object = 'acnt'	
	set @column = 'acc_cnt'	
else	
	set @column = 'ssn_cnt'
	

if @mtype = 'total'
	begin
 	set @strsql = 'select right(convert(char(13), min(date), 120), 11) as std_date, sum(isnull(' + @column + ', 0)) as total_acc '  + 
		        ' from history_conuser where date between ''' + @before + ''' and ''' + @strDate + '' +
		        ''' group by  convert(char(10),date,120), datepart(' + @per + ', date) order by std_date ' 	
	end 
else if @mtype = 'age'
	begin
	--set @strsql = 'select cast(age as varchar(2)) as age, convert(char(13), min(date), 120) as std_date, sum(isnull(' + @column + ', 0)) as total_acc ' +
		--        ' from history_conuser where age <41 and date = ''' + @before  + ''' group by age, convert(char(10),date,120), datepart(' + @per + ', date) ' 	+
		 --       ' UNION ' +
		  --      'select ''41??'' as age, convert(char(13), min(date), 120) as std_date, sum(' + @column + ') as total_acc ' +
		   --     ' from history_conuser where age > 40 and date = ''' +  @before + ''' group by convert(char(10),date,120), datepart(' + @per + ', date) order by age, std_date '

 	set @strsql = 'select c.stat stat, right(convert(char(13), min(c.date), 120),11) std_date, sum(isnull(d.total,0)) total '  + 
		        ' from (select * from (select distinct(date) date from history_conuser) as a ' +
		        ' cross join (select  distinct(age) stat  from history_conuser) as b)as c ' +
		        ' left join (select age stat, date, sum(' + @column + ') total from history_conuser group by age, date) as d ' +	
		        ' on d.date = c.date and d.stat = c.stat ' +
		        ' where c.date between ''' +@before + ''' and ''' + @strDate +''' and left(c.stat,2)  < 41 group by c.stat, datepart(' + @per + ', c.date) '  +
		        ' UNION ' +
		        'select min(c.stat) stat, right(convert(char(13), min(c.date), 120),11) std_date, sum(isnull(d.total,0)) total '  + 
		        ' from (select * from (select distinct(date) date from history_conuser) as a ' +
		        ' cross join (select  distinct(age) stat  from history_conuser) as b)as c ' +
		        ' left join (select age stat, date, sum(' + @column + ') total from history_conuser group by age, date) as d ' +	
		        ' on d.date = c.date and d.stat = c.stat ' +
		        ' where c.date between ''' +@before + ''' and ''' + @strDate + ''' and left(c.stat,2)  > 40 group by datepart(' + @per + ', c.date) '  +
		        ' order by stat,  std_date' 
	end

else if @mtype = 'agepay'
	begin
	set @strsql = ' select  e.stat stat, right(convert(char(13), min(e.date), 120),11)std_date, sum(isnull(f.' + @column + ', 0)) total '  + 
		        ' from (select * from  (select distinct(date) date from history_conuser) as a ' 	+
		        ' cross join  (select distinct(cast(age as varchar(3))+''-''+ case cast(stat as varchar(4)) when ''1002'' then ''obt'' else cast(stat as varchar(4)) end) stat from history_conuser) as b )  as e ' +
		        ' left outer join (select cast(age as varchar(3))+''-''+case cast(stat as varchar(4)) when ''1002'' then ''obt'' else cast(stat as varchar(4)) end stat, date, sum(' + @column + ') ' + @column + ' from history_conuser group by age, stat, date ) as f ' +
		        ' on e.date=f.date and e.stat = f.stat ' +
		        ' where e.date between ''' +@before + ''' and ''' + @strDate + ''' and left(e.stat,2)  < 41 group by e.stat, datepart(' + @per + ', e.date) ' +
		        ' UNION ' +
		        ' select  min(e.stat) stat, right(convert(char(13), min(e.date), 120),11) std_date, sum(isnull(' + @column + ',0)) total '  + 
		        ' from (select * from  (select distinct(date) date from history_conuser) as a ' 	+
		        ' cross join  (select distinct(cast(age as varchar(3))+''-''+case cast(stat as varchar(4)) when ''1002'' then ''obt'' else cast(stat as varchar(4)) end) stat from history_conuser) as b )  as e ' +
		        ' left outer join (select cast(age as varchar(3))+''-''+case cast(stat as varchar(4)) when ''1002'' then ''obt'' else cast(stat as varchar(4)) end stat, date, sum(' + @column + ') ' + @column + ' from history_conuser group by age, stat, date ) as f ' +
		        ' on e.date=f.date and e.stat = f.stat ' +
		        ' where e.date  between ''' +@before + ''' and ''' + @strDate + ''' and left(e.stat,2) > 40 group by datepart(' + @per + ', e.date)  ' +
		        ' order by stat,  std_date' 
	end
else
	begin
	select 'error' as message
	end

print @strsql
exec (@strsql)
GO

CREATE TRIGGER [dbo].[dt_UpdatePayStatus] ON [dbo].[user_time]
FOR UPDATE
AS
declare @account varchar(14), @present_time integer, @total_time integer, @old_pay_id char(16), @pay_id char(16), @method integer, @feature integer, @duration integer
set nocount off
select @account=account, @present_time=present_time, @total_time=total_time from inserted
if @@rowcount=0 or (@present_time > 0)
	goto exit_trigger

select @account=account from user_account with (nolock) where account=@account and block_flag=0 and block_flag2=0 
if @@rowcount = 0
	goto exit_trigger

select @old_pay_id=pay_id from user_pay where account=@account
while @present_time <= 0 
begin
	select  top 1@pay_id=user_pay_reserve.pay_id,  @method=pay_method_code.method, @duration=pay_method_code.duration from user_pay_reserve with (nolock)
	left join pay_method_code on user_pay_reserve.method=pay_method_code.method where account=@account order by pay_id
	if @@rowcount > 0
	begin
		if (@method/100)%10= 1
		begin
			delete user_pay where account=@account
			delete user_time where account=@account and flag=1
			update pay_master set 	play_end=dateadd(ss, @present_time, getdate()) ,real_end_date=dateadd(ss, @present_time, getdate()), loc=3 where pay_id=@old_pay_id
			update pay_master set  play_start=dateadd(ss, @present_time, getdate()), play_end=dateadd(day, @duration-1, getdate()), real_end_date=dateadd(day, @duration-1, getdate()), loc=1 where pay_id=@pay_id
			insert into user_pay (pay_id, account, start_date, method, duration) values (@pay_id, @account, getdate(), @method, @duration)
			delete user_pay_reserve where pay_id=@pay_id and account=@account
			update user_account set pay_stat=@method where account=@account
			break
		end
		else if (@method/100)%10= 2
		begin
			delete user_pay where account=@account
			update pay_master set play_end=dateadd(ss, @present_time, getdate()), real_end_date=dateadd(ss, @present_time, getdate()), loc=3 where pay_id=@old_pay_id
			update pay_master set   play_start=dateadd(ss, @present_time, getdate()), loc=1 where pay_id=@pay_id
			insert into user_pay (pay_id, account, start_date, method, duration) values (@pay_id, @account,dateadd(ss, @present_time, getdate()), @method, @duration)
			delete user_pay_reserve where pay_id=@pay_id and account=@account
			set @old_pay_id = @pay_id
			set @present_time = @present_time+@duration
			update user_time set present_time=@present_time  where account=@account and flag=1
			update user_account set pay_stat=@method where account=@account
		end
		else
			goto exit_trigger
		
	end
	else
	begin
		delete user_time where account=@account and flag=1
		delete user_pay where account=@account
		update pay_master set play_end=getdate(), real_end_date=getdate(), loc=3 where pay_id=@old_pay_id
		update user_account set pay_stat=0 where account=@account
		break
	end
end
exit_trigger:
	return
GO
