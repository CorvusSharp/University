#include <iostream>
#include "vector"

void fun2(){
    float totalPrise = 0;
    std::vector<float> price = {2.5,4.25,3.0,10.0};
    std::vector<int> items = {1,1,0,3};
    for (int i : items) {
        totalPrise += price[i];
    }
    std::cout<<totalPrise;
}

void fun1(){
    int n,num;
    int delNum;
    std::cout<< "Input vector size: ";
    std::cin>>n;
    std::vector<int> vec(n);
    std::cout << "Input num: ";
    for (int i = 0; i < n; i++) {
        std::cin>>num;
        vec[i] = num;
    }
    std::cout<< "Input number to delete: ";
    std::cin>>delNum;
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] == delNum){
            for(int j = i; j < vec.size() - 1; j++){
                vec[j] = vec[j+1];
            }
            vec.pop_back();
            --i;
        }
    }

    for (int i: vec) {
        std::cout << i << " ";
    }
}
int main() {
fun2();
}
