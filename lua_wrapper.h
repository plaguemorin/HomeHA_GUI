#ifndef LUA_WRAPPER_H
#define LUA_WRAPPER_H

class AWidget;

void SCRIPT_StartWrapper();
bool SCRIPT_ExecuteInitial();

void SCRIPT_DoCallback_Widget(AWidget *, int);
void SCRIPT_DoCallback_Message(int callback, char ** headerKeys, char ** headerValues, const char * body, size_t headerSize, size_t bodySize);

#endif // LUA_WRAPPER_H
