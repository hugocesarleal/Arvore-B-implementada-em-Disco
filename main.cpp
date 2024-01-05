/*_____________________________________________________
       Trabalho Final - Técnicas de Programação

    Árvore B em Disco - Feito por:
    Andressa Caroline Lopes de Assis - RA:0072749
    Hugo César Leal - RA:0072753
    Sadi Martins de Castro Garcia Mendes - RA:0073489
  _____________________________________________________*/

#include <iostream>
#include <clocale>

#include "bTree.h"
#include "serialChar.h"

using namespace std;

int main()
{
    setlocale(LC_ALL,"Portuguese");

    bTree<serialChar, 3> tree("arvore.txt","char", 1);
    //Caracteres para serem inseridos e removidos no teste
    serialChar inserir[] = {'u', '<', 'd', 'e', 'i', '{', '!', 'X', '.', '4', 'r', 'J', '#', 'B', '7', 'x', ',', '?', '9', ';', 'Q', '8', 'I', 'P', 'C'};
    serialChar remover[] = {'4', 'r', '#', ',', 'i', '<', 'e', '8', '9', 'C', '!', 'P'};
    bool repetir = true;
    int op;
    char c;

    while(repetir)
    {
        cout << "\nArvore B Implementada em Arquivo Tipado" << endl;

        cout << "1 - Inserir Caracter" << endl;
        cout << "2 - Remover Caracter" << endl;
        cout << "3 - Testes de Insercao e Remocao" << endl;
        cout << "4 - Procurar Caracter" << endl;
        cout << "5 - Mostrar a Arvore B" << endl;
        cout << "6 - Fechar o Programa\n" << endl;

        cin >> op;

        switch(op)
        {

        case 1:{
            cout << "Digite um valor para inserir na arvore B: ";
            cin >> tree; //sobrecarga do operador >>, substituindo a chamada do tree.inserir(chave)
            break;
        }
        case 2:
            cout << "Qual caracter sera removido: ";
            cin >> c;
            tree.remover(c);
            cout << "Caracter " << c << " removido!" << endl;
            break;

        case 3:
            cout << "\nTeste de Insercao" << endl;
            for(serialChar i : inserir)
            {
                cout << "\nInserindo Caracter " << i << endl;
                tree.inserir(i);
                cout << tree;
            }
            cout << "\n\nTeste de Remocao" << endl;
            for (serialChar i : remover)
            {
                cout << "\nRemovendo Caracter " << i << endl;
                tree.remover(i);
                cout << tree;
            }
            cout << "\n" << endl;
            break;

        case 4:
            cout << "Qual caracter sera buscado: ";
            cin >> c;
            if(tree.buscar(c) >= 0)
            {
                cout << "\nO caracter " << c << " foi encontrado!" << endl;
            }
            else
            {
                cout << "\nO caracter " << c << " nao foi encontrado!" << endl;
            }
            break;

        case 5:
            cout << "\nMostrando a Arvore B:" << endl;
            cout << tree; //sobrecarga do poerador <<, substituindo a chamada do tree.mostrar(getRaizNo())
            cout << "\n";
            break;

        case 6:
        {
            cout << "Saindo..." << endl;
            repetir = false;
            break;
        }
        default:
            cout << "Opcao Invalida.\n" << endl;
            break;
        }
    }
    return 0;
}
