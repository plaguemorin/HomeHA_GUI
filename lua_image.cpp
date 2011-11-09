#include <Qt/QtCore>
#include <QImage>
#include "mainwindow.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "script_internal.h"
#include "aimage.h"

static AImage ** toimage(lua_State *L, int index) {
    AImage ** bar;
    bar = (AImage **) lua_touserdata(L, index);
    if (bar == NULL ) luaL_typerror(L, index, IMAGE);
    return bar;
}

AImage ** checkimage(lua_State *L, int index) {
    AImage ** bar;
    luaL_checktype(L, index, LUA_TUSERDATA);
    bar = (AImage **) luaL_checkudata(L, index, IMAGE);

    if (bar == NULL) {
        luaL_typerror(L, index, IMAGE);
    }

    if (*bar == NULL) {
        luaL_typerror(L, index, IMAGE);
    }

    return bar;
}

static AImage ** pushimage(lua_State *L) {
    AImage ** bar;
    bar = (AImage **) lua_newuserdata(L, sizeof(AImage*));
    luaL_getmetatable(L, IMAGE);
    lua_setmetatable(L, -2);
    return bar;
}

void putimage(lua_State *L, AImage * node) {
    AImage ** bar;
    bar = pushimage(L);
    *bar = node;
}

/*-----------------------------------------------------------------------------------------*/

int ScriptImage_Load(lua_State *L) {
    const char * x;
    AImage ** bar;

    x = luaL_checklstring(L, 1, NULL);
    bar = (AImage**) pushimage(L);

    *bar = new AImage();
    (*bar)->loadImage(x);

    return 1;
}

static int ScriptImage_Copy(lua_State *L) {
    AImage * bar;
    AImage ** self;

    self = checkimage(L, 1);
    bar = (*self)->copy();
    putimage(L, bar);

    return 1;
}

static int ScriptImage_Tint(lua_State *L) {
    AImage ** self;

    self = checkimage(L, 1);

    (*self)->tint(
                luaL_checknumber(L, 2) * 255.0,
                luaL_checknumber(L, 3) * 255.0,
                luaL_checknumber(L, 4) * 255.0,
                luaL_checknumber(L, 5) * 255.0
                );

    return 0;
}

/*-----------------------------------------------------------------------------------------*/
static int ScriptImage__gc(lua_State *L) {
    printf("Trashing node %p\n", checkimage(L, 1));
    return 0;
}

static int ScriptImage__tostring(lua_State *L) {
    AImage ** self;
    self = checkimage(L, 1);

    lua_pushfstring(L, "Image (%s)", (*self)->name());
    return 1;
}

static const luaL_Reg imageMethodsLib[] = {
    { "load", ScriptImage_Load },
    { "clone", ScriptImage_Copy },
    { "tint", ScriptImage_Tint },
    { NULL, NULL }
};

static const luaL_Reg imageMetaLib[] = {
    { "__gc", ScriptImage__gc },
    { "__tostring", ScriptImage__tostring },
    { NULL, NULL }
};

int SCRIPT_LibraryImage_Load(lua_State *L) {
    /* create methods table, add it to the globals */
    luaL_openlib(L, IMAGE, imageMethodsLib, 0);

    /* create metatable for the new object, and add it to the Lua registry */
    luaL_newmetatable(L, IMAGE);

    /* fill metatable */
    luaL_openlib(L, 0, imageMetaLib, 0);
    lua_pushliteral(L, "__index");

    /* dup methods table*/
    lua_pushvalue(L, -3);

    /* metatable.__index = methods */
    lua_rawset(L, -3);

    /* dup methods table*/
    lua_pushliteral(L, "__metatable");

    /* hide metatable:     metatable.__metatable = methods */
    lua_pushvalue(L, -3);

    /* drop metatable */
    lua_rawset(L, -3);

    /* return methods on the stack */
    lua_pop(L, 1);

    /* After the new object register the methods are still on the stack, remove them. */
    lua_pop(L, 1);

    return 1;
}
