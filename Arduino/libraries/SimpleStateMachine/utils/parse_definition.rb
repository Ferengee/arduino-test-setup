#!/usr/bin/env ruby

$:.unshift File.dirname(__FILE__) + "/lib"
require "simple_state_machine_dsl"
require "pp"
require "optparse"
#
# converts state machine definitions found in the stdin to a grapvis dot format
#  ruby machine_to_dot.rb definition.txt > out.dot
#  dot -Tpng -O out.dot
#

definition = ""
options = {}

ARGV.options do |opts|
  
  opts.on("-i", "--input [FILE]", String,
          "use FILE as source"){ |f| options[:input]=f}
  opts.on("-t", "--type [TYPE]", [:dot, :ino],
          "select output type (dot or ino)") { |t| options[:type] = t}
  opts.on("-o", "--output [FILE]", String,
          "write output to FILE (output is written to STDOUT if omitted)"){|f|options[:output] = f}
  opts.parse!

end

if (options[:input].nil?)
  definition = ARGF.read
else
  definition = File.read(options[:input])
end

if (options[:type] == :dot)
  document = DotDocument.new
else
  document = InoDocument.new
end


parser = SimpleStateMachineDSL.new
transf = SimpleStateMachineTransform.new

result = transf.apply(parser.parse(definition), :document => document)
#pp parser.parse(definition)
if (options[:output].nil?)
  puts document
else
  File.open(options[:output], "w").write(document)
end