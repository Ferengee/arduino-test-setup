#!/usr/bin/env ruby
#
# converts state machine definitions found in the stdin to a grapvis dot format
#  ruby machine_to_dot.rb ../examples/FourSteps/FourSteps.ino > out.dot
#  dot -Tpng -O out.dot                                              [~/src/ruby]

#

definition = ""

if (ARGV.length > 0)
  definition = File.read(ARGV[0]).split("\n")
else
  definition = ARGF
end

edge_pattern = /^\s*(\w+)(.|->)on\(.*,\s+(\w+)\)->to\((\w+)\);/
enterfunc_pattern = /^\s*(\w+).enterfunc\s+=\s+(\w+);/


puts %Q(digraph\{
  node [shape = Mrecord,height=.1];
)
definition.each do |line|
  r = line.match(edge_pattern)
  if(r)
   puts "#{r[1]}->#{r[4]}[label=#{r[3]}];"
  end
  r = line.match(enterfunc_pattern)
  if(r)
   puts "#{r[1]}[label=\"#{r[1]}|#{r[2]}\"];"
  end
end
puts "}"