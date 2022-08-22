#include <iostream>
#include <vector>
#include "ctime"
#include "iomanip"

/*
class RomanPapin{   //начало описания класса
public:
    int rostRomana = 184; // поле(атрибут)
    float huiRomana = 20.1; //поле(атрибут)

    void romaGovoritRazmerChlena() const{  //метод
        std::cout<<"moi hui: " << huiRomana;
    }
};
*/

class Track{
public:
    std::string name;
    int  year;
    int duration;

};
class Player{
    std::vector<int> list;
    
};
int main(){
    Track hope;
    std::cout<<"Puts track name: ";
    std::cin>>hope.name;
    std::cout<<"Puts track year :";
    std::cin>>hope.year;
    std::cout<<"Puts track duration: ";
    std::cin>>hope.duration;
    

    //std::time_t t = std::time(nullptr);
    //std::tm local = *std::localtime(&t);
    //std::cin >> std::get_time(&local,"%H:%M");
  //  std::cout<< std::asctime(&local)<<"\n";
  //std::time_t t = std::time(nullptr);
  //std::tm* local = std::localtime(&t);
  //std::cout << std::asctime(local)<< std::endl;
  //std::cout << std::put_time(local,"%y-%m-%d  %H:%M:%S")<<std::endl;
    //RomanPapin Roma; // объект(экземпляр)
    //Roma.romaGovoritRazmerChlena(); // вызов метода класса для объекта roma
}
