require 'rubygems'
require 'ffi-rzmq'
require 'socket'

context = ZMQ::Context.new(1)

requester = context.socket(ZMQ::REQ)
requester.connect("tcp://localhost:5559")
 
webserver = TCPServer.new('localhost', 8080)
while (session = webserver.accept)
#  session.print "HTTP/1.1 200/OK\r\n"
#  session.print "Access-Control-Allow-Origin: * \r\n"
#  session.print "Content-type:text/html\r\n\r\n"
  request = ""
  line = session.gets 
  content_length = 0
  
  while line.chomp != "" do
    line = session.gets
    cl_line = line.match(/^Content-Length: (\d+)/)
    if cl_line
      content_length =  cl_line[1].to_i
    end
    request += line
  end
  
  if content_length > 0
    request += session.read(content_length)
  end
  puts "request:"
  puts request.tr "\r", '|'
  requester.send_string request

  reply = requester.recv_string(string = '')
  session.print string
  session.close
end
