#include <iostream>

using namespace ::std;
class CustomStack{
public:
    CustomStack(){
        mData = new int[5];
        sp = 0;
    }
    void push(int val){
    mData[sp++] = val;
    }

    void pop(){
        if (!empty()) sp--;
    }
    int top(){
        return mData[sp-1];
    }

    size_t size(){
        return sp;
    }

    bool empty(){
    if (sp == 0){
        return true;
    }
        return false;
    }

    void extend(int n){
        sp += n;
    }

protected:
    char** mData;
    int sp;
};
bool may(string &z) {
    try {
        stoi(z);
        return true;
    }
    catch (...){
        return false;
    }
}
int main() {
    CustomStack st;
    string z;

    while (cin >> z){
        if (may(z)){
            st.push(stoi(z));
        }
        else {
            if (z == "print") {
                cout << st.top();
            } else {


                int a = st.top();
                st.pop();
                int b = st.top();
                st.pop();
                if (z == "+") {
                    st.push( b + a);
                }
                if (z == "-") {
                    st.push(b- a);
                }
                if (z == "*") {
                    st.push( b * a);
                }
                if (z == "/") {
                    st.push(b / a);
                }
                if (z == "\n") break;

            }
        }
    }
    if (st.size() == 1) {
        cout << st.top();
    }
    else cout << "error";
    return 0;
}


