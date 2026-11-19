
#pragma once
#include <iostream>
#include "lista.cpp"
#include "pilha.cpp"
#include "fila.cpp"
#include "ordenacao.cpp"
#include "arvore.cpp"
#include "busca.cpp"
using namespace std;

// Lê um número inteiro do teclado, exibindo uma mensagem.
int lerInteiro(const string& mensagem) {
    int valor;
    cout << mensagem;
    while (!(cin >> valor)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Valor invalido. " << mensagem;
    }
    cin.ignore(1000, '\n');
    return valor;
}

// Lê uma linha de texto do teclado.
string lerTexto(const string& mensagem) {
    string texto;
    cout << mensagem;
    getline(cin, texto);
    return texto;
}

// Pede os dados, insere e registra a ação na pilha.
void menuAdicionar(ListaTarefas& lista, PilhaAcoes& pilha) {
    string titulo = lerTexto("Titulo: ");
    int prioridade = lerInteiro("Prioridade (1-alta, 2-media, 3-baixa): ");
    int prazo = lerInteiro("Prazo (AAAAMMDD, 0 = sem prazo): ");
    int id = inserirTarefa(lista, titulo, prioridade, prazo);
    empilhar(pilha, ADICAO, *buscarPorId(lista, id));
    cout << "Tarefa criada com id " << id << ".\n";
}

// Opções do menu
void mostrarMenu() {
    cout << "\n=== Gestor de Tarefas ===\n"
         << "1 - Adicionar tarefa\n"
         << "2 - Listar tarefas\n"
         << "3 - Concluir tarefa\n"
         << "4 - Remover tarefa\n"
         << "5 - Desfazer ultima acao\n"
         << "6 - Resumo [recursivo]\n"
         << "7 - Buscar por id [recursivo]\n"
         << "8 - Listar em ordem inversa [recursivo]\n"
         << "9 - Ordenar por prazo [Insertion Sort]\n"
         << "10 - Ordenar por prioridade [Insertion Sort]\n"
         << "11 - Colocar tarefa na fila de execucao\n"
         << "12 - Executar proxima da fila\n"
         << "13 - Ver fila de execucao\n"
         << "14 - Listar por titulo A-Z [arvore]\n"
         << "15 - Buscar por titulo [arvore]\n"
         << "16 - Buscar tarefa por id: sequencial x binaria\n"
         << "17 - Benchmark: sequencial x binaria\n"
         << "0 - Sair\n";
}

// (3) e (4) registram o estado ANTES de alterar.
void menuPorId(ListaTarefas& lista, PilhaAcoes& pilha, int opcao) {
    int id = lerInteiro("Id da tarefa: ");
    Tarefa* tarefa = buscarPorId(lista, id);
    if (tarefa == nullptr) {
        cout << "Tarefa nao encontrada.\n";
        return;
    }
    if (opcao == 3 && tarefa->concluida) {
        cout << "A tarefa ja esta concluída.\n";
        return;
    }
    empilhar(pilha, (opcao == 3) ? CONCLUSAO : REMOCAO, *tarefa);
    if (opcao == 3) concluirTarefa(lista, id);
    else removerTarefa(lista, id);
    cout << "Operacao realizada.\n";
}

// Trata a opção de desfazer e informa o resultado.
void menuDesfazer(ListaTarefas& lista, PilhaAcoes& pilha) {
    if (desfazerUltimaAcao(lista, pilha)) {
        cout << "Ultima acao desfeita.\n";
    } else {
        cout << "Nada para desfazer.\n";
    }
}

// Trata as opções recursivas (6, 7 e 8)
void menuRecursivo(const ListaTarefas& lista, int opcao) {
    if (opcao == 6) {
        cout << "Total: " << contarTarefas(lista.inicio)
             << " | Pendentes: " << contarPendentes(lista.inicio) << "\n";
    } else if (opcao == 7) {
        int id = lerInteiro("Id da tarefa: ");
        const Tarefa* achada = buscarRecursivo(lista.inicio, id);
        if (achada != nullptr) imprimirTarefa(achada);
        else cout << "Tarefa nao encontrada.\n";
    } else {
        imprimirInvertida(lista.inicio);
    }
}

// Ordena a lista pelo critério escolhido (9 = prazo, 10 = prioridade)
void menuOrdenar(ListaTarefas& lista, int opcao) {
    ordenarLista(lista, (opcao == 9) ? POR_PRAZO : POR_PRIORIDADE);
    cout << "Lista ordenada:\n";
    listarTarefas(lista);
}

// Imprime a fila de execução, da frente para o fim.
void mostrarFila(const ListaTarefas& lista, const FilaExecucao& fila) {
    if (filaVazia(fila)) {
        cout << "Fila de execucao vazia.\n";
        return;
    }
    int posicao = 1;
    for (NoFila* atual = fila.frente; atual != nullptr; atual = atual->proximo) {
        const Tarefa* tarefa = buscarRecursivo(lista.inicio, atual->idTarefa);
        cout << posicao++ << "o - ";
        if (tarefa != nullptr) imprimirTarefa(tarefa);
        else cout << "(tarefa " << atual->idTarefa << " removida)\n";
    }
}

// Coloca uma tarefa pendente no fim da fila.
void menuEnfileirar(const ListaTarefas& lista, FilaExecucao& fila) {
    int id = lerInteiro("Id da tarefa: ");
    const Tarefa* tarefa = buscarRecursivo(lista.inicio, id);
    if (tarefa == nullptr || tarefa->concluida) {
        cout << "Tarefa inexistente ou ja concluída.\n";
    } else if (filaContem(fila, id)) {
        cout << "Essa tarefa ja esta na fila.\n";
    } else {
        enfileirar(fila, id);
        cout << "Tarefa " << id << " na posicao " << fila.tamanho << ".\n";
    }
}

// Trata as opções da fila: 11 (enfileirar), 12 (executar), 13 (ver).
void menuFila(ListaTarefas& lista, PilhaAcoes& pilha, FilaExecucao& fila,
              int opcao) {
    if (opcao == 11) {
        menuEnfileirar(lista, fila);
    } else if (opcao == 12) {
        Tarefa* feita = executarProxima(lista, pilha, fila);
        if (feita != nullptr) {
            cout << "Executada: ";
            imprimirTarefa(feita);
        } else {
            cout << "Nenhuma tarefa pendente na fila.\n";
        }
    } else {
        mostrarFila(lista, fila);
    }
}

// Trata as opções da árvore: 14  e 15
void menuArvore(const ListaTarefas& lista, int opcao) {
    ArvoreTitulos arvore;
    inicializarArvore(arvore);
    construirIndice(lista, arvore);
    if (opcao == 14) {
        percorrerEmOrdem(arvore.raiz, lista);
        cout << "(altura da arvore: " << alturaArvore(arvore.raiz) << ")\n";
    } else {
        string titulo = lerTexto("Título exato: ");
        const NoArvore* achado = buscarNo(arvore.raiz, minusculas(titulo));
        if (achado != nullptr) imprimirTarefa(buscarPorId(lista, achado->idTarefa));
        else cout << "Titulo nao encontrado.\n";
    }
    liberarArvore(arvore);
}

// Compara as duas buscas sobre as tarefas reais, por id.
void menuBuscaReal(const ListaTarefas& lista) {
    int* ids = copiarIdsOrdenados(lista);
    if (ids == nullptr) {
        cout << "Nenhuma tarefa cadastrada.\n";
        return;
    }
    int id = lerInteiro("Id procurado: ");
    ResultadoBusca seq = buscaSequencial(ids, lista.tamanho, id);
    ResultadoBusca bin = buscaBinaria(ids, lista.tamanho, id);
    cout << "Sequencial: posicao " << seq.posicao << ", "
         << seq.comparacoes << " comparacoes\n"
         << "Binaria:    posicao " << bin.posicao << ", "
         << bin.comparacoes << " comparacoes\n";
    if (bin.posicao >= 0) imprimirTarefa(buscarPorId(lista, id));
    else cout << "Id nao encontrado.\n";
    delete[] ids;
    ids = nullptr;
}

// Trata as opções de busca: 16 (tarefas reais) e 17 (benchmark).
void menuBusca(const ListaTarefas& lista, int opcao) {
    if (opcao == 16) {
        menuBuscaReal(lista);
        return;
    }
    const int tamanhos[] = {10, 1000, 100000, 1000000};
    for (int i = 0; i < 4; i++) {
        compararDesempenho(tamanhos[i]);
    }
}