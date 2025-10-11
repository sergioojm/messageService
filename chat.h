#pragma once

#include <string>

using namespace std;

typedef enum {
	chat_message = 0,
	chat_closed = 1,
  chat_private = 2,
} msgType;

typedef struct DATA_STRUCT
{
  string& sender;
  string& end_user;
  string& body;
  msgType type;

  // asi puedo inicializar el struct en vez de
  // DATA_STRUCT struct
  // struct.sender = ....
  // mas comodo asi
  // DATA_STRUCT struct("sender", "florentino", "cristiano mejor que messi", chat_private)


  DATA_STRUCT(string& s, string& e, string& b, msgType t)
    : sender(s), end_user(e), body(b), type(t) {}

} DATA_STRUCT;

