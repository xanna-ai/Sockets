#include <iostream>
#include <sstream>
#include <string>
#include <time.h>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>

// Для корректной работы нужна линковка с библиотекой Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

// Размер буферов
const int MAXLEN = 1024;
// IP адрес сервера
char serverAddress[1024];
// Порт сервера
unsigned int serverPort;
// Сообщение для сервера
const char* clientMessage = "Манджиева Анна Романовна М30-324Б-19";

// Функция отправки HTTP запроса и получения HTTP ответа
// sockfd - дескриптор сокета 
/*
void SendHTTPRequest(int sockfd)
{
	//HTTP GET
	//std::string get_http = "GET / HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";
	std::stringstream get_http;
	get_http << "GET /index.html HTTP/1.1\r\nHost: "
		<< url
		<< "\r\nConnection: close\r\n\r\n";

	// Отправляем запрос
	send(sockfd, get_http.str().c_str(), strlen(get_http.str().c_str()), 0);

	// Кол-во полученных байт
	int nread;
	// Буфер для получения ответа
	char buf[MAXLEN];

	while ((nread = recv(sockfd, buf, sizeof(buf), 0)) > 0)
	{
		printf("%s", buf);
	}
}*/

int main()
{
	// Получаем адрес и порт сервера
	FILE* settings;
	if ((settings = fopen("serverinfo.txt", "r")) == NULL) {
		printf("Cannot open settings file.\n");
		exit(1);
	}

	fgets(serverAddress, 1024, settings);
	serverAddress[strlen(serverAddress) - 1] = '\0';
	char bufport[1024] = { 0 };
	fgets(bufport, 1024, settings);
	serverPort = atoi(bufport);
	fclose(settings);

	// Открытие файла логгирования
	FILE* logFile;
	if ((logFile = fopen("log.txt", "a")) == NULL) {
		printf("Cannot open log file.\n");
		exit(1);
	}

	// Служебная структура для хранения информации о реализации WindowsSocket
	WSADATA wsadata;

	int result = WSAStartup(MAKEWORD(2, 2), &wsadata);

	// Если произошла ошибка
	if (result != 0)
	{
		printf("WSAStartup failed: %s\n", result);
		return result;
	}

	// Дескриптор сокета клиента
	int sockfd;
	struct sockaddr_in servaddr;

	// Создаем сокет
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// Если возникла ошибка при создании
	if (sockfd == INVALID_SOCKET)
	{
		printf("INVALID SOCKET: %s\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Заполнение структуры адреса сокета
	ZeroMemory(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(serverPort);
	inet_pton(AF_INET, serverAddress, &servaddr.sin_addr);

	// Соединение с сервером
	result = connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));

	if (result != 0)
	{
		printf("Ошибка подключения к серверу: %s\n", serverAddress);
		closesocket(sockfd);
		return -1;
	}

	// задержка
	Sleep(2000);

	time_t tt;
	struct tm* ptm;
	char buf[BUFSIZ];

	// Время подключения
	tt = time(NULL);
	ptm = localtime(&tt);
	strftime(buf, BUFSIZ, "%d-%m-%Y", ptm);

	fprintf(logFile, "\n%s", asctime(ptm));

	// Адрес сервера(лог)
	fprintf(logFile, "Cоединение с сервером: %s\n", serverAddress);


	// Отправляем сообщение серверу
	send(sockfd, clientMessage, strlen(clientMessage), 0);

	// Время отправки сообщения
	tt = time(NULL);
	ptm = localtime(&tt);
	strftime(buf, BUFSIZ, "%d-%m-%Y", ptm);

	fprintf(logFile, "\n%s", asctime(ptm));

	// Отправленное сообщение(лог)
	fprintf(logFile, "Введите сообщение: %s\n", clientMessage);

	// Кол-во полученных байт
	int nread;
	// Буфер для получения ответа
	char bufAnswer[MAXLEN];

	// Ждем ответ от сервера
	nread = recv(sockfd, bufAnswer, sizeof(bufAnswer), 0);
	bufAnswer[nread] = '\0';

	// Время получения ответа
	tt = time(NULL);
	ptm = localtime(&tt);
	strftime(buf, BUFSIZ, "%d-%m-%Y", ptm);

	fprintf(logFile, "\n%s", asctime(ptm));

	fprintf(logFile, "Ответ от сервера: %s\n", bufAnswer);

	fclose(logFile);
	closesocket(sockfd);
	WSACleanup();
}

