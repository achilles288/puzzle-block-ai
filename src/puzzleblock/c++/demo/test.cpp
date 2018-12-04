#include <iostream>
#include <vector>

using namespace std;

void func(vector<int>& vec) {
  vec.push_back(1);
}

vector<int> v;

int main() {
  cout << v.size() << endl;
  func(v);
  cout << v.size() << endl;
}
