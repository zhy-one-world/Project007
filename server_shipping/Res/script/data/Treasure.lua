local treasure_data =
{
	--皇家宝藏
	royal_treasure_data=
	{
		{item_id = 31080095, item_num = 1, item_quality = 2},--1  黄金圣衣
		{item_id = 31000105, item_num = 16, item_quality = 0},--2  神兽升级丹
		{item_id = 31000106, item_num = 5, item_quality = 0},--3  神兽进阶丹
		{item_id = 31090543, item_num = 1, item_quality = 0},--4  紫色兽魂礼包
		{item_id = 31090544, item_num = 1, item_quality = 0},--5  橙色兽魂礼包
		{item_id = 31025241, item_num = 1, item_quality = 1},--6  魔法书
		{item_id = 31020004, item_num = 100, item_quality = 0},--7  强化石
		{item_id = 31020003, item_num = 15, item_quality = 0},--8  强化结晶
		{item_id = 31090039, item_num = 1, item_quality = 1},--9  7级宝石礼包
		{item_id = 31000099, item_num = 16, item_quality = 0},--10  洛克神羽
		{item_id = 31090543, item_num = 1, item_quality = 0},--11  紫色精魂礼包
		{item_id = 31090544, item_num = 1, item_quality = 0},--12  橙色精魂礼包
		{item_id = 31020005, item_num = 8, item_quality = 0},--13  神佑卷轴
		{item_id = 31000166, item_num = 1, item_quality = 1},--14  8级宝石礼包
		{item_id = 31010060, item_num = 40, item_quality = 0},--15  100火种点数卡
		{item_id = 31010054, item_num = 40, item_quality = 0},--16  10000魔晶提取卡
	},
	royal_treasure_percent =
	{
		prize_1 =
		{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
		},
		prize_10 =
		{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
								}
			}
		},
	},
		--精灵宝藏
	spirit_treasure_data=
	{
		{item_id = 31051011, item_num = 20, item_quality = 2},--1  精灵11_天猛星__天神碎片
		{item_id = 31051001, item_num = 10, item_quality = 0},--2  精灵01_仙女座__青铜碎片
		{item_id = 31051002, item_num = 10, item_quality = 0},--3  精灵02_天鹅座__青铜碎片
		{item_id = 31051003, item_num = 10, item_quality = 0},--4  精灵03_凤凰座__青铜碎片
		{item_id = 31051013, item_num = 10, item_quality = 0},--5  精灵13_幼师座__青铜碎片
		{item_id = 31051007, item_num = 5, item_quality = 1},--6  精灵07_双鱼座__黄金碎片
		{item_id = 31010107, item_num = 8, item_quality = 0},--7  灵晶100*8
		{item_id = 31010107, item_num = 10, item_quality = 0},--8  灵晶100*10
		{item_id = 31051007, item_num = 2, item_quality = 1},--9  精灵07_双鱼座__黄金碎片
		{item_id = 31051004, item_num = 5, item_quality = 0},--10 精灵04_天琴座__白银碎片
		{item_id = 31051005, item_num = 5, item_quality = 0},--11 精灵05_猎户座__白银碎片
		{item_id = 31051004, item_num = 5, item_quality = 0},--12 精灵04_天琴座__白银碎片
		{item_id = 31051005, item_num = 5, item_quality = 0},--13 精灵05_猎户座__白银碎片
		{item_id = 31000170, item_num = 2, item_quality = 1},--14 觉醒丹
		{item_id = 31010107, item_num = 12, item_quality = 0},--15 灵晶100*12
		{item_id = 31010107, item_num = 15, item_quality = 0},--16 灵晶100*15
	},
	spirit_treasure_percent =
	{
		prize_1 =
		{
			cost_money = {money_type = 3, money_value = 100},
			cost_item = {item_id = 31000171, item_num = 1},
			add_score = 6,
			add_luck = 1,
			add_item = {item_id = 31000172, item_num = 1},
			first  ={
						{prize_index = 10, prize_percent = 2500},
						{prize_index = 11, prize_percent = 2500},
						{prize_index = 12, prize_percent = 2500},
						{prize_index = 13, prize_percent = 2500}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 850},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 6, prize_percent = 200},
								{prize_index = 7, prize_percent = 1400},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 9, prize_percent = 450},
								{prize_index = 10, prize_percent = 800},
								{prize_index = 11, prize_percent = 600},
								{prize_index = 12, prize_percent = 400},
								{prize_index = 13, prize_percent = 300},
								{prize_index = 14, prize_percent = 300},
								{prize_index = 15, prize_percent = 900},
								{prize_index = 16, prize_percent = 500}
							},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 10000}
			}
		},
		prize_10 =
		{
			cost_money = {money_type = 3, money_value = 800},
			cost_item = {item_id = 31000171, item_num = 8},
			add_score = 60,
			add_luck = 10,
			add_item = {item_id = 31000172, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 850},
								{prize_index = 4, prize_percent = 750},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1600},
								{prize_index = 8, prize_percent = 1300},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 700},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 350},
								{prize_index = 15, prize_percent = 1000},
								{prize_index = 16, prize_percent = 600}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 14, prize_percent = 10000}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 850},
								{prize_index = 4, prize_percent = 750},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1600},
								{prize_index = 8, prize_percent = 1300},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 700},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 350},
								{prize_index = 15, prize_percent = 1000},
								{prize_index = 16, prize_percent = 600}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 2200},
									{prize_index = 9, prize_percent = 4400},
									{prize_index = 14, prize_percent = 3400}
								},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 7, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 7, prize_percent = 10000}
								}
			}
		},
	},
	--符文宝藏
	element_treasure_data=
	{
		{item_id = 31060404, item_num = 1, item_quality = 2},--1  红色暴击率符文
		{item_id = 31060302, item_num = 1, item_quality = 0},--2  橙色命中率符文
		{item_id = 31060303, item_num = 1, item_quality = 0},--3  橙色闪避率符文
		{item_id = 31060306, item_num = 1, item_quality = 0},--4  橙色暴伤符文
		{item_id = 31060307, item_num = 1, item_quality = 0},--5  橙色免暴伤符文
		{item_id = 31060408, item_num = 1, item_quality = 1},--6  红色纯粹伤害符文
		{item_id = 31060206, item_num = 1, item_quality = 0},--7 紫色暴伤符文
		{item_id = 31060304, item_num = 1, item_quality = 0},--8  橙色暴击率符文
		{item_id = 31060409, item_num = 1, item_quality = 1},--9  红色纯粹防御符文
		{item_id = 31060204, item_num = 1, item_quality = 0},--10 紫色暴击率符文
		{item_id = 31060205, item_num = 1, item_quality = 0},--11 紫色暴抗率符文
		{item_id = 31060208, item_num = 1, item_quality = 0},--12 紫色纯粹伤害符文
	--[[	{item_id = 31060034, item_num = 1, item_quality = 0},--13 紫色生命
		{item_id = 31061005, item_num = 1, item_quality = 1},--14 红色符文精华
		{item_id = 31061004, item_num = 2, item_quality = 0},--15 橙色符文精华
		{item_id = 31061003, item_num = 4, item_quality = 0},--16 紫色符文精华--]]
	},
	element_treasure_percent =
	{
		prize_1 =
		{
			cost_money = {money_type = 3, money_value = 150},
			cost_item = {item_id = 31000163, item_num = 1},
			add_score = 0,
			add_luck = 1,
			add_item = {item_id = 31090045, item_num = 0},
			first  ={
						{prize_index = 8, prize_percent = 10000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 1000},
								{prize_index = 4, prize_percent = 1000},
								{prize_index = 5, prize_percent = 1000},
								{prize_index = 6, prize_percent = 100},
								{prize_index = 7, prize_percent = 10000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 9, prize_percent = 100},
								{prize_index = 10, prize_percent = 10000},
								{prize_index = 11, prize_percent = 10000},
								{prize_index = 12, prize_percent = 10000}
					--[[			{prize_index = 13, prize_percent = 315},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1350},
								{prize_index = 16, prize_percent = 1728}--]]
							},
				best_prize =
				{
					basics = 0,
					score_add = 0,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 1800},
				{prize_index = 10, prize_percent = 1800},
				{prize_index = 11, prize_percent = 1800},
				{prize_index = 12, prize_percent = 1800},
				{prize_index = 2, prize_percent = 200},				
				{prize_index = 3, prize_percent = 200},
				{prize_index = 4, prize_percent = 200},
				{prize_index = 5, prize_percent = 200},
				{prize_index = 8, prize_percent = 200}				
			}
		},
		prize_10 =
		{
			cost_money = {money_type = 3, money_value = 1500},
			cost_item = {item_id = 31000163, item_num = 10},
			add_score = 0,
			add_luck = 10,
			add_item = {item_id = 31090045, item_num = 0},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 1000},
								{prize_index = 4, prize_percent = 1000},
								{prize_index = 5, prize_percent = 1000},
								{prize_index = 6, prize_percent = 100},
								{prize_index = 7, prize_percent = 10000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 9, prize_percent = 100},
								{prize_index = 10, prize_percent = 10000},
								{prize_index = 11, prize_percent = 10000},
								{prize_index = 12, prize_percent = 10000},
					--[[			{prize_index = 13, prize_percent = 315},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1350},
								{prize_index = 16, prize_percent = 1728}--]]
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 4, prize_percent = 5000}
							},
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 1000},
								{prize_index = 4, prize_percent = 1000},
								{prize_index = 5, prize_percent = 1000},
								{prize_index = 6, prize_percent = 100},
								{prize_index = 7, prize_percent = 10000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 9, prize_percent = 100},
								{prize_index = 10, prize_percent = 10000},
								{prize_index = 11, prize_percent = 10000},
								{prize_index = 12, prize_percent = 10000},
					--[[			{prize_index = 13, prize_percent = 315},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1350},
								{prize_index = 16, prize_percent = 1728}--]]
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 4, prize_percent = 5000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 0,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 11, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 11, prize_percent = 10000}
							},
			}
		},
	},

	time_limit_activity_treasure_data=
	{
		{
			{item_id = 31080095, item_num = 1, item_quality = 2},--1  黄金圣衣   （皇家转盘1）
			{item_id = 31000105, item_num = 16, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 5, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 1, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 1, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31025241, item_num = 1, item_quality = 1},--6  魔法书
			{item_id = 31020004, item_num = 100, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 15, item_quality = 0},--8  强化结晶
			{item_id = 31090039, item_num = 1, item_quality = 1},--9  7级宝石礼包
			{item_id = 31000099, item_num = 16, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 1, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 1, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31020005, item_num = 8, item_quality = 0},--13  神佑卷轴
			{item_id = 31000166, item_num = 1, item_quality = 1},--14  8级宝石礼包
			{item_id = 31010060, item_num = 40, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 40, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31080103, item_num = 1, item_quality = 2},--1  机械时装 （皇家转盘2）
			{item_id = 31000105, item_num = 18, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 6, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 2, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 2, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31025241, item_num = 1, item_quality = 1},--6  魔法书
			{item_id = 31020004, item_num = 100, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 16, item_quality = 0},--8  强化结晶
			{item_id = 31000174, item_num = 1, item_quality = 1},--9  冰翎羽解锁
			{item_id = 31000099, item_num = 18, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 2, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 2, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 1, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31000166, item_num = 1, item_quality = 1},--14  8级宝石礼包
			{item_id = 31010060, item_num = 50, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 50, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31090559, item_num = 1, item_quality = 2},--1  海军时装-衣服+武器  （皇家转盘3）
			{item_id = 31000105, item_num = 20, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 8, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31025241, item_num = 1, item_quality = 1},--6  魔法书
			{item_id = 31020004, item_num = 110, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 18, item_quality = 0},--8  强化结晶
			{item_id = 31000174, item_num = 1, item_quality = 1},--9  冰翎羽解锁
			{item_id = 31000099, item_num = 20, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 1, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31000175, item_num = 1, item_quality = 1},--14  火翎羽解锁
			{item_id = 31010060, item_num = 60, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 60, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31090560, item_num = 1, item_quality = 2},--1  泳装时装-衣服+武器  （皇家转盘4）
			{item_id = 31000105, item_num = 20, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 8, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31000174, item_num = 1, item_quality = 1},--6  冰翎羽解锁
			{item_id = 31020004, item_num = 120, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 20, item_quality = 0},--8  强化结晶
			{item_id = 31000175, item_num = 1, item_quality = 1},--9  火翎羽解锁
			{item_id = 31000099, item_num = 20, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 60, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 60, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31080095, item_num = 2, item_quality = 2},--1  黄金时装   （皇家转盘5）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31000174, item_num = 1, item_quality = 1},--6  冰翎羽解锁
			{item_id = 31020004, item_num = 150, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 25, item_quality = 0},--8  强化结晶
			{item_id = 31000175, item_num = 1, item_quality = 1},--9  火翎羽解锁
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 75, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 75, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31080103, item_num = 2, item_quality = 2},--1  机械时装  （皇家转盘6）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31000174, item_num = 1, item_quality = 1},--6  冰翎羽解锁
			{item_id = 31020004, item_num = 150, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 25, item_quality = 0},--8  强化结晶
			{item_id = 31000175, item_num = 1, item_quality = 1},--9  火翎羽解锁
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 75, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 75, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31090559, item_num = 2, item_quality = 2},--1  海军时装-衣服+武器  （皇家转盘7）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31000174, item_num = 1, item_quality = 1},--6  冰翎羽解锁
			{item_id = 31020004, item_num = 150, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 25, item_quality = 0},--8  强化结晶
			{item_id = 31000175, item_num = 1, item_quality = 1},--9  火翎羽解锁
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 75, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 75, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31090560, item_num = 2, item_quality = 2},--1  泳装时装-衣服+武器 （皇家转盘8）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31000174, item_num = 1, item_quality = 1},--6  冰翎羽解锁
			{item_id = 31020004, item_num = 150, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 25, item_quality = 0},--8  强化结晶
			{item_id = 31000175, item_num = 1, item_quality = 1},--9  火翎羽解锁
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 75, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 75, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31090606, item_num = 2, item_quality = 2},--1  机械时装-蓝色-衣服+武器  （皇家转盘9）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31091153, item_num = 1, item_quality = 1},--6  宝藏自选箱A
			{item_id = 31020004, item_num = 180, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 30, item_quality = 0},--8  强化结晶
			{item_id = 31091154, item_num = 1, item_quality = 1},--9  宝藏自选箱B
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 90, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 90, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31090567, item_num = 2, item_quality = 2},--1  圣诞时装-衣服+武器 （皇家转盘10）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31091153, item_num = 1, item_quality = 1},--6  宝藏自选箱A
			{item_id = 31020004, item_num = 180, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 30, item_quality = 0},--8  强化结晶
			{item_id = 31091154, item_num = 1, item_quality = 1},--9  宝藏自选箱B
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 90, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 90, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31090593, item_num = 2, item_quality = 2},--1  贺岁时装-衣服+武器 （皇家转盘11）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31091153, item_num = 1, item_quality = 1},--6  宝藏自选箱A
			{item_id = 31020004, item_num = 180, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 30, item_quality = 0},--8  强化结晶
			{item_id = 31091154, item_num = 1, item_quality = 1},--9  宝藏自选箱B
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 90, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 90, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31090607, item_num = 2, item_quality = 2},--1  机械时装-紫色-衣服+武器 （皇家转盘12）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31091153, item_num = 1, item_quality = 1},--6  宝藏自选箱A
			{item_id = 31020004, item_num = 180, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 30, item_quality = 0},--8  强化结晶
			{item_id = 31091154, item_num = 1, item_quality = 1},--9  宝藏自选箱B
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 90, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 90, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31051010, item_num = 20, item_quality = 2},--1  精灵10_天贵星__天神碎片  (精灵转盘1-13）
			{item_id = 31051007, item_num = 2, item_quality = 0},--2  精灵07_双鱼座__黄金碎片
			{item_id = 31051008, item_num = 2, item_quality = 0},--3  精灵08_天蝎座__黄金碎片
			{item_id = 31051009, item_num = 2, item_quality = 0},--4  精灵09_处女座__黄金碎片
			{item_id = 31140137, item_num = 2, item_quality = 0},--5  精灵印记
			{item_id = 31051007, item_num = 20, item_quality = 1},--6  精灵07_双鱼座__黄金碎片
			{item_id = 31010107, item_num = 8, item_quality = 0},--7  灵晶100
			{item_id = 31010107, item_num = 10, item_quality = 0},--8  灵晶100
			{item_id = 31090573, item_num = 1, item_quality = 1},--9  红色灵魂随机礼包
			{item_id = 31090577, item_num = 1, item_quality = 0},--10 绿色灵魂随机礼包
			{item_id = 31090576, item_num = 1, item_quality = 0},--11 蓝色灵魂随机礼包
			{item_id = 31090575, item_num = 1, item_quality = 0},--12 紫色灵魂随机礼包
			{item_id = 31090574, item_num = 1, item_quality = 0},--13 橙色灵魂随机礼包
			{item_id = 31000170, item_num = 2, item_quality = 1},--14 觉醒丹
			{item_id = 31010107, item_num = 12, item_quality = 0},--15 灵晶100
			{item_id = 31010107, item_num = 15, item_quality = 0},--16 灵晶100
		},
		{
			{item_id = 31051011, item_num = 20, item_quality = 2},--1  精灵11_天猛星__天神碎片   (精灵转盘2-14）
			{item_id = 31051007, item_num = 2, item_quality = 0},--2  精灵07_双鱼座__黄金碎片
			{item_id = 31051008, item_num = 2, item_quality = 0},--3  精灵08_天蝎座__黄金碎片
			{item_id = 31051009, item_num = 2, item_quality = 0},--4  精灵09_处女座__黄金碎片
			{item_id = 31140137, item_num = 2, item_quality = 0},--5  精灵印记
			{item_id = 31051008, item_num = 20, item_quality = 1},--6  精灵08_天蝎座__黄金碎片
			{item_id = 31010107, item_num = 10, item_quality = 0},--7  灵晶100
			{item_id = 31010107, item_num = 12, item_quality = 0},--8  灵晶100
			{item_id = 31090573, item_num = 1, item_quality = 1},--9  红色灵魂随机礼包
			{item_id = 31090577, item_num = 1, item_quality = 0},--10 绿色灵魂随机礼包
			{item_id = 31090576, item_num = 1, item_quality = 0},--11 蓝色灵魂随机礼包
			{item_id = 31090575, item_num = 1, item_quality = 0},--12 紫色灵魂随机礼包
			{item_id = 31090574, item_num = 1, item_quality = 0},--13 橙色灵魂随机礼包
			{item_id = 31000170, item_num = 2, item_quality = 1},--14 觉醒丹
			{item_id = 31010107, item_num = 15, item_quality = 0},--15 灵晶100
			{item_id = 31010107, item_num = 18, item_quality = 0},--16 灵晶100
		},
		{
			{item_id = 31051012, item_num = 20, item_quality = 2},--1  精灵12_天雄星__天神碎片   (精灵转盘3-15）
			{item_id = 31051007, item_num = 2, item_quality = 0},--2  精灵07_双鱼座__黄金碎片
			{item_id = 31051008, item_num = 2, item_quality = 0},--3  精灵08_天蝎座__黄金碎片
			{item_id = 31051009, item_num = 2, item_quality = 0},--4  精灵09_处女座__黄金碎片
			{item_id = 31140137, item_num = 2, item_quality = 0},--5  精灵印记
			{item_id = 31051009, item_num = 20, item_quality = 1},--6  精灵09_处女座__黄金碎片
			{item_id = 31010107, item_num = 12, item_quality = 0},--7  灵晶100
			{item_id = 31010107, item_num = 15, item_quality = 0},--8  灵晶100
			{item_id = 31090573, item_num = 1, item_quality = 1},--9  红色灵魂随机礼包
			{item_id = 31090577, item_num = 1, item_quality = 0},--10 绿色灵魂随机礼包
			{item_id = 31090576, item_num = 1, item_quality = 0},--11 蓝色灵魂随机礼包
			{item_id = 31090575, item_num = 1, item_quality = 0},--12 紫色灵魂随机礼包
			{item_id = 31090574, item_num = 1, item_quality = 0},--13 橙色灵魂随机礼包
			{item_id = 31000170, item_num = 2, item_quality = 1},--14 觉醒丹
			{item_id = 31010107, item_num = 18, item_quality = 0},--15 灵晶100
			{item_id = 31010107, item_num = 20, item_quality = 0},--16 灵晶100
		},
		{
			{item_id = 31051015, item_num = 20, item_quality = 2},--1  精灵15_白娘子__天神碎片   (精灵转盘4-16）
			{item_id = 31051007, item_num = 2, item_quality = 0},--2  精灵07_双鱼座__黄金碎片
			{item_id = 31051008, item_num = 2, item_quality = 0},--3  精灵08_天蝎座__黄金碎片
			{item_id = 31051009, item_num = 2, item_quality = 0},--4  精灵09_处女座__黄金碎片
			{item_id = 31140137, item_num = 2, item_quality = 0},--5  精灵印记
			{item_id = 31090135, item_num = 1, item_quality = 1},--6  橙色精灵任选礼包
			{item_id = 31010107, item_num = 15, item_quality = 0},--7  灵晶100
			{item_id = 31010107, item_num = 18, item_quality = 0},--8  灵晶100
			{item_id = 31090573, item_num = 1, item_quality = 1},--9  红色灵魂随机礼包
			{item_id = 31090577, item_num = 1, item_quality = 0},--10 绿色灵魂随机礼包
			{item_id = 31090576, item_num = 1, item_quality = 0},--11 蓝色灵魂随机礼包
			{item_id = 31090575, item_num = 1, item_quality = 0},--12 紫色灵魂随机礼包
			{item_id = 31090574, item_num = 1, item_quality = 0},--13 橙色灵魂随机礼包
			{item_id = 31000170, item_num = 2, item_quality = 1},--14 觉醒丹
			{item_id = 31010107, item_num = 20, item_quality = 0},--15 灵晶100
			{item_id = 31010107, item_num = 22, item_quality = 0},--16 灵晶100
		},
		{
			{item_id = 31090505, item_num = 1, item_quality = 2},--1  红色精灵任选礼包          (精灵转盘5-17）
			{item_id = 31051007, item_num = 2, item_quality = 0},--2  精灵07_双鱼座__黄金碎片
			{item_id = 31051008, item_num = 2, item_quality = 0},--3  精灵08_天蝎座__黄金碎片
			{item_id = 31051009, item_num = 2, item_quality = 0},--4  精灵09_处女座__黄金碎片
			{item_id = 31140137, item_num = 2, item_quality = 0},--5  精灵印记
			{item_id = 31090135, item_num = 1, item_quality = 1},--6  橙色精灵任选礼包
			{item_id = 31010107, item_num = 18, item_quality = 0},--7  灵晶100
			{item_id = 31010107, item_num = 20, item_quality = 0},--8  灵晶100
			{item_id = 31090573, item_num = 1, item_quality = 1},--9  红色灵魂随机礼包
			{item_id = 31090577, item_num = 1, item_quality = 0},--10 绿色灵魂随机礼包
			{item_id = 31090576, item_num = 1, item_quality = 0},--11 蓝色灵魂随机礼包
			{item_id = 31090575, item_num = 1, item_quality = 0},--12 紫色灵魂随机礼包
			{item_id = 31090574, item_num = 1, item_quality = 0},--13 橙色灵魂随机礼包
			{item_id = 31000170, item_num = 2, item_quality = 1},--14 觉醒丹
			{item_id = 31010107, item_num = 22, item_quality = 0},--15 灵晶100
			{item_id = 31010107, item_num = 25, item_quality = 0},--16 灵晶100
		},
		{
			{item_id = 31091060, item_num = 1, item_quality = 2},--1  原初破坏符文Lv60        (符文转盘1-18）
			{item_id = 31060051, item_num = 1, item_quality = 0},--2  橙色最小攻击
			{item_id = 31060052, item_num = 1, item_quality = 0},--3  橙色最大攻击
			{item_id = 31060053, item_num = 1, item_quality = 0},--4  橙色防御
			{item_id = 31060050, item_num = 1, item_quality = 0},--5  橙色生命上限
			{item_id = 31061005, item_num = 1, item_quality = 1},--6  符文精华-红
			{item_id = 31091056, item_num = 1, item_quality = 0},--7  符文福袋-紫
			{item_id = 31091057, item_num = 1, item_quality = 0},--8  符文福袋-橙
			{item_id = 31091058, item_num = 1, item_quality = 1},--9  符文福袋-红
			{item_id = 31060035, item_num = 1, item_quality = 0},--10 紫色最小攻击
			{item_id = 31060036, item_num = 1, item_quality = 0},--11 紫色最大攻击
			{item_id = 31060037, item_num = 1, item_quality = 0},--12 紫色防御
			{item_id = 31060034, item_num = 1, item_quality = 0},--13 紫色生命上限
			{item_id = 31061005, item_num = 1, item_quality = 1},--14 符文精华-红
			{item_id = 31061004, item_num = 2, item_quality = 0},--15 符文精华-橙
			{item_id = 31061003, item_num = 4, item_quality = 0},--16 符文精华-紫
		},
		{
			{item_id = 31061026, item_num = 2, item_quality = 2},--1  融具       (符文转盘2-19）
			{item_id = 31060051, item_num = 1, item_quality = 0},--2  橙色最小攻击
			{item_id = 31060052, item_num = 1, item_quality = 0},--3  橙色最大攻击
			{item_id = 31060053, item_num = 1, item_quality = 0},--4  橙色防御
			{item_id = 31060050, item_num = 1, item_quality = 0},--5  橙色生命上限
			{item_id = 31061033, item_num = 1, item_quality = 1},--6  橙色融具
			{item_id = 31091056, item_num = 1, item_quality = 0},--7  符文福袋-紫
			{item_id = 31091057, item_num = 1, item_quality = 0},--8  符文福袋-橙
			{item_id = 31091058, item_num = 1, item_quality = 1},--9  符文福袋-红
			{item_id = 31060035, item_num = 1, item_quality = 0},--10 紫色最小攻击
			{item_id = 31060036, item_num = 1, item_quality = 0},--11 紫色最大攻击
			{item_id = 31060037, item_num = 1, item_quality = 0},--12 紫色防御
			{item_id = 31060034, item_num = 1, item_quality = 0},--13 紫色生命上限
			{item_id = 31061027, item_num = 1, item_quality = 1},--14 聚尘
			{item_id = 31061005, item_num = 1, item_quality = 0},--15 符文精华-红
			{item_id = 31061004, item_num = 2, item_quality = 0},--16 符文精华-橙
		},
		{
			{item_id = 31061026, item_num = 2, item_quality = 2},--1  融具       (符文转盘3-20）
			{item_id = 31060051, item_num = 1, item_quality = 0},--2  橙色最小攻击
			{item_id = 31060052, item_num = 1, item_quality = 0},--3  橙色最大攻击
			{item_id = 31061008, item_num = 1, item_quality = 0},--4  倍化精密符文
			{item_id = 31061009, item_num = 1, item_quality = 0},--5  倍化愤怒符文
			{item_id = 31061028, item_num = 1, item_quality = 1},--6  克隆
			{item_id = 31091056, item_num = 1, item_quality = 0},--7  符文福袋-紫
			{item_id = 31091057, item_num = 1, item_quality = 0},--8  符文福袋-橙
			{item_id = 31091058, item_num = 1, item_quality = 1},--9  符文福袋-红
			{item_id = 31060035, item_num = 1, item_quality = 0},--10 紫色最小攻击
			{item_id = 31060036, item_num = 1, item_quality = 0},--11 紫色最大攻击
			{item_id = 31060037, item_num = 1, item_quality = 0},--12 紫色防御
			{item_id = 31060034, item_num = 1, item_quality = 0},--13 紫色生命上限
			{item_id = 31061027, item_num = 1, item_quality = 1},--14 聚尘
			{item_id = 31061005, item_num = 1, item_quality = 0},--15 符文精华-红
			{item_id = 31061004, item_num = 3, item_quality = 0},--16 符文精华-橙
		},
		{
			{item_id = 31061026, item_num = 2, item_quality = 2},--1  融具       (符文转盘4-21）
			{item_id = 31061006, item_num = 1, item_quality = 0},--2  倍化坚毅符文
			{item_id = 31061007, item_num = 1, item_quality = 0},--3  倍化能量符文
			{item_id = 31060053, item_num = 1, item_quality = 0},--4  橙色防御
			{item_id = 31060050, item_num = 1, item_quality = 0},--5  橙色生命上限
			{item_id = 31061029, item_num = 1, item_quality = 1},--6  裂变
			{item_id = 31091056, item_num = 1, item_quality = 0},--7  符文福袋-紫
			{item_id = 31091057, item_num = 1, item_quality = 0},--8  符文福袋-橙
			{item_id = 31091058, item_num = 1, item_quality = 1},--9  符文福袋-红
			{item_id = 31060035, item_num = 1, item_quality = 0},--10 紫色最小攻击
			{item_id = 31060036, item_num = 1, item_quality = 0},--11 紫色最大攻击
			{item_id = 31060037, item_num = 1, item_quality = 0},--12 紫色防御
			{item_id = 31060034, item_num = 1, item_quality = 0},--13 紫色生命上限
			{item_id = 31061027, item_num = 1, item_quality = 1},--14 聚尘
			{item_id = 31061005, item_num = 1, item_quality = 0},--15 符文精华-红
			{item_id = 31061004, item_num = 3, item_quality = 0},--16 符文精华-橙
		},
		{
			{item_id = 31061026, item_num = 2, item_quality = 2},--1  融具       (符文转盘5-22）
			{item_id = 31061006, item_num = 1, item_quality = 0},--2  倍化坚毅符文
			{item_id = 31061007, item_num = 1, item_quality = 0},--3  倍化能量符文
			{item_id = 31061008, item_num = 1, item_quality = 0},--4  倍化精密符文
			{item_id = 31061009, item_num = 1, item_quality = 0},--5  倍化愤怒符文
			{item_id = 31061030, item_num = 1, item_quality = 1},--6  激活
			{item_id = 31061005, item_num = 2, item_quality = 0},--7  符文精华-红
			{item_id = 31091057, item_num = 1, item_quality = 0},--8  符文福袋-橙
			{item_id = 31091058, item_num = 1, item_quality = 1},--9  符文福袋-红
			{item_id = 31061003, item_num = 5, item_quality = 0},--10 符文精华-紫
			{item_id = 31061004, item_num = 3, item_quality = 0},--11 符文精华-橙
			{item_id = 31061003, item_num = 10, item_quality = 0},--12 符文精华-紫
			{item_id = 31061004, item_num = 5, item_quality = 0},--13 符文精华-橙
			{item_id = 31061027, item_num = 1, item_quality = 1},--14 聚尘
			{item_id = 31061005, item_num = 1, item_quality = 0},--15 符文精华-红
			{item_id = 31061004, item_num = 3, item_quality = 0},--16 符文精华-橙
		},		
		{
			{item_id = 31061026, item_num = 2, item_quality = 2},--1  融具       (符文转盘6-23）
			{item_id = 31061006, item_num = 1, item_quality = 0},--2  倍化坚毅符文
			{item_id = 31061007, item_num = 1, item_quality = 0},--3  倍化能量符文
			{item_id = 31061008, item_num = 1, item_quality = 0},--4  倍化精密符文
			{item_id = 31061009, item_num = 1, item_quality = 0},--5  倍化愤怒符文
			{item_id = 31061032, item_num = 1, item_quality = 1},--6  裂变+激活任选礼包
			{item_id = 31061005, item_num = 2, item_quality = 0},--7  符文精华-红
			{item_id = 31091057, item_num = 1, item_quality = 0},--8  符文福袋-橙
			{item_id = 31091058, item_num = 1, item_quality = 1},--9  符文福袋-红
			{item_id = 31061003, item_num = 5, item_quality = 0},--10 符文精华-紫
			{item_id = 31061004, item_num = 3, item_quality = 0},--11 符文精华-橙
			{item_id = 31061003, item_num = 10, item_quality = 0},--12 符文精华-紫
			{item_id = 31061004, item_num = 5, item_quality = 0},--13 符文精华-橙
			{item_id = 31061031, item_num = 1, item_quality = 1},--14 聚尘+克隆任选礼包
			{item_id = 31061005, item_num = 1, item_quality = 0},--15 符文精华-红
			{item_id = 31061004, item_num = 3, item_quality = 0},--16 符文精华-橙
		},
		{
			{item_id = 31090606, item_num = 2, item_quality = 2},--1  机械时装-蓝色-衣服+武器  （皇家转盘13-24）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31091153, item_num = 1, item_quality = 1},--6  宝藏自选箱A
			{item_id = 31020004, item_num = 200, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 33, item_quality = 0},--8  强化结晶
			{item_id = 31091154, item_num = 1, item_quality = 1},--9  宝藏自选箱B
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 100, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 100, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31090567, item_num = 2, item_quality = 2},--1  圣诞时装-衣服+武器 （皇家转盘14-25）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31091153, item_num = 1, item_quality = 1},--6  宝藏自选箱A
			{item_id = 31020004, item_num = 200, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 33, item_quality = 0},--8  强化结晶
			{item_id = 31091154, item_num = 1, item_quality = 1},--9  宝藏自选箱B
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 100, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 100, item_quality = 0},--16  10000魔晶提取卡
		},	
		{
			{item_id = 31090593, item_num = 2, item_quality = 2},--1  贺岁时装黑-衣服+武器 （皇家转盘15-26）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31091153, item_num = 1, item_quality = 1},--6  宝藏自选箱A
			{item_id = 31020004, item_num = 200, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 33, item_quality = 0},--8  强化结晶
			{item_id = 31091154, item_num = 1, item_quality = 1},--9  宝藏自选箱B
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 100, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 100, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31090607, item_num = 2, item_quality = 2},--1  机械时装-紫色-衣服+武器 （皇家转盘16-27）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31091153, item_num = 1, item_quality = 1},--6  宝藏自选箱A
			{item_id = 31020004, item_num = 200, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 33, item_quality = 0},--8  强化结晶
			{item_id = 31091154, item_num = 1, item_quality = 1},--9  宝藏自选箱B
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 100, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 100, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31090610, item_num = 2, item_quality = 2},--1  英伦时装-衣服+武器  （皇家转盘17-28）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31091153, item_num = 1, item_quality = 1},--6  宝藏自选箱A
			{item_id = 31020004, item_num = 200, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 33, item_quality = 0},--8  强化结晶
			{item_id = 31091154, item_num = 1, item_quality = 1},--9  宝藏自选箱B
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 100, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 100, item_quality = 0},--16  10000魔晶提取卡
		},	
		{
			{item_id = 31090643, item_num = 2, item_quality = 2},--1  幻影之森时装-衣服+武器 （皇家转盘18-29）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31091153, item_num = 1, item_quality = 1},--6  宝藏自选箱A
			{item_id = 31020004, item_num = 200, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 33, item_quality = 0},--8  强化结晶
			{item_id = 31091154, item_num = 1, item_quality = 1},--9  宝藏自选箱B
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 100, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 100, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31090644, item_num = 2, item_quality = 2},--1  璀璨苍蓝时装-衣服+武器 （皇家转盘19-30）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31091153, item_num = 1, item_quality = 1},--6  宝藏自选箱A
			{item_id = 31020004, item_num = 200, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 33, item_quality = 0},--8  强化结晶
			{item_id = 31091154, item_num = 1, item_quality = 1},--9  宝藏自选箱B
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 100, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 100, item_quality = 0},--16  10000魔晶提取卡
		},
		{
			{item_id = 31041109, item_num = 2, item_quality = 2},--1  极品时装任选箱  （皇家转盘20-31）
			{item_id = 31000105, item_num = 32, item_quality = 0},--2  神兽升级丹
			{item_id = 31000106, item_num = 16, item_quality = 0},--3  神兽进阶丹
			{item_id = 31090543, item_num = 3, item_quality = 0},--4  紫色兽魂礼包
			{item_id = 31090544, item_num = 3, item_quality = 0},--5  橙色兽魂礼包
			{item_id = 31091153, item_num = 1, item_quality = 1},--6  宝藏自选箱A
			{item_id = 31020004, item_num = 200, item_quality = 0},--7  强化石
			{item_id = 31020003, item_num = 33, item_quality = 0},--8  强化结晶
			{item_id = 31091154, item_num = 1, item_quality = 1},--9  宝藏自选箱B
			{item_id = 31000099, item_num = 32, item_quality = 0},--10  洛克神羽
			{item_id = 31090547, item_num = 3, item_quality = 0},--11  紫色精魂礼包
			{item_id = 31090548, item_num = 3, item_quality = 0},--12  橙色精魂礼包
			{item_id = 31000063, item_num = 2, item_quality = 0},--13  亘古神佑卷轴
			{item_id = 31027000, item_num = 1, item_quality = 1},--14  4阶橙色通用魔法书
			{item_id = 31010060, item_num = 100, item_quality = 0},--15  100火种点数卡
			{item_id = 31010054, item_num = 100, item_quality = 0},--16  10000魔晶提取卡
		},	
		{
			{item_id = 31051016, item_num = 20, item_quality = 2},--1  精灵16_绿野仙灵__天神碎片   (精灵转盘6-32）
			{item_id = 31051007, item_num = 4, item_quality = 0},--2  精灵07_双鱼座__黄金碎片
			{item_id = 31051008, item_num = 4, item_quality = 0},--3  精灵08_天蝎座__黄金碎片
			{item_id = 31051009, item_num = 4, item_quality = 0},--4  精灵09_处女座__黄金碎片
			{item_id = 31140137, item_num = 4, item_quality = 0},--5  精灵印记
			{item_id = 31090135, item_num = 1, item_quality = 1},--6  橙色精灵任选礼包
			{item_id = 31010107, item_num = 18, item_quality = 0},--7  灵晶100
			{item_id = 31010107, item_num = 20, item_quality = 0},--8  灵晶100
			{item_id = 31090573, item_num = 1, item_quality = 1},--9  红色灵魂随机礼包
			{item_id = 31090577, item_num = 2, item_quality = 0},--10 绿色灵魂随机礼包
			{item_id = 31090576, item_num = 2, item_quality = 0},--11 蓝色灵魂随机礼包
			{item_id = 31090575, item_num = 1, item_quality = 0},--12 紫色灵魂随机礼包
			{item_id = 31090574, item_num = 1, item_quality = 0},--13 橙色灵魂随机礼包
			{item_id = 31000170, item_num = 3, item_quality = 1},--14 觉醒丹
			{item_id = 31010107, item_num = 22, item_quality = 0},--15 灵晶100
			{item_id = 31010107, item_num = 25, item_quality = 0},--16 灵晶100
		},
		{
			{item_id = 31051019, item_num = 20, item_quality = 2},--1  精灵19_超凰__天神碎片   (精灵转盘7-33）
			{item_id = 31051007, item_num = 4, item_quality = 0},--2  精灵07_双鱼座__黄金碎片
			{item_id = 31051008, item_num = 4, item_quality = 0},--3  精灵08_天蝎座__黄金碎片
			{item_id = 31051009, item_num = 4, item_quality = 0},--4  精灵09_处女座__黄金碎片
			{item_id = 31140137, item_num = 4, item_quality = 0},--5  精灵印记
			{item_id = 31090135, item_num = 1, item_quality = 1},--6  橙色精灵任选礼包
			{item_id = 31010107, item_num = 18, item_quality = 0},--7  灵晶100
			{item_id = 31010107, item_num = 20, item_quality = 0},--8  灵晶100
			{item_id = 31090573, item_num = 1, item_quality = 1},--9  红色灵魂随机礼包
			{item_id = 31090577, item_num = 2, item_quality = 0},--10 绿色灵魂随机礼包
			{item_id = 31090576, item_num = 2, item_quality = 0},--11 蓝色灵魂随机礼包
			{item_id = 31090575, item_num = 2, item_quality = 0},--12 紫色灵魂随机礼包
			{item_id = 31090574, item_num = 1, item_quality = 0},--13 橙色灵魂随机礼包
			{item_id = 31000170, item_num = 3, item_quality = 1},--14 觉醒丹
			{item_id = 31010107, item_num = 22, item_quality = 0},--15 灵晶100
			{item_id = 31010107, item_num = 25, item_quality = 0},--16 灵晶100
		},
		{
			{item_id = 31090646, item_num = 1, item_quality = 2},--1  6种红色精灵任选礼包          (精灵转盘8-34）
			{item_id = 31051007, item_num = 4, item_quality = 0},--2  精灵07_双鱼座__黄金碎片
			{item_id = 31051008, item_num = 4, item_quality = 0},--3  精灵08_天蝎座__黄金碎片
			{item_id = 31051009, item_num = 4, item_quality = 0},--4  精灵09_处女座__黄金碎片
			{item_id = 31140137, item_num = 4, item_quality = 0},--5  精灵印记
			{item_id = 31090135, item_num = 1, item_quality = 1},--6  橙色精灵任选礼包
			{item_id = 31010107, item_num = 18, item_quality = 0},--7  灵晶100
			{item_id = 31010107, item_num = 20, item_quality = 0},--8  灵晶100
			{item_id = 31090573, item_num = 2, item_quality = 1},--9  红色灵魂随机礼包
			{item_id = 31090577, item_num = 2, item_quality = 0},--10 绿色灵魂随机礼包
			{item_id = 31090576, item_num = 2, item_quality = 0},--11 蓝色灵魂随机礼包
			{item_id = 31090575, item_num = 2, item_quality = 0},--12 紫色灵魂随机礼包
			{item_id = 31090574, item_num = 2, item_quality = 0},--13 橙色灵魂随机礼包
			{item_id = 31000170, item_num = 4, item_quality = 1},--14 觉醒丹
			{item_id = 31010107, item_num = 22, item_quality = 0},--15 灵晶100
			{item_id = 31010107, item_num = 25, item_quality = 0},--16 灵晶100
		},
		{
			{item_id = 31061034, item_num = 2, item_quality = 2},--1  粉色融具       (符文转盘9-35）
			{item_id = 31061006, item_num = 1, item_quality = 0},--2  倍化坚毅符文
			{item_id = 31061007, item_num = 1, item_quality = 0},--3  倍化能量符文
			{item_id = 31061008, item_num = 1, item_quality = 0},--4  倍化精密符文
			{item_id = 31061009, item_num = 1, item_quality = 0},--5  倍化愤怒符文
			{item_id = 31061026, item_num = 1, item_quality = 1},--6  原初融具
			{item_id = 31061005, item_num = 2, item_quality = 0},--7  符文精华-红
			{item_id = 31091057, item_num = 1, item_quality = 0},--8  符文福袋-橙
			{item_id = 31091058, item_num = 1, item_quality = 1},--9  符文福袋-红
			{item_id = 31061003, item_num = 5, item_quality = 0},--10 符文精华-紫
			{item_id = 31061004, item_num = 3, item_quality = 0},--11 符文精华-橙
			{item_id = 31061003, item_num = 10, item_quality = 0},--12 符文精华-紫
			{item_id = 31061004, item_num = 5, item_quality = 0},--13 符文精华-橙
			{item_id = 31061031, item_num = 1, item_quality = 1},--14 聚尘+克隆任选礼包
			{item_id = 31061005, item_num = 1, item_quality = 0},--15 符文精华-红
			{item_id = 31061004, item_num = 3, item_quality = 0},--16 符文精华-橙
		},
		{
			{item_id = 31142101, item_num = 1, item_quality = 2},--1  星尘符文任选      (符文转盘9-36）
			{item_id = 31061006, item_num = 1, item_quality = 0},--2  倍化坚毅符文
			{item_id = 31061007, item_num = 1, item_quality = 0},--3  倍化能量符文
			{item_id = 31061008, item_num = 1, item_quality = 0},--4  倍化精密符文
			{item_id = 31061009, item_num = 1, item_quality = 0},--5  倍化愤怒符文
			{item_id = 31061032, item_num = 1, item_quality = 1},--6  裂变+激活任选礼包
			{item_id = 31061005, item_num = 2, item_quality = 0},--7  符文精华-红
			{item_id = 31091057, item_num = 1, item_quality = 0},--8  符文福袋-橙
			{item_id = 31091058, item_num = 1, item_quality = 1},--9  符文福袋-红
			{item_id = 31061003, item_num = 5, item_quality = 0},--10 符文精华-紫
			{item_id = 31061004, item_num = 3, item_quality = 0},--11 符文精华-橙
			{item_id = 31061003, item_num = 10, item_quality = 0},--12 符文精华-紫
			{item_id = 31061004, item_num = 5, item_quality = 0},--13 符文精华-橙
			{item_id = 31061026, item_num = 1, item_quality = 1},--14 原初融具
			{item_id = 31061005, item_num = 1, item_quality = 0},--15 符文精华-红
			{item_id = 31061004, item_num = 3, item_quality = 0},--16 符文精华-橙
		},
	},
	time_limit_activity_treasure_percent =
	{
		{																--(皇家转盘1）
			prize_1 = 						
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
				{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																			--(皇家转盘2）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																			--(皇家转盘3）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																--(皇家转盘4）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{															--(皇家转盘5）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																			--(皇家转盘6）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																			--(皇家转盘7）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																		--(皇家转盘8）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																	--(皇家转盘9）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																	--(皇家转盘10）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																	--(皇家转盘11）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																--(皇家转盘12）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																--(精灵转盘1-13）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 100},
			cost_item = {item_id = 31000171, item_num = 1},
			add_score = 6,
			add_luck = 1,
			add_item = {item_id = 31000172, item_num = 1},
			first  ={
						{prize_index = 10, prize_percent = 2500},
						{prize_index = 11, prize_percent = 2500},
						{prize_index = 12, prize_percent = 2500},
						{prize_index = 13, prize_percent = 2500}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 800},
								{prize_index = 3, prize_percent = 700},
								{prize_index = 4, prize_percent = 600},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 6, prize_percent = 250},
								{prize_index = 7, prize_percent = 1600},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 9, prize_percent = 150},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 14, prize_percent = 600},
								{prize_index = 15, prize_percent = 1200},
								{prize_index = 16, prize_percent = 1000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 800},
			cost_item = {item_id = 31000171, item_num = 8},
			add_score = 60,
			add_luck = 10,
			add_item = {item_id = 31000172, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 14, prize_percent = 10000}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 2500},
									{prize_index = 9, prize_percent = 1500},
									{prize_index = 14, prize_percent = 6000}
								},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 7, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 7, prize_percent = 10000}
								}
			}
			},
		},
		{																--(精灵转盘2-14）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 100},
			cost_item = {item_id = 31000171, item_num = 1},
			add_score = 6,
			add_luck = 1,
			add_item = {item_id = 31000172, item_num = 1},
			first  ={
						{prize_index = 10, prize_percent = 2500},
						{prize_index = 11, prize_percent = 2500},
						{prize_index = 12, prize_percent = 2500},
						{prize_index = 13, prize_percent = 2500}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 800},
								{prize_index = 3, prize_percent = 700},
								{prize_index = 4, prize_percent = 600},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 6, prize_percent = 250},
								{prize_index = 7, prize_percent = 1600},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 9, prize_percent = 150},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 14, prize_percent = 600},
								{prize_index = 15, prize_percent = 1200},
								{prize_index = 16, prize_percent = 1000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 800},
			cost_item = {item_id = 31000171, item_num = 8},
			add_score = 60,
			add_luck = 10,
			add_item = {item_id = 31000172, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 14, prize_percent = 10000}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 2500},
									{prize_index = 9, prize_percent = 1500},
									{prize_index = 14, prize_percent = 6000}
								},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 7, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 7, prize_percent = 10000}
								}
			}
			},
		},
		{																--(精灵转盘3-15）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 100},
			cost_item = {item_id = 31000171, item_num = 1},
			add_score = 6,
			add_luck = 1,
			add_item = {item_id = 31000172, item_num = 1},
			first  ={
						{prize_index = 10, prize_percent = 2500},
						{prize_index = 11, prize_percent = 2500},
						{prize_index = 12, prize_percent = 2500},
						{prize_index = 13, prize_percent = 2500}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 800},
								{prize_index = 3, prize_percent = 700},
								{prize_index = 4, prize_percent = 600},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 6, prize_percent = 250},
								{prize_index = 7, prize_percent = 1600},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 9, prize_percent = 150},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 14, prize_percent = 600},
								{prize_index = 15, prize_percent = 1200},
								{prize_index = 16, prize_percent = 1000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 800},
			cost_item = {item_id = 31000171, item_num = 8},
			add_score = 60,
			add_luck = 10,
			add_item = {item_id = 31000172, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 14, prize_percent = 10000}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 2500},
									{prize_index = 9, prize_percent = 1500},
									{prize_index = 14, prize_percent = 6000}
								},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 7, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 7, prize_percent = 10000}
								}
			}
			},
		},
		{																--(精灵转盘4-16）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 100},
			cost_item = {item_id = 31000171, item_num = 1},
			add_score = 6,
			add_luck = 1,
			add_item = {item_id = 31000172, item_num = 1},
			first  ={
						{prize_index = 10, prize_percent = 2500},
						{prize_index = 11, prize_percent = 2500},
						{prize_index = 12, prize_percent = 2500},
						{prize_index = 13, prize_percent = 2500}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 800},
								{prize_index = 3, prize_percent = 700},
								{prize_index = 4, prize_percent = 600},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 6, prize_percent = 250},
								{prize_index = 7, prize_percent = 1600},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 9, prize_percent = 150},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 14, prize_percent = 600},
								{prize_index = 15, prize_percent = 1200},
								{prize_index = 16, prize_percent = 1000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 800},
			cost_item = {item_id = 31000171, item_num = 8},
			add_score = 60,
			add_luck = 10,
			add_item = {item_id = 31000172, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 14, prize_percent = 10000}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 2500},
									{prize_index = 9, prize_percent = 1500},
									{prize_index = 14, prize_percent = 6000}
								},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 7, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 7, prize_percent = 10000}
								}
			}
			},
		},
		{																--(精灵转盘5-17）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 100},
			cost_item = {item_id = 31000171, item_num = 1},
			add_score = 6,
			add_luck = 1,
			add_item = {item_id = 31000172, item_num = 1},
			first  ={
						{prize_index = 10, prize_percent = 2500},
						{prize_index = 11, prize_percent = 2500},
						{prize_index = 12, prize_percent = 2500},
						{prize_index = 13, prize_percent = 2500}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 800},
								{prize_index = 3, prize_percent = 700},
								{prize_index = 4, prize_percent = 600},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 6, prize_percent = 250},
								{prize_index = 7, prize_percent = 1600},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 9, prize_percent = 150},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 14, prize_percent = 600},
								{prize_index = 15, prize_percent = 1200},
								{prize_index = 16, prize_percent = 1000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 800},
			cost_item = {item_id = 31000171, item_num = 8},
			add_score = 60,
			add_luck = 10,
			add_item = {item_id = 31000172, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 14, prize_percent = 10000}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 2500},
									{prize_index = 9, prize_percent = 1500},
									{prize_index = 14, prize_percent = 6000}
								},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 7, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 7, prize_percent = 10000}
								}
			}
			},
		},
		{																--(符文转盘1-18）
		prize_1 =
		{
			cost_money = {money_type = 3, money_value = 150},
			cost_item = {item_id = 31000163, item_num = 1},
			add_score = 9,
			add_luck = 1,
			add_item = {item_id = 31090045, item_num = 1},
			first  ={
						{prize_index = 11, prize_percent = 10000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 108},
								{prize_index = 3, prize_percent = 108},
								{prize_index = 4, prize_percent = 108},
								{prize_index = 5, prize_percent = 108},
								{prize_index = 6, prize_percent = 150},
								{prize_index = 7, prize_percent = 3150},
								{prize_index = 8, prize_percent = 1080},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 315},
								{prize_index = 11, prize_percent = 315},
								{prize_index = 12, prize_percent = 315},
								{prize_index = 13, prize_percent = 315},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1350},
								{prize_index = 16, prize_percent = 1728}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 1800},
				{prize_index = 10, prize_percent = 300},
				{prize_index = 11, prize_percent = 300},
				{prize_index = 12, prize_percent = 300},
				{prize_index = 13, prize_percent = 300},
				{prize_index = 15, prize_percent = 2000},
				{prize_index = 16, prize_percent = 5000}
			}
		},
		prize_10 =
		{
			cost_money = {money_type = 3, money_value = 1200},
			cost_item = {item_id = 31000163, item_num = 8},
			add_score = 90,
			add_luck = 10,
			add_item = {item_id = 31090045, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 3500},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 350},
								{prize_index = 12, prize_percent = 350},
								{prize_index = 13, prize_percent = 350},
								{prize_index = 15, prize_percent = 1500},
								{prize_index = 16, prize_percent = 1920}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 3500},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 350},
								{prize_index = 12, prize_percent = 350},
								{prize_index = 13, prize_percent = 350},
								{prize_index = 15, prize_percent = 1500},
								{prize_index = 16, prize_percent = 1920}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 16, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 16, prize_percent = 10000}
								}
			}
			},
		},
		{																--(符文转盘2-19）
		prize_1 =
			{
			cost_money = {money_type = 3, money_value = 150},
			cost_item = {item_id = 31000163, item_num = 1},
			add_score = 9,
			add_luck = 1,
			add_item = {item_id = 31090045, item_num = 1},
			first  ={
						{prize_index = 11, prize_percent = 10000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 108},
								{prize_index = 3, prize_percent = 108},
								{prize_index = 4, prize_percent = 108},
								{prize_index = 5, prize_percent = 108},
								{prize_index = 6, prize_percent = 150},
								{prize_index = 7, prize_percent = 3150},
								{prize_index = 8, prize_percent = 1080},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 315},
								{prize_index = 11, prize_percent = 315},
								{prize_index = 12, prize_percent = 315},
								{prize_index = 13, prize_percent = 315},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1350},
								{prize_index = 16, prize_percent = 1728}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 1800},
				{prize_index = 10, prize_percent = 300},
				{prize_index = 11, prize_percent = 300},
				{prize_index = 12, prize_percent = 300},
				{prize_index = 13, prize_percent = 300},
				{prize_index = 15, prize_percent = 2000},
				{prize_index = 16, prize_percent = 5000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1200},
			cost_item = {item_id = 31000163, item_num = 8},
			add_score = 90,
			add_luck = 10,
			add_item = {item_id = 31090045, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 3500},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 350},
								{prize_index = 12, prize_percent = 350},
								{prize_index = 13, prize_percent = 350},
								{prize_index = 15, prize_percent = 1500},
								{prize_index = 16, prize_percent = 1920}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 3500},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 350},
								{prize_index = 12, prize_percent = 350},
								{prize_index = 13, prize_percent = 350},
								{prize_index = 15, prize_percent = 1500},
								{prize_index = 16, prize_percent = 1920}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 16, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 16, prize_percent = 10000}
								}
			}
			},
		},
		{																--(符文转盘3-20）
		prize_1 =
		{
			cost_money = {money_type = 3, money_value = 150},
			cost_item = {item_id = 31000163, item_num = 1},
			add_score = 9,
			add_luck = 1,
			add_item = {item_id = 31090045, item_num = 1},
			first  ={
						{prize_index = 11, prize_percent = 10000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 108},
								{prize_index = 3, prize_percent = 108},
								{prize_index = 4, prize_percent = 108},
								{prize_index = 5, prize_percent = 108},
								{prize_index = 6, prize_percent = 150},
								{prize_index = 7, prize_percent = 3150},
								{prize_index = 8, prize_percent = 1080},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 315},
								{prize_index = 11, prize_percent = 315},
								{prize_index = 12, prize_percent = 315},
								{prize_index = 13, prize_percent = 315},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1350},
								{prize_index = 16, prize_percent = 1728}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 1800},
				{prize_index = 10, prize_percent = 300},
				{prize_index = 11, prize_percent = 300},
				{prize_index = 12, prize_percent = 300},
				{prize_index = 13, prize_percent = 300},
				{prize_index = 15, prize_percent = 2000},
				{prize_index = 16, prize_percent = 5000}
			}
		},
		prize_10 =
		{
			cost_money = {money_type = 3, money_value = 1200},
			cost_item = {item_id = 31000163, item_num = 8},
			add_score = 90,
			add_luck = 10,
			add_item = {item_id = 31090045, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 3500},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 350},
								{prize_index = 12, prize_percent = 350},
								{prize_index = 13, prize_percent = 350},
								{prize_index = 15, prize_percent = 1500},
								{prize_index = 16, prize_percent = 1920}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 3500},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 350},
								{prize_index = 12, prize_percent = 350},
								{prize_index = 13, prize_percent = 350},
								{prize_index = 15, prize_percent = 1500},
								{prize_index = 16, prize_percent = 1920}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 16, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 16, prize_percent = 10000}
								}
			}
			},
		},
		{																--(符文转盘4-21）
		prize_1 =
		{
			cost_money = {money_type = 3, money_value = 150},
			cost_item = {item_id = 31000163, item_num = 1},
			add_score = 9,
			add_luck = 1,
			add_item = {item_id = 31090045, item_num = 1},
			first  ={
						{prize_index = 11, prize_percent = 10000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 108},
								{prize_index = 3, prize_percent = 108},
								{prize_index = 4, prize_percent = 108},
								{prize_index = 5, prize_percent = 108},
								{prize_index = 6, prize_percent = 150},
								{prize_index = 7, prize_percent = 3150},
								{prize_index = 8, prize_percent = 1080},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 315},
								{prize_index = 11, prize_percent = 315},
								{prize_index = 12, prize_percent = 315},
								{prize_index = 13, prize_percent = 315},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1350},
								{prize_index = 16, prize_percent = 1728}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 1800},
				{prize_index = 10, prize_percent = 300},
				{prize_index = 11, prize_percent = 300},
				{prize_index = 12, prize_percent = 300},
				{prize_index = 13, prize_percent = 300},
				{prize_index = 15, prize_percent = 2000},
				{prize_index = 16, prize_percent = 5000}
			}
		},
		prize_10 =
		{
			cost_money = {money_type = 3, money_value = 1200},
			cost_item = {item_id = 31000163, item_num = 8},
			add_score = 90,
			add_luck = 10,
			add_item = {item_id = 31090045, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 3500},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 350},
								{prize_index = 12, prize_percent = 350},
								{prize_index = 13, prize_percent = 350},
								{prize_index = 15, prize_percent = 1500},
								{prize_index = 16, prize_percent = 1920}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 3500},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 350},
								{prize_index = 12, prize_percent = 350},
								{prize_index = 13, prize_percent = 350},
								{prize_index = 15, prize_percent = 1500},
								{prize_index = 16, prize_percent = 1920}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 16, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 16, prize_percent = 10000}
								}
			}
			},
		},
		{																--(符文转盘5-22）
		prize_1 =
		{
			cost_money = {money_type = 3, money_value = 150},
			cost_item = {item_id = 31000163, item_num = 1},
			add_score = 9,
			add_luck = 1,
			add_item = {item_id = 31090045, item_num = 1},
			first  ={
						{prize_index = 11, prize_percent = 10000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 108},
								{prize_index = 3, prize_percent = 108},
								{prize_index = 4, prize_percent = 108},
								{prize_index = 5, prize_percent = 108},
								{prize_index = 6, prize_percent = 150},
								{prize_index = 7, prize_percent = 540},
								{prize_index = 8, prize_percent = 1080},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 1845},
								{prize_index = 11, prize_percent = 1116},
								{prize_index = 12, prize_percent = 846},
								{prize_index = 13, prize_percent = 846},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 900},
								{prize_index = 16, prize_percent = 1395}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 720},
				{prize_index = 10, prize_percent = 2480},
				{prize_index = 11, prize_percent = 1490},
				{prize_index = 12, prize_percent = 1130},
				{prize_index = 13, prize_percent = 1130},
				{prize_index = 15, prize_percent = 1200},
				{prize_index = 16, prize_percent = 1850}
			}
		},
		prize_10 =
		{
			cost_money = {money_type = 3, money_value = 1200},
			cost_item = {item_id = 31000163, item_num = 8},
			add_score = 90,
			add_luck = 10,
			add_item = {item_id = 31090045, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 600},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 2050},
								{prize_index = 11, prize_percent = 1240},
								{prize_index = 12, prize_percent = 940},
								{prize_index = 13, prize_percent = 940},
								{prize_index = 15, prize_percent = 1000},
								{prize_index = 16, prize_percent = 1550}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 600},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 2050},
								{prize_index = 11, prize_percent = 1240},
								{prize_index = 12, prize_percent = 940},
								{prize_index = 13, prize_percent = 940},
								{prize_index = 15, prize_percent = 1000},
								{prize_index = 16, prize_percent = 1550}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 16, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 16, prize_percent = 10000}
								}
			}
			},
		},
		{																--(符文转盘6-23）
		prize_1 =
		{
			cost_money = {money_type = 3, money_value = 150},
			cost_item = {item_id = 31000163, item_num = 1},
			add_score = 9,
			add_luck = 1,
			add_item = {item_id = 31090045, item_num = 1},
			first  ={
						{prize_index = 11, prize_percent = 10000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 108},
								{prize_index = 3, prize_percent = 108},
								{prize_index = 4, prize_percent = 108},
								{prize_index = 5, prize_percent = 108},
								{prize_index = 6, prize_percent = 150},
								{prize_index = 7, prize_percent = 540},
								{prize_index = 8, prize_percent = 1080},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 1845},
								{prize_index = 11, prize_percent = 1116},
								{prize_index = 12, prize_percent = 846},
								{prize_index = 13, prize_percent = 846},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 900},
								{prize_index = 16, prize_percent = 1395}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 720},
				{prize_index = 10, prize_percent = 2480},
				{prize_index = 11, prize_percent = 1490},
				{prize_index = 12, prize_percent = 1130},
				{prize_index = 13, prize_percent = 1130},
				{prize_index = 15, prize_percent = 1200},
				{prize_index = 16, prize_percent = 1850}
			}
		},
		prize_10 =
		{
			cost_money = {money_type = 3, money_value = 1200},
			cost_item = {item_id = 31000163, item_num = 8},
			add_score = 90,
			add_luck = 10,
			add_item = {item_id = 31090045, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 600},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 2050},
								{prize_index = 11, prize_percent = 1240},
								{prize_index = 12, prize_percent = 940},
								{prize_index = 13, prize_percent = 940},
								{prize_index = 15, prize_percent = 1000},
								{prize_index = 16, prize_percent = 1550}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 600},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 2050},
								{prize_index = 11, prize_percent = 1240},
								{prize_index = 12, prize_percent = 940},
								{prize_index = 13, prize_percent = 940},
								{prize_index = 15, prize_percent = 1000},
								{prize_index = 16, prize_percent = 1550}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 16, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 16, prize_percent = 10000}
								}
			}
			},
		},
		{																	--(皇家转盘13-24）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																	--(皇家转盘14-25）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																	--(皇家转盘15-26）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																--(皇家转盘16-27）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																--(皇家转盘17-28）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																	--(皇家转盘18-29）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																--(皇家转盘19-30）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																--(皇家转盘20-31）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 246},
			cost_item = {item_id = 31000162, item_num = 1},
			add_score = 15,
			add_luck = 1,
			add_item = {item_id = 31090044, item_num = 1},
			first  ={
						{prize_index = 2, prize_percent = 1000},
						{prize_index = 3, prize_percent = 1000},
						{prize_index = 4, prize_percent = 1000},
						{prize_index = 5, prize_percent = 1000},
						{prize_index = 7, prize_percent = 1000},
						{prize_index = 8, prize_percent = 1000},
						{prize_index = 10, prize_percent = 1000},
						{prize_index = 11, prize_percent = 1000},
						{prize_index = 12, prize_percent = 1000},
						{prize_index = 13, prize_percent = 1000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 450},
								{prize_index = 4, prize_percent = 720},
								{prize_index = 5, prize_percent = 450},
								{prize_index = 6, prize_percent = 400},
								{prize_index = 7, prize_percent = 900},
								{prize_index = 8, prize_percent = 900},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 900},
								{prize_index = 11, prize_percent = 720},
								{prize_index = 12, prize_percent = 450},
								{prize_index = 13, prize_percent = 900},
								{prize_index = 14, prize_percent = 250},
								{prize_index = 15, prize_percent = 630},
								{prize_index = 16, prize_percent = 1080}
							},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 15, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 1980},
			cost_item = {item_id = 31000162, item_num = 8},
			add_score = 150,
			add_luck = 10,
			add_item = {item_id = 31090044, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 1000},
								{prize_index = 3, prize_percent = 500},
								{prize_index = 4, prize_percent = 800},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 7, prize_percent = 1000},
								{prize_index = 8, prize_percent = 1000},
								{prize_index = 10, prize_percent = 1000},
								{prize_index = 11, prize_percent = 800},
								{prize_index = 12, prize_percent = 500},
								{prize_index = 13, prize_percent = 1000},
								{prize_index = 15, prize_percent = 700},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 4000},
									{prize_index = 9, prize_percent = 3500},
									{prize_index = 14, prize_percent = 2500}
								},
				best_prize =
				{
					basics = 0,
					score_add = 4,
					min_score_add = 100,
					max_score_add = 242,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 15, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 15, prize_percent = 10000}
									}
				}
			},
		},
		{																--(精灵转盘6-32）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 100},
			cost_item = {item_id = 31000171, item_num = 1},
			add_score = 6,
			add_luck = 1,
			add_item = {item_id = 31000172, item_num = 1},
			first  ={
						{prize_index = 10, prize_percent = 2500},
						{prize_index = 11, prize_percent = 2500},
						{prize_index = 12, prize_percent = 2500},
						{prize_index = 13, prize_percent = 2500}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 800},
								{prize_index = 3, prize_percent = 700},
								{prize_index = 4, prize_percent = 600},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 6, prize_percent = 250},
								{prize_index = 7, prize_percent = 1600},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 9, prize_percent = 150},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 14, prize_percent = 600},
								{prize_index = 15, prize_percent = 1200},
								{prize_index = 16, prize_percent = 1000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 800},
			cost_item = {item_id = 31000171, item_num = 8},
			add_score = 60,
			add_luck = 10,
			add_item = {item_id = 31000172, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 14, prize_percent = 10000}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 2500},
									{prize_index = 9, prize_percent = 1500},
									{prize_index = 14, prize_percent = 6000}
								},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 7, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 7, prize_percent = 10000}
								}
			}
			},
		},
		{																--(精灵转盘7-33）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 100},
			cost_item = {item_id = 31000171, item_num = 1},
			add_score = 6,
			add_luck = 1,
			add_item = {item_id = 31000172, item_num = 1},
			first  ={
						{prize_index = 10, prize_percent = 2500},
						{prize_index = 11, prize_percent = 2500},
						{prize_index = 12, prize_percent = 2500},
						{prize_index = 13, prize_percent = 2500}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 800},
								{prize_index = 3, prize_percent = 700},
								{prize_index = 4, prize_percent = 600},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 6, prize_percent = 250},
								{prize_index = 7, prize_percent = 1600},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 9, prize_percent = 150},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 14, prize_percent = 600},
								{prize_index = 15, prize_percent = 1200},
								{prize_index = 16, prize_percent = 1000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 800},
			cost_item = {item_id = 31000171, item_num = 8},
			add_score = 60,
			add_luck = 10,
			add_item = {item_id = 31000172, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 14, prize_percent = 10000}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 2500},
									{prize_index = 9, prize_percent = 1500},
									{prize_index = 14, prize_percent = 6000}
								},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 7, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 7, prize_percent = 10000}
								}
			}
			},
		},
		{																--(精灵转盘8-34）
			prize_1 =
			{
			cost_money = {money_type = 3, money_value = 100},
			cost_item = {item_id = 31000171, item_num = 1},
			add_score = 6,
			add_luck = 1,
			add_item = {item_id = 31000172, item_num = 1},
			first  ={
						{prize_index = 10, prize_percent = 2500},
						{prize_index = 11, prize_percent = 2500},
						{prize_index = 12, prize_percent = 2500},
						{prize_index = 13, prize_percent = 2500}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 800},
								{prize_index = 3, prize_percent = 700},
								{prize_index = 4, prize_percent = 600},
								{prize_index = 5, prize_percent = 500},
								{prize_index = 6, prize_percent = 250},
								{prize_index = 7, prize_percent = 1600},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 9, prize_percent = 150},
								{prize_index = 10, prize_percent = 350},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 14, prize_percent = 600},
								{prize_index = 15, prize_percent = 1200},
								{prize_index = 16, prize_percent = 1000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 10000}
			}
			},
		prize_10 =
			{
			cost_money = {money_type = 3, money_value = 800},
			cost_item = {item_id = 31000171, item_num = 8},
			add_score = 60,
			add_luck = 10,
			add_item = {item_id = 31000172, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 14, prize_percent = 10000}
								}
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 900},
								{prize_index = 3, prize_percent = 800},
								{prize_index = 4, prize_percent = 700},
								{prize_index = 5, prize_percent = 600},
								{prize_index = 7, prize_percent = 1700},
								{prize_index = 8, prize_percent = 1500},
								{prize_index = 10, prize_percent = 450},
								{prize_index = 11, prize_percent = 300},
								{prize_index = 12, prize_percent = 250},
								{prize_index = 13, prize_percent = 200},
								{prize_index = 15, prize_percent = 1400},
								{prize_index = 16, prize_percent = 1200}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 6, prize_percent = 2500},
									{prize_index = 9, prize_percent = 1500},
									{prize_index = 14, prize_percent = 6000}
								},
				best_prize =
				{
					basics = 0,
					score_add = 3,
					min_score_add = 140,
					max_score_add = 302,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 7, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
									{prize_index = 7, prize_percent = 10000}
								}
			}
			},
		},
		{																--(符文转盘9-35）
		prize_1 =
		{
			cost_money = {money_type = 3, money_value = 150},
			cost_item = {item_id = 31000163, item_num = 1},
			add_score = 9,
			add_luck = 1,
			add_item = {item_id = 31090045, item_num = 1},
			first  ={
						{prize_index = 11, prize_percent = 10000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 108},
								{prize_index = 3, prize_percent = 108},
								{prize_index = 4, prize_percent = 108},
								{prize_index = 5, prize_percent = 108},
								{prize_index = 6, prize_percent = 150},
								{prize_index = 7, prize_percent = 540},
								{prize_index = 8, prize_percent = 1080},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 1845},
								{prize_index = 11, prize_percent = 1116},
								{prize_index = 12, prize_percent = 846},
								{prize_index = 13, prize_percent = 846},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 900},
								{prize_index = 16, prize_percent = 1395}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 720},
				{prize_index = 10, prize_percent = 2480},
				{prize_index = 11, prize_percent = 1490},
				{prize_index = 12, prize_percent = 1130},
				{prize_index = 13, prize_percent = 1130},
				{prize_index = 15, prize_percent = 1200},
				{prize_index = 16, prize_percent = 1850}
			}
		},
		prize_10 =
		{
			cost_money = {money_type = 3, money_value = 1200},
			cost_item = {item_id = 31000163, item_num = 8},
			add_score = 90,
			add_luck = 10,
			add_item = {item_id = 31090045, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 600},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 2050},
								{prize_index = 11, prize_percent = 1240},
								{prize_index = 12, prize_percent = 940},
								{prize_index = 13, prize_percent = 940},
								{prize_index = 15, prize_percent = 1000},
								{prize_index = 16, prize_percent = 1550}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 600},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 2050},
								{prize_index = 11, prize_percent = 1240},
								{prize_index = 12, prize_percent = 940},
								{prize_index = 13, prize_percent = 940},
								{prize_index = 15, prize_percent = 1000},
								{prize_index = 16, prize_percent = 1550}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 16, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 16, prize_percent = 10000}
								}
			}
		    },
		},
		{																--(符文转盘9-36）
		prize_1 =
		{
			cost_money = {money_type = 3, money_value = 150},
			cost_item = {item_id = 31000163, item_num = 1},
			add_score = 9,
			add_luck = 1,
			add_item = {item_id = 31090045, item_num = 1},
			first  ={
						{prize_index = 11, prize_percent = 10000}
					},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 108},
								{prize_index = 3, prize_percent = 108},
								{prize_index = 4, prize_percent = 108},
								{prize_index = 5, prize_percent = 108},
								{prize_index = 6, prize_percent = 150},
								{prize_index = 7, prize_percent = 540},
								{prize_index = 8, prize_percent = 1080},
								{prize_index = 9, prize_percent = 350},
								{prize_index = 10, prize_percent = 1845},
								{prize_index = 11, prize_percent = 1116},
								{prize_index = 12, prize_percent = 846},
								{prize_index = 13, prize_percent = 846},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 900},
								{prize_index = 16, prize_percent = 1395}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				{prize_index = 7, prize_percent = 720},
				{prize_index = 10, prize_percent = 2480},
				{prize_index = 11, prize_percent = 1490},
				{prize_index = 12, prize_percent = 1130},
				{prize_index = 13, prize_percent = 1130},
				{prize_index = 15, prize_percent = 1200},
				{prize_index = 16, prize_percent = 1850}
			}
		},
		prize_10 =
		{
			cost_money = {money_type = 3, money_value = 1200},
			cost_item = {item_id = 31000163, item_num = 8},
			add_score = 90,
			add_luck = 10,
			add_item = {item_id = 31090045, item_num = 10},
			first  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 600},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 2050},
								{prize_index = 11, prize_percent = 1240},
								{prize_index = 12, prize_percent = 940},
								{prize_index = 13, prize_percent = 940},
								{prize_index = 15, prize_percent = 1000},
								{prize_index = 16, prize_percent = 1550}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
			},
			next  =
			{
				nomal_prize = {
								{prize_index = 2, prize_percent = 120},
								{prize_index = 3, prize_percent = 120},
								{prize_index = 4, prize_percent = 120},
								{prize_index = 5, prize_percent = 120},
								{prize_index = 7, prize_percent = 600},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 10, prize_percent = 2050},
								{prize_index = 11, prize_percent = 1240},
								{prize_index = 12, prize_percent = 940},
								{prize_index = 13, prize_percent = 940},
								{prize_index = 15, prize_percent = 1000},
								{prize_index = 16, prize_percent = 1550}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 6, prize_percent = 1500},
								{prize_index = 9, prize_percent = 3500},
								{prize_index = 14, prize_percent = 5000}
							},
				best_prize =
				{
					basics = 0,
					score_add = 2,
					min_score_add = 160,
					max_score_add = 362,
					percent = {
								{prize_index = 1, prize_percent = 10000}
							},
				}
			},
			free =
			{
				nomal_prize = {
								{prize_index = 16, prize_percent = 10000}
							},
				better_percent = 1500,
				better_max = 1,
				better_min = 1,
				better_prize = {
								{prize_index = 16, prize_percent = 10000}
								}
			}
			},
		},
	},
	new_server_treasure_data=
	{
		{
			{item_id = 31102204, item_num = 1, item_quality = 1},--1  橙色兽魂   （新开服活动）
			{item_id = 31020004, item_num = 2, item_quality = 0},--2  强化石
			{item_id = 31020003, item_num = 1, item_quality = 0},--3  强化结晶
			{item_id = 31000086, item_num = 1, item_quality = 0},--4  扫荡卷
			{item_id = 31010004, item_num = 1, item_quality = 0},--5  金币10000
			{item_id = 31000164, item_num = 1, item_quality = 1},--6  4级宝石
			{item_id = 31000028, item_num = 2, item_quality = 0},--7  坐骑升级石
			{item_id = 31000040, item_num = 2, item_quality = 0},--8  洛克之羽
			{item_id = 31020005, item_num = 1, item_quality = 1},--9  神佑卷轴
			{item_id = 31010060, item_num = 1, item_quality = 0},--10  100女神徽记
			{item_id = 31020007, item_num = 1, item_quality = 0},--11  创世结晶
			{item_id = 31000029, item_num = 1, item_quality = 0},--12  坐骑进阶石
			{item_id = 31010054, item_num = 1, item_quality = 0},--13  10000魔晶提取卡
			{item_id = 31130408, item_num = 1, item_quality = 1},--14  升华神符
			{item_id = 31000094, item_num = 1, item_quality = 0},--15  1.5倍经验药
			{item_id = 31000172, item_num = 1, item_quality = 0},--16  1级宝石
		},
	},
	new_server_treasure_percent =
	{
		{																--(新开服活动）
			prize_1 = 						
			{
				cost_money = {money_type = 3, money_value = 999},--没用
				cost_item = {item_id = 31142100, item_num = 1},
				add_score = 0,--积分不加
				add_luck = 0,--幸运值没有
				add_item = {item_id = 0, item_num = 0},--没有奖励
				next  =
				{
					--prize_percent为权重 总权重需要等于10000
					nomal_prize = {
								{prize_index = 1, prize_percent = 25},
								{prize_index = 2, prize_percent = 852},
								{prize_index = 3, prize_percent = 284},
								{prize_index = 4, prize_percent = 1705},
								{prize_index = 5, prize_percent = 852},
								{prize_index = 6, prize_percent = 71},
								{prize_index = 7, prize_percent = 427},
								{prize_index = 8, prize_percent = 852},
								{prize_index = 9, prize_percent = 143},
								{prize_index = 10, prize_percent = 852},
								{prize_index = 11, prize_percent = 1705},
								{prize_index = 12, prize_percent = 284},
								{prize_index = 13, prize_percent = 852},
								{prize_index = 14, prize_percent = 100},
								{prize_index = 15, prize_percent = 569},
								{prize_index = 16, prize_percent = 427}
								},
					best_prize =--大奖没用
					{
						basics = 0,
						score_add = 200,
						min_score_add = 999,
						max_score_add = 999,
						percent = {
									{prize_index = 1, prize_percent = 10000}
								  },
					}
				},
			},
		prize_10 =
			{
				cost_money = {money_type = 3, money_value = 9999},
				cost_item = {item_id = 31142100, item_num = 10},
				add_score = 0,
				add_luck = 0,
				add_item = {item_id = 0, item_num = 0},
				next  =
				{
					nomal_prize = {--普通奖励                                      
								{prize_index = 2, prize_percent = 882},
								{prize_index = 3, prize_percent = 294},
								{prize_index = 4, prize_percent = 1765},
								{prize_index = 5, prize_percent = 882},
								{prize_index = 7, prize_percent = 442},
								{prize_index = 8, prize_percent = 882},
								{prize_index = 10, prize_percent = 882},
								{prize_index = 11, prize_percent = 1765},
								{prize_index = 12, prize_percent = 294},
								{prize_index = 13, prize_percent = 882},
								{prize_index = 15, prize_percent = 588},
								{prize_index = 16, prize_percent = 442}
							},
					better_percent = 350,--稀有奖励概率
					better_max = 1,
					better_min = 0,
					better_prize = {
					                {prize_index = 1, prize_percent = 737},
									{prize_index = 6, prize_percent = 2094},
									{prize_index = 9, prize_percent = 4218},
									{prize_index = 14, prize_percent = 2951}
									},
					best_prize =--
					{
						basics = 0,
						score_add = 4,
						min_score_add = 100,
						max_score_add = 242,
						percent = {
									{prize_index = 1, prize_percent = 10000}
								  },
					}
				},

			},
		},
	},
	
	time_limit_activity_treasure_data2 =
	{
		-- 从左上角开始顺时针循环计算
		{                                                                                                                                    --日本新服1
			--{item_id = "物品Id", item_num = "物品数量", item_quality = "奖品类型"},  注: 奖品类型 0是普通物品 1是稀有物品 2是极品物品
			{item_id = 31090593, item_num = 20, item_quality = 2},--物品位置1
			{item_id = 31020005, item_num = 50, item_quality = 0},--物品位置2
			{item_id = 31140418, item_num = 90, item_quality = 0},--物品位置3
			{item_id = 31140418, item_num = 25, item_quality = 0},--物品位置4
			{item_id = 31140418, item_num = 10, item_quality = 0},--物品位置5
			{item_id = 31010172, item_num = 10000, item_quality = 1},--物品位置6
			{item_id = 31010107, item_num = 10, item_quality = 0},--物品位置7
			{item_id = 31000099, item_num = 5, item_quality = 0},--物品位置8
			{item_id = 31010172, item_num = 500, item_quality = 1},--物品位置9
			{item_id = 31000170, item_num = 1, item_quality = 0},--物品位置10
			{item_id = 31020056, item_num = 3, item_quality = 0},--物品位置11
			{item_id = 31020055, item_num = 3, item_quality = 0},--物品位置12
			{item_id = 31000100, item_num = 1, item_quality = 0},--物品位置13
			{item_id = 31010172, item_num = 200, item_quality = 1},--物品位置14
			{item_id = 31000105, item_num = 5, item_quality = 0},--物品位置15
			{item_id = 31000106, item_num = 2, item_quality = 0},--物品位置16
		},
		{                                                                                                                                    --日本跨服2
			--{item_id = "物品Id", item_num = "物品数量", item_quality = "奖品类型"},  注: 奖品类型 0是普通物品 1是稀有物品 2是极品物品
			{item_id = 31090593, item_num = 20, item_quality = 2},--物品位置1
			{item_id = 31140303, item_num = 1, item_quality = 0},--物品位置2
			{item_id = 31140418, item_num = 90, item_quality = 0},--物品位置3
			{item_id = 31140418, item_num = 25, item_quality = 0},--物品位置4
			{item_id = 31140418, item_num = 10, item_quality = 0},--物品位置5
			{item_id = 31010172, item_num = 10000, item_quality = 1},--物品位置6
			{item_id = 31010107, item_num = 15, item_quality = 0},--物品位置7
			{item_id = 31000099, item_num = 8, item_quality = 0},--物品位置8
			{item_id = 31010172, item_num = 500, item_quality = 1},--物品位置9
			{item_id = 31000178, item_num = 1, item_quality = 0},--物品位置10
			{item_id = 31000181, item_num = 1, item_quality = 0},--物品位置11
			{item_id = 31000180, item_num = 1, item_quality = 0},--物品位置12
			{item_id = 31000100, item_num = 1, item_quality = 0},--物品位置13
			{item_id = 31010172, item_num = 200, item_quality = 1},--物品位置14
			{item_id = 31000105, item_num = 8, item_quality = 0},--物品位置15
			{item_id = 31000106, item_num = 4, item_quality = 0},--物品位置16
		},
		{                                                                                                                                    --日本合服3
			--{item_id = "物品Id", item_num = "物品数量", item_quality = "奖品类型"},  注: 奖品类型 0是普通物品 1是稀有物品 2是极品物品
			{item_id = 31090593, item_num = 1, item_quality = 2},--物品位置1
			{item_id = 31090563, item_num = 1, item_quality = 0},--物品位置2
			{item_id = 31140418, item_num = 90, item_quality = 0},--物品位置3
			{item_id = 31140418, item_num = 25, item_quality = 0},--物品位置4
			{item_id = 31140418, item_num = 10, item_quality = 0},--物品位置5
			{item_id = 31010172, item_num = 10000, item_quality = 1},--物品位置6
			{item_id = 31010107, item_num = 15, item_quality = 0},--物品位置7
			{item_id = 31000099, item_num = 8, item_quality = 0},--物品位置8
			{item_id = 31010172, item_num = 500, item_quality = 1},--物品位置9
			{item_id = 31041116, item_num = 1, item_quality = 0},--物品位置10
			{item_id = 31000181, item_num = 1, item_quality = 0},--物品位置11
			{item_id = 31041117, item_num = 1, item_quality = 0},--物品位置12
			{item_id = 31000100, item_num = 1, item_quality = 0},--物品位置13
			{item_id = 31010172, item_num = 200, item_quality = 1},--物品位置14
			{item_id = 31000105, item_num = 8, item_quality = 0},--物品位置15
			{item_id = 31000106, item_num = 4, item_quality = 0},--物品位置16
		},
		{                                                                                                                                    --韩服翅膀4
			--{item_id = "物品Id", item_num = "物品数量", item_quality = "奖品类型"},  注: 奖品类型 0是普通物品 1是稀有物品 2是极品物品
			{item_id = 31040226, item_num = 20, item_quality = 2},--物品位置1
			{item_id = 31020055, item_num = 1, item_quality = 0},--物品位置2
			{item_id = 31020063, item_num = 1, item_quality = 0},--物品位置3
			{item_id = 31000099, item_num = 32, item_quality = 0},--物品位置4
			{item_id = 31020056, item_num = 1, item_quality = 0},--物品位置5
			{item_id = 31040178, item_num = 20, item_quality = 1},--物品位置6
			{item_id = 31140208, item_num = 1, item_quality = 0},--物品位置7
			{item_id = 31020003, item_num = 33, item_quality = 0},--物品位置8
			{item_id = 31141063, item_num = 2, item_quality = 1},--物品位置9
			{item_id = 31090546, item_num = 5, item_quality = 0},--物品位置10
			{item_id = 31090545, item_num = 5, item_quality = 0},--物品位置11
			{item_id = 31020064, item_num = 1, item_quality = 0},--物品位置12
			{item_id = 31000063, item_num = 2, item_quality = 0},--物品位置13
			{item_id = 31141064, item_num = 2, item_quality = 1},--物品位置14
			{item_id = 31000100, item_num = 1, item_quality = 0},--物品位置15
			{item_id = 31020068, item_num = 1, item_quality = 0},--物品位置16
		},
		{                                                                                                                                    --韩服精灵5
			--{item_id = "物品Id", item_num = "物品数量", item_quality = "奖品类型"},  注: 奖品类型 0是普通物品 1是稀有物品 2是极品物品
			{item_id = 31051017, item_num = 20, item_quality = 2},--物品位置1
			{item_id = 31020055, item_num = 1, item_quality = 0},--物品位置2
			{item_id = 31142101, item_num = 1, item_quality = 0},--物品位置3
			{item_id = 31010107, item_num = 100, item_quality = 0},--物品位置4
			{item_id = 31020056, item_num = 1, item_quality = 0},--物品位置5
			{item_id = 31140143, item_num = 1, item_quality = 1},--物品位置6
			{item_id = 31140214, item_num = 1, item_quality = 0},--物品位置7
			{item_id = 31020003, item_num = 33, item_quality = 0},--物品位置8
			{item_id = 31061034, item_num = 2, item_quality = 1},--物品位置9
			{item_id = 31090576, item_num = 5, item_quality = 0},--物品位置10
			{item_id = 31090577, item_num = 5, item_quality = 0},--物品位置11
			{item_id = 31020064, item_num = 1, item_quality = 0},--物品位置12
			{item_id = 31000063, item_num = 2, item_quality = 0},--物品位置13
			{item_id = 31090505, item_num = 1, item_quality = 1},--物品位置14
			{item_id = 31141065, item_num = 1, item_quality = 0},--物品位置15
			{item_id = 31020068, item_num = 1, item_quality = 0},--物品位置16
		},
		{                                                                                                                                    --韩服坐骑6
			--{item_id = "物品Id", item_num = "物品数量", item_quality = "奖品类型"},  注: 奖品类型 0是普通物品 1是稀有物品 2是极品物品
			{item_id = 31030093, item_num = 20, item_quality = 2},--物品位置1
			{item_id = 31020055, item_num = 1, item_quality = 0},--物品位置2
			{item_id = 31020063, item_num = 1, item_quality = 0},--物品位置3
			{item_id = 31000105, item_num = 32, item_quality = 0},--物品位置4
			{item_id = 31020056, item_num = 1, item_quality = 0},--物品位置5
			{item_id = 31030111, item_num = 20, item_quality = 1},--物品位置6
			{item_id = 31140214, item_num = 1, item_quality = 0},--物品位置7
			{item_id = 31141060, item_num = 6, item_quality = 0},--物品位置8
			{item_id = 31030105, item_num = 20, item_quality = 1},--物品位置9
			{item_id = 31090542, item_num = 5, item_quality = 0},--物品位置10
			{item_id = 31090541, item_num = 5, item_quality = 0},--物品位置11
			{item_id = 31020064, item_num = 1, item_quality = 0},--物品位置12
			{item_id = 31141059, item_num = 12, item_quality = 0},--物品位置13
			{item_id = 31028004, item_num = 20, item_quality = 1},--物品位置14
			{item_id = 31000106, item_num = 12, item_quality = 0},--物品位置15
			{item_id = 31020068, item_num = 1, item_quality = 0},--物品位置16
		},
		{                                                                                                                                    --东南亚新服7
			--{item_id = "物品Id", item_num = "物品数量", item_quality = "奖品类型"},  注: 奖品类型 0是普通物品 1是稀有物品 2是极品物品
			{item_id = 31090593, item_num = 1, item_quality = 2},--物品位置1
			{item_id = 31020005, item_num = 50, item_quality = 0},--物品位置2
			{item_id = 31141103, item_num = 30, item_quality = 0},--物品位置3
			{item_id = 31090573, item_num = 1, item_quality = 0},--物品位置4
			{item_id = 31090574, item_num = 1, item_quality = 0},--物品位置5
			{item_id = 31010172, item_num = 10000, item_quality = 2},--物品位置6
			{item_id = 31010107, item_num = 10, item_quality = 0},--物品位置7
			{item_id = 31000099, item_num = 5, item_quality = 0},--物品位置8
			{item_id = 31010172, item_num = 500, item_quality = 1},--物品位置9
			{item_id = 31000170, item_num = 1, item_quality = 0},--物品位置10
			{item_id = 31020056, item_num = 3, item_quality = 0},--物品位置11
			{item_id = 31020055, item_num = 3, item_quality = 0},--物品位置12
			{item_id = 31000100, item_num = 1, item_quality = 0},--物品位置13
			{item_id = 31010172, item_num = 200, item_quality = 1},--物品位置14
			{item_id = 31000105, item_num = 5, item_quality = 0},--物品位置15
			{item_id = 31000106, item_num = 2, item_quality = 0},--物品位置16
		},
	},
	time_limit_activity_treasure_percent2 =
	{
		{							                                                                                                            --日本新服1
			-- 单抽配置
			prize_1 = 
			{
				cost_money = {money_type = 3, money_value = 60},	-- 单次抽奖使用的货币类型与货币数量
				cost_item = {item_id = 31142106, item_num = 1},		-- 单次抽奖使用的物品与物品数量
				add_score = 1,										-- 每次抽奖增加的活动积分		(填1即可  不能删除)
				add_luck = 0,										-- 每次抽奖增加的幸运值			(此处不填  不能删除)
				add_item = {item_id = 0, item_num = 0},				-- 每次钻石抽奖获得的额外奖品	(不使用填0就可以  不能删除)
				next  =
				{
								--prize_percent为权重 总权重需要等于10000
					nomal_prize = {
								-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
								{prize_index = 1, prize_percent = 15},
								{prize_index = 2, prize_percent = 100},
								{prize_index = 3, prize_percent = 50},
								{prize_index = 4, prize_percent = 100},
								{prize_index = 5, prize_percent = 400},
								{prize_index = 6, prize_percent = 5},
								{prize_index = 7, prize_percent = 1630},
								{prize_index = 8, prize_percent = 1300},
								{prize_index = 9, prize_percent = 100},
								{prize_index = 10, prize_percent = 1500},
								{prize_index = 11, prize_percent = 200},
								{prize_index = 12, prize_percent = 200},
								{prize_index = 13, prize_percent = 1300},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1300},
								{prize_index = 16, prize_percent = 1300}
								},
					best_prize = 				-- 大奖的配置 (不需要 basics与score_add填0就可以)
					{
						basics = 0,				-- 基础权重
						score_add = 200,		-- 大于最低幸运值后每点幸运值增加的权重
						min_score_add = 999,	-- 获得最低幸运值	获得此物品的最低幸运值
						max_score_add = 999,	-- 获得最高幸运值	幸运值到最高使用下面配置的权重
						percent = {
									{prize_index = 1, prize_percent = 10000} -- 幸运值最高后的物品权重
								  },
					}
				},
			},
		--10连抽奖配置
		prize_10 =
			{
				cost_money = {money_type = 3, money_value = 600},	-- 10连抽奖使用的货币类型与货币数量
				cost_item = {item_id = 31142106, item_num = 10},	-- 10连抽奖使用的物品与物品数量
				add_score = 10,										-- 每次10连抽奖增加的活动积分			(填10即可  不能删除)
				add_luck = 0,										-- 每次10连抽奖增加的幸运值			    (此处不填  不能删除)
				add_item = {item_id = 0, item_num = 0},				-- 每次使用钻石10连抽奖获得的额外奖品	(不使用填0就可以  不能删除)

				next  =
				{
					--普通奖励权重 
					nomal_prize = {       
								-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
								{prize_index = 1, prize_percent = 15},
								{prize_index = 2, prize_percent = 100},
								{prize_index = 3, prize_percent = 50},
								{prize_index = 4, prize_percent = 100},
								{prize_index = 5, prize_percent = 400},
								{prize_index = 6, prize_percent = 5},
								{prize_index = 7, prize_percent = 1630},
								{prize_index = 8, prize_percent = 1300},
								{prize_index = 9, prize_percent = 100},
								{prize_index = 10, prize_percent = 1500},
								{prize_index = 11, prize_percent = 200},
								{prize_index = 12, prize_percent = 200},
								{prize_index = 13, prize_percent = 1300},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1300},
								{prize_index = 16, prize_percent = 1300}
							},
					better_percent = 0,		-- 10连抽奖中每次抽奖获得稀有奖励的权重
					better_max = 10,				-- 10连抽奖最大获得稀有奖励次数
					better_min = 0,				-- 10连抽奖最少获得稀有奖励次数
					--稀有奖励权重
					better_prize = {
									-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
					                {prize_index = 1, prize_percent = 737},
									{prize_index = 6, prize_percent = 2094},
									{prize_index = 9, prize_percent = 4218},
									{prize_index = 14, prize_percent = 2951}
									},
					
					best_prize =				-- 大奖的配置 (不需要 basics与score_add填0就可以)
					{
						basics = 0,				-- 基础权重
						score_add = 200,		-- 大于最低幸运值后每点幸运值增加的权重
						min_score_add = 999,	-- 获得最低幸运值	获得此物品的最低幸运值
						max_score_add = 999,	-- 获得最高幸运值	幸运值到最高使用下面配置的权重
						percent = {
									{prize_index = 1, prize_percent = 10000} -- 幸运值最高后的物品权重
								  },
					}
				},

			}
		},
		{						                                                                                                                --日本跨服2
			prize_1 = 
			{
				cost_money = {money_type = 3, money_value = 60},	-- 单次抽奖使用的货币类型与货币数量
				cost_item = {item_id = 31142106, item_num = 1},		-- 单次抽奖使用的物品与物品数量
				add_score = 1,										-- 每次抽奖增加的活动积分		(填1即可  不能删除)
				add_luck = 0,										-- 每次抽奖增加的幸运值			(此处不填  不能删除)
				add_item = {item_id = 0, item_num = 0},				-- 每次钻石抽奖获得的额外奖品	(不使用填0就可以  不能删除)
				next  =
				{
								--prize_percent为权重 总权重需要等于10000
					nomal_prize = {
								-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
								{prize_index = 1, prize_percent = 15},
								{prize_index = 2, prize_percent = 20},
								{prize_index = 3, prize_percent = 50},
								{prize_index = 4, prize_percent = 100},
								{prize_index = 5, prize_percent = 400},
								{prize_index = 6, prize_percent = 5},
								{prize_index = 7, prize_percent = 1580},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 9, prize_percent = 100},
								{prize_index = 10, prize_percent = 1630},
								{prize_index = 11, prize_percent = 250},
								{prize_index = 12, prize_percent = 550},
								{prize_index = 13, prize_percent = 1200},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1200},
								{prize_index = 16, prize_percent = 1200}
								},
					best_prize = 				-- 大奖的配置 (不需要 basics与score_add填0就可以)
					{
						basics = 0,				-- 基础权重
						score_add = 200,		-- 大于最低幸运值后每点幸运值增加的权重
						min_score_add = 999,	-- 获得最低幸运值	获得此物品的最低幸运值
						max_score_add = 999,	-- 获得最高幸运值	幸运值到最高使用下面配置的权重
						percent = {
									{prize_index = 1, prize_percent = 10000} -- 幸运值最高后的物品权重
								  },
					}
				},
			},
		--10连抽奖配置
		prize_10 =
			{
				cost_money = {money_type = 3, money_value = 600},	-- 10连抽奖使用的货币类型与货币数量
				cost_item = {item_id = 31142106, item_num = 10},	-- 10连抽奖使用的物品与物品数量
				add_score = 10,										-- 每次10连抽奖增加的活动积分			(填10即可  不能删除)
				add_luck = 0,										-- 每次10连抽奖增加的幸运值			    (此处不填  不能删除)
				add_item = {item_id = 0, item_num = 0},				-- 每次使用钻石10连抽奖获得的额外奖品	(不使用填0就可以  不能删除)

				next  =
				{
					--普通奖励权重 
					nomal_prize = {       
								-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
								{prize_index = 1, prize_percent = 15},
								{prize_index = 2, prize_percent = 20},
								{prize_index = 3, prize_percent = 50},
								{prize_index = 4, prize_percent = 100},
								{prize_index = 5, prize_percent = 400},
								{prize_index = 6, prize_percent = 5},
								{prize_index = 7, prize_percent = 1580},
								{prize_index = 8, prize_percent = 1200},
								{prize_index = 9, prize_percent = 100},
								{prize_index = 10, prize_percent = 1630},
								{prize_index = 11, prize_percent = 250},
								{prize_index = 12, prize_percent = 550},
								{prize_index = 13, prize_percent = 1200},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1200},
								{prize_index = 16, prize_percent = 1200}
							},
					better_percent = 0,		-- 10连抽奖中每次抽奖获得稀有奖励的权重
					better_max = 10,				-- 10连抽奖最大获得稀有奖励次数
					better_min = 0,				-- 10连抽奖最少获得稀有奖励次数
					--稀有奖励权重
					better_prize = {
									-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
					                {prize_index = 1, prize_percent = 737},
									{prize_index = 6, prize_percent = 2094},
									{prize_index = 9, prize_percent = 4218},
									{prize_index = 14, prize_percent = 2951}
									},
					
					best_prize =				-- 大奖的配置 (不需要 basics与score_add填0就可以)
					{
						basics = 0,				-- 基础权重
						score_add = 200,		-- 大于最低幸运值后每点幸运值增加的权重
						min_score_add = 999,	-- 获得最低幸运值	获得此物品的最低幸运值
						max_score_add = 999,	-- 获得最高幸运值	幸运值到最高使用下面配置的权重
						percent = {
									{prize_index = 1, prize_percent = 10000} -- 幸运值最高后的物品权重
								  },
					}
				},

			}
		},
		{					                                                                                                                    --日本合服3	
			prize_1 = 
			{
				cost_money = {money_type = 3, money_value = 60},	-- 单次抽奖使用的货币类型与货币数量
				cost_item = {item_id = 31142106, item_num = 1},		-- 单次抽奖使用的物品与物品数量
				add_score = 1,										-- 每次抽奖增加的活动积分		(填1即可  不能删除)
				add_luck = 0,										-- 每次抽奖增加的幸运值			(此处不填  不能删除)
				add_item = {item_id = 0, item_num = 0},				-- 每次钻石抽奖获得的额外奖品	(不使用填0就可以  不能删除)
				next  =
				{
								--prize_percent为权重 总权重需要等于10000
					nomal_prize = {
								-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
								{prize_index = 1, prize_percent = 15},
								{prize_index = 2, prize_percent = 100},
								{prize_index = 3, prize_percent = 50},
								{prize_index = 4, prize_percent = 100},
								{prize_index = 5, prize_percent = 400},
								{prize_index = 6, prize_percent = 5},
								{prize_index = 7, prize_percent = 1580},
								{prize_index = 8, prize_percent = 1300},
								{prize_index = 9, prize_percent = 100},
								{prize_index = 10, prize_percent = 1100},
								{prize_index = 11, prize_percent = 250},
								{prize_index = 12, prize_percent = 600},
								{prize_index = 13, prize_percent = 1300},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1300},
								{prize_index = 16, prize_percent = 1300}
								},
					best_prize = 				-- 大奖的配置 (不需要 basics与score_add填0就可以)
					{
						basics = 0,				-- 基础权重
						score_add = 200,		-- 大于最低幸运值后每点幸运值增加的权重
						min_score_add = 999,	-- 获得最低幸运值	获得此物品的最低幸运值
						max_score_add = 999,	-- 获得最高幸运值	幸运值到最高使用下面配置的权重
						percent = {
									{prize_index = 1, prize_percent = 10000} -- 幸运值最高后的物品权重
								  },
					}
				},
			},
		--10连抽奖配置
		prize_10 =
			{
				cost_money = {money_type = 3, money_value = 600},	-- 10连抽奖使用的货币类型与货币数量
				cost_item = {item_id = 31142106, item_num = 10},	-- 10连抽奖使用的物品与物品数量
				add_score = 10,										-- 每次10连抽奖增加的活动积分			(填10即可  不能删除)
				add_luck = 0,										-- 每次10连抽奖增加的幸运值			    (此处不填  不能删除)
				add_item = {item_id = 0, item_num = 0},				-- 每次使用钻石10连抽奖获得的额外奖品	(不使用填0就可以  不能删除)

				next  =
				{
					--普通奖励权重 
					nomal_prize = {       
								-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
								{prize_index = 1, prize_percent = 15},
								{prize_index = 2, prize_percent = 100},
								{prize_index = 3, prize_percent = 50},
								{prize_index = 4, prize_percent = 100},
								{prize_index = 5, prize_percent = 400},
								{prize_index = 6, prize_percent = 5},
								{prize_index = 7, prize_percent = 1580},
								{prize_index = 8, prize_percent = 1300},
								{prize_index = 9, prize_percent = 100},
								{prize_index = 10, prize_percent = 1100},
								{prize_index = 11, prize_percent = 250},
								{prize_index = 12, prize_percent = 600},
								{prize_index = 13, prize_percent = 1300},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1300},
								{prize_index = 16, prize_percent = 1300}
							},
					better_percent = 0,		-- 10连抽奖中每次抽奖获得稀有奖励的权重
					better_max = 10,				-- 10连抽奖最大获得稀有奖励次数
					better_min = 0,				-- 10连抽奖最少获得稀有奖励次数
					--稀有奖励权重
					better_prize = {
									-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
					                {prize_index = 1, prize_percent = 737},
									{prize_index = 6, prize_percent = 2094},
									{prize_index = 9, prize_percent = 4218},
									{prize_index = 14, prize_percent = 2951}
									},
					
					best_prize =				-- 大奖的配置 (不需要 basics与score_add填0就可以)
					{
						basics = 0,				-- 基础权重
						score_add = 200,		-- 大于最低幸运值后每点幸运值增加的权重
						min_score_add = 999,	-- 获得最低幸运值	获得此物品的最低幸运值
						max_score_add = 999,	-- 获得最高幸运值	幸运值到最高使用下面配置的权重
						percent = {
									{prize_index = 1, prize_percent = 10000} -- 幸运值最高后的物品权重
								  },
					}
				},

			},
		},
        { 					                                                                                                                    --韩服翅膀4	
			prize_1 =
			{
				cost_money = {money_type = 3, money_value = 300},	-- 单次抽奖使用的货币类型与货币数量
				cost_item = {item_id = 31142106, item_num = 1},		-- 单次抽奖使用的物品与物品数量
				add_score = 1,										-- 每次抽奖增加的活动积分		(填1即可  不能删除)
				add_luck = 0,										-- 每次抽奖增加的幸运值			(此处不填  不能删除)
				add_item = {item_id = 0, item_num = 0},				-- 每次钻石抽奖获得的额外奖品	(不使用填0就可以  不能删除)
				next  =
				{
								--prize_percent为权重 总权重需要等于10000
					nomal_prize = {
								-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
								{prize_index = 1, prize_percent = 5},
								{prize_index = 2, prize_percent = 850},
								{prize_index = 3, prize_percent = 300},
								{prize_index = 4, prize_percent = 1700},
								{prize_index = 5, prize_percent = 850},
								{prize_index = 6, prize_percent = 75},
								{prize_index = 7, prize_percent = 450},
								{prize_index = 8, prize_percent = 850},
								{prize_index = 9, prize_percent = 150},
								{prize_index = 10, prize_percent = 850},
								{prize_index = 11, prize_percent = 1700},
								{prize_index = 12, prize_percent = 300},
								{prize_index = 13, prize_percent = 850},
								{prize_index = 14, prize_percent = 120},
								{prize_index = 15, prize_percent = 550},
								{prize_index = 16, prize_percent = 400}
								},
					best_prize = 				-- 大奖的配置 (不需要 basics与score_add填0就可以)
					{
						basics = 0,				-- 基础权重
						score_add = 200,		-- 大于最低幸运值后每点幸运值增加的权重
						min_score_add = 999,	-- 获得最低幸运值	获得此物品的最低幸运值
						max_score_add = 999,	-- 获得最高幸运值	幸运值到最高使用下面配置的权重
						percent = {
									{prize_index = 1, prize_percent = 10000} -- 幸运值最高后的物品权重
								  },
					}
				},
			},
		--10连抽奖配置
		prize_10 =
			{
				cost_money = {money_type = 3, money_value = 2700},	-- 10连抽奖使用的货币类型与货币数量
				cost_item = {item_id = 31142106, item_num = 9},	-- 10连抽奖使用的物品与物品数量
				add_score = 10,										-- 每次10连抽奖增加的活动积分			(填10即可  不能删除)
				add_luck = 0,										-- 每次10连抽奖增加的幸运值			    (此处不填  不能删除)
				add_item = {item_id = 0, item_num = 0},				-- 每次使用钻石10连抽奖获得的额外奖品	(不使用填0就可以  不能删除)

				next  =
				{
					--普通奖励权重 
					nomal_prize = {       
								-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
								{prize_index = 1, prize_percent = 5},
								{prize_index = 2, prize_percent = 850},
								{prize_index = 3, prize_percent = 300},
								{prize_index = 4, prize_percent = 1700},
								{prize_index = 5, prize_percent = 850},
								{prize_index = 6, prize_percent = 75},
								{prize_index = 7, prize_percent = 450},
								{prize_index = 8, prize_percent = 850},
								{prize_index = 9, prize_percent = 150},
								{prize_index = 10, prize_percent = 850},
								{prize_index = 11, prize_percent = 1700},
								{prize_index = 12, prize_percent = 300},
								{prize_index = 13, prize_percent = 850},
								{prize_index = 14, prize_percent = 120},
								{prize_index = 15, prize_percent = 550},
								{prize_index = 16, prize_percent = 400}
							},
					better_percent = 0,		-- 10连抽奖中每次抽奖获得稀有奖励的权重
					better_max = 10,				-- 10连抽奖最大获得稀有奖励次数
					better_min = 0,				-- 10连抽奖最少获得稀有奖励次数
					--稀有奖励权重
					better_prize = {
									-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
					                {prize_index = 1, prize_percent = 737},
									{prize_index = 6, prize_percent = 2094},
									{prize_index = 9, prize_percent = 4218},
									{prize_index = 14, prize_percent = 2951}
									},
					
					best_prize =				-- 大奖的配置 (不需要 basics与score_add填0就可以)
					{
						basics = 0,				-- 基础权重
						score_add = 200,		-- 大于最低幸运值后每点幸运值增加的权重
						min_score_add = 999,	-- 获得最低幸运值	获得此物品的最低幸运值
						max_score_add = 999,	-- 获得最高幸运值	幸运值到最高使用下面配置的权重
						percent = {
									{prize_index = 1, prize_percent = 10000} -- 幸运值最高后的物品权重
								  },
					}
				},

			},
		},
        {					                                                                                                                    --韩服精灵5
			prize_1 = 
			{
				cost_money = {money_type = 3, money_value = 300},	-- 单次抽奖使用的货币类型与货币数量
				cost_item = {item_id = 31142106, item_num = 1},		-- 单次抽奖使用的物品与物品数量
				add_score = 1,										-- 每次抽奖增加的活动积分		(填1即可  不能删除)
				add_luck = 0,										-- 每次抽奖增加的幸运值			(此处不填  不能删除)
				add_item = {item_id = 0, item_num = 0},				-- 每次钻石抽奖获得的额外奖品	(不使用填0就可以  不能删除)
				next  =
				{
								--prize_percent为权重 总权重需要等于10000
					nomal_prize = {
								-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
								{prize_index = 1, prize_percent = 5},
								{prize_index = 2, prize_percent = 850},
								{prize_index = 3, prize_percent = 300},
								{prize_index = 4, prize_percent = 1700},
								{prize_index = 5, prize_percent = 850},
								{prize_index = 6, prize_percent = 75},
								{prize_index = 7, prize_percent = 450},
								{prize_index = 8, prize_percent = 850},
								{prize_index = 9, prize_percent = 150},
								{prize_index = 10, prize_percent = 850},
								{prize_index = 11, prize_percent = 1700},
								{prize_index = 12, prize_percent = 300},
								{prize_index = 13, prize_percent = 850},
								{prize_index = 14, prize_percent = 120},
								{prize_index = 15, prize_percent = 550},
								{prize_index = 16, prize_percent = 400}
								},
					best_prize = 				-- 大奖的配置 (不需要 basics与score_add填0就可以)
					{
						basics = 0,				-- 基础权重
						score_add = 200,		-- 大于最低幸运值后每点幸运值增加的权重
						min_score_add = 999,	-- 获得最低幸运值	获得此物品的最低幸运值
						max_score_add = 999,	-- 获得最高幸运值	幸运值到最高使用下面配置的权重
						percent = {
									{prize_index = 1, prize_percent = 10000} -- 幸运值最高后的物品权重
								  },
					}
				},
			},
		--10连抽奖配置
		prize_10 =
			{
				cost_money = {money_type = 3, money_value = 2700},	-- 10连抽奖使用的货币类型与货币数量
				cost_item = {item_id = 31142106, item_num = 9},	-- 10连抽奖使用的物品与物品数量
				add_score = 10,										-- 每次10连抽奖增加的活动积分			(填10即可  不能删除)
				add_luck = 0,										-- 每次10连抽奖增加的幸运值			    (此处不填  不能删除)
				add_item = {item_id = 0, item_num = 0},				-- 每次使用钻石10连抽奖获得的额外奖品	(不使用填0就可以  不能删除)

				next  =
				{
					--普通奖励权重 
					nomal_prize = {       
								-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
								{prize_index = 1, prize_percent = 5},
								{prize_index = 2, prize_percent = 850},
								{prize_index = 3, prize_percent = 300},
								{prize_index = 4, prize_percent = 1700},
								{prize_index = 5, prize_percent = 850},
								{prize_index = 6, prize_percent = 75},
								{prize_index = 7, prize_percent = 450},
								{prize_index = 8, prize_percent = 850},
								{prize_index = 9, prize_percent = 150},
								{prize_index = 10, prize_percent = 850},
								{prize_index = 11, prize_percent = 1700},
								{prize_index = 12, prize_percent = 300},
								{prize_index = 13, prize_percent = 850},
								{prize_index = 14, prize_percent = 120},
								{prize_index = 15, prize_percent = 550},
								{prize_index = 16, prize_percent = 400}
							},
					better_percent = 0,		-- 10连抽奖中每次抽奖获得稀有奖励的权重
					better_max = 10,				-- 10连抽奖最大获得稀有奖励次数
					better_min = 0,				-- 10连抽奖最少获得稀有奖励次数
					--稀有奖励权重
					better_prize = {
									-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
					                {prize_index = 1, prize_percent = 737},
									{prize_index = 6, prize_percent = 2094},
									{prize_index = 9, prize_percent = 4218},
									{prize_index = 14, prize_percent = 2951}
									},
					
					best_prize =				-- 大奖的配置 (不需要 basics与score_add填0就可以)
					{
						basics = 0,				-- 基础权重
						score_add = 200,		-- 大于最低幸运值后每点幸运值增加的权重
						min_score_add = 999,	-- 获得最低幸运值	获得此物品的最低幸运值
						max_score_add = 999,	-- 获得最高幸运值	幸运值到最高使用下面配置的权重
						percent = {
									{prize_index = 1, prize_percent = 10000} -- 幸运值最高后的物品权重
								  },
					}
				},

			},
		},
        { 					                                                                                                                    --韩服坐骑6
			prize_1 =
			{
				cost_money = {money_type = 3, money_value = 300},	-- 单次抽奖使用的货币类型与货币数量
				cost_item = {item_id = 31142106, item_num = 1},		-- 单次抽奖使用的物品与物品数量
				add_score = 1,										-- 每次抽奖增加的活动积分		(填1即可  不能删除)
				add_luck = 0,										-- 每次抽奖增加的幸运值			(此处不填  不能删除)
				add_item = {item_id = 0, item_num = 0},				-- 每次钻石抽奖获得的额外奖品	(不使用填0就可以  不能删除)
				next  =
				{
								--prize_percent为权重 总权重需要等于10000
					nomal_prize = {
								-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
								{prize_index = 1, prize_percent = 5},
								{prize_index = 2, prize_percent = 850},
								{prize_index = 3, prize_percent = 300},
								{prize_index = 4, prize_percent = 1700},
								{prize_index = 5, prize_percent = 850},
								{prize_index = 6, prize_percent = 75},
								{prize_index = 7, prize_percent = 450},
								{prize_index = 8, prize_percent = 850},
								{prize_index = 9, prize_percent = 150},
								{prize_index = 10, prize_percent = 850},
								{prize_index = 11, prize_percent = 1700},
								{prize_index = 12, prize_percent = 300},
								{prize_index = 13, prize_percent = 850},
								{prize_index = 14, prize_percent = 120},
								{prize_index = 15, prize_percent = 550},
								{prize_index = 16, prize_percent = 400}
								},
					best_prize = 				-- 大奖的配置 (不需要 basics与score_add填0就可以)
					{
						basics = 0,				-- 基础权重
						score_add = 200,		-- 大于最低幸运值后每点幸运值增加的权重
						min_score_add = 999,	-- 获得最低幸运值	获得此物品的最低幸运值
						max_score_add = 999,	-- 获得最高幸运值	幸运值到最高使用下面配置的权重
						percent = {
									{prize_index = 1, prize_percent = 10000} -- 幸运值最高后的物品权重
								  },
					}
				},
			},
		--10连抽奖配置
		prize_10 =
			{
				cost_money = {money_type = 3, money_value = 2700},	-- 10连抽奖使用的货币类型与货币数量
				cost_item = {item_id = 31142106, item_num = 9},	-- 10连抽奖使用的物品与物品数量
				add_score = 10,										-- 每次10连抽奖增加的活动积分			(填10即可  不能删除)
				add_luck = 0,										-- 每次10连抽奖增加的幸运值			    (此处不填  不能删除)
				add_item = {item_id = 0, item_num = 0},				-- 每次使用钻石10连抽奖获得的额外奖品	(不使用填0就可以  不能删除)

				next  =
				{
					--普通奖励权重 
					nomal_prize = {       
								-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
								{prize_index = 1, prize_percent = 5},
								{prize_index = 2, prize_percent = 850},
								{prize_index = 3, prize_percent = 300},
								{prize_index = 4, prize_percent = 1700},
								{prize_index = 5, prize_percent = 850},
								{prize_index = 6, prize_percent = 75},
								{prize_index = 7, prize_percent = 450},
								{prize_index = 8, prize_percent = 850},
								{prize_index = 9, prize_percent = 150},
								{prize_index = 10, prize_percent = 850},
								{prize_index = 11, prize_percent = 1700},
								{prize_index = 12, prize_percent = 300},
								{prize_index = 13, prize_percent = 850},
								{prize_index = 14, prize_percent = 120},
								{prize_index = 15, prize_percent = 550},
								{prize_index = 16, prize_percent = 400}
							},
					better_percent = 0,		-- 10连抽奖中每次抽奖获得稀有奖励的权重
					better_max = 10,				-- 10连抽奖最大获得稀有奖励次数
					better_min = 0,				-- 10连抽奖最少获得稀有奖励次数
					--稀有奖励权重
					better_prize = {
									-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
					                {prize_index = 1, prize_percent = 737},
									{prize_index = 6, prize_percent = 2094},
									{prize_index = 9, prize_percent = 4218},
									{prize_index = 14, prize_percent = 2951}
									},
					
					best_prize =				-- 大奖的配置 (不需要 basics与score_add填0就可以)
					{
						basics = 0,				-- 基础权重
						score_add = 200,		-- 大于最低幸运值后每点幸运值增加的权重
						min_score_add = 999,	-- 获得最低幸运值	获得此物品的最低幸运值
						max_score_add = 999,	-- 获得最高幸运值	幸运值到最高使用下面配置的权重
						percent = {
									{prize_index = 1, prize_percent = 10000} -- 幸运值最高后的物品权重
								  },
					}
				},

			},
		},	
        {							                                                                                                            --东南亚新服7
			-- 单抽配置
			prize_1 = 
			{
				cost_money = {money_type = 3, money_value = 60},	-- 单次抽奖使用的货币类型与货币数量
				cost_item = {item_id = 31142106, item_num = 1},		-- 单次抽奖使用的物品与物品数量
				add_score = 1,										-- 每次抽奖增加的活动积分		(填1即可  不能删除)
				add_luck = 0,										-- 每次抽奖增加的幸运值			(此处不填  不能删除)
				add_item = {item_id = 0, item_num = 0},				-- 每次钻石抽奖获得的额外奖品	(不使用填0就可以  不能删除)
				next  =
				{
								--prize_percent为权重 总权重需要等于10000
					nomal_prize = {
								-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
								{prize_index = 1, prize_percent = 15},
								{prize_index = 2, prize_percent = 100},
								{prize_index = 3, prize_percent = 50},
								{prize_index = 4, prize_percent = 100},
								{prize_index = 5, prize_percent = 400},
								{prize_index = 6, prize_percent = 5},
								{prize_index = 7, prize_percent = 1630},
								{prize_index = 8, prize_percent = 1300},
								{prize_index = 9, prize_percent = 100},
								{prize_index = 10, prize_percent = 1500},
								{prize_index = 11, prize_percent = 200},
								{prize_index = 12, prize_percent = 200},
								{prize_index = 13, prize_percent = 1300},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1300},
								{prize_index = 16, prize_percent = 1300}
								},
					best_prize = 				-- 大奖的配置 (不需要 basics与score_add填0就可以)
					{
						basics = 0,				-- 基础权重
						score_add = 200,		-- 大于最低幸运值后每点幸运值增加的权重
						min_score_add = 999,	-- 获得最低幸运值	获得此物品的最低幸运值
						max_score_add = 999,	-- 获得最高幸运值	幸运值到最高使用下面配置的权重
						percent = {
									{prize_index = 1, prize_percent = 10000} -- 幸运值最高后的物品权重
								  },
					}
				},
			},
		--10连抽奖配置
		prize_10 =
			{
				cost_money = {money_type = 3, money_value = 600},	-- 10连抽奖使用的货币类型与货币数量
				cost_item = {item_id = 31142106, item_num = 10},	-- 10连抽奖使用的物品与物品数量
				add_score = 10,										-- 每次10连抽奖增加的活动积分			(填10即可  不能删除)
				add_luck = 0,										-- 每次10连抽奖增加的幸运值			    (此处不填  不能删除)
				add_item = {item_id = 0, item_num = 0},				-- 每次使用钻石10连抽奖获得的额外奖品	(不使用填0就可以  不能删除)

				next  =
				{
					--普通奖励权重 
					nomal_prize = {       
								-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
								{prize_index = 1, prize_percent = 15},
								{prize_index = 2, prize_percent = 100},
								{prize_index = 3, prize_percent = 50},
								{prize_index = 4, prize_percent = 100},
								{prize_index = 5, prize_percent = 400},
								{prize_index = 6, prize_percent = 5},
								{prize_index = 7, prize_percent = 1630},
								{prize_index = 8, prize_percent = 1300},
								{prize_index = 9, prize_percent = 100},
								{prize_index = 10, prize_percent = 1500},
								{prize_index = 11, prize_percent = 200},
								{prize_index = 12, prize_percent = 200},
								{prize_index = 13, prize_percent = 1300},
								{prize_index = 14, prize_percent = 500},
								{prize_index = 15, prize_percent = 1300},
								{prize_index = 16, prize_percent = 1300}
							},
					better_percent = 0,		-- 10连抽奖中每次抽奖获得稀有奖励的权重
					better_max = 10,				-- 10连抽奖最大获得稀有奖励次数
					better_min = 0,				-- 10连抽奖最少获得稀有奖励次数
					--稀有奖励权重
					better_prize = {
									-- {prize_index = "位置索引", prize_percent = "抽奖权重"},
					                {prize_index = 1, prize_percent = 737},
									{prize_index = 6, prize_percent = 2094},
									{prize_index = 9, prize_percent = 4218},
									{prize_index = 14, prize_percent = 2951}
									},
					
					best_prize =				-- 大奖的配置 (不需要 basics与score_add填0就可以)
					{
						basics = 0,				-- 基础权重
						score_add = 200,		-- 大于最低幸运值后每点幸运值增加的权重
						min_score_add = 999,	-- 获得最低幸运值	获得此物品的最低幸运值
						max_score_add = 999,	-- 获得最高幸运值	幸运值到最高使用下面配置的权重
						percent = {
									{prize_index = 1, prize_percent = 10000} -- 幸运值最高后的物品权重
								  },
					}
				},

			}
		},		
	},
	
}
return treasure_data
