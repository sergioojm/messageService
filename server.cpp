#include "utils.h"
#include <iostream>
#include <string>
#include <thread>
#include <list>
#include "chat.h"
#include "clientManager.h"


int main(int argc, char** argv)
{
	//init server
	int serverSocketID = initServer(3000);
	std::vector<unsigned char> buffer;
	//esperar conexion


  std::cout << "Esperando conexión en puerto 3000...\n";
	while(1){
		while(!checkClient()) usleep(100);

		std::cout << "Cliente conectado\n";
		int clientId = getLastClientID();
		std::thread *th = new std::thread(clientManager::atiendeCliente, clientId);
		th->detach();
	}
	
	//cerrar
	close(serverSocketID);
    return 0; 
}
