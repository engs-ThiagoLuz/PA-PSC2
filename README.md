# Gestor de Tarefas em C++

**Integrante:** `Thiago Ferreira da Luz`

## 1. Descrição do sistema

O **Gestor de Tarefas** é um sistema de terminal, escrito apenas em C++ (biblioteca padrão), para cadastrar tarefas com prioridade e prazo e organizá-las. Todas as estruturas de dados (lista, pilha, fila e árvore) foram implementadas manualmente, sem `std::list`, `std::stack`, `std::queue` ou `std::map`.

Funcionalidades:

- Cadastrar, listar, concluir e remover tarefas (título, prioridade 1–3, prazo `AAAAMMDD`).
- **Desfazer** a última ação (estilo Ctrl+Z): cadastro, remoção ou conclusão.
- **Ordenar** por prazo ou por prioridade (Insertion Sort).
- **Fila de execução:** executar tarefas na ordem em que foram agendadas.
- **Índice por título** em árvore binária de busca: listagem de A a Z e busca por título.
- **Buscar por id** com busca sequencial e binária, comparando comparações e tempo.
- Operações recursivas sobre a lista: contagem, resumo, busca e listagem inversa.

## 2. Tema escolhido

**Gestor de Tarefas**: lista de tarefas com prioridades, pilha de ações desfeitas (Ctrl+Z), ordenação por prazo ou prioridade e busca por título.

## 3. Como compilar e executar

```bash
g++ -std=c++11 -Wall -Wextra main.cpp -o gestor_tarefas
./gestor_tarefas
```

> Não compile todos os .cpp juntos: As definições seriam duplicadas


## 4. Organização do código

| Arquivo | Conteúdo |
|---|---|
| `main.cpp` | Laço principal e despacho das opções do menu |
| `tarefa.cpp` | `struct Tarefa` e `struct ListaTarefas` |
| `lista.cpp` | Lista encadeada (R5) e funções recursivas (R3) |
| `pilha.cpp` | Pilha de desfazer (R6) |
| `fila.cpp` | Fila de execução (R7) |
| `arvore.cpp` | Árvore binária de busca de títulos (R8) |
| `ordenacao.cpp` | Insertion Sort sobre a lista (R9) |
| `busca.cpp` | Busca sequencial, busca binária e benchmark (R10) |
| `interface.cpp` | Menus e leitura do teclado |

## 5. Menu

| Opção | Ação |
|---|---|
| 1 | Adicionar tarefa |
| 2 | Listar tarefas |
| 3 | Concluir tarefa |
| 4 | Remover tarefa |
| 5 | Desfazer última ação |
| 6 | Resumo (total e pendentes), recursivo |
| 7 | Buscar por id, recursivo |
| 8 | Listar em ordem inversa, recursivo |
| 9 | Ordenar por prazo (Insertion Sort) |
| 10 | Ordenar por prioridade (Insertion Sort) |
| 11 | Colocar tarefa na fila de execução |
| 12 | Executar próxima da fila |
| 13 | Ver fila de execução |
| 14 | Listar por título A–Z (árvore) |
| 15 | Buscar por título (árvore) |
| 16 | Buscar por id: sequencial × binária |
| 17 | Benchmark: sequencial × binária (vários tamanhos) |
| 0 | Sair |

## 6. Estruturas de dados utilizadas

| Estrutura | Onde é usada | Por que foi escolhida |
|---|---|---|
| **Lista encadeada** | Armazenamento principal das tarefas | Inserção e remoção sem deslocar elementos; o tamanho varia livremente |
| **Pilha (LIFO)** | Histórico de ações para desfazer | A última ação feita é a primeira a ser desfeita |
| **Fila (FIFO)** | Fila de execução das tarefas | A primeira tarefa agendada é a primeira a ser executada |
| **Árvore binária de busca** | Índice de títulos | Travessia in-ordem entrega a ordem alfabética; busca descarta metade da árvore por comparação |
| **Vetor dinâmico (`new[]`)** | Cópia ordenada dos ids para a busca binária | A busca binária exige acesso direto por índice, que a lista não oferece |


Decisões de projeto:

- A **pilha** guarda uma **cópia** da tarefa, não um ponteiro: após um `delete`, o ponteiro ficaria inválido.
- A **fila** e a **árvore** guardam o **id** da tarefa, não um ponteiro para o nó da lista, pelo mesmo motivo. Ids de tarefas removidas são descartados ao executar a fila.
- O **índice da árvore** é reconstruído a partir da lista a cada consulta, o que o mantém sempre atualizado após remoções e desfazer, sem precisar de remoção em BST.

## 7. Requisitos técnicos (R1–R10)

| Requisito | Onde aparece |
|---|---|
| **R1 Funções** | Todas as funções têm responsabilidade única, até 30 linhas e comentário de cabeçalho. Passagem por valor (`int id`), por referência (`ListaTarefas&`), por referência constante (`const string&`) e parâmetros default em `inserirTarefa(lista, titulo, prioridade = 2, prazo = 0)` |
| **R2 Ponteiros** | Encadeamento de todas as estruturas; `Tarefa*&` em `inserirOrdenado`; ponteiro para função em `medirMicrossegundos` |
| **R3 Recursividade** | `contarTarefas`, `contarPendentes`, `buscarRecursivo`, `imprimirInvertida` (`lista.cpp`); `inserirNo`, `buscarNo`, `percorrerEmOrdem`, `alturaArvore`, `liberarNos` (`arvore.cpp`) |
| **R4 Memória dinâmica** | `new` em `criarTarefa`, `empilhar`, `enfileirar`, `criarNoArvore`, `new[]` em `copiarIdsOrdenados`; `delete`/`delete[]` correspondentes, com `nullptr` nos ponteiros liberados |
| **R5 Lista encadeada** | `inserirTarefa`, `removerTarefa`, `listarTarefas` (`lista.cpp`) |
| **R6 Pilha** | `empilhar`, `desempilhar`, `desfazerUltimaAcao` (`pilha.cpp`) |
| **R7 Fila** | `enfileirar`, `desenfileirar`, `executarProxima` (`fila.cpp`) |
| **R8 Árvore binária de busca** | Inserção, busca e travessia in-ordem por título (`arvore.cpp`) |
| **R9 Ordenação** | Insertion Sort sobre a lista (`ordenacao.cpp`), com análise de complexidade comentada |
| **R10 Busca** | `buscaSequencial` e `buscaBinaria` com contagem de comparações e medição de tempo (`busca.cpp`) |

## 8. Complexidade dos algoritmos

| Operação | Complexidade |
|---|---|
| Inserir no final da lista | O(n) (percorre até o fim) |
| Remover / buscar por id na lista | O(n) |
| Empilhar / desempilhar | O(1) |
| Enfileirar / desenfileirar | O(1) (ponteiro `tras` mantido) |
| Insertion Sort na lista | O(n²) no pior caso e no caso médio; espaço extra O(1) |
| Inserir / buscar na árvore | O(log n) em média; O(n) no pior caso (árvore degenerada) |
| Busca sequencial | O(n) |
| Busca binária | O(log n), exige vetor ordenado |

Resultado da demonstração comparativa (opção 17), em número de comparações para achar o último elemento:

| n | sequencial | binária |
|---|---|---|
| 10 | 10 | 4 |
| 1.000 | 1.000 | 10 |
| 100.000 | 100.000 | 17 |
| 1.000.000 | 1.000.000 | 20 |

Os tempos em microssegundos também são exibidos pela opção 17 e variam conforme o computador.

## 9. Gerenciamento de memória

- Todo nó é criado com `new` e liberado com `delete` quando sai da estrutura (`removerTarefa`, `desempilhar`, `desenfileirar`, `liberarNos`).
- Ao encerrar, `liberarLista`, `liberarPilha` e `liberarFila` liberam o que restou.
- A árvore e o vetor de ids são temporários. Criados e liberados dentro da própria operação do menu.
- Ponteiros liberados recebem `nullptr`.
- O projeto foi testado com AddressSanitizer e UndefinedBehaviorSanitizer, sem erros e vazamentos.
