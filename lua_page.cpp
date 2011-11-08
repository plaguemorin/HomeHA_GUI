#include <Qt/QtCore>
#include <QList>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "script_internal.h"
#include "mainwindow.h"
#include "apage.h"


/*-----------------------------------------------------------------------------------------*/

static QList<APage *> listOfPages;


static APage ** topage(lua_State *L, int index) {
    APage ** bar;
    bar = (APage **) lua_touserdata(L, index);
    if (bar == NULL ) luaL_typerror(L, index, PAGE);
    return bar;
}

APage ** checkpage(lua_State *L, int index) {
    APage ** bar;
    luaL_checktype(L, index, LUA_TUSERDATA);
    bar = (APage **) luaL_checkudata(L, index, PAGE);

    if (bar == NULL) {
        luaL_typerror(L, index, PAGE);
    }

    if (*bar == NULL) {
        luaL_typerror(L, index, PAGE);
    }

    return bar;
}

static APage ** pushpage(lua_State *L) {
    APage ** bar;
    bar = (APage **) lua_newuserdata(L, sizeof(APage*));
    luaL_getmetatable(L, PAGE);
    lua_setmetatable(L, -2);
    return bar;
}

static void putpage(lua_State *L, APage * node) {
    APage ** bar;
    bar = pushpage(L);
    *bar = node;
}

static void addpage(APage * widget) {
    listOfPages.append(widget);
}

static APage * findPageByName(const char * name) {
     for (int i = 0; i < listOfPages.size(); ++i) {
         APage * the = listOfPages.at(i);
         if (!strcmp(name, the->name())) {
             return the;
         }
     }

     return NULL;
}
/*-----------------------------------------------------------------------------------------*/

static int ScriptPage_New(lua_State *L) {
    const char * x;
    APage ** bar;

    x = luaL_optlstring(L, 1, NULL, NULL);
    bar = (APage**) pushpage(L);

    *bar = new APage();
    if (x) {
        (*bar)->setName(x);
    }

    addpage((*bar));
    return 1;
}

/*-----------------------------------------------------------------------------------------*/
static int ScriptPage__gc(lua_State *L) {
    printf("Trashing node %p\n", checkpage(L, 1));
    return 0;
}

static int ScriptPage__tostring(lua_State *L) {
    APage ** self;
    self = checkpage(L, 1);

    lua_pushfstring(L, "Page (%s)", (*self)->name());
    return 1;
}


static const luaL_Reg pageMethodsLib[] = {
    {"new", ScriptPage_New},
    { NULL, NULL }
};

static const luaL_Reg pageMetaLib[] = {
    { "__gc", ScriptPage__gc },
    { "__tostring", ScriptPage__tostring },
    { NULL, NULL }
};

int SCRIPT_LibraryPage_Load(lua_State * L) {
    /* create methods table, add it to the globals */
    luaL_openlib(L, PAGE, pageMethodsLib, 0);

    /* create metatable for the new object, and add it to the Lua registry */
    luaL_newmetatable(L, PAGE);

    /* fill metatable */
    luaL_openlib(L, 0, pageMetaLib, 0);
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
