from http.server import BaseHTTPRequestHandler,HTTPServer
import threading

data=0
flag=0

class RequestHandler_httpd(BaseHTTPRequestHandler):
	def do_GET(self):
		global data
		global flag
		data=self.requestline
		data = data[5:int(len(data)-9)]
		messagetosend=bytes('Welcome to Piserver',"utf")
		self.send_response(200)
		self.send_header('Content-Type','text/plain')
		self.send_header('Content-Length',len(messagetosend))
		self.end_headers()
		self.wfile.write(messagetosend)
		flag=1
		return

server_address_httpd=('192.168.43.154',8080)
httpd= HTTPServer(server_address_httpd,RequestHandler_httpd)
print('Starting the Server: ')
try:
	t1=threading.Thread(target=httpd.serve_forever)
	t1.start()
	while 1:
		if flag is 1:
			print(data)
			flag = 0
except:
	print("done")
