#include <iostream>

using std::cout;
using std::cin;
void num_3() {
    int weight = 15,height = 20, opt,xPossion = 10,yPossion = 8 ;
    cout<<"[Программа]: Марсоход находится на позиции "<< xPossion<<","<<yPossion<<" введите команду:";
    cout<<"\n[Оператор]:";
    while (1) {
        cin>>opt;
        switch (opt) {
            case 'W':
                if(xPossion<weight && yPossion<height) {
                    yPossion=+1;
                    cout<<"\n[Оператор]:";
                    cout<<"[Программа]: Марсоход находится на позиции"<< xPossion<<","<<yPossion<<"введите команду: ";
                    break;
                } else puts("Вы уперлись В стену:)");
            case 'A':
                if(xPossion<weight && yPossion<height) {
                    xPossion=-1;
                    cout<<"\n[Оператор]:";
                    cout<<"[Программа]: Марсоход находится на позиции"<< xPossion<<","<<yPossion<<"введите команду: ";
                } else puts("Вы уперлись В стену:)");
            case 'S':
                if(xPossion<weight && yPossion<height) {
                    yPossion=-1;
                    cout<<"\n[Оператор]:";
                    cout<<"[Программа]: Марсоход находится на позиции"<< xPossion<<","<<yPossion<<"введите команду: ";
                } else puts("Вы уперлись В стену:)");
            case 'D':
                if(xPossion<weight && yPossion<height) {
                    xPossion=-1;
                    cout<<"\n[Оператор]:";
                    cout<<"[Программа]: Марсоход находится на позиции"<< xPossion<<","<<yPossion<<"введите команду: ";
                } else puts("Вы уперлись В стену:)");
                break;
        }
    }

}
void num_2() {
    int Stwater, Stmilk, opt = 0;
    int lateeCounter = 0;
    int amerCounter = 0;
    cout << "ВВведите количество воды в мл: ";
    cin >> Stwater;
    cout << "Введите количество молока в мл: ";
    cin >> Stmilk;
    while (Stwater>=300 || Stmilk>=270) {
        cout << "Выберите напиток (1 — американо, 2 — латте):";
        cin>>opt;
        switch (opt) {
            case 1:
                if(Stwater>=300 ) {
                    Stwater -= 300;
                    amerCounter++;
                    break;
                } else puts("Не хватает Компонентов для латте");
            case 2:
                if(Stwater>=30 && Stmilk>=270) {
                    Stwater -= 30;
                    Stmilk -= 270;
                    lateeCounter++;
                } else puts("Не хватает Компонентов для капучино");
                break;
        }
    }
    cout<<"Ингредиентов осталось:\n"<<"\t\t\tВода:"<<Stwater<<"\n\t\t\tМолоко:"<<Stmilk<<"\n";
    cout<<"Кружек американо приготовлено:"<<amerCounter<<"\n";
    cout<<"Кружек латте приготовлено:"<<lateeCounter<<"\n";
}
void num_1(){
    int buck,monthCounter = 0;
    cout<< "Введите кол-во кил. гречки:";
    cin>>buck;
    for (int i = buck; i > 0; i-=4) {
        monthCounter++;
        cout<<"После "<< monthCounter<< " месяца у вас в запасе останется "<< i <<"кг гречки \n";
    }
    cout<<"Гречка закончилась спустя: "<<monthCounter<<" Месяцев";
}
int main() {
num_3();
    return 0;
}
