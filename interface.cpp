
#pragma once
#include <iostream>
#include "lista.cpp"
#include "pilha.cpp"
#include "ordenacao.cpp"
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
