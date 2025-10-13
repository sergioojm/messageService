#pragma once

#include <string>

using namespace std;

typedef enum {
	chat_message = 0,
	chat_closed = 1,
  chat_private = 2,
  server_hello = 3, // se usa para la primera conexion entre cliente y servidor, de tal forma, que el cliente
                    // sea incluido en la lista de clientes sin necesidad que este escriba un mensaje
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

