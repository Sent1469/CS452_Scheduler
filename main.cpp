#include <iostream>
#include <cctype>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <chrono>

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

std::string toLower(std::string str);
void cleanFile(std::string file);
bool func(Process p1, Process p2);
std::string userInput(int *nQs, int *ageTks, bool *isIO, bool *isInteractive, int *ioTks);
std::vector<Process> readFile(std::string file);
//std::vector<Process> createProcesses();

int main()
{ 
  int nQs = 0;
  int ageTks = 0;
  int ioTks = 0;
  bool isInteractive = false;
  bool isIO = false;
  std::vector<Process> pList;
  // COMMENT OUT LINES 84-96 IF ONLY NEEDING TO TEST FILE
  std::string scheduler = userInput(&nQs, &ageTks, &isInteractive, &isIO, &ioTks);

  std::cout<<nQs<<std::endl<<ageTks<<std::endl<<ioTks<<std::endl
    <<isInteractive<<std::endl<<isIO<<std::endl;

  if (scheduler == "mfqs")
  {
    std::cout<<"in mfqs\n";
  }
  else // RTS
  {
    std::cout<<"in rts: "<<scheduler<<std::endl;
  }

  if (!isInteractive) // If not interactive, must read from file.
  {
    std::ifstream input;
    std::string file;
    std::cout<<"Please enter a file to read from: ";
    std::cin>>file;

    input.open(file);
    while (!input.is_open()) // Go until ifstream can open file, meaning file exists.
    {
      std::cout<<"Entered incorrect file name.\nPlease enter a file to read from: ";
      std::cin>>file;

      input.open(file);
    }
    // Since opened for testing, must close it.
    input.close();
    // Reads the file and creates Process vector.
    pList = readFile(file);
  }
  // else
  //   pList = createProcesses();

  // QUEUES HERE.
  for (int i = 0; i < pList.size(); i++)
  {
    std::cout<<pList.at(i).getPID()<<std::endl;
  }

  if (!pList.empty()) // If the vector is empty, no reason to clear it.
  {
    pList.clear();
  }

  return 0;
}

// Sets a string to lovercase for error checking.
std::string toLower(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(),
    [](unsigned char c){ return std::tolower(c); });

  return str;
}

bool func(Process p1, Process p2)
{
  return p1.getPID() < p2.getPID();
}

std::string userInput(int *nQs, int *ageTks, bool *isInteractive, bool *isIO, int *ioTks)
{
  std::string scheduler, numQueues, ageTicks, interactive, io, ioTicks;
  // Start user input
  std::cout<<"MFQS or RTS? ";
  std::cin>>scheduler;

  scheduler = toLower(scheduler);
  // Go until we get the correct input
  while (scheduler.compare("mfqs") != 0 && scheduler.compare("rts") != 0)
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
    *nQs = stoi(numQueues);

    std::cout<<"How many clock ticks for aging? ";
    std::cin>>ageTicks;
    // Number of clock ticks for the process to move up a queue.
    while (stoi(ageTicks) < 1) // Go until correct user input.
    {
      std::cout<<"Value for aging must be an integer greater than 0\n"
      <<"How many clock ticks for aging? ";
      std::cin>>ageTicks;
    }
    *ageTks = stoi(ageTicks);
  }
  else // RTS
  {
    std::cout<<"Soft or hard RTS (s/h)? ";
    std::cin>>scheduler;
    scheduler = toLower(scheduler);
    // Specific type of scheduler for RTS.
    while (scheduler != "s" && scheduler != "h") // Go until correct user input.
    {
      std::cout<<"Incorrect input.\nSoft or hard RTS? ";
      std::cin>>scheduler;
      scheduler = toLower(scheduler);
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
    *isInteractive = !*isInteractive;

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
  {
    std::cout<<"What clock tick will I/O occur? ";
    std::cin>>ioTicks;

    while (stoi(ioTicks) < 0)
    {
      std::cout<<"I/O clock tick must be greater than 0\nWhat clock tick will I/O occur? ";
      std::cin>>ioTicks;
    }
    *ioTks = stoi(ioTicks);
    *isIO = !*isIO;
  }

  return scheduler;
}

std::vector<Process> readFile(std::string file)
{
  int numProcesses = 0;
  int i;
  std::ifstream input;
  std::ofstream output;
  std::stringstream ss;
  std::string line, token;
  //file = "500k"; // Input file containing processes info.
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
    exit(-1);
  }

  output.open("t", std::ofstream::trunc | std::ofstream::out); // Creates temp file.
  if (!output.is_open())
  {
    std::perror("t");
    exit(1);
  }

  std::cout<<"before first while"<<std::endl;

  std::getline(input, line, '\n'); // On Windows, new line char is \r
  //input.get();

  std::cout<<"'"<<line<<"'"<<std::endl;
  output<<line<<std::endl;

  while (input) // Keep going until we have read all content in input.
  {
    std::getline(input, line, '\n'); // On Windows, new line char is \r
    //input.get(); // Grabs the extra \t character at the end of the line.

    //std::cout<<"'"<<line<<"'"<<std::endl;

    // If we do not find a negative char and are not first line in file.
    if (line.find('-') == std::string::npos)
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
  output.close(); // Close temp.

  std::rename("t", "clean");
  remove("t"); // Remove the data transfering.

  // for (i = 0; i < pList.size(); i++)
  // {
  //   std::cout<<pList.at(i).getPID()<<std::endl;
  // }

  //pList.clear(); // Clear the vector to free memory allocated.
  std::cout<<"does this take forever #4\n";

  auto t2 = high_resolution_clock::now();
  /* Getting number of milliseconds as an integer. */
  auto ms_int = duration_cast<milliseconds>(t2 - t1);

  /* Getting number of milliseconds as a double. */
  duration<double, std::milli> ms_double = t2 - t1;

  std::cout << ms_int.count() << "ms\n";
  std::cout << ms_double.count() << "ms\n";

  return pList;
}