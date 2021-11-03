#include <iostream>
#include <cctype>
#include <string>
#include <algorithm>

std::string toLower(std::string str);

int main()
{
  // All the values the user can set.
  std::string scheduler, numQueues, io, rtsType, ageTicks, interactive;
  bool isIO = false;
  bool isInteractive = false;
  // Start user input
  std::cout<<"MFQS or RTS? ";
  std::cin>>scheduler;

  scheduler = toLower(scheduler);
  // Go until we get the correct input
  while (scheduler.compare("mfqs\0") != 0 && scheduler.compare("rts\0") != 0)
  {
    std::cout<<"Incorrect input.\nMFQS or RTS? ";
    std::cin>>scheduler;
    scheduler = toLower(scheduler);
  }
  
  if (scheduler == "mfqs") // MFQS specific questions
  {
    std::cout<<"How many queues? ";
    std::cin>>numQueues; 
    // Based on assignment need 2-5 queues.
    while (stoi(numQueues) < 2 || stoi(numQueues) > 5) // Go until correct user input. 
    {
      if (stoi(numQueues) < 2)
      {
        std::cout<<"To few queues, number of queues needs to be > 2\nHow many queues? ";
      }
      else if (stoi(numQueues) > 5)
      {
        std::cout<<"To many queues, number of queues needs to be < 5\nHow many queues? ";
      }
      else
      {
        std::cout<<"Incorrect input.\nHow many queues? ";
      }
      std::cin>>numQueues;
    }

    std::cout<<"How many clock ticks for aging? ";
    std::cin>>ageTicks;
    // Number of clock ticks for the process to move up a queue.
    while (stoi(ageTicks) < 1) // Go until correct user input.
    {
      std::cout<<"Value for aging must be an integer greater than 0\n"
      <<"How many clock ticks for aging? ";
      std::cin>>ageTicks;
    }
  }
  else // RTS
  {
    std::cout<<"Soft or hard RTS (s/h)? ";
    std::cin>>rtsType;
    rtsType = toLower(rtsType);
    // Specific type of scheduler for RTS.
    while (rtsType != "s" && rtsType != "h") // Go until correct user input.
    {
      std::cout<<"Incorrect input.\nSoft or hard RTS? ";
      std::cin>>rtsType;
      rtsType = toLower(rtsType);
    }
  }

  std::cout<<"Would you like to enter values for processes (y/n)? ";
  std::cin>>interactive;
  interactive = toLower(interactive);
  // Check to see if the user wants to create processes (y) or use a text file (n).
  while (interactive != "y" && interactive != "n") // Go until correct user input.
  {
    std::cout<<"Incorrect input.\nWould you like to enter values for processes (y/n)? ";
    std::cin>>interactive;
    interactive = toLower(interactive);
  }

  if (interactive == "y")
    isInteractive = !isInteractive;

  std::cout<<"Is there I/O (y/n)? ";
  std::cin>>io;
  io = toLower(io);
  // If a process has to undergo I/O wait time (not actually doing I/O).
  while (io != "y" && io != "n") // Wait until correct user input
  {
    std::cout<<"Incorrect input.\nIs there I/O (y/n)? ";
    std::cin>>io;
    io = toLower(io);
  }
  // End user input
  if (io == "y")
    isIO = !isIO;

  std::cout<<"I/O = "<<isIO<<"\n";

  std::cout<<"we made it\n";

  return 0;
}

// Sets a string to lowercase for error checking.
std::string toLower(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(),
    [](unsigned char c){ return std::tolower(c); });

  return str;
}