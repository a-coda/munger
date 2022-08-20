// Build runes:
// gcc munge.cpp -Wall -Wl,-Bstatic -lstdc++ -I lua-5.2.3_Win64_dllw4_lib/include/ -L lua-5.2.3_Win64_dllw4_lib -llua52 -o munge
 
#include <iostream>
#include <string>
 
extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
 
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
 
lua_State* L;
 
string create_definition (const string& option, const string& expression)
{
  if (option == "-e") 
  {
    return "function munge (_) return " + expression + " or '' end";
  }
  else if (option == "-s" )
  {
    return "function munge (_) " + expression + " end";
  }
  else
  {
    cerr << "unknown option: " << option << endl;
    throw;
  }
}
 
void lua_define_munge (const string& option, const string& expression)
{
  string definition = create_definition(option, expression);
  int err = luaL_loadstring(L, definition.c_str());
  if (err)
  {
    cerr << lua_tostring(L,-1);
    throw;
  }
  lua_call(L, 0, 0);
}
 
string lua_call_munge (const string& line)
{
  lua_getglobal(L, "munge");
  lua_pushstring(L, line.c_str());
  lua_call(L, 1, 1);
  string result = lua_tostring(L, -1);
  lua_pop(L, 1);
  
  return result;
}
 
int main(int argc, char *argv[])
{
  L = luaL_newstate();
  luaL_openlibs(L);
 
  if (argc > 2)
  {
    lua_define_munge(argv[1], argv[2]);
  }
  else if (argc > 1)
  {
    lua_define_munge("-e", argv[1]);
  }
 
  for (string sLine; getline(cin, sLine); )
  {
    if (argc > 1)
    {
      sLine = lua_call_munge(sLine);
    }
    cout << sLine << endl;
  }
 
  lua_close(L);
}
