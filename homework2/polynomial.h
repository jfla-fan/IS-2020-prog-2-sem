#include <cassert>
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <map>

#define MAX_POLYNOMIAL_POWER  2147483647
#define MIN_POLYNOMIAL_POWER -2147483648
 


class Polynomial
{
public:

    Polynomial();

    Polynomial(int min_power, int max_power, int* coefficients = nullptr);

    Polynomial(const Polynomial& another);

    ~Polynomial();

    double get(int argument) const;

    Polynomial& operator = (const Polynomial& another);

    inline int size() const { return _size; }

    friend bool operator == (const Polynomial& lhs, const Polynomial& rhs);
    friend bool operator != (const Polynomial& lhs, const Polynomial& rhs);

    friend std::ostream& operator << (std::ostream& os, const Polynomial& pol);
    friend std::istream& operator >> (std::istream& is, Polynomial& pol);

    inline int operator [] (int power) const { return (power < _min_power || power > _max_power) ? 0 : _coefficients[power - _min_power]; }
    int& operator [] (int power);

    Polynomial operator + (const Polynomial& another) const;
    Polynomial operator - (const Polynomial& another) const;

    friend Polynomial operator + (const Polynomial& another);
    friend Polynomial operator - (const Polynomial& another);

    Polynomial& operator += (const Polynomial& another);
    Polynomial& operator -= (const Polynomial& another);


    Polynomial operator * (const int& another) const;
    Polynomial operator / (const int& another) const;
    friend Polynomial operator * (const int& another, const Polynomial& pol);

    Polynomial& operator *= (const int& another);
    Polynomial& operator /= (const int& another);

    Polynomial operator * (const Polynomial& another) const;
    // Polynomial operator / (const Polynomial& another) const;

    Polynomial& operator *= (const Polynomial& another);
    // Polynomial& operator /= (const Polynomial& another);


private:

    typedef struct token
    {
        int power;
        int coefficient;

        token();
        token(int p, int c);

        std::istream& operator >> (std::istream& is);
    } token_t;

    void _expand(int min_power, int max_power);
    inline int& _get_coefficient(int power);
    void _trim();

    int _min_power, _max_power;
    int* _coefficients;

    int _size;
};
