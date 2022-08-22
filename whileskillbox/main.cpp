#include <iostream>

using std::cout;
using std::cin;
int num_6(){
    int deposit;
    float tax;
    int finalSum;
    cout<<"Введите депозит:";
    cin>>deposit;
    cout<<"Введите Процентную ставку:";
    cin>>tax;
    cout<<"Введите желаемую сумму:";
    cin>>finalSum;
    tax /= 100;
    if(finalSum<deposit){
        puts("Желаемая сумму должна быть больше вашего депозита");
        return 0;
    }
    int yearCouter=0;
    while(deposit<finalSum){
        deposit += (int)((float)deposit*tax);
        yearCouter++;
    }
    cout<<"Придётся подождать:"<<yearCouter;
}
void num_5(){
   int num,i0 = 1,i1=1,sum = 0,i;
    cout<<"Введите число:";
    cin>>num;
    while(i<num - 2){
        sum = i1+i0;
        i0 = i1;
        i1 = sum;
        i +=1;
        if(i1<0){
            puts("Введите число меньше");
            break;
        }
    }
    cout<<i1;
}
void num_4(){
    int num,a1,a2,a3;
    int b1,b2,b3,sum1,sum2;
    cout<<"введите число:";
    cin>>num;
    while(1){
        a1 = num / 100000;
        a2 = num / 10000 % 10;
        a3 = num/ 1000 % 100 % 10;
        b1 = num/ 100 % 1000 % 100 % 10;
        b2 = num / 10 % 10000 % 1000 % 100 % 10;
        b3 = num % 10;
        sum1 = (a1 + a2 + a3);
        sum2 = (b1 + b2 + b3);
        break;
    }
    if(sum1 == sum2){
        puts("Вы счастливчик");
    }
    else{
        puts("не повезло");
    }
}
void num_3(){
    int sum_duty;
    std::string name;
    int deposit;
    cout << "Как вас зовут:";
    cin >> name;
    printf("Введите сумму долга:");
    cin >> sum_duty;
    while (1) {
        cout << "Введите сумму пополнения:";
        cin >> deposit;
        sum_duty = sum_duty - deposit;
        if (sum_duty > 0) {
            cout << name << " Вам осталось выплатить:" << sum_duty << "\n";
        }
        if (sum_duty < 0) {
            cout << "на ващем счету:" << abs(sum_duty);
            break;
        }
        if (sum_duty == 0){
            puts("Вы все выплатили)");
            break;
        }
    }
}

void num_2() {
    long long int a;
    int counter = 0;
    cout << "Введите число:";
    cin >> a;
    while (a) {
        a /= 10;
        counter++;
    }
    cout << "Цифр в числе:" << counter;
}
void num_1() {
    int a;
    cout << "Введите который час\n";
    scanf("%d", &a);
    while (a > 0) {
        a--;
        puts("ky-ky");
    }
}
int main() {
 num_6();
}
