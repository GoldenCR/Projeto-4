#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CLIENTES 1000

typedef struct {
    int CPF;
    char tipoConta[10];
    float saldo;
    char senhaUsuario[50];
} Cliente;

void menu() {
    printf("\nMenu:\n");
    printf("1. Novo cliente\n");
    printf("2. Apaga cliente\n");
    printf("3. Listar clientes\n");
    printf("4. Débito\n");
    printf("5. Depósito\n");
    printf("6. Extrato\n");
    printf("7. Transferência entre contas\n");
    printf("8. Sair\n");
}

void novo_cliente(Cliente *clientes, int *num_clientes) {
    if (*num_clientes >= MAX_CLIENTES) {
        printf("Limite máximo de clientes atingido.\n");
        return;
    }

    system("cls || clear");
    printf("\nNovo Cliente\n");
    printf("CPF: ");
    scanf("%d", &clientes[*num_clientes].CPF);
    printf("Tipo de conta (1) Comum ou (2) Plus: ");
    scanf("%s", clientes[*num_clientes].tipoConta);
    printf("Valor inicial da conta: ");
    scanf("%f", &clientes[*num_clientes].saldo);
    printf("Senha: ");
    scanf("%s", clientes[*num_clientes].senhaUsuario);

    FILE *arquivo;
    arquivo = fopen("Cliente.txt", "a");
    fprintf(arquivo, "CPF: %d\nConta: %s\nSaldo: %.2f\nSenha: %s\n",
            clientes[*num_clientes].CPF, (strcmp(clientes[*num_clientes].tipoConta, "1") == 0) ? "Comum" : "Plus", clientes[*num_clientes].saldo, clientes[*num_clientes].senhaUsuario);
    fclose(arquivo);

    (*num_clientes)++;
}

void apaga_cliente() {
    system("cls || clear");
    printf("\nApagar Cliente\n");
    int Apaga_CPF;
    printf("Insira o CPF para apagar a conta: ");
    scanf("%d", &Apaga_CPF);

    FILE *arquivo;
    FILE *temp;
    arquivo = fopen("Cliente.txt", "r");
    temp = fopen("temp.txt", "w");

    int cpf_encontrado = 0;
    int CPF;
    char tipoConta[10];
    float saldo;
    char senhaUsuario[50];

    while (fscanf(arquivo, "CPF: %d\nConta: %s\nSaldo: %f\nSenha: %s\n", &CPF, tipoConta, &saldo, senhaUsuario) != EOF) {
        if (CPF == Apaga_CPF) {
            cpf_encontrado = 1;
        } else {
            fprintf(temp, "CPF: %d\nConta: %s\nSaldo: %.2f\nSenha: %s\n", CPF, tipoConta, saldo, senhaUsuario);
        }
    }

    fclose(arquivo);
    fclose(temp);

    remove("Cliente.txt");
    rename("temp.txt", "Cliente.txt");

    if (cpf_encontrado) {
        printf("Conta apagada.\n");
    } else {
        printf("CPF não encontrado.\n");
    }
}

void listar_clientes(Cliente *clientes, int num_clientes) {
    system("cls || clear");
    printf("Lista de clientes: \n");

    for (int i = 0; i < num_clientes; i++) {
        printf("CPF: %d\n", clientes[i].CPF);
        printf("Conta: %s\n", strcmp(clientes[i].tipoConta, "1") == 0 ? "Comum" : "Plus");
        printf("Saldo: %.2f\n", clientes[i].saldo);
        printf("Senha: %s\n\n", clientes[i].senhaUsuario);
    }
}

void debitar(Cliente *clientes, int num_clientes) {
    int cpf;
    float valor;
    char senha[50];

    printf("\nDébito em Conta Corrente\n");
    printf("Digite o CPF: ");
    scanf("%d", &cpf);
    printf("Digite sua senha: ");
    scanf("%s", senha);
    printf("Digite o valor de débito: ");
    scanf("%f", &valor);

    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i].CPF == cpf && strcmp(clientes[i].senhaUsuario, senha) == 0) {
            if (clientes[i].saldo >= valor) {
                clientes[i].saldo -= valor;
                printf("\nDébito de %.2f realizado com sucesso na conta de CPF %d.\n", valor, cpf);
                FILE *extrato;
                extrato = fopen("Extrato.txt", "a");
                fprintf(extrato, "\nCPF: %d | Débito de %.2f realizado | Saldo atual: %.2f\n", cpf, valor, clientes[i].saldo);
                fclose(extrato);
            } else {
                printf("\nSaldo insuficiente para realizar o débito.\n");
            }
            return;
        }
    }

    printf("\nCPF ou senha incorretos.\n");
}

void depositar(Cliente *clientes, int num_clientes) {
    int cpf;
    float valor;
    char senha[50];

    printf("\nDepósito em Conta Corrente\n");
    printf("Digite o CPF: ");
    scanf("%d", &cpf);
    printf("Digite sua senha: ");
    scanf("%s", senha);
    printf("Digite o valor a ser depositado: ");
    scanf("%f", &valor);

    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i].CPF == cpf && strcmp(clientes[i].senhaUsuario, senha) == 0) {
            clientes[i].saldo += valor;
            printf("\nDepósito de %.2f realizado com sucesso na conta de CPF %d.\n", valor, cpf);
            FILE *extrato;
            extrato = fopen("Extrato.txt", "a");
            fprintf(extrato, "\nCPF: %d | Depósito de %.2f realizado | Saldo atual: %.2f\n", cpf, valor, clientes[i].saldo);
            fclose(extrato);
            return;
        }
    }

    printf("\nCPF ou senha incorretos.\n");
}

void extrato(Cliente *clientes, int num_clientes) {
    int cpf;
    char senha[50];

    printf("\nExtrato Bancário\n");
    printf("Digite o CPF: ");
    scanf("%d", &cpf);
    printf("Digite sua senha: ");
    scanf("%s", senha);

    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i].CPF == cpf && strcmp(clientes[i].senhaUsuario, senha) == 0) {
            printf("\nExtrato para o CPF %d:\n", cpf);
            printf("Tipo de conta: %s\n", clientes[i].tipoConta);
            printf("Saldo atual: %.2f\n", clientes[i].saldo);
            printf("-----\n");
            return;
        }
    }

    printf("\nCPF ou senha incorretos.\n");
}

void transferencia(Cliente *clientes, int num_clientes) {
    int cpf_origem, cpf_destino;
    float valor;
    char senha[50];

    printf("\nTransferência entre Contas\n");
    printf("Digite o CPF da conta de origem: ");
    scanf("%d", &cpf_origem);
    printf("Digite sua senha: ");
    scanf("%s", senha);

    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i].CPF == cpf_origem && strcmp(clientes[i].senhaUsuario, senha) == 0) {
            printf("Digite o CPF da conta de destino: ");
            scanf("%d", &cpf_destino);
            printf("Digite o valor a ser transferido: ");
            scanf("%f", &valor);

            for (int j = 0; j < num_clientes; j++) {
                if (clientes[j].CPF == cpf_destino) {
                    if (clientes[i].saldo >= valor) {
                        clientes[i].saldo -= valor;
                        clientes[j].saldo += valor;
                        printf("\nTransferência de %.2f realizada com sucesso da conta de CPF %d para a conta de CPF %d.\n", valor, cpf_origem, cpf_destino);
                        FILE *extrato;
                        extrato = fopen("Extrato.txt", "a");
                        fprintf(extrato, "\nTransferência de %.2f da conta CPF: %d para CPF: %d\n", valor, cpf_origem, cpf_destino);
                        fclose(extrato);
                    } else {
                        printf("\nSaldo insuficiente para realizar a transferência.\n");
                    }
                    return;
                }
            }
            printf("\nConta de destino não encontrada.\n");
            return;
        }
    }

    printf("\nCPF ou senha incorretos.\n");
}

int main() {
    Cliente clientes[MAX_CLIENTES];
    int num_clientes = 0;

    // Carregar clientes do arquivo ao iniciar o programa
    FILE *arquivo;
    arquivo = fopen("Cliente.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo Cliente.txt\n");
        return 1;
    }

    while (fscanf(arquivo, "CPF: %d\nConta: %s\nSaldo: %f\nSenha: %s\n",
                  &clientes[num_clientes].CPF,
                  clientes[num_clientes].tipoConta,
                  &clientes[num_clientes].saldo,
                  clientes[num_clientes].senhaUsuario) != EOF) {
        num_clientes++;
        if (num_clientes >= MAX_CLIENTES) {
            printf("Limite máximo de clientes atingido.\n");
            break;
        }
    }

    fclose(arquivo);

    int opcao;
    do {
        menu();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                novo_cliente(clientes, &num_clientes);
                break;
            case 2:
                apaga_cliente();
                break;
            case 3:
                if (num_clientes > 0) {
                    listar_clientes(clientes, num_clientes);
                } else {
                    printf("Nenhum cliente cadastrado.\n");
                }
                break;
            case 4:
                if (num_clientes > 0) {
                    debitar(clientes, num_clientes);
                } else {
                    printf("Nenhum cliente cadastrado.\n");
                }
                break;
            case 5:
                if (num_clientes > 0) {
                    depositar(clientes, num_clientes);
                } else {
                    printf("Nenhum cliente cadastrado.\n");
                }
                break;
            case 6:
                if (num_clientes > 0) {
                    extrato(clientes, num_clientes);
                } else {
                    printf("Nenhum cliente cadastrado.\n");
                }
                break;
            case 7:
                if (num_clientes > 1) {
                    transferencia(clientes, num_clientes);
                } else {
                    printf("É necessário ter pelo menos duas contas cadastradas para realizar uma transferência.\n");
                }
                break;
            case 8:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 8);
    return 0;
}
