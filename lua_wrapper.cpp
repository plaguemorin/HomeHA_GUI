#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    SCRIPT_LibraryPage_Load(L);
    SCRIPT_LibraryImage_Load(L);
    SCRIPT_LibraryCore_Load(L);
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

void SCRIPT_DoCallback_Widget(AWidget * widget, int callback) {
    lua_rawgeti(L, LUA_REGISTRYINDEX, callback);
    putwidget(L, widget);
    lua_pcall(L, 1, 0, 0);
}

void SCRIPT_DoCallback_Message(int callback, char ** headerKeys, char ** headerValues, const char * body, size_t headerSize, size_t bodySize) {
    size_t i;
    lua_rawgeti(L, LUA_REGISTRYINDEX, callback);

    lua_newtable(L);
    for (i = 0; i <= headerSize - 1; i++) {
        lua_pushstring(L, headerKeys[i]);   /* Push the table index */
        lua_pushstring(L, headerValues[i]); /* Push the cell value */
        lua_settable(L, -3);
    }

    lua_pushlstring(L, body, bodySize);
    lua_pcall(L, 2, 0, 0);
}
