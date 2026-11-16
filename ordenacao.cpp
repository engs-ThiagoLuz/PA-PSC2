// R9 - Insertion Sort
#pragma once
#include "tarefa.cpp"

// Critérios disponíveis para ordenar a lista.
enum CriterioOrdenacao { POR_PRAZO, POR_PRIORIDADE };


// Define se a tarefa 'a' deve ficar ANTES da tarefa 'b'.
bool vemAntes(const Tarefa* a, const Tarefa* b, CriterioOrdenacao criterio) {
    if (criterio == POR_PRIORIDADE) return a->prioridade < b->prioridade;
    if (a->prazo == 0) return false;
    if (b->prazo == 0) return true;
    return a->prazo < b->prazo;
}

// Insere um nó na posição correta de uma lista que já está ordenada.
void inserirOrdenado(Tarefa*& cabeca, Tarefa* no, CriterioOrdenacao criterio) {
    if (cabeca == nullptr || vemAntes(no, cabeca, criterio)) {
        no->proxima = cabeca; // vira o novo primeiro nó
        cabeca = no;
        return;
    }
    Tarefa* anterior = cabeca;
    while (anterior->proxima != nullptr &&
           !vemAntes(no, anterior->proxima, criterio)) {
        anterior = anterior->proxima; // avança até o ponto de inserção
    }
    no->proxima = anterior->proxima;
    anterior->proxima = no;
}

/* Insertion Sort:
Retira cada nó da lista original e o encaixa na lista ordenada auxiliar. */

void ordenarLista(ListaTarefas& lista, CriterioOrdenacao criterio) {
    Tarefa* ordenada = nullptr;
    Tarefa* atual = lista.inicio;
    while (atual != nullptr) {
        Tarefa* proximo = atual->proxima; // guarda antes de religar
        inserirOrdenado(ordenada, atual, criterio);
        atual = proximo;
    }
    lista.inicio = ordenada;
}
