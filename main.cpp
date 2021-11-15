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

    void subtractBurst()
    {
      burst--;
    }

    void subtractIO()
    {
      io--;
    }
};

std::string toLower(std::string str);
void mergeSort(std::vector<Process>& v, int s, int e);
void mergeSortedIntervals(std::vector<Process>& v, int s, int m, int e);
std::string userInput(int *nQs, int *ageTks, bool *isIO, bool *isInteractive, int *ioTks);
std::vector<Process> readFile(std::string file);
void createSortedFile(std::vector<Process>& pList);
void createProcesses(std::vector<Process>& pList);
void softRealTime(std::vector<Process>& pList, bool isIO, int ioTicks);
void hardRealTime(std::vector<Process>& pList, bool isIO, int ioTicks);
void printVector(std::vector<Process>& pList);
void freeVector(std::vector<Process>& v);

int main()
{ 
  int numQueues = 0;
  int ageTicks = 0;
  int ioTicks = 0;
  bool isInteractive = false;
  bool isIO = false;
  std::vector<Process> pList;
  std::string scheduler;

  scheduler = userInput(&numQueues, &ageTicks, &isInteractive, &isIO, &ioTicks);

  std::cout<<numQueues<<std::endl<<ageTicks<<std::endl<<ioTicks<<std::endl
    <<isInteractive<<std::endl<<isIO<<std::endl;

  if (!isInteractive) // If not interactive, must read from file.
  {
    std::ifstream inputf;
    std::string file;
    std::cout<<"Please enter a file to read from: ";
    std::cin>>file;

    inputf.open(file);
    while (!inputf.is_open()) // Go until ifstream can open file, meaning file exists.
    {
      std::cout<<"Entered incorrect file name.\nPlease enter a file to read from: ";
      std::cin>>file;

      inputf.open(file);
    }
    // Since opened for testing, must close it.
    inputf.close();
    // Reads the file and creates Process vector.
    pList = readFile(file);
  }
  else
    createProcesses(pList);

  if (scheduler == "mfqs") // MFQS
  {
    std::cout<<"in mfqs\n";
  }
  else // RTS
  {
    mergeSort(pList, 0, pList.size() - 1); // Merge sort based on arrival time.
    std::cout<<"in rts: "<<scheduler<<std::endl;
    if (scheduler == "s")
      softRealTime(pList, isIO, ioTicks);
    else
      hardRealTime(pList, isIO, ioTicks);
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

/* 
* Asks for user input on certain variables for each scheduler type.
* Scheduler types are Hard and Soft RTS, and MLFQS.
*/
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

/* 
* Reads a file and cleans its input and writes it to another file. 
* Puts valid processes in pList vector.
*/
std::vector<Process> readFile(std::string file)
{
  int numProcesses = 0;
  int i;
  std::ifstream inputf;
  std::ofstream outputf;
  std::stringstream ss;
  std::string line, token;
  std::vector<Process> pList;

  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;

  auto t1 = high_resolution_clock::now();

  inputf.open(file);
  if (!inputf.is_open())
  {
    std::perror("file");
    exit(-1);
  }

  outputf.open("t"); // Creates temp file.
  if (!outputf.is_open())
  {
    std::perror("t");
    exit(1);
  }

  std::getline(inputf, line, '\n'); // Either is /n or /r. On Windows, new line char is \r
  outputf<<line;

  while (inputf.peek() != EOF) // Keep going until we have read all content in input.
  {
    std::getline(inputf, line, '\n');
    //input.get(); // Grabs the extra \t character at the end of the line.

    // If we do not find a negative char and are not first line in file.
    if (line.find('-') == std::string::npos)
    {
      //outputf<<std::endl; // FENCEPOST ISSUE.  Adds line only when we know process nonnegative.
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
      outputf<<line;
      numProcesses++; // Counts the number of processes.

      pList.push_back(p); // Put process in vector.
    }
  }
  // TODO: Remove, used for error checking
  std::cout<<"numProcesses = "<<numProcesses<<std::endl;
  inputf.close(); // Closing input file.
  outputf.close(); // Close temp.

  std::rename("t", "clean");

  auto t2 = high_resolution_clock::now();
  /* Getting number of milliseconds as an integer. */
  auto ms_int = duration_cast<milliseconds>(t2 - t1);

  /* Getting number of milliseconds as a double. */
  duration<double, std::milli> ms_double = t2 - t1;

  std::cout << ms_int.count() << "ms\n";
  std::cout << ms_double.count() << "ms\n";

  return pList;
}

/* Allows the user to create processes */
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
      pList.push_back(p);
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

void softRealTime(std::vector<Process>& pList, bool isIO, int ioTicks)
{

}

// CHECK MEMORY LEAKS LATER
/* Hard Real Time Scheduling */
void hardRealTime(std::vector<Process>& pList, bool isIO, int ioTicks)
{
  int tick = 0;
  int burst;
  std::vector<Process> ioQueue;

  while (pList.size() > 0) // Go until we are through every process
  {
    if (pList[0].getArrival() <= tick) // If arrival time is late or equal to clock tick.
    {
      while (pList[0].getBurst() > 0 && tick < pList[0].getDeadline()) // Execution time of process
      {
        tick++;
        pList[0].subtractBurst();
      }

      if (pList[0].getBurst() != 0) // If process did not finish, exit.
      {
        freeVector(pList);
        exit(-1);
      }
      pList.erase(pList.begin()); // Remove process from pList.
      continue; // Return back to the top to not incriment tick uneccessarily.
    }
    tick++;
    freeVector(pList);
  }
}

// The interval from [s to m] and [m+1 to e] in v are sorted
// The function will merge both of these intervals
// Such the interval from [s to e] in v becomes sorted
void mergeSortedIntervals(std::vector<Process>& pList, int s, int m, int e) 
{
  // Temp is used to temporary store the vector obtained by merging
  // Elements from [s to m] and [m+1 to e] in v
  std::vector<Process> temp;
  int i, j;
  i = s;
  j = m + 1;

  while (i <= m && j <= e) {
    if (pList[i].getArrival() <= pList[j].getArrival()) 
    {
      temp.push_back(pList[i]);
      ++i;
    }
    else 
    {
      temp.push_back(pList[j]);
      ++j;
    }
  }
  while (i <= m) 
  {
    temp.push_back(pList[i]);
        ++i;
  }
  while (j <= e) 
  {
    temp.push_back(pList[j]);
    ++j;
  }
  for (int i = s; i <= e; ++i)
    pList[i] = temp[i - s];

  freeVector(temp);
}

// The MergeSort function
// Sorts the array in the range [s to e] in v using
// Merge sort algorithm
void mergeSort(std::vector<Process>& pList, int s, int e) 
{
  if (s < e) {
    int m = (s + e) / 2;
    mergeSort(pList, s, m);
    mergeSort(pList, m + 1, e);
    mergeSortedIntervals(pList, s, m, e);
  }
}

/* Method to print any Process in a vector */
void printVector(std::vector<Process>& v)
{
  int i;

  std::cout<<"Pid"<<"\t"<<"Bst"<<"\t"<<"Arr"<<"\t"<<"Pri"<<"\t"<<"Dline"<<"\t"<<"I/O\n";
  for (int i = 0; i < v.size(); i++)
  {
    std::cout<<v.at(i).getPID()<<"\t"<<v.at(i).getBurst()<<"\t"<<v.at(i).getArrival()
    <<"\t"<<v.at(i).getPriority()<<"\t"<<v.at(i).getDeadline()<<"\t"<<v.at(i).getIO()<<std::endl;
  }
  std::cout<<std::endl;
}

/* Error checking method that puts all the contents of pList after it being sorted into a file */
void createSortedFile(std::vector<Process>& pList)
{
  int i;
  std::ofstream outputf;

  outputf.open("sortedList", std::ios::trunc | std::ios::out);
  if (!outputf.is_open())
  {
    std::perror("sortedList");
  }
  outputf<<"Pid"<<"\t"<<"Bst"<<"\t"<<"Arr"<<"\t"<<"Pri"<<"\t"<<"Dline"<<"\t"<<"I/O";

  for (i = 0; i < pList.size(); i++)
  {
    outputf<<"\n"<<pList[i].getPID()<<"\t"<<pList[i].getBurst()<<"\t"<<pList[i].getArrival()<<"\t"
      <<pList[i].getPriority()<<"\t"<<pList[i].getDeadline()<<"\t"<<pList[i].getIO();
  }
}

/* To remove memory, create empty vector and swap values of used vector with empty vector */
void freeVector(std::vector<Process>& v)
{
  std::vector<Process> empty;
  v.swap(empty);
}