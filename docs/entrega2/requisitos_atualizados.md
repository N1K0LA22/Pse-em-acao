# Requisitos atualizados — PSE em Ação (2ª etapa)

Este documento apresenta a versão final dos requisitos funcionais (RF) e não funcionais (RNF) depois da implementação em `src/main.c`. A base são os requisitos da 1ª entrega (`docs/entrega1/requisitos.md` e seções 5.3 e 5.4 do documento `docs/entrega1/AEP_Entrega1_PSE_em_Acao.pdf`). Os IDs foram mantidos. Os documentos da 1ª entrega **não tiveram o conteúdo alterado** (apenas a limpeza da Tarefa 1: remoção de uma linha solta de upload no topo de `requisitos.md` e `sprints.md` e mudança dos PNG para `docs/fluxogramas/`); as mudanças de requisitos ficam registradas apenas aqui.

## Como ler este documento

Cada requisito traz o texto final e um marcador de status:

| Status | Significado |
|---|---|
| **Sem alteração** | O texto da 1ª entrega continua valendo do jeito que estava. |
| **Detalhado** | O objetivo é o mesmo, mas o requisito ganhou regras mais precisas (limites, mensagens, formato) que a 1ª entrega não definia. |
| **Alterado** | O comportamento mudou em relação ao que a 1ª entrega descrevia (no requisito, no pseudocódigo ou no fluxograma). |
| **Novo** | Requisito que não existia na 1ª entrega. |

Quando o requisito muda, aparecem também os itens **O que mudou** e **Motivo**.

Referências usadas nas tabelas:

- **Pseudocódigos** (seção 6.4 do PDF): 1 – Programa principal e menu (RF07, RF08); 2 – Cadastrar ação (RF01, RF07); 3 – Pesquisar ação por código, escola ou tema (RF03); 4 – Atualizar situação e registrar participantes (RF04, RF05); 5 – Gerar resumo geral (RF06).
- **Fluxogramas** (seções 6.2 e 6.3 do PDF): Figura 1 – fluxograma geral (`docs/fluxogramas/fluxograma_geral.png`); Figura 2 – cadastro, parte 1: limite de registros e validação do código (`docs/fluxogramas/fluxograma_cadastro_parte1.png`); Figura 3 – cadastro, parte 2: demais campos, quantidade prevista e gravação (`docs/fluxogramas/fluxograma_cadastro_parte2.png`).
- **Quadro 6** (seção 6.1 do PDF): estrutura de dados `AcaoPSE`.
- **Casos de teste**: `tests/casos/CTxx_*.txt`, descritos em `docs/entrega2/testes.md`.

## Resumo das mudanças

| ID | Requisito | Status |
|---|---|---|
| RF01 | Cadastrar ação do PSE | Detalhado |
| RF02 | Listar ações cadastradas | Detalhado |
| RF03 | Pesquisar ações | Alterado |
| RF04 | Atualizar situação da ação | Detalhado |
| RF05 | Registrar participantes efetivos | Alterado |
| RF06 | Gerar resumo geral | Alterado |
| RF07 | Validar entradas | Alterado |
| RF08 | Encerrar a aplicação | Detalhado |
| RNF01 | Usabilidade | Sem alteração |
| RNF02 | Portabilidade | Detalhado |
| RNF03 | Manutenibilidade | Sem alteração |
| RNF04 | Privacidade e conformidade | Sem alteração |
| RNF05 | Confiabilidade | Detalhado |
| RNF06 | Capacidade e desempenho | Detalhado |
| RNF07 | Leitura segura dos dados digitados | Novo |

Contagem: **RF** — 0 sem alteração, 4 detalhados, 4 alterados; **RNF** — 3 sem alteração, 3 detalhados, 1 novo.

## Requisitos funcionais

### RF01 — Cadastrar ação do PSE

**Status:** Detalhado

**Texto final:** Permitir o cadastro de uma ação informando código, escola, tema, data prevista, público-alvo, responsável e quantidade prevista de participantes, atribuindo automaticamente a situação inicial "Planejada" e a quantidade efetiva 0. Todos os textos são obrigatórios e têm tamanho máximo: código até 10 caracteres, escola até 60, tema até 40, data exatamente no formato DD/MM/AAAA (10 caracteres), público-alvo até 40 e responsável até 50. A quantidade prevista deve ser um número inteiro de 1 a 100000. Ao final, o sistema confirma o cadastro mostrando o código, a situação e quantas ações já estão cadastradas em relação ao limite (ex.: "Acoes cadastradas: 1 de 100.").

**O que mudou:**
- Os tamanhos máximos de cada campo (que já existiam no Quadro 6 como `char[11]`, `char[61]` etc.) passaram a ser verificados na digitação, com aviso ao usuário.
- A data prevista, que no Quadro 6 era "tratada como texto nesta versão", continua guardada como texto, mas agora tem o formato validado (ver RF07).
- A quantidade prevista ganhou um teto (`MAX_PARTICIPANTES` = 100000).
- A mensagem de confirmação ficou mais completa.

**Motivo:** os campos da struct têm tamanho fixo, então um texto maior não cabe; validar a data evita registros como "amanhã" ou "15-03"; o teto evita números absurdos ou que estourem o tipo `int`; o contador "X de Y" ajuda o usuário a perceber que existe um limite (RNF06).

A situação inicial continua sempre "Planejada", exatamente como no RF01, no Quadro 6 ("inicia sempre em 1"), no pseudocódigo 2 e na Figura 3. O enunciado fala em "situação inicial"; a equipe manteve a interpretação da 1ª entrega de que ela é atribuída automaticamente, e não digitada.

### RF02 — Listar ações cadastradas

**Status:** Detalhado

**Texto final:** Exibir todas as ações registradas em formato organizado, uma por bloco, com código, escola, tema, data prevista, público-alvo, responsável, situação (em texto: Planejada, Realizada ou Cancelada) e quantidades prevista e efetiva. Ao final, mostrar o total de ações listadas. Se não houver ações, exibir "Nenhuma acao cadastrada.".

**O que mudou:** a listagem mostra também o código e o público-alvo (o RF02 da 1ª entrega citava escola, tema, data, responsável, situação e quantidades), mostra a situação em texto, informa o total listado e trata a lista vazia.

**Motivo:** o código é necessário para usar as opções 4 e 5, que localizam a ação por ele; a situação em texto é mais clara que o número 1, 2 ou 3 (RNF01); a 1ª entrega não tinha pseudocódigo para a listagem, então o caso "nenhuma ação" foi definido agora, no mesmo padrão dos pseudocódigos 3 e 5.

### RF03 — Pesquisar ações

**Status:** Alterado

**Texto final:** Permitir a busca de ações por código, por escola ou por tema. Por código, a ação precisa ter o código igual ao digitado, sem diferenciar maiúsculas e minúsculas ("a01" encontra "A01"). Por escola ou por tema, basta que o texto da ação **contenha** o termo digitado, também sem diferenciar maiúsculas e minúsculas ("municipal" encontra "Escola Municipal Sol Nascente"). Exibir todos os registros encontrados e o total, ou "Nenhum registro encontrado.". Se não houver ações cadastradas, avisar antes de pedir o critério. Critério diferente de 1, 2 ou 3 é recusado e o programa volta ao menu.

**O que mudou:** o pseudocódigo 3 comparava com igualdade exata (`acoes[i].escola = termo`), o que obrigaria a digitar o nome completo da escola ou do tema, com as mesmas maiúsculas. Agora a busca por escola e tema é parcial e as três buscas não diferenciam maiúsculas e minúsculas.

**Motivo:** nomes de escolas e temas são longos; o representante da escola (Quadro 1 da 1ª entrega) precisa achar as ações digitando só parte do nome. A comparação foi feita com `tolower`, da biblioteca padrão `ctype.h`, sem funções que não existem em todos os compiladores (RNF02).

### RF04 — Atualizar situação da ação

**Status:** Detalhado

**Texto final:** Permitir alterar a situação de uma ação, localizada por código (sem diferenciar maiúsculas e minúsculas), para Planejada (1), Realizada (2) ou Cancelada (3). Antes de pedir a nova situação, mostrar a situação atual. Se a nova situação for Realizada, pedir a quantidade efetiva de participantes (ver RF05) antes de gravar a mudança. Se for Planejada ou Cancelada, zerar a quantidade efetiva. Código inexistente, situação inválida e ausência de ações cadastradas são avisados, e nenhuma alteração é feita.

**O que mudou:** a situação atual é mostrada; há aviso quando não existem ações; a quantidade efetiva é lida **antes** de gravar a nova situação (o pseudocódigo 4 gravava a situação primeiro).

**Motivo:** mostrar a situação atual evita alterações por engano; ler a quantidade antes garante que a ação nunca fique "Realizada" sem quantidade registrada. Como a leitura repete até receber um valor válido, o resultado final é o mesmo do pseudocódigo 4.

### RF05 — Registrar participantes efetivos

**Status:** Alterado

**Texto final:** Solicitar e armazenar a quantidade efetiva de participantes (número inteiro de 0 a 100000) quando a ação for marcada como Realizada na opção 4, e zerar esse valor quando a ação for marcada como Cancelada ou voltar a Planejada. A opção 5 do menu ("Registrar participantes") localiza a ação pelo código e permite registrar ou corrigir a quantidade efetiva **somente** de ações com situação Realizada; para ações Planejadas ou Canceladas, recusa e orienta o usuário a marcar a ação como Realizada na opção 4. A quantidade efetiva pode ser maior que a prevista.

**O que mudou:**
- A 1ª entrega tinha a opção 5 no menu (pseudocódigo 1 e Figura 1), mas não descrevia a rotina `registrar_participantes`; o pseudocódigo 4 fazia todo o registro dentro de `atualizar_situacao`. A regra da opção 5 foi definida agora.
- O valor também é zerado quando a ação volta para Planejada (o RF05 citava só o cancelamento; o pseudocódigo 4 já zerava nos dois casos, no `senao`).
- Foi definido o teto de 100000 e que a efetiva pode passar da prevista.

**Motivo:** a opção 5 precisava de uma função própria para cumprir o menu da 1ª entrega sem repetir a opção 4. Restringir a ações Realizadas mantém a regra do Quadro 6 ("só é preenchida quando a ação é marcada como realizada"). A leitura da quantidade é a mesma função nas opções 4 e 5 (`ler_qtd_efetiva`), para as regras serem iguais. A efetiva pode passar da prevista porque mais pessoas podem comparecer do que o planejado.

### RF06 — Gerar resumo geral

**Status:** Alterado

**Texto final:** Apresentar o total de ações cadastradas, a quantidade de ações planejadas, realizadas e canceladas, o total de participantes previstos e efetivos **das ações realizadas** e o percentual de participação (efetivos ÷ previstos × 100, com 1 casa decimal), deixando claro na tela que o percentual considera apenas as ações realizadas. Se nenhuma ação estiver realizada, o percentual aparece como "nao calculado", com explicação. Se não houver ações, exibir "Nenhuma acao cadastrada. Resumo indisponivel.".

**O que mudou:**
- O pseudocódigo 5 fazia `percentual <- 0` quando não havia ação realizada e mostrava "0%"; agora aparece "nao calculado".
- O resumo mostra também o total de ações cadastradas e um título "Participacao (somente acoes realizadas)".
- O percentual é calculado em ponto flutuante e mostrado com 1 casa decimal.

**Motivo:** "0%" daria a impressão de que ninguém participou, quando na verdade nada foi realizado ainda. O fato de o percentual considerar só as ações realizadas já estava no texto abaixo do pseudocódigo 5, mas não no RF06 nem na tela; agora está nos dois. A divisão em ponto flutuante evita que a conta `total_efetivo / total_previsto` com inteiros dê 0 (em C, 77 / 90 com inteiros vale 0).

### RF07 — Validar entradas

**Status:** Alterado

**Texto final:** O sistema deve recusar, com mensagem explicativa e sem encerrar o programa:

1. opção inexistente no menu principal (número fora de 1 a 7, letras ou linha vazia) e nos submenus de critério de pesquisa e de situação;
2. código vazio ou repetido, comparando sem diferenciar maiúsculas e minúsculas ("a01" é igual a "A01");
3. campos obrigatórios vazios ou só com espaços (escola, tema, data, público-alvo, responsável e termo de busca);
4. texto maior que o tamanho do campo, informando o máximo aceito;
5. data fora do formato DD/MM/AAAA, com dia fora de 1–31, mês fora de 1–12 ou ano fora de 2000–2100;
6. quantidade prevista menor ou igual a zero e quantidade efetiva negativa, além de valores acima de 100000;
7. texto com letras onde se espera número (ex.: "abc", "12abc");
8. código inexistente nas opções 4 e 5 (registro inexistente);
9. cadastro com o limite de armazenamento atingido, avisado **antes** de pedir qualquer dado.

No cadastro, só o campo inválido é perguntado de novo. Nos submenus (critério e situação), a opção inválida faz o programa voltar ao menu principal, como nos pseudocódigos 3 e 4.

**O que mudou:** foram acrescentados os itens 4, 5, 7 e parte do 6 (teto); o item 2 passou a ignorar maiúsculas e minúsculas; os itens 8 e 9 estavam nos pseudocódigos/RNF06, mas não no texto do RF07. Ver também a seção de divergências sobre a repetição só do campo inválido.

**Motivo:** o enunciado pede validar códigos repetidos, quantidades negativas, campos vazios e opções inexistentes, e tratar registro inexistente e limite atingido; os itens novos surgiram na implementação, porque sem eles o programa aceitaria dados que não cabem nos campos, datas sem sentido ou números digitados com letras. Comparar o código sem diferenciar maiúsculas evita ter "A01" e "a01" como ações diferentes, o que confundiria a pesquisa.

### RF08 — Encerrar a aplicação

**Status:** Detalhado

**Texto final:** Permitir o encerramento do programa por opção do menu principal (opção 7), exibindo mensagem de confirmação ("Programa encerrado. Ate logo!").

**O que mudou:** o texto passou a indicar o número da opção (7) e a mensagem exata exibida. O comportamento é o mesmo da 1ª entrega.

**Motivo:** o RF08 da 1ª entrega não dizia qual era a opção nem a mensagem; o pseudocódigo 1 usava "Programa encerrado." e a Figura 1 ia direto para FIM, sem mensagem (ver D16). O texto final registra o que foi implementado.

## Requisitos não funcionais

### RNF01 — Usabilidade

**Status:** Sem alteração

**Texto final:** O menu deve ser numérico e autoexplicativo, e toda operação deve produzir mensagem clara de confirmação ou de erro, sem uso de termos técnicos desnecessários, pois os usuários não são da área de tecnologia.

**Como foi atendido:** menu numerado de 1 a 7; mensagens de erro dizem o que fazer (ex.: "Use o formato DD/MM/AAAA (ex.: 15/03/2026).", "Confira o codigo na listagem (opcao 2).", "Primeiro marque a acao como Realizada na opcao 4"). As mensagens não têm acentos para não aparecerem quebradas no console do Windows.

### RNF02 — Portabilidade

**Status:** Detalhado

**Texto final:** A aplicação deve ser escrita em linguagem C padrão, em um único arquivo `src/main.c`, usando apenas `stdio.h`, `stdlib.h`, `string.h` e `ctype.h`, e compilar sem erros e sem warnings com GCC em Windows (inclusive no Dev-C++, que pode usar um GCC antigo), Linux e macOS. Não pode usar recursos específicos de sistema operacional (`conio.h`, `system("cls")`, `system("pause")`, `getch`, `fflush(stdin)`, `strcasecmp`, `stricmp`, cores ANSI). O código-fonte não tem acentos.

**O que mudou:** foram fixadas as bibliotecas permitidas, a lista do que não usar, o macOS como ambiente de desenvolvimento e os dois comandos de verificação: `gcc -std=gnu89 -Wall -Wextra -Wdeclaration-after-statement` e `gcc -std=c99 -Wall -Wextra -pedantic`.

**Motivo:** o programa é corrigido no Dev-C++ no Windows e desenvolvido em um Mac. Compilar nos dois modos garante compatibilidade com o C antigo (variáveis declaradas no início dos blocos, sem `bool`). A leitura também aceita linhas com final do Windows (`\r\n`), verificado no CT27.

### RNF03 — Manutenibilidade

**Status:** Sem alteração

**Texto final:** A solução deve ser modularizada em funções com responsabilidade única e nomes significativos, mantendo a função principal restrita ao controle do menu.

**Como foi atendido:** a `main` só exibe o menu, lê a opção e chama a função correspondente em um `switch`. Há 22 funções além da `main`, com protótipos no topo do arquivo. As constantes (`MAX_ACOES`, `TAM_CODIGO` etc.) ficam em `#define`. O vetor `acoes` e o contador `total_acoes` são globais, para as funções do menu não precisarem de parâmetros, como nos pseudocódigos (que chamam `cadastrar_acao()`, `listar_acoes()` etc. sem parâmetros).

### RNF04 — Privacidade e conformidade

**Status:** Sem alteração

**Texto final:** O sistema deve operar apenas com dados fictícios e informações coletivas, não permitindo o armazenamento de nome, matrícula, diagnóstico, prontuário ou qualquer dado individual sensível de estudantes.

**Como foi atendido:** a struct `AcaoPSE` não tem nenhum campo para dados de estudantes; o público-alvo é descrito de forma coletiva (a pergunta sugere "ex.: 5o ano - turmas A e B"); todos os dados dos testes são fictícios. Ver a limitação sobre campos de texto livre na seção seguinte.

### RNF05 — Confiabilidade

**Status:** Detalhado

**Texto final:** Nenhuma entrada inválida deve encerrar o programa de forma abrupta: o sistema deve tratar o erro, informar o usuário e retornar ao fluxo normal de operação. Se a entrada de dados acabar (fim de arquivo, Ctrl+D no Linux/macOS ou Ctrl+Z no Windows), o programa deve encerrar de forma limpa, com a mensagem "Fim da entrada de dados. Programa encerrado." e código de saída 0, sem entrar em laço infinito.

**O que mudou:** acrescentado o tratamento do fim da entrada.

**Motivo:** quando a entrada acaba, não há mais o que ler; continuar perguntando faria o programa repetir a mesma pergunta para sempre. Esse é o único caso em que o programa termina sem a opção 7, e ele termina de forma controlada, com mensagem. Isso também permite rodar os testes automáticos por redirecionamento (`./pse < caso.txt`).

### RNF06 — Capacidade e desempenho

**Status:** Detalhado

**Texto final:** O armazenamento ocorre em memória, em vetor com limite de 100 ações por execução (`MAX_ACOES`); as buscas são sequenciais e devem responder de forma imediata nesse volume. Quando o limite for atingido, o usuário é informado ao escolher a opção 1, **antes** de digitar qualquer dado. O limite pode ser trocado na compilação (`-DMAX_ACOES=3`) para testar essa situação.

**O que mudou:** definido o momento do aviso (antes de pedir os dados, como já mostravam o pseudocódigo 2 e a Figura 2) e a forma de testar o limite sem cadastrar 100 ações.

**Motivo:** avisar antes evita que o usuário digite todos os campos para só então descobrir que não há espaço; o `#ifndef MAX_ACOES` permite que o CT09 teste o limite com 3 ações.

### RNF07 — Leitura segura dos dados digitados

**Status:** Novo

**Texto final:** Toda leitura do teclado deve ser feita linha a linha com `fgets`, por funções auxiliares reutilizáveis (`ler_linha`, `ler_texto`, `ler_inteiro`, `ler_data`, `ler_opcao_menu`), nunca com `scanf` direto. Essas funções devem: remover a quebra de linha (`\n` e `\r`) e os espaços das pontas; descartar o resto de uma linha longa demais, avisando o usuário; recusar números com letras, vazios ou fora da faixa, perguntando de novo; e encerrar o programa de forma limpa quando a entrada acabar (RNF05).

**Por que foi criado:** a 1ª entrega já apontava, no Quadro 5 (riscos), a "Dificuldade com manipulação de cadeias de caracteres e leitura de dados em C" e planejava "Padronizar a leitura de textos com fgets [...] concentrar a leitura em funções auxiliares reutilizáveis". O risco virou requisito porque, sem isso, o RF07 e o RNF05 não poderiam ser cumpridos: com `scanf("%d")`, digitar uma letra deixa o texto preso na entrada e o programa repete a pergunta sem parar; com `scanf("%s")`, um texto maior que o campo escreve fora do vetor. Não é uma funcionalidade nova para o usuário, e sim a forma de garantir as validações já exigidas.

## Decisões e limitações conhecidas

| # | Decisão ou limitação | Detalhe |
|---|---|---|
| 1 | Validação simples de data | `validar_data` confere o formato DD/MM/AAAA, dia 1–31, mês 1–12 e ano 2000–2100, mas não confere quantos dias cada mês tem nem anos bissextos. Por isso 31/02/2026 e 29/02/2027 (ano não bissexto) são aceitas. |
| 2 | Teto das quantidades | `MAX_PARTICIPANTES` = 100000 para a quantidade prevista (1 a 100000) e para a efetiva (0 a 100000). Toda mensagem de faixa traz o sufixo "(maximo 100000)", inclusive para 0 e valores negativos (ex.: "Quantidade invalida. Digite um valor maior que zero (maximo 100000)." e "Quantidade invalida. Digite zero ou mais (maximo 100000)."), porque `ler_inteiro` acrescenta o máximo a qualquer valor fora da faixa. |
| 3 | Tamanho máximo de cada campo | Constantes do código, iguais ao Quadro 6 (já contando o `\0` final): `TAM_CODIGO` 11 (até 10 caracteres), `TAM_ESCOLA` 61 (até 60), `TAM_TEMA` 41 (até 40), `TAM_DATA` 11 (até 10), `TAM_PUBLICO` 41 (até 40), `TAM_RESPONSAVEL` 51 (até 50). O termo de busca usa `TAM_TERMO` = `TAM_ESCOLA` (até 60). Números são lidos em um texto de `TAM_ENTRADA_NUMERO` = 20 (até 19 caracteres). |
| 4 | Linha acima de 255 caracteres | Cada linha é lida em um texto de `TAM_LINHA` = 256 (até 255 caracteres). Uma linha maior é recusada como "Texto muito longo" (ou "Valor invalido" em campos numéricos) e o resto é descartado. No menu principal e nos submenus, uma linha com mais de 255 caracteres resulta em "Opcao invalida" (menu e critério de pesquisa) ou "Situacao invalida" (nova situação na opção 4). |
| 5 | Número com mais de 19 caracteres | Recebe "Valor invalido. Digite apenas numeros inteiros." em vez da mensagem de faixa, porque não cabe no texto de leitura de números. Números menores, mas fora da faixa (ex.: 99999999999), recebem a mensagem de faixa (CT07). |
| 6 | Sinal "+" em números | Por usar `strtol`, um número com sinal na frente, como "+7", é aceito como 7 (inclusive no menu). |
| 7 | Percentual acima de 100% | Como a quantidade efetiva pode passar da prevista, o percentual pode passar de 100% (ex.: 12 efetivos para 10 previstos mostra 120.0%). |
| 8 | Percentual sem ação realizada | Aparece "nao calculado", com a explicação "Nenhuma acao realizada ainda; o percentual so considera acoes realizadas.", em vez do 0% do pseudocódigo 5 (CT24). |
| 9 | Separador decimal | O percentual aparece com ponto (85.6%), padrão do `printf` em C. |
| 10 | Situação inicial | Sempre Planejada, atribuída automaticamente (RF01 da 1ª entrega); o usuário não escolhe a situação no cadastro. |
| 11 | Dados só em memória | Ao encerrar o programa, as ações são perdidas. A gravação em arquivo continua como melhoria futura, como na seção 5.2 da 1ª entrega. |
| 12 | Marcar como Realizada uma ação já Realizada | A opção 4 pede a quantidade efetiva de novo (coerente com o pseudocódigo 4). Para só corrigir a quantidade, o caminho indicado é a opção 5. |
| 13 | Voltar para Planejada | Uma ação Realizada que volta a Planejada tem a quantidade efetiva zerada, como no pseudocódigo 4. |
| 14 | Maiúsculas e minúsculas | O código é guardado como foi digitado, mas comparado sem diferenciar maiúsculas (cadastro, pesquisa, opções 4 e 5). A comparação usa `tolower`, que só trata letras sem acento; letras acentuadas digitadas em maiúscula e minúscula não são consideradas iguais. |
| 15 | Submenus | Critério de pesquisa ou situação inválidos voltam ao menu principal (não perguntam de novo), como nos pseudocódigos 3 e 4. |
| 16 | Campos de texto livre | O sistema não tem campos para dados de estudantes, mas não consegue impedir que alguém digite um nome em um campo livre (ex.: público-alvo). O cumprimento do RNF04 depende também do uso correto, com dados fictícios e coletivos. |
| 17 | Sem exclusão ou edição de dados | Não há opção para excluir uma ação nem para editar escola, tema, data etc., porque isso não faz parte do escopo do enunciado nem da 1ª entrega. |

## Divergências em relação aos artefatos da 1ª entrega

Diferenças entre o que foi implementado em `src/main.c` e os pseudocódigos, fluxogramas e o Quadro 6 da 1ª entrega. Os artefatos da 1ª entrega não tiveram o conteúdo alterado (apenas a limpeza da Tarefa 1: remoção de uma linha solta de upload no topo de `requisitos.md` e `sprints.md` e mudança dos PNG para `docs/fluxogramas/`).

### Estrutura de dados e organização (Quadro 6, pseudocódigo 1)

| # | Na 1ª entrega | No código | Motivo |
|---|---|---|---|
| D1 | Quadro 6: campos `codigo char[11]`, `escola char[61]`, `tema char[41]`, `data_prevista char[11]`, `publico_alvo char[41]`, `responsavel char[51]`, `qtd_prevista`, `qtd_efetiva`, `situacao` (int). | `struct AcaoPSE` com os mesmos campos, tipos e tamanhos, usando constantes `TAM_*`. | Sem divergência; registrado para conferência. |
| D2 | Pseudocódigo 1 declara `acoes` e `total_acoes` nas variáveis do algoritmo principal. | `acoes[MAX_ACOES]` e `total_acoes` são variáveis globais. | Em C, variáveis locais da `main` não são vistas pelas outras funções; deixá-las globais mantém as rotinas sem parâmetros, como nos pseudocódigos. |
| D3 | Vetor `vetor[1..MAX_ACOES]` (índices de 1 a 100); pseudocódigo 4 usa `indice <- 0` para "não encontrado". | Vetor de 0 a `MAX_ACOES - 1`; `buscar_por_codigo` devolve -1 quando não encontra. | Em C, vetores começam no índice 0, então 0 é uma posição válida. |
| D4 | `MAX_ACOES = 100` como constante fixa. | `#define MAX_ACOES 100` dentro de `#ifndef MAX_ACOES`. | Permite testar o limite compilando com `-DMAX_ACOES=3` (CT09), sem mudar o valor normal. |
| D5 | Não existiam. | Constantes novas: `MAX_PARTICIPANTES` (100000), `TAM_LINHA` (256), `TAM_ENTRADA_NUMERO` (20), `TAM_TERMO`, `SITUACAO_*`, `OPCAO_*` e `CRITERIO_*`. | Evitar números "soltos" no código e definir os limites de leitura (RNF03, RNF07). |
| D6 | Pseudocódigo 1 usa `se ... senao se` para escolher a opção. | `switch (opcao)` com `default` para opção inválida. | Mesma lógica; o `switch` é a forma usual de menu em C e fica mais legível. |

### Funções: nomes, assinaturas e funções auxiliares novas

| # | Na 1ª entrega | No código | Motivo |
|---|---|---|---|
| D7 | Rotinas `cadastrar_acao()`, `listar_acoes()`, `pesquisar_acao()`, `atualizar_situacao()`, `registrar_participantes()`, `gerar_resumo()` (pseudocódigo 1). | Mesmos nomes, todas `void` e sem parâmetros. | Sem divergência; mantidos os nomes da 1ª entrega. |
| D8 | Pseudocódigo 3 chama `exibir_acao(i)`, recebendo o índice. | `void exibir_acao(const struct AcaoPSE *acao)`, recebendo um ponteiro para a ação (chamada como `exibir_acao(&acoes[i])`). | A função mostra uma ação sem depender de onde ela está guardada; o `const` garante que ela só lê os dados. Usada na listagem e na pesquisa. |
| D9 | `registrar_participantes()` aparece no menu, mas não tem pseudocódigo. | Função implementada conforme o RF05 atualizado. | Ver RF05. |
| D10 | O laço que procura o código estava repetido dentro do cadastro (pseudocódigo 2) e da atualização (pseudocódigo 4). | `buscar_por_codigo(codigo)`, que devolve o índice ou -1, usada no cadastro, na opção 4 e na opção 5. | Evita repetir o mesmo laço três vezes (RNF03). |
| D11 | A leitura era feita com `leia` direto. | Funções auxiliares novas de leitura: `encerrar_fim_entrada`, `remover_espacos_pontas`, `ler_linha`, `ler_texto`, `texto_para_inteiro`, `ler_inteiro`, `validar_data`, `ler_data`, `ler_qtd_efetiva`, `ler_opcao_menu`. | Leitura segura (RNF07) e validações (RF07). `ler_qtd_efetiva` é usada nas opções 4 e 5. |
| D12 | Não existiam. | Funções auxiliares novas de apoio: `textos_iguais_sem_caixa`, `contem_texto_sem_caixa`, `texto_situacao`, `exibir_menu`. | Comparação sem diferenciar maiúsculas (RF03, RF07), situação em texto (RF02) e menu separado da `main`. |
| D13 | Quadro 5 (riscos): "comparação com strcmp". | As comparações de código e de pesquisa usam `textos_iguais_sem_caixa` e `contem_texto_sem_caixa` (com `tolower`), e não `strcmp`. | `strcmp` diferencia maiúsculas e minúsculas; `strcasecmp`/`stricmp` não existem em todos os compiladores (RNF02). |

### Menu e encerramento (pseudocódigo 1, Figura 1)

| # | Na 1ª entrega | No código | Motivo |
|---|---|---|---|
| D14 | Pseudocódigo 1: menu com as 7 opções, sem texto pedindo a opção. | Mesmo título e opções, mais a linha "Escolha uma opcao: ". | Deixar claro que o programa espera uma digitação (RNF01). |
| D15 | Figura 1: mensagem "Opção inválida. Informe um número de 1 a 7". Pseudocódigo 1: "Opcao invalida. Digite um numero de 1 a 7." | "Opcao invalida. Digite um numero de 1 a 7." | Seguida a mensagem do pseudocódigo 1. |
| D16 | Pseudocódigo 1 escreve "Programa encerrado." depois do laço; na Figura 1, a opção 7 vai direto para FIM, sem mensagem. | "Programa encerrado. Ate logo!" exibido no `case` da opção 7. | Atender ao RF08 (mensagem de confirmação); a Figura 1 não mostrava essa saída. |
| D17 | Não previsto. | Letras, linha vazia ou número que não cabe na leitura são tratados como opção inválida (`ler_opcao_menu` devolve -1). | RF07 e RNF07: evitar laço infinito ou valor indefinido. |

### Cadastro (pseudocódigo 2, Figuras 2 e 3)

| # | Na 1ª entrega | No código | Motivo |
|---|---|---|---|
| D18 | Figura 2 / pseudocódigo 2: "Limite de registros atingido." | "Limite de registros atingido (N acoes). Nao e possivel cadastrar." | Mensagem mais completa (RNF01); a verificação continua antes de ler o código. |
| D19 | Pseudocódigo 2: procura o código repetido antes de testar se está vazio; mensagem "Campo obrigatorio. Digite um codigo." (Figura 2: "Campo obrigatório. Informe um código"). | `ler_texto` recusa o código vazio primeiro, com "Campo obrigatorio. Preencha este campo."; depois `buscar_por_codigo` procura o repetido ("Codigo ja cadastrado. Digite outro.", igual ao pseudocódigo). | Uma única mensagem de campo obrigatório para todos os textos, vinda de `ler_texto`. A ordem não muda o resultado. |
| D20 | Pseudocódigo 2 compara `acoes[i].codigo = codigo` (igualdade exata). | Comparação sem diferenciar maiúsculas e minúsculas. | Ver RF07, item 2. |
| D21 | Figura 3: lê escola, tema, data, público-alvo e responsável juntos e, se algum estiver vazio, mostra "Preencha todos os campos obrigatórios" e volta a pedir **todos** os campos. O pseudocódigo 2 já repetia campo a campo, com a mesma mensagem. | Cada campo é lido e repetido sozinho até ser válido, com "Campo obrigatorio. Preencha este campo.". | Repetir só o campo inválido evita redigitar o que já estava certo; segue o pseudocódigo 2, que já fazia campo a campo. |
| D22 | Pseudocódigo 2: "tema, data prevista, publico-alvo e responsavel sao lidos e validados da mesma forma que a escola" (só não vazio). | A data, além de obrigatória, tem o formato validado por `ler_data`/`validar_data` ("Data invalida. Use o formato DD/MM/AAAA (ex.: 15/03/2026)."). Todos os textos têm tamanho máximo. | Ver RF01 e RF07. |
| D23 | Pseudocódigo 2 e Figura 3: quantidade prevista `> 0`, mensagem "Quantidade invalida. Digite um valor maior que zero." (Figura 3: "Informe valor maior que zero"). | Faixa de 1 a 100000; mensagem "Quantidade invalida. Digite um valor maior que zero (maximo 100000)."; letras recebem "Valor invalido. Digite apenas numeros inteiros.". | Teto `MAX_PARTICIPANTES` e recusa de letras (RF07). |
| D24 | Pseudocódigo 2 incrementa `total_acoes` e depois grava em `acoes[total_acoes]` (índices a partir de 1); Figura 3 mostra a mesma ordem. | Grava em `acoes[total_acoes]` e depois incrementa `total_acoes`. | Com índices a partir de 0, a primeira posição livre é `total_acoes` (ver D3). Resultado igual. |
| D25 | Pseudocódigo 2 / Figura 3: "Acao cadastrada com sucesso." | "Acao cadastrada com sucesso! Codigo: X - Situacao: Planejada" e "Acoes cadastradas: N de 100.". | Confirmação mais informativa (RNF01, RNF06). |
| D26 | Não previsto. | A pergunta do público-alvo traz um exemplo: "Publico-alvo (ex.: 5o ano - turmas A e B): ". | Orientar o preenchimento coletivo (RNF04), usando o exemplo do Quadro 6. |

### Listagem (Figura 1)

| # | Na 1ª entrega | No código | Motivo |
|---|---|---|---|
| D27 | Só a caixa "Listar ações cadastradas" na Figura 1; sem pseudocódigo. | `listar_acoes` com aviso "Nenhuma acao cadastrada.", um bloco por ação via `exibir_acao` e "Total de acoes listadas: N". | Ver RF02. |

### Pesquisa (pseudocódigo 3)

| # | Na 1ª entrega | No código | Motivo |
|---|---|---|---|
| D28 | Comparação exata para código, escola e tema. | Código: igualdade sem diferenciar maiúsculas; escola e tema: "contém o termo", sem diferenciar maiúsculas. | Ver RF03. |
| D29 | Menu "1 - Codigo 2 - Escola 3 - Tema"; critério inválido: "Opcao invalida." | "Pesquisar por: 1 - Codigo   2 - Escola   3 - Tema" e "Escolha o criterio: "; critério inválido: "Opcao invalida. Escolha 1, 2 ou 3. Voltando ao menu.". | Mensagens mais claras (RNF01); o comportamento (voltar ao menu) é o mesmo. |
| D30 | Três `se` separados para os critérios dentro do laço. | `if ... else if ... else` escolhendo a função de comparação. | Mesmo resultado, com menos repetição. |
| D31 | Pseudocódigo 3 lê o termo sem validar. | O termo é obrigatório (até 60 caracteres), lido por `ler_texto`. | Um termo vazio encontraria todas as ações na busca parcial. |

### Atualizar situação (pseudocódigo 4)

| # | Na 1ª entrega | No código | Motivo |
|---|---|---|---|
| D32 | Não verificava se havia ações cadastradas. | Avisa "Nenhuma acao cadastrada." antes de pedir o código (também na opção 5). | Mesmo padrão dos pseudocódigos 3 e 5 (RNF01). |
| D33 | "Acao nao encontrada." | "Acao nao encontrada. Confira o codigo na listagem (opcao 2)." | Mensagem que orienta o usuário (RNF01). |
| D34 | Mostrava direto "1 - Planejada 2 - Realizada 3 - Cancelada". | Mostra antes "Acao X - situacao atual: ..." e depois "Nova situacao: ..." e "Escolha a nova situacao: ". | Evitar alteração por engano. |
| D35 | "Situacao invalida." | "Situacao invalida. Escolha 1, 2 ou 3. Nenhuma alteracao foi feita." | Deixa claro que nada mudou (RNF01). |
| D36 | Ordem: grava `situacao <- nova_situacao` e depois lê a quantidade. | Ordem: lê a quantidade (se Realizada) e depois grava a nova situação. | A ação nunca fica Realizada sem quantidade registrada. |
| D37 | Laço próprio para ler a quantidade (`qtd >= 0`), mensagem "Quantidade invalida. Digite zero ou mais." | Usa `ler_qtd_efetiva` (faixa 0 a 100000), mensagem "Quantidade invalida. Digite zero ou mais (maximo 100000).". | Mesma função da opção 5; teto `MAX_PARTICIPANTES`. |
| D38 | "Situacao atualizada com sucesso." | "Situacao atualizada com sucesso! Acao X agora esta: ..." mais a quantidade efetiva registrada (ou 0). | Confirmação mais informativa (RNF01). |

### Resumo (pseudocódigo 5)

| # | Na 1ª entrega | No código | Motivo |
|---|---|---|---|
| D39 | `total_previsto` e `total_efetivo` do tipo inteiro. | Tipo `long`, mostrados com `%ld`. | Evitar estouro da soma se `MAX_ACOES` for aumentado (100 ações × 100000 ainda cabe em `int`, mas a margem é pequena). |
| D40 | Três `se` separados para contar as situações. | `if ... else if ... else if`. | Mesmo resultado. |
| D41 | `percentual <- (total_efetivo / total_previsto) * 100`. | `(double) total_efetivo / (double) total_previsto * 100.0`, mostrado com `%.1f`. | Em C, a divisão de inteiros descarta a parte decimal; a conversão para `double` evita isso. |
| D42 | Sem ação realizada: `percentual <- 0` e mostra "0%". | Mostra "nao calculado" com explicação. | Ver RF06. A proteção contra divisão por zero (`total_previsto > 0`) é a mesma do pseudocódigo. |
| D43 | Mostrava planejadas, realizadas, canceladas, previstos, efetivos e percentual. | Mostra também o total de ações cadastradas, o título "Participacao (somente acoes realizadas):" e a explicação "(participantes efetivos / previstos das acoes realizadas)". | Deixar claro na tela o que o percentual considera (RF06). |

### Observação sobre o nome do PDF da 1ª entrega

O `README.md` (linha da árvore de pastas), o `docs/entrega1/requisitos.md`, o `docs/entrega1/sprints.md` e a própria seção 7 do PDF citam o documento como `AEP_Entrega1.pdf`, mas o arquivo real no repositório é `docs/entrega1/AEP_Entrega1_PSE_em_Acao.pdf`. Os arquivos `.md` e o PDF da 1ª entrega não tiveram o conteúdo alterado (apenas a limpeza da Tarefa 1: remoção de uma linha solta de upload no topo de `requisitos.md` e `sprints.md` e mudança dos PNG para `docs/fluxogramas/`), para preservar o que foi avaliado; o nome correto fica registrado aqui.

## Matriz de rastreabilidade

Nomes de funções conferidos em `src/main.c`; casos de teste conferidos em `tests/casos/` e `docs/entrega2/testes.md`. Quase todos os casos terminam com a opção 7, então todos exercitam também o RF08; na tabela aparece só o caso dedicado.

### Requisitos funcionais

| RF | Função(ões) em `src/main.c` | Pseudocódigo / fluxograma da 1ª entrega | Casos de teste |
|---|---|---|---|
| RF01 – Cadastrar ação | `cadastrar_acao`, `ler_texto`, `ler_data`, `validar_data`, `ler_inteiro`, `buscar_por_codigo`, `texto_situacao` | Pseudocódigo 2; Figura 2 (cadastro parte 1); Figura 3 (cadastro parte 2); Figura 1 (caixa "Cadastrar ação do PSE") | CT02, CT03, CT04, CT05, CT06, CT07, CT08, CT09, CT27 |
| RF02 – Listar ações | `listar_acoes`, `exibir_acao`, `texto_situacao` | Figura 1 (caixa "Listar ações cadastradas"); sem pseudocódigo próprio (`exibir_acao` aparece no pseudocódigo 3) | CT02, CT10, CT11 |
| RF03 – Pesquisar ações | `pesquisar_acao`, `ler_opcao_menu`, `ler_texto`, `textos_iguais_sem_caixa`, `contem_texto_sem_caixa`, `exibir_acao` | Pseudocódigo 3; Figura 1 (caixa "Pesquisar ação (código, escola ou tema)") | CT12, CT13, CT14, CT15 |
| RF04 – Atualizar situação | `atualizar_situacao`, `ler_texto`, `buscar_por_codigo`, `ler_opcao_menu`, `ler_qtd_efetiva`, `texto_situacao` | Pseudocódigo 4; Figura 1 (caixa "Atualizar situação da ação") | CT16, CT17, CT18, CT19 |
| RF05 – Registrar participantes | `ler_qtd_efetiva`, `ler_inteiro`, `atualizar_situacao`, `registrar_participantes`, `buscar_por_codigo` | Pseudocódigo 4 (leitura da quantidade e zeragem); Figura 1 (caixa "Registrar participantes efetivos"); `registrar_participantes` sem pseudocódigo | CT16, CT18, CT19, CT20, CT21 |
| RF06 – Gerar resumo | `gerar_resumo` | Pseudocódigo 5; Figura 1 (caixa "Gerar resumo geral") | CT22, CT23, CT24 |
| RF07 – Validar entradas | `main` (`default` do `switch`), `ler_opcao_menu`, `ler_texto`, `ler_inteiro`, `texto_para_inteiro`, `ler_data`, `validar_data`, `ler_qtd_efetiva`, `buscar_por_codigo`, `textos_iguais_sem_caixa`, `cadastrar_acao` (limite), `pesquisar_acao` (critério), `atualizar_situacao` (situação e código), `registrar_participantes` (código) | Pseudocódigos 1, 2, 3 e 4; Figura 1 ("Opção inválida"); Figuras 2 e 3 (limite, código vazio/repetido, campos, quantidade) | CT01, CT03, CT04, CT05, CT06, CT07, CT08, CT09, CT15, CT16, CT17, CT18, CT21 |
| RF08 – Encerrar | `main` (`case OPCAO_ENCERRAR`) | Pseudocódigo 1; Figura 1 ("opção = 7?" → FIM) | CT26 |

### Requisitos não funcionais rastreáveis

| RNF | Onde é atendido em `src/main.c` | Referência da 1ª entrega | Casos de teste / verificação |
|---|---|---|---|
| RNF01 – Usabilidade | `exibir_menu`, `texto_situacao` e as mensagens de todas as funções | Pseudocódigo 1 (menu numérico); Quadro 3 | CT01, CT10, CT15, CT16, CT20, CT24 (mensagens de orientação) |
| RNF02 – Portabilidade | Arquivo inteiro: só `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`; `ler_linha` aceita `\r\n` | Quadro 3 | Compilação nos modos `gnu89` e `c99` em `tests/rodar_testes.sh` (zero warnings); CT27 |
| RNF03 – Manutenibilidade | `main` restrita ao menu; 22 funções com protótipos no topo; constantes `#define` | Pseudocódigo 1 (rotinas separadas); Quadro 3 | Inspeção do código (não há caso de teste automático) |
| RNF04 – Privacidade | `struct AcaoPSE` sem campos individuais; exemplo coletivo na pergunta do público-alvo em `cadastrar_acao` | Quadro 6; seção 5.5 | Dados fictícios em todos os casos de `tests/casos/` |
| RNF05 – Confiabilidade | `ler_linha`, `encerrar_fim_entrada`, `ler_texto`, `ler_inteiro`, `ler_opcao_menu` | Quadro 3; Quadro 5 ("Entrada de dados inválida provocando comportamento inesperado") | CT01, CT07, CT08, CT25, CT27 (todos com código de saída 0) |
| RNF06 – Capacidade | `MAX_ACOES` (`#ifndef`), verificação no início de `cadastrar_acao`, buscas sequenciais em `buscar_por_codigo` e `pesquisar_acao` | Pseudocódigo 2; Figura 2 ("total_acoes < MAX_ACOES?"); seção 6.1 | CT09 (executável com `-DMAX_ACOES=3`) |
| RNF07 – Leitura segura | `ler_linha`, `remover_espacos_pontas`, `ler_texto`, `texto_para_inteiro`, `ler_inteiro`, `ler_data`, `ler_opcao_menu`, `encerrar_fim_entrada` | Quadro 5 (risco "Dificuldade com manipulação de cadeias de caracteres e leitura de dados em C") | CT03, CT06, CT07, CT08, CT25, CT27 |

### Situações sem caso de teste dedicado

Comportamentos confirmados na leitura do código e em execução manual, mas que não têm um caso próprio em `tests/casos/`:

- opção 5 sem nenhuma ação cadastrada ("Nenhuma acao cadastrada."; os casos CT15 e CT16 cobrem as opções 3 e 4);
- voltar uma ação Realizada para Planejada (quantidade efetiva zerada; o CT19 cobre o cancelamento);
- marcar como Realizada uma ação que já estava Realizada (pede a quantidade de novo);
- percentual acima de 100% e datas como 31/02 e 29/02 em ano não bissexto;
- linha com mais de 255 caracteres.
