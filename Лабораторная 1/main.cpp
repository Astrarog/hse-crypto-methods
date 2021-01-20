#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <locale>
#include <numeric>
#include <optional>
#include <sstream>
#include <type_traits>

using namespace std;

constexpr int alphabet_size = 32;
constexpr const char* whitespaces = " \t\f\v\n\r";

struct cipher_params
{
    int a, b;
};

template <typename T, typename = enable_if_t<!std::is_same<T, std::string>::value>>
optional<T> check_input(string input_str)
{
    T value;
    istringstream in(std::move(input_str));
    if(!(in >> value) && in.str().find_first_of(whitespaces)==string::npos)
    {
        return nullopt;
    }
    return value;
}

template<typename Func>
auto input_repeater(Func function)
{
again:

    auto result = function();

    if(result == nullopt){
        cout.clear();
        cin.clear();
        cout << u8"\nОшибка ввода попробуйте снова.\n";
        goto again;
    }
    return *result;
}

optional<cipher_params> init_params()
{
    cout << u8"Введите параметные a, b шифра Хилла (y = a*x+b(mod n)) \n"
         << "a=";

    string buffer;
    buffer.reserve(128);

    bool success = static_cast<bool>(getline(cin, buffer));
    auto a_opt = check_input<int>(std::move(buffer));
    if(a_opt!=nullopt)
        success &= ((*a_opt %= alphabet_size)==1) || gcd(*a_opt, alphabet_size);


    cout << "b=";
    success &= static_cast<bool>(getline(cin, buffer));
    auto b_opt = check_input<int>(std::move(buffer));
    success &= b_opt!=nullopt;

    if(!success)
        return nullopt;

    return cipher_params{*a_opt, *b_opt % alphabet_size};
}

void enable_utf8()
{
    try {

#ifdef WINDOWS_PLATFORM
        setlocale(LC_ALL, "Russian_Russia.65001");
#else
        locale::global(locale("ru_RU.UTF-8"));
#endif

    } catch (runtime_error&) {
        cerr << "No UTF-8 support on system. Please provide it and rerun the programm.\n";
    } catch(...)
    {
        cerr << "\nUnrecognised error\n";
        throw ;
    }
}


optional<string> read_open_text()
{
    cout << u8"Введите открытый текст.\n";
    string open_text;

    open_text.reserve(256);
    if(!(std::getline(cin, open_text)))
        return nullopt;
    return open_text;
}

string encrypt(string text, cipher_params params)
{

}

string decrypt(string text, cipher_params params)
{

}


int main() {
    enable_utf8();
//    cipher_params params = input_repeater(init_params);
    string open_text = input_repeater(read_open_text);
    std::cout << open_text << '\n';
    std::cout << u8"Тест test 1234\n";
}
