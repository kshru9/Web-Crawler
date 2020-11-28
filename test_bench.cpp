#include <iostream>
using namespace std;
int main(){
	system("make test_targets threads=1");
	for(int i = 1; i<=100; i+=10){
		std::system(string("make threads=" + to_string(i)).c_str());
	}
	return 0;
}