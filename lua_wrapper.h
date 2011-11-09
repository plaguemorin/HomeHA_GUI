#ifndef LUA_WRAPPER_H
#define LUA_WRAPPER_H

class AWidget;

void SCRIPT_StartWrapper();
bool SCRIPT_ExecuteInitial();

void SCRIPT_DoCallback_Widget(AWidget *, int);

#endif // LUA_WRAPPER_H
