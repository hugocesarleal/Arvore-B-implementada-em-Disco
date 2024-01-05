/*_____________________________________________________
       Trabalho Final - Técnicas de Programação

    Árvore B em Disco - Feito por:
    Andressa Caroline Lopes de Assis - RA:0072749
    Hugo César Leal - RA:0072753
    Sadi Martins de Castro Garcia Mendes - RA:0073489
  _____________________________________________________*/

#include <iostream>

#include "serialChar.h"

using namespace std;

serialChar::serialChar() : serializable()
{
    caracter = '\0';
}

serialChar::serialChar(char c) : serializable()
{
    caracter = c;
}

serialChar::serialChar(const serialChar& c)
{
    caracter = c.caracter;
}

serialChar::~serialChar()
{
    caracter = '\0';
}

serialChar serialChar::operator=(const serialChar& c)
{
    serialChar aux(c);
    if(this == &c)
    {
        return *this;
    }
    caracter = c.caracter;
    return aux;
}

bool serialChar::operator==(const serialChar& c) const
{
    return caracter == c.caracter;
}

bool serialChar::operator!=(const serialChar& c) const
{
    return caracter != c.caracter;
}

bool serialChar::operator<=(const serialChar& c) const
{
    return caracter <= c.caracter;
}

bool serialChar::operator>=(const serialChar& c) const
{
    return caracter >= c.caracter;
}

bool serialChar::operator<(const serialChar& c) const
{
    return caracter < c.caracter;
}

bool serialChar::operator>(const serialChar& c) const
{
    return caracter > c.caracter;
}

void serialChar::setCaracter(char c)
{
    caracter = c;
}

string serialChar::toString()
{
    string r = "";
    r += caracter;
    return r;
}

void serialChar::fromString(string r)
{
    r.copy(reinterpret_cast<char*>(&caracter), sizeof(caracter), 0);
}

unsigned long long int serialChar::size() const
{
    return sizeof(caracter);
}
