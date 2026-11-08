# PSE em Ação — Sistema de Planejamento e Acompanhamento de Ações de Saúde na Escola

Atividade de Estudo Prático (AEP) — Universidade Cesumar (UniCesumar)
Análise e Desenvolvimento de Sistemas — Série ADSIS2S-N-A — 2º semestre de 2026

Este repositório reúne a documentação da **1ª etapa** da AEP (análise do problema, requisitos, fluxogramas, pseudocódigos e planejamento do desenvolvimento) e a **2ª etapa**: o programa em linguagem C (`src/main.c`), os casos de teste automatizados e a documentação da implementação.

**Status:** implementação da etapa 2 concluída. Todas as opções do menu estão implementadas e testadas (27 casos de teste); a sprint 6 (documentação final e apresentação) está em andamento.

## Integrantes da equipe

| Integrantes | Responsabilidade principal |
|---|---|
| Nicollas Daniel - 26006093-2 | Coordenação, produção textual e documentação (METEP) |
| Mateus Chagas - 26004950-2 | Modelagem, requisitos, fluxogramas e pseudocódigos |
| Lucas Piaza - 26014182-2| Implementação, testes, pesquisa, estruturação |

## Sobre a proposta

Aplicação de terminal para apoiar uma equipe intersetorial fictícia (IntegraPSE) no planejamento, registro e acompanhamento de **ações coletivas** do Programa Saúde na Escola (PSE), instituído pelo Decreto nº 6.286/2007.

A proposta prevê o cadastro de ações, a consulta por código, escola ou tema, a atualização da situação de cada ação (planejada, realizada ou cancelada), o registro da quantidade efetiva de participantes e a geração de um resumo geral com o percentual de participação em relação ao previsto.

**Questão norteadora:** como uma aplicação desenvolvida em linguagem C pode auxiliar uma equipe escolar e de saúde a planejar, registrar e acompanhar ações coletivas do Programa Saúde na Escola, apresentando informações claras e preservando a privacidade dos estudantes?

## Limites éticos do projeto

- O sistema registra **ações coletivas**, nunca estudantes individualmente.
- Não são cadastrados nome, matrícula, prontuário, diagnóstico ou condição clínica.
- O programa **não** realiza diagnóstico, triagem médica, prescrição ou recomendação de tratamento.
- Todos os dados utilizados em exemplos e demonstrações são **fictícios**.

## Escopo definido (requisitos funcionais)

| ID | Requisito |
|---|---|
| RF01 | Cadastrar ação do PSE (código, escola, tema, data prevista, público-alvo, responsável e quantidade prevista); a situação inicial é atribuída automaticamente como Planejada |
| RF02 | Listar todas as ações cadastradas |
| RF03 | Pesquisar ações por código, escola ou tema |
| RF04 | Atualizar a situação da ação (planejada, realizada ou cancelada) |
| RF05 | Registrar a quantidade efetiva de participantes das ações realizadas |
| RF06 | Gerar resumo geral com totais por situação, participantes e percentual de participação |
| RF07 | Validar entradas (código duplicado, quantidade negativa, campo vazio, opção inexistente) |
| RF08 | Encerrar a aplicação por opção do menu |

Os requisitos não funcionais (usabilidade, portabilidade, manutenibilidade, privacidade, confiabilidade e capacidade) estão descritos em [`docs/entrega1/requisitos.md`](docs/entrega1/requisitos.md). A versão final dos requisitos, com as mudanças feitas na implementação, os motivos e a matriz de rastreabilidade, está em [`docs/entrega2/requisitos_atualizados.md`](docs/entrega2/requisitos_atualizados.md).

## Como compilar e executar

O programa é um único arquivo, `src/main.c`, e usa apenas a biblioteca padrão do C (`stdio.h`, `stdlib.h`, `string.h` e `ctype.h`). Os dados ficam só na memória: ao encerrar o programa, as ações cadastradas são perdidas.

### Windows — Dev-C++

1. Abra o Dev-C++.
2. Vá em **Arquivo > Abrir** e selecione o arquivo `src/main.c` deste repositório. Abra o `.c` diretamente, sem criar projeto.
3. Vá em **Executar > Compilar & Executar** (ou pressione **F11**).
4. O programa abre no console e mostra o menu.

Se o console fechar sozinho ao terminar, abra o **Prompt de Comando**, entre na pasta `src` (onde o Dev-C++ gera o executável, com o mesmo nome do arquivo-fonte) e execute o `.exe` gerado por ali. Outra opção é compilar pelo terminal, como descrito abaixo, e executar o `pse.exe`.

### Terminal com gcc (Windows, Linux e Mac)

Na pasta raiz do repositório, compile com um dos dois comandos abaixo (são os dois modos usados para conferir que o código compila sem erros e sem warnings):

```bash
gcc -std=gnu89 -Wall -Wextra -Wdeclaration-after-statement -o pse src/main.c
gcc -std=c99 -Wall -Wextra -pedantic -o pse src/main.c
```

Depois execute:

- Linux e Mac: `./pse`
- Windows (Prompt de Comando): `pse.exe`

Observações por sistema:

- **Mac:** se o comando `gcc` não existir, instale as ferramentas de linha de comando com `xcode-select --install`.
- **Windows:** o `gcc` precisa estar no `PATH`. Pode ser o MinGW ou o próprio gcc que acompanha o Dev-C++ (basta adicionar a pasta `bin` do compilador ao `PATH`).
- **Linux:** se o `gcc` não estiver instalado, instale-o pelo gerenciador de pacotes da distribuição.

### VS Code

1. Abra a pasta do repositório no VS Code (**Arquivo > Abrir Pasta**).
2. Abra o terminal integrado (**Terminal > Novo Terminal**).
3. Use os mesmos comandos de compilação e execução da seção anterior.

Nenhuma extensão é obrigatória. A extensão C/C++ é opcional e só ajuda na edição do código.

### Dicas de uso

- **Digite sem acentos.** O console do Windows pode exibir acentos de forma errada.
- Para sair, use a **opção 7** do menu.
- Se a entrada de dados acabar, o programa encerra sozinho com a mensagem `Fim da entrada de dados. Programa encerrado.` No terminal, isso acontece com **Ctrl+D** (Mac/Linux) ou com **Ctrl+Z** seguido de **Enter** no início de uma linha (Windows).

### Testes automatizados

```bash
./tests/rodar_testes.sh
```

Funciona no Mac e no Linux; no Windows, rode pelo **Git Bash** (com o `gcc` no `PATH`). O script compila nos dois modos, executa os 27 casos de `tests/casos/`, grava as saídas em `tests/saidas/` e confere cada uma com `tests/esperado/`. A tabela de casos de teste e os resultados obtidos estão em [`docs/entrega2/testes.md`](docs/entrega2/testes.md).

## Opções do menu

| Opção | Texto no menu | O que faz |
|---|---|---|
| 1 | Cadastrar acao | Cadastra uma ação com código (até 10 caracteres, sem repetir), escola, tema, data prevista (DD/MM/AAAA), público-alvo, responsável e quantidade prevista de participantes. Toda ação nova começa como Planejada. |
| 2 | Listar acoes | Mostra todas as ações cadastradas, com a situação em texto, e o total listado. |
| 3 | Pesquisar acao | Pesquisa por código (igual), escola ou tema (contém o termo), sem diferenciar maiúsculas e minúsculas. Mostra os registros encontrados ou `Nenhum registro encontrado.` |
| 4 | Atualizar situacao | Localiza a ação pelo código e muda a situação para Planejada, Realizada ou Cancelada. Se Realizada, pede a quantidade efetiva de participantes; se Planejada ou Cancelada, a quantidade efetiva volta a 0. |
| 5 | Registrar participantes | Registra ou corrige a quantidade efetiva de participantes de uma ação que já está Realizada. |
| 6 | Gerar resumo | Mostra o total de ações, as quantidades planejadas, realizadas e canceladas, os participantes previstos e efetivos e o percentual de participação (considera somente as ações realizadas). |
| 7 | Encerrar | Encerra o programa. |

### Exemplo rápido de uso

Entradas digitadas (dados fictícios), uma por linha: opção `1` e os dados da ação `ALI01`, com uma quantidade negativa (`-5`) antes da correta (`60`); opção `4`, código `ali01` (em minúsculas), situação `2` (Realizada) e `54` participantes; opção `6`; uma opção inexistente (`9`); e opção `7`.

Trechos da saída real do programa (as repetições do menu foram omitidas):

```text
--- Cadastrar acao ---
Codigo da acao (ate 10 caracteres): Escola: Tema: Data prevista (DD/MM/AAAA): Publico-alvo (ex.: 5o ano - turmas A e B): Responsavel: Quantidade prevista de participantes: Quantidade invalida. Digite um valor maior que zero (maximo 100000).
Quantidade prevista de participantes:
Acao cadastrada com sucesso! Codigo: ALI01 - Situacao: Planejada
Acoes cadastradas: 1 de 100.

--- Atualizar situacao ---
Codigo da acao que sera atualizada: Acao ALI01 - situacao atual: Planejada
Nova situacao: 1 - Planejada   2 - Realizada   3 - Cancelada
Escolha a nova situacao: Quantidade efetiva de participantes:
Situacao atualizada com sucesso! Acao ALI01 agora esta: Realizada.
Participantes efetivos registrados: 54 (previstos: 60).

--- Resumo geral ---
Total de acoes cadastradas......: 1
Acoes planejadas................: 0
Acoes realizadas................: 1
Acoes canceladas................: 0

Participacao (somente acoes realizadas):
Participantes previstos.........: 60
Participantes efetivos..........: 54
Percentual de participacao......: 90.0%
(participantes efetivos / previstos das acoes realizadas)

Escolha uma opcao:
Opcao invalida. Digite um numero de 1 a 7.

Escolha uma opcao:
Programa encerrado. Ate logo!
```

Como a entrada foi redirecionada de um arquivo, os textos digitados não aparecem na saída (por isso os rótulos aparecem em sequência na mesma linha). Um roteiro completo de demonstração, com 6 ações fictícias, está em [`docs/entrega2/dados_demonstracao.md`](docs/entrega2/dados_demonstracao.md).

## Estrutura do repositório

```
Pse-em-acao/
├── README.md
├── .gitignore                      (executáveis e arquivos do sistema/editor)
├── .gitattributes                  (finais de linha dos scripts e dos casos de teste)
├── src/
│   └── main.c                      (código-fonte do programa)
├── tests/
│   ├── rodar_testes.sh             (compila e executa todos os casos)
│   ├── casos/                      (27 arquivos CT01..CT27: entradas de cada caso)
│   ├── esperado/                   (27 arquivos CT01..CT27: trechos esperados na saída)
│   └── saidas/                     (27 arquivos CT01..CT27: saídas reais, como evidência)
└── docs/
    ├── entrega1/
    │   ├── AEP_Entrega1_PSE_em_Acao.pdf  (documento escrito da 1ª etapa)
    │   ├── requisitos.md           (RF e RNF em formato consultável)
    │   └── sprints.md              (planejamento e riscos)
    ├── entrega2/
    │   ├── requisitos_atualizados.md  (requisitos finais, mudanças e rastreabilidade)
    │   ├── testes.md               (tabela de casos de teste e resultados)
    │   ├── explicacao_codigo.md    (explicação do código, função por função)
    │   └── dados_demonstracao.md   (dados fictícios e roteiro da demonstração)
    └── fluxogramas/
        ├── fluxograma_geral.png
        ├── fluxograma_cadastro_parte1.png
        └── fluxograma_cadastro_parte2.png
```

### Documentos

1ª etapa:

- [Documento escrito da 1ª etapa (PDF)](docs/entrega1/AEP_Entrega1_PSE_em_Acao.pdf)
- [Requisitos funcionais e não funcionais](docs/entrega1/requisitos.md)
- [Planejamento das sprints e riscos](docs/entrega1/sprints.md)
- [Fluxograma geral](docs/fluxogramas/fluxograma_geral.png)
- [Fluxograma do cadastro — parte 1](docs/fluxogramas/fluxograma_cadastro_parte1.png)
- [Fluxograma do cadastro — parte 2](docs/fluxogramas/fluxograma_cadastro_parte2.png)

2ª etapa:

- [Código-fonte (`src/main.c`)](src/main.c)
- [Requisitos atualizados e matriz de rastreabilidade](docs/entrega2/requisitos_atualizados.md)
- [Testes](docs/entrega2/testes.md)
- [Explicação do código](docs/entrega2/explicacao_codigo.md)
- [Dados e roteiro da demonstração](docs/entrega2/dados_demonstracao.md)
- [Script de testes](tests/rodar_testes.sh)

## Conteúdo da 1ª etapa

- Documento acadêmico com contextualização, problema, objetivos e justificativa.
- Identificação dos usuários e delimitação do escopo do sistema.
- Oito requisitos funcionais e seis requisitos não funcionais.
- Fluxograma geral do sistema e fluxograma detalhado da operação de cadastro.
- Pseudocódigos do programa principal e das quatro rotinas centrais.
- Definição da estrutura de dados prevista (registro e vetor em memória).
- Planejamento em sprints semanais e matriz de riscos.

## Conteúdo da 2ª etapa

- Programa em C (`src/main.c`) com menu em laço de repetição, registro `AcaoPSE`, vetor em memória e uma função para cada funcionalidade.
- Leitura segura das entradas (baseada em `fgets`) e validações: código duplicado, campos obrigatórios vazios, data, quantidades inválidas, opções inexistentes, registro inexistente e limite de armazenamento.
- 27 casos de teste automatizados, com entradas, trechos esperados e saídas reais.
- Requisitos atualizados com as mudanças em relação à 1ª etapa e matriz de rastreabilidade.
- Material de apoio para o vídeo: explicação do código e roteiro de demonstração.

## Planejamento (processo incremental, sprints semanais)

| Sprint | Foco | Situação |
|---|---|---|
| 1 | Levantamento, contextualização, problema, objetivos e justificativa | Concluída |
| 2 | Requisitos, estrutura de dados, fluxogramas, pseudocódigos e repositório | Concluída |
| 3 | Incremento 1 — menu, registro, vetor, cadastro e listagem | Concluída |
| 4 | Incremento 2 — pesquisa, atualização de situação e participantes | Concluída |
| 5 | Incremento 3 — resumo geral, validações e testes | Concluída |
| 6 | Documentação final, revisão dos artefatos e preparação da apresentação | Em andamento |

O detalhamento de cada sprint e a matriz de riscos estão em [`docs/entrega1/sprints.md`](docs/entrega1/sprints.md).

## Fluxo de trabalho no Git

- **Planejado na 1ª etapa:** uma branch por funcionalidade, com integração na branch principal após revisão de outro integrante.
- **Usado na 2ª etapa:** os commits foram feitos diretamente na branch `main`, em incrementos, um ou mais commits por tarefa (Tarefa 1 a Tarefa 9), com mensagens descritivas em português.
- Antes do commit, cada incremento de código foi compilado nos dois modos (`gcc -std=gnu89 -Wall -Wextra -Wdeclaration-after-statement` e `gcc -std=c99 -Wall -Wextra -pedantic`) e conferido; a partir da Tarefa 5, os testes automatizados de `tests/` (`tests/rodar_testes.sh`) também passaram a ser executados.

## Referências principais

- BRASIL. Decreto nº 6.286, de 5 de dezembro de 2007. Institui o Programa Saúde na Escola — PSE.
- BRASIL. Portaria Interministerial MS/MEC nº 1.055, de 25 de abril de 2017.
- BRASIL. Lei nº 13.709, de 14 de agosto de 2018 (LGPD).
- SOUSA, M. C.; ESPERIDIÃO, M. A.; MEDINA, M. G. A intersetorialidade no Programa Saúde na Escola. *Ciência & Saúde Coletiva*, v. 22, n. 6, 2017.
