#include "clientManager.h"
#include "chat.h"
#include "utils.h"
#include <string>


void clientManager::addClient(string nombre, int clientId){
	
	if(clients.find(nombre)==clients.end())
  {
	  clients[nombre]=clientId;
  } else 
  {
    // este cliente ya existe en la lista
    // puede ser un cliente distinto que se llame igual
    // o puede ser el mismo cliente incluso
  }
}


void clientManager::createTextMessage(vector<unsigned char> &buffer, DATA_STRUCT msg)
{
   msg.type = chat_message;
  if (msg.body == "exit()") msg.type = chat_closed;
  if (msg.end_user != "unknown") msg.type = chat_private;

  pack<msgType>(buffer, msg.type);

  pack(buffer, (int) msg.sender.size());
  packv(buffer, (unsigned char*)msg.sender.c_str(), (int) msg.sender.size());


  pack(buffer, (int) msg.body.size());
  packv(buffer, (unsigned char*)msg.body.c_str(), (int) msg.body.size());

  pack(buffer, (int) msg.end_user.size());
  packv(buffer, (unsigned char*)msg.end_user.c_str(), (int) msg.end_user.size());
}

void clientManager::unpackTextMessage(vector<unsigned char> &buffer, DATA_STRUCT &msg)
{

  int name_size = unpack<int>(buffer);
  msg.sender.resize(name_size);
  unpackv(buffer, (unsigned char*)msg.sender.c_str(), name_size);

  int msg_size = unpack<int>(buffer);
  msg.body.resize(msg_size);
  unpackv(buffer, (unsigned char*)msg.body.c_str(), msg_size);


  int reciever_size = unpack<int>(buffer);
  msg.end_user.resize(reciever_size);
  unpackv(buffer, (unsigned char*)msg.end_user.c_str(), reciever_size);
}

void clientManager::broadcastMessages(string nombre, string msg){
	
	vector<unsigned char> buffer;

  string end_user = "unknown";
  DATA_STRUCT my_struct(nombre, end_user, msg, chat_message);
  createTextMessage(buffer, my_struct);
  if (buffer.size() <= 0) return;

  for(auto &client:clients)
	{
		if(client.first != nombre) // No se lo reenv�o a quien lo envi�
		{
      sendMSG(client.second, buffer);
		}	
	}
}

void clientManager::sendToSpecificUser(DATA_STRUCT msg)
{
	vector<unsigned char> buffer;

  createTextMessage(buffer, msg);
  if (buffer.size() <= 0) return;

  for (auto &client : clients)
  {
    if (client.first == msg.end_user && msg.end_user != msg.sender)
    {
      sendMSG(client.second, buffer);
    }
  }
} 

void clientManager::atiendeCliente(int clientId)
{
	msgType tipoMsg = chat_message;
	std::vector<unsigned char> buffer;

  do
  {
    recvMSG(clientId, buffer);
    if (buffer.size() <= 0) continue;

    tipoMsg = unpack<msgType>(buffer);

		//switch por tipo mensaje
		switch(tipoMsg){
			case chat_message:
			{
				std::string nombre, msg, reciever;
	      DATA_STRUCT my_struct(nombre, reciever, msg, chat_message);	
        unpackTextMessage(buffer, my_struct); 

				std::cout << nombre << ":" << msg << "\n";
				addClient(nombre, clientId);
				broadcastMessages(nombre, msg);
			} break;
			case chat_closed:
			{
        // es de buena educacion despedirse....
        buffer.clear();
        string nombre = "server";
        string end_user = "unknown";
	      string msg = "closing connection";
        DATA_STRUCT my_struct(nombre, end_user, msg, chat_closed);	
        createTextMessage(buffer, my_struct);
        sendMSG(clientId, buffer);

        closeConnection(clientId);			
			} break;

      case chat_private:
      {
				std::string nombre, msg, reciever;
	      DATA_STRUCT my_struct(nombre, reciever, msg, chat_message);	
        unpackTextMessage(buffer, my_struct); 
        std::cout << "[Whisper from " <<  nombre << "]:" << msg << " [Sent to -> " << reciever << "]\n";
				addClient(nombre, clientId);
        sendToSpecificUser(my_struct);
      } break;

			default:{
				std::cout<<"ERROR: tipo de mensaje no valido\n";
			} break;
		}
	//mientras no sea tipo closed
	} while (tipoMsg != chat_closed);
}
