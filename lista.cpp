// Lista encadeada e recursividade

#pragma once
#include <iostream>
#include "tarefa.cpp"
using namespace std;

// Prepara uma lista vazia.
void inicializarLista(ListaTarefas& lista) {
    lista.inicio = nullptr;
    lista.tamanho = 0;
    lista.proximoId = 1;
}

// Aloca dinamicamente (new) um nó de tarefa.
Tarefa* criarTarefa(int id, const string& titulo, int prioridade, int prazo) {
    Tarefa* nova = new Tarefa;
    nova->id = id;
    nova->titulo = titulo;
    nova->prioridade = prioridade;
    nova->prazo = prazo;
    nova->concluida = false;
    nova->proxima = nullptr;
    return nova;
}

// Anexa um nó já alocado no final da lista.
void anexarNo(ListaTarefas& lista, Tarefa* nova) {
    if (lista.inicio == nullptr) {
        lista.inicio = nova;
    } else {
        Tarefa* atual = lista.inicio;
        while (atual->proxima != nullptr) {
            atual = atual->proxima;
        }
        atual->proxima = nova;
    }
    lista.tamanho++;
}

// Cria e insere uma tarefa no final da lista.
int inserirTarefa(ListaTarefas& lista, const string& titulo,
                  int prioridade = 2, int prazo = 0) {
    Tarefa* nova = criarTarefa(lista.proximoId, titulo, prioridade, prazo);
    anexarNo(lista, nova);
    return lista.proximoId++;
}

// Busca sequencial por id.

Tarefa* buscarPorId(const ListaTarefas& lista, int id) {
    Tarefa* atual = lista.inicio;
    while (atual != nullptr && atual->id != id) {
        atual = atual->proxima;
    }
    return atual;
}

// Remove uma tarefa pelo id e libera sua memória (delete).
bool removerTarefa(ListaTarefas& lista, int id) {
    Tarefa* atual = lista.inicio;
    Tarefa* anterior = nullptr;
    while (atual != nullptr && atual->id != id) {
        anterior = atual;
        atual = atual->proxima;
    }
    if (atual == nullptr) return false;
    if (anterior == nullptr) {
        lista.inicio = atual->proxima;   // removendo o primeiro nó
    } else {
        anterior->proxima = atual->proxima;
    }
    delete atual;
    atual = nullptr;
    lista.tamanho--;
    return true;
}

// Marca uma tarefa como concluída.
bool concluirTarefa(ListaTarefas& lista, int id) {
    Tarefa* tarefa = buscarPorId(lista, id);
    if (tarefa == nullptr) return false;
    tarefa->concluida = true;
    return true;
}

// Imprime uma única tarefa no terminal.
void imprimirTarefa(const Tarefa* tarefa) {
    cout << "[" << tarefa->id << "] "
         << (tarefa->concluida ? "[x] " : "[ ] ")
         << tarefa->titulo
         << " | prioridade: " << tarefa->prioridade
         << " | prazo: " << tarefa->prazo << "\n";
}

// Percorre a lista e imprime todas as tarefas.
void listarTarefas(const ListaTarefas& lista) {
    if (lista.inicio == nullptr) {
        cout << "Nenhuma tarefa cadastrada.\n";
        return;
    }
    for (Tarefa* atual = lista.inicio; atual != nullptr; atual = atual->proxima) {
        imprimirTarefa(atual);
    }
}

// Libera todos os nós da lista.
void liberarLista(ListaTarefas& lista) {
    Tarefa* atual = lista.inicio;
    while (atual != nullptr) {
        Tarefa* proximo = atual->proxima;
        delete atual;
        atual = proximo;
    }
    lista.inicio = nullptr;
    lista.tamanho = 0;
}

// Recursividade (R3)

// Conta quantas tarefas existem a partir de um nó.
int contarTarefas(const Tarefa* no) {
    if (no == nullptr) return 0;
    return 1 + contarTarefas(no->proxima); 
}

// Conta quantas tarefas ainda não foram concluídas.
int contarPendentes(const Tarefa* no) {
    if (no == nullptr) return 0;
    int atual = no->concluida ? 0 : 1;
    return atual + contarPendentes(no->proxima);
}

// Busca recursiva por id.
const Tarefa* buscarRecursivo(const Tarefa* no, int id) {
    if (no == nullptr || no->id == id) return no;
    return buscarRecursivo(no->proxima, id);
}

// Imprime a lista do último para o primeiro nó.
void imprimirInvertida(const Tarefa* no) {
    if (no == nullptr) return;
    imprimirInvertida(no->proxima);
    imprimirTarefa(no);
}
