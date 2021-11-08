#include <iostream>
#include <cctype>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <chrono>

std::string toLower(std::string str);
void cleanFile(std::string file);

class Process 
{
  private:
    int pid, burst, arrival, priority, deadline, io;
  
  public:
    void setPID(int pid)
    {
      this->pid = pid;
    }
    void setBurst(int burst)
    {
      this->burst = burst;
    }
    void setArrival(int arrival)
    {
      this->arrival = arrival;
    }
    void setPriority(int priority)
    {
      this->priority = priority;
    }
    void setDeadline(int deadline)
    {
      this->deadline = deadline;
    }
    void setIO(int io)
    {
      this->io = io;
    }

    int getPID()
    {
      return pid;
    }
    int getBurst()
    {
      return burst;
    }
    int getArrival()
    {
      return arrival;
    }
    int getPriority()
    {
      return priority;
    }
    int getDeadline()
    {
      return deadline;
    }
    int getIO()
    {
      return io;
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
  std::ifstream input;
  std::ofstream output;
  std::stringstream ss;
  std::string line, token;
  std::string file = "500k"; // Input file containing processes info.
  //Process arr; // TODO: Change this, was creating for testing
  std::vector<Process> pList;

  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;

  auto t1 = high_resolution_clock::now();

  input.open(file);
  if (!input.is_open())
  {
    std::perror("file");
    return 1;
  }

  output.open("t"); // Creates temp file.
  if (!output.is_open())
  {
    std::perror("t");
    return 1;
  }

  std::cout<<"before first while"<<std::endl;

  while (input) // Keep going until we have read all content in input.
  {
    std::getline(input, line, '\n'); // On Windows, new line char is \r
    input.get(); // Grabs the extra \t character at the end of the line.

    // If we do not find a negative char and are not first line in file.
    if (line.find('-') == std::string::npos && line[0] != 'P')
    {
      Process p;
      ss.str(line); // Puts line in stringstream
      ss>>token;
      p.setPID(stoi(token));
      ss>>token;
      p.setBurst(stoi(token));
      ss>>token;
      p.setArrival(stoi(token));
      ss>>token;
      p.setPriority(stoi(token));
      ss>>token;
      p.setDeadline(stoi(token));
      ss>>token;
      p.setIO(stoi(token));
      // for (i = 0; i < 6; i++) // 6 is max num of tokens
      // {
      //   if (ss >> token) // Stringstream through the tokens of line.
      //   {
      //     if (i == 0)
      //       p.setPID(stoi(token));
      //     else if (i == 1)
      //       p.setBurst(stoi(token));
      //     else if (i == 2)
      //       p.setArrival(stoi(token));
      //     else if (i == 3)
      //       p.setPriority(stoi(token));
      //     else if (i == 4)
      //       p.setDeadline(stoi(token));
      //     else
      //       p.setIO(stoi(token));
      //   }
      // }
      ss.clear(); // After using the stream, clear to take more input.
      output<<line<<std::endl; // Puts line in temp.
      numProcesses++; // Counts the number of processes.
      // TODO: Remove error checker below
      // std::cout<<p.getPID()<<" "<<p.getBurst()<<" "<<p.getArrival()<<" "
      // <<p.getPriority()<<" "<<p.getDeadline()<<" "<<p.getIO()<<std::endl;
      pList.push_back(p);
    }
    //std::cout<<"at bottom of while"<<std::endl;
  }
  // TODO: Remove, used for error checking
  std::cout<<"numProcesses = "<<numProcesses<<std::endl;
  input.close(); // Closing input file.
  std::cout<<"does this take forever #1\n";
  output.close(); // Close temp.
  std::cout<<"does this take forever #1.1\n";

  std::rename("t", "500kc");
  std::cout<<"does this take forever #2\n";
  remove("t"); // Remove the data transfering.
  std::cout<<"does this take forever #3\n";

  // for (i = 0; i < pList.size(); i++)
  // {
  //   std::cout<<pList.at(i).getPID()<<std::endl;
  // }

  pList.clear(); // Clear the vector to free memory allocated.
  std::cout<<"does this take forever #4\n";

  auto t2 = high_resolution_clock::now();
  /* Getting number of milliseconds as an integer. */
  auto ms_int = duration_cast<milliseconds>(t2 - t1);

  /* Getting number of milliseconds as a double. */
  duration<double, std::milli> ms_double = t2 - t1;

  std::cout << ms_int.count() << "ms\n";
  std::cout << ms_double.count() << "ms";

  return 0;
}

// Sets a string to lovercase for error checking.
std::string toLower(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(),
    [](unsigned char c){ return std::tolower(c); });

  return str;
}