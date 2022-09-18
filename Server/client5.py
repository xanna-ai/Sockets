import socket
import logging

log = logging.getLogger("exampleApp")
log.setLevel(logging.INFO)
    # create the logging file handler
fh = logging.FileHandler("connection.log")
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
fh.setFormatter(formatter)
    # add handler to logger object
log.addHandler(fh)
sock = socket.socket()
log.info('Socket has been created!')
domain='172.17.132.89'
log.info('Server domain name: '+ domain)
port=5050
log.info('port: '+str(port)+'\n')
sock.connect((domain, port))
log.info('Server connection succesful!')
mystring='Манджиева Анна романовна М30-324Б-19'
byteString=mystring.encode('utf-8')
log.info('message query: '+mystring)
sock.send(byteString)
log.info('query has been sent!')
log.info('Retrieving data...')
data = sock.recv(1040)
log.info('Retrieving data succesful!')
log.info('Retrieved data: '+data.decode('utf-8'))
sock.close()
log.info('Socket closed!')
print(data.decode('utf-8'))
input()
