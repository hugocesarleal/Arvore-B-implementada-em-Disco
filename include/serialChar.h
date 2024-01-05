/*_____________________________________________________
       Trabalho Final - Técnicas de Programação

    Árvore B em Disco - Feito por:
    Andressa Caroline Lopes de Assis - RA:0072749
    Hugo César Leal - RA:0072753
    Sadi Martins de Castro Garcia Mendes - RA:0073489
  _____________________________________________________*/

#ifndef SERIALCHAR_H_INCLUDED
#define SERIALCHAR_H_INCLUDED

#include "serializable.h"
#include <iostream>

using namespace std;

class serialChar : public serializable {
    public:
        serialChar();
        serialChar(char c);
        serialChar(const serialChar& c);
        virtual ~serialChar();
        serialChar operator=(const serialChar& c);
        bool operator==(const serialChar& c) const;
        bool operator!=(const serialChar& c) const;
        bool operator<=(const serialChar& c) const;
        bool operator>=(const serialChar& c) const;
        bool operator<(const serialChar& c) const;
        bool operator>(const serialChar& c) const;
        void setCaracter(char c);

        // Sobrecarga do operador >> para a classe serialChar paara substituir o setCaracter
        // Não precisou ser usado ao longo do código
        friend std::istream& operator>>(std::istream& input, serialChar& obj)
        {
            char c;
            input >> c;
            obj.setCaracter(c);
            return input;
        }//Conseguimos fazer apenas com a implementação direto na classe

        // Sobrecarga do operador << para a classe serialChar paara substituir o getCaracter
        friend std::ostream& operator<<(std::ostream& output, const serialChar& c)
        {
            cout << c.caracter;
            return output;
        }//Conseguimos fazer apenas com a implementação direto na classe

        virtual string toString();
        virtual void fromString(string r);
        virtual string toXML() { return ""; };
        virtual void fromXML(string r) {};
        virtual string toCSV() { return ""; };
        virtual void fromCSV(string r) {};
        virtual string toJSON() { return ""; };
        virtual void fromJSON(string r) {};
        virtual unsigned long long int size() const;

    protected:
        char caracter;
};

#endif // SERIALCHAR_H_INCLUDED
