#include <QtCore/QSettings>
#include "housekeepingobject.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "script_internal.h"

static int SCRIPT_CORE_Settings(lua_State * L) {
    QSettings settings;
    const char * group;
    const char * item;
    int n = lua_gettop(L);

    group = luaL_checklstring(L, 1, NULL);

    /* 2 Params == SET, Else get */
    if (n >= 2) {
        item = luaL_checklstring(L, 2, NULL);
        settings.setValue(group, item);
        return 0;
    }

    QVariant result = settings.value(group, "");
    lua_pushstring(L, result.toString().toLatin1());

    return 1;
}

static int SCRIPT_CORE_OnMessage(lua_State * L) {
    if (lua_isfunction(L, 1)) {
        HouseKeepingObject::getInstance()->setStompMessageCallback(luaL_ref(L, LUA_REGISTRYINDEX));
    } else {
        // CRASH
    }

    return 0;
}

static int SCRIPT_CORE_SendMessage(lua_State * L) {
    const char * destination = lua_tostring(L, 1);
    const char * body = lua_tostring(L, 2);

    HouseKeepingObject::getInstance()->sendMessage(destination, body);
    return 0;
}

static int SCRIPT_CORE_AddActivity(lua_State * L) {
    return 0;
}

static const luaL_Reg coreLIB[]  = {
    { "settings", SCRIPT_CORE_Settings },
    { "onMessage", SCRIPT_CORE_OnMessage },
    { "sendMessage", SCRIPT_CORE_SendMessage },
    { "addActivity", SCRIPT_CORE_AddActivity },
    { NULL, NULL }
};

int SCRIPT_LibraryCore_Load(lua_State * L) {
    luaL_register(L, "Core", coreLIB);

    return 1;
}
