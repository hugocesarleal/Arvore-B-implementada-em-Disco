/*_____________________________________________________
       Trabalho Final - Técnicas de Programação

    Árvore B em Disco - Feito por:
    Andressa Caroline Lopes de Assis - RA:0072749
    Hugo César Leal - RA:0072753
    Sadi Martins de Castro Garcia Mendes - RA:0073489
  _____________________________________________________*/

#ifndef CABECALHO_H_INCLUDED
#define CABECALHO_H_INCLUDED

#include "serializable.h"

class cabecalho : public serializable
{
public:
    cabecalho();
    cabecalho(const string t, unsigned int v);
    cabecalho(const cabecalho &c);
    virtual ~cabecalho(){};
    cabecalho operator=(const cabecalho &c);
    bool operator==(const cabecalho &c);
    string getTipo() const;
    void setTipo(string t);
    unsigned int getVersao() const;
    void setVersao(unsigned int v);
    unsigned long long int getRaiz() const;
    void setRaiz(unsigned long long int r);
    unsigned long long int getFirstDel() const;
    void setFirstDel(unsigned long long int r);

    virtual string toString();
    virtual void fromString(string r);
    virtual string toXML(){return "";};
    virtual void fromXML(string r){};
    virtual string toCSV(){return "";};
    virtual void fromCSV(string r){};
    virtual string toJSON(){return "";};
    virtual void fromJSON(string r){};
    virtual unsigned long long int size() const;

protected:
    unsigned long long int raiz;
    unsigned long long int firstDel;
    string tipo;
    unsigned int versao;
};

#endif // CABECALHO_H_INCLUDED
