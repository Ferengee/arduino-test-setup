#!/usr/bin/env ruby
require 'parslet'
#
# converts state machine definitions found in the stdin to a grapvis dot format
#  ruby machine_to_dot.rb ../examples/FourSteps/FourSteps.ino > out.dot
#  dot -Tpng -O out.dot                                              [~/src/ruby]

#

class SimpleStateDSL < Parslet::Parser
  rule(:space)      { match('\s').repeat(1) }
  rule(:space?) { space.maybe }
  rule(:terminator) { str(";")| str("\n")  }
  rule(:state)  { match('[a-zA-Z]').repeat(1)}
  rule(:event)  { match('[A-Z]').repeat(1)}
  rule(:vertex) { str("-") >> event.as(:event) >> str("->") >> space? }
  rule(:link){ state.as(:start_state) >> vertex >> state.as(:end_state)}
  rule(:language) { link >> (terminator >> link).repeat(0)}
  root(:language)
end

