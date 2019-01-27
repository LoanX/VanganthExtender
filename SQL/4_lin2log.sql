USE [lin2log]
GO

CREATE TABLE [dbo].[log_insert] (
  [log_file] nvarchar(255) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [log_table] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [rowsprocessed] int NOT NULL,
  [log_year] int NOT NULL,
  [log_month] int NOT NULL,
  [log_day] int NOT NULL,
  [log_hour] int NOT NULL,
  [log_ip] int NOT NULL,
  [log_svr] nvarchar(20) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [log_inout] nvarchar(20) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [process_time] int NULL,
  [inserted] int NULL
)
ON [PRIMARY]
GO

CREATE PROCEDURE [dbo].[lin_BulkInsert]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CheckLogTimeTable2]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DropLogTable]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetWorldSnap]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_MakeChatLogTable]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_MakeItemLogTable]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_MakeLogTable]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetInserted]

AS
SELECT 0
GO

ALTER PROCEDURE dbo.lin_BulkInsert 
(
	@log_table	varchar(512),
	@log_file	varchar(128)
)
AS

set nocount on

declare @sql varchar(1024)

set @sql = ' set nocount on'
	+ ' BULK INSERT ' + @log_table + ' FROM ''' + @log_file + ''' WITH ( MAXERRORS = 65535, FIELDTERMINATOR = '','',  ROWTERMINATOR = ''\n'' ) '
	+ ' select @@ROWCOUNT  ' 
exec ( @sql )
GO

/********************************************
lin_CheckLogTimeTable2
	do check whether real time log table is exist or not
INPUT
	@strDate varchar(16),
	@nWorld  int
OUTPUT
	
return
made by
	young
date
	2002-11-11
********************************************/
ALTER PROCEDURE [DBO].[lin_CheckLogTimeTable2]
(
	@strDate varchar(16),
	@nWorld  int
)
AS
SET NOCOUNT ON

declare @table_name varchar(60)
declare @table2_name varchar(60)
declare @view_name varchar(60)
declare @sql varchar(2048)

-- check log_realtime
set @table_name = @strDate + 'log_realtime_' + cast (@nWorld as varchar)
set @sql = 'select * from sysobjects (nolock) where name = '''+ @table_name + ''''

exec ( @sql)
if ( @@ROWCOUNT = 0) 
begin
	set @sql = 'exec lin_MakeLogTable ''' + @table_name + ''''
	exec (@sql)
end

-- check log_audit
set @table_name = @strDate + 'log_audit_' + cast (@nWorld as varchar)
set @sql = 'select * from sysobjects (nolock) where name = '''+ @table_name + ''''
exec ( @sql)
if ( @@ROWCOUNT = 0) 
begin
	set @sql = 'exec lin_MakeLogTable ''' + @table_name + ''''
	exec (@sql)
end

-- check log_data ( store 0~12 hour log)
set @table_name = @strDate + 'log_data_' + cast (@nWorld as varchar)
set @sql = 'select * from sysobjects (nolock) where name = '''+ @table_name + ''''
exec ( @sql)
if ( @@ROWCOUNT = 0) 
begin
	set @sql = 'exec lin_MakeLogTable ''' + @table_name + ''''
	exec (@sql)
end

-- check log_data2 ( store 12~24 hour log)
set @table2_name = @strDate + 'log_data2_' + cast (@nWorld as varchar)
set @sql = 'select * from sysobjects (nolock) where name = '''+ @table2_name + ''''
exec ( @sql)
if ( @@ROWCOUNT = 0) 
begin
	set @sql = 'exec lin_MakeLogTable ''' + @table2_name + ''''
	exec (@sql)
end

-- check log_data0 ( view )
set @view_name = @strDate + 'log_data0_' + cast (@nWorld as varchar)
set @sql = 'select * from sysobjects (nolock) where name = '''+ @view_name + ''''
exec ( @sql)
if ( @@ROWCOUNT = 0) 
begin
--	set @sql = 'exec lin_MakeLogTable ''' + @table_name + ''''
	set @sql = 'CREATE VIEW dbo.' + @view_name + '  ( ' 
		+ ' act_time,  log_id, actor, actor_account, target, target_account, location_x, location_y, location_z, ' 
		+ 'etc_str1, etc_str2, etc_str3,  ' 
		+ 'etc_num1, etc_num2, etc_num3, etc_num4, etc_num5, etc_num6, etc_num7, etc_num8, etc_num9, etc_num10, ' 
		+ 'STR_actor, STR_actor_account, STR_target, STR_target_account, item_id ' 
		+ ' ) AS ' 
		+ ' SELECT  act_time,  log_id, actor, actor_account, target, target_account, location_x, location_y, location_z, '
		+ ' etc_str1, etc_str2, etc_str3,  ' 
		+ ' etc_num1, etc_num2, etc_num3, etc_num4, etc_num5, etc_num6, etc_num7, etc_num8, etc_num9, etc_num10, '
		+ ' STR_actor, STR_actor_account, STR_target, STR_target_account, item_id ' 
		+ ' from   ' + @table_name  + '  (nolock) UNION '
		+ ' SELECT  act_time,  log_id, actor, actor_account, target, target_account, location_x, location_y, location_z, '
		+ ' etc_str1, etc_str2, etc_str3,  '
		+ ' etc_num1, etc_num2, etc_num3, etc_num4, etc_num5, etc_num6, etc_num7, etc_num8, etc_num9, etc_num10, '
		+ ' STR_actor, STR_actor_account, STR_target, STR_target_account, item_id '
		+ ' from   ' + @table2_name + '  (nolock)  '
	exec (@sql)
end

-- check chat 
set @table_name = @strDate + 'log_chat_' + cast (@nWorld as varchar)
set @sql = 'select * from sysobjects (nolock) where name = '''+ @table_name + ''''
exec ( @sql)
if ( @@ROWCOUNT = 0) 
begin
	set @sql = 'exec lin_MakeChatLogTable ''' + @table_name + ''''
	exec (@sql)
end
GO

ALTER PROCEDURE dbo.lin_DropLogTable
	@drop_date	datetime,
	@drop_world	int
AS

if @drop_date is null
begin
	set @drop_date = getdate()
	set @drop_date = dateadd(d, -4, getdate())
end

DECLARE @nyear int
DECLARE @nmonth int
DECLARE @nday int
DECLARE @stryear varchar(10)
DECLARE @strmonth varchar(10)
DECLARE @strday varchar(10)
DECLARE @str_report varchar(32)
DECLARE @logdate int

set @nyear = datepart(yyyy, @drop_date)
set @nmonth = datepart(mm, @drop_date)
set @nday = datepart(dd, @drop_date)

set @stryear = cast(@nyear as varchar)
if @nmonth < 10
	set @strmonth = '0' + cast(@nmonth as varchar)
else
	set @strmonth = cast (@nmonth as varchar)

if @nday < 10
	set @strday = '0' + cast(@nday as varchar)
else
	set @strday = cast (@nday as varchar)	

set @str_report = @stryear + '/' + @strmonth + '/' + @strday
set @logdate = cast(@stryear + @strmonth + @strday as int)

------------- now.. we have year, month, day string
DECLARE @table_from varchar(60)
declare @sql varchar (1024)


set @table_from = 'L' + @stryear  + '_' + @strmonth + '_' + @strday + '_log_data0_' + cast ( @drop_world as varchar)
set @sql = ' drop view ' + @table_from

exec (@sql)


set @table_from = 'L' + @stryear  + '_' + @strmonth + '_' + @strday + '_log_data_' + cast ( @drop_world as varchar)
set @sql = ' drop table ' + @table_from

exec (@sql)


set @table_from = 'L' + @stryear  + '_' + @strmonth + '_' + @strday + '_log_data2_' + cast ( @drop_world as varchar)
set @sql = ' drop table ' + @table_from

exec (@sql)

/*
set @table_from = 'L' + @stryear  + '_' + @strmonth + '_' + @strday + '_log_chat_' + cast ( @drop_world as varchar)
set @sql = ' drop table ' + @table_from

exec (@sql)
*/
GO

/******************************************************************************
#Name:	lin_GetWorldSnap
#Desc:	do make snap shot table of user_data, user_item, pledge, user_nobless

#Argument:
	Input:	@db_server	varchar(30)	server name
		@user_id	varchar(30)	login id
		@user_pass	varchar(30)	password
		@world_id	int		world id
		@dtnow		varchar(8)	yyyyMMdd
	Output:	
#Return:
#Result Set:

#Remark:
#Example:	exec lin_GetWorldSnap 'l2db2', 'gamma', '********', 8
#See:

#History:
	Create	flagoftiger	2004-06-14
	Modify	btwinuni	2005-05-16	add: pledge
	Modify	btwinuni	2005-09-29	add: user_nobless
	Modify	btwinuni	2005-10-27	add parameter: dtnow
******************************************************************************/
ALTER PROCEDURE [DBO].[lin_GetWorldSnap]
	@db_server	varchar(30),
	@user_id	varchar(30),
	@user_pass	varchar(30),
	@world_id	int,
	@dtnow		varchar(8) = ''
AS
SET NOCOUNT ON
SET ANSI_WARNINGS ON
SET ANSI_NULLS ON

declare @tmp_user_item nvarchar(50)
declare @tmp_user_data nvarchar(50)
declare @tmp_pledge nvarchar(50)
declare @tmp_user_nobless nvarchar(50)
declare @drop_user_item nvarchar(50)
declare @drop_user_data nvarchar(50)
declare @drop_pledge nvarchar(50)
declare @drop_user_nobless nvarchar(50)
declare @dtnow2 nvarchar(8)
declare @sql varchar(4000)

if @dtnow = ''
begin
	set @dtnow = convert(varchar, getdate(), 112)
end

set @dtnow2 = convert(varchar, DATEADD(d, -2, cast(@dtnow as datetime)), 112)

-- set table name
set @tmp_user_item = 'S' + left(@dtnow,4) + '_' + substring(@dtnow,5,2) + '_' + right(@dtnow,2) + '_snap_item_' + cast(@world_id as varchar)
set @tmp_user_data = 'S' + left(@dtnow,4) + '_' + substring(@dtnow,5,2) + '_' + right(@dtnow,2) + '_snap_data_' + cast(@world_id as varchar)
set @tmp_pledge = 'S' + left(@dtnow,4) + '_' + substring(@dtnow,5,2) + '_' + right(@dtnow,2) + '_snap_pledge_' + cast(@world_id as varchar)
set @tmp_user_nobless = 'S' + left(@dtnow,4) + '_' + substring(@dtnow,5,2) + '_' + right(@dtnow,2) + '_snap_nobless_' + cast(@world_id as varchar)

-- set drop table name
set @drop_user_item = 'S' + left(@dtnow2,4) + '_' + substring(@dtnow2,5,2) + '_' + right(@dtnow2,2) + '_snap_item_' + cast(@world_id as varchar)
set @drop_user_data = 'S' + left(@dtnow2,4) + '_' + substring(@dtnow2,5,2) + '_' + right(@dtnow2,2) + '_snap_data_' + cast(@world_id as varchar)
set @drop_pledge = 'S' + left(@dtnow2,4) + '_' + substring(@dtnow2,5,2) + '_' + right(@dtnow2,2) + '_snap_pledge_' + cast(@world_id as varchar)
set @drop_user_nobless = 'S' + left(@dtnow2,4) + '_' + substring(@dtnow2,5,2) + '_' + right(@dtnow2,2) + '_snap_nobless_' + cast(@world_id as varchar)


--------------------------------------------------------------------------------------------------------------------------------------------
-- user_item snap shot
--------------------------------------------------------------------------------------------------------------------------------------------

-- check table whether @drop_user_item is exists or not
set @sql = 'if exists (select * from dbo.sysobjects where id = object_id(N''[dbo].[' + @drop_user_item + ']'') and objectproperty(id, N''IsUserTable'') = 1)'
	+ ' begin'
	+ ' drop table dbo.' + @drop_user_item
	+ ' end'
exec (@sql)

-- check table whether @tmp_user_item is exists or not
set @sql = 'if exists (select * from dbo.sysobjects where id = object_id(N''[dbo].[' + @tmp_user_item + ']'') and objectproperty(id, N''IsUserTable'') = 1)'
	+ ' begin'
	+ ' drop table dbo.' + @tmp_user_item
	+ ' end'
exec (@sql)


set @sql = ' select * into dbo.' + @tmp_user_item
	+ ' from OPENROWSET ( ''SQLOLEDB'', ''' + @db_server + ''';''' + @user_id + ''';''' + @user_pass  + ''',  ''select * from lin2world.dbo.tmp_user_item (nolock) where char_id > 0 '') '
exec (@sql )

set @sql = 'CREATE CLUSTERED INDEX IX_' + @tmp_user_item + '_1 on dbo.' + @tmp_user_item + ' (char_id asc, item_type asc, enchant desc ) with fillfactor = 90 '
exec (@sql)
set @sql = 'CREATE NONCLUSTERED INDEX IX_' + @tmp_user_item + '_2 on dbo.' + @tmp_user_item + ' (item_type asc, enchant desc ) with fillfactor = 90 '
exec (@sql)



--------------------------------------------------------------------------------------------------------------------------------------------
-- user_data snap shot
--------------------------------------------------------------------------------------------------------------------------------------------

-- check table whether @drop_user_data is exists or not
set @sql = 'if exists (select * from dbo.sysobjects where id = object_id(N''[dbo].[' + @drop_user_data + ']'') and objectproperty(id, N''IsUserTable'') = 1)'
	+ ' begin'
	+ ' drop table dbo.' + @drop_user_data
	+ ' end'
exec (@sql)

-- check table whether 'tmp_user_data' is exists or not
set @sql = 'if exists (select * from dbo.sysobjects where id = object_id(N''[dbo].[' + @tmp_user_data + ']'') and objectproperty(id, N''IsUserTable'') = 1)'
	+ ' begin'
	+ ' drop table dbo.' + @tmp_user_data
	+ ' end'
exec (@sql)


set @sql = ' select * into dbo.' + @tmp_user_data
	+ ' from OPENROWSET ( ''SQLOLEDB'', ''' + @db_server + ''';''' + @user_id + ''';''' + @user_pass  + ''',  ''select * from lin2world.dbo.tmp_user_data (nolock)'') '
exec (@sql )

set @sql = 'CREATE CLUSTERED INDEX IX_' + @tmp_user_data + '_1 on dbo.' + @tmp_user_data + ' (exp desc) with fillfactor = 90 '
exec (@sql )
set @sql = 'CREATE NONCLUSTERED INDEX IX_' + @tmp_user_data + '_2 on dbo.' + @tmp_user_data + ' (race asc, exp desc) with fillfactor = 90 '
exec (@sql )
set @sql = 'CREATE NONCLUSTERED INDEX IX_' + @tmp_user_data + '_3 on dbo.' + @tmp_user_data + ' (class asc, exp desc) with fillfactor = 90 '
exec (@sql )



--------------------------------------------------------------------------------------------------------------------------------------------
-- pledge snap shot
--------------------------------------------------------------------------------------------------------------------------------------------

-- check table whether @drop_pledge is exists or not
set @sql = 'if exists (select * from dbo.sysobjects where id = object_id(N''[dbo].[' + @drop_pledge + ']'') and objectproperty(id, N''IsUserTable'') = 1)'
	+ ' begin'
	+ ' drop table dbo.' + @drop_pledge
	+ ' end'
exec (@sql)

-- check table whether '@tmp_pledge' is exists or not
set @sql = 'if exists (select * from dbo.sysobjects where id = object_id(N''[dbo].[' + @tmp_pledge + ']'') and objectproperty(id, N''IsUserTable'') = 1)'
	+ ' begin'
	+ ' drop table dbo.' + @tmp_pledge
	+ ' end'
exec (@sql)


set @sql = ' select * into dbo.' + @tmp_pledge
	+ ' from OPENROWSET ( ''SQLOLEDB'', ''' + @db_server + ''';''' + @user_id + ''';''' + @user_pass  + ''',  ''select * from lin2world.dbo.tmp_pledge (nolock)'') '
exec (@sql )

set @sql = 'CREATE CLUSTERED INDEX IX_' + @tmp_pledge + '_1 on dbo.' + @tmp_pledge + ' (pledge_id) with fillfactor = 90 '
exec (@sql )


--------------------------------------------------------------------------------------------------------------------------------------------
-- user_nobless snap shot
--------------------------------------------------------------------------------------------------------------------------------------------

-- check table whether @drop_user_nobless is exists or not
set @sql = 'if exists (select * from dbo.sysobjects where id = object_id(N''[dbo].[' + @drop_user_nobless + ']'') and objectproperty(id, N''IsUserTable'') = 1)'
	+ ' begin'
	+ ' drop table dbo.' + @drop_user_nobless
	+ ' end'
exec (@sql)

-- check table whether '@tmp_user_nobless' is exists or not
set @sql = 'if exists (select * from dbo.sysobjects where id = object_id(N''[dbo].[' + @tmp_user_nobless + ']'') and objectproperty(id, N''IsUserTable'') = 1)'
	+ ' begin'
	+ ' drop table dbo.' + @tmp_user_nobless
	+ ' end'
exec (@sql)


set @sql = ' select * into dbo.' + @tmp_user_nobless
	+ ' from OPENROWSET ( ''SQLOLEDB'', ''' + @db_server + ''';''' + @user_id + ''';''' + @user_pass  + ''',  ''select * from lin2world.dbo.tmp_user_nobless (nolock)'') '
exec (@sql )

set @sql = 'CREATE CLUSTERED INDEX IX_' + @tmp_user_nobless + '_1 on dbo.' + @tmp_user_nobless + ' (char_id) with fillfactor = 90 '
exec (@sql )
GO

/********************************************
lin_MakeChatLogTable
	do make whether log table is exist or not
INPUT
	@table_name varchar(60)
OUTPUT
	
return
made by
	young
date
	2003-09-19
********************************************/
ALTER PROCEDURE [DBO].[lin_MakeChatLogTable]
(
	@table_name varchar(60)
)
AS
SET NOCOUNT ON

declare @sql varchar(1024)

set @sql = 'CREATE TABLE dbo.' + @table_name + ' (' 
	+ 'act_time  datetime NULL ,' 
	+ 'log_id  smallint NULL ,' 
	+ 'actor  int  NULL , ' 
	+ 'target  int NULL , ' 
	+ 'location_x  int NULL , ' 
	+ 'location_y  int NULL , ' 
	+ 'location_z  int NULL , ' 
	+ 'say varchar (256) NULL , ' 
	+ 'STR_actor  varchar (32) NULL , ' 
	+ 'STR_target  varchar (32) NULL  '
 	+ ' )'

exec (@sql)

set @sql = 'CREATE CLUSTERED INDEX IX_' + @table_name + '_ACTOR on dbo.' + @table_name + ' (log_id, actor )   '
exec (@sql)

set @sql = 'CREATE NONCLUSTERED INDEX IX_' + @table_name + '_TARGET on dbo.' + @table_name + ' ( log_id, target )   '
exec (@sql)
GO

/********************************************
lin_MakeItemLogTable
	do make whether log table is exist or not
INPUT
	@table_name varchar(60)
OUTPUT
	
return
made by
	young
date
	2002-11-11
********************************************/
ALTER PROCEDURE [DBO].[lin_MakeItemLogTable]
(
	@table_name varchar(60)
)
AS
SET NOCOUNT ON

declare @sql varchar(1024)

set @sql = 'CREATE TABLE dbo.' + @table_name + ' (' 
	+ 'act_time  datetime NULL ,' 
	+ 'log_id  smallint NULL ,' 
	+ 'item_id  int NULL , ' 
	+ 'item_type  int NULL , ' 
	+ 'actor  varchar(32)  NULL , ' 
	+ 'actor_account  varchar(32)  NULL , ' 
	+ 'target  varchar(32)  NULL , ' 
	+ 'target_account  varchar(32)  NULL , ' 
	+ 'location_x  int NULL , ' 
	+ 'location_y  int NULL , ' 
	+ 'location_z  int NULL , ' 
	+ 'etc_num1  int NULL , ' 
	+ 'etc_num2  int NULL , ' 
	+ 'etc_num3  int NULL , ' 
	+ 'etc_num4  int NULL , ' 
	+ 'etc_num5  int NULL , ' 
	+ 'etc_num6  int NULL , ' 
	+ 'etc_num7  int NULL , ' 
	+ 'etc_num8  int NULL  ' 
	+ ' )'

exec (@sql)


set @sql = 'CREATE INDEX IX_' + @table_name + '_1 on dbo.' + @table_name + ' (log_id) '
exec (@sql)

set @sql = 'CREATE INDEX IX_' + @table_name + '_2 on dbo.' + @table_name + ' (item_id)  '
exec (@sql)

set @sql = 'CREATE INDEX IX_' + @table_name + '_3 on dbo.' + @table_name + ' (item_type) '
exec (@sql)
GO

/********************************************
lin_MakeLogTable
	do make whether log table is exist or not
INPUT
	@table_name varchar(60)
OUTPUT
	
return
made by
	young
date
	2002-11-11
********************************************/
ALTER PROCEDURE [DBO].[lin_MakeLogTable]
(
	@table_name varchar(60)
)
AS
SET NOCOUNT ON

declare @sql varchar(1024)

set @sql = 'CREATE TABLE dbo.' + @table_name + ' (' 
	+ 'act_time  datetime NULL ,' 
	+ 'log_id  smallint NULL ,' 
	+ 'actor  int  NULL , ' 
	+ 'actor_account  int  NULL , ' 
	+ 'target  int NULL , ' 
	+ 'target_account  int NULL , ' 
	+ 'location_x  int NULL , ' 
	+ 'location_y  int NULL , ' 
	+ 'location_z  int NULL , ' 
	+ 'etc_str1  varchar (200) NULL , ' 
	+ 'etc_str2  varchar (50) NULL , ' 
	+ 'etc_str3  varchar (50) NULL , ' 
	+ 'etc_num1  float NULL , ' 
	+ 'etc_num2  float NULL , ' 
	+ 'etc_num3  int NULL , ' 
	+ 'etc_num4  int NULL , ' 
	+ 'etc_num5  int NULL , ' 
	+ 'etc_num6  int NULL , ' 
	+ 'etc_num7  int NULL , ' 
	+ 'etc_num8  int NULL , ' 
	+ 'etc_num9  int NULL , ' 
	+ 'etc_num10  int NULL,'
	+ 'STR_actor  varchar (48) NULL , ' 
	+ 'STR_actor_account  varchar (32) NULL , ' 
	+ 'STR_target  varchar (48) NULL , ' 
	+ 'STR_target_account  varchar (32) NULL,  ' 
	+ 'item_id int NULL'
	+ ' )'

exec (@sql)

set @sql = 'CREATE CLUSTERED INDEX IX_' + @table_name + '_ACTOR on dbo.' + @table_name + ' ( log_id, actor ) WITH FILLFACTOR = 90 ON [PRIMARY]  '
exec (@sql)

set @sql = 'CREATE NONCLUSTERED INDEX IX_' + @table_name + '_ITEMTYPE on dbo.' + @table_name + ' ( actor , log_id   ) WITH FILLFACTOR = 90 ON [PRIMARY]  '
exec (@sql)

set @sql = 'CREATE NONCLUSTERED INDEX IX_' + @table_name + '_ITEMID on dbo.' + @table_name + ' ( item_id ) WITH FILLFACTOR = 90 ON [PRIMARY]  '
exec (@sql)
GO

/********************************************
lin_SetInserted
	insert or update log file as inserted
INPUT
	@log_file	nvarchar(255),
	@log_table	nvarchar(50),
	@log_year	int,
	@log_month	int,
	@log_day	int,
	@log_hour	int,
	@log_ip		int,
	@log_svr	nvarchar(20),
	@log_inout	nvarchar(20),
	@rowsprocessed int
OUTPUT

return
made by
	young
date
	2002-10-14
********************************************/
ALTER PROCEDURE [DBO].[lin_SetInserted]
(
	@log_file	nvarchar(255),
	@log_table	nvarchar(50),
	@log_year	int,
	@log_month	int,
	@log_day	int,
	@log_hour	int,
	@log_ip		int,
	@log_svr	nvarchar(20),
	@log_inout	nvarchar(20),
	@rowsprocessed int,
	@process_time int  = 0
)
AS
SET NOCOUNT ON

insert into log_insert( log_file, log_table, rowsprocessed, log_year, log_month, log_day, log_hour, log_ip, log_svr, log_inout, inserted, process_time )
values
(@log_file, @log_table, @rowsprocessed, @log_year, @log_month, @log_day, @log_hour, @log_ip, @log_svr, @log_inout, 1, @process_time )
GO

