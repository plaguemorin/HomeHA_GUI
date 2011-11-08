#ifndef SCRIPT_INTERNAL_H
#define SCRIPT_INTERNAL_H

#define PAGE "Page"
#define WIDGET "Widget"

class APage;
class AWidget;

int SCRIPT_LibraryGUI_Load(lua_State *);
int SCRIPT_LibraryWidget_Load(lua_State *);
int SCRIPT_LibraryPage_Load(lua_State *);

APage ** checkpage(lua_State *L, int index);

#endif // SCRIPT_INTERNAL_H
