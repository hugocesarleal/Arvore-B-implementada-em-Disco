/*_____________________________________________________
       Trabalho Final - Técnicas de Programação

    Árvore B em Disco - Feito por:
    Andressa Caroline Lopes de Assis - RA:0072749
    Hugo César Leal - RA:0072753
    Sadi Martins de Castro Garcia Mendes - RA:0073489
  _____________________________________________________*/

#ifndef REGISTRO_H_INCLUDED
#define REGISTRO_H_INCLUDED

#include "serializable.h"

using namespace std;

template <class T>
class registro : public serializable
{
    static_assert(is_base_of<serializable, T>::value, "T precisa ser serializado");
public:
    registro(){};
    registro(T d);
    registro(const registro<T> &r);
    virtual ~registro() {};
    registro<T> operator=(const registro<T> &r);
    bool operator==(const registro<T> &r);
    unsigned long long int getProximo() const;
    void setProximo(unsigned long long int p);
    void setDel(bool d);
    bool isDeleted() const;
    T getData() const;
    void setData(T d);

    virtual string toString();
    virtual void fromString(string r);
    virtual string toCSV(){return "";}
    virtual void fromCSV(string r) {}
    virtual string toJSON(){return "";}
    virtual void fromJSON(string r) {}
    virtual string toXML(){return "";}
    virtual void fromXML(string r) {}
    virtual unsigned long long int size() const;

protected:
    T data;
    bool deletado;
    unsigned long long int prox;
};

//contrutores
template <class T>
registro<T>::registro(T d) : serializable()
{
    data = d;
    deletado = false;
    next = 0;
}

template <class T>
registro<T>::registro(const registro<T> &r)
{
    data = r.getData();
    deletado = r.isDeleted();
    prox = r.getProximo();
}

template <class T>
registro<T> registro<T>::operator=(const registro<T> &r)
{
    registro<T> recordAux(r);
    if (this == &r)
    {
        return *this;
    }
    data = r.getData();
    deletado = r.isDeleted();
    prox = r.getProximo();

    return recordAux;
}

template <class T>
bool registro<T>::operator==(const registro<T> &r)
{
    return data == r.getData() && deletado == r.isDeleted() && prox == r.getProximo();
}

template <class T>
unsigned long long int registro<T>::getProximo() const
{
    return prox;
}

template <class T>
void registro<T>::setProximo(unsigned long long int p)
{
    prox = p;
}

template <class T>
void registro<T>::setDel(bool d)
{
    deletado = d;
}

template <class T>
bool registro<T>::isDeleted() const
{
    return deletado;
}

template <class T>
T registro<T>::getData() const
{
    return data;
}

template <class T>
void registro<T>::setData(T d)
{
    data = d;
}

template <class T>
string registro<T>::toString() //serialização para string
{
    string s = "";
    s += data.toString();
    s += string(reinterpret_cast<char*>(&deletado), sizeof(deletado));
    s += string(reinterpret_cast<char*>(&prox), sizeof(prox));
    return s;
}

template <class T>
void registro<T>::fromString(string r) //desserialização
{
    int p = 0;
    string stringData = r.substr(p, data.size());
    data.fromString(stringData);
    p += data.size();
    r.copy(reinterpret_cast<char*>(&deletado), sizeof(deletado), p);
    p += sizeof(deletado);
    r.copy(reinterpret_cast<char*>(&prox), sizeof(prox), p);
    p += sizeof(prox);
}

template <class T>
unsigned long long int registro<T>::size() const //retorna o tamanho total do registro
{
    return (data.size() + sizeof(deletado) + sizeof(prox));
}

#endif // REGISTRO_H_INCLUDED
