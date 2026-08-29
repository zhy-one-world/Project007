local proto_mgr={}


local t_pb_name = 
{
    "achievement.pb",
    "arena.pb",
    "cross_ladder.pb",
    "auction.pb",
    "belief.pb",

    "chat.pb",
    "convert.pb",
    "character.pb",
    "elementheart.pb",
    "first_time_do.pb", 

    "game.pb",
    "cloud_shop.pb",
    "belief_rune.pb",
    "item.pb",
    "legion.pb",
    "pokedex.pb",
    "ranking.pb",
   

    "time_limit_activity.pb",
    "service_goal.pb",
    "relation.pb",
    "skill.pb",
    "featherheart.pb",
    "phantom.pb",
    "recycle.pb",
    "welfare.pb",
    "assist_fight.pb",

    "pokedex.pb",
    "mission.pb",
    "title.pb",
    "special_name.pb",
    "goods.pb", 
    "interaction.pb",
    

    "dragontrip.pb",
    "talent.pb",
    "server2dp.pb",
  
    "db.pb",
  
    "st.pb",
    "cs2dp.pb",
    "dp2s.pb",
    "dp2cs.pb",
    "ws2cs.pb",
    "cs2ws.pb",
    "ws2ws.pb",
  
    "team.pb",
   
    "aoi.pb",
    "mail.pb",
    "mountpower.pb",
    "skytreasure.pb",
    "starark.pb",

    "buff.pb",
    "web.pb",
    "login.pb",
    "cross.pb",
}

function proto_mgr:init()
    --[[local path = package.path
    local start_idx = string.find(path,":")
    local end_idx = string.find(path,"\\bin")
    local abs_path = string.sub(path,start_idx - 1,end_idx - 1)
    local pb_path = abs_path.."\\res\\script\\protobuf\\pb\\"
    package.path = package.path .. ";"..abs_path.."\\res\\script\\protobuf\\?.lua"]]
    require("protobuf_binary");

    
   --[[ ProtoFile=io.open(pb_path.."achievement.pb","rb");
    achievement=ProtoFile:read "*a"
    register(achievement)
    io.close()
   
    ProtoFile=io.open(pb_path.."arena.pb","rb");
	local arena=ProtoFile:read "*a"
    register(arena)
    arena=nil;
	io.close()

    ProtoFile=io.open(pb_path.."cross_ladder.pb","rb");
    local cross_ladder=ProtoFile:read "*a"
    register(cross_ladder)
    cross_ladder=nil;
    io.close()

    ProtoFile=io.open(pb_path.."auction.pb","rb");
    local auction=ProtoFile:read "*a"
    register(auction)
    auction=nil;
    io.close()

    ProtoFile=io.open(pb_path.."belief.pb","rb");
    local belief=ProtoFile:read "*a"
    register(belief)
    belief=nil;
    io.close()

    ProtoFile=io.open(pb_path.."buff.pb","rb");
    local buff=ProtoFile:read "*a"
    register(buff)
    buff=nil;
    io.close()

    ProtoFile=io.open(pb_path.."chat.pb","rb");
	local chat=ProtoFile:read "*a"
    register(chat)
    chat=nil;
    io.close()
    
    ProtoFile=io.open(pb_path.."convert.pb","rb");
    local convert=ProtoFile:read "*a"
    register(convert)
    convert=nil;
    io.close()

    ProtoFile=io.open(pb_path.."character.pb","rb");
    local character=ProtoFile:read "*a"
    register(character)
    character=nil;
    io.close()

    ProtoFile=io.open(pb_path.."elementheart.pb","rb");
	local elementheart=ProtoFile:read "*a"
    register(elementheart)
    elementheart=nil;
    io.close()
    
    ProtoFile=io.open(pb_path.."first_time_do.pb","rb");
    local first_time_do=ProtoFile:read "*a"
    register(first_time_do)
    first_time_do=nil;
    io.close()

    ProtoFile=io.open(pb_path.."game.pb","rb");
    local game=ProtoFile:read "*a"
    register(game)
    game=nil;
    io.close()

    ProtoFile=io.open(pb_path.."cloud_shop.pb","rb");
    local cloud_shop=ProtoFile:read "*a"
    register(cloud_shop)
    cloud_shop=nil;
    io.close()

    ProtoFile=io.open(pb_path.."belief_rune.pb","rb");
    local belief_rune=ProtoFile:read "*a"
    register(belief_rune)
    belief_rune=nil;
    io.close()

    ProtoFile=io.open(pb_path.."item.pb","rb");
    local item=ProtoFile:read "*a"
    register(item)
    item=nil;
    io.close()

    ProtoFile=io.open(pb_path.."legion.pb","rb");
    local legion=ProtoFile:read "*a"
    register(legion)
    legion=nil;
    io.close()

    ProtoFile=io.open(pb_path.."pokedex.pb","rb");
    local pokedex=ProtoFile:read "*a"
    register(pokedex)
    pokedex=nil;
    io.close()

    ProtoFile=io.open(pb_path.."ranking.pb","rb");
    local ranking=ProtoFile:read "*a"
    register(ranking)
    ranking=nil;
    io.close()

    ProtoFile=io.open(pb_path.."time_limit_activity.pb","rb");
    local time_limit_activity=ProtoFile:read "*a"
    register(time_limit_activity)
    time_limit_activity=nil;
    io.close()

    ProtoFile=io.open(pb_path.."service_goal.pb","rb");
    local service_goal=ProtoFile:read "*a"
    register(service_goal)
    service_goal=nil;
    io.close()

    ProtoFile=io.open(pb_path.."relation.pb","rb");
    local relation=ProtoFile:read "*a"
    register(relation)
    relation=nil;
    io.close()

    ProtoFile=io.open(pb_path.."skill.pb","rb");
	local skill=ProtoFile:read "*a"
    register(skill)
    skill=nil;
    io.close()
    
    ProtoFile=io.open(pb_path.."featherheart.pb","rb");
    local featherheart=ProtoFile:read "*a"
    register(featherheart)
    featherheart=nil;
    io.close()

    ProtoFile=io.open(pb_path.."phantom.pb","rb");
    local phantom=ProtoFile:read "*a"
    register(phantom)
    phantom=nil;
    io.close()

    ProtoFile=io.open(pb_path.."recycle.pb","rb");
    local recycle=ProtoFile:read "*a"
    register(recycle)
    recycle=nil;
    io.close()

    ProtoFile=io.open(pb_path.."welfare.pb","rb");
    local welfare=ProtoFile:read "*a"
    register(welfare)
    welfare=nil;
    io.close()

    ProtoFile=io.open(pb_path.."assist_fight.pb","rb");
    local assist_fight=ProtoFile:read "*a"
    register(assist_fight)
    assist_fight=nil;
    io.close()

    ProtoFile=io.open(pb_path.."pokedex.pb","rb");
    local pokedex=ProtoFile:read "*a"
    register(pokedex)
    pokedex=nil;
    io.close()

    ProtoFile=io.open(pb_path.."mission.pb","rb");
    local mission =ProtoFile:read "*a"
    register(mission)
    mission=nil;
    io.close()


    ProtoFile=io.open(pb_path.."title.pb","rb");
    local title=ProtoFile:read "*a"
    register(title)
    title=nil;
    io.close()

    ProtoFile=io.open(pb_path.."special_name.pb","rb");
    local special_name=ProtoFile:read "*a"
    register(special_name)
    special_name=nil;
    io.close()

    ProtoFile=io.open(pb_path.."goods.pb","rb");
    local goods=ProtoFile:read "*a"
    register(goods)
    goods=nil;
    io.close()
	
	ProtoFile=io.open(pb_path.."interaction.pb","rb");
    local interaction=ProtoFile:read "*a"
    register(interaction)
    interaction=nil;
    io.close()

    ProtoFile=io.open(pb_path.."dragontrip.pb","rb");
    local dragontrip=ProtoFile:read "*a"
    register(dragontrip)
    dragontrip=nil;
    io.close()

    ProtoFile=io.open(pb_path.."talent.pb","rb");
    local talent=ProtoFile:read "*a"
    register(talent)
    talent=nil;
    io.close()

    ProtoFile=io.open(pb_path.."server2dp.pb","rb");
    local serverws2dp=ProtoFile:read "*a"
    register(serverws2dp)
    serverws2dp=nil;
    io.close()

    ProtoFile=io.open(pb_path.."db.pb","rb");
    local cs2dp=ProtoFile:read "*a"
    register(cs2dp)
    cs2dp=nil;
    io.close()

    ProtoFile=io.open(pb_path.."st.pb","rb");
    local cs2dp=ProtoFile:read "*a"
    register(cs2dp)
    cs2dp=nil;
    io.close()

    ProtoFile=io.open(pb_path.."cs2dp.pb","rb");
    local cs2dp=ProtoFile:read "*a"
    register(cs2dp)
    cs2dp=nil;
    io.close()
	
	ProtoFile=io.open(pb_path.."dp2s.pb","rb");
    local dp2s=ProtoFile:read "*a"
    register(dp2s)
    dp2s=nil;
    io.close()

    ProtoFile=io.open(pb_path.."dp2cs.pb","rb");
    local cs2dp=ProtoFile:read "*a"
    register(cs2dp)
    cs2dp=nil;
    io.close()

    ProtoFile=io.open(pb_path.."ws2cs.pb","rb");
    local ws2cs=ProtoFile:read "*a"
    register(ws2cs)
    ws2cs=nil;
    io.close()

    ProtoFile=io.open(pb_path.."team.pb","rb");
    local team=ProtoFile:read "*a"
    register(team)
    team=nil;
    io.close()]]

    for i=1, #t_pb_name do
        local pb_name = t_pb_name[i]
        --print("------load ---pb_name", pb_name )
        ProtoFile=io.open(pb_path..pb_name,"rb");
        local t_file=ProtoFile:read "*a"
        register(t_file)
        t_file = nil;
        io.close()
        --print("------load end---", pb_name )
    end
end

ProtoMgr = proto_mgr

return proto_mgr;