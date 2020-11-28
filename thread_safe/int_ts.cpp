#include <iostream>
#include <mutex>  
using namespace std;

class int_ts{
	int num = 0;
	mutex m;

public:
	void assign(int a){
		lock_guard<mutex> guard_obj (m);
		num = a;
	}
	int value(){
		lock_guard<mutex> guard_obj (m);
		return num;
	}
	int add(int x){
		lock_guard<mutex> guard_obj (m);
		num+=x;
		return num;
	}
};