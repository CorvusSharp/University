#include <cmath>
#include <iostream>
using std::cout;
using std::cin;
void fun6(){
    int countHesitation=0;
    float endAmp,distance;
    cout<<"Введите начальную амплитуду: ";
    cin>>distance;
    cout<<"Введите конечную амплитуду: ";
    cin>>endAmp;
    float curAmp = distance;
    while(curAmp>endAmp){
        countHesitation++;
        cout<<"Качнулся " << countHesitation <<" Раз\n";
        curAmp = (distance - distance*0.084f);
        distance = curAmp;
        cout << "Текущая амплитуда: " << curAmp<<"\n";
    }
    cout <<"Затух!";
}
void fun5() {
    int second, average,minutes;
    float km,tempo,sumTime = 0.f,countKm= 1;
    cout << "Привет, Сэм! Сколько километров ты сегодня пробежал? ";
    cin >> km;
    while (countKm<= km) {
        cout << "Какой у тебя был темп на километре " << countKm << "? ";
        cin >> tempo;
        sumTime += tempo;
        puts("");
        countKm++;
    }
    average = (int)std::round(sumTime/km);
    minutes = (int)average/60;
    second = average - minutes*60;
    cout<<"Среднее время "<< minutes <<" минут " << second <<"секунд.";
//305,291,243,255,279,312
}

void fun4() {
    float size, speed, curSize=0;
    int flag = 1;
    int progress,seconds = 0;
    while(flag == 1) {
        cout << "Введите размер файла: ";
        cin >> size;
        cout << "Введите размер speed: ";
        cin >> speed;
        if (speed < 0) puts("Скорость не может быть отрицательной");
        if (size < 0) puts("Размер файла не должен быть меньше 0");
        if(speed > 0 && size > 0) {
            flag = 0;
        }
    }
    while (curSize < size) {
    seconds++;
    curSize += speed;
    progress = (int)((curSize/size)*100);
        if (curSize > size) {
            progress = 100;
            curSize = size;
        }
    cout<<"Прошло "<< seconds<<" сек. Скачано "<< curSize<<" из "<< size<<"("<<progress<<")"<<"%"<<"\n";
    }
}
void fun3(){
    int count = 0;
    int powCounter = 0;
    float x,y,z;
    int countCube, flag = 1;
    int side = 5;
    cout<<"Введите размеры бруска в см\n";
    while(flag == 1) {
        cout << "X: ";
        cin >> x;
        cout << "Y: ";
        cin >> y;
        cout << "Z: ";
        cin >> z;
        if(x >= 5 && y>=5 && z>=5){
            flag = 0;
        }
        else puts("Стороны бруска не могут быть меньше 5см");
    }
    countCube = ((int)x/side)* ((int)y/side) * ((int)z/side);
    if(countCube < 8 ){
        cout<<"Нельзя составить набор из " << countCube<<" кубиков";
        return;
    }
    cout<<"Кол-во кубов = "<<countCube<<"\n";
    while(pow(powCounter+1,3) < countCube){
        powCounter++;
    }
    cout<<"Можно составить набор из "<<pow(powCounter,3)<<" кубиков";
}
void fun2(){
    float hp,resistance,power;
    int flag = 1;
    int flag1 = 1;
    while (flag == 1) {
        cout << "Введите кол-во  HP: ";
        cin >> hp;
        if (hp > 1 || hp <= 0) {
            cout << "Введите HP в диапазоне от 0 до 1\n";
        }
            if (hp <= 1 && hp > 0) {
                flag = 0;
        }
    }
    while (flag1 == 1){
        cout << "Введите кол-во Резиста: ";
        cin >> resistance;
        if (resistance > 1 ||  resistance < 0) {
            cout << "Введите сопротивление магии в диапазоне от 0 до 1\n";
        }
        if (resistance <= 1 &&  resistance >= 0) {
            flag1 = 0;
        }
    }
    if(resistance == 1){
        puts("Вы не пробьете босса:(");
        return;
    }
    while(hp>0){
        cout<<"Введите вашу мощность:";
        cin>>power;
        power -= power*resistance;
        hp -= power;
        cout<<"HP = "<<hp<<"\n";
    }
    puts("Вы убили Орка");
}
void fun1(){
 float m,F,t,distance,a;
    cout<<"Введите массу: ";
    cin >> m;
    cout<<"Введите силу двигателя: ";
    cin>>F;
    cout<<"Введите время: ";
    cin>>t;
    a = F/m;
    cout<<"Растояние = ";
    cout<<float(a * std::pow(t,2))/2;;
}
int main() {
   fun6();
    return 0;
}
