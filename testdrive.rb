#!/usr/bin/ruby
puts "starting #{ARGV[0]}"
process = IO.popen(ARGV[0], "w+")
chars = %W{"hello" 345|34.65}
chars.push ""
inp = ""
while inp = process.gets do
  puts inp
  c = chars.shift
  puts c
  process.write c
  sleep 1
end
