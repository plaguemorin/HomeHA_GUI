#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Qt/QtCore>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "script_internal.h"
#include "awidget.h"

static lua_State *L;

static void report_errors(lua_State *L, int status) {
    if (status != 0) {
        printf(" %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

void SCRIPT_StartWrapper() {
    /* Make new LUA State */
    L = luaL_newstate();

    /* Load Lua libraries */
    luaL_openlibs(L);

    /* Register Custom libraries */
    SCRIPT_LibraryGUI_Load(L);
    SCRIPT_LibraryWidget_Load(L);
}

bool SCRIPT_ExecuteInitial() {
    int s;

    /* Execute Main Script */
    s = luaL_loadfile(L, "main.lua");

    /* No errors ? Execute it */
    if (s == 0) {
        s = lua_pcall(L, 0, LUA_MULTRET, 0);
    }

    /* Report any errors at this stage */
    report_errors(L, s);

    return (s != 0) ? false : true;
}

bool SCRIPT_LoadPage(const char * pageName) {
    qDebug() << "Loading page" << pageName;

    return true;
}

void SCRIPT_DoCallback_Click(AWidget * widget) {
    lua_rawgeti(L, LUA_REGISTRYINDEX, widget->get_callback_click());
    lua_pushfstring(L, "%s", widget->name());
    lua_pcall(L, 1, 0, 0);
}
