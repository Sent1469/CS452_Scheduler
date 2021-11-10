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
void MergeSort(std::vector<Process>& v, int s, int e);
void MergeSortedIntervals(std::vector<Process>& v, int s, int m, int e);
void softRealTime(std::vector<Process> v);
bool func(Process p1, Process p2);
std::string userInput(int *nQs, int *ageTks, bool *isIO, bool *isInteractive, int *ioTks);
std::vector<Process> readFile(std::string file);
void createProcesses(std::vector<Process>& pList);
//std::vector<Process> createProcesses();

int main()
{ 
  int numQueues = 0;
  int ageTicks = 0;
  int ioTicks = 0;
  bool isInteractive = false;
  bool isIO = false;
  std::vector<Process> pList;
  std::string scheduler;
  // COMMENT OUT LINES 84-96 IF ONLY NEEDING TO TEST FILE
  scheduler = userInput(&numQueues, &ageTicks, &isInteractive, &isIO, &ioTicks);

  std::cout<<numQueues<<std::endl<<ageTicks<<std::endl<<ioTicks<<std::endl
    <<isInteractive<<std::endl<<isIO<<std::endl;

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
  else
    createProcesses(pList);

  MergeSort(pList, 0, pList.size() - 1);

  // for (int i = 0; i < pList.size(); i++)
  // {
  //   std::cout<<pList.at(i).getArrival()<<std::endl;
  // }

  if (scheduler == "mfqs") //MFQS
  {
    std::cout<<"in mfqs\n";
  }
  else // RTS
  {
    std::cout<<"in rts: "<<scheduler<<std::endl;
    softRealTime(pList);
  }

  if (!pList.empty()) // If the vector is empty, no reason to clear it.
  {
    pList.clear();
  }

  return 0;
}

/* Sets a string to lowercase for error checking */
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

/* Asks for user input on certain variables for each scheduler */
std::string userInput(int *numQueues, int *ageTicks, bool *isInteractive, bool *isIO, int *ioTicks)
{
  std::string scheduler, interactive, io;
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
    std::cin>>*numQueues;
    // Based on assignment need 2-5 queues.
    while (*numQueues < 2 || *numQueues > 5) // Go until correct user input. 
    {
      if (*numQueues < 2)
      {
        std::cout<<"To few queues, number of queues needs to be > 2\nHow many queues? ";
      }
      else if (*numQueues > 5)
      {
        std::cout<<"To many queues, number of queues needs to be < 5\nHow many queues? ";
      }
      else
      {
        std::cout<<"Incorrect input.\nHow many queues? ";
      }
      std::cin>>*numQueues;
    }

    std::cout<<"How many clock ticks for aging? ";
    std::cin>>*ageTicks;
    // Number of clock ticks for the process to move up a queue.
    while (*ageTicks < 1) // Go until correct user input.
    {
      std::cout<<"Value for aging must be an integer greater than 0\n"
      <<"How many clock ticks for aging? ";
      std::cin>>*ageTicks;
    }
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

  std::cout<<"Would you like to create processes manually (y/n)? ";
  std::cin>>interactive;

  while (interactive != "y" && interactive != "n")
  {
    std::cout<<"Incorrect input, please enter y or n.\nWould you like to create processes manually (y/n)? ";
    std::cin>>interactive;
  }

  if (interactive == "y")
    *isInteractive = !*isInteractive;

  std::cout<<"Is there I/O (y/n)? ";
  std::cin>>io;

  while (io != "y" && io != "n")
  {
    std::cout<<"Incorrect input, please enter y or n.\nIs there I/O (y/n)? ";
    std::cin>>io;
  }

  if (io == "y")
  {
    *isIO = !*isIO;

    std::cout<<"What clock tick does I/O occur? ";
    std::cin>>*ioTicks;

    while (*ioTicks < 0)
    {
      std::cout<<"Value for I/O clock tick must be greater than 0.\nWhat clock tick does I/O occur? ";
      std::cin>>*ioTicks;
    }
  }

  return scheduler;
}

/* */
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

  std::getline(input, line, '\n'); // On Windows, new line char is \r
  //input.get();

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
     
      ss.clear(); // After using the stream, clear to take more input.
      output<<line<<std::endl; // Puts line in temp.
      numProcesses++; // Counts the number of processes.
      // TODO: Remove error checker below
      // std::cout<<p.getPID()<<" "<<p.getBurst()<<" "<<p.getArrival()<<" "
      // <<p.getPriority()<<" "<<p.getDeadline()<<" "<<p.getIO()<<std::endl;
      pList.push_back(p);
    }
  }
  // TODO: Remove, used for error checking
  std::cout<<"numProcesses = "<<numProcesses<<std::endl;
  input.close(); // Closing input file.
  output.close(); // Close temp.

  std::rename("t", "clean");
  remove("t"); // Remove the data transfering.

  auto t2 = high_resolution_clock::now();
  /* Getting number of milliseconds as an integer. */
  auto ms_int = duration_cast<milliseconds>(t2 - t1);

  /* Getting number of milliseconds as a double. */
  duration<double, std::milli> ms_double = t2 - t1;

  std::cout << ms_int.count() << "ms\n";
  std::cout << ms_double.count() << "ms\n";

  return pList;
}

void createProcesses(std::vector<Process>& pList)
{
  int numProcesses, i, burst, arrival, priority, deadline, io;
  int totalProcesses = 0; // Guarenteed to be 1 process.
  int pid = 1;
  std::string moreProcesses = "y"; // FENCEPOST

  while (moreProcesses == "y") // Keep going until user no longer wants to create processes.
  {
    std::cout<<"How many processes would you like to create? ";
    std::cin>>numProcesses;

    while (numProcesses <= 0)
    {
      std::cout<<"Number of processes must be greater than 0.\nHow many processes would you like to create? ";
      std::cin>>numProcesses;
    }

    for (i = 0; i < numProcesses; i++)
    {
      Process p;
      totalProcesses++;
      p.setPID(totalProcesses);

      std::cout<<"Process "<<totalProcesses<<"\'s burst time? ";
      std::cin>>burst;

      while (burst <= 0)
      {
        std::cout<<"Burst time must be greater than 0.\nProcess "<<totalProcesses<<"\'s burst time? ";
        std::cin>>burst;
      }
      p.setBurst(burst);

      std::cout<<"Process "<<totalProcesses<<"\'s arrival time? ";
      std::cin>>arrival;

      while (arrival < 0)
      {
        std::cout<<"Arrival time must be nonnegative.\nProcess "<<totalProcesses<<"\'s arrival time? ";
        std::cin>>arrival;
      }
      p.setArrival(arrival);

      std::cout<<"Process "<<totalProcesses<<"\'s priority? ";
      std::cin>>priority;
      
      while (priority < 0)
      {
        std::cout<<"Process priority must be nonnegatve.\nProcess "<<totalProcesses<<"\'s priority? ";
        std::cin>>priority;
      }
      p.setPriority(priority);

      std::cout<<"Process "<<totalProcesses<<"\'s deadline? ";
      std::cin>>deadline;

      while (deadline <= 0)
      {
        std::cout<<"Deadline must be a number greater than 0.\nProcess "<<totalProcesses<<"\'s deadline? ";
        std::cin>>deadline;
      }
      p.setDeadline(deadline);

      std::cout<<"Process "<<totalProcesses<<"\'s I/O? ";
      std::cin>>io;

      while (io < 0)
      {
        std::cout<<"Process I/O must be nonnegatve.\nProcess "<<totalProcesses<<"\'s I/O? ";
        std::cin>>io;
      }
      p.setIO(io);
      std::cout<<std::endl;
    }

    std::cout<<"Would you like to create more processes (y/n)? ";
    std::cin>>moreProcesses;

    while (moreProcesses != "y" && moreProcesses != "n")
    {
      std::cout<<"Incorrect input.\nWould you like to create more processes (y/n)? ";
      std::cin>>moreProcesses;
    }
  }
  
}

// The interval from [s to m] and [m+1 to e] in v are sorted
// The function will merge both of these intervals
// Such the interval from [s to e] in v becomes sorted
void MergeSortedIntervals(std::vector<Process>& v, int s, int m, int e) 
{
  // Temp is used to temporary store the vector obtained by merging
  // Elements from [s to m] and [m+1 to e] in v
  std::vector<Process> temp;

  int i, j;
  i = s;
  j = m + 1;

  while (i <= m && j <= e) {
    if (v[i].getArrival() <= v[j].getArrival()) 
    {
      temp.push_back(v[i]);
      ++i;
    }
    else 
    {
      temp.push_back(v[j]);
      ++j;
    }
  }
  while (i <= m) 
  {
    temp.push_back(v[i]);
        ++i;
  }
  while (j <= e) 
  {
    temp.push_back(v[j]);
    ++j;
  }
  for (int i = s; i <= e; ++i)
    v[i] = temp[i - s];
}

// The MergeSort function
// Sorts the array in the range [s to e] in v using
// Merge sort algorithm
void MergeSort(std::vector<Process>& v, int s, int e) 
{
  if (s < e) {
    int m = (s + e) / 2;
    MergeSort(v, s, m);
    MergeSort(v, m + 1, e);
    MergeSortedIntervals(v, s, m, e);
  }
}

void softRealTime(std::vector<Process> v) 
{
  // Start scheduling process here

}