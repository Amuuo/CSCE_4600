#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>
#include <iterator>
#include <queue>
#include <iomanip>
#include <cctype>

using namespace std;




struct Process {
  
  Process(){}
  Process(long long b, long long m) : burst_time{b}, mem_req{m} {}
  
  int       arrival_time;
  long long burst_time;
  long long mem_req;
};


struct Process_List {

  Process_List(){}
  Process_List(const vector<Process>& p_vec) : processes{p_vec}{} 

  static constexpr long long  max_burst_length = 50 * pow(10, 12);
  static constexpr long long  min_burst_length = 10 * pow(10, 6);
  static constexpr long long  max_mem_size     = 8  * pow(10, 9);
  long long                   min_mem_size     = 250000;
  
  uniform_int_distribution<long long> burst_range {min_burst_length, max_burst_length};
  uniform_int_distribution<long long> mem_range   {min_mem_size, max_mem_size};
  
  
  long long                   burst_mean       = 0;
  long long                   mem_mean         = 0;
  long long                   burst_dev        = 0;
  long long                   mem_dev          = 0;
  vector<Process>             processes;
  random_device               rd;


  void get_means();
  void get_devs();
  void generate_processes(int);
  void print_processes();
  void print_stats();
  void sort_by_burst();
  void sort_by_mem();
};


struct Processors {
  
  Processors(){}
  Processors(int size, float spd) : num_of_processors{size}, 
                                    speed{spd*1000000000}{}

  vector<Process_List> processor_queues;
  long long speed;
  int num_of_processors;



};





int main() {
  
  Process_List my_processes;
  Processors my_processors{5, 4.0};
  my_processes.generate_processes(200);
  my_processes.print_processes();
  my_processes.print_stats();
  my_processes.sort_by_burst();
  my_processes.print_processes();

  printf("\nNumber of Processors: %d", my_processors.num_of_processors);
  printf("\nProcessors speed:     %lld\n\n", my_processors.speed);

  return 0;
}








void Process_List::get_means() { 
  
  long long burst_sum = 0;
  long long mem_sum   = 0;

  for (auto& process : processes) {
    burst_sum += process.burst_time;
    mem_sum   += process.mem_req;
  }
  
  burst_mean = (burst_sum / processes.size());
  mem_mean   = (mem_sum   / processes.size());

}



void Process_List::get_devs() {

  long long burst_dev_sum = 0;
  long long mem_dev_sum   = 0;

  for (auto& process : processes) {
    burst_dev_sum += abs(burst_mean - process.burst_time);
    mem_dev_sum   += abs(mem_mean - process.mem_req);
  }

  burst_dev = (burst_dev_sum / processes.size());
  mem_dev   = (mem_dev_sum   / processes.size());
}



void Process_List::generate_processes(int size) {

  for (int i = 0; i < size; ++i) {
    processes.push_back({burst_range(rd), mem_range(rd)});
  }
  get_means();
  get_devs();
}



void Process_List::print_processes() {

  int i = 1;
  for (auto& process : processes) {
    printf("\n%4d:  Burst: %-15lld Mem: %-15lld", i++, process.burst_time, process.mem_req);    
  }
  printf("\n\n");
}


void Process_List::print_stats() {
  
  printf("\n\n");
  printf("Burst Mean:          %lld\n",   burst_mean);    
  printf("Burst Standard Dev:  %lld\n\n", burst_dev);
  printf("Memory Mean:         %lld\n",   mem_mean);      
  printf("Memory Standard Dev: %lld",     mem_dev);
  printf("\n\n");
}



void Process_List::sort_by_burst() {

  sort(processes.begin(), processes.end(), 
       [](const Process& p1, const Process& p2){
         return p1.burst_time > p2.burst_time;
      }
  );
}



void Process_List::sort_by_mem() {

  sort(processes.begin(), processes.end(), 
       [](const Process& p1, const Process& p2){
         return p1.mem_req > p2.mem_req;
      }
  );
}