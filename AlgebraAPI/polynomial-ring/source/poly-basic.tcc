#include <list>
#include <vector>
#include <iostream>
#include "node.tcc"

using namespace modular;

#ifndef POLYNOMIAL
#define POLYNOMIAL

template <typename T>
class Polynomial
{
private:
    std::list<Node<T>> poly;
    size_t degree = 0;
    T numMod = 0;

public:
    Polynomial(T mod) : numMod(mod){};
    Polynomial(std::vector<std::pair<T, size_t>>, T);
    Polynomial(std::pair<T, size_t> *arr, size_t n, T);

    ~Polynomial() = default;

    Node<T> operator[](const size_t i); // use only when really necessary

    bool empty();
    typename std::list<T>::const_iterator begin() const;
    typename std::list<Node<T>>::const_iterator end() const;

    void addNode(const Node<T>);
    void addNode(const T, size_t);

    void removeNode(const Node<T>); // by value
    void removeNode(const size_t);  // by degree

    size_t deg() const;

    void print() const;

    //////////////////////////////////////////////////////////////////////////////

    Polynomial<T> operator+(const Polynomial<T> &) const;
    Polynomial<T> operator-(const Polynomial<T> &) const;
    Polynomial<T> operator*(const Polynomial<T> &) const;

    //////////////////////////////////////////////////////////////////////////////

    Polynomial<T> der() const; // deriveative
    modNum<T> evaluate(const T) const;
    modNum<T> evaluate(const modNum<T>) const;

    //////////////////////////////////////////////////////////////////////////////

    std::pair<Polynomial<T>, Polynomial<T>> operator/(const Polynomial<T> &) const;

    Polynomial<T> gcd(const Polynomial<T> &) const;

    //////////////////////////////////////////////////////////////////////////////

    static Polynomial<T> getPolynomialByOrder(size_t);
};

#endif

template <typename T>
Node<T> Polynomial<T>::operator[](const size_t i)
{

    if (i < 0 || i >= poly.size())
        throw std::out_of_range("Index out of range");

    int j = 0;
    for (auto it = poly.begin(); it != poly.end(); ++it)
    {
        if (i == j)
            return *it;
        j++;
    }

    throw std::out_of_range("Index out of range");
}

template <typename T>
std::ostream &operator<<(std::ostream &os, Node<T> &p)
{
    return os << '{' << p.deg() << ", " << p.k().getValue() << '}';
}

template <typename T>
Polynomial<T> Polynomial<T>::der() const
{
    Polynomial<T> returned_field;
    Node<T> current_newnode;
    T new_koef;
    T new_pow;

    for (auto it = poly.begin(); it != poly.end(); ++it)
    {
        new_koef = it->k() * it->deg();
        new_pow = it->deg() - 1;
        if (new_pow < 1)
        {
            new_koef = 0;
            new_pow = 0;
        }
        current_newnode = {new_pow, new_koef};
        returned_field.addNode(current_newnode);
    }

    return returned_field;
}

template <typename T>
modNum<T> Polynomial<T>::evaluate(const T x_value) const
{

    modNum<T> sum(0, numMod);
    modNum<T> current_num(0, numMod);

    for (auto it = poly.begin(); it != poly.end(); ++it)
    {
        if (it->deg() > 0)
        {
            current_num = fpow(modNum<T>(x_value, numMod), it->deg()) * it->k();
        }
        else
        {
            current_num = it->k();
        }
        sum = sum + current_num;
    }

    return sum;
}

template <typename T>
void Polynomial<T>::print() const
{
    // boolean used for adding the plus sign
    bool first_number_checked = false;
    for (auto it = poly.begin(); it != poly.end(); ++it)
    {
        if (first_number_checked)
        {
            std::cout << " + ";
        }

        else
        {
            first_number_checked = true;
        }
        if (it->deg() > 0)
        {
            if (it->k().getValue() > 1)
                std::cout << (it->k().getValue()) << "x^" << (it->deg());
            else
                std::cout << "x^" << (it->deg());
        }
        else
        {
            std::cout << (it->k().getValue());
        }
    }
    std::cout << std::endl;
}

template <typename T>
void Polynomial<T>::addNode(const Node<T> node)
{
    if (node.deg() > degree)
    {
        degree = node.deg();
        poly.push_front(node);
        return;
    }

    for (auto it = poly.begin(); it != poly.end(); it++)
    {
        if (it->deg() == node.deg())
        {
            *it = *it + node;
            return;
        }
        if (it->deg() < node.deg())
        {
            poly.insert(it, node);
            return;
        }
    }
    poly.push_back(node);
}

template <typename T>
void Polynomial<T>::addNode(const T num, size_t deg)
{
    Node<T> a(modNum(num, numMod), deg);
    this->addNode(a);
}

template <typename T>
Polynomial<T>::Polynomial(std::vector<std::pair<T, size_t>>, T mod) : Polynomial(mod)
{
}