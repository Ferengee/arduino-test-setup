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
class Object
  def to_bin_s
    (self ? 1 : 0).to_s
  end
end
class Client
  attr_reader :out_message, :sending_bits, :bautrate
  attr_writer :out_message, :sending_bits, :collision
  
  def initialize (socket, server)
    @socket = socket
    @server = server
    
    @running = true

    @in_message = nil
    @out_message = nil
    @sending_bits = 0
    @receiving_bits = 0
    @collision = false
    @bautrate = 2000
  end
  
  def run()
    while @running do
      if @socket.closed?
        @running = false
      else
        readCommand()
      end
      
    end
  end
  
  def is_receiving?
    @server.is_anyone_transmitting?(self)
  end
  
  def is_transmitting?
    @sending_bits > 0
  end
  
  def has_message_available
    if(@in_message.nil?)
      return 0
    elsif(@collision)
      return 2
    else
      return 1
    end
  end
  
  def receive(data)
    @receiving_bits = 80 * 8
    @in_message = data;
  end
  
  def readCommand()
    type, data = @socket.recv(80 + 2).split(":")
    case type
    when nil
      close
    when 'b'
      @bautrate = data.to_i
      puts "Set bautrate to: #{@bautrate}"
    when 't'
      @socket.send(is_transmitting?.to_bin_s, 0)
    when 'r'
      @socket.send(is_receiving?.to_bin_s, 0)
      #@socket.send(receiving_bits.to_s, 0)
    when 'a'
      @socket.send(has_message_available.to_s, 0)
    when 'g'
      @socket.send(@in_message.to_s, 0)
      @in_message = nil
    when 's'
      puts "#{type}:#{data.unpack('H*')}"
      @out_message = data
    else
      puts("unknown command")
      puts([type, data,].join("-"))
    end
  end
  
  def close
    @running = false
    begin
      @socket.close
    rescue Exception => e
      #puts e.inspect
    end
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
  
  def collision?
    @active_connections.reject{|c| !c.is_transmitting? }.length > 1
  end
  
  def is_anyone_transmitting?(but_me)
    @active_connections.reject{|c| c == but_me || !c.is_transmitting? }.length > 0
  end
  
  def addClient(client)
    @active_connections.push(client)
    puts "client added"
    client.run()
  end


  def removeClient(client)
    @active_connections.delete(client)
    puts "client removed"
    client.close
  end
  
  def comunicate
    @active_connections.each do | client |
      if (!client.out_message.nil? )
        if (client.is_transmitting?)
          client.sending_bits +=  -1 * (client.bautrate / 100.0)
          if (client.sending_bits <= 0)
            broadcast(client.out_message, client)
            client.out_message = nil
          end
        else
          client.sending_bits = 80 * 8
           if !collision?
            @active_connections.each do | client |
              client.collision = false
            end
          end
        end
      end
     
    end
    if collision?
      @active_connections.each do | client |
        client.collision = true
      end
    end
  end
  
  def run
    Thread.new {
      loop do
        comunicate
        sleep 0.01
      end
      }
    Socket.tcp_server_loop(@port) do |sock, client_addrinfo|
      Thread.new {
        begin
          client = Client.new(sock, self)
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