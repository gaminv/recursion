#include <iostream>


//Вариант 8
//Понятие выражение определено следующим образом : 
//<выражение> :: = <терм> | <терм> +<выражение> | <терм> –<выражение>
//<терм> :: = <множитель> | <множитель> *<терм> | (<множитель> +<терм>) 
//<множитель> :: = <целое без знака> | <идентификатор> | (<выражение>) 
//<целое без знака> :: = <цифра> | <цифра> <целое без знака> 
//<идентификатор> :: = <буква>
//<цифра> :: = 0 | 1 
//<буква> :: = x | y | z

//<идентификатор>
bool is_id(char* left);
//<целое без знака>
bool is_unsigned_int(char* left);
//<множитель>
bool is_multiplier(char* left, char* right);
//<терм>
bool is_term(char* left, char* right);
//<выражение>
bool is_expression(char* left, char* right);
//find corresponding back brace
char* back_brace(char* left, char* right, int k);
//find end of c-string
char* endOfStr(char* num);

int main()
{
    while (true) {
        char* line = new char[100]();
        std::cout << "Input\n";
        std::cin >> line;
        char* end = endOfStr(line);
        if (is_expression(line, end))
            std::cout << "TRUE\n\n";
        else
            std::cout << "FALSE\n\n";
        delete[] line;
    }
}

//<идентификатор> :: = <буква>
//<буква> :: = x | y | z
bool is_id(char* left)
{
    if (*left == 'x' || *left == 'y' || *left == 'z') {
        if (*(left + 1) == '+' || *(left + 1) == '*' || *(left + 1) == '-' || *(left + 1) == '\0' || *(left + 1) == ')') {
            return true;
        }
    }
    return false;
}

//<целое без знака> :: = <цифра> | <цифра> <целое без знака>
//<цифра> :: = 0 | 1
bool is_unsigned_int(char* left)
{
    if (*left == '0' or *left == '1') {
        if (*(left + 1) == '+' || *(left + 1) == '*' || *(left + 1) == '-' || *(left + 1) == '\0' || *(left + 1) == ')') {
            return true;
        }
        return is_unsigned_int(left + 1);
    }
    return false;
}

//<множитель> :: = <целое без знака> | <идентификатор> | (<выражение>)
bool is_multiplier(char* left, char* right)
{
    if (is_unsigned_int(left) || is_id(left))
        return true;

    if (*left == '(') {
        char* b_brace = back_brace(left + 1, right, 1);
        if (b_brace == nullptr) {
            return false;
        }
        return (is_expression(left + 1, b_brace));
    }
    return false;
}

//<терм> :: = <множитель> | <множитель> * <терм> | (<множитель> + <терм>)
bool is_term(char* left, char* right)
{
    if (*left == '(') {
        char* plus = strstr(left, "+");
        char* b_brace = back_brace(left + 1, right, 1);
        left++;

        if (b_brace == nullptr)
            return false;

        if (plus != nullptr && plus < b_brace) {
            if (is_multiplier(left, plus) && is_term(plus + 1, b_brace + 1)) {
                return true;
            }
            return false;
        }
    }

    char* star = strstr(left, "*");

    if (star != nullptr and star <= right) {
        if (is_multiplier(left, star)) {
            return is_term(star + 1, right);
        }
        return false;
    }

    if (*(left - 1) == '(') left--;
    return is_multiplier(left, right);
}

//<выражение> ::= <терм> | <терм> + <выражение> | <терм> – <выражение>
bool is_expression(char* left, char* right)
{
    char* plus, * mines, * sign;

    if (*left == '(') {
        left++;
        char* n_left = back_brace(left, right, 1);
        if (n_left == nullptr)
            return false;
        plus = strstr(n_left, "+");
        mines = strstr(n_left, "-");
    }
    else {
        plus = strstr(left, "+");
        mines = strstr(left, "-");
    }

    if (plus != nullptr && mines != nullptr)
        sign = std::min(plus, mines);
    else
        sign = std::max(plus, mines);

    if (sign > strstr(left + 1, "("))
        sign = nullptr;

    if (sign != nullptr && sign < strstr(left, "(")) {
        if (is_term(left, sign)) {
            return is_expression(sign + 1, right);
        }
        return false;
    }
    return is_term(left, right);
}

//find corresponding back brace
char* back_brace(char* left, char* right, int k)
{
    if (*left == '(')
        k++;
    if (*left == ')')
        k--;
    if (k == 0)
        return left;
    if (left == right)
        return nullptr;
    return back_brace(left + 1, right, k);
}

//find end of c-string
char* endOfStr(char* num) {
    if (*num == '\0')
        return num;
    return endOfStr(num + 1);
}