require './debugger.rb'
require './statistics.rb'

range_size_vector = []
(0..9).each do |i|
  range_size_vector << 10**i
end
range_num_threads = (1..15).step(2)

range_size_vector.each do |size_vector|
  best_time = 2**100
  best_num_threads = -1
  best_ifs = -1
  range_num_threads.each do |num_threads|
    STDERR.print "Trying with size_vector = #{size_vector}, num_threads = #{num_threads}"
    out = `./contention.sh #{size_vector} #{num_threads}`.split("\n")
    out.reject! { |i| i.include? 'Number' }.map! { |i| i = i.to_f }
    i_min = -1
    min = 2**100
    out.each_with_index do |val, idx|
      if val < min
        min = val
        i_min = idx
      end
    end
    STDERR.print ": i_min=#{i_min} min=#{min}\n"
    if min < best_time
      # Got a better time
      best_time = min
      best_num_threads = num_threads
      best_ifs = i_min
    end
  end
  puts "vector_size=#{size_vector} best_time=#{best_time} best_num_threads=#{best_num_threads} best_ifs=#{best_ifs}"
end
