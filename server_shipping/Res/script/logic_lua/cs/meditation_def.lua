local meditation_def = {};
function meditation_def:init()
end

meditation_def.e_meditation_reward_usual      		 = 0  				--一倍奖励
meditation_def.e_meditation_reward_double     		 = 1				--两倍奖励
meditation_def.e_meditation_reward_four       		 = 2				--四倍奖励

meditation_def.e_meditation_get_reward_success       = 0  				--成功
meditation_def.e_meditation_reward_vip      		 = 1				--vip等级不足
meditation_def.e_meditation_reward_money_lack        = 2				--货币不足
meditation_def.e_meditation_reward_less_one_munite   = 3				--少于一分钟
meditation_def.e_meditation_reward_unknown       	 = 4				--未知错误


Meditationdef = meditation_def;
return Meditationdef;