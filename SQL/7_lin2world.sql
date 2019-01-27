USE [lin2world]
GO

CREATE TABLE [dbo].[account_ch2] (
  [account] varchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[agit] (
  [id] int NOT NULL,
  [name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [pledge_id] int DEFAULT 0 NULL,
  [auction_price] int DEFAULT 0 NULL,
  [auction_date] int DEFAULT 0 NULL,
  [hp_stove] tinyint DEFAULT 0 NULL,
  [mp_flame] tinyint DEFAULT 0 NULL,
  [hatcher] tinyint DEFAULT 0 NULL,
  [hp_stove_expire] int DEFAULT 0 NULL,
  [mp_flame_expire] int DEFAULT 0 NULL,
  [status] tinyint DEFAULT 2 NULL,
  [next_war_time] int DEFAULT 0 NULL,
  [auction_id] int NULL,
  [last_price] int NULL,
  [last_cancel] int NULL,
  [teleport_level] int DEFAULT 0 NULL,
  [teleport_expire] int DEFAULT 0 NULL,
  [next_cost] int NULL,
  [cost_fail] int NULL,
  [tax_rate] int DEFAULT 0 NOT NULL,
  [shop_income] int DEFAULT 0 NOT NULL,
  [tax_rate_to_change] smallint DEFAULT (-1) NOT NULL,
  [tax_child_rate] smallint DEFAULT 0 NOT NULL,
  [tax_child_rate_to_change] smallint DEFAULT (-1) NOT NULL,
  [shop_income_temp] int DEFAULT 0 NOT NULL,
  [team_battle_status] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[agit_adena] (
  [agit_id] int NULL,
  [pledge_id] int NULL,
  [auction_id] int NULL,
  [reason] int NULL,
  [adena] int NULL,
  [log_id] int IDENTITY(1, 1) NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[agit_auction] (
  [agit_id] int NULL,
  [auction_id] int IDENTITY(1, 1) NOT NULL,
  [auction_desc] nvarchar(200) COLLATE Korean_Wansung_CI_AS NULL,
  [min_price] int NULL,
  [accepted_bid] int NULL,
  [auction_time] int NULL,
  [auction_tax] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[agit_bid] (
  [auction_id] int NULL,
  [attend_id] int IDENTITY(1, 1) NOT NULL,
  [attend_price] int NULL,
  [attend_pledge_id] int NULL,
  [attend_date] datetime DEFAULT getdate() NULL,
  [attend_time] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[agit_deco] (
  [agit_id] int NOT NULL,
  [type] int NOT NULL,
  [id] int NOT NULL,
  [name] varchar(256) COLLATE Korean_Wansung_CI_AS NULL,
  [level] int NULL,
  [expire] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[Alliance] (
  [id] int IDENTITY(1, 1) NOT NULL,
  [name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [master_pledge_id] int DEFAULT 0 NOT NULL,
  [oust_time] int DEFAULT 0 NOT NULL,
  [crest_id] int DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[bookmark] (
  [char_id] int NOT NULL,
  [name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [world] int NULL,
  [x] int NULL,
  [y] int NULL,
  [z] int NULL,
  [bookmarkid] int IDENTITY(1, 1) NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[builder_account] (
  [account_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [default_builder] int DEFAULT 0 NOT NULL,
  [account_id] int DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[castle] (
  [id] int NOT NULL,
  [name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [pledge_id] int DEFAULT 0 NOT NULL,
  [next_war_time] int DEFAULT 0 NOT NULL,
  [tax_rate] smallint DEFAULT 0 NOT NULL,
  [type] tinyint DEFAULT 1 NOT NULL,
  [status] tinyint DEFAULT 0 NOT NULL,
  [crop_income] int DEFAULT 0 NOT NULL,
  [shop_income] int DEFAULT 0 NOT NULL,
  [siege_elapsed_time] int DEFAULT 0 NOT NULL,
  [tax_child_rate] int DEFAULT 0 NOT NULL,
  [shop_income_temp] int DEFAULT 0 NOT NULL,
  [tax_rate_to_change] smallint DEFAULT (-1) NOT NULL,
  [tax_child_rate_to_change] smallint DEFAULT (-1) NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[castle_crop] (
  [castle_id] int NOT NULL,
  [item_type] int NOT NULL,
  [dropRate] int NOT NULL,
  [price] int NOT NULL,
  [level] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[castle_tax] (
  [income_update] datetime NOT NULL,
  [tax_change] datetime NOT NULL,
  [manor_reset] datetime DEFAULT getdate() NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[castle_war] (
  [castle_id] int NOT NULL,
  [pledge_id] int NOT NULL,
  [type] tinyint NOT NULL,
  [propose_time] int DEFAULT 0 NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[ch2_temp] (
  [uid] int NOT NULL,
  [account] varchar(14) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [last_login] datetime NULL,
  [last_world] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[ch3_lotto_char] (
  [char_id] int NOT NULL,
  [item_count] int NULL,
  [round_number] int NULL,
  [item_id] int NULL,
  [number_flag] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[char_pet] (
  [char_id] int NOT NULL,
  [pet_id] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[class_list] (
  [class] smallint NULL,
  [name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [race] tinyint NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[control_tower] (
  [name] varchar(256) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [residence_id] int NOT NULL,
  [control_level] int DEFAULT 0 NULL,
  [hp] int NOT NULL,
  [status] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[door] (
  [name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [hp] int NOT NULL,
  [max_hp] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[dt_adena_rank_temp] (
  [char_id] int NOT NULL,
  [amount] numeric(18, 0) NOT NULL,
  [rank] int IDENTITY(1, 1) NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[err_item_1] (
  [etc_num5] int NULL,
  [str_actor] varchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [actor] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[err_pet1_9_28] (
  [uiid] int NOT NULL,
  [char_id] int NOT NULL,
  [item_type] int NOT NULL,
  [amount] int NOT NULL,
  [enchant] int NOT NULL,
  [eroded] int NOT NULL,
  [bless] tinyint NOT NULL,
  [ident] tinyint NOT NULL,
  [wished] tinyint NOT NULL,
  [warehouse] int NOT NULL,
  [act_time] datetime NULL,
  [log_id] smallint NULL,
  [actor] int NULL,
  [actor_account] int NULL,
  [target] int NULL,
  [target_account] int NULL,
  [location_x] int NULL,
  [location_y] int NULL,
  [location_z] int NULL,
  [etc_str1] varchar(200) COLLATE Korean_Wansung_CI_AS NULL,
  [etc_str2] varchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [etc_str3] varchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [etc_num1] float NULL,
  [etc_num2] float NULL,
  [etc_num3] int NULL,
  [etc_num4] int NULL,
  [etc_num5] int NULL,
  [etc_num6] int NULL,
  [etc_num7] int NULL,
  [etc_num8] int NULL,
  [etc_num9] int NULL,
  [etc_num10] int NULL,
  [STR_actor] varchar(48) COLLATE Korean_Wansung_CI_AS NULL,
  [STR_actor_account] varchar(32) COLLATE Korean_Wansung_CI_AS NULL,
  [STR_target] varchar(48) COLLATE Korean_Wansung_CI_AS NULL,
  [STR_target_account] varchar(32) COLLATE Korean_Wansung_CI_AS NULL,
  [item_id] int NULL,
  [iid] int NULL,
  [at] datetime NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[err_pet1_9_30] (
  [uiid] int NOT NULL,
  [char_id] int NOT NULL,
  [item_type] int NOT NULL,
  [amount] int NOT NULL,
  [enchant] int NOT NULL,
  [eroded] int NOT NULL,
  [bless] tinyint NOT NULL,
  [ident] tinyint NOT NULL,
  [wished] tinyint NOT NULL,
  [warehouse] int NOT NULL,
  [act_time] datetime NULL,
  [log_id] smallint NULL,
  [actor] int NULL,
  [actor_account] int NULL,
  [target] int NULL,
  [target_account] int NULL,
  [location_x] int NULL,
  [location_y] int NULL,
  [location_z] int NULL,
  [etc_str1] varchar(200) COLLATE Korean_Wansung_CI_AS NULL,
  [etc_str2] varchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [etc_str3] varchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [etc_num1] float NULL,
  [etc_num2] float NULL,
  [etc_num3] int NULL,
  [etc_num4] int NULL,
  [etc_num5] int NULL,
  [etc_num6] int NULL,
  [etc_num7] int NULL,
  [etc_num8] int NULL,
  [etc_num9] int NULL,
  [etc_num10] int NULL,
  [STR_actor] varchar(48) COLLATE Korean_Wansung_CI_AS NULL,
  [STR_actor_account] varchar(32) COLLATE Korean_Wansung_CI_AS NULL,
  [STR_target] varchar(48) COLLATE Korean_Wansung_CI_AS NULL,
  [STR_target_account] varchar(32) COLLATE Korean_Wansung_CI_AS NULL,
  [item_id] int NULL,
  [iid] int NULL,
  [at] datetime NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[err_pet2_9_28] (
  [uiid] int NOT NULL,
  [char_id] int NOT NULL,
  [item_type] int NOT NULL,
  [amount] int NOT NULL,
  [enchant] int NOT NULL,
  [eroded] int NOT NULL,
  [bless] tinyint NOT NULL,
  [ident] tinyint NOT NULL,
  [wished] tinyint NOT NULL,
  [warehouse] int NOT NULL,
  [act_time] datetime NULL,
  [log_id] smallint NULL,
  [actor] int NULL,
  [actor_account] int NULL,
  [target] int NULL,
  [target_account] int NULL,
  [location_x] int NULL,
  [location_y] int NULL,
  [location_z] int NULL,
  [etc_str1] varchar(200) COLLATE Korean_Wansung_CI_AS NULL,
  [etc_str2] varchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [etc_str3] varchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [etc_num1] float NULL,
  [etc_num2] float NULL,
  [etc_num3] int NULL,
  [etc_num4] int NULL,
  [etc_num5] int NULL,
  [etc_num6] int NULL,
  [etc_num7] int NULL,
  [etc_num8] int NULL,
  [etc_num9] int NULL,
  [etc_num10] int NULL,
  [STR_actor] varchar(48) COLLATE Korean_Wansung_CI_AS NULL,
  [STR_actor_account] varchar(32) COLLATE Korean_Wansung_CI_AS NULL,
  [STR_target] varchar(48) COLLATE Korean_Wansung_CI_AS NULL,
  [STR_target_account] varchar(32) COLLATE Korean_Wansung_CI_AS NULL,
  [item_id] int NULL,
  [iid] int NULL,
  [at] datetime NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[err_pet2_9_30] (
  [uiid] int NOT NULL,
  [char_id] int NOT NULL,
  [item_type] int NOT NULL,
  [amount] int NOT NULL,
  [enchant] int NOT NULL,
  [eroded] int NOT NULL,
  [bless] tinyint NOT NULL,
  [ident] tinyint NOT NULL,
  [wished] tinyint NOT NULL,
  [warehouse] int NOT NULL,
  [act_time] datetime NULL,
  [log_id] smallint NULL,
  [actor] int NULL,
  [actor_account] int NULL,
  [target] int NULL,
  [target_account] int NULL,
  [location_x] int NULL,
  [location_y] int NULL,
  [location_z] int NULL,
  [etc_str1] varchar(200) COLLATE Korean_Wansung_CI_AS NULL,
  [etc_str2] varchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [etc_str3] varchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [etc_num1] float NULL,
  [etc_num2] float NULL,
  [etc_num3] int NULL,
  [etc_num4] int NULL,
  [etc_num5] int NULL,
  [etc_num6] int NULL,
  [etc_num7] int NULL,
  [etc_num8] int NULL,
  [etc_num9] int NULL,
  [etc_num10] int NULL,
  [STR_actor] varchar(48) COLLATE Korean_Wansung_CI_AS NULL,
  [STR_actor_account] varchar(32) COLLATE Korean_Wansung_CI_AS NULL,
  [STR_target] varchar(48) COLLATE Korean_Wansung_CI_AS NULL,
  [STR_target_account] varchar(32) COLLATE Korean_Wansung_CI_AS NULL,
  [item_id] int NULL,
  [iid] int NULL,
  [at] datetime NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[err_skill_1] (
  [class_name] nvarchar(255) COLLATE Korean_Wansung_CI_AS NULL,
  [class] int NULL,
  [lev] int NULL,
  [skill_id] int NULL,
  [skill_name] nvarchar(255) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[event_items] (
  [char_id] int NULL,
  [class_id] int NULL,
  [id] int IDENTITY(1, 1) NOT NULL,
  [log_date] datetime DEFAULT getdate() NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[event_point] (
  [point_id] int IDENTITY(1, 1) NOT NULL,
  [group_id] int NULL,
  [group_point] float DEFAULT 0.0 NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[item_amount0_backup] (
  [item_id] int NULL,
  [char_id] int NULL,
  [item_type] int NULL,
  [amount] int NULL,
  [enchant] int NULL,
  [eroded] int NULL,
  [bless] int NULL,
  [ident] int NULL,
  [wished] int NULL,
  [warehouse] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[item_classid_normal] (
  [id] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[item_list] (
  [id] int NULL,
  [name] varchar(50) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[item_market_price] (
  [item_type] int NOT NULL,
  [enchant] int NOT NULL,
  [avg_price] float NOT NULL,
  [frequency] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[item_temp] (
  [char_id] int DEFAULT 0 NOT NULL,
  [warehouse] int DEFAULT 0 NOT NULL,
  [amount] int DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[ItemData] (
  [id] int NOT NULL,
  [name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [desc] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [logdate] smalldatetime DEFAULT getdate() NOT NULL,
  [consumetype] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [IsQuest] tinyint DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[login_announce] (
  [announce_id] int NOT NULL,
  [announce_msg] nvarchar(256) COLLATE Korean_Wansung_CI_AS NULL,
  [interval_10] int DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[lotto_game] (
  [round_number] int NOT NULL,
  [state] int NOT NULL,
  [left_time] int NOT NULL,
  [chosen_number_flag] int NOT NULL,
  [rule_number] int NOT NULL,
  [winner1_count] int NOT NULL,
  [winner2_count] int NOT NULL,
  [winner3_count] int NOT NULL,
  [winner4_count] int NOT NULL,
  [total_count] int NOT NULL,
  [carried_adena] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[lotto_game_backup_20050302] (
  [round_number] int NOT NULL,
  [state] int NOT NULL,
  [left_time] int NOT NULL,
  [chosen_number_flag] int NOT NULL,
  [rule_number] int NOT NULL,
  [winner1_count] int NOT NULL,
  [winner2_count] int NOT NULL,
  [winner3_count] int NOT NULL,
  [winner4_count] int NOT NULL,
  [total_count] int NOT NULL,
  [carried_adena] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[lotto_items] (
  [round_number] int NOT NULL,
  [item_id] int NOT NULL,
  [number_flag] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[manor_data] (
  [manor_id] int NOT NULL,
  [data_index] int NOT NULL,
  [seed_id] int NOT NULL,
  [seed_price] int NOT NULL,
  [seed_sell_count] int NOT NULL,
  [seed_remain_count] int NOT NULL,
  [crop_id] int NOT NULL,
  [crop_buy_count] int NOT NULL,
  [crop_price] int NOT NULL,
  [crop_type] int NOT NULL,
  [crop_remain_count] int NOT NULL,
  [crop_deposit] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[manor_data_n] (
  [manor_id] int NOT NULL,
  [data_index] int NOT NULL,
  [seed_id_n] int NOT NULL,
  [seed_price_n] int NOT NULL,
  [seed_sell_count_n] int NOT NULL,
  [crop_id_n] int NOT NULL,
  [crop_buy_count_n] int NOT NULL,
  [crop_price_n] int NOT NULL,
  [crop_type_n] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[manor_fix] (
  [char_id] int NOT NULL,
  [item_type] int NOT NULL,
  [amount] int NOT NULL,
  [warehouse] int NOT NULL,
  [error_amount] int NULL,
  [to_dec] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[manor_info] (
  [manor_id] int NOT NULL,
  [residence_id] int NOT NULL,
  [adena_vault] int NOT NULL,
  [crop_buy_vault] int NOT NULL,
  [last_changed] datetime NOT NULL,
  [change_state] tinyint DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[map] (
  [map2] int NULL,
  [x2] int NULL,
  [y2] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[mercenary] (
  [id] int IDENTITY(1, 1) NOT NULL,
  [residence_id] int NOT NULL,
  [npc_id] int NOT NULL,
  [x] int NOT NULL,
  [y] int NOT NULL,
  [z] int NOT NULL,
  [angle] int NOT NULL,
  [hp] int NOT NULL,
  [mp] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[monrace] (
  [race_id] int IDENTITY(1, 1) NOT NULL,
  [make_date] datetime DEFAULT getdate() NULL,
  [lane1] smallint NULL,
  [lane2] smallint NULL,
  [lane3] smallint NULL,
  [lane4] smallint NULL,
  [lane5] smallint NULL,
  [lane6] smallint NULL,
  [lane7] smallint NULL,
  [lane8] smallint NULL,
  [run1] float NULL,
  [run2] float NULL,
  [run3] float NULL,
  [run4] float NULL,
  [run5] float NULL,
  [run6] float NULL,
  [run7] float NULL,
  [run8] float NULL,
  [win1] smallint DEFAULT 0 NULL,
  [win2] smallint DEFAULT 0 NULL,
  [winrate1] float DEFAULT 0.0 NULL,
  [winrate2] float DEFAULT 0.0 NULL,
  [race_end] smallint DEFAULT 0 NULL,
  [tax_rate] int DEFAULT 0 NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[monrace_mon] (
  [runner_id] int NULL,
  [initial_win] smallint DEFAULT 0 NULL,
  [run_count] int DEFAULT 0 NULL,
  [win_count] int DEFAULT 0 NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[monrace_ticket] (
  [ticket_id] int IDENTITY(1, 1) NOT NULL,
  [monraceid] int NULL,
  [bet_type] smallint DEFAULT 0 NULL,
  [bet_1] smallint DEFAULT 0 NULL,
  [bet_2] smallint DEFAULT 0 NULL,
  [bet_3] smallint NULL,
  [bet_money] int NULL,
  [item_id] int NULL,
  [tax_money] int DEFAULT 0 NULL,
  [deleted] int DEFAULT 0 NULL,
  [remotefee] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[nobless_achievements] (
  [char_id] int NULL,
  [win_type] int NULL,
  [target] int NULL,
  [win_time] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[npc_boss] (
  [npc_db_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [alive] int NOT NULL,
  [hp] int NULL,
  [mp] int NULL,
  [pos_x] int NULL,
  [pos_y] int NULL,
  [pos_z] int NULL,
  [time_low] int NULL,
  [time_high] int NULL,
  [i0] int DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[npcname] (
  [npc_id] int NULL,
  [npc_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[object_data] (
  [id] int IDENTITY(1, 1) NOT NULL,
  [owner_id] int DEFAULT 0 NOT NULL,
  [residence_id] int DEFAULT 0 NOT NULL,
  [max_hp] int DEFAULT 0 NOT NULL,
  [hp] int DEFAULT 0 NOT NULL,
  [x_pos] int DEFAULT 0 NOT NULL,
  [y_pos] int DEFAULT 0 NOT NULL,
  [z_pos] int DEFAULT 0 NOT NULL,
  [type] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[OHEC_SUBJOB] (
  [server] int NULL,
  [All_75] int NULL,
  [Sub0] int NULL,
  [Sub1] int NULL,
  [Sub2] int NULL,
  [Sub3] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[OHEC_SUBJOB_TEMP] (
  [server] int NULL,
  [All_75] int NULL,
  [Sub0] int NULL,
  [Sub1] int NULL,
  [Sub2] int NULL,
  [Sub3] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[olympiad] (
  [season] int IDENTITY(1, 1) NOT NULL,
  [step] int NULL,
  [season_start_time] int NULL,
  [start_sec] int NULL,
  [bonus1_sec] int NULL,
  [bonus2_sec] int NULL,
  [bonus3_sec] int NULL,
  [bonus4_sec] int NULL,
  [nominate_sec] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[olympiad_match] (
  [season] int NULL,
  [class] int NULL,
  [match_time] int NULL,
  [char_id] int NULL,
  [rival_id] int NULL,
  [point] int NULL,
  [is_winner] tinyint NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[olympiad_result] (
  [season] int NOT NULL,
  [class] int NOT NULL,
  [char_id] int NOT NULL,
  [point] int NOT NULL,
  [match_count] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[pet_data] (
  [pet_id] int NOT NULL,
  [npc_class_id] int NOT NULL,
  [expoint] int NOT NULL,
  [nick_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [hp] float DEFAULT 1 NOT NULL,
  [mp] float DEFAULT 0 NOT NULL,
  [sp] int DEFAULT 0 NOT NULL,
  [meal] int DEFAULT 1 NOT NULL,
  [slot1] int DEFAULT 0 NOT NULL,
  [slot2] int DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[PetitionMsg] (
  [Char_Id] int NOT NULL,
  [msg] nvarchar(1024) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[Pledge] (
  [pledge_id] int IDENTITY(1, 1) NOT NULL,
  [ruler_id] int NOT NULL,
  [name] nvarchar(24) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [alliance_id] int DEFAULT 0 NOT NULL,
  [challenge_time] int DEFAULT 0 NOT NULL,
  [root_name_value] int DEFAULT 0 NOT NULL,
  [now_war_id] int DEFAULT 0 NOT NULL,
  [oust_time] int DEFAULT 0 NOT NULL,
  [skill_level] smallint DEFAULT 0 NOT NULL,
  [castle_id] int DEFAULT 0 NOT NULL,
  [agit_id] int DEFAULT 0 NOT NULL,
  [rank] int DEFAULT 0 NOT NULL,
  [name_value] int DEFAULT 0 NOT NULL,
  [status] int DEFAULT 0 NOT NULL,
  [private_flag] int DEFAULT 0 NOT NULL,
  [crest_id] int DEFAULT 0 NOT NULL,
  [is_guilty] int DEFAULT 0 NOT NULL,
  [dismiss_reserved_time] int DEFAULT 0 NOT NULL,
  [alliance_withdraw_time] int DEFAULT 0 NOT NULL,
  [alliance_dismiss_time] int DEFAULT 0 NOT NULL,
  [alliance_ousted_time] int DEFAULT 0 NOT NULL,
  [siege_kill] int DEFAULT 0 NOT NULL,
  [siege_death] int DEFAULT 0 NOT NULL,
  [emblem_id] int DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[pledge_announce] (
  [pledge_id] int NOT NULL,
  [show_flag] smallint NOT NULL,
  [content] nvarchar(3000) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[pledge_contribution] (
  [residence_id] int NOT NULL,
  [pledge_id] int NOT NULL,
  [contribution] int DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[Pledge_Crest] (
  [crest_id] int IDENTITY(1, 1) NOT NULL,
  [bitmap_size] smallint DEFAULT 0 NOT NULL,
  [bitmap] varbinary(3072) NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[point] (
  [map] int NULL,
  [align] int NULL,
  [loc] int NULL,
  [x] int NULL,
  [y] int NULL,
  [z] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[quest] (
  [char_id] int NOT NULL,
  [q1] int DEFAULT 0 NOT NULL,
  [s1] int DEFAULT 0 NULL,
  [q2] int DEFAULT 0 NOT NULL,
  [s2] int DEFAULT 0 NOT NULL,
  [q3] int DEFAULT 0 NOT NULL,
  [s3] int DEFAULT 0 NOT NULL,
  [q4] int DEFAULT 0 NOT NULL,
  [s4] int DEFAULT 0 NOT NULL,
  [q5] int DEFAULT 0 NOT NULL,
  [s5] int DEFAULT 0 NOT NULL,
  [q6] int DEFAULT 0 NOT NULL,
  [s6] int DEFAULT 0 NOT NULL,
  [q7] int DEFAULT 0 NOT NULL,
  [s7] int DEFAULT 0 NOT NULL,
  [q8] int DEFAULT 0 NOT NULL,
  [s8] int DEFAULT 0 NOT NULL,
  [q9] int DEFAULT 0 NOT NULL,
  [s9] int DEFAULT 0 NOT NULL,
  [q10] int DEFAULT 0 NOT NULL,
  [s10] int DEFAULT 0 NOT NULL,
  [q11] int DEFAULT 0 NOT NULL,
  [s11] int DEFAULT 0 NOT NULL,
  [q12] int DEFAULT 0 NOT NULL,
  [s12] int DEFAULT 0 NOT NULL,
  [q13] int DEFAULT 0 NOT NULL,
  [s13] int DEFAULT 0 NOT NULL,
  [q14] int DEFAULT 0 NOT NULL,
  [s14] int DEFAULT 0 NOT NULL,
  [q15] int DEFAULT 0 NOT NULL,
  [s15] int DEFAULT 0 NOT NULL,
  [q16] int DEFAULT 0 NOT NULL,
  [s16] int DEFAULT 0 NOT NULL,
  [j1] int DEFAULT 0 NOT NULL,
  [j2] int DEFAULT 0 NOT NULL,
  [j3] int DEFAULT 0 NOT NULL,
  [j4] int DEFAULT 0 NOT NULL,
  [j5] int DEFAULT 0 NOT NULL,
  [j6] int DEFAULT 0 NOT NULL,
  [j7] int DEFAULT 0 NOT NULL,
  [j8] int DEFAULT 0 NOT NULL,
  [j9] int DEFAULT 0 NOT NULL,
  [j10] int DEFAULT 0 NOT NULL,
  [j11] int DEFAULT 0 NOT NULL,
  [j12] int DEFAULT 0 NOT NULL,
  [j13] int DEFAULT 0 NOT NULL,
  [j14] int DEFAULT 0 NOT NULL,
  [j15] int DEFAULT 0 NOT NULL,
  [j16] int DEFAULT 0 NOT NULL,
  [s2_1] int DEFAULT 0 NULL,
  [s2_2] int DEFAULT 0 NULL,
  [s2_3] int DEFAULT 0 NULL,
  [s2_4] int DEFAULT 0 NULL,
  [s2_5] int DEFAULT 0 NULL,
  [s2_6] int DEFAULT 0 NULL,
  [s2_7] int DEFAULT 0 NULL,
  [s2_8] int DEFAULT 0 NULL,
  [s2_9] int DEFAULT 0 NULL,
  [s2_10] int DEFAULT 0 NULL,
  [s2_11] int DEFAULT 0 NULL,
  [s2_12] int DEFAULT 0 NULL,
  [s2_13] int DEFAULT 0 NULL,
  [s2_14] int DEFAULT 0 NULL,
  [s2_15] int DEFAULT 0 NULL,
  [s2_16] int DEFAULT 0 NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[QuestData] (
  [id] int NOT NULL,
  [name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [data] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [logdate] smalldatetime DEFAULT getdate() NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[req_char] (
  [server_id] int NULL,
  [char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[req_charmove] (
  [old_char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [old_char_id] int NULL,
  [account_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [account_id] int NULL,
  [old_world_id] int NULL,
  [req_date] datetime NULL,
  [req_id] int NOT NULL,
  [new_world_id] int NULL,
  [new_char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [new_char_id] int NULL,
  [is_pc_bang] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[req_charmove_old] (
  [old_char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [old_char_id] int NULL,
  [account_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [account_id] int NULL,
  [old_world_id] int NULL,
  [req_date] datetime DEFAULT getdate() NULL,
  [req_id] int IDENTITY(1, 1) NOT NULL,
  [new_world_id] int NULL,
  [new_char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [new_char_id] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[req_pledge] (
  [server_id] int NULL,
  [pledge_id] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[residence_guard] (
  [residence_id] int NOT NULL,
  [item_id] int NOT NULL,
  [npc_id] int NOT NULL,
  [guard_type] int NOT NULL,
  [can_move] int NOT NULL,
  [x] int NOT NULL,
  [y] int NOT NULL,
  [z] int NOT NULL,
  [angle] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[shortcut_data] (
  [char_id] int NOT NULL,
  [slotnum] int NOT NULL,
  [shortcut_type] int NOT NULL,
  [shortcut_id] int NOT NULL,
  [shortcut_macro] nvarchar(256) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [subjob_id] int DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[siege_agit_pledge] (
  [agit_id] int NOT NULL,
  [pledge_id] int NOT NULL,
  [propose_time] int NOT NULL,
  [status] int DEFAULT 0 NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[SkillData] (
  [id] int NOT NULL,
  [lev] smallint NOT NULL,
  [name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [logdate] smalldatetime DEFAULT getdate() NOT NULL,
  [skill_desc] nvarchar(255) COLLATE Korean_Wansung_CI_AS NULL,
  [bIsMagic] tinyint NULL,
  [activate_type] nvarchar(5) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[ssq_data] (
  [round_number] int NOT NULL,
  [status] tinyint NOT NULL,
  [winner] tinyint NOT NULL,
  [event_start_time] int NOT NULL,
  [seal_effect_time] int NOT NULL,
  [event_end_time] int NOT NULL,
  [seal_effect_end_time] int NOT NULL,
  [seal1] tinyint NOT NULL,
  [seal2] tinyint NOT NULL,
  [seal3] tinyint NOT NULL,
  [seal4] tinyint NOT NULL,
  [seal5] tinyint NOT NULL,
  [seal6] tinyint NOT NULL,
  [seal7] tinyint NOT NULL,
  [last_changed_time] datetime NOT NULL,
  [castle_snapshot_time] int NULL,
  [can_drop_guard] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[ssq_join_data] (
  [round_number] int NOT NULL,
  [type] tinyint NOT NULL,
  [point] int NOT NULL,
  [main_event_point] int NULL,
  [collected_point] int NULL,
  [member_count] int NOT NULL,
  [seal1_selection_count] int NOT NULL,
  [seal2_selection_count] int NOT NULL,
  [seal3_selection_count] int NOT NULL,
  [seal4_selection_count] int NOT NULL,
  [seal5_selection_count] int NOT NULL,
  [seal6_selection_count] int NOT NULL,
  [seal7_selection_count] int NOT NULL,
  [last_changed_time] datetime NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[ssq_top_point_user] (
  [ssq_round] int NOT NULL,
  [record_id] int NOT NULL,
  [rank_time] int NOT NULL,
  [ssq_point] int NOT NULL,
  [char_id] int NOT NULL,
  [char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [ssq_part] tinyint NOT NULL,
  [ssq_position] tinyint NOT NULL,
  [seal_selection_no] tinyint NOT NULL,
  [last_changed_time] datetime NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[ssq_user_data] (
  [char_id] int NOT NULL,
  [round_number] int NOT NULL,
  [ssq_part] tinyint NOT NULL,
  [ssq_position] tinyint NOT NULL,
  [seal_selection_no] tinyint NOT NULL,
  [ssq_join_time] int NOT NULL,
  [ssq_point] int NOT NULL,
  [twilight_a_item_num] int NOT NULL,
  [twilight_b_item_num] int NOT NULL,
  [twilight_c_item_num] int NOT NULL,
  [dawn_a_item_num] int NOT NULL,
  [dawn_b_item_num] int NOT NULL,
  [dawn_c_item_num] int NOT NULL,
  [ticket_buy_count] tinyint NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[stat_acc_class] (
  [class] tinyint NOT NULL,
  [count] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[stat_acc_lev] (
  [lev] tinyint NOT NULL,
  [count] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[stat_acc_mlev] (
  [account_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [lev] tinyint NOT NULL,
  [race] tinyint NOT NULL,
  [class] tinyint NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[stat_acc_race] (
  [race] tinyint NOT NULL,
  [count] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[stat_item_cnt] (
  [id] int NOT NULL,
  [count] int NOT NULL,
  [sum] float NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[stat_item_ent] (
  [item_type] int NOT NULL,
  [enchant] int NOT NULL,
  [count] int NOT NULL,
  [sum] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[stat_item_ment] (
  [item_type] int NOT NULL,
  [item_id] int NOT NULL,
  [char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [max_ent] int NOT NULL,
  [builder] tinyint NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[stat_item_mincnt] (
  [item_type] int NOT NULL,
  [item_id] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[team_battle_agit_member] (
  [agit_id] int NOT NULL,
  [char_id] int NOT NULL,
  [pledge_id] int NOT NULL,
  [propose_time] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[team_battle_agit_pledge] (
  [agit_id] int NOT NULL,
  [pledge_id] int NOT NULL,
  [propose_time] int NOT NULL,
  [color] int NOT NULL,
  [npc_type] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[temp_account] (
  [real_account] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [user_id] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[temp_inv] (
  [item_id] int NOT NULL,
  [item_type] int NOT NULL,
  [amount] int NOT NULL,
  [user_id] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[temp_pet_inv] (
  [pet_item_id] int NOT NULL,
  [pet_item_type] int NOT NULL,
  [amount] int NOT NULL,
  [pet_collar_id] int NOT NULL,
  [user_id] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[temp_recipe] (
  [char_id] int NULL,
  [item_type] int NULL,
  [warehouse] int NULL,
  [recipecount] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[temp_skill] (
  [char_id] int NULL,
  [skill] int NULL,
  [lev] int NULL,
  [temptime] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[temp_skill2] (
  [char_id] int NULL,
  [skill_id] int NULL,
  [skill_lev] int NULL,
  [temptime] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[temp_update] (
  [char_id] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[TEST] (
  [??] int NOT NULL,
  [char_id] int NULL,
  [char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [account_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [lev] tinyint NULL,
  [class] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[time_attack_record] (
  [ssq_round] int NOT NULL,
  [room_no] tinyint NOT NULL,
  [record_type] tinyint NOT NULL,
  [ssq_part] tinyint NOT NULL,
  [point] int NOT NULL,
  [record_time] int NOT NULL,
  [elapsed_time] int NOT NULL,
  [member_count] int NOT NULL,
  [member_names] nvarchar(256) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [member_dbid_list] nvarchar(128) COLLATE Korean_Wansung_CI_AS NULL,
  [member_reward_flags] int NULL,
  [fee] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[time_attack_record_test] (
  [Col001] varchar(255) COLLATE Korean_Wansung_CI_AS NULL,
  [Col002] varchar(255) COLLATE Korean_Wansung_CI_AS NULL,
  [Col003] varchar(255) COLLATE Korean_Wansung_CI_AS NULL,
  [Col004] varchar(255) COLLATE Korean_Wansung_CI_AS NULL,
  [Col005] varchar(255) COLLATE Korean_Wansung_CI_AS NULL,
  [Col006] varchar(255) COLLATE Korean_Wansung_CI_AS NULL,
  [Col007] varchar(255) COLLATE Korean_Wansung_CI_AS NULL,
  [Col008] varchar(255) COLLATE Korean_Wansung_CI_AS NULL,
  [Col009] varchar(255) COLLATE Korean_Wansung_CI_AS NULL,
  [Col010] varchar(255) COLLATE Korean_Wansung_CI_AS NULL,
  [Col011] varchar(255) COLLATE Korean_Wansung_CI_AS NULL,
  [Col012] varchar(255) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[time_data] (
  [account_id] int NOT NULL,
  [last_logout] datetime NOT NULL,
  [used_sec] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[tmp_alliance] (
  [id] int IDENTITY(1, 1) NOT NULL,
  [name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [master_pledge_id] int NOT NULL,
  [oust_time] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[tmp_die_user] (
  [char_name] varchar(64) COLLATE Korean_Wansung_CI_AS NULL,
  [account_name] varchar(64) COLLATE Korean_Wansung_CI_AS NULL,
  [before_die_exp] int NULL,
  [die_minus_exp] int NULL,
  [after_resurrect_exp] int NULL,
  [resurrect_add_exp] int NULL,
  [hasSubjob] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[tmp_lotto_price_given] (
  [char_id] int NULL,
  [char_name] varchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [rank] int NULL,
  [price] int NULL,
  [new_price] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[tmp_pledge] (
  [pledge_id] int IDENTITY(1, 1) NOT NULL,
  [ruler_id] int NOT NULL,
  [name] nvarchar(24) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [alliance_id] int NOT NULL,
  [root_name_value] int NOT NULL,
  [name_value] int NOT NULL,
  [oust_time] int NOT NULL,
  [skill_level] smallint NOT NULL,
  [castle_id] int NOT NULL,
  [agit_id] int NOT NULL,
  [rank] int NOT NULL,
  [status] int NOT NULL,
  [is_guilty] int NOT NULL,
  [dismiss_reserved_time] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[tmp_recover_user] (
  [char_name] nvarchar(60) COLLATE Korean_Wansung_CI_AS NULL,
  [addedExp] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[tmp_seed_dp] (
  [seed_id] int NULL,
  [dp] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[tmp_user_data] (
  [char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [char_id] int IDENTITY(1, 1) NOT NULL,
  [account_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [account_id] int NOT NULL,
  [pledge_id] int NOT NULL,
  [builder] tinyint NOT NULL,
  [gender] tinyint NOT NULL,
  [race] tinyint NOT NULL,
  [class] tinyint NOT NULL,
  [world] smallint NOT NULL,
  [SP] int NOT NULL,
  [EXP] int NOT NULL,
  [Lev] tinyint NOT NULL,
  [align] int NOT NULL,
  [PK] int NOT NULL,
  [PKpardon] int NOT NULL,
  [Duel] int NOT NULL,
  [create_date] datetime NOT NULL,
  [login] datetime NULL,
  [logout] datetime NULL,
  [nickname] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [max_hp] int NOT NULL,
  [max_mp] int NOT NULL,
  [use_time] int NOT NULL,
  [drop_exp] int NOT NULL,
  [cp] float NOT NULL,
  [max_cp] float NOT NULL,
  [subjob_id] int NULL,
  [subjob0_class] int NOT NULL,
  [subjob1_class] int NOT NULL,
  [subjob2_class] int NOT NULL,
  [subjob3_class] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[tmp_user_item] (
  [item_id] int IDENTITY(1, 1) NOT NULL,
  [char_id] int NOT NULL,
  [item_type] int NOT NULL,
  [amount] int NOT NULL,
  [enchant] int NOT NULL,
  [warehouse] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[tmp_user_nobless] (
  [char_id] int NOT NULL,
  [nobless_type] tinyint NULL,
  [hero_type] tinyint NULL,
  [win_count] int NULL,
  [previous_point] int NULL,
  [olympiad_point] int NULL,
  [match_count] int NULL,
  [words] varchar(128) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_ActiveSkill] (
  [char_id] int NOT NULL,
  [s1] int NULL,
  [l1] tinyint NULL,
  [d1] int NULL,
  [s2] int NULL,
  [l2] tinyint NULL,
  [d2] int NULL,
  [s3] int NULL,
  [l3] tinyint NULL,
  [d3] int NULL,
  [s4] int NULL,
  [l4] tinyint NULL,
  [d4] int NULL,
  [s5] int NULL,
  [l5] tinyint NULL,
  [d5] int NULL,
  [s6] int NULL,
  [l6] tinyint NULL,
  [d6] int NULL,
  [s7] int NULL,
  [l7] tinyint NULL,
  [d7] int NULL,
  [s8] int NULL,
  [l8] tinyint NULL,
  [d8] int NULL,
  [s9] int NULL,
  [l9] tinyint NULL,
  [d9] int NULL,
  [s10] int NULL,
  [l10] tinyint NULL,
  [d10] int NULL,
  [s11] int NULL,
  [l11] tinyint NULL,
  [d11] int NULL,
  [s12] int NULL,
  [l12] tinyint NULL,
  [d12] int NULL,
  [s13] int NULL,
  [l13] tinyint NULL,
  [d13] int NULL,
  [s14] int NULL,
  [l14] tinyint NULL,
  [d14] int NULL,
  [s15] int NULL,
  [l15] tinyint NULL,
  [d15] int NULL,
  [s16] int NULL,
  [l16] tinyint NULL,
  [d16] int NULL,
  [s17] int NULL,
  [l17] tinyint NULL,
  [d17] int NULL,
  [s18] int NULL,
  [l18] tinyint NULL,
  [d18] int NULL,
  [s19] int NULL,
  [l19] tinyint NULL,
  [d19] int NULL,
  [s20] int NULL,
  [l20] tinyint NULL,
  [d20] int NULL,
  [c1] tinyint NULL,
  [c2] tinyint NULL,
  [c3] tinyint NULL,
  [c4] tinyint NULL,
  [c5] tinyint NULL,
  [c6] tinyint NULL,
  [c7] tinyint NULL,
  [c8] tinyint NULL,
  [c9] tinyint NULL,
  [c10] tinyint NULL,
  [c11] tinyint NULL,
  [c12] tinyint NULL,
  [c13] tinyint NULL,
  [c14] tinyint NULL,
  [c15] tinyint NULL,
  [c16] tinyint NULL,
  [c17] tinyint NULL,
  [c18] tinyint NULL,
  [c19] tinyint NULL,
  [c20] tinyint NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_ban] (
  [char_id] int NOT NULL,
  [status] int DEFAULT 0 NULL,
  [ban_date] datetime DEFAULT getdate() NULL,
  [ban_hour] smallint DEFAULT 0 NULL,
  [ban_end] int DEFAULT 0 NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_blocklist] (
  [char_id] int NOT NULL,
  [block_char_id] int NOT NULL,
  [block_char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_comment] (
  [char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [char_id] int NULL,
  [comment_id] int IDENTITY(1, 1) NOT NULL,
  [comment] nvarchar(200) COLLATE Korean_Wansung_CI_AS NULL,
  [comment_date] datetime DEFAULT getdate() NULL,
  [writer] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [deleted] tinyint DEFAULT 0 NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_data] (
  [char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [char_id] int IDENTITY(1, 1) NOT NULL,
  [account_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [account_id] int NOT NULL,
  [pledge_id] int DEFAULT 0 NOT NULL,
  [builder] tinyint DEFAULT 0 NOT NULL,
  [gender] tinyint NOT NULL,
  [race] tinyint NOT NULL,
  [class] tinyint NOT NULL,
  [world] smallint NOT NULL,
  [xloc] int NOT NULL,
  [yloc] int NOT NULL,
  [zloc] int NOT NULL,
  [IsInVehicle] smallint DEFAULT 0 NOT NULL,
  [HP] float NOT NULL,
  [MP] float NOT NULL,
  [SP] int DEFAULT 0 NOT NULL,
  [Exp] int NOT NULL,
  [Lev] tinyint NOT NULL,
  [align] int NOT NULL,
  [PK] int DEFAULT 0 NOT NULL,
  [PKpardon] int DEFAULT 0 NOT NULL,
  [Duel] int DEFAULT 0 NOT NULL,
  [ST_underware] int DEFAULT 0 NOT NULL,
  [ST_right_ear] int DEFAULT 0 NOT NULL,
  [ST_left_ear] int DEFAULT 0 NOT NULL,
  [ST_neck] int DEFAULT 0 NOT NULL,
  [ST_right_finger] int DEFAULT 0 NOT NULL,
  [ST_left_finger] int DEFAULT 0 NOT NULL,
  [ST_head] int DEFAULT 0 NOT NULL,
  [ST_right_hand] int DEFAULT 0 NOT NULL,
  [ST_left_hand] int DEFAULT 0 NOT NULL,
  [ST_gloves] int DEFAULT 0 NOT NULL,
  [ST_chest] int DEFAULT 0 NOT NULL,
  [ST_legs] int DEFAULT 0 NOT NULL,
  [ST_feet] int DEFAULT 0 NOT NULL,
  [ST_back] int DEFAULT 0 NOT NULL,
  [ST_both_hand] int DEFAULT 0 NOT NULL,
  [create_date] datetime DEFAULT getdate() NOT NULL,
  [login] datetime NULL,
  [logout] datetime NULL,
  [quest_flag] binary(32) DEFAULT 0x00 NOT NULL,
  [nickname] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [power_flag] binary(32) DEFAULT 0x0000000000000000000000000000000000000000000000000000000000000000 NOT NULL,
  [pledge_dismiss_time] int DEFAULT 0 NOT NULL,
  [pledge_leave_time] int DEFAULT 0 NOT NULL,
  [pledge_leave_status] tinyint DEFAULT 0 NOT NULL,
  [max_hp] int DEFAULT 0 NOT NULL,
  [max_mp] int DEFAULT 0 NOT NULL,
  [quest_memo] char(32) COLLATE Korean_Wansung_CI_AS NULL,
  [face_index] int DEFAULT 0 NOT NULL,
  [hair_shape_index] int DEFAULT 0 NOT NULL,
  [hair_color_index] int DEFAULT 0 NOT NULL,
  [use_time] int DEFAULT 0 NOT NULL,
  [temp_delete_date] smalldatetime NULL,
  [pledge_ousted_time] int DEFAULT 0 NOT NULL,
  [pledge_withdraw_time] int DEFAULT 0 NOT NULL,
  [surrender_war_id] int DEFAULT 0 NOT NULL,
  [drop_exp] int DEFAULT 0 NOT NULL,
  [old_x] int NULL,
  [old_y] int NULL,
  [old_z] int NULL,
  [subjob_id] int NULL,
  [ssq_dawn_round] int NULL,
  [cp] float DEFAULT 0 NOT NULL,
  [max_cp] float DEFAULT 0 NOT NULL,
  [ST_hair] int DEFAULT 0 NOT NULL,
  [subjob0_class] int DEFAULT (-1) NOT NULL,
  [subjob1_class] int DEFAULT (-1) NOT NULL,
  [subjob2_class] int DEFAULT (-1) NOT NULL,
  [subjob3_class] int DEFAULT (-1) NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_data_moved] (
  [char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [char_id] int IDENTITY(1, 1) NOT NULL,
  [account_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [account_id] int NOT NULL,
  [pledge_id] int NOT NULL,
  [builder] tinyint NOT NULL,
  [gender] tinyint NOT NULL,
  [race] tinyint NOT NULL,
  [class] tinyint NOT NULL,
  [world] smallint NOT NULL,
  [xloc] int NOT NULL,
  [yloc] int NOT NULL,
  [zloc] int NOT NULL,
  [IsInVehicle] smallint NOT NULL,
  [HP] float NOT NULL,
  [MP] float NOT NULL,
  [SP] int NOT NULL,
  [Exp] int NOT NULL,
  [Lev] tinyint NOT NULL,
  [align] int NOT NULL,
  [PK] int NOT NULL,
  [PKpardon] int NOT NULL,
  [Duel] int NOT NULL,
  [ST_underware] int NOT NULL,
  [ST_right_ear] int NOT NULL,
  [ST_left_ear] int NOT NULL,
  [ST_neck] int NOT NULL,
  [ST_right_finger] int NOT NULL,
  [ST_left_finger] int NOT NULL,
  [ST_head] int NOT NULL,
  [ST_right_hand] int NOT NULL,
  [ST_left_hand] int NOT NULL,
  [ST_gloves] int NOT NULL,
  [ST_chest] int NOT NULL,
  [ST_legs] int NOT NULL,
  [ST_feet] int NOT NULL,
  [ST_back] int NOT NULL,
  [ST_both_hand] int NOT NULL,
  [create_date] datetime NOT NULL,
  [login] datetime NULL,
  [logout] datetime NULL,
  [quest_flag] binary(32) NOT NULL,
  [nickname] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [power_flag] binary(32) NOT NULL,
  [pledge_dismiss_time] int NOT NULL,
  [pledge_leave_time] int NOT NULL,
  [pledge_leave_status] tinyint NOT NULL,
  [max_hp] int NOT NULL,
  [max_mp] int NOT NULL,
  [quest_memo] char(32) COLLATE Korean_Wansung_CI_AS NULL,
  [face_index] int NOT NULL,
  [hair_shape_index] int NOT NULL,
  [hair_color_index] int NOT NULL,
  [use_time] int NOT NULL,
  [temp_delete_date] smalldatetime NULL,
  [pledge_ousted_time] int NOT NULL,
  [pledge_withdraw_time] int NOT NULL,
  [surrender_war_id] int NOT NULL,
  [drop_exp] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_data_temp] (
  [char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [account_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [account_id] int NOT NULL,
  [pledge_id] int NOT NULL,
  [builder] tinyint NOT NULL,
  [gender] tinyint NOT NULL,
  [race] tinyint NOT NULL,
  [class] tinyint NOT NULL,
  [world] smallint NOT NULL,
  [xloc] int NOT NULL,
  [yloc] int NOT NULL,
  [zloc] int NOT NULL,
  [IsInVehicle] smallint NOT NULL,
  [HP] float NOT NULL,
  [MP] float NOT NULL,
  [SP] int NOT NULL,
  [Exp] int NOT NULL,
  [Lev] tinyint NOT NULL,
  [align] int NOT NULL,
  [PK] int NOT NULL,
  [PKpardon] int NOT NULL,
  [Duel] int NOT NULL,
  [ST_underware] int NOT NULL,
  [ST_right_ear] int NOT NULL,
  [ST_left_ear] int NOT NULL,
  [ST_neck] int NOT NULL,
  [ST_right_finger] int NOT NULL,
  [ST_left_finger] int NOT NULL,
  [ST_head] int NOT NULL,
  [ST_right_hand] int NOT NULL,
  [ST_left_hand] int NOT NULL,
  [ST_gloves] int NOT NULL,
  [ST_chest] int NOT NULL,
  [ST_legs] int NOT NULL,
  [ST_feet] int NOT NULL,
  [ST_back] int NOT NULL,
  [ST_both_hand] int NOT NULL,
  [create_date] datetime NOT NULL,
  [login] datetime NULL,
  [logout] datetime NULL,
  [quest_flag] binary(32) NOT NULL,
  [nickname] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [power_flag] binary(32) NOT NULL,
  [pledge_dismiss_time] int NOT NULL,
  [pledge_leave_time] int NOT NULL,
  [pledge_leave_status] tinyint NOT NULL,
  [max_hp] int NOT NULL,
  [max_mp] int NOT NULL,
  [quest_memo] char(32) COLLATE Korean_Wansung_CI_AS NULL,
  [face_index] int NOT NULL,
  [hair_shape_index] int NOT NULL,
  [hair_color_index] int NOT NULL,
  [use_time] int NOT NULL,
  [temp_delete_date] smalldatetime NULL,
  [pledge_ousted_time] int NOT NULL,
  [pledge_withdraw_time] int NOT NULL,
  [surrender_war_id] int NOT NULL,
  [drop_exp] int NOT NULL,
  [old_x] int NULL,
  [old_y] int NULL,
  [old_z] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_deleted] (
  [char_id] int NOT NULL,
  [delete_date] datetime NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_friend] (
  [char_id] int NOT NULL,
  [friend_char_id] int NOT NULL,
  [friend_char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_henna] (
  [char_id] int NOT NULL,
  [henna_1] int DEFAULT 0 NOT NULL,
  [henna_2] int DEFAULT 0 NOT NULL,
  [henna_3] int DEFAULT 0 NOT NULL,
  [subjob_id] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_history] (
  [char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [char_id] int NOT NULL,
  [log_date] datetime DEFAULT getdate() NOT NULL,
  [log_action] tinyint NOT NULL,
  [account_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NULL,
  [create_date] datetime NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_item] (
  [item_id] int IDENTITY(1, 1) NOT FOR REPLICATION NOT NULL,
  [char_id] int NOT NULL,
  [item_type] int NOT NULL,
  [amount] int NOT NULL,
  [enchant] int NOT NULL,
  [eroded] int NOT NULL,
  [bless] tinyint NOT NULL,
  [ident] int NOT NULL,
  [wished] tinyint DEFAULT 0 NOT NULL,
  [warehouse] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_journal] (
  [char_id] int NOT NULL,
  [type] smallint NOT NULL,
  [int_data_1] int NULL,
  [int_data_2] int NULL,
  [log_data] smalldatetime DEFAULT getdate() NOT NULL,
  [play_time] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_log] (
  [char_id] int NULL,
  [log_id] tinyint NULL,
  [log_date] datetime DEFAULT getdate() NULL,
  [log_from] int NULL,
  [log_to] int NULL,
  [use_time] int NULL,
  [subjob_id] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_macro] (
  [macro_id] int IDENTITY(1, 1) NOT NULL,
  [char_id] int NULL,
  [macro_name] nvarchar(64) COLLATE Korean_Wansung_CI_AS NULL,
  [macro_tooltip] nvarchar(64) COLLATE Korean_Wansung_CI_AS NULL,
  [macro_iconname] nvarchar(64) COLLATE Korean_Wansung_CI_AS NULL,
  [macro_icontype] int NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_macroinfo] (
  [macro_id] int NOT NULL,
  [macro_order] int NULL,
  [macro_int1] int NULL,
  [macro_int2] int NULL,
  [macro_int3] int NULL,
  [macro_str] nvarchar(255) COLLATE Korean_Wansung_CI_AS NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_mail] (
  [id] int IDENTITY(1, 1) NOT NULL,
  [created_date] datetime DEFAULT getdate() NOT NULL,
  [title] nvarchar(200) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [content] nvarchar(3500) COLLATE Korean_Wansung_CI_AS NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_mail_receiver] (
  [mail_id] int NOT NULL,
  [mailbox_type] tinyint DEFAULT 0 NOT NULL,
  [receiver_id] int NOT NULL,
  [receiver_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [read_date] datetime NULL,
  [read_status] tinyint DEFAULT 0 NOT NULL,
  [deleted] tinyint DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_mail_sender] (
  [mail_id] int NOT NULL,
  [related_id] int NOT NULL,
  [mail_type] tinyint NOT NULL,
  [mailbox_type] tinyint DEFAULT 1 NOT NULL,
  [sender_id] int NOT NULL,
  [sender_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [send_date] datetime DEFAULT getdate() NOT NULL,
  [receiver_name_list] nvarchar(250) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [deleted] tinyint DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_name_color] (
  [char_id] int NOT NULL,
  [color_rgb] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_name_reserved] (
  [char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL,
  [account_id] int DEFAULT 0 NOT NULL,
  [used] tinyint DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_newbie] (
  [account_id] int NOT NULL,
  [char_id] int DEFAULT 0 NOT NULL,
  [newbie_stat] smallint NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_nobless] (
  [char_id] int NOT NULL,
  [nobless_type] tinyint DEFAULT 0 NULL,
  [hero_type] tinyint DEFAULT 0 NULL,
  [win_count] int DEFAULT 0 NULL,
  [previous_point] int DEFAULT 0 NULL,
  [olympiad_point] int DEFAULT 0 NULL,
  [match_count] int DEFAULT 0 NULL,
  [words] varchar(128) COLLATE Korean_Wansung_CI_AS NULL,
  [olympiad_win_count] int DEFAULT 0 NULL,
  [olympiad_lose_count] int DEFAULT 0 NULL,
  [history_open] tinyint DEFAULT 0 NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_prohibit] (
  [char_name] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_prohibit_word] (
  [words] nvarchar(50) COLLATE Korean_Wansung_CI_AS NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_punish] (
  [char_id] int NOT NULL,
  [punish_id] int NOT NULL,
  [punish_on] tinyint DEFAULT 0 NOT NULL,
  [remain_game] int NULL,
  [remain_real] int NULL,
  [punish_seconds] int NULL,
  [punish_date] datetime DEFAULT getdate() NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_recipe] (
  [char_id] int NOT NULL,
  [recipe_id] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_skill] (
  [char_id] int NOT NULL,
  [skill_id] int NOT NULL,
  [skill_lev] tinyint DEFAULT 0 NOT NULL,
  [to_end_time] int DEFAULT 0 NOT NULL,
  [subjob_id] int DEFAULT 0 NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_skill_old] (
  [char_id] int NOT NULL,
  [skill_id] int NOT NULL,
  [skill_lev] tinyint NOT NULL,
  [to_end_time] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_sociality] (
  [char_id] int NOT NULL,
  [sociality] int NOT NULL,
  [used_sulffrage] int NOT NULL,
  [last_changed] datetime NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_subjob] (
  [char_id] int NULL,
  [hp] float NULL,
  [mp] float NULL,
  [sp] int NULL,
  [exp] int NULL,
  [level] tinyint NULL,
  [henna_1] int NULL,
  [henna_2] int NULL,
  [henna_3] int NULL,
  [subjob_id] int NULL,
  [create_date] datetime NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_surrender] (
  [char_id] int NOT NULL,
  [surrender_war_id] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[user_warehouse] (
  [item_id] int NOT NULL,
  [char_id] int NOT NULL,
  [item_type] int NOT NULL,
  [amount] int NOT NULL,
  [enchant] int NOT NULL,
  [eroded] int NOT NULL,
  [bless] tinyint NOT NULL,
  [ident] tinyint NOT NULL,
  [wished] tinyint NULL,
  [warehouse] int NOT NULL
)
ON [PRIMARY]
GO

CREATE TABLE [dbo].[war_declare] (
  [challenger] int NOT NULL,
  [challengee] int NOT NULL,
  [declare_time] int NOT NULL
)
ON [PRIMARY]
GO

ALTER TABLE [dbo].[agit]
ADD CONSTRAINT [PK_agit] 
PRIMARY KEY CLUSTERED ([id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_agit_adena] ON [dbo].[agit_adena]
  ([pledge_id], [auction_id], [reason])
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_agit_auction_1] ON [dbo].[agit_auction]
  ([agit_id], [auction_time] DESC)
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_agit_auction_2] ON [dbo].[agit_auction]
  ([auction_id])
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_agit_auction] ON [dbo].[agit_bid]
  ([auction_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[agit_bid]
ADD CONSTRAINT [IX_agit_bid] 
UNIQUE NONCLUSTERED ([auction_id], [attend_pledge_id])
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_agit_price] ON [dbo].[agit_bid]
  ([attend_price] DESC, [attend_date])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[agit_deco]
ADD CONSTRAINT [PK_agit_deco] 
PRIMARY KEY CLUSTERED ([agit_id], [type])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[Alliance]
ADD CONSTRAINT [PK_Alliance] 
PRIMARY KEY CLUSTERED ([name])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [ix_bookmark] ON [dbo].[bookmark]
  ([char_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[builder_account]
ADD CONSTRAINT [PK_account_builder] 
PRIMARY KEY CLUSTERED ([account_name])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[castle]
ADD CONSTRAINT [PK_castle] 
PRIMARY KEY CLUSTERED ([id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[castle_crop]
ADD CONSTRAINT [PK_castle_crop] 
PRIMARY KEY CLUSTERED ([castle_id], [item_type])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[ch3_lotto_char]
ADD PRIMARY KEY CLUSTERED ([char_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[control_tower]
ADD PRIMARY KEY CLUSTERED ([name])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[door]
ADD CONSTRAINT [PK_door] 
PRIMARY KEY CLUSTERED ([name])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_err_table2] ON [dbo].[err_skill_1]
  ([class], [lev])
ON [PRIMARY]
GO

CREATE CLUSTERED INDEX [IX_event_point] ON [dbo].[event_point]
  ([group_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[item_classid_normal]
ADD CONSTRAINT [PK_item_classid_normal] 
PRIMARY KEY CLUSTERED ([id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_itemdata_name] ON [dbo].[ItemData]
  ([name])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_ItemData] ON [dbo].[ItemData]
  ([id], [consumetype])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[ItemData]
ADD CONSTRAINT [PK_ItemData] 
PRIMARY KEY CLUSTERED ([id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[lotto_game]
ADD CONSTRAINT [PK_lotto_game] 
PRIMARY KEY CLUSTERED ([round_number])
ON [PRIMARY]
GO

CREATE CLUSTERED INDEX [IX_lotto_items] ON [dbo].[lotto_items]
  ([round_number])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[manor_data]
ADD CONSTRAINT [PK_manor_data] 
PRIMARY KEY CLUSTERED ([manor_id], [data_index])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[manor_data_n]
ADD CONSTRAINT [PK_manor_data_n] 
PRIMARY KEY CLUSTERED ([manor_id], [data_index])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[manor_info]
ADD CONSTRAINT [PK_manor_info] 
PRIMARY KEY CLUSTERED ([manor_id])
ON [PRIMARY]
GO

CREATE CLUSTERED INDEX [IX_monrace] ON [dbo].[monrace]
  ([race_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_monrace_ticket] ON [dbo].[monrace_ticket]
  ([item_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE CLUSTERED INDEX [IX_monrace_ticket_1] ON [dbo].[monrace_ticket]
  ([monraceid], [bet_type], [bet_1], [bet_2])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE UNIQUE NONCLUSTERED INDEX [idx_npc_boss_unique_name] ON [dbo].[npc_boss]
  ([npc_db_name])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[object_data]
ADD CONSTRAINT [PK_object_data] 
PRIMARY KEY CLUSTERED ([id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[olympiad_result]
ADD CONSTRAINT [PK_olympiad_result] 
PRIMARY KEY CLUSTERED ([season], [class], [char_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[pet_data]
ADD CONSTRAINT [PK_pet_data] 
PRIMARY KEY CLUSTERED ([pet_id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_petmsg_charid] ON [dbo].[PetitionMsg]
  ([Char_Id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[Pledge]
ADD CONSTRAINT [IX_Pledge] 
UNIQUE NONCLUSTERED ([name])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[Pledge]
ADD CONSTRAINT [PK_Pledge] 
PRIMARY KEY CLUSTERED ([pledge_id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[pledge_contribution]
ADD CONSTRAINT [PK_pledge_contribution] 
PRIMARY KEY CLUSTERED ([residence_id], [pledge_id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[Pledge_Crest]
ADD CONSTRAINT [PK_Pledge_Crest] 
PRIMARY KEY CLUSTERED ([crest_id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE UNIQUE NONCLUSTERED INDEX [IX_Quest] ON [dbo].[quest]
  ([char_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[req_charmove_old]
ADD CONSTRAINT [IX_req_charmove] 
UNIQUE NONCLUSTERED ([new_char_name])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[req_charmove_old]
ADD CONSTRAINT [IX_req_charmove_1] 
UNIQUE NONCLUSTERED ([old_char_id], [old_world_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[req_charmove_old]
ADD CONSTRAINT [IX_req_charmove_2] 
UNIQUE NONCLUSTERED ([account_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[req_charmove_old]
ADD CONSTRAINT [IX_req_charmove_3] 
UNIQUE NONCLUSTERED ([new_char_name], [new_world_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[residence_guard]
ADD CONSTRAINT [pk_residence_guard] 
PRIMARY KEY CLUSTERED ([x], [y], [z])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[shortcut_data]
ADD CONSTRAINT [PK_shortcut_data] 
PRIMARY KEY CLUSTERED ([char_id], [slotnum], [subjob_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[siege_agit_pledge]
ADD CONSTRAINT [sap_uniq] 
UNIQUE NONCLUSTERED ([agit_id], [pledge_id])
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_name] ON [dbo].[SkillData]
  ([name])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[SkillData]
ADD CONSTRAINT [PK_SkillData] 
PRIMARY KEY CLUSTERED ([id], [lev])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[team_battle_agit_member]
ADD CONSTRAINT [tbam_uniq] 
UNIQUE NONCLUSTERED ([agit_id], [char_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[team_battle_agit_pledge]
ADD CONSTRAINT [tbap_uniq] 
UNIQUE NONCLUSTERED ([agit_id], [pledge_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[time_data]
ADD CONSTRAINT [PK_char_data2] 
PRIMARY KEY CLUSTERED ([account_id])
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_user_ActiveSkill] ON [dbo].[user_ActiveSkill]
  ([char_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE UNIQUE NONCLUSTERED INDEX [IX_user_ActiveSkill] ON [dbo].[user_ActiveSkill]
  ([char_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_ban]
ADD CONSTRAINT [PK_user_ban] 
PRIMARY KEY CLUSTERED ([char_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_blocklist]
ADD CONSTRAINT [PK_user_blocklist] 
PRIMARY KEY CLUSTERED ([char_id], [block_char_id])
ON [PRIMARY]
GO

CREATE CLUSTERED INDEX [IX_user_comment] ON [dbo].[user_comment]
  ([char_id], [deleted], [comment_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_user_comment_1] ON [dbo].[user_comment]
  ([comment_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_user_data_1] ON [dbo].[user_data]
  ([account_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_user_data_account_name] ON [dbo].[user_data]
  ([account_name])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_user_data_pledge] ON [dbo].[user_data]
  ([pledge_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE UNIQUE NONCLUSTERED INDEX [idx_user_data1] ON [dbo].[user_data]
  ([char_name])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_data]
ADD CONSTRAINT [PK_user_data] 
PRIMARY KEY CLUSTERED ([char_id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_friend]
ADD CONSTRAINT [PK_user_friend] 
PRIMARY KEY CLUSTERED ([char_id], [friend_char_id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_henna]
ADD CONSTRAINT [PK_user_henna] 
PRIMARY KEY CLUSTERED ([char_id])
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_user_history] ON [dbo].[user_history]
  ([char_name])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_item_type] ON [dbo].[user_item]
  ([item_type])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE CLUSTERED INDEX [idx_user_item_charid] ON [dbo].[user_item]
  ([char_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_useritem_ware] ON [dbo].[user_item]
  ([warehouse])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_warehouse] ON [dbo].[user_item]
  ([warehouse])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_item]
ADD CONSTRAINT [pk_user_item] 
PRIMARY KEY NONCLUSTERED ([item_id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE CLUSTERED INDEX [IX_user_log] ON [dbo].[user_log]
  ([char_id], [log_id], [log_to])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_macro]
ADD CONSTRAINT [IX_user_macro] 
UNIQUE CLUSTERED ([char_id], [macro_id])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE CLUSTERED INDEX [IX_user_macroinfo] ON [dbo].[user_macroinfo]
  ([macro_id], [macro_order])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_user_mail_1] ON [dbo].[user_mail]
  ([id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_mail]
ADD CONSTRAINT [PK_user_mail] 
PRIMARY KEY CLUSTERED ([id])
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_user_mail_receiver_1] ON [dbo].[user_mail_receiver]
  ([mail_id])
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_user_mail_receiver_2] ON [dbo].[user_mail_receiver]
  ([receiver_id], [read_status], [deleted])
ON [PRIMARY]
GO

CREATE CLUSTERED INDEX [IX_user_mail_sender] ON [dbo].[user_mail_sender]
  ([sender_id], [mailbox_type], [deleted])
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_user_mail_sender_1] ON [dbo].[user_mail_sender]
  ([mail_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_name_color]
ADD CONSTRAINT [PK_user_name_color] 
PRIMARY KEY CLUSTERED ([char_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_name_reserved]
ADD CONSTRAINT [PK_user_name_reserved] 
PRIMARY KEY CLUSTERED ([char_name])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_newbie]
ADD CONSTRAINT [PK_user_newbie] 
PRIMARY KEY CLUSTERED ([account_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_nobless]
ADD PRIMARY KEY CLUSTERED ([char_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_prohibit]
ADD CONSTRAINT [PK_user_prohibit] 
PRIMARY KEY CLUSTERED ([char_name])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_prohibit_word]
ADD CONSTRAINT [PK_user_prohibit_word] 
PRIMARY KEY CLUSTERED ([words])
WITH FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [IX_user_punish] ON [dbo].[user_punish]
  ([char_id], [punish_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_recipe]
ADD CONSTRAINT [PK_user_recipe] 
PRIMARY KEY CLUSTERED ([char_id], [recipe_id])
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_skill_lev] ON [dbo].[user_skill]
  ([skill_lev])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_skill_toend] ON [dbo].[user_skill]
  ([to_end_time])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_skill]
ADD CONSTRAINT [PK_user_skill] 
PRIMARY KEY CLUSTERED ([char_id], [skill_id], [subjob_id])
ON [PRIMARY]
GO

ALTER TABLE [dbo].[user_sociality]
ADD CONSTRAINT [PK_user_sociality] 
PRIMARY KEY CLUSTERED ([char_id])
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_user_warehouse_charid] ON [dbo].[user_warehouse]
  ([char_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE UNIQUE NONCLUSTERED INDEX [idx_user_warehouse_itemid] ON [dbo].[user_warehouse]
  ([item_id])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

CREATE NONCLUSTERED INDEX [idx_user_warehouse_itemtype] ON [dbo].[user_warehouse]
  ([item_type])
WITH
  FILLFACTOR = 90
ON [PRIMARY]
GO

ALTER TABLE [dbo].[war_declare]
ADD CONSTRAINT [PK_war_declare] 
PRIMARY KEY CLUSTERED ([challenger], [challengee])
ON [PRIMARY]
GO

CREATE PROCEDURE [dbo].[DB_BACKUP]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_adena_rank]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_GetItemListByAccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_GetItemListByPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[dp_GetItemListByUserID]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[give_lotto_price]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_AddAllOlympiadBonusPoint]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_AddBlockList]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_AddBookMark]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_AddLottoItem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_AddOlympiadPoint]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_AddProhibit]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_AddPunishmentHistory]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_AddSubJob]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_AddSubJobHenna]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_AddUserLog]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_AdenaChange]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_AllianceWarChallengeRejected]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_AmountChange]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ApproveBattle]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ArchiveMail]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_BetaAddItem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_BreakFriendship]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CancelAgitAuction]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CancelWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ChallengeRejected]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ChangeCharacterLocation]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ChangeCharacterName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ChangeCharacterName2]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ChangeSubJob]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ChangeSubJobBySubJobId]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ChangeTimeData]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CharLogin]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CharLogout]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CheckPetName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CheckPreviousAllianceWarHistory]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CheckPreviousWarHistory]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CheckReserved]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CleanUpGhostData]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ClearGroupPoint]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CommentDelete]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CommentSearch]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CommentWrite]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CopyChar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateActiveSkill]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateAgitAuction]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateAgitBid]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateAgitIfNotExist]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateAlliance]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateCastle]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateChar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateCharWithSubjob]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateDoorDataIfNotExist]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateGroupPoint]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateItem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateLottoGame]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateLottoItem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateMacro]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateMacroInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateMonRace]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateMonRaceMon]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateMonRaceTicket]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateNewbieData]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreatePet]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreatePledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateSSQRound]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateSSQTopPointUser]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateSSQUserInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateSubJob]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_CreateTimeAttackRecord]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeclareWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DelAquireSkill]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DelBookMark]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteAgitBid]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteAllResidenceGuard]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteBBSBoard]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteBlockList]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteCastleIncome]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteCastleWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteChar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteCharClearPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteFriends]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteItem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteMacro]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteMail]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteMasterRelatedCastleWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteMercenary]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteNotOwnedItem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeletePet]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeletePledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeletePledgeContribution]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeletePreviousOlympiadPoint]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteResidenceGuard]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteSiegeAgitPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteSubJob]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteSubJobSkill]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteSurrenderWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteTeamBattleAgitMember]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DeleteTeamBattleAgitPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DelIntervalAnnounce]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DelLoginAnnounce]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DelMonRaceTicket]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DelPledgeCrest]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DelProhibit]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DelRecipeInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DelSubJobHenna]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DepositBank]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DisableChar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DismissAlliance]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_DropItem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_EnableChar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_EnableChar2]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[Lin_EnchantItem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_Err]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_Err2]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ErrItem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_EstablishFriendship]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_EventChangeGender]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_FinishPledgeWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_FlushItemName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_FlushQuestName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_FlushSkillName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetAgitAdena]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetAgitBid]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetAgitBidOne]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetAgitList]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetAllCastleSiege]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetAllGroupPoint]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetAllHeroes]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetAllMemberPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetAllNobless]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetAquireSkill]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetBbsallList]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetBBSBoard]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetBbsTGSList]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetBlockCount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetBookMark]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetBuilderAccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetCastleList]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetCharByAccountId]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetContributionRelatedPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetContributionWinnerPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetDBIDByCharName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetDbRelatedCounts]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetDefaultBuilder]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetExpBySubjob]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetGroupPoint]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetHeroByClassId]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetHeroById]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetInitBoard]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetIntervalAnnounce]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetItem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetItemAmount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetItemData]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetItemType]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetListBookMark]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetLoginAnnounce]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetMacro]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetMacroCount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetMail]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetMailCount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetMailList]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetMonRaceBet]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetMonRaceResult]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetMonRaceTaxSum]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetMonRaceTicket]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetNoblessAchievements]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetNoblessById]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetNoblessTop10]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetOneMacro]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetPetitionMsg]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetPledgeAdena]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetPledgeKillDeathCount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetPunish]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetRelatedCastleSiege]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetSendMailCount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetShortCut]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetSiegeRelatedAlliancePledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetSSQMainEventRecord]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetSSQStatus]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetSubJobLevel]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[Lin_GetSummary]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetSurrenderWarId]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetTempMail]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetUnreadMailCount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetUserDataByCharId]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetUserDataByCharName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetUserLogTime]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetWarDeclare]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_GetWearingItemList]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InitAllOlympiadPoint]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InitPledgeCrest]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InitPledgeEmblem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InsertAgitAdena]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InsertIntoControlTower]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InsertIntoMercenary]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InsertIntoResidenceGuard]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InsertIntoTeamBattleAgitMember]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InsertIntoTeamBattleAgitPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InsertItemName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InsertQuestName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InsertSkillName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InsertUserHistory]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InsertUserLog]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_InstallBattleCamp]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_JoinAlliance]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_JoinPledgeMember]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadAccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadAgit]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadAgitDeco]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadAllAgit]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadAllAllianceId]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadAllAllianceWarData]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadAllCastle]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadAlliance]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadAllianceWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadAllPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadAllWarData]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadBlockList]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadBuilderAccountByAccountName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadCastle]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadCastleCrop]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadCastleWarRelatedPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadControlTowerByName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadControlTowerByResidenceId]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadDayUsedTime]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadDismissReservedPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadDoor]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadFriends]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadHenna]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadItemByItemId]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadItems]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadLastLogout]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadLastTaxUpdate]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadLottoGame]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadLottoItems]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadManorInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadManorSeed]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadManorSeed_N]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadMarketPriceList]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadMercenary]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadNewbieData]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadNpcBoss]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadOlympiad]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadPet]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadPetItems]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadPledgeAnnounce]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadPledgeById]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadPledgeByName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadPledgeCrest]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadPledgeMember]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadPledgeWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadQuest]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadRecipeInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadResidenceGuard]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadSiegeAgitPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadSociality]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadSSQJoinInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadSSQSystemInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadSSQTopPointUser]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadSSQUserInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadTeamBattleAgitMember]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadTeamBattleAgitPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadTeamBattleAgitStatus]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_LoadTimeAttackRecord]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_MakeBBSBoard]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_MakeDailyWorldSnap]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[LIN_MakeNewBlankItem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_MakeSnapTable]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ManageUserNameReserved]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ManBookMark]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ModChar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ModChar2]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ModChar3]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ModifyAllianceName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ModifyPledgeName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ModifySendTempMail]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ModifyTempMail]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ModItemOwner]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ModOlympiadPoint]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ModSubJobAbility]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ModSubJobAbilityAbsolutely]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ModSubJobClass]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_MoveCharacter]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_MoveCharClear]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_MoveCharCopyChar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_MoveCharCopyPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_MoveCharPlg]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_MoveCharSvr]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_NewNobless]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_NewTeamBattleAgitMember]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_NewTeamBattleAgitPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_NominateHeroes]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_OustMemberPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_OustPledgeMember]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ReadBbsall]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ReadBbsTGS]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_RefreshCharacterByName]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_RegisterAccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_RegisterSiegeAgitPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ReloadOlympiadPoint]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_RenewAgitDeco]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_RenewSubjob]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ResetAgitDeco]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ResetOlympiadPoint]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ResetSociality]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_RestoreChar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_RestoreChar2]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveCastleIncome]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveCastleSiegeElapsedTime]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveCastleStatus]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveCharacter]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveCharacter_temp]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveCharacter_temp2]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveCharacterPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveCropData]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveDoorStatus]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveDropExp]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveHenna]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveHeroType]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveKillDeath]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveMail]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveManorInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveManorSeed]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveManorSeed_N]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveNoblessType]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveNpcBoss]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveObjectHp]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveOlympiadRecord]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveOlympiadTerm]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SavePet]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SavePledgeContribution]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveRecipeInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveSeasonStartTime]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveSSQJoinInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveSSQSystemInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveSSQTopPointUser]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveSSQUserInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveTimeAttackRecord]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SaveTimeData]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SeizeItem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SeizeItem2]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SendMail]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SendMailToReceiver]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetAgitAuction]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetAgitDeco]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetAgitNextCost]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_setAgitOwner]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetAgitOwner2]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetAllianceInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetAquireSkill]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetBuilderAccount]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetBuilderCharacter]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_setCastleOwner]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetCastleOwner2]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetCharacterDelete]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetCharacterDeleteRestore]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetClass]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetHistoryOpen]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetIntervalAnnounce]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetLastTaxUpdate]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetLoginAnnounce]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetMacro]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetNewbieStat]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetPetitionMsg]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetPledgeAnnounce]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetPledgeCrest]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetPledgeInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetPunish]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetQuest]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetShortCut]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetSiegeTime]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetSkillCoolTime]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetSubJobAcquireSkill]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetTeamBattleNpcType]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetTeamBattleStatus]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetTeamBattleWinner]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetUserBan]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetUserDataEx]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetUserNickname]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SetUserSSQDawnRound]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_StartAllianceWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_StartOlympiadSeason]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_StartPledgeWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_StopAllianceWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_StopPledgeWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SurrenderAllianceWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SurrenderPersonally]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SurrenderPledgeWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_SwitchPledgeAnnounceShowFlag]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_TrasferWarehouseNewAll]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_TrasferWarehouseNewPartial]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UninstallAllBattleCamp]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UninstallBattleCamp]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UnregisterSiegeAgitPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UnregisterTeamBattleAgitMember]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UnregisterTeamBattleAgitPledge]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateActiveSkill]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateCastle]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateCastleWar]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateControlTower]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateJournal]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateLottoGame]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateLottoGameState]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateMarketPrice]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateMercenary]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateMonRace]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateMonRaceInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateMonRaceMon]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateNewbieCharData]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateNpcBossVariable]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdatePledgeInfo]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_updateSociality]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_updateSulffrageUsed]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateUserItem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateUseTime]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_UpdateWarehouseItem]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_ViewSiegeList]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_WithdrawAlliance]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_WithdrawPledgeMember]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_writeBbsall]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_writeBbsTGS]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_WriteCastleTax]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_WriteHeroWords]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[lin_WriteNoblessAchievement]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[SP_DBCC]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[sp_test]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[spCopyXYZlocForDummy]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[STAT_ITEM_ACC]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[table_info]

AS
SELECT 0
GO

CREATE PROCEDURE [dbo].[TranLogBackup]

AS
SELECT 0
GO

ALTER PROC [dbo].DB_BACKUP
	@DBNAME  varCHAR(16),    
	@path  varchar(32) = 'F:\MSSQL\BACKUP\',    
	@gen  int = 7
   AS
	begin    
		SET QUOTED_IDENTIFIER OFF  
  
		declare @dev_name varchar(64)    
		declare @dev_name_del varchar(64)    
		declare @physical_name  varchar(64)    
		declare @description varchar(64)    
    
		if ( select count(*) from master.dbo.sysdatabases where name = @dbname ) = 0     
		begin     
			select 'Database "' + @DBNAME + '" does not exist.'    
			return    
		end    
    
		select @description = @DBNAME  + ' , ' + CONVERT(CHAR(20),GETDATE())    


		if datepart(hour,getdate()) =  8
		  begin
			set @GEN = 7
			select @dev_name = @@SERVERNAME + '.' + @DBNAME + '.' + convert(char(10),getdate() , 120 )    
			select @dev_name_del = @@SERVERNAME + '.' + @DBNAME + '.' + convert(char(10),getdate() - 7 , 120 )    
			select @physical_name = rtrim(upper(@PATH)) + rtrim(upper(@dev_name)) + '.PM'    
			select @description = 'Daily 2nd Backup , ' + @DBNAME  + '   ' + CONVERT(CHAR(20),GETDATE())    			
		  end 

		else if datepart(hour,getdate()) < 12 
		  begin
			select @dev_name = @@SERVERNAME + '.' + @DBNAME + ' ' + convert(char(10),getdate() , 120 )    
			select @dev_name_del = @@SERVERNAME + '.' + @DBNAME + ' ' + convert(char(10),getdate() - @gen , 120 )    
			select @physical_name = rtrim(upper(@PATH)) + rtrim(upper(@dev_name)) + '.BAK'    
			select @description = 'Daily Backup , ' + @DBNAME  + '   ' + CONVERT(CHAR(20),GETDATE())    
		  end 

     		else
		  begin
			select @dev_name = @@SERVERNAME + '.' + @DBNAME + '.' + convert(char(10),getdate() , 120 )    
			select @dev_name_del = @@SERVERNAME + '.' + @DBNAME + '.' + convert(char(10),getdate() - 1 , 120 )    
			select @physical_name = rtrim(upper(@PATH)) + rtrim(upper(@dev_name)) + '.PMBAK'    
			select @description = 'Daily 2nd Backup , ' + @DBNAME  + '   ' + CONVERT(CHAR(20),GETDATE())    			
		  end 

		if ( select count(*) from master.dbo.sysdevices where name = rtrim(@dev_name_del) ) > 0     
			EXEC sp_dropdevice @dev_name_del,'delfile'    
    
		if ( select count(*) from master.dbo.sysdevices where name = rtrim(@dev_name) ) = 0     
			EXEC sp_addumpdevice 'disk' , @dev_name , @physical_name    
    
		BACKUP DATABASE @DBNAME TO @dev_name WITH INIT , NAME = @description , NOSKIP , NOFORMAT    
  
	end
GO

/****** Object:  Stored Procedure dbo.dp_adena_rank    Script Date: 2003-09-20 ?? 11:51:56 ******/
ALTER PROCEDURE dbo.dp_adena_rank
(
@limit int
)
  AS

declare @crdate datetime

select @crdate = crdate   from dbo.sysobjects where id = object_id(N'[dbo].[dt_adena_rank_temp]') and OBJECTPROPERTY(id, N'IsUserTable') = 1

if @crdate is not null
	if  datediff(hh,@crdate, getdate()) > 1
		drop table [dbo].[dt_adena_rank_temp]
	else
		goto SelectData


CREATE TABLE [dbo].[dt_adena_rank_temp] (
		[char_id] [int] NOT NULL ,
		[amount] [numeric](18, 0) NOT NULL ,
		[rank] [int] IDENTITY (1, 1) NOT NULL 
	) ON [PRIMARY]

insert dbo.dt_adena_rank_temp (char_id, amount)
select  top 1000  char_id, sum(cast(amount as numeric )) as amount from user_item with (readpast) where item_type = 57 group by char_id
order by 2 desc


SelectData:

select  b.char_name, b.account_name, a.amount, a.rank from [dbo].[dt_adena_rank_temp] as a, user_data as b
where a.rank <= @limit and a.char_id = b.char_id 
order by rank
GO

/****** Object:  Stored Procedure dbo.dp_GetItemListByAccount    Script Date: 2003-09-20 ?? 11:51:57 ******/

ALTER PROCEDURE [dbo].[dp_GetItemListByAccount] @user_id varchar(12)
AS
SELECT b.item_id, c.name, a.char_name, b.warehouse, b.amount, b.enchant, b.eroded, b.bless, b.wished
FROM user_data a, user_item b, itemdata  c
WHERE a.account_name = @user_id
AND a.char_id = b.char_id AND b.item_type = c.id 
ORDER BY a.char_name, b.warehouse, b.item_id
GO

/****** Object:  Stored Procedure dbo.dp_GetItemListByPledge    Script Date: 2003-09-20 ?? 11:51:57 ******/

ALTER PROCEDURE [dbo].[dp_GetItemListByPledge] @user_id varchar(30)
AS
SELECT b.item_id, c.name, a.char_name, b.warehouse, b.amount, b.enchant, b.eroded, b.bless, b.wished
FROM user_data a INNER JOIN user_item b ON a.char_id = b.char_id, itemdata c
WHERE a.char_id in (SELECT char_id FROM user_data a INNER JOIN pledge b ON a.pledge_id = b.pledge_id WHERE b.name = @user_id)
AND b.item_type = c.id
ORDER BY a.char_name, b.warehouse, b.item_id
GO

/****** Object:  Stored Procedure dbo.dp_GetItemListByUserID    Script Date: 2003-09-20 ?? 11:51:58 ******/

ALTER PROCEDURE [dbo].[dp_GetItemListByUserID] @user_id varchar(12)
AS
SELECT user_item.item_id, itemdata.name, user_data.char_name, user_item.warehouse, user_item.amount, user_item.enchant, user_item.eroded, user_item.bless, user_item.wished
FROM user_data, user_item, itemdata 
WHERE char_name = @user_id
AND user_data.char_id = user_item.char_id AND user_item.item_type = itemdata.id
ORDER BY  user_item.warehouse, user_item.item_id
GO

ALTER procedure [dbo].give_lotto_price
	@log_server	varchar(16),
	@world_id	int
as

declare	@round_number int
select @round_number = max(round_number) - 2 from lotto_game(nolock)

/**
	1) lotto_game? ??? ?? ??? 1??? ??
	    - ??? ??? ?? ??? ?? ??
*/
update lotto_game
set carried_adena = 100000000 - (total_count * 2000) * 0.5
where round_number = @round_number

declare	@win1_price	int
declare	@win2_price	int
declare	@win3_price	int
select @win1_price = case winner1_count when 0 then 0 else (100000000 - (winner4_count * 2)) * 0.6 / winner1_count end,
	@win2_price = case winner2_count when 0 then 0 else (100000000 - (winner4_count * 2)) * 0.2 / winner2_count end,
	@win3_price = case winner3_count when 0 then 0 else (100000000 - (winner4_count * 2)) * 0.2 / winner3_count end
from lotto_game(nolock)
where round_number = @round_number


/**
	2) ?? ?????? ??? ??
*/
create table tmp_lotto_price_given (
	char_id	int,
	char_name	varchar(50),
	rank	int,
	price	int,
	new_price	int
)

declare	@sql	varchar(4000)
set @sql = 'insert into tmp_lotto_price_given'
	+ ' select *'
	+ ' from  openrowset(''SQLOLEDB'', ''' + @log_server + ''';''sa'';''sa'', ''exec lin2log.dbo.get_lotto_given ' + cast(@world_id as varchar) + ', ' + cast(@round_number as varchar) + ''')'
exec (@sql)

update tmp_lotto_price_given set char_id = T.char_id from user_data T(nolock) where tmp_lotto_price_given.char_name = T.char_name
print 'Candidates: ' + cast(@@rowcount as varchar)


/**
	3) ?????? ?? ??
*/
update tmp_lotto_price_given set new_price = @win1_price where rank = 1
update tmp_lotto_price_given set new_price = @win2_price where rank = 2
update tmp_lotto_price_given set new_price = @win3_price where rank = 3


/**
	4) ???? ??
*/
update user_item
set amount = amount + T.new_price - T.price
from tmp_lotto_price_given T(nolock)
where item_type = 57
	and warehouse = 0
	and user_item.char_id = T.char_id
	and T.new_price > T.price
print 'Updated: ' + cast(@@rowcount as varchar)

insert into user_item (char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse)
select T.char_id, 57, new_price - price, 0, 0, 0, 0, 0, 0
from tmp_lotto_price_given T(nolock)
where T.char_id not in (select char_id from user_item(nolock) where item_type = 57 and warehouse = 0)
	and T.new_price > T.price
print 'Inserted: ' + cast(@@rowcount as varchar)


/**
	5) ?? ??? ??
drop table tmp_lotto_price_given
*/
GO

ALTER PROCEDURE dbo.lin_AddAllOlympiadBonusPoint
(
@season INT,
@step INT,
@bonus_point INT
)
AS
SET NOCOUNT ON

UPDATE user_nobless
SET olympiad_point = olympiad_point + @bonus_point

UPDATE olympiad
SET step = @step
WHERE season = @season
GO

/********************************************  
lin_AddBlockList  
 add character's blocked list.  
INPUT  
 char_id,  
 target char_name  
OUTPUT  
return  
made by  
 carrot  
date  
 2003-12-01  
change  
********************************************/  
ALTER PROCEDURE [DBO].[lin_AddBlockList]  
(  
 @char_id INT,  
 @target_char_name NVARCHAR(50)  
)  
AS  
  
SET NOCOUNT ON  
  
DECLARE @target_char_id INT  
DECLARE @target_builder_lev INT  
SET @target_char_id = 0  
SET @target_builder_lev  = 0

SELECT @target_char_id = char_id, @target_builder_lev = builder FROM user_data WHERE char_name = @target_char_name  

IF (@target_builder_lev  > 0 AND @target_builder_lev  <= 5)
BEGIN
  RAISERROR ('Try block builder : char id = [%d], target naem[%s]', 16, 1, @char_id, @target_char_name)  
  RETURN -1;
END
  
IF @target_char_id > 0  
BEGIN  
 INSERT INTO user_blocklist VALUES (@char_id, @target_char_id, @target_char_name)  
 IF NOT @@ROWCOUNT = 1  
 BEGIN  
  RAISERROR ('Cannot find add blocklist: char id = [%d], target naem[%s]', 16, 1, @char_id, @target_char_name)  
 END  
 ELSE  
 BEGIN  
  SELECT @target_char_id  
 END  
END  
ELSE  
BEGIN  
 RAISERROR ('Cannot find add blocklist: char id = [%d], target naem[%s]', 16, 1, @char_id, @target_char_name)  
END
GO

/****** Object:  Stored Procedure dbo.lin_AddBookMark    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_AddBookMark
	add bookmark item
INPUT
	@char_id	int,
	@bookmark_name	nvarchar(50),
	@world		int,
	@x		int,
	@y		int,
	@z		int
OUTPUT

return
made by
	young
date
	2002-11-13
********************************************/
ALTER PROCEDURE [DBO].[lin_AddBookMark]
(
	@char_id	int,
	@bookmark_name	nvarchar(50),
	@world		int,
	@x		int,
	@y		int,
	@z		int
)
AS
SET NOCOUNT ON

if exists(select name from bookmark where char_id = @char_id and name = @bookmark_name)
begin
	update bookmark set world=@world, x=@x, y=@y, z=@z where char_id = @char_id and name = @bookmark_name
end 
else 
begin
	insert into bookmark(char_id, name, world, x, y, z) 
	values( @char_id, @bookmark_name, @world, @x,@y,@z)
end
GO

ALTER PROCEDURE [DBO].[lin_AddLottoItem]  
(  
 @round int ,
 @item_id int ,
 @number_flag int
)  
AS  
  
SET NOCOUNT ON  

insert lotto_items
values (@round, @item_id, @number_flag)

declare @tot_count int

select @tot_count = count(*) from lotto_items (nolock) where round_number = @round
update lotto_game set total_count = @tot_count where round_number = @round
select @tot_count
GO

ALTER PROCEDURE dbo.lin_AddOlympiadPoint
(
@char_id INT,
@diff INT
)
AS
SET NOCOUNT ON

UPDATE user_nobless
SET olympiad_point = olympiad_point + @diff
WHERE char_id = @char_id

DECLARE @olympiad_point INT
SELECT @olympiad_point = olympiad_point FROM user_nobless WHERE char_id = @char_id
IF @olympiad_point < 0
BEGIN
	UPDATE user_nobless
	SET olympiad_point = 0
	WHERE char_id = @char_id
END
GO

/****** Object:  Stored Procedure dbo.lin_AddProhibit    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_AddProhibit
	
INPUT	
	@char_name	nvarchar(50)
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_AddProhibit]
(
	@char_name	nvarchar(50),
	@noption	int
)
AS
SET NOCOUNT ON

if ( @noption = 1) 
	insert into user_prohibit values (@char_name)
else if ( @noption = 3)
	insert into user_prohibit_word values (@char_name)
GO

/****** Object:  Stored Procedure dbo.lin_InsertUserHistory    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_InsertUserHistory
	make user history log
INPUT
	@char_name	nvarchar(50),
	@char_id	int,
	@log_action	tinyint,
	@account_name	nvarchar(50),
	@create_date		datetime
OUTPUT
return
made by
	young
date
	2003-1-14
********************************************/
ALTER PROCEDURE [DBO].[lin_InsertUserHistory]
(
	@char_name	nvarchar(50),
	@char_id	int,
	@log_action	tinyint,
	@account_name	nvarchar(50),
	@create_date		datetime
)
AS
SET NOCOUNT ON

declare @create_date2 datetime

if @create_date is NULL
begin
	set @create_date2 = getdate()
end
else
begin
	set @create_date2 = @create_date
end
insert into user_history( char_name, char_id, log_action, account_name, create_date) values
( @char_name, @char_id, @log_action, @account_name, @create_date2)
GO

/********************************************
lin_AddPunishmentHistory
	Add Punishment Histroy to user_history
INPUT
	@char_name	NVARCHAR(24),
	@char_id	INT,
	@log_type	TINYINT,
	@account_name	NVARCHAR(24)
OUTPUT
	
return
made by
	zzangse
date
	2005-08-31			created by zzangse
********************************************/
ALTER  PROCEDURE [dbo].[lin_AddPunishmentHistory] 
(
	@char_name	NVARCHAR(24),
	@char_id	INT,
	@log_type	TINYINT,
	@account_name	NVARCHAR(24)
)
AS

SET NOCOUNT ON  
SET @char_name = RTRIM(@char_name)  
DECLARE @create_date datetime

SELECT
	@create_date = create_date
FROM user_data
WHERE char_id = @char_id

if @char_id>0
begin  
 -- make user_history  
 exec lin_InsertUserHistory @char_name, @char_id, @log_type, @account_name, @create_date 
end
GO

/******************************************************************************
#Name:	lin_AddSubjob
#Desc:	add subjob

#Argument:
	Input:	@char_id	INT
		@new_class	TINYINT
		@hp		FLOAT
		@mp		FLOAT
		@sp		INT
		@exp		INT
		@level		TINYINT
		@subjob_id	TINYINT
		@henna_1	INT
		@henna_2	INT
		@henna_3	INT
	Output:
#Return:	Success(1) / Failure(0)
#Result Set:

#Remark:
#Example:
#See:
#History:
	Create	btwinuni	2005-07-28
	Modify	btwinuni	2005-09-07
******************************************************************************/
ALTER PROCEDURE [dbo].[lin_AddSubJob]
(
	@char_id	INT,
	@new_class	TINYINT,
	@hp		FLOAT,
	@mp		FLOAT,
	@sp		INT,
	@exp		INT,
	@level		TINYINT,
	@subjob_id	TINYINT,
	@henna_1	INT,
	@henna_2	INT,
	@henna_3	INT
)
AS

SET NOCOUNT ON

DECLARE @ret INT
SELECT @ret = 1

BEGIN TRAN

-- subjob_id duplication check
IF EXISTS(SELECT subjob_id FROM user_subjob WHERE char_id = @char_id AND subjob_id = @subjob_id)
BEGIN
	SELECT @ret = 0
	GOTO EXIT_TRAN
END

ELSE	-- insert
BEGIN
	INSERT INTO user_subjob
	(char_id, hp, mp, sp, exp, level, henna_1, henna_2, henna_3, subjob_id, create_date)
	VALUES
	(@char_id, @hp, @mp, @sp, @exp, @level, @henna_1, @henna_2, @henna_3, @subjob_id, getdate())

	DECLARE @sql VARCHAR(1000)
	SET @sql = 'UPDATE user_data '
		+ ' SET subjob' + cast(@subjob_id as varchar) + '_class = ' + cast(@new_class as varchar)
		+ ' WHERE char_id = ' + cast(@char_id as varchar)
	EXEC (@sql)
END

IF @@ERROR <> 0 OR @@ROWCOUNT <> 1	-- update, insert check
BEGIN
	SELECT @ret = 0
	GOTO EXIT_TRAN
END

EXIT_TRAN:
IF @ret<> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END

SELECT @ret
GO

/********************************************
lin_AddSubJobHenna
	
INPUT
	@char_id	int,
	@subjob_id	int,
	@henna	int

OUTPUT
return
made by
	kks
date
	2005-01-18
********************************************/
ALTER PROCEDURE [DBO].[lin_AddSubJobHenna]
(
	@char_id	int,
	@subjob_id	int,
	@henna	int
)
AS
SET NOCOUNT ON

declare @henna1 int
declare @henna2 int
declare @henna3 int

set @henna1 = 0
set @henna2 = 0
set @henna3 = 0

select @henna1 = isnull(henna_1, 0), @henna2 = isnull(henna_2, 0), @henna3 = isnull(henna_3, 0) from user_subjob where char_id = @char_id and subjob_id = @subjob_id

if (@henna1 = 0)
begin
	update user_subjob set henna_1 = @henna where char_id = @char_id and subjob_id = @subjob_id
end
else if  (@henna2 = 0)
begin
	update user_subjob set henna_2 = @henna where char_id = @char_id and subjob_id = @subjob_id
end
else if (@henna3 = 0)
begin
	update user_subjob set henna_3 = @henna where char_id = @char_id and subjob_id = @subjob_id
end
GO

/********************************************
lin_AddUserLog
	
INPUT	
	@char_id	int,
	@log_id		int,
	@log_from	int,
	@log_to		int,
	@use_time	int,
	@subjob_id	int
OUTPUT
return
made by
	carrot
date
	2002-06-16
modified by 
	kks
date
	2005-01-17	
modified by
	btwinuni
date
	2005-10-24
********************************************/
ALTER PROCEDURE [DBO].[lin_AddUserLog]
(
	@char_id	int,
	@log_id		int,
	@log_from	int,
	@log_to		int,
	@use_time	int,
	@subjob_id	int = -1
)
AS
SET NOCOUNT ON


if @subjob_id < 0
begin
	SELECT @subjob_id = subjob_id FROM user_data(NOLOCK) WHERE char_id = @char_id
end

insert into user_log(char_id, log_id, log_from, log_to, use_time, subjob_id) values(@char_id, @log_id, @log_from, @log_to, @use_time, @subjob_id)
GO

/****** Object:  Stored Procedure dbo.lin_AdenaChange    Script Date: 2003-09-20 ?? 11:51:56 ******/
/********************************************
lin_AdenaChange
	change adena and return result amount
INPUT
	@char_id	INT,
	@price	 	INT
OUTPUT
	adena id		INT,
	amount		INT
return
made by
	carrot
date
	2002-04-22
********************************************/
ALTER PROCEDURE [DBO].[lin_AdenaChange]
(
	@char_id	INT,
	@price	 	INT
)
AS
SET NOCOUNT ON

DECLARE @nAmount	INT

SET @nAmount = NULL

SELECT @nAmount = amount FROM user_item WHERE char_id = @char_id AND item_type = 57
IF @nAmount IS NULL
	BEGIN
		SELECT -1, -1
	END
ELSE IF @nAmount + @price < 0
	BEGIN
		SELECT -1, -1
	END
ELSE
	BEGIN
		UPDATE user_item SET amount = amount + @price WHERE char_id = @char_id AND item_type = 57 AND warehouse = 0
		SELECT item_id, amount FROM user_item WHERE char_id = @char_id AND item_type = 57 AND warehouse = 0
	END
GO

/********************************************
lin_AllianceWarChallengeRejected
	
INPUT	
	@challenger int, 
	@challenger_name nvarchar(50),
	@challengee int, 
	@challengee_name nvarchar(50),
	@begin_time int, 
	@status int
OUTPUT
return
made by
	bert
date
	2003-11-04
********************************************/
ALTER PROCEDURE [DBO].[lin_AllianceWarChallengeRejected]
(
	@challenger int, 
	@challenger_name nvarchar(50),
	@challengee int, 
	@challengee_name nvarchar(50),
	@begin_time int, 
	@status int
)
AS
SET NOCOUNT ON

INSERT INTO alliance_war (challenger, challenger_name,  challengee, challengee_name, begin_time, status) 
VALUES (@challenger, @challenger_name,  @challengee, @challengee_name, @begin_time, @status)
GO

/****** Object:  Stored Procedure dbo.lin_AmountChange    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_AmountChange
	change item's amount and return result amount
INPUT
	@char_id	INT,
	@item_id 	INT,
	@change 	INT,
	@bZeorDelete 	INT = 0
OUTPUT
	amount		INT
return
made by
	carrot
date
	2002-04-22
change
	2002-10-15
		??? ??? 0? ??? ? ??? ?? ?? ??? ??
change
	2003-05-12
		?? ??? ??? ??? ??
********************************************/
ALTER PROCEDURE [DBO].[lin_AmountChange]
(
	@char_id	INT,
	@item_id 	INT,
	@change 	INT,
	@bZeorDelete 	INT = 0
)
AS
SET NOCOUNT ON

DECLARE @nResultAmount 	INT
SET @nResultAmount = -1

IF(select top 1 amount from user_item where char_id = @char_id AND item_id = @item_id ) + @change >= 0
begin
	UPDATE user_item SET amount = amount + @change WHERE char_id = @char_id AND item_id = @item_id
	
	IF NOT @@ROWCOUNT = 1 
		SELECT -1
	ELSE
		SELECT @nResultAmount = ISNULL(amount, -1) FROM user_item WHERE char_id = @char_id AND item_id = @item_id
		IF ( @nResultAmount = 0 AND @bZeorDelete = 1) 
		BEGIN
			UPDATE user_item SET char_id = 0, item_type = 0 WHERE char_id = @char_id AND item_id = @item_id
	--		UPDATE user_item SET char_id = 0, item_type = 0 WHERE char_id = @char_id AND item_id = @item_id
		END
	
		SELECT @nResultAmount
end
else
select -1
GO

/****** Object:  Stored Procedure dbo.lin_ApproveBattle    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_ApproveBattle
	
INPUT	
	@castle_id	int,
	@type	int
OUTPUT
	pledge_id, 
	name 
	type
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_ApproveBattle]
(
	@castle_id	int,
	@type	int
)
AS
SET NOCOUNT ON

SELECT 
	p.pledge_id, 
	p.name, 
	type 
FROM 
	pledge p (nolock), 
	castle_war cw (nolock) 
WHERE 
	p.pledge_id = cw.pledge_id 
	AND cw.castle_id = @castle_id
	AND cw.type <> @type
GO

/********************************************
lin_ArchiveMail
	archive mail
INPUT
	@char_id		int,
	@mail_id		int,
	@mailbox_type			int

OUTPUT
return
made by
	kks
date
	2004-12-19
********************************************/
ALTER PROCEDURE [DBO].[lin_ArchiveMail]
(
	@char_id		int,
	@mail_id		int,
	@mailbox_type			int
)
AS
SET NOCOUNT ON

IF @mailbox_type = 0
BEGIN
	UPDATE user_mail_receiver
	SET mailbox_type = 2
	WHERE mail_id = @mail_id AND
		receiver_id = @char_id
END

if @mailbox_type = 1
BEGIN
	UPDATE user_mail_sender
	SET mailbox_type = 2
	WHERE mail_id = @mail_id AND
		sender_id = @char_id 
END
GO

/****** Object:  Stored Procedure dbo.lin_BetaAddItem    Script Date: 2003-09-20 ?? 11:51:56 ******/
ALTER PROCEDURE [DBO].[lin_BetaAddItem]  
(
	@char_id int,
	@Item_type int,
	@amount int
)
AS

SET NOCOUNT ON

DECLARE @tempRowCount INT
DECLARE @bIsStackable TINYINT
SELECT @bIsStackable = IsStackable FROM ITEMNAME WHERE id = @Item_type

If @bIsStackable Is NULL 
Begin
	RAISERROR ('Not exist Item Type', 16, 1)
End
Else
Begin
	If @bIsStackable = 1
	Begin
		Update user_item set amount = amount + @amount  where item_type = @Item_type and char_id = @char_id
		Set @tempRowCount = @@ROWCOUNT
		If @tempRowCount = 0
			INSERT INTO user_item (char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse) VALUES (@char_id, @Item_type, @amount, 0,0,0,0,0,0)
	End
	Else If @amount = 1
	Begin
		INSERT INTO user_item (char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse) VALUES (@char_id, @Item_type, @amount, 0,0,0,0,0,0)
		Set @tempRowCount = @@ROWCOUNT
	End
	Else
	Begin
		RAISERROR ('Amount is invalid', 16, 1)
	End
End

If @tempRowCount Is NOT NULL
	Select @tempRowCount
GO

/****** Object:  Stored Procedure dbo.lin_BreakFriendship    Script Date: 2003-09-20 ?? 11:51:58 ******/
-- lin_BreakFriendship
-- by bert

ALTER PROCEDURE dbo.lin_BreakFriendship (@char_id INT, @friend_char_id INT)
AS

SET NOCOUNT ON

DECLARE @ret INT

DELETE FROM user_friend
WHERE
(char_id = @char_id AND friend_char_id = @friend_char_id)
OR
(char_id = @friend_char_id AND friend_char_id = @char_id)

IF @@ERROR = 0 AND @@ROWCOUNT = 2
BEGIN
	SELECT @ret = 1
END
ELSE
BEGIN
	SELECT @ret = 0
END

SELECT @ret
GO

/********************************************
lin_CancelAgitAuction
	cancel agit auction
INPUT
	@agit_id	int,


OUTPUT
return
made by
	young
date
	2003-12-11
********************************************/
ALTER PROCEDURE [DBO].[lin_CancelAgitAuction]
(
@agit_id		INT,
@last_cancel		INT
)
AS
SET NOCOUNT ON

declare @auction_id int
set @auction_id = 0

select @auction_id = isnull(auction_id , 0) from agit (nolock) where id = @agit_id

update agit set auction_id = 0 , last_cancel = @last_cancel where id = @agit_id

select @auction_id
GO

ALTER PROCEDURE dbo.lin_CancelWar
(
@challenger INT,
@challengee INT
)
AS
SET NOCOUNT ON

DELETE FROM war_declare
WHERE challenger = @challenger AND challengee = @challengee
GO

/****** Object:  Stored Procedure dbo.lin_ChallengeRejected    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_ChallengeRejected
	
INPUT	
	@challenger int, 
	@challenger_name nvarchar(50),
	@challengee int, 
	@challengee_name nvarchar(50),
	@begin_time int, 
	@status int
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_ChallengeRejected]
(
	@challenger int, 
	@challenger_name nvarchar(50),
	@challengee int, 
	@challengee_name nvarchar(50),
	@begin_time int, 
	@status int
)
AS
SET NOCOUNT ON

INSERT INTO pledge_war (challenger, challenger_name,  challengee, challengee_name, begin_time, status) 
VALUES (@challenger, @challenger_name,  @challengee, @challengee_name, @begin_time, @status)
GO

/****** Object:  Stored Procedure dbo.lin_ChangeCharacterLocation    Script Date: 2003-09-20 ?? 11:51:56 ******/
/********************************************
lin_ChangeCharacterLocation
	Set Character location
INPUT
	@char_name	nvarchar,
	@nWorld		SMALLINT,
	@nX		INT,
	@nY		INT,
	@nZ		INT
OUTPUT
	char_id
return
made by
	carrot
date
	2002-07-02
********************************************/
ALTER PROCEDURE [DBO].[lin_ChangeCharacterLocation]
(
	@char_name	NVARCHAR(24),
	@nWorld		INT,
	@nX		INT,
	@nY		INT,
	@nZ		INT
)
AS

SET NOCOUNT ON

DECLARE @Char_id INT
SET @Char_id = 0

UPDATE user_data SET world = @nWorld, xLoc = @nX , yLoc = @nY , zLoc = @nZ WHERE char_name = @char_name
IF @@ROWCOUNT > 0
BEGIN
	SELECT @Char_id = char_id FROM user_data WHERE char_name = @char_name
END

SELECT @Char_id
GO

/****** Object:  Stored Procedure dbo.lin_ChangeCharacterName    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_ChangeCharacterName
	change character name
INPUT
	@old_char_name	nvarchar,
	@new_char_name	nvarchar
OUTPUT
	char_id
return
made by
	young
date
	2002-010-08
********************************************/
ALTER PROCEDURE [DBO].[lin_ChangeCharacterName]
(
	@old_char_name	NVARCHAR(24),
	@new_char_name	NVARCHAR(24)
)
AS

SET NOCOUNT ON

DECLARE @Char_id INT
DECLARE @nTmpCount INT
DECLARE @account_name NVARCHAR(50)
DECLARE @create_date datetime

SET @Char_id = 0

IF not exists(SELECT char_name FROM user_data WHERE char_name = @new_char_name)
BEGIN
	UPDATE user_data set char_name = @new_char_name where char_name = @old_char_name
	IF @@ROWCOUNT > 0
	BEGIN
		SELECT @char_id = char_id , @account_name = account_name, @create_date = create_date  FROM user_data WHERE char_name = @new_char_name
	END
END

SELECT @Char_id

if @char_id > 0
begin
	-- make user_history
	exec lin_InsertUserHistory @new_char_name, @char_id, 3, @account_name, @create_date
end
GO

/********************************************
lin_ChangeCharacterName2
	change character name
INPUT
	@char_id	int,
	@old_char_name	nvarchar,
	@new_char_name	nvarchar
OUTPUT
	char_id
return
made by
	kks
date
	2004-12-21
********************************************/
ALTER PROCEDURE [DBO].[lin_ChangeCharacterName2]
(
	@char_id		INT,
	@old_char_name	NVARCHAR(24),
	@new_char_name	NVARCHAR(24)
)
AS

SET NOCOUNT ON

DECLARE @nTmpCount INT
DECLARE @account_name NVARCHAR(50)
DECLARE @create_date datetime

DECLARE @ret_char_id INT
SET @ret_char_id = 0

IF not exists(SELECT char_name FROM user_data WHERE char_name = @new_char_name and char_id != @char_id)
BEGIN
	UPDATE user_data set char_name = @new_char_name where char_name = @old_char_name
	
	IF @@ROWCOUNT > 0
	BEGIN
		SELECT @char_id = char_id , @account_name = account_name, @create_date = create_date  FROM user_data WHERE char_name = @new_char_name
	
		-- make user_history
		exec lin_InsertUserHistory @new_char_name, @char_id, 3, @account_name, @create_date

		set @ret_char_id = @char_id
	END
END

SELECT @ret_char_id
GO

ALTER PROCEDURE dbo.lin_ChangeSubJob
(
	@char_id	INT,
	@subjob_id	TINYINT,
	@old_subjob_id	TINYINT,
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

SET NOCOUNT ON

DECLARE @ret INT
SELECT @ret = 0

BEGIN TRAN

UPDATE user_subjob
SET hp = @hp, mp = @mp, sp = @sp, exp = @exp, level = @level, 
	henna_1 = @henna_1, henna_2 = @henna_2, henna_3 = @henna_3
WHERE char_id = @char_id AND subjob_id = @old_subjob_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1	-- update, insert check
BEGIN
	SELECT @ret = 1
END
ELSE
BEGIN
	SELECT @ret = 0
END

IF @ret<> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END

SELECT hp, mp, sp, exp, level, henna_1, henna_2, henna_3 FROM user_subjob
	WHERE char_id = @char_id AND subjob_id = @subjob_id
GO

/********************************************
lin_ChangeSubJobBySubJobId
	get temp mail 
INPUT
	@char_id		int,
	@subjob_id		int
OUTPUT
return
made by
	kks
date
	2005-04-06
	2005-09-07	modified by btwinuni
********************************************/
ALTER PROCEDURE [DBO].[lin_ChangeSubJobBySubJobId]
(
	@char_id		int,
	@subjob_id		int
)
AS
SET NOCOUNT ON


IF EXISTS(SELECT TOP 1 char_id FROM user_data(NOLOCK) WHERE char_id = @char_id AND subjob_id = @subjob_id)
BEGIN
	RETURN
END

-- update subjob char property
UPDATE 	user_subjob 
SET hp = R.hp,
	mp = R.mp,
	sp = R.sp,
	exp = R.exp,
	level = R.lev
FROM ( SELECT hp, mp, sp, exp, lev, subjob_id FROM user_data(nolock) WHERE  char_id = @char_id ) AS R
WHERE user_subjob.char_id = @char_id
	AND user_subjob.subjob_id = R.subjob_id


-- update subjob char henna
UPDATE user_subjob
SET henna_1 = R.henna_1,
	henna_2 = R.henna_2,
	henna_3 = R.henna_3
FROM ( SELECT henna_1, henna_2, henna_3 FROM user_henna(nolock) WHERE char_id = @char_id ) AS R
WHERE user_subjob.char_id = @char_id
	AND user_subjob.subjob_id = @subjob_id


-- update user data property
UPDATE 	user_data
SET hp = R.hp,
	mp = R.mp,
	sp = R.sp,
	exp = R.exp,
	lev = R.level,
	subjob_id = @subjob_id
FROM ( SELECT hp, mp, sp, exp, level FROM user_subjob(nolock) WHERE char_id = @char_id AND subjob_id = @subjob_id ) AS R
WHERE user_data.char_id = @char_id

declare @sql varchar(1000)
set @sql = 'update user_data '
	+ ' set class = subjob' + cast(@subjob_id as varchar) + '_class '
	+ ' where char_id = ' + cast(@char_id as varchar)
exec (@sql)


-- update char henna
UPDATE user_henna
SET henna_1 = R.henna_1,
	henna_2 = R.henna_2,
	henna_3 = R.henna_3
FROM ( SELECT henna_1, henna_2, henna_3 FROM user_subjob(nolock) WHERE char_id = @char_id AND subjob_id = @subjob_id ) AS R
WHERE user_henna.char_id = @char_id
GO

/********************************************        
lin_ChangeTimeData      
 chagne time data table      
INPUT        
 @char_name NVARCHAR(30),        
 @nType INT,        
 @nChangeMin INT        
OUTPUT        
 changed acount id      
 result used sec      
return        
       
made by        
 carrot        
date        
 2004-04-29        
change        
********************************************/        
ALTER PROCEDURE [DBO].[lin_ChangeTimeData]      
(        
 @char_name NVARCHAR(30),        
 @nType INT,        
 @nChangeMin INT        
)        
AS        
        
SET NOCOUNT ON        
        
      
SET @nChangeMin = @nChangeMin * 60      -- change min to sec
IF (@nType < 1 OR @nType > 2 OR @nChangeMin <= 0) -- 1 : add, 2 : del
BEGIN        
    RAISERROR ('Not valid parameter : charnname[%s] type[%d], min[%d] ',16, 1,  @char_name,  @nType , @nChangeMin )        
    RETURN -1        
END        
        
DECLARE @account_id INT        
SET @account_id = 0    
SELECT @account_id = account_id FROM user_data (nolock) WHERE char_name = @char_name      
        
IF (@account_id <= 0)        
BEGIN        
    RAISERROR ('Not valid account id : charnname[%s] type[%d], min[%d] ',16, 1,  @char_name,  @nType , @nChangeMin )        
    RETURN -1        
END        
      
DECLARE @used_sec INT        
SET @used_sec = -1      
SELECT TOP 1 @used_sec = used_sec FROM time_data WHERE account_id = @account_id      
IF (@used_sec < 0 )      
BEGIN      
    RAISERROR ('Not exist time data : account_id[%d], charnname[%s] type[%d], min[%d] ',16, 1,  @account_id, @char_name,  @nType , @nChangeMin )        
    RETURN -1        
END      
      
IF (@nType = 1) -- add      
BEGIN      
 UPDATE time_data SET used_sec = @used_sec + @nChangeMin WHERE account_id = @account_id      
END      
ELSE IF (@nType = 2) -- del      
BEGIN      
 IF (@used_sec < @nChangeMin)      
 BEGIN      
  SET @nChangeMin = @used_sec
 END      
 UPDATE time_data SET used_sec = @used_sec - @nChangeMin WHERE account_id = @account_id      
END      
ELSE      
BEGIN      
    RAISERROR ('Not valid parameter : charnname[%s] type[%d], min[%d] ',16, 1,  @char_name,  @nType , @nChangeMin )        
    RETURN -1        
END      
      
SELECT TOP 1 account_id, used_sec FROM time_data WHERE account_id = @account_id
GO

/****** Object:  Stored Procedure dbo.lin_CharLogin    Script Date: 2003-09-20 ?? 11:51:56 ******/
/********************************************
lin_CharLogin
	log character login
INPUT
	char_id
OUTPUT
return
made by
	carrot
date
	2002-06-11
change
********************************************/
ALTER PROCEDURE [DBO].[lin_CharLogin]
(
	@char_id	INT
)
AS

SET NOCOUNT ON

UPDATE user_data SET login = GETDATE() WHERE char_id = @char_id
GO

/********************************************    
lin_CharLogout    
 log character Logout    
INPUT    
 char_id    
OUTPUT    
return    
 week play time by sec    
made by    
 carrot    
date    
 2002-06-11    
change    
 2002.-12-20    
  add usetime set    
 2004-03-29    
  add function to write today's used sec    
********************************************/    
ALTER PROCEDURE [DBO].[lin_CharLogout]    
(    
 @char_id INT,    
 @usedTimeSec INT
)    
AS    
    
SET NOCOUNT ON    
    
DECLARE @logoutDate DATETIME    
SET @logoutDate = GETDATE()    
    
UPDATE user_data SET Logout = @logoutDate, use_time = use_time + @usedTimeSec WHERE char_id = @char_id
GO

ALTER PROCEDURE [DBO].[lin_CheckPetName]
(
@pet_name NVARCHAR(24)
)
AS

SET NOCOUNT ON

declare @result  int
set @result = 1

SET @pet_name = RTRIM(@pet_name)
IF @pet_name  LIKE N' ' 
BEGIN
	RAISERROR ('pet name has space : name = [%s]', 16, 1, @pet_name)
	set @result = -1
END

-- check user_prohibit 
if exists(select char_name from user_prohibit (nolock) where char_name = @pet_name)
begin
	RAISERROR ('Pet  name is prohibited: name = [%s]', 16, 1, @pet_name)
	set @result = -2
end

-- prohibit word
declare @user_prohibit_word nvarchar(20)
select top 1 @user_prohibit_word = words from user_prohibit_word (nolock) where PATINDEX('%' + words + '%', @pet_name) > 0 
if @user_prohibit_word is not null
begin
	RAISERROR ('pet  name has prohibited word: name = [%s], word[%s]', 16, 1, @pet_name, @user_prohibit_word)
	set @result = -3
end

-- check reserved name
--declare @reserved_name nvarchar(50)
--select top 1 @reserved_name = char_name from user_name_reserved (nolock) where used = 0 and char_name = @pet_name
--if not @reserved_name is null
--begin
--	RAISERROR ('pet name is reserved by other player: name = [%s]', 16, 1, @pet_name)
--	set @result = -4
--end

-- check duplicated pet name
declare @dup_pet_name nvarchar(50)
select top 1 @dup_pet_name = nick_name from pet_data (nolock) where nick_name = @pet_name
if not @dup_pet_name is null
begin
	RAISERROR ('duplicated pet name[%s]', 16, 1, @pet_name)
	set @result = -4
end

select @result
GO

/********************************************
lin_CheckPreviousAllianceWarHistory
	
INPUT	
	@challenger_alliance_id		int
	@challengee_alliance_id		int

OUTPUT

made by
	bert
date
	2003-11-04
********************************************/

ALTER PROCEDURE [DBO].[lin_CheckPreviousAllianceWarHistory]
(
	@challenger		int,
	@challengee		int
)
AS
SET NOCOUNT ON

SELECT 
	id, challenger, challengee, status, begin_time 
FROM 
	alliance_war (nolock)  
WHERE 
	(challenger = @challenger AND challengee = @challengee) 
	OR (challengee = @challengee AND challenger = @challenger) 
ORDER BY 
	begin_time DESC
GO

/****** Object:  Stored Procedure dbo.lin_CheckPreviousWarHistory    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_CheckPreviousWarHistory
	
INPUT	
	@residence_id		int
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_CheckPreviousWarHistory]
(
	@challenger1		int,
	@challengee1		int,
	@challengee2		int,
	@challenger2		int
)
AS
SET NOCOUNT ON

SELECT 
	id, challenger, challengee, status, begin_time 
FROM 
	pledge_war (nolock)  
WHERE 
	(challenger = @challenger1 AND challengee = @challengee1) 
	OR (challengee = @challengee2 AND challenger = @challenger2) 
ORDER BY 
	begin_time DESC
GO

/****** Object:  Stored Procedure dbo.lin_CheckReserved    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_CheckReserved
	
INPUT
	@char_name	NCHAR(12),
	@account_name	NCHAR(13),
	@account_id	INT
OUTPUT
	
return
made by
	carrot
date
	2003-07-09
change
********************************************/
ALTER PROCEDURE [DBO].[lin_CheckReserved]
(
@char_name	NVARCHAR(24),
@account_name	NVARCHAR(24),
@account_id	INT
)
AS

SET NOCOUNT ON

SET @char_name = RTRIM(@char_name)

-- check reserved name
declare @reserved_name nvarchar(50)
declare @reserved_account_id int
select top 1 @reserved_name = char_name, @reserved_account_id = account_id from user_name_reserved (nolock) where used = 0 and char_name = @char_name
if not @reserved_name is null
begin
	if not @reserved_account_id = @account_id
	begin
		RAISERROR ('Character name is reserved by other player: name = [%s]', 16, 1, @char_name)
		RETURN -1
	end
end
GO

/********************************************    
lin_CleanUpGhostData  
     
INPUT    
OUTPUT    
    
return    
made by    
 bert   
date    
 2004-04-27    
 2005-11-21	by btwinuni	delete pledge_war, alliance_war
 2005-12-20	by btwinuni	add clean up war_declare
********************************************/    
ALTER PROCEDURE dbo.lin_CleanUpGhostData  
AS  
SET NOCOUNT ON  
  
-- ?? ??  
UPDATE user_data  
SET pledge_id = 0, nickname = ''  
WHERE account_id IN (-1, -3)  
  
-- ?? ?? ??  
DELETE FROM user_friend  
WHERE  
char_id IN (SELECT char_id FROM user_data WHERE account_id IN (-1, -3))  
OR  
friend_char_id IN (SELECT char_id FROM user_data WHERE account_id IN (-1, -3))  

-- ?? ???? ??  
DELETE FROM user_blocklist
WHERE  
char_id IN (SELECT char_id FROM user_data WHERE account_id IN (-1, -3))  
OR  
block_char_id IN (SELECT char_id FROM user_data WHERE account_id IN (-1, -3))  
  
-- ?? ?? ?? (???? ?? ?)  
DELETE FROM pledge WHERE ruler_id IN (SELECT char_id FROM user_data WHERE account_id IN (-1, -3))  
  
-- ???? ????  
UPDATE user_data  
SET pledge_id = 0  
WHERE pledge_id <> 0 AND pledge_id NOT IN (SELECT pledge_id FROM pledge)  
  
UPDATE user_data  
SET nickname = ''  
WHERE pledge_id = 0  
  
-- ?? ??? ??? ??  
UPDATE agit  
SET pledge_id = 0  
WHERE pledge_id <> 0 AND pledge_id NOT IN (SELECT pledge_id FROM pledge)  
  
-- ?? ? ??? ??  
UPDATE castle  
SET pledge_id = 0  
WHERE pledge_id <> 0 AND pledge_id NOT IN (SELECT pledge_id FROM pledge)  
  
-- 2005-12-20	by btwinuni
-- ?? ?? ?? ??  
DELETE FROM war_declare
WHERE  
(challengee <> 0 AND challengee NOT IN (SELECT pledge_id FROM pledge))  
OR  
(challenger <> 0 AND challenger NOT IN (SELECT pledge_id FROM pledge))  
  
DELETE FROM siege_agit_pledge  
WHERE  
pledge_id <> 0 AND pledge_id NOT IN (SELECT pledge_id FROM pledge)  
  
DELETE FROM team_battle_agit_pledge  
WHERE  
pledge_id <> 0 AND pledge_id NOT IN (SELECT pledge_id FROM pledge)  
  
-- ?? ?? ??(???? ?? ??)  
DELETE FROM alliance WHERE master_pledge_id NOT IN (SELECT pledge_id FROM pledge)  
  
-- ?? ???? ??  
UPDATE pledge  
SET alliance_id = 0  
WHERE alliance_id <> 0 AND alliance_id NOT IN (SELECT id FROM alliance)  
  
-- 2005-11-21	by btwinuni
-- ?? ??? ??  
--DELETE FROM alliance_war  
--WHERE  
--status = 0  
--AND  
--(  
--(challengee <> 0 AND challengee NOT IN (SELECT id FROM alliance))  
--OR  
--(challenger <> 0 AND challenger NOT IN (SELECT id FROM alliance))  
--)
GO

/********************************************
lin_ClearGroupPoint
	
INPUT	
OUTPUT
return
made by
	young
date
	2004-07-15
********************************************/
ALTER PROCEDURE [DBO].[lin_ClearGroupPoint ]

AS
SET NOCOUNT ON


delete from event_point
GO

/********************************************
lin_CommentDelete
	
INPUT	
	@delete		int,
	@comment_id		int
OUTPUT
return
made by
	young
date
	2003-09-02
********************************************/
ALTER PROCEDURE [DBO].[lin_CommentDelete]
(
	@delete		int,
	@comment_id		int
)
AS
SET NOCOUNT ON

if @delete = 1
	update user_comment set deleted = 1 where comment_id = @comment_id
else if @delete = 2
	update user_comment set deleted = 0 where comment_id = @comment_id
GO

/****** Object:  Stored Procedure dbo.lin_CommentSearch    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_CommentSearch
	
INPUT	
	@char_id		int,
	@page			int
OUTPUT
return
made by
	young
date
	2003-09-02
********************************************/
ALTER PROCEDURE [DBO].[lin_CommentSearch]
(
	@char_id		int,
	@page			int
)
AS
SET NOCOUNT ON


select comment_id, char_name, char_id, comment, writer from user_comment (nolock) 
where char_id = @char_id and deleted = 0
order by comment_id desc
GO

/****** Object:  Stored Procedure dbo.lin_CommentWrite    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_CommentWrite
	
INPUT	
	@char_name		nvarchar(50),
	@char_id		int,
	@comment		nvarchar(200),
	@writer			nvarchar(50)
OUTPUT
return
made by
	young
date
	2003-09-02
********************************************/
ALTER PROCEDURE [DBO].[lin_CommentWrite]
(
	@char_name		nvarchar(50),
	@char_id		int,
	@comment		nvarchar(200),
	@writer			nvarchar(50)
)
AS
SET NOCOUNT ON

insert into user_comment ( char_name, char_id, comment, writer )
values ( @char_name, @char_id, @comment, @writer )
GO

/********************************************
lin_CopyChar
	copy character sp
INPUT
	@char_id		int
	@new_char_name	nvarchar(24)
	@account_id		int
	@account_name	nvarchar(24)
	@builder		int
OUTPUT

return
made by
	young
date
	2003-11-17
	2005-09-07	modified by btwinuni
	2005-10-28	modified by btwinuni	for CH4
********************************************/
ALTER PROCEDURE [DBO].[lin_CopyChar]
(
@char_id		int,
@new_char_name	nvarchar(24),
@account_id		int,
@account_name	nvarchar(24),
@builder		int
)
AS

SET NOCOUNT ON


declare @new_char_id	int

set @new_char_id = 0

SET @new_char_name = RTRIM(@new_char_name)


IF @new_char_name LIKE N' ' 
BEGIN
	RAISERROR ('Character name has space : name = [%s]', 16, 1, @new_char_name)
	RETURN -1
END

-- check user_prohibit 
if exists(select char_name from user_prohibit (nolock) where char_name = @new_char_name)
begin
	RAISERROR ('Character name is prohibited: name = [%s]', 16, 1, @new_char_name)
	RETURN -1	
end

declare @user_prohibit_word nvarchar(20)
select top 1 @user_prohibit_word = words from user_prohibit_word (nolock) where PATINDEX('%' + words + '%', @new_char_name) > 0   
if @user_prohibit_word is not null
begin
	RAISERROR ('Character name has prohibited word: name = [%s], word[%s]', 16, 1, @new_char_name, @user_prohibit_word)
	RETURN -1	
end

-- check reserved name
declare @reserved_name nvarchar(50)
declare @reserved_account_id int
select top 1 @reserved_name = char_name, @reserved_account_id = account_id from user_name_reserved (nolock) where used = 0 and char_name = @new_char_name
if not @reserved_name is null
begin
	if not @reserved_account_id = @account_id
	begin
		RAISERROR ('Character name is reserved by other player: name = [%s]', 16, 1, @new_char_name)
		RETURN -1
	end
end

declare	@gender	int
declare	@race	int
declare	@class	int
declare	@world	int
declare	@xloc	int
declare	@yloc	int
declare	@zloc	int
declare	@HP	float
declare	@MP	float
declare	@SP	int
declare	@Exp	int
declare	@Lev	int
declare	@align	int
declare	@PK	int
declare	@PKpardon	int
declare	@Duel		int
declare	@quest_flag	binary
declare	@max_hp	int
declare	@max_mp	int
declare	@quest_memo	char(32)
declare	@face_index	int
declare	@hair_shape_index	int
declare	@hair_color_index	int
declare	@drop_exp	int
declare	@subjob_id	int
declare	@cp		float
declare	@max_cp	float
declare	@subjob0_class	int
declare	@subjob1_class	int
declare	@subjob2_class	int
declare	@subjob3_class	int

-- insert user_data
select  	@builder = builder,
	@gender = gender,
	@race = race, 
	@class = class,
	@world = world,
	@xloc = xloc,
	@yloc = yloc,
	@zloc = zloc,
	@HP = HP,
	@MP = MP,
	@SP = SP,
	@Exp = Exp,
	@Lev = Lev,
	@align = align,
	@PK = PK,
	@PKpardon = PKpardon,
	@Duel = Duel,
	@quest_flag = quest_flag,
	@max_hp = max_hp,
	@max_mp = max_mp,
	@quest_memo = quest_memo,
	@face_index = face_index,
	@hair_shape_index = hair_shape_index,
	@hair_color_index = hair_color_index,
	@drop_exp = drop_exp,
	@subjob_id = subjob_id,
	@cp = cp,
	@max_cp = max_cp,
	@subjob0_class = subjob0_class,
	@subjob1_class = subjob1_class,
	@subjob2_class = subjob2_class,
	@subjob3_class = subjob3_class
	from user_data (nolock) 
	where char_id = @char_id

if ( @@ROWCOUNT > 0 ) 
begin
	INSERT INTO user_data 
	( char_name, account_name, account_id, pledge_id, create_date,
	builder, gender, race, class, world, xloc, yloc, zloc, HP, MP, SP, Exp, Lev, align, PK, PKpardon, Duel,
	quest_flag, max_hp, max_mp, quest_memo, face_index, hair_shape_index, hair_color_index, drop_exp, subjob_id, cp, max_cp,
	subjob0_class, subjob1_class, subjob2_class, subjob3_class)
	VALUES
	(@new_char_name, @account_name, @account_id, 0, GETDATE(),
	@builder, @gender, @race, @class, @world, @xloc, @yloc, @zloc, @HP, @MP, @SP, @Exp, @Lev, @align, @PK, @PKpardon, @Duel,
	@quest_flag, @max_hp, @max_mp, @quest_memo, @face_index, @hair_shape_index, @hair_color_index, @drop_exp, @subjob_id, @cp, @max_cp,
	@subjob0_class, @subjob1_class, @subjob2_class, @subjob3_class)

	IF (@@error = 0)
	BEGIN
		SET @new_char_id = @@IDENTITY

		insert into user_item ( char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse)
		select @new_char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse
		from user_item (nolock) where char_id = @char_id and item_type not in (
			2375, 3500, 3501, 3502, 4422, 4423, 4424, 4425,	-- pet
			4442, 4443, 4444 )				-- lotto, monster race

		insert into user_skill ( char_id,  skill_id, skill_lev, to_end_time, subjob_id )
		select @new_char_id, skill_id, skill_lev, to_end_time, subjob_id
		from user_skill (nolock) where char_id = @char_id

		insert into quest ( char_id, q1, s1, q2, s2, q3, s3, q4, s4, q5, s5, q6, s6, q7, s7, q8, s8, q9, s9, q10, s10, q11, s11, q12, s12, q13, s13, q14, s14, q15, s15, q16, s16,
		j1, j2, j3, j4, j5, j6, j7, j8, j9, j10, j11, j12, j13, j14, j15, j16, s2_1, s2_2, s2_3, s2_4, s2_5, s2_6, s2_7, s2_8, s2_9, s2_10, s2_11, s2_12, s2_13, s2_14, s2_15, s2_16 )
		select @new_char_id,  q1, s1, q2, s2, q3, s3, q4, s4, q5, s5, q6, s6, q7, s7, q8, s8, q9, s9, q10, s10, q11, s11, q12, s12, q13, s13, q14, s14, q15, s15, q16, s16,
		j1, j2, j3, j4, j5, j6, j7, j8, j9, j10, j11, j12, j13, j14, j15, j16, s2_1, s2_2, s2_3, s2_4, s2_5, s2_6, s2_7, s2_8, s2_9, s2_10, s2_11, s2_12, s2_13, s2_14, s2_15, s2_16
		from quest (nolock) where char_id = @char_id

		insert into user_subjob ( char_id, hp, mp, sp, exp, level, henna_1, henna_2, henna_3, subjob_id, create_date )
		select @new_char_id, hp, mp, sp, exp, level, 0, 0, 0, subjob_id, GETDATE()
		from user_subjob (nolock) where char_id = @char_id

	END
end

SELECT @new_char_id

if @new_char_id > 0
begin
	-- make user_history
	exec lin_InsertUserHistory @new_char_name, @new_char_id, 1, @account_name, NULL
	if not @reserved_name is null
		update user_name_reserved set used = 1 where char_name = @reserved_name
end
GO

/****** Object:  Stored Procedure dbo.lin_CreateActiveSkill    Script Date: 2003-09-20 ?? 11:51:58 ******/
--by bert for clee

ALTER PROCEDURE [DBO].[lin_CreateActiveSkill]
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
	@s20 INT, @l20 TINYINT, @d20 INT, @c20 TINYINT
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
s20, l20, d20, c20) 
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
@s20, @l20, @d20, @c20)
GO

/********************************************
lin_CreateAgitAuction
	create agit auction
INPUT
	@agit_id	int,
	@auction_desc	nvarchar(200),
	@min_price	int

OUTPUT
return
made by
	young
date
	2003-12-1
********************************************/
ALTER PROCEDURE [DBO].[lin_CreateAgitAuction]
(
@agit_id		INT,
@auction_desc		nvarchar(200),
@min_price		INT,
@auction_time		INT,
@auction_tax		INT
)
AS
SET NOCOUNT ON

declare @auction_id int

insert into agit_auction ( agit_id, auction_desc, min_price, auction_time , auction_tax)
values ( @agit_id, @auction_desc, @min_price, @auction_time  , @auction_tax )

select @auction_id = @@IDENTITY

update agit set auction_id = @auction_id where id = @agit_id

select @auction_id
GO

/********************************************
lin_CreateAgitBid
	create agit bid
INPUT
	@auction_id	int,
	@price		int,
	@pledge_id	int

OUTPUT
return
made by
	young
date
	2003-12-1
********************************************/
ALTER PROCEDURE [DBO].[lin_CreateAgitBid]
(
@agit_id		INT,
@auction_id		INT,
@price			INT,
@pledge_id		INT,
@attend_time		INT
)
AS
SET NOCOUNT ON

declare @last_bid int
declare @bid_id int
declare @diff_adena int

set @last_bid = 0
set @bid_id = 0
set @diff_adena = 0

select @last_bid  = attend_price , @bid_id = attend_id from agit_bid (nolock) where auction_id = @auction_id and attend_pledge_id = @pledge_id

if ( @@ROWCOUNT > 0 ) 
begin

	update agit_bid set attend_price = @price  , attend_date = getdate() where auction_id = @auction_id and attend_pledge_id = @pledge_id
	set @diff_adena = @last_bid - @price 

end else begin

	insert into agit_bid ( auction_id, attend_price, attend_pledge_id , attend_time )
	values (  @auction_id, @price, @pledge_id , @attend_time )

	set @bid_id = @@IDENTITY
	set @diff_adena = @price
end

select @auction_id, @bid_id, @diff_adena
GO

/****** Object:  Stored Procedure dbo.lin_CreateAgitIfNotExist    Script Date: 2003-09-20 ?? 11:51:58 ******/
ALTER PROCEDURE [DBO].[lin_CreateAgitIfNotExist]
(
	@agit_id	INT,
	@agit_name VARCHAR(50)
)
AS

SET NOCOUNT ON

INSERT INTO agit
(id, name)
VALUES (@agit_id, @agit_name)
GO

ALTER PROCEDURE
dbo.lin_CreateAlliance (@name NVARCHAR(50), @master_pledge_id INT)
AS

SET NOCOUNT ON

DECLARE @alliance_id INT

BEGIN TRAN

IF @name LIKE N' '   
BEGIN  
 RAISERROR ('alliance name has space : name = [%s]', 16, 1, @name)  
 GOTO EXIT_TRAN
END  
  
-- check user_prohibit   
IF EXISTS(SELECT char_name FROM user_prohibit (nolock) WHERE char_name = @name)  
BEGIN
 RAISERROR ('alliance name is prohibited: name = [%s]', 16, 1, @name)  
 GOTO EXIT_TRAN
END
  
DECLARE @user_prohibit_word NVARCHAR(20)  
SELECT TOP 1 @user_prohibit_word = words FROM user_prohibit_word (nolock) WHERE PATINDEX('%' + words + '%', @name) > 0   
IF @user_prohibit_word IS NOT NULL
BEGIN
 RAISERROR ('alliance name has prohibited word: name = [%s], word[%s]', 16, 1, @name, @user_prohibit_word)  
 GOTO EXIT_TRAN
END

INSERT INTO alliance (name, master_pledge_id) VALUES (@name, @master_pledge_id)

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @alliance_id = @@IDENTITY
	UPDATE pledge
	SET alliance_id = @alliance_id
	WHERE pledge_id = @master_pledge_id
END
ELSE
BEGIN
	SELECT @alliance_id = 0
	GOTO EXIT_TRAN
END

IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
BEGIN
	SELECT @alliance_id = 0
END

EXIT_TRAN:
IF @alliance_id <> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END

SELECT @alliance_id
GO

/****** Object:  Stored Procedure dbo.lin_CreateCastle    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_CreateCastle
	
INPUT	
	@id	int,
	@name	nvarchar(50)
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_CreateCastle]
(
	@id	int,
	@name	nvarchar(50)
)
AS
SET NOCOUNT ON

INSERT INTO castle (id, name) VALUES (@id, @name)
GO

ALTER PROCEDURE dbo.lin_CreateChar
(  
@char_name NVARCHAR(24),  
@account_name NVARCHAR(24),  
@account_id INT,  
@pledge_id INT,  
@builder  TINYINT,  
@gender TINYINT,  
@race  TINYINT,  
@class  TINYINT,  
@world  SMALLINT,  
@xloc  INT,  
@yloc  INT,  
@zloc  INT,  
@HP  FLOAT,  
@MP  FLOAT,  
@SP  INT,  
@Exp  INT,  
@Lev  TINYINT,  
@align  SMALLINT,  
@PK  INT,  
@Duel  INT,  
@PKPardon  INT,  
@FaceIndex   INT = 0,  
@HairShapeIndex  INT = 0,  
@HairColorIndex  INT = 0  
)  
AS  
  
SET NOCOUNT ON  
  
SET @char_name = RTRIM(@char_name)  
DECLARE @char_id int  
SET @char_id = 0  
  

IF @char_name LIKE N' '   
BEGIN  
 RAISERROR ('Character name has space : name = [%s]', 16, 1, @char_name)  
 RETURN -1  
END  
  
-- check user_prohibit   
if exists(select char_name from user_prohibit (nolock) where char_name = @char_name)  
begin  
 RAISERROR ('Character name is prohibited: name = [%s]', 16, 1, @char_name)  
 RETURN -1   
end  
  
declare @user_prohibit_word nvarchar(20)  
select top 1 @user_prohibit_word = words from user_prohibit_word (nolock) where @char_name like '%' + words + '%'
if @user_prohibit_word is not null  
begin  
 RAISERROR ('Character name has prohibited word: name = [%s], word[%s]', 16, 1, @char_name, @user_prohibit_word)  
 RETURN -1   
end  
  
-- check reserved name  
declare @reserved_name nvarchar(50)  
declare @reserved_account_id int  
select top 1 @reserved_name = char_name, @reserved_account_id = account_id from user_name_reserved (nolock) where used = 0 and char_name = @char_name  
if not @reserved_name is null  
begin  
 if not @reserved_account_id = @account_id  
 begin  
  RAISERROR ('Character name is reserved by other player: name = [%s]', 16, 1, @char_name)  
  RETURN -1  
 end  
end  
  
IF @race>5
BEGIN  
 RAISERROR ('Race overflow : = [%s]', 16, 1, @char_name)  
 RETURN -1  
END  

IF @race=0 and @class!=0 and @class!=10
BEGIN  
 RAISERROR ('Class Overflow for Human: = [%s]', 16, 1, @class)  
 RETURN -1  
END  

IF @race=1 and @class!=18 and @class!=25
BEGIN  
 RAISERROR ('Class Overflow for Elf: = [%s]', 16, 1, @class)  
 RETURN -1  
END  

IF @race=2 and @class!=31 and @class!=38
BEGIN  
 RAISERROR ('Class Overflow for DE: = [%s]', 16, 1, @class)  
 RETURN -1  
END  

IF @race=3 and @class!=44 and @class!=49
BEGIN  
 RAISERROR ('Class Overflow for Orc: = [%s]', 16, 1, @class)  
 RETURN -1  
END  

IF @race=4 and @class!=53
BEGIN  
 RAISERROR ('Class Overflow for Dwarf: = [%s]', 16, 1, @class)  
 RETURN -1  
END  

IF @race=5 and @class!=123 and @class!=124
 BEGIN
 RAISERROR ('Class Overflow for Kamael: = [%s]', 16, 1, @class)
 RETURN -1
END

-- insert user_data  
INSERT INTO user_data   
( char_name, account_name, account_id, pledge_id, builder, gender, race, class, subjob0_class, 
world, xloc, yloc, zloc, HP, MP, max_hp, max_mp, SP, Exp, Lev, align, PK, PKpardon, duel, create_date, face_index, hair_shape_index, hair_color_index )  
VALUES  
(@char_name, @account_name, @account_id, @pledge_id, @builder, @gender, @race, @class, @class, 
@world, @xloc, @yloc, @zloc, @HP, @MP, @HP, @MP, @SP, @Exp, @Lev, @align, @PK, @Duel, @PKPardon, GETDATE(), @FaceIndex, @HairShapeIndex, @HairColorIndex)  
  
IF (@@error = 0)  
BEGIN  
 SET @char_id = @@IDENTITY  
 INSERT INTO quest (char_id) VALUES (@char_id)  
END  
  
SELECT @char_id  
  
if @char_id > 0  
begin  
 -- make user_history  
 exec lin_InsertUserHistory @char_name, @char_id, 1, @account_name, NULL  
 if not @reserved_name is null  
  update user_name_reserved set used = 1 where char_name = @reserved_name  
end
GO

/******************************************************************************
#Name:	lin_CreateCharWithSubjob
#Desc:	create a character with subjob

#Argument:
	Input:	@char_name	NVARCHAR(24)
		@account_name	NVARCHAR(24)
		@account_id	INT
		@pledge_id	INT
		@builder		TINYINT
		@gender	TINYINT
		@race		TINYINT
		@class		TINYINT
		@world		SMALLINT
		@xloc		INT
		@yloc		INT
		@zloc		INT
		@HP		FLOAT
		@MP		FLOAT
		@SP		INT
		@Exp		INT
		@Lev		TINYINT
		@align		SMALLINT
		@PK		INT
		@Duel		INT
		@PKPardon	INT
		@FaceIndex	INT = 0
		@HairShapeIndex		INT = 0
		@HairColorIndex		INT = 0
		@SubjobID	INT = 0
		@MainClass	INT = 0
		@CP 		FLOAT = 0
	Output:	@char_id	INT
#Return:
#Result Set:

#Remark:
#Example:
#See:
	lin_CreateChar

#History:
	Create	btwinuni	2005-07-28
	Modify	btwinuni	2005-09-07	main_class -> subjob0_class
	Modify	btwinuni	2005-11-17
******************************************************************************/
ALTER PROCEDURE [DBO].[lin_CreateCharWithSubjob]  
(  
@char_name NVARCHAR(24),  
@account_name NVARCHAR(24),  
@account_id INT,  
@pledge_id INT,  
@builder  TINYINT,  
@gender TINYINT,  
@race  TINYINT,  
@class  TINYINT,  
@world  SMALLINT,  
@xloc  INT,  
@yloc  INT,  
@zloc  INT,  
@HP  FLOAT,  
@MP  FLOAT,  
@SP  INT,  
@Exp  INT,  
@Lev  TINYINT,  
@align  SMALLINT,  
@PK  INT,  
@Duel  INT,  
@PKPardon  INT,  
@FaceIndex   INT = 0,  
@HairShapeIndex  INT = 0,  
@HairColorIndex  INT = 0,
@SubjobID INT = -1,
@MainClass INT = -1,
@CP FLOAT = 0
)  
AS  
  
SET NOCOUNT ON  
  
SET @char_name = RTRIM(@char_name)  
DECLARE @char_id int  
SET @char_id = 0  

if @MainClass = -1
begin
	set @MainClass = null
end
  
-- debug code delete in commercial  
IF (@@servername = 'DL380G3NEW')  
BEGIN
 SET @builder = 1  
END
--ELSE
--BEGIN
--	if NOT exists(select * from account_ch2 (nolock) where account = @account_name)  
--	--BEGIN  
--		 RAISERROR ('Character name has space : name = [%s]', 16, 1, @account_name)  
--		 RETURN -1  
--	END  
--END

IF @char_name LIKE N' '   
BEGIN  
 RAISERROR ('Character name has space : name = [%s]', 16, 1, @char_name)  
 RETURN -1  
END  
  
-- check user_prohibit   
if exists(select char_name from user_prohibit (nolock) where char_name = @char_name)  
begin  
 RAISERROR ('Character name is prohibited: name = [%s]', 16, 1, @char_name)  
 RETURN -1   
end  
  
declare @user_prohibit_word nvarchar(20)  
select top 1 @user_prohibit_word = words from user_prohibit_word (nolock) where @char_name like '%' + words + '%'
if @user_prohibit_word is not null  
begin  
 RAISERROR ('Character name has prohibited word: name = [%s], word[%s]', 16, 1, @char_name, @user_prohibit_word)  
 RETURN -1   
end  
  
-- check reserved name  
declare @reserved_name nvarchar(50)  
declare @reserved_account_id int  
select top 1 @reserved_name = char_name, @reserved_account_id = account_id from user_name_reserved (nolock) where used = 0 and char_name = @char_name  
if not @reserved_name is null  
begin  
 if not @reserved_account_id = @account_id  
 begin  
  RAISERROR ('Character name is reserved by other player: name = [%s]', 16, 1, @char_name)  
  RETURN -1  
 end  
end  
  
-- insert user_data  
INSERT INTO user_data   
( char_name, account_name, account_id, pledge_id, builder, gender, race, class,   
world, xloc, yloc, zloc, HP, MP, max_hp, max_mp, SP, Exp, Lev, align, PK, PKpardon, duel, create_date, face_index, hair_shape_index, hair_color_index,
subjob_id, subjob0_class, cp, max_cp
 )  
VALUES  
(@char_name, @account_name, @account_id, @pledge_id, @builder, @gender, @race, @class,   
@world, @xloc, @yloc, @zloc, @HP, @MP, @HP, @MP, @SP, @Exp, @Lev, @align, @PK, @Duel, @PKPardon, GETDATE(), @FaceIndex, @HairShapeIndex, @HairColorIndex,
@SubjobID, @MainClass, @CP, @CP)  
  
IF (@@error = 0)  
BEGIN  
 SET @char_id = @@IDENTITY  
 INSERT INTO quest (char_id) VALUES (@char_id)  
END  
  
SELECT @char_id  
  
if @char_id > 0  
begin  
 -- make user_history  
 exec lin_InsertUserHistory @char_name, @char_id, 1, @account_name, NULL  
 if not @reserved_name is null  
  update user_name_reserved set used = 1 where char_name = @reserved_name  
end
GO

ALTER PROCEDURE [dbo].[lin_CreateDoorDataIfNotExist]  
(  
 @name  NVARCHAR(50),  
 @default_hp INT  
)  
AS  
SET NOCOUNT ON  
INSERT INTO door (name, hp, max_hp) VALUES (@name, @default_hp, @default_hp)
GO

/********************************************
lin_CreateGroupPoint

INPUT
OUTPUT

return
made by
	young
date
	2004-07-15
********************************************/
ALTER PROCEDURE [DBO].[lin_CreateGroupPoint]
(
	@group_id	int,
	@group_point	float
)
AS
SET NOCOUNT ON

insert into event_point ( group_id, group_point) values ( @group_id, @group_point)
GO

/****** Object:  Stored Procedure dbo.lin_CreateItem    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_CreateItem
	create item sp
INPUT
	@char_id	INT,
	@item_type 	INT,
	@amount 	INT,
	@enchant 	INT,
	@eroded 	INT,
	@bless 		TINYINT,
	@ident 		TINYINT,
	@ready 		TINYINT,
	@wished 	TINYINT,
	@warehouse	INT
OUTPUT
	Item_ID, @@IDENTITY
return
made by
	carrot
date
	2002-01-31
********************************************/
ALTER PROCEDURE [DBO].[lin_CreateItem]
(
@char_id	INT,
@item_type 	INT,
@amount 	INT,
@enchant 	INT,
@eroded 	INT,
@bless 		TINYINT,
@ident 		TINYINT,
@wished 	TINYINT,
@warehouse	INT
)
AS
SET NOCOUNT ON

insert into user_item 
	(char_id , item_type , amount , enchant , eroded , bless , ident , wished , warehouse) 
	values 
	(@char_id, @item_type , @amount , @enchant , @eroded , @bless , @ident , @wished , @warehouse)

SELECT @@IDENTITY
GO

ALTER PROCEDURE [DBO].[lin_CreateLottoGame]
(
	@round_number int,
	@state int,
	@left_time int,
	@chosen_nuimber_flag int,
	@rule_number int,
	@total_count int,
	@winner1_count int,
	@winner2_count int,
	@winner3_count int,
	@winner4_count int,
	@carried_adena int
) 
AS       
SET NOCOUNT ON    

insert into lotto_game
values
(
	@round_number ,
	@state,
	@left_time,
	@chosen_nuimber_flag ,
	@rule_number ,
	@total_count ,
	@winner1_count ,
	@winner2_count ,
	@winner3_count ,
	@winner4_count ,
	@carried_adena )
GO

ALTER PROCEDURE [DBO].[lin_CreateLottoItem]
(
	@round_number int,
	@item_id int,
	@number_flag int	
) 
AS       
SET NOCOUNT ON    

insert into lotto_items
values
(
	@round_number ,
	@item_id,
	@number_flag
)
GO

/********************************************
lin_CreateMacro
	create macro
INPUT
	@char_id		int,
	@macro_name		nvarchar(64),
	@macro_tooltip		nvarchar(64)
	@macro_iconname	nvarchar(64)
	@macro_icontype		int

OUTPUT
return
made by
	young
date
	2004-6-11
********************************************/
ALTER PROCEDURE [DBO].[lin_CreateMacro]
(
@char_id		int,
@macro_name		nvarchar(64),
@macro_tooltip		nvarchar(64),
@macro_iconname	nvarchar(64),
@macro_icontype	int
)
AS
SET NOCOUNT ON

insert into user_macro ( char_id, macro_name, macro_tooltip, macro_iconname, macro_icontype)
values
( @char_id, @macro_name, @macro_tooltip, @macro_iconname, @macro_icontype)

select @@IDENTITY
GO

/********************************************
lin_CreateMacroInfo
	create macroinfo
INPUT
	@macro_id		int,
	@macro_order		int,
	@macro_int1		int,
	@macro_int2		int,
	@macro_int3		int,
	@macro_str		nvarchar(255)

OUTPUT
return
made by
	young
date
	2004-6-11
********************************************/
ALTER PROCEDURE [DBO].[lin_CreateMacroInfo]
(
@macro_id		int,
@macro_order		int,
@macro_int1		int,
@macro_int2		int,
@macro_int3		int,
@macro_str		nvarchar(255)
)
AS
SET NOCOUNT ON

if ( exists ( select * from user_macro where macro_id = @macro_id ) )
begin
	if ( exists ( select * from user_macroinfo where macro_id = @macro_id and macro_order = @macro_order ) )
	begin
		update user_macroinfo set macro_int1 = @macro_int1, macro_int2 = @macro_int2, macro_int3 = @macro_int3 where macro_id = @macro_id and macro_order = @macro_order

	end else begin
		insert into user_macroinfo ( macro_id, macro_order, macro_int1, macro_int2, macro_int3, macro_str )
		values ( @macro_id, @macro_order, @macro_int1, @macro_int2, @macro_int3, @macro_str )
	end

end
GO

/********************************************
lin_CreateMonRace
	create monster race
INPUT
	@mon1		smallint,
	@mon2		smallint,
	@mon3		smallint,
	@mon4		smallint,
	@mon5		smallint,
	@mon6		smallint,
	@mon7		smallint,
	@mon8		smallint

OUTPUT
return
made by
	young
date
	2004-5-18
********************************************/
ALTER PROCEDURE [DBO].[lin_CreateMonRace]
(
@mon1			SMALLINT,
@mon2			SMALLINT,
@mon3			SMALLINT,
@mon4			SMALLINT,
@mon5			SMALLINT,
@mon6			SMALLINT,
@mon7			SMALLINT,
@mon8			SMALLINT,
@tax_rate		int
)
AS
SET NOCOUNT ON

declare @race_id int

insert into monrace ( lane1, lane2, lane3, lane4, lane5, lane6, lane7, lane8 , tax_rate)
values ( @mon1, @mon2, @mon3, @mon4, @mon5,@mon6, @mon7,  @mon8 , @tax_rate )

select @race_id  = @@IDENTITY

select @race_id
GO

/********************************************
lin_CreateMonRaceMon
	create monster race
INPUT
	@runner_id	int
	@initial_win	int
OUTPUT
return
made by
	young
date
	2004-5-19
********************************************/
ALTER PROCEDURE [DBO].[lin_CreateMonRaceMon]
(
@runner_id		INT,
@initial_win		INT,
@run_count		INT,
@win_count		INT
)
AS
SET NOCOUNT ON

if not exists ( select * from monrace_mon (nolock) where runner_id = @runner_id ) 
begin
	insert into monrace_mon( runner_id, initial_win , run_count, win_count ) values ( @runner_id, @initial_win , @run_count, @win_count)
end 

select initial_win, run_count, win_count  from monrace_mon ( nolock) where runner_id = @runner_id
GO

/********************************************
lin_CreateMonRaceTicket
	ALTER  monster race ticket
INPUT
	@monraceid	int,
	@bet_type	smallint,
	@bet_1		smallint,
	@bet_2		smallint,
	@bet_3		smallint,
	@bet_money	int,
	@tax_money	int,
	@item_id	int,
	@remotefee	int=0

OUTPUT
return
made by
	young
date
	2004-5-18
********************************************/

ALTER PROCEDURE [DBO].[lin_CreateMonRaceTicket]
(
	@monraceid	int,
	@bet_type	smallint,
	@bet_1		smallint,
	@bet_2		smallint,
	@bet_3		smallint,
	@bet_money	int,
	@tax_money	int,
	@item_id	int,
	@remotefee	int=0
)
AS
SET NOCOUNT ON

declare @ticket_id int

insert into monrace_ticket ( monraceid, bet_type, bet_1, bet_2, bet_3, bet_money,  tax_money, item_id , remotefee )
values ( @monraceid, @bet_type, @bet_1, @bet_2, @bet_3,  @bet_money,  @tax_money, @item_id , @remotefee )

select @ticket_id  = @@IDENTITY

select @ticket_id
GO

/********************************************
lin_CreateNewbieData
	create  newbie data
INPUT
	@account_id	INT,

OUTPUT
return
made by
	kks
date
	2004-11-25

********************************************/
ALTER PROCEDURE [DBO].[lin_CreateNewbieData]
(
	@account_id	INT
)
AS
SET NOCOUNT ON

INSERT INTO user_newbie(account_id, char_id, newbie_stat)
VALUES (@account_id, 0, 0)
GO

/********************************************
lin_CreatePet
	create item sp
INPUT
	@pet_id	INT,  // same as pet_collar_dbid
	@npc_class_id	INT
OUTPUT
return
made by
	kuooo
date
	2002-08-19
********************************************/
ALTER PROCEDURE [DBO].[lin_CreatePet]
(
@pet_dbid		INT,
@npc_class_id		INT,
@exp_in		INT,
@hp			float,
@mp			float,
@meal			int
)
AS
SET NOCOUNT ON

insert into pet_data
	(pet_id, npc_class_id,  expoint, hp, mp, meal)
	values (@pet_dbid, @npc_class_id, @exp_in, @hp, @mp, @meal)

/*SELECT @@IDENTITY*/
GO

-- lin_CreatePledge
-- by bert
-- return pledge_id, ruler_name
ALTER PROCEDURE
dbo.lin_CreatePledge (@name NVARCHAR(50), @ruler_id INT)
AS

SET NOCOUNT ON

DECLARE @pledge_id INT


BEGIN TRAN

IF @name LIKE N' '   
BEGIN  
 RAISERROR ('pledge name has space : name = [%s]', 16, 1, @name)  
 GOTO EXIT_TRAN
END  
  
-- check user_prohibit   
IF EXISTS(SELECT char_name FROM user_prohibit (nolock) WHERE char_name = @name)  
BEGIN
 RAISERROR ('pledge name is prohibited: name = [%s]', 16, 1, @name)  
 GOTO EXIT_TRAN
END
  
DECLARE @user_prohibit_word NVARCHAR(20)  
SELECT TOP 1 @user_prohibit_word = words FROM user_prohibit_word (nolock) WHERE PATINDEX('%' + words + '%', @name) > 0   
IF @user_prohibit_word IS NOT NULL
BEGIN
 RAISERROR ('pledge name has prohibited word: name = [%s], word[%s]', 16, 1, @name, @user_prohibit_word)  
 GOTO EXIT_TRAN
END

INSERT INTO Pledge (name, ruler_id) VALUES (@name, @ruler_id)

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @pledge_id = @@IDENTITY
	UPDATE user_data
	SET pledge_id = @pledge_id
	WHERE char_id = @ruler_id
END
ELSE
BEGIN
	SELECT @pledge_id = 0
	GOTO EXIT_TRAN
END

IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
BEGIN
	SELECT @pledge_id = 0
END

EXIT_TRAN:
IF @pledge_id <> 0
BEGIN
	COMMIT TRAN
	SELECT @pledge_id AS pledge_id, (SELECT char_name FROM user_data WHERE char_id = @ruler_id) AS pledge_ruler_name
END
ELSE
BEGIN
	ROLLBACK TRAN
	SELECT 0 AS pledge_id, N'' AS pledge_ruler_name
END
GO

/**  
  * @procedure lin_CreateSSQRound  
  * @brief SSQ ?? ??  
  *  
  * @date 2004/11/18  
  * @author sonai  <sonai@ncsoft.net>  
  */  
ALTER PROCEDURE [DBO].[lin_CreateSSQRound]   
(  
@round_number INT,  
@status INT,  
@winner INT,  
@event_start_time INT,  
@event_end_time INT,  
@seal_effect_time INT,  
@seal_effect_end_time INT,  
@seal1 INT,  
@seal2 INT,  
@seal3 INT,  
@seal4 INT,  
@seal5 INT,  
@seal6 INT,  
@seal7 INT,  
@castle_snapshot_time INT,  
@can_drop_guard INT  
)  
AS  
  
SET NOCOUNT ON  
/*DECLARE @ret AS INT*/  
  
/* ?? ?? ?? ?? */  
INSERT ssq_join_data(round_number, point,  collected_point, main_event_point, type, member_count,   
                         seal1_selection_count, seal2_selection_count, seal3_selection_count,  
             seal4_selection_count, seal5_selection_count, seal6_selection_count, seal7_selection_count,  
             last_changed_time)  
  VALUES(@round_number, 0, 0, 0, 1, 0,   0, 0, 0, 0, 0, 0, 0, GETDATE())  
  
/*SELECT @ret = @@ROWCOUNT*/  
  
/* ?? ?? ?? ?? */  
INSERT ssq_join_data(round_number, point, collected_point, main_event_point, type, member_count,   
             seal1_selection_count, seal2_selection_count, seal3_selection_count,  
             seal4_selection_count, seal5_selection_count, seal6_selection_count, seal7_selection_count,  
             last_changed_time)  
  VALUES(@round_number,  0, 0, 0, 2, 0,   0, 0, 0, 0, 0, 0, 0, GETDATE())  
  
/*SELECT @ret = @ret + @@ROWCOUNT */  
  
INSERT ssq_data(round_number, status, winner, event_start_time, event_end_time, seal_effect_time, seal_effect_end_time,  
       seal1, seal2, seal3, seal4, seal5, seal6, seal7,  
       last_changed_time, castle_snapshot_time, can_drop_guard)  
   VALUES(@round_number, @status, @winner,  @event_start_time,  @event_end_time,  @seal_effect_time, @seal_effect_end_time,  
      @seal1, @seal2, @seal3, @seal4, @seal5, @seal6, @seal7,  
       GETDATE(), @castle_snapshot_time, @can_drop_guard)  
  
/*  
SELECT @ret = @ret + @@ROWCOUNT  
  
SELECT @ret  
*/
GO

/**
  * @procedure lin_CreateSSQTopPointUser
  * @brief ?? ?? ?? ??.
  *
  * @date 2004/12/09
  * @author Seongeun Park  <sonai@ncsoft.net>
  */
ALTER PROCEDURE [DBO].[lin_CreateSSQTopPointUser] 
(
@ssq_round INT,
@record_id INT,

@ssq_point INT,
@rank_time INT,
@char_id  INT,
@char_name NVARCHAR(50),
@ssq_part TINYINT,
@ssq_position TINYINT,
@seal_selection_no TINYINT
)
AS

SET NOCOUNT ON

INSERT INTO ssq_top_point_user  
	         (ssq_round, record_id, ssq_point, rank_time, char_id, char_name, 
	          ssq_part, ssq_position, seal_selection_no, last_changed_time)
	VALUES (@ssq_round, @record_id, @ssq_point, @rank_time, @char_id, @char_name, @ssq_part, @ssq_position, @seal_selection_no, GETDATE())
GO

/**
 * @fn lin_CreateSSQUserInfo
 * @brief  SSQ?? ?? ??? ??.
 */
ALTER PROCEDURE [DBO].[lin_CreateSSQUserInfo]
(
@char_id INT,
@round_number INT,
@ssq_join_time INT,
@ssq_part INT,
@ssq_position INT,
@seal_selection_no INT,
@ssq_point INT,
@twilight_a_item_num INT,
@twilight_b_item_num INT,
@twilight_c_item_num INT,
@dawn_a_item_num INT,
@dawn_b_item_num INT,
@dawn_c_item_num INT,
@ticket_buy_count INT
)
AS
SET NOCOUNT ON

INSERT INTO ssq_user_data  
	(char_id, round_number, ssq_join_time, ssq_part, ssq_position, seal_selection_no, ssq_point,
              twilight_a_item_num, twilight_b_item_num, twilight_c_item_num, dawn_a_item_num, dawn_b_item_num, dawn_c_item_num,
              ticket_buy_count) 
	values 
	(@char_id, @round_number, @ssq_join_time, @ssq_part, @ssq_position, @seal_selection_no, @ssq_point,
              @twilight_a_item_num, @twilight_b_item_num, @twilight_c_item_num, @dawn_a_item_num, @dawn_b_item_num, @dawn_c_item_num,
              @ticket_buy_count)
GO

ALTER PROCEDURE dbo.lin_CreateSubJob
(
	@char_id	INT,
	@new_subjob_id TINYINT,
	@new_class	TINYINT,
	@old_subjob_id	TINYINT,
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

SET NOCOUNT ON

DECLARE @ret INT
SELECT @ret = 0
-- transaction on
BEGIN TRAN

IF @new_subjob_id = 1
	UPDATE user_data SET subjob1_class = @new_class WHERE char_id = @char_id
ELSE IF @new_subjob_id = 2
	UPDATE user_data SET subjob1_class = @new_class WHERE char_id = @char_id
ELSE IF @new_subjob_id = 3
	UPDATE user_data SET subjob1_class = @new_class WHERE char_id = @char_id

IF @@ERROR <> 0 OR @@ROWCOUNT <> 1	-- update, insert check
BEGIN
	SELECT @ret = 0
	GOTO EXIT_TRAN
END

-- save old class

IF @old_subjob_id = 0 AND (NOT EXISTS(SELECT char_id FROM user_subjob WHERE char_id = @char_id AND subjob_id = 0))
BEGIN
	DECLARE @original_date DATETIME
	SELECT @original_date = create_date FROM user_data(NOLOCK) WHERE char_id = @char_id
	INSERT INTO user_subjob
	(char_id, subjob_id, hp, mp, sp, exp, level, henna_1, henna_2, henna_3, create_date)
	VALUES
	(@char_id, 0, @hp, @mp, @sp, @exp, @level, @henna_1, @henna_2, @henna_3, @original_date)
END

UPDATE user_subjob
SET hp = @hp, mp = @mp, sp = @sp, exp = @exp, level = @level, 
	henna_1 = @henna_1, henna_2 = @henna_2, henna_3 = @henna_3
WHERE char_id = @char_id AND subjob_id = @old_subjob_id

IF @@ERROR <> 0 OR @@ROWCOUNT <> 1	-- update, insert check
BEGIN
	SELECT @ret = 0
	GOTO EXIT_TRAN
END

-- create new class
INSERT INTO user_subjob (char_id, subjob_id, create_date) VALUES (@char_id, @new_subjob_id, GETDATE())
IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @ret = @new_subjob_id
END
ELSE
BEGIN
	SELECT @ret = 0
END

EXIT_TRAN:
IF @ret<> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END

SELECT @ret
GO

/**
 * @fn lin_CreateTimeAttackRecord
 * @brief ?? ?? ??? ?? ???? ?? ??
 */
ALTER PROCEDURE [DBO].[lin_CreateTimeAttackRecord]
(
@room_no TINYINT,
@record_type TINYINT,
@ssq_round INT,
@ssq_part TINYINT,
@point       INT,
@record_time INT,
@elapsed_time INT,
@member_count INT,
@member_names NVARCHAR(256),
@member_dbid_list NVARCHAR(128),
@member_reward_flags INT,
@fee INT

)
AS
SET NOCOUNT ON

INSERT INTO time_attack_record  
	(room_no, record_type, ssq_round, ssq_part, point, record_time, elapsed_time, member_count, member_names,
              member_dbid_list, member_reward_flags, fee) 
	values 
	(@room_no, @record_type, @ssq_round, @ssq_part, @point, @record_time, @elapsed_time, @member_count, @member_names,
              @member_dbid_list, @member_reward_flags, @fee)
GO

ALTER PROCEDURE dbo.lin_DeclareWar
(
@challenger INT,
@challengee INT,
@declare_time INT
)
AS
SET NOCOUNT ON

INSERT INTO war_declare (challenger, challengee, declare_time) VALUES (@challenger, @challengee, @declare_time)
GO

ALTER PROCEDURE dbo.lin_DelAquireSkill
(
	@char_id	INT,
	@subjob_id	INT,
	@skill_id		INT
)
AS
SET NOCOUNT ON

DELETE FROM user_skill WHERE char_id = @char_id AND skill_id = @skill_id AND ISNULL(subjob_id, 0) = @subjob_id
GO

/****** Object:  Stored Procedure dbo.lin_DelBookMark    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_DelBookMark
	add bookmark item
INPUT
	@char_id	int,
	@bookmark_name	nvarchar(50)
OUTPUT

return
made by
	young
date
	2002-11-13
********************************************/
ALTER PROCEDURE [DBO].[lin_DelBookMark]
(
	@char_id	int,
	@bookmark_name	nvarchar(50)
)
AS
SET NOCOUNT ON

delete from bookmark where char_id = @char_id and name = @bookmark_name
GO

/********************************************
lin_DeleteAgitBid
	delete agit bid
INPUT
	@agit_id	int
	@pledge_id	int

OUTPUT
return
made by
	young
date
	2003-12-1
********************************************/
ALTER PROCEDURE [DBO].[lin_DeleteAgitBid]
(
@agit_id		INT,
@pledge_id		INT
)
AS
SET NOCOUNT ON

declare @auction_id	int
declare @price int

set @auction_id = 0
set @price = 0

select @auction_id = isnull( auction_id , 0) from agit (nolock) where id = @agit_id

if @auction_id > 0
begin
	select @price = attend_price from agit_bid where  auction_id = @auction_id and attend_pledge_id = @pledge_id
	delete from agit_bid where auction_id = @auction_id and attend_pledge_id = @pledge_id
end 

select @agit_id, @pledge_id, @auction_id, @price
GO

ALTER PROCEDURE [dbo].lin_DeleteAllResidenceGuard  
(  
@res INT  
)  
AS  
IF EXISTS(SELECT * FROM residence_guard WHERE residence_id = @res)
	DELETE FROM residence_guard WHERE residence_id = @res
GO

/****** Object:  Stored Procedure dbo.lin_DeleteBBSBoard    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_DeleteBBSBoard
	Delete BBS board
INPUT
	@board_name	nvarchar(20)
OUTPUT

return
made by
	young
date
	2002-10-16
********************************************/
ALTER PROCEDURE [DBO].[lin_DeleteBBSBoard]
(
	@board_name	nvarchar(20)
)
AS

set nocount on

declare @ncount int
declare @table_name nvarchar(20)
declare @exec nvarchar(500)

set @table_name = 'bbs_' + @board_name

set @exec = 'drop table ' + @table_name
exec (@exec)

delete from bbs_board where board_name =  @table_name
GO

/********************************************    
lin_DeleteBlockList    
 delete character's blocked list.    
INPUT    
 char_id,    
 target char_name    
OUTPUT    
return    
made by    
 carrot    
date    
 2003-12-01    
change    
********************************************/    
ALTER PROCEDURE [DBO].[lin_DeleteBlockList]    
(    
 @char_id INT,    
 @target_char_name NVARCHAR(50)    
)    
AS    
    
SET NOCOUNT ON    
    
DECLARE @target_char_id INT    
SET @target_char_id = 0    
  
SELECT @target_char_id = block_char_id FROM user_blocklist  WHERE block_char_name = @target_char_name    
  
--SELECT @target_char_id = char_id FROM user_data WHERE char_name = @target_char_name    
    
IF @target_char_id > 0    
BEGIN    
 DELETE user_blocklist  WHERE char_id = @char_id AND block_char_id = @target_char_id    
 IF NOT @@ROWCOUNT = 1    
 BEGIN    
  RAISERROR ('Cannot find delete blocklist: char id = [%d], target name[%s]', 16, 1, @char_id, @target_char_name)    
 END    
 ELSE    
 BEGIN    
  SELECT @target_char_id    
 END    
END    
ELSE    
BEGIN    
 RAISERROR ('Cannot find delete blocklist: char id = [%d], target naem[%s]', 16, 1, @char_id, @target_char_name)    
END
GO

/****** Object:  Stored Procedure dbo.lin_DeleteCastleIncome    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_DeleteCastleIncome
	
INPUT	
	@castle_id	int,
	@item_type	int
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_DeleteCastleIncome]
(
	@castle_id	int,
	@item_type	int
)
AS
SET NOCOUNT ON

delete castle_crop where castle_id = @castle_id and item_type = @item_type
GO

/****** Object:  Stored Procedure dbo.lin_DeleteCastleWar    Script Date: 2003-09-20 ?? 11:52:00 ******/
-- lin_DeleteCastleWar
-- by kks

ALTER PROCEDURE
dbo.lin_DeleteCastleWar (@pledge_id INT,  @castle_id INT)
AS
SET NOCOUNT ON

Delete from castle_war where pledge_id = @pledge_id and castle_id = @castle_id

SELECT @@ROWCOUNT
GO

/****** Object:  Stored Procedure dbo.lin_DeleteChar    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_DeleteChar
	Delete character sp
INPUT
	@char_id	INT
OUTPUT
	item_id
return
made by
	carrot
date
	2002-02-16
	Update all item and return item_ids
********************************************/
ALTER PROCEDURE [DBO].[lin_DeleteChar]
(
@char_id	INT
)
AS

SET NOCOUNT ON

DECLARE @backup_char_name NVARCHAR(50)
DECLARE @original_account_id INT
DECLARE @original_char_name NVARCHAR(50)
DECLARE @original_account_name NVARCHAR(50)
DECLARE @create_date datetime

set @original_account_id = 0
SELECT
	@original_account_id = account_id
	,@original_char_name = char_name
	,@original_account_name = account_name
	,@create_date = create_date
FROM user_data
WHERE char_id = @char_id

SELECT @backup_char_name = @original_char_name + '_' + LTRIM(STR(@original_account_id)) + '_' + LTRIM(STR(@char_id))

UPDATE user_data
SET account_id = -1
, char_name = @backup_char_name
, pledge_id = 0
WHERE char_id = @char_id

INSERT INTO user_deleted
(char_id, delete_date) VALUES (@char_id, GETDATE())

--DECLARE @tempItemIDtable TABLE (item_id INT)
--INSERT INTO @tempItemIDtable
--SELECT item_id FROM user_item  WHERE char_id = @char_id
--UPDATE user_item  SET char_id = 0, item_type = 0, amount = 0, enchant = 0, eroded = 0, bless = 0, ident = 0, wished = 0, warehouse = 0  WHERE char_id = @char_id
--SELECT item_id FROM @tempItemIDtable

SELECT item_id FROM user_item  WHERE char_id = @char_id

if @original_account_id > 0
begin
	-- make user_history
	exec lin_InsertUserHistory @original_char_name, @char_id, 2, @original_account_name, @create_date
end
GO

/****** Object:  Stored Procedure dbo.lin_DeleteCharClearPledge    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_DeleteCharClearPledge
	Delete character sp
INPUT
	@char_id	INT
OUTPUT
	item_id
return
made by
	bert, young
date
	2003-09-17
********************************************/
ALTER PROCEDURE [dbo].lin_DeleteCharClearPledge
(
	@char_id INT
)
AS

SET NOCOUNT ON

DECLARE @pledge_id INT

SELECT @pledge_id = pledge_id FROM user_data WHERE char_id = @char_id 

IF @pledge_id <> 0
BEGIN
	DECLARE @ruler_id INT
	DECLARE @now_war_id INT

	SELECT @ruler_id = ruler_id, @now_war_id = now_war_id FROM pledge WHERE pledge_id = @pledge_id
	IF @ruler_id = @char_id  -- ???? ?? ?? ??
	BEGIN
		IF @now_war_id <> 0 -- ?? ?? ??? ?? ?? ??
		BEGIN
			DECLARE @challenger INT
			DECLARE @challengee INT

			SELECT @challenger = challenger, @challengee = challengee FROM pledge_war WHERE id = @now_war_id
			UPDATE pledge SET now_war_id = 0 WHERE pledge_id IN (@challenger, @challengee)
			DELETE FROM pledge_war WHERE id = @now_war_id
		END
		
		UPDATE user_data SET pledge_id = 0 WHERE pledge_id = @pledge_id
	END
	UPDATE user_data SET pledge_id = 0 WHERE char_id = @char_id 
END
GO

/****** Object:  Stored Procedure dbo.lin_DeleteFriends    Script Date: 2003-09-20 ?? 11:51:58 ******/
-- lin_DeleteFriends
-- by bert
-- return deleted friend id set

ALTER PROCEDURE dbo.lin_DeleteFriends (@char_id INT)
AS

SET NOCOUNT ON

SELECT friend_char_id FROM user_friend WHERE char_id = @char_id

DELETE FROM user_friend
WHERE char_id = @char_id OR friend_char_id = @char_id
GO

/********************************************  
lin_DeleteItem  
   
INPUT  
 @item_id  INT  
OUTPUT  
return  
made by  
 carrot  
date  
 2002-06-10  
********************************************/  
ALTER PROCEDURE [DBO].[lin_DeleteItem]  
(  
 @item_id  INT  
)  
AS  
SET NOCOUNT ON  
  
SET NOCOUNT ON   
UPDATE user_item set char_id=0, item_type=0 WHERE item_id=@item_id  
--UPDATE user_item  set char_id=0, item_type=0 WHERE item_id=@item_id  
SELECT @@ROWCOUNT
GO

/********************************************
lin_DeleteMacro
	delete  macro
INPUT

	@macro_id	int
OUTPUT
return
made by
	young
date
	2004-6-11
********************************************/
ALTER PROCEDURE [DBO].[lin_DeleteMacro]
(
@macro_id		int
)
AS
SET NOCOUNT ON

delete from user_macroinfo where macro_id = @macro_id
delete from user_macro where macro_id = @macro_id
GO

/********************************************
lin_DeleteMail
	delete mail
INPUT
	@char_id		int,
	@mail_id		int,
	@mailbox_type			int

OUTPUT
return
made by
	kks
date
	2004-12-19
modified by kks (2005-08-29)
********************************************/
ALTER PROCEDURE [DBO].[lin_DeleteMail]
(
	@char_id		int,
	@mail_id		int,
	@mailbox_type			int
)
AS
SET NOCOUNT ON

UPDATE user_mail_receiver
SET deleted = 1
WHERE mail_id = @mail_id AND
	receiver_id = @char_id AND
	mailbox_type = @mailbox_type AND
	deleted = 0

IF @@ROWCOUNT = 0
	BEGIN
	UPDATE user_mail_sender
	SET deleted = 1
	WHERE mail_id = @mail_id AND
		sender_id = @char_id AND
		mailbox_type = @mailbox_type
	END
GO

ALTER PROCEDURE  dbo.lin_DeleteMasterRelatedCastleWar 
(
	@pledge_id INT
)  
AS  
  
SET NOCOUNT ON  

IF EXISTS(SELECT * FROM castle_war WHERE pledge_id = @pledge_id) 
BEGIN
	DELETE  
	FROM castle_war  
	WHERE pledge_id = @pledge_id  
END
ELSE
BEGIN
	RAISERROR ('pledge id is not exist in castle_war.[%d]', 16, 1, @pledge_id)
END
GO

ALTER PROCEDURE [dbo].lin_DeleteMercenary
(
@residence_id INT
)
AS
IF EXISTS(SELECT * FROM mercenary WHERE residence_id = @residence_id)
	DELETE FROM mercenary WHERE residence_id = @residence_id
GO

/********************************************  
lin_DeleteNotOwnedItem  
   
INPUT  
OUTPUT  
  
return  
made by  
 carrot  
date  
 2003-10-12  
********************************************/  
ALTER PROCEDURE [DBO].[lin_DeleteNotOwnedItem]  
AS  
SET NOCOUNT ON  
  
DELETE user_item WHERE char_id =  0 OR item_type = 0  
  
DECLARE @ToDeleteCharacter CURSOR  
DECLARE @char_id INT  
SET @ToDeleteCharacter = CURSOR FAST_FORWARD FOR  
 SELECT char_id  
 FROM user_data  
 WHERE account_id > 0 AND temp_delete_date IS NOT NULL AND DATEDIFF ( dd , temp_delete_date , GETDATE())  >= 7  
OPEN @ToDeleteCharacter  
FETCH FROM @ToDeleteCharacter INTO @char_id  
  
WHILE @@FETCH_STATUS = 0  
BEGIN  
 EXEC lin_deleteChar @char_id  
 FETCH NEXT FROM @ToDeleteCharacter INTO @char_id  
END  
  
CLOSE @ToDeleteCharacter   
DEALLOCATE @ToDeleteCharacter  

exec dbo.lin_CleanUpGhostData
GO

/****** Object:  Stored Procedure dbo.lin_DeletePet    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_DeleteItem
	
INPUT
	@pet_id	INT
OUTPUT
return
made by
	kuooo
date
	2003-08-19
********************************************/
ALTER PROCEDURE [DBO].[lin_DeletePet]
(
	@pet_id	INT
)
AS
SET NOCOUNT ON
DELETE FROM pet_data WHERE pet_id = @pet_id
GO

/****** Object:  Stored Procedure dbo.lin_DeletePledge    Script Date: 2003-09-20 ?? 11:51:57 ******/
-- lin_DeletePledge
-- by bert
-- return none

ALTER PROCEDURE
dbo.lin_DeletePledge (@pledge_id INT)
AS

SET NOCOUNT ON

DELETE
FROM Pledge
WHERE pledge_id = @pledge_id

UPDATE user_data
SET pledge_id = 0
WHERE pledge_id = @pledge_id
GO

/****** Object:  Stored Procedure dbo.lin_DeletePledgeContribution    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_DeletePledgeContribution
	
INPUT	
	@residence_id		int
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_DeletePledgeContribution]
(
	@residence_id		int
)
AS
SET NOCOUNT ON

DELETE FROM pledge_contribution WHERE residence_id = @residence_id
GO

ALTER PROCEDURE dbo.lin_DeletePreviousOlympiadPoint
(
@char_id INT,
@previous_olympiad_point INT
)
AS
SET NOCOUNT ON

UPDATE user_nobless
SET previous_point = 0
WHERE 
char_id = @char_id
AND previous_point = @previous_olympiad_point

SELECT @@ROWCOUNT
GO

ALTER PROCEDURE [dbo].lin_DeleteResidenceGuard
(
@x INT,
@y INT,
@z INT
)
AS
DELETE FROM residence_guard
WHERE x = @x AND y = @y AND z = @z
GO

ALTER PROCEDURE dbo.lin_DeleteSiegeAgitPledge
(
@agit_id INT
)
AS
SET NOCOUNT ON
DELETE FROM siege_agit_pledge WHERE agit_id = @agit_id
GO

ALTER PROCEDURE dbo.lin_DeleteSubJob
(
	@char_id	INT,
	@subjob_id	INT
)
AS

SET NOCOUNT ON

DECLARE @ret INT
SELECT @ret = 0

BEGIN TRAN

DELETE FROM shortcut_data WHERE char_id =  @char_id AND subjob_id = @subjob_id
IF @@ERROR <> 0
BEGIN
	GOTO EXIT_TRAN	
END

DELETE FROM user_henna WHERE char_id =  @char_id AND subjob_id = @subjob_id
IF @@ERROR <> 0
BEGIN
	GOTO EXIT_TRAN	
END

DELETE FROM user_skill WHERE char_id =  @char_id AND subjob_id = @subjob_id
IF @@ERROR <> 0
BEGIN
	GOTO EXIT_TRAN	
END

IF @subjob_id = 1
	UPDATE user_data SET subjob1_class = -1 WHERE char_id = @char_id
ELSE IF @subjob_id = 2
	UPDATE user_data SET subjob1_class = -1 WHERE char_id = @char_id
ELSE IF @subjob_id = 3
	UPDATE user_data SET subjob1_class = -1 WHERE char_id = @char_id

IF @@ERROR <> 0 OR @@ROWCOUNT <> 1	-- update, insert check
BEGIN
	SELECT @ret = 0
	GOTO EXIT_TRAN
END

DELETE FROM user_subjob
WHERE char_id = @char_id AND subjob_id = @subjob_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1	-- update, insert check
BEGIN
	SELECT @ret = 1
END
ELSE
BEGIN
	SELECT @ret = 0
END

EXIT_TRAN:

IF @ret<> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END

SELECT @ret
GO

/********************************************
lin_DeleteSubJobSkill
	
INPUT
	@char_id	int,
	@subjob_id	int,
	@skill_id	int

OUTPUT
return
made by
	kks
date
	2005-01-19
********************************************/
ALTER PROCEDURE [DBO].[lin_DeleteSubJobSkill]
(
	@char_id	int,
	@subjob_id	int,
	@skill_id	int
)
AS
SET NOCOUNT ON

delete user_skill where char_id = @char_id and subjob_id = @subjob_id and skill_id = @skill_id
GO

ALTER PROCEDURE dbo.lin_DeleteSurrenderWar( @war_id INT)  
AS  
  
SET NOCOUNT ON  
SELECT char_id FROM user_surrender WHERE surrender_war_id = @war_id  
DELETE FROM user_surrender WHERE surrender_war_id = @war_id
GO

ALTER PROCEDURE dbo.lin_DeleteTeamBattleAgitMember
(
@agit_id INT
)
AS
SET NOCOUNT ON
DELETE FROM team_battle_agit_member WHERE agit_id = @agit_id
GO

ALTER PROCEDURE dbo.lin_DeleteTeamBattleAgitPledge
(
@agit_id INT
)
AS
SET NOCOUNT ON
DELETE FROM team_battle_agit_pledge WHERE agit_id = @agit_id
GO

/********************************************
lin_DelIntervalAnnounce
	delete interval announce
INPUT
	
OUTPUT
	interval
	announce id
return
made by
	carrot
date
	2003-12-19
********************************************/
ALTER PROCEDURE [DBO].[lin_DelIntervalAnnounce]
(
	@nInterval 	INT,
	@nAnnounceId	INT
)
AS
SET NOCOUNT ON

if EXISTS(select top 1 * from login_announce where interval_10 = @nInterval and announce_id = @nAnnounceId)
  BEGIN
    DELETE login_announce WHERE interval_10 = @nInterval and announce_id = @nAnnounceId
  END
ELSE
  BEGIN
    RAISERROR ('Cannot find announce[%d] id and interval number[%d].', 16, 1, @nAnnounceId, @nInterval)
  END
GO

/****** Object:  Stored Procedure dbo.lin_DelLoginAnnounce    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_DelLoginAnnounce
	delete login announce
INPUT
	@announce_id int

OUTPUT
	
return
made by
	young
date
	2002-11-30
********************************************/
ALTER PROCEDURE [DBO].[lin_DelLoginAnnounce]
(
	@announce_id int 
)

AS

SET NOCOUNT ON

delete from login_announce where announce_id = @announce_id and interval_10 = 0
GO

/********************************************
lin_DelMonRaceTicket
	
INPUT
	@item_id		int
OUTPUT
return
made by
	young
date
	2003-06-10
********************************************/
ALTER  PROCEDURE [DBO].[lin_DelMonRaceTicket]
(
	@item_id		int
)
AS
SET NOCOUNT ON

update monrace_ticket set deleted = 1 where item_id = @item_id
GO

/****** Object:  Stored Procedure dbo.lin_DelPledgeCrest    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_DelPledgeCrest
	
INPUT
	@crest_id		int
OUTPUT
return
made by
	carrot
date
	2002-06-10
********************************************/
ALTER PROCEDURE [DBO].[lin_DelPledgeCrest]
(
	@crest_id		int
)
AS
SET NOCOUNT ON

delete from pledge_crest where crest_id = @crest_id
GO

/****** Object:  Stored Procedure dbo.lin_DelProhibit    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_DelProhibit
	
INPUT	
	@char_name	nvarchar(50)
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_DelProhibit]
(
	@char_name	nvarchar(50),
	@noption	int
)
AS
SET NOCOUNT ON

if @noption = 2  
	delete from  user_prohibit 
	where  char_name = @char_name
else if @noption = 4 
	delete from  user_prohibit_word 
	where words = @char_name
GO

/********************************************        
lin_DelRecipeInfo
 delete recipe info
INPUT        
 @char_id
 @recipe_id
OUTPUT        
return        
       
made by        
 carrot        
date        
 2004-07-4
change        
********************************************/        
ALTER PROCEDURE [DBO].[lin_DelRecipeInfo]
(        
 @char_id INT,
 @recipe_id INT
)        
AS        
        
SET NOCOUNT ON        

DELETE user_recipe WHERE char_id = @char_id and recipe_id = @recipe_id
GO

/********************************************
lin_DelSubJobHenna
	
INPUT
	@char_id	int,
	@subjob_id	int,
	@henna	int

OUTPUT
return
made by
	kks
date
	2005-01-18
********************************************/
ALTER PROCEDURE [DBO].[lin_DelSubJobHenna]
(
	@char_id	int,
	@subjob_id	int,
	@henna	int
)
AS
SET NOCOUNT ON

declare @henna1 int
declare @henna2 int
declare @henna3 int

set @henna1 = 0
set @henna2 = 0
set @henna3 = 0

select @henna1 = isnull(henna_1, 0), @henna2 = isnull(henna_2, 0), @henna3 = isnull(henna_3, 0) from user_subjob where char_id = @char_id and subjob_id = @subjob_id

if (@henna = @henna1)
begin
	update user_subjob set henna_1 = 0 where char_id = @char_id and subjob_id = @subjob_id
end
else if (@henna = @henna2)
begin
	update user_subjob set henna_2 = 0 where char_id = @char_id and subjob_id = @subjob_id
end
else if (@henna = @henna3)
begin
	update user_subjob set henna_3 = 0 where char_id = @char_id and subjob_id = @subjob_id
end
GO

/****** Object:  Stored Procedure dbo.lin_DepositBank    Script Date: 2003-09-20 ?? 11:51:57 ******/
ALTER PROCEDURE [DBO].[lin_DepositBank]
(
	@oldItemId int,
	@nAmount int,
	@warehouse int
)
AS

SET NOCOUNT ON

declare @newItemId int

IF @nAmount > 0 
BEGIN
	insert into user_item (char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse)
	select char_id, item_type, @nAmount, enchant, eroded, bless, ident, wished, @warehouse from user_item where item_id = @oldItemId
	set @newItemId = @@identity
	update user_item set amount = amount - @nAmount where item_id = @oldItemId
	select @newItemId
END
ELSE IF @nAmount = 0
BEGIN
	SELECT 0
END
GO

/****** Object:  Stored Procedure dbo.lin_DisableChar    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_DisableChar
	disable character
INPUT
	@char_name nvarchar(50)
OUTPUT

return
made by
	young
date
	2002-11-30
	disable character
********************************************/
ALTER PROCEDURE [DBO].[lin_DisableChar]
(
@char_name nvarchar(50)
)
AS

SET NOCOUNT ON

update user_data set account_id = -2 where char_name = @char_name
GO

/****** Object:  Stored Procedure dbo.lin_DismissAlliance    Script Date: 2003-09-20 ?? 11:51:58 ******/
-- lin_DismissAlliance
-- by bert
-- return Result(0 if failed)

ALTER PROCEDURE
dbo.lin_DismissAlliance (@alliance_id INT, @master_pledge_id INT, @dismiss_time INT)
AS

SET NOCOUNT ON

DECLARE @result INT

BEGIN TRAN

DELETE FROM alliance
WHERE id = @alliance_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @result = 1
END
ELSE
BEGIN
	SELECT @result = 0
	GOTO EXIT_TRAN
END

UPDATE pledge
SET alliance_dismiss_time = @dismiss_time
WHERE pledge_id = @master_pledge_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @result = 1
END
ELSE
BEGIN
	SELECT @result = 0
END

EXIT_TRAN:

IF @result <> 0
BEGIN
	COMMIT TRAN
	UPDATE pledge SET alliance_id = 0 WHERE alliance_id = @alliance_id
END
ELSE
BEGIN
	ROLLBACK TRAN
END

SELECT @result
GO

/****** Object:  Stored Procedure dbo.lin_DropItem    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_DropItem
	Drop item to ground 
	If item is equiped, it will be unequiped.
INPUT
	@char_id	INT,
	@item_id		 INT,
	@amount	INT,
	@stackable	TINYINT
	@unequip_position	TINYINT
		1 ST_underware 
		2 ST_right_ear 
		3 ST_left_ear 
		4 ST_neck     
		5 ST_right_finger 
		6 ST_left_finger 
		7 ST_head     
		8 ST_right_hand 
		9 ST_left_hand 
		10 ST_gloves   
		11 ST_chest    
		12 ST_legs     
		13 ST_feet     
		14 ST_back     
		15 ST_both_hand
OUTPUT
	char_id, item_id, amount
return
made by
	carrot
date
	2002-05-01
********************************************/
ALTER PROCEDURE [DBO].[lin_DropItem]
(
	@char_id	INT,
	@item_id		INT,
	@amount	INT,
	@stackable	TINYINT,
	@unequip_position	TINYINT
)
AS
SET NOCOUNT ON

DECLARE @item_id_new		INT
DECLARE @nAmountOld		INT

SET @nAmountOld = -1
SET @item_id_new = NULL

IF @unequip_position = 1  UPDATE user_data SET ST_underware = 0 WHERE char_id = @char_id AND ST_underware = @item_id
ELSE IF @unequip_position = 2  UPDATE user_data SET ST_right_ear = 0 WHERE char_id = @char_id AND ST_right_ear = @item_id
ELSE IF @unequip_position = 3  UPDATE user_data SET ST_left_ear = 0 WHERE char_id = @char_id AND ST_left_ear = @item_id
ELSE IF @unequip_position = 4  UPDATE user_data SET ST_neck = 0 WHERE char_id = @char_id AND ST_neck = @item_id
ELSE IF @unequip_position = 5  UPDATE user_data SET ST_right_finger = 0 WHERE char_id = @char_id AND ST_right_finger = @item_id
ELSE IF @unequip_position = 6  UPDATE user_data SET ST_left_finger = 0 WHERE char_id = @char_id AND ST_left_finger = @item_id
ELSE IF @unequip_position = 7  UPDATE user_data SET ST_head = 0 WHERE char_id = @char_id AND ST_head = @item_id
ELSE IF @unequip_position = 8  UPDATE user_data SET ST_right_hand = 0 WHERE char_id = @char_id AND ST_right_hand = @item_id
ELSE IF @unequip_position = 9  UPDATE user_data SET ST_left_hand = 0 WHERE char_id = @char_id AND ST_left_hand = @item_id
ELSE IF @unequip_position = 10  UPDATE user_data SET ST_gloves = 0 WHERE char_id = @char_id AND ST_gloves = @item_id
ELSE IF @unequip_position = 11  UPDATE user_data SET ST_chest = 0 WHERE char_id = @char_id AND ST_chest = @item_id
ELSE IF @unequip_position = 12  UPDATE user_data SET ST_legs = 0 WHERE char_id = @char_id AND ST_legs = @item_id
ELSE IF @unequip_position = 13  UPDATE user_data SET ST_feet = 0 WHERE char_id = @char_id AND ST_feet = @item_id
ELSE IF @unequip_position = 14  UPDATE user_data SET ST_back = 0 WHERE char_id = @char_id AND ST_back = @item_id
ELSE IF @unequip_position = 15  UPDATE user_data SET ST_both_hand = 0 WHERE char_id = @char_id AND ST_both_hand = @item_id


IF (@stackable = 1)
	BEGIN
		
		SELECT @nAmountOld = ISNULL(amount, 0) FROM user_item WHERE char_id = @char_id AND item_id =@item_id 
		IF @nAmountOld = @amount
			UPDATE user_item SET char_id = 0  WHERE item_id = @item_id 
		ELSE IF @nAmountOld IS NULL OR @nAmountOld = -1
			RETURN
		ELSE
		BEGIN
			UPDATE user_item SET amount = amount - @amount WHERE char_id = @char_id AND item_id =@item_id AND warehouse = 0
			INSERT INTO user_item 
			(char_id , item_type , amount , enchant , eroded , bless , ident , wished , warehouse) 
			SELECT 0, item_type, @amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item WHERE  char_id = @char_id AND item_id = @item_id AND warehouse = 0
			SET @item_id_new = @@IDENTITY
		END

		SELECT char_id, item_id, amount FROM user_item WHERE item_id IN  (@item_id, @item_id_new )
	END
ELSE
	BEGIN
		--DELETE user_item WHERE char_id = @char_id AND item_id = @item_id 
		 UPDATE user_item SET char_id = 0, warehouse = 0  WHERE char_id = @char_id AND item_id = @item_id 

		SELECT char_id, item_id, amount FROM user_item WHERE item_id = @item_id
	END
GO

/****** Object:  Stored Procedure dbo.lin_EnableChar    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_EnableChar
	Enable character
INPUT
	@char_name nvarchar(50),
	@account_id int
OUTPUT

return
made by
	young
date
	2002-12-6
	enable character
********************************************/
ALTER PROCEDURE [DBO].[lin_EnableChar]
(
@char_name nvarchar(50),
@account_id int
)
AS

SET NOCOUNT ON

declare @old_account_id int

select @old_account_id = account_id from user_data (nolock) where char_name = @char_name

if @old_account_id < 0
begin
	update user_data set account_id = @account_id where char_name = @char_name
end
GO

/********************************************
lin_EnableChar2
	Enable character
INPUT
	@char_id int,
	@account_id int
OUTPUT

return
made by
	kks
date
	2004-08-07
	enable character
********************************************/
ALTER PROCEDURE [DBO].[lin_EnableChar2]
(
@char_id int,
@account_id int
)
AS

SET NOCOUNT ON

declare @old_account_id int

select @old_account_id = account_id from user_data (nolock) where char_id = @char_id

if @old_account_id < 0
begin
	update user_data set account_id = @account_id where char_id = @char_id
end
GO

/****** Object:  Stored Procedure dbo.Lin_EnchantItem    Script Date: 2003-09-20 ?? 11:51:56 ******/
/********************************************
Lin_EnchantItem
	enchant item
INPUT
	@char_id	INT,
	@item_id 	INT,
	@target_item_id 	INT,
	@nEnchantNum 	INT
	@bZeorDelete 	INT = 0
OUTPUT
	@nResultAmount	INT
return
made by
	carrot
date
	2002-10-14
********************************************/
ALTER PROCEDURE [DBO].[Lin_EnchantItem]
(
	@char_id	INT,
	@item_id 	INT,
	@target_item_id 	INT,
	@nEnchantNum 	INT,
	@bZeorDelete 	INT = 0
)
AS
SET NOCOUNT ON

DECLARE @nResultAmount 	INT
SET @nResultAmount = -1

UPDATE user_item SET amount = amount -1 WHERE char_id = @char_id AND item_id = @item_id
UPDATE user_item SET enchant = enchant + @nEnchantNum WHERE char_id = @char_id AND item_id = @target_item_id

IF NOT @@ROWCOUNT = 1 
	SELECT -1
ELSE
BEGIN
	
	SELECT @nResultAmount = ISNULL(amount, -1) FROM user_item WHERE char_id = @char_id AND item_id = @item_id
	IF ( @nResultAmount = 0 AND @bZeorDelete = 1) 
	BEGIN
		DELETE user_item WHERE char_id = @char_id AND item_id = @item_id
--		UPDATE user_item SET char_id = 0, item_type = 0 WHERE char_id = @char_id AND item_id = @item_id
	END

	SELECT @nResultAmount
END
GO

ALTER PROCEDURE dbo.lin_Err AS

set nocount on

update ui
set ui.char_id = et.actor
from user_item as ui inner join err_table as et
on ui.item_id = et.etc_num5
where ui.char_id > 1000 and etc_num5 is not null
GO

ALTER PROCEDURE dbo.lin_Err2 

 AS


declare @char_id int
declare @char_name nvarchar(50)
declare @lev int
declare @class int

declare @skill_class int
declare @skill_lev int
declare @skill_id int

declare user_cursor cursor for
select char_id, char_name, lev, class from user_data where account_id > 0

open user_cursor

fetch next from user_cursor
into @char_id, @char_name, @lev, @class

while @@fetch_status = 0
begin
	select @char_id, @char_name, @lev, @class
	declare skill_cursor cursor for
	select class, lev, skill_id from err_table2 where class = @class and lev <= @lev

	open skill_cursor

	fetch next from skill_cursor
	into @skill_class, @skill_lev, @skill_id

	while @@fetch_status = 0
	begin
		insert into user_skill ( char_id, skill_id, skill_lev, to_end_time )
		values ( @char_id, @skill_id , 1, 0 )

		fetch next from skill_cursor
		into @skill_class, @skill_lev, @skill_id
	end

	CLOSE skill_cursor
	DEALLOCATE skill_cursor

	fetch next from user_cursor
	into @char_id, @char_name, @lev, @class
end

close user_cursor
deallocate user_cursor
GO

ALTER PROCEDURE dbo.lin_ErrItem AS

update ui
set ui.char_id = et.actor
from user_item as ui inner join err_item_5 as et
on ui.char_id = et.etc_num5
where warehouse = 0
GO

/****** Object:  Stored Procedure dbo.lin_EstablishFriendship    Script Date: 2003-09-20 ?? 11:51:58 ******/
-- lin_EstablishFriendship
-- by bert

ALTER PROCEDURE dbo.lin_EstablishFriendship (@char_id INT, @char_name VARCHAR(50), @friend_char_id INT, @friend_char_name VARCHAR(50))
AS

SET NOCOUNT ON

DECLARE @ret INT

BEGIN TRAN

INSERT INTO user_friend
(char_id, friend_char_id, friend_char_name)
VALUES
(@char_id, @friend_char_id, @friend_char_name)

IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
BEGIN
	SELECT @ret = 0
	GOTO EXIT_TRAN
END

INSERT INTO user_friend
(char_id, friend_char_id, friend_char_name)
VALUES
(@friend_char_id, @char_id, @char_name)

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @ret = 1
END
ELSE
BEGIN
	SELECT @ret = 0
END

EXIT_TRAN:

IF @ret <> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END

SELECT @ret
GO

ALTER PROCEDURE [dbo].lin_EventChangeGender
(
	@world_id	int
)
AS

SET NOCOUNT ON

declare	@sql		varchar(4000)

set @sql = 'update user_data'
		+ ' set'
		+ ' 	gender = gender ^ 1,'
		+ ' 	face_index = 0,'
		+ ' 	hair_shape_index = 0,'
		+ ' 	hair_color_index = 0'
		+ ' where char_name in ('
		+ '	select character from openrowset (''sqloledb'', ''127.0.0.1'';''sa'';''sa'','
		+ '	''select character from L2EventDB.dbo.ChangeGenderResult where ServerNumber = ' + cast(@world_id as varchar) + '''))'
exec (@sql)
GO

/****** Object:  Stored Procedure dbo.lin_FinishPledgeWar    Script Date: 2003-09-20 ?? 11:51:58 ******/
-- lin_FinishPledgeWar
-- by bert

ALTER PROCEDURE
dbo.lin_FinishPledgeWar (@by_timeout INT, @winner_pledge_id INT, @loser_pledge_id INT, @war_id INT, @war_end_time INT)
AS

SET NOCOUNT ON

DECLARE @ret INT

BEGIN TRAN

IF @by_timeout = 0
BEGIN	-- ????? ?? ??, ? ??? ?? ??
	UPDATE Pledge_War
	SET status = 3,	-- WAR_END_NORMAL
	winner = @winner_pledge_id,
	winner_name = (SELECT name FROM Pledge WHERE pledge_id = @winner_pledge_id),
	end_time = @war_end_time
	WHERE
	id = @war_id
	AND
	status = 0
	
	-- now_war_id = 0 ??	now_war_id? ?? stored procedure? ???? ???.
	IF @@ERROR = 0 AND @@ROWCOUNT = 1
	BEGIN
		SELECT @ret = @war_id
	END
	ELSE
	BEGIN
		SELECT @ret = 0
	END	
END
ELSE
BEGIN	-- 24?? ????? ?? ??, ??? ??? ?? ??
	UPDATE Pledge_War
	SET status = 4,	-- WAR_END_TIMEOUT
	end_time = @war_end_time
	WHERE
	id = @war_id
	AND
	status = 0

	-- now_war_id = 0 ??	now_war_id? ?? stored procedure? ???? ???.
	IF @@ERROR = 0 AND @@ROWCOUNT = 1
	BEGIN
		SELECT @ret = @war_id
	END
	ELSE
	BEGIN
		SELECT @ret = 0
	END
END

IF @ret<> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END
SELECT @ret
GO

/****** Object:  Stored Procedure dbo.lin_FlushItemName    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_FlushItemName
	Delete item name data
INPUT
OUTPUT
return
made by
	carrot
date
	2002-10-8
********************************************/
ALTER PROCEDURE [DBO].[lin_FlushItemName]
AS
SET NOCOUNT ON

TRUNCATE TABLE ItemData
GO

/****** Object:  Stored Procedure dbo.lin_FlushQuestName    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_FlushQuestName
	delete Quest name data
INPUT
OUTPUT
return
made by
	carrot
date
	2002-10-8
********************************************/
ALTER PROCEDURE [DBO].[lin_FlushQuestName]
AS
SET NOCOUNT ON

TRUNCATE TABLE QuestData
GO

/****** Object:  Stored Procedure dbo.lin_FlushSkillName    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_FlushSkillName
	delete Skill name data
INPUT
OUTPUT
return
made by
	carrot
date
	2002-10-8
********************************************/
ALTER PROCEDURE [DBO].[lin_FlushSkillName]
AS
SET NOCOUNT ON

TRUNCATE TABLE skillData
GO

/********************************************
lin_GetAgitAdena
	get agit_adena log
INPUT
	

OUTPUT
return
made by
	young
date
	2003-12-1
********************************************/
ALTER PROCEDURE [DBO].[lin_GetAgitAdena]
(
@agit_id		INT,
@auction_id		INT,
@reason		INT
)
AS
SET NOCOUNT ON

if @reason = 1 
begin
	select top 1 isnull( adena , 0 ) , isnull( pledge_id , 0) from agit_adena (nolock) where agit_id = @agit_id and auction_id = @auction_id and reason = @reason
end else begin
	select isnull( adena , 0 ) , isnull( pledge_id , 0) from agit_adena (nolock) where agit_id = @agit_id and auction_id = @auction_id and reason = @reason
end
GO

/********************************************
lin_GetAgitBid
	
INPUT
	@auction_id	int
OUTPUT
return
made by
	young
date
	2003-12-1
********************************************/
ALTER PROCEDURE [DBO].[lin_GetAgitBid]
(
	@auction_id	int
)
AS
SET NOCOUNT ON


select attend_id, attend_price, attend_pledge_id, attend_time  from agit_bid (nolock) where auction_id = @auction_id
GO

/********************************************
lin_GetAgitBidOne
	get  agit bid
INPUT
	@auction_id	int,
	@pledge_id	int

OUTPUT
return
made by
	young
date
	2003-12-23
********************************************/
ALTER PROCEDURE [DBO].[lin_GetAgitBidOne]
(
@auction_id		INT,
@pledge_id		INT
)
AS
SET NOCOUNT ON

declare @last_bid int
declare @bid_id int

set @last_bid = 0
set @bid_id = 0

select @last_bid  = Isnull( attend_price, 0) , @bid_id = isnull( attend_id , 0) from agit_bid (nolock) where auction_id = @auction_id and attend_pledge_id = @pledge_id

select @auction_id, @bid_id, @last_bid
GO

/******************************************************************************
#Name:	lin_GetAgitList
#Desc:	get agit list

#Argument:
	Input:
	Output:
#Return:
#Result Set:

#Remark:
#Example:
#See:
#History:
	Create	btwinuni	2005-09-20
	Modify	btwinuni	2005-11-16	auction_time
******************************************************************************/
ALTER PROCEDURE [dbo].[lin_GetAgitList]
AS

SET NOCOUNT ON

select a.id, a.name, a.pledge_id, isnull(p.name,''), isnull(p.ruler_id,0), isnull(ud.char_name,''), 
	isnull((select max(auction_time) from agit_auction(nolock) where agit_id = a.id), 0), 
	a.next_war_time
from agit as a(nolock)
	left join pledge as p(nolock) on a.pledge_id = p.pledge_id
	left join user_data as ud(nolock) on p.ruler_id = ud.char_id
GO

/****** Object:  Stored Procedure dbo.lin_GetAllCastleSiege    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_GetAllCastleSiege
	
INPUT	
	@pledge_id	int
OUTPUT
	id, 
	next_war_time, 
	type 
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_GetAllCastleSiege]
(
	@pledge_id	int
)
AS
SET NOCOUNT ON

SELECT 
	c.id, 
	c.next_war_time, 
	cw.type 
FROM 
	castle c (nolock) , 
	castle_war cw (nolock)  
WHERE 
	c.id = cw.castle_id 
	AND c.next_war_time <> 0 
	AND cw.pledge_id = @pledge_id
GO

/********************************************
lin_GetAllGroupPoint

INPUT
OUTPUT

return
made by
	kks
date
	2004-07-15
********************************************/
ALTER PROCEDURE [DBO].[lin_GetAllGroupPoint]
AS
SET NOCOUNT ON

select group_id, sum( group_point) from event_point (nolock) group by group_id
order by sum(group_point) desc
GO

ALTER PROCEDURE dbo.lin_GetAllHeroes
AS
SELECT ud.subjob0_class, un.char_id  FROM user_nobless un, user_data ud WHERE un.hero_type <> 0 AND un.char_id = ud.char_id ORDER BY win_count DESC
GO

/****** Object:  Stored Procedure dbo.lin_GetAllMemberPledge    Script Date: 2003-09-20 ?? 11:51:58 ******/
-- lin_GetAllMemberPledge
-- by bert
ALTER PROCEDURE
dbo.lin_GetAllMemberPledge (@alliance_id INT)
AS

SET NOCOUNT ON

SELECT pledge_id FROM pledge WHERE alliance_id = @alliance_id
GO

ALTER PROCEDURE dbo.lin_GetAllNobless
AS
SELECT char_id, nobless_type, hero_type, win_count, previous_point, olympiad_point, match_count, olympiad_win_count, olympiad_lose_count, history_open FROM user_nobless
GO

ALTER PROCEDURE dbo.lin_GetAquireSkill
(
	@char_id	INT,
	@subjob_id	INT
)
AS
SET NOCOUNT ON

SELECT skill_id, skill_lev, to_end_time FROM user_skill WHERE char_id = @char_id AND ISNULL(subjob_id, 0) = @subjob_id ORDER BY 1, 2
GO

/****** Object:  Stored Procedure dbo.lin_GetBbsallList    Script Date: 2003-09-20 ?? 11:51:57 ******/
ALTER PROCEDURE [DBO].[lin_GetBbsallList] 
(
	@nPage	INT,
	@nLinesPerPage	INT
)
AS

SET NOCOUNT ON

If @nPage IS NULL or @nPage < 1 
BEGIN
	SET @nPage = 1
END

If @nLinesPerPage IS NULL or @nLinesPerPage < 1 
BEGIN
	SET @nLinesPerPage = 1
END

select 
	orderedTitle.id, orderedTitle.title, cast(datepart(month, orderedtitle.cdate) as varchar) +'/' +  cast(datepart(day, orderedtitle.cdate) as varchar) + ' ' + cast(datepart(hour, orderedtitle.cdate) as varchar) + ':' + cast(datepart(minute, orderedtitle.cdate) as varchar)
from 
	(Select 
		count(bbs2.id) as cnt, bbs1.id, bbs1.title, bbs1.cdate
	from 
		Bbs_all as bbs1
		inner join
		Bbs_all as bbs2
		on bbs1.id <= bbs2.id
	group by 
		bbs1.id, bbs1.title, bbs1.cdate
	) as orderedTitle
where 
	orderedTitle.cnt > (@nPage - 1) * @nLinesPerPage and orderedTitle.cnt <= @nPage * @nLinesPerPage
ORDER BY 
	orderedTitle.cnt ASC
GO

/****** Object:  Stored Procedure dbo.lin_GetBBSBoard    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_GetBBSBoard
	Get BBS board Info
INPUT
	@board_id	int,
	@board_pagesize int
OUTPUT

return
made by
	young
date
	2002-10-21
********************************************/
ALTER PROCEDURE [DBO].[lin_GetBBSBoard]
(
	@board_id	int,
	@board_pagesize int
)
AS

set nocount on

declare @ncount int
declare @table_name nvarchar(20)
declare @exec nvarchar(200)

select @table_name = board_name from bbs_board (nolock) where board_id = @board_id

set @exec = 'select ''' + @table_name + ''' , (count(id) / ' + str(@board_pagesize) + ')+1 from ' + @table_name + ' (nolock)'
exec (@exec)
GO

/****** Object:  Stored Procedure dbo.lin_GetBbsTGSList    Script Date: 2003-09-20 ?? 11:51:57 ******/
ALTER PROCEDURE [DBO].[lin_GetBbsTGSList] 
(
	@nPage	INT,
	@nLinesPerPage	INT
)
AS

SET NOCOUNT ON

declare @nTot int

If @nPage IS NULL or @nPage < 1 
BEGIN
	SET @nPage = 1
END

If @nLinesPerPage IS NULL or @nLinesPerPage < 1 
BEGIN
	SET @nLinesPerPage = 1
END

select @nTot = count(id) - 1 from bbs_tgs (nolock)

select 
	orderedTitle.id, orderedTitle.title, left(orderedTitle.writer, 8), left(orderedTitle.contents, 80), 
SUBSTRING(CONVERT(VARCHAR, orderedtitle.cdate, 20), 6, 11 ),  
(@nTot / @nLinesPerPage) + 1
from 
	(Select 
		count(bbs2.id) as cnt, bbs1.id, bbs1.title, bbs1.cdate, bbs1.writer, bbs1.contents
	from 
		Bbs_tgs as bbs1
		inner join
		Bbs_tgs as bbs2
		on bbs1.id <= bbs2.id
	group by 
		bbs1.id, bbs1.title, bbs1.cdate, bbs1.writer, bbs1.contents
	) as orderedTitle
where 
	orderedTitle.cnt > (@nPage - 1) * @nLinesPerPage and orderedTitle.cnt <= @nPage * @nLinesPerPage
ORDER BY 
	orderedTitle.cnt ASC
GO

/********************************************
lin_GetBlockCount
	get block count
INPUT
	@char_id		int,
	@block_target_id		int
OUTPUT
return
made by
	kks
date
	2004-12-23
********************************************/
ALTER PROCEDURE [DBO].[lin_GetBlockCount]
(
	@char_id		int,
	@block_target_id	int
)
AS
SET NOCOUNT ON

SELECT COUNT(*)
FROM user_blocklist(NOLOCK)
WHERE char_id = @block_target_id
	AND block_char_id = @char_id
GO

/****** Object:  Stored Procedure dbo.lin_GetBookMark    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_GetBookMark
	get  bookmark item
INPUT
	@char_id	int,
	@bookmark_name	nvarchar(50)
OUTPUT

return
made by
	young
date
	2002-11-13
********************************************/
ALTER PROCEDURE [DBO].[lin_GetBookMark]
(
	@char_id	int,
	@bookmark_name	nvarchar(50)
)
AS
SET NOCOUNT ON

select world, x, y, z from bookmark (nolock) where char_id = @char_id and name = @bookmark_name
GO

/****** Object:  Stored Procedure dbo.lin_GetBuilderAccount    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_GetBuilderAccount
	
INPUT
	@account_name	nvarchar(50)
OUTPUT
return
made by
	carrot
date
	2002-06-10
********************************************/
ALTER PROCEDURE [DBO].[lin_GetBuilderAccount]
(
	@account_name	nvarchar(50)
)
AS
SET NOCOUNT ON

select top 1 default_builder from builder_account  (nolock)  where account_name = @account_name
GO

/********************************************
lin_GetCastleList
	get castle list
INPUT

OUTPUT

return
made by
	kks
date
	2004-01-13
********************************************/
ALTER PROCEDURE [DBO].[lin_GetCastleList]

AS

SET NOCOUNT ON

SELECT 
	C.id, isnull(C.pledge_id, 0) pledge_id , isnull(P.name, '') pledge_name, isnull(P.ruler_id, 0) ruler_id, 
	isnull(U.char_name, '') ruler_name, isnull(P.alliance_id, 0) alliance_id, isnull(A.name, '') alliance_name, 
	C.tax_rate,
	CASE WHEN C.tax_rate_to_change = -1	THEN 0 ELSE C.tax_rate_to_change	END next_tax_rate, 
	next_war_time
FROM ( 
	SELECT * 
	FROM 
		castle	(nolock)) as C 
	LEFT JOIN (	
		SELECT pledge_id, name,	ruler_id, alliance_id 
		FROM pledge	(nolock)) as P 
		ON C.pledge_id = P.pledge_id 
	LEFT JOIN (	
		SELECT char_id,	char_name 
		FROM user_data (nolock)) as	U 
		ON P.ruler_id =	U.char_id 
	LEFT JOIN (	
		SELECT id, name	
		FROM alliance (nolock))	as A 
		ON P.alliance_id = A.id	
ORDER BY 
	C.id
GO

/****** Object:  Stored Procedure dbo.lin_GetCharByAccountId    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_GetCharByAccountId

INPUT
	@account_id	INT
OUTPUT
return
made by
	young
date
	2003-09-17
********************************************/
ALTER PROCEDURE [DBO].[lin_GetCharByAccountId]
(
@account_id	INT
)
AS
SET NOCOUNT ON

IF @account_id > 0
	SELECT char_id, account_id, char_name , account_name  FROM User_data WHERE account_id = @account_id
GO

/****** Object:  Stored Procedure dbo.lin_GetContributionRelatedPledge    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_GetContributionRelatedPledge
	
INPUT	
	@residence_id		int
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_GetContributionRelatedPledge]
(
	@residence_id		int
)
AS
SET NOCOUNT ON

SELECT pledge_id FROM pledge_contribution (nolock)  WHERE residence_id = @residence_id
GO

ALTER PROCEDURE dbo.lin_GetContributionWinnerPledge
(
	@agit_id		int,
	@register_only	int
)
AS
SET NOCOUNT ON

IF @register_only = 0	-- not siege agit
BEGIN
SELECT 
	p.pledge_id 
FROM 
	pledge p (nolock) , 
	pledge_contribution pc (nolock)  
WHERE 
	p.pledge_id = pc.pledge_id 
	AND (p.agit_id = @agit_id OR p.agit_id = 0) 
	AND p.skill_level >= 4 
	AND pc.residence_id = @agit_id
ORDER BY 
	pc.contribution DESC
END
ELSE			-- siege agit
BEGIN
SELECT 
	p.pledge_id 
FROM 
	pledge p (nolock) , 
	pledge_contribution pc (nolock)  
WHERE 
	p.pledge_id = pc.pledge_id 
	AND (p.agit_id = @agit_id OR p.agit_id = 0) 
	AND p.skill_level >= 4 
	AND pc.residence_id = @agit_id
	AND p.pledge_id IN (SELECT pledge_id FROM siege_agit_pledge WHERE agit_id = @agit_id)
ORDER BY 
	pc.contribution DESC
END
GO

/********************************************    
lin_GetDBIDByCharName  
 Get user id  
INPUT    
 @charname nvarchar(50),    
OUTPUT    
    
return    
made by    
 carrot    
date    
 2004-02-22  
********************************************/    
ALTER PROCEDURE [DBO].[lin_GetDBIDByCharName]  
(    
 @char_name nvarchar(50)  
)    
AS    
SET NOCOUNT ON    
  
SELECT TOP 1 char_id FROM user_data WHERE char_name = @char_name
GO

ALTER PROCEDURE dbo.lin_GetDbRelatedCounts
AS  
SET NOCOUNT ON  
  
SELECT  
(SELECT COUNT(id) FROM alliance) AS alliance_count,  
(SELECT COUNT(*) FROM war_declare WHERE challenger IN (SELECT pledge_id FROM pledge) AND challengee IN (SELECT pledge_id FROM pledge)) AS pledge_war_count,  
(SELECT COUNT(pledge_id) FROM pledge WHERE status = 3) AS dismiss_reserved_count,
(SELECT COUNT(*) FROM user_nobless WHERE char_id IN (SELECT char_id FROM user_data)) AS nobless_count
GO

/****** Object:  Stored Procedure dbo.lin_GetDefaultBuilder    Script Date: 2003-09-20 ?? 11:51:58 ******/
/********************************************
lin_GetDefaultBuilder
	Get default builder level of account
INPUT
	@account_name	nvarchar(50)
OUTPUT
return
made by
	young
date
	2002-11-26
********************************************/
ALTER PROCEDURE [DBO].[lin_GetDefaultBuilder]
(
@account_name	nvarchar(50)
)
AS
SET NOCOUNT ON

select default_builder from account_builder (nolock) where account_name = @account_name
GO

/********************************************
lin_GetExpBySubjob
	get experience of user_subjob by subjob_id
INPUT
	@char_id		int
	@subjob_id		int
OUTPUT
return
made by
	kernel0
date
	2005-04-20
********************************************/
ALTER PROCEDURE [DBO].[lin_GetExpBySubjob]
(
	@char_id		INT,
	@subjob_id		INT
)
AS
SET NOCOUNT ON

SELECT 
	exp
FROM 
	user_subjob
WHERE 
	char_id = @char_id 
	AND subjob_id = isnull(@subjob_id, 0)
GO

/********************************************
lin_GetGroupPoint

INPUT
OUTPUT

return
made by
	young
date
	2004-07-15
********************************************/
ALTER PROCEDURE [DBO].[lin_GetGroupPoint]
(
	@group_id	int
)
AS
SET NOCOUNT ON

select sum( group_point) from event_point (nolock) where group_id = @group_id
GO

ALTER PROCEDURE dbo.lin_GetHeroByClassId
	@class_id	int
AS
SELECT un.char_id  
FROM user_nobless un, user_data ud 
WHERE un.char_id = ud.char_id 
	AND un.hero_type <> 0 
	AND ud.subjob0_class = @class_id
ORDER BY win_count DESC
GO

ALTER PROCEDURE dbo.lin_GetHeroById
(
@char_id INT
)
AS

SELECT 
udnp.char_name AS char_name, 
udnp.main_class AS main_class, 
udnp.hero_type AS hero_type, 
udnp.pledge_id AS pledge_id,
ISNULL(udnp.pledge_name, N'') AS pledge_name,
ISNULL(udnp.pledge_crest_id, 0) AS pledge_crest_id,
ISNULL(udnp.alliance_id, 0) AS alliance_id, 
ISNULL(a.name, N'') AS alliance_name,
ISNULL(a.crest_id, 0) AS alliance_crest_id, 
udnp.win_count AS win_count,
ISNULL(udnp.words, N'') AS words, 
udnp.history_open AS history_open 

FROM
	(SELECT
	udn.char_name AS char_name, 
	udn.main_class AS main_class, 
	udn.hero_type AS hero_type, 
	udn.pledge_id AS pledge_id,
	p.name AS pledge_name,
	p.crest_id AS pledge_crest_id,
	p.alliance_id AS alliance_id, 
	udn.win_count AS win_count,
	udn.words AS words, 
	udn.history_open AS history_open
 
	FROM
		(SELECT
		ud.char_name AS char_name, 
		ud.subjob0_class AS main_class, 
		un.hero_type AS hero_type, 
		ud.pledge_id AS pledge_id,
		un.win_count AS win_count,
		un.words AS words, 
		un.history_open AS history_open
 
		FROM user_data AS ud
			INNER JOIN user_nobless AS un
			ON ud.char_id = @char_id AND ud.char_id = un.char_id
		) AS udn
			LEFT OUTER JOIN pledge AS p 
			ON udn.pledge_id = p.pledge_id
	) AS udnp
		LEFT OUTER JOIN alliance AS a 
		ON udnp.alliance_id = a.id
GO

/****** Object:  Stored Procedure dbo.lin_GetInitBoard    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_GetInitBoard
	
INPUT
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_GetInitBoard]
--(
--	@account_name	nvarchar(50)
--)
AS
SET NOCOUNT ON

select 
	board_id, board_name, board_desc, board_order 
from 
	bbs_board (nolock) 
where 
	viewable = 1 
order by 
	board_order desc
GO

/********************************************
lin_GetIntervalAnnounce
	get interval announce
INPUT
	
OUTPUT
	interval
	announce id
	msg
return
made by
	carrot
date
	2003-12-19
********************************************/
ALTER PROCEDURE [DBO].[lin_GetIntervalAnnounce]
AS
SET NOCOUNT ON

select interval_10, announce_id, announce_msg from login_announce (nolock) where interval_10 > 0 order by interval_10, announce_id asc
GO

/****** Object:  Stored Procedure dbo.lin_GetItem    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_GetItem
	Get item from ground 
INPUT
	@char_id	INT,
	@item_id		 INT
OUTPUT
	item_id, amount
return
made by
	carrot
date
	2002-04-30
********************************************/
ALTER PROCEDURE [DBO].[lin_GetItem]
(
	@char_id	INT,
	@item_id		INT
)
AS
SET NOCOUNT ON
DECLARE @nCount	INT
DECLARE @nAmountIn	INT
DECLARE @nAmountOld	INT
DECLARE @nItemType	INT

SET @nCount = -1
SET @nAmountIn = -1
SET @nItemType = -1

SELECT @nAmountIn = amount, @nItemType = item_type FROM user_item WHERE item_id = @item_id
IF @nItemType = -1 
BEGIN
	RETURN
END

IF (@nItemType = 57)
	BEGIN
		SELECT @nCount = count(*) FROM user_item WHERE char_id = @char_id AND item_type = 57 AND warehouse = 0
		IF @nCount = 0
			UPDATE user_item SET char_id = @char_id  WHERE item_id = @item_id
		ELSE
		BEGIN
			UPDATE user_item SET char_id = 0, item_type = 0 WHERE item_id = @item_id
			UPDATE user_item SET amount = amount + @nAmountIn  WHERE char_id = @char_id AND item_type = 57 AND warehouse = 0
		END

		SELECT item_id, amount FROM user_item WHERE char_id = @char_id AND item_type = 57
	END
ELSE
	BEGIN
		UPDATE user_item SET char_id = @char_id  WHERE item_id = @item_id

		SELECT item_id, amount FROM user_item WHERE item_id = @item_id
	END
GO

/****** Object:  Stored Procedure dbo.lin_GetItemAmount    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_GetItemAmount
	get item amount
INPUT
	@item_type  int
OUTPUT
	
return
made by
	young
date
	2003-06-18
********************************************/
ALTER PROCEDURE [DBO].[lin_GetItemAmount]
(
	@item_type  int 
)

AS

SET NOCOUNT ON

if exists(select  * from  itemdata (nolock) where id = @item_type and consumetype = 'consume_type_normal' ) 
begin
	select count(*) as count from user_item (nolock) where item_type = @item_type
end
else
begin
	select -1 as count
end
GO

/****** Object:  Stored Procedure dbo.lin_GetItemData    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_GetItemData
	
INPUT
	@id	int
OUTPUT
return
made by
	carrot
date
	2002-06-10
********************************************/
ALTER PROCEDURE [DBO].[lin_GetItemData]
(
	@id	int
)
AS
SET NOCOUNT ON

select top 1 consumetype from itemdata  (nolock)  where id = @id
GO

/********************************************
lin_GetItemType
	Get item type
INPUT
	@item_id		 INT
OUTPUT

return
made by
	young
date
	2004-01-09
********************************************/
ALTER PROCEDURE [DBO].[lin_GetItemType]
(
	@item_id		INT
)
AS

SET NOCOUNT ON

select item_id, char_id, item_type, amount, warehouse from user_item (nolock) where item_id = @item_id
GO

/****** Object:  Stored Procedure dbo.lin_GetListBookMark    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_GetListBookMark
	get ths list of  bookmark item
INPUT
	@char_id	int
OUTPUT

return
made by
	young
date
	2002-11-13
********************************************/
ALTER PROCEDURE [DBO].[lin_GetListBookMark]
(
	@char_id	int
)
AS
SET NOCOUNT ON

select name from bookmark (nolock) where char_id = @char_id order by name
GO

/****** Object:  Stored Procedure dbo.lin_GetLoginAnnounce    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_GetLoginAnnounce
	get login announce
INPUT
	
OUTPUT
	
return
made by
	young
date
	2002-11-30
change 	2003-12-18	carrot
	add interval_10 check
********************************************/
ALTER PROCEDURE [DBO].[lin_GetLoginAnnounce]

AS
SET NOCOUNT ON

select announce_id, announce_msg from login_announce (nolock) where interval_10 = 0 order by announce_id asc
GO

/********************************************
lin_GetMacro
	get macro
INPUT
	@char_id		int,
OUTPUT
return
made by
	young
date
	2004-6-11
********************************************/
ALTER PROCEDURE [DBO].[lin_GetMacro]
(
@char_id		int
)
AS
SET NOCOUNT ON

select R1.macro_id,  char_id, macro_name, macro_tooltip, macro_iconname, macro_icontype, 
macro_order, macro_int1, macro_int2, macro_int3, macro_str from (
select * from user_macro  where char_id = @char_id ) as R1
left join ( select * from user_macroinfo ) as R2
on R1.macro_id = R2.macro_id
order by R1.macro_id asc ,   macro_order asc
GO

/********************************************
lin_GetMacroCount
	get macro coubt
INPUT
	@char_id		int,
OUTPUT
return
made by
	young
date
	2004-6-14
********************************************/
ALTER PROCEDURE [DBO].[lin_GetMacroCount]
(
@char_id		int
)
AS
SET NOCOUNT ON

select count(*) from user_macro where char_id = @char_id
GO

/********************************************
lin_GetMail
	get mail 
INPUT
	@char_id		int,
	@mail_id		int
OUTPUT
return
made by
	kks
date
	2004-12-10
********************************************/
ALTER PROCEDURE [DBO].[lin_GetMail]
(
	@char_id		int,
	@mail_id		int
)
AS
SET NOCOUNT ON

UPDATE user_mail_receiver
SET read_date = GETDATE(),
read_status = 1
WHERE mail_id = @mail_id
AND receiver_id = @char_id
AND read_status = 0
AND deleted = 0

SELECT
	m.id, s.sender_id, s.sender_name, s.receiver_name_list, m.title, m.content, datediff( ss, '1970/1/1 0:0:0' , s.send_date ), s.mail_type
FROM user_mail m (nolock), user_mail_sender s(nolock)
WHERE m.id = @mail_id
	AND s.mail_id = m.id

/*AND (m.id IN (SELECT mail_id FROM user_mail_receiver(nolock) WHERE receiver_id = @char_id)
OR m.id IN (SELECT mail_id FROM user_mail_sender(nolock) WHERE sender_id = @char_id))
*/
GO

/********************************************
lin_GetMailCount
	get mail count
INPUT
	@char_id		int,

OUTPUT
return
made by
	kks
date
	2004-12-17
********************************************/
ALTER PROCEDURE [DBO].[lin_GetMailCount]
(
	@char_id		int
)
AS
SET NOCOUNT ON

SELECT
	(SELECT COUNT(*) 
	FROM user_mail_receiver(NOLOCK)
	WHERE receiver_id = @char_id
		AND deleted = 0
		AND mailbox_type = 0) incomming_mail,
	(SELECT COUNT(*)
	FROM user_mail_sender(NOLOCK)
	WHERE sender_id = @char_id
		AND deleted = 0
		AND mailbox_type = 1) sent_mail,
	(SELECT COUNT(*) 
	FROM user_mail_receiver(NOLOCK)
	WHERE receiver_id = @char_id
		AND deleted = 0
		AND mailbox_type = 2)
	+
	(SELECT COUNT(*)
	FROM user_mail_sender(NOLOCK)
	WHERE sender_id = @char_id
		AND deleted = 0
		AND mailbox_type = 2) archived_mail,
	(SELECT COUNT(*)
	FROM user_mail_sender(NOLOCK)
	WHERE sender_id = @char_id
		AND deleted = 0
		AND mailbox_type = 3) temp_mail
GO

/********************************************
lin_GetMailList
	get mail list
INPUT
	@char_id		int,
	@mailbox_type			int,
	@page			int,
	@rows_per_page	int,
	@search_target		int,
	@keyword		nvarchar(20)
OUTPUT
return
made by
	kks
date
	2004-12-10
********************************************/
ALTER PROCEDURE [DBO].[lin_GetMailList]
(
	@char_id		int,
	@mailbox_type			int,
	@page			int,
	@rows_per_page	int,
	@search_target		int,
	@keyword		nvarchar(20)
)
AS
SET NOCOUNT ON

DECLARE @sql NVARCHAR(4000)
DECLARE @top int
DECLARE @total_count int
SET @total_count = 0

IF @page < 1
	SET @page = 1

SET @top = @page * @rows_per_page

IF @top < 1 
	RETURN

-- set search condition
DECLARE @search_condition NVARCHAR(1000)
SET @search_condition = N''

-- incomming mailbox
if (@mailbox_type = 0)
begin
	IF (@keyword != N'') 
	BEGIN
		IF (@search_target = 0)
			SET @search_condition = 'and r.mail_id IN (SELECT mail_id FROM user_mail_sender(nolock) WHERE sender_name = N''' + @keyword + ''') '
		IF (@search_target = 1)
			SET @search_condition = 'and m.title LIKE N''%' + @keyword + '%'' '
	END

	SET @sql = 
	'DECLARE @total_count int ' +
	'SET @total_count = 0 ' +
	'SELECT @total_count = COUNT(*) ' + 
	'FROM user_mail m(nolock), user_mail_receiver r(nolock) ' +
	'WHERE m.id = r.mail_id ' +
	'	and r.receiver_id = ' + CAST(@char_id as NVARCHAR) + ' ' +
	'	and r.deleted = 0 ' +
	'	and r.mailbox_type = ' + CAST(@mailbox_type as NVARCHAR) + ' ' +
	@search_condition +

	'SELECT TOP ' + CAST(@top as NVARCHAR) +  
	'	m.id, s.sender_id, s.sender_name, m.title, datediff( ss, ''1970/1/1 0:0:0'' , s.send_date ) s_date, ' +
	'	r.read_status, @total_count ' +
	'FROM user_mail m(nolock), user_mail_sender s(nolock), user_mail_receiver r(nolock) ' +
	'WHERE m.id = r.mail_id ' +
	'	and s.mail_id = r.mail_id ' +
	'	and r.receiver_id = ' + CAST(@char_id as NVARCHAR) + ' ' +
	'	and r.deleted = 0 ' +
	'	and r.mailbox_type = ' + CAST(@mailbox_type as NVARCHAR) + ' ' +
	@search_condition +
	'ORDER BY s_date DESC'

	EXEC(@sql)
end

-- sent mailbox
if (@mailbox_type = 1)
begin
	IF (@keyword != N'') 
	BEGIN
		IF (@search_target = 0)
			SET @search_condition = 'and s.mail_id IN (SELECT mail_id FROM user_mail_receiver(nolock) WHERE receiver_name = N''' + @keyword + ''') '
		IF (@search_target = 1)
			SET @search_condition = 'and m.title LIKE N''%' + @keyword + '%'' '
	END

	SET @sql = 
	'DECLARE @total_count int ' +
	'SET @total_count = 0 ' +
	'SELECT @total_count = COUNT(*) ' + 
	'FROM user_mail m(nolock), user_mail_sender s(nolock) ' +
	'WHERE m.id = s.mail_id ' +
	'	and s.sender_id = ' + CAST(@char_id as NVARCHAR) + ' ' +
	'	and s.deleted = 0 ' +
	'	and s.mailbox_type = ' + CAST(@mailbox_type as NVARCHAR) + ' ' +
	@search_condition +

	'SELECT TOP ' + CAST(@top as NVARCHAR) +  
	'	m.id, s.sender_id, s.receiver_name_list, m.title, datediff( ss, ''1970/1/1 0:0:0'' , s.send_date ) s_date, ' +
	'	s.mail_type, @total_count ' +
	'FROM user_mail m(nolock), user_mail_sender s(nolock) ' +
	'WHERE m.id = s.mail_id ' +
	'	and s.sender_id = ' + CAST(@char_id as NVARCHAR) + ' ' +
	'	and s.deleted = 0 ' +
	'	and s.mailbox_type = ' + CAST(@mailbox_type as NVARCHAR) + ' ' +
	@search_condition +
	'ORDER BY s_date DESC'

	EXEC(@sql)
end

-- archive mailbox
if (@mailbox_type = 2)
begin
	-- 
	DECLARE @search_condition2 NVARCHAR(128)
	SET @search_condition2 = N''

	IF (@keyword != N'') 
	BEGIN
		IF (@search_target = 0)
		BEGIN
			SET @search_condition = 'and r.mail_id IN (SELECT mail_id FROM user_mail_sender(nolock) WHERE sender_name = N''' + @keyword + ''') '
			SET @search_condition2 = 'and s.mail_id IN (SELECT mail_id FROM user_mail_receiver(nolock) WHERE receiver_name = N''' + @keyword + ''') '
		END

		IF (@search_target = 1)
		BEGIN
			SET @search_condition = 'and m.title LIKE N''%' + @keyword + '%'' '
			SET @search_condition2 = 'and m.title LIKE N''%' + @keyword + '%'' '
		END
	END

	SET @sql = 
	'DECLARE @total_count int ' +
	'SET @total_count = 0 ' +
	'SELECT @total_count = ' + 
	'	(SELECT COUNT(*) cnt1' +
	'	FROM user_mail m(nolock), user_mail_receiver r(nolock) ' +
	'	WHERE m.id = r.mail_id ' +
	'		and r.receiver_id = ' + CAST(@char_id as NVARCHAR) + ' ' +
	'		and r.deleted = 0 ' +
	'		and r.mailbox_type = ' + CAST(@mailbox_type as NVARCHAR) + ' ' +
	@search_condition +
	'	) + ' +
	'	(SELECT COUNT(*) cnt2' +
	'	FROM user_mail m(nolock), user_mail_sender s(nolock) ' +
	'	WHERE m.id = s.mail_id ' +
	'		and s.sender_id = ' + CAST(@char_id as NVARCHAR) + ' ' +
	'		and s.deleted = 0 ' +
	'		and s.mailbox_type = ' + CAST(@mailbox_type as NVARCHAR) + ' ' +
	@search_condition2 +
	'	) ' +
	' ' +
	'SELECT TOP ' + CAST(@top as NVARCHAR) + ' *, @total_count ' +
	'FROM ( ' +
	'	SELECT ' +
	'		m.id, s.sender_id, s.sender_name, m.title, datediff( ss, ''1970/1/1 0:0:0'' , s.send_date ) s_date, ' +
	'		r.read_status type_flag ' +
	'	FROM user_mail m(nolock), user_mail_sender s(nolock), user_mail_receiver r(nolock) ' +
	'	WHERE m.id = r.mail_id ' +
	'		and s.mail_id = r.mail_id ' +
	'		and r.receiver_id = ' + CAST(@char_id as NVARCHAR) + ' ' +
	'		and r.deleted = 0 ' +
	'		and r.mailbox_type = ' + CAST(@mailbox_type as NVARCHAR) + ' ' +
	@search_condition +
	'	union all ' +
	'	SELECT ' +
	'		m.id, s.sender_id, s.receiver_name_list, m.title, datediff( ss, ''1970/1/1 0:0:0'' , s.send_date ) s_date, ' +
	'		s.mail_type type_flag ' +
	'	FROM user_mail m(nolock), user_mail_sender s(nolock) ' +
	'	WHERE m.id = s.mail_id ' +
	'		and s.sender_id = ' + CAST(@char_id as NVARCHAR) + ' ' +
	'		and s.deleted = 0 ' +
	'		and s.mailbox_type = ' + CAST(@mailbox_type as NVARCHAR) + ' ' +
	@search_condition2 +
	') as R1 ' +
	'	ORDER BY s_date DESC'
		
	EXEC(@sql)

end

-- temp mailbox
if (@mailbox_type = 3)
begin
	IF (@keyword != N'') 
	BEGIN
		IF (@search_target = 0)
			SET @search_condition = 'and s.receiver_name_list LIKE  N''%' + @keyword + '%'' '
		IF (@search_target = 1)
			SET @search_condition = 'and m.title LIKE N''%' + @keyword + '%'' '
	END

	SET @sql = 
	'DECLARE @total_count int ' +
	'SET @total_count = 0 ' +
	'SELECT @total_count = COUNT(*) ' + 
	'FROM user_mail m(nolock), user_mail_sender s(nolock) ' +
	'WHERE m.id = s.mail_id ' +
	'	and s.sender_id = ' + CAST(@char_id as NVARCHAR) + ' ' +
	'	and s.deleted = 0 ' +
	'	and s.mailbox_type = ' + CAST(@mailbox_type as NVARCHAR) + ' ' +
	@search_condition +
	
	'SELECT TOP ' + CAST(@top as NVARCHAR) +  
	'	m.id, s.sender_id, s.receiver_name_list, m.title, datediff( ss, ''1970/1/1 0:0:0'' , s.send_date ) s_date, ' +
	'	s.mail_type, @total_count ' +
	'FROM user_mail m(nolock), user_mail_sender s(nolock) ' +
	'WHERE m.id = s.mail_id ' +
	'	and s.sender_id = ' + CAST(@char_id as NVARCHAR) + ' ' +
	'	and s.deleted = 0 ' +
	'	and s.mailbox_type = ' + CAST(@mailbox_type as NVARCHAR) + ' ' +
	@search_condition +
	'ORDER BY s_date DESC'

	EXEC(@sql)

end
GO

/********************************************
lin_GetMonRaceBet
	
INPUT
	@bet_type		int
	@race_id		int
OUTPUT
return
made by
	young
date
	2003-05-31
********************************************/
ALTER PROCEDURE [DBO].[lin_GetMonRaceBet]
(
	@bet_type		int,
	@race_id		int
)
AS
SET NOCOUNT ON

if ( @bet_type = 1 )
begin

	select bet_1, sum( bet_money) from monrace_ticket (nolock) 
	where monraceid = @race_id
	and bet_type = 1 and deleted = 0
	group by bet_1

end else begin

	select bet_1, bet_2 , sum( bet_money ) from monrace_ticket (nolock) 
	where monraceid = @race_id
	and bet_type = 2 and deleted = 0
	group by bet_1, bet_2

end
GO

/********************************************  
lin_GetMonRaceResult  
   
INPUT  
 @nPage  int,
 @nLinePerPage int
OUTPUT  
return  
made by  
 young  
date  
 2003-06-15  
change	
 2004-12-24 carrot
 change top 2000 to get by page number
 2005-02-22 kks
 fix column mismatch & page default
********************************************/  
ALTER    PROCEDURE [DBO].[lin_GetMonRaceResult]  
(  
 @nPage  int,
 @nLinePerPage int
)  
AS  
SET NOCOUNT ON  
  
--select top 2000 race_id, lane1, lane2, lane3, lane4, lane5, lane6, lane7, lane8, win1, win2, winrate1, winrate2, race_end from  
--monrace (nolock)  
--order by race_id desc  
 
IF (@nPage <= 0)
	SET @nPage = 1

DECLARE @nMaxRaceId int  
SET @nMaxRaceId = 0
  
select top 1 @nMaxRaceId = race_id from monrace order by race_id desc  

select race_id, lane1, lane2, lane3, lane4, lane5, lane6, lane7, lane8, win1, win2, winrate1, winrate2, race_end 
from monrace where race_id <= (@nMaxRaceId - (@nPage - 1) * @nLinePerPage) and race_id > (@nMaxRaceId - @nPage * @nLinePerPage)
order by race_id desc
GO

/********************************************
lin_GetMonRaceTaxSum
	
INPUT
	@race_id		int
OUTPUT
return
made by
	young
date
	2004-08-05
********************************************/
ALTER  PROCEDURE [DBO].[lin_GetMonRaceTaxSum]
(
	@race_id		int
)
AS
SET NOCOUNT ON

select isnull( sum ( tax_money) , 0) from monrace_ticket (nolock) where monraceid = @race_id and deleted = 0
GO

/********************************************
lin_GetMonRaceTicket
	
INPUT
	@item_id		int
OUTPUT
return
made by
	young
date
	2003-06-10
********************************************/
ALTER  PROCEDURE [DBO].[lin_GetMonRaceTicket]
(
	@item_id		int
)
AS
SET NOCOUNT ON

declare @ticket_id	int
declare @monraceid	int
declare @bet_type	int
declare @bet_1		int
declare @bet_2		int
declare @bet_3		int
declare @bet_money	int
declare @winrate1	float
declare @winrate2	float
declare @win1		int
declare @win2		int
declare @race_end	int
declare @tax_money	int
declare @remotefee	int

select @ticket_id = ticket_id, @monraceid = monraceid, @bet_type=bet_type, @bet_1 = bet_1, @bet_2 = bet_2 , @bet_3 = bet_3, @bet_money = bet_money  , @tax_money = tax_money , @remotefee = remotefee from monrace_ticket where item_id = @item_id and deleted = 0
select @winrate1=winrate1, @winrate2=winrate2, @win1 = win1, @win2=win2 , @race_end = race_end  from monrace where race_id = @monraceid

select @ticket_id, @monraceid, @bet_type, @bet_1, @bet_2, @bet_3, @bet_money, @winrate1, @winrate2, @win1, @win2, @race_end, @tax_money, @remotefee
GO

ALTER PROCEDURE dbo.lin_GetNoblessAchievements
(
@char_id INT
)
AS
SELECT win_type, target, win_time FROM nobless_achievements WHERE char_id = @char_id ORDER BY win_time DESC
GO

ALTER PROCEDURE dbo.lin_GetNoblessById
(
@char_id AS INT
)
AS
SELECT nobless_type, hero_type, win_count, previous_point, olympiad_point, match_count, olympiad_win_count, olympiad_lose_count, history_open FROM user_nobless WHERE char_id = @char_id
GO

ALTER PROCEDURE dbo.lin_GetNoblessTop10
(
@class INT,
@season INT
)
AS  
SET NOCOUNT ON  

SELECT TOP 10 WITH TIES u.char_id, u.char_name, o.point, o.match_count 
FROM user_data u, olympiad_result o 
WHERE o.class = @class AND u.char_id = o.char_id AND o.season = @season
ORDER BY point DESC
GO

/********************************************
lin_GetOneMacro
	get one macro
INPUT
	@char_id		int,
	@macro_number	int
OUTPUT
return
made by
	young
date
	2004-6-11
********************************************/
ALTER PROCEDURE [DBO].[lin_GetOneMacro]
(
@char_id		int,
@macro_id	int
)
AS
SET NOCOUNT ON

select R1.macro_id,  char_id, macro_name, macro_tooltip, macro_iconname, macro_icontype, 
macro_order, macro_int1, macro_int2, macro_int3, macro_str from (
select * from user_macro  where char_id = @char_id and macro_id = @macro_id ) as R1
left join ( select * from user_macroinfo where macro_id = @macro_id  ) as R2
on R1.macro_id = R2.macro_id
 order by macro_order asc
GO

/****** Object:  Stored Procedure dbo.lin_GetPetitionMsg    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_GetPetitionMsg
	get Petition Msg and delete it
INPUT
	@char_id	int
OUTPUT
	msg(nvarchar 500)
return
made by
	carrot
date
	2003-02-27
********************************************/
ALTER PROCEDURE [DBO].[lin_GetPetitionMsg]
(
	@char_id	int
)
AS
SET NOCOUNT ON

select ISNULL(msg, '') as msg from PetitionMsg where char_id = @char_id
delete PetitionMsg where char_id = @char_id
GO

/********************************************
lin_GetPledgeAdena
	Get pledge adena
INPUT
	@pledge_id	INT
OUTPUT
return
made by
	young
date
	2003-12-11
********************************************/
ALTER PROCEDURE [DBO].[lin_GetPledgeAdena ]
(
	@pledge_id	INT
)

as

set nocount on

select isnull( sum(amount) , 0) from user_item (nolock) where warehouse = 2 and char_id = @pledge_id and item_type = 57
GO

ALTER PROCEDURE dbo.lin_GetPledgeKillDeathCount
(
@castle_id INT
)
AS
SET NOCOUNT ON

SELECT pledge_id, siege_kill, siege_death FROM pledge 
WHERE pledge_id IN 
(SELECT pledge_id FROM castle_war WHERE castle_id = @castle_id AND (type = 1 OR type = 2))
GO

/****** Object:  Stored Procedure dbo.lin_GetPunish    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_GetPunish
	Get punish
INPUT
	@char_id	INT
OUTPUT
return
made by
	young
date
	2002-11-27
********************************************/
ALTER PROCEDURE [DBO].[lin_GetPunish]
(
	@char_id	INT
)

as

set nocount on

select punish_id, punish_on, remain_game, remain_real from user_punish (nolock) where char_id = @char_id
GO

/****** Object:  Stored Procedure dbo.lin_GetRelatedCastleSiege    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_GetRelatedCastleSiege
	
INPUT	
	@castle_id	int,
	@pledge_id	int
OUTPUT
	id, 
	next_war_time, 
	type 
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_GetRelatedCastleSiege]
(
	@castle_id	int,
	@pledge_id	int
)
AS
SET NOCOUNT ON

SELECT 
	c.id, 
	c.next_war_time, 
	cw.type 
FROM 
	castle c (nolock) , 
	castle_war cw (nolock)  
WHERE 
	c.id = cw.castle_id 
	AND c.next_war_time <> 0 
	AND c.id = @castle_id
	AND cw.pledge_id = @pledge_id
GO

/********************************************
lin_GetSendMailCount
	get send mail count
INPUT
	@char_id		int

OUTPUT
return
made by
	kks
date
	2004-12-23
********************************************/
ALTER PROCEDURE [DBO].[lin_GetSendMailCount]
(
	@char_id		int
)
AS
SET NOCOUNT ON

SELECT COUNT(*)
FROM user_mail_sender(NOLOCK)
WHERE sender_id IN (SELECT char_id FROM user_data(nolock) WHERE account_id = (SELECT account_id FROM user_data (nolock) WHERE char_id = @char_id))
	AND mailbox_type = 1
	AND send_date BETWEEN CONVERT(DATETIME, CONVERT(NVARCHAR(10), GETDATE(), 120) + ' 00:00:00') AND
		CONVERT(DATETIME, CONVERT(NVARCHAR(10), GETDATE(), 120) + ' 23:59:59')
GO

ALTER  PROCEDURE  dbo.lin_GetShortCut
(  
@char_id  INT,
@subjob_id INT
)  
AS  
SET NOCOUNT ON  
  
SELECT slotnum, shortcut_type, shortcut_id, shortcut_macro
FROM shortcut_data (nolock)
WHERE char_id = @char_id AND subjob_id = @subjob_id
ORDER BY slotnum
GO

/****** Object:  Stored Procedure dbo.lin_GetSiegeRelatedAlliancePledge    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_GetSiegeRelatedAlliancePledge
	
INPUT	
	@alliance_id	int

OUTPUT
	castle_id,
	pledge_id, 
	type 
return
made by
	bert
********************************************/
ALTER PROCEDURE [DBO].[lin_GetSiegeRelatedAlliancePledge]
(
	@alliance_id	int
)
AS
SET NOCOUNT ON

SELECT castle_id, pledge_id, type 
FROM castle_war
WHERE pledge_id IN (SELECT pledge_id FROM pledge WHERE alliance_id = @alliance_id)
GO

/********************************************
lin_GetMainEventRecord
	get ssq main event record
INPUT

OUTPUT

return
made by
	kks
date
	2005-02-23
********************************************/
ALTER PROCEDURE [DBO].[lin_GetSSQMainEventRecord]

AS

SET NOCOUNT ON

select ssq_round, room_no, record_type, point, record_time, elapsed_time, member_count, member_names, datediff(s, '1970-01-01', getutcdate())
from time_attack_record
where record_type > 0
and ssq_round = (select max(round_number) max_ssq_round from ssq_data)
order by record_type asc, room_no desc
GO

/********************************************
lin_GetSSQStatus
	get ssq status
INPUT

OUTPUT

return
made by
	kks
date
	2005-02-23
changed by
	carrot
date
	2005-03-08
changed by 
	kks
date	
	2005-03-15
changed by 
	kks
date	2005-03-21
changed by 
	kks
date	2005-03-23
********************************************/
ALTER PROCEDURE [DBO].[lin_GetSSQStatus]

AS

SET NOCOUNT ON

declare @dawn_collected_point int
declare @dawn_main_event_point int
declare @twilight_collected_point int
declare @twilight_main_event_point int
declare @current_round_number int

select  @current_round_number = max(round_number) from ssq_data

select @dawn_collected_point = case when dawn_point + twilight_point = 0 then 0 else  ((convert(float, dawn_point) / (dawn_point + twilight_point)) * 500) end, 
@twilight_collected_point = case when dawn_point + twilight_point = 0 then 0 else ((convert(float, twilight_point) / (dawn_point + twilight_point)) * 500) end
from
(
select sum(dawn) dawn_point, sum(twilight) twilight_point
from
(select 
case when type = 1 then sum(collected_point) else 0 end twilight, 
case when type = 2 then sum(collected_point) else 0 end dawn
from ssq_join_data
where round_number = @current_round_number
group by type
) as a
) as b

select @dawn_main_event_point = sum(dawn), @twilight_main_event_point = sum(twilight)
from 
(
select 
case when ssq_part = 1 then sum(point) else 0 end twilight,
case when ssq_part = 2 then sum(point) else 0 end dawn
from
(
select 
case 
when sum_point > 0 then 1	-- if sum(point) is positive, twilight wins!!!
when sum_point < 0 then 2	-- if sum(point) is negative, dawn wins!!!
else 0 end ssq_part,		-- even, none wins
case when room_no = 1 then 60 
when room_no = 2 then 70
when room_no = 3 then 100
when room_no = 4 then 120
when room_no = 5 then 150
else 0 end point
from
(
select room_no, sum(point) sum_point -- twilight point + dawn point
from
(
select room_no,
case
when ssq_part = 1 then point 	-- twilight point
when ssq_part = 2 then -point	-- dawn point
else 0 end point
from time_attack_record
where record_type > 0
and ssq_round = @current_round_number
) as x
group by room_no
) as y
) as a
group by ssq_part
) as b

select top 1 round_number, @twilight_main_event_point + @twilight_collected_point twilight_point, @dawn_main_event_point + @dawn_collected_point dawn_point,  
seal1, seal2, seal3, seal4, seal5, seal6, seal7, datediff(s, '1970-01-01', getutcdate()), status
from ssq_data
where round_number = @current_round_number
GO

ALTER PROCEDURE dbo.lin_GetSubJobLevel
(  
 @char_id INT  
)  
AS  
 
SET NOCOUNT ON  
SELECT subjob_id, ISNULL(level, 40) FROM user_subjob WHERE char_id = @char_id ORDER BY subjob_id
GO

/****** Object:  Stored Procedure dbo.Lin_GetSummary    Script Date: 2003-09-20 ?? 11:51:56 ******/
ALTER PROCEDURE [DBO].[Lin_GetSummary] 
AS

SET NOCOUNT ON

Select count(id) as Total from BBS_All
GO

/********************************************
lin_GetSurrenderWarId

INPUT
	@char_id	INT
OUTPUT
return
made by
	bert
date
	2003-10-07
********************************************/
ALTER PROCEDURE [DBO].[lin_GetSurrenderWarId]
(
@char_id	INT
)
AS
SET NOCOUNT ON


SELECT char_id, surrender_war_id FROM user_surrender us, pledge_war pw WHERE us.char_id = @char_id AND us.surrender_war_id = pw.id AND pw.status = 0
GO

/********************************************
lin_GetTempMail
	get temp mail 
INPUT
	@char_id		int,
	@mail_id		int
OUTPUT
return
made by
	kks
date
	2004-12-19
********************************************/
ALTER PROCEDURE [DBO].[lin_GetTempMail]
(
	@char_id		int,
	@mail_id		int
)
AS
SET NOCOUNT ON

SELECT
	m.id, s.receiver_name_list, m.title, m.content
FROM user_mail m (nolock), user_mail_sender s(nolock)
WHERE m.id = @mail_id
	AND s.mail_id = m.id

/*AND (m.id IN (SELECT mail_id FROM user_mail_receiver(nolock) WHERE receiver_id = @char_id)
OR m.id IN (SELECT mail_id FROM user_mail_sender(nolock) WHERE sender_id = @char_id))
*/
GO

/********************************************
lin_GetUnreadMailCount
	get unread mail count
INPUT
	@char_id		int,

OUTPUT
return
made by
	kks
date
	2004-12-23
********************************************/
ALTER PROCEDURE [DBO].[lin_GetUnreadMailCount]
(
	@char_id		int
)
AS
SET NOCOUNT ON

SELECT COUNT(*)
FROM user_mail_receiver(NOLOCK)
WHERE receiver_id = @char_id
	AND mailbox_type = 0
	AND read_status = 0;
GO

ALTER    PROCEDURE dbo.lin_GetUserDataByCharId
(      
@char_id INT      
)      
AS      
SET NOCOUNT ON      
      
IF @char_id > 0      
SELECT       
 RTRIM(ud.char_name), ud.char_id, RTRIM(ud.account_name), ud.account_id, ud.pledge_id, ud.builder, ud.gender, ud.race, ud.class,       
 ud.world, ud.xloc, ud.yloc, ud.zloc, ud.IsInVehicle, ud.HP, ud.MP, ud.Max_HP, ud.Max_MP, ud.CP, ud.Max_CP, ud.SP, ud.Exp, ud.Lev, ud.align, ud.PK, ud.duel, ud.pkpardon,       
 ud.ST_underware, ud.ST_right_ear, ud.ST_left_ear, ud.ST_neck, ud.ST_right_finger, ud.ST_left_finger, ud.ST_head, ud.ST_right_hand, ud.ST_left_hand, ud.ST_gloves, ud.ST_chest, ud.ST_legs, ud.ST_feet, ud.ST_back, ud.ST_both_hand, ud.ST_hair,  
 ISNULL(YEAR(temp_delete_date),0), ISNULL(MONTH(temp_delete_date),0), ISNULL(DAY(temp_delete_date),0),  
 ISNULL(DATEPART(HOUR, temp_delete_date),0), ISNULL(DATEPART(mi, temp_delete_date),0), ISNULL(DATEPART(s, temp_delete_date),0),  
 ISNULL(uas.s1, 0), ISNULL(uas.l1, 0), ISNULL(uas.d1, 0), ISNULL(uas.c1, 0),       
 ISNULL(uas.s2, 0), ISNULL(uas.l2, 0), ISNULL(uas.d2, 0), ISNULL(uas.c2, 0),       
 ISNULL(uas.s3, 0), ISNULL(uas.l3, 0), ISNULL(uas.d3, 0), ISNULL(uas.c3, 0),       
 ISNULL(uas.s4, 0), ISNULL(uas.l4, 0), ISNULL(uas.d4, 0), ISNULL(uas.c4, 0),       
 ISNULL(uas.s5, 0), ISNULL(uas.l5, 0), ISNULL(uas.d5, 0), ISNULL(uas.c5, 0),       
 ISNULL(uas.s6, 0), ISNULL(uas.l6, 0), ISNULL(uas.d6, 0), ISNULL(uas.c6, 0),       
 ISNULL(uas.s7, 0), ISNULL(uas.l7, 0), ISNULL(uas.d7, 0), ISNULL(uas.c7, 0),       
 ISNULL(uas.s8, 0), ISNULL(uas.l8, 0), ISNULL(uas.d8, 0), ISNULL(uas.c8, 0),       
 ISNULL(uas.s9, 0), ISNULL(uas.l9, 0), ISNULL(uas.d9, 0), ISNULL(uas.c9, 0),       
 ISNULL(uas.s10, 0), ISNULL(uas.l10, 0), ISNULL(uas.d10, 0), ISNULL(uas.c10, 0),       
 ISNULL(uas.s11, 0), ISNULL(uas.l11, 0), ISNULL(uas.d11, 0), ISNULL(uas.c11, 0),       
 ISNULL(uas.s12, 0), ISNULL(uas.l12, 0), ISNULL(uas.d12, 0), ISNULL(uas.c12, 0),       
 ISNULL(uas.s13, 0), ISNULL(uas.l13, 0), ISNULL(uas.d13, 0), ISNULL(uas.c13, 0),       
 ISNULL(uas.s14, 0), ISNULL(uas.l14, 0), ISNULL(uas.d14, 0), ISNULL(uas.c14, 0),       
 ISNULL(uas.s15, 0), ISNULL(uas.l15, 0), ISNULL(uas.d15, 0), ISNULL(uas.c15, 0),       
 ISNULL(uas.s16, 0), ISNULL(uas.l16, 0), ISNULL(uas.d16, 0), ISNULL(uas.c16, 0),       
 ISNULL(uas.s17, 0), ISNULL(uas.l17, 0), ISNULL(uas.d17, 0), ISNULL(uas.c17, 0),       
 ISNULL(uas.s18, 0), ISNULL(uas.l18, 0), ISNULL(uas.d18, 0), ISNULL(uas.c18, 0),       
 ISNULL(uas.s19, 0), ISNULL(uas.l19, 0), ISNULL(uas.d19, 0), ISNULL(uas.c19, 0),     
 ISNULL(uas.s20, 0), ISNULL(uas.l20, 0), ISNULL(uas.d20, 0), ISNULL(uas.c20, 0),       
 ud.quest_flag, ud.face_index, ud.hair_shape_index, ud.hair_color_index,       
 ud.nickname, ud.power_flag, ud.pledge_dismiss_time, ud.pledge_ousted_time, ud.pledge_withdraw_time, ud.surrender_war_id, ud.use_time, ud.drop_exp,
 ISNULL( ub.status, 0), ISNULL( ub.ban_end , 0), ISNULL(ud.subjob_id , 0), 
 ud.subjob0_class, ud.subjob1_class, ud.subjob2_class, ud.subjob3_class, ISNULL(ssq_dawn_round, 0),
 ISNULL(uun.color_rgb, 0x00ffffff)  
FROM        
 (SELECT * FROM User_data (nolock) WHERE char_id = @char_id) AS ud       
 LEFT OUTER JOIN       
 (SELECT * FROM user_ActiveSkill (nolock) WHERE char_id = @char_id) as uas ON ud.char_id = uas.char_id
 left  OUTER JOIN
 (select * from user_ban (nolock) where char_id = @char_id) as ub on ud.char_id = ub.char_id  
 left OUTER JOIN
 (select * from user_name_color (nolock) where char_id = @char_id) as uun on ud.char_id = uun.char_id
GO

/****** Object:  Stored Procedure dbo.lin_GetUserDataByCharName    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_GetUserDataByCharName

INPUT
	@char_name	NVARCHAR(50)
OUTPUT
	ud.char_name, ud.account_name, ud.account_id, ud.pledge_id, ud.builder, ud.gender, ud.race, ud.class, ud.world, ud.xloc, ud.yloc, ud.zloc, 
	ud.HP, ud.MP, ud.SP, ud.Exp, ud.Lev, ud.align, ud.PK, ud.Str, ud.Dex, ud.Con, ud.Int, ud.Wit, ud.Men, 
	ud.ST_underware, ud.ST_right_ear, ud.ST_left_ear, ud.ST_neck, ud.ST_right_finger, ud.ST_left_finger, ud.ST_head, ud.ST_right_hand, 
	ud.ST_left_hand, ud.ST_gloves, ud.ST_chest, ud.ST_legs, ud.ST_feet, ud.ST_back, ud.ST_both_hand,
	uas.s1, uas.l1, uas.d1, uas.s2, uas.l2, uas.d2, uas.s3, uas.l3, uas.d3, uas.s4, uas.l4, uas.d4, uas.s5, uas.l5, uas.d5, uas.s6, uas.l6, uas.d6, uas.s7, uas.l7, uas.d7, uas.s8, uas.l8, uas.d8, uas.s9, uas.l9, uas.d9, uas.s10, uas.l10, uas.d10, 
	uas.s11, uas.l11, uas.d11, uas.s12, uas.l12, uas.d12, uas.s13, uas.l13, uas.d13, uas.s14, uas.l14, uas.d14, uas.s15, uas.l15, uas.d15, uas.s16, uas.l16, uas.d16, uas.s17, uas.l17, uas.d17, uas.s18, uas.l18, uas.d18, uas.s19, uas.l19, uas.d19, uas.s20, uas.l20, uas.d20
return
made by
	carrot
date
	2002-06-09
********************************************/
ALTER PROCEDURE [DBO].[lin_GetUserDataByCharName]
(
@char_name	nvarchar(50)
)
AS
SET NOCOUNT ON

declare @char_id	INT

set @char_id = 0

select top 1 @char_id = char_id from user_data where char_name = @char_name

exec lin_GetUserDataByCharId @char_id
GO

/****** Object:  Stored Procedure dbo.lin_GetUserLogTime    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_GetUserLogTime
	get user log time
INPUT
	char_id, 
	log_id,
	log_to

OUTPUT
	time diff
return
made by
	young
date
	2003-01-22
change
********************************************/
ALTER PROCEDURE [DBO].[lin_GetUserLogTime]
(
	@char_id	INT,
	@log_id		INT,
	@log_to	INT
)
AS

SET NOCOUNT ON

DECLARE @use_time int

select 
	top 1 @use_time = use_time
from 
	user_log (nolock)
where 
	char_id = @char_id 
	and log_id = @log_id 
	and log_to = @log_to
order by 
	log_date desc

if @use_time = NULL
begin
	select @use_time = use_time
	from user_data (nolock)
	where char_id = @char_id
end

select @use_time
GO

ALTER PROCEDURE dbo.lin_GetWarDeclare
AS
SELECT challenger, challengee FROM war_declare WHERE challenger IN (SELECT pledge_id FROM pledge) AND challengee IN (SELECT pledge_id FROM pledge)
GO

/********************************************
lin_GetWearingItemList
	GetWearingItemList
INPUT
	@char_id	int,
OUTPUT
return
made by
	kks
date
	2004-02-17
********************************************/
ALTER PROCEDURE [dbo].[lin_GetWearingItemList] 
(
@char_id	int
)
AS
SET NOCOUNT ON

SELECT 'st_underware', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_underware FROM user_data WHERE char_id = @char_id) UNION
SELECT 'st_right_ear', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_right_ear FROM user_data WHERE char_id = @char_id) UNION
SELECT 'st_left_ear', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_left_ear FROM user_data WHERE char_id = @char_id) UNION
SELECT 'st_neck', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_neck FROM user_data WHERE char_id = @char_id) UNION
SELECT 'st_right_finger', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_right_finger FROM user_data WHERE char_id = @char_id) UNION
SELECT 'st_left_finger', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_left_finger FROM user_data WHERE char_id = @char_id) UNION
SELECT 'st_head', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_head FROM user_data WHERE char_id = @char_id) UNION
SELECT 'st_right_hand', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_right_hand FROM user_data WHERE char_id = @char_id) UNION
SELECT 'st_left_hand', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_left_hand FROM user_data WHERE char_id = @char_id) UNION
SELECT 'st_gloves', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_gloves FROM user_data WHERE char_id = @char_id) UNION
SELECT 'st_chest', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_chest FROM user_data WHERE char_id = @char_id) UNION
SELECT 'st_legs', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_legs FROM user_data WHERE char_id = @char_id) UNION
SELECT 'st_feet', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_feet FROM user_data WHERE char_id = @char_id) UNION
SELECT 'st_back', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_back FROM user_data WHERE char_id = @char_id) UNION
SELECT 'st_both_hand', item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = (SELECT st_both_hand FROM user_data WHERE char_id = @char_id)
GO

ALTER PROCEDURE dbo.lin_InitAllOlympiadPoint
(
@season INT,
@step INT,
@init_point INT
)
AS
SET NOCOUNT ON

UPDATE user_nobless
SET olympiad_point = @init_point

UPDATE olympiad
SET step = @step
WHERE season = @season
GO

/****** Object:  Stored Procedure dbo.lin_InitPledgeCrest    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_InitPledgeCrest
	
INPUT	
	@pledge_id	int
OUTPUT

return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_InitPledgeCrest]
(
	@pledge_id	int
)
AS
SET NOCOUNT ON

UPDATE Pledge SET crest_id = 0 WHERE pledge_id = @pledge_id	-- update tuple from pledge table
GO

/********************************************
lin_InitPledgeEmblem
	
INPUT	
	@pledge_id	int
OUTPUT

return
made by
	kks
date
	2005-07-22
********************************************/
ALTER PROCEDURE [DBO].[lin_InitPledgeEmblem]
(
	@pledge_id	int
)
AS
SET NOCOUNT ON

UPDATE Pledge SET emblem_id = 0 WHERE pledge_id = @pledge_id	-- update tuple from pledge table
GO

/********************************************
lin_InsertAgitAdena
	create agit_adena
INPUT
	

OUTPUT
return
made by
	young
date
	2003-12-1
********************************************/
ALTER PROCEDURE [DBO].[lin_InsertAgitAdena]
(
@agit_id		INT,
@pledge_id		INT,
@auction_id		INT,
@reason		INT,
@adena		INT
)
AS
SET NOCOUNT ON

insert into agit_adena ( agit_id, pledge_id, auction_id, reason, adena)
values ( @agit_id, @pledge_id, @auction_id, @reason, @adena)
GO

ALTER PROCEDURE [dbo].lin_InsertIntoControlTower
(
@name VARCHAR(256),
@residence_id INT,
@control_level INT,
@hp INT,
@status INT
)
AS
INSERT INTO control_tower
(name, residence_id, control_level, hp, status)
VALUES
(@name, @residence_id, @control_level, @hp, @status)
GO

ALTER PROCEDURE [dbo].lin_InsertIntoMercenary  
(  
@residence_id INT,  
@npc_id INT,  
@x INT,  
@y INT,  
@z INT,  
@angle INT,  
@hp INT,  
@mp INT  
)  
AS  
SET NOCOUNT ON  
  
if exists(select * from mercenary where x= @x and y = @y and  z = @z)  
begin  
 delete mercenary where x= @x and y = @y and  z = @z  
end  
  
INSERT INTO mercenary  
(residence_id, npc_id, x, y, z, angle, hp, mp)  
VALUES  
(@residence_id, @npc_id, @x, @y, @z, @angle, @hp, @mp)  
SELECT @@IDENTITY
GO

ALTER PROCEDURE [dbo].lin_InsertIntoResidenceGuard
(
@residence_id INT,
@item_id INT,
@npc_id INT,
@guard_type INT,
@can_move INT,
@x INT,
@y INT,
@z INT,
@angle INT
)
AS
INSERT INTO residence_guard
(residence_id, item_id, npc_id, guard_type, can_move, x, y, z, angle)
VALUES
(@residence_id, @item_id, @npc_id, @guard_type, @can_move, @x, @y, @z, @angle)
GO

ALTER PROCEDURE dbo.lin_InsertIntoTeamBattleAgitMember
(
@agit_id INT,
@char_id INT,
@pledge_id INT,
@propose_time INT
)
AS
SET NOCOUNT ON
INSERT INTO team_battle_agit_member
(agit_id, char_id, pledge_id, propose_time)
VALUES
(@agit_id, @char_id, @pledge_id, @propose_time)
GO

ALTER PROCEDURE dbo.lin_InsertIntoTeamBattleAgitPledge
(
@agit_id INT,
@pledge_id INT,
@propose_time INT,
@color INT,
@npc_type INT
)
AS
SET NOCOUNT ON
INSERT INTO team_battle_agit_pledge
(agit_id, pledge_id, propose_time, color, npc_type)
VALUES
(@agit_id, @pledge_id, @propose_time, @color, @npc_type)
GO

/****** Object:  Stored Procedure dbo.lin_InsertItemName    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_InsertItemName
	insert item name data
INPUT
	@id	INT,
	@name 	nvarchar(50),
	@desc 	nvarchar(50)
OUTPUT
return
made by
	carrot
date
	2002-10-8
change		carrot
	2003-1-9	add IsQuest field
********************************************/
ALTER PROCEDURE [DBO].[lin_InsertItemName]
(
@id	INT,
@name 	nvarchar(50),
@desc 	nvarchar(50),
@consumeType 	nvarchar(50),
@IsQuest	TINYINT = 0
)
AS
SET NOCOUNT ON


INSERT INTO ItemData
	(id, name, [desc], consumeType, IsQuest) 
	values 
	(@id, @name, @desc, @consumeType, @IsQuest)
GO

/****** Object:  Stored Procedure dbo.lin_InsertQuestName    Script Date: 2003-09-20 ?? 11:51:59 ******/

/********************************************
lin_InsertQuestName
	insert Quest name data
INPUT
	@id	INT,
	@name 	nvarchar(50),
	@data 	nvarchar(50)
OUTPUT
return
made by
	carrot
date
	2002-10-8
********************************************/
ALTER PROCEDURE [DBO].[lin_InsertQuestName]
(
@id	INT,
@name 	nvarchar(50),
@data 	nvarchar(50)
)
AS
SET NOCOUNT ON


INSERT INTO QuestData
	(id, name, data) 
	values 
	(@id, @name, @data)
GO

/****** Object:  Stored Procedure dbo.lin_InsertSkillName    Script Date: 2003-09-20 ?? 11:51:59 ******/

/********************************************
lin_InsertSkillName
	insert Skill name data
INPUT
	@id	INT,
	@lev 	INT,
	@name 	nvarchar(50)
	@skill_desc 	nvarchar(50),
	@magic_skill	INT,
	@activateType	nvarchar(5)
OUTPUT
return
made by
	carrot
date
	2002-10-8
********************************************/
ALTER PROCEDURE [DBO].[lin_InsertSkillName]
(
@id	INT,
@lev 	INT,
@name 	nvarchar(50),
@skill_desc 	nvarchar(50),
@magic_skill	INT,
@activateType	nvarchar(5)
)
AS
SET NOCOUNT ON


INSERT INTO skillData
	(id, lev, name, skill_desc, bIsMagic, activate_type) 
	values 
	(@id, @lev, @name, @skill_desc, @magic_skill, @activateType)
GO

/****** Object:  Stored Procedure dbo.lin_InsertUserLog    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_InsertUserLog
	add user log data
INPUT
	char_id
	log_id
OUTPUT
return
made by
	young
date
	2003-1-15
change
********************************************/
ALTER PROCEDURE [DBO].[lin_InsertUserLog]
(
	@char_id	INT,
	@log_id	TINYINT
)
AS

SET NOCOUNT ON

insert into user_log( char_id, log_id)
values (@char_id, @log_id)
GO

/****** Object:  Stored Procedure dbo.lin_InstallBattleCamp    Script Date: 2003-09-20 ?? 11:51:59 ******/
-- lin_InstallBattleCamp
-- by bert
-- return new battle camp id

ALTER PROCEDURE dbo.lin_InstallBattleCamp (@pledge_id INT, @castle_id INT, @max_hp INT, @hp INT, @x INT, @y INT, @z INT, @type INT)
AS

SET NOCOUNT ON

INSERT INTO object_data
(owner_id, residence_id, max_hp, hp, x_pos, y_pos, z_pos, type)
VALUES 
(@pledge_id, @castle_id, @max_hp, @hp, @x, @y, @z, @type)

SELECT @@IDENTITY
GO

/****** Object:  Stored Procedure dbo.lin_JoinAlliance    Script Date: 2003-09-20 ?? 11:51:59 ******/
-- lin_JoinAlliance
-- by bert
-- return Result(0 if failed)

ALTER PROCEDURE
dbo.lin_JoinAlliance (@alliance_id INT, @member_pledge_id INT)
AS

SET NOCOUNT ON

DECLARE @result INT

UPDATE pledge
SET alliance_id = @alliance_id
WHERE pledge_id = @member_pledge_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @result = 1
END
ELSE
BEGIN
	SELECT @result = 0
END

SELECT @result
GO

/****** Object:  Stored Procedure dbo.lin_JoinPledgeMember    Script Date: 2003-09-20 ?? 11:51:57 ******/
-- lin_JoinPledgeMember
-- by bert

ALTER PROCEDURE
dbo.lin_JoinPledgeMember (@pledge_id INT, @member_id INT)
AS

SET NOCOUNT ON

DECLARE @ret INT

BEGIN TRAN

UPDATE user_data
SET pledge_id = @pledge_id
WHERE char_id = @member_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	-- ???? ??? ???
	SELECT @ret = 1
END
ELSE
BEGIN
	SELECT @ret = 0
	GOTO EXIT_TRAN
END

EXIT_TRAN:
IF @ret<> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END
SELECT @ret
GO

/****** Object:  Stored Procedure dbo.lin_LoadAccount    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_LoadAccount
	
INPUT
	@account_id	int
OUTPUT
return
made by
	carrot
date
	2002-06-09
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadAccount]
(
@account_id	int
)
AS
SET NOCOUNT ON

if @account_id < 1
begin
	RAISERROR ('lin_LoadAccount : invalid account_id [%d]', 16, 1, @account_id)
	RETURN -1	
end

SELECT top 10 char_id, account_name FROM user_data (nolock) WHERE account_id= @account_id
GO

/********************************************    
lin_LoadAgit    
     
INPUT     
 @agit_id  int    
OUTPUT    
 pledge_id,     
 hp_stove,     
 hp_stove_expire,     
 mp_flame,     
 mp_flame_expire,     
 hatcher,     
 status,     
 next_war_time     
return    
made by    
 carrot    
date    
 2002-06-16    
********************************************/    
ALTER PROCEDURE [DBO].[lin_LoadAgit]    
(    
 @agit_id  int    
)    
AS    
    
SET NOCOUNT ON    
    
select pledge_id, hp_stove, hp_stove_expire, mp_flame, mp_flame_expire, hatcher, status, next_war_time ,  
  isnull(R1.auction_id, 0) , isnull( auction_time, 0)   , isnull ( last_price , 0) , isnull(last_cancel, 0) , isnull (min_price, 0), isnull(teleport_level, 0),   
 isnull(teleport_expire, 0),  isnull ( auction_desc, '') , isnull( next_cost, 0) , isnull ( cost_fail, 0 )  , isnull ( tax_rate, 0 ),   
 isnull (tax_rate_to_change, 0), isnull (tax_child_rate, 0), isnull (tax_child_rate_to_change, 0), isnull(shop_income, 0), isnull(shop_income_temp, 0)  
from (    
 select id, pledge_id, hp_stove, hp_stove_expire, mp_flame, mp_flame_expire, hatcher, status, next_war_time , auction_id, last_price, last_cancel,   
 teleport_level, teleport_expire, next_cost, cost_fail  , tax_rate, tax_rate_to_change, tax_child_rate, tax_child_rate_to_change, shop_income_temp, shop_income  
 from agit (nolock)     
 where id = @agit_id      
 ) as R1    
 left join    
 (     
 select agit_id, auction_id, auction_time , min_price, auction_desc    
 from agit_auction (nolock)     
 where agit_id = @agit_id     
 ) as R2    
 on R1.id = R2.agit_id and R1.auction_id = R2.auction_id
GO

-- lin_LoadAgitDeco
-- by bert
-- return (1, 0)
ALTER PROCEDURE dbo.lin_LoadAgitDeco
(
	@agit_id INT
)
AS
SET NOCOUNT ON

SELECT type, id, level, expire FROM agit_deco WHERE agit_id = @agit_id
GO

/****** Object:  Stored Procedure dbo.lin_LoadAllAgit    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_LoadAllAgit
	
INPUT
OUTPUT
return
made by
	carrot
date
	2002-06-10
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadAllAgit]
AS
SET NOCOUNT ON

SELECT id, pledge_id FROM castle  (nolock) WHERE type = 2 ORDER BY id
GO

/****** Object:  Stored Procedure dbo.lin_LoadAllAllianceId    Script Date: 2003-09-20 ?? 11:51:57 ******/
-- lin_LoadAllAllianceId
-- by bert
ALTER PROCEDURE
dbo.lin_LoadAllAllianceId
AS

SET NOCOUNT ON

SELECT id FROM alliance
GO

/********************************************
lin_LoadAllAllianceWarData
	
INPUT	
	@status	int
OUTPUT
	id, 
	begin_time, 
	challenger, 
	challengee 
return
made by
	bert
date
	2003-11-07
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadAllAllianceWarData]
(
	@status	int
)
AS
SET NOCOUNT ON

SELECT 
	id, begin_time, challenger, challengee 
FROM 
	alliance_war (nolock)  
WHERE 
	status = @status
GO

/****** Object:  Stored Procedure dbo.lin_LoadAllCastle    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_LoadAllCastle
	
INPUT
OUTPUT
return
made by
	carrot
date
	2002-06-10
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadAllCastle]
--(
--	@tax_rate		int,
--	@castle_id		int
--)
AS
SET NOCOUNT ON

SELECT castle_d.id, castle_d.pledge_id, castle_d.next_war_time, castle_d.tax_rate, (select char_name from user_data where char_id = p.ruler_id)
FROM 
	(select * from castle (nolock) where type= 1) as castle_d
	inner join
	(select * from pledge (nolock) where pledge_id in (select pledge_id from castle)) as p
	on 
	castle_d.pledge_id = p.pledge_id
ORDER BY castle_d.id
GO

/****** Object:  Stored Procedure dbo.lin_LoadAlliance    Script Date: 2003-09-20 ?? 11:51:57 ******/
-- lin_LoadAlliance
-- by bert
ALTER PROCEDURE
dbo.lin_LoadAlliance (@alliance_id INT)
AS

SET NOCOUNT ON

SELECT id, name, master_pledge_id, oust_time, crest_id FROM alliance WHERE id = @alliance_id
GO

/********************************************
lin_LoadAllianceWar
	
INPUT
	@war_id		int
OUTPUT
return
made by
	bert
date
	2003-11-07
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadAllianceWar]
(
	@war_id		int
)
AS
SET NOCOUNT ON

SELECT challenger, challengee, begin_time, status FROM alliance_war (nolock)  WHERE id = @war_id
GO

/****** Object:  Stored Procedure dbo.lin_LoadAllPledge    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_LoadAllPledge
	
INPUT
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadAllPledge]
--(
--	@account_name	nvarchar(50)
--)
AS
SET NOCOUNT ON

SELECT 
	p.pledge_id, p.name, p.ruler_id, ud.char_name, 
	p.alliance_id, p.challenge_time, p.now_war_id, p.name_value, p.oust_time, p.skill_level, 
	p.private_flag, p.status, p.rank, p.castle_id, p.agit_id, p.root_name_value, 
	p.crest_id, p.is_guilty, p.dismiss_reserved_time 
FROM 
	pledge p (nolock),
	(select * from user_data (nolock) where pledge_id > 0 ) ud 
WHERE
	p.ruler_id = ud.char_id
GO

/****** Object:  Stored Procedure dbo.lin_LoadAllWarData    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_LoadAllWarData
	
INPUT	
	@status	int
OUTPUT
	id, 
	begin_time, 
	challenger, 
	challengee 
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadAllWarData]
(
	@status	int
)
AS
SET NOCOUNT ON

SELECT 
	id, begin_time, challenger, challengee 
FROM 
	pledge_war (nolock)  
WHERE 
	status = @status
GO

/********************************************
lin_LoadBlockList
	when character log in, load he's blocked list.
INPUT
	char_id
OUTPUT
return
made by
	carrot
date
	2003-12-01
change
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadBlockList]
(
	@char_id	INT
)
AS

SET NOCOUNT ON

SELECT block_char_id, block_char_name FROM user_blocklist WHERE char_id = @char_id
GO

/****** Object:  Stored Procedure dbo.lin_LoadBuilderAccountByAccountName    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_LoadBuilderAccountByAccountName
	
INPUT
	@account_name	NVARCHAR(50)
OUTPUT
return
made by
	carrot
date
	2002-06-09
change 	2003-07-03	carrot
	restrict only builer account
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadBuilderAccountByAccountName]
(
@account_name	NVARCHAR(50)
)
AS
SET NOCOUNT ON

SELECT top 1 account_id FROM builder_account (nolock) WHERE account_name= @account_name and account_id > 0
GO

/********************************************  
lin_LoadCastle  
   
INPUT   
 @id int,  
 @type int  
OUTPUT  
 pledge_id,   
 next_war_time,   
 tax_rate,   
 status,   
 name,   
 crop_income,   
 shop_income,   
 siege_elapsed_time   
 shop_Income_Temp,   
 Tax_rate_to_change,  
 tax_child_rate_to_change  
return  
made by  
 carrot  
date  
 2002-06-16  
change 2004-02-29 carrot  
 add CastleIncomeTemp and TaxRateTochange, tax_child_rate_to_change  
********************************************/  
ALTER PROCEDURE [DBO].[lin_LoadCastle]  
(  
 @id int,  
 @type int  
)  
AS  
SET NOCOUNT ON  
  
SELECT   
 pledge_id, next_war_time, tax_rate, tax_child_rate, status, name, crop_income, shop_income, siege_elapsed_time, shop_Income_Temp, Tax_rate_to_change,  
 tax_child_rate_to_change  
FROM   
 castle (nolock)   
WHERE   
 id = @id AND type = @type
GO

/****** Object:  Stored Procedure dbo.lin_LoadCastleCrop    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_LoadCastleCrop
	
INPUT	
	@castle_id	int,
OUTPUT
	item_type, 
	droprate, 
	price, 
	level 
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadCastleCrop]
(
	@castle_id	int
)
AS
SET NOCOUNT ON

SELECT 
	item_type, droprate, price, level 
from 
	castle_crop (nolock)  
WHERE 
	castle_id = @castle_id
GO

/****** Object:  Stored Procedure dbo.lin_LoadCastleWarRelatedPledge    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_LoadCastleWarRelatedPledge
	
INPUT	
	@castle_id	int
OUTPUT
return
made by
	bert
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadCastleWarRelatedPledge]
(
	@castle_id	int
)
AS
SET NOCOUNT ON

SELECT 
	pledge_id, type, propose_time 
FROM 
	castle_war (nolock)  
WHERE 
	castle_id = @castle_id
GO

ALTER PROCEDURE [dbo].lin_LoadControlTowerByName
(
@name VARCHAR(256)
)
AS
SELECT residence_id, control_level, hp, status
FROM control_tower
WHERE name = @name
GO

ALTER PROCEDURE [dbo].lin_LoadControlTowerByResidenceId
(
@residence_id INT
)
AS
SELECT name, control_level, hp, status
FROM control_tower
WHERE residence_id = @residence_id
GO

/********************************************    
lin_LoadDayUsedTime    
 load account's today used time    
INPUT    
 account_id    
OUTPUT    
return    
 used sec INT    
made by    
 carrot    
date    
 2004-03-29    
********************************************/    
ALTER PROCEDURE [DBO].[lin_LoadDayUsedTime]    
(    
 @account_id INT    
)    
AS    
    
SET NOCOUNT ON    

SELECT TOP 1 used_sec, convert(varchar(19), last_logout, 121) FROM time_data (nolock) WHERE account_id = @account_id
GO

/****** Object:  Stored Procedure dbo.lin_LoadDismissReservedPledge    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_LoadDismissReservedPledge
	
INPUT	
	@status	int
OUTPUT
	pledge_id, 
	dismiss_reserved_time 
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadDismissReservedPledge]
(
	@status	int
)
AS
SET NOCOUNT ON

SELECT pledge_id, dismiss_reserved_time FROM pledge  (nolock) WHERE status = @status
GO

ALTER PROCEDURE [dbo].[lin_LoadDoor]  
(  
 @name NVARCHAR(50)  
)  
AS  
SET NOCOUNT ON  
SELECT hp, ISNULL(max_hp, hp) FROM door  (nolock) WHERE name = @name
GO

ALTER PROCEDURE dbo.lin_LoadFriends (@char_id INT)  
AS  
  
SET NOCOUNT ON  
  
SELECT friend_char_id, ud.char_name AS friend_char_name   
FROM user_friend AS uf, user_data AS ud  
WHERE uf.char_id = @char_id AND uf.friend_char_id = ud.char_id AND ud.account_id <> -1
GO

ALTER PROCEDURE dbo.lin_LoadHenna
(        
@char_id INT
)        
AS    
SET NOCOUNT ON        

SELECT  henna_1, henna_2, henna_3 FROM user_henna WHERE char_id = @char_id
GO

/****** Object:  Stored Procedure dbo.lin_LoadItemByItemId    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_LoadItemByItemId 
	
INPUT
	@item_id		INT
OUTPUT
return
made by
	carrot
date
	2002-06-10
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadItemByItemId]
(
	@item_id		INT
)
AS
SET NOCOUNT ON

SELECT char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item (nolock) WHERE item_id = @item_id AND (NOT item_type = 0)
GO

/********************************************  
lin_LoadItems  
 Load item data and check adena data  
INPUT  
 @char_id INT,  
 @warehouse  INT  
OUTPUT  
 item_id, item_type, amount, enchant, eroded, bless, ident, wished  
return  
made by  
 carrot  
date  
 2002-04-23  
//const WCHAR* qsLoadItems = L"SELECT item_id, item_type, amount, enchant, eroded, bless, ident, wished FROM user_item WHERE char_id = %d AND warehouse = %d AND (NOT item_type = 0) AND NOT ITEM_TYPE = 57";  
********************************************/  
ALTER PROCEDURE [DBO].[lin_LoadItems]  
(  
 @char_id INT,  
 @warehouse INT  
)  
AS  
SET NOCOUNT ON  
  
DECLARE @nSum  INT  
DECLARE @nCount INT  
  
SET @nSum = 0  
SET @nCount = 0  
  
  
--SELECT @nCount = count(amount), @nSum = sum(amount) FROM user_item WHERE char_id = @char_id AND item_type = 57 AND warehouse = @warehouse  
--IF @nCount > 1  
--BEGIN  
-- DECLARE @nMaxItemId INT  
-- SELECT @nMaxItemId = Max(item_id) FROM user_item WHERE char_id = @char_id AND item_type = 57 AND warehouse = @warehouse  
-- IF @nMaxItemId > 0   
-- BEGIN  
--  UPDATE user_item Set amount = @nSum WHERE char_id = @char_id AND item_type = 57 AND warehouse = @warehouse And item_id = @nMaxItemId  
--  DELETE user_item WHERE char_id = @char_id AND item_type = 57 AND warehouse = @warehouse And NOT item_id = @nMaxItemId  
-- END  
--END  
  
SELECT item_id, item_type, amount, enchant, eroded, bless, ident, wished FROM user_item (nolock) WHERE char_id = @char_id AND warehouse = @warehouse  AND item_type > 0
GO

/********************************************
lin_LoadLastLogout
	load char last logout
INPUT
	char_id	int

OUTPUT
return
made by
	carrot
date
	2003-12-1
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadLastLogout]
(
@char_id		INT

)
AS
SET NOCOUNT ON


select Year(logout), Month(logout), Day(logout), DATEPART(HOUR, logout), DATEPART(mi, logout), DATEPART(s, logout),
Year(login), Month(login), Day(login), DATEPART(HOUR, login), DATEPART(mi, login), DATEPART(s, login),
Year(create_date), Month(create_date), Day(create_date), DATEPART(HOUR, create_date), DATEPART(mi, create_date), DATEPART(s, create_date)
from user_data (nolock) where char_id = @char_id
GO

/********************************************    
lin_LoadLastTaxUpdate  
     
INPUT     
OUTPUT    
made by    
 carrot    
date    
 2004-02-29  
********************************************/    
ALTER PROCEDURE [DBO].[lin_LoadLastTaxUpdate]  
AS    
    
SET NOCOUNT ON    
  
SELECT TOP 1  
 YEAR(income_update), MONTH(income_update), DAY(income_update),   
 DATEPART ( hh , income_update ), DATEPART ( mi , income_update ), DATEPART ( ss , income_update ),  
 YEAR(tax_change), MONTH(tax_change), DAY(tax_change),   
 DATEPART ( hh , tax_change ), DATEPART ( mi , tax_change), DATEPART ( ss , tax_change),  
 YEAR(manor_reset), MONTH(manor_reset), DAY(manor_reset),   
 DATEPART ( hh , manor_reset ), DATEPART ( mi , manor_reset), DATEPART ( ss , manor_reset)  
FROM   
 castle_tax
GO

ALTER PROCEDURE [DBO].[lin_LoadLottoGame]
AS    
   
SET NOCOUNT ON    

select 	top 65535
	round_number,	
	state,
	left_time,
	chosen_number_flag,
	rule_number,

	total_count,
	winner1_count,
	winner2_count,
	winner3_count,
	winner4_count,
	carried_adena

from lotto_game	(nolock)
order by round_number
GO

ALTER PROCEDURE [DBO].[lin_LoadLottoItems]
(
	@round int
)
AS    
   
SET NOCOUNT ON    

select 	item_id,
	number_flag
from lotto_items
where round_number = @round
GO

/********************************************        
lin_LoadManorInfo
 load manor seed next
INPUT        
 @manor_id
OUTPUT        
return        
       
made by        
 carrot        
date        
 2004-07-4
change        
********************************************/        
ALTER  PROCEDURE [DBO].[lin_LoadManorInfo]
(        
 @manor_id INT
)        
AS        
        
SET NOCOUNT ON        

IF EXISTS(SELECT * FROM manor_info WHERE manor_id = @manor_id)
BEGIN
	SELECT 
		residence_id, adena_vault, crop_buy_vault, change_state, convert(nvarchar(19), last_changed, 121)
	FROM 
		manor_info
	WHERE 
		manor_id = @manor_id
END
ELSE
BEGIN
	SELECT 0, 0, 0, 0, '0000-00-00 00:00:00'

END
GO

/********************************************        
lin_LoadManorSeed
 load manor seed
INPUT        
 @manor_id
OUTPUT        
return        
       
made by        
 carrot        
date        
 2004-07-4
change        
********************************************/        
ALTER PROCEDURE [DBO].[lin_LoadManorSeed]
(        
 @manor_id INT
)        
AS        
        
SET NOCOUNT ON        

SELECT 
	data_index, seed_id, seed_price, seed_sell_count, seed_remain_count, 
	crop_id, crop_price, crop_buy_count, crop_remain_count, crop_type, crop_deposit
FROM 
	manor_data
WHERE 
	manor_id = @manor_id
ORDER BY data_index
GO

/********************************************        
lin_LoadManorSeed_N
 load manor seed next
INPUT        
 @manor_id
OUTPUT        
return        
       
made by        
 carrot        
date        
 2004-07-4
change        
********************************************/        
ALTER PROCEDURE [DBO].[lin_LoadManorSeed_N]
(        
 @manor_id INT
)        
AS        
        
SET NOCOUNT ON        

SELECT 
	data_index, seed_id_n, seed_price_n, seed_sell_count_n,
	crop_id_n, crop_buy_count_n, crop_price_n, crop_type_n
FROM 
	manor_data_n
WHERE 
	manor_id = @manor_id
ORDER BY 
	data_index
GO

/********************************************
lin_LoadMarketPriceList
	load item market price list
INPUT

OUTPUT
return
made by
	kks
date
	2005-04-01
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadMarketPriceList] 
AS

SET NOCOUNT ON

SELECT item_type, enchant, avg_price FROM item_market_price (nolock)
WHERE frequency >= 10
GO

ALTER PROCEDURE [dbo].lin_LoadMercenary
(
@residence_id INT
)
AS
SELECT id, npc_id, x, y, z, angle, hp, mp
FROM mercenary
WHERE residence_id = @residence_id
GO

/********************************************
lin_LoadNewbieData
	load newbie data
INPUT

OUTPUT
return
made by
	kks
date
	2004-11-25
change
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadNewbieData]

AS

SET NOCOUNT ON

SELECT account_id, char_id, newbie_stat
FROM user_newbie (nolock)
GO

/****** Object:  Stored Procedure dbo.lin_LoadNpcBoss    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_LoadNpcBoss
	
INPUT	
	@NPC_name	nvarchar(50)
OUTPUT
	alive, 
	hp, 
	mp, 
	pos_x, 
	pos_y, 
	pos_z, 
	time_low, 
	time_high 
	i0
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadNpcBoss]
(
	@NPC_name	nvarchar(50)
)
AS
SET NOCOUNT ON

select 
	alive,  hp,  mp, pos_x, pos_y, pos_z, time_low, time_high , i0
from 
	npc_boss  (nolock)  
where 
	npc_db_name = @NPC_name
GO

ALTER PROCEDURE dbo.lin_LoadOlympiad
AS
SET NOCOUNT ON

DECLARE @cnt INT
SELECT @cnt = COUNT(*) FROM olympiad

IF @cnt = 0
BEGIN
	INSERT INTO olympiad (step) VALUES (0)
END
SELECT TOP 1 season, step, 
ISNULL(season_start_time, 0),
ISNULL(start_sec, 0), 
ISNULL(bonus1_sec, 0), 
ISNULL(bonus2_sec, 0), 
ISNULL(bonus3_sec, 0), 
ISNULL(bonus4_sec, 0), 
ISNULL(nominate_sec, 0) 
FROM olympiad ORDER BY season DESC
GO

/********************************************
lin_LoadPet
	
INPUT	
	@pet_id int
OUTPUT
return
made by
	kuooo
date
	2003-08-22
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadPet]
(
	@pet_id int
)
AS
SET NOCOUNT ON

SELECT npc_class_id  , expoint , hp, mp, sp, meal, nick_name , slot1, slot2 FROM pet_data (nolock) WHERE pet_id = @pet_id
GO

/****** Object:  Stored Procedure dbo.lin_LoadPetItems    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_LoadPetItems
	Load item data from pet inventory
INPUT
	@pet_id 	INT
OUTPUT
	item_id, item_type, amount, enchant, eroded, bless, ident, wished
return
made by
	kuooo
date
	2003-08-25

********************************************/
ALTER PROCEDURE [DBO].[lin_LoadPetItems]
(
	@pet_id 	INT
)
AS
SET NOCOUNT ON

SELECT item_id, item_type, amount, enchant, eroded, bless, ident, wished FROM user_item WHERE char_id = @pet_id  AND warehouse = 5
GO

/********************************************
lin_LoadPledgeAnnounce
	load pledge announce 
INPUT
	@pledge_id		int
OUTPUT
return
made by
	kks
date
	2005-07-22
********************************************/
ALTER PROCEDURE dbo.lin_LoadPledgeAnnounce
(  
	@pledge_id		int
)  
AS  
SET NOCOUNT ON  
  

select show_flag, content 
from pledge_announce (nolock)
WHERE pledge_id = @pledge_id
GO

/****** Object:  Stored Procedure dbo.lin_LoadPledgeById    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_LoadPledgeById
	
INPUT
	@CharName		NVARCHAR(50)
OUTPUT
return
made by
	carrot
date
	2002-06-10
modified by kks (2005-07-22)
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadPledgeById]
(
	@PledgeId		int
)
AS
SET NOCOUNT ON

SELECT 
	p.pledge_id, p.name, p.ruler_id, ud.char_name, 
	p.alliance_id, p.challenge_time, p.now_war_id, p.name_value, p.oust_time, p.skill_level, 
	p.private_flag, p.status, p.rank, p.castle_id, p.agit_id, p.root_name_value, 
	p.crest_id, p.is_guilty, p.dismiss_reserved_time, p.alliance_ousted_time, p.alliance_withdraw_time, p.alliance_dismiss_time,
	p.emblem_id
FROM 
	(select * from pledge (nolock)where pledge_id = @PledgeId) as  p  
	JOIN 
	(select * from user_data (nolock)where pledge_id = @PledgeId) as  ud
	ON p.ruler_id = ud.char_id
GO

ALTER PROCEDURE [dbo].[lin_LoadPledgeByName]
(
 @PledgeName  NVARCHAR(50)  
) 
AS
SET NOCOUNT ON

SELECT
	p.pledge_id, p.name, p.ruler_id, ud.char_name,   
	p.alliance_id, p.challenge_time, p.now_war_id, p.name_value, p.oust_time, p.skill_level,   
	p.private_flag, p.status, p.rank, p.castle_id, p.agit_id, p.root_name_value,   
	p.crest_id, p.is_guilty, p.dismiss_reserved_time, p.alliance_ousted_time, p.alliance_withdraw_time, p.alliance_dismiss_time,
	p.emblem_id
FROM
	pledge p (NOLOCK), user_data ud (NOLOCK)
WHERE
	p.name = @PledgeName AND p.ruler_id = ud.char_id AND p.pledge_id = ud.pledge_id AND p.pledge_id <> -1
GO

/****** Object:  Stored Procedure dbo.lin_LoadPledgeCrest    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_LoadPledgeCrest
	
INPUT
	@crest_id	int
OUTPUT
	bitmap_size, 
	bitmap 
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadPledgeCrest]
(
	@crest_id	int
)
AS
SET NOCOUNT ON

SELECT 
	bitmap_size, bitmap 
FROM 
	pledge_crest  (nolock) 
WHERE 
	crest_id = @crest_id
GO

/********************************************  
lin_LoadPledgeMember  
   
INPUT  
 pledge_id = @pledge_id  
OUTPUT  
return  
made by  
 carrot  
date  
 2002-06-10  
change 2003-07-22 carrot  
 check character is deleted.  
********************************************/  
ALTER PROCEDURE [DBO].[lin_LoadPledgeMember]  
(  
 @pledge_id  int  
)  
AS  
SET NOCOUNT ON  
  
IF (@pledge_id > 0)
SELECT TOP 40 char_id FROM user_data (nolock) WHERE pledge_id = @pledge_id and account_id > 0
GO

/****** Object:  Stored Procedure dbo.lin_LoadPledgeWar    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_LoadPledgeWar
	
INPUT
	@war_id		int
OUTPUT
return
made by
	carrot
date
	2002-06-10
********************************************/
ALTER PROCEDURE [DBO].[lin_LoadPledgeWar]
(
	@war_id		int
)
AS
SET NOCOUNT ON

SELECT challenger, challengee, begin_time, status FROM pledge_war (nolock)  WHERE id = @war_id
GO

/********************************************  
lin_LoadQuest  
   
INPUT  
 @char_id int  
OUTPUT  
return  
made by  
 carrot  
date  
 2002-06-09  
********************************************/  
ALTER PROCEDURE [DBO].[lin_LoadQuest]  
(  
@char_id int  
)  
AS  
SET NOCOUNT ON  
  
SELECT TOP 1  q1, s1,  s2_1, j1,    
  q2, s2,  s2_2, j2,   
  q3, s3,  s2_3, j3,   
  q4, s4,  s2_4, j4,   
  q5, s5,  s2_5, j5,   
  q6, s6,  s2_6, j6,   
  q7, s7, s2_7,  j7,   
  q8, s8, s2_8,  j8,  
  q9, s9,  s2_9, j9,   
  q10, s10, s2_10,  j10,   
  q11, s11,  s2_11,  j11,   
  q12, s12, s2_12,  j12,  
  q13, s13, s2_13,  j13,  
   q14, s14, s2_14,  j14,   
  q15, s15,  s2_15, j15,   
  q16, s16,  s2_16, j16    
FROM quest (nolock)   
WHERE char_id = @char_id
GO

/********************************************        
lin_LoadRecipeInfo
 load all recipe
INPUT        
 @char_id
OUTPUT        
return        
       
made by        
 carrot        
date        
 2004-07-4
change        
********************************************/        
ALTER PROCEDURE [DBO].[lin_LoadRecipeInfo]
(        
 @char_id INT
)        
AS        
        
SET NOCOUNT ON        

SELECT recipe_id FROM user_recipe WHERE char_id = @char_id ORDER BY recipe_id
GO

ALTER PROCEDURE [dbo].lin_LoadResidenceGuard
(
@residence_id INT
)
AS
SELECT item_id, npc_id, guard_type, can_move, x, y, z, angle
FROM residence_guard
WHERE residence_id = @residence_id
GO

ALTER PROCEDURE dbo.lin_LoadSiegeAgitPledge
(
	@agit_id INT
)
AS
SET NOCOUNT ON
SELECT pledge_id, propose_time, status FROM siege_agit_pledge WHERE agit_id = @agit_id
GO

/********************************************        
lin_LoadSociality
 load sociality
INPUT        
 @char_id
OUTPUT        
return        
       
made by        
 carrot        
date        
 2004-07-4
change        
********************************************/        
ALTER PROCEDURE [DBO].[lin_LoadSociality]
(        
 @char_id INT
)        
AS        
        
SET NOCOUNT ON        

IF EXISTS(SELECT  * FROM user_sociality WHERE char_id = @char_id)
BEGIN
	SELECT  sociality, used_sulffrage, convert(nvarchar(19), last_changed, 121) FROM user_sociality WHERE char_id = @char_id
END
ELSE
BEGIN
	SELECT  0, 0, '0000-00-00 00:00:00'
END
GO

/**
  * @procedure lin_LoadSSQJoinInfo
  * @brief SSQ ??/?? ?? ?? ??
  *
  * @date 2004/11/18
  * @author sonai  <sonai@ncsoft.net>
  */
ALTER PROCEDURE [DBO].[lin_LoadSSQJoinInfo]
(
@round_number INT
)
 AS 

SELECT point, collected_point, main_event_point, type, member_count, 
	 seal1_selection_count, 
	 seal2_selection_count, 
	 seal3_selection_count, 
	 seal4_selection_count, 
	 seal5_selection_count, 
	 seal6_selection_count, 
	 seal7_selection_count
	 
	FROM ssq_join_data WHERE round_number = @round_number
GO

/**  
  * @procedure lin_LoadSSQSystemInfo  
  * @brief SSQ ?? ??  
  *  
  * @date 2004/11/18  
  * @author sonai  <sonai@ncsoft.net>  
  */  
ALTER PROCEDURE [DBO].[lin_LoadSSQSystemInfo] AS  
  
  
  
 SELECT TOP 1 round_number,  status, winner,  
   event_start_time, event_end_time, seal_effect_time, seal_effect_end_time,  
   seal1, seal2, seal3, seal4, seal5, seal6, seal7, ISNULL(castle_snapshot_time, 0), ISNULL(can_drop_guard, 0)  
   FROM ssq_data order by round_number desc
GO

/**
  * @procedure lin_LoadSSQTopPointUser
  * @brief SSQ top point user? ??.
  *
  * @date 2004/12/08
  * @author sonai  <sonai@ncsoft.net>
  */
ALTER PROCEDURE [DBO].[lin_LoadSSQTopPointUser] 
(
@ssq_round INT
)
AS

SELECT ssq_round, record_id, ssq_point, rank_time, char_id, char_name, ssq_part, ssq_position, seal_selection_no
			FROM ssq_top_point_user  WHERE ssq_round  = @ssq_round
GO

/**
 * @procedure lin_LoadSSQUserInfo
 * @brief  Load user's ssq info from ssq_user_data
 *
 * @date  2004/11/18
 * @author sonai <sonai@ncsoft.net>
 *
 * @param[in]  char_id user_data's id
 */
ALTER PROCEDURE [DBO].[lin_LoadSSQUserInfo] 
(
@char_id INT
)
AS
SET NOCOUNT ON

SELECT round_number, ssq_join_time, ssq_part, ssq_position, seal_selection_no, 
              ssq_point,
              twilight_a_item_num,twilight_b_item_num,twilight_c_item_num,
              dawn_a_item_num,dawn_b_item_num, dawn_c_item_num,
              ticket_buy_count       
             FROM ssq_user_data WHERE char_id = @char_id
GO

ALTER PROCEDURE dbo.lin_LoadTeamBattleAgitMember
(
	@agit_id INT
)
AS
SET NOCOUNT ON
SELECT char_id, pledge_id, propose_time FROM team_battle_agit_member WHERE agit_id = @agit_id
GO

ALTER PROCEDURE dbo.lin_LoadTeamBattleAgitPledge
(
	@agit_id INT
)
AS
SET NOCOUNT ON
SELECT pledge_id, propose_time, color, npc_type FROM team_battle_agit_pledge WHERE agit_id = @agit_id
GO

ALTER PROCEDURE dbo.lin_LoadTeamBattleAgitStatus
(
	@agit_id INT
)
AS
SET NOCOUNT ON
SELECT ISNULL(team_battle_status, 0) FROM agit WHERE id = @agit_id
GO

/**
  * @procedure lin_LoadTimeAttackRecord
  * @brief TimeAttackRecord ??
  *
  * @date 2004/12/04
  * @author sonai  <sonai@ncsoft.net>
  */
ALTER PROCEDURE [DBO].[lin_LoadTimeAttackRecord] 
(
@ssq_round INT
)
AS

SELECT room_no, record_type, ssq_round, ssq_part, point, record_time, elapsed_time, member_count, member_names, 
	 ISNULL(member_dbid_list, ''), ISNULL(member_reward_flags, 0), ISNULL(fee, 0)
	 FROM time_attack_record  WHERE ssq_round = @ssq_round
GO

/****** Object:  Stored Procedure dbo.lin_MakeBBSBoard    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_MakeBBSBoard
	Make BBS board
INPUT
	@board_name	nvarchar(20),
	@board_desc	nvarchar(50)
OUTPUT

return
made by
	young
date
	2002-10-16
********************************************/
ALTER PROCEDURE [DBO].[lin_MakeBBSBoard]
(
	@board_name	nvarchar(20),
	@board_desc	nvarchar(50)
)
AS

set nocount on

declare @ncount int
declare @table_name nvarchar(20)
declare @exec nvarchar(500)

set @table_name = 'bbs_' + @board_name

select @ncount = count(*) from sysobjects (nolock) where name = @table_name
select @ncount
if @ncount = 0
begin

	set @exec = 'CREATE TABLE dbo.' + @table_name + '(' + char(13)
	set @exec = @exec + ' id int IDENTITY (1,1) NOT NULL, ' + char(13)
	set @exec = @exec + ' title nvarchar(100) NULL, ' + char(13)
	set @exec = @exec + ' contents nvarchar(3000) NULL, ' + char(13)
	set @exec = @exec + ' writer nvarchar(50) NULL, ' + char(13)
	set @exec = @exec + ' cdate datetime NOT NULL, ' + char(13)
	set @exec = @exec + ' nread int NOT NULL)  ' + char(13)
	exec (@exec)
	set @exec = 'ALTER TABLE dbo.' + @table_name + ' WITH NOCHECK ADD' + char(13)
	set @exec = @exec + 'CONSTRAINT PK_' + @table_name + ' PRIMARY KEY CLUSTERED' + char(13)
	set @exec = @exec + '( ' + char(13)
	set @exec = @exec + 'id ' + char(13)
	set @exec = @exec + ')' + char(13)
	exec (@exec)
	set @exec = 'ALTER TABLE dbo.' + @table_name + ' WITH NOCHECK ADD' + char(13)
	set @exec = @exec + 'CONSTRAINT DF_' + @table_name + '_cdate DEFAULT getdate() FOR cdate, ' + char(13)
	set @exec = @exec + 'CONSTRAINT DF_' + @table_name + '_nread DEFAULT 0 FOR nread ' + char(13)
	exec (@exec)

	insert into bbs_board(board_name, board_desc) values(@table_name, @board_desc)
end
GO

/********************************************
lin_MakeDailyWorldSnap
Daily World DB Snap

#argument	@db_server	varchar(30)	name of database server
#argument	@user_id	varchar(30)	user id
#argument	@user_pass	varchar(30)	user password
#argument	@worldId	int		world id 
#return
#result_set
#remark
#example	lin_RPMakeDailyWorldSnap 'db_server', 'gamma', '', 8
#history	create	zzangse		2005-03-03
#see lin_RPJoinSSQMember
#see lin_RPSSQSealSelection
#see lin_RPSnapSSQData
#see lin_RPSnapSSQJoinData
#see lin_RPSnapTimeAttackRecord
********************************************/

ALTER             PROCEDURE [DBO].[lin_MakeDailyWorldSnap]
	@db_server	varchar(30) ,
	@user_id	varchar(30),
	@user_pass	varchar(30),
	@worldId	int 
AS
SET NOCOUNT ON


declare @table_to varchar(60)
declare @sql varchar(512)

-----------------------------
-- exec DailyWorldSnaptSP --
-----------------------------

-- lin_RPJoinSSQMember
set @table_to =  'RP_TBLJOINSSQMEMBER_' + cast(@worldId as varchar)
exec [lin2report].[dbo].[lin_RPJoinSSQMember] @db_server, @user_id, @user_pass, @table_to
waitfor delay '0:0:6'


-- lin_RPSSQSealSelection
set @table_to =  'RP_TBLSSQSEALSELECTION_' + cast(@worldId as varchar)
exec [lin2report].[dbo].[lin_RPSSQSealSelection] @db_server, @user_id, @user_pass, @table_to
waitfor delay '0:0:6'

-- lin_RPSnapSSQData
set @table_to = 'RP_SNAPSSQDATA_' + cast ( @worldId  as varchar )
exec [lin2report].[dbo].[lin_RPSnapSSQData] @db_server, @user_id, @user_pass, @table_to
waitfor delay '0:0:6'

-- lin_RPSnapSSQJoinData
set @table_to = 'RP_SNAPSSQJOINDATA_' + cast ( @worldId as varchar )
exec [lin2report].[dbo].[lin_RPSnapSSQJoinData] @db_server, @user_id, @user_pass, @table_to
waitfor delay '0:0:6'

-- lin_RPSnapTimeAttackRecord
set @table_to = 'RP_SNAPTIMEATTACKRECORD_' + cast (@worldId as varchar)
exec [lin2report].[dbo].[lin_RPSnapTimeAttackRecord] @db_server, @user_id, @user_pass, @table_to
waitfor delay '0:0:6'
GO

/****** Object:  Stored Procedure dbo.LIN_MakeNewBlankItem    Script Date: 2003-09-20 ?? 11:51:56 ******/
ALTER PROCEDURE [DBO].[LIN_MakeNewBlankItem]
AS

SET NOCOUNT ON

declare @newItemId int

insert into user_item (char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse)
values(0,0,0,0,0,0,0,0,0)

SET @newItemId = @@IDENTITY
select @newItemId
GO

/******************************************************************************
#Name:	lin_MakeSnapTable
#Desc:	make tmp table for world snap shot of user_data, user_item, pledge, user_nobless

#Argument:
	Input:
	Output:	
#Return:
#Result Set:

#Remark:
#Example:	exec lin_MakeSnapTable
#See:

#History:
	Create	flagoftiger	2004-06-15
	Modify	btwinuni	2005-05-16	add: pledge
	MOdify	btwinuni	2005-09-29	add: user_nobless
******************************************************************************/
ALTER PROCEDURE [DBO].[lin_MakeSnapTable]

AS
SET NOCOUNT ON

declare @sql varchar(4000)

-- check table whether tmp_user_item is exists or not
set @sql = 'if exists (select * from dbo.sysobjects where id = object_id(N''[dbo].[tmp_user_item]'') and objectproperty(id, N''IsUserTable'') = 1)'
	+ ' begin'
	+ ' drop table dbo.tmp_user_item'
	+ ' end'
exec (@sql)

-- user_item copy
set @sql = 'select item_id, char_id, item_type, amount, enchant, warehouse '
	+ 'into tmp_user_item from user_item (nolock) where char_id > 0' 
exec (@sql)



-- check table whether tmp_user_data is exists or not
set @sql = 'if exists (select * from dbo.sysobjects where id = object_id(N''[dbo].[tmp_user_data]'') and objectproperty(id, N''IsUserTable'') = 1)'
	+ ' begin'
	+ ' drop table dbo.tmp_user_data'
	+ ' end'
exec (@sql)

-- user_data copy
set @sql = 'select char_name, char_id, account_name, account_id, pledge_id, builder, '
	+ ' gender, race, class, world, SP, EXP, Lev, align, PK, PKpardon, Duel, create_date, '
	+ ' login, logout, nickname, max_hp, max_mp, use_time, drop_exp, '
	+ ' cp, max_cp, subjob_id, subjob0_class, subjob1_class, subjob2_class, subjob3_class '		-- add by btwinuni 2005-09-29
	+ ' into tmp_user_data from user_data (nolock)' 
exec (@sql)



-- check table whether tmp_pledge is exists or not
set @sql = 'if exists (select * from dbo.sysobjects where id = object_id(N''[dbo].[tmp_pledge]'') and objectproperty(id, N''IsUserTable'') = 1)'
	+ ' begin'
	+ ' drop table dbo.tmp_pledge'
	+ ' end'
exec (@sql)

-- pledge copy
set @sql = 'select pledge_id, ruler_id, name, alliance_id, root_name_value, name_value, oust_time, skill_level,'
	+ ' castle_id, agit_id, rank, status, is_guilty, dismiss_reserved_time'
	+ ' into tmp_pledge from pledge (nolock)' 
exec (@sql)



-- check table whether tmp_user_nobless is exists or not
set @sql = 'if exists (select * from dbo.sysobjects where id = object_id(N''[dbo].[tmp_user_nobless]'') and objectproperty(id, N''IsUserTable'') = 1)'
	+ ' begin'
	+ ' drop table dbo.tmp_user_nobless'
	+ ' end'
exec (@sql)

-- user_nobless copy
set @sql = 'select char_id, nobless_type, hero_type, win_count, previous_point, olympiad_point, match_count, words'
	+ ' into tmp_user_nobless from user_nobless (nolock)' 
exec (@sql)
GO

/********************************************
lin_ManageUserNameReserved
	manage user name reserved ( add, del )
INPUT
	@option	int,
	@char_name	nvarchar(50),
	@account_id	int,
	@used		int
OUTPUT

return
made by
	kks
date
	2004-12-13
********************************************/
ALTER PROCEDURE [DBO].[lin_ManageUserNameReserved]
(
	@option	int,
	@char_name	nvarchar(50),
	@account_id	int,
	@used		int
)
AS
SET NOCOUNT ON

declare @reservedcount int
set @reservedcount  = 0

if ( @option = 0 )
begin
	-- add user name reserved
	select @reservedcount  = count(*) from user_name_reserved (nolock) where char_name = @char_name
	if ( @reservedcount >= 1)
		return

	insert into user_name_reserved ( char_name, account_id, used )
	values ( @char_name, @account_id, @used )
end 

if ( @option = 1 )
begin
	-- del user_name_reserved
	delete from user_name_reserved where char_name = @char_name
end
GO

/********************************************
lin_ManBookMark
	manage bookmark ( add, get-list, get , del )
INPUT
	@char_id	int,
	@bookmark_name	nvarchar(50)
OUTPUT

return
made by
	young
date
	2002-11-13
********************************************/
ALTER PROCEDURE [DBO].[lin_ManBookMark]
(
	@option	int,
	@char_id	int,
	@bookmark_id	int=0,
	@bookmark_name	nvarchar(50)='',
	@x_loc		int= 0,
	@y_loc		int=0,
	@z_loc		int=0
)
AS
SET NOCOUNT ON

declare @bookmarkcount int
set @bookmarkcount  = 0

if ( @option = 1 )
begin
	-- add bookmark
	select @bookmarkcount  = count(*) from bookmark (nolock) where char_id = @char_id
	if ( @bookmarkcount >= 200 )
		return

	insert into bookmark ( char_id, name, world, x, y, z )
	values ( @char_id, @bookmark_name, 0, @x_loc, @y_loc, @z_loc )
end 

if ( @option = 2 )
begin
	-- get bookmark list
	select bookmarkid, name, x, y, z from bookmark (nolock) where char_id = @char_id order by name asc
end

if ( @option = 3 )
begin
	-- get on ebookmark
	select name, x, y, z from bookmark (nolock) where bookmarkid = @bookmark_id
end

if ( @option = 4 )
begin
	-- del one bookmark
	delete from bookmark where bookmarkid = @bookmark_id
end
GO

/****** Object:  Stored Procedure dbo.lin_ModChar    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_ModChar
	
INPUT
	@sp		int,
	@exp		int,
	@align		int,
	@pk		int,
	@pkpardon	int,
	@duel		int,
	@char_id	int,
	@level		int
OUTPUT
return
made by
	carrot
date
	2002-06-10
********************************************/
ALTER PROCEDURE [DBO].[lin_ModChar]
(
	@sp		int,
	@exp		int,
	@level		int,
	@align		int,
	@pk		int,
	@pkpardon	int,
	@duel		int,
	@char_id	int
)
AS
SET NOCOUNT ON

update user_data set sp=@sp, exp=@exp, lev = @level,  align=@align, pk=@pk, pkpardon=@pkpardon, duel=@duel where char_id = @char_id
GO

/****** Object:  Stored Procedure dbo.lin_ModChar2    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_ModChar2
	
INPUT
	@sp		int,
	@exp		int,
	@align		int,
	@pk		int,
	@pkpardon	int,
	@duel		int,
	@char_id	int
OUTPUT
return
made by
	carrot
date
	2002-06-10
********************************************/
ALTER PROCEDURE [DBO].[lin_ModChar2]
(
	@gender		int,
	@race		int,
	@class		int,
	@face_index		int,
	@hair_shape_index	int,
	@hair_color_index		int,
	@char_id	int
)
AS
SET NOCOUNT ON

update user_data set gender=@gender, race=@race, class=@class, face_index=@face_index, hair_shape_index=@hair_shape_index, hair_color_index=@hair_color_index where char_id = @char_id
GO

/****** Object:  Stored Procedure dbo.lin_ModChar3    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_ModChar3
	
INPUT
	@mod field	int,
	@delta		int,
	@char_id	int
OUTPUT
return
made by
	young
date
	2003-08-26
********************************************/
ALTER PROCEDURE [DBO].[lin_ModChar3]
(
	@sp		int,
	@exp		int,
	@level		int,
	@align		int,
	@pk		int,
	@pkpardon	int,
	@duel		int,
	@char_id	int
)
AS
SET NOCOUNT ON

update user_data set 
	sp = sp + @sp,
	exp = exp + @exp,	
	align = align + @align,
	pk = pk + @pk,
	pkpardon = pkpardon + @pkpardon,
	duel = duel + @duel,
	lev = @level 
where char_id = @char_id
GO

ALTER PROCEDURE dbo.lin_ModifyAllianceName
(  
@alliance_name  NVARCHAR(50),  
@alliance_id  int  
)  
AS  
SET NOCOUNT ON 

UPDATE alliance SET name = @alliance_name WHERE id = @alliance_id
GO

/****** Object:  Stored Procedure dbo.lin_ModifyPledgeName    Script Date: 2003-09-20 ?? 11:51:59 ******/
/********************************************
lin_ModifyPledgeName
	
INPUT
	@crest_id		int
OUTPUT
return
made by
	carrot
date
	2002-06-10
********************************************/
ALTER PROCEDURE [DBO].[lin_ModifyPledgeName]
(
	@pledge_name		NVARCHAR(50),
	@pledge_id		int
)
AS
SET NOCOUNT ON

update pledge set name = @pledge_name where pledge_id = @pledge_id
GO

/********************************************
lin_SendMailToReceiver
	send mail  to receiver
INPUT
	@mail_id		int,
	@receiverName		nvarchar(50)
OUTPUT
return
made by
	kks
date
	2004-12-19
modified by
	kks
date
	2005-04-06
********************************************/
ALTER PROCEDURE [DBO].[lin_SendMailToReceiver]
(
	@mail_id		int,
	@receiver_name		nvarchar(50)
)
AS
SET NOCOUNT ON

if (@receiver_name = NULL)
BEGIN
	RETURN
END

DECLARE @receiver_id int
SET @receiver_id = 0

SELECT @receiver_id  = char_id FROM user_data(nolock) WHERE char_name = @receiver_name

IF (@receiver_id > 0) 
BEGIN
	INSERT INTO user_mail_receiver
	(mail_id, receiver_id, receiver_name)
	VALUES 
	(@mail_id, @receiver_id, @receiver_name)
END
GO

/********************************************
lin_ModifySendTempMail
	modify and send temp mail 
INPUT
	@char_id		int,
	@mail_id		int,
	@receiver_name_list		nvarchar(250),
	@title			nvarchar(200),
	@content		nvarchar(4000)
OUTPUT
return
made by
	kks
date
	2004-12-19
********************************************/
ALTER PROCEDURE [dbo].[lin_ModifySendTempMail]
(
	@char_id		int,
	@mail_id		int,
	@receiver_name_list		nvarchar(250),
	@title			nvarchar(200),
	@content		nvarchar(4000)
)
AS
SET NOCOUNT ON

UPDATE user_mail
SET title = @title,
	content = @content,
	created_date = GETDATE()
WHERE id = @mail_id

UPDATE user_mail_sender
SET receiver_name_list = @receiver_name_list,
	send_date = GETDATE(),
	mailbox_type = 1
WHERE 
	mail_id = @mail_id AND 
	sender_id = @char_id

DECLARE @mail_type INT
SELECT @mail_type = mail_type FROM user_mail_sender(nolock) WHERE mail_id = @mail_id

if @mail_type = 1
BEGIN
	DECLARE @char_name NVARCHAR(50)
	DECLARE name_cursor CURSOR FOR
	(SELECT char_name FROM user_data(nolock) WHERE pledge_id = (SELECT pledge_id FROM pledge(nolock) WHERE name = @receiver_name_list))
	OPEN name_cursor
	FETCH NEXT FROM name_cursor into @char_name
	WHILE @@FETCH_STATUS = 0
	BEGIN
		EXEC lin_SendMailToReceiver @mail_id, @char_name
	END

	CLOSE name_cursor
	DEALLOCATE name_cursor
END
ELSE
BEGIN
	DECLARE @receiver_name_list_replaced nvarchar(300)
	SELECT @receiver_name_list_replaced = REPLACE(@receiver_name_list,';', ' ; ')
	
	DECLARE @receiver_name1 nvarchar (50), @receiver_name2 nvarchar (50), @receiver_name3 nvarchar (50), @receiver_name4 nvarchar (50), @receiver_name5 nvarchar (50)
	EXEC master..xp_sscanf @receiver_name_list_replaced, '%s ; %s ; %s ; %s ; %s',
	   @receiver_name1 OUTPUT, @receiver_name2 OUTPUT, @receiver_name3 OUTPUT, @receiver_name4 OUTPUT, @receiver_name5 OUTPUT
	
	EXEC lin_SendMailToReceiver @mail_id, @receiver_name1
	EXEC lin_SendMailToReceiver @mail_id, @receiver_name2
	EXEC lin_SendMailToReceiver @mail_id, @receiver_name3
	EXEC lin_SendMailToReceiver @mail_id, @receiver_name4
	EXEC lin_SendMailToReceiver @mail_id, @receiver_name5
END
GO

/********************************************
lin_ModifyTempMail
	modify temp mail 
INPUT
	@char_id		int,
	@mail_id		int,
	@receiver_name_list		nvarchar(250),
	@title			nvarchar(200),
	@content		nvarchar(4000)
OUTPUT
return
made by
	kks
date
	2004-12-19
********************************************/
ALTER PROCEDURE [dbo].[lin_ModifyTempMail]
(
	@char_id		int,
	@mail_id		int,
	@receiver_name_list		nvarchar(250),
	@title			nvarchar(200),
	@content		nvarchar(4000)
)
AS
SET NOCOUNT ON

UPDATE user_mail
SET title = @title,
	content = @content,
	created_date = GETDATE()
WHERE id = @mail_id

UPDATE user_mail_sender
SET receiver_name_list = @receiver_name_list,
	send_date = GETDATE()
WHERE 
	mail_id = @mail_id AND 
	sender_id = @char_id
GO

/********************************************
lin_ModItemOwner
	
INPUT
	@item_id	INT,
	@new_char_id	INT,
	@new_warehouse	INT
OUTPUT
return
made by
	young
date
	2003-11-07
********************************************/
ALTER PROCEDURE [DBO].[lin_ModItemOwner]
(
@item_id	INT,
@new_char_id 	INT,
@new_warehouse	INT

)
AS
SET NOCOUNT ON

UPDATE user_item  set char_id=@new_char_id,  warehouse=@new_warehouse WHERE item_id=@item_id
GO

-- 2005-11-16	created by btwinuni
-- 2005-11-23	modified by btwinuni	check nobless
ALTER PROCEDURE dbo.lin_ModOlympiadPoint
(
	@char_id	int,
	@previous_point	int,
	@olympiad_point	int,
	@mod_type	int
)
AS

declare
	@old_previous_point	int,
	@old_olympiad_point	int,
	@new_previous_point	int,
	@new_olympiad_point	int

select @old_previous_point = previous_point, @old_olympiad_point = olympiad_point
from user_nobless (nolock)
where char_id = @char_id

-- is not nobless
if @old_previous_point is null or @old_olympiad_point is null
return

if @mod_type = 1	-- relative
begin
	set @new_previous_point = @old_previous_point + @previous_point
	if @new_previous_point < 0
		set @new_previous_point = 0

	set @new_olympiad_point = @old_olympiad_point + @olympiad_point
	if @new_olympiad_point < 0
		set @new_olympiad_point = 0
end
else		-- absolute
begin
	set @new_previous_point = @previous_point
	if @new_previous_point < 0
		set @new_previous_point = 0

	set @new_olympiad_point = @olympiad_point
	if @new_olympiad_point < 0
		set @new_olympiad_point = 0
end

update user_nobless
set previous_point = @new_previous_point,
	olympiad_point = @new_olympiad_point
where char_id = @char_id
GO

/********************************************
lin_ModSubJobAbility
	
INPUT
	@char_id	int,
	@subjob_id	int,
	@sp		int,
	@exp		int,
	@lev		int
OUTPUT
return
made by
	kks
date
	2005-01-18
modified by
	kernel0
date	2005-04-20	
********************************************/
ALTER PROCEDURE [DBO].[lin_ModSubJobAbility]
(
	@char_id	int,
	@subjob_id	int,
	@sp		int,
	@exp		int,
	@lev		int
)
AS
SET NOCOUNT ON

update user_subjob set sp = sp + @sp, exp = exp + @exp,  level = @lev where char_id = @char_id and subjob_id = @subjob_id
GO

/********************************************
lin_ModSubJobAbilityAbsolutely
	
INPUT
	@char_id	int,
	@subjob_id	int,
	@sp		int,
	@exp		int,
	@lev		int
OUTPUT
return
made by
	kks
date
	2005-01-18
********************************************/
ALTER PROCEDURE [DBO].[lin_ModSubJobAbilityAbsolutely]
(
	@char_id	int,
	@subjob_id	int,
	@sp		int,
	@exp		int,
	@lev		int
)
AS
SET NOCOUNT ON

update user_subjob set sp =  @sp, exp = @exp,  level =  @lev where char_id = @char_id and subjob_id = @subjob_id
GO

/********************************************
lin_ModSubJobClass
	
INPUT
	@char_id	int,
	@subjob_id	int,
	@class		int
OUTPUT
return
made by
	kks
date
	2005-01-18
	2005-09-07	modified by btwinuni
********************************************/
ALTER PROCEDURE [DBO].[lin_ModSubJobClass]
(
	@char_id	int,
	@subjob_id	int,
	@class		int
)
AS
SET NOCOUNT ON

--update user_subjob set class=@class where char_id = @char_id and subjob_id = @subjob_id
declare @sql varchar(1000)
set @sql = 'update user_data '
	+ ' set subjob' + cast(@subjob_id as varchar) + '_class = ' + cast(@class as varchar)
	+ ' where char_id = ' + cast(@char_id as varchar)
exec (@sql)
GO

ALTER PROCEDURE [dbo].lin_MoveCharacter
(
	@player_id INT
)
AS
SET NOCOUNT ON

DECLARE @pledge_id INT
declare @alliance_id	int

SELECT @pledge_id = pledge_id  FROM user_data WHERE char_id = @player_id

IF @pledge_id <> 0
BEGIN
	DECLARE @ruler_id INT
	SELECT @ruler_id = ruler_id FROM pledge WHERE pledge_id = @pledge_id

--	IF @ruler_id = @player_id -- ???? ?? ?? ??
--	BEGIN
--		DELETE FROM pledge WHERE pledge_id = @pledge_id
--	END
	UPDATE user_data SET pledge_id = 0 , nickname = null WHERE char_id = @player_id
END
GO

/********************************************
lin_MoveCharClear
	clear moved character
INPUT

OUTPUT

return
made by
	young
date
	2003-7-30
********************************************/
ALTER PROCEDURE [DBO].[lin_MoveCharClear]
	@world_id	int
AS

SET NOCOUNT ON

-- set character as non-playable
-- update user_data set old_account_id = account_id 
-- where account_id > 0 and char_id in ( select old_char_id from lin2comm.dbo.req_charmove (nolock) where old_world_id = @world_id )

update user_data set account_id = -3
where char_id in ( select old_char_id from dbo.req_charmove (nolock) where old_world_id = @world_id )

-- update pledge info
declare @old_char_id int
declare @old_char_name nvarchar(50)

DECLARE move_cursor CURSOR FOR 
SELECT old_char_id, old_char_name
FROM dbo.req_charmove (nolock)
WHERE old_world_id = @world_id

OPEN move_cursor 

FETCH NEXT FROM move_cursor  
INTO @old_char_id, @old_char_name

WHILE @@FETCH_STATUS = 0
BEGIN
	select @old_char_id, @old_char_name
	exec lin_MoveCharacter @old_char_id

	-- Get the next 
	FETCH NEXT FROM move_cursor 
	INTO @old_char_id, @old_char_name

END

close move_cursor 
deallocate move_cursor
GO

ALTER PROCEDURE dbo.lin_MoveCharCopyChar
(
	@world_id	varchar(5)
)
AS


declare @sql varchar(1024)
declare @conn_str varchar(256)

set @conn_str = '127.0.0.' + @world_id + ''';''sa'';''LjweikT157'


set @sql = ' insert into req_charmove ( old_char_name, old_char_id, account_name, account_id,  old_world_id, new_world_id, new_char_name ) select R1.char_name, char_id, account_name, account_id, ' + @world_id + ' , 100, R1.char_name + ''-' + @world_id + '''   from ( select * from '
	+ ' OPENROWSET ( ''SQLOLEDB'', ''' + @conn_str + ''', '' select char_id, char_name, account_id, account_name from lin2world.dbo.user_data (nolock)  '' ) )  as R1 '
	+ ' left join ( select * from req_char (nolock) where server_id = ' + @world_id + ' ) as R2 '
	+ ' on R1.char_name = R2.char_name '
	+ ' where server_id is not null '
exec ( @sql )
GO

ALTER PROCEDURE dbo.lin_MoveCharCopyPledge
(
	@world_id	varchar(5)
)
AS


declare @sql varchar(1024)
declare @conn_str varchar(256)

set @conn_str = '127.0.0.' + @world_id + ''';''sa'';''LjweikT157'

set @sql = ' insert into req_charmove ( old_char_name, old_char_id, account_name, account_id,  old_world_id, new_world_id, new_char_name ) select R1.char_name, char_id, account_name, account_id, ' + @world_id + ' , 100, R1.char_name + ''-' + @world_id + '''   from ( select * from '
	+ ' OPENROWSET ( ''SQLOLEDB'', ''' + @conn_str + ''', '' select pledge_id, char_id, char_name, account_id, account_name from lin2world.dbo.user_data (nolock) where account_id > 0  '' ) )  as R1 '
	+ ' left join ( select * from req_pledge (nolock) where server_id = ' + @world_id + ' ) as R2 '
	+ ' on R1.pledge_id = R2.pledge_id '
	+ ' where server_id is not null '

exec ( @sql )
GO

ALTER PROCEDURE dbo.lin_MoveCharPlg
(
	@world_id	varchar(5)
)
AS


declare @sql varchar(1024)
declare @conn_str varchar(256)

set @conn_str = '127.0.0.' + @world_id + ''';''sa'';''LjweikT157'

set @sql = ' insert into pledge ( old_world_id, old_pledge_id,  old_ruler_id, ruler_id, name, alliance_id, challenge_time, root_name_value, now_war_id, oust_time, skill_level, castle_id, agit_id, rank, name_value, status, private_flag, crest_id, is_guilty, dismiss_reserved_time, alliance_ousted_time, alliance_withdraw_time, alliance_dismiss_time )  '
	+ ' select ' + @world_id + '  , R1.pledge_id, ruler_id, 0, name + ''-' + @world_id + ''' , 0, 0, 0, 0 ,0 ,0 , 0, 0, 0, name_value, 0, 0, 0, 0, 0, 0, 0, 0  from ( select * from '
	+ ' OPENROWSET ( ''SQLOLEDB'', ''' + @conn_str + ''', '' select * from lin2world.dbo.pledge (nolock)  '' ) )  as R1 '
	+ ' left join ( select * from req_pledge (nolock) where server_id = ' + @world_id + ' ) as R2 '
	+ ' on R1.pledge_id = R2.pledge_id '
	+ ' where server_id is not null '

exec ( @sql )
GO

-- drop procedure lin_MoveCharSvr

/********************************************
lin_MoveCharSvr
	move char
INPUT
	@world_id	int
OUTPUT

return
made by
	young
date
	2003-7-30
	2003-10-06
	2004-1-6
********************************************/
ALTER PROCEDURE [DBO].[lin_MoveCharSvr]
(
	@old_world_id		int,
	@new_world_id		int,
	@conn_str		varchar(100)
)
AS

SET NOCOUNT ON

declare @sql varchar(4000)

--	& ' OPENROWSET ( 'SQLOLEDB', '127.0.0.1';'sa';'sa', ' 


-- make user_data 

set @sql = ' insert into user_data ( ' 
	+ ' char_name, pledge_id, account_name, account_id, builder, gender, race, class, world, xloc, yloc, zloc,  ' 
	+ ' IsInVehicle, HP, MP, SP, Exp, Lev, align, PK, PKpardon, Duel, ST_underware, ST_right_ear, ST_left_ear, ' 
	+ ' ST_neck, ST_right_finger, ST_left_finger, ST_head, ST_right_hand, ST_left_hand, ST_gloves, ST_chest,  ' 
	+ ' ST_feet, ST_back, ST_both_hand, ST_legs, create_date, login, logout, quest_flag, power_flag, max_hp, max_mp, ' 
	+ ' quest_memo, face_index, hair_color_index,  hair_shape_index, use_time, drop_exp,  ' 
	+ ' surrender_war_id, pledge_withdraw_time, pledge_ousted_time, pledge_leave_status, pledge_leave_time, ' 
	+ ' pledge_dismiss_time , old_pledge_id, old_char_id ' 
	+ ' ) ' 
	+ ' select R2.new_char_name, 0, account_name, R1.account_id, builder, gender, race, class, world, xloc, yloc, zloc,  ' 
	+ ' IsInVehicle, HP, MP, SP, Exp, Lev, align, PK, PKpardon, Duel, 0, 0, 0, ' 
	+ ' 0, 0, 0, 0, 0, 0, 0, 0,  ' 
	+ ' 0, 0, 0, 0,  create_date, login, logout, quest_flag, power_flag, max_hp, max_mp, ' 
	+ ' quest_memo, face_index, hair_color_index, hair_shape_index, use_time, drop_exp,  ' 
	+ ' 0, 0, 0, 0, 0, ' 
	+ '  0 , pledge_id, char_id ' 
	+ ' from ' 
	+ ' ( ' 
	+ ' select pledge_id, char_id, account_name, account_id, builder, gender, race, class, world, xloc, yloc, zloc,  ' 
	+ ' IsInVehicle, HP, MP, SP, Exp, Lev, align, PK, PKpardon, Duel, ST_underware, ST_right_ear, ST_left_ear, ' 
	+ ' ST_neck, ST_right_finger, ST_left_finger, ST_head, ST_right_hand, ST_left_hand, ST_gloves, ST_chest,  ' 
	+ ' ST_feet, ST_back, ST_both_hand, ST_legs,  create_date, login, logout, quest_flag, power_flag, max_hp, max_mp, ' 
	+ ' quest_memo, face_index, hair_color_index, hair_shape_index, use_time, drop_exp,  ' 
	+ ' surrender_war_id, pledge_withdraw_time, pledge_ousted_time, pledge_leave_status, pledge_leave_time, ' 
	+ ' pledge_dismiss_time ' 
	+ ' from ' 
	+ ' OPENROWSET ( ''SQLOLEDB'', ''' + @conn_str + ''', ' 
	+ ' '' select * from lin2world.dbo.user_data (nolock) '' ) ' 
	+ ' where char_id in ( select old_char_id from req_charmove (nolock) where old_world_id = ' + CAST ( @old_world_id as VARCHAR) + ' and new_world_id = ' + CAST ( @new_world_id as varchar) + ' ) ' 
	+ ' ) as R1 ' 
	+ ' left join ( select new_char_name, account_id, old_char_id from req_charmove (nolock) where old_world_id = ' + CAST ( @old_world_id as varchar) + ' and new_world_id = ' + cast ( @new_world_id  as varchar ) + ' )  ' 
	+ ' as R2 ' 
	+ ' on R1.char_id = R2.old_char_id ' 

select @sql

exec (@sql)

-- update char_id
set @sql = ' update r '
	+ ' set r.new_char_id = u.char_id '
	+ ' from req_charmove as r  inner join user_data as u '
	+ ' on r.new_char_name = u.char_name '
	+ ' where r.old_world_id = ' +  CAST ( @old_world_id as VARCHAR)  + ' and r.new_world_id = ' + CAST ( @new_world_id as varchar)   + '  and u.account_id > 0  '

exec (@sql)

-- update pledge in user_data table
set @sql = ' update r '
	+ ' set r.pledge_id = u.pledge_id '
	+ ' from user_data as r  inner join pledge as u '
	+ ' on r.old_pledge_id = u.old_pledge_id '
	+ ' where r.char_name like ''%-' +  CAST ( @old_world_id as VARCHAR)  + '''  and u.pledge_id is not null  '
select @sql

exec (@sql)


-- update pledge owner in pledge table
set @sql = ' update r '
	+ ' set r.ruler_id = u.char_id'
	+ ' from pledge as r  inner join user_data as u '
	+ ' on r.old_ruler_id = u.old_char_id '
	+ ' where r.old_world_id = ' +  CAST ( @old_world_id as VARCHAR)  + '  and u.char_id is not null  '

exec (@sql)


-- copy user_item
set @sql = ' insert into user_item (char_id, item_type, amount, enchant, eroded, bless,  ident, wished, warehouse, old_world_id, old_item_id ) '
	+ ' select R2.new_char_id, R1.item_type, R1.amount, R1.enchant, R1.eroded, R1.bless, R1. ident, R1.wished, R1.warehouse, ' + CAST ( @old_world_id as varchar ) + ' , R1.item_id '
	+ ' from '
	+ ' ( '
	+ ' select * from '
	+ ' OPENROWSET ( ''SQLOLEDB'', ''' + @conn_str + ''', '
	+ ' '' select * from lin2world.dbo.user_item (nolock) '' ) '
	+ ' where char_id in ( select old_char_id from req_charmove (nolock) where old_world_id = ' +  CAST ( @old_world_id as VARCHAR) + ' and new_world_id = ' + CAST ( @new_world_id as varchar) + '  ) '
	+ ' ) as R1 '
	+ ' left join ( select new_char_id , old_char_id from req_charmove (nolock) where old_world_id = ' +   CAST ( @old_world_id as VARCHAR) + ' and new_world_id = ' + CAST ( @new_world_id as varchar) + ' ) '
	+ ' as R2 '
	+ ' on R1.char_id = R2.old_char_id '
	+ ' where new_char_id is not null '

exec (@sql)


-- copy user_skill
set @sql = ' insert into user_skill ( char_id, skill_id, skill_lev, to_end_time) '
	+ ' select R2.new_char_id, R1.skill_id, R1.skill_lev, R1.to_end_time ' 
	+ ' from ' 
	+ ' ( ' 
	+ ' select * from  ' 
	+ ' OPENROWSET ( ''SQLOLEDB'', ''' + @conn_str + ''', ' 
	+ ' '' select * from lin2world.dbo.user_skill (nolock) '' ) ' 
	+ ' where char_id in ( select old_char_id from req_charmove (nolock) where old_world_id = ' +   CAST ( @old_world_id as VARCHAR) + ' and new_world_id = ' + CAST ( @new_world_id as varchar) + ' ) ' 
	+ ' ) as R1 ' 
	+ ' left join ( select new_char_id , old_char_id from req_charmove (nolock) where old_world_id = ' +  CAST ( @old_world_id as VARCHAR) + ' and new_world_id = ' + CAST ( @new_world_id as varchar) + ' ) ' 
	+ ' as R2 ' 
	+ ' on R1.char_id = R2.old_char_id ' 
	+ ' where new_char_id is not null ' 
exec (@sql)


-- copy quest
set @sql = ' insert into quest ( char_id, q1, s1, q2, s2, q3, s3, q4, s4, q5, s5, q6, s6, q7, s7, q8, s8, q9, s9, q10, s10, q11, s11, q12, s12, q13, s13, q14, s14, q15, s15, q16, s16) ' 
	+ ' select R2.new_char_id, q1, s1, q2, s2, q3, s3, q4, s4, q5, s5, q6, s6, q7, s7, q8, s8, q9, s9, q10, s10, q11, s11, q12, s12, q13, s13, q14, s14, q15, s15, q16, s16 ' 
	+ ' from ' 
	+ ' ( ' 
	+ ' select * from  ' 
	+ ' OPENROWSET ( ''SQLOLEDB'', ''' + @conn_str + ''', ' 
	+ ' '' select * from lin2world.dbo.quest (nolock) '' ) ' 
	+ ' where char_id in ( select old_char_id from req_charmove (nolock) where old_world_id = ' +  CAST ( @old_world_id as VARCHAR) + ' and new_world_id = ' + CAST ( @new_world_id as varchar) + ' ) ' 
	+ ' ) as R1 ' 
	+ ' left join ( select new_char_id , old_char_id from req_charmove (nolock) where old_world_id = ' +  CAST ( @old_world_id as VARCHAR) + ' and new_world_id = ' + CAST ( @new_world_id as varchar) + ' ) ' 
	+ ' as R2 ' 
	+ ' on R1.char_id = R2.old_char_id ' 
	+ ' where new_char_id is not null ' 

exec (@sql)


-- copy user_history
set @sql = ' insert into user_history( char_name, char_id, log_date, log_action, account_name, create_date) ' 
	+ ' select R2.new_char_name, R2.new_char_id, R1.log_date, R1.log_action, R1.account_name, R1.create_date ' 
	+ ' from ' 
	+ ' ( ' 
	+ ' select * from  ' 
	+ ' OPENROWSET ( ''SQLOLEDB'', ''' + @conn_str + ''', ' 
	+ ' '' select * from lin2world.dbo.user_history (nolock) '' ) ' 
	+ ' where char_id in ( select old_char_id from req_charmove (nolock) where old_world_id = ' +   CAST ( @old_world_id as VARCHAR) + ' and new_world_id = ' + CAST ( @new_world_id as varchar) + ' ) ' 
	+ ' ) as R1 ' 
	+ ' left join ( select new_char_name, new_char_id , old_char_id from req_charmove (nolock) where old_world_id = ' +   CAST ( @old_world_id as VARCHAR) + ' and new_world_id = ' + CAST ( @new_world_id as varchar) + ' ) ' 
	+ ' as R2 ' 
	+ ' on R1.char_id = R2.old_char_id ' 
	+ ' where new_char_id is not null ' 

exec (@sql)


-- copy user_log
set @sql = ' insert into user_log  ( char_id, log_id, log_date, log_from, log_to, use_time ) '
	+ ' select R2.new_char_id, log_id, log_date, log_from, log_to, use_time  ' 
	+ ' from ' 
	+ ' ( ' 
	+ ' select * from  ' 
	+ ' OPENROWSET ( ''SQLOLEDB'', ''' + @conn_str + ''', ' 
	+ ' '' select * from lin2world.dbo.user_log  (nolock) '' ) ' 
	+ ' where char_id in ( select old_char_id from req_charmove (nolock) where old_world_id = ' +   CAST ( @old_world_id as VARCHAR) + ' and new_world_id = ' + CAST ( @new_world_id as varchar)  + ' ) ' 
	+ ' ) as R1 ' 
	+ ' left join ( select new_char_id , old_char_id from req_charmove (nolock) where old_world_id = ' +   CAST ( @old_world_id as VARCHAR) + '  and new_world_id = ' + CAST ( @new_world_id as varchar)  + '  ) ' 
	+ ' as R2 ' 
	+ ' on R1.char_id = R2.old_char_id ' 
	+ ' where new_char_id is not null ' 
exec (@sql)


-- shortcut_data
set @sql = ' insert into shortcut_data ( char_id, slotnum, shortcut_type, shortcut_id, shortcut_macro ) ' 
	+ ' select R2.new_char_id, slotnum, shortcut_type, shortcut_id, shortcut_macro ' 
	+ ' from ' 
	+ ' ( ' 
	+ ' select * from  ' 
	+ ' OPENROWSET ( ''SQLOLEDB'', ''' + @conn_str + ''', ' 
	+ ' '' select * from lin2world.dbo.shortcut_data (nolock) '' ) ' 
	+ ' where char_id in ( select old_char_id from req_charmove (nolock) where old_world_id = ' +   CAST ( @old_world_id as VARCHAR)  + ' and new_world_id = ' + CAST ( @new_world_id as varchar) + ' ) ' 
	+ ' ) as R1 ' 
	+ ' left join ( select new_char_id , old_char_id from req_charmove (nolock) where old_world_id = ' +  CAST ( @old_world_id as VARCHAR) + ' and new_world_id = ' + CAST ( @new_world_id as varchar)  + ' ) ' 
	+ ' as R2 ' 
	+ ' on R1.char_id = R2.old_char_id ' 
	+ ' where new_char_id is not null ' 
exec (@sql)


-- user_comment
set @sql = ' insert into user_comment ( char_name, char_id, comment, comment_date, writer, deleted ) ' 
	+ ' select R2.new_char_name, R2.new_char_id, comment, comment_date, writer, deleted  ' 
	+ ' from ' 
	+ ' ( ' 
	+ ' select * from  ' 
	+ ' OPENROWSET ( ''SQLOLEDB'', ''' + @conn_str + ''', ' 
	+ ' '' select * from lin2world.dbo.user_comment  (nolock) '' ) ' 
	+ ' where char_id in ( select old_char_id from req_charmove (nolock) where old_world_id = ' +   CAST ( @old_world_id as VARCHAR)  + ' and new_world_id = ' + CAST ( @new_world_id as varchar) + ' ) ' 
	+ ' ) as R1 ' 
	+ ' left join ( select new_char_name, new_char_id , old_char_id from req_charmove (nolock) where old_world_id = ' +  CAST ( @old_world_id as VARCHAR) + ' and new_world_id = ' + CAST ( @new_world_id as varchar)  + ' ) ' 
	+ ' as R2 ' 
	+ ' on R1.char_id = R2.old_char_id ' 
	+ ' where new_char_id is not null ' 
exec (@sql)


-- pet_data
set @sql = ' insert into pet_data ( pet_id, npc_class_id, expoint, nick_name, hp, mp, sp, meal  ) ' 
	+ ' select item_id, npc_class_id, expoint, null, hp, mp, sp, meal   ' 
	+ ' from ' 
	+ ' ( select * from user_item (nolock) where item_type = 2375 and old_world_id = ' + CAST ( @old_world_id as varchar ) + ' ) as R1 '
	+ ' left join '
	+ ' ( ' 
	+ ' select * from  ' 
	+ ' OPENROWSET ( ''SQLOLEDB'', ''' + @conn_str + ''', ' 
	+ ' '' select pet_id, npc_class_id, expoint, hp, mp, sp, meal  from lin2world.dbo.pet_data (nolock) '' ) ' 
	+ ' ) as R2 ' 
	+ ' on R1.old_item_id = R2.pet_id '
	+ ' where R2.npc_class_id is not null ' 
exec (@sql)



-- user_item owned by pet
set @sql = ' insert into user_item( char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse  ) ' 
	+ ' select R2.item_id, R1.item_type, R1.amount, R1.enchant, R1.eroded, R1.bless, R1.ident, R1.wished, R1.warehouse  ' 
	+ ' from ' 
	+ ' ( ' 
	+ ' select * from  ' 
	+ ' OPENROWSET ( ''SQLOLEDB'', ''' + @conn_str + ''', ' 
	+ ' '' select * from lin2world.dbo.user_item (nolock) where warehouse = 5 '' ) ' 
	+ ' ) as R1 ' 
	+ ' left join '
	+ ' ( select * from user_item (nolock) where old_world_id = ' + CAST ( @old_world_id as varchar ) + ' ) as R2 '
	+ ' on R1.char_id = R2.old_item_id '
	+ ' where R2.item_id is not null '

exec (@sql)
GO

ALTER PROCEDURE dbo.lin_NewNobless
(
@char_id INT,
@nobless_type INT,
@olympiad_point INT
)
AS
SET NOCOUNT ON

INSERT user_nobless
(char_id,  nobless_type, olympiad_point)
VALUES
(@char_id, @nobless_type, @olympiad_point)

SELECT @@ROWCOUNT
GO

ALTER PROCEDURE dbo.lin_NewTeamBattleAgitMember
(
	@agit_id INT,
	@char_id INT,
	@pledge_id INT,
	@propose_time INT
)
AS
SET NOCOUNT ON

DECLARE @ret INT

INSERT INTO team_battle_agit_member
(agit_id, char_id, pledge_id, propose_time)
VALUES
(@agit_id, @char_id, @pledge_id, @propose_time)

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @ret = 1
END
ELSE
BEGIN
	SELECT @ret = 0
END

SELECT @ret
GO

ALTER PROCEDURE dbo.lin_NewTeamBattleAgitPledge
(
	@agit_id INT,
	@pledge_id INT,
	@char_id INT,
	@propose_time INT,
	@color INT,
	@npc_type INT
)
AS
SET NOCOUNT ON

DECLARE @ret INT

BEGIN TRAN

INSERT INTO team_battle_agit_pledge
(agit_id, pledge_id, propose_time, color, npc_type)
VALUES
(@agit_id, @pledge_id, @propose_time, @color, @npc_type)

IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
BEGIN
	SELECT @ret = 0
	GOTO EXIT_TRAN
END

INSERT INTO team_battle_agit_member
(agit_id, char_id, pledge_id, propose_time)
VALUES
(@agit_id, @char_id, @pledge_id, @propose_time)

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @ret = 1
END
ELSE
BEGIN
	SELECT @ret = 0
END

EXIT_TRAN:

IF @ret <> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END

SELECT @ret
GO

ALTER PROCEDURE dbo.lin_NominateHeroes
(
@now_season INT,
@new_step INT,
@new_season_start_time INT
)
AS
SET NOCOUNT ON

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

/****** Object:  Stored Procedure dbo.lin_OustMemberPledge    Script Date: 2003-09-20 ?? 11:51:59 ******/
-- lin_OustMemberPledge
-- by bert
-- return Result(0 if failed)

ALTER PROCEDURE
dbo.lin_OustMemberPledge (@alliance_id INT, @member_pledge_id INT, @oust_time INT)
AS

SET NOCOUNT ON

DECLARE @result INT

BEGIN TRAN

UPDATE alliance
SET oust_time = @oust_time
WHERE id = @alliance_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @result = 1
END
ELSE
BEGIN
	SELECT @result = 0
	GOTO EXIT_TRAN
END

UPDATE pledge
SET alliance_id = 0, alliance_ousted_time = @oust_time
WHERE pledge_id = @member_pledge_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @result = 1
END
ELSE
BEGIN
	SELECT @result = 0
END

EXIT_TRAN:
IF @result <> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END

SELECT @result
GO

/****** Object:  Stored Procedure dbo.lin_OustPledgeMember    Script Date: 2003-09-20 ?? 11:51:57 ******/
-- lin_OustPledgeMember
-- by bert
-- return ousted member id
ALTER PROCEDURE [DBO].[lin_OustPledgeMember]
(
	@pledge_id	INT,
	@char_name	NVARCHAR(50)
)
AS

DECLARE @ret INT
DECLARE @char_id INT

SELECT @char_id = char_id
FROM user_data
WHERE char_name = @char_name

IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
BEGIN
	SELECT @ret = 0
	GOTO EXIT_PROC
END

SET NOCOUNT ON

BEGIN TRAN

UPDATE user_data
SET pledge_id = 0
WHERE char_id = @char_id AND pledge_id = @pledge_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @ret = @char_id
	-- ???? ??? ??? ????.
END
ELSE
BEGIN
	SELECT @ret = 0
	GOTO EXIT_TRAN
END

EXIT_TRAN:
IF @ret<> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END

EXIT_PROC:
SELECT @ret
GO

/****** Object:  Stored Procedure dbo.lin_ReadBbsall    Script Date: 2003-09-20 ?? 11:51:57 ******/
ALTER PROCEDURE [DBO].[lin_ReadBbsall] 
(
	@nId	INT
)
AS

SET NOCOUNT ON

select 
	cnt, orderedTitle.title, orderedTitle.contents, orderedTitle.writer, cast( orderedTitle.cdate as smalldatetime)
from 
	(Select 
		count(bbs2.id) as cnt, bbs1.id, bbs1.title, bbs1.contents, bbs1.writer, bbs1.cdate
	from 
		Bbs_all as bbs1
		inner join
		Bbs_all as bbs2
		on bbs1.id <= bbs2.id
	group by 
		bbs1.id, bbs1.title, bbs1.contents, bbs1.writer, bbs1.cdate
	) as orderedTitle
where
	orderedTitle.id = @nId

IF @@rowcount = 1  
	UPDATE Bbs_all SET nRead = nRead + 1 WHERE id = @nId
Else
	RAISERROR ('Unavailable id.', 16, 1)
GO

/****** Object:  Stored Procedure dbo.lin_ReadBbsTGS    Script Date: 2003-09-20 ?? 11:51:57 ******/
ALTER PROCEDURE [DBO].[lin_ReadBbsTGS] 
(
	@nId	INT
)
AS

SET NOCOUNT ON

select id, title, contents, writer, cast( cdate as smalldatetime)
from bbs_tgs (nolock)
where id = @nId

update bbs_tgs set nRead = nRead + 1 where id = @nId
GO

/****** Object:  Stored Procedure dbo.lin_RefreshCharacterByName    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_RefreshCharacterByName
	
INPUT
	@CharName		NVARCHAR(50)
OUTPUT
return
made by
	carrot
date
	2002-06-10
********************************************/
ALTER PROCEDURE [DBO].[lin_RefreshCharacterByName]
(
	@CharName		NVARCHAR(50)
)
AS
SET NOCOUNT ON

SELECT 
	char_id, account_name, account_id, pledge_id, builder, gender, race, class, world, xloc, yloc, zloc, 
	HP, MP, SP, Exp, Lev, align, PK, duel, pkpardon, 
	ST_underware, ST_right_ear, ST_left_ear, ST_neck, ST_right_finger, ST_left_finger, ST_head, ST_right_hand, ST_left_hand, ST_gloves, ST_chest, ST_legs, ST_feet, ST_back, ST_both_hand 
FROM user_data (nolock)  
WHERE char_name = @CharName
GO

/******************************************************************************
#Name:	lin_RegisterAccount
#Desc:	add account to account_ch2 table

#Argument:
	Input:	@account_name	VARCHAR(50)
	Output:
#Return:
#Result Set:

#Remark:
#Example:
#See:
#History:
	Create	btwinuni	2005-09-12
******************************************************************************/
ALTER PROCEDURE [dbo].[lin_RegisterAccount]
(
	@account_name	nvarchar(50)
)
AS

SET NOCOUNT ON

-- if there is not account_ch2 table, end procedure
if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[account_ch2]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
begin
	return
end

-- account duplication check
if not exists (select * from account_ch2 where account = @account_name)
begin
	insert into account_ch2 values (@account_name)
end
GO

ALTER PROCEDURE dbo.lin_RegisterSiegeAgitPledge
(
	@agit_id INT,
	@pledge_id INT,
	@propose_time INT
)
AS
SET NOCOUNT ON

DECLARE @ret INT

INSERT INTO siege_agit_pledge
(agit_id, pledge_id, propose_time)
VALUES
(@agit_id, @pledge_id, @propose_time)

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @ret = 1
END
ELSE
BEGIN
	SELECT @ret = 0
END

SELECT @ret
GO

ALTER PROCEDURE dbo.lin_ReloadOlympiadPoint
(
@char_id INT
)
AS
SELECT olympiad_point FROM user_nobless WHERE char_id = @char_id
GO

-- lin_RenewAgitDeco
-- by bert
-- return (1, 0)
ALTER PROCEDURE dbo.lin_RenewAgitDeco
(
	@agit_id INT,
	@type INT,
	@expire INT
)
AS
SET NOCOUNT ON

UPDATE agit_deco
SET expire = @expire
WHERE agit_id = @agit_id AND type = @type

SELECT @@ROWCOUNT
GO

ALTER PROCEDURE dbo.lin_RenewSubjob
(
	@char_id		INT,
	@subjob_id		TINYINT,
	@new_class		TINYINT,
	@old_subjob_id 		TINYINT,
	@hp				FLOAT,
	@mp				FLOAT,
	@sp				INT,
	@exp			INT,
	@level			TINYINT,
	@henna_1		INT,
	@henna_2		INT,
	@henna_3		INT
)
AS

SET NOCOUNT ON

DECLARE @ret INT
SELECT @ret = 0
-- transaction on
BEGIN TRAN

IF (@subjob_id != @old_subjob_id)	-- save now info
BEGIN
	UPDATE user_subjob
	SET hp = @hp, mp = @mp, sp = @sp, exp = @exp, level = @level, 
		henna_1 = @henna_1, henna_2 = @henna_2, henna_3 = @henna_3
	WHERE char_id = @char_id AND subjob_id = @old_subjob_id

	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1	-- update, insert check
	BEGIN
		GOTO EXIT_TRAN
	END		
END


DELETE FROM shortcut_data WHERE char_id =  @char_id AND subjob_id = @subjob_id
IF @@ERROR <> 0
BEGIN
	GOTO EXIT_TRAN	
END

DELETE FROM user_henna WHERE char_id =  @char_id AND subjob_id = @subjob_id
IF @@ERROR <> 0
BEGIN
	GOTO EXIT_TRAN	
END

DELETE FROM user_skill WHERE char_id =  @char_id AND subjob_id = @subjob_id
IF @@ERROR <> 0
BEGIN
	GOTO EXIT_TRAN	
END

IF @subjob_id = 1
	UPDATE user_data SET subjob1_class = @new_class WHERE char_id = @char_id
ELSE IF @subjob_id = 2
	UPDATE user_data SET subjob1_class = @new_class WHERE char_id = @char_id
ELSE IF @subjob_id = 3
	UPDATE user_data SET subjob1_class = @new_class WHERE char_id = @char_id

IF @@ERROR <> 0 OR @@ROWCOUNT <> 1	-- update, insert check
BEGIN
	SELECT @ret = 0
	GOTO EXIT_TRAN
END

-- update new class
UPDATE user_subjob
SET hp = NULL, mp = NULL, sp = NULL, exp = NULL, level = NULL, henna_1 = NULL, henna_2 = NULL, henna_3 = NULL, create_date = GETDATE()
WHERE char_id = @char_id AND subjob_id = @subjob_id
IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @ret = 1
END

EXIT_TRAN:
IF @ret<> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END

SELECT @ret
GO

-- lin_ResetAgitDeco
-- by bert
-- return (1, 0)
ALTER PROCEDURE dbo.lin_ResetAgitDeco
(
	@agit_id INT,
	@type INT
)
AS
SET NOCOUNT ON

DELETE FROM agit_deco
WHERE agit_id = @agit_id AND type = @type

SELECT @@ROWCOUNT
GO

ALTER PROCEDURE dbo.lin_ResetOlympiadPoint
(
@give_point INT
)
AS
SET NOCOUNT ON

UPDATE user_nobless
SET previous_point = olympiad_point

UPDATE user_nobless
SET olympiad_point = @give_point
GO

/********************************************        
lin_ResetSociality
 set sociality
INPUT        
 @char_id
 @sociality
 @sulffrage
 @last_changed
OUTPUT        
return        
       
made by        
 carrot        
date        
 2004-07-4
change        
********************************************/        
ALTER PROCEDURE [DBO].[lin_ResetSociality]
(        
 @char_id INT,
 @sociality INT,
 @sulffrage INT,
 @last_changed DATETIME
)        
AS        
        
SET NOCOUNT ON        

IF EXISTS(SELECT  * FROM user_sociality WHERE char_id = @char_id)
BEGIN
	UPDATE user_sociality 
	SET sociality = @sociality , used_sulffrage = @sulffrage , last_changed = @last_changed
	WHERE char_id = @char_id
END
ELSE
BEGIN
	INSERT INTO user_sociality (char_id, sociality , used_sulffrage , last_changed ) VALUES (@char_id, @sociality , @sulffrage , @last_changed)
END
GO

/********************************************
lin_SetCharAccountId
	update account_id character
INPUT
	@account_id	int,
	@char_id	int
OUTPUT

return
made by
	young
date
	2003-09-02
	update account_id from character
********************************************/
ALTER PROCEDURE [DBO].[lin_RestoreChar ]
(
	@account_id	int,
	@char_id	int,
	@account_name	nvarchar(50) = ''
)
AS

SET NOCOUNT ON

declare @char_inx int
declare @old_char_name nvarchar(50)
declare @new_char_name nvarchar(50)

select @old_char_name = char_name from user_data (nolock) where char_id = @char_id
select @char_inx = CHARINDEX ( '_', @old_char_name )

if @char_inx > 0 
begin
	select @new_char_name = SUBSTRING( @old_char_name, 1, @char_inx - 1)
end
else
begin
	select @new_char_name = @old_char_name
end

if ( len ( @account_name ) > 0 ) 
begin
	update user_data set account_id = @account_id , char_name = @new_char_name, account_name = @account_name , temp_delete_date = null  where char_id = @char_id
end else begin
	update user_data set account_id = @account_id , char_name = @new_char_name , temp_delete_date = null  where char_id = @char_id
end
GO

/********************************************
lin_RestoreChar2
	restore deleted char
INPUT
	@account_id	int,
	@char_id	int
OUTPUT

return
made by
	young
date
	2004-03-11
	update account_id from character
********************************************/
ALTER PROCEDURE [DBO].[lin_RestoreChar2 ]
(
	@account_id	int,
	@char_id	int,
	@new_char_name	nvarchar(50),
	@account_name	nvarchar(50) = ''
)
AS

SET NOCOUNT ON

if ( len ( @account_name ) > 0 ) 
begin
	update user_data set account_id = @account_id , char_name = @new_char_name, account_name = @account_name , temp_delete_date = null  where char_id = @char_id
end else begin
	update user_data set account_id = @account_id , char_name = @new_char_name , temp_delete_date = null  where char_id = @char_id
end
GO

/********************************************      
lin_SaveCastleIncome      
       
INPUT       
 @crop_income int,      
 @shop_income int,      
 @castle_id int      
OUTPUT      
return      
made by      
 carrot      
date      
 2002-06-16      
change 2003-12-22 carrot    
 add about agit income    
change 2004-03-02 carrot    
 add taxincomeTemp   
 ********************************************/      
ALTER PROCEDURE [DBO].[lin_SaveCastleIncome]      
(      
 @crop_income int,      
 @shop_income int,      
 @castle_id int  ,    
 @crop_income_temp int,      
 @is_castle int    
)      
AS      
SET NOCOUNT ON      
      
if (@is_castle = 1) -- castle    
 UPDATE castle SET crop_income = @crop_income, shop_income = @shop_income, shop_income_temp = @crop_income_temp WHERE id = @castle_id      
else -- agit    
 UPDATE agit SET shop_income = @shop_income, shop_income_temp = @crop_income_temp WHERE id = @castle_id
GO

/****** Object:  Stored Procedure dbo.lin_SaveCastleSiegeElapsedTime    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SaveCastleSiegeElapsedTime
	
INPUT	
	@siege_elapsed_time	int,
	@castle_id	int
OUTPUT

return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_SaveCastleSiegeElapsedTime]
(
	@siege_elapsed_time	int,
	@castle_id	int
)
AS
SET NOCOUNT ON

UPDATE castle SET siege_elapsed_time = @siege_elapsed_time WHERE id = @castle_id
GO

/****** Object:  Stored Procedure dbo.lin_SaveCastleStatus    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SaveCastleStatus
	
INPUT	
	@status		int,
	@castle_id	int
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_SaveCastleStatus]
(
	@status		int,
	@castle_id	int
)
AS
SET NOCOUNT ON

UPDATE castle SET status = @status WHERE id = @castle_id
GO

ALTER PROCEDURE dbo.lin_SaveCharacter
(
	@subjob_id INT,
	@pledge_id INT,
	@class INT,
	@world INT,
	@xloc INT,
	@yloc INT,
	@zloc INT,
	@IsInVehicle INT,
	@HP FLOAT,
	@MP FLOAT,
	@max_HP FLOAT,
	@max_MP FLOAT,
	@SP INT,
	@Exp INT,
	@Lev INT,
	@align INT,
	@PK INT,
	@duel INT,
	@pkpardon INT,
	@ST_underware INT, 
	@ST_right_ear INT, 
	@ST_left_ear INT, 
	@ST_neck INT, 
	@ST_right_finger INT, 
	@ST_left_finger INT, 
	@ST_head INT, 
	@ST_right_hand INT, 
	@ST_left_hand INT, 
	@ST_gloves INT, 
	@ST_chest INT, 
	@ST_legs INT, 
	@ST_feet INT, 
	@ST_back INT, 
	@ST_both_hand INT, 
	@Face_Index INT, 
	@Hair_Shape_Index INT, 
	@Hair_Color_Index INT, 
	@ssq_dawn_round INT,
	@char_id  INT,
	@subjob0_class INT, 
	@subjob1_class INT, 
	@subjob2_class INT, 
	@subjob3_class INT, 
	@CP FLOAT,
	@max_CP FLOAT,
	@ST_hair INT
)
AS

SET NOCOUNT ON

UPDATE 
	user_data 
SET
	subjob_id = @subjob_id,
	class= @class,
	world= @world,
	xloc= @xloc,
	yloc= @yloc,
	zloc= @zloc,
	IsInVehicle= @IsInVehicle,
	HP= @HP,
	MP= @MP,
	max_HP= @max_HP,
	max_MP= @max_MP,
	SP= @SP,
	Exp= @Exp,
	Lev= @Lev,
	align= @align,
	PK= @PK,
	duel= @duel,
	pkpardon= @pkpardon,
	ST_underware= @ST_underware, 
	ST_right_ear= @ST_right_ear, 
	ST_left_ear= @ST_left_ear, 
	ST_neck= @ST_neck, 
	ST_right_finger= @ST_right_finger, 
	ST_left_finger= @ST_left_finger, 
	ST_head= @ST_head, 
	ST_right_hand= @ST_right_hand, 
	ST_left_hand= @ST_left_hand, 
	ST_gloves= @ST_gloves, 
	ST_chest= @ST_chest, 
	ST_legs= @ST_legs, 
	ST_feet= @ST_feet, 
	ST_back= @ST_back, 
	ST_both_hand= @ST_both_hand,
	Face_Index= @Face_Index, 
	Hair_Shape_Index= @Hair_Shape_Index, 
	Hair_Color_Index= @Hair_Color_Index ,
	ssq_dawn_round = @ssq_dawn_round,
	subjob0_class = @subjob0_class, 
	subjob1_class = @subjob1_class, 
	subjob2_class = @subjob2_class, 
	subjob3_class = @subjob3_class, 
	cp = @CP,
	max_cp = @max_CP,
	ST_hair = @ST_hair
WHERE 
	char_id= @char_id
GO

/****** Object:  Stored Procedure dbo.lin_SaveCharacter_temp    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SaveCharacter_temp

INPUT
OUTPUT
return
made by
	carrot
date
	2003-06-13
change
********************************************/
ALTER PROCEDURE [DBO].[lin_SaveCharacter_temp]
(
	@pledge_id INT,
	@class INT,
	@world INT,
	@xloc INT,
	@yloc INT,
	@zloc INT,
	@IsInVehicle INT,
	@HP FLOAT,
	@MP FLOAT,
	@max_HP FLOAT,
	@max_MP FLOAT,
	@SP INT,
	@Exp INT,
	@Lev INT,
	@align INT,
	@PK INT,
	@duel INT,
	@pkpardon INT,
	@char_id  INT
)
AS

SET NOCOUNT ON

UPDATE 
	user_data 
set 
	class= @class,
	world= @world,
	xloc= @xloc,
	yloc= @yloc,
	zloc= @zloc,
	IsInVehicle= @IsInVehicle,
	HP= @HP,
	MP= @MP,
	max_HP= @max_HP,
	max_MP= @max_MP,
	SP= @SP,
	Exp= @Exp,
	Lev= @Lev,
	align= @align,
	PK= @PK,
	duel= @duel,
	pkpardon= @pkpardon
WHERE 
	char_id= @char_id
GO

/****** Object:  Stored Procedure dbo.lin_SaveCharacter_temp2    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SaveCharacter_temp

INPUT
OUTPUT
return
made by
	carrot
date
	2003-06-13
change
********************************************/
ALTER PROCEDURE [DBO].[lin_SaveCharacter_temp2]
(
	@ST_underware INT, 
	@ST_right_ear INT, 
	@ST_left_ear INT, 
	@ST_neck INT, 
	@ST_right_finger INT, 
	@ST_left_finger INT, 
	@ST_head INT, 
	@ST_right_hand INT, 
	@ST_left_hand INT, 
	@ST_gloves INT, 
	@ST_chest INT, 
	@ST_legs INT, 
	@ST_feet INT, 
	@ST_back INT, 
	@ST_both_hand INT, 
	@char_id  INT
)
AS

SET NOCOUNT ON

UPDATE 
	user_data 
set 
	ST_underware= @ST_underware, 
	ST_right_ear= @ST_right_ear, 
	ST_left_ear= @ST_left_ear, 
	ST_neck= @ST_neck, 
	ST_right_finger= @ST_right_finger, 
	ST_left_finger= @ST_left_finger, 
	ST_head= @ST_head, 
	ST_right_hand= @ST_right_hand, 
	ST_left_hand= @ST_left_hand, 
	ST_gloves= @ST_gloves, 
	ST_chest= @ST_chest, 
	ST_legs= @ST_legs, 
	ST_feet= @ST_feet, 
	ST_back= @ST_back, 
	ST_both_hand= @ST_both_hand
WHERE 
	char_id= @char_id
GO

/****** Object:  Stored Procedure dbo.lin_SaveCharacterPledge    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SaveCharacterPledge

INPUT
OUTPUT
return
made by
	carrot
date
	2003-06-30
change
********************************************/
ALTER PROCEDURE [DBO].[lin_SaveCharacterPledge]
(
	@pledge_id INT,
	@char_id  INT
)
AS

SET NOCOUNT ON

UPDATE 
	user_data 
set 
	pledge_id= @pledge_id
WHERE 
	char_id= @char_id
GO

/****** Object:  Stored Procedure dbo.lin_SaveCropData    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SaveCropData
	create  or update castle crop data
INPUT
	@castle_id	INT,
	@item_type 	INT,
	@droprate 	INT,
	@price 	INT
OUTPUT
return
made by
	carrot
date
	2003-05-06
change 2003-06-11
	add level
********************************************/
ALTER PROCEDURE [DBO].[lin_SaveCropData]
(
	@castle_id	INT,
	@item_type 	INT,
	@droprate 	INT,
	@price 		INT,
	@level 		INT
)
AS
SET NOCOUNT ON

if exists(select item_type from castle_crop where castle_id = @castle_id and item_type = @item_type)
begin
	update castle_crop set droprate =@droprate, price = @price , level = @level where castle_id = @castle_id and item_type = @item_type
end
else
begin
	insert into  castle_crop (castle_id, item_type, droprate, price, level) values (@castle_id,@item_type,@droprate ,@price,@level )
end
GO

ALTER PROCEDURE [dbo].[lin_SaveDoorStatus]  
(  
 @hp INT,  
 @max_hp INT,  
 @name NVARCHAR(50)  
)  
AS  
SET NOCOUNT ON  
UPDATE door SET hp = @hp, max_hp = @max_hp WHERE name = @name
GO

/****** Object:  Stored Procedure dbo.lin_SaveDropExp    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SaveDropExp
	
INPUT	
	@drop_exe	int,
	@char_id	int
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_SaveDropExp]
(
	@drop_exe	int,
	@char_id	int
)
AS
SET NOCOUNT ON

update user_data set drop_exp = @drop_exe where char_id = @char_id
GO

ALTER PROCEDURE dbo.lin_SaveHenna
(        
@char_id INT,
@henna_1 INT,
@henna_2 INT,
@henna_3 INT
)        
AS  
SET NOCOUNT ON        

IF EXISTS(SELECT * FROM user_henna WHERE char_id = @char_id)
BEGIN
	UPDATE user_henna SET henna_1 = @henna_1, henna_2 = @henna_2, henna_3 = @henna_3 
	WHERE char_id = @char_id
END
ELSE
BEGIN
	INSERT INTO user_henna (char_id, henna_1, henna_2, henna_3)
	VALUES (@char_id, @henna_1, @henna_2, @henna_3)
END
GO

ALTER PROCEDURE dbo.lin_SaveHeroType
(
@char_id INT,
@hero_type INT
)
AS
SET NOCOUNT ON

UPDATE user_nobless
SET hero_type = @hero_type
WHERE char_id = @char_id
GO

ALTER PROCEDURE dbo.lin_SaveKillDeath
(
@pledge_id INT,
@kill_no INT,
@death_no INT
)
AS
SET NOCOUNT ON

UPDATE pledge
SET siege_kill = @kill_no, siege_death = @death_no
WHERE pledge_id = @pledge_id
GO

/********************************************
lin_SaveMaill
	save mail 
INPUT
	@char_id		int,
	@mail_type		int,
	@related_id		int,
	@receiver_name_list		nvarchar(250),
	@title			nvarchar(200),
	@content		nvarchar(4000)
OUTPUT
return
made by
	kks
date
	2004-12-15
********************************************/
ALTER PROCEDURE [DBO].[lin_SaveMail]
(
	@char_id		int,
	@mail_type		int,
	@related_id		int,
	@receiver_name_list		nvarchar(250),
	@title			nvarchar(200),
	@content		nvarchar(4000)
)
AS
SET NOCOUNT ON

DECLARE @mail_id int
SET @mail_id = 0

INSERT INTO user_mail
(title, content) VALUES (@title, @content)

SET @mail_id = @@IDENTITY

DECLARE @sender_name nvarchar(50)

SELECT @sender_name = char_name FROM user_data(nolock) WHERE char_id = @char_id

INSERT INTO user_mail_sender
(mail_id, related_id, mail_type, mailbox_type, sender_id, sender_name, receiver_name_list)
VALUES
(@mail_id, @related_id, @mail_type, 3, @char_id, @sender_name, @receiver_name_list)
GO

/********************************************        
lin_SaveManorInfo

INPUT        
 @manor_id
 
OUTPUT        
return        
       
made by        
 carrot        
date        
 2004-07-4
change        
********************************************/        
ALTER  PROCEDURE [DBO].[lin_SaveManorInfo]
(        
 @manor_id INT,
 @residence_id INT,
 @AdenaSeedSell INT,
 @AdenaCropBuy INT,
 @change_state TINYINT,
 @last_changed DATETIME
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
		last_changed = @last_changed
	WHERE
		manor_id = @manor_id
END
ELSE
BEGIN
	INSERT INTO manor_info (manor_id, residence_id, adena_vault, crop_buy_vault, change_state, last_changed) 
	VALUES (@manor_id, @residence_id,@AdenaSeedSell,@AdenaCropBuy, @change_state, @last_changed)
END
GO

/********************************************        
lin_SaveManorSeed
 save manor seed crop data
INPUT
	
OUTPUT        
return        
       
made by        
 carrot        
date        
 2004-06-21
change        
********************************************/        
ALTER PROCEDURE [DBO].[lin_SaveManorSeed]
(
@manor_id INT,
@data_index INT,
@seed_id INT,
@seed_price INT,
@seed_sell_count INT,
@seed_remain_count INT,
@crop_id INT,
@crop_buy_count INT,
@crop_remain_count INT,
@crop_price INT,
@crop_type INT,
@crop_deposit INT
)
AS        
        
SET NOCOUNT ON        

IF EXISTS(SELECT * FROM manor_data WHERE manor_id = @manor_id AND data_index =@data_index)
BEGIN
	UPDATE	
		manor_data
	SET 
		seed_id = @seed_id, 
		seed_price = @seed_price, 
		seed_sell_count = @seed_sell_count, 
		seed_remain_count = @seed_remain_count, 
		crop_id = @crop_id, 
		crop_buy_count = @crop_buy_count, 
		crop_price = @crop_price, 
		crop_type = @crop_type, 
		crop_remain_count = @crop_remain_count, 
		crop_deposit = @crop_deposit
	WHERE
		manor_id = @manor_id AND data_index = @data_index
END
ELSE
BEGIN
	INSERT INTO 
	manor_data 
	(manor_id, data_index, seed_id, seed_price, seed_sell_count, seed_remain_count, crop_id, crop_buy_count, crop_price, crop_type, crop_remain_count, crop_deposit) 
	VALUES 
	(@manor_id, @data_index, @seed_id, @seed_price, @seed_sell_count, @seed_remain_count, @crop_id, @crop_buy_count, @crop_price, @crop_type, @crop_remain_count, @crop_deposit)
END
GO

/********************************************        
lin_SaveManorSeed_N
 save manor seed crop data next
INPUT
	
OUTPUT        
return        
       
made by        
 carrot        
date        
 2004-07-4
change        
********************************************/        
ALTER PROCEDURE [DBO].[lin_SaveManorSeed_N]
(
@manor_id INT,
@data_index INT,
@seed_id_n INT,
@seed_price_n INT,
@seed_sell_count_n INT,
@crop_id_n INT,
@crop_buy_count_n INT,
@crop_price_n INT,
@crop_type_n INT
)
AS        
        
SET NOCOUNT ON        

IF EXISTS(SELECT * FROM manor_data_n WHERE manor_id = @manor_id AND data_index =@data_index)
BEGIN
	UPDATE	
		manor_data_n
	SET 
		seed_id_n = @seed_id_n, 
		seed_price_n = @seed_price_n, 
		seed_sell_count_n = @seed_sell_count_n, 
		crop_id_n = @crop_id_n, 
		crop_buy_count_n = @crop_buy_count_n, 
		crop_price_n = @crop_price_n, 
		crop_type_n = @crop_type_n
	WHERE
		manor_id = @manor_id AND data_index = @data_index
END
ELSE
BEGIN
	INSERT INTO 
	manor_data_n
	(manor_id, data_index, seed_id_n, seed_price_n, seed_sell_count_n, crop_id_n, crop_buy_count_n, crop_price_n, crop_type_n) 
	VALUES 
	(@manor_id, @data_index, @seed_id_n, @seed_price_n, @seed_sell_count_n, @crop_id_n, @crop_buy_count_n, @crop_price_n, @crop_type_n)
END
GO

ALTER PROCEDURE dbo.lin_SaveNoblessType
(
@char_id INT,
@nobless_type INT
)
AS
SET NOCOUNT ON

UPDATE user_nobless
SET nobless_type = @nobless_type
WHERE char_id = @char_id

SELECT @@ROWCOUNT
GO

/****** Object:  Stored Procedure dbo.lin_SaveNpcBoss    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SaveNpcBoss 
	
INPUT
OUTPUT
return
made by
date
********************************************/
ALTER PROCEDURE [DBO].[lin_SaveNpcBoss]
(
@npc_db_name	nvarchar(50),
@alive		INT,
@hp 		INT,
@mp 		INT,
@pos_x 		INT,
@pos_y 		INT,
@pos_z 		INT,
@time_low 	INT,
@time_high	INT
)
AS
SET NOCOUNT ON

if exists(select alive from npc_boss where npc_db_name = @npc_db_name)
begin
	update npc_boss 
	set
		alive=@alive, hp=@hp, mp=@mp, pos_x=@pos_x, pos_y=@pos_y, pos_z=@pos_z, time_low=@time_low, time_high=@time_high
	where npc_db_name = @npc_db_name
end
else
begin
	insert into npc_boss values 
	(@npc_db_name, @alive, @hp,@mp, @pos_x, @pos_y,@pos_z, @time_low, @time_high, 0)
end
GO

/****** Object:  Stored Procedure dbo.lin_SaveObjectHp    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SaveObjectHp
	
INPUT	
	@hp	int,
	@id	int
OUTPUT

return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_SaveObjectHp]
(
	@hp	int,
	@id	int
)
AS
SET NOCOUNT ON

UPDATE object_data SET hp = @hp WHERE id = @id
GO

ALTER PROCEDURE dbo.lin_SaveOlympiadRecord
(
@season INT,
@winner_id INT,
@winner_point INT,
@loser_id INT,
@loser_point INT,
@time INT
)
AS
SET NOCOUNT ON

IF @winner_id <> 0
BEGIN
	UPDATE user_nobless
	SET olympiad_point = olympiad_point + @winner_point, match_count = match_count+1, olympiad_win_count = olympiad_win_count+1
	WHERE char_id = @winner_id

	DECLARE @winner_class INT
	SELECT @winner_class = subjob0_class FROM user_data WHERE char_id = @winner_id

	INSERT olympiad_match
	(season, class, match_time, char_id, rival_id, point, is_winner)
	VALUES
	(@season, @winner_class, @time, @winner_id, @loser_id, @winner_point, 1)
END

IF @loser_id <> 0
BEGIN
	UPDATE user_nobless
	SET olympiad_point = olympiad_point + @loser_point, match_count = match_count+1, olympiad_lose_count = olympiad_lose_count+1
	WHERE char_id = @loser_id

	DECLARE @loser_class INT
	SELECT @loser_class = subjob0_class FROM user_data WHERE char_id = @loser_id

	INSERT olympiad_match
	(season, class, match_time, char_id, rival_id, point, is_winner)
	VALUES
	(@season, @loser_class, @time, @loser_id, @winner_id, @loser_point, 0)
END
GO

ALTER PROCEDURE dbo.lin_SaveOlympiadTerm
(
@season INT,
@start_sec INT,
@bonus1_sec INT,
@bonus2_sec INT,
@bonus3_sec INT,
@bonus4_sec INT,
@nominate_sec INT
)
AS
SET NOCOUNT ON

UPDATE olympiad
SET start_sec = @start_sec, 
bonus1_sec = @bonus1_sec, bonus2_sec = @bonus2_sec, bonus3_sec = @bonus3_sec, bonus4_sec = @bonus4_sec, 
nominate_sec = @nominate_sec
WHERE season = @season

SELECT @@ROWCOUNT
GO

/********************************************
lin_SavePet

INPUT
OUTPUT
return
made by
	kuooo
date
	2003-08-19
change
********************************************/
ALTER PROCEDURE [DBO].[lin_SavePet]
(
	@pet_id 	INT,
	@expoint	INT,
	@hp		float,
	@mp		float,
	@sp		INT,
	@meal		INT,
	@nick_name	NVARCHAR(50),
	@slot1		int,
	@slot2		int
)
AS

SET NOCOUNT ON

UPDATE 
	pet_data
set 
	expoint = @expoint,
	hp = @hp,
	mp = @mp,
	sp = @sp,
	meal = @meal,
	nick_name = @nick_name,
	slot1 = @slot1,
	slot2 = @slot2
WHERE 
	pet_id = @pet_id
GO

ALTER PROCEDURE dbo.lin_SavePledgeContribution
(
@agit_id INT,
@pledge_id INT,
@contribution INT
)
AS
SET NOCOUNT ON

UPDATE pledge_contribution SET contribution = contribution + @contribution WHERE pledge_id = @pledge_id AND residence_id = @agit_id
IF @@ROWCOUNT = 0
INSERT INTO pledge_contribution (contribution, pledge_id, residence_id) VALUES (@contribution, @pledge_id , @agit_id)
GO

/********************************************        
lin_SaveRecipeInfo
 delete recipe info
INPUT        
 @char_id
 @recipe_id
OUTPUT        
return        
       
made by        
 carrot        
date        
 2004-07-4
change        
********************************************/        
ALTER PROCEDURE [DBO].[lin_SaveRecipeInfo]
(        
 @char_id INT,
 @recipe_id INT
)        
AS        
        
SET NOCOUNT ON        

INSERT user_recipe VALUES (@char_id, @recipe_id)
GO

ALTER PROCEDURE dbo.lin_SaveSeasonStartTime
(
@season AS INT,
@season_start_time AS INT
)
AS
SET NOCOUNT ON
UPDATE olympiad
SET season_start_time = @season_start_time
WHERE season = @season
GO

/**
  * @procedure lin_SaveSSQJoinInfo
  * @brief SSQ ??? ?? DB? ????.
  *
  * @date 2004/11/26
  * @author Seongeun Park  <sonai@ncsoft.net>
  */
ALTER PROCEDURE [DBO].[lin_SaveSSQJoinInfo] 
(
@round_number INT,
@type TINYINT,
@point INT,
@collected_point INT,
@main_event_point INT,
@member_count INT,
@seal1 INT,
@seal2 INT,
@seal3 INT,
@seal4 INT,
@seal5 INT,
@seal6 INT,
@seal7 INT
)
AS

SET NOCOUNT ON

UPDATE ssq_join_data SET point = @point, collected_point = @collected_point, main_event_point = @main_event_point,
			       member_count = @member_count,
			       seal1_selection_count = @seal1,
			       seal2_selection_count = @seal2,
			       seal3_selection_count = @seal3,
			       seal4_selection_count = @seal4,
			       seal5_selection_count = @seal5,
			       seal6_selection_count = @seal6,
			       seal7_selection_count = @seal7,
			       last_changed_time = GETDATE()		                          
  	                   WHERE round_number = @round_number AND type =@type
GO

/**  
  * @procedure lin_SaveSSQSystemInfo  
  * @brief SSQ ??? ?? DB? ????.  
  *  
  * @date 2004/11/26  
  * @author Seongeun Park  <sonai@ncsoft.net>  
  */  
ALTER PROCEDURE [DBO].[lin_SaveSSQSystemInfo]   
(  
@round_number INT,  
@status TINYINT,  
@winner TINYINT,  
@event_start_time INT,  
@event_end_time  INT,  
@seal_effect_time INT,  
@seal_effect_end_time INT,  
@seal1 TINYINT,  
@seal2 TINYINT,  
@seal3 TINYINT,  
@seal4 TINYINT,  
@seal5 TINYINT,  
@seal6 TINYINT,  
@seal7 TINYINT,  
@castle_snapshot_time INT,  
@can_drop_guard INT  
)  
AS  
  
SET NOCOUNT ON  
  
UPDATE ssq_data SET  status = @status,   
                                        winner = @winner,   
   event_start_time = @event_start_time,  
   event_end_time = @event_end_time,  
               seal_effect_time  =  @seal_effect_time,  
   seal_effect_end_time = @seal_effect_end_time,   
   seal1 = @seal1,  
   seal2 = @seal2,  
   seal3 = @seal3,  
   seal4 = @seal4,  
   seal5 = @seal5,  
   seal6 = @seal6,  
   seal7 = @seal7,  
   castle_snapshot_time = @castle_snapshot_time,  
               can_drop_guard = @can_drop_guard,  
   last_changed_time = GETDATE()  
                      WHERE round_number = @round_number
GO

/**
  * @procedure lin_SaveSSQTopPointUser
  * @brief ?? ?? ?? ??.
  *
  * @date 2004/12/09
  * @author Seongeun Park  <sonai@ncsoft.net>
  */
ALTER PROCEDURE [DBO].[lin_SaveSSQTopPointUser] 
(
@ssq_round INT,
@record_id INT,

@ssq_point INT,
@rank_time INT,
@char_id  INT,
@char_name NVARCHAR(50),
@ssq_part TINYINT,
@ssq_position TINYINT,
@seal_selection_no TINYINT
)
AS

SET NOCOUNT ON

UPDATE ssq_top_point_user  SET  ssq_point = @ssq_point,
				    rank_time = @rank_time,
				    char_id = @char_id,
				    char_name = @char_name,
				    ssq_part = @ssq_part,
				    ssq_position = @ssq_position,
				    seal_selection_no = @seal_selection_no,
				    last_changed_time = GETDATE()
				                       
  	                   WHERE record_id = @record_id AND ssq_round = @ssq_round
GO

/**
  * @procedure lin_SaveSSQUserInfo
  * @brief Save user's ssq parameter  to ssq_user_data
  * 
  * @date 2004/11/18
  * @author sonai <sonai@ncsoft.net>
  * 
  * ???? ?? ???? ??? ?? ???.
  *
  * @param[in]  ssq_part	SSQ ??(?? : 1,  ?? : 2)
  * @param[in]  ssq_position  SSQ ??
  * @param[in]  seal_selection_no  ??? ??? ?? ??
  * @param[in]  ssq_point  SSQ ??? ??
  */

ALTER PROCEDURE [DBO].[lin_SaveSSQUserInfo] 
(
@char_id INT,
@round_number INT,
@ssq_join_time INT,
@ssq_part INT,
@ssq_position INT,
@seal_selection_no INT,
@ssq_point INT,
@twilight_a_item_num INT,
@twilight_b_item_num INT,
@twilight_c_item_num INT,
@dawn_a_item_num INT,
@dawn_b_item_num INT,
@dawn_c_item_num INT,
@ticket_buy_count INT
)
AS

SET NOCOUNT ON

IF EXISTS(SELECT * FROM ssq_user_data WHERE char_id = @char_id)
    BEGIN

    UPDATE ssq_user_data SET  round_number = @round_number,
                                                     ssq_join_time = @ssq_join_time,
 			             ssq_part = @ssq_part, 
		                          ssq_position = @ssq_position, 
			             seal_selection_no = @seal_selection_no, 
		                          ssq_point = @ssq_point,                                         
			             twilight_a_item_num = @twilight_a_item_num,
			             twilight_b_item_num = @twilight_b_item_num,
			             twilight_c_item_num = @twilight_c_item_num,
			             dawn_a_item_num = @dawn_a_item_num,
			             dawn_b_item_num = @dawn_b_item_num,
			             dawn_c_item_num = @dawn_c_item_num,
			             ticket_buy_count = @ticket_buy_count	
			             WHERE char_id = @char_id

   END
ELSE
   BEGIN

   INSERT INTO ssq_user_data (char_id, round_number, ssq_join_time, ssq_part, ssq_position,
			            seal_selection_no, ssq_point,
                                                    twilight_a_item_num, twilight_b_item_num, twilight_c_item_num,
                                                    dawn_a_item_num, dawn_b_item_num, dawn_c_item_num,
                                                    ticket_buy_count) 
	VALUES (@char_id, @round_number, @ssq_join_time, @ssq_part, @ssq_position,
			            @seal_selection_no, @ssq_point,
                                                    @twilight_a_item_num, @twilight_b_item_num, @twilight_c_item_num,
                                                    @dawn_a_item_num, @dawn_b_item_num, @dawn_c_item_num,
                                                    @ticket_buy_count)
   END
GO

/**
  * @procedure lin_SaveTimeAttackRecord
  * @brief ?? ?? ?? ??.
  *
  * @date 2004/12/04
  * @author Seongeun Park  <sonai@ncsoft.net>
  */
ALTER PROCEDURE [DBO].[lin_SaveTimeAttackRecord] 
(
@room_no TINYINT,
@record_type TINYINT,
@ssq_round INT,

@ssq_part TINYINT,
@point INT,
@record_time INT,
@elapsed_time INT,
@member_count  INT,
@member_names NVARCHAR(256),
@member_dbid_list NVARCHAR(128),
@member_reward_flags INT,
@fee INT
)
AS

SET NOCOUNT ON

UPDATE time_attack_record SET  ssq_part = @ssq_part,
				  point = @point,
				  record_time = @record_time,
				  elapsed_time = @elapsed_time,
			      	  member_count = @member_count,
				  member_names = @member_names,
				  member_dbid_list = @member_dbid_list,
				  member_reward_flags = @member_reward_flags,
				  fee = @fee			       	                          
  	                   WHERE room_no = @room_no AND record_type = @record_type AND ssq_round = @ssq_round
GO

/********************************************            
lin_SaveTimeData    
 Save time data table          
INPUT            
 @char_name NVARCHAR(30),            
 @nUsedSec INT            
OUTPUT            
return            
           
made by            
 carrot            
date            
 2004-05-10    
change            
********************************************/            
ALTER PROCEDURE [DBO].[lin_SaveTimeData]    
(            
 @account_id INT,
 @nUsedSec INT,
 @dtLastSaveDate NVARCHAR(20)
)            
AS            
            
SET NOCOUNT ON            

IF (@nUsedSec < 0)            
BEGIN            
    RAISERROR ('Not valid parameter : account id[%d] sec[%d], dt[%s] ',16, 1,  @account_id,  @nUsedSec, @dtLastSaveDate)
    RETURN -1            
END            

UPDATE  time_data SET last_logout = @dtLastSaveDate, used_sec = @nUsedSec WHERE account_id = @account_id 

IF (@@ROWCOUNT = 0)
BEGIN
	INSERT INTO time_data (account_id, last_logout, used_sec) VALUES (@account_id, @dtLastSaveDate, @nUsedSec)    
END
GO

/****** Object:  Stored Procedure dbo.lin_SeizeItem    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SeizeItem
	
INPUT
	@option	INT,
	@warehouse	INT,
	@item_id	INT
OUTPUT
return
made by
	young
date
	2003-06-25
	2004-5-21 modified by young
********************************************/
ALTER PROCEDURE [DBO].[lin_SeizeItem]
(
@option	INT,
@warehouse	INT,
@item_id	INT
)
AS
SET NOCOUNT ON


if @option = 1
	update user_item set warehouse = 1001 where item_id = @item_id
else if @option = 2
	update user_item set warehouse = @warehouse where item_id = @item_id
GO

/********************************************
lin_SeizeItem2
	
INPUT
	@option	INT,
	@warehouse	INT,
	@item_id	INT
OUTPUT
return
made by
	young
date
	2004-6-22 
********************************************/
ALTER PROCEDURE [DBO].[lin_SeizeItem2]
(
@option	INT,
@warehouse	INT,
@item_id	INT
)
AS
SET NOCOUNT ON

update user_item set warehouse = @warehouse where item_id = @item_id
GO

/********************************************
lin_SendMail
	send mail 
INPUT
	@char_id		int,
	@mail_type		int,
	@related_id		int,
	@receiverName		nvarchar(50),
	@title			nvarchar(200),
	@content		nvarchar(4000)
OUTPUT
return
made by
	kks
date
	2004-12-15
modified by
	kks
date
	2005-04-26
********************************************/
ALTER PROCEDURE [DBO].[lin_SendMail]
(
	@char_id		int,
	@mail_type		int,
	@related_id		int,
	@receiver_name_list		nvarchar(250),
	@title			nvarchar(200),
	@content		nvarchar(4000)
)
AS
SET NOCOUNT ON

DECLARE @mail_id int
SET @mail_id = 0

INSERT INTO user_mail
(title, content) VALUES (@title, @content)

SET @mail_id = @@IDENTITY

DECLARE @sender_name nvarchar(50)
DECLARE @char_name NVARCHAR(50)

SELECT @sender_name = char_name FROM user_data(nolock) WHERE char_id = @char_id

INSERT INTO user_mail_sender
(mail_id, related_id, mail_type, sender_id, sender_name, receiver_name_list)
VALUES
(@mail_id, @related_id, @mail_type, @char_id, @sender_name, @receiver_name_list)

if @mail_type = 1
BEGIN
	DECLARE name_cursor CURSOR FOR
	(SELECT char_name FROM user_data(nolock) WHERE pledge_id = (SELECT pledge_id FROM pledge(nolock) WHERE name = @receiver_name_list))
	OPEN name_cursor
	FETCH NEXT FROM name_cursor into @char_name
	WHILE @@FETCH_STATUS = 0
	BEGIN
		EXEC lin_SendMailToReceiver @mail_id, @char_name
		FETCH NEXT FROM name_cursor into @char_name
	END

	CLOSE name_cursor
	DEALLOCATE name_cursor
END
ELSE
BEGIN
--	DECLARE @receiver_name_list_replaced nvarchar(300)
--	SELECT @receiver_name_list_replaced = REPLACE(@receiver_name_list,';', ' ; ')
	
--	DECLARE @receiver_name1 nvarchar (50), @receiver_name2 nvarchar (50), @receiver_name3 nvarchar (50), @receiver_name4 nvarchar (50), @receiver_name5 nvarchar (50)
--	EXEC master..xp_sscanf @receiver_name_list_replaced, '%s ; %s ; %s ; %s ; %s ',
--	   @receiver_name1 OUTPUT, @receiver_name2 OUTPUT, @receiver_name3 OUTPUT, @receiver_name4 OUTPUT, @receiver_name5 OUTPUT
	
--	EXEC lin_SendMailToReceiver @mail_id, @receiver_name1
--	EXEC lin_SendMailToReceiver @mail_id, @receiver_name2
--	EXEC lin_SendMailToReceiver @mail_id, @receiver_name3
--	EXEC lin_SendMailToReceiver @mail_id, @receiver_name4
--	EXEC lin_SendMailToReceiver @mail_id, @receiver_name5
	
	DECLARE @pos int

	set @pos = charindex(';', @receiver_name_list)
	while (@pos > 0)
	begin
		SET @char_name = substring(@receiver_name_list, 0, @pos)
		EXEC lin_SendMailToReceiver @mail_id, @char_name

		set @receiver_name_list = substring(@receiver_name_list, @pos+1, len(@receiver_name_list)-@pos+1)
		set @pos = charindex(';', @receiver_name_list)
	end
END
GO

/********************************************
lin_SetAgitAuction
	create agit bid
INPUT
	@auction_id	int

OUTPUT
return
made by
	young
date
	2003-12-1
********************************************/
ALTER PROCEDURE [DBO].[lin_SetAgitAuction]
(
@agit_id		INT,
@auction_id		INT,
@next_cost		INT

)
AS
SET NOCOUNT ON

declare @max_price  int
declare @attend_id int
declare @pledge_id int
declare @tax int

set @attend_id = 0
set @max_price = 0
set @pledge_id = 0
set @tax = 0

select top 1 @max_price = attend_price, @attend_id = attend_id, @pledge_id = attend_pledge_id from agit_bid (nolock) 
	where auction_id = @auction_id 
	order by attend_price desc, attend_id asc

if ( @max_price > 0 )
begin
	-- ??
	update agit_auction set accepted_bid = @attend_id where auction_id = @auction_id
	update agit set auction_id = null , last_price = @max_price, next_cost = @next_cost  , cost_fail = NULL  where id = @agit_id
	update pledge set agit_id = @agit_id where pledge_id = @pledge_id
	select @tax = isnull( auction_tax , 0) from agit_auction (nolock) where auction_id = @auction_id
end  else begin
	-- ??
	update agit_auction set accepted_bid = 0 where auction_id = @auction_id
	update agit set auction_id = null , next_cost = 0  , cost_fail = NULL where id = @agit_id
end

select @attend_id, @max_price, @pledge_id, @tax
GO

-- lin_SetAgitDeco
-- by bert
-- return (1, 0)
ALTER PROCEDURE dbo.lin_SetAgitDeco
(
	@agit_id INT,
	@type INT,
	@id INT,
	@name VARCHAR(256),
	@level INT,
	@expire INT
)
AS
SET NOCOUNT ON

DELETE FROM agit_deco
WHERE agit_id = @agit_id AND type = @type

INSERT INTO agit_deco
(agit_id, type, id, name, level, expire)
VALUES
(@agit_id, @type, @id, @name, @level, @expire)

SELECT @@ROWCOUNT
GO

/********************************************
lin_SetAgitNextCost
	
INPUT
	

OUTPUT
return
made by
	young
date
	2003-12-1
********************************************/
ALTER PROCEDURE [DBO].[lin_SetAgitNextCost]
(
@agit_id		INT,
@next_cost		INT,
@cost_fail		int=0

)
AS
SET NOCOUNT ON

update agit set next_cost = @next_cost  , cost_fail = @cost_fail where id = @agit_id
GO

/****** Object:  Stored Procedure dbo.lin_setAgitOwner    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_setAgitOwner
	set Agit Owner
INPUT
	nAgit_id, 
	nPledge_id

OUTPUT
return
made by
	carrot
date
	2002-06-12
change
********************************************/
ALTER PROCEDURE [DBO].[lin_setAgitOwner]
(
	@Agit_id		INT,
	@Pledge_id	INT
)
AS

SET NOCOUNT ON

IF NOT exists(SELECT id FROM castle WHERE id = @Agit_id )
	INSERT INTO castle (id, name, pledge_id, type) VALUES (@Agit_id, 'test', @Pledge_id,  2)
ELSE
	UPDATE castle  SET pledge_id = @Pledge_id WHERE id = @Agit_id AND type = 2 

IF @@ROWCOUNT > 0
	SELECT 1
ELSE
	SELECT 0
GO

/****** Object:  Stored Procedure dbo.lin_SetAgitOwner2    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SetAgitOwner2
	
INPUT	
	@pledge_id	int,
	@agit_id		int
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_SetAgitOwner2]
(
	@pledge_id	int,
	@agit_id		int
)
AS
SET NOCOUNT ON

UPDATE agit SET pledge_id = @pledge_id WHERE id = @agit_id
GO

/****** Object:  Stored Procedure dbo.lin_SetAllianceInfo    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_SetAllianceInfo
	
INPUT	
	@fieldName	nvarchar(50),
	@field_data	INT,
	@pledge_id	INT
OUTPUT

return
made by
	bert ^^
********************************************/

ALTER PROCEDURE [DBO].[lin_SetAllianceInfo]
(
@fieldName	nvarchar(50),
@field_data	INT,
@alliance_id	INT
)
AS
SET NOCOUNT ON

IF @fieldName = N'oust_time' begin update alliance set oust_time = @field_data where id =  @alliance_id end
ELSE IF @fieldName = N'crest_id' begin update alliance set crest_id = @field_data where id =  @alliance_id end
ELSE 
BEGIN 
	RAISERROR ('lin_SetAllianceInfo : invalid field [%s]', 16, 1, @fieldName)
	RETURN -1	
END
GO

ALTER PROCEDURE dbo.lin_SetAquireSkill
(
	@char_id	INT,
	@subjob_id	INT,
	@skill_id		INT,
	@skill_level	TINYINT
)
AS
SET NOCOUNT ON

IF EXISTS(SELECT skill_lev FROM user_skill WHERE char_id = @char_id AND skill_id = @skill_id AND ISNULL(subjob_id, 0) = @subjob_id)
	UPDATE user_skill SET skill_lev = @skill_level WHERE char_id = @char_id AND skill_id = @skill_id AND ISNULL(subjob_id, 0) = @subjob_id
ELSE
	INSERT INTO user_skill (char_id, subjob_id, skill_id, skill_lev) VALUES (@char_id, @subjob_id, @skill_id, @skill_level)
GO

/****** Object:  Stored Procedure dbo.lin_SetBuilderAccount     Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SetBuilderAccount
	set builder account
INPUT
	@account_name	nvarchar(50),
	@default_level		int

OUTPUT
return
made by
	young
date
	2002-11-28
change
********************************************/
ALTER PROCEDURE [DBO].[lin_SetBuilderAccount ]
(
	@account_name	nvarchar(50),
	@default_level		int
)
AS

SET NOCOUNT ON

if ( @default_level = 0)
begin
	delete from builder_account where account_name = @account_name
end else begin
	if exists(select * from builder_account where account_name = @account_name)
	begin
		update builder_account set default_builder = @default_level where account_name = @account_name
	end else begin
		declare @account_id int
		set @account_id = 0
		select top 1 @account_id = account_id from user_data where account_name = @account_name and account_id > 0
		insert into builder_account(account_name, default_builder, account_id) values(@account_name, @default_level, @account_id)
	end
end
GO

/****** Object:  Stored Procedure dbo.lin_SetBuilderCharacter    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_SetBuilderCharacter
	Set Builder Character flag
INPUT
	@char_name	nvarchar,
	@nBuilderLev	int
OUTPUT
	char_id
return
made by
	carrot
date
	2002-06-28
********************************************/
ALTER PROCEDURE [DBO].[lin_SetBuilderCharacter]
(
	@char_name	NVARCHAR(24),
	@nBuilderLev	INT
)
AS

SET NOCOUNT ON

DECLARE @Char_id INT
SET @Char_id = 0

UPDATE user_data SET builder =  @nBuilderLev WHERE char_name = @char_name
IF @@ROWCOUNT > 0
BEGIN
	SELECT @Char_id = char_id FROM user_data WHERE char_name = @char_name
END

SELECT @Char_id
GO

/****** Object:  Stored Procedure dbo.lin_setCastleOwner    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_setCastleOwner
	set castle Owner
INPUT
	nCastle_id, 
	nPledge_id

OUTPUT
return
made by
	carrot
date
	2002-06-12
change
********************************************/
ALTER PROCEDURE [DBO].[lin_setCastleOwner]
(
	@Castle_id	INT,
	@Pledge_id	INT
)
AS

SET NOCOUNT ON

DECLARE @Castle_idOld INT
DECLARE @nPledgeIdOld INT

SET @Castle_idOld = 0
SET @nPledgeIdOld = 0

SELECT @Castle_idOld = id, @nPledgeIdOld = pledge_id FROM castle WHERE id = @Castle_id 

IF @Castle_idOld = 0 
	INSERT INTO castle (id, name, pledge_id, type) VALUES (@Castle_id , 'test', @Pledge_id,  1)
ELSE
	UPDATE castle  SET pledge_id = @Pledge_id WHERE id = @Castle_id  AND type = 1

IF @@ROWCOUNT > 0
	SELECT 1
ELSE
	SELECT 0
GO

/****** Object:  Stored Procedure dbo.lin_SetCastleOwner2    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SetCastleOwner2
	
INPUT	
	@pledge_id	int,
	@agit_id		int
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_SetCastleOwner2]
(
	@pledge_id	int,
	@castle_id		int
)
AS
SET NOCOUNT ON

UPDATE castle SET pledge_id = @pledge_id WHERE id = @castle_id
GO

/****** Object:  Stored Procedure dbo.lin_SetCharacterDelete    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_SetCharacterDelete
	Set character is deleted
INPUT
	@char_id	INT
OUTPUT
	year
	month
	day
return
made by
	carrot
date
	2003-01-7
********************************************/
ALTER  PROCEDURE [DBO].[lin_SetCharacterDelete]
(
	@char_id	INT
)
AS

SET NOCOUNT ON

DECLARE @deletedDate	SMALLDATETIME

SET @deletedDate = GetDate()

UPDATE user_data SET temp_delete_date = @deletedDate WHERE char_id = @char_id

IF @@ROWCOUNT > 0 
BEGIN
	SELECT	YEAR(@deletedDate), MONTH(@deletedDate), DAY(@deletedDate),
		DATEPART(HOUR, @deletedDate), DATEPART(mi, @deletedDate), DATEPART(s, @deletedDate)
END
ELSE
BEGIN
	SELECT 0,0,0,0,0,0
END
GO

/****** Object:  Stored Procedure dbo.lin_SetCharacterDeleteRestore    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_SetCharacterDeleteRestore
	Restore character which is set deleted
INPUT
	@char_id	INT
OUTPUT
return
made by
	carrot
date
	2003-01-7
********************************************/
ALTER PROCEDURE [DBO].[lin_SetCharacterDeleteRestore]
(
	@char_id	INT
)
AS

SET NOCOUNT ON

UPDATE user_data SET temp_delete_date = NULL WHERE char_id = @char_id
GO

ALTER PROCEDURE dbo.lin_SetClass
(
@char_id AS INT,
@subjob_id AS INT,
@class AS INT
)
AS
SET NOCOUNT ON

IF @subjob_id = 0
	UPDATE user_data SET class = @class, subjob0_class = @class WHERE char_id = @char_id
ELSE IF @subjob_id = 1
	UPDATE user_data SET class = @class, subjob1_class = @class WHERE char_id = @char_id
ELSE IF @subjob_id = 2
	UPDATE user_data SET class = @class, subjob2_class = @class WHERE char_id = @char_id
ELSE IF @subjob_id = 3
	UPDATE user_data SET class = @class, subjob3_class = @class WHERE char_id = @char_id
GO

ALTER PROCEDURE dbo.lin_SetHistoryOpen
(
@char_id AS INT,
@history_open AS TINYINT
)
AS
SET NOCOUNT ON

UPDATE user_nobless
SET history_open = @history_open
WHERE char_id = @char_id
GO

/********************************************
lin_SetIntervalAnnounce
	set interval announce
INPUT
	
OUTPUT
	interval
	announce id
	msg
return
made by
	carrot
date
	2003-12-19
********************************************/
ALTER PROCEDURE [DBO].[lin_SetIntervalAnnounce]
(
	@nInterval 	INT,
	@nAnnounceId	INT,
	@wsMsg	NVARCHAR(100)
)
AS
SET NOCOUNT ON

if EXISTS(select top 1 * from login_announce where interval_10 = @nInterval and announce_id = @nAnnounceId)
  BEGIN
    UPDATE login_announce SET announce_msg = @wsMsg WHERE interval_10 = @nInterval and announce_id = @nAnnounceId
  END
ELSE
  BEGIN
    INSERT INTO login_announce (interval_10, announce_id, announce_msg) VALUES (@nInterval, @nAnnounceId, @wsMsg)
  END
GO

/********************************************    
lin_SetLastTaxUpdate  
     
INPUT     
@income  datetime,
@tax datetime
OUTPUT    
made by    
 carrot    
date    
 2004-02-29  
********************************************/    
ALTER PROCEDURE [DBO].[lin_SetLastTaxUpdate]  
(  
@income  datetime,
@tax datetime,
@manor datetime
)  
AS    
    
SET NOCOUNT ON    

IF EXISTS(SELECT * FROM castle_tax)  
BEGIN  
 UPDATE castle_tax SET   income_update =  @income, tax_change =   @tax, manor_reset =   @manor 
END  
ELSE  
BEGIN  
 INSERT INTO castle_tax VALUES  
 (  @income, @tax, @manor)  
  
END
GO

/****** Object:  Stored Procedure dbo.lin_SetLoginAnnounce    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SetLoginAnnounce
	set login announce
INPUT
	@announce_id int,
	@announce_msg nvarchar(64)	
OUTPUT
	
return
made by
	young
date
	2002-11-30
********************************************/
ALTER PROCEDURE [DBO].[lin_SetLoginAnnounce]
(
	@announce_id int , 
	@announce_msg nvarchar(64)
)

AS

SET NOCOUNT ON


if exists(select announce_id from login_announce (nolock) where announce_id = @announce_id  and interval_10 = 0 )
begin

	update login_announce set announce_msg = @announce_msg where announce_id = @announce_id and interval_10 = 0

end else begin

	insert into login_announce(announce_id, announce_msg , interval_10 ) values(@announce_id, @announce_msg, 0 ) 

end
GO

/********************************************
lin_SetMacro
	ALTER  macro
INPUT
	@macro_id		int,
	@macro_name		nvarchar(64),
	@macro_tooltip		nvarchar(64)
	@macro_iconname	nvarchar(64)
	@macro_icontype		int

OUTPUT
return
made by
	young
date
	2004-6-16
********************************************/
ALTER PROCEDURE [DBO].[lin_SetMacro]
(
@macro_id		int,
@macro_name		nvarchar(64),
@macro_tooltip		nvarchar(64),
@macro_iconname	nvarchar(64),
@macro_icontype	int
)
AS
SET NOCOUNT ON

if ( exists ( select * from user_macro where macro_id = @macro_id ) )
begin
	update user_macro
	set macro_name = @macro_name, macro_tooltip = @macro_tooltip,
		macro_iconname = @macro_iconname, macro_icontype = @macro_icontype
	where macro_id = @macro_id


	delete from user_macroinfo where macro_id = @macro_id


end
GO

/********************************************
lin_SetNewbieStat
	set newbie staus
INPUT
	@account_id	INT,
	@newbie_stat 	INT
OUTPUT
return
made by
	kks
date
	2004-11-25
********************************************/
ALTER PROCEDURE [DBO].[lin_SetNewbieStat]
(
	@account_id	INT,
	@newbie_stat 	INT
)
AS
SET NOCOUNT ON

UPDATE user_newbie 
SET newbie_stat = @newbie_stat
WHERE account_id = @account_id
GO

/****** Object:  Stored Procedure dbo.lin_SetPetitionMsg    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SetPetitionMsg
	store Petition Msg
INPUT
	@char_id	int,
	@msg	nvarchar(500)
OUTPUT

return
made by
	carrot
date
	2003-02-27
********************************************/
ALTER PROCEDURE [DBO].[lin_SetPetitionMsg]
(
	@char_id	int,
	@msg	nvarchar(500)
)
AS
SET NOCOUNT ON

if exists(select * from PetitionMsg where char_id =@char_id)
begin
	update PetitionMsg set msg = @msg where char_id = @char_id
end 
else 
begin
	insert into PetitionMsg  (char_id, msg) 
	values( @char_id, @msg)
end
GO

/********************************************
lin_SetPledgeAnnounce
	set pledge announce 
INPUT
	@pledge_id		int,
	@show_flag		smallint,
	@content		nvarchar(3000)
OUTPUT
return
made by
	kks
date
	2005-07-22
********************************************/
ALTER PROCEDURE dbo.lin_SetPledgeAnnounce
(  
	@pledge_id		int,
	@show_flag		smallint,
	@content		nvarchar(3000)
)  
AS  
SET NOCOUNT ON  
  

UPDATE pledge_announce SET show_flag = @show_flag , content = @content
WHERE pledge_id = @pledge_id

IF (@@ROWCOUNT = 0)  
 BEGIN  
  INSERT INTO pledge_announce (pledge_id, show_flag, content)
	VALUES (@pledge_id, @show_flag, @content)
 END
GO

/****** Object:  Stored Procedure dbo.lin_SetPledgeCrest    Script Date: 2003-09-20 ?? 11:51:57 ******/
-- lin_SetPledgeCrest
-- by bert
-- return crest id
-- modified by kks (2005-08-18)

ALTER PROCEDURE [DBO].[lin_SetPledgeCrest]
(
	@bitmap_size	INT,
	@bitmap	VARBINARY(3072)
)
AS

SET NOCOUNT ON

INSERT INTO Pledge_Crest
(bitmap_size, bitmap) VALUES (@bitmap_size, @bitmap)

SELECT @@IDENTITY
GO

/****** Object:  Stored Procedure dbo.lin_SetPledgeInfo    Script Date: 2003-09-20 ?? 11:52:00 ******/

/********************************************
lin_SetPledgeInfo
	
INPUT	
	@fieldName	nvarchar(50),
	@field_data	INT,
	@pledge_id	INT
OUTPUT

return
made by
	carrot
date
	2002-06-16
modified by 
	kks (2005-07-22)
********************************************/
ALTER PROCEDURE [DBO].[lin_SetPledgeInfo]
(
@fieldName	nvarchar(50),
@field_data	INT,
@pledge_id	INT
)
AS
SET NOCOUNT ON

IF @fieldName = N'alliance_id' begin update pledge set alliance_id = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'challenge_time' begin update pledge set challenge_time = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'root_name_value' begin update pledge set root_name_value = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'now_war_id' begin update pledge set now_war_id = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'oust_time' begin update pledge set oust_time = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'skill_level' begin update pledge set skill_level = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'castle_id' begin update pledge set castle_id = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'agit_id' begin update pledge set agit_id = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'rank' begin update pledge set rank = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'name_value' begin update pledge set name_value = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'status' begin update pledge set status = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'private_flag' begin update pledge set private_flag = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'crest_id' begin update pledge set crest_id = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'is_guilty' begin update pledge set is_guilty = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'dismiss_reserved_time' begin update pledge set dismiss_reserved_time = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'alliance_ousted_time' begin update pledge set alliance_ousted_time = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'alliance_withdraw_time' begin update pledge set alliance_withdraw_time = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'alliance_dismiss_time' begin update pledge set alliance_dismiss_time = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'ruler_id' begin update pledge set ruler_id = @field_data where pledge_id =  @pledge_id end
ELSE IF @fieldName = N'emblem_id' begin update pledge set emblem_id = @field_data where pledge_id =  @pledge_id end
ELSE 
BEGIN 
	RAISERROR ('lin_SetPledgeInfo : invalid field [%s]', 16, 1, @fieldName)
	RETURN -1	
END
GO

/****** Object:  Stored Procedure dbo.lin_SetPunish    Script Date: 2003-09-20 ??? 11:52:00 ******/
/********************************************
lin_SetPunish
	Set punish
INPUT
	@char_id	INT,
	@punish_id 	INT,
	@punish_on	INT,
	@remain	INT
OUTPUT
return
made by
	young
date
	2002-11-27
********************************************/
ALTER   PROCEDURE [DBO].[lin_SetPunish]
(
	@char_id	INT,
	@punish_id	INT,
	@punish_on	INT,
	@remain	INT

)
AS
SET NOCOUNT ON

declare @nCount int

select @nCount = count(*) from user_punish (nolock) where char_id = @char_id and punish_id = @punish_id

if ( @nCount > 0 and @remain = 0)
begin
	delete from user_punish where char_id = @char_id and punish_id = @punish_id
end

if ( @nCount > 0 and @remain > 0)
begin
	if @punish_on = 1
		update user_punish set  remain_game = @remain where char_id = @char_id and punish_id = @punish_id

	if @punish_on = 0
		update user_punish set  remain_real = @remain  where char_id = @char_id and punish_id = @punish_id
		
end

if ( @nCount = 0 and @remain > 0)
begin
	if @punish_on = 1
		insert into user_punish(char_id, punish_id, punish_on, remain_game) values (@char_id, @punish_id, 1, @remain)

	if @punish_on = 0
		insert into user_punish(char_id, punish_id, punish_on, remain_real) values (@char_id, @punish_id, 0, @remain)
end
GO

/********************************************  
lin_SetQuest   
   
INPUT  
@q1 INT,  
@s1 INT,  
@j1 INT,  
  
@q2 INT,  
@s2 INT,  
@j2 INT,  
  
@q3 INT,  
@s3 INT,  
@j3 INT,  
  
@q4 INT,  
@s4 INT,  
@j4 INT,  
  
@q5 INT,  
@s5 INT,  
@j5 INT,  
  
@q6 INT,  
@s6 INT,  
@j6 INT,  
  
  
@q7 INT,  
@s7 INT,  
@j7 INT,  
  
@q8 INT,  
@s8 INT,  
@j8 INT,  
  
@q9 INT,  
@s9 INT,  
@j9 INT,  
  
@q10 INT,  
@s10 INT,  
@j10 INT,  
  
@q11 INT,  
@s11 INT,  
@j11 INT,  
  
@q12 INT,  
@s12 INT,  
@j12 INT,  
  
@q13 INT,  
@s13 INT,  
@j13 INT,  
  
@q14 INT,  
@s14 INT,  
@j14 INT,  
  
@q15 INT,  
@s15 INT,  
@j15 INT,  
  
@q16 INT,  
@s16 INT,  
@j16 INT,  
  
@char_id  INT  
OUTPUT  
return  
made by  
 carrot  
date  
 2002-06-09  
********************************************/  
ALTER PROCEDURE [DBO].[lin_SetQuest]  
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
q16=@q16, s16=@s16, s2_16=@s2_16, j16=@j16  
WHERE char_id = @char_id
GO

ALTER PROCEDURE dbo.lin_SetShortCut
(  
 @char_id INT, 
 @subjob_id INT,
 @slotnum  INT,  
 @type   INT,  
 @id   INT,  
 @macro   NVARCHAR(256)  
)  
AS  
SET NOCOUNT ON  
  
IF (@char_id = 0)
BEGIN
SET @char_id = @subjob_id
SET @subjob_id = 0
END

IF (@type = 0)  
BEGIN  
 DELETE shortcut_data WHERE  char_id = @char_id AND subjob_id = @subjob_id AND slotnum = @slotnum  
END  
ELSE   
BEGIN  
 UPDATE shortcut_data SET shortcut_type=@type , shortcut_id= @id, shortcut_macro = @macro WHERE char_id = @char_id AND subjob_id = @subjob_id AND slotnum = @slotnum  
 IF (@@ROWCOUNT = 0)  
 BEGIN  
  INSERT INTO shortcut_data (char_id, slotnum, shortcut_type, shortcut_id, shortcut_macro, subjob_id) VALUES (@char_id, @slotnum, @type, @id, @macro, @subjob_id)  
 END  
END
GO

/****** Object:  Stored Procedure dbo.lin_SetSiegeTime    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SetSiegeTime
	
INPUT	
	@next_war_time	int,
	@castle_id	int,
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_SetSiegeTime]
(
	@next_war_time	int,
	@castle_id	int
)
AS
SET NOCOUNT ON

UPDATE 
	castle 
SET 
	next_war_time = @next_war_time 
WHERE 
	id = @castle_id
GO

ALTER PROCEDURE dbo.lin_SetSkillCoolTime
(
	@char_id	INT,
	@subjob_id	INT,
	@skill_id		INT,
	@nToEndTime	INT
)
AS
SET NOCOUNT ON

UPDATE user_skill SET to_end_time = @nToEndTime WHERE char_id = @char_id AND skill_id = @skill_id AND ISNULL(subjob_id, 0) = @subjob_id
GO

/********************************************
lin_SetSubJobAcquireSkill
	
INPUT
	@char_id	int,
	@subjob_id	int,
	@skill_id	int,
	@skill_level	int

OUTPUT
return
made by
	kks
date
	2005-01-19
********************************************/
ALTER PROCEDURE [DBO].[lin_SetSubJobAcquireSkill]
(
	@char_id	int,
	@subjob_id	int,
	@skill_id	int,
	@skill_level	int
)
AS
SET NOCOUNT ON

DECLARE @cnt INT
SET @cnt = 0

IF exists(select * from user_skill(nolock) where char_id = @char_id and subjob_id = @subjob_id and skill_id = @skill_id)    
BEGIN    
	update user_skill set skill_lev = @skill_level where char_id = @char_id and subjob_id = @subjob_id and skill_id = @skill_id
END    
ELSE
BEGIN    
	insert into user_skill values (@char_id, @skill_id, @skill_level, 0, @subjob_id)
END
GO

ALTER PROCEDURE dbo.lin_SetTeamBattleNpcType
(
	@agit_id INT,
	@pledge_id INT,
	@npc_type INT
)
AS
SET NOCOUNT ON
UPDATE team_battle_agit_pledge
SET npc_type = @npc_type 
WHERE agit_id = @agit_id AND pledge_id = @pledge_id
GO

ALTER PROCEDURE dbo.lin_SetTeamBattleStatus
(
@agit_id INT,
@new_team_battle_status INT
)
AS
SET NOCOUNT ON
UPDATE agit SET team_battle_status = @new_team_battle_status WHERE id = @agit_id
GO

ALTER PROCEDURE dbo.lin_SetTeamBattleWinner
(
@agit_id INT,
@winner_pledge_id INT,
@master_pledge_id INT,
@is_final INT
)
AS
SET NOCOUNT ON
IF @is_final <> 0	-- battle royal
BEGIN
	DELETE FROM team_battle_agit_pledge WHERE agit_id = @agit_id AND pledge_id <> @winner_pledge_id AND pledge_id <> @master_pledge_id
	DELETE FROM team_battle_agit_member WHERE agit_id = @agit_id AND pledge_id <> @winner_pledge_id AND pledge_id <> @master_pledge_id
END
ELSE	-- final winner
BEGIN
	DELETE FROM team_battle_agit_pledge WHERE agit_id = @agit_id AND pledge_id <> @winner_pledge_id
	DELETE FROM team_battle_agit_member WHERE agit_id = @agit_id
END
GO

/********************************************
lin_SetUserBan

INPUT
	@char_id	INT
	@ban_hour	smallint
OUTPUT
return
made by
	young
date
	2003-09-22
********************************************/
ALTER PROCEDURE [DBO].[lin_SetUserBan]
(
	@char_id	INT,
	@status		INT,
	@ban_hour	smallint,
	@ban_end	INT
)
AS
SET NOCOUNT ON

if @ban_hour = 0 
begin
	delete from user_ban where char_id = @char_id
end else begin

	if ( exists ( select * from user_ban (nolock) where char_id = @char_id ) )
	begin
		-- update
		update user_ban set status = @status, ban_date = getdate(), ban_hour = @ban_hour, ban_end = @ban_end where char_id = @char_id
	end else begin
		-- insert
		insert into user_ban ( char_id, status, ban_hour , ban_end ) values ( @char_id, @status, @ban_hour, @ban_end  )
	end
end
GO

/****** Object:  Stored Procedure dbo.lin_SetUserDataEx    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_SetUserDataEx
	
INPUT	
	@char_id	int,
	@dismiss_penalty_reserved	int
OUTPUT
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_SetUserDataEx]
(
	@char_id	int,
	@dismiss_penalty_reserved	int
)
AS
SET NOCOUNT ON

INSERT INTO user_data_ex (char_id, dismiss_penalty_reserved) VALUES (@char_id, @dismiss_penalty_reserved)
GO

/****** Object:  Stored Procedure dbo.lin_SetUserNickname    Script Date: 2003-09-20 ?? 11:51:57 ******/
ALTER PROCEDURE
dbo.lin_SetUserNickname (@user_id INT, @nickname NVARCHAR(50))
AS

SET NOCOUNT ON

DECLARE @ret INT

UPDATE user_data
SET nickname = @nickname
WHERE char_id = @user_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @ret = @user_id
END
ELSE
BEGIN
	SELECT @ret = 0
END

SELECT @ret
GO

/**  
  * @procedure lin_SetUserSSQDawnRound  
  * @brief SSQ ?? ?? ??  
  *  
  * @date 2004/12/17  
  * @author Seongeun Park  <sonai@ncsoft.net>  
  */  
ALTER PROCEDURE dbo.lin_SetUserSSQDawnRound  
(  
 @char_id  INT,  
 @ssq_dawn_round INT  
)  
AS  
SET NOCOUNT ON  
  
UPDATE user_data SET ssq_dawn_round = @ssq_dawn_round WHERE char_id = @char_id
GO

-- lin_StartAllianceWar
-- by bert

ALTER PROCEDURE
dbo.lin_StartAllianceWar (@challenger_alliance_id INT, @challengee_alliance_id INT, @war_begin_time INT)
AS

SET NOCOUNT ON

DECLARE @ret INT

DECLARE @challenger_name VARCHAR(50)
DECLARE @challengee_name VARCHAR(50)

SELECT @challenger_name = name FROM alliance WHERE id = @challenger_alliance_id
SELECT @challengee_name = name FROM alliance WHERE id = @challengee_alliance_id

INSERT INTO Alliance_War
(challenger, challengee, begin_time, challenger_name, challengee_name)
VALUES
(@challenger_alliance_id, @challengee_alliance_id, @war_begin_time, @challenger_name, @challengee_name)

SELECT @@IDENTITY
GO

ALTER PROCEDURE dbo.lin_StartOlympiadSeason
(
@season INT,
@step INT,
@season_start_time INT
)
AS
SET NOCOUNT ON

UPDATE olympiad
SET step = @step, season_start_time = @season_start_time
WHERE season = @season

SELECT @@ROWCOUNT
GO

/****** Object:  Stored Procedure dbo.lin_StartPledgeWar    Script Date: 2003-09-20 ?? 11:52:00 ******/
-- lin_StartPledgeWar
-- by bert

ALTER PROCEDURE
dbo.lin_StartPledgeWar (@challenger_pledge_id INT, @challengee_pledge_id INT, @war_begin_time INT)
AS

SET NOCOUNT ON

DECLARE @ret INT

DECLARE @challenger_name VARCHAR(50)
DECLARE @challengee_name VARCHAR(50)

SELECT @challenger_name = name FROM Pledge WHERE pledge_id = @challenger_pledge_id
SELECT @challengee_name = name FROM Pledge WHERE pledge_id = @challengee_pledge_id

INSERT INTO Pledge_War
(challenger, challengee, begin_time, challenger_name, challengee_name)
VALUES
(@challenger_pledge_id, @challengee_pledge_id, @war_begin_time, @challenger_name, @challengee_name)

SELECT @@IDENTITY
GO

ALTER PROCEDURE
dbo.lin_StopAllianceWar (@proposer_alliance_id INT, @proposee_alliance_id INT, @war_id INT,  @war_end_time INT)
AS

SET NOCOUNT ON

DECLARE @ret INT

UPDATE Alliance_War
SET status = 1,	-- WAR_END_STOP
winner = 0,
end_time = @war_end_time
WHERE
id = @war_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @ret = @war_id
END
ELSE
BEGIN
	SELECT @ret = 0
END

SELECT @ret
GO

/****** Object:  Stored Procedure dbo.lin_StopPledgeWar    Script Date: 2003-09-20 ?? 11:52:00 ******/
-- lin_StopPledgeWar
-- by bert

ALTER PROCEDURE
dbo.lin_StopPledgeWar (@proposer_pledge_id INT, @proposee_pledge_id INT, @war_id INT,  @war_end_time INT)
AS

SET NOCOUNT ON

DECLARE @ret INT

BEGIN TRAN

UPDATE Pledge_War
SET status = 1,	-- WAR_END_STOP
winner = 0,
end_time = @war_end_time
WHERE
id = @war_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @ret = @war_id
END
ELSE
BEGIN
	SELECT @ret = 0
END

EXIT_TRAN:
IF @ret<> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END
SELECT @ret
GO

-- lin_SurrenderAllianceWar
-- by bert

ALTER PROCEDURE
dbo.lin_SurrenderAllianceWar (@proposer_alliance_id INT, @proposee_alliance_id INT, @war_id INT, @war_end_time INT)
AS

SET NOCOUNT ON

DECLARE @ret INT

UPDATE alliance_war
SET status = 2,	-- WAR_END_SURRENDER
winner = @proposee_alliance_id,
winner_name = (SELECT name FROM alliance WHERE id = @proposee_alliance_id),
end_time = @war_end_time
WHERE
id = @war_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @ret = @war_id
END
ELSE
BEGIN
	SELECT @ret = 0
END

SELECT @ret
GO

ALTER PROCEDURE dbo.lin_SurrenderPersonally(@char_id INT, @war_id INT)  
AS  
  
SET NOCOUNT ON  
  
INSERT INTO user_surrender  
(char_id, surrender_war_id)  
VALUES   
(@char_id, @war_id)  
  
SELECT @@ROWCOUNT
GO

/****** Object:  Stored Procedure dbo.lin_SurrenderPledgeWar    Script Date: 2003-09-20 ?? 11:52:00 ******/
-- lin_SurrenderPledgeWar
-- by bert

ALTER PROCEDURE
dbo.lin_SurrenderPledgeWar (@proposer_pledge_id INT, @proposee_pledge_id INT, @war_id INT, @war_end_time INT)
AS

SET NOCOUNT ON

DECLARE @ret INT

BEGIN TRAN

UPDATE Pledge_War
SET status = 2,	-- WAR_END_SURRENDER
winner = @proposee_pledge_id,
winner_name = (SELECT name FROM Pledge WHERE pledge_id = @proposee_pledge_id),
end_time = @war_end_time
WHERE
id = @war_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @ret = @war_id
END
ELSE
BEGIN
	SELECT @ret = 0
END

IF @ret<> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END
SELECT @ret
GO

/********************************************
lin_SwitchPledgeAnnounceShowFlag
	set pledge announce show flag
INPUT
	@pledge_id		int,
	@show_flag		smallint
OUTPUT
return
made by
	kks
date
	2005-07-22
********************************************/
ALTER PROCEDURE dbo.lin_SwitchPledgeAnnounceShowFlag
(  
	@pledge_id		int,
	@show_flag		smallint
)  
AS  
SET NOCOUNT ON  
  

UPDATE pledge_announce SET show_flag = @show_flag
WHERE pledge_id = @pledge_id
GO

/****** Object:  Stored Procedure dbo.lin_TrasferWarehouseNewAll    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_TrasferWarehouseNewPartial
	movet inventory to warehouse || move warehouse to inventory total item
INPUT
	@char_id		INT,
	@item_id			INT,
	@ToWarehouseID	INT,
	@bIsToInven		TINYINT
OUTPUT
	
return
made by
	carrot
date
	2002-10-17
********************************************/
ALTER PROCEDURE [DBO].[lin_TrasferWarehouseNewAll]
(
	@char_id		INT,
	@item_id			INT,
	@ToWarehouseID	INT,
	@bIsToInven		TINYINT
)
AS
SET NOCOUNT ON

IF (@bIsToInven > 0) 
BEGIN
	SET IDENTITY_INSERT user_item ON

	IF (SELECT COUNT(*) FROm user_warehouse WHERE item_id = @item_id) = 1
	BEGIN
		INSERT INTO user_item (item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse)
			SELECT item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_warehouse WHERE item_id = @item_id
		IF @@ROWCOUNT = 0
		BEGIN
			SELECT 0
		END
		ELSE 
		BEGIN
			DELETE user_warehouse WHERE item_id = @item_id
			SELECT 1
		END
	END
	ELSE
	BEGIN
		SELECT 0
	END

	SET IDENTITY_INSERT user_item OFF
END
ELSE
BEGIN
	IF (SELECT COUNT(*) FROm user_item WHERE item_id = @item_id) = 1
	BEGIN
		INSERT INTO user_warehouse (item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse)
			SELECT item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse FROM user_item WHERE item_id = @item_id
		IF @@ROWCOUNT = 0
		BEGIN
			SELECT 0
		END
		ELSE 
		BEGIN
			DELETE user_item WHERE item_id = @item_id
			SELECT 1
		END
	END
	ELSE
	BEGIN
		SELECT 0
	END
END
GO

/****** Object:  Stored Procedure dbo.lin_TrasferWarehouseNewPartial    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_TrasferWarehouseNewPartial
	move inventory to warehouse || move warehouse to inventory total partial item 
INPUT
	@char_id		INT,
	@item_id			INT,
	@ToWarehouseID	INT,
	@bIsToInven		TINYINT
	@nCount			INT
OUTPUT
	bIsSuccess		1/0
return
made by
	carrot
date
	2002-10-17
********************************************/
ALTER PROCEDURE [DBO].[lin_TrasferWarehouseNewPartial]
(
	@char_id		INT,
	@item_id			INT,
	@ToWarehouseID	INT,
	@bIsToInven		TINYINT,
	@nCount			INT
)
AS
SET NOCOUNT ON

IF (@bIsToInven > 0) 
BEGIN
	IF (SELECT amount FROm user_warehouse WHERE item_id = @item_id) > @nCount
	BEGIN
		INSERT INTO user_item (char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse)
			SELECT char_id, item_type, @nCount, enchant, eroded, bless, ident, wished, warehouse FROM user_warehouse WHERE item_id = @item_id
		IF @@ROWCOUNT = 0
		BEGIN
			SELECT 0
		END
		ELSE 
		BEGIN
			SELECT item_id FROM user_item WHERE item_id = @@IDENTITY
			UPDATE user_warehouse SET amount = amount - @nCount WHERE item_id = @item_id
		END
	END
	ELSE
	BEGIN
		SELECT 0
	END
END
ELSE
BEGIN
	IF (SELECT amount FROm user_item WHERE item_id = @item_id) > @nCount
	BEGIN
		INSERT INTO user_item (char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse)
			SELECT char_id, item_type, @nCount, enchant, eroded, bless, ident, wished, warehouse FROM user_item WHERE item_id = @item_id
		INSERT INTO user_warehouse (item_id, char_id, item_type, amount, enchant, eroded, bless, ident, wished, warehouse)
			SELECT item_id, char_id, item_type, @nCount, enchant, eroded, bless, ident, wished, warehouse FROM user_item WHERE item_id = @@IDENTITY
		IF @@ROWCOUNT = 0
		BEGIN
			SELECT 0
		END
		ELSE 
		BEGIN
			SELECT item_id FROM user_warehouse WHERE item_id = @@IDENTITY
			DELETE user_item WHERE item_id = @@IDENTITY
			UPDATE user_item SET amount = amount - @nCount WHERE item_id = @item_id
		END
	END
	ELSE
	BEGIN
		SELECT 0
	END
END
GO

/****** Object:  Stored Procedure dbo.lin_UninstallAllBattleCamp    Script Date: 2003-09-20 ?? 11:52:00 ******/
-- lin_UninstallAllBattleCamp
-- by bert
-- return deleted battle camp ids

ALTER PROCEDURE dbo.lin_UninstallAllBattleCamp (@castle_id INT, @type INT)
AS

SET NOCOUNT ON

SELECT id FROM object_data WHERE residence_id = @castle_id AND type = @type

DELETE FROM object_data WHERE residence_id = @castle_id AND type = @type
GO

/****** Object:  Stored Procedure dbo.lin_UninstallBattleCamp    Script Date: 2003-09-20 ?? 11:52:00 ******/
-- lin_UninstalllBattleCamp
-- by bert
-- return deleted battle camp id

ALTER PROCEDURE dbo.lin_UninstallBattleCamp (@pledge_id INT, @type INT)
AS

SET NOCOUNT ON

SELECT id FROM object_data WHERE owner_id = @pledge_id AND type = @type

DELETE FROM object_data WHERE owner_id = @pledge_id AND type = @type
GO

ALTER PROCEDURE dbo.lin_UnregisterSiegeAgitPledge
(
	@agit_id INT,
	@pledge_id INT
)
AS
SET NOCOUNT ON

DECLARE @ret INT

DELETE FROM siege_agit_pledge
WHERE 
agit_id = @agit_id AND pledge_id = @pledge_id

IF @@ERROR = 0
BEGIN
	SELECT @ret = 1
END
ELSE
BEGIN
	SELECT @ret = 0
END

SELECT @ret
GO

ALTER PROCEDURE dbo.lin_UnregisterTeamBattleAgitMember
(
	@agit_id INT,
	@char_id INT,
	@pledge_id INT
)
AS
SET NOCOUNT ON

DECLARE @ret INT

DELETE FROM team_battle_agit_member
WHERE 
agit_id = @agit_id AND char_id = @char_id AND pledge_id = @pledge_id

IF @@ERROR = 0
BEGIN
	SELECT @ret = 1
END
ELSE
BEGIN
	SELECT @ret = 0
END

SELECT @ret
GO

ALTER PROCEDURE dbo.lin_UnregisterTeamBattleAgitPledge
(
	@agit_id INT,
	@pledge_id INT
)
AS
SET NOCOUNT ON

DECLARE @ret INT

BEGIN TRAN

DELETE FROM team_battle_agit_pledge
WHERE 
agit_id = @agit_id AND pledge_id = @pledge_id

IF @@ERROR <> 0
BEGIN
	SELECT @ret = 0
	GOTO EXIT_TRAN
END

DELETE FROM  team_battle_agit_member
WHERE agit_id = @agit_id AND pledge_id = @pledge_id

IF @@ERROR = 0
BEGIN
	SELECT @ret = 1
END
ELSE
BEGIN
	SELECT @ret = 0
END

EXIT_TRAN:

IF @ret <> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END

SELECT @ret
GO

/****** Object:  Stored Procedure dbo.lin_UpdateActiveSkill    Script Date: 2003-09-20 ?? 11:52:00 ******/    
ALTER PROCEDURE [DBO].[lin_UpdateActiveSkill]    
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
 @s20 INT, @l20 TINYINT, @d20 INT, @c20 TINYINT    
)    
AS    
SET NOCOUNT ON    
  
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
 s20 = @s20, l20 = @l20, d20 = @d20, c20 = @c20     
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
 s20, l20, d20, c20)     
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
 @s20, @l20, @d20, @c20)    
END
GO

/****** Object:  Stored Procedure dbo.lin_UpdateCastle    Script Date: 2003-09-20 ?? 11:51:57 ******/
ALTER PROCEDURE
dbo.lin_UpdateCastle (@id INT, @pledge_id INT, @next_war_time INT, @tax_rate SMALLINT)
AS
UPDATE castle
SET pledge_id = @pledge_id, next_war_time = @next_war_time, tax_rate = @tax_rate
WHERE id = @id
GO

/****** Object:  Stored Procedure dbo.lin_UpdateCastleWar    Script Date: 2003-09-20 ?? 11:52:00 ******/
-- lin_UpdateCastleWar
-- by bert ( -_-)/

ALTER PROCEDURE
dbo.lin_UpdateCastleWar (@castle_id INT, @pledge_id INT, @status INT)
AS
SET NOCOUNT ON

UPDATE castle_war
SET type = @status
WHERE castle_id = @castle_id AND pledge_id = @pledge_id

SELECT @@ROWCOUNT
GO

ALTER PROCEDURE [dbo].lin_UpdateControlTower
(
@control_level INT,
@hp INT,
@status INT,
@name VARCHAR(256)
)
AS
UPDATE control_tower
SET
control_level = @control_level,
hp = @hp,
status = @status
WHERE name = @name
IF @@ROWCOUNT <> 1
BEGIN
RAISERROR ('Failed to Update Control Tower name = %s.', 16, 1, @name)
END
GO

--	kuooo
ALTER PROCEDURE
dbo.lin_UpdateJournal ( @id INT,  
@j1 int, @j2 int, @j3 int, @j4 int, @j5 int, @j6 int, @j7 int, @j8 int,
@j9 int, @j10 int, @j11 int, @j12 int, @j13 int, @j14 int, @j15 int, @j16 int)
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
j16 = @j16
where char_id = @id
GO

ALTER PROCEDURE [DBO].[lin_UpdateLottoGame]
(
	@round_number int,
	@state int,
	@left_time int,
	@chosen_nuimber_flag int,
	@rule_number int,
	@total_count int,
	@winner1_count int,
	@winner2_count int,
	@winner3_count int,
	@winner4_count int,
	@carried_adena int
) 
AS       
SET NOCOUNT ON    

update lotto_game
set  	state = @state,
	left_time = @left_time,
	chosen_number_flag = @chosen_nuimber_flag ,
	rule_number = @rule_number ,
	total_count = @total_count ,
	winner1_count = @winner1_count ,
	winner2_count = @winner2_count ,
	winner3_count = @winner3_count ,
	winner4_count = @winner4_count ,
	carried_adena = @carried_adena 
where round_number = @round_number
GO

ALTER PROCEDURE [DBO].[lin_UpdateLottoGameState]
(
	@round_number int,
	@state int,
	@left_time int
) 
AS       
SET NOCOUNT ON    

update lotto_game
set  	state = @state,
	left_time = @left_time
where round_number = @round_number
GO

/********************************************
lin_UpdateMarketPrice
	update item market price
INPUT
	@item_type INT,
	@price INT
OUTPUT
return
made by
	kks
date
	2005-04-01
********************************************/
ALTER PROCEDURE [DBO].[lin_UpdateMarketPrice] 
(
@item_type INT,
@enchant INT,
@price INT
)
AS

SET NOCOUNT ON

DECLARE @avg_price FLOAT
DECLARE @frequency INT
DECLARE @new_avg_price FLOAT

IF EXISTS(SELECT TOP 1 * FROM item_market_price(nolock) WHERE item_type = @item_type AND enchant = @enchant)
    BEGIN
	UPDATE item_market_price
	SET 
		avg_price = ((avg_price * frequency) + @price) / (frequency + 1),
		frequency = frequency + 1
	WHERE item_type = @item_type AND
		enchant = @enchant
   END
ELSE
   BEGIN
	INSERT INTO item_market_price 
		(item_type, enchant, avg_price, frequency)
	VALUES 
		(@item_type, @enchant, @price, 1)
   END
GO

ALTER PROCEDURE [dbo].lin_UpdateMercenary
(
@id INT,
@x INT,
@y INT,
@z INT,
@angle INT,
@hp INT,
@mp INT
)
AS
UPDATE mercenary
SET
x = @x,
y = @y,
z = @z, 
angle = @angle,
hp = @hp,
mp = @mp
WHERE id = @id
IF @@ROWCOUNT <> 1
BEGIN
RAISERROR ('Failed to Update Mercenary id = %d.', 16, 1, @id)
END
GO

/********************************************
lin_UpdateMonRace
	update monster race
INPUT
	@race_id	int
	@winrate1	float,
	@winrate2	float
OUTPUT
return
made by
	young
date
	2004-5-18
********************************************/
ALTER PROCEDURE [DBO].[lin_UpdateMonRace]
(
@race_id		INT,
@winrate1		FLOAT,
@winrate2		FLOAT
)
AS
SET NOCOUNT ON

update monrace set winrate1 = @winrate1, winrate2 = @winrate2 where race_id = @race_id
GO

/********************************************
lin_UpdateMonRaceInfo
	update monster race info
INPUT
	@race_id	int
	@run1	float,
	@run2	float,
	@run3	float,
	@run4	float,
	@run5	float,
	@run6	float,
	@run7	float,
	@run8	float,
	@win1	int,
	@win2	int,

OUTPUT
return
made by
	young
date
	2004-5-18
********************************************/
ALTER  PROCEDURE [DBO].[lin_UpdateMonRaceInfo]
(
@race_id		INT,
@run1			FLOAT,
@run2			FLOAT,
@run3			FLOAT,
@run4			FLOAT,
@run5			FLOAT,
@run6			FLOAT,
@run7			FLOAT,
@run8			FLOAT,
@win1			int,
@win2			int

)
AS
SET NOCOUNT ON

update monrace set run1 = @run1, run2 = @run2, run3 = @run3, run4 = @run4, run5 = @run5, run6 = @run6, run7 = @run7, run8 = @run8, win1 = @win1, win2 = @win2 , race_end = 1 where race_id = @race_id
GO

/********************************************
lin_UpdateMonRaceMon
	update monster race
INPUT
	@runner_id	int
	@win_rate	int
OUTPUT
return
made by
	young
date
	2004-5-19
********************************************/
ALTER PROCEDURE [DBO].[lin_UpdateMonRaceMon]
(
@runner_id		INT,
@win_rate		INT,
@run_count		INT,
@win_count		INT
)
AS
SET NOCOUNT ON

if exists ( select * from monrace_mon (nolock) where runner_id = @runner_id ) 
begin
	update monrace_mon set initial_win = @win_rate , run_count = @run_count, win_count = @win_count where runner_id = @runner_id
end else begin
	insert into monrace_mon( runner_id, initial_win , run_count, win_count ) values ( @runner_id, @win_rate , @run_count, @win_count )
end 

select initial_win , run_count, win_count  from monrace_mon ( nolock) where runner_id = @runner_id
GO

/********************************************
lin_UpdateNewbieCharData
	update newbie char
INPUT
	@account_id	INT,
	@char_id 	INT
OUTPUT
return
made by
	kks
date
	2004-11-25
********************************************/
ALTER PROCEDURE [DBO].[lin_UpdateNewbieCharData]
(
	@account_id	INT,
	@char_id 	INT
)
AS
SET NOCOUNT ON

UPDATE user_newbie 
SET char_id = @char_id
WHERE account_id = @account_id
GO

-- kuooo
ALTER PROCEDURE [DBO].[lin_UpdateNpcBossVariable]
(
	@npc_name 	nvarchar(50),
	@i0		int
)
AS
SET NOCOUNT ON
UPDATE npc_boss	
SET 
i0 = @i0
where npc_db_name = @npc_name
GO

/****** Object:  Stored Procedure dbo.lin_UpdatePledgeInfo    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_UpdatePledgeInfo

INPUT
	@fieldName	nvarchar(50),
	@field_data	INT,
	@char_id	INT
OUTPUT
return
made by
	carrot
date
	2003-06-13
********************************************/
ALTER PROCEDURE [DBO].[lin_UpdatePledgeInfo]
(
@fieldName	nvarchar(50),
@field_data	INT,
@char_id	INT
)
AS
SET NOCOUNT ON

IF @fieldName = N'pledge_dismiss_time' begin update user_data set pledge_dismiss_time = @field_data where char_id =  @char_id end
ELSE IF @fieldName = N'pledge_ousted_time' begin update user_data set pledge_ousted_time = @field_data where char_id =  @char_id end
ELSE IF @fieldName = N'pledge_withdraw_time' begin update user_data set pledge_withdraw_time = @field_data where char_id =  @char_id end
ELSE IF @fieldName = N'surrender_war_id' begin update user_data set surrender_war_id = @field_data where char_id =  @char_id end
ELSE 
BEGIN 
	RAISERROR ('lin_UpdatePledgeInfo : invalid field [%s]', 16, 1, @fieldName)
	RETURN -1	
END
GO

/********************************************        
lin_updateSociality
 set sociality
INPUT        
 @char_id
 @sociality
 @sulffrage
 @last_changed
OUTPUT        
return        
       
made by        
 carrot        
date        
 2004-07-4
change        
********************************************/        
ALTER PROCEDURE [DBO].[lin_updateSociality]
(        
 @char_id INT,
 @sociality INT
)        
AS        
        
SET NOCOUNT ON        

UPDATE user_sociality 
SET sociality = @sociality 
WHERE char_id = @char_id
GO

/********************************************        
lin_updateSulffrageUsed
 set sulffrage
INPUT        
 @char_id
 @sociality
 @sulffrage
 @last_changed
OUTPUT        
return        
       
made by        
 carrot        
date        
 2004-07-4
change        
********************************************/        
ALTER PROCEDURE [DBO].[lin_updateSulffrageUsed]
(        
 @char_id INT,
 @sulffrage INT
)        
AS        
        
SET NOCOUNT ON        

UPDATE user_sociality 
SET used_sulffrage = @sulffrage
WHERE char_id = @char_id
GO

/****** Object:  Stored Procedure dbo.lin_UpdateUserItem    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_UpdateUserItem
	
INPUT
	@char_id	INT,
	@item_type	INT,
	@amount	INT,
	@enchant	INT,
	@eroded	INT,
	@bless		INT,
	@ident		INT,
	@wished	INT,
	@warehouse	INT,
	@item_id		INT
OUTPUT
return
made by
	carrot
date
	2002-06-09
********************************************/
ALTER PROCEDURE [DBO].[lin_UpdateUserItem]
(
@char_id	INT,
@item_type	INT,
@amount	INT,
@enchant	INT,
@eroded	INT,
@bless		INT,
@ident		INT,
@wished	INT,
@warehouse	INT,
@item_id		INT
)
AS
SET NOCOUNT ON

UPDATE user_item  set char_id=@char_id, item_type=@item_type, amount=@amount, enchant=@enchant, eroded=@eroded, bless=@bless, ident=@ident, wished=@wished, warehouse=@warehouse WHERE item_id=@item_id
GO

/****** Object:  Stored Procedure dbo.lin_UpdateUseTime    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_UpdateUseTime
	update character use time
INPUT
	char_id		int,
	usedtimesec	int
OUTPUT
return
made by
	young
date
	2003-03-26

		add usetime set
********************************************/
ALTER PROCEDURE [DBO].[lin_UpdateUseTime]
(
	@char_id	INT,
	@usedTimeSec	INT
)
AS

SET NOCOUNT ON

UPDATE user_data SET use_time = use_time + @usedTimeSec WHERE char_id = @char_id
GO

/****** Object:  Stored Procedure dbo.lin_UpdateWarehouseItem    Script Date: 2003-09-20 ?? 11:51:57 ******/
/********************************************
lin_UpdateWarehouseItem 
	
INPUT
	@warehouse	INT,
	@amount	INT,
	@item_id		INT
OUTPUT
return
made by
	carrot
date
	2002-06-10
********************************************/
ALTER PROCEDURE [DBO].[lin_UpdateWarehouseItem]
(
	@warehouse	INT,
	@amount	INT,
	@item_id		INT
)
AS
SET NOCOUNT ON

UPDATE user_item SET warehouse=@warehouse,amount=@amount WHERE item_id=@item_id
GO

/****** Object:  Stored Procedure dbo.lin_ViewSiegeList    Script Date: 2003-09-20 ?? 11:52:00 ******/
/********************************************
lin_ViewSiegeList
	
INPUT	
	@castle_id	int,
	@type	int
OUTPUT
	pledge_id, 
	name 
return
made by
	carrot
date
	2002-06-16
********************************************/
ALTER PROCEDURE [DBO].[lin_ViewSiegeList]
(
	@castle_id	int,
	@type	int
)
AS
SET NOCOUNT ON

SELECT 
	p.pledge_id, p.name 
FROM 
	pledge p (nolock) , 
	castle_war cw (nolock)  
WHERE 
	p.pledge_id = cw.pledge_id 
	AND cw.castle_id = @castle_id
	AND cw.type = @type
GO

/****** Object:  Stored Procedure dbo.lin_WithdrawAlliance    Script Date: 2003-09-20 ?? 11:52:00 ******/
-- lin_WithdrawAlliance
-- by bert
-- return Result(0 if failed)

ALTER PROCEDURE
dbo.lin_WithdrawAlliance (@alliance_id INT, @member_pledge_id INT, @alliance_withdraw_time INT)
AS

SET NOCOUNT ON

DECLARE @result INT

UPDATE pledge
SET alliance_id = 0, alliance_withdraw_time = @alliance_withdraw_time
WHERE pledge_id = @member_pledge_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	SELECT @result = 1
END
ELSE
BEGIN
	SELECT @result = 0
END

SELECT @result
GO

/****** Object:  Stored Procedure dbo.lin_WithdrawPledgeMember    Script Date: 2003-09-20 ?? 11:51:57 ******/
-- lin_WithdrawPledgeMember
-- by bert

ALTER PROCEDURE
dbo.lin_WithdrawPledgeMember (@pledge_id INT, @member_id INT)
AS

SET NOCOUNT ON

DECLARE @ret INT

BEGIN TRAN

UPDATE user_data
SET pledge_id = 0
WHERE char_id = @member_id
AND pledge_id = @pledge_id

IF @@ERROR = 0 AND @@ROWCOUNT = 1
BEGIN
	-- ???? ??? ???
	SELECT @ret = 1
END
ELSE
BEGIN
	SELECT @ret = 0
	GOTO EXIT_TRAN
END

EXIT_TRAN:
IF @ret<> 0
BEGIN
	COMMIT TRAN
END
ELSE
BEGIN
	ROLLBACK TRAN
END
SELECT @ret
GO

/****** Object:  Stored Procedure dbo.lin_writeBbsall    Script Date: 2003-09-20 ?? 11:51:57 ******/
ALTER PROCEDURE [DBO].[lin_writeBbsall]
(
	@title NVARCHAR(50), 
	@contents NVARCHAR(4000), 
	@writer NVARCHAR(50)
)
AS
insert into bbs_all (title, contents, writer) values (@title, @contents, @writer)
GO

/****** Object:  Stored Procedure dbo.lin_writeBbsTGS    Script Date: 2003-09-20 ?? 11:51:57 ******/
ALTER PROCEDURE [DBO].[lin_writeBbsTGS]
(
	@title NVARCHAR(50), 
	@contents NVARCHAR(4000), 
	@writer NVARCHAR(50)
)
AS


insert into bbs_tgs (title, contents, writer) values (@title, @contents, @writer)
GO

/********************************************      
lin_WriteCastleTax      
       
INPUT      
OUTPUT      
return      
made by      
 carrot      
date      
 2002-06-10      
change carrot 2003-12-22    
 add tax_type, residence type to differentiate agit from castle    
change carrot 2004-02-29  
 add TaxRateToChange  
********************************************/      
ALTER PROCEDURE [DBO].[lin_WriteCastleTax]      
(      
 @nIsCastle int,    
 @tax_type int,      
 @tax_rate  int,      
 @to_change  int,      
 @residence_id  int      
)      
AS      
SET NOCOUNT ON      
    
IF(@nIsCastle = 1)    
BEGIN    
 IF (@tax_type = 2)    
  UPDATE castle SET tax_child_rate = @tax_rate, tax_child_rate_to_change = @to_change  WHERE id = @residence_id      
 ELSE IF (@tax_type = 1)    
  UPDATE castle SET tax_rate = @tax_rate, tax_rate_to_change = @to_change  WHERE id = @residence_id      
 ELSE    
  RAISERROR ('tax type is invalid. castle cannot save type[%d] id and castle id[%d].', 16, 1, @tax_type, @residence_id )    
END    
ELSE IF(@nIsCastle = 0)    
BEGIN    
 IF (@tax_type = 2)    
BEGIN  
  UPDATE agit SET tax_child_rate = @tax_rate, tax_child_rate_to_change = @to_change  WHERE id = @residence_id      
END  
 ELSE IF (@tax_type = 1)    
  UPDATE agit SET tax_rate = @tax_rate, tax_rate_to_change = @to_change WHERE id = @residence_id      
 ELSE    
  RAISERROR ('tax type is invalid. agit cannot save type[%d] id and agit id[%d].', 16, 1, @tax_type, @residence_id )    
END
GO

ALTER PROCEDURE dbo.lin_WriteHeroWords
(
@char_id INT,
@hero_words VARCHAR(128)
)
AS
SET NOCOUNT ON

UPDATE user_nobless
SET words = @hero_words
WHERE char_id = @char_id
GO

ALTER PROCEDURE dbo.lin_WriteNoblessAchievement
(
@char_id INT,
@win_type INT,
@target INT,
@win_time INT
)
AS
SET NOCOUNT ON

INSERT INTO nobless_achievements
(char_id, win_type, target, win_time)
VALUES
(@char_id, @win_type, @target, @win_time)
GO

ALTER PROCEDURE [dbo].SP_DBCC

AS  

DECLARE @JOBKIND VARCHAR(20)
DECLARE @ST_TIME DATETIME  
DECLARE @EN_TIME DATETIME  
DECLARE @ST_SIZE  INT  
DECLARE @END_SIZE  INT  

SELECT @ST_SIZE=0, @END_SIZE=0  


--==========================================
SELECT @JOBKIND = 'CLEANUP'
BEGIN
	SELECT @ST_TIME=getdate()  
	--1.  ?? ?? ITEM ??
	EXEC lin_DeleteNotOwnedItem  
	--2. GHOST ??? ??
	EXEC lin_CleanUpGhostData
	SELECT @EN_TIME=getdate()
	
	INSERT lin2audit..dbcc_history  
		SELECT hostname,loginame,@JOBKIND,@ST_TIME,@EN_TIME,datediff(minute,@ST_TIME,@EN_TIME),@ST_SIZE,@END_SIZE  
		FROM master.dbo.sysprocesses 
		WHERE spid = @@spid
END
--==========================================

--==========================================   
SELECT @JOBKIND = 'CHECKDB'  
BEGIN  
	SELECT @ST_TIME=getdate()  
	DBCC CHECKDB('lin2world')
	SELECT @EN_TIME=getdate()  

	INSERT lin2audit..dbcc_history  
		SELECT hostname,loginame,@JOBKIND,@ST_TIME,@EN_TIME,datediff(minute,@ST_TIME,@EN_TIME),@ST_SIZE,@END_SIZE  
		FROM master.dbo.sysprocesses 
		WHERE spid = @@spid
END
--==========================================

--==========================================  
SELECT @JOBKIND = 'reindex'  
BEGIN  
	SELECT @ST_SIZE=sum(dpages)*8/1024 from sysindexes where id = object_id('user_item')  
	SELECT @ST_TIME=getdate()  
	DBCC DBREINDEX('user_item','',90)  
	SELECT @EN_TIME=getdate()  
	SELECT @END_SIZE=sum(dpages)*8/1024 from sysindexes where id = object_id('user_item')  

	INSERT lin2audit..dbcc_history  
		SELECT hostname,loginame,@JOBKIND,@ST_TIME,@EN_TIME,datediff(minute,@ST_TIME,@EN_TIME),@ST_SIZE,@END_SIZE  
		FROM master.dbo.sysprocesses 
		WHERE spid = @@spid
END  
--==========================================
GO

/****** Object:  Stored Procedure dbo.sp_test    Script Date: 2003-09-20 ?? 11:51:57 ******/
ALTER procedure [dbo].sp_test
(
	@strIn	NCHAR(30)
)
as 

set @strIn = RTRIM(@strIn)

IF @strIn LIKE N' %' 
BEGIN
	print 'Space [' + @strIn + ']'
	RAISERROR ('Character name has space : name = [%s]', 16, 1, '')
	return -1
END

print 'No space [' + @strIn + ']'
GO

/****** Object:  Stored Procedure dbo.spCopyXYZlocForDummy    Script Date: 2003-09-20 ?? 11:51:57 ******/
ALTER PROCEDURE [dbo].spCopyXYZlocForDummy (@nSrc INT, @nDest INT, @nCount INT)
AS

DECLARE @i INT
DECLARE @xloc INT
DECLARE @yloc INT
DECLARE @zloc INT

SELECT @i = 0

WHILE @i < @nCount
BEGIN

SELECT @xloc = xloc, @yloc = yloc, @zloc = zloc
FROM user_data
WHERE char_name = 'dummy0' + STR(@nSrc + @i, 4)

UPDATE user_data
SET xloc = @xloc, yloc = @yloc, zloc = @zloc
WHERE char_name = 'dummy0' + STR(@nDest + @i, 4)

SELECT @i = @i + 1

END
GO

ALTER PROCEDURE [dbo].STAT_ITEM_ACC      
AS        
BEGIN        
        
TRUNCATE TABLE LIN2AUDIT..STAT_ITEM_CNT      
TRUNCATE TABLE LIN2AUDIT..STAT_ITEM_ENT        
TRUNCATE TABLE LIN2AUDIT..STAT_ITEM_MENT        
TRUNCATE TABLE LIN2AUDIT..STAT_ITEM_MINCNT        
        
--1. ?? ??? ???? ???? ??? ??        
--================================================================================================================================      
DECLARE @LAST_ITEM_TYPE INT      
DECLARE @SQL    VARCHAR(1000)      
      
DECLARE ITEM_TYPE_CURSOR CURSOR      
 FOR SELECT TOP 1 ITEM_TYPE FROM USER_ITEM ORDER BY ITEM_TYPE DESC      
OPEN ITEM_TYPE_CURSOR      
FETCH NEXT FROM ITEM_TYPE_CURSOR      
INTO @LAST_ITEM_TYPE      
WHILE @@FETCH_STATUS = 0      
BEGIN      
      
DECLARE @I VARCHAR(10)      
SET @I = 1      
WHILE (@I < @LAST_ITEM_TYPE)      
BEGIN      
 SET @SQL = 'INSERT INTO LIN2AUDIT..STAT_ITEM_CNT '      
 SET @SQL = @SQL + 'SELECT '+@I+' AS ITEM_TYPE, COUNT(*) AS ''??? ???'', SUM(CAST(AMOUNT AS BIGINT))  AS ''??? ??'' '      
 SET @SQL = @SQL + 'FROM USER_ITEM WITH (NOLOCK) WHERE ITEM_TYPE = '+@I+''      
 EXEC (@SQL)      
 SET @I = @I + 1      
END      
      
FETCH NEXT FROM ITEM_TYPE_CURSOR      
INTO @LAST_ITEM_TYPE      
END      
CLOSE ITEM_TYPE_CURSOR      
DEALLOCATE ITEM_TYPE_CURSOR      
--================================================================================================================================      
       
---------------------------------------------------------------        
--2. ?? ??? ???? +1 ??? ???? ???? ? ??        
        
INSERT INTO LIN2AUDIT..STAT_ITEM_ENT        
 SELECT ITEM_TYPE, ENCHANT , COUNT(ITEM_TYPE)  AS '??? ???', SUM(AMOUNT) AS '??? ??'        
 FROM USER_ITEM WITH (NOLOCK)         
 WHERE ENCHANT >0        
 GROUP BY ITEM_TYPE, ENCHANT        
 ORDER BY ITEM_TYPE, ENCHANT ASC        
        
----------------------------------------------------------------        
--3. ?? ???? ???? ?? ?? ???? unique_id         
INSERT INTO LIN2AUDIT..STAT_ITEM_MENT        
 SELECT T2.ITEM_TYPE, T2.ITEM_ID, UD.CHAR_NAME, T2.ME,  UD.BUILDER        
 FROM      
 (        
 SELECT UI.CHAR_ID AS CHAR_ID ,UI.ITEM_TYPE AS ITEM_TYPE, UI.ITEM_ID AS ITEM_ID, t.MAX_ENCHANT AS ME        
 FROM USER_ITEM AS UI INNER JOIN         
 (         
 SELECT ITEM_TYPE, MAX(ENCHANT) AS MAX_ENCHANT        
 FROM USER_ITEM WITH (NOLOCK)        
 WHERE ENCHANT >0        
 GROUP BY ITEM_TYPE        
         
 ) AS T        
 on UI.ITEM_TYPE = T.ITEM_TYPE AND UI.ENCHANT = T.MAX_ENCHANT         
 --ORDER BY UI.ITEM_TYPE        
 ) AS T2 INNER JOIN USER_DATA AS UD        
 on T2.CHAR_ID = UD.CHAR_ID        
 WHERE UD.ACCOUNT_ID<>-1        
 ORDER BY T2.ITEM_TYPE        
      
------------------------------------------------------------------        
        
--4. 1??? ??? ?? ??? ? ??? 10??? ???? unique_id        
INSERT INTO LIN2AUDIT..STAT_ITEM_MINCNT        
 SELECT  ITEM_TYPE , ITEM_ID        
 FROM USER_ITEM        
 WHERE ITEM_TYPE in (SELECT ITEM_TYPE        
   FROM USER_ITEM WITH (NOLOCK)         
   GROUP BY ITEM_TYPE        
   HAVING COUNT(ITEM_TYPE)<11)        
 ORDER BY ITEM_TYPE        
        
--====================================================================        
        
TRUNCATE TABLE LIN2AUDIT..STAT_ACC_RACE      
TRUNCATE TABLE LIN2AUDIT..STAT_ACC_CLASS        
TRUNCATE TABLE LIN2AUDIT..STAT_ACC_LEV        
TRUNCATE TABLE LIN2AUDIT..STAT_ACC_MLEV        
        
--1. ??? ???? COUNT        
INSERT INTO LIN2AUDIT..STAT_ACC_RACE        
 SELECT RACE,COUNT(RACE) FROM USER_DATA WITH (NOLOCK) WHERE BUILDER=0 AND ACCOUNT_ID > 0 GROUP BY RACE ORDER BY RACE        
        
INSERT INTO LIN2AUDIT..STAT_ACC_CLASS        
 SELECT CLASS,COUNT(CLASS) FROM USER_DATA WITH (NOLOCK) WHERE BUILDER=0 AND ACCOUNT_ID > 0 GROUP BY CLASS ORDER BY CLASS        
        
--2. ??? COUNT        
INSERT INTO LIN2AUDIT..STAT_ACC_LEV        
 SELECT LEV,COUNT(LEV) FROM USER_DATA WITH (NOLOCK) WHERE BUILDER=0 AND ACCOUNT_ID > 0 GROUP BY LEV ORDER BY LEV        
        
--3. 70????? ?????        
INSERT INTO LIN2AUDIT..STAT_ACC_MLEV        
 SELECT ACCOUNT_NAME, CHAR_NAME, LEV, RACE, CLASS        
 FROM USER_DATA WITH (NOLOCK)      
 WHERE LEV>=76 AND BUILDER=0 AND ACCOUNT_ID > 0    
 ORDER BY LEV DESC        
         
END
GO

ALTER proc [dbo].table_info    
as    
    
    
declare @id int       
declare @type character(2)       
declare @pages int       
declare @dbname sysname    
declare @dbsize dec(15,0)    
declare @bytesperpage dec(15,0)    
declare @pagesperMB  dec(15,0)    
    
create table #spt_space    
(    
 objid  int null,    
 rows  int null,    
 reserved dec(15) null,    
 data  dec(15) null,    
 indexp  dec(15) null,    
 unused  dec(15) null    
)    
    
set nocount on    
    
-- Create a cursor to loop through the user tables    
declare c_tables cursor for    
select id    
from sysobjects    
where xtype = 'U'    
    
open c_tables    
    
fetch next from c_tables    
into @id    
    
while @@fetch_status = 0    
begin    
    
 /* Code from sp_spaceused */    
 insert into #spt_space (objid, reserved)    
  select objid = @id, sum(reserved)    
   from sysindexes    
    where indid in (0, 1, 255)    
     and id = @id    
    
 select @pages = sum(dpages)    
   from sysindexes    
    where indid < 2    
     and id = @id    
 select @pages = @pages + isnull(sum(used), 0)    
  from sysindexes    
   where indid = 255    
    and id = @id    
 update #spt_space    
  set data = @pages    
 where objid = @id    
    
    
 /* index: sum(used) where indid in (0, 1, 255) - data */    
 update #spt_space    
  set indexp = (select sum(used)    
    from sysindexes    
    where indid in (0, 1, 255)    
    and id = @id)    
       - data    
  where objid = @id    
    
 /* unused: sum(reserved) - sum(used) where indid in (0, 1, 255) */    
 update #spt_space    
  set unused = reserved    
    - (select sum(used)    
     from sysindexes    
      where indid in (0, 1, 255)    
      and id = @id)    
  where objid = @id    
    
 update #spt_space    
  set rows = i.rows    
   from sysindexes i    
    where i.indid < 2    
    and i.id = @id    
    and objid = @id    
    
 fetch next from c_tables    
 into @id    
end    
    
select  TableName = (select left(name,60) from sysobjects where id = objid),    
 Rows = convert(char(11), rows),    
 ReservedKB = ltrim(str(reserved * d.low / 1024.,15,0)),    
 DataKB = ltrim(str(data * d.low / 1024.,15,0)),    
 IndexSizeKB = ltrim(str(indexp * d.low / 1024.,15,0)),    
 UnusedKB = ltrim(str(unused * d.low / 1024.,15,0))    
      
from  #spt_space, master.dbo.spt_values d    
where  d.number = 1    
and  d.type = 'E'    
order by tablename     
drop table #spt_space    
close c_tables    
deallocate c_tables
GO

ALTER PROC [dbo].TranLogBackup
	@DBNAME varchar(20)
AS

--==================================================================================
--?? ?? ??
	DECLARE 
		@query varchar(1000),
		@cur_hour tinyint,
		@del_hour tinyint,
		@cur_device varchar(50),
		@del_device varchar(50),
		@description varchar(48),
		@physical_name  varchar(64)  
		

--==================================================================================
--?? ?? ??
	SET @cur_hour = datepart(hour,getdate())
	SET @del_hour = datepart( hour,dateadd(hour,-24,getdate())) 

--==================================================================================
--???? 
	IF @cur_hour = 6 --full backup ?? ??
		RETURN


	SET @cur_device = @@servername + '_' + @DBNAME + '_tlog_' + CONVERT(char(8),getdate(),112) + '_' 
	SET @del_device = @@servername + '_' + @DBNAME + '_tlog_' + CONVERT(char(8),dateadd(hour,-24,getdate()),112) + '_' 
	SET @description = 'Log Hourly Backup , ' + @DBNAME  + '  ' + CONVERT(CHAR(16),GETDATE(),20)  

	
	IF @cur_hour < 10
		SET @cur_device = @cur_device + '0' + convert(char(1),@cur_hour) 
	ELSE
		SET @cur_device = @cur_device + convert(char(2),@cur_hour)


	IF @del_hour < 10
		SET @del_device = @del_device + '0' + convert(char(1),@del_hour) 
	ELSE
		SET @del_device = @del_device + convert(char(2),@del_hour)

	SET @physical_name = 'D:\MSSQL\BACKUP\' + @cur_device + '.tbak'

	PRINT 'cur_device : ' + @cur_device
	PRINT 'del_device : ' + @del_device
	PRINT 'physical_name : ' + @physical_name
	

--==================================================================================
--12 ?? ??? device? ????.
	IF ( select count(*) from master.dbo.sysdevices where name = rtrim(@del_device) ) = 1
		EXEC sp_dropdevice @del_device,'delfile'    


--==================================================================================
--dump device? ????.
	IF ( select count(*) from master.dbo.sysdevices where name = rtrim(@cur_device) ) = 0   
		EXEC sp_addumpdevice 'disk' , @cur_device , @physical_name  


--==================================================================================
--transaction log ? ?????.
	SET @query = 'BACKUP LOG ' + @DBNAME + ' TO ' + @cur_device
	PRINT @query
	EXECUTE(@query)
GO
