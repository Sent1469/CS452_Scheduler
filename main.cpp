#include <iostream>
#include <cctype>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <time.h>

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

};

/* First main that contains code for user input */
// int main()
// {
//   // All the values the user can set.
//   std::string scheduler, numQueues, io, rtsType, ageTicks, interactive, ioTickValue;
//   bool isIO = false;
//   bool isInteractive = false;
//   // Start user input
//   std::cout<<"MFQS or RTS? ";
//   std::cin>>scheduler;

//   scheduler = toLower(scheduler);
//   // Go until we get the correct input
//   while (scheduler.compare("mfqs\0") != 0 && scheduler.compare("rts\0") != 0)
//   {
//     std::cout<<"Incorrect input.\nMFQS or RTS? ";
//     std::cin>>scheduler;
//     scheduler = toLower(scheduler);
//   }
  
//   if (scheduler == "mfqs") // MFQS specific questions
//   {
//     std::cout<<"How many queues? ";
//     std::cin>>numQueues; 
//     // Based on assignment need 2-5 queues.
//     while (stoi(numQueues) < 2 || stoi(numQueues) > 5) // Go until correct user input. 
//     {
//       if (stoi(numQueues) < 2)
//       {
//         std::cout<<"To few queues, number of queues needs to be > 2\nHow many queues? ";
//       }
//       else if (stoi(numQueues) > 5)
//       {
//         std::cout<<"To many queues, number of queues needs to be < 5\nHow many queues? ";
//       }
//       else
//       {
//         std::cout<<"Incorrect input.\nHow many queues? ";
//       }
//       std::cin>>numQueues;
//     }

//     std::cout<<"How many clock ticks for aging? ";
//     std::cin>>ageTicks;
//     // Number of clock ticks for the process to move up a queue.
//     while (stoi(ageTicks) < 1) // Go until correct user input.
//     {
//       std::cout<<"Value for aging must be an integer greater than 0\n"
//       <<"How many clock ticks for aging? ";
//       std::cin>>ageTicks;
//     }
//   }
//   else // RTS
//   {
//     std::cout<<"Soft or hard RTS (s/h)? ";
//     std::cin>>rtsType;
//     rtsType = toLower(rtsType);
//     // Specific type of scheduler for RTS.
//     while (rtsType != "s" && rtsType != "h") // Go until correct user input.
//     {
//       std::cout<<"Incorrect input.\nSoft or hard RTS? ";
//       std::cin>>rtsType;
//       rtsType = toLower(rtsType);
//     }
//   }

//   std::cout<<"Would you like to enter values for processes (y/n)? ";
//   std::cin>>interactive;
//   interactive = toLower(interactive);
//   // Check to see if the user wants to create processes (y) or use a text file (n).
//   while (interactive != "y" && interactive != "n") // Go until correct user input.
//   {
//     std::cout<<"Incorrect input.\nWould you like to enter values for processes (y/n)? ";
//     std::cin>>interactive;
//     interactive = toLower(interactive);
//   }

//   if (interactive == "y")
//     isInteractive = !isInteractive;

//   std::cout<<"Is there I/O (y/n)? ";
//   std::cin>>io;
//   io = toLower(io);
//   // If a process has to undergo I/O wait time (not actually doing I/O).
//   while (io != "y" && io != "n") // Wait until correct user input
//   {
//     std::cout<<"Incorrect input.\nIs there I/O (y/n)? ";
//     std::cin>>io;
//     io = toLower(io);
//   }
//   // End user input
//   if (io == "y")
//   {
//     std::cout<<"What clock tick will I/O occur? ";
//     std::cin>>ioTickValue;

//     while (stoi(ioTickValue) > 0)
//     {
      
//     }
//     isIO = !isIO;
//   }

//   std::cout<<"I/O = "<<isIO<<"\n";

//   std::cout<<"we made it\n";

//   int arr[1000000];

//   for (int i = 0; i < 1000000; i++)
//   {
//     arr[i] = 0;
//     std::cout<<arr[i]<<"\n";
//   }

//   return 0;
// }

// Second main that deals with file input.
int main()
{
  int numProcesses = 0;
  int i;
  int* argArr;
  std::ifstream input;
  std::ofstream output;
  std::string line;
  std::string token;
  std::string file = "10"; // Input file containing processes info.
  Process arr[10]; // TODO: Change this, was creating for testing

  input.open(file);
  if (!input.is_open())
  {
    std::perror("10 opened");
  }

  output.open("temp"); // Creates temp file.
  if (!output.is_open())
  {
    std::cout<<"Incorrect input.\nWould you like to enter values for processes (y/n)? ";
    std::cin>>interactive;
    interactive = toLower(interactive);
  }

  while (input) // Keep going until we have read all content in input.
  { 
    std::getline(input, line, '\r'); // On Windows, new line char is \r
    input.get(); // Grabs the extra \t character at the end of the line.

    // If we do not find a negative char and are not first line in file.
    if (line.find('-') == std::string::npos && line[0] != 'P')
    {
      
      output<<line<<std::endl; // Puts line in temp.
      numProcesses++; // Counts the number of processes.

      // TODO: Remove error checker below
      std::cout<<p.getPID()<<" "<<p.getBurst()<<" "<<p.getArrival()<<" "
      <<p.getPriority()<<" "<<p.getDeadline()<<" "<<p.getIO()<<std::endl;
    }
  }
  // TODO: Remove, used for error checking
  std::cout<<"numProcesses = "<<numProcesses<<std::endl;
  // Malloc the space for the array needed
  argArr = (int*)malloc(numProcesses * sizeof(int));
  input.close(); // Closing input file.
  output.close(); // Close temp.

  cleanFile(file);

  // Sort array here
  
  // Set values of class to each index of the array

  return 0;
}

// Sets a string to lovercase for error checking.
std::string toLower(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(),
    [](unsigned char c){ return std::tolower(c); });

  return str;
}

void cleanFile(std::string file)
{
  std::ifstream input;
  std::ofstream output;
  std::stringstream ss;
  std::string line;
  clock_t t;
  input.open("temp");
  if (!input.is_open())
  {
    std::perror("temp open for reading");
  }

  output.open(file);
  if (!output.is_open())
  {
    std::perror("10 is open for the second time");
  }

  while (input) // Keep going until we have read all content in input.
  {
    std::getline(input, line, '\n'); // Now file is created on linux hardware, so \n, not \r
    std::cerr<<line<<std::endl; // TODO: Remove, used for error testing.
    if (line[0] != 'P') {
        Process p;
        ss.str(line); // Puts line in stringstream
        for (i = 0; i < 6; i++) // 6 is max num of tokens
        {
            if (ss >> token) // Stringstream through the tokens of line.
            {
                if (i == 0)
                    p.setPID(stoi(token));
                else if (i == 1)
                    p.setBurst(stoi(token));
                else if (i == 2)
                    p.setArrival(stoi(token));
                else if (i == 3)
                    p.setPriority(stoi(token));
                else if (i == 4)
                    p.setDeadline(stoi(token));
                else
                    p.setIO(stoi(token));
            }
        }
        ss.clear(); // After using the stream, clear to take more input.
    }
    output<<line<<std::endl; // Puts line in output (input file).
  }

  input.close(); // Close temp.
  output.close(); // Close input file.

  remove("temp"); // Remove the data transfering.
}
// Todo
// Very much WIP but just a start with clock ticks
void realTimeScheduler(Process p, clock_t t) {
    std::ofstream file;
    t = clock();
    while (clock() != t + p.getBurst) {
        // Do nothing while waiting
    }
    t = clock() - t; // How long the process took in clock ticks
    file.open("times.txt");
    file << t + " ";
}