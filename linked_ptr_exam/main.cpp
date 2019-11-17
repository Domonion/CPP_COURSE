//
// Created by domonion on 25.02.19.
//
#include "linked_ptr.hpp"
#include <iostream>
using namespace std;
using namespace smart_ptr;
int main(){
    linked_ptr<int> a;
    linked_ptr<int> b(new int(3));
    a = b;
    return 0;
}
