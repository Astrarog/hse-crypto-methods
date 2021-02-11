#include "utf.hpp"
#include "hill.hpp"
#include "rus.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <locale>
#include <numeric>
#include <optional>
#include <sstream>
#include <type_traits>
#include <functional>


using namespace std;
using namespace hse;
namespace fs = filesystem;

namespace  {

constexpr const char* whitespaces = " \t\f\v\n\r";

}

template <typename T, typename = enable_if_t<!std::is_same<T, std::string>::value>>
optional<T> check_input(string input_str)
{
    T value;
    stringstream in(std::move(input_str));
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

optional<hill_cipher> get_cipher()
{
    cout << u8"Введите параметные a, b шифра Хилла (y = a*x+b(mod n), n=33; НОД(a, n)==1) \n"
         << u8"a=";

    string buffer;
    buffer.reserve(128);

    bool success = static_cast<bool>(getline(cin, buffer));
    auto a_opt = check_input<int64_t>(std::move(buffer));

    if(a_opt!=nullopt)
        success &= ((*a_opt %= alphabet_size)==1) || (gcd(*a_opt, alphabet_size)==1);

    cout << u8"b=";
    success &= static_cast<bool>(getline(cin, buffer));
    auto b_opt = check_input<int64_t>(std::move(buffer));
    success &= b_opt!=nullopt;

    if(!success)
        return nullopt;

    *a_opt += alphabet_size; *a_opt %= alphabet_size;
    *b_opt += alphabet_size; *b_opt %= alphabet_size;
    return hill_cipher(*a_opt, *b_opt % alphabet_size, alphabet_size);
}


optional<fs::path> get_text_filename()
{
    cout << u8"Введите имя файла с открытым текстом: ";
    fs::path file;

    string buffer;
    getline(cin, buffer);

    istringstream sin (buffer);
    if(!(sin>>file) || (sin.rdbuf()->in_avail()) || !(fs::exists(file) && fs::is_regular_file(file)))
        return nullopt;
    return file;
}


string exctract_text(fs::path input_file)
{
    ifstream in(move(input_file).string());

    in.seekg(0, ios_base::end);
    string text;
    text.reserve(in.tellg());
    in.seekg(0, ios_base::beg);

    text.assign(istreambuf_iterator<char>(in),
                istreambuf_iterator<char>());

    return text;
}

vector<uint32_t> russian_to_int(const string& text){
    auto unicodes = utf8_to_unicode(text);
    for(auto& c: unicodes)
    {
        if(auto res = russia_to_int(c); res!=alphabet_size)
            c=res;
        else
            c=alphabet_size;
    }
    auto pos = remove(unicodes.begin(), unicodes.end(), alphabet_size);
    unicodes.erase(pos, unicodes.end());
    return unicodes;
};

int main() {

    fs::path input_file = input_repeater(get_text_filename);
    string open_text  = exctract_text(move(input_file));

    cout << u8"Текст из файла:\n" << quoted(open_text) << '\n';

    hill_cipher cipher = input_repeater(get_cipher);

    vector<uint32_t> open_text_codes = russian_to_int(open_text);

    cout << u8"Численное представление исходного текста\n";
    for(auto c: open_text_codes)
        cout << c << ' ';
    cout << '\n';

    string encoded_text = cipher.encode(open_text);
    string decoded_text = cipher.decode(encoded_text);
    cout << u8"Шифртекст:\n" << quoted(encoded_text) << '\n'
         << u8"Декодированный текст:\n" << quoted(decoded_text) << '\n';

    vector<uint32_t> encoded_text_codes = russian_to_int(encoded_text);
    cout << u8"Численное представление шифртекста\n";
    for(auto c: encoded_text_codes)
        cout << c << ' ';
    cout << '\n';
}
