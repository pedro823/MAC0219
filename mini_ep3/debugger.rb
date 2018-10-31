# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                                                 #
#                       EP2 - Redes de computadores e sistemas distribuidos                       #
#                                   Pedro Pereira, 9778794                                        #
#                                   Rafael Gusmão, 9778561                                        #
#                                                                                                 #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Class responsible for debug prints
class Debugger

  @@debug_priority = 0
  def self.set_debug_priority(value)
    @@debug_priority = value
  end

  def self.status(priority, *args)
    if priority >= @@debug_priority
      print "STATUS:"
      args.each do |i|
        print " " + i.to_s
      end
      print "\r"
    end
  end

  def self.debug_print(priority, *args)
    if priority >= @@debug_priority
      # Solves race problem with puts
      print "debug:" + priority.to_s
      args.each do |i|
        print " " + i.to_s
      end
      print "\n"
    end
  end

  def self.show_usage
    puts "Usage: #{$0} [FLAGS] [PRIME]"
    puts "FLAGS:", "\t-d[dddd]:", "\t\t Decrease debug priority."
    puts "\t\t Debug priority starts at 5, every d in the flag decreases\n" \
         "\t\t priority by 1, showing more debug messages."
    puts "\t-p PORT:", "\t\t Sets port to use in the program to PORT."
    puts "\t--help:", "\t\t shows this message and exit."
  end

  def self.formal_print(text, *args)
    final_message = text
    args.each do |i|
      final_message += " " + i
    end
    print "+" + "-" * 78 + "+\n"
    lines = (final_message.length + 77) / 78
    if lines > 1
      words = final_message.split(" ")
      total_line_length = 0
      index = last_index = 0
      while index < words.length
        if words[index].length > 78
          # prints word, moves onto next
          print "|" + words[index][0..78] + "|\n"
          total_line_length = 0
          last_index = index + 1
          index += 1
        end
        # if words[index].count("\n") > 0
        #
        # end
        if total_line_length + words[index].length > 78
          # Pretty prints line, resets counter
          to_print = words[last_index..index-1].join(" ")
          print "|" + to_print.center(78, " ") + "|\n"
          total_line_length = 0
          last_index = index
        else
          total_line_length += words[index].length + 1
          index += 1
        end
      end
      # Prints last line
      print "|" + words[last_index..index].join(" ").center(78, " ") + "|\n"
    else
      print "|" + final_message.center(78, " ") + "|\n"
    end
    print "+" + "-" * 78 + "+\n\n"
  end
end

## unit testing
if __FILE__ == $0
  Debugger.set_debug_priority(10)
  Debugger.debug_print(4, "This should print")
  Debugger.debug_print(2, "This should not print")
  Debugger.formal_print("Pedro Pereira, 9778794")
  Debugger.formal_print("MAC0425 - EP1 - Busca")
end
