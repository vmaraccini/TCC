//
//  Servidor-UDP.h
//  Lumiar-Final
//
//  Created by Victor Maraccini on 6/19/16.
//
//

#ifndef Servidor_UDP_h
#define Servidor_UDP_h

/*Mensagens que o servidor aceita
  [4]    [15]         [0-10]    // Tamanho dos campos
 ALTE <PARAMETRO> <NOVO VALOR>
 CONS <VALOR>
 CONS <PARAMETRO>
*/

//Comandos que o servidor aceita
#define COM_LEN             4
#define ALTERA_COM_STR      "ALTE"
#define CONSULTA_COM_STR    "CONS"

#include "../Comum.h"

#include <stdio.h>

void main_udp();

#endif /* Servidor_UDP_h */
