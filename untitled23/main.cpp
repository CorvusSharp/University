#include <iostream>
#include <cstring>
using namespace std;
#define SIZE_STACK 1000
#define SIZE_STR 100
#define SIZE_HTML 3000

class CustomStack {
private:
    int ind_last_elem;
protected:
    char **mData;
public:
    CustomStack() {
        ind_last_elem = -1;
        mData = new char *[SIZE_STACK];
        for (int i = 0; i < SIZE_STACK; ++i) {
            mData[i] = new char[SIZE_STR];
            mData[i][0];
        }
    }

    ~CustomStack() {
        for (int i = 0; i < SIZE_STACK; ++i) {
            delete mData[i];
        }
        delete[]mData;
    }

    void pop() {
        mData[ind_last_elem][0] = '\0';
        ind_last_elem--;
    }

    void push(const char *tg) {
        ind_last_elem++;
        strcpy(mData[ind_last_elem], tg);
    }

    char *top() {
        return mData[ind_last_elem];
    }

    bool empty() {
        return (ind_last_elem == -1);
    }

    size_t size() {
        return ind_last_elem + 1;
    };

    char **make_a(int k) {
        char **arr = new char *[size() + k];
        for (int i = 0; i < size(); ++i) {
            arr[i] = mData[i];
        }
    }
};


    int main() {
        char *html = new char[3000];
        char htmlBR[] = "br";
        char htmlHR[] = "hr";
        fgets(html, SIZE_HTML, stdin);
        if (html[strlen(html) - 1] == '\n') html[strlen(html) - 1] = '\0';
        CustomStack *stack = new CustomStack;
        int k = 0;
        char end = '/';
        int flag = 0;
        int index_of_str = 0, check_closing = 0, check_of_begin = 0;
        char *elem = new char[SIZE_STR];

        for (int i = 0; i < strlen(html); i++) {
            if (html[i] == '<') {
                check_of_begin = 1;
                continue;
            }
            if (check_of_begin == 0 && html[i] == '>') {
                cout << "wrong" << endl;
                return 0;
            }
            if (check_of_begin == 1) {
                if (html[i] == '>') {
                    check_of_begin = 0;
                    elem[index_of_str] = '\0';
                    index_of_str = 0;
                    if (check_closing == 0) {
                        if (strcmp(htmlBR, elem) == 0 || strcmp(htmlHR, elem) == 0) {
                            delete[]elem;
                            elem = new char[SIZE_STR];
                        } else {
                            (*stack).push(elem);
                            elem = new char[SIZE_STR];
                        }
                    } else {
                        if (strcmp((*stack).top(), elem + 1) != 0) {
                            cout << "wrong" << endl;
                            return 0;
                        } else {
                            delete[]elem;
                            (*stack).pop();
                            elem = new char[SIZE_STR];
                            check_closing = 0;
                        }
                    }
                } else {
                    if (html[i] == '<') {
                        cout << "wrong" << endl;
                        return 0;

                    } else {
                        if (index_of_str == 0 && html[i] == '/')check_closing = 1;
                        elem[index_of_str] = html[i];
                        index_of_str++;
                    }
                }
            }
        }
        if ((stack)->size() == 0)cout << "correct" << endl;
        else cout << "wrong" << endl;
        delete[]html;
        return 0;
    }


