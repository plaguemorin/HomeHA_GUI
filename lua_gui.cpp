#include <Qt/QtCore>
#include "mainwindow.h"

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
static int SCRIPT_GUI_BackgroundColor(lua_State * L) {
    MainWindow * main;
    float r, g, b;

    r = luaL_checknumber(L, 1);
    g = luaL_checknumber(L, 2);
    b = luaL_checknumber(L, 3);

    main = MainWindow::getInstance();
    QPalette pal = main->palette();
    pal.setColor(main->backgroundRole(), QColor(r * 255.0, g * 255.0, b * 255.0));
    main->setPalette(pal);

    return 0;
}

static int SCRIPT_GUI_BackgroundImage(lua_State * L) {
    MainWindow * main;
    const char * imagePath;

    imagePath = luaL_checklstring(L, 1, NULL);

    main = MainWindow::getInstance();
    QPalette pal = main->palette();
    pal.setBrush(main->backgroundRole(), QBrush(QImage(imagePath)));
    main->setPalette(pal);

    return 0;
}

static int SCRIPT_GUI_Version(lua_State * L) {
    lua_pushnumber(L, 1);
    return 1;
}

static const luaL_Reg guiLIB[]  = {
    { "backgroundImage", SCRIPT_GUI_BackgroundImage },
    { "backgroundColor", SCRIPT_GUI_BackgroundColor },
    { "version", SCRIPT_GUI_Version },
    { NULL, NULL }
};

int SCRIPT_LibraryGUI_Load(lua_State * L) {
    luaL_register(L, "GUI", guiLIB);

    return 1;
}
