#include <iostream>
#include <queue>
#include <mutex>  
using namespace std;

class queue_ts{
    queue<string> q;
    mutex m;

public:
    void push(string str){
        lock_guard<mutex> guard_obj (m);

        q.push(str);
    }
    string pop(){
        lock_guard<mutex> guard_obj (m);

        if(q.size() == 0){
						return "";
        }
				string str = q.front();
				q.pop();
        return str;
        
    }
    int size(){
        lock_guard<mutex> guard_obj (m);

        return q.size();
    }
};