# Snorlax Logger 📌

## Visão Geral

[cite_start]Este projeto é um exemplo simples, inspirado na ferramenta Snorlax, apresentada no artigo "Lazy Diagnosis of In-Production Concurrency Bugs" (Kasikci et al., 2017)[cite: 13]. [cite_start]O código simula dois threads que adquirem dois locks (mutexes) em ordens diferentes, o que pode gerar um **deadlock**[cite: 14]. [cite_start]Todas as operações de lock e unlock são registradas com timestamps em microssegundos no arquivo `trace.log`[cite: 15].

***

## 📁 Arquivos do Projeto

* `snorlax_log.c`: Código-fonte principal em C.
* [cite_start]`Makefile`: Script de build para compilar e executar o projeto usando o comando `make`[cite: 22].
* `trace.log`: Arquivo de log gerado automaticamente durante a execução, registrando os eventos das threads e mutexes[cite: 15, 16].
* [cite_start]`README.rm`: Este documento[cite: 16].

***

## 🚀 Como Compilar e Executar

Para compilar e executar o projeto, siga os passos abaixo:

1.  **Compilar**: Navegue até o diretório do projeto no terminal e execute o comando `make`. Isso utilizará o `Makefile` para compilar o `snorlax_log.c` e criar o executável `snorlax_log.out`.

    ```bash
    make
    ```

2.  **Executar**: Após a compilação, execute o programa com o comando `make run`. Isso também exibirá o conteúdo do arquivo `trace.log`[cite: 22].

    ```bash
    make run
    ```

3.  **Limpar**: Para remover os arquivos gerados (o executável e o log), execute o comando `make clean`.

    ```bash
    make clean
    ```

***

## 🔬 Entendendo o Comportamento

O programa `snorlax_log.c` cria duas threads: `Thread 1` e `Thread 2`.

* `Thread 1` tenta adquirir os locks na ordem **L1** e depois **L2**.
* `Thread 2` tenta adquirir os locks na ordem **L2** e depois **L1**.

Essa diferença na ordem de aquisição dos locks pode levar a um deadlock, dependendo da temporização da execução das threads.

### Exemplo de saída que não ocorre o deadlock (`trace_not_deadlock.log`)

### Exemplo de saída que ocorre o deadlock (`trace_deadlock.log`)
Neste cenário de deadlock, `Thread 1` adquiriu `L1` e está esperando por `L2`, enquanto `Thread 2` adquiriu `L2` e está esperando por `L1`, resultando em um impasse onde nenhuma das threads pode prosseguir.

***

## 📚 Referências

* Kasikci, B., et al. (2017). "Lazy Diagnosis of In-Production Concurrency Bugs"[cite: 13].
