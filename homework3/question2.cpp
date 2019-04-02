#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <mutex>

using namespace std;


ofstream* out;   //ofstream for sample file to eventually read from
ifstream* in;    //ifstream for reading when threads deadlock
mutex lock1;     //first mutex that threads will compete for
mutex lock2;     //second mutex that threads will compete for


void write_to_file(string);
void read_from_file2();
void read_from_file1();




/* ===========
	 M A I N
   ===========*/

int main() {

	out = new ofstream{"text.txt"};

	string string1(100000000, '1');

	write_to_file(string1);
	in = new ifstream{"text.txt"};


	thread thread1{read_from_file1};
	thread thread2{read_from_file2};

	thread1.join();
	thread2.join();

}




/* function writes a long text file that threads will eventually
   attempt to read from */
void write_to_file(string to_write) {

	for(auto& c : to_write)
		*out << c;
}

/* deadlocking fucntion for thread 2 */
void read_from_file2(){

	string s;
	{
		lock_guard<mutex> lck{lock1};
		while(!in->eof()) {
			lock_guard<mutex> lck2{lock2};
			s.push_back(in->get());
		}
	}
}

/* deadlocking function for thread 1 */
void read_from_file1() { 
	string s;
	{
		lock_guard<mutex>lck{lock2};
		while(!in->eof()) {
			lock_guard<mutex> lck2{lock1};
			s.push_back(in->get());
		}
	}
}
