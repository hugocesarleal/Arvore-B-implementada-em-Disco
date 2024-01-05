/*_____________________________________________________
       Trabalho Final - Técnicas de Programação

    Árvore B em Disco - Feito por:
    Andressa Caroline Lopes de Assis - RA:0072749
    Hugo César Leal - RA:0072753
    Sadi Martins de Castro Garcia Mendes - RA:0073489
  _____________________________________________________*/

#ifndef TYPEDFILE_H_INCLUDED
#define TYPEDFILE_H_INCLUDED

#include <fstream>

#include "registro.h"
#include "cabecalho.h"

using namespace std;

const ios::openmode mode = ios::in | ios::out | ios::binary;

template <class T>
class typedFile : private fstream
{
    static_assert(is_base_of<serializable, T>::value, "T precisa ser serializado");

public:
    typedFile() {};
    typedFile(const string nome, const string tipo, const unsigned int versao, ios::openmode openmode = mode);
    virtual ~typedFile() {};
    bool abrir(const string nome, const string tipo, const unsigned int versao, ios::openmode openmode = mode);
    bool close();
    void limpar();
    bool isOpen();
    bool inserirRegistro(registro<T>& r);
    bool escreverRegistro(registro<T>& r, unsigned long long int i);
    bool lerRegistro(registro<T>& r, unsigned long long int i);
    bool apagarRegistro(registro<T> r, unsigned long long int i);
    unsigned long long int buscar(T data);
    unsigned long long int alocarPosicao();
    unsigned long long int getRaiz();
    unsigned long long int getFirstDel();
    unsigned long long int pos2index(unsigned long long int p);
    unsigned long long int index2pos(unsigned long long int i);

protected:
    bool escreverCabecalho(cabecalho &c);
    bool lerCabecalho(cabecalho &c);

private:
    cabecalho cabeca;
};

template <class T>//retorna o tamanho total do registro
typedFile<T>::typedFile(const string nome, const string tipo, const unsigned int versao, ios::openmode openmode) : fstream(nome.c_str(), mode)
{
    cabeca.setTipo(tipo);
    cabeca.setVersao(versao);
    if (isOpen())
    {
        lerCabecalho(cabeca);
    }
    else
    {
        abrir(nome, tipo, versao, openmode);
    }
}

template <class T>//abre o arquivo
bool typedFile<T>::abrir(const string nome, const string tipo, const unsigned int versao, ios::openmode openmode)
{
    if(isOpen())
    {
        return true;
    }

    fstream::open(nome, ios::out);//abre e fecha "gambiarra pra criar o arquivo"
    fstream::close();

    if (fstream::fail())
    {
        cerr << "Erro - Não foi possível escrever no arquivo!" << endl;
        return false;
    }

    fstream::open(nome, openmode);//teste pra ver se esta abrindo corretamente

    if (!isOpen())
    {
        cerr << "Erro - Não foi possível abrir o arquivo!" << endl;
        return false;
    }

    return escreverCabecalho(cabeca);//grava cabeçalho
}

template <class T>//fecha arquivo
bool typedFile<T>::close()
{
    fstream::close();
    return true;
}

template <class T>//limpa
void typedFile<T>::limpar()
{
    fstream::clear();
}

template <class T>//verifica se esta aberto
bool typedFile<T>::isOpen()
{
    return fstream::is_open();
}

template <class T>//insere registro no cabeçalho
bool typedFile<T>::inserirRegistro(registro<T>& r)
{
    registro<T> recordX;
    if (isOpen())
    {
        unsigned long long int p = getFirstDel();
        if (p == 0)  //se p for 0, não tem deletados
        {
            if (getRaiz() == 0)  //se o getRaiz retornar 0, então não tem registros válidos
            {
                p = 1;
            }
            else
            {
                seekp(0, ios::end);//cabeça de escrita no fim do arquivo
                p = pos2index(tellp());
                r.setProximo(getRaiz());//atualiza o ponteiro
            }
        }
        else
        {
            lerRegistro(recordX, p);//pega a posição do primeiro deletado
            cabeca.setFirstDel(recordX.getProximo());//atualiza o primeiro deletado para o proximo
            r.setProximo(getRaiz());//insere o registro no inicio da lista dos validos
        }
        if(escreverRegistro(r, p))  //grava o registro na posição e coloca como raiz
        {
            cabeca.setRaiz(p);
            return escreverCabecalho(cabeca);
        }
    }
    else
    {
        cerr << "O arquivo não foi aberto!" << endl;
        return false;
    }
}

template <class T>//serializa o que esta no registro e escreve no arquivo
bool typedFile<T>::escreverRegistro(registro<T> &r, unsigned long long int i)
{
    if (isOpen())
    {
        seekp(index2pos(i));
        if (write(r.toString().c_str(), r.size()))
        {
            return good();
        }
        else
        {
            cerr << "Erro na escrita do registro." << endl;
            return false;
        }
    }
    else
    {
        cerr << "Arquivo não aberto!" << endl;
        return false;
    }
}

template <class T>//coloca no registro r a informação lida no número de registro i
bool typedFile<T>::lerRegistro(registro<T> &r, unsigned long long int i)
{
    if (!isOpen())
    {
        cerr << "Arquivo não aberto!" << endl;
        return false;
    }

    char *ponteiro = new char[r.size()];//cria ponteiro de char
    seekp(index2pos(i));
    read(ponteiro, r.size());

    if (good())
    {
        r.fromString(string(ponteiro, r.size()));// desserialização para o registro
        delete(ponteiro);//del ponteiro
        return true;
    }
    else
    {
        cerr << "Erro na leitura do registro." << endl;
        delete(ponteiro); //del ponteiro
        return false;
    }
}

template <class T>//apaga o registro informado
bool typedFile<T>::apagarRegistro(registro<T> r, unsigned long long int i)
{
    r.setDel(true);//atualiza a flag de deletado
    cabeca.setFirstDel(i);//coloca ele como primeiro deletado
    r.setProximo(getFirstDel());//atualiza seu proximo

    escreverCabecalho(cabeca);//grava o registro
    return escreverRegistro(r, i);
}

template <class T>//busca a chave informada
unsigned long long int typedFile<T>::buscar(T data)
{
    unsigned long long int r = cabeca.getRaiz();
    unsigned long long int i = 0;
    registro<T> recordX;

    while (r != 0)  //verifica se a chave buscada é igual a daquele registro, se não for, pula para o proximo
    {
        lerRegistro(recordX, r);

        if (T(recordX.getData().getValue()) == data)
        {
            i = r;
            break;
        }

        r = recordX.getNext();
    }

    return i;
}

template <class T>//retorna a posição adequada para alocar o proximo rregistro
unsigned long long int typedFile<T>::alocarPosicao()
{
    unsigned long long int p = getFirstDel();
    registro<T> recordX;
    if(isOpen())
    {
        if(p != 0)
        {
            lerRegistro(recordX, p);
            cabeca.setFirstDel(recordX.getProximo());
            escreverCabecalho(cabeca);
            return p;
        }
        else
        {
            seekp(0, ios::end);
            p = tellp();
            return pos2index(p);
        }
    }
    else
    {
        cerr << "O arquivo não está aberto!" << endl;
        return -1;
    }
}

template <class T>//retorna o index do primeiro valido(raiz)
unsigned long long int typedFile<T>::getRaiz()
{
    return cabeca.getRaiz();
}

template <class T>//retorna o primeiro deletado
unsigned long long int typedFile<T>::getFirstDel()
{
    return cabeca.getFirstDel();
}

template <class T>//converte a posição no arquivo para um index
unsigned long long int typedFile<T>::pos2index(unsigned long long int p)
{
    registro<T> rec;
    return (p - cabeca.size()) / rec.size() + 1;
}

template <class T>//converte o index na posição no arquivo
unsigned long long int typedFile<T>::index2pos(unsigned long long int i)
{
    registro<T> rec;
    return (cabeca.size() + (i-1) * rec.size());
}

template <class T>//grava cabeçalho no arquivo
bool typedFile<T>::escreverCabecalho(cabecalho &c)
{
    if (!isOpen())
    {
        cerr << "Arquivo não aberto!" << endl;
        return false;
    }
    else
    {
        string cabecalhoSerializado = c.toString();//serialização
        seekp(0, ios::beg);
        write(cabecalhoSerializado.c_str(), c.size());//converte string pra char e grava no arquivo

        if (good())
        {
            return true;
        }
        else
        {
            cerr << "Erro na escrita do cabeçalho." << endl;
            return false;
        }
    }
}

template <class T>//le o cabeçlho informado
bool typedFile<T>::lerCabecalho(cabecalho &c)
{
    if(isOpen())
    {
        char *vetChar = new char [cabeca.size()];
        fstream::seekg(0, ios::beg);
        fstream::read(vetChar,cabeca.size());
        c.fromString(string(vetChar,c.size()));//desserializacao convertendo char pra string
        return fstream::good();
    }
    else
    {
        return close();
    }
    return false;
}

#endif // TYPEDFILE_H_INCLUDED
