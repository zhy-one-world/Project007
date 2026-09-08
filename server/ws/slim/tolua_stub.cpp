/* provides missing tolua binding entry for WS_SLIM_BUILD when tolua++.lib is absent */
struct lua_State;
int tolua_logic_open(lua_State*)
{
	return 0;
}
