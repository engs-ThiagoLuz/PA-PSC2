
#include "interface.cpp"

int main() {
    ListaTarefas lista;
    PilhaAcoes pilha;
    FilaExecucao fila;
    inicializarLista(lista);
    inicializarPilha(pilha);
    inicializarFila(fila);
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
        else if (opcao >= 11 && opcao <= 13) menuFila(lista, pilha, fila, opcao);
        else if (opcao == 14 || opcao == 15) menuArvore(lista, opcao);
        else if (opcao == 16 || opcao == 17) menuBusca(lista, opcao);
    } while (opcao != 0);
    liberarLista(lista);
    liberarPilha(pilha);
    liberarFila(fila);
    return 0;
}
