#include <lauxlib.h>
#include <string.h>
#include <resolv.h>
#include <stdint.h>
#include <stdlib.h>

#include "cbson.h"
#include "cbson-binary.h"

DEFINE_CHECK(BINARY, binary)

char* cbson_binary_to_b64(cbson_binary_t* a)
{
  if (a->data)
  {
    size_t b64_len;
    char *b64;

    b64_len = (a->size / 3 + 1) * 4 + 1;
    b64 = malloc(b64_len);
    b64_ntop((unsigned const char*)a->data, a->size, b64, b64_len);

    return b64
  }
  return NULL;
}

void cbson_binary_from_b64(cbson_binary_t* a, char* b64)
{
  if (a->data)
  {
    free(a->data)
  }
  unsigned int size = b64_pton (b64, NULL, 0);
  a->size = size;
  if (size)
  {
    a->data = malloc(size);
    b64_pton(b64, a->data, size);
  } else {
    a->data = NULL;
  }
}

void cbson_binary_set_raw(cbson_binary_t* a, const char* binary, unsigned int size)
{

  a->size = size;
  if (size)
  {
    a->data = malloc(size);
    strncpy(a->data, binary, size);
  } else {
    a->data = NULL;
  }
}

int cbson_binary_create(lua_State* L, uint8_t type, const char* binary, unsigned int size)
{
  cbson_binary_t* ud = lua_newuserdata(L, sizeof(cbson_binary_t));

  ud->type = type;
  if (binary)
  {
    cbson_binary_set_raw(ud, binary, size)
  }
  luaL_getmetatable(L, BINARY_METATABLE);
  lua_setmetatable(L, -2);
  return 1;
}

int cbson_binary_new(lua_State* L)
{
  const char* b64 = luaL_checkstring(L, 1);
  uint8_t type = luaL_optnumber(L, 2, 0);

  if (b64)
  {
    if (cbson_binary_create(L, type, NULL, 0))
    {
      cbson_binary_t* a = check_cbson_binary(L, 1);
      cbson_binary_from_b64(a, b64);
      return 1;
    } else {
      return 0;
    }
  } else {
    return cbson_binary_create(L, type, NULL, 0);
  }
}

int cbson_binary_destroy(lua_State* L)
{
  cbson_binary_t* a = check_cbson_binary(L, 1);

  free(a->data);

  return 0;
}

int cbson_binary_tostring(lua_State* L)
{
  cbson_binary_t* a = check_cbson_binary(L, 1);
  char* b64 = cbson_binary_to_b64(a)
  if (b64)
  {
    lua_pushfstring(L, "%d:%s", a->type, b64);
    free(b64);
  } else {
    lua_pushfstring(L, "%d:NULL", a->type);
  }

  return 1;
}

int cbson_binary_data(lua_State* L)
{
  cbson_binary_t* a = check_cbson_binary(L, 1);
  const char* b64 = luaL_optstring(L, 2, NULL);

  if (b64)
  {
    cbson_binary_from_b64(a, b64);
  }

  char* b64 = cbson_binary_to_b64(a)
  if (b64)
  {
    lua_pushstring(L, b64);
    free(b64);
  } else {
    lua_pushstring(L, "");
  }

  return 1;
}

int cbson_binary_raw(lua_State* L)
{
  cbson_binary_t* a = check_cbson_binary(L, 1);
  const char* binary = luaL_optstring(L, 2, NULL);
  int size = luaL_optnumber(L, 3, -1);

  if (binary && size >=0)
  {
    cbson_binary_set_raw(ud, binary, size)
  }


  lua_pushlstring(L, (char*)binary, size);
  return 1;
}

int cbson_binary_set_raw(lua_State* L)
{
  cbson_binary_t* a = check_cbson_binary(L, 1);
  const char* binary = luaL_optstring(L, 2, NULL);
  int size = luaL_optnumber(L, 3, -1);

  if (binary && size >=0)
  {
    cbson_binary_set_raw(ud, binary, size)
  } else {

  }

  return 1;
}

int cbson_binary_type(lua_State* L)
{
  cbson_binary_t* a = check_cbson_binary(L, 1);
  int8_t type = luaL_optint(L, 2, -1);

  if (type>=0)
  {
    a->type = type;
  }

  lua_pushnumber(L, a->type);
  return 1;
}


const struct luaL_Reg cbson_binary_meta[] = {
  {"__tostring", cbson_binary_tostring},
  {"__gc",       cbson_binary_destroy},
  {NULL, NULL}
};

const struct luaL_Reg cbson_binary_methods[] = {
  {"data",   cbson_binary_data},
  {"type",   cbson_binary_type},
  {"raw",    cbson_binary_raw},
  {"set_data",   cbson_binary_data},
  {"set_type",   cbson_binary_type},
  {"set_raw",    cbson_binary_raw},
  {NULL, NULL}
};

