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

	string string1(10000000, '1');

	cout << "\n>> Writing to file" << endl;
	write_to_file(string1);
	cout << ">> Done writing text.txt" << endl;
	in = new ifstream{"text.txt"};


	cout << ">> Initializing threads" << endl;
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
		cout << ">> Thread 2 attempting to acquire lock1" << endl;
		lock_guard<mutex> lck{lock1};
		cout << ">> Thread 2 acquired lock1" << endl;
		while(!in->eof()) {
			cout << ">> Thread 2 attempting to acquire lock2" << endl;
			lock_guard<mutex> lck2{lock2};
			cout << ">> Thread 2 acquired lock2" << endl;
			s.push_back(in->get());
			cout << ">> Thread 2 releasing lock2" << endl;
		}
	}
	cout << ">> Thread 2 released lock1" << endl;
}

/* deadlocking function for thread 1 */
void read_from_file1() { 
	string s;
	{
		cout << ">> Thread 1 attempting to acquire lock2" << endl;
		lock_guard<mutex>lck{lock2};
		cout << ">> Thread 1 acquired lock2" << endl;
		while(!in->eof()) {
			cout << ">> Thread 1 attempting to acquire lock1" << endl;
			lock_guard<mutex> lck2{lock1};
			cout << ">> Thread 1 acquired lock1" << endl;
			s.push_back(in->get());
			cout << ">> Thread 1 releasing lock1" << endl;
		}
	}
	cout << ">> Thread 1 released lock2" << endl;
}
