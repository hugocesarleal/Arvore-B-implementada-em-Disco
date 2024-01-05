/*_____________________________________________________
       Trabalho Final - Técnicas de Programação

    Árvore B em Disco - Feito por:
    Andressa Caroline Lopes de Assis - RA:0072749
    Hugo César Leal - RA:0072753
    Sadi Martins de Castro Garcia Mendes - RA:0073489
  _____________________________________________________*/

#include "cabecalho.h"

cabecalho::cabecalho() : serializable()
{
    tipo = "";
    versao = 0;
    raiz = 0;
    firstDel = 0;
}

cabecalho::cabecalho(const string t, unsigned int v) : serializable()
{
    tipo = t.substr(0, 3);
    versao = v;
    raiz = 0;
    firstDel = 0;
}

cabecalho::cabecalho(const cabecalho &c)
{
    raiz = c.getRaiz();
    firstDel = c.getFirstDel();
    tipo = c.getTipo();
    versao = c.getVersao();
}

cabecalho cabecalho::operator=(const cabecalho &c)
{
    cabecalho cabecaAux(c);
    if (this == &c)
        return *this;

    raiz = c.getRaiz();
    firstDel = c.getFirstDel();
    tipo = c.getTipo();
    versao = c.getVersao();
    return cabecaAux;
}

bool cabecalho::operator==(const cabecalho &c)
{
    return tipo == c.getTipo() && versao == c.getVersao() &&
           firstDel == c.getFirstDel() &&
           raiz == c.getRaiz();
}

string cabecalho::getTipo() const //retorna o tipo do arquivo
{
    return tipo;
}

void cabecalho::setTipo(string t)//atualiza o tipo
{
    tipo = t.substr(0, 3);
}

unsigned int cabecalho::getVersao() const //retorna a versão
{
    return versao;
}

void cabecalho::setVersao(unsigned int v)//atualiza a versão
{
    versao = v;
}

unsigned long long int cabecalho::getRaiz() const //retorna a raiz
{
    return raiz;
}

void cabecalho::setRaiz(unsigned long long int r)//atualiza a raiz
{
    raiz = r;
}

unsigned long long int cabecalho::getFirstDel() const //retorna o primeiro deletado
{
    return firstDel;
}

void cabecalho::setFirstDel(unsigned long long int r)//atualiza o primeiro deletado
{
    firstDel = r;
}

string cabecalho::toString()//serialização para string
{
    string s = "";
    s += tipo;
    s += string(reinterpret_cast<char*>(&versao), sizeof(versao));
    s += string(reinterpret_cast<char*>(&raiz), sizeof(raiz));
    s += string(reinterpret_cast<char*>(&firstDel), sizeof(firstDel));
    return s;
}

void cabecalho::fromString(string r)//desserialização
{
    int p = 0;
    tipo = r.substr(p, 3);
    p += tipo.length();
    r.copy(reinterpret_cast<char*>(&versao), sizeof(versao), p);
    p += sizeof(versao);
    r.copy(reinterpret_cast<char*>(&raiz), sizeof(raiz), p);
    p += sizeof(raiz);
    r.copy(reinterpret_cast<char*>(&firstDel), sizeof(firstDel), p);
}

unsigned long long int cabecalho::size() const //retorna tamanho do cabeçalho
{
    return (tipo.length() + sizeof(versao) + sizeof(raiz) + sizeof(firstDel));
}
