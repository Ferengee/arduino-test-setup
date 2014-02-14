#require 'machine_to_dot_parslet'  
require 'parslet/rig/rspec'

load "machine_to_dot_parslet.rb"
require 'pp'

describe SimpleStateDSL do
  let(:parser) { SimpleStateDSL.new }
  
  describe :state do
    
    
    it { parser.state.parse("a").should == "a" }
    it { should_not parse ("-") }
  end
  
  describe :event do
    subject { SimpleStateDSL.new.event }
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
  
  describe :language do
    let(:multiline){%Q(aState-EVENT->endState
aState-EVENT->endState)}
    subject {parser}
    it { should parse (multiline) }

  end
end
