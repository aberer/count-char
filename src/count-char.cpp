#include "Counter.hpp"

#include <iostream>
#include <cassert>
#include <thread>
#include <vector>
#include <chrono>

using std::cerr;
using std::cout;
using std::endl; 
using std::vector;
using std::thread;

#define TIME std::chrono::system_clock


int main(int argc, char **argv)
{
  if(argc != 3 )
    {
      cerr << argv[0] << " fileName numReaders " << endl; 
      exit(0);
    }

  auto startTime = TIME::now();

  auto numThreads = std::atoi(argv[2]);

  auto cntrs = vector<Counter>{};
  for(int i =0 ; i < numThreads; ++i)
    cntrs.emplace_back(argv[1], i,numThreads);

  auto threads = vector<thread>{};
  auto fun = []( Counter &cnt) { cnt.count(); }; 
  
  for(int i = 0; i < numThreads; ++i)
    threads.push_back( thread( fun , std::ref(cntrs[i]))  );

  for(int i = 0; i < numThreads; ++i)
    threads.at(i).join();
  
  auto combinedCounter = Counter(argv[1],0,1);
  for(int i =0 ; i < numThreads; ++i)
    combinedCounter = combinedCounter + cntrs.at(i);

  cout << "parallel read took " << std::chrono::duration_cast<std::chrono::microseconds>( TIME::now() - startTime  ).count() << endl; 
  startTime = TIME::now();
  
  // auto controlCounter = Counter(argv[1],0,1);
  // controlCounter.count();

  // cout << "sequential read took " << std::chrono::duration_cast<std::chrono::microseconds>( TIME::now() - startTime  ).count() << endl;
  
  // assert(controlCounter == combinedCounter);

  combinedCounter.printResult();
}
