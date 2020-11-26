#include <iostream>
#include <map>
#include <mutex>
using namespace std;

// pair<string, set<string>> a;
// a["web"].push_back("web1");

template <class Key, class Value>
class map_ts
{
	map<Key, Value> m;
	mutex m_lock;

public:
	const Value get(const Key k)
	{
		lock_guard<mutex> guard_obj(m_lock);
		return m[k];
	}

	void inc(const Key k)
	{
		lock_guard<mutex> guard_obj(m_lock);
		m[k] = m[k] + 1;
	}

	map<Key, Value> value()
	{
		lock_guard<mutex> guard_obj(m_lock);
		return m;
	}
	// Special call, only for map with Value as set
	void add(const Key k, string str)
	{
		lock_guard<mutex> guard_obj(m_lock);
		m[k].insert(str);
	}
};