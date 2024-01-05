/*_____________________________________________________
       Trabalho Final - Técnicas de Programação

    Árvore B em Disco - Feito por:
    Andressa Caroline Lopes de Assis - RA:0072749
    Hugo César Leal - RA:0072753
    Sadi Martins de Castro Garcia Mendes - RA:0073489
  _____________________________________________________*/

#ifndef SERIALIZABLE_H_INCLUDED
#define SERIALIZABLE_H_INCLUDED

#include <iostream>

using namespace std;

class serializable {
    public:
        serializable() {};
        virtual ~serializable() {};
        virtual string toString() = 0;
        virtual void fromString(string r) = 0;
        virtual string toXML() = 0;
        virtual void fromXML(string r) = 0;
        virtual string toCSV() = 0;
        virtual void fromCSV(string r) = 0;
        virtual string toJSON() = 0;
        virtual void fromJSON(string r) = 0;
        virtual unsigned long long int size() const = 0;
};

#endif // SERIALIZABLE_H_INCLUDED
