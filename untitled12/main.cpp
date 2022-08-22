#include <iostream>
class Stack{
public:
    Stack(){
    i = 0;
    }
    void push(int val){
        arr[i] = val;
        i++;
    }
    void pop(){
        i--;
    }
    bool isempty(){
        if(i == 0) return true;
        else return false;

    }
    int top(){
        return arr[i-1];
    }
private:
    int arr[50];
    int i;
};
int main() {
    Stack st;
    st.push(10);
    std::cout << st.top() << std::endl;
    return 0;
}
