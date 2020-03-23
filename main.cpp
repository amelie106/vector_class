#include<iostream>
#include"vector.h"

using namespace std;

int main () {

    Vector<int> v ({1,2,3,4,5,6});

    for (const auto& i:v);

    return 0;
}
