[requisitos.md](https://github.com/user-attachments/files/32133703/requisitos.md)

# Requisitos do sistema — PSE em Ação

Documento de apoio da 1ª etapa da AEP. As descrições completas, com a justificativa de cada decisão, estão no documento escrito em `docs/entrega1/AEP_Entrega1.pdf` (seções 5.3 e 5.4).

## Usuários do sistema

| Usuário | Como utiliza o sistema |
|---|---|
| Coordenador do GTI Municipal | Cadastra e revisa ações planejadas, consulta o resumo geral e acompanha o percentual de participação. |
| Profissional da Atenção Primária à Saúde | Atualiza a situação da ação para realizada ou cancelada e registra a quantidade efetiva de participantes. |
| Representante da escola | Pesquisa ações por escola ou tema para confirmar datas, público-alvo e responsável. |
| Apoio administrativo da Secretaria | Lista as ações cadastradas e gera o resumo geral para subsidiar relatórios. |

Sistema de perfil único, sem autenticação: qualquer integrante autorizado da equipe opera o mesmo menu.

## Requisitos funcionais

| ID | Requisito | Descrição | Prioridade |
|---|---|---|---|
| RF01 | Cadastrar ação do PSE | Permitir o cadastro de uma ação informando código, escola, tema, data prevista, público-alvo, responsável e quantidade prevista de participantes, atribuindo automaticamente a situação inicial "Planejada". | Alta |
| RF02 | Listar ações cadastradas | Exibir todas as ações registradas em formato organizado, com identificação da escola, do tema, da data prevista, do responsável, da situação e das quantidades prevista e efetiva. | Alta |
| RF03 | Pesquisar ações | Permitir a busca de ações por código, por escola ou por tema, exibindo todos os registros correspondentes e informando quando nenhum registro for encontrado. | Alta |
| RF04 | Atualizar situação da ação | Permitir alterar a situação de uma ação localizada por código para planejada, realizada ou cancelada. | Alta |
| RF05 | Registrar participantes efetivos | Solicitar e armazenar a quantidade efetiva de participantes quando a ação for marcada como realizada, zerando esse valor quando a ação for cancelada. | Alta |
| RF06 | Gerar resumo geral | Apresentar a quantidade de ações planejadas, realizadas e canceladas, o total de participantes previstos e efetivos e o percentual de participação em relação ao previsto. | Alta |
| RF07 | Validar entradas | Impedir códigos repetidos, quantidades negativas e campos obrigatórios vazios, e recusar opções inexistentes no menu, exibindo mensagem explicativa e mantendo o programa em execução. | Alta |
| RF08 | Encerrar a aplicação | Permitir o encerramento do programa por opção do menu principal, exibindo mensagem de confirmação. | Média |

## Requisitos não funcionais

| ID | Categoria | Descrição |
|---|---|---|
| RNF01 | Usabilidade | O menu deve ser numérico e autoexplicativo, e toda operação deve produzir mensagem clara de confirmação ou de erro, sem uso de termos técnicos desnecessários, pois os usuários não são da área de tecnologia. |
| RNF02 | Portabilidade | A aplicação deve ser escrita em linguagem C padrão, compilar com GCC em Windows e Linux e não depender de bibliotecas externas ou de recursos específicos de sistema operacional. |
| RNF03 | Manutenibilidade | A solução deve ser modularizada em funções com responsabilidade única e nomes significativos, mantendo a função principal restrita ao controle do menu. |
| RNF04 | Privacidade e conformidade | O sistema deve operar apenas com dados fictícios e informações coletivas, não permitindo o armazenamento de nome, matrícula, diagnóstico, prontuário ou qualquer dado individual sensível de estudantes. |
| RNF05 | Confiabilidade | Nenhuma entrada inválida deve encerrar o programa de forma abrupta: o sistema deve tratar o erro, informar o usuário e retornar ao fluxo normal de operação. |
| RNF06 | Capacidade e desempenho | O armazenamento ocorre em memória, em vetor com limite de 100 ações por execução; as buscas são sequenciais e devem responder de forma imediata nesse volume, informando o usuário quando o limite for atingido. |

## Estrutura de dados prevista

Cada ação é representada por um registro e o conjunto de ações por um vetor dimensionado pela constante `MAX_ACOES`, fixada em 100.

| Campo | Tipo | Observação |
|---|---|---|
| codigo | char[11] | Identificador único da ação, informado pelo usuário; não pode repetir nem ficar vazio. |
| escola | char[61] | Nome fictício da escola pactuada; campo obrigatório. |
| tema | char[41] | Temática do PSE; campo obrigatório e livre, para não limitar o sistema a um único tema. |
| data_prevista | char[11] | Data no formato DD/MM/AAAA, tratada como texto nesta versão. |
| publico_alvo | char[41] | Descrição coletiva do público, sem identificar estudantes. |
| responsavel | char[51] | Nome fictício do profissional responsável pela condução da ação. |
| qtd_prevista | int | Quantidade prevista de participantes; deve ser maior que zero. |
| qtd_efetiva | int | Quantidade efetiva de participantes; inicia em 0 e só é preenchida quando a ação é marcada como realizada. |
| situacao | int | 1 = Planejada, 2 = Realizada, 3 = Cancelada; inicia sempre em 1. |

## Fora do escopo

- Banco de dados, interface gráfica, aplicação web e bibliotecas avançadas.
- Autenticação de usuários, perfis de acesso e trilha de auditoria.
- Qualquer dado individual de estudante: nome, matrícula, prontuário, diagnóstico ou condição clínica.
- Diagnóstico, triagem médica, prescrição ou recomendação de tratamento.
- Integração com sistemas oficiais e emissão de relatórios oficiais de prestação de contas..
- Persistência definitiva dos dados: a gravação em arquivo é tratada como melhoria opcional.
