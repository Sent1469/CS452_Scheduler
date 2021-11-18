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
std::string userInput(int *nQs, int *timeQuantum, int *ageTks, bool *isIO, bool *isInteractive, int *ioTks);
std::vector<Process> readFile(std::string file, bool isIO, int ioTicks);
void createSortedFile(std::vector<Process>& pList);
void createProcesses(std::vector<Process>& pList);
void softRealTime(std::vector<Process>& pList, bool isIO, int ioTicks);
void hardRealTime(std::vector<Process>& pList, bool isIO, int ioTicks);
void printVector(std::vector<Process>& pList);
void freeVector(std::vector<Process>& v);
void multilevelFeedbackPriorityQueue(std::vector<Process>& pList, int numQueues, int timeQuantum);
void demoteQueue(std::vector<Process>& topQueue, std::vector<Process>& lowerQueue, int timeQuantum, int tick, int ageTicks);
void FCFS(std::vector<Process>& fcfsQueue, int timeQuantum, int tick);

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

  scheduler = userInput(&numQueues, &timeQuantum, &ageTicks, &isInteractive, &isIO, &ioTicks);

  std::cout<<numQueues<<std::endl<<timeQuantum<<std::endl<<ageTicks<<std::endl<<ioTicks<<std::endl
    <<isInteractive<<std::endl<<isIO<<std::endl;

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
      mergeSort(pList, 0, pList.size() - 1);
      //std::cout << "im in mfqs";
      multilevelFeedbackPriorityQueue(pList, numQueues, timeQuantum);
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

  std::getline(inputf, line, '\n'); // Either is /n or /r. On Windows, new line char is \r

  while (inputf.peek() != EOF) // Keep going until we have read all content in input.
  {
    std::getline(inputf, line, '\n');
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
      if (isIO && p.getBurst() < p.getIO() && p.getIO() > 0) // If
      {
        #if DEBUG
        {
          std::cout<<"PID not added = "<<p.getPID()<<std::endl;
        }
        ss.clear();
        continue;
      }
      
      ss.clear(); // After using the stream, clear to take more input.
      pList.push_back(p); // Put process in vector.
    }
  }
  inputf.close(); // Closing input file.

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

void multilevelFeedbackPriorityQueue(std::vector<Process>& pList, int numQueues, int timeQuantum) {
    int tick = 0;

    // Create RR Queues needed
    std::vector<Process> lowerQueue1;
    std::vector<Process> lowerQueue2;
    std::vector<Process> lowerQueue3;

    // Create FCFS Queue
    std::vector<Process> finalQueue;

    // Schedule the processes
    switch (numQueues) {
    case 2:
        demoteQueue(lowerQueue1, finalQueue, timeQuantum, tick, 2);
        break;
    case 3:
        demoteQueue(pList, lowerQueue1, timeQuantum, tick, 2);
        demoteQueue(lowerQueue1, finalQueue, (timeQuantum * 2), tick, 2);
        break;
    case 4:
        demoteQueue(pList, lowerQueue1, timeQuantum, tick, 2);
        demoteQueue(lowerQueue1, lowerQueue2, (timeQuantum * 2), tick, 2);
        demoteQueue(lowerQueue2, finalQueue, (timeQuantum * 4), tick, 2);
        break;
    case 5:
        demoteQueue(pList, lowerQueue1, timeQuantum, tick, 2);
        demoteQueue(lowerQueue1, lowerQueue2, (timeQuantum * 2), tick, 2);
        demoteQueue(lowerQueue2, lowerQueue3, (timeQuantum * 4), tick, 2);
        demoteQueue(lowerQueue3, finalQueue, (timeQuantum * 8), tick, 2);
        break;
    }
    freeVector(lowerQueue1);
    freeVector(lowerQueue2);
    freeVector(lowerQueue3);
}

// Used for subtracting burst times and demoting processes to a lower queue
void demoteQueue(std::vector<Process>& topQueue, std::vector<Process>& lowerQueue, int timeQuantum, int tick, int ageTicks) {
    bool broke = false;
    // Todo I/O
    while (topQueue.size() > 0) // Go until we are through every process
    {
        if (topQueue[0].getArrival() <= tick) // If arrival time is late or equal to clock tick.
        {       
                std::cout << tick;
                std::cout << "\n";
                //printVector(topQueue);
                //std::cout << "\n";
            
            for (int i = 0; i < timeQuantum; i++) {
                int burst = topQueue[0].getBurst() - 1;
                topQueue[0].setBurst(burst);
                if (topQueue[0].getBurst() <= 0) {
                    //std::cout << "\n erasing " + topQueue[i];
                    topQueue.erase(topQueue.begin());
                    tick++;
                    broke = true;
                    break;
                }
                tick++;
            }
            if (topQueue[0].getBurst() > 0 && !broke) {
                lowerQueue.push_back(topQueue[0]);
                topQueue.erase(topQueue.begin());
            }
            broke = false;
        } 
        else {
            tick++;
        }
    }
    std::cout << "\n leaving while \n";
    freeVector(topQueue);
}

void FCFS(std::vector<Process>& fcfsQueue, int timeQuantum, int tick) {
    while (fcfsQueue.size() > 0) // Go until we are through every process
    {
        if (fcfsQueue[0].getArrival() <= tick) // If arrival time is late or equal to clock tick.
        {
            for (int i = 0; i < timeQuantum; i++) {
                int burst = fcfsQueue[i].getBurst() - 1;
                fcfsQueue[i].setBurst(burst);
                if (fcfsQueue[i].getBurst() <= 0) {
                    fcfsQueue.erase(fcfsQueue.begin());
                    tick++;
                    continue;
                }
                tick++;
            }
        }
        else {
            tick++;
        }
    }
    freeVector(fcfsQueue);
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
  std::vector<Process> ioQueue;
  std::vector<Process> queue;
  //printVector(pList); // TODO: Remove later

  while (pList.size() > 0) // Go until we are through every process
  {
    if (queue.size() > 0 || pList[0].getArrival() <= tick) // If arrival time is late or equal to clock tick.
    {
      queue.push_back(pList[0]);
      std::cout<<"queue[0] = "<<queue[0].getPID()<<std::endl;
      pList.erase(pList.begin());
      std::cout<<"pList[0] before loop = "<<pList[0].getPID()<<std::endl;
      while (queue[queue.size() - 1].getArrival() == pList[0].getArrival())
      {
        // std::cout<<"pList[0] = "<<pList[0].getPID()<<std::endl;
        queue.push_back(pList[0]);
        pList.erase(pList.begin());
      }
      lowestDeadline = 0; // Set to 0 as initial value for finding smallest deadline.
      for (i = 1; i < queue.size(); i++) // Loop to find the smallest deadline.
      {
        if (queue[lowestDeadline].getDeadline() > queue[i].getDeadline())
        {
          lowestDeadline = i;
        }
      }
      // printVector(queue);
      //exit(-1);

      // if (queue.size() > 1)
      // {
      //   mergeSort(queue, 0, queue.size() - 1);
      //   printVector(queue);
      // }
      //std::cout<<"pid = "<<pList[0].getPID()<<std::endl; // TODO: Remove later
      while (queue.size() > 0) // Execution time of process
      {
        // if (isIO && pList[0].getBurst() == 1 && pList[0].getIO() > 0) // && pList[0].getBurst() > 0
        // {
        //   ioQueue.push_back(pList[0]);
        //   pList.erase(pList.begin());
        //   leftLoop = !leftLoop;
        //   std::cout<<"before break\n"; // TODO: Remove later
        //   break;
        // }

        tick++;
        #if DEBUG
        {

        }

        std::cout<<"tick = "<<tick<<std::endl; // TODO: Remove later
        std::cout<<"PID = "<<queue[lowestDeadline].getPID(); // TODO: Remove later
        queue[lowestDeadline].subtractBurst();
        if (queue[lowestDeadline].getBurst() != 0 && queue[lowestDeadline].getDeadline() < tick) // If process did not finish, exit.
        {
          #if DEBUG
          {
            std::cout<<"Collision occured. Exiting\n";
          }
          freeVector(pList);
          freeVector(ioQueue);
          freeVector(queue);
          exit(-1);
        }
        std::cout<<" burst = "<<queue[lowestDeadline].getBurst()<<std::endl<<std::endl; // TODO: Remove later

        if (queue[lowestDeadline].getBurst() == 0) // If process in queue finished, pop it off
        {
          #if DEBUG
          {
            std::cout<<"Process "<<queue[lowestDeadline].getPID()<<" finished at tick "<<tick<<std::endl;
          }
          queue.erase(queue.begin() + lowestDeadline);
        }

        if (pList[0].getArrival() == tick) // If arrival of next process in queue = tick, leave inner loop.
        {
          break;
        }
        // if (ioQueue.size() > 0)
        // {
        //   std::cout<<"in size > 0 inner\n"; // TODO: Remove later
        //   for (i = 0; i < ioQueue.size(); i++)
        //   {
        //     ioQueue[i].subtractIO();
        //     if (ioQueue[i].getIO() == 0)
        //     {
        //       std::cout<<"in io if while\n"; // TODO: Remove later
        //       if (ioQueue[i].getDeadline() > totalTicks)
        //       {
        //         std::cout<<"Collision occured. Exiting\n";
        //         freeVector(pList);
        //         freeVector(ioQueue);
        //         exit(-1);
        //       }

        //       pList.insert(pList.begin() + 1, ioQueue[i]); // Add Process one after the beginning.
        //       ioQueue.erase(ioQueue.begin() + i);
        //     }
        //   }
        // }
      }
      // std::cout<<"leftLoop = "<<leftLoop<<std::endl; // TODO: Remove later
      // if (!leftLoop)
      // {
      //   std::cout<<"entered !leftLoop\n"; // TODO: Remove later
      //   if (pList[0].getBurst() != 0) // If process did not finish, exit.
      //   {
      //     std::cout<<"Collision occured. Exiting\n";
      //     freeVector(pList);
      //     freeVector(ioQueue);
      //     exit(-1);
      //   }
      //   pList.erase(pList.begin()); // Remove process from pList.
      // }
      for (i = 0; i < queue.size(); i++) // Look through the entire queue
      {
        if (queue[i].getBurst() != 0 && queue[i].getDeadline() < tick) // If process did not finish, exit.
        {
          std::cout<<"Collision occured. Exiting\n";
          freeVector(pList);
          freeVector(ioQueue);
          freeVector(queue);
          exit(-1);
        }
      }
      // printVector(pList);
      // std::cout<<"before continue\n"; // TODO: Remove later
      // leftLoop = false;
      continue; // Return back to the top to not incriment tick uneccessarily.
    }

    tick++;
    // std::cout<<"tick = "<<tick<<std::endl;
    // if (ioQueue.size() > 0)
    // {
    //   std::cout<<"in size > 0 outer\n"; // TODO: Remove later
    //   for (i = 0; i < ioQueue.size(); i++)
    //   {
    //     ioQueue[i].subtractIO();
    //     if (ioQueue[i].getIO() == 0)
    //     {
    //       std::cout<<"in io if while\n"; // TODO: Remove later
    //       if (ioQueue[i].getDeadline() > totalTicks)
    //       {
    //         std::cout<<"Collision occured. Exiting\n";
    //         freeVector(pList);
    //         freeVector(ioQueue);
    //         exit(-1);
    //       }
    //       pList.insert(pList.begin(), ioQueue[i]);
    //       ioQueue.erase(ioQueue.begin() + i);
    //     }
    //   }
    // }
  }
  freeVector(queue);
  freeVector(pList);
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