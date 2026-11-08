# Testes — PSE em Ação (2ª etapa)

Este documento registra os testes da versão final de `src/main.c`. Cada caso de teste tem um arquivo de entrada em `tests/casos/`, que simula tudo o que o usuário digitaria, uma linha por resposta. O programa é executado com a entrada redirecionada (`./pse < tests/casos/CTxx.txt`) e a saída completa fica gravada em `tests/saidas/CTxx.txt`, como evidência.

## Como rodar

Na raiz do repositório:

```bash
./tests/rodar_testes.sh
```

O script:

1. compila `src/main.c` com os dois comandos exigidos e para se houver erro, avisando também se houver warnings:
   - `gcc -std=gnu89 -Wall -Wextra -Wdeclaration-after-statement -o pse src/main.c`
   - `gcc -std=c99 -Wall -Wextra -pedantic -o pse src/main.c`
2. gera um segundo executável com `-DMAX_ACOES=3` (`pse_limite`), usado só no caso de limite (o nome do caso termina em `max3`);
3. executa cada caso com limite de 10 segundos (usa `timeout` se existir; senão, `perl` com `alarm`), para detectar laço infinito;
4. confere a saída com `tests/esperado/CTxx.txt`: cada linha desse arquivo é um trecho que **deve** aparecer na saída; uma linha que começa com `!` é um trecho que **não pode** aparecer. A conferência usa `grep -F`, que verifica só se o trecho está presente (não conta quantas vezes ele aparece), e o caso é marcado como PASSOU ou FALHOU. Um `\r` no fim da linha do arquivo esperado é ignorado, para o script funcionar também com arquivos em formato Windows (CRLF);
5. apaga os executáveis e mostra um resumo (casos executados, aprovados, código de saída diferente de 0 e tempo esgotado).

No Windows, o script roda pelo Git Bash, desde que o `gcc` esteja no PATH. No Dev-C++ os testes automatizados não rodam; nele o teste é manual, digitando as entradas de cada arquivo de `tests/casos/`.

**Ambiente da execução registrada:** macOS (Darwin 27.0.0, arm64), `gcc` = Apple clang version 21.0.0 (clang-2100.3.34.2). Data: 2026-11-07. Os casos foram executados com o executável compilado em modo `c99` (o último dos dois comandos); a compilação em `gnu89` também foi feita, sem erros nem warnings.

**Resultado geral:** 27 casos executados, 27 aprovados, nenhum com código de saída diferente de 0, nenhum com tempo esgotado e zero warnings nos dois modos de compilação.

## Tabela de casos de teste

As mensagens em "Resultado obtido" foram copiadas das saídas reais em `tests/saidas/`. Na coluna Entrada, a barra (`/`) separa as linhas digitadas e "cadastro completo" significa a sequência de dados válidos de uma ação (escola, tema, data, público-alvo, responsável e quantidade prevista).

| ID | Requisito | Objetivo | Entrada | Resultado esperado | Resultado obtido | Status |
|---|---|---|---|---|---|---|
| CT01 | RF07, RNF05 | Recusar opção inexistente no menu (número e letra) | `CT01_menu_opcao_inexistente.txt`: 9 / 0 / -1 / abc / (linha vazia) / 7 | Mensagem de opção inválida para cada entrada e o menu reaparece | 5 vezes "Opcao invalida. Digite um numero de 1 a 7."; depois "Programa encerrado. Ate logo!" | PASSOU |
| CT02 | RF01, RF02 | Cadastrar uma ação válida | `CT02_cadastro_valido.txt`: 1 / A01 / Escola Municipal Sol Nascente / Alimentacao saudavel / 15/03/2026 / 5o ano - turmas A e B / Enf. Carla Mendes / 40 / 2 / 7 | Confirmação do cadastro com situação Planejada e qtd. efetiva 0 | "Acao cadastrada com sucesso! Codigo: A01 - Situacao: Planejada" e "Acoes cadastradas: 1 de 100."; na listagem, "Situacao...........: Planejada", "Qtd. prevista......: 40", "Qtd. efetiva.......: 0" | PASSOU |
| CT03 | RF07 | Recusar código vazio (linha vazia e só espaços) | `CT03_codigo_vazio.txt`: 1 / (vazio) / (3 espaços) / A01 / cadastro completo / 7 | Aviso de campo obrigatório e nova pergunta do código | 2 vezes "Campo obrigatorio. Preencha este campo."; depois "Acao cadastrada com sucesso! Codigo: A01 - Situacao: Planejada" | PASSOU |
| CT04 | RF07 | Recusar código repetido, inclusive com maiúsculas/minúsculas diferentes | `CT04_codigo_duplicado.txt`: cadastro A01 / 1 / A01 / a01 / A02 / cadastro completo / 2 / 7 | "A01" e "a01" recusados; A02 aceito | 2 vezes "Codigo ja cadastrado. Digite outro."; "Acao cadastrada com sucesso! Codigo: A02 - Situacao: Planejada"; "Total de acoes listadas: 2" | PASSOU |
| CT05 | RF07 | Recusar campos obrigatórios vazios (escola, tema, público-alvo só com espaços, responsável) | `CT05_campo_obrigatorio_vazio.txt`: 1 / A01 / (vazio) / escola / (vazio) / tema / 15/03/2026 / (espaços) / público / (vazio) / responsável / 40 / 7 | Aviso em cada campo vazio e nova pergunta do mesmo campo | 4 vezes "Campo obrigatorio. Preencha este campo." (após Escola, Tema, Publico-alvo e Responsavel); depois "Acao cadastrada com sucesso! Codigo: A01 - Situacao: Planejada" | PASSOU |
| CT06 | RF07 | Recusar data inválida e aceitar data com espaços nas pontas | `CT06_data_invalida.txt`: 1 / A01 / escola / tema / 32/01/2026 / 15/13/2026 / 00/03/2026 / 15/03/1999 / 15/03/2101 / 2026-03-15 / 1/3/2026 / ab/cd/efgh / 15/03/20260 / (vazio) / " 15/03/2026 " / ... / 2 / 7 | Cada data inválida recusada; " 15/03/2026 " aceita como 15/03/2026 | 8 vezes "Data invalida. Use o formato DD/MM/AAAA (ex.: 15/03/2026)."; para 15/03/20260, "Texto muito longo. Use no maximo 10 caracteres."; para a linha vazia, "Campo obrigatorio. Preencha este campo."; cadastro concluído e listagem com "Data prevista......: 15/03/2026" | PASSOU |
| CT07 | RF07 | Recusar quantidade prevista zero, negativa, com letras, vazia e acima do máximo | `CT07_qtd_prevista_invalida.txt`: cadastro até a quantidade / 0 / -5 / abc / 12abc / (vazio) / 100001 / 99999999999 / 40 / 2 / 7 | Zero, negativa e acima do máximo: mensagem de faixa; letras e vazio: mensagem de valor inválido; 40 aceito | 4 vezes "Quantidade invalida. Digite um valor maior que zero (maximo 100000)." (0, -5, 100001, 99999999999); 3 vezes "Valor invalido. Digite apenas numeros inteiros." (abc, 12abc, vazio); listagem com "Qtd. prevista......: 40" | PASSOU |
| CT08 | RF07, RNF05 | Recusar texto maior que o campo e aceitar texto no tamanho máximo | `CT08_texto_maior_que_campo.txt`: 1 / A0123456789 (11 caract.) / ACAO000001 (10 caract.) / escola com 72 caracteres / Escola Municipal Sol Nascente / ... / 2 / 7 | Aviso de texto longo com o tamanho máximo do campo; código de 10 caracteres aceito | "Texto muito longo. Use no maximo 10 caracteres." (código) e "Texto muito longo. Use no maximo 60 caracteres." (escola); "Acao cadastrada com sucesso! Codigo: ACAO000001 - Situacao: Planejada" | PASSOU |
| CT09 | RF07, RNF06 | Avisar limite de armazenamento antes de pedir dados (executável com `-DMAX_ACOES=3`) | `CT09_limite_atingido_max3.txt`: 3 cadastros completos (A01, A02, A03) / 1 / 2 / 7 | Na 4ª tentativa, aviso de limite sem pedir o código; programa continua | "Acoes cadastradas: 3 de 3."; na opção 1 seguinte, "Limite de registros atingido (3 acoes). Nao e possivel cadastrar." e volta ao menu; listagem com "Total de acoes listadas: 3" | PASSOU |
| CT10 | RF02 | Listar sem ações cadastradas | `CT10_listar_sem_acoes.txt`: 2 / 7 | Mensagem de lista vazia | "--- Lista de acoes ---" seguido de "Nenhuma acao cadastrada." | PASSOU |
| CT11 | RF02 | Listar com ações cadastradas | `CT11_listar_com_acoes.txt`: cadastros A01 e A02 / 2 / 7 | Todas as ações exibidas com todos os campos e a situação em texto | Blocos com "Codigo.............: A01" e "Codigo.............: A02", "Situacao...........: Planejada"; "Total de acoes listadas: 2" | PASSOU |
| CT12 | RF03 | Pesquisa por código encontrada (sem diferenciar maiúsculas) e não encontrada | `CT12_pesquisa_por_codigo.txt`: cadastros A01 e A02 / 3 / 1 / a01 / 3 / 1 / Z99 / 7 | "a01" encontra A01; "Z99" não encontra | Exibe a ação A01 e "Registros encontrados: 1"; para Z99, "Nenhum registro encontrado." | PASSOU |
| CT13 | RF03 | Pesquisa parcial por escola | `CT13_pesquisa_parcial_escola.txt`: cadastros em Escola Municipal Sol Nascente, Escola Estadual Monte Verde e Escola Municipal Rio Azul / 3 / 2 / municipal / 3 / 2 / xyz / 7 | "municipal" encontra as 2 escolas municipais; "xyz" não encontra | Exibe "Escola Municipal Sol Nascente" e "Escola Municipal Rio Azul" e "Registros encontrados: 2"; para xyz, "Nenhum registro encontrado." | PASSOU |
| CT14 | RF03 | Pesquisa parcial por tema | `CT14_pesquisa_parcial_tema.txt`: mesmos 3 cadastros (temas Alimentacao saudavel, Saude mental, Verificacao vacinal) / 3 / 3 / VACINAL / 3 / 3 / saud / 7 | "VACINAL" encontra 1; "saud" encontra 2 | "Tema...............: Verificacao vacinal" e "Registros encontrados: 1"; depois "Alimentacao saudavel", "Saude mental" e "Registros encontrados: 2" | PASSOU |
| CT15 | RF03, RF07 | Recusar critério de pesquisa inválido e pesquisar sem ações | `CT15_pesquisa_criterio_invalido.txt`: 3 (sem ações) / cadastro A01 / 3 / 4 / 3 / x / 3 / 0 / 7 | Sem ações: aviso; critérios 4, x e 0 recusados, volta ao menu | "Nenhuma acao cadastrada."; 3 vezes "Opcao invalida. Escolha 1, 2 ou 3. Voltando ao menu." | PASSOU |
| CT16 | RF04, RF05, RF07 | Atualizar e registrar participantes com código inexistente | `CT16_atualizar_codigo_inexistente.txt`: 4 (sem ações) / cadastro A01 / 4 / Z99 / 5 / Z99 / 7 | Aviso de registro inexistente nas opções 4 e 5 | "Nenhuma acao cadastrada."; nas opções 4 e 5, "Acao nao encontrada. Confira o codigo na listagem (opcao 2)." | PASSOU |
| CT17 | RF04, RF07 | Recusar situação inválida sem alterar a ação | `CT17_situacao_invalida.txt`: cadastro A01 / 4 / A01 / 5 / 4 / A01 / abc / 4 / A01 / 0 / 2 / 7 | Situações 5, abc e 0 recusadas; ação continua Planejada | 3 vezes "Situacao invalida. Escolha 1, 2 ou 3. Nenhuma alteracao foi feita."; listagem com "Situacao...........: Planejada"; a mensagem "Situacao atualizada com sucesso" não aparece | PASSOU |
| CT18 | RF04, RF05, RF07 | Marcar como realizada com participantes e recusar quantidade negativa | `CT18_marcar_realizada.txt`: cadastro A01 (previstos 40) / 4 / a01 / 2 / -10 / abc / 35 / 2 / 7 | -10 e abc recusados; 35 aceito; ação Realizada | "Quantidade invalida. Digite zero ou mais (maximo 100000)." (-10); "Valor invalido. Digite apenas numeros inteiros." (abc); "Situacao atualizada com sucesso! Acao A01 agora esta: Realizada." e "Participantes efetivos registrados: 35 (previstos: 40)."; listagem com "Qtd. efetiva.......: 35" | PASSOU |
| CT19 | RF04, RF05 | Cancelar uma ação realizada e zerar a quantidade efetiva | `CT19_cancelar_zera_efetiva.txt`: cadastro A01 / 4 / A01 / 2 / 35 / 4 / A01 / 3 / 2 / 7 | Ação passa de Realizada (35) para Cancelada com qtd. efetiva 0 | "Acao A01 - situacao atual: Realizada"; "Situacao atualizada com sucesso! Acao A01 agora esta: Cancelada." e "Quantidade efetiva de participantes: 0."; listagem com "Situacao...........: Cancelada" e "Qtd. efetiva.......: 0" | PASSOU |
| CT20 | RF05 | Opção 5 com ação planejada (recusa) | `CT20_opcao5_planejada_recusa.txt`: cadastro A01 / 5 / A01 / 7 | Recusa e orientação para usar a opção 4 | "A acao A01 esta Planejada. So e possivel registrar participantes de acoes realizadas." e "Primeiro marque a acao como Realizada na opcao 4 (Atualizar situacao)." | PASSOU |
| CT21 | RF05, RF07 | Opção 5 com ação realizada (corrige a quantidade) | `CT21_opcao5_realizada_corrige.txt`: cadastro A01 / 4 / A01 / 2 / 35 / 5 / a01 / -1 / 38 / 2 / 7 | Mostra valor atual; -1 recusado; 38 grava | "Acao A01 - participantes efetivos atuais: 35 (previstos: 40)."; "Quantidade invalida. Digite zero ou mais (maximo 100000)." (-1); "Participantes registrados com sucesso! Acao A01: 38 participantes efetivos."; listagem com "Qtd. efetiva.......: 38" | PASSOU |
| CT22 | RF06 | Resumo sem ações | `CT22_resumo_sem_acoes.txt`: 6 / 7 | Aviso de resumo indisponível | "Nenhuma acao cadastrada. Resumo indisponivel." | PASSOU |
| CT23 | RF06 | Resumo com dados e percentual conferido à mão | `CT23_resumo_com_dados.txt`: A01 (prev. 40), A02 (prev. 50), A03 (prev. 30), A04 (prev. 25) / A01 Realizada 35 / A02 Realizada 42 / A03 Cancelada / 6 / 7 | 4 cadastradas, 1 planejada, 2 realizadas, 1 cancelada; previstos 90, efetivos 77; 85,6% (cálculo abaixo) | "Total de acoes cadastradas......: 4", "Acoes planejadas................: 1", "Acoes realizadas................: 2", "Acoes canceladas................: 1", "Participantes previstos.........: 90", "Participantes efetivos..........: 77", "Percentual de participacao......: 85.6%" | PASSOU |
| CT24 | RF06 | Resumo sem nenhuma ação realizada (sem divisão por zero) | `CT24_resumo_sem_realizada.txt`: A01 (planejada) e A02 (cancelada) / 6 / 7 | Contagens corretas e percentual não calculado, sem `nan`/`inf` | "Acoes planejadas................: 1", "Acoes realizadas................: 0", "Acoes canceladas................: 1", "Participantes previstos.........: 0", "Percentual de participacao......: nao calculado." e "Nenhuma acao realizada ainda; o percentual so considera acoes realizadas." | PASSOU |
| CT25 | RNF05 | Fim da entrada (EOF) sem laço infinito | `CT25_fim_da_entrada_eof.txt`: 1 / A01 / Escola Municipal Sol Nascente (sem quebra de linha final e sem a opção 7) | Programa termina sozinho, com mensagem, código de saída 0 e dentro do tempo | Após a pergunta "Tema: ", "Fim da entrada de dados. Programa encerrado."; código de saída 0, sem tempo esgotado; nenhuma ação cadastrada | PASSOU |
| CT26 | RF08 | Encerrar pela opção 7 | `CT26_encerrar_opcao7.txt`: 7 | Mensagem de encerramento | Menu "===== PSE EM ACAO =====" exibido e "Programa encerrado. Ate logo!"; código de saída 0 | PASSOU |
| CT27 | RF01, RNF02, RNF05 | Regressão: entrada com quebra de linha do Windows (CRLF) e espaços nas pontas | `CT27_entrada_crlf_e_espacos.txt`: cadastro completo com todas as linhas terminando em `\r\n`, código "  A01  ", data " 15/03/2026 " e quantidade " 40 " / 2 / 7 | Valores aceitos sem espaços e sem `\r`; nenhuma recusa | "Acao cadastrada com sucesso! Codigo: A01 - Situacao: Planejada"; listagem com "Data prevista......: 15/03/2026" e "Qtd. prevista......: 40"; não aparecem "Texto muito longo" nem "Data invalida" | PASSOU |

### Cálculo à mão do CT23

O percentual considera apenas as ações realizadas (A01 e A02). A03 (cancelada) e A04 (planejada) entram só na contagem por situação.

- Participantes previstos das realizadas: 40 (A01) + 50 (A02) = **90**
- Participantes efetivos das realizadas: 35 (A01) + 42 (A02) = **77**
- Percentual: 77 ÷ 90 × 100 = 85,555...% → arredondado para 1 casa decimal = **85,6%**

O programa mostrou `Percentual de participacao......: 85.6%`, igual ao cálculo à mão (o C usa ponto como separador decimal).

## Saída da execução final do script

```
Compilando (gnu89): gcc -std=gnu89 -Wall -Wextra -Wdeclaration-after-statement -o pse src/main.c
  OK, zero erros e zero warnings.
Compilando (c99): gcc -std=c99 -Wall -Wextra -pedantic -o pse src/main.c
  OK, zero erros e zero warnings.
Compilando (limite MAX_ACOES=3): gcc -std=c99 -Wall -Wextra -pedantic -DMAX_ACOES=3 -o pse_limite src/main.c
  OK, zero erros e zero warnings.

PASSOU  CT01_menu_opcao_inexistente  (codigo de saida 0)
...
PASSOU  CT27_entrada_crlf_e_espacos  (codigo de saida 0)

===== Resumo =====
Casos executados..........: 27
Passaram..................: 27
Falharam..................: 0
Codigo de saida != 0......: nenhum
Tempo esgotado............: nenhum
Compilacao................: zero warnings nos dois modos
Saidas gravadas em tests/saidas/
```

Também foi conferido que a verificação funciona de verdade: ao acrescentar de propósito um trecho inexistente em um arquivo de `tests/esperado/`, o script marcou o caso como FALHOU; e a proteção de tempo com `perl` interrompeu um laço infinito de teste em 2 segundos, com código 142, que o script reconhece como tempo esgotado. Os arquivos foram restaurados antes da execução final.

## Defeitos encontrados e correções

Nesta tarefa nenhum teste falhou, então não foi preciso corrigir defeitos no programa. Antes de criar os testes, foram feitos dois ajustes pedidos pela revisão anterior (não eram defeitos de funcionamento):

- o comentário de cabeçalho do `main.c` passou de "Incremento 3" para "Etapa 2 - versao final";
- as mensagens de quantidade fora da faixa deixaram de ter "100000" escrito no texto: agora `ler_inteiro` mostra o máximo com `printf("%s (maximo %d).\n", msg_faixa, maximo)`, e o máximo vem da constante `MAX_PARTICIPANTES`. O texto exibido continua o mesmo (CT07, CT18 e CT21 confirmam).

Defeito encontrado e corrigido durante o desenvolvimento (Tarefas 2/3):

| Defeito | Causa | Correção | Caso que protege contra a volta do defeito |
|---|---|---|---|
| A data " 15/03/2026 " (com espaços) e as linhas com final de linha do Windows (CRLF, `\r\n`) eram recusadas com "Texto muito longo". | A leitura conferia o tamanho do texto **antes** de remover os espaços e o `\r` das pontas; assim, uma data de 10 caracteres com espaços ou `\r` passava do tamanho do campo. | Em `ler_linha`, a linha passou a ser lida primeiro em um texto local de 256 caracteres (`TAM_LINHA`); as pontas são limpas e só depois o tamanho é comparado com o do campo. | CT06 (data com espaços) e CT27 (entrada CRLF) |

## Limitações conhecidas

- **Validação simples de data:** o programa confere o formato DD/MM/AAAA, dia de 1 a 31, mês de 1 a 12 e ano de 2000 a 2100, mas não confere quantos dias cada mês tem. Por isso uma data como 31/02/2026 é aceita.
- **Linhas muito longas:** a linha digitada é lida em um texto de até 255 caracteres. Uma linha maior é recusada como "Texto muito longo" (ou "Valor invalido" nos números), e o resto dela é descartado, mas o tamanho real acima de 255 caracteres não é medido.
- **Testes automatizados fora do Dev-C++:** o script `tests/rodar_testes.sh` precisa de um terminal bash (macOS, Linux ou Git Bash no Windows). No Dev-C++ o teste é manual: compilar, executar e digitar as entradas de cada arquivo de `tests/casos/`, comparando com `tests/saidas/`.
