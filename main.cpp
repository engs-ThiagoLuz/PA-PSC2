
#include "interface.cpp"

int main() {
    ListaTarefas lista;
    PilhaAcoes pilha;
    inicializarLista(lista);
    inicializarPilha(pilha);
    int opcao;
    do {
        mostrarMenu();
        opcao = lerInteiro("Opcao: ");
        if (opcao == 1) menuAdicionar(lista, pilha);
        else if (opcao == 2) listarTarefas(lista);
        else if (opcao == 3 || opcao == 4) menuPorId(lista, pilha, opcao);
        else if (opcao == 5) menuDesfazer(lista, pilha);
        else if (opcao >= 6 && opcao <= 8) menuRecursivo(lista, opcao);
        else if (opcao == 9 || opcao == 10) menuOrdenar(lista, opcao);
    } while (opcao != 0);
    liberarLista(lista);
    liberarPilha(pilha);
    return 0;
}
