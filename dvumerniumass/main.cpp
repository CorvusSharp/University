#include "vector"
#include <iostream>

void task1(){
    int maxSum = 0,minSum = 0,iMax = 0, jMax = 0,minPos = -1;
    std::vector<int> vec = {-2,1,-3,4,-1,2,1,-5,4};
    int ans = vec[0];
    for (int i = 0; i < vec.size(); ++i) {
        maxSum += vec[i];
        int cur = maxSum - minSum;
        if(cur > ans){
            ans = cur;
            iMax = minPos +1;
            jMax = i;
        }
        if(maxSum< minSum){
            minSum = maxSum;
            minPos = i;
        }
    }
    std::cout<<iMax << "&" << jMax;
}
void task2(){
    std::vector<int> vec = {2, 7, 11, 15};
    int value = 17,i = 0,j = 0;
    for(int i1 = 0; i1<vec.size();++i1) {
        for (int j1 = i1 + 1; j1 < vec.size(); ++j1) {
            if (vec[i1] + vec[j1] == value) {
                i = vec[i1];
                j = vec[j1];
                break;
            }
        }
    }
    std::cout <<i<<" & "<< j;
}
void task3(){
    fla
    while
    std::vector<int> newVec;
    int tmp = 0;
    std::vector<int> vec = {10,2, 7, 11, 15};
    for (int i = 0; i < vec.size() - 1; i++) {
        for (int j = 0; j < vec.size() - i - 1; j++) {
            if(vec[j] >vec[j + 1]) {
                tmp = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = tmp;
            }
        }
    }
    for (int i : vec) {
        std::cout<<i<<" ";
    }
}
int main() {
   task3();
    return 0;
}
