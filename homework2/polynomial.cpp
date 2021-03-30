#include "polynomial.h"


Polynomial::token::token()
    : power(0), coefficient(0)
{ }

Polynomial::token::token(int p, int c)
    : power(p), coefficient(c)
{ }

std::istream& Polynomial::token::operator >> (std::istream& is)
{
    is.setstate(std::ios::goodbit);

    int n, p;
    is >> n;

    if (!is)
        return is;

    char c;
    while (!(c = is.get()));

    if (c == 'x')
    {
        if (n == 0) n = 1;
        coefficient = n;

        c = is.get();
        if (c != '^')
        {
            power = 1;
            is.unget();
            return is;
        }
        else
        {
            is >> p;
            if (p == 0)
            {
                is.setstate(std::ios::badbit);
                return is;
            }
            power = p;
        }

    }
    else
    {
        power = 0;
        coefficient = n;
    }

    return is;
}

void Polynomial::_expand(int min_power, int max_power)
{
    if (min_power == _min_power && max_power == _max_power)
        return;

    int new_size = max_power - min_power + 1;
    assert(new_size >= 1);

    int* new_coefficients = new int[new_size];
    std::memset(new_coefficients, 0, sizeof(int) * new_size);

    for (int p = min_power; p <= max_power; ++p)
    {
        new_coefficients[p - min_power] = ((p < _min_power || p > _max_power) ? 0 : _coefficients[p - _min_power]);
    }
    
    _min_power = min_power;
    _max_power = max_power;
    _size = new_size;

    delete[] _coefficients;
    _coefficients = new_coefficients;
}

inline int& Polynomial::_get_coefficient(int power) { return _coefficients[power - _min_power]; }

void Polynomial::_trim()
{
    int leftBorder, rightBorder, passed = 0;

    for (leftBorder = _min_power; leftBorder <= _max_power && _size - passed > 1 && this->operator[](leftBorder) == 0; ++leftBorder, ++passed);
    for (rightBorder = _max_power; rightBorder > leftBorder && _size - passed > 1 && this->operator[](rightBorder) == 0; --rightBorder, ++passed);

    if (passed == 0)
        return;

    int new_size = _size - passed;

    int *coefficients = new int[new_size];
    memcpy(coefficients, &this->_get_coefficient(leftBorder), sizeof(int) * new_size);

    delete[] _coefficients;
    _coefficients = coefficients;

    _size = new_size;
    _min_power = leftBorder;
    _max_power = rightBorder;
}

int& Polynomial::operator [] (int power)
{
    if (power < _min_power)
        _expand(power, _max_power);
    else if (power > _max_power)
        _expand(_min_power, power);

    return this->_get_coefficient(power);
}

Polynomial::Polynomial()
    : _min_power(0), _max_power(0), _size(1)
{
    _coefficients = new int[_size] { 0 };
}

Polynomial::Polynomial(int min_power, int max_power, int* coefficients)
    : _min_power(min_power), _max_power(max_power),
    _size(_max_power - _min_power + 1)
{
    assert(_size >= 1);
    _coefficients = new int[_size];
    if (coefficients != nullptr) {
        std::memcpy(_coefficients, coefficients, sizeof(int) * _size);
        _trim();
    }
    else
        std::memset(_coefficients, 0, sizeof(int) * _size);
    // for (int i = 0; i < _size; ++i)
    // {
    //     std::cout << "_coefficients[" << i << "] = " << _coefficients[i] << '\n';
    // }
}

Polynomial::Polynomial(const Polynomial& another)
{
    //delete[] _coefficients;
    _min_power = another._min_power;
    _max_power = another._max_power;
    _size = another._size;
    _coefficients = new int[_size];
    std::memcpy(_coefficients, another._coefficients, sizeof(int) * _size);
}

Polynomial::~Polynomial()
{
    delete[] _coefficients;
}

double Polynomial::get(int argument) const
{
    double result = 0;
    for (int i = 0; i < _size; ++i)
    {
        result += _coefficients[i] * pow(argument, _min_power + i);
    }

    return result;
}

Polynomial& Polynomial::operator = (const Polynomial &another)
{
    if (&another == this)
        return *this;

    this->_min_power = another._min_power;
    this->_max_power = another._max_power;
    this->_size      = another._size;

    delete[] this->_coefficients;
    this->_coefficients = new int[this->_size];

    std::memcpy(this->_coefficients, another._coefficients, sizeof(int) * this->_size);

    return *this;
}

bool operator == (const Polynomial &lhs, const Polynomial &rhs)
{
    if (&rhs == &lhs)
        return true;

    for (int p = std::min(lhs._min_power, rhs._min_power); p <= std::max(lhs._max_power, rhs._max_power); ++p)
    {
        if (lhs[p] != rhs[p])
            return false;
    }

    return true;
}

bool operator != (const Polynomial &lhs, const Polynomial &rhs)
{
    return ! (lhs == rhs);
}

std::ostream &operator << (std::ostream &os, const Polynomial &pol)
{
    for (int i = pol._max_power; i >= pol._min_power; --i)
    {
        int coefficient = pol[i];
        if (coefficient == 0)
        {
            if (pol._size == 1)
            {
                return os << "0";
            }
            else
            {
                continue;
            }
        }
        else if (coefficient == -1 && i == pol._max_power)
        {
            os << "-";
        }
        else 
        {
            os << ((coefficient > 0 && i != pol._max_power) ? "+" : "");
            os << ((coefficient != 1) ? std::to_string(coefficient) : "");
        }
        
        if (i == 1)
            os << "x";
        else if (i == 0)
            os << ((coefficient == 1 || coefficient == -1) ? std::to_string(coefficient) : "");
        else
            os << "x^" << i;
    }

    return os;
}

// todo
std::istream &operator >> (std::istream &is, Polynomial &pol)
{
    std::map<int, int> tokens; // coefficients[power] = coefficient

    Polynomial::token_t t;

    int min_p = MAX_POLYNOMIAL_POWER;
    int max_p = MIN_POLYNOMIAL_POWER;

    while ((t.operator>>(is)))
    {
        if (t.power < min_p) min_p = t.power;
		if (t.power > max_p) max_p = t.power;

        tokens[t.power] += t.coefficient;
    }

    int size = max_p - min_p + 1;
    int* coefficients = new int[size];

    int i = 0;
    for (auto& [key, value] : tokens)
    {
        coefficients[i] = value;
        ++i;
    }

    pol = Polynomial(min_p, max_p, coefficients);

    return is;
}


Polynomial operator + (const Polynomial& pol)
{
    return pol;
}

Polynomial operator - (const Polynomial& pol)
{
    Polynomial result(pol);
    for (int i = 0; i < result._size; ++i)
        result._coefficients[i] *= -1;
    return result;
}

Polynomial Polynomial::operator + (const Polynomial &another) const
{
    int min_p   = std::min(this->_min_power, another._min_power);
    int max_p   = std::max(this->_max_power, another._max_power);
    Polynomial result(min_p, max_p);

    for (int i = min_p; i <= max_p; ++i)
    {
        if (i >= this->_min_power && i <= this->_max_power)
            result._get_coefficient(i) += this->operator[](i);
        if (i >= another._min_power && i <= another._max_power)
            result._get_coefficient(i) += another[i];
    }

    result._trim();

    return result;
}

Polynomial Polynomial::operator - (const Polynomial &another) const
{
    return (*this) + (-another);
}

Polynomial &Polynomial::operator += (const Polynomial &another)
{
    int min_p   = std::min(this->_min_power, another._min_power);
    int max_p   = std::max(this->_max_power, another._max_power);
    int size_p  = max_p - min_p + 1;

    int* coefficients = new int[size_p];
    std::memset(coefficients, 0, size_p);

    for (int i = min_p; i <= max_p; ++i)
    {
        if (i >= this->_min_power && i <= this->_max_power)
            coefficients[abs(min_p) + i] += this->_get_coefficient(i);
        if (i >= another._min_power && i <= another._max_power)
            coefficients[abs(min_p) + i] += another[i];
    }

    this->_trim();

    return *this;
}

Polynomial &Polynomial::operator -= (const Polynomial &another)
{
    return (*this) += (-another);
}

Polynomial Polynomial::operator * (const int &another) const
{
    Polynomial result(*this);
    for (int i = 0; i < result._size; ++i)
        result._coefficients[i] *= another;

    result._trim();

    return result;
}

Polynomial Polynomial::operator / (const int &another) const
{
    assert(another != 0);
	Polynomial result(*this);
	for (int i = 0; i < result._size; ++i)
		result._coefficients[i] /= another;

	result._trim();

	return result;
}

Polynomial operator * (const int& another, const Polynomial& pol)
{
    return pol * another;
}

Polynomial &Polynomial::operator *= (const int &another)
{
    for (int i = 0; i < _size; ++i)
        _coefficients[i] *= another;

    _trim();

    return *this;
}

Polynomial &Polynomial::operator /= (const int &another)
{
    return (*this) *= (1/another);
}

Polynomial Polynomial::operator * (const Polynomial &another) const
{
    Polynomial result(this->_min_power + another._min_power, this->_max_power + another._max_power, nullptr);

    for (int p1 = this->_min_power; p1 <= this->_max_power; ++p1)
    {
        for (int p2 = another._min_power; p2 <= another._max_power; ++p2)
        {
            result._get_coefficient(p1 + p2) += this->operator[](p1) * another[p2];
        }
    }

    result._trim();

    return result;
}

// // do this
// Polynomial Polynomial::operator / (const Polynomial &another) const
// {
//     return *this;
// }

Polynomial &Polynomial::operator *= (const Polynomial &another)
{
    int min_p = this->_min_power + another._min_power;
    int max_p = this->_max_power + another._max_power;
    int size_p = max_p - min_p + 1;

    int* coefficients = new int[size_p];
    std::memset(coefficients, 0, sizeof(int) * size_p);

    for (int p1 = this->_min_power; p1 <= this->_max_power; ++p1)
    {
        for (int p2 = another._min_power; p2 <= another._max_power; ++p2)
        {
            coefficients[abs(min_p) + p1 + p2] += this->operator[](p1) * another[p2];
        }
    }

    delete[] _coefficients;
    _coefficients = coefficients;
    this->_min_power = min_p;
    this->_max_power = max_p;
    this->_size = size_p;

    _trim();

    return *this;
}

// // todo
// Polynomial &Polynomial::operator /= (const Polynomial &another)
// {
//     return *this;
// }









