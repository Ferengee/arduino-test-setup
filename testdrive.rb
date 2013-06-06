#!/usr/bin/ruby
puts "starting #{ARGV[0]}"
process = IO.popen(ARGV[0], "w+")
chars = %W{w d w w w d w d w w}
chars.push ""
inp = ""
while inp = process.gets do
  puts inp
  c = chars.shift
  puts c
  process.puts c
  sleep 10
end
