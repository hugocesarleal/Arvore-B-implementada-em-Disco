/*_____________________________________________________
       Trabalho Final - Técnicas de Programação

    Árvore B em Disco - Feito por:
    Andressa Caroline Lopes de Assis - RA:0072749
    Hugo César Leal - RA:0072753
    Sadi Martins de Castro Garcia Mendes - RA:0073489
  _____________________________________________________*/

#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include "no.h"
#include "typedFile.h"
#include "serialChar.h"
#include "registro.h"
#include <iostream>

using namespace std;

template <class T, const unsigned int MIN_DEGREE>
class bTree : protected typedFile<no<T, MIN_DEGREE>>
{
public:
    bTree(const string nome, const string tipo, const unsigned int versao);
    virtual ~bTree() {};
    void inserir(T k);
    void remover(T k);
    void mostrar(no<T, MIN_DEGREE> x);
    int buscar(T k);
    void setRaiz(no<T, MIN_DEGREE> r);
    no<T, MIN_DEGREE> getRaizNo();

    // Sobrecarga do operador >> para a classe bTree
    friend std::istream& operator>>(std::istream& input, bTree<T, MIN_DEGREE>& tree)
    {
        char value;
        input >> value;
        tree.inserir(value);
        cout << "Caracter " << value << " inserido!" << endl;
        return input;
    }//Conseguimos fazer apenas com a implementação direto na classe

     // Sobrecarga do operador << para a classe bTree
    friend std::ostream& operator<<(std::ostream& output, bTree<T, MIN_DEGREE>& tree)
    {
        tree.mostrar(tree.getRaizNo());
        return output;
    }//Conseguimos fazer apenas com a implementação direto na classe

protected:
    void removerAux(no<T, MIN_DEGREE> &x, unsigned long long int i, T k);
    T buscarPredecessorSucessor(no<T, MIN_DEGREE> &x, unsigned long long int i, char c);
    void inserirNaoCheio(no<T, MIN_DEGREE> &x, unsigned long long int i, T k);
    void registrar3(registro<no<T, MIN_DEGREE>> &rx, no<T, MIN_DEGREE> x, registro<no<T, MIN_DEGREE>> &ry,
                 no<T, MIN_DEGREE> y, registro<no<T, MIN_DEGREE>> &rz, no<T, MIN_DEGREE> z);

    no<T, MIN_DEGREE> raiz;
};

template <class T, const unsigned int MIN_DEGREE>///Costrutor da árvore
bTree<T, MIN_DEGREE>::bTree(const string nome, const string tipo, const unsigned int versao) : typedFile<no<T, MIN_DEGREE>>(nome, tipo, versao)
{
    registro<no<T, MIN_DEGREE>> r;
    if (bTree<T, MIN_DEGREE>::getRaiz() == 0)
    {
        bTree<T, MIN_DEGREE>::inserirRegistro(r);
    }
    else
    {
        bTree<T, MIN_DEGREE>::lerRegistro(r, bTree<T, MIN_DEGREE>::getRaiz());
    }
    setRaiz(r.getData());
}

template <class T, const unsigned int MIN_DEGREE>///Função de inserção de chave na árvore B
void bTree<T, MIN_DEGREE>::inserir(T k)
{
    unsigned long long int i = bTree<T, MIN_DEGREE>::getRaiz();
    unsigned int tam = raiz.sizeNo();

    no<T, MIN_DEGREE> x, z;
    registro<no<T, MIN_DEGREE>> recordX, recordY, recordZ;

    if (tam != 2*MIN_DEGREE-1)  //Caso raiz não cheia, só insere
    {
        inserirNaoCheio(raiz, i, k);
    }
    else if(tam <= 2*MIN_DEGREE-1) //Se tiver cheia, faz o split, definindo uma nova raiz e inserindo no filho adequado
    {
        z.setLeaf(false);
        z.setFilho(i, 0);

        //Aloca nova posição
        z.dividirFilhos(raiz, x, 0, bTree<T, MIN_DEGREE>::alocarPosicao());

        registrar3(recordX, x, recordY, raiz, recordZ, z);

        bTree<T, MIN_DEGREE>::escreverRegistro(recordX, bTree<T, MIN_DEGREE>::alocarPosicao());
        bTree<T, MIN_DEGREE>::inserirRegistro(recordZ);
        bTree<T, MIN_DEGREE>::escreverRegistro(recordY, i);

        inserirNaoCheio(z, bTree<T, MIN_DEGREE>::getRaiz(), k);
        setRaiz(z);
    }
}

template <class T, const unsigned int MIN_DEGREE>///A função remove um elemento específico da árvore B
void bTree<T, MIN_DEGREE>::remover(T k)
{
    removerAux(raiz, bTree<T, MIN_DEGREE>::getRaiz(), k);
}

template <class T, const unsigned int MIN_DEGREE>///Função de impressão para uma árvore B
void bTree<T, MIN_DEGREE>::mostrar(no<T, MIN_DEGREE> x)
{
    registro<no<T, MIN_DEGREE>> recordX;

    unsigned int i = 0;

    while(i < x.sizeNo())
    {
        if (!x.isLeaf())
        {
            bTree<T, MIN_DEGREE>::lerRegistro(recordX, x.getFilho(i));
            mostrar(recordX.getData());
        }
        cout << x.getChave(i) << " ";
        i++;

        if (i > x.sizeNo()-1 && !x.isLeaf())
        {
            bTree<T, MIN_DEGREE>::lerRegistro(recordX, x.getFilho(x.sizeNo()));
            mostrar(recordX.getData());
        }
    }
}

template <class T, const unsigned int MIN_DEGREE>///A função remove um elemento específico da árvore B
int bTree<T, MIN_DEGREE>::buscar(T k)
{
    no<T, MIN_DEGREE> x = raiz;

    while (!x.isLeaf())
    {
        int i = 0;
        while (i < x.sizeNo() && k > x.getChave(i))
        {
            i++;
        }
        if (k == x.getChave(i))
        {
            return i;
        }
        else
        {
            registro<no<T, MIN_DEGREE>> rx;
            bTree<T, MIN_DEGREE>::lerRegistro(rx, x.getFilho(i));
            x = rx.getData();
        }
    }

    for (int i = 0; i < x.sizeNo(); i++)
    {
        if (x.getChave(i) == k)
        {
            return i;
        }
    }
    return no<T, MIN_DEGREE>::notFound;
}

template <class T, const unsigned int MIN_DEGREE>///Define a raíz
void bTree<T, MIN_DEGREE>::setRaiz(no<T, MIN_DEGREE> r)
{
    raiz = r;
}

template <class T, const unsigned int MIN_DEGREE>///Retorna o nó raiz da árvore
no<T, MIN_DEGREE> bTree<T, MIN_DEGREE>::getRaizNo()
{
    no<T, MIN_DEGREE> x = raiz;
    return x;
}

template <class T, const unsigned int MIN_DEGREE>///Realiza a remoção de uma chave em uma árvore B
void bTree<T, MIN_DEGREE>::removerAux(no<T, MIN_DEGREE> &x, unsigned long long int i, T k)
{
    unsigned long long int p = x.acharChave(k);

    registro<no<T, MIN_DEGREE>> recordX, recordY, recordZ;
    no<T, MIN_DEGREE> y, z;

    T charAux;

    if (p != no<T, MIN_DEGREE>::notFound && x.isLeaf())
    {
        x.removerChave(p);
        recordX.setData(x);
        bTree<T, MIN_DEGREE>::escreverRegistro(recordX, i);
    }
    if(!x.isLeaf()) // Caso 2, verifica se está em página interna
    {
        if (p != no<T, MIN_DEGREE>::notFound && !x.isLeaf())
        {
            bTree<T, MIN_DEGREE>::lerRegistro(recordZ, x.getFilho(p));
            z = recordZ.getData();

            if (z.sizeNo() > MIN_DEGREE-1)  // 2a, filho z tem t chaves
            {
                charAux = buscarPredecessorSucessor(z, x.getFilho(p),'p');
                x.setChave(charAux, p);
                recordX.setData(x);
                bTree<T, MIN_DEGREE>::escreverRegistro(recordX, i);

                removerAux(z, x.getFilho(p), k);
            }
            else
            {
                bTree<T, MIN_DEGREE>::lerRegistro(recordY, x.getFilho(p + 1));
                y = recordY.getData();

                if (y.sizeNo() > no<T, MIN_DEGREE>::minimo)  // 2b filho y tem menos que t chaves
                {
                    charAux = buscarPredecessorSucessor(y, x.getFilho(p + 1),'s');
                    x.setChave(charAux, p);
                    recordY.setData(x);
                    bTree<T, MIN_DEGREE>::escreverRegistro(recordY, i);

                    removerAux(y, x.getFilho(p + 1), k);
                }
                else    // 2c, y e z tem menos que t chaves, faz a fusão
                {
                    x.juntarFilhos(z, y, p);
                    registrar3(recordX, x, recordY, y, recordZ, z);

                    bTree<T, MIN_DEGREE>::escreverRegistro(recordX, i);
                    bTree<T, MIN_DEGREE>::apagarRegistro(recordY, x.getFilho(p + 1));
                    bTree<T, MIN_DEGREE>::escreverRegistro(recordZ, x.getFilho(p));

                    if (x.isLeaf() && x.sizeNo() != 0)
                    {
                        z = recordZ.getData();
                        bTree<T, MIN_DEGREE>::lerRegistro(recordZ, x.getFilho(p));

                        removerAux(z, x.getFilho(p), k);
                    }
                    else
                    {
                        bTree<T, MIN_DEGREE>::lerRegistro(recordX, i);

                        if (x.sizeNo() != 0)
                        {
                            bTree<T, MIN_DEGREE>::lerRegistro(recordZ, x.getFilho(0));
                            bTree<T, MIN_DEGREE>::apagarRegistro(recordX, x.getFilho(0));
                            bTree<T, MIN_DEGREE>::escreverRegistro(recordZ, i);

                            x = recordZ.getData();

                            removerAux(x, i, k);
                        }
                    }
                }
            }
        }
        else    //Caso 1, inserindo no nó que é folha
        {
            p = 0;

            while (p <= x.sizeNo() && k > x.getChave(p))
            {
                p++;
            }

            if (p > x.sizeNo())
            {
                p--;
            }

            bTree<T, MIN_DEGREE>::lerRegistro(recordZ, x.getFilho(p));
            z = recordZ.getData();

            if (z.sizeNo() == no<T, MIN_DEGREE>::minimo)
            {
                if (p > 0)
                {
                    bTree<T, MIN_DEGREE>::lerRegistro(recordY, x.getFilho(p - 1));
                    y = recordY.getData();
                }

                if (p > 0 && y.sizeNo() > no<T, MIN_DEGREE>::minimo)
                {
                    x.rodarChaves(y, z, p - 1);//rotação pra direita
                    registrar3(recordX, x, recordY, y, recordZ, z);

                    bTree<T, MIN_DEGREE>::escreverRegistro(recordX, i);
                    bTree<T, MIN_DEGREE>::escreverRegistro(recordZ, x.getFilho(p));
                    bTree<T, MIN_DEGREE>::escreverRegistro(recordY, x.getFilho(p - 1));
                }
                else if (p < x.sizeNo())
                {
                    bTree<T, MIN_DEGREE>::lerRegistro(recordY, x.getFilho(p + 1));
                    y = recordY.getData();
                    unsigned int pDel;

                    if (y.sizeNo() > no<T, MIN_DEGREE>::minimo)
                    {
                        x.rodarChaves(z, y, p);//rotação pra esquerda
                        registrar3(recordX, x, recordY, y, recordZ, z);

                        bTree<T, MIN_DEGREE>::escreverRegistro(recordX, i);
                        bTree<T, MIN_DEGREE>::escreverRegistro(recordZ, x.getFilho(p));
                        bTree<T, MIN_DEGREE>::escreverRegistro(recordY, x.getFilho(p + 1));
                    }
                    else if (p == x.sizeNo())
                    {
                        pDel = x.getFilho(p);
                        x.juntarFilhos(y, z, p - 1);//junta com filho da direita
                        registrar3(recordX, x, recordY, y, recordZ, z);

                        bTree<T, MIN_DEGREE>::escreverRegistro(recordX, i);
                        bTree<T, MIN_DEGREE>::apagarRegistro(recordZ, pDel);
                        bTree<T, MIN_DEGREE>::escreverRegistro(recordY, x.getFilho(p - 1));
                    }
                    else
                    {
                        pDel = x.getFilho(p + 1);
                        x.juntarFilhos(z, y, p);//junta com filho da esquerda
                        registrar3(recordX, x, recordY, y, recordZ, z);

                        bTree<T, MIN_DEGREE>::escreverRegistro(recordX, i);
                        bTree<T, MIN_DEGREE>::escreverRegistro(recordZ, x.getFilho(p));
                        bTree<T, MIN_DEGREE>::apagarRegistro(recordY, pDel);
                    }
                }
            }

            if (!x.isLeaf() && x.sizeNo() == 0)
            {
                bTree<T, MIN_DEGREE>::lerRegistro(recordX, i);
                bTree<T, MIN_DEGREE>::lerRegistro(recordZ, x.getFilho(0));
                bTree<T, MIN_DEGREE>::apagarRegistro(recordX, x.getFilho(0));
                bTree<T, MIN_DEGREE>::escreverRegistro(recordZ, i);

                x = recordZ.getData();
            }


            for(p = 0; p < x.sizeNo() && k > x.getChave(p); p++)
            {
                //coloca o p na posição certa
            }

            bTree<T, MIN_DEGREE>::lerRegistro(recordZ, x.getFilho(p));
            z = recordZ.getData();

            removerAux(z, x.getFilho(p), k);
        }
    }
}

template <class T, const unsigned int MIN_DEGREE>///Encontra o predecessor ou Sucessor de uma chave em uma árvore B
T bTree<T, MIN_DEGREE>::buscarPredecessorSucessor(no<T, MIN_DEGREE> &x, unsigned long long int i, char c)
{
    registro<no<T, MIN_DEGREE>> recordX;
    no<T, MIN_DEGREE> y;
    T k;

    if(c=='p')
    {
        if (x.isLeaf())
        {
            k = x.getChave(x.sizeNo()-1);//chave k é a ultima do nó
            removerAux(x, i, k);
            return k;
        }
        else
        {
            bTree<T, MIN_DEGREE>::lerRegistro(recordX, x.getFilho(x.sizeNo()));
            y = recordX.getData();
            if (y.isLeaf() && y.sizeNo() == x.minimo)
            {
                k = y.getChave(y.sizeNo()-1);//chave k é a ultima do nó
                removerAux(x, i, y.getChave(y.sizeNo()-1));
                bTree<T, MIN_DEGREE>::lerRegistro(recordX, x.getFilho(y.sizeNo()-1));
                y = recordX.getData();
                return k;
            }
            return buscarPredecessorSucessor(y, x.getFilho(x.sizeNo()),'p');//volta recursivo com o filho adequado
        }

    }
    else if(c=='s')
    {
        if (x.isLeaf())
        {
            k = x.getChave(0);//chave k é a primeria do nó
            removerAux(x, i, k);
            return k;
        }
        else     //
        {
            bTree<T, MIN_DEGREE>::lerRegistro(recordX, x.getFilho(0));
            y = recordX.getData();
            if (y.isLeaf() && y.sizeNo() == x.minimo)
            {
                k = y.getChave(0);//chave k é a primeria do nó
                removerAux(x, i, y.getChave(0));
                bTree<T, MIN_DEGREE>::lerRegistro(recordX, x.getFilho(0));
                y = recordX.getData();
                return k;
            }
            return buscarPredecessorSucessor(y, x.getFilho(0),'s');//volta recursivo com o filho adequado
        }
    }else{
        cerr << "Caracter errado informado na função buscarPredecessorSucessor" << endl;
    }
}

template <class T, const unsigned int MIN_DEGREE>///Inserção de uma chave em um nó não cheio da árvore B
void bTree<T, MIN_DEGREE>::inserirNaoCheio(no<T, MIN_DEGREE>& x, unsigned long long int i, T k)
{
    int j = x.sizeNo()-1;
    no<T, MIN_DEGREE> y,z;
    registro<no<T, MIN_DEGREE>> recordX,recordY,recordZ;

    if(x.isLeaf())  //Se for folha, insira
    {
        x.inserirChave(k);
        recordX.setData(x);
        bTree<T, MIN_DEGREE>::escreverRegistro(recordX, i);
    }
    else
    {
        for (; j >= 0 && k < x.getChave(j); j--)
        {
            //coloca o index na posição correta
        }
        j++;
        bTree<T, MIN_DEGREE>::lerRegistro(recordZ, x.getFilho(j));
        z = recordZ.getData();

        if(z.sizeNo() == no<T, MIN_DEGREE>::maximo)  //nó cheio
        {
            x.dividirFilhos(z, y, j, bTree<T, MIN_DEGREE>::alocarPosicao());

            registrar3(recordX, x, recordY, y, recordZ, z);
            bTree<T, MIN_DEGREE>::escreverRegistro(recordX, i);
            bTree<T, MIN_DEGREE>::escreverRegistro(recordY, bTree<T, MIN_DEGREE>::alocarPosicao());
            bTree<T, MIN_DEGREE>::escreverRegistro(recordZ, x.getFilho(j));
        }

        bTree<T, MIN_DEGREE>::lerRegistro(recordZ, x.getFilho(j));
        z = recordZ.getData();
        inserirNaoCheio(z, x.getFilho(j), k);//Recursivamente insere k no filho apropriado
    }
}

template <class T, const unsigned int MIN_DEGREE>///Atualiza três registros com dados de três nós diferentes
void bTree<T, MIN_DEGREE>::registrar3(registro<no<T, MIN_DEGREE>> &rx, no<T, MIN_DEGREE> x, registro<no<T, MIN_DEGREE>> &ry, no<T, MIN_DEGREE> y, registro<no<T, MIN_DEGREE>> &rz, no<T, MIN_DEGREE> z)
{
    rx.setData(x);
    ry.setData(y);
    rz.setData(z);
}

#endif // BTREE_H_INCLUDED
