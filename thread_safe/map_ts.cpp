#include <iostream>
#include <map>
#include <mutex>
using namespace std;

template<class Key, class Value>
class map_ts
{
    map<Key, Value> m;
    mutex m_lock;

public:
    const Value get(const Key k) {
        lock_guard<mutex> guard_obj(m_lock);
        return m[k];
    }

    void inc(const Key k) {
        lock_guard<mutex> guard_obj(m_lock);
        m[k] = m[k]+1;
    }
		
    map<Key, Value> value(){
				lock_guard<mutex> guard_obj(m_lock);
        return m;
    }
};