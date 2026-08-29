local att_def = {};
function att_def:init()
end

--角色当前的移动方式
--enum e_move_ment
att_def.e_move_ment_walk        = 0;	-- 人形态跑路
att_def.e_move_ment_wing_walk   = 1;	-- 背翅膀跑
att_def.e_move_ment_mount_walk  = 2;	-- 骑坐骑形态下跑路
att_def.e_move_ment_mount_fly   = 3;	-- 骑坐骑形态下飞行 
att_def.e_move_ment_wing_fly    = 4;	-- 翅膀形态下飞行 
att_def.e_move_ment_max         = 5;

--enum e_pawn_att_sync_type
att_def.send_att_to_client      = 0;       --属性同步
att_def.send_gs_to_client       = 1;       --GS同步
att_def.send_nothing_to_client  = 2;       --都不同步
att_def.send_max                = 3;

AttDef = att_def;
return AttDef;