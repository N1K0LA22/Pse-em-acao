/*
 * PSE em Acao - Sistema de Planejamento e Acompanhamento
 * de Acoes Coletivas do Programa Saude na Escola (PSE)
 *
 * AEP - Analise e Desenvolvimento de Sistemas - UniCesumar
 * Etapa 2 - Incremento 3: resumo geral e revisao das validacoes
 * (alem do cadastro, listagem, pesquisa, atualizacao da situacao
 * e registro de participantes dos incrementos anteriores).
 *
 * Todos os dados usados no sistema sao ficticios e coletivos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Limite de acoes em memoria (pode ser trocado na compilacao com -DMAX_ACOES=3) */
#ifndef MAX_ACOES
#define MAX_ACOES 100
#endif

/* Tamanhos dos campos (Quadro 6 da entrega 1), ja contando o '\0' final */
#define TAM_CODIGO      11
#define TAM_ESCOLA      61
#define TAM_TEMA        41
#define TAM_DATA        11
#define TAM_PUBLICO     41
#define TAM_RESPONSAVEL 51

/* Tamanho do texto usado para ler numeros digitados */
#define TAM_ENTRADA_NUMERO 20

/* Tamanho da linha lida do teclado antes de conferir o tamanho do campo */
#define TAM_LINHA 256

/* Tamanho do termo de busca (o maior campo pesquisado e a escola) */
#define TAM_TERMO TAM_ESCOLA

/* Maior quantidade de participantes aceita (evita numeros absurdos) */
#define MAX_PARTICIPANTES 100000

/* Situacoes possiveis de uma acao */
#define SITUACAO_PLANEJADA 1
#define SITUACAO_REALIZADA 2
#define SITUACAO_CANCELADA 3

/* Opcoes do menu principal */
#define OPCAO_CADASTRAR   1
#define OPCAO_LISTAR      2
#define OPCAO_PESQUISAR   3
#define OPCAO_ATUALIZAR   4
#define OPCAO_REGISTRAR   5
#define OPCAO_RESUMO      6
#define OPCAO_ENCERRAR    7

/* Criterios da pesquisa (pseudocodigo 3) */
#define CRITERIO_CODIGO 1
#define CRITERIO_ESCOLA 2
#define CRITERIO_TEMA   3

/* Registro de uma acao coletiva do PSE (Quadro 6 da entrega 1) */
struct AcaoPSE {
    char codigo[TAM_CODIGO];
    char escola[TAM_ESCOLA];
    char tema[TAM_TEMA];
    char data_prevista[TAM_DATA];
    char publico_alvo[TAM_PUBLICO];
    char responsavel[TAM_RESPONSAVEL];
    int qtd_prevista;
    int qtd_efetiva;
    int situacao;
};

/* Vetor de acoes e contador, como no pseudocodigo 1 da entrega 1 */
struct AcaoPSE acoes[MAX_ACOES];
int total_acoes = 0;

/* ---------- Prototipos ---------- */

/* Leitura segura de dados */
void encerrar_fim_entrada(void);
void remover_espacos_pontas(char *texto);
int ler_linha(char *destino, int tamanho);
void ler_texto(const char *rotulo, char *destino, int tamanho);
int texto_para_inteiro(const char *texto, int *valor);
int ler_inteiro(const char *rotulo, int minimo, int maximo, const char *msg_faixa);
int validar_data(const char *data);
void ler_data(const char *rotulo, char *destino);
int ler_qtd_efetiva(void);

/* Apoio */
int textos_iguais_sem_caixa(const char *a, const char *b);
int contem_texto_sem_caixa(const char *texto, const char *termo);
int buscar_por_codigo(const char *codigo);
const char *texto_situacao(int situacao);
void exibir_acao(const struct AcaoPSE *acao);

/* Menu e funcionalidades */
void exibir_menu(void);
int ler_opcao_menu(void);
void cadastrar_acao(void);
void listar_acoes(void);
void pesquisar_acao(void);
void atualizar_situacao(void);
void registrar_participantes(void);
void gerar_resumo(void);

/* ---------- Programa principal: so controla o menu ---------- */

int main(void)
{
    int opcao;

    total_acoes = 0;

    do {
        exibir_menu();
        opcao = ler_opcao_menu();

        switch (opcao) {
        case OPCAO_CADASTRAR:
            cadastrar_acao();
            break;
        case OPCAO_LISTAR:
            listar_acoes();
            break;
        case OPCAO_PESQUISAR:
            pesquisar_acao();
            break;
        case OPCAO_ATUALIZAR:
            atualizar_situacao();
            break;
        case OPCAO_REGISTRAR:
            registrar_participantes();
            break;
        case OPCAO_RESUMO:
            gerar_resumo();
            break;
        case OPCAO_ENCERRAR:
            printf("\nPrograma encerrado. Ate logo!\n");
            break;
        default:
            printf("\nOpcao invalida. Digite um numero de 1 a 7.\n");
            break;
        }
    } while (opcao != OPCAO_ENCERRAR);

    return 0;
}

/* ---------- Leitura segura de dados ---------- */

/* Chamada quando a entrada acaba (Ctrl+D / Ctrl+Z ou fim do arquivo) */
void encerrar_fim_entrada(void)
{
    printf("\n\nFim da entrada de dados. Programa encerrado.\n");
    exit(0);
}

/* Tira espacos, tabs e quebras de linha do inicio e do fim do texto */
void remover_espacos_pontas(char *texto)
{
    int inicio = 0;
    int fim;
    int i;

    while (texto[inicio] != '\0' && isspace((unsigned char) texto[inicio])) {
        inicio++;
    }

    /* Desloca o texto para o comeco, se havia espacos na frente */
    if (inicio > 0) {
        i = 0;
        while (texto[inicio + i] != '\0') {
            texto[i] = texto[inicio + i];
            i++;
        }
        texto[i] = '\0';
    }

    fim = (int) strlen(texto) - 1;
    while (fim >= 0 && isspace((unsigned char) texto[fim])) {
        texto[fim] = '\0';
        fim--;
    }
}

/*
 * Le uma linha do teclado com fgets e guarda em destino, ja sem
 * o '\n', o '\r' (arquivos do Windows) e os espacos das pontas.
 * A linha e lida primeiro em um texto local maior; so depois de
 * limpar as pontas e que o tamanho e comparado com o do campo.
 * Retorna 1 se o texto coube em destino e 0 se era maior
 * (nesse caso o resto da linha ja foi descartado).
 * Se a entrada acabar, encerra o programa.
 */
int ler_linha(char *destino, int tamanho)
{
    char linha[TAM_LINHA];
    int c;

    if (fgets(linha, TAM_LINHA, stdin) == NULL) {
        encerrar_fim_entrada();
    }

    /* Sem '\n' no texto lido: ou a entrada acabou, ou a linha e enorme */
    if (strchr(linha, '\n') == NULL) {
        c = getchar();
        if (c != '\n' && c != EOF) {
            /* Descarta o resto da linha que nao coube */
            while (c != '\n' && c != EOF) {
                c = getchar();
            }
            return 0;
        }
    }

    remover_espacos_pontas(linha);
    if ((int) strlen(linha) > tamanho - 1) {
        return 0;
    }

    strcpy(destino, linha);
    return 1;
}

/* Le um texto obrigatorio, repetindo a pergunta ate ser valido */
void ler_texto(const char *rotulo, char *destino, int tamanho)
{
    int valido = 0;

    while (!valido) {
        printf("%s", rotulo);
        if (!ler_linha(destino, tamanho)) {
            printf("Texto muito longo. Use no maximo %d caracteres.\n", tamanho - 1);
            continue;
        }

        if (destino[0] == '\0') {
            printf("Campo obrigatorio. Preencha este campo.\n");
        } else {
            valido = 1;
        }
    }
}

/*
 * Converte um texto em inteiro.
 * Retorna 1 se o texto inteiro for um numero (ex.: "12", "-3")
 * e 0 se estiver vazio ou tiver letras (ex.: "abc", "12abc").
 */
int texto_para_inteiro(const char *texto, int *valor)
{
    char *resto;
    long numero;

    if (texto[0] == '\0') {
        return 0;
    }

    numero = strtol(texto, &resto, 10);
    if (*resto != '\0') {
        return 0;
    }

    /* Numeros enormes viram um valor fora da faixa, que sera recusado */
    if (numero > MAX_PARTICIPANTES * 10L) {
        numero = MAX_PARTICIPANTES * 10L;
    }
    if (numero < -MAX_PARTICIPANTES * 10L) {
        numero = -MAX_PARTICIPANTES * 10L;
    }

    *valor = (int) numero;
    return 1;
}

/* Le um inteiro entre minimo e maximo, repetindo a pergunta ate ser valido */
int ler_inteiro(const char *rotulo, int minimo, int maximo, const char *msg_faixa)
{
    char entrada[TAM_ENTRADA_NUMERO];
    int valor = 0;
    int valido = 0;

    while (!valido) {
        printf("%s", rotulo);
        if (!ler_linha(entrada, TAM_ENTRADA_NUMERO)) {
            printf("Valor invalido. Digite apenas numeros inteiros.\n");
            continue;
        }

        if (!texto_para_inteiro(entrada, &valor)) {
            printf("Valor invalido. Digite apenas numeros inteiros.\n");
        } else if (valor < minimo || valor > maximo) {
            printf("%s\n", msg_faixa);
        } else {
            valido = 1;
        }
    }
    return valor;
}

/* Validacao simples de data DD/MM/AAAA: dia 1-31, mes 1-12, ano 2000-2100 */
int validar_data(const char *data)
{
    int i;
    int dia, mes, ano;

    if (strlen(data) != 10 || data[2] != '/' || data[5] != '/') {
        return 0;
    }

    for (i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit((unsigned char) data[i])) {
            return 0;
        }
    }

    dia = (data[0] - '0') * 10 + (data[1] - '0');
    mes = (data[3] - '0') * 10 + (data[4] - '0');
    ano = (data[6] - '0') * 1000 + (data[7] - '0') * 100
        + (data[8] - '0') * 10 + (data[9] - '0');

    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 2000 || ano > 2100) {
        return 0;
    }
    return 1;
}

/* Le a data prevista, repetindo ate estar no formato DD/MM/AAAA */
void ler_data(const char *rotulo, char *destino)
{
    int valida = 0;

    while (!valida) {
        ler_texto(rotulo, destino, TAM_DATA);
        if (validar_data(destino)) {
            valida = 1;
        } else {
            printf("Data invalida. Use o formato DD/MM/AAAA (ex.: 15/03/2026).\n");
        }
    }
}

/*
 * Le a quantidade efetiva de participantes (zero ou mais).
 * Usada na opcao 4 (ao marcar como Realizada) e na opcao 5.
 * Pode passar da quantidade prevista (vieram mais pessoas).
 */
int ler_qtd_efetiva(void)
{
    return ler_inteiro("Quantidade efetiva de participantes: ",
                       0, MAX_PARTICIPANTES,
                       "Quantidade invalida. Digite zero ou mais (maximo 100000).");
}

/* ---------- Funcoes de apoio ---------- */

/* Compara dois textos sem diferenciar maiusculas e minusculas (1 = iguais) */
int textos_iguais_sem_caixa(const char *a, const char *b)
{
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') {
        if (tolower((unsigned char) a[i]) != tolower((unsigned char) b[i])) {
            return 0;
        }
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}

/*
 * Verifica se termo aparece dentro de texto, sem diferenciar
 * maiusculas e minusculas (1 = contem, 0 = nao contem).
 * Ex.: "VACINAL" esta dentro de "Verificacao vacinal".
 */
int contem_texto_sem_caixa(const char *texto, const char *termo)
{
    int inicio;
    int j;

    for (inicio = 0; texto[inicio] != '\0'; inicio++) {
        /* Compara o termo a partir desta posicao do texto */
        j = 0;
        while (termo[j] != '\0' && texto[inicio + j] != '\0'
               && tolower((unsigned char) texto[inicio + j])
                  == tolower((unsigned char) termo[j])) {
            j++;
        }
        if (termo[j] == '\0') {
            return 1;
        }
    }
    return 0;
}

/* Procura uma acao pelo codigo. Retorna o indice no vetor ou -1 */
int buscar_por_codigo(const char *codigo)
{
    int i;

    for (i = 0; i < total_acoes; i++) {
        if (textos_iguais_sem_caixa(acoes[i].codigo, codigo)) {
            return i;
        }
    }
    return -1;
}

/* Converte o numero da situacao no texto mostrado ao usuario */
const char *texto_situacao(int situacao)
{
    switch (situacao) {
    case SITUACAO_PLANEJADA:
        return "Planejada";
    case SITUACAO_REALIZADA:
        return "Realizada";
    case SITUACAO_CANCELADA:
        return "Cancelada";
    default:
        return "Desconhecida";
    }
}

/* Mostra os dados de uma acao (usada na listagem e na pesquisa) */
void exibir_acao(const struct AcaoPSE *acao)
{
    printf("----------------------------------------\n");
    printf("Codigo.............: %s\n", acao->codigo);
    printf("Escola.............: %s\n", acao->escola);
    printf("Tema...............: %s\n", acao->tema);
    printf("Data prevista......: %s\n", acao->data_prevista);
    printf("Publico-alvo.......: %s\n", acao->publico_alvo);
    printf("Responsavel........: %s\n", acao->responsavel);
    printf("Situacao...........: %s\n", texto_situacao(acao->situacao));
    printf("Qtd. prevista......: %d\n", acao->qtd_prevista);
    printf("Qtd. efetiva.......: %d\n", acao->qtd_efetiva);
}

/* ---------- Menu ---------- */

void exibir_menu(void)
{
    printf("\n===== PSE EM ACAO =====\n");
    printf("1 - Cadastrar acao\n");
    printf("2 - Listar acoes\n");
    printf("3 - Pesquisar acao\n");
    printf("4 - Atualizar situacao\n");
    printf("5 - Registrar participantes\n");
    printf("6 - Gerar resumo\n");
    printf("7 - Encerrar\n");
    printf("Escolha uma opcao: ");
}

/*
 * Le a opcao do menu (e tambem dos submenus de pesquisa e de situacao).
 * Retorna -1 se nao for um numero (quem chamou avisa o erro).
 */
int ler_opcao_menu(void)
{
    char entrada[TAM_ENTRADA_NUMERO];
    int opcao;

    if (!ler_linha(entrada, TAM_ENTRADA_NUMERO)) {
        return -1;
    }
    if (!texto_para_inteiro(entrada, &opcao)) {
        return -1;
    }
    return opcao;
}

/* ---------- Funcionalidades ---------- */

/* Opcao 1: cadastra uma nova acao (pseudocodigo 2 e fluxograma de cadastro) */
void cadastrar_acao(void)
{
    struct AcaoPSE nova;
    int codigo_ok = 0;

    printf("\n--- Cadastrar acao ---\n");

    /* O limite e verificado antes de pedir qualquer dado */
    if (total_acoes >= MAX_ACOES) {
        printf("Limite de registros atingido (%d acoes). Nao e possivel cadastrar.\n",
               MAX_ACOES);
        return;
    }

    /* Codigo: obrigatorio e sem repetir */
    while (!codigo_ok) {
        ler_texto("Codigo da acao (ate 10 caracteres): ", nova.codigo, TAM_CODIGO);
        if (buscar_por_codigo(nova.codigo) != -1) {
            printf("Codigo ja cadastrado. Digite outro.\n");
        } else {
            codigo_ok = 1;
        }
    }

    ler_texto("Escola: ", nova.escola, TAM_ESCOLA);
    ler_texto("Tema: ", nova.tema, TAM_TEMA);
    ler_data("Data prevista (DD/MM/AAAA): ", nova.data_prevista);
    ler_texto("Publico-alvo (ex.: 5o ano - turmas A e B): ", nova.publico_alvo, TAM_PUBLICO);
    ler_texto("Responsavel: ", nova.responsavel, TAM_RESPONSAVEL);

    nova.qtd_prevista = ler_inteiro("Quantidade prevista de participantes: ",
                                    1, MAX_PARTICIPANTES,
                                    "Quantidade invalida. Digite um valor maior que zero (maximo 100000).");

    /* Toda acao nova comeca como Planejada e sem participantes efetivos */
    nova.situacao = SITUACAO_PLANEJADA;
    nova.qtd_efetiva = 0;

    acoes[total_acoes] = nova;
    total_acoes++;

    printf("\nAcao cadastrada com sucesso! Codigo: %s - Situacao: %s\n",
           nova.codigo, texto_situacao(nova.situacao));
    printf("Acoes cadastradas: %d de %d.\n", total_acoes, MAX_ACOES);
}

/* Opcao 2: lista todas as acoes cadastradas */
void listar_acoes(void)
{
    int i;

    printf("\n--- Lista de acoes ---\n");

    if (total_acoes == 0) {
        printf("Nenhuma acao cadastrada.\n");
        return;
    }

    for (i = 0; i < total_acoes; i++) {
        exibir_acao(&acoes[i]);
    }
    printf("----------------------------------------\n");
    printf("Total de acoes listadas: %d\n", total_acoes);
}

/* Opcao 3: pesquisa por codigo, escola ou tema (pseudocodigo 3) */
void pesquisar_acao(void)
{
    char termo[TAM_TERMO];
    int criterio;
    int i;
    int achou;
    int encontrados = 0;

    printf("\n--- Pesquisar acao ---\n");

    if (total_acoes == 0) {
        printf("Nenhuma acao cadastrada.\n");
        return;
    }

    printf("Pesquisar por: 1 - Codigo   2 - Escola   3 - Tema\n");
    printf("Escolha o criterio: ");
    criterio = ler_opcao_menu();
    if (criterio < CRITERIO_CODIGO || criterio > CRITERIO_TEMA) {
        printf("Opcao invalida. Escolha 1, 2 ou 3. Voltando ao menu.\n");
        return;
    }

    ler_texto("Digite o termo de busca: ", termo, TAM_TERMO);

    for (i = 0; i < total_acoes; i++) {
        if (criterio == CRITERIO_CODIGO) {
            /* Codigo: precisa ser igual (sem diferenciar maiusculas) */
            achou = textos_iguais_sem_caixa(acoes[i].codigo, termo);
        } else if (criterio == CRITERIO_ESCOLA) {
            /* Escola e tema: basta conter o termo digitado */
            achou = contem_texto_sem_caixa(acoes[i].escola, termo);
        } else {
            achou = contem_texto_sem_caixa(acoes[i].tema, termo);
        }

        if (achou) {
            exibir_acao(&acoes[i]);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum registro encontrado.\n");
    } else {
        printf("----------------------------------------\n");
        printf("Registros encontrados: %d\n", encontrados);
    }
}

/* Opcao 4: atualiza a situacao de uma acao (pseudocodigo 4) */
void atualizar_situacao(void)
{
    char codigo[TAM_CODIGO];
    int indice;
    int nova_situacao;
    struct AcaoPSE *acao;

    printf("\n--- Atualizar situacao ---\n");

    if (total_acoes == 0) {
        printf("Nenhuma acao cadastrada.\n");
        return;
    }

    ler_texto("Codigo da acao que sera atualizada: ", codigo, TAM_CODIGO);
    indice = buscar_por_codigo(codigo);
    if (indice == -1) {
        printf("Acao nao encontrada. Confira o codigo na listagem (opcao 2).\n");
        return;
    }

    acao = &acoes[indice];
    printf("Acao %s - situacao atual: %s\n", acao->codigo, texto_situacao(acao->situacao));
    printf("Nova situacao: 1 - Planejada   2 - Realizada   3 - Cancelada\n");
    printf("Escolha a nova situacao: ");
    nova_situacao = ler_opcao_menu();
    if (nova_situacao < SITUACAO_PLANEJADA || nova_situacao > SITUACAO_CANCELADA) {
        printf("Situacao invalida. Escolha 1, 2 ou 3. Nenhuma alteracao foi feita.\n");
        return;
    }

    if (nova_situacao == SITUACAO_REALIZADA) {
        /* Realizada: pede quantas pessoas participaram */
        acao->qtd_efetiva = ler_qtd_efetiva();
    } else {
        /* Planejada ou Cancelada: nao ha participantes efetivos */
        acao->qtd_efetiva = 0;
    }
    acao->situacao = nova_situacao;

    printf("\nSituacao atualizada com sucesso! Acao %s agora esta: %s.\n",
           acao->codigo, texto_situacao(acao->situacao));
    if (acao->situacao == SITUACAO_REALIZADA) {
        printf("Participantes efetivos registrados: %d (previstos: %d).\n",
               acao->qtd_efetiva, acao->qtd_prevista);
    } else {
        printf("Quantidade efetiva de participantes: 0.\n");
    }
}

/* Opcao 5: registra ou corrige os participantes de uma acao realizada */
void registrar_participantes(void)
{
    char codigo[TAM_CODIGO];
    int indice;
    struct AcaoPSE *acao;

    printf("\n--- Registrar participantes ---\n");

    if (total_acoes == 0) {
        printf("Nenhuma acao cadastrada.\n");
        return;
    }

    ler_texto("Codigo da acao: ", codigo, TAM_CODIGO);
    indice = buscar_por_codigo(codigo);
    if (indice == -1) {
        printf("Acao nao encontrada. Confira o codigo na listagem (opcao 2).\n");
        return;
    }

    acao = &acoes[indice];
    if (acao->situacao != SITUACAO_REALIZADA) {
        printf("A acao %s esta %s. So e possivel registrar participantes de acoes realizadas.\n",
               acao->codigo, texto_situacao(acao->situacao));
        printf("Primeiro marque a acao como Realizada na opcao 4 (Atualizar situacao).\n");
        return;
    }

    printf("Acao %s - participantes efetivos atuais: %d (previstos: %d).\n",
           acao->codigo, acao->qtd_efetiva, acao->qtd_prevista);
    acao->qtd_efetiva = ler_qtd_efetiva();

    printf("\nParticipantes registrados com sucesso! Acao %s: %d participantes efetivos.\n",
           acao->codigo, acao->qtd_efetiva);
}

/*
 * Opcao 6: mostra o resumo geral (pseudocodigo 5).
 * O percentual de participacao considera apenas as acoes realizadas,
 * para que acoes planejadas ou canceladas nao distorcam o indicador.
 */
void gerar_resumo(void)
{
    int i;
    int planejadas = 0;
    int realizadas = 0;
    int canceladas = 0;
    long total_previsto = 0;   /* soma das quantidades previstas das realizadas */
    long total_efetivo = 0;    /* soma das quantidades efetivas das realizadas */
    double percentual;

    printf("\n--- Resumo geral ---\n");

    if (total_acoes == 0) {
        printf("Nenhuma acao cadastrada. Resumo indisponivel.\n");
        return;
    }

    for (i = 0; i < total_acoes; i++) {
        if (acoes[i].situacao == SITUACAO_PLANEJADA) {
            planejadas++;
        } else if (acoes[i].situacao == SITUACAO_REALIZADA) {
            realizadas++;
            total_previsto = total_previsto + acoes[i].qtd_prevista;
            total_efetivo = total_efetivo + acoes[i].qtd_efetiva;
        } else if (acoes[i].situacao == SITUACAO_CANCELADA) {
            canceladas++;
        }
    }

    printf("Total de acoes cadastradas......: %d\n", total_acoes);
    printf("Acoes planejadas................: %d\n", planejadas);
    printf("Acoes realizadas................: %d\n", realizadas);
    printf("Acoes canceladas................: %d\n", canceladas);
    printf("\nParticipacao (somente acoes realizadas):\n");
    printf("Participantes previstos.........: %ld\n", total_previsto);
    printf("Participantes efetivos..........: %ld\n", total_efetivo);

    /* Sem acao realizada o total previsto e zero: nao pode dividir */
    if (total_previsto > 0) {
        /* Converte para double antes de dividir para nao perder as casas decimais */
        percentual = (double) total_efetivo / (double) total_previsto * 100.0;
        printf("Percentual de participacao......: %.1f%%\n", percentual);
        printf("(participantes efetivos / previstos das acoes realizadas)\n");
    } else {
        printf("Percentual de participacao......: nao calculado.\n");
        printf("Nenhuma acao realizada ainda; o percentual so considera acoes realizadas.\n");
    }
}
