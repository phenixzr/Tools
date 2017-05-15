// Include lib lua
#ifndef LUA_BRIDGE_WRAPPER
#define LUA_BRIDGE_WRAPPER

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "LuaBridge\LuaBridge.h"
#include <unordered_map>

namespace tools
{
	class LuaBridgeWrapper
	{
	public:

		LuaBridgeWrapper(const char * file)
			: lState( luaL_newstate() )
		{
			luaL_dofile(lState, file);
			luaL_openlibs(lState);
			lua_pcall(lState, 0, 0, 0);
		}

		void init(const char * file)
		{
			luaL_dofile(lState, file);
			luaL_openlibs(lState);
			lua_pcall(lState, 0, 0, 0);
		}

		template <typename T>
		T getValue(const char* key) const
		{
			return luabridge::getGlobal(lState, key).cast<T>();
		}

	private:
		lua_State* lState;
	}; 
}

#endif // LUA_BRIDGE_WRAPPER