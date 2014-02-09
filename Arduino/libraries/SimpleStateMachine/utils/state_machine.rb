#
#  I use ruby for my pseudo code
#  This is a sketch for how i would like my arduino state machine to work
#
class Vertex
  attr_reader :token, :nextSibbling, :end_state
  attr_writer :nextSibbling
  
  def initialize(token)
    @token = token
  end
  
  def accept?(token)
    @token == token
  end
  
  def join(start_state, end_state)
    @start_state = start_state
    @end_state = end_state
    @start_state.link(self)
  end
  
end

class State
  attr_reader :enterfunc, :outgoing
  attr_writer :enterfunc, :outgoing
  
  def link(vertex)
    vertex.nextSibbling = @outgoing
    @outgoing = vertex
  end
  
  def try(token)
    puts "trying #{token}"
    vertex = @outgoing
    while(!vertex.nil?)
      if vertex.accept?(token)
        return vertex.end_state
      end
      vertex = vertex.nextSibbling
    end
    return nil
  end
  
end


class Machine
  attr_reader :current_state
  
  def initialize(state)
    @current_state = state
  end
  
  def receive(token, data=nil?)
    next_state = @current_state.try(token)
    if (!next_state.nil?)
      @current_state = next_state
      @current_state.enterfunc
      send(@current_state.enterfunc, token, data) unless @current_state.enterfunc.nil?
    end
    return nil
  end
end


start_state = State.new
end_state = State.new

a_state = State.new
b_state = State.new

@s1 = Vertex.new(1)
@s1_1 = Vertex.new(1)

@s2 = Vertex.new(2)
@s2_1 = Vertex.new(2)

@s3 = Vertex.new(3)
@s4 = Vertex.new(4)
@s5 = Vertex.new(1)


@s1.join(start_state, a_state)
@s1_1.join(a_state, start_state)

@s2.join(start_state, b_state)
@s2_1.join(b_state, start_state)

@s3.join(a_state, end_state)
@s4.join(b_state, end_state)
@s5.join(end_state, start_state)

@m = Machine.new(start_state)
@start_state = start_state

def hello(token, data)
  puts "Hello #{token}"
end

def goodby(token, data)
  puts "Goodbye #{token} - #{data}"
end

def astate(token, data)
  puts "In A"
end

def bstate(token, data)
  puts "In B"
end 

start_state.enterfunc = :hello
end_state.enterfunc = :goodby
a_state.enterfunc = :astate
b_state.enterfunc = :bstate

