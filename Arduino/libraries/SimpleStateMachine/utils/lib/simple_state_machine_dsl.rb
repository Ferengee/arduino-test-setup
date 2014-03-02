require 'parslet'
#
# converts state machine definitions found in the stdin to a grapvis dot format
#  ruby machine_to_dot.rb ../examples/FourSteps/FourSteps.ino > out.dot
#  dot -Tpng -O out.dot
#

class SimpleStateMachineDSL < Parslet::Parser
  rule(:space)      { match['\t\s'].repeat(1) }
  rule(:space?) { space.maybe }
  rule(:terminator) { str(";")| str("\n")  }
  rule(:state)  { match('[a-zA-Z]').repeat(1)}
  rule(:enterfunc)  { match('[a-zA-Z]').repeat(1)}

  rule(:event)  { match('[A-Z]').repeat(1)}
  rule(:vertex) { str("-") >> event.as(:event) >> str("->") >> space? }
  rule(:link){ state.as(:start_state) >> vertex >> state.as(:end_state)}
  rule(:state_definition) { state.as(:state) >> str(":") >> enterfunc.as(:enterfunc)}
  rule(:line) {space? >> (link | state_definition | terminator | comment | machine) >> space?}
  rule(:comment) { str("#") >> match["^\n"].repeat.as(:comment) >> str("\n") } 
  rule(:machine) { match('[a-zA-Z]').repeat(1).as(:machine) >> str("(") >> state.as(:start_state) >> str(")") }
  rule(:language) {line.repeat(0)}
  root(:language)
end

class StateMachineDocument
  def initialize
    @states = []
    @events = []
    @machine_declarations = []
    @enterfuncs = []
  
  end
  
  def addLink(start_state, event, end_state)
    @states.push(start_state)
    @states.push(end_state)
    @events.push(event)
  end
  
  def addDefinition(state, enterfunc)
    @enterfuncs.push(enterfunc)
    @states.push(state)
  end
end

class InoDocument < StateMachineDocument
  #TODO: a state and an enterfunc can't have the same name, should throw an error or rename the state by adding State to the end of the name
  def initialize
      super()
      @setup_body = []
      @link_count = 0
      @machine_declarations = []      
  end
  
  def addLink(start_state, event, end_state)
    super(start_state, event, end_state)
    @setup_body.push("  #{start_state}.on(l++, #{event})->to(#{end_state});")
    @link_count += 1

  end
  
  def addComment(comment)
    @setup_body.push("\n/* #{comment} */")
  end
  
  def addDefinition(state, enterfunc)
    super(state, enterfunc)
    @setup_body.push("  #{state}.enterfunc = #{enterfunc};")
  end
  
  def addMachine(machine, start_state)
    @states.push(start_state)
    @setup_body.push("  #{machine}.start();")
    @machine_declarations.push("Machine #{machine} = Machine(#{start_state});")
  end
  
  
  
  def includes
    %Q(#include <SimpleStateMachine.h>
#include <EventChannel.h>)
  end
  
  def enterfuncs
    @enterfuncs.map {|f| f.to_s}.uniq.map { |enterfunc|
      %Q(void #{enterfunc}(int token, void * data){})
                   }.join("\n")
  end
  
  #TODO: break up states over multiple lines if line becomes to long
  def declarations
    ["State " + @states.map{|s| s.to_s}.uniq.join(", ") + ";",
     "Vertex state_machine_links[#{@link_count}];",
     @machine_declarations.join("\n"),
     "enum TransitionEvents {#{@events.map{|s| s.to_s}.uniq.join(", ")}};"].join("\n\n")
    
  end
  
  def setup
    ["void setup_machines(){",
     "  Vertex * l = state_machine_links;",
     @setup_body.join("\n"),
     "}"].join("\n")
  end
  
  def to_s
    %Q(#{includes}

#{enterfuncs}

#{declarations}

#{setup}
)
  end 
end

class DotDocument < StateMachineDocument
  def initialize
      super()
      @links = []
      @state_definitions = []
      @start_states = []
  end
  
  def addLink(start_state, event, end_state)
    super(start_state, event, end_state)
    @links.push("  #{start_state}->#{end_state}[label=#{event}];")
  end
  
  def addComment(comment)
  end
  
  def addMachine(machine, start_state)
    @states.push(start_state)
    @start_states.push(start_state)
  end
  
    def addDefinition(state, enterfunc)
    super(state, enterfunc)
    @state_definitions.push({:state => state, :enterfunc => enterfunc })
  end
  
  def state_definitions
    @state_definitions.map { |definition|
      state = definition[:state]
      enterfunc = definition[:enterfunc]
      penwidth = @start_states.include?(state) ? "penwidth=2," : ""
      "  #{state}[#{penwidth}label=\"#{state}:#{enterfunc}\"]"
      }.join("\n")
  end

  def to_s
    %Q(digraph{
  node [shape = ellipse,height=.1];

#{@links.join("\n")}

#{state_definitions}
}
)
  end 
end


class SimpleStateMachineTransform < Parslet::Transform
  rule(
    :start_state => simple(:start_state), 
    :event => simple(:event), 
    :end_state => simple(:end_state)) { document.addLink(start_state, event, end_state) }
  rule(
    :state => simple(:state), 
    :enterfunc => simple(:enterfunc)) { document.addDefinition(state, enterfunc) }
  rule(:comment => simple(:comment)) { document.addComment(comment)}
  rule(
    :machine => simple(:machine),
    :start_state => simple(:start_state)) { document.addMachine(machine, start_state)}
end
  
  