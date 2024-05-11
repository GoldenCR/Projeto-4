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
    printf("8. Operação livre\n");
    printf("9. Sair\n");
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

void listar_clientes() {
    system("cls || clear");
    printf("Lista de clientes: \n");

    FILE *arquivo;
    arquivo = fopen("Cliente.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    int CPF;
    char tipoConta[10];
    float saldo;
    char senhaUsuario[50];

    while (fscanf(arquivo, "CPF: %d\nConta: %s\nSaldo: %f\nSenha: %s\n", &CPF, tipoConta, &saldo, senhaUsuario) != EOF) {
        printf("CPF: %d\nConta: %s\nSaldo: %.2f\nSenha: %s\n\n", CPF, tipoConta, saldo, senhaUsuario);
    }

    fclose(arquivo);
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



int main() {
    Cliente clientes[MAX_CLIENTES]; 
    int num_clientes = 0; 

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
                listar_clientes();
                break;
            case 4:
                if (num_clientes > 0) {
                    debitar(clientes, num_clientes);
                } else {
                    printf("Nenhum cliente cadastrado.\n");
                }
                break;
            case 9:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 9);
    return 0;
}