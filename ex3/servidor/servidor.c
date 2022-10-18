﻿/*============================== Servidor basico UDP ==========================
  Este servidor UDP destina-se a mostrar os conteudos dos datagramas recebidos,
  bem como as respectivas origens, a reenvia-los às respectivas origens.
  O porto de escuta encontra-se definido pela constante SERV_UDP_PORT.
  Assume-se que as mensagens recebida sao cadeias de caracteres (ou seja,
  "strings").
  Mostra a origem da mensagem (IP + porto do cliente).
  Reenvia a mensagem ao cliente.
===============================================================================*/

#include <stdio.h>
#include <winsock.h>

#pragma comment(lib, "Ws2_32.lib")

#define SERV_UDP_PORT 6000

#define BUFFERSIZE 4096

void Abort(char* msg);

/*________________________________ main ________________________________________
*/

int main(int argc, char* argv[])
{
	SOCKET sockfd;
	int iResult, nbytes;
	int length_addr;
	struct sockaddr_in serv_addr, cli_addr;
	char buffer[BUFFERSIZE];
	WSADATA wsaData;

	/*=============== INICIA OS WINSOCKS ==============*/

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		getchar();
		exit(1);
	}

	/*============ CRIA O SOCKET PARA RECEPCAO/ENVIO DE DATAGRAMAS UDP ============*/

	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		Abort("Impossibilidade de abrir socket");

	/*=============== ASSOCIA O SOCKET AO  ENDERECO DE ESCUTA ===============*/

	/*Define que pretende receber datagramas vindos de qualquer interface de
	rede, no porto pretendido*/

	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET; /*Address Family: Internet*/
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /*Host TO Network Long*/
	serv_addr.sin_port = htons(SERV_UDP_PORT);  /*Host TO Network Short*/

	/*Associa o socket ao porto pretendido*/

	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
		Abort("Impossibilidade de registar-se para escuta");

	/*================ PASSA A ATENDER CLIENTES INTERACTIVAMENTE =============*/

	while (1) {

		fprintf(stderr, "<SER1>Esperando datagram...\n");

		length_addr = sizeof(cli_addr);
		nbytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cli_addr, &length_addr);

		if (nbytes == SOCKET_ERROR)
			Abort("Erro na recepcao de datagrams");

		buffer[nbytes] = '\0'; /*Termina a cadeia de caracteres recebidos com '\0'*/

		printf("\n<SER1>Mensagem recebida {%s}\n", buffer);


		//Enviar mensagem de volta ao cliente
		printf("\n<SER1>Vou reenviar a mensagem ao cliente\n");
		nbytes = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&cli_addr, sizeof(cli_addr));
		if (nbytes == SOCKET_ERROR) {
			printf("\n<SER1>Erro ao reenviar a mensagem ao cliente\n");
		}

	}

}


/*________________________________ Abort________________________________________
  Mostra uma mensagem de erro e o código associado ao ultimo erro com Winsocks.
  Termina a aplicacao com "exit status" a 1 (constante EXIT_FAILURE)
________________________________________________________________________________*/

void Abort(char* msg)
{
	fprintf(stderr, "<SERV>Erro fatal: <%s> (%d)\n", msg, WSAGetLastError());
	exit(EXIT_FAILURE);
}