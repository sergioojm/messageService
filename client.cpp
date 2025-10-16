#include "utils.h"
#include <string>
#include <iostream>
#include <string>
#include "chat.h"
#include "clientManager.h" 
 
void encrypt(int key, string &msg)
{
  for (int i = 0; i < msg.size(); i++)
  {
    msg.at(i) = msg.at(i) + key;
  }
}

void decrpyt(int key, string &msg)
{
  for (int i = 0; i < msg.size(); i++)
  {
    msg.at(i) = msg.at(i) - key;
  }
}

void receiveTextFromServer(int serverId)
{
	vector<unsigned char> buffer;
	while(!clientManager::salir)
	{
		recvMSG(serverId,buffer);
    if (buffer.size() <= 0) continue;
		auto type=unpack<msgType>(buffer);


    switch (type)
    {
      case chat_message:
      {
			  string nombre,msg, recv;
	      DATA_STRUCT my_struct(nombre, recv, msg, chat_message);	
        clientManager::unpackTextMessage(buffer,my_struct);
			  cout<<nombre<<":"<<msg<<"\n";	 	 
      }break;
      
      case chat_closed:
      {
			  clientManager::salir=true;
      } break;
      
      case chat_private:
      {
			  string nombre,msg, recv;
	      DATA_STRUCT my_struct(nombre, recv, msg, chat_closed);	
			  clientManager::unpackTextMessage(buffer,my_struct);
        decrpyt(13, msg);          
			  cout<<"[Whisper from " <<nombre<<"]:"<<msg<<"\n";	 	    
      } break;
     
      case server_hello:
      {
          cout << "[Succesfully connected to server]" << endl;
      } break;

      default:
      {
        cout << "Woops, unexpected msg type" << endl;
      } break;
    }
		
	}	
}

 
int main(int argc,char** argv)
{
	//Iniciar conexion
	std::string nombre, msg;
	std::vector<unsigned char> buffer;

  string host = "127.0.0.1";
  if (argc > 1) {
     host = argv[1];
  }   

  cout << "Intentando conectar a " << host << "..." << endl;
	auto connection=initClient(host, 3000);
	
  if (connection.socket < 0) 
  {
    cout << "Failed to connect to " << host << ":3000" << endl;
    return 1; 
  }
	std::cout<<"Introduzca nombre de usuario\n";
	std::getline(std::cin, nombre);

  //lanzar hilo recepción de mensajes de servidor
	thread *th=new thread(receiveTextFromServer,connection.serverId);

  
  string end_user = "unknown";
  string msg_to_server = "hi there server!";
  
  DATA_STRUCT my_struct(nombre, end_user, msg_to_server, server_hello);
  clientManager::createTextMessage(buffer, my_struct);
	sendMSG(connection.serverId,buffer);

  buffer.clear();
	
  do
  {
		std::cout<<"Introduzca mensaje\n";
		std::getline(std::cin, msg);
		buffer.clear();
		if(msg!="exit()"){		
	

      if (msg == "/send")
      {
        string destinatario = "unknown"; 
        string new_msg;
        std::cout << "Destinatario -> ";
        std::getline(std::cin, destinatario);

        std::cout << "-> ";
        std::getline(std::cin, new_msg);
        if (new_msg == "exit()" || new_msg == "/send") new_msg = "Hi! Im stupid.";
     
        encrypt(13, new_msg);

        DATA_STRUCT my_struct(nombre, destinatario, new_msg, chat_private);
        clientManager::createTextMessage(buffer, my_struct);
      }
      else
      {
        string end_user = "unknown";
        DATA_STRUCT my_struct(nombre, end_user, msg, chat_message);
        clientManager::createTextMessage(buffer, my_struct);
      }
		}
		//recibir mensajes
		//mostrarlos
		else{
			//desconectar de server
			//empaquetar tipo salir 
			pack<msgType>(buffer, chat_closed);
			clientManager::salir = true;
		}
		//enviar 
			sendMSG(connection.serverId,buffer);
 
		
	}while(msg!="exit()");

	th->join(); // El hilo main espera aquí hasta que el hilo receptor termine.
	delete th; // Buena práctica para liberar la memoria.
	closeConnection(connection.serverId);
	
    return 0;
}
