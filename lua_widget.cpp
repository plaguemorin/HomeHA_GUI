#include <Qt/QtCore>
#include <QList>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "script_internal.h"
#include "mainwindow.h"
#include "awidget.h"

static QList<AWidget *> listOfWidgets;

static AWidget ** towidget(lua_State *L, int index) {
    AWidget ** bar;
    bar = (AWidget **) lua_touserdata(L, index);
    if (bar == NULL ) luaL_typerror(L, index, WIDGET);
    return bar;
}

static AWidget ** checkwidget(lua_State *L, int index) {
    AWidget ** bar;
    luaL_checktype(L, index, LUA_TUSERDATA);
    bar = (AWidget **) luaL_checkudata(L, index, WIDGET);

    if (bar == NULL) {
        luaL_typerror(L, index, WIDGET);
    }

    if (*bar == NULL) {
        luaL_typerror(L, index, WIDGET);
    }

    return bar;
}

static AWidget ** pushwidget(lua_State *L) {
    AWidget ** bar;
    bar = (AWidget **) lua_newuserdata(L, sizeof(AWidget*));
    luaL_getmetatable(L, WIDGET);
    lua_setmetatable(L, -2);
    return bar;
}

void putwidget(lua_State *L, AWidget * node) {
    AWidget ** bar;
    bar = pushwidget(L);
    *bar = node;
}

static void addwidget(AWidget * widget) {
    listOfWidgets.append(widget);
}

static AWidget * findWidgetByName(const char * name) {
     for (int i = 0; i < listOfWidgets.size(); ++i) {
         AWidget * the = listOfWidgets.at(i);
         if (!strcmp(name, the->name())) {
             return the;
         }
     }

     return NULL;
}

/*-----------------------------------------------------------------------------------------*/
static int ScriptWidget_ByName(lua_State *L) {
    const char * x;
    AWidget * bar;

    x = luaL_checklstring(L, 1, NULL);
    bar = findWidgetByName(x);

    if (bar) {
        putwidget(L, bar);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

static int ScriptWidget_New(lua_State *L) {
    const char * x;
    AWidget ** bar;

    x = luaL_optlstring(L, 1, NULL, NULL);
    bar = (AWidget**) pushwidget(L);

    *bar = new AWidget();
    if (x) {
        (*bar)->setName(x);
    }

    addwidget(*bar);

    return 1;
}

static int ScriptWidget_PageAttach(lua_State *L) {
    AWidget ** self;
    APage ** page;

    self = checkwidget(L, 1);
    page = checkpage(L, 2);

    (*self)->setParent((QWidget *) *page);

    return 0;
}

static int ScriptWidget_Size(lua_State *L) {
    AWidget ** self;
    int w;

    self = checkwidget(L, 1);
    w = luaL_optinteger(L, 2, -1);

    if (w == -1) {
        lua_pushnumber(L, (*self)->width());
        lua_pushnumber(L, (*self)->height());

        return 2;
    } else {
        int h = luaL_checkinteger(L, 3);

        (*self)->setSize(w, h);

        return 0;
    }
}

static int ScriptWidget_Name(lua_State *L) {
    AWidget ** self;
    self = checkwidget(L, 1);

    lua_pushfstring(L, "%s", (*self)->name());

    return 1;
}

static int ScriptWidget_Label(lua_State *L) {
    AWidget ** self;
    const char * x;
    size_t size;

    self = checkwidget(L, 1);
    x = luaL_optlstring(L, 2, NULL, &size);

    if (size > 0 && x != NULL) {
        (*self)->setLabel(x);
        return 0;
    }

    lua_pushfstring(L, "%s", (*self)->label());
    return 1;
}

static int ScriptWidget_Position(lua_State *L) {
    AWidget ** self;
    int x;

    self = checkwidget(L, 1);
    x = luaL_optinteger(L, 2, -1);

    if (x == -1) {
        lua_pushnumber(L, (*self)->x());
        lua_pushnumber(L, (*self)->y());

        return 2;
    } else {
        int y = luaL_checkinteger(L, 3);

        (*self)->setPosition(x, y);

        return 0;
    }
}

static int ScriptWidget_Hide(lua_State *L) {
    AWidget ** self;
    self = checkwidget(L, 1);

    (*self)->setVisibility(false);

    return 0;
}

static int ScriptWidget_Show(lua_State *L) {
    AWidget ** self;
    self = checkwidget(L, 1);

    (*self)->setVisibility(true);
    return 0;
}

static int ScriptWidget_BackgroundColor(lua_State *L) {
    AWidget ** self;
    float r, g, b;

    self = checkwidget(L, 1);
    r = luaL_optnumber(L, 2, -1);

    if (r != -1) {
        g = luaL_checknumber(L, 3);
        b = luaL_checknumber(L, 4);

        (*self)->setBackgroundColor(r * 255.0, g * 255.0, b * 255.0);
        return 0;
    } else {

        return 0;
    }
}

static int ScriptWidget_Image(lua_State *L) {
    AWidget ** self;
    AImage ** img;
    self = checkwidget(L, 1);

    if (lua_isuserdata(L, 2)) {
        img = checkimage(L, 2);

        (*self)->setImage(*img);
        return 0;
    } else {
        putimage(L, (*self)->image());

        return 1;
    }

}

static int ScriptWidget_OnClick(lua_State *L) {
    AWidget ** self;
    self = checkwidget(L, 1);

    if (lua_isfunction(L, 2)) {
        (*self)->set_callback_click(luaL_ref(L, LUA_REGISTRYINDEX));
    } else {
        // CRASH
    }

    return 0;
}

static int ScriptWidget_Delete(lua_State *L) {
    AWidget ** self;
    self = checkwidget(L, 1);

    (*self)->setParent(NULL);
    delete (*self);

    return 0;
}

/*-----------------------------------------------------------------------------------------*/
static int ScriptWidget__gc(lua_State *L) {
    printf("Trashing node %p\n", checkwidget(L, 1));
    return 0;
}

static int ScriptWidget__tostring(lua_State *L) {
    AWidget ** self;
    self = checkwidget(L, 1);

    lua_pushfstring(L, "Widget (%s)", (*self)->name());
    return 1;
}

static const luaL_Reg widgetMethodsLib[] = {
    { "retreive", ScriptWidget_ByName },
    { "new", ScriptWidget_New },
    { "destory", ScriptWidget_Delete },

    { "attachToPage", ScriptWidget_PageAttach },

    { "name", ScriptWidget_Name },
    { "size", ScriptWidget_Size },
    { "position", ScriptWidget_Position },
    { "label", ScriptWidget_Label },
    { "image", ScriptWidget_Image },
    { "background", ScriptWidget_BackgroundColor },

    { "hide", ScriptWidget_Hide },
    { "show", ScriptWidget_Show },

    { "onClick", ScriptWidget_OnClick },
    { NULL, NULL }
};

static const luaL_Reg widgetMetaLib[] = {
    { "__gc", ScriptWidget__gc },
    { "__tostring", ScriptWidget__tostring },
    { NULL, NULL }
};

int SCRIPT_LibraryWidget_Load(lua_State * L) {
    /* create methods table, add it to the globals */
    luaL_openlib(L, WIDGET, widgetMethodsLib, 0);

    /* create metatable for the new object, and add it to the Lua registry */
    luaL_newmetatable(L, WIDGET);

    /* fill metatable */
    luaL_openlib(L, 0, widgetMetaLib, 0);
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

