#include <Qt/QtCore>
#include "housekeepingobject.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "script_internal.h"

/* ******************************************************************
   ** Wrapper for GUI singleton
   ******************************************************************
   */

static int SCRIPT_GUI_Version(lua_State * L) {
    lua_pushnumber(L, 1);
    return 1;
}

static const luaL_Reg guiLIB[]  = {
    { "version", SCRIPT_GUI_Version },
    { NULL, NULL }
};

int SCRIPT_LibraryGUI_Load(lua_State * L) {
    luaL_register(L, "GUI", guiLIB);

    return 1;
}
