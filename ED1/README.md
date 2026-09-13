# Lista Sequencial vs Lista Encadeada — Benchmark em C++

Uma comparação prática entre duas implementações clássicas de estruturas de dados em C++ — uma lista sequencial baseada em array e uma lista simplesmente encadeada — ambas armazenando registros simples de `Person` (nome + ID numérico). Cada operação é instrumentada para contar comparações e movimentações, e o projeto inclui benchmarks automatizados com gráficos comparativos, tanto para as operações de lista quanto para algoritmos de ordenação.

## Estrutura do repositório

```
.
├── sequential-list/
│   └── sequential_list.cpp       # CLI interativa, lista sequencial (array)
├── linked-list/
│   └── linked_list.cpp           # CLI interativa, lista encadeada (ponteiros)
├── sorting-algorithms/
│   └── sequential_list_sort.cpp  # CLI interativa + busca binária + 6 algoritmos de ordenação
├── benchmark/
│   ├── benchmark_sequential.cpp  # Medição de custo (lista sequencial)
│   ├── benchmark_linked.cpp      # Medição de custo (lista encadeada)
│   ├── benchmark_sorting.cpp     # Medição de custo (algoritmos de ordenação)
│   ├── plot_results.py           # Gera o gráfico de listas a partir de results.csv
│   ├── plot_sorting.py           # Gera o gráfico de ordenação a partir de results_sorting.csv
│   ├── run_benchmark.sh          # Compila, roda e plota tudo (listas)
│   ├── results.csv               # Última saída do benchmark de listas
│   └── results_sorting.csv       # Última saída do benchmark de ordenação
└── assets/
    ├── comparison_chart.png          # Gráfico de listas
    └── sorting_comparison_chart.png  # Gráfico de ordenação
```

## As duas implementações de lista

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

## Benchmark de listas

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

## Algoritmos de ordenação (`sorting-algorithms/`)

Além das operações de lista, o projeto implementa e mede seis algoritmos clássicos de ordenação sobre a lista sequencial, ordenando por `id`:

* **Selection Sort**
* **Insertion Sort**
* **Bubble Sort** (com otimização de parar cedo se nenhuma troca ocorreu)
* **Shell Sort**
* **Quick Sort** (partição estilo Hoare, pivô no elemento do meio)
* **Merge Sort**

A versão interativa também inclui **busca binária** (`binarySearchPerson`), que exige que a lista esteja ordenada previamente.

### Benchmark de ordenação

O benchmark (`benchmark_sorting.cpp`) testa os 6 algoritmos em 3 ordens iniciais dos dados — **aleatória**, **já ordenada** e **ordem reversa** — para tamanhos de 100 a 10.000 elementos, medindo comparações e movimentações em cada combinação:

```
cd benchmark
g++ -O2 -o bench_sorting benchmark_sorting.cpp
./bench_sorting > results_sorting.csv
python3 plot_sorting.py
```

O que o gráfico mostra (eixo Y em escala logarítmica, já que os algoritmos variam de `O(n)` a `O(n²)`):

* **Selection, Insertion e Bubble Sort** são `O(n²)` no pior caso e dominam claramente o gráfico para `n` grande.
* **Shell, Quick e Merge Sort** são `O(n log n)` (ou próximo disso) e formam um grupo bem mais baixo e compacto no gráfico, independente da ordem dos dados.
* **Insertion Sort em lista já ordenada** é o destaque: o laço interno nunca precisa deslocar nada, então o custo cai para praticamente `O(n)` — visível no gráfico como uma curva quase reta e baixa.
* **Bubble Sort em lista já ordenada** também se beneficia da otimização de parada antecipada: zero trocas, só a passada inicial de comparações.
* **Selection Sort** é o único cujo número de *comparações* não muda com a ordem dos dados — ele sempre varre o restante da lista procurando o mínimo, independentemente de já estar ordenado ou não. Só o número de *movimentações* muda.

## Requisitos

* Compilador C++ (g++). No Windows, instale via [MinGW-w64](https://www.mingw-w64.org/) ou use o [WSL](https://learn.microsoft.com/windows/wsl/install) para um ambiente Linux dentro do Windows. No macOS/Linux normalmente já vem instalado ou é só um `apt install g++` / `xcode-select --install`.
* Python 3 com `pandas` e `matplotlib` (só necessário para gerar os gráficos): `pip install pandas matplotlib`.

### Rodando no VS Code

1. Instale a extensão **C/C++** (da Microsoft) e confirme que o `g++` está disponível rodando `g++ --version` no terminal integrado (`` Ctrl+` ``).
2. Abra a pasta raiz do repositório com **File → Open Folder**.
3. Use o terminal integrado do VS Code para compilar e rodar, por exemplo:
   ```
   cd benchmark
   g++ -O2 -o bench_sorting benchmark_sorting.cpp
   ./bench_sorting > results_sorting.csv
   python3 plot_sorting.py
   ```
4. **Atenção no Windows sem WSL**: o executável se chama `bench_sorting.exe`, então o comando de rodar é `.\bench_sorting.exe > results_sorting.csv` em vez de `./bench_sorting`. O script `run_benchmark.sh` é bash e só roda direto em Linux/macOS ou dentro do WSL — no PowerShell/cmd, rode os comandos de `g++` e `python` manualmente (sem o `.sh`).

## Como compilar e rodar os programas interativos

```
# Lista sequencial
g++ -O2 -o sequential_list sequential-list/sequential_list.cpp
./sequential_list

# Lista encadeada
g++ -O2 -o linked_list linked-list/linked_list.cpp
./linked_list

# Lista sequencial com ordenação e busca binária
g++ -O2 -o sequential_list_sort sorting-algorithms/sequential_list_sort.cpp
./sequential_list_sort
```

Os programas leem/gravam seus dados de um arquivo de texto local (`IdName10.txt` ou `IdName10K.txt`, conforme o programa) no diretório de trabalho.

## Por que este projeto

Desenvolvido como projeto de estudo para a disciplina de Estrutura de Dados, depois organizado para tornar visíveis e mensuráveis — em vez de apenas teóricos — os trade-offs entre armazenamento contíguo e baseado em ponteiros, e entre os diferentes algoritmos clássicos de ordenação.
