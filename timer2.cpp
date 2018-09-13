#include <cstdlib>
#include <string>
#include <cstdio>
#include <chrono>
#include <iostream>

int main(int argc,const char * argv[]) {
	using namespace std;
    std::string cmd;
	for(int i = 1; i < argc; i++) {
		cmd += argv[i];
		cmd += ' ';
	}
	printf("cmd: %s\n",cmd.c_str());
	
	auto stating_time = chrono::high_resolution_clock::now();
	system(cmd.c_str());
	auto end = chrono::high_resolution_clock::now();    
    auto dur = end - stating_time;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	cout << "time: " << ms << "ms\n";
}