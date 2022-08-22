#include <iostream>

using namespace std;
int main() {
    int grechka = 100;
    int month_counter = 0;
    while (grechka>0){
        grechka = grechka - 6;
        month_counter++;
        if (grechka > 0) {
            cout << "После " << month_counter << " У вас в запасе останется" << grechka << "Кг гречки\n";
        }
        if (grechka < 0){
            printf("После %d месяца не останется гречи(",month_counter);
        }
    }

    return 0;
}
