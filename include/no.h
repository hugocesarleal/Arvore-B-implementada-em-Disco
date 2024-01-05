/*_____________________________________________________
       Trabalho Final - Técnicas de Programação

    Árvore B em Disco - Feito por:
    Andressa Caroline Lopes de Assis - RA:0072749
    Hugo César Leal - RA:0072753
    Sadi Martins de Castro Garcia Mendes - RA:0073489
  _____________________________________________________*/

#ifndef NO_H_INCLUDED
#define NO_H_INCLUDED

#include <iostream>
#include <vector>

#include "serializable.h"

using namespace std;

template <class T, const unsigned int MIN_DEGREE>
class no : public serializable
{
public:
    no();
    virtual ~no() {};
    void setLeaf(bool l);
    bool isLeaf();
    unsigned int sizeNo();
    void resize(unsigned int s);
    void inserirChave(T k);
    void removerChave(unsigned int i);
    unsigned int acharChave(T k);
    T getChave(int i);
    void setChave(T k, unsigned int i);
    void juntarFilhos(no<T, MIN_DEGREE>& y, no<T, MIN_DEGREE>& z, unsigned int i);
    void dividirFilhos(no<T, MIN_DEGREE>& y,no<T, MIN_DEGREE>& z, unsigned int i, unsigned long long int i2);
    unsigned long long int getFilho(unsigned int i);
    void setFilho(unsigned long long int c, unsigned int i);
    void rodarChaves(no<T, MIN_DEGREE>& y, no<T, MIN_DEGREE>& z, unsigned int i);
    //Serializable
    virtual string toString();
    virtual void fromString(string r);
    virtual string toXML(){return "";};
    virtual void fromXML(string r) {};
    virtual string toCSV(){return "";};
    virtual void fromCSV(string r) {};
    virtual string toJSON(){return "";};
    virtual void fromJSON(string r) {};
    virtual unsigned long long int size() const;

    //Constantes
    static const unsigned int minimo = MIN_DEGREE - 1;
    static const unsigned int maximo = 2 * MIN_DEGREE - 1;
    static const unsigned int notFound = -1;

protected:
    vector<T> chaves;
    vector<unsigned long long int> filhos;
    bool leaf;
    unsigned int n;
};

template <class T, const unsigned int MIN_DEGREE> ///Inicializa os atributos de um nó da árvore B
no<T, MIN_DEGREE>::no()
{
    unsigned int i;
    leaf = true;
    n = 0;
    chaves.resize(maximo);
    filhos.resize(maximo + 1);

    for (i = 0; i < filhos.size(); i++)
    {
        filhos[i] = notFound;
    }
}

template <class T, const unsigned int MIN_DEGREE> ///Atualiza o status do nó como folha ou não folha
void no<T, MIN_DEGREE>::setLeaf(bool l)
{
    leaf = l;
}

template <class T, const unsigned int MIN_DEGREE> ///Procedimento para dizer se o nó é folha ou não
bool no<T, MIN_DEGREE>::isLeaf()
{
    return leaf;
}

template <class T, const unsigned int MIN_DEGREE> ///Obtem o tamanho atual do nó
unsigned int no<T, MIN_DEGREE>::sizeNo()
{
    return n;
}

template <class T, const unsigned int MIN_DEGREE> ///Ajusta o tamanho do nó
void no<T, MIN_DEGREE>::resize(unsigned int s)
{
    if(s == 0)
    {
        filhos[0] = notFound;
    }
    else
    {
        for(unsigned int i = s + 1; i <= n; i++) //Desaloca os filhos de s
        {
            filhos[i] = notFound;
        }
    }
    n = s;
}

template <class T, const unsigned int MIN_DEGREE> ///Insere uma nova chave no nó
void no<T, MIN_DEGREE>::inserirChave(T k)
{
    unsigned int i;
    for (i = 0; i < n; ++i)
    {
        if (k < chaves[i])
        {
            break; //Coloca o índice na posição adequada
        }
    }

    for (unsigned int j = n; j > i; --j)
    {
        chaves[j] = chaves[j - 1];//Move as chaves para a direita para fazer espaço para a nova chave
    }

    chaves[i] = k;

    resize(n+1);//Atualiza numero de elementos no nó
}

template <class T, const unsigned int MIN_DEGREE> ///Remove uma chave em um nó
void no<T, MIN_DEGREE>::removerChave(unsigned int i)
{
    //desloca as chaves e filhos para as posições adequadas
    if(!isLeaf())
    {
        for(unsigned int j = i + 1; j < n; j++)
        {
            filhos[j] = filhos[j + 1];
        }
    }
    for(unsigned int j = i; j < n - 1; j++)
    {
        chaves[j] = chaves[j + 1];
    }
    resize(n-1);//diminui número ne elementos no nó
}

template <class T, const unsigned int MIN_DEGREE> ///Busca uma chave específica em um nó
unsigned int no<T, MIN_DEGREE>::acharChave(T k)
{
    for (unsigned int i = 0; i < n; ++i)
    {
        if (chaves[i] == k)
        {
            return i;
        }
    }
    return -1;
}

template <class T, const unsigned int MIN_DEGREE> ///Obtem a chave na posição i de um nó
T no<T, MIN_DEGREE>::getChave(int i)
{
    return chaves[i];
}

template <class T, const unsigned int MIN_DEGREE> ///Atualiza o valor de uma chave no nó
void no<T, MIN_DEGREE>::setChave(T k, unsigned int i)
{
    chaves[i] = k;
}

template <class T, const unsigned int MIN_DEGREE> ///Funde dois filhos
void no<T, MIN_DEGREE>::juntarFilhos(no<T, MIN_DEGREE>& z, no<T, MIN_DEGREE>& y, unsigned int i)
{
    unsigned long long int j = 0;

    y.inserirChave(getChave(i));
    while(j < minimo)
    {
        z.inserirChave(y.getChave(j));
        j++;
    }
    if(!y.isLeaf())
    {
        for(j = 0; j <= z.sizeNo(); j++)
        {
            z.setFilho(y.getFilho(j), MIN_DEGREE+j);
        }
    }
    removerChave(i);//remove a chave após a fusão

    y.resize(0); //altera o tamanho
}

template <class T, const unsigned int MIN_DEGREE> ///Divide um filho
void no<T, MIN_DEGREE>::dividirFilhos(no<T, MIN_DEGREE>& z,no<T, MIN_DEGREE>& y, unsigned int i, unsigned long long int i2)
{
    unsigned int j = 0;
    T meio = z.getChave(MIN_DEGREE - 1);

    while (j < MIN_DEGREE - 1)
    {
        y.inserirChave(z.getChave(j + MIN_DEGREE));
        j++;
    }

    j=n;
    while (j > i )
    {
        filhos[j + 1] = filhos[j];//Desloca os filhos pra posição adequada
        j--;
    }

    if (!y.isLeaf()) //Atualiza os filhos
    {
        y.setLeaf(false);
        j = 0;
        while(j < MIN_DEGREE)
        {
            y.filhos[j] = z.getFilho(j + MIN_DEGREE);
            j++;
        }
    }

    filhos[i + 1] = i2;
    inserirChave(meio);

    z.resize(minimo);//Define tamanho de y para minimo
}

template <class T, const unsigned int MIN_DEGREE> ///Obtem o indice do filho i do nó
unsigned long long int no<T, MIN_DEGREE>::getFilho(unsigned int i)
{
    return filhos[i];
}

template <class T, const unsigned int MIN_DEGREE> ///Atualiza o filho i de um nó
void no<T, MIN_DEGREE>::setFilho(unsigned long long int c, unsigned int i)
{
    filhos[i] = c;
}

template <class T, const unsigned int MIN_DEGREE> ///Rotaciona as chaves entre dois filhos de nó
void no<T, MIN_DEGREE>::rodarChaves(no<T, MIN_DEGREE>& z, no<T, MIN_DEGREE>& y, unsigned int i)
{
    if (z.sizeNo() > minimo)  // Tenta a rotação com o predecessor
    {
        y.inserirChave(getChave(i));

        setChave(z.getChave(z.sizeNo() - 1), i);

        if (!z.isLeaf())  // Se os nós não são folhas, realoca os filhos
        {
            for (int j = y.sizeNo(); j >= 0; j--)
            {
                y.setFilho(y.getFilho(j), j + 1);
            }
        }
        z.resize(z.sizeNo() - 1);// Reduz o tamanho do nó y
    }

    else if (y.sizeNo() > minimo)  // Caso contrário, tenta a rotação com o sucessor
    {
        z.inserirChave(getChave(i));

        setChave(y.getChave(0), i);

        if (!y.isLeaf())  // Se os nós não são folhas, move o primeiro filho do nó z para o último filho do nó y
        {
            z.setFilho(y.getFilho(0), z.sizeNo());
        }
        y.removerChave(0);// Remove a chave movida do nó z
    }
}

template <class T, const unsigned int MIN_DEGREE> ///Serializa um nó para tipo string
string no<T, MIN_DEGREE>::toString()
{
    string a = "";
    unsigned int i=0;
    a += string(reinterpret_cast<char*>(&leaf), sizeof(leaf));
    a += string(reinterpret_cast<char*>(&n), sizeof(n));
    while(i < chaves.size())
    {
        a += chaves[i].toString();
        i++;
    }
    i = 0;
    while(i < filhos.size())
    {
        a += string(reinterpret_cast<char*>(&filhos[i]), sizeof(filhos[i]));
        i++;
    }
    return a;
}

template <class T, const unsigned int MIN_DEGREE> ///Desserializa tipo string para um nó
void no<T, MIN_DEGREE>::fromString(string r)
{
    unsigned int i = 0;
    int p = 0;
    string sub;
    r.copy(reinterpret_cast<char*>(&leaf), sizeof(leaf), p);
    p += sizeof(leaf);
    r.copy(reinterpret_cast<char*>(&n), sizeof(n), p);
    p += sizeof(n);

    while(i < chaves.size())
    {
        sub = r.substr(p,chaves[i].size());
        chaves[i].fromString(sub);
        p += chaves[i].size();
        sub = "";
        i++;
    }
    i=0;
    while(i < filhos.size())
    {
        r.copy(reinterpret_cast<char*>(&filhos[i]), sizeof(filhos[i]), p);
        p += sizeof(filhos[i]);
        i++;
    }
}

template <class T, const unsigned int MIN_DEGREE> ///Calcula o espaço necessário para armazenar uma representação serializada do nó
unsigned long long int no<T, MIN_DEGREE>::size() const
{
    return (chaves.size() * chaves[0].size()) + (filhos.size() * sizeof(filhos[0])) + sizeof(leaf) + sizeof(n);
    //(num de chaves * tam chave)+(num de filhos * tam filhos)+tam da folha + tam n
}

#endif // NO_H_INCLUDED
