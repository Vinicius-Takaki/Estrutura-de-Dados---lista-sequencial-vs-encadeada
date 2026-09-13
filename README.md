# Lista Sequencial vs Lista Encadeada — Benchmark em C++

Uma comparação prática entre duas implementações clássicas de estruturas de dados em C++ — uma lista sequencial baseada em array e uma lista simplesmente encadeada — ambas armazenando registros simples de `Person` (nome + ID numérico). Cada operação é instrumentada para contar comparações e movimentações, e o projeto inclui um benchmark automatizado com um gráfico comparativo.

## Estrutura do repositório

```
.
├── sequential-list/
│   └── sequential_list.cpp   # CLI interativa, implementação baseada em array
├── linked-list/
│   └── linked_list.cpp       # CLI interativa, implementação baseada em ponteiros
├── benchmark/
│   ├── benchmark_sequential.cpp  # Medição de custo não interativa (sequencial)
│   ├── benchmark_linked.cpp      # Medição de custo não interativa (encadeada)
│   ├── plot_results.py           # Gera o gráfico comparativo a partir de results.csv
│   ├── run_benchmark.sh           # Compila, roda e plota tudo em um único passo
│   └── results.csv               # Última saída do benchmark
└── assets/
    └── comparison_chart.png      # Gráfico gerado (veja abaixo)
```

## As duas implementações

### Lista sequencial (`sequential-list/`)
Um array de tamanho fixo (`MAX_PEOPLE = 50`). Inserção e remoção exigem deslocar elementos, então a maioria das operações custa `O(n)` no pior caso, mas não há sobrecarga de alocação por nó e a iteração é amigável ao cache.

### Lista encadeada (`linked-list/`)
Nós alocados dinamicamente (`struct Node`) conectados por ponteiros `next`, com referências `head` e `tail`. Inserção/remoção no início é `O(1)`; inserção/remoção no meio ainda exige percorrer a lista (`O(n)`). Por ser uma lista simplesmente encadeada (sem ponteiro `prev`), remover do fim também exige uma travessia completa — um trade-off interessante que aparece claramente no benchmark abaixo.

Ambas as versões suportam:

* Inserir no início / meio / fim
* Remover do início / meio / fim
* Buscar por ID
* Exibir a lista
* Salvar/carregar de um arquivo de texto
* Contadores por operação para comparações `C(n)` e movimentações `M(n)`, além do tempo de execução

## Resultados do benchmark

O benchmark constrói listas de tamanho crescente (de 100 a 10.000 elementos) e mede o número de comparações para cada operação. Rode você mesmo com:

```
cd benchmark
./run_benchmark.sh
```

O que o gráfico mostra:

* **Inserir/remover no início**: a lista sequencial é `O(n)` (precisa deslocar todos os elementos), a lista encadeada é `O(1)` (constante, próximo de zero).
* **Inserir/remover no meio**: ambas são `O(n)` — a lista sequencial desloca elementos, a lista encadeada percorre ponteiros — e o custo é praticamente idêntico.
* **Inserir no fim**: a lista encadeada é `O(1)` graças ao ponteiro `tail`; a lista sequencial também é `O(1)` aqui, já que inserir no fim significa apenas escrever na posição `size`.
* **Remover do fim**: a lista sequencial é `O(1)` (basta decrementar `size`), mas a lista encadeada é `O(n)` — como ela só mantém um ponteiro `next`, precisa percorrer toda a lista para encontrar o nó anterior ao último.
* **Busca (pior caso)**: custo idêntico para ambas, já que nenhuma das duas estruturas permite busca mais rápida que linear em uma lista não ordenada.

Esse último ponto (remoção do fim) ilustra bem por que uma lista duplamente encadeada costuma ser preferida na prática quando há remoção frequente no fim.


Ambos leem/gravam seus dados de um arquivo local `IdName10.txt` no diretório de trabalho.

## Por que este projeto

Desenvolvido como projeto de estudo para a disciplina de Estrutura de Dados, depois organizado para tornar visíveis e mensuráveis — em vez de apenas teóricos — os trade-offs entre armazenamento contíguo e baseado em ponteiros.
