#include <iostream>
#include <stack>
#include <cmath>
#include <sstream>
#include <string>
using namespace std;

double Sin(double x) {
    return (round(sin(x) * 10000000) / 10000000);
}

double Cos(double x) {
    return (round(cos(x) * 100000000) / 100000000);
}

double Ctg(double x) {
    double a = Cos(x);
    double b = Sin(x);
    return (a / b);
}


int Rang(char Ch) {
    if (Ch == '+' || Ch == '-') return 1;
    if (Ch == '*' || Ch == '/') return 2;
    if (Ch == 's' || Ch == 'c' || Ch == 't' || Ch == 'g' || Ch == 'e') return 3;
    else return 0;
}

bool Math(stack<double>& Stack_n, stack<char>& Stack_o) {
    double a, b, c;
    a = Stack_n.top();
    Stack_n.pop();

    switch (Stack_o.top()) {
        case '+': {
            b = Stack_n.top();
            Stack_n.pop();
            c = b + a;
            break;
        }
        case '-': {
            b = Stack_n.top();
            Stack_n.pop();
            c = b - a;
            break;
        }
        case '*': {
            b = Stack_n.top();
            Stack_n.pop();
            c = b * a;
            break;
        }
        case '/': {
            b = Stack_n.top();
            Stack_n.pop();
            if (a == 0) {
                cerr << "Ошибка: Деление на ноль." << endl;
                return false;
            }
            c = b / a;
            break;
        }
        case 's': {
            c = Sin(a);
            break;
        }
        case 'c': {
            c = Cos(a);
            break;
        }
        case 't': {
            if (Cos(a) == 0) {
                cerr << "Ошибка: Неверный аргумент для тангенса!";
                return false;
            }
            else {
                c = tan(a);
                break;
            }
        }
        case 'g': {
            if (Sin(a) == 0) {
                cerr << "Ошибка:Неверный аргумент для котангенса!";
                return false;
            }
            else {
                c = Ctg(a);
                break;
            }
        }
        case 'e': {
            c = exp(a);
            break;
        }
        default: {
            cerr << "Ошибка: Недопустимая операция." << endl;
            abort();
        }
    }
    Stack_o.pop();
    Stack_n.push(c);
    return true;
}

int main() {
    string input;
    cout<<"Функции которые вы можете использовать: sin, cos, tan, ctg, exp\n";
    cout << "Введите выражение: ";
    getline(cin, input);
    char cheke;

    bool x_present = false;
    for (char ch : input) {
        if (ch == 'x'&&cheke!='e') {
            x_present = true;
            break;
        }
        cheke = ch;
    }

    if (x_present) {
        string x_str;
        cout << "Введите x=";
        cin >> x_str;

        size_t pos = 0;
        while ((pos = input.find('x', pos)) != string::npos) {
            if (pos > 0 && input[pos - 1] == 'e' && pos + 1 < input.size() && input[pos + 1] == 'p') {
                pos++;
                continue;
            }
            input.replace(pos, 1, "(" + x_str + ")");
            pos += x_str.length() + 2;
        }
    }

    istringstream iss(input);
    char Ch;
    double value;
    bool flag = 1;
    stack<double> Stack_n;
    stack<char> Stack_o;

    while (1) {
        Ch = iss.peek();
        if (Ch == EOF) break;
        if (Ch == '\n') break;
        if (Ch == ' ') {
            iss.ignore();
            continue;
        }
        if (Ch == 's' || Ch == 'c' || Ch == 't' || Ch == 'g' || Ch == 'e') {
            char array[3];
            for (int i = 0; i < 3; i++) {
                Ch = iss.peek();
                array[i] = Ch;
                iss.ignore();
            }
            if (array[0] == 's' && array[1] == 'i' && array[2] == 'n') {
                Stack_o.push('s');
                continue;
            }
            if (array[0] == 'c' && array[1] == 'o' && array[2] == 's') {
                Stack_o.push('c');
                continue;
            }
            if (array[0] == 't' && array[1] == 'a' && array[2] == 'n') {
                Stack_o.push('t');
                continue;
            }
            if (array[0] == 'c' && array[1] == 't' && array[2] == 'g') {
                Stack_o.push('g');
                continue;
            }
            if (array[0] == 'e' && array[1] == 'x' && array[2] == 'p') {
                Stack_o.push('e');
                continue;
            }
            else {
                cerr<<"Ошибка: Неизвестная функция";
                return 2;
            }
        }
        if ((Ch >= '0' && Ch <= '9') || (Ch == '-' && flag == 1) || Ch == '.') {
            iss >> value;
            Stack_n.push(value);
            flag = 0;
            continue;
        }
        if (Ch == '+' || Ch == '-' || Ch == '*' || Ch == '/') {
            if (Stack_o.size() == 0) {
                Stack_o.push(Ch);
                iss.ignore();
                continue;
            }
            if (Stack_o.size() != 0 && Rang(Ch) > Rang(Stack_o.top())) {
                Stack_o.push(Ch);
                iss.ignore();
                continue;
            }
            if (Stack_o.size() != 0 && Rang(Ch) <= Rang(Stack_o.top())) {
                if (!Math(Stack_n, Stack_o)) {
                    return 1;
                }
                continue;
            }
        }
        if (Ch == '(') {
            Stack_o.push(Ch);
            iss.ignore();
            continue;
        }
        if (Ch == ')') {
            while (Stack_o.top() != '(') {
                if (!Math(Stack_n, Stack_o)) {
                    return 1;
                }
                continue;
            }
            Stack_o.pop();
            iss.ignore();
            continue;
        }
        cerr << "Ошибка: Нераспознанный символ '" << Ch << "'." << endl;
        return 1;
    }
    while (Stack_o.size() != 0) {
        if (!Math(Stack_n, Stack_o)) {
            return 1;
        }
    }
    cout << "Результат: " << Stack_n.top() << endl;
    return 0;
}
