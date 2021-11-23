#include <stdio.h>
#include <iostream>
#include <cctype>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <chrono>
#include <queue>

#define DEBUG

class Process 
{
  public:
    Process() // Default constructor to initialize Process arr[] in quicksort.
    {
      pid = 0, burst = 0, arrival = 0, priority = 0, deadline = 0, io = 0, initialBurst = 0, tickEntered = 0, timeQuantum =0;
    }
    int pid, burst, arrival, priority, deadline, io, initialBurst, tickEntered, timeQuantum;
  
    void setPID(int pid)
    {
      this->pid = pid;
    }
    void setBurst(int burst)
    {
      this->burst = burst;
      initialBurst = this->burst;
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
    void setTickEntered(int tick)
    {
      tickEntered = tick;
    }
    void setTimeQuantum(int timeQuantum)
    {
      this->timeQuantum = timeQuantum;
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
    int getInitialBurst()
    {
      return initialBurst;
    }
    int getTickEntered()
    {
      return tickEntered;
    }
    int getTimeQuantum()
    {
      return timeQuantum;
    }

    void subtractBurst()
    {
      burst--;
    }
    void subtractIO()
    {
      io--;
    }

    void addTimeQuantum()
    {
      timeQuantum++;
    }
};

std::string toLower(std::string str);
void mergeSort(std::vector<Process>& v, int s, int e, bool arrival);
void mergeSortedIntervalsByArrival(std::vector<Process>& v, int s, int m, int e);
void mergeSortedIntervalsByTick(std::vector<Process>& pList, int s, int m, int e);
std::string userInput(int *nQs, int *timeQuantum, int *ageTks, bool *isIO, bool *isInteractive, int *ioTks);
std::vector<Process> readFile(std::string file, bool isIO, int ioTicks);
void createSortedFile(std::vector<Process>& pList);
void createProcesses(std::vector<Process>& pList);
void softRealTime(std::vector<Process>& pList, bool isIO, int ioTicks);
void hardRealTime(std::vector<Process>& pList, bool isIO, int ioTicks);
void printVector(std::vector<Process>& pList);
void printQueue(std::queue<Process>& v);
void freeVector(std::vector<Process>& v);
void freeQueue(std::queue<Process>& q);
std::queue<Process> sortByPriority(std::vector<Process>& pList);
void multilevelFeedbackPriorityQueue(std::queue<Process>& pList, int numQueues, int timeQuantum, int ageTicks, bool isIO);
void topQueue(std::queue<Process>& higherQueue, std::queue<Process>& lowerQueue, std::queue<Process>& ioQueue, 
  std::queue<Process>& processList, int timeQuantum, int *tick, bool isIO, std::queue<int>& turnAroundTime);
void demoteQueue(std::queue<Process>& higherQueue, std::queue<Process>& lowerQueue, std::queue<Process>& ioQueue, 
  std::queue<Process>& processList, int timeQuantum, int *tick, bool isIO, std::queue<int>& turnAroundTime);
void FCFS(std::queue<Process>& fcfsQueue, std::queue<Process>& higherQueue, std::queue<Process>& ioQueue,
  std::queue<Process>& processList, int *tick, int ageTicks, int *ageTickCounter, bool isIO, std::queue<int>& turnAroundTime);
int getAverageWaitingTime(std::vector<Process>& pList);
int getAverageTurnaroundTime(std::queue<int>& pList);
bool decrementProcessIO(std::queue<Process>& ioQueue, std::queue<Process>& pList, int *tick);
void quickSort(Process arr[], int low, int high);
void arrSwap(Process* a, Process* b);
int partition(Process arr[], int low, int high);

int main()
{ 
  int numQueues = 0;
  int timeQuantum = 0;
  int ageTicks = 0;
  int ioTicks = 0;
  bool isInteractive = false;
  bool isIO = false;
  std::vector<Process> pList;
  std::string scheduler;

  // test(&apple);
  // std::cout<<apple;

  scheduler = userInput(&numQueues, &timeQuantum, &ageTicks, &isInteractive, &isIO, &ioTicks);
  if (!isInteractive) // If not interactive, must read from file.
  {
    std::ifstream inputf;
    std::string file;
    std::cout<<"Please enter a file to read from: ";
    std::getline(std::cin, file);

    inputf.open(file);
    while (!inputf.is_open()) // Go until ifstream can open file, meaning file exists.
    {
      std::cout<<"Entered incorrect file name.\nPlease enter a file to read from: ";
      std::getline(std::cin, file);

      inputf.open(file);
    }
    // Since opened for testing, must close it.
    inputf.close();
    // Reads the file and creates Process vector.
    pList = readFile(file, isIO, ioTicks);
  }
  else
    createProcesses(pList);

  if (scheduler == "mfqs") // MFQS
  {
      // Get Average Waiting Time
      int waitTime = getAverageWaitingTime(pList);
      // Start timer
      auto start = std::chrono::high_resolution_clock::now();
      mergeSort(pList, 0, pList.size() - 1, true);
      createSortedFile(pList);
      std::queue<Process> sortedList = sortByPriority(pList);
      multilevelFeedbackPriorityQueue(sortedList, numQueues, timeQuantum, ageTicks, isIO);
      // End Timer
      auto stop = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed_seconds = stop - start;
      std::cout << "Total Runtime: " << elapsed_seconds.count() << "s\n";
      std::cout << "Average Waiting Time: " << waitTime << " ticks\n";
  }
  else // RTS
  {
    mergeSort(pList, 0, pList.size() - 1, true); // Merge sort based on arrival time.
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
std::string userInput(int *numQueues, int *timeQuantum, int *ageTicks, bool *isInteractive, bool *isIO, int *ioTicks)
{
  std::string scheduler, nQs, timeQ, ageTks, interactive, io;
  // Start user input
  std::cout<<"MFQS or RTS? ";
  std::getline(std::cin, scheduler);
  while (scheduler.empty())
  {
    std::cout<<"Incorrect input.\nMFQS or RTS? ";
    std::getline(std::cin, scheduler);
  }
  scheduler = toLower(scheduler);
  // Go until we get the correct input
  while (scheduler.compare("mfqs") != 0 && scheduler.compare("rts") != 0)
  {
    std::cout<<"Incorrect input, must be \"mfqs\" or \"MFQS\" or \"rts\" or \"RTS\".\nMFQS or RTS? ";
    std::getline(std::cin, scheduler);
    while (scheduler.empty())
    {
      std::cout<<"Incorrect input.\nMFQS or RTS? ";
      std::getline(std::cin, scheduler);
    }
    scheduler = toLower(scheduler);
  }
  
  if (scheduler == "mfqs") // MFQS specific questions
  {
    std::cout<<"How many queues? ";
    std::getline(std::cin, nQs);
    while (nQs.empty())
    {
      std::cout<<"Incorrect input.\nHow many queues? ";
      std::getline(std::cin, nQs);
    }
    // Based on assignment need 2-5 queues.
    while (std::stoi(nQs) < 2 || std::stoi(nQs) > 5) // Go until correct user input. 
    {
      if (std::stoi(nQs) < 2)
      {
        std::cout<<"To few queues, number of queues needs to be > 2\nHow many queues? ";
      }
      else if (std::stoi(nQs) > 5)
      {
        std::cout<<"To many queues, number of queues needs to be < 5\nHow many queues? ";
      }
      else
      {
        std::cout<<"Incorrect input.\nHow many queues? ";
      }
      std::getline(std::cin, nQs);
      while (nQs.empty())
      {
        std::cout<<"Incorrect input.\nHow many queues? ";
        std::getline(std::cin, nQs);
      }
    }
    *numQueues = std::stoi(nQs);
    // Time quantum
    std::cout<<"What is the time quantum? ";
    std::getline(std::cin, timeQ);
    while (timeQ.empty())
    {
      std::cout<<"Incorrect input\nWhat is the time quantum? ";
      std::getline(std::cin, timeQ);
    }
    while (std::stoi(timeQ) < 1)
    {
      std::cout<<"Value for time quantum must be greater than 0\nWhat is the time quantum? ";
      std::getline(std::cin, timeQ);
      while (timeQ.empty())
      {
        std::cout<<"Incorrect input\nWhat is the time quantum? ";
        std::getline(std::cin, timeQ);
      }
    }
    *timeQuantum = std::stoi(timeQ);
    // Aging
    std::cout<<"How many clock ticks for aging? ";
    std::getline(std::cin, ageTks);
    while (ageTks.empty())
    {
      std::cout<<"Incorrect input.\nHow many clock ticks for aging? ";
      std::getline(std::cin, ageTks);
    }
    // Number of clock ticks for the process to move up a queue.
    while (std::stoi(ageTks) < 1) // Go until correct user input.
    {
      std::cout<<"Value for aging must be an integer greater than 0\n"
      <<"How many clock ticks for aging? ";
      std::getline(std::cin, ageTks);
      while (ageTks.empty())
      {
        std::cout<<"Incorrect input.\nHow many clock ticks for aging? ";
        std::getline(std::cin, ageTks);
      }
    }
    *ageTicks = std::stoi(ageTks);
  }
  else // RTS
  {
    std::cout<<"Soft or hard RTS (s/h)? ";
    std::getline(std::cin, scheduler);
    while (scheduler.empty())
    {
      std::cout<<"Incorrect input.\nSoft or hard RTS (s/h)? ";
      std::getline(std::cin, scheduler);
    }
    // Specific type of scheduler for RTS.
    while (scheduler != "s" && scheduler != "h") // Go until correct user input.
    {
      std::cout<<"Incorrect input.\nSoft or hard RTS (s/h)? ";
      std::getline(std::cin, scheduler);
      while (scheduler.empty())
      {
        std::cout<<"Incorrect input.\nSoft or hard RTS (s/h)? ";
        std::getline(std::cin, scheduler);
      }
    }
  }
  // Process creation
  std::cout<<"Would you like to create processes manually (y/n)? ";
  std::getline(std::cin, interactive);
  while (interactive.empty())
  {
    std::cout<<"Incorrect input.\nWould you like to create processes manually (y/n)? ";
    std::getline(std::cin, interactive);
  }
  while (interactive != "y" && interactive != "n")
  {
    std::cout<<"Incorrect input, please enter y or n.\nWould you like to create processes manually (y/n)? ";
    std::getline(std::cin, interactive);
    while (interactive.empty())
    {
      std::cout<<"Incorrect input.\nWould you like to create processes manually (y/n)? ";
      std::getline(std::cin, interactive);
    }
  }
  // Assigning interactive boolean.
  if (interactive == "y")
    *isInteractive = !*isInteractive;
  // I/O
  if (scheduler == "mfqs")
  {
     std::cout<<"Is there I/O (y/n)? ";
    std::getline(std::cin, io);
    while (io.empty())
    {
      std::cout<<"Incorrect input.\nIs there I/0 (y/n)? ";
      std::getline(std::cin, io);
    }
    while (io != "y" && io != "n")
    {
      std::cout<<"Incorrect input, please enter y or n.\nIs there I/O (y/n)? ";
      std::getline(std::cin, io);
      while (io.empty())
      {
        std::cout<<"Incorrect input.\nIs there I/0 (y/n)? ";
        std::getline(std::cin, io);
      }
    }
    // Assigning I/O boolean.
    if (io == "y")
    {
      *isIO = !*isIO;
    }
  }
  // Return what scheduler we are using.
  return scheduler;
}

/* 
* Reads a file and cleans its input and writes it to another file. 
* Puts valid processes in pList vector.
*/
std::vector<Process> readFile(std::string file, bool isIO, int ioTicks)
{
  int i;
  std::ifstream inputf;
  std::stringstream ss;
  std::string line, token;
  std::vector<Process> pList;

  inputf.open(file);
  if (!inputf.is_open())
  {
    std::perror("file");
    exit(-1);
  }

  std::getline(inputf, line, '\n'); // Either is /n or /r. On Windows, new line char is \r
  // std::cout<<line<<std::endl;
  while (inputf.peek() != EOF) // Keep going until we have read all content in input.
  {
    std::getline(inputf, line, '\n');
    // std::cout<<line<<std::endl;
    //input.get(); // Grabs the extra \t character at the end of the line.

    // If we do not find a negative char and are not first line in file.
    if (line.find('-') == std::string::npos)
    {
      Process p;
      ss.str(line); // Puts line in stringstream
      ss>>token;
      p.setPID(std::stoi(token));
      ss>>token;
      p.setBurst(std::stoi(token));
      ss>>token;
      p.setArrival(std::stoi(token));
      ss>>token;
      p.setPriority(std::stoi(token));
      ss>>token;
      p.setDeadline(std::stoi(token));
      ss>>token;
      p.setIO(std::stoi(token));
      if (isIO && p.getBurst() <= p.getIO() && p.getIO() > 0)
      {
        // std::cout<<"PID not added = "<<p.getPID()<<std::endl;
        ss.clear();
        continue;
      }
      
      ss.clear(); // After using the stream, clear to take more input.
      pList.push_back(p); // Put process in vector.
    }
  }
  inputf.close(); // Closing input file.

  return pList;
}

/* Allows the user to create processes */
void createProcesses(std::vector<Process>& pList)
{
  std::string numProcesses, burst, arrival, priority, deadline, io;
  int i;
  int totalProcesses = 0; // Guarenteed to be 1 process.
  int pid = 1;
  std::string moreProcesses = "y"; // FENCEPOST

  while (moreProcesses == "y") // Keep going until user no longer wants to create processes.
  {
    std::cout<<"How many processes would you like to create? ";
    std::getline(std::cin, numProcesses);
    while (numProcesses.empty())
    {
      std::cout<<"Number of processes must be greater than 0.\nHow many processes would you like to create? ";
      std::getline(std::cin, numProcesses);
    }
    while (std::stoi(numProcesses) <= 0)
    {
      std::cout<<"Number of processes must be greater than 0.\nHow many processes would you like to create? ";
      std::getline(std::cin, numProcesses);
      while (numProcesses.empty())
      {
        std::cout<<"Number of processes must be greater than 0.\nHow many processes would you like to create? ";
        std::getline(std::cin, numProcesses);
      }
    }

    for (i = 0; i < std::stoi(numProcesses); i++)
    {
      Process p;
      totalProcesses++;
      p.setPID(totalProcesses);
      // Burst
      std::cout<<"Process "<<totalProcesses<<"\'s burst time? ";
      std::getline(std::cin, burst);
      while (burst.empty())
      {
        std::cout<<"Incorrect input.\nProcess "<<totalProcesses<<"\'s burst time? ";
        std::getline(std::cin, burst);
      }
      while (std::stoi(burst) <= 0)
      {
        std::cout<<"Burst time must be greater than 0.\nProcess "<<totalProcesses<<"\'s burst time? ";
        std::getline(std::cin, burst);
        while (burst.empty())
        {
          std::cout<<"Incorrect input.\nProcess "<<totalProcesses<<"\'s burst time? ";
          std::getline(std::cin, burst);
        }
      }
      p.setBurst(std::stoi(burst));
      // Arrival
      std::cout<<"Process "<<totalProcesses<<"\'s arrival time? ";
      std::getline(std::cin, arrival);
      while (arrival.empty())
      {
        std::cout<<"Arrival time must be nonnegative.\nProcess "<<totalProcesses<<"\'s arrival time? ";
        std::getline(std::cin, arrival);
      }
      while (std::stoi(arrival) < 0)
      {
        std::cout<<"Arrival time must be nonnegative.\nProcess "<<totalProcesses<<"\'s arrival time? ";
        std::getline(std::cin, arrival);
        while (arrival.empty())
        {
          std::cout<<"Arrival time must be nonnegative.\nProcess "<<totalProcesses<<"\'s arrival time? ";
          std::getline(std::cin, arrival);
        }
      }
      p.setArrival(std::stoi(arrival));
      // Priority
      std::cout<<"Process "<<totalProcesses<<"\'s priority? ";
      std::getline(std::cin, priority);
      while (priority.empty())
      {
        std::cout<<"Process priority must be nonnegatve.\nProcess "<<totalProcesses<<"\'s priority? ";
        std::getline(std::cin, priority);
      }
      while (std::stoi(priority) < 0)
      {
        std::cout<<"Process priority must be nonnegatve.\nProcess "<<totalProcesses<<"\'s priority? ";
        std::getline(std::cin, priority);
        while (priority.empty())
        {
          std::cout<<"Process priority must be nonnegatve.\nProcess "<<totalProcesses<<"\'s priority? ";
          std::getline(std::cin, priority);
        }
      }
      p.setPriority(std::stoi(priority));
      // Deadline
      std::cout<<"Process "<<totalProcesses<<"\'s deadline? ";
      std::getline(std::cin, deadline);
      while (deadline.empty())
      {
        std::cout<<"Deadline must be a number greater than 0.\nProcess "<<totalProcesses<<"\'s deadline? ";
        std::getline(std::cin, deadline);
      }
      while (std::stoi(deadline) <= 0)
      {
        std::cout<<"Deadline must be a number greater than 0.\nProcess "<<totalProcesses<<"\'s deadline? ";
        std::getline(std::cin, deadline);
        while (deadline.empty())
        {
          std::cout<<"Deadline must be a number greater than 0.\nProcess "<<totalProcesses<<"\'s deadline? ";
          std::getline(std::cin, deadline);
        }
      }
      p.setDeadline(std::stoi(deadline));
      // IO
      std::cout<<"Process "<<totalProcesses<<"\'s I/O? ";
      std::getline(std::cin, io);
      while (io.empty())
      {
        std::cout<<"Process I/O must be nonnegatve.\nProcess "<<totalProcesses<<"\'s I/O? ";
        std::getline(std::cin, io);
      }
      while (std::stoi(io) < 0)
      {
        std::cout<<"Process I/O must be nonnegatve.\nProcess "<<totalProcesses<<"\'s I/O? ";
        std::getline(std::cin, io);
        while (deadline.empty())
        {
          std::cout<<"Process I/O must be nonnegatve.\nProcess "<<totalProcesses<<"\'s I/O? ";
          std::getline(std::cin, io);
        }
      }
      p.setIO(std::stoi(io));
      pList.push_back(p);
      std::cout<<std::endl;
    }

    std::cout<<"Would you like to create more processes (y/n)? ";
    std::getline(std::cin, moreProcesses);
    while (moreProcesses.empty())
    {
      std::cout<<"Incorrect input.\nWould you like to create more processes (y/n)? ";
      std::getline(std::cin, moreProcesses);
    }
    while (moreProcesses != "y" && moreProcesses != "n")
    {
      std::cout<<"Incorrect input.\nWould you like to create more processes (y/n)? ";
      std::getline(std::cin, moreProcesses);
      while (moreProcesses.empty())
      {
        std::cout<<"Incorrect input.\nWould you like to create more processes (y/n)? ";
        std::getline(std::cin, moreProcesses);
      }
    }
  } 
}
// Key for ordering least to greatest priority
struct less_than_key
{
    inline bool operator() (const Process struct1, const Process struct2)
    {
        return (struct1.priority < struct2.priority);
    }
};

std::queue<Process> sortByPriority(std::vector<Process>& pList) {
    std::vector<Process> sameArrival;
    std::queue<Process> prioSorted;
    int index = 0;
    int arr = 0;
    while(prioSorted.size() != pList.size()) {
        if(index != pList.size() && pList[index].getArrival() == arr) {
            sameArrival.push_back(pList[index]);
            index++;
        }
        else {
            std::sort(sameArrival.begin(), sameArrival.end(), less_than_key());
            for (int i = 0; i < sameArrival.size(); i++) {
                prioSorted.push(sameArrival[i]);
            }
            freeVector(sameArrival);
            arr++;
        }
    }
    freeVector(pList);
    return prioSorted;
}

void multilevelFeedbackPriorityQueue(std::queue<Process>& pList, int numQueues, int timeQuantum, int ageTicks, bool isIO) {
  int tick = 0;
  int ageTickCounter = 0;
  // Create RR Queues needed
  std::queue<Process> higherQueue = pList;
  freeQueue(pList); // higherQueue holds values of pList, so free it.
  std::queue<Process> lowerQueue1;
  std::queue<Process> lowerQueue2;
  std::queue<Process> lowerQueue3;
  std::queue<Process> ioQueue;
  std::queue<int> turnAroundTime;

  // Create FCFS Queue
  std::queue<Process> finalQueue;

  // Schedule the processes
  // higherQueue = queue for the 1st queue. pList represents processes that came from I/O.
  while (higherQueue.size() > 0 || pList.size() > 0) // Keep going till higherQueue is filled or pList.
  {
    switch (numQueues) {
    case 2:
      #ifdef DEBUG
        std::cout<<"\nEntering queue 1\n";
      #endif
      topQueue(higherQueue, finalQueue, ioQueue, pList, timeQuantum, &tick, isIO, turnAroundTime);
      if (pList.size() == 0)
      {
        #ifdef DEBUG
          std::cout<<"\nEntering queue 2\n";
        #endif
        FCFS(finalQueue, higherQueue, ioQueue, pList, &tick, ageTicks, &ageTickCounter, isIO, turnAroundTime);
      }
      std::cout<<"in switch case: higherQueue size = "<<higherQueue.size()<<", finalQueue size = "
        <<finalQueue.size()<<", ioQueue size = "<<ioQueue.size()<<std::endl<<", pList size = "<<pList.size()<<std::endl;
      // Case where we have an extra processes in ioQueue and none in other queues.
      if (ioQueue.size() > 0 && higherQueue.size() == 0 && finalQueue.size() == 0 && pList.size() == 0)
      {
        std::cout<<"\nentered if in switch case\n";
        while (pList.size() == 0)
        {
          decrementProcessIO(ioQueue, pList, &tick);
        }
      }
      break;
    case 3:
      #ifdef DEBUG
        std::cout<<"\nEntering queue 1\n";
      #endif
      topQueue(higherQueue, lowerQueue1, ioQueue, pList, timeQuantum, &tick, isIO, turnAroundTime);
      if (pList.size() == 0)
      {
        #ifdef DEBUG
          std::cout<<"\nEntering queue 2\n";
        #endif
        demoteQueue(lowerQueue1, finalQueue, ioQueue, pList, (timeQuantum * 2), &tick, isIO, turnAroundTime);
      }
      if (pList.size() == 0)
      {
        #ifdef DEBUG
          std::cout<<"\nEntering queue 3\n";
        #endif
        FCFS(finalQueue, higherQueue, ioQueue, pList, &tick, ageTicks, &ageTickCounter, isIO, turnAroundTime);
      }
      // std::cout<<"in switch case: higherQueue size = "<<higherQueue.size()<<", lowerQueue1 size = "<<lowerQueue1.size()
      //   <<", finalQueue size = "<<finalQueue.size()<<", ioQueue size = "<<ioQueue.size()<<std::endl
      //   <<", pList size = "<<pList.size()<<std::endl;
      // Case where we have an extra processes in ioQueue and none in other queues.
      if (ioQueue.size() > 0 && higherQueue.size() == 0 && lowerQueue1.size() == 0 &&
        finalQueue.size() == 0 && pList.size() == 0)
      {
        std::cout<<"\nentered if in switch case\n";
        while (pList.size() == 0)
        {
          decrementProcessIO(ioQueue, pList, &tick);
        }
      }
      break;
    case 4:
      #ifdef DEBUG
        std::cout<<"\nEntering queue 1\n";
      #endif
      topQueue(higherQueue, lowerQueue1, ioQueue, pList, timeQuantum, &tick, isIO, turnAroundTime);
      if (pList.size() == 0)
      {
        #ifdef DEBUG
          std::cout<<"\nEntering queue 2\n";
        #endif
        demoteQueue(lowerQueue1, lowerQueue2, ioQueue, pList, (timeQuantum * 2), &tick, isIO, turnAroundTime);
      }
      if (pList.size() == 0)
      {
        #ifdef DEBUG
          std::cout<<"\nEntering queue 3\n";
        #endif
        demoteQueue(lowerQueue2, finalQueue, ioQueue, pList, (timeQuantum * 4), &tick, isIO, turnAroundTime);
      }
      if (pList.size() == 0)
      {
        #ifdef DEBUG
          std::cout<<"\nEntering queue 4\n";
        #endif
        FCFS(finalQueue, higherQueue, ioQueue, pList, &tick, ageTicks, &ageTickCounter, isIO, turnAroundTime);
      }
      std::cout<<"in switch case: higherQueue size = "<<higherQueue.size()<<", lowerQueue1 size = "<<lowerQueue1.size()
        <<", lowerQueue2 size = "<<lowerQueue2.size()<<", finalQueue size = "<<finalQueue.size()<<", ioQueue size = "
        <<ioQueue.size()<<std::endl<<", pList size = "<<pList.size()<<std::endl;
      // Case where we have an extra processes in ioQueue and none in other queues.
      if (ioQueue.size() > 0 && higherQueue.size() == 0 && lowerQueue1.size() == 0 &&
        lowerQueue2.size() == 0 && finalQueue.size() == 0 && pList.size() == 0)
      {
        std::cout<<"\nentered if in switch case\n";
        while (pList.size() == 0)
        {
          decrementProcessIO(ioQueue, pList, &tick);
        }
      }
      break;
    case 5:
      #ifdef DEBUG
        std::cout<<"\nEntering queue 1\n";
      #endif
      topQueue(higherQueue, lowerQueue1, ioQueue, pList, timeQuantum, &tick, isIO, turnAroundTime);
      if (pList.size() == 0)
      {
        #ifdef DEBUG
          std::cout<<"\nEntering queue 2\n";
        #endif
        demoteQueue(lowerQueue1, lowerQueue2, ioQueue, pList, (timeQuantum * 2), &tick, isIO, turnAroundTime);
      }
      if (pList.size() == 0)
      {
        #ifdef DEBUG
          std::cout<<"\nEntering queue 3\n";
        #endif
        demoteQueue(lowerQueue2, lowerQueue3, ioQueue, pList, (timeQuantum * 4), &tick, isIO, turnAroundTime);
      }
      if (pList.size() == 0)
      {
        #ifdef DEBUG
          std::cout<<"\nEntering queue 4\n";
        #endif
        demoteQueue(lowerQueue3, finalQueue, ioQueue, pList, (timeQuantum * 8), &tick, isIO, turnAroundTime);
      }
      if (pList.size() == 0)
      {
        #ifdef DEBUG
          std::cout<<"\nEntering queue 5\n";
        #endif
        FCFS(finalQueue, higherQueue, ioQueue, pList, &tick, ageTicks, &ageTickCounter, isIO, turnAroundTime);
      }
      // std::cout<<"in switch case: higherQueue size = "<<higherQueue.size()<<", lowerQueue1 size = "<<lowerQueue1.size()
      //   <<", lowerQueue2 size = "<<lowerQueue2.size()<<", finalQueue size = "<<", lowerQueue3 size = "<<lowerQueue3.size()
      //   <<finalQueue.size()<<", ioQueue size = "<<ioQueue.size()<<std::endl<<", pList size = "<<pList.size()<<std::endl;
      // Case where we have an extra processes in ioQueue and none in other queues.
      if (ioQueue.size() > 0 && higherQueue.size() == 0 && lowerQueue1.size() == 0 &&
        lowerQueue2.size() == 0 && lowerQueue3.size() == 0 && finalQueue.size() == 0 && pList.size() == 0)
      {
        std::cout<<"\nentered if in switch case\n";
        while (pList.size() == 0)
        {
          decrementProcessIO(ioQueue, pList, &tick);
        }
      }
      break;
    }
  }
  int avgTurnAroundTime = getAverageTurnaroundTime(turnAroundTime);
  std::cout << "Average Turnaround Time: " << avgTurnAroundTime << " ticks\n";
}

// This method is for the first queue in MFQS
void topQueue(std::queue<Process>& higherQueue, std::queue<Process>& lowerQueue, 
    std::queue<Process>& ioQueue, std::queue<Process>& pList, int timeQuantum, int *tick, 
    bool isIO, std::queue<int>& turnAroundTime)
{
  int i, j, k, index, queueSize, pListSize; // queueSize is used for when popping off elements and need to for loop for correct #.
  int pTimeQuantumValue;
  bool broke = false; // Check for if we left the "CPU".
  // std::cout<<"higherQueue size = "<<higherQueue.size()<<std::endl
  //   <<"lowerQueue size = "<<lowerQueue.size()<<std::endl<<"ioQueue size = "
  //   <<ioQueue.size()<<std::endl<<"pList size = "<<pList.size()<<std::endl;
  
  // Go until we are through every process or have processes in waiting queue to enter higherQueue
  while (higherQueue.size() > 0 || pList.size() > 0)
  {
    // Check for processes that left ioQueue before running processes in "CPU".
    // If more than two processes in pList, need to sort them by tickEntered and potentially priority.
    if (pList.size() > 1)
    {
      // std::cout<<"entering pList > 1 in topQueue\n";
      std::vector<Process> temp, priorityTemp;
      std::queue<Process> higherQueueTemp;
      // std::cout<<"after Process arr\n";
      int highestPriority; // Highest priority means lower number (UNIX way of priority).
      pListSize = pList.size(); //DONT DELETE THIS
      // std::cout<<"after queueSize\n";

      for (i = 0; i < pListSize; i++) // Putting elements in pList inside temp queue so it can be sorted by tickEntered.
      {
        temp.push_back(pList.front());
        pList.pop();
      }
      // std::cout<<"before mergeSort\n";
      // for (i = 0; i < pListSize; i++) // TODO: Remove later
      // {
      //   std::cout<<"PID = "<<temp[i].getPID()<<" tickEntered = "<<temp[i].getTickEntered()<<" priority = "<<temp[i].getPriority()<<std::endl;
      // }
      mergeSort(temp, 0, pListSize - 1, false); // sorting by tickEntered.
      // std::cout<<"\nafter mergeSort\n";
      // for (i = 0; i < queueSize; i++) // TODO: Remove later
      // {
      //   std::cout<<"PID = "<<temp[i].getPID()<<" tickEntered = "<<temp[i].getTickEntered()<<" priority = "<<temp[i].getPriority()<<std::endl;
      // }
      // Double loop structure to see every index in vector temp and find
      // which Processes entered I/O at the same tick.
      for (i = 0; i < pListSize; i++)
      {
        for (j = i + 1; j < pListSize; j++)
        {
          if (temp[i].getTickEntered() == temp[j].getTickEntered()) // If entered at the same tick, add to priorityTemp
          {
            // std::cout<<"in temp[i] == temp[j]\n";
            if (priorityTemp.size() == 0) // Only for first find.
            {
              // std::cout<<"in first push\n";
              priorityTemp.push_back(temp[i]);
            }
            // std::cout<<"in push temp[j]\n";
            priorityTemp.push_back(temp[j]);
          }
          else // Didn't find another Process with the same tick X values in front, so leave inner loop.
          {
            break;
          }
        }
        // If in that iteration we have more than two processes that entered at
        // the same tick, sort by priority.
        if (priorityTemp.size() > 1)
        {
          index = 0;
          std::sort(priorityTemp.begin(), priorityTemp.end(), less_than_key()); // Sort by priority, 0 is highest.
          // Put Processes back in sorted order into temp.
          for (k = i; k < j; k++)
          {
            temp[k] = priorityTemp[index];
            index++;
          }
          freeVector(priorityTemp); // No longer need it, so free it.
          i += j - 1; // Skip some indexes to make abstraction faster. j - 1 since extra i++.
        }
      }
      // Putting all elements in higherQueue into its temp so we can put processes leaving
      // the IO queue into the higherQueue first
      queueSize = higherQueue.size();
      for (i = 0; i < queueSize; i++)
      {
        higherQueueTemp.push(higherQueue.front());
        higherQueue.pop();
      }
      // Put sorted vector into queue.
      queueSize = temp.size();
      for (i = 0; i < queueSize; i++)
      {
        // std::cout<<"PID = "<<temp[i].getPID()<<" tickEntered = "<<temp[i].getTickEntered()<<" priority = "<<temp[i].getPriority()<<std::endl;
        higherQueue.push(temp[i]);
      }
      freeVector(temp); // No longer need it, so free it.
      // Reput all elements in that were in higherQueue back into it.
      queueSize = higherQueueTemp.size();
      for (i = 0; i < queueSize; i++)
      {
        higherQueue.push(higherQueueTemp.front());
        higherQueueTemp.pop();
      }
    }
    else if (pList.size() == 1) // No need to check tickEntered and priority
    {
      // std::cout<<"entered pList == 1\n";
      std::queue<Process> higherQueueTemp;
      queueSize = higherQueue.size();
      for (i = 0; i < queueSize; i++)
      {
        // std::cout<<"entered first for\n";
        higherQueueTemp.push(higherQueue.front());
        higherQueue.pop();
      }
      higherQueue.push(pList.front());
      pList.pop();
      for (i = 0; i < queueSize; i++)
      {
        // std::cout<<"entered second for loop\n";
        higherQueue.push(higherQueueTemp.front());
        higherQueueTemp.pop();
      }
    }
    // Where we are running processes in "CPU".
    if (higherQueue.front().getArrival() <= *tick) // If arrival time is late or equal to clock tick.
    {
      // std::cout<<"Process "<<higherQueue.front().getPID()<<" arrived in CPU with timeQuantum "<<higherQueue.front().getTimeQuantum()<<std::endl;
      broke = false;
      for (i = higherQueue.front().getTimeQuantum(); i < timeQuantum; i++) // Giving process "CPU" for time quantum.
      {
        *tick = *tick + 1;
        higherQueue.front().subtractBurst();
        higherQueue.front().addTimeQuantum();
        if (isIO)
        {
          broke = decrementProcessIO(ioQueue, pList, tick);
          // If process burst = 1, we put it into ioQueue if it has any.
          if (higherQueue.front().getBurst() == 1 && higherQueue.front().getIO() != 0)
          {
            #ifdef DEBUG
              std::cout<<"Process "<<higherQueue.front().getPID()<<" entered ioQueue at tick "<<*tick<<std::endl;
            #endif
            higherQueue.front().setTimeQuantum(0);
            ioQueue.push(higherQueue.front());
            higherQueue.pop();
            broke = true;
            // pTimeQuantumValue = 0;
            break;
          }
        }
        // If process in queue is <= 0 it finished, so pop it off, get turnaround time, and leave for loop.
        if (higherQueue.front().getBurst() <= 0) 
        {
          #ifdef DEBUG
            std::cout<<"Process "<<higherQueue.front().getPID()<<" finished at tick "<<*tick<<" with burst "<<higherQueue.front().getBurst()<<std::endl;
          #endif
          turnAroundTime.push((*tick - higherQueue.front().getArrival()));
          higherQueue.pop();
          // pTimeQuantumValue = 0;
          broke = true;
          break;
        }

        if (broke)
          break;
      }
      // If we did not break from the loop, the process did not finish or enter IO, so push it to next queue.
      if (!broke) 
      {
        higherQueue.front().setTimeQuantum(0);
        lowerQueue.push(higherQueue.front());
        higherQueue.pop();
      }
    }
    else
    {
      *tick = *tick + 1;
    }
  }
  std::cout<<"finished topQueue\n";
  // std::cout<<"higherQueue size = "<<higherQueue.size()<<std::endl
  //   <<"lowerQueue size = "<<lowerQueue.size()<<std::endl<<"ioQueue size = "
  //   <<ioQueue.size()<<std::endl<<"pList size = "<<pList.size()<<std::endl;
  // freeVector(topQueue);
}

// Used for subtracting burst times and demoting processes to a lower queue
void demoteQueue(std::queue<Process>& higherQueue, std::queue<Process>& lowerQueue, 
    std::queue<Process>& ioQueue, std::queue<Process>& pList, int timeQuantum, int *tick, 
    bool isIO, std::queue<int>& turnAroundTime)
{
  int i; // queueSize is used for when popping off elements and need to for loop for correct #.
  bool broke = false;
  // std::cout<<"higherQueue size = "<<higherQueue.size()<<std::endl
  //   <<"lowerQueue size = "<<lowerQueue.size()<<std::endl<<"ioQueue size = "
  //   <<ioQueue.size()<<std::endl<<"pList size = "<<pList.size()<<std::endl;
  
  while (higherQueue.size() > 0) // Go until we are through every process
  {
    if (higherQueue.front().getArrival() <= *tick) // If arrival time is late or equal to clock tick.
    {
      // std::cout<<"Process "<<higherQueue.front().getPID()<<" arrived in CPU with timeQuantum "<<higherQueue.front().getTimeQuantum()<<std::endl;
      broke = false;
      for (i = higherQueue.front().getTimeQuantum(); i < timeQuantum; i++) // Giving process "CPU" for time quantum.
      {
        *tick = *tick + 1;
        higherQueue.front().subtractBurst();
        if (isIO)
        {
          broke = decrementProcessIO(ioQueue, pList, tick);
          // If process burst = 1, we put it into ioQueue if processes IO > 0.
          if (higherQueue.front().getBurst() == 1 && higherQueue.front().getIO() != 0)
          {
            #ifdef DEBUG
              std::cout<<"Process "<<higherQueue.front().getPID()<<" entered ioQueue at tick "<<*tick<<std::endl;
            #endif
            higherQueue.front().setTimeQuantum(0);
            ioQueue.push(higherQueue.front());
            higherQueue.pop();
            broke = true;
            break;
          }
        }
        // If process in queue is <= 0 it finished, so pop it off, get turnaround time, and leave for loop.
        if (higherQueue.front().getBurst() <= 0)
        {
          #ifdef DEBUG
            std::cout<<"Process "<<higherQueue.front().getPID()<<" finished at tick "<<*tick<<" with burst "<<higherQueue.front().getBurst()<<std::endl;
          #endif
          turnAroundTime.push((*tick - higherQueue.front().getArrival()));
          higherQueue.pop();
          broke = true;
          break;
        }

        if (broke)
          break;
      }
      // If we did not break from the loop, the process did not finish or enter IO, so push it to next queue.
      if (!broke)
      {
        higherQueue.front().setTimeQuantum(0);
        lowerQueue.push(higherQueue.front());
        higherQueue.pop();
      }
      if (pList.size() > 0)
      {
        std::cout<<"pList size > 0, so leaving demote queue to finish these processes\n";
        return;
      }
    }
    else 
    {
      *tick = *tick + 1;
    }
  }
  std::cout<<"finished demote queue\n";
  // std::cout<<"higherQueue size = "<<higherQueue.size()<<std::endl
  //   <<"lowerQueue size = "<<lowerQueue.size()<<std::endl<<"ioQueue size = "
  //   <<ioQueue.size()<<std::endl<<"pList size = "<<pList.size()<<std::endl;
  // freeVector(topQueue);
}

void FCFS(std::queue<Process>& fcfsQueue, std::queue<Process>& higherQueue, 
  std::queue<Process>& ioQueue, std::queue<Process>& pList, int *tick, int ageTicks,
  int *ageTickCounter, bool isIO, std::queue<int>& turnAroundTime)
{
  bool broke = false;
  // std::cout<<"higherQueue size = "<<higherQueue.size()<<std::endl
  //   <<"fcfsQueue size = "<<fcfsQueue.size()<<std::endl<<"ioQueue size = "
  //   <<ioQueue.size()<<std::endl<<"pList size = "<<pList.size()<<std::endl;
  
  while (*ageTickCounter != ageTicks && fcfsQueue.size() > 0) // Go until we are through every process
  {
    // std::cout<<"entered fcfs > 0\n";
    if (fcfsQueue.front().getArrival() <= *tick) // If arrival time is late or equal to clock tick.
    {
      broke = false;
      // std::cout<<"entered fcfs getArrical\n";
      // Giving the process "CPU" time till it finishes or leaves for IO.
      while (fcfsQueue.front().getBurst() != 0)
      {
        *tick = *tick + 1;
        if (*ageTickCounter != ageTicks) // Only incrememnt till we reach ageTick value.
        {
          *ageTickCounter = *ageTickCounter + 1;
          // std::cout<<"ageTickCounter = "<<*ageTickCounter<<std::endl;
        }
        fcfsQueue.front().subtractBurst();
        if (isIO)
        {
          decrementProcessIO(ioQueue, pList, tick);
          // If process burst = 1, we put it into ioQueue if processes IO > 0.
          if (fcfsQueue.front().getBurst() == 1 && fcfsQueue.front().getIO() != 0)
          {
            #ifdef DEBUG
              std::cout<<"Process "<<higherQueue.front().getPID()<<" entered ioQueue at tick "<<*tick<<std::endl;
              std::cout<<"Process time quantum inside FCFS = "<<higherQueue.front().getTimeQuantum()<<std::endl;
            #endif
            fcfsQueue.front().setTickEntered(*tick);
            std::cout<<"tickEntered = "<<fcfsQueue.front().getTickEntered()<<std::endl;
            ioQueue.push(fcfsQueue.front());
            // turnAroundTime.push((tick - fcfsQueue.front().getArrival())); // POTENTIALLY WRONG
            fcfsQueue.pop();
            broke = true;
            break;
          }
        }
      }
      if (!broke) // if we did not leave the "CPU" for IO, we finished process.
      {
        #ifdef DEBUG
          std::cout<<"Process "<<fcfsQueue.front().getPID()<<" finished at tick "<<*tick<<std::endl;
          std::cout<<"Process time quantum inside FCFS = "<<higherQueue.front().getTimeQuantum()<<std::endl;
        #endif
        turnAroundTime.push((*tick - fcfsQueue.front().getArrival()));
        fcfsQueue.pop();
        // break;
      }
      // If processes have aged enough, don't put another process in the "CPU" and leave while loop.
      if (*ageTickCounter >= ageTicks)
      {
        std::cout<<"do we enter ageTicks\n";
        break;
      }
      // If we had processes leave ioQueue, go back to the topQueue and return to FCFS later.
      if (pList.size() > 0)
      {
        std::cout<<"pList size > 0, so leaving fcfs queue to finish these processes\n";
        return;
      }
    }
    else
    {
      *tick = *tick + 1;
    }
  }
  // // If we had processes leave ioQueue, go back to the topQueue and return to FCFS later.
  // if (pList.size() > 0)
  // {
  //   std::cout<<"pList size > 0, so leaving fcfs queue to finish these processes\n";
  //   return;
  // }
  // We reached ageTick, so put each process into higherQueue to restart.
  while (fcfsQueue.size() > 0)
  {
    // std::cout<<"Process "<<fcfsQueue.front().getPID()<<" leaving FCFS with timeQuantum "<<fcfsQueue.front().getTimeQuantum()<<std::endl;
    higherQueue.push(fcfsQueue.front());
    fcfsQueue.pop();
  }
  std::cout << "left fcfs queue.\n";
  // std::cout<<"higherQueue size = "<<higherQueue.size()<<std::endl
  //   <<"fcfsQueue size = "<<fcfsQueue.size()<<std::endl<<"ioQueue size = "
  //   <<ioQueue.size()<<std::endl<<"pList size = "<<pList.size()<<std::endl;
  // freeVector(fcfsQueue);
}

bool decrementProcessIO(std::queue<Process>& ioQueue, std::queue<Process>& pList, int *tick)
{
  bool pLeftIO = false;
  int i, queueSize;
  queueSize = ioQueue.size(); // Size of queue.
  for (i = 0; i < queueSize; i++) // Decrement the IO of each process in IO queue.
  {
    ioQueue.front().subtractIO();
    // std::cout<<"Process "<<ioQueue.front().getPID()<<" io = "<<ioQueue.front().getIO()<<std::endl;
    if (ioQueue.front().getIO() == 0) // If IO is now 0, it finished, so push to pList.
    {
      #ifdef DEBUG
        std::cout<<"Process "<<ioQueue.front().getPID()<<" left ioQueue at tick "<<*tick<<std::endl;
      #endif
      ioQueue.front().setTickEntered(*tick);
      pList.push(ioQueue.front());
      if (!pLeftIO)
        pLeftIO = true;
    }
    else // If not finished, push back into queue.
    {
      ioQueue.push(ioQueue.front());
    }
    ioQueue.pop(); // Popping front to continue through the queue.
  }
  return pLeftIO;
}


void softRealTime(std::vector<Process>& pList, bool isIO, int ioTicks)
{

}

// CHECK MEMORY LEAKS LATER
/* Hard Real Time Scheduling */
void hardRealTime(std::vector<Process>& pList, bool isIO, int ioTicks)
{
  createSortedFile(pList); // TODO: Remove later
  int i;
  int lowestDeadline;
  // bool leftLoop = false;
  int tick = 0;
  std::vector<Process> queue;
  std::queue<Process> processList;
  for (i = 0; i < pList.size(); i++) // Convert pList to queue.
  {
    processList.push(pList[i]);
  }
  freeVector(pList);

  while (processList.size() > 0) // Go until we are through every process
  {
    if (queue.size() > 0 || processList.front().getArrival() <= tick) // If arrival time is late or equal to clock tick.
    {
      queue.push_back(processList.front());
      processList.pop();
      #ifdef DEBUG
        std::cout<<"queue[0] = "<<queue[0].getPID()<<std::endl;
        std::cout<<"processList front = "<<processList.front().getPID()<<std::endl;
      #endif
      while (queue[queue.size() - 1].getArrival() == processList.front().getArrival())
      {
        // std::cout<<"pList[0] = "<<pList[0].getPID()<<std::endl;
        queue.push_back(processList.front());
        processList.pop();
      }
      lowestDeadline = 0; // Set to 0 as initial value for finding smallest deadline.
      for (i = 1; i < queue.size(); i++) // Loop to find the smallest deadline.
      {
        if (queue[lowestDeadline].getDeadline() > queue[i].getDeadline())
        {
          lowestDeadline = i;
        }
      }
      // if (queue.size() > 1)
      // {
      //   mergeSort(queue, 0, queue.size() - 1);
      //   printVector(queue);
      // }
      //std::cout<<"pid = "<<pList[0].getPID()<<std::endl; // TODO: Remove later
      while (queue.size() > 0) // Execution time of process
      {
        tick++;
        queue[lowestDeadline].subtractBurst();
        #ifdef DEBUG
          std::cout<<"tick = "<<tick<<std::endl;
          std::cout<<"PID = "<<queue[lowestDeadline].getPID();
          std::cout<<" burst = "<<queue[lowestDeadline].getBurst()<<std::endl<<std::endl;
        #endif
        if (queue[lowestDeadline].getBurst() != 0 && queue[lowestDeadline].getDeadline() < tick) // If process did not finish, exit.
        {
          std::cout<<"Collision occured for process "<<queue[lowestDeadline].getPID()<<". Exiting\n";
          freeVector(queue);
          freeQueue(processList);
          exit(-1);
        }

        if (queue[lowestDeadline].getBurst() == 0) // If process in queue finished, pop it off
        {
          #ifdef DEBUG
            std::cout<<"Process "<<queue[lowestDeadline].getPID()<<" finished at tick "<<tick<<std::endl;
          #endif
          queue.erase(queue.begin() + lowestDeadline);
        }

        if (processList.front().getArrival() == tick) // If arrival of next process in queue = tick, leave inner loop.
        {
          break;
        }
      }
      for (i = 0; i < queue.size(); i++) // Look through the entire queue to find processes that didn't finish
      {
        if (queue[i].getBurst() != 0 && queue[i].getDeadline() < tick) // If process did not finish, exit.
        {
          std::cout<<"Collision occured for process "<<queue[i].getPID()<<". Exiting\n";
          freeVector(queue);
          freeQueue(processList);
          exit(-1);
        }
      }
      continue; // Return back to the top to not incriment tick uneccessarily.
    }

    tick++;
  }
  // freeVector(queue);
}

// The interval from [s to m] and [m+1 to e] in v are sorted
// The function will merge both of these intervals
// Such the interval from [s to e] in v becomes sorted
void mergeSortedIntervalsByArrival(std::vector<Process>& pList, int s, int m, int e) 
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

// The interval from [s to m] and [m+1 to e] in v are sorted
// The function will merge both of these intervals
// Such the interval from [s to e] in v becomes sorted
void mergeSortedIntervalsByTick(std::vector<Process>& pList, int s, int m, int e) 
{
  // Temp is used to temporary store the vector obtained by merging
  // Elements from [s to m] and [m+1 to e] in v
  std::vector<Process> temp;
  int i, j;
  i = s;
  j = m + 1;

  while (i <= m && j <= e) {
    if (pList[i].getTickEntered() <= pList[j].getTickEntered()) 
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
void mergeSort(std::vector<Process>& pList, int s, int e, bool byArrival) 
{
  if (s < e) {
    int m = (s + e) / 2;
    mergeSort(pList, s, m, byArrival);
    mergeSort(pList, m + 1, e, byArrival);
    if (byArrival)
    {
      mergeSortedIntervalsByArrival(pList, s, m, e);
    }
    else // By ticksEntered
    {
      // std::cout<<"entered by tick\n";
      mergeSortedIntervalsByTick(pList, s, m, e);
    }
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
// Destroys the queue to print it so USE SPARRINGLY
void printQueue(std::queue<Process>& v)
{
    int i;

    std::cout<<"Pid"<<"\t"<<"Bst"<<"\t"<<"Arr"<<"\t"<<"Pri"<<"\t"<<"Dline"<<"\t"<<"I/O\n";
    while(!v.empty()) {
        std::cout<<v.front().getPID()<<"\t"<<v.front().getBurst()<<"\t"<<v.front().getArrival()
        <<"\t"<<v.front().getPriority()<<"\t"<<v.front().getDeadline()<<"\t"<<v.front().getIO()<<std::endl;
        v.pop();
    }
    std::cout << std::endl;
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

void freeQueue(std::queue<Process>& q)
{
  std::queue<Process> empty;
  q.swap(empty);
}

int getAverageWaitingTime(std::vector<Process>& pList) {
    int waitingTime = 0;

    for (int i = 0; i < pList.size(); i++) {
        waitingTime = waitingTime + pList[i].getBurst();
    }
    waitingTime = waitingTime / pList.size();
    return waitingTime;
}

int getAverageTurnaroundTime(std::queue<int>& pList) {
    int size = pList.size();
    int turnAroundTime = 0;

    for (int i = 0; i < pList.size(); i++) {
        turnAroundTime = turnAroundTime + pList.front();
        pList.pop();
    }
    turnAroundTime = turnAroundTime / size;
    return turnAroundTime;
}
 
// /* The main function that implements QuickSort
//  arr[] --> Array to be sorted,
//   low  --> Starting index,
//   high  --> Ending index */
// void quickSort(Process arr[], int low, int high)
// {
//   if (low < high)
//   {
//     /* pi is partitioning index, arr[p] is now
//         at right place */
//     int pi = partition(arr, low, high);

//     // Separately sort elements before
//     // partition and after partition
//     quickSort(arr, low, pi - 1);
//     quickSort(arr, pi + 1, high);
//   }
// }

// // A utility function to swap two elements
// void arrSwap(Process* a, Process* b)
// {
//   Process t = *a;
//   *a = *b;
//   *b = t;
// }
 
// /* This function takes last element as pivot, places
//    the pivot element at its correct position in sorted
//     array, and places all smaller (smaller than pivot)
//    to left of pivot and all greater elements to right
//    of pivot */
// int partition(Process arr[], int low, int high)
// {
//   int pivot = arr[high].getTickEntered(); // pivot
//   int i = (low - 1); // Index of smaller element

//   for (int j = low; j <= high- 1; j++)
//   {
//     // If current element is smaller than or
//     // equal to pivot
//     if (arr[j].getTickEntered() <= pivot)
//     {
//       i++; // increment index of smaller element
//       arrSwap(&arr[i], &arr[j]);
//     }
//   }
//   arrSwap(&arr[i + 1], &arr[high]);
//   return (i + 1);
// }