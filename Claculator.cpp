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
    return (round(cos(x) * 10000000) / 10000000);
}

double Ctg(double x) {
    double a = Cos(x);
    double b = Sin(x);
    return (a / b);
}

struct Lecksema {
    char type;
    double value;
};

int Rang(char Ch) {
    if (Ch == '+' || Ch == '-') return 1;
    if (Ch == '*' || Ch == '/') return 2;
    if (Ch == 's' || Ch == 'c' || Ch == 't' || Ch == 'g' || Ch == 'e') return 3;
    else return 0;
}

bool Math(stack<Lecksema>& Stack_n, stack<Lecksema>& Stack_o, Lecksema& item) {
    double a, b, c;
    a = Stack_n.top().value;
    Stack_n.pop();

    switch (Stack_o.top().type) {
        case '+': {
            b = Stack_n.top().value;
            Stack_n.pop();
            c = b + a;
            break;
        }
        case '-': {
            b = Stack_n.top().value;
            Stack_n.pop();
            c = b - a;
            break;
        }
        case '*': {
            b = Stack_n.top().value;
            Stack_n.pop();
            c = b * a;
            break;
        }
        case '/': {
            b = Stack_n.top().value;
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
    item.type = '0';
    item.value = c;
    Stack_n.push(item);
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
        double x_value;
        cout << "Введите x=";
        cin >> x_value;

        string x_str = to_string(x_value);

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
    stack<Lecksema> Stack_n;
    stack<Lecksema> Stack_o;
    Lecksema item;

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
                item.type = 's';
                item.value = 0;
                Stack_o.push(item);
                continue;
            }
            if (array[0] == 'c' && array[1] == 'o' && array[2] == 's') {
                item.type = 'c';
                item.value = 0;
                Stack_o.push(item);
                continue;
            }
            if (array[0] == 't' && array[1] == 'a' && array[2] == 'n') {
                item.type = 't';
                item.value = 0;
                Stack_o.push(item);
                continue;
            }
            if (array[0] == 'c' && array[1] == 't' && array[2] == 'g') {
                item.type = 'g';
                item.value = 0;
                Stack_o.push(item);
                continue;
            }
            if (array[0] == 'e' && array[1] == 'x' && array[2] == 'p') {
                item.type = 'e';
                item.value = 0;
                Stack_o.push(item);
                continue;
            }
            else {
                cerr<<"Ошибка: Неизвестная функция";
                return 2;
            }
        }
        if ((Ch >= '0' && Ch <= '9') || (Ch == '-' && flag == 1) || Ch == '.') {
            iss >> value;
            item.type = '0';
            item.value = value;
            Stack_n.push(item);
            flag = 0;
            continue;
        }
        if (Ch == '+' || Ch == '-' || Ch == '*' || Ch == '/') {
            if (Stack_o.size() == 0) {
                item.type = Ch;
                item.value = 0;
                Stack_o.push(item);
                iss.ignore();
                continue;
            }
            if (Stack_o.size() != 0 && Rang(Ch) > Rang(Stack_o.top().type)) {
                item.type = Ch;
                item.value = 0;
                Stack_o.push(item);
                iss.ignore();
                continue;
            }
            if (Stack_o.size() != 0 && Rang(Ch) <= Rang(Stack_o.top().type)) {
                if (!Math(Stack_n, Stack_o, item)) {
                    return 1;
                }
                continue;
            }
        }
        if (Ch == '(') {
            item.type = Ch;
            item.value = 0;
            Stack_o.push(item);
            iss.ignore();
            continue;
        }
        if (Ch == ')') {
            while (Stack_o.top().type != '(') {
                if (!Math(Stack_n, Stack_o, item)) {
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
        if (!Math(Stack_n, Stack_o, item)) {
            return 1;
        }
    }
    cout << "Результат: " << Stack_n.top().value << endl;
    return 0;
}
