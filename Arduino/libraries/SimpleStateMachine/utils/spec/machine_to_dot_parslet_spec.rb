#require 'machine_to_dot_parslet'  
require 'spec_helper'
require 'parslet/rig/rspec'

load "simple_state_machine_dsl.rb"
require 'pp'

describe SimpleStateMachineDSL do
  let(:parser) { SimpleStateMachineDSL.new }
  
  describe :state do
    
    
    it { parser.state.parse("a").should == "a" }
    it { should_not parse ("-") }
  end
  
  describe :event do
    subject { parser.event }
    it { subject.parse("EVENT").should == "EVENT" }
    it { should_not parse("event") }
    
  end
  
  describe :vertex do
    subject { parser.vertex }
    it { should parse("-A->") }
    it { should_not parse("-->") }
  end
  
  describe :link do
    subject { parser.link }
    it { should parse ("aState-EVENT->endState") }
  end
  
  describe :comment do
    subject { parser.comment }
    it { should parse ("# a comment line\n")}
    it { should_not parse ("# a comment")}

    it { should_not parse ("a regular line\n")}

  end
  
  describe :state_definition do
    subject { parser.state_definition }
    it { should parse ("aState:helloWorld") }
    it { should_not parse ("aState") }
    it { should_not parse ("a-State") }
    it { should_not parse ("EVENT") }
    
         
  end
  
  describe :machine do
    subject { parser.machine }
    it { should parse ("machine(startState)")}
    
  end
  
  describe :language do
    let(:multiline){%Q(# events
  startState-NEXT->aState
  aState-NEXT->endState
  aState-BACK->startState
#functions                 
  startState:hello
  endState:goodbye
#machines       
  machine(startState)
                   )}
    subject {parser}
    it { should parse (multiline) }

  end
end
