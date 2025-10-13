#pragma once
#include "chat.h"
#include "utils.h"

using namespace std;

class clientManager{

		public:
		
			static inline map<string, int> clients;
			static inline bool salir=false;

			static void addClient(string nombre, int clientId);
			static void broadcastMessages(string nombre, string msg);
			static void atiendeCliente(int clientId);
 
      static void createTextMessage(vector<unsigned char> &buffer, DATA_STRUCT msg);
      static void unpackTextMessage(vector<unsigned char> &buffer, DATA_STRUCT &msg);
      static void sendToSpecificUser(DATA_STRUCT msg);

};
