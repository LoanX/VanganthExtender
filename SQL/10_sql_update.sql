use lin2world
go

ALTER  TABLE user_data add 
	ST_hair_deco int NOT NULL DEFAULT 0,
	ST_hair_all int NOT NULL DEFAULT 0
go

IF EXISTS (SELECT 1 
    FROM sys.procedures
    WHERE [name]='lin_AddKillDeathStat') 
        drop procedure lin_AddKillDeathStat
go

IF EXISTS (SELECT 1 
    FROM INFORMATION_SCHEMA.TABLES 
    WHERE TABLE_NAME='KillDeathStat') 
        drop table KillDeathStat
go

create table KillDeathStat(
	victim_id int not null,
	victim_class_id int not null default 0,
	victim_level int not null default 0,
	killer_id int not null,
	killer_class_id int not null default 0,
	killer_level int not null default 0,
	killer_hp int not null default 0,
	killer_max_hp int not null default 0,
	killer_cp int not null default 0,
	killer_max_cp int not null default 0,
	x int not null default 0,
	y int not null default 0,
	z int not null default 0,
	pvp int not null default 0,
	skill_id int not null default 0,
	skill_level int not null default 0,
	death_time int not null default 0
	)
GO

CREATE PROCEDURE lin_AddKillDeathStat
	@victim_id AS INT,
	@victim_class_id AS INT,
	@victim_level AS INT,
	@killer_id AS INT,
	@killer_class_id AS INT,
	@killer_level AS INT,
	@killer_hp AS INT,
	@killer_max_hp AS INT,
	@killer_cp AS INT,
	@killer_max_cp AS INT,
	@x AS INT,
	@y AS INT,
	@z AS INT,
	@pvp AS INT,
	@skill_id AS INT,
	@skill_level AS INT,
	@death_time AS INT
AS
BEGIN

	SET NOCOUNT ON;

	insert into KillDeathStat( victim_id, victim_class_id, victim_level, killer_id, killer_class_id, killer_level, killer_hp, killer_max_hp, killer_cp, killer_max_cp, x, y, z, pvp, skill_id, skill_level, death_time)
	values ( @victim_id, @victim_class_id, @victim_level, @killer_id, @killer_class_id, @killer_level, @killer_hp, @killer_max_hp, @killer_cp, @killer_max_cp, @x, @y, @z, @pvp, @skill_id, @skill_level, @death_time )

END
GO

use lin2world
go

create table PrivateStore(
	char_id int not null,
	store_type int null,
	x_loc int null,
	y_loc int null,
	z_loc int null,
	is_offline int null,
	item1_id int null,
	item1_count int null,
	item1_price int null,
	item1_enchant int null,
	item2_id int null,
	item2_count int null,
	item2_price int null,
	item2_enchant int null,
	item3_id int null,
	item3_count int null,
	item3_price int null,
	item3_enchant int null,
	item4_id int null,
	item4_count int null,
	item4_price int null,
	item4_enchant int null,
	item5_id int null,
	item5_count int null,
	item5_price int null,
	item5_enchant int null,
	item6_id int null,
	item6_count int null,
	item6_price int null,
	item6_enchant int null,
	item7_id int null,
	item7_count int null,
	item7_price int null,
	item7_enchant int null,
	item8_id int null,
	item8_count int null,
	item8_price int null,
	item8_enchant int null
	)
go

USE [lin2world] 
GO
CREATE PROCEDURE lin_UpdatePrivateStore
	@char_id AS INT,
	@store_type AS INT,
	@x_loc AS INT,
	@y_loc AS INT,
	@z_loc AS INT,
	@is_offline AS INT,
	@item1_id AS INT,
	@item1_count AS INT,
	@item1_price AS INT,
	@item1_enchant AS INT,
	@item2_id AS INT,
	@item2_count AS INT,
	@item2_price AS INT,
	@item2_enchant AS INT,
	@item3_id AS INT,
	@item3_count AS INT,
	@item3_price AS INT,
	@item3_enchant AS INT,
	@item4_id AS INT,
	@item4_count AS INT,
	@item4_price AS INT,
	@item4_enchant AS INT,
	@item5_id AS INT,
	@item5_count AS INT,
	@item5_price AS INT,
	@item5_enchant AS INT,
	@item6_id AS INT,
	@item6_count AS INT,
	@item6_price AS INT,
	@item6_enchant AS INT,
	@item7_id AS INT,
	@item7_count AS INT,
	@item7_price AS INT,
	@item7_enchant AS INT,
	@item8_id AS INT,
	@item8_count AS INT,
	@item8_price AS INT,
	@item8_enchant AS INT

AS
BEGIN

	SET NOCOUNT ON;

	update PrivateStore set store_type=@store_type, x_loc=@x_loc, y_loc=@y_loc, z_loc=@z_loc, is_offline = @is_offline, 
	@item1_id=@item1_id, item1_count=@item1_count, item1_price=@item1_price, item1_enchant=@item1_enchant,
	@item2_id=@item2_id, item2_count=@item2_count, item2_price=@item2_price, item2_enchant=@item2_enchant,
	@item3_id=@item3_id, item3_count=@item3_count, item3_price=@item3_price, item3_enchant=@item3_enchant,
	@item4_id=@item4_id, item4_count=@item4_count, item4_price=@item4_price, item4_enchant=@item4_enchant,
	@item5_id=@item5_id, item5_count=@item5_count, item5_price=@item5_price, item5_enchant=@item5_enchant,
	@item6_id=@item6_id, item6_count=@item6_count, item6_price=@item6_price, item6_enchant=@item6_enchant,
	@item7_id=@item7_id, item7_count=@item7_count, item7_price=@item7_price, item7_enchant=@item7_enchant,
	@item8_id=@item8_id, item8_count=@item8_count, item8_price=@item8_price, item8_enchant=@item8_enchant where char_id = @char_id
	
	

END
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
USE [lin2world] 
GO
CREATE PROCEDURE lin_DeletePrivateStore
	@char_id AS INT
AS
BEGIN

	SET NOCOUNT ON;

	delete from PrivateStore where char_id = @char_id

END
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
USE [lin2world] 
GO
CREATE PROCEDURE lin_CreatePrivateStore
	@char_id AS INT,
	@store_type AS INT,
	@x_loc AS INT,
	@y_loc AS INT,
	@z_loc AS INT,
	@is_offline AS INT,
	@item1_id AS INT,
	@item1_count AS INT,
	@item1_price AS INT,
	@item1_enchant AS INT,
	@item2_id AS INT,
	@item2_count AS INT,
	@item2_price AS INT,
	@item2_enchant AS INT,
	@item3_id AS INT,
	@item3_count AS INT,
	@item3_price AS INT,
	@item3_enchant AS INT,
	@item4_id AS INT,
	@item4_count AS INT,
	@item4_price AS INT,
	@item4_enchant AS INT,
	@item5_id AS INT,
	@item5_count AS INT,
	@item5_price AS INT,
	@item5_enchant AS INT,
	@item6_id AS INT,
	@item6_count AS INT,
	@item6_price AS INT,
	@item6_enchant AS INT,
	@item7_id AS INT,
	@item7_count AS INT,
	@item7_price AS INT,
	@item7_enchant AS INT,
	@item8_id AS INT,
	@item8_count AS INT,
	@item8_price AS INT,
	@item8_enchant AS INT
AS
BEGIN

	SET NOCOUNT ON;

	insert into PrivateStore( char_id, store_type, x_loc, y_loc, z_loc, is_offline, 
	item1_id, item1_count, item1_price, item1_enchant,
	item2_id, item2_count, item2_price, item2_enchant,
	item3_id, item3_count, item3_price, item3_enchant,
	item4_id, item4_count, item4_price, item4_enchant,
	item5_id, item5_count, item5_price, item5_enchant,
	item6_id, item6_count, item6_price, item6_enchant,
	item7_id, item7_count, item7_price, item7_enchant,
	item8_id, item8_count, item8_price, item8_enchant ) 
	values ( @char_id, @store_type, @x_loc, @y_loc, @z_loc, @is_offline, 
	@item1_id, @item1_count, @item1_price, @item1_enchant,
	@item2_id, @item2_count, @item2_price, @item2_enchant,
	@item3_id, @item3_count, @item3_price, @item3_enchant,
	@item4_id, @item4_count, @item4_price, @item4_enchant,
	@item5_id, @item5_count, @item5_price, @item5_enchant,
	@item6_id, @item6_count, @item6_price, @item6_enchant,
	@item7_id, @item7_count, @item7_price, @item7_enchant,
	@item8_id, @item8_count, @item8_price, @item8_enchant )

END
GO

USE [lin2world]
GO

ALTER TABLE dbo.user_data ADD
	spirit_count int NOT NULL DEFAULT 0
GO

USE [LIN2WORLD]
GO
/****** Object:  Table [dbo].[user_ActiveSkill]    Script Date: 02/01/2009 23:21:34 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
DROP TABLE [dbo].[user_ActiveSkill]
GO
CREATE TABLE [dbo].[user_ActiveSkill](
	[char_id] [int] NOT NULL,
	[s1] [int] NULL,
	[l1] [tinyint] NULL,
	[d1] [int] NULL,
	[s2] [int] NULL,
	[l2] [tinyint] NULL,
	[d2] [int] NULL,
	[s3] [int] NULL,
	[l3] [tinyint] NULL,
	[d3] [int] NULL,
	[s4] [int] NULL,
	[l4] [tinyint] NULL,
	[d4] [int] NULL,
	[s5] [int] NULL,
	[l5] [tinyint] NULL,
	[d5] [int] NULL,
	[s6] [int] NULL,
	[l6] [tinyint] NULL,
	[d6] [int] NULL,
	[s7] [int] NULL,
	[l7] [tinyint] NULL,
	[d7] [int] NULL,
	[s8] [int] NULL,
	[l8] [tinyint] NULL,
	[d8] [int] NULL,
	[s9] [int] NULL,
	[l9] [tinyint] NULL,
	[d9] [int] NULL,
	[s10] [int] NULL,
	[l10] [tinyint] NULL,
	[d10] [int] NULL,
	[s11] [int] NULL,
	[l11] [tinyint] NULL,
	[d11] [int] NULL,
	[s12] [int] NULL,
	[l12] [tinyint] NULL,
	[d12] [int] NULL,
	[s13] [int] NULL,
	[l13] [tinyint] NULL,
	[d13] [int] NULL,
	[s14] [int] NULL,
	[l14] [tinyint] NULL,
	[d14] [int] NULL,
	[s15] [int] NULL,
	[l15] [tinyint] NULL,
	[d15] [int] NULL,
	[s16] [int] NULL,
	[l16] [tinyint] NULL,
	[d16] [int] NULL,
	[s17] [int] NULL,
	[l17] [tinyint] NULL,
	[d17] [int] NULL,
	[s18] [int] NULL,
	[l18] [tinyint] NULL,
	[d18] [int] NULL,
	[s19] [int] NULL,
	[l19] [tinyint] NULL,
	[d19] [int] NULL,
	[s20] [int] NULL,
	[l20] [tinyint] NULL,
	[d20] [int] NULL,
	[s21] [int] NULL,
	[l21] [tinyint] NULL,
	[d21] [int] NULL,
	[s22] [int] NULL,
	[l22] [tinyint] NULL,
	[d22] [int] NULL,
	[s23] [int] NULL,
	[l23] [tinyint] NULL,
	[d23] [int] NULL,
	[s24] [int] NULL,
	[l24] [tinyint] NULL,
	[d24] [int] NULL,
	[s25] [int] NULL,
	[l25] [tinyint] NULL,
	[d25] [int] NULL,
	[s26] [int] NULL,
	[l26] [tinyint] NULL,
	[d26] [int] NULL,
	[s27] [int] NULL,
	[l27] [tinyint] NULL,
	[d27] [int] NULL,
	[s28] [int] NULL,
	[l28] [tinyint] NULL,
	[d28] [int] NULL,
	[s29] [int] NULL,
	[l29] [tinyint] NULL,
	[d29] [int] NULL,
	[s30] [int] NULL,
	[l30] [tinyint] NULL,
	[d30] [int] NULL,
	[s31] [int] NULL,
	[l31] [tinyint] NULL,
	[d31] [int] NULL,
	[s32] [int] NULL,
	[l32] [tinyint] NULL,
	[d32] [int] NULL,
	[s33] [int] NULL,
	[l33] [tinyint] NULL,
	[d33] [int] NULL,
	[s34] [int] NULL,
	[l34] [tinyint] NULL,
	[d34] [int] NULL,
	[s35] [int] NULL,
	[l35] [tinyint] NULL,
	[d35] [int] NULL,
	[s36] [int] NULL,
	[l36] [tinyint] NULL,
	[d36] [int] NULL,
	[s37] [int] NULL,
	[l37] [tinyint] NULL,
	[d37] [int] NULL,
	[s38] [int] NULL,
	[l38] [tinyint] NULL,
	[d38] [int] NULL,
	[s39] [int] NULL,
	[l39] [tinyint] NULL,
	[d39] [int] NULL,
	[s40] [int] NULL,
	[l40] [tinyint] NULL,
	[d40] [int] NULL,
	[c1] [tinyint] NULL,
	[c2] [tinyint] NULL,
	[c3] [tinyint] NULL,
	[c4] [tinyint] NULL,
	[c5] [tinyint] NULL,
	[c6] [tinyint] NULL,
	[c7] [tinyint] NULL,
	[c8] [tinyint] NULL,
	[c9] [tinyint] NULL,
	[c10] [tinyint] NULL,
	[c11] [tinyint] NULL,
	[c12] [tinyint] NULL,
	[c13] [tinyint] NULL,
	[c14] [tinyint] NULL,
	[c15] [tinyint] NULL,
	[c16] [tinyint] NULL,
	[c17] [tinyint] NULL,
	[c18] [tinyint] NULL,
	[c19] [tinyint] NULL,
	[c20] [tinyint] NULL,
	[c21] [tinyint] NULL,
	[c22] [tinyint] NULL,
	[c23] [tinyint] NULL,
	[c24] [tinyint] NULL,
	[c25] [tinyint] NULL,
	[c26] [tinyint] NULL,
	[c27] [tinyint] NULL,
	[c28] [tinyint] NULL,
	[c29] [tinyint] NULL,
	[c30] [tinyint] NULL,
	[c31] [tinyint] NULL,
	[c32] [tinyint] NULL,
	[c33] [tinyint] NULL,
	[c34] [tinyint] NULL,
	[c35] [tinyint] NULL,
	[c36] [tinyint] NULL,
	[c37] [tinyint] NULL,
	[c38] [tinyint] NULL,
	[c39] [tinyint] NULL,
	[c40] [tinyint] NULL
) ON [PRIMARY]

go

USE [lin2world]
GO
drop procedure lin_CreateActiveSkill
go

CREATE PROCEDURE [dbo].[lin_CreateActiveSkill]
(
 @char_id INT,
 @s1 INT, @l1 TINYINT, @d1 INT, @c1 TINYINT,
 @s2 INT, @l2 TINYINT, @d2 INT, @c2 TINYINT,
 @s3 INT, @l3 TINYINT, @d3 INT, @c3 TINYINT,
 @s4 INT, @l4 TINYINT, @d4 INT, @c4 TINYINT,
 @s5 INT, @l5 TINYINT, @d5 INT, @c5 TINYINT,
 @s6 INT, @l6 TINYINT, @d6 INT, @c6 TINYINT,
 @s7 INT, @l7 TINYINT, @d7 INT, @c7 TINYINT,
 @s8 INT, @l8 TINYINT, @d8 INT, @c8 TINYINT,
 @s9 INT, @l9 TINYINT, @d9 INT, @c9 TINYINT,
 @s10 INT, @l10 TINYINT, @d10 INT, @c10 TINYINT,
 @s11 INT, @l11 TINYINT, @d11 INT, @c11 TINYINT,
 @s12 INT, @l12 TINYINT, @d12 INT, @c12 TINYINT,
 @s13 INT, @l13 TINYINT, @d13 INT, @c13 TINYINT,
 @s14 INT, @l14 TINYINT, @d14 INT, @c14 TINYINT,
 @s15 INT, @l15 TINYINT, @d15 INT, @c15 TINYINT,
 @s16 INT, @l16 TINYINT, @d16 INT, @c16 TINYINT,
 @s17 INT, @l17 TINYINT, @d17 INT, @c17 TINYINT,
 @s18 INT, @l18 TINYINT, @d18 INT, @c18 TINYINT,
 @s19 INT, @l19 TINYINT, @d19 INT, @c19 TINYINT,
 @s20 INT, @l20 TINYINT, @d20 INT, @c20 TINYINT,
 @s21 INT, @l21 TINYINT, @d21 INT, @c21 TINYINT,
 @s22 INT, @l22 TINYINT, @d22 INT, @c22 TINYINT,
 @s23 INT, @l23 TINYINT, @d23 INT, @c23 TINYINT,
 @s24 INT, @l24 TINYINT, @d24 INT, @c24 TINYINT,
 @s25 INT, @l25 TINYINT, @d25 INT, @c25 TINYINT,
 @s26 INT, @l26 TINYINT, @d26 INT, @c26 TINYINT,
 @s27 INT, @l27 TINYINT, @d27 INT, @c27 TINYINT,
 @s28 INT, @l28 TINYINT, @d28 INT, @c28 TINYINT,
 @s29 INT, @l29 TINYINT, @d29 INT, @c29 TINYINT,
 @s30 INT, @l30 TINYINT, @d30 INT, @c30 TINYINT,
 @s31 INT, @l31 TINYINT, @d31 INT, @c31 TINYINT,
 @s32 INT, @l32 TINYINT, @d32 INT, @c32 TINYINT,
 @s33 INT, @l33 TINYINT, @d33 INT, @c33 TINYINT,
 @s34 INT, @l34 TINYINT, @d34 INT, @c34 TINYINT,
 @s35 INT, @l35 TINYINT, @d35 INT, @c35 TINYINT,
 @s36 INT, @l36 TINYINT, @d36 INT, @c36 TINYINT,
 @s37 INT, @l37 TINYINT, @d37 INT, @c37 TINYINT,
 @s38 INT, @l38 TINYINT, @d38 INT, @c38 TINYINT,
 @s39 INT, @l39 TINYINT, @d39 INT, @c39 TINYINT,
 @s40 INT, @l40 TINYINT, @d40 INT, @c40 TINYINT
)
AS
SET NOCOUNT ON

 INSERT INTO user_activeskill
 (char_id,
 s1, l1, d1, c1,
 s2, l2, d2, c2,
 s3, l3, d3, c3,
 s4, l4, d4, c4,
 s5, l5, d5, c5,
 s6, l6, d6, c6,
 s7, l7, d7, c7,
 s8, l8, d8, c8,
 s9, l9, d9, c9,
 s10, l10, d10, c10,
 s11, l11, d11, c11,
 s12, l12, d12, c12,
 s13, l13, d13, c13,
 s14, l14, d14, c14,
 s15, l15, d15, c15,
 s16, l16, d16, c16,
 s17, l17, d17, c17,
 s18, l18, d18, c18,
 s19, l19, d19, c19,
 s20, l20, d20, c20,
 s21, l21, d21, c21,
 s22, l22, d22, c22,
 s23, l23, d23, c23,
 s24, l24, d24, c24,
 s25, l25, d25, c25,
 s26, l26, d26, c26,
 s27, l27, d27, c27,
 s28, l28, d28, c28,
 s29, l29, d29, c29,
 s30, l30, d30, c30,
 s31, l31, d31, c31,
 s32, l32, d32, c32,
 s33, l33, d33, c33,
 s34, l34, d34, c34,
 s35, l35, d35, c35,
 s36, l36, d36, c36,
 s37, l37, d37, c37,
 s38, l38, d38, c38,
 s39, l39, d39, c39,
 s40, l40, d40, c40 )
 VALUES
 (@char_id,
 @s1, @l1, @d1, @c1,
 @s2, @l2, @d2, @c2,
 @s3, @l3, @d3, @c3,
 @s4, @l4, @d4, @c4,
 @s5, @l5, @d5, @c5,
 @s6, @l6, @d6, @c6,
 @s7, @l7, @d7, @c7,
 @s8, @l8, @d8, @c8,
 @s9, @l9, @d9, @c9,
 @s10, @l10, @d10, @c10,
 @s11, @l11, @d11, @c11,
 @s12, @l12, @d12, @c12,
 @s13, @l13, @d13, @c13,
 @s14, @l14, @d14, @c14,
 @s15, @l15, @d15, @c15,
 @s16, @l16, @d16, @c16,
 @s17, @l17, @d17, @c17,
 @s18, @l18, @d18, @c18,
 @s19, @l19, @d19, @c19,
 @s20, @l20, @d20, @c20,
 @s21, @l21, @d21, @c21,
 @s22, @l22, @d22, @c22,
 @s23, @l23, @d23, @c23,
 @s24, @l24, @d24, @c24,
 @s25, @l25, @d25, @c25,
 @s26, @l26, @d26, @c26,
 @s27, @l27, @d27, @c27,
 @s28, @l28, @d28, @c28,
 @s29, @l29, @d29, @c29,
 @s30, @l30, @d30, @c30,
 @s31, @l31, @d31, @c31,
 @s32, @l32, @d32, @c32,
 @s33, @l33, @d33, @c33,
 @s34, @l34, @d34, @c34,
 @s35, @l35, @d35, @c35,
 @s36, @l36, @d36, @c36,
 @s37, @l37, @d37, @c37, 
 @s38, @l38, @d38, @c38, 
 @s39, @l39, @d39, @c39, 
 @s40, @l40, @d40, @c40  )
 
 go

-- ================================================
-- Template generated from Template Explorer using:
-- Create Procedure (New Menu).SQL
--
-- Use the Specify Values for Template Parameters 
-- command (Ctrl-Shift-M) to fill in the parameter 
-- values below.
--
-- This block of comments will not be included in
-- the definition of the procedure.
-- ================================================
USE lin2world
go
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		Vanganth
-- Create date: 
-- Description:	Procedure for new func RequestLoadAbnormalStatus
-- =============================================

IF EXISTS (SELECT 1 
    FROM sys.procedures
    WHERE [name]='lin_GetActiveSkills') 
        drop procedure lin_GetActiveSkills
go

CREATE PROCEDURE lin_GetActiveSkills 
	-- Add the parameters for the stored procedure here
	@char_id AS INT
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	SELECT [s1]
      ,[l1]
      ,[d1]
      ,[s2]
      ,[l2]
      ,[d2]
      ,[s3]
      ,[l3]
      ,[d3]
      ,[s4]
      ,[l4]
      ,[d4]
      ,[s5]
      ,[l5]
      ,[d5]
      ,[s6]
      ,[l6]
      ,[d6]
      ,[s7]
      ,[l7]
      ,[d7]
      ,[s8]
      ,[l8]
      ,[d8]
      ,[s9]
      ,[l9]
      ,[d9]
      ,[s10]
      ,[l10]
      ,[d10]
      ,[s11]
      ,[l11]
      ,[d11]
      ,[s12]
      ,[l12]
      ,[d12]
      ,[s13]
      ,[l13]
      ,[d13]
      ,[s14]
      ,[l14]
      ,[d14]
      ,[s15]
      ,[l15]
      ,[d15]
      ,[s16]
      ,[l16]
      ,[d16]
      ,[s17]
      ,[l17]
      ,[d17]
      ,[s18]
      ,[l18]
      ,[d18]
      ,[s19]
      ,[l19]
      ,[d19]
      ,[s20]
      ,[l20]
      ,[d20]
      ,[s21]
      ,[l21]
      ,[d21]
      ,[s22]
      ,[l22]
      ,[d22]
      ,[s23]
      ,[l23]
      ,[d23]
      ,[s24]
      ,[l24]
      ,[d24]
      ,[s25]
      ,[l25]
      ,[d25]
      ,[s26]
      ,[l26]
      ,[d26]
      ,[s27]
      ,[l27]
      ,[d27]
      ,[s28]
      ,[l28]
      ,[d28]
      ,[s29]
      ,[l29]
      ,[d29]
      ,[s30]
      ,[l30]
      ,[d30]
      ,[s31]
      ,[l31]
      ,[d31]
      ,[s32]
      ,[l32]
      ,[d32]
      ,[s33]
      ,[l33]
      ,[d33]
      ,[s34]
      ,[l34]
      ,[d34]
      ,[s35]
      ,[l35]
      ,[d35]
      ,[s36]
      ,[l36]
      ,[d36]
      ,[s37]
      ,[l37]
      ,[d37]
      ,[s38]
      ,[l38]
      ,[d38]
      ,[s39]
      ,[l39]
      ,[d39]
      ,[s40]
      ,[l40]
      ,[d40]
      ,[c1]
      ,[c2]
      ,[c3]
      ,[c4]
      ,[c5]
      ,[c6]
      ,[c7]
      ,[c8]
      ,[c9]
      ,[c10]
      ,[c11]
      ,[c12]
      ,[c13]
      ,[c14]
      ,[c15]
      ,[c16]
      ,[c17]
      ,[c18]
      ,[c19]
      ,[c20]
      ,[c21]
      ,[c22]
      ,[c23]
      ,[c24]
      ,[c25]
      ,[c26]
      ,[c27]
      ,[c28]
      ,[c29]
      ,[c30]
      ,[c31]
      ,[c32]
      ,[c33]
      ,[c34]
      ,[c35]
      ,[c36]
      ,[c37]
      ,[c38]
      ,[c39]
      ,[c40]
	FROM [lin2world].[dbo].[user_ActiveSkill] WHERE char_id = @char_id
END
GO

USE lin2world 
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
USE [lin2world] 
GO
drop procedure lin_UpdateActiveSkill
go
-- =============================================
-- Author:		Vanganth
-- Create date: 
-- Description:	Procedure for new func RequestSaveAbnormalStatus
-- =============================================
CREATE PROCEDURE lin_UpdateActiveSkill 
(
 @char_id INT,
 @s1 INT, @l1 TINYINT, @d1 INT, @c1 TINYINT,
 @s2 INT, @l2 TINYINT, @d2 INT, @c2 TINYINT,
 @s3 INT, @l3 TINYINT, @d3 INT, @c3 TINYINT,
 @s4 INT, @l4 TINYINT, @d4 INT, @c4 TINYINT,
 @s5 INT, @l5 TINYINT, @d5 INT, @c5 TINYINT,
 @s6 INT, @l6 TINYINT, @d6 INT, @c6 TINYINT,
 @s7 INT, @l7 TINYINT, @d7 INT, @c7 TINYINT,
 @s8 INT, @l8 TINYINT, @d8 INT, @c8 TINYINT,
 @s9 INT, @l9 TINYINT, @d9 INT, @c9 TINYINT,
 @s10 INT, @l10 TINYINT, @d10 INT, @c10 TINYINT,
 @s11 INT, @l11 TINYINT, @d11 INT, @c11 TINYINT,
 @s12 INT, @l12 TINYINT, @d12 INT, @c12 TINYINT,
 @s13 INT, @l13 TINYINT, @d13 INT, @c13 TINYINT,
 @s14 INT, @l14 TINYINT, @d14 INT, @c14 TINYINT,
 @s15 INT, @l15 TINYINT, @d15 INT, @c15 TINYINT,
 @s16 INT, @l16 TINYINT, @d16 INT, @c16 TINYINT,
 @s17 INT, @l17 TINYINT, @d17 INT, @c17 TINYINT,
 @s18 INT, @l18 TINYINT, @d18 INT, @c18 TINYINT,
 @s19 INT, @l19 TINYINT, @d19 INT, @c19 TINYINT,
 @s20 INT, @l20 TINYINT, @d20 INT, @c20 TINYINT,
 @s21 INT, @l21 TINYINT, @d21 INT, @c21 TINYINT,
 @s22 INT, @l22 TINYINT, @d22 INT, @c22 TINYINT,
 @s23 INT, @l23 TINYINT, @d23 INT, @c23 TINYINT,
 @s24 INT, @l24 TINYINT, @d24 INT, @c24 TINYINT,
 @s25 INT, @l25 TINYINT, @d25 INT, @c25 TINYINT,
 @s26 INT, @l26 TINYINT, @d26 INT, @c26 TINYINT,
 @s27 INT, @l27 TINYINT, @d27 INT, @c27 TINYINT,
 @s28 INT, @l28 TINYINT, @d28 INT, @c28 TINYINT,
 @s29 INT, @l29 TINYINT, @d29 INT, @c29 TINYINT,
 @s30 INT, @l30 TINYINT, @d30 INT, @c30 TINYINT,
 @s31 INT, @l31 TINYINT, @d31 INT, @c31 TINYINT,
 @s32 INT, @l32 TINYINT, @d32 INT, @c32 TINYINT,
 @s33 INT, @l33 TINYINT, @d33 INT, @c33 TINYINT,
 @s34 INT, @l34 TINYINT, @d34 INT, @c34 TINYINT,
 @s35 INT, @l35 TINYINT, @d35 INT, @c35 TINYINT,
 @s36 INT, @l36 TINYINT, @d36 INT, @c36 TINYINT,
 @s37 INT, @l37 TINYINT, @d37 INT, @c37 TINYINT,
 @s38 INT, @l38 TINYINT, @d38 INT, @c38 TINYINT,
 @s39 INT, @l39 TINYINT, @d39 INT, @c39 TINYINT,
 @s40 INT, @l40 TINYINT, @d40 INT, @c40 TINYINT
)
AS
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
IF EXISTS(SELECT * FROM user_activeskill WHERE char_id = @char_id)
BEGIN
 UPDATE user_activeskill
 SET
 s1 = @s1, l1 = @l1, d1 = @d1, c1 = @c1,
 s2 = @s2, l2 = @l2, d2 = @d2, c2 = @c2,
 s3 = @s3, l3 = @l3, d3 = @d3, c3 = @c3,
 s4 = @s4, l4 = @l4, d4 = @d4, c4 = @c4,
 s5 = @s5, l5 = @l5, d5 = @d5, c5 = @c5,
 s6 = @s6, l6 = @l6, d6 = @d6, c6 = @c6,
 s7 = @s7, l7 = @l7, d7 = @d7, c7 = @c7,
 s8 = @s8, l8 = @l8, d8 = @d8, c8 = @c8,
 s9 = @s9, l9 = @l9, d9 = @d9, c9 = @c9,
 s10 = @s10, l10 = @l10, d10 = @d10, c10 = @c10,
 s11 = @s11, l11 = @l11, d11 = @d11, c11 = @c11,
 s12 = @s12, l12 = @l12, d12 = @d12, c12 = @c12,
 s13 = @s13, l13 = @l13, d13 = @d13, c13 = @c13,
 s14 = @s14, l14 = @l14, d14 = @d14, c14 = @c14,
 s15 = @s15, l15 = @l15, d15 = @d15, c15 = @c15,
 s16 = @s16, l16 = @l16, d16 = @d16, c16 = @c16,
 s17 = @s17, l17 = @l17, d17 = @d17, c17 = @c17,
 s18 = @s18, l18 = @l18, d18 = @d18, c18 = @c18,
 s19 = @s19, l19 = @l19, d19 = @d19, c19 = @c19,
 s20 = @s20, l20 = @l20, d20 = @d20, c20 = @c20,
 s21 = @s21, l21 = @l21, d21 = @d21, c21 = @c21,
 s22 = @s22, l22 = @l22, d22 = @d22, c22 = @c22,
 s23 = @s23, l23 = @l23, d23 = @d23, c23 = @c23,
 s24 = @s24, l24 = @l24, d24 = @d24, c24 = @c24,
 s25 = @s25, l25 = @l25, d25 = @d25, c25 = @c25,
 s26 = @s26, l26 = @l26, d26 = @d26, c26 = @c26,
 s27 = @s27, l27 = @l27, d27 = @d27, c27 = @c27,
 s28 = @s28, l28 = @l28, d28 = @d28, c28 = @c28,
 s29 = @s29, l29 = @l29, d29 = @d29, c29 = @c29,
 s30 = @s30, l30 = @l30, d30 = @d30, c30 = @c30,
 s31 = @s31, l31 = @l31, d31 = @d31, c31 = @c31,
 s32 = @s32, l32 = @l32, d32 = @d32, c32 = @c32,
 s33 = @s33, l33 = @l33, d33 = @d33, c33 = @c33,
 s34 = @s34, l34 = @l34, d34 = @d34, c34 = @c34,
 s35 = @s35, l35 = @l35, d35 = @d35, c35 = @c35,
 s36 = @s36, l36 = @l36, d36 = @d36, c36 = @c36,
 s37 = @s37, l37 = @l37, d37 = @d37, c37 = @c37,
 s38 = @s38, l38 = @l38, d38 = @d38, c38 = @c38,
 s39 = @s39, l39 = @l39, d39 = @d39, c39 = @c39,
 s40 = @s40, l40 = @l40, d40 = @d40, c40 = @c40
 WHERE char_id = @char_id
END  
ELSE
BEGIN  
 INSERT INTO user_activeskill
 (char_id,
 s1, l1, d1, c1,
 s2, l2, d2, c2,
 s3, l3, d3, c3,
 s4, l4, d4, c4,
 s5, l5, d5, c5,
 s6, l6, d6, c6,
 s7, l7, d7, c7,
 s8, l8, d8, c8,
 s9, l9, d9, c9,
 s10, l10, d10, c10,
 s11, l11, d11, c11,
 s12, l12, d12, c12,
 s13, l13, d13, c13,
 s14, l14, d14, c14,
 s15, l15, d15, c15,
 s16, l16, d16, c16,
 s17, l17, d17, c17,
 s18, l18, d18, c18,
 s19, l19, d19, c19,
 s20, l20, d20, c20,
 s21, l21, d21, c21,
 s22, l22, d22, c22,
 s23, l23, d23, c23,
 s24, l24, d24, c24,
 s25, l25, d25, c25,
 s26, l26, d26, c26,
 s27, l27, d27, c27,
 s28, l28, d28, c28,
 s29, l29, d29, c29,
 s30, l30, d30, c30,
 s31, l31, d31, c31,
 s32, l32, d32, c32,
 s33, l33, d33, c33,
 s34, l34, d34, c34,
 s35, l35, d35, c35,
 s36, l36, d36, c36,
 s37, l37, d37, c37,
 s38, l38, d38, c38,
 s39, l39, d39, c39,
 s40, l40, d40, c40 )
 VALUES
 (@char_id,
 @s1, @l1, @d1, @c1,
 @s2, @l2, @d2, @c2,
 @s3, @l3, @d3, @c3,
 @s4, @l4, @d4, @c4,
 @s5, @l5, @d5, @c5,
 @s6, @l6, @d6, @c6,
 @s7, @l7, @d7, @c7,
 @s8, @l8, @d8, @c8,
 @s9, @l9, @d9, @c9,
 @s10, @l10, @d10, @c10,
 @s11, @l11, @d11, @c11,
 @s12, @l12, @d12, @c12,
 @s13, @l13, @d13, @c13,
 @s14, @l14, @d14, @c14,
 @s15, @l15, @d15, @c15,
 @s16, @l16, @d16, @c16,
 @s17, @l17, @d17, @c17,
 @s18, @l18, @d18, @c18,
 @s19, @l19, @d19, @c19,
 @s20, @l20, @d20, @c20,
 @s21, @l21, @d21, @c21,
 @s22, @l22, @d22, @c22,
 @s23, @l23, @d23, @c23,
 @s24, @l24, @d24, @c24,
 @s25, @l25, @d25, @c25,
 @s26, @l26, @d26, @c26,
 @s27, @l27, @d27, @c27,
 @s28, @l28, @d28, @c28,
 @s29, @l29, @d29, @c29,
 @s30, @l30, @d30, @c30,
 @s31, @l31, @d31, @c31,
 @s32, @l32, @d32, @c32,
 @s33, @l33, @d33, @c33,
 @s34, @l34, @d34, @c34,
 @s35, @l35, @d35, @c35,
 @s36, @l36, @d36, @c36,
 @s37, @l37, @d37, @c37, 
 @s38, @l38, @d38, @c38, 
 @s39, @l39, @d39, @c39, 
 @s40, @l40, @d40, @c40  )
END

go

ALTER TABLE [dbo].[quest] ADD
	[q17] [int] NOT NULL CONSTRAINT [DF_Quest_q17]  DEFAULT ((0)),
	[s17] [int] NOT NULL CONSTRAINT [DF_Quest_s17]  DEFAULT ((0)),
	[q18] [int] NOT NULL CONSTRAINT [DF_Quest_q18]  DEFAULT ((0)),
	[s18] [int] NOT NULL CONSTRAINT [DF_Quest_s18]  DEFAULT ((0)),
	[q19] [int] NOT NULL CONSTRAINT [DF_Quest_q19]  DEFAULT ((0)),
	[s19] [int] NOT NULL CONSTRAINT [DF_Quest_s19]  DEFAULT ((0)),
	[q20] [int] NOT NULL CONSTRAINT [DF_Quest_q20]  DEFAULT ((0)),
	[s20] [int] NOT NULL CONSTRAINT [DF_Quest_s20]  DEFAULT ((0)),
	[q21] [int] NOT NULL CONSTRAINT [DF_Quest_q21]  DEFAULT ((0)),
	[s21] [int] NOT NULL CONSTRAINT [DF_Quest_s21]  DEFAULT ((0)),
	[q22] [int] NOT NULL CONSTRAINT [DF_Quest_q22]  DEFAULT ((0)),
	[s22] [int] NOT NULL CONSTRAINT [DF_Quest_s22]  DEFAULT ((0)),
	[q23] [int] NOT NULL CONSTRAINT [DF_Quest_q23]  DEFAULT ((0)),
	[s23] [int] NOT NULL CONSTRAINT [DF_Quest_s23]  DEFAULT ((0)),
	[q24] [int] NOT NULL CONSTRAINT [DF_Quest_q24]  DEFAULT ((0)),
	[s24] [int] NOT NULL CONSTRAINT [DF_Quest_s24]  DEFAULT ((0)),
	[q25] [int] NOT NULL CONSTRAINT [DF_Quest_q25]  DEFAULT ((0)),
	[s25] [int] NOT NULL CONSTRAINT [DF_Quest_s25]  DEFAULT ((0)),
	[q26] [int] NOT NULL CONSTRAINT [DF_Quest_q26]  DEFAULT ((0)),
	[s26] [int] NOT NULL CONSTRAINT [DF_Quest_s26]  DEFAULT ((0)),
	[j17] [int] NOT NULL CONSTRAINT [DF_Quest_j17]  DEFAULT ((0)),
	[j18] [int] NOT NULL CONSTRAINT [DF_Quest_j18]  DEFAULT ((0)),
	[j19] [int] NOT NULL CONSTRAINT [DF_Quest_j19]  DEFAULT ((0)),
	[j20] [int] NOT NULL CONSTRAINT [DF_Quest_j20]  DEFAULT ((0)),
	[j21] [int] NOT NULL CONSTRAINT [DF_Quest_j21]  DEFAULT ((0)),
	[j22] [int] NOT NULL CONSTRAINT [DF_Quest_j22]  DEFAULT ((0)),
	[j23] [int] NOT NULL CONSTRAINT [DF_Quest_j23]  DEFAULT ((0)),
	[j24] [int] NOT NULL CONSTRAINT [DF_Quest_j24]  DEFAULT ((0)),
	[j25] [int] NOT NULL CONSTRAINT [DF_Quest_j25]  DEFAULT ((0)),
	[j26] [int] NOT NULL CONSTRAINT [DF_Quest_j26]  DEFAULT ((0)),
	[s2_17] [int] NOT NULL CONSTRAINT [DF_QUEST_S2_17]  DEFAULT ((0)),
	[s2_18] [int] NOT NULL CONSTRAINT [DF_QUEST_S2_18]  DEFAULT ((0)),
	[s2_19] [int] NOT NULL CONSTRAINT [DF_QUEST_S2_19]  DEFAULT ((0)),
	[s2_20] [int] NOT NULL CONSTRAINT [DF_QUEST_S2_20]  DEFAULT ((0)),
	[s2_21] [int] NOT NULL CONSTRAINT [DF_QUEST_S2_21]  DEFAULT ((0)),
	[s2_22] [int] NOT NULL CONSTRAINT [DF_QUEST_S2_22]  DEFAULT ((0)),
	[s2_23] [int] NOT NULL CONSTRAINT [DF_QUEST_S2_23]  DEFAULT ((0)),
	[s2_24] [int] NOT NULL CONSTRAINT [DF_QUEST_S2_24]  DEFAULT ((0)),
	[s2_25] [int] NOT NULL CONSTRAINT [DF_QUEST_S2_25]  DEFAULT ((0)),
	[s2_26] [int] NOT NULL CONSTRAINT [DF_QUEST_S2_26]  DEFAULT ((0))
GO

if exists (select * from dbo.sysobjects where id = object_id(N'lin_LoadQuest'))
drop proc [lin_LoadQuest]
GO
/********************************************  
lin_LoadQuest  
   
INPUT  
 @char_id int  
OUTPUT  
return  
made by  
 carrot
 kuroi - extended number of quests to 25
date  
 2002-06-09  
********************************************/  
CREATE PROCEDURE [dbo].[lin_LoadQuest]
(
@char_id int
)
AS
SET NOCOUNT ON

SELECT TOP 1
  q1,  s1,  s2_1,  j1,
  q2,  s2,  s2_2,  j2,
  q3,  s3,  s2_3,  j3,
  q4,  s4,  s2_4,  j4,
  q5,  s5,  s2_5,  j5,
  q6,  s6,  s2_6,  j6,
  q7,  s7,  s2_7,  j7,
  q8,  s8,  s2_8,  j8,
  q9,  s9,  s2_9,  j9,
  q10, s10, s2_10, j10,
  q11, s11, s2_11, j11,
  q12, s12, s2_12, j12,
  q13, s13, s2_13, j13,
  q14, s14, s2_14, j14,
  q15, s15, s2_15, j15,
  q16, s16, s2_16, j16,
  q17, s17, s2_17, j17,
  q18, s18, s2_18, j18,
  q19, s19, s2_19, j19,
  q20, s20, s2_20, j20,
  q21, s21, s2_21, j21,
  q22, s22, s2_22, j22,
  q23, s23, s2_23, j23,
  q24, s24, s2_24, j24,
  q25, s25, s2_25, j25,
  q26, s26, s2_26, j26

FROM quest (nolock)
WHERE char_id = @char_id
GO

if exists (select * from dbo.sysobjects where id = object_id(N'lin_SetQuest'))
drop proc [lin_SetQuest]
GO

/********************************************  
lin_SetQuest   
   
OUTPUT  
return  
made by  
 carrot  
date  
 2002-06-09  
********************************************/  
CREATE PROCEDURE [dbo].[lin_SetQuest]
(  
@q1 INT,  
@s1 INT,  
@s2_1 INT,  
@j1 INT,  
  
@q2 INT,  
@s2 INT,  
@s2_2 INT,  
@j2 INT,  
  
@q3 INT,  
@s3 INT,  
@s2_3 INT,  
@j3 INT,  
  
@q4 INT,  
@s4 INT,  
@s2_4 INT,  
@j4 INT,  
  
@q5 INT,  
@s5 INT,  
@s2_5 INT,  
@j5 INT,  
  
@q6 INT,  
@s6 INT,  
@s2_6 INT,  
@j6 INT,  
  
  
@q7 INT,  
@s7 INT,  
@s2_7 INT,  
@j7 INT,  
  
@q8 INT,  
@s8 INT,  
@s2_8 INT,  
@j8 INT,  
  
@q9 INT,  
@s9 INT,  
@s2_9 INT,  
@j9 INT,  
  
@q10 INT,  
@s10 INT,  
@s2_10 INT,  
@j10 INT,  
  
@q11 INT,  
@s11 INT,  
@s2_11 INT,  
@j11 INT,  
  
@q12 INT,  
@s12 INT,  
@s2_12 INT,  
@j12 INT,  
  
@q13 INT,  
@s13 INT,  
@s2_13 INT,  
@j13 INT,  
  
@q14 INT,  
@s14 INT,  
@s2_14 INT,  
@j14 INT,  
  
@q15 INT,  
@s15 INT,  
@s2_15 INT,  
@j15 INT,  
  
@q16 INT,  
@s16 INT,  
@s2_16 INT,  
@j16 INT,  

@q17 INT,  
@s17 INT,  
@s2_17 INT,  
@j17 INT,  

@q18 INT,  
@s18 INT,  
@s2_18 INT,  
@j18 INT,  

@q19 INT,  
@s19 INT,  
@s2_19 INT,  
@j19 INT,  

@q20 INT,  
@s20 INT,  
@s2_20 INT,  
@j20 INT,  

@q21 INT,  
@s21 INT,  
@s2_21 INT,  
@j21 INT,  

@q22 INT,  
@s22 INT,  
@s2_22 INT,  
@j22 INT,  

@q23 INT,  
@s23 INT,  
@s2_23 INT,  
@j23 INT,  

@q24 INT,  
@s24 INT,  
@s2_24 INT,  
@j24 INT,  

@q25 INT,  
@s25 INT,  
@s2_25 INT,  
@j25 INT,  

@q26 INT,  
@s26 INT,  
@s2_26 INT,  
@j26 INT,  

@char_id  INT  
  
)  
AS  
SET NOCOUNT ON  
  
UPDATE quest   
SET   
q1=@q1, s1=@s1, s2_1=@s2_1, j1=@j1,   
q2=@q2, s2=@s2, s2_2=@s2_2, j2=@j2,   
q3=@q3, s3=@s3, s2_3=@s2_3, j3=@j3,   
q4=@q4, s4=@s4, s2_4=@s2_4, j4=@j4,   
q5=@q5, s5=@s5, s2_5=@s2_5, j5=@j5,   
q6=@q6, s6=@s6, s2_6=@s2_6, j6=@j6,   
q7=@q7, s7=@s7, s2_7=@s2_7, j7=@j7,   
q8=@q8, s8=@s8, s2_8=@s2_8, j8=@j8,   
q9=@q9, s9=@s9, s2_9=@s2_9, j9=@j9,   
q10=@q10, s10=@s10, s2_10=@s2_10, j10=@j10,   
q11=@q11, s11=@s11, s2_11=@s2_11, j11=@j11,   
q12=@q12, s12=@s12, s2_12=@s2_12, j12=@j12,   
q13=@q13, s13=@s13, s2_13=@s2_13, j13=@j13,   
q14=@q14, s14=@s14, s2_14=@s2_14, j14=@j14,   
q15=@q15, s15=@s15, s2_15=@s2_15, j15=@j15,   
q16=@q16, s16=@s16, s2_16=@s2_16, j16=@j16,
q17=@q17, s17=@s17, s2_17=@s2_17, j17=@j17,
q18=@q18, s18=@s18, s2_18=@s2_18, j18=@j18,
q19=@q19, s19=@s19, s2_19=@s2_19, j19=@j19,
q20=@q20, s20=@s20, s2_20=@s2_20, j20=@j20,
q21=@q21, s21=@s21, s2_21=@s2_21, j21=@j21,
q22=@q22, s22=@s22, s2_22=@s2_22, j22=@j22,
q23=@q23, s23=@s23, s2_23=@s2_23, j23=@j23,
q24=@q24, s24=@s24, s2_24=@s2_24, j24=@j24,
q25=@q25, s25=@s25, s2_25=@s2_25, j25=@j25,
q26=@q26, s26=@s26, s2_26=@s2_26, j26=@j26
WHERE char_id = @char_id
GO

if exists (select * from dbo.sysobjects where id = object_id(N'lin_UpdateJournal'))
drop proc [lin_UpdateJournal]
GO

CREATE PROCEDURE
[dbo].[lin_UpdateJournal] ( @id INT,  
@j1 int, @j2 int, @j3 int, @j4 int, @j5 int, @j6 int, @j7 int, @j8 int,
@j9 int, @j10 int, @j11 int, @j12 int, @j13 int, @j14 int, @j15 int, @j16 int,
@j17 int, @j18 int, @j19 int, @j20 int, @j21 int, @j22 int, @j23 int, @j24 int, @j25 int, @j26 int
)
AS
UPDATE Quest
set 
j1 = @j1,
j2 = @j2,
j3 = @j3,
j4 = @j4,
j5 = @j5,
j6 = @j6,
j7 = @j7,
j8 = @j8,
j9 = @j9,
j10 = @j10,
j11 = @j11,
j12 = @j12,
j13 = @j13,
j14 = @j14,
j15 = @j15,
j16 = @j16,
j17 = @j17,
j18 = @j18,
j19 = @j19,
j20 = @j20,
j21 = @j21,
j22 = @j22,
j23 = @j23,
j24 = @j24,
j25 = @j25,
j26 = @j26
where char_id = @id
GO

USE [lin2world]
GO

ALTER TABLE dbo.user_data ADD
	augmentation int NOT NULL DEFAULT 0
GO

USE [lin2world]
GO

ALTER TABLE dbo.user_item ADD
	augmentation int NOT NULL DEFAULT 0
GO

use lin2world
go

ALTER TABLE dbo.Pledge ADD
	delegate_clid int NULL DEFAULT 0


USE [lin2world]
GO
/****** Object:  Table [dbo].[pledge_ext]    Script Date: 01/21/2008 03:50:36 ******/
/****** Made by Vanganth@hotmail.com to support C5Ext.dll                ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[pledge_ext](
	[pledge_id] [int] NOT NULL,
	[reputation_points] [int] NOT NULL,
	[Rank1] [bigint] NOT NULL DEFAULT ((0)),
	[Rank2] [bigint] NOT NULL DEFAULT ((0)),
	[Rank3] [bigint] NOT NULL DEFAULT ((0)),
	[Rank4] [bigint] NOT NULL DEFAULT ((0)),
	[Rank5] [bigint] NOT NULL DEFAULT ((0)),
	[Rank6] [bigint] NOT NULL DEFAULT ((0)),
	[Rank7] [bigint] NOT NULL DEFAULT ((0)),
	[Rank8] [bigint] NOT NULL DEFAULT ((0)),
	[Rank9] [bigint] NOT NULL DEFAULT ((0)),
	[Skill0] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill1] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill2] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill3] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill4] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill5] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill6] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill7] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill8] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill9] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill10] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill11] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill12] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill13] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill14] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill15] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill16] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill17] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill18] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill19] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill20] [tinyint] NOT NULL DEFAULT ((0)),
	[Skill21] [tinyint] NOT NULL DEFAULT ((0)),
 CONSTRAINT [PK_pledge_ext] PRIMARY KEY CLUSTERED 
(
	[pledge_id] ASC
)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
go

USE [lin2world]
GO
/****** Object:  Table [dbo].[pledge_ext]    Script Date: 01/21/2008 03:50:36 ******/
/****** Made by Vanganth@hotmail.com to support C5Ext.dll                ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE dbo.pledge_subpledge
	(
	pledge_id int NOT NULL,
	pledge_type int NOT NULL,
	sub_name nvarchar(50) NOT NULL DEFAULT N'',
	leader_name nvarchar(50) NOT NULL DEFAULT N'',
	leader_dbid int NOT NULL DEFAULT ((0))
	)  ON [PRIMARY]
GO

CREATE TABLE dbo.SiegeStat
	(
	char_id int NOT NULL,
	pledge_id int NOT NULL,
	castle_id int NOT NULL,
	kill_count int NULL,
	death_count int NULL,
	damage_taken int NULL,
	damage_dealt int NULL
	) 
	
go

USE [lin2world]
GO
/* To prevent any potential data loss issues, you should review this script in detail before running it outside the context of the database designer.*/
ALTER TABLE dbo.user_data ADD
	[Rank] tinyint NOT NULL DEFAULT 6,
	PledgeType int NOT NULL DEFAULT 0,
	sponsor_id int NOT NULL DEFAULT 0,
	JoinPledgeLevel int NOT NULL DEFAULT 0
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
USE [lin2world] 
GO

CREATE PROCEDURE lin_AddMemberStat
	-- Add the parameters for the stored procedure here
	@castle_id AS INT,
	@pledge_id AS INT,
	@char_id AS INT,
	@kill_count AS INT,
	@death_count AS INT,
	@damage_dealt AS INT,
	@damage_taken AS INT

AS
BEGIN

	SET NOCOUNT ON;
	
	insert into SiegeStat (castle_id, pledge_id, char_id, kill_count, death_count, damage_dealt, damage_taken) 
							values(@castle_id, @pledge_id, @char_id, @kill_count, @death_count, @damage_dealt, @damage_taken)

END
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
USE [lin2world] 
GO
-- =============================================
-- Author:		Vanganth
-- Create date: 
-- Description:	Procedure for new func RequestAddPledgeExt
-- =============================================
CREATE PROCEDURE lin_AddPledgeExt
	-- Add the parameters for the stored procedure here
	@pledge_id AS INT
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
    -- Insert statements for procedure here
INSERT INTO [lin2world].[dbo].[pledge_ext] ([pledge_id] ,[reputation_points],[Rank1],[Rank2],[Rank3],[Rank4],[Rank5],[Rank6],[Rank7],[Rank8],[Rank9],[Skill0],[Skill1],[Skill2],[Skill3],[Skill4],[Skill5],[Skill6],[Skill7],[Skill8],[Skill9],[Skill10],[Skill11],[Skill12],[Skill13],[Skill14],[Skill15],[Skill16],[Skill17],[Skill18],[Skill19],[Skill20],[Skill21])VALUES(@pledge_id,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)

END
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
USE [lin2world] 
GO
CREATE PROCEDURE lin_ClearPledgeStat
	-- Add the parameters for the stored procedure here
	@castle_id AS INT,
	@pledge_id AS INT

AS
BEGIN

	SET NOCOUNT ON;

	delete from SiegeStat where castle_id = @castle_id AND pledge_id = @pledge_id
	

END
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
USE [lin2world] 
GO
-- =============================================
-- Author:		Vanganth
-- Create date: 
-- Description:	Procedure for new func RequestCreateSubPledge
-- =============================================
CREATE PROCEDURE lin_CreateSubPledge 
	-- Add the parameters for the stored procedure here
	@pledge_id AS INT,
	@pledge_type AS INT,
	@sub_name AS nvarchar(50)
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;


IF(@pledge_id<>0)
BEGIN
	IF(@pledge_type = -1)
	BEGIN
		INSERT INTO [lin2world].[dbo].[pledge_subpledge]
				   ([pledge_id]
				   ,[pledge_type]
				   ,[sub_name]
				   ,[leader_name]
				   ,[leader_dbid])
			 VALUES
				   (@pledge_id
				   ,@pledge_type
				   ,@sub_name
				   ,''
				   ,0)
	END
ELSE IF(@pledge_type = 100)
	BEGIN
		INSERT INTO [lin2world].[dbo].[pledge_subpledge]
				   ([pledge_id]
				   ,[pledge_type]
				   ,[sub_name]
				   ,[leader_name]
				   ,[leader_dbid])
			 VALUES
				   (@pledge_id
				   ,@pledge_type
				   ,@sub_name
				   ,''
				   ,0)
	END
ELSE IF(@pledge_type = 200)
	BEGIN
		INSERT INTO [lin2world].[dbo].[pledge_subpledge]
				   ([pledge_id]
				   ,[pledge_type]
				   ,[sub_name]
				   ,[leader_name]
				   ,[leader_dbid])
			 VALUES
				   (@pledge_id
				   ,@pledge_type
				   ,@sub_name
				   ,''
				   ,0)
	END
ELSE IF(@pledge_type = 1001)
	BEGIN
		INSERT INTO [lin2world].[dbo].[pledge_subpledge]
				   ([pledge_id]
				   ,[pledge_type]
				   ,[sub_name]
				   ,[leader_name]
				   ,[leader_dbid])
			 VALUES
				   (@pledge_id
				   ,@pledge_type
				   ,@sub_name
				   ,''
				   ,0)
	END
ELSE IF(@pledge_type = 1002)
	BEGIN
		INSERT INTO [lin2world].[dbo].[pledge_subpledge]
				   ([pledge_id]
				   ,[pledge_type]
				   ,[sub_name]
				   ,[leader_name]
				   ,[leader_dbid])
			 VALUES
				   (@pledge_id
				   ,@pledge_type
				   ,@sub_name
				   ,''
				   ,0)
	END
ELSE IF(@pledge_type = 2001)
	BEGIN
		INSERT INTO [lin2world].[dbo].[pledge_subpledge]
				   ([pledge_id]
				   ,[pledge_type]
				   ,[sub_name]
				   ,[leader_name]
				   ,[leader_dbid])
			 VALUES
				   (@pledge_id
				   ,@pledge_type
				   ,@sub_name
				   ,''
				   ,0)
	END
ELSE IF(@pledge_type = 2002)
	BEGIN
		INSERT INTO [lin2world].[dbo].[pledge_subpledge]
				   ([pledge_id]
				   ,[pledge_type]
				   ,[sub_name]
				   ,[leader_name]
				   ,[leader_dbid])
			 VALUES
				   (@pledge_id
				   ,@pledge_type
				   ,@sub_name
				   ,''
				   ,0)
	END
END

END
GO

/****** Object:  StoredProcedure [dbo].[lin_DelegateClanMaster]    Script Date: 10/20/2009 20:05:01 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		Vanganth
-- Create date: 
-- Description:	Procedure for Delegating clan masters
-- =============================================
CREATE PROCEDURE [dbo].[lin_DelegateClanMaster] 
@pledge_id AS INT
AS

SET NOCOUNT ON;
DECLARE @delegate_clid int;
DECLARE @clan_id int;

SELECT @delegate_clid=delegate_clid from Pledge where pledge_id = @pledge_id
if( @delegate_clid <> 0 )
 BEGIN
  SELECT @clan_id = pledge_id from user_data where char_id = @delegate_clid
  if( @clan_id = @pledge_id )
  BEGIN
   UPDATE Pledge SET ruler_id = @delegate_clid WHERE pledge_id = @pledge_id
   UPDATE Pledge SET delegate_clid = 0 WHERE pledge_id = @pledge_id
  END
  ELSE
  BEGIN
   UPDATE Pledge SET delegate_clid = 0 WHERE pledge_id = @pledge_id
  END
 END
 
go

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
USE [lin2world] 
GO
-- =============================================
-- Author:		Vanganth
-- Create date: 
-- Description:	Procedure for new func RequestLearnClanSkill
-- =============================================
CREATE PROCEDURE lin_LearnClanSkill 
	-- Add the parameters for the stored procedure here
	@pledge_id AS INT,
	@clan_skill AS INT,
	@new_level AS INT
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

IF((@clan_skill >= 370 AND @clan_skill < 392) AND @new_level < 4)
BEGIN

	IF(@clan_skill = 370)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill0]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 371)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill1]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 372)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill2]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 373)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill3]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 374)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill4]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 375)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill5]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 376)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill6]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 377)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill7]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 378)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill8]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 379)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill9]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 380)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill10]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 381)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill11]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 382)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill12]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 383)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill13]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 384)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill14]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 385)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill15]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 386)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill16]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 387)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill17]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 388)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill18]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 389)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill19]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 390)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill20]=@new_level WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@clan_skill = 391)
	BEGIN
			UPDATE [lin2world].[dbo].[pledge_ext] SET [Skill21]=@new_level WHERE [pledge_id] = @pledge_id
	END
END

END
GO

USE lin2world
go

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		Vanganth
-- Create date: 
-- Description:	Procedure for new func RequestLoadPledgeExtendInfo
-- =============================================
CREATE PROCEDURE lin_LoadPledgeExtendInfo 
	-- Add the parameters for the stored procedure here
	@pledge_id AS INT
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
SELECT [reputation_points]
      ,[Rank1]
      ,[Rank2]
      ,[Rank3]
      ,[Rank4]
      ,[Rank5]
      ,[Rank6]
      ,[Rank7]
      ,[Rank8]
      ,[Rank9]
      ,[Skill0]
      ,[Skill1]
      ,[Skill2]
      ,[Skill3]
      ,[Skill4]
      ,[Skill5]
      ,[Skill6]
      ,[Skill7]
      ,[Skill8]
      ,[Skill9]
      ,[Skill10]
      ,[Skill11]
      ,[Skill12]
      ,[Skill13]
      ,[Skill14]
      ,[Skill15]
      ,[Skill16]
      ,[Skill17]
      ,[Skill18]
      ,[Skill19]
      ,[Skill20]
      ,[Skill21]
  FROM [lin2world].[dbo].[pledge_ext] WHERE [pledge_id] = @pledge_id
END
GO

USE lin2world
go

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		Vanganth
-- Create date: 
-- Description:	Procedure for new func RequestLoadPledgeExtendInfo
-- =============================================
CREATE PROCEDURE lin_LoadSubPledges 
	-- Add the parameters for the stored procedure here
	@pledge_id AS INT
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
IF(@pledge_id <> 0)
BEGIN
SELECT TOP 7 pledge_type, sub_name, leader_name, leader_dbid  FROM pledge_subpledge (nolock) WHERE pledge_id = @pledge_id and pledge_type != 0
END
END
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
USE [lin2world] 
GO
-- =============================================
-- Author:		Vanganth
-- Create date: 
-- Description:	Procedure for new func RequestRenameSubPledge
-- =============================================
CREATE PROCEDURE lin_RenameSubPledge 
	-- Add the parameters for the stored procedure here
	@pledge_id AS INT,
	@pledge_type AS INT,
	@sub_name AS nvarchar(50)
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;


IF(@pledge_id<>0)
BEGIN
	IF(@pledge_type = -1)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_subpledge] SET sub_name = @sub_name WHERE pledge_id = @pledge_id AND pledge_type = @pledge_type
	END
ELSE IF(@pledge_type = 100)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_subpledge] SET sub_name = @sub_name WHERE pledge_id = @pledge_id AND pledge_type = @pledge_type
	END
ELSE IF(@pledge_type = 200)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_subpledge] SET sub_name = @sub_name WHERE pledge_id = @pledge_id AND pledge_type = @pledge_type
	END
ELSE IF(@pledge_type = 1001)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_subpledge] SET sub_name = @sub_name WHERE pledge_id = @pledge_id AND pledge_type = @pledge_type
	END
ELSE IF(@pledge_type = 1002)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_subpledge] SET sub_name = @sub_name WHERE pledge_id = @pledge_id AND pledge_type = @pledge_type
	END
ELSE IF(@pledge_type = 2001)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_subpledge] SET sub_name = @sub_name WHERE pledge_id = @pledge_id AND pledge_type = @pledge_type
	END
ELSE IF(@pledge_type = 2002)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_subpledge] SET sub_name = @sub_name WHERE pledge_id = @pledge_id AND pledge_type = @pledge_type
	END
END

END
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
USE [lin2world] 
GO
-- =============================================
-- Author:		Vanganth
-- Create date: 
-- Description:	Procedure for new func RequestReorganizeMember
-- =============================================
CREATE PROCEDURE lin_ReorganizePledgeMember 
	-- Add the parameters for the stored procedure here
	@char_name AS nvarchar(50),
	@pledge_type AS INT,
	@pledge_id AS INT

AS
BEGIN

	SET NOCOUNT ON;

UPDATE [lin2world].[dbo].[user_data] SET [PledgeType] = @pledge_type WHERE ([char_name] = @char_name AND [pledge_id] = @pledge_id)
SELECT [char_id] FROM [lin2world].[dbo].[user_data] WHERE [char_name] = @char_name

END
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
USE [lin2world] 
GO
-- =============================================
-- Author:		Vanganth
-- Create date: 
-- Description:	Procedure for Running Delegate clan master procedure for each pledge
-- =============================================
CREATE PROCEDURE lin_RunDelegateClanMaster
AS

SET NOCOUNT ON;
DECLARE
@pledge_id INT
DECLARE CUR1 CURSOR FOR
SELECT [pledge_id] from lin2world.dbo.Pledge where [delegate_clid] > 0
OPEN CUR1
FETCH NEXT FROM CUR1
INTO @pledge_id
WHILE (@@FETCH_STATUS <> -1)
BEGIN
exec lin2world.dbo.[lin_DelegateClanMaster] @pledge_id
FETCH NEXT FROM CUR1
INTO @pledge_id
END
CLOSE CUR1
DEALLOCATE CUR1
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
USE [lin2world] 
GO
-- =============================================
-- Author:		Vanganth
-- Create date: 
-- Description:	Procedure for new func RequestSetPledgePrivilege
-- =============================================
CREATE PROCEDURE lin_SetPledgePrivilege 
	-- Add the parameters for the stored procedure here
	@pledge_id AS INT,
	@rank AS INT,
	@privilege AS INT
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;


IF(@rank > 0 AND @rank < 10)
BEGIN
	IF(@rank = 1)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_ext] SET [Rank1]=@privilege WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@rank = 2)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_ext] SET [Rank2]=@privilege WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@rank = 3)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_ext] SET [Rank3]=@privilege WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@rank = 4)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_ext] SET [Rank4]=@privilege WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@rank = 5)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_ext] SET [Rank5]=@privilege WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@rank = 6)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_ext] SET [Rank6]=@privilege WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@rank = 7)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_ext] SET [Rank7]=@privilege WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@rank = 8)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_ext] SET [Rank8]=@privilege WHERE [pledge_id] = @pledge_id
	END
ELSE IF(@rank = 9)
	BEGIN
		UPDATE [lin2world].[dbo].[pledge_ext] SET [Rank9]=@privilege WHERE [pledge_id] = @pledge_id
	END
END

END
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
USE [lin2world] 
GO
-- =============================================
-- Author:		Vanganth
-- Create date: 
-- Description:	Procedure for new func RequestSetSubCaptain
-- =============================================
CREATE PROCEDURE lin_SetSubCaptain 
	-- Add the parameters for the stored procedure here
	@pledge_id AS INT,
	@pledge_type AS INT,
	@captain_name AS nvarchar(50),
	@captain_id AS INT
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;


IF(@pledge_id<>0)
BEGIN
	UPDATE [lin2world].[dbo].[pledge_subpledge] SET [leader_name]=@captain_name, [leader_dbid]=@captain_id WHERE ([pledge_id]=@pledge_id AND [pledge_type] = @pledge_type)
END

END
GO

use lin2world
go

ALTER TABLE dbo.user_item ADD
	mana_left int NOT NULL DEFAULT 0
GO

use lin2world
go

ALTER  TABLE user_data add 
	effective_mana0 int NOT NULL DEFAULT 0,
	effective_mana1 int NOT NULL DEFAULT 0,
	effective_mana2 int NOT NULL DEFAULT 0,
	effective_mana3 int NOT NULL DEFAULT 0
go

ALTER TABLE user_item
ADD life_time int NOT NULL DEFAULT 0
go

CREATE TABLE NpcDeath
	(
	npc_class_id int NOT NULL,
	death_time int NOT NULL
	) 
	
go

ALTER  TABLE user_data add 
	vip_level int NOT NULL DEFAULT 0,
	vip_timestamp int NOT NULL DEFAULT 0
go

CREATE TABLE user_vote
	(
	account_id int NOT NULL,
	vote_points int NOT NULL DEFAULT 0,
	vote_timestamp int NOT NULL DEFAULT 0
	) 
	
go

ALTER TABLE dbo.user_item ADD
	protection_timeout int NOT NULL DEFAULT 0
go


CREATE TABLE [dbo].[banned_account](
	[account_id] [int] NOT NULL,
	[account_name] nvarchar(50) NOT NULL DEFAULT N'',
	[timestamp] [int] NOT NULL DEFAULT 0,
	[reason] nvarchar(255) NOT NULL DEFAULT N''
	CONSTRAINT [PK_banned_account] PRIMARY KEY CLUSTERED 
	(
		[account_id] ASC
	)WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO


CREATE PROCEDURE lin_BanAccount
(
	@accountId as INT,
	@accountName as nvarchar(50),
	@timestamp as INT,
	@reason as nvarchar(255)
)
AS

SET NOCOUNT ON;
BEGIN
	IF EXISTS(SELECT * FROM [dbo].[banned_account] WHERE [account_id] = @accountId)
	BEGIN
		UPDATE [banned_account] SET [timestamp] = @timestamp, [reason] = @reason WHERE [account_id] = @accountId
	END
	ELSE
	BEGIN
		INSERT INTO [banned_account] ([account_id], [account_name], [timestamp], [reason]) VALUES (@accountId, @accountName, @timestamp, @reason)
	END
END

go

CREATE PROCEDURE lin_LoadBannedAccounts
AS

SET NOCOUNT ON;
BEGIN
	SELECT [account_id], [timestamp] FROM [dbo].[banned_account]
END

go

CREATE TABLE MultiSell_Log
	(
	[char_id] int NOT NULL,
	[account_id] int NOT NULL,
	[char_name] nvarchar(50),
	[account_name] nvarchar(50),
	[price1_id] int NOT NULL DEFAULT 0,
	[price1_count] int NOT NULL DEFAULT 0,
	[price2_id] int NOT NULL DEFAULT 0,
	[price2_count] int NOT NULL DEFAULT 0,
	[price3_id] int NOT NULL DEFAULT 0,
	[price3_count] int NOT NULL DEFAULT 0,
	[price4_id] int NOT NULL DEFAULT 0,
	[price4_count] int NOT NULL DEFAULT 0,
	[price5_id] int NOT NULL DEFAULT 0,
	[price5_count] int NOT NULL DEFAULT 0,
	[reward1_id] int NOT NULL DEFAULT 0,
	[reward1_count] int NOT NULL DEFAULT 0,
	[reward2_id] int NOT NULL DEFAULT 0,
	[reward2_count] int NOT NULL DEFAULT 0,
	[reward3_id] int NOT NULL DEFAULT 0,
	[reward3_count] int NOT NULL DEFAULT 0,
	[reward4_id] int NOT NULL DEFAULT 0,
	[reward4_count] int NOT NULL DEFAULT 0,
	[reward5_id] int NOT NULL DEFAULT 0,
	[reward5_count] int NOT NULL DEFAULT 0,
	[timestamp] int NOT NULL DEFAULT 0
	) 
	
go

CREATE PROCEDURE lin_AddMultiSellLog
(
	@charId as INT,
	@accountId as INT,
	@charName as nvarchar(50),
	@accountName as nvarchar(50),
	@price1Id as INT,
	@price1Count as INT,
	@price2Id as INT,
	@price2Count as INT,
	@price3Id as INT,
	@price3Count as INT,
	@price4Id as INT,
	@price4Count as INT,
	@price5Id as INT,
	@price5Count as INT,
	@reward1Id as INT,
	@reward1Count as INT,
	@reward2Id as INT,
	@reward2Count as INT,
	@reward3Id as INT,
	@reward3Count as INT,
	@reward4Id as INT,
	@reward4Count as INT,
	@reward5Id as INT,
	@reward5Count as INT,	
	@timestamp as INT
)
AS

SET NOCOUNT ON;
BEGIN
	INSERT INTO [MultiSell_Log] ([char_id], [account_id], [char_name], [account_name], [price1_id], [price1_count], [price2_id], [price2_count], [price3_id], [price3_count], [price4_id], [price4_count], [price5_id], [price5_count], [reward1_id], [reward1_count], [reward2_id], [reward2_count], [reward3_id], [reward3_count], [reward4_id], [reward4_count], [reward5_id], [reward5_count], [timestamp]) VALUES (@charId, @accountId, @charName, @accountName, @price1Id, @price1Count, @price2Id, @price2Count, @price3Id, @price3Count, @price4Id, @price4Count, @price5Id, @price5Count, @reward1Id, @reward1Count, @reward2Id, @reward2Count, @reward3Id, @reward3Count, @reward4Id, @reward4Count, @reward5Id, @reward5Count, @timestamp )
END

go

ALTER TABLE [user_data] ADD
	hardware_id int NOT NULL DEFAULT 0
go



CREATE TABLE HardwareId_Lock
	(
	[char_id] int NOT NULL DEFAULT 0,
	[account_id] int NOT NULL DEFAULT 0,
	[hardware_id] int NOT NULL DEFAULT 0,
	[locked_by] nvarchar(255) NOT NULL DEFAULT N''
	) 
	
go


CREATE PROCEDURE lin_ProcessHardwareIdLock
(
	@accountId as INT,
	@charId as INT,
	@hardwareId as INT,
	@locker as nvarchar(255)
)
AS

SET NOCOUNT ON;
BEGIN
	IF(@hardwareId = 0)
	BEGIN
	--remove lock
		IF(@accountId != 0)
		BEGIN
			DELETE FROM [HardwareId_Lock] WHERE [account_id] = @accountId
		END
		IF(@charId != 0)
		BEGIN
			DELETE FROM [HardwareId_Lock] WHERE [char_id] = @charId
		END
	END
	ELSE IF(@hardwareId != 0)
	BEGIN
		--Add/Update lock
		IF(@accountId != 0)
		BEGIN
			IF EXISTS(SELECT * FROM [HardwareId_Lock] WHERE [account_id] = @accountId)
			BEGIN
				UPDATE [HardwareId_Lock] SET [hardware_id] = @hardwareId, [locked_by] = @locker WHERE [account_id] = @accountId
			END
			ELSE
			BEGIN
				INSERT INTO [HardwareId_Lock] ([account_id], [char_id], [hardware_id], [locked_by]) VALUES (@accountId, @charId, @hardwareId, @locker)
			END
		END
		IF(@charId != 0)
		BEGIN
			IF EXISTS(SELECT * FROM [HardwareId_Lock] WHERE [char_id] = @charId)
			BEGIN
				UPDATE [HardwareId_Lock] SET [hardware_id] = @hardwareId, [locked_by] = @locker WHERE [char_id] = @charId
			END
			ELSE
			BEGIN
				INSERT INTO [HardwareId_Lock] ([account_id], [char_id], [hardware_id], [locked_by]) VALUES (@accountId, @charId, @hardwareId, @locker)
			END
		END
	END
END

go

CREATE TABLE HardwareId_Ban
	(
	[hardware_id] int NOT NULL DEFAULT 0,
	[timestamp] int NOT NULL DEFAULT 0,
	[locked_by] nvarchar(255) NOT NULL DEFAULT N'',
	[reason] nvarchar(255) NOT NULL DEFAULT N''
	) 
	
go

CREATE PROCEDURE lin_ProcessHardwareIdBan
(
	@hardwareId as INT,
	@timestamp as INT,
	@locker as nvarchar(255),
	@reason as nvarchar(255)
)
AS

SET NOCOUNT ON;
BEGIN
	IF(@timestamp = 0)
	BEGIN
	--remove lock
		DELETE FROM [HardwareId_Ban] WHERE [hardware_id] = @hardwareId
	END
	ELSE IF(@timestamp != 0)
	BEGIN
		--Add/Update lock
		IF EXISTS(SELECT * FROM [HardwareId_Ban] WHERE [hardware_id] = @hardwareId)
		BEGIN
			UPDATE [HardwareId_Ban] SET [timestamp] = @timestamp, [locked_by] = @locker, [reason] = @reason WHERE [hardware_id] = @hardwareId
		END
		ELSE
		BEGIN
			INSERT INTO [HardwareId_Ban] ([hardware_id], [timestamp], [locked_by], [reason]) VALUES (@hardwareId, @timestamp, @locker, @reason)
		END
	END
END

go

CREATE TABLE AdditionalBox
	(
	[account_id] int NOT NULL DEFAULT 0,
	[timestamp] int NOT NULL DEFAULT 0
	) 	
go

CREATE PROCEDURE lin_ProcessAdditionalBox
(
	@accountId as INT,
	@timestamp as INT
)
AS

SET NOCOUNT ON;
BEGIN
	IF(@timestamp = 0)
	BEGIN
	--remove box
		DELETE FROM [AdditionalBox] WHERE [account_id] = @accountId
	END
	ELSE IF(@timestamp != 0)
	BEGIN
		--Add/Update box
		IF EXISTS(SELECT * FROM [AdditionalBox] WHERE [account_id] = @accountId)
		BEGIN
			UPDATE [AdditionalBox] SET [timestamp] = @timestamp WHERE [account_id] = @accountId
		END
		ELSE
		BEGIN
			INSERT INTO [AdditionalBox] ([account_id], [timestamp]) VALUES (@accountId, @timestamp)
		END
	END
END

go

CREATE TABLE [ItemDelivery] (
  [job_id] int IDENTITY(1, 1) NOT NULL,
  [char_id] int DEFAULT (0) NOT NULL,
  [item_id] int DEFAULT (0) NOT NULL,
  [item_amount] int DEFAULT (0) NOT NULL
)
ON [PRIMARY]
GO

CREATE PROCEDURE lin_ItemDeliveryAsk
(
	@charId as INT
)
AS

SET NOCOUNT ON;
BEGIN
	SELECT * FROM [ItemDelivery] WHERE [char_id] = @charId
END

go

CREATE PROCEDURE lin_ItemDeliveryDone
(
	@charId as INT,
	@jobId as INT
)
AS

SET NOCOUNT ON;

BEGIN
	DELETE FROM [ItemDelivery] WHERE [char_id] = @charId AND [job_id] = @jobId
END

GO

--Update itemDelivery
ALTER TABLE [ItemDelivery] ADD
	[enchant] int NOT NULL DEFAULT 0
go

ALTER TABLE [user_data] ADD
	aio_id int NOT NULL DEFAULT 0,
	aio_timestamp int NOT NULL DEFAULT 0
go


IF EXISTS (SELECT 1 
    FROM INFORMATION_SCHEMA.TABLES 
    WHERE TABLE_NAME='BuffList') 
        drop table BuffList

go

create table user_scheme_buffer(
	char_id int not null,
	subjob_index int not null default 0,
	type int not null default 0,
	b0 int not null default 0,
	b1 int not null default 0,
	b2 int not null default 0,
	b3 int not null default 0,
	b4 int not null default 0,
	b5 int not null default 0,
	b6 int not null default 0,
	b7 int not null default 0,
	b8 int not null default 0,
	b9 int not null default 0,
	b10 int not null default 0,
	b11 int not null default 0,
	b12 int not null default 0,
	b13 int not null default 0,
	b14 int not null default 0,
	b15 int not null default 0,
	b16 int not null default 0,
	b17 int not null default 0,
	b18 int not null default 0,
	b19 int not null default 0,
	b20 int not null default 0,
	b21 int not null default 0,
	b22 int not null default 0,
	b23 int not null default 0,
	b24 int not null default 0,
	b25 int not null default 0,
	b26 int not null default 0,
	b27 int not null default 0,
	b28 int not null default 0,
	b29 int not null default 0,
	b30 int not null default 0,
	b31 int not null default 0,
	b32 int not null default 0,
	b33 int not null default 0,
	b34 int not null default 0,
	b35 int not null default 0,
	b36 int not null default 0,
	b37 int not null default 0,
	b38 int not null default 0,
	b39 int not null default 0,
	b40 int not null default 0,
	b41 int not null default 0,
	b42 int not null default 0,
	b43 int not null default 0,
	b44 int not null default 0,
	b45 int not null default 0,
	b46 int not null default 0,
	b47 int not null default 0,
	)
go

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		Vanganth
-- Create date: 2012-11-03
-- Description:	lin_SaveSubJob saves subjob stats
-- =============================================
CREATE PROCEDURE lin_SaveSubJob
(
	@char_id	INT,
	@subjob_id	TINYINT,
	@hp		FLOAT,
	@mp		FLOAT,
	@sp		INT,
	@exp		INT,
	@level		TINYINT,
	@henna_1	INT,
	@henna_2	INT,
	@henna_3	INT
	)
AS
BEGIN
	SET NOCOUNT ON;
	UPDATE [user_subjob] SET [hp] = @hp, [mp] = @mp, [sp] = @sp, [exp] = @exp, [level] = @level, [henna_1] = @henna_1, [henna_2] = @henna_2, [henna_3] = @henna_3 WHERE [char_id] = @char_id AND [subjob_id] = @subjob_id
END
GO


-- Auction system
IF EXISTS (SELECT 1 FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME='Auction')
BEGIN
        drop table Auction
        drop table AuctionPayment
        drop procedure lin_AuctionPaymentAsk
        drop procedure lin_AuctionPaymentDone
        
END
go

CREATE TABLE [Auction] (
  [auction_id] int IDENTITY(1, 1) NOT NULL,
  [seller_id] int DEFAULT (0) NOT NULL,
  [seller_name] nvarchar(25) NOT NULL DEFAULT N'',
  [item_id] int DEFAULT (0) NOT NULL,
  [amount] int DEFAULT (0) NOT NULL,
  [enchant] int DEFAULT (0) NOT NULL,
  [augmentation] int DEFAULT (0) NOT NULL,
  [price] int DEFAULT (0) NOT NULL,
  [expire_time] int DEFAULT (0) NOT NULL
  
)
ON [PRIMARY]
GO

CREATE PROCEDURE lin_AuctionCreate
(
	@sellerId as INT,
	@sellerName as nvarchar(25),
	@itemId as INT,
	@amount as INT,
	@enchant as INT,
	@augmentation as INT,
	@price as INT,
	@expireTime as INT
)
AS

SET NOCOUNT ON;
DECLARE @auctionId int  
SET @auctionId = 0
INSERT INTO [Auction] ( seller_id, seller_name, item_id, amount, enchant, augmentation, price, expire_time ) VALUES (@sellerId, @sellerName, @itemId, @amount, @enchant, @augmentation, @price, @expireTime)
IF (@@error = 0)  
BEGIN  
	SET @auctionId = @@IDENTITY
END  
  
SELECT @auctionId 

GO

CREATE PROCEDURE lin_AuctionModdifyAmount
(
	@auctionId as INT,
	@amount as INT
)
AS

SET NOCOUNT ON;
UPDATE [Auction] SET [amount] = @amount WHERE [auction_id] = @auctionId

GO

CREATE PROCEDURE lin_AuctionDelete
(
	@auctionId as INT
)
AS

SET NOCOUNT ON;
DELETE FROM [Auction] WHERE [auction_id] = @auctionId

GO

CREATE PROCEDURE lin_AuctionLoadItems
AS

SET NOCOUNT ON;
SELECT [auction_id], [seller_id], [seller_name], [item_id], [amount], [enchant], [augmentation], [price], [expire_time]  FROM [Auction]
GO

CREATE TABLE [AuctionPayment] (
  [job_id] int IDENTITY(1, 1) NOT NULL,
  [char_id] int DEFAULT (0) NOT NULL,
  [price_id] int DEFAULT (0) NOT NULL,
  [price_amount] int DEFAULT (0) NOT NULL,
  [item_id] int DEFAULT (0) NOT NULL,
  [item_amount] int DEFAULT (0) NOT NULL,
  [buyer] nvarchar(25) NOT NULL DEFAULT N''  
)
ON [PRIMARY]
GO

CREATE PROCEDURE lin_AuctionAddPaymentJob
(
	@charId as INT,
	@priceId as INT,
	@priceAmount as INT,
	@itemId as INT,
	@itemAmount as INT,
	@buyer as nvarchar(25)
)
AS
SET NOCOUNT on;
BEGIN
	INSERT INTO [AuctionPayment] ([char_id], [price_id], [price_amount], [item_id], [item_amount], [buyer]) VALUES (@charId, @priceId, @priceAmount, @itemId, @itemAmount, @buyer)
END

go

CREATE PROCEDURE lin_AuctionPaymentAsk
(
	@charId as INT
)
AS

SET NOCOUNT ON;
BEGIN
	SELECT * FROM [AuctionPayment] WHERE [char_id] = @charId
END

go

CREATE PROCEDURE lin_AuctionPaymentDone
(
	@charId as INT,
	@jobId as INT
)
AS

SET NOCOUNT ON;

BEGIN
	DELETE FROM [AuctionPayment] WHERE [char_id] = @charId AND [job_id] = @jobId
END

GO

CREATE TABLE pledge_skill
	(
	[pledge_id] int NOT NULL DEFAULT 0,
	[skill_id] int NOT NULL DEFAULT 0,
	[skill_level] int NOT NULL DEFAULT 0,
	) 	
go

CREATE PROCEDURE lin_AddPledgeSkill
(
	@pledgeId as INT,
	@skillId as INT,
	@skillLevel as INT
)
AS

SET NOCOUNT ON;

BEGIN
	IF EXISTS(SELECT * FROM [pledge_skill] WHERE [pledge_id] = @pledgeId AND [skill_id] = @skillId)
	BEGIN
		UPDATE [pledge_skill] SET [skill_level] = @skillLevel WHERE [pledge_id] = @pledgeId AND [skill_id] = @skillId
	END
	ELSE
	BEGIN
		INSERT INTO [pledge_skill] ([pledge_id], [skill_id], [skill_level]) VALUES (@pledgeId, @skillId, @skillLevel)
	END
END

GO

CREATE PROCEDURE lin_LoadPledgeSkills
(
	@pledgeId as INT
)
AS

SET NOCOUNT ON;
SELECT [skill_id], [skill_level]  FROM [pledge_skill] WHERE [pledge_id] = @pledgeId
GO

CREATE TABLE user_wedding
	(
	[char_id] int NOT NULL DEFAULT 0,
	[state] int NOT NULL DEFAULT 0,
	[partner_id] int NOT NULL DEFAULT 0,
	[partner_name] nvarchar(25) NOT NULL DEFAULT N'',
	[penalty_timeout] int NOT NULL DEFAULT 0,
	[teleport_timeout] int NOT NULL DEFAULT 0,
	[wedding_time] int NOT NULL DEFAULT 0
	) 	
go

CREATE PROCEDURE lin_LoadWeddingStatus
(
	@charId as INT
)
AS

SET NOCOUNT ON;
SELECT [state], [partner_id], [partner_name], [penalty_timeout], [teleport_timeout], [wedding_time] FROM [user_wedding] WHERE [char_id] = @charId
GO

CREATE PROCEDURE lin_SaveWeddingStatus
(
	@charId as INT,
	@state as INT,
	@partnerId as INT,
	@partnerName as nvarchar(25),
	@penaltyTimeout as INT,
	@teleportTimeout as INT,
	@weddingTime as INT
)
AS

SET NOCOUNT ON;
BEGIN
	IF EXISTS(SELECT * FROM [dbo].[user_wedding] WHERE [char_id] = @charId)
	BEGIN
		UPDATE [user_wedding] SET [state]=@state, [partner_id] = @partnerId, [partner_name] = @partnerName, [penalty_timeout] = @penaltyTimeout, [teleport_timeout] = @teleportTimeout, [wedding_time] = @weddingTime WHERE [char_id] = @charId
	END
	ELSE
	BEGIN
		INSERT INTO [user_wedding] ([char_id], [state], [partner_id], [partner_name], [penalty_timeout], [teleport_timeout], [wedding_time]) VALUES (@charId, @state, @partnerId, @partnerName, @penaltyTimeout, @teleportTimeout, @weddingTime)
	END
END

go

ALTER TABLE dbo.user_data ADD
	long_hardware_id nvarchar(50) NOT NULL DEFAULT N'00000000000000000000000000000000'
GO

CREATE PROCEDURE lin_SaveHardwareId
(
 @char_id INT,
 @hwid NVARCHAR(50)
)
AS

	SET NOCOUNT ON;
	UPDATE [user_data] SET [long_hardware_id] = @hwid WHERE [char_id] = @char_id
go

--New HWID system

CREATE TABLE BannedHardwareId
	(
	[hardware_id] nvarchar(50) NOT NULL DEFAULT N'00000000000000000000000000000000',
	[account] nvarchar(25) NOT NULL DEFAULT N'',
	[gm] nvarchar(25) NOT NULL DEFAULT N'',
	[timeout] int NOT NULL DEFAULT 0
	) 	
go

CREATE PROCEDURE lin_BanHardwareId
(
 @hwid NVARCHAR(50),
 @account as nvarchar(25),
 @gm as nvarchar(25),
 @timeout INT
)
AS

SET NOCOUNT ON;
BEGIN
	IF EXISTS(SELECT * FROM [dbo].[BannedHardwareId] WHERE [hardware_id] = @hwid)
	BEGIN
		UPDATE [BannedHardwareId] SET [account]=@account, [gm] = @gm, [timeout] = @timeout WHERE [hardware_id] = @hwid
	END
	ELSE
	BEGIN
		INSERT INTO [BannedHardwareId] ([hardware_id], [account], [gm], [timeout]) VALUES (@hwid, @account, @gm, @timeout)
	END
END
go

CREATE PROCEDURE lin_UnBanHardwareId
(
 @hwid NVARCHAR(50)
)
AS

SET NOCOUNT ON;

DELETE FROM [BannedHardwareId] WHERE [hardware_id] = @hwid

go


CREATE PROCEDURE lin_LoadBannedHardwareId
AS

SET NOCOUNT ON;

SELECT [hardware_id], [account], [gm], [timeout] FROM [BannedHardwareId]

go

DROP TABLE [dbo].[HardwareId_Lock]
GO

DROP TABLE [dbo].[HardwareId_Ban]
GO


DROP PROCEDURE lin_ProcessHardwareIdLock
GO

CREATE TABLE [dbo].[LockedHardwareId](
	[char_id] [int] NOT NULL DEFAULT 0,
	[account_id] [int] NOT NULL DEFAULT 0,
	[hardware_id] [nvarchar](33) NOT NULL DEFAULT N'',
	[locked_by] [nvarchar](255) NOT NULL DEFAULT N''
) ON [PRIMARY]

GO

CREATE PROCEDURE lin_ProcessLockedHardwareId
(
	@accountId as INT,
	@charId as INT,
	@hardwareId as nvarchar(33),
	@locker as nvarchar(255)
)
AS

SET NOCOUNT ON;
BEGIN
	IF(@hardwareId = N'')
	BEGIN
	--remove lock
		IF(@accountId != 0)
		BEGIN
			DELETE FROM [LockedHardwareId] WHERE [account_id] = @accountId
		END
		IF(@charId != 0)
		BEGIN
			DELETE FROM [LockedHardwareId] WHERE [char_id] = @charId
		END
	END
	ELSE IF(@hardwareId != N'')
	BEGIN
		--Add/Update lock
		IF(@accountId != 0)
		BEGIN
			IF EXISTS(SELECT * FROM [LockedHardwareId] WHERE [account_id] = @accountId)
			BEGIN
				UPDATE [LockedHardwareId] SET [hardware_id] = @hardwareId, [locked_by] = @locker WHERE [account_id] = @accountId
			END
			ELSE
			BEGIN
				INSERT INTO [LockedHardwareId] ([account_id], [char_id], [hardware_id], [locked_by]) VALUES (@accountId, @charId, @hardwareId, @locker)
			END
		END
		IF(@charId != 0)
		BEGIN
			IF EXISTS(SELECT * FROM [LockedHardwareId] WHERE [char_id] = @charId)
			BEGIN
				UPDATE [LockedHardwareId] SET [hardware_id] = @hardwareId, [locked_by] = @locker WHERE [char_id] = @charId
			END
			ELSE
			BEGIN
				INSERT INTO [LockedHardwareId] ([account_id], [char_id], [hardware_id], [locked_by]) VALUES (@accountId, @charId, @hardwareId, @locker)
			END
		END
	END
END

go

--Manor fix
ALTER TABLE dbo.manor_info ADD
	last_changed_t int NOT NULL DEFAULT 0
GO

ALTER TABLE dbo.manor_info
	DROP COLUMN last_changed
GO

ALTER  PROCEDURE [dbo].[lin_SaveManorInfo]
(        
 @manor_id INT,
 @residence_id INT,
 @AdenaSeedSell INT,
 @AdenaCropBuy INT,
 @change_state TINYINT,
 @last_changed INT
)        
AS        
        
SET NOCOUNT ON        

IF EXISTS(SELECT * FROM manor_info WHERE manor_id = @manor_id)
BEGIN
	UPDATE
		manor_info
	SET
		residence_id = @residence_id,
		adena_vault = @AdenaSeedSell,
		crop_buy_vault = @AdenaCropBuy,
		change_state = @change_state,
		last_changed_t = @last_changed
	WHERE
		manor_id = @manor_id
END
ELSE
BEGIN
	INSERT INTO manor_info (manor_id, residence_id, adena_vault, crop_buy_vault, change_state, last_changed_t) 
	VALUES (@manor_id, @residence_id,@AdenaSeedSell,@AdenaCropBuy, @change_state, @last_changed)
END

GO

ALTER  PROCEDURE [dbo].[lin_LoadManorInfo]
(        
 @manor_id INT
)        
AS        
        
SET NOCOUNT ON        

IF EXISTS(SELECT * FROM manor_info WHERE manor_id = @manor_id)
BEGIN
	SELECT 
		residence_id, adena_vault, crop_buy_vault, change_state, last_changed_t
	FROM 
		manor_info
	WHERE 
		manor_id = @manor_id
END
ELSE
BEGIN
	SELECT 0, 0, 0, 0, 0

END

GO

ALTER TABLE dbo.user_data ADD
	long_hardware_id nvarchar(50) NOT NULL DEFAULT N'00000000000000000000000000000000'
GO

CREATE PROCEDURE lin_SaveHardwareId
(
 @char_id INT,
 @hwid NVARCHAR(50)
)
AS

	SET NOCOUNT ON;
	UPDATE [user_data] SET [long_hardware_id] = @hwid WHERE [char_id] = @char_id
go

ALTER TABLE dbo.user_data ADD
	name_color int NOT NULL DEFAULT 0
GO

ALTER TABLE dbo.user_data ADD
	title_color int NOT NULL DEFAULT 0
GO


ALTER PROCEDURE [dbo].[lin_GetNoblessTop10]
(
@class INT,
@season INT
)
AS  
SET NOCOUNT ON  

SELECT TOP 10 WITH TIES u.char_id, u.char_name, o.point, o.match_count 
FROM user_data u, olympiad_result o 
WHERE o.class = @class AND u.char_id = o.char_id AND o.season = @season
ORDER BY point DESC, match_count DESC

GO

--SetLastTaxUpdate fix

DROP TABLE dbo.castle_tax
go

CREATE TABLE castle_tax
	(
	income_update_t INT NOT NULL DEFAULT 0,
	tax_change_t INT NOT NULL DEFAULT 0,
	manor_reset_t INT NOT NULL DEFAULT 0
	) 	
go

CREATE PROCEDURE lin_SetLastTaxUpdateEx
(
 @incomeUpdate int,
 @taxChange int,
 @manorReset int
)
AS
	SET NOCOUNT ON;
BEGIN
  UPDATE castle_tax SET income_update_t = @incomeUpdate, tax_change_t = @taxChange, manor_reset_t = @manorReset
  
  IF @@ROWCOUNT=0
  BEGIN
    INSERT INTO castle_tax (income_update_t, tax_change_t, manor_reset_t) VALUES(@incomeUpdate, @taxChange, @manorReset)
  END
END

GO

CREATE TABLE [dbo].[CharacterLock](
	[char_id] [int] NOT NULL DEFAULT 0,
	[code_hash] [nvarchar](33) NOT NULL DEFAULT N'',
	[hardware_id] [nvarchar](33) NOT NULL DEFAULT N''
) ON [PRIMARY]

GO

-- Daily PvP feature

ALTER TABLE dbo.user_data ADD
	daily_pvp int NOT NULL DEFAULT 0
GO

ALTER TABLE dbo.user_data ADD
	daily_pvp_timestamp int NOT NULL DEFAULT 0
GO

CREATE TABLE [dbo].[DailyPvP](
	[char_id] [int] NOT NULL DEFAULT 0,
	[hardware_id] [nvarchar](33) NOT NULL DEFAULT N'',
	[timestamp] [int] NOT NULL DEFAULT 0
) ON [PRIMARY]

GO

CREATE PROCEDURE lin_AddDailyPvPEntry
(
 @charId INT,
 @hwid [nvarchar](33),
 @timestamp INT
)
AS
-- SET NOCOUNT ON added to prevent extra result sets from
-- interfering with SELECT statements.
SET NOCOUNT ON;
UPDATE [DailyPvP] SET [timestamp] = @timestamp WHERE [char_id] = @charId OR [hardware_id] = @hwid
IF @@ROWCOUNT=0
BEGIN
    INSERT INTO [DailyPvP] ([char_id], [hardware_id], [timestamp] ) VALUES (@charId, @hwid, @timestamp )
END

GO

CREATE TABLE [SubStack](
	[char_id] [int] NOT NULL DEFAULT 0,
	[baseClass0] [int] NOT NULL DEFAULT -1,
	[baseClass1] [int] NOT NULL DEFAULT -1,
	[baseClass2] [int] NOT NULL DEFAULT -1,
	[baseClass3] [int] NOT NULL DEFAULT -1,
	[stackClass0] [int] NOT NULL DEFAULT -1,
	[stackClass1] [int] NOT NULL DEFAULT -1,
	[stackClass2] [int] NOT NULL DEFAULT -1,
	[stackClass3] [int] NOT NULL DEFAULT -1
) ON [PRIMARY]

GO

CREATE PROCEDURE lin_SaveSubStack
(
 @charId INT,
 @classIndex INT,
 @baseClass INT,
 @stackClass INT
)
AS
SET NOCOUNT ON;

IF @classIndex = 0
BEGIN
	UPDATE [SubStack] SET [baseClass0] = @baseClass, [stackClass0] = @stackClass WHERE [char_id] = @charId
	IF @@ROWCOUNT=0
	BEGIN
		INSERT INTO [SubStack] ([char_id], [baseClass0], [stackClass0] ) VALUES (@charId, @baseClass, @stackClass )
	END
END
ELSE IF @classIndex = 1
BEGIN
	UPDATE [SubStack] SET [baseClass1] = @baseClass, [stackClass1] = @stackClass WHERE [char_id] = @charId
	IF @@ROWCOUNT=0
	BEGIN
		INSERT INTO [SubStack] ([char_id], [baseClass1], [stackClass1] ) VALUES (@charId, @baseClass, @stackClass )
	END
END
ELSE IF @classIndex = 2
BEGIN
	UPDATE [SubStack] SET [baseClass2] = @baseClass, [stackClass2] = @stackClass WHERE [char_id] = @charId
	IF @@ROWCOUNT=0
	BEGIN
		INSERT INTO [SubStack] ([char_id], [baseClass2], [stackClass2] ) VALUES (@charId, @baseClass, @stackClass )
	END
END
ELSE IF @classIndex = 3
BEGIN
	UPDATE [SubStack] SET [baseClass3] = @baseClass, [stackClass3] = @stackClass WHERE [char_id] = @charId
	IF @@ROWCOUNT=0
	BEGIN
		INSERT INTO [SubStack] ([char_id], [baseClass3], [stackClass3] ) VALUES (@charId, @baseClass, @stackClass )
	END
END

GO

CREATE TABLE [user_instance](
	[char_id] [int] NOT NULL DEFAULT 0,
	[instance_id] [int] NOT NULL DEFAULT 0,
	[time_spent] [int] NOT NULL DEFAULT 0,
	[reuse_timeout] [int] NOT NULL DEFAULT 0
) ON [PRIMARY]

GO

CREATE PROCEDURE lin_SaveInstanceTime
(
 @charId INT,
 @instanceId INT,
 @timeSpent INT,
 @reuseTimeout INT
)
AS
SET NOCOUNT ON;

UPDATE [user_instance] SET [instance_id] = @instanceId, [time_spent] = @timeSpent, [reuse_timeout] = @reuseTimeout WHERE [char_id] = @charId
IF @@ROWCOUNT=0
BEGIN
	INSERT INTO [user_instance] ([char_id], [instance_id], [time_spent], [reuse_timeout] ) VALUES (@charId, @instanceId, @timeSpent, @reuseTimeout )
END

GO

/****** Object:  StoredProcedure [dbo].[lin_NominateHeroes]    Script Date: 2015-03-03 13:28:57 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

ALTER PROCEDURE [dbo].[lin_NominateHeroes]
(
@now_season INT,
@new_step INT,
@new_season_start_time INT
)
AS
SET NOCOUNT ON

-- delete duplicated entries from olympiad_result for current season
DELETE FROM olympiad_result where season = @now_season

UPDATE user_nobless
SET hero_type = 0

CREATE TABLE #hero_candidate (char_id INT, main_class INT, match_count INT, olympiad_point INT)
INSERT INTO #hero_candidate 
	SELECT un.char_id, ud.subjob0_class, un.match_count, un.olympiad_point 
	FROM user_nobless un, user_data ud 
	WHERE un.char_id = ud.char_id AND un.match_count >= 9 AND un.olympiad_win_count >= 1

CREATE TABLE #highest_score (main_class INT, olympiad_point INT, match_count INT)

DECLARE @hero_candidate_class INT
DECLARE hero_candidate_cursor CURSOR FOR
SELECT DISTINCT main_class FROM #hero_candidate

OPEN hero_candidate_cursor

FETCH NEXT FROM hero_candidate_cursor INTO @hero_candidate_class

WHILE @@FETCH_STATUS = 0
BEGIN

INSERT INTO #highest_score
	SELECT TOP 1 main_class, olympiad_point, match_count 
	FROM #hero_candidate 
	WHERE main_class = @hero_candidate_class
	ORDER BY olympiad_point DESC, match_count DESC

	FETCH NEXT FROM hero_candidate_cursor INTO @hero_candidate_class
END

CLOSE hero_candidate_cursor
DEALLOCATE hero_candidate_cursor

CREATE TABLE #highest_score_nobless (char_id INT, main_class INT)
INSERT INTO #highest_score_nobless
	SELECT c.char_id, c.main_class FROM #hero_candidate c, #highest_score s
		WHERE c.main_class = s.main_class AND c.olympiad_point = s.olympiad_point AND c.match_count = s.match_count

CREATE TABLE #hero (char_id INT, main_class INT)
INSERT INTO #hero
	SELECT char_id, main_class  FROM #highest_score_nobless WHERE main_class IN (SELECT main_class FROM #highest_score_nobless GROUP BY main_class HAVING COUNT(main_class) = 1)

UPDATE user_nobless
SET hero_type = 1, win_count = win_count+1
WHERE char_id IN (SELECT char_id FROM #hero)

UPDATE user_nobless
SET previous_point = olympiad_point

UPDATE user_nobless
SET olympiad_point = 0, match_count = 0, olympiad_win_count = 0, olympiad_lose_count = 0

INSERT INTO olympiad_result
	SELECT @now_season, main_class, char_id, olympiad_point, match_count FROM #hero_candidate

INSERT INTO olympiad
	(step, season_start_time) VALUES (@new_step, @new_season_start_time)

SELECT @@IDENTITY

GO

-- L2Network Vote
CREATE TABLE [L2NetworkVote](
	[identity] NVARCHAR(32) NOT NULL DEFAULT N'',
	[last_vote_time] [int] NOT NULL DEFAULT 0,
	[last_reward_time] [int] NOT NULL DEFAULT 0
) ON [PRIMARY]

GO

CREATE PROCEDURE lin_SaveL2NetworkVote
(
 @identity NVARCHAR(32),
 @lastVoteTime int,
 @lastRewardTime int
)
AS
SET NOCOUNT ON;
UPDATE [L2NetworkVote] SET [last_vote_time] = @lastVoteTime, [last_reward_time] = @lastRewardTime WHERE [identity] = @identity
IF @@ROWCOUNT=0
BEGIN
    INSERT INTO [L2NetworkVote] ([identity], [last_vote_time], [last_reward_time] ) VALUES (@identity, @lastVoteTime, @lastRewardTime )
END

GO

CREATE PROCEDURE lin_LoadL2NetworkVote
(
 @identity NVARCHAR(32)
)
AS
SET NOCOUNT ON;
SELECT [last_vote_time], [last_reward_time] FROM [L2NetworkVote] WHERE [identity] = @identity

GO

-- Visual Armor
ALTER TABLE dbo.user_data ADD
	visual_armor_id int NOT NULL DEFAULT 0
GO

-- Rebirth System
CREATE TABLE [user_rebirth](
	[char_id] [int] NOT NULL DEFAULT 0,
	[subjob_id] [int] NOT NULL DEFAULT 0,
	[rebirth_count] [int] NOT NULL DEFAULT 0,
	[available_points] [int] NOT NULL DEFAULT 0,
	[bonus_int] [int] NOT NULL DEFAULT 0,
	[bonus_str] [int] NOT NULL DEFAULT 0,
	[bonus_con] [int] NOT NULL DEFAULT 0,
	[bonus_men] [int] NOT NULL DEFAULT 0,
	[bonus_dex] [int] NOT NULL DEFAULT 0,
	[bonus_wit] [int] NOT NULL DEFAULT 0	
) ON [PRIMARY]

GO

CREATE PROCEDURE lin_SaveRebirth
(
 @charId INT,
 @subjobId INT,
 @rebirthCount INT,
 @availablePoints INT,
 @bonusInt INT,
 @bonusStr INT,
 @bonusCon INT,
 @bonusMen INT,
 @bonusDex INT,
 @bonusWit INT
)
AS
SET NOCOUNT ON;
UPDATE [user_rebirth] SET [rebirth_count] = @rebirthCount, [available_points] = @availablePoints, [bonus_int] = @bonusInt, [bonus_str] = @bonusStr, [bonus_con] = @bonusCon, [bonus_men] = @bonusMen, [bonus_dex] = @bonusDex, [bonus_wit] = @bonusWit WHERE [char_id] = @charId AND [subjob_id] = @subjobId
IF @@ROWCOUNT=0
BEGIN
    INSERT INTO [user_rebirth] ([char_id], [subjob_id], [rebirth_count], [available_points], [bonus_int], [bonus_str], [bonus_con], [bonus_men], [bonus_dex], [bonus_wit] ) VALUES (@charId, @subjobId, @rebirthCount, @availablePoints, @bonusInt, @bonusStr, @bonusCon, @bonusMen, @bonusDex, @bonusWit )
END

GO

-- DBNpcMaker
CREATE TABLE [DBNpcMaker](
	[id] INT IDENTITY(1, 1) NOT NULL,
	[npc_id] INT NOT NULL DEFAULT 0,
	[spawn_x] INT NOT NULL DEFAULT 0,
	[spawn_y] INT NOT NULL DEFAULT 0,
	[spawn_z] INT NOT NULL DEFAULT 0,
	[heading] INT NOT NULL DEFAULT 0,
	[start_time] INT NOT NULL DEFAULT DATEDIFF(s, '1970-01-01 00:00:00', GETUTCDATE()),
	[end_time] INT NOT NULL DEFAULT DATEDIFF(s, '1970-01-01 00:00:00', GETUTCDATE()) + 2592000,
	[comment] NVARCHAR(255) NOT NULL DEFAULT N''
	 
) ON [PRIMARY]
GO

CREATE PROCEDURE lin_LoadDBNpcMaker

AS
SET NOCOUNT ON;
SELECT [npc_id], [spawn_x], [spawn_y], [spawn_z], [heading], [start_time], [end_time] FROM [DBNpcMaker]

GO

-- PIN System
CREATE TABLE [user_pin] (
  [char_id] int DEFAULT (0) NOT NULL,
  [pin] int DEFAULT (0) NOT NULL
)
GO

CREATE PROCEDURE lin_SetPIN
(
	@charId as INT,
	@pin as INT
)
AS

SET NOCOUNT ON;

IF EXISTS(SELECT * FROM [user_pin] WHERE [char_id] = @charId)
BEGIN
	UPDATE [user_pin] SET [pin] = @pin WHERE [char_id] = @charId
END
ELSE
BEGIN
	INSERT INTO [user_pin] ([char_id], [pin]) VALUES (@charId, @pin)
END

GO

CREATE PROCEDURE lin_LoadPINS
AS

SET NOCOUNT ON;
SELECT [char_id], [pin] FROM [user_pin]

GO

ALTER TABLE dbo.user_pin ADD
	timestamp int NOT NULL DEFAULT 0
go
	
ALTER PROCEDURE [dbo].[lin_LoadPINS]
AS

SET NOCOUNT ON;
SELECT [char_id], [pin], [timestamp] FROM [user_pin]
go

ALTER PROCEDURE [dbo].[lin_SetPIN]
(
	@charId as INT,
	@pin as INT,
	@timestamp as INT
)
AS

SET NOCOUNT ON;

IF EXISTS(SELECT * FROM [user_pin] WHERE [char_id] = @charId)
BEGIN
	UPDATE [user_pin] SET [pin] = @pin, [timestamp] = @timestamp WHERE [char_id] = @charId
END
ELSE
BEGIN
	INSERT INTO [user_pin] ([char_id], [pin], [timestamp]) VALUES (@charId, @pin, @timestamp)
END

go

CREATE PROCEDURE [dbo].[lin_GetPledgeTotalPvP]
(
	@pledgeLevel	INT
)
AS
SET NOCOUNT ON

DECLARE @pledgeId as int
DECLARE @pvpCount as int
DECLARE @pledgeName as NVARCHAR(32)

CREATE TABLE #pledges (pledge_id int, pledge_name NVARCHAR(32), pvp_count INT)

DECLARE pledgeCur CURSOR FOR
SELECT [pledge_id] FROM [dbo].[pledge] WHERE [skill_level] >= @pledgeLevel
OPEN pledgeCur
FETCH NEXT FROM pledgeCur INTO @pledgeId
WHILE (@@FETCH_STATUS <> -1)
BEGIN
SELECT @pledgeName = [name] FROM [dbo].[pledge] WHERE [pledge_id] = @pledgeId
SELECT @pvpCount = SUM([duel]) FROM [user_data] WHERE [pledge_id] = @pledgeId
INSERT INTO #pledges SELECT @pledgeId, @pledgeName, @pvpCount
FETCH NEXT FROM pledgeCur INTO @pledgeId
END
CLOSE pledgeCur
DEALLOCATE pledgeCur

SELECT pledge_id, pledge_name, pvp_count FROM #pledges ORDER BY pvp_count DESC

go