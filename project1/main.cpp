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
};






int main() {
  
  Process_List my_processes;
  my_processes.generate_processes(200);
  my_processes.print_processes();
  my_processes.print_stats();
  

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
}


void Process_List::print_stats() {
  
  printf("\n\nBurst Mean: %lld\nBurst Standard Dev: %lld\n", burst_mean, burst_dev);    
  printf("\nMemory Mean: %lld\nMemory Standard Dev: %lld\n\n", mem_mean, mem_dev);      
}