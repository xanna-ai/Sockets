using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;
//using System.Threading.Tasks;

namespace SocketTcpServer
{
    class Program
    {
        static void write(string fullpath, string message)
        {
            using (StreamWriter sw = new StreamWriter(fullpath, true))
            {
                sw.WriteLine(DateTime.Now.ToString() + "    INFO    " + message);
            }
        }
        static int port = 23000; // порт для приема входящих запросов
        static string path = "serverlog.txt";
        static void Main(string[] args)
        {
            // получаем адреса для запуска сокета
            IPEndPoint ipPoint = new IPEndPoint(IPAddress.Parse("172.20.10.3"), port);

            // создаем сокет
            Socket listenSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            write(path, "Socket initialisation succesful!");
            write(path, "ServerIP:" + ipPoint);
            write(path, "Port:" + port);
            try
            {
                // связываем сокет с локальной точкой, по которой будем принимать данные
                listenSocket.Bind(ipPoint);
                write(path, "Socket Local Point connection succesful!");
                Console.WriteLine("Socket Local Point connection succesful!");
                // начинаем прослушивание
                write(path, "Server launched! Start listening...");
                listenSocket.Listen(10);

                Console.WriteLine("Сервер запущен. Ожидание подключений...");

                while (true)
                {
                    Socket handler = listenSocket.Accept();
                    write(path, "New query!");
                    // получаем сообщение
                    StringBuilder builder = new StringBuilder();
                    int bytes = 0; // количество полученных байтов
                    byte[] data = new byte[256]; // буфер для получаемых данных

                    do
                    {
                        write(path, "Receiving data...");
                        bytes = handler.Receive(data);
                        builder.Append(Encoding.UTF8.GetString(data, 0, bytes));
                    }
                    while (handler.Available > 0);
                    write(path, "Received query data: " + builder.ToString());
                    Console.WriteLine("Received query data: " + builder.ToString());
                    Console.WriteLine("Processing data");
                    write(path, "Processing data");
                    char letter;
                    Console.WriteLine(DateTime.Now.ToShortTimeString() + ": " + builder.ToString());
                    for (int i = 0; i < builder.Length / 2; i++)
                    {
                        if (i == (builder.Length - 1 - i))
                            break;
                        letter = builder[i];
                        builder[i] = builder[builder.Length - 1 - i];
                        builder[builder.Length - 1 - i] = letter;
                    }
                    builder.Append(" автор сервера: Манджиева Анна Романовна М30-324Б-19.");
                    Console.WriteLine("Processed data: " + builder.ToString());
                    Console.WriteLine("sending data...");
                    // отправляем ответ
                    string message = "ваше сообщение доставлено";
                    data = Encoding.UTF8.GetBytes(message);
                    handler.Send(data);
                    // закрываем сокет
                    write(path, "Datas been sent succesfully!");
                    Console.WriteLine("Datas been sent succesfully!");
                    handler.Shutdown(SocketShutdown.Both);
                    handler.Close();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
    }
}