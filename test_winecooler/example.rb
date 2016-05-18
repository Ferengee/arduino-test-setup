numbers = open("input.txt").readlines.map {|n| n.chomp.to_i }

puts numbers.inspect

maxlen = 3;

one = []
two = []
three = []

class Chain
  
  attr_reader :store, :next
  
  def initialize link
    @next = link
    @index = 0
    @store = []
  end
  def add number
    if (@index > 2)
      @index = 0
      
      if (!@next.nil?)
        @next.add(average())
      end
    end
    
    @store[@index] = number
    @index += 1
    
  end
  
  def average
    @store.reduce { |val, acc| val + acc } / @store.length
  end
  
  def to_a
    r = []
    chain = self
    while(!chain.nil?)
      r.push(chain.store)
      chain = chain.next
    end
    r
  end
end

chain = Chain.new(Chain.new(Chain.new(nil)))

numbers.each do | number |
  chain.add number
puts "analogRead 14";
puts "temp:"
puts chain.to_a.inspect
end

