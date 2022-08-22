
void swap(int& a, int& b){
    int c = a;
    a = b;
    b = c;
}

void swap(char*& a, char*& b){
    char c[20];
    char d[20];
    strcpy(c, a);
    strcpy(d, b);
    strcpy(b, c);
    strcpy(a, d);
}