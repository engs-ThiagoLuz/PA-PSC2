// Lista encadeada de tarefas (R2, R5)
#pragma once
#include <string>

// Nó da lista encadeada
struct Tarefa {
    int id;
    std::string titulo;
    int prioridade; // 1 = alta, 2 = média, 3 = baixa
    int prazo; // formato AAAAMMDD (0 para sem prazo)
    bool concluida;
    Tarefa* proxima;
};

// Cabeça da lista
struct ListaTarefas {
    Tarefa* inicio;
    int tamanho;
    int proximoId;
};
