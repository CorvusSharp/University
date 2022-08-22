#include <iostream>

void fun3(){
    std::string str;
    int wordCounter = 0;
    while(1){
        std::cout<<"Puts your string:\n";
        std::getline(std::cin, str);
        if(str.length() < 100){
            break;
        } else puts("Your string > 100 sym.");
    }

    for (int i = 0; i < str.length(); ++i){
        if(str[i] == ' ' && str[i+1] != ' '){
         wordCounter++;
        }
    }
    std::cout<<"4islo slov: "<<wordCounter;
}
void fun1() {
    int hoursDeparture, minutesDeparture, hoursArrival, minutesArrival;
    std::string departure;
    std::string arrival;
    bool timeCheck = true;

    while (timeCheck) {
        std::cout << "Puts time departure into format HH::MM: \n";
        std::cin >> departure;
        if (departure.length() != 5 || departure[2] != ':' || departure[0] < '0' || departure[0] > '9' ||
            departure[1] < '0' || departure[1] > '9' || departure[3] > '9' ||
            departure[3] < '0' || departure[4] < '0' || departure[4] > '9' ||
            (departure[0] == '2' && departure[1] > '4')) {
            puts("you entered the wrong time");
        } else timeCheck = false;
    }
    timeCheck = true;
    while (timeCheck) {
        std::cout << "puts Time arrival into format HH::MM: \n";
        std::cin >> arrival;
        (arrival.length() != 5 || arrival[2] != ':' ||
         arrival[0] < '0' || arrival[0] > '2' ||
         arrival[1] < '0' || arrival[1] > '9'  || arrival[3] > '9' ||
         arrival[3] < '0' || arrival[4] < '0' || arrival[4] > '9' || (arrival[0] == '2' && arrival[1] > '4') ?
         puts("you entered the wrong time") : timeCheck = false);

    }
    hoursDeparture = 10 * (departure[0] - '0') + (departure[1] - '0');
    minutesDeparture = 10 * (departure[3] - '0') + (departure[4] - '0');
    hoursArrival = 10 * (arrival[0] - '0') + (arrival[1] - '0');
    minutesArrival = 10 * (arrival[3] - '0') + (arrival[4] - '0');
    int totalHours = ((24+hoursArrival)-hoursDeparture)%24;
    int totalMinutes = minutesArrival - minutesDeparture ;
        std::cout << "поездка составил" << (totalHours> 0 ?
        totalHours: -totalHours)<< " ч. " << (totalMinutes > 0 ? totalMinutes : -totalMinutes) << " м.";
}
    int main() {
    fun3();
}


