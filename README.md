# Sockets
Client server app
# 1. программf клиент, работающая на сокетах  должена хранить лог файл соединений. В лог файле хранится время подключения и адрес сервера, время отправки сообщения и само сообщение, время получения сообщения и само сообщение. При подключении к серверу клиент через определенный промежуток времени (задается разработчиком клиента) отправляет сообщение серверу
# программf сервер, который ожидает строку от клиента. Через промежуток времени (моделирование работы сервера), он отправляет в ответ эту же строку, только зеркально отраженную и добавляет к сообщению ФИО, написавшего сервер.