[README.md](https://github.com/user-attachments/files/32133732/README.md)
# PSE em Ação — Sistema de Planejamento e Acompanhamento de Ações de Saúde na Escola

Atividade de Estudo Prático (AEP) — Universidade Cesumar (UniCesumar)
Análise e Desenvolvimento de Sistemas — Série ADSIS2S-N-A — 2º semestre de 2026

Este repositório reúne a documentação da **1ª etapa** da AEP: análise do problema, requisitos, fluxogramas, pseudocódigos e planejamento do desenvolvimento.

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
| RF01 | Cadastrar ação do PSE (código, escola, tema, data prevista, público-alvo, responsável, quantidade prevista, situação inicial) |
| RF02 | Listar todas as ações cadastradas |
| RF03 | Pesquisar ações por código, escola ou tema |
| RF04 | Atualizar a situação da ação (planejada, realizada ou cancelada) |
| RF05 | Registrar a quantidade efetiva de participantes das ações realizadas |
| RF06 | Gerar resumo geral com totais por situação, participantes e percentual de participação |
| RF07 | Validar entradas (código duplicado, quantidade negativa, campo vazio, opção inexistente) |
| RF08 | Encerrar a aplicação por opção do menu |

Os requisitos não funcionais (usabilidade, portabilidade, manutenibilidade, privacidade, confiabilidade e capacidade) estão descritos em `docs/entrega1/requisitos.md`.

## Estrutura do repositório

```
pse-em-acao/
├── README.md
├── src/                          (reservado para o código-fonte)
├── docs/
│   ├── entrega1/
│   │   ├── AEP_Entrega1.pdf      (documento escrito desta etapa)
│   │   ├── requisitos.md         (RF e RNF em formato consultável)
│   │   └── sprints.md            (planejamento e riscos)
│   └── fluxogramas/
│       ├── fluxograma_geral.png
│       ├── fluxograma_cadastro_parte1.png
│       └── fluxograma_cadastro_parte2.png
└── .gitignore
```

## Conteúdo desta etapa

- Documento acadêmico com contextualização, problema, objetivos e justificativa.
- Identificação dos usuários e delimitação do escopo do sistema.
- Oito requisitos funcionais e seis requisitos não funcionais.
- Fluxograma geral do sistema e fluxograma detalhado da operação de cadastro.
- Pseudocódigos do programa principal e das quatro rotinas centrais.
- Definição da estrutura de dados prevista (registro e vetor em memória).
- Planejamento em sprints semanais e matriz de riscos.

## Planejamento (processo incremental, sprints semanais)

| Sprint | Foco | Situação |
|---|---|---|
| 1 | Levantamento, contextualização, problema, objetivos e justificativa | Concluída |
| 2 | Requisitos, estrutura de dados, fluxogramas, pseudocódigos e repositório | Concluída |
| 3 | Incremento 1 — menu, registro, vetor, cadastro e listagem | Planejada |
| 4 | Incremento 2 — pesquisa, atualização de situação e participantes | Planejada |
| 5 | Incremento 3 — resumo geral, validações e testes | Planejada |
| 6 | Documentação final, revisão dos artefatos e preparação da apresentação | Planejada |

## Fluxo de trabalho no Git.

- Uma branch por funcionalidade (ex.: `feature/cadastro-acao`).
- Commits pequenos, com mensagens descritivas.
- Integração na branch principal apenas após revisão de outro integrante.

## Referências principais

- BRASIL. Decreto nº 6.286, de 5 de dezembro de 2007. Institui o Programa Saúde na Escola — PSE.
- BRASIL. Portaria Interministerial MS/MEC nº 1.055, de 25 de abril de 2017.
- BRASIL. Lei nº 13.709, de 14 de agosto de 2018 (LGPD).
- SOUSA, M. C.; ESPERIDIÃO, M. A.; MEDINA, M. G. A intersetorialidade no Programa Saúde na Escola. *Ciência & Saúde Coletiva*, v. 22, n. 6, 2017.
