[sprints.md](https://github.com/user-attachments/files/32133719/sprints.md)

# Planejamento das sprints e riscos — PSE em Ação

Documento de apoio da 1ª etapa da AEP. O detalhamento completo está no documento escrito em `docs/entrega1/AEP_Entrega1.pdf` (seções 5.6, 5.7 e 5.8).

## Processo de desenvolvimento

A equipe adotou um processo **incremental, com entregas semanais**. Cada incremento segue o mesmo miniciclo: revisão do requisito correspondente, ajuste do algoritmo (fluxograma ou pseudocódigo), implementação, teste manual com dados fictícios, commit no repositório e registro do resultado na pasta de documentação.

A ordem dos incrementos respeita as dependências reais: a estrutura de dados e o cadastro precedem a listagem, que precede a pesquisa; a atualização de situação precede o resumo, porque o resumo consome os dados de situação e de participantes.

## Cronograma

| Sprint | Foco | Atividades | Entregável / responsável | Situação |
|---|---|---|---|---|
| Sprint 1 (Semana 1) | Levantamento e produção textual | Leitura do enunciado; estudo da legislação do PSE e da literatura; definição do problema, dos objetivos e da justificativa; identificação dos usuários e dos limites éticos. | Contextualização, problema, objetivos e justificativa. Responsável principal: Nicollas. | Concluída |
| Sprint 2 (Semana 2) | Modelagem e algoritmos | Especificação dos requisitos funcionais e não funcionais; definição do registro e do vetor; elaboração do fluxograma geral, do fluxograma de cadastro e dos pseudocódigos; criação do repositório com README. | Documento escrito e repositório organizado. Responsável principal: Mateus. | Concluída |
| Sprint 3 (Semana 3) | Incremento 1 — base e cadastro | Implementação do menu com laço de repetição, do registro AcaoPSE, do vetor de armazenamento, do cadastro (RF01) e da listagem (RF02), com validação de campos obrigatórios e de código duplicado. | Versão 0.1 executável no repositório. Responsável principal: Lucas. | Planejada |
| Sprint 4 (Semana 4) | Incremento 2 — consulta e atualização | Implementação da pesquisa por código, escola e tema (RF03), da atualização de situação (RF04) e do registro de participantes efetivos (RF05); tratamento de registro inexistente. | Versão 0.2 executável. Responsável principal: Lucas, com revisão de Mateus. | Planejada |
| Sprint 5 (Semana 5) | Incremento 3 — resumo, validações e testes | Implementação do resumo geral com percentual de participação (RF06); reforço das validações (RF07); testes manuais com dados fictícios e registro dos casos de teste; correção de defeitos. | Versão 1.0 testada e planilha de testes. Responsável: equipe. | Planejada |
| Sprint 6 (Semana 6) | Documentação final e apresentação | Revisão final dos artefatos de modelagem; documentação de uso do sistema; registro dos resultados obtidos; organização final do repositório e preparação da apresentação final do projeto. | Documentação final e apresentação. Responsável: equipe. | Planejada |

## Riscos identificados

| Risco | Prob. / Impacto | Resposta planejada |
|---|---|---|
| Indisponibilidade temporária de um integrante (trabalho, saúde, imprevistos) | Média / Alto | Documentar todas as decisões no repositório e manter tarefas descritas por escrito, para que qualquer integrante possa assumir uma atividade em andamento. |
| Dificuldade com manipulação de cadeias de caracteres e leitura de dados em C | Alta / Médio | Padronizar a leitura de textos com `fgets` e a comparação com `strcmp` desde o primeiro incremento; concentrar a leitura em funções auxiliares reutilizáveis. |
| Crescimento do escopo (pedidos de banco de dados, interface gráfica, relatórios) | Média / Alto | Manter a delimitação de escopo como referência formal e tratar novas ideias como melhorias futuras, registradas no README. |
| Perda de código ou sobrescrita de trabalho de outro integrante | Média / Alto | Trabalhar em branches por funcionalidade, com commits pequenos e frequentes, e integrar na branch principal somente após revisão. |
| Entrada de dados inválida provocando comportamento inesperado | Alta / Médio | Implementar as validações do RF07 no mesmo incremento da funcionalidade correspondente, e não ao final do projeto. |
| Atraso na preparação da apresentação final por dependência de agenda dos três integrantes | Média / Médio | Iniciar a preparação já na sprint 5 e definir previamente a divisão das tarefas entre os integrantes. |

## Fluxo de trabalho no Git

- Uma branch por funcionalidade (ex.: `feature/cadastro-acao`).
- Commits pequenos, com mensagens descritivas.
- Integração na branch principal apenas após revisão de outro integrante.
