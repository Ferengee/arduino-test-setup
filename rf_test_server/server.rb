#!/usr/bin/env ruby
Thread.abort_on_exception = true
require 'socket'


##
# TODO: track if client is sending
# - calculate send duration based on clients speed
# - store data in servers message property until client finished sending
# - make sure message comunicates as invalid if two clients transmit at the same time
# - 
##

class Client
  def initialize (socket)
    @socket = socket
    @running = true
    @is_transmitting = false
    @is_receiving = true
    @message = nil
    @collision = false;
  end
  
  def run(server)
    while @running do
      if @socket.closed?
        @running = false
      else
        readCommand(server)
      end
      #server.broadcast(l, self)
      
    end
  end
  
  def is_receiving
    @is_receiving ? 1 : 0
  end
  
  def is_transmitting
    @is_transmitting ? 1 : 0
  end
  
  def has_message_available
    if(@message.nil?)
      return 0
    elsif(@collision)
      return 2
    else
      return 1
    end
  end
  
  def receive(data)
    @message = data;
  end
  
  def readCommand(server)
    type, data = @socket.recv(80).split(":")
    case type
    when 't'
      @socket.send(is_transmitting.to_s, 0)
    when 'r'
      @socket.send(is_receiving.to_s, 0)
    when 'a'
      @socket.send(has_message_available.to_s, 0)
    when 'g'
      @socket.send(@message.to_s, 0)
      @message = nil
    when 's'
      puts "#{type}:#{data.unpack('H*')}"
      server.broadcast(data, self)
    else
      puts("unknown command")
      puts([type, data,].join("-"))
    end
  end
  
  def close
    @running = false
    @socket.close
  end
  
end

class Server
  def initialize(port)
    @active_connections = []
    @port = port
  end
  

  def broadcast(str, sender)
    @active_connections.reject{|c| c == sender}.each do |client|
      client.receive(str)
    end
  end

  def addClient(client)
    @active_connections.push(client)
    puts "client added"
    client.run(self)
  end


  def removeClient(client)
    @active_connections.delete(client)
    puts "client removed"
    client.close
  end
  
  def run
    Socket.tcp_server_loop(@port) do |sock, client_addrinfo|
      Thread.new {
        begin
          client = Client.new(sock)
          addClient(client)
        ensure
          removeClient(client)
        end
      }  
    end
  end
end  

server = Server.new(4340)
server.run