#include <iostream>
#include <mutex>
#include <pthread.h>
using namespace std;

class int_ts{
	int num = 0;
	pthread_rwlock_t lk;

public:
	int_ts(){
		pthread_rwlock_init(&lk, NULL);
	}

	void assign(int a){
		pthread_rwlock_wrlock(&lk);
		num = a;
		pthread_rwlock_unlock(&lk);
	}
	int value(){
		pthread_rwlock_rdlock(&lk);
		int ret = num;
		pthread_rwlock_unlock(&lk);
		return ret;
	}
	int add(int x){
		pthread_rwlock_wrlock(&lk);
		num+=x;
		int ret = num;
		pthread_rwlock_unlock(&lk);
		return ret;
	}
};