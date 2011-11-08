#ifndef LUA_WRAPPER_H
#define LUA_WRAPPER_H

class AWidget;

void SCRIPT_StartWrapper();
bool SCRIPT_ExecuteInitial();
bool SCRIPT_LoadPage(const char *);

void SCRIPT_DoCallback_Click(AWidget *);

#endif // LUA_WRAPPER_H
