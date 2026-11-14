// (R1, R2, R4, R5)

#include <iostream>
#include <string>
using namespace std;

// Nó da lista encadeada: Cada tarefa aponta para a próxima.
struct Tarefa {
    int id;
    string titulo;
    int prioridade; // 1 = alta, 2 = média, 3 = baixa
    int prazo; 
    bool concluida;
    Tarefa* proxima;
};

// Cabeça da lista: guarda o primeiro nó e controla tamanho e ids.
struct ListaTarefas {
    Tarefa* inicio;
    int tamanho;
    int proximoId;
};

// Prepara uma lista vazia.
// Parâmetro: lista (passagem por referência).
void inicializarLista(ListaTarefas& lista) {
    lista.inicio = nullptr;
    lista.tamanho = 0;
    lista.proximoId = 1;
}

/* Aloca dinamicamente (new) um nó de tarefa.
   Parâmetros: id, titulo, prioridade, prazo.
   Retorno: ponteiro para o novo nó (proxima = nullptr). */
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

/* Insere uma tarefa no final da lista.
   Parâmetros: lista, titulo, prioridade (padrão 2), prazo (padrão 0).
   Retorno: id da tarefa criada. */
int inserirTarefa(ListaTarefas& lista, const string& titulo,
                  int prioridade = 2, int prazo = 0) {
    Tarefa* nova = criarTarefa(lista.proximoId, titulo, prioridade, prazo);
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
    return lista.proximoId++;
}

/* Busca sequencial por id.
   Parâmetros: lista, id procurado.
   Retorno: ponteiro para a tarefa ou nullptr se não existir. */
Tarefa* buscarPorId(const ListaTarefas& lista, int id) {
    Tarefa* atual = lista.inicio;
    while (atual != nullptr && atual->id != id) {
        atual = atual->proxima;
    }
    return atual;
}

/* Remove uma tarefa pelo id e libera sua memória.
   Parâmetros: lista, id da tarefa.
   Retorno: true se removeu, false se não encontrou. */
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

/* Marca uma tarefa como concluída.
   Parâmetros: lista, id da tarefa.
   Retorno: true se encontrou e marcou, false caso contrário. */
bool concluirTarefa(ListaTarefas& lista, int id) {
    Tarefa* tarefa = buscarPorId(lista, id);
    if (tarefa == nullptr) return false;
    tarefa->concluida = true;
    return true;
}

/* Imprime uma única tarefa no terminal.
   Parâmetro: tarefa (ponteiro constante, não altera o nó). */
void imprimirTarefa(const Tarefa* tarefa) {
    cout << "[" << tarefa->id << "] "
         << (tarefa->concluida ? "[x] " : "[ ] ")
         << tarefa->titulo
         << " | prioridade: " << tarefa->prioridade
         << " | prazo: " << tarefa->prazo << "\n";
}

/* Percorre a lista e imprime todas as tarefas.
   Parâmetro: lista (somente leitura). */
void listarTarefas(const ListaTarefas& lista) {
    if (lista.inicio == nullptr) {
        cout << "Nenhuma tarefa cadastrada.\n";
        return;
    }
    for (Tarefa* atual = lista.inicio; atual != nullptr; atual = atual->proxima) {
        imprimirTarefa(atual);
    }
}

/* Libera todos os nós da lista.
   Parâmetro: lista, que volta ao estado vazio. */
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

// Pilha de ações para desfazer

/* Prepara uma pilha vazia.
   Parâmetro: pilha (por referência). */
void inicializarPilha(PilhaAcoes& pilha) {
    pilha.topo = nullptr;
    pilha.tamanho = 0;
}

/* Verifica se a pilha está vazia.
   Retorno: true se não há ações para desfazer. */
bool pilhaVazia(const PilhaAcoes& pilha) {
    return pilha.topo == nullptr;
}

/* Empilha uma ação, guardando uma cópia da tarefa. Custo O(1).
   Parâmetros: pilha, tipo da ação, tarefa (copiada antes de ser alterada). */
void empilhar(PilhaAcoes& pilha, TipoAcao tipo, const Tarefa& tarefa) {
    Acao* nova = new Acao;
    nova->tipo = tipo;
    nova->copia = tarefa;
    nova->copia.proxima = nullptr;
    nova->abaixo = pilha.topo;
    pilha.topo = nova;
    pilha.tamanho++;
}

/* Desempilha (pop) a ação do topo e libera o nó. Custo O(1).
   Parâmetros: pilha, destino (recebe a ação removida).
   Retorno: true se desempilhou, false se a pilha estava vazia. */
bool desempilhar(PilhaAcoes& pilha, Acao& destino) {
    if (pilhaVazia(pilha)) return false;
    Acao* removida = pilha.topo;
    destino = *removida;
    destino.abaixo = nullptr;
    pilha.topo = removida->abaixo;
    delete removida;
    removida = nullptr;
    pilha.tamanho--;
    return true;
}

/* Libera todos os nós restantes da pilha.
   Parâmetro: pilha, que volta ao estado vazio. */
void liberarPilha(PilhaAcoes& pilha) {
    Acao* atual = pilha.topo;
    while (atual != nullptr) {
        Acao* abaixo = atual->abaixo;
        delete atual;
        atual = abaixo;
    }
    pilha.topo = nullptr;
    pilha.tamanho = 0;
}

/* Recoloca na lista uma tarefa removida, mantendo o id original.
   Parâmetros: lista, modelo */
void restaurarTarefa(ListaTarefas& lista, const Tarefa& modelo) {
    Tarefa* nova = criarTarefa(modelo.id, modelo.titulo,
                               modelo.prioridade, modelo.prazo);
    nova->concluida = modelo.concluida;
    anexarNo(lista, nova);
}

/* Desfaz a última ação registrada.
   Parâmetros: lista, pilha.
   Retorno: true se desfez algo, false se não havia ação. */
bool desfazerUltimaAcao(ListaTarefas& lista, PilhaAcoes& pilha) {
    Acao acao;
    if (!desempilhar(pilha, acao)) return false;
    if (acao.tipo == ADICAO) {
        removerTarefa(lista, acao.copia.id);       // desfaz o cadastro
    } else if (acao.tipo == REMOCAO) {
        restaurarTarefa(lista, acao.copia);        // desfaz a remoção
    } else {
        Tarefa* tarefa = buscarPorId(lista, acao.copia.id);
        if (tarefa != nullptr) tarefa->concluida = false;
    }
    return true;
}

// Interface

/* Lê um número inteiro do teclado, exibindo uma mensagem.
   Retorno: valor digitado. */
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

/* Lê uma linha de texto do teclado.
   Retorno: texto digitado. */
string lerTexto(const string& mensagem) {
    string texto;
    cout << mensagem;
    getline(cin, texto);
    return texto;
}

/* Fluxo de cadastro: pede os dados e insere na lista.
   Parâmetro: lista. */
void menuAdicionar(ListaTarefas& lista) {
    string titulo = lerTexto("Titulo: ");
    int prioridade = lerInteiro("Prioridade (1-alta, 2-media, 3-baixa): ");
    int prazo = lerInteiro("Prazo (AAAAMMDD, 0 = sem prazo): ");
    int id = inserirTarefa(lista, titulo, prioridade, prazo);
    cout << "Tarefa criada com id " << id << ".\n";
}

/* Exibe as opções do menu principal. */
void mostrarMenu() {
    cout << "\n=== Gestor de Tarefas ===\n"
         << "1 - Adicionar tarefa\n"
         << "2 - Listar tarefas\n"
         << "3 - Concluir tarefa\n"
         << "4 - Remover tarefa\n"
         << "0 - Sair\n";
}

/* Trata as opções que dependem de um id (concluir/remover).
   Parâmetros: lista, opcao escolhida (3 ou 4). */
void menuPorId(ListaTarefas& lista, int opcao) {
    int id = lerInteiro("Id da tarefa: ");
    bool ok = (opcao == 3) ? concluirTarefa(lista, id)
                           : removerTarefa(lista, id);
    cout << (ok ? "Operacao realizada.\n" : "Tarefa nao encontrada.\n");
}

int main() {
    ListaTarefas lista;
    inicializarLista(lista);
    int opcao;
    do {
        mostrarMenu();
        opcao = lerInteiro("Opcao: ");
        if (opcao == 1) menuAdicionar(lista);
        else if (opcao == 2) listarTarefas(lista);
        else if (opcao == 3 || opcao == 4) menuPorId(lista, opcao);
    } while (opcao != 0);
    liberarLista(lista);
    return 0;
}
