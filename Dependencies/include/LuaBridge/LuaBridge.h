#ifndef LUABRIDGE_LUABRIDGE_HEADER
#define LUABRIDGE_LUABRIDGE_HEADER

// All #include dependencies are listed here
// instead of in the individual header files.
//
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>

#define LUABRIDGE_MAJOR_VERSION 2
#define LUABRIDGE_MINOR_VERSION 0
#define LUABRIDGE_VERSION 200

namespace luabridge
{

// Forward declaration
//
template <class T>
struct Stack;

#include "detail/LuaHelpers.h"
#include "detail/TypeTraits.h"
#include "detail/TypeList.h"
#include "detail/FuncTraits.h"
#include "detail/Constructor.h"
#include "detail/Stack.h"
#include "detail/ClassInfo.h"

class LuaRef;

#include "detail/LuaException.h"
#include "detail/LuaRef.h"
#include "detail/Iterator.h"

//------------------------------------------------------------------------------
/**
    security options.
*/
class Security
{
public:
  static bool hideMetatables ()
  {
    return getSettings().hideMetatables;
  }

  static void setHideMetatables (bool shouldHide)
  {
    getSettings().hideMetatables = shouldHide;
  }

private:
  struct Settings
  {
    Settings () : hideMetatables (true)
    {
    }

    bool hideMetatables;
  };

  static Settings& getSettings ()
  {
    static Settings settings;
    return settings;
  }
};

#include "detail/Userdata.h"
#include "detail/CFunctions.h"
#include "detail/Namespace.h"

//------------------------------------------------------------------------------
/**
    Push an object onto the Lua stack.
*/
template <class T>
inline void push (lua_State* L, T t)
{
  Stack <T>::push (L, t);
}

//------------------------------------------------------------------------------
/**
  Set a global value in the lua_State.

  @note This works on any type specialized by `Stack`, including `LuaRef` and
        its table proxies.
*/
template <class T>
inline void setGlobal (lua_State* L, T t, char const* name)
{
  push (L, t);
  lua_setglobal (L, name);
}

//------------------------------------------------------------------------------
/**
  Change whether or not metatables are hidden (on by default).
*/
inline void setHideMetatables (bool shouldHide)
{
  Security::setHideMetatables (shouldHide);
}

}

#endif
