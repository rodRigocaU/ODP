#ifndef UTILS_HPP_
#define UTILS_HPP_
#include <sstream>

using namespace std;

// retorna el string de un número, con
// zeros a la izquierda dependiendo de la cantidad de dígitos
// p.ej. digitsitostr(15,5) -> 00015
string digitsitostr(int n, int digits)
{
  string zeros(digits - (int)floor(log10(n) + 1), '0');
  stringstream ss;
  ss << digits;
  return zeros + ss.str();
}


#endif // UTILS_HPP_
