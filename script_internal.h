#ifndef SCRIPT_INTERNAL_H
#define SCRIPT_INTERNAL_H

#define PAGE "Page"
#define WIDGET "Widget"
#define IMAGE "Image"

class APage;
class AImage;
class AWidget;

int SCRIPT_LibraryGUI_Load(lua_State *);
int SCRIPT_LibraryWidget_Load(lua_State *);
int SCRIPT_LibraryPage_Load(lua_State *);
int SCRIPT_LibraryImage_Load(lua_State *);
int SCRIPT_LibraryCore_Load(lua_State * );

void putwidget(lua_State *, AWidget *);

APage ** checkpage(lua_State *, int);

AImage ** checkimage(lua_State *, int);
void putimage(lua_State *, AImage *);

#endif // SCRIPT_INTERNAL_H
