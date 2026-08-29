
local map_data = {
    __rt1 = "6000,6000,6000,6000",
    __rt2 = "31000057,31000057,31000057,31000057",
    __rt3 = "31000047,31000047,31000047,31000047",
    __rt4 = "20,20,20,20",
    __rt5 = "1,1,1,1",
    __rt6 = "31025197,31025197,31025197,31025197",
    __rt7 = "31060068,31060068,31060068,31060068",
}local data = { 
    [134000101] = {["BigReward"] = map_data.__rt6,["BigRewardNumber"] = map_data.__rt5,["BasicReward1"] = 31010004,["BasicReward2"] = 31010054,["BasicReward3"] = 31010022,["BasicReward4"] = 31010060,},
    [134000201] = {["ActivityDay"] = 2,["BasicReward1Number"] = 3,["BasicReward2Number"] = 1,["BasicReward3Number"] = 1,["BasicReward3Weight"] = 37,["BasicReward4"] = 31000106,["BasicReward4Number"] = 1,["BasicReward4Weight"] = 13,["MinBuyCountNeedForBigReward"] = 90,},
    [134000301] = {["ActivityDay"] = 3,["NumberReleases"] = 450,["BasicReward1Number"] = 3,["BasicReward2Number"] = 1,["BasicReward3Number"] = 1,["BasicReward3Weight"] = 37,["BasicReward4"] = 31000106,["BasicReward4Number"] = 1,["BasicReward4Weight"] = 13,["MinBuyCountNeedForBigReward"] = 80,},
    [134000401] = {["ActivityDay"] = 4,["NumberReleases"] = 400,["BigReward"] = map_data.__rt7,["BigRewardNumber"] = map_data.__rt5,["BasicReward1"] = 31010059,["BasicReward2"] = 31010004,["BasicReward3"] = 31010054,["MinBuyCountNeedForBigReward"] = 70,},
    [134000501] = {["ActivityDay"] = 5,["NumberReleases"] = 350,["BigReward"] = map_data.__rt3,["BasicReward1"] = 31000099,["BasicReward1Number"] = 1,["BasicReward2"] = 31000040,["BasicReward3"] = 31010004,["MinBuyCountNeedForBigReward"] = 60,},
    [134000601] = {["ActivityDay"] = 6,["ContinueTomorrow"] = 0,["NumberReleases"] = 300,["BigReward"] = map_data.__rt3,["BasicReward1"] = 31000099,["BasicReward1Number"] = 1,["BasicReward2"] = 31000040,["BasicReward3"] = 31010004,["MinBuyCountNeedForBigReward"] = 50,},

 }
local _default_values = {
    ["ServerLevel"] = 999,
    ["ActivityDay"] = 1,
    ["SaleStartTime"] = 0,
    ["SaleEndTime"] = 64800,
    ["RobberyStartTime"] = 64801,
    ["RobberyEndTime"] = 75600,
    ["AwardOpeningTime"] = 75601,
    ["ContinueTomorrow"] = 1,
    ["NumberReleases"] = 500,
    ["Currency"] = 3,
    ["BuyingPrice"] = 10,
    ["BuyingPriceAdd"] = 0,
    ["RobberyPrice"] = 10,
    ["RobberyPriceAdd"] = 0,
    ["NumberBig"] = 1,
    ["BigReward"] = "31000057,31000057,31000057,31000057",
    ["BigValue"] = "6000,6000,6000,6000",
    ["BigRewardNumber"] = "20,20,20,20",
    ["BasicReward1"] = 31000028,
    ["BasicReward1Number"] = 5,
    ["BasicReward1Weight"] = 25,
    ["BasicReward2"] = 31000029,
    ["BasicReward2Number"] = 5,
    ["BasicReward2Weight"] = 25,
    ["BasicReward3"] = 31000105,
    ["BasicReward3Number"] = 5,
    ["BasicReward3Weight"] = 25,
    ["BasicReward4"] = 31010022,
    ["BasicReward4Number"] = 5,
    ["BasicReward4Weight"] = 25,
    ["MinBuyCountNeedForBigReward"] = 100,
}
do
    local base = { __index = _default_values, __newindex = function() error( "Attempt to modify read-only table" ) end } 
    for k,v in pairs(data) do
        setmetatable( v, base )
    end
end
return data