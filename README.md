# messageService

Un sistema de mensajería simple en C++ con soporte para mensajes públicos (broadcast) y mensajes privados.
El proyecto utiliza un modelo cliente-servidor, donde el servidor recibe los mensajes de un cliente y los reenvía según corresponda.

Realizado para la asignatura de Sistemas Distribuidos en U-Tad.


# Características
- Envío de mensajes públicos a todos los clientes conectados.
- Envío de mensajes privados a usuarios específicos con el comando "/send" 
- Soporte para desconexión limpia (exit() o mensajes de cierre).
