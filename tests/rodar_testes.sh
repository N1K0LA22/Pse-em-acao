#!/usr/bin/env bash
#
# Roda os casos de teste do PSE em Acao.
#
# Uso (a partir de qualquer pasta):  ./tests/rodar_testes.sh
# No Windows, rode pelo Git Bash (precisa do gcc no PATH).
#
# O que o script faz:
#  1. Compila src/main.c com os dois comandos exigidos (gnu89 e c99);
#     aborta se houver erro e mostra se houve warnings.
#  2. Gera um segundo executavel com -DMAX_ACOES=3 para o teste de limite
#     (usado pelos casos cujo nome contem "max3").
#  3. Executa cada caso de tests/casos/ por redirecionamento:
#        ./pse < tests/casos/CTxx.txt > tests/saidas/CTxx.txt 2>&1
#     com limite de tempo, para detectar laco infinito.
#  4. Confere a saida com tests/esperado/CTxx.txt: cada linha e um trecho
#     que DEVE aparecer na saida; linha que comeca com "!" e um trecho que
#     NAO pode aparecer.
#  5. Apaga os executaveis e mostra um resumo.

cd "$(dirname "$0")/.." || exit 1

CC="${CC:-gcc}"
LIMITE_SEGUNDOS=10
EXEC="./pse"
EXEC_LIMITE="./pse_limite"
LOG_COMPILACAO="tests/saidas/.compilacao.log"

mkdir -p tests/saidas

# ---------- 1 e 2: compilacao ----------

compilar() {
    # $1 = descricao; demais argumentos = comando de compilacao
    descricao="$1"
    shift
    echo "Compilando ($descricao): $*"
    if ! "$@" > "$LOG_COMPILACAO" 2>&1; then
        echo "ERRO de compilacao ($descricao):"
        cat "$LOG_COMPILACAO"
        rm -f "$LOG_COMPILACAO"
        exit 1
    fi
    if [ -s "$LOG_COMPILACAO" ]; then
        echo "  ATENCAO: houve warnings:"
        cat "$LOG_COMPILACAO"
        HOUVE_WARNING=1
    else
        echo "  OK, zero erros e zero warnings."
    fi
    rm -f "$LOG_COMPILACAO"
}

HOUVE_WARNING=0
compilar "gnu89" "$CC" -std=gnu89 -Wall -Wextra -Wdeclaration-after-statement -o pse src/main.c
compilar "c99" "$CC" -std=c99 -Wall -Wextra -pedantic -o pse src/main.c
compilar "limite MAX_ACOES=3" "$CC" -std=c99 -Wall -Wextra -pedantic -DMAX_ACOES=3 -o pse_limite src/main.c
echo

# ---------- protecao contra travamento ----------

if command -v timeout > /dev/null 2>&1; then
    COM_TEMPO="timeout $LIMITE_SEGUNDOS"
elif command -v gtimeout > /dev/null 2>&1; then
    COM_TEMPO="gtimeout $LIMITE_SEGUNDOS"
elif command -v perl > /dev/null 2>&1; then
    COM_TEMPO="perl -e alarm($LIMITE_SEGUNDOS);exec(@ARGV)"
else
    COM_TEMPO=""
    echo "Aviso: sem timeout nem perl; os casos rodam sem limite de tempo."
fi

# ---------- 3 e 4: execucao e conferencia ----------

total=0
passou=0
lista_falhou=""
lista_codigo=""
lista_tempo=""

for caso in tests/casos/CT*.txt; do
    nome="$(basename "$caso" .txt)"
    saida="tests/saidas/$nome.txt"
    esperado="tests/esperado/$nome.txt"
    total=$((total + 1))

    case "$nome" in
        *max3*) programa="$EXEC_LIMITE" ;;
        *)      programa="$EXEC" ;;
    esac

    # shellcheck disable=SC2086
    $COM_TEMPO "$programa" < "$caso" > "$saida" 2>&1
    codigo=$?

    ok=1
    detalhe=""

    # 124 = timeout (coreutils); 142 = SIGALRM do perl (128 + 14)
    if [ "$codigo" -eq 124 ] || [ "$codigo" -eq 142 ]; then
        ok=0
        detalhe="tempo esgotado (${LIMITE_SEGUNDOS}s)"
        lista_tempo="$lista_tempo $nome"
    elif [ "$codigo" -ne 0 ]; then
        ok=0
        detalhe="codigo de saida $codigo"
        lista_codigo="$lista_codigo $nome($codigo)"
    fi

    if [ -f "$esperado" ]; then
        while IFS= read -r trecho || [ -n "$trecho" ]; do
            [ -z "$trecho" ] && continue
            case "$trecho" in
                !*)
                    proibido="${trecho#!}"
                    if grep -qF -- "$proibido" "$saida"; then
                        ok=0
                        detalhe="$detalhe | apareceu (nao devia): \"$proibido\""
                    fi
                    ;;
                *)
                    if ! grep -qF -- "$trecho" "$saida"; then
                        ok=0
                        detalhe="$detalhe | faltou: \"$trecho\""
                    fi
                    ;;
            esac
        done < "$esperado"
    else
        detalhe="$detalhe | sem arquivo de esperado"
    fi

    if [ "$ok" -eq 1 ]; then
        passou=$((passou + 1))
        echo "PASSOU  $nome  (codigo de saida $codigo)"
    else
        lista_falhou="$lista_falhou $nome"
        echo "FALHOU  $nome  (codigo de saida $codigo) $detalhe"
    fi
done

# ---------- 5: limpeza e resumo ----------

rm -f "$EXEC" "$EXEC_LIMITE" pse.exe pse_limite.exe

echo
echo "===== Resumo ====="
echo "Casos executados..........: $total"
echo "Passaram..................: $passou"
echo "Falharam..................: $((total - passou))${lista_falhou:+ ->$lista_falhou}"
echo "Codigo de saida != 0......:${lista_codigo:- nenhum}"
echo "Tempo esgotado............:${lista_tempo:- nenhum}"
if [ "$HOUVE_WARNING" -eq 1 ]; then
    echo "Compilacao................: com warnings (ver acima)"
else
    echo "Compilacao................: zero warnings nos dois modos"
fi
echo "Saidas gravadas em tests/saidas/"

if [ "$passou" -ne "$total" ]; then
    exit 1
fi
exit 0
