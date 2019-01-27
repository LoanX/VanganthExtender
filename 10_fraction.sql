use lin2world
go

ALTER TABLE [user_data] ADD
	fraction_id int NOT NULL DEFAULT 0,
	fraction_points int NOT NULL DEFAULT 0,
	fraction_timestamp int NOT NULL DEFAULT 0
go

CREATE PROCEDURE lin_UpdateUserFraction
(
	@charId as INT,
	@fractionId as INT,
	@fractionPoints as INT,
	@fractionTimestamp as INT
)
AS

SET NOCOUNT ON;
BEGIN
	UPDATE [user_data] SET [fraction_id] = @fractionId, [fraction_points] = @fractionPoints, [fraction_timestamp] = @fractionTimestamp WHERE [char_id] = @charId
END

go

CREATE PROCEDURE lin_LoadUserFraction
(
	@charId as INT
)
AS

SET NOCOUNT ON;
BEGIN
	SELECT [fraction_id], [fraction_points], [fraction_timestamp] FROM [user_data] where [char_id] = @charId
END

go

CREATE TABLE FractionData
	(
	[fraction_id] int NOT NULL DEFAULT 0,
	[points] int NOT NULL DEFAULT 0,
	[online_members] int NOT NULL DEFAULT 0
	) 
	
go

CREATE PROCEDURE lin_LoadFractionData
(
	@fractionId as INT
)
AS

SET NOCOUNT ON;
BEGIN
	SELECT [points] FROM [FractionData] WHERE [fraction_id] = @fractionId
END

go

CREATE PROCEDURE lin_SaveFractionData
(
	@fractionId as INT,
	@points as INT,
	@onlineMembers as INT
)
AS

SET NOCOUNT ON;
BEGIN
	--Add/Update fraction
	IF EXISTS(SELECT * FROM [FractionData] WHERE [fraction_id] = @fractionId)
	BEGIN
		UPDATE [FractionData] SET [points] = @points, [online_members] = @onlineMembers WHERE [fraction_id] = @fractionId
	END
	ELSE
	BEGIN
		INSERT INTO [FractionData] ([fraction_id], [points], [online_members]) VALUES (@fractionId, @points, @onlineMembers)
	END
END

go