#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>

#define MAX_QUARTOS 10
#define MAX_RESERVAS 10
#define MAX_USUARIOS 100
#define SUITE 0
#define QUARTO_LUXO 1
#define QUARTO_STANDARD 2


// Definição de estruturas
typedef struct {
    int numero;
    int tipo;  // Indica o tipo de quarto (exemplo: 1 - suíte, 2 - luxo, 3 - padrão)
    bool disponivel;
    float preco; // Preço do quarto
} Quarto;

typedef struct {
    int tipo;
    float valorBase; // Valor base para esse tipo de quarto
} TipoQuarto;

typedef struct {
    int numero;
    TipoQuarto tipoQuarto;
} InfoQuarto;

typedef struct {
    int id_reserva;
    char id_usuario[15];
    int numero_quarto;
    char data_check_in[20];
    char data_check_out[20];
    bool ativa;
    bool paga;
    float valor_total;
} Reserva;

typedef struct {
    char nome[50];
    char cpf[15];
} Usuario;

Usuario listaUsuarios[MAX_USUARIOS]; // Array para armazenar usuários
int contadorUsuarios = 0; // Contador de usuários cadastrados


int calcularDiferencaDias(const char dataInicio[], const char dataFim[]) {
    int diaInicio, mesInicio, anoInicio;
    int diaFim, mesFim, anoFim;

    sscanf(dataInicio, "%d/%d/%d", &diaInicio, &mesInicio, &anoInicio);
    sscanf(dataFim, "%d/%d/%d", &diaFim, &mesFim, &anoFim);

    int diasJulianosInicio = anoInicio * 365 + mesInicio * 30 + diaInicio;
    int diasJulianosFim = anoFim * 365 + mesFim * 30 + diaFim;

    return diasJulianosFim - diasJulianosInicio;
}

float calcularValorEstadia(Quarto quarto, Reserva reserva) {
    // Lógica para cálculo do valor total com base no período e tipo de quarto
    // Exemplo básico: Valor total = preço do quarto * número de dias
    float valorTotal = quarto.preco; // Valor base

    // Lógica para calcular o número de dias entre check-in e check-out
    // Exemplo: considera-se dias corridos, sem levar em conta horas exatas
    int diasEstadia = calcularDiferencaDias(reserva.data_check_in, reserva.data_check_out);

    // Calcula o valor total da estadia
    valorTotal *= diasEstadia;

    return valorTotal;
}

Usuario cadastrarNovoUsuario(Usuario usuarios[], int *contadorUsuarios) {
    Usuario novoUsuario;

    printf("Digite o nome do novo usuário: ");
    scanf("%s", novoUsuario.nome);
    printf("Digite o CPF do novo usuário: ");
    scanf("%s", novoUsuario.cpf);

    // Verifica se o CPF já está cadastrado
    for (int i = 0; i < *contadorUsuarios; i++) {
        if (strcmp(usuarios[i].cpf, novoUsuario.cpf) == 0) {
            printf("CPF já cadastrado. Operação cancelada.\n");
            return novoUsuario; // Retorna usuário sem adicionar, já que CPF já existe
        }
    }

    if (*contadorUsuarios < MAX_USUARIOS) {
        usuarios[*contadorUsuarios] = novoUsuario;
        (*contadorUsuarios)++;
        printf("Novo usuário cadastrado com sucesso!\n");
    } else {
        printf("Limite de usuários atingido. Não foi possível cadastrar o novo usuário.\n");
    }

    return novoUsuario;
}

// Função para login
Usuario fazerLogin(Usuario usuarios[], int contadorUsuarios) {
    Usuario user;
    printf("Digite seu nome: ");
    scanf("%s", user.nome);
    printf("Digite seu CPF: ");
    scanf("%s", user.cpf);

    for (int i = 0; i < contadorUsuarios; i++) {
        if (strcmp(user.nome, usuarios[i].nome) == 0 && strcmp(user.cpf, usuarios[i].cpf) == 0) {
            printf("Login realizado com sucesso para %s\n", user.nome);
            return user; // Usuário encontrado
        }
    }

    printf("Usuário não cadastrado. Realize o cadastro primeiro!\n");
    // Aqui você pode adicionar a lógica para redirecionar o usuário para o cadastro
    // ou qualquer outra ação apropriada para lidar com o usuário não cadastrado.
    return user;
}

// Função para listar quartos disponíveis
void listarQuartos(Quarto quartos[], int tamanho) {
    printf("Quartos disponíveis:\n");

    for (int i = 0; i < tamanho; i++) {
        if (quartos[i].disponivel) {
            printf("Quarto %d\n", quartos[i].numero);

        }
    }
}

// Função para fazer reserva

void fazerReserva(Reserva reservas[], int *contadorReservas, Usuario user, Quarto quartos[], int tamanhoQuartos) {
    int quartoSelecionado;
    char cpf[15];
    char data_check_in[20];
    char data_check_out[20];

    printf("Digite o número do quarto desejado: ");
    scanf("%d", &quartoSelecionado);


    for (int i = 0; i < tamanhoQuartos; i++) {
        if (quartos[i].numero == quartoSelecionado && quartos[i].disponivel) {
            printf("Digite o CPF: ");
            scanf("%s", cpf);

            if (strlen(cpf) != 11) {
                printf("CPF inválido. Insira um CPF válido.\n");
                return;
            }

            printf("Digite a data de check-in (DD/MM/AAAA): ");
            scanf("%s", data_check_in);
            printf("Digite a data de check-out (DD/MM/AAAA): ");
            scanf("%s", data_check_out);

            strcpy(reservas[*contadorReservas].id_usuario, cpf);
            reservas[*contadorReservas].numero_quarto = quartoSelecionado;
            strcpy(reservas[*contadorReservas].data_check_in, data_check_in);
            strcpy(reservas[*contadorReservas].data_check_out, data_check_out);
            reservas[*contadorReservas].id_reserva = *contadorReservas + 1; // Simulação de ID de reserva
            reservas[*contadorReservas].ativa = true;
            reservas[*contadorReservas].paga = false;
            quartos[i].disponivel = false;
            (*contadorReservas)++;
            printf("Reserva realizada com sucesso!\n");
            return;
        }
    }

    printf("Quarto não disponível ou inexistente.\n");
}


void listarReservasUsuario(Reserva reservas[], int contadorReservas, char idUsuario[15]) {
    printf("Reservas do usuário ID %s:\n", idUsuario);
    bool encontrouReserva = false;

    for (int i = 0; i < contadorReservas; i++) {
        if (strcmp(reservas[i].id_usuario, idUsuario) == 0) {
            printf("Reserva ID: %d, Quarto: %d, Check-in: %s, Check-out: %s\n", reservas[i].id_reserva, reservas[i].numero_quarto, reservas[i].data_check_in, reservas[i].data_check_out);
            encontrouReserva = true;
        }
    }

    if (!encontrouReserva) {
        printf("Nenhuma reserva encontrada para este usuário.\n");
    }
}

InfoQuarto encontrarInfoQuarto(Quarto quartos[], int tamanhoQuartos, int numeroQuarto) {
    InfoQuarto info;

    for (int i = 0; i < tamanhoQuartos; i++) {
        if (quartos[i].numero == numeroQuarto) {
            info.numero = quartos[i].numero;
            info.tipoQuarto.tipo = quartos[i].tipo;
            // Suponha que aqui haja uma tabela de tipos de quarto com seus valores base
            // para recuperar o valor base do tipo de quarto
            // Exemplo simples: valor base de uma suíte é 200.0, luxo é 150.0, padrão é 100.0
            switch (info.tipoQuarto.tipo) {
                case 1: // Suíte
                    info.tipoQuarto.valorBase = 200.0;
                    break;
                case 2: // Luxo
                    info.tipoQuarto.valorBase = 150.0;
                    break;
                case 3: // Padrão
                    info.tipoQuarto.valorBase = 100.0;
                    break;
                default:
                    info.tipoQuarto.valorBase = 100.0; // Padrão se não for encontrado
            }
            break;
        }
    }

    return info;
}

// Função para calcular o valor base do tipo de quarto
float calcularValorBaseTipoQuarto(int tipo) {
    switch (tipo) {
        case 1: // Suíte
            return 200.0;
        case 2: // Luxo
            return 150.0;
        case 3: // Padrão
            return 100.0;
        default:
            return 100.0; // Padrão se não for encontrado
    }
}

// Função para calcular o valor total da reserva
float calcularValorReserva(Reserva reserva, Quarto quartos[], int tamanhoQuartos) {
    InfoQuarto infoQuarto = encontrarInfoQuarto(quartos, tamanhoQuartos, reserva.numero_quarto);
    int diferencaDias = calcularDiferencaDias(reserva.data_check_in, reserva.data_check_out);
    float valorTotal = infoQuarto.tipoQuarto.valorBase * diferencaDias;

    return valorTotal;
}


void realizarPagamento(Reserva reservas[], int contadorReservas, Quarto quartos[], int tamanhoQuartos) {
    int idReserva;
    printf("Digite o ID da reserva a pagar: ");
    scanf("%d", &idReserva);

    for (int i = 0; i < contadorReservas; i++) {
        if (reservas[i].id_reserva == idReserva && reservas[i].ativa && !reservas[i].paga) {
            printf("Reserva ID: %d, Quarto: %d\n", reservas[i].id_reserva, reservas[i].numero_quarto);

            // Encontrar o tipo do quarto associado à reserva
            int numeroQuartoReserva = reservas[i].numero_quarto;
            int tipoQuartoReserva = -1; // Inicializa como valor inválido

            for (int j = 0; j < tamanhoQuartos; j++) {
                if (quartos[j].numero == numeroQuartoReserva) {
                    tipoQuartoReserva = quartos[j].tipo;
                    quartos[j].disponivel = true;
                    break;
                }
            }

            if (tipoQuartoReserva != -1) {
                // Cálculo do valor base do tipo de quarto
                float valorBaseTipoQuarto = calcularValorBaseTipoQuarto(tipoQuartoReserva);

                // Cálculo do valor total da reserva
                int diferencaDias = calcularDiferencaDias(reservas[i].data_check_in, reservas[i].data_check_out);
                float valorReserva = valorBaseTipoQuarto * diferencaDias;

                printf("Valor a pagar: R$%.2f\n", valorReserva);

                // Lógica para marcar a reserva como paga
                reservas[i].paga = true;
                reservas[i].valor_total = valorReserva;
                printf("Pagamento realizado com sucesso!\n");
                return;
            } else {
                printf("Tipo de quarto associado à reserva não encontrado.\n");
                return;
            }
        }
    }

    printf("Reserva não encontrada ou pagamento já efetuado.\n");
}


void alterarReserva(Reserva reservas[], int contadorReservas, Usuario user) {
    int idReserva;
    printf("Digite o ID da reserva a ser alterada: ");
    scanf("%d", &idReserva);

    for (int i = 0; i < contadorReservas; i++) {
        if (reservas[i].id_reserva == idReserva && strcmp(reservas[i].id_usuario, user.cpf)==0 && reservas[i].ativa && !reservas[i].paga) {
            printf("Reserva ID: %d, Quarto: %d\n", reservas[i].id_reserva, reservas[i].numero_quarto);
            printf("Digite a nova data de check-in (DD/MM/AAAA): ");
            scanf("%s", reservas[i].data_check_in);
            printf("Digite a nova data de check-out (DD/MM/AAAA): ");
            scanf("%s", reservas[i].data_check_out);
            printf("Reserva alterada com sucesso!\n");
            return;
        }
    }

    printf("Reserva não encontrada ou não pode ser alterada.\n");
}

void gerenciarEstadia(Reserva reservas[], int contadorReservas) {
    int idReserva;
    printf("Digite o ID da reserva para gerenciar a estadia: ");
    scanf("%d", &idReserva);

    for (int i = 0; i < contadorReservas; i++) {
        if (reservas[i].id_reserva == idReserva) {
            char novaDataCheckIn[20];
            char novaDataCheckOut[20];

            printf("Digite a nova data de check-in (DD/MM/AAAA): ");
            scanf("%s", novaDataCheckIn);
            printf("Digite a nova data de check-out (DD/MM/AAAA): ");
            scanf("%s", novaDataCheckOut);

            // Atualiza as datas de check-in e check-out
            strcpy(reservas[i].data_check_in, novaDataCheckIn);
            strcpy(reservas[i].data_check_out, novaDataCheckOut);

            printf("Estadia atualizada com sucesso!\n");
            return;
        }
    }
    printf("Reserva não encontrada.\n");
}

void gerarFatura(Reserva reservas[], int contadorReservas, Quarto quartos[], int tamanhoQuartos) {
    int idReserva;
    printf("Digite o ID da reserva para gerar a fatura: ");
    scanf("%d", &idReserva);

    for (int i = 0; i < contadorReservas; i++) {
        if (reservas[i].id_reserva == idReserva && reservas[i].ativa && reservas[i].paga) {
            printf("Reserva ID: %d, Quarto: %d\n", reservas[i].id_reserva, reservas[i].numero_quarto);

            int numeroQuartoReserva = reservas[i].numero_quarto;
            float totalPagar = 0.0;

            for (int j = 0; j < tamanhoQuartos; j++) {
                if (quartos[j].numero == numeroQuartoReserva) {
                    totalPagar = calcularValorReserva(reservas[i], quartos, tamanhoQuartos);
                    printf("Fatura gerada com sucesso. Valor recebido: R$ %.2f\n", totalPagar);
                    return;
                }
            }
        }
    }

    printf("Reserva não encontrada ou não paga.\n");
}

void realizarCheckOut(Reserva reservas[], int contadorReservas, Quarto quartos[], int tamanhoQuartos) {
    int idReserva;
    printf("Digite o ID da reserva para realizar o check-out: ");
    scanf("%d", &idReserva);

    for (int i = 0; i < contadorReservas; i++) {
        if (reservas[i].id_reserva == idReserva && reservas[i].ativa && reservas[i].paga) {
            printf("Reserva ID: %d, Quarto: %d\n", reservas[i].id_reserva, reservas[i].numero_quarto);

            int numeroQuartoReserva = reservas[i].numero_quarto;

            for (int j = 0; j < tamanhoQuartos; j++) {
                if (quartos[j].numero == numeroQuartoReserva) {
                    // Atualizar status para disponível
                    quartos[j].disponivel = true;
                    reservas[i].ativa = false;
                    printf("Check-out realizado com sucesso. Quarto agora está disponível.\n");
                    return;
                }
            }
        }
    }

    printf("Reserva não encontrada ou não paga.\n");
}


void registrarCheckIn(Reserva reservas[], int contadorReservas) {
    int idReserva;
    printf("Digite o ID da reserva para registrar o check-in: ");
    scanf("%d", &idReserva);

    for (int i = 0; i < contadorReservas; i++) {
        if (reservas[i].id_reserva == idReserva) {
            reservas[i].ativa = true;  // Registra o check-in marcando a reserva como ativa
            printf("Check-in registrado com sucesso!\n");
            return;
        }
    }
    printf("Reserva não encontrada.\n");
}

void configurarTarifas(Quarto quartos[], int tamanhoQuartos) {
    // Definição de preços base para cada tipo de quarto
    quartos[SUITE].preco = 200.0;
    quartos[QUARTO_LUXO].preco = 150.0;
    quartos[QUARTO_STANDARD].preco = 100.0;

    // Lógica adicional para permitir ajuste de tarifas para diferentes épocas ou eventos
    // Exemplo: entrada manual de preços ou atualização baseada em eventos
    printf("Tarifas configuradas com sucesso!\n");
}

void exibirDisponibilidadeQuartos(Quarto quartos[], int tamanhoQuartos) {
    printf("Disponibilidade de Quartos:\n");
    for (int i = 0; i < tamanhoQuartos; i++) {
        printf("Quarto %d - ", i + 1);
        if (quartos[i].disponivel) {
            printf("Disponível - Preço: R$ %.2f\n", quartos[i].preco);
        } else {
            printf("Ocupado\n");
        }
    }
}


void relatorioDesempenhoReservas(Reserva reservas[], int contadorReservas) {
    printf("Relatório de Desempenho das Reservas:\n");
    printf("Número total de reservas: %d\n", contadorReservas);

    // Lógica adicional para relatórios específicos (por período, tipo de quarto, etc.)
    // Aqui, poderia ser feita a análise de tendências, por exemplo
}

void relatorioOcupacaoQuartos(Quarto quartos[], int tamanhoQuartos) {
    printf("Relatório de Ocupação dos Quartos:\n");
    int quartosOcupados = 0;

    for (int i = 0; i < tamanhoQuartos; i++) {
        if (!quartos[i].disponivel) {
            printf("Quarto %d: Ocupado\n", quartos[i].numero);
            quartosOcupados++;
        }
    }

    int quartosDisponiveis = tamanhoQuartos - quartosOcupados;
    printf("Total de quartos: %d\nQuartos ocupados: %d\nQuartos disponíveis: %d\n", tamanhoQuartos, quartosOcupados, quartosDisponiveis);
}

void relatorioReceitasGeradas(Reserva reservas[], int contadorReservas) {
    printf("Relatório de Receitas Geradas:\n");
    float receitaTotal = 0.0;

    for (int i = 0; i < contadorReservas; i++) {
        if (reservas[i].paga) {
            receitaTotal += reservas[i].valor_total;
        }
    }

    printf("Receita total gerada: R$ %.2f\n", receitaTotal);
}


// Função principal
int main() {
    setlocale(LC_ALL, "Portuguese");
    Usuario usuarios[MAX_USUARIOS];
    int contadorUsuarios = 0;

    Reserva reservas[MAX_RESERVAS];
    int contadorReservas = 0;

    Quarto quartos[MAX_QUARTOS]; // Array de quartos (simulado)
    int contadorTiposQuarto = 3; // Considerando 3 tipos de quarto no exemplo

    // Inicialização dos quartos (simulado)
    for (int i = 0; i < MAX_QUARTOS; i++) {
        quartos[i].numero = i + 1;
        quartos[i].disponivel = true;
        // Definição de tipos e preços (exemplo genérico)
        if (i < MAX_QUARTOS / 3) {
            quartos[i].tipo = 1; // Suíte
            quartos[i].preco = 200.0;
        } else if (i < (MAX_QUARTOS / 3) * 2) {
            quartos[i].tipo = 2; // Luxo
            quartos[i].preco = 150.0;
        } else {
            quartos[i].tipo = 3; // Padrão
            quartos[i].preco = 100.0;
        }
    }

    int opcao;
    Usuario usuarioAtual;

    while (1) {
        printf("==================BEM VINDO AO NOSSO HOTEL=================================================================\n");
        printf(" TIPOS DE QUARTOS\n QUARTO 1: SUÍTE R$200,00\n QUARTO 2: LUXO R$150,00\n QUARTOS 3 AO 10: PADRÕES R$100,00\n");
        printf("\n1. Login\n2. Listar quartos disponíveis\n3. Fazer reserva\n4. Listar reservas do usuário\n5. Pagar reserva\n6. Alterar reserva\n7. Gerar fatura\n8. Relatório de desempenho das reservas\n9. Relatório de ocupação dos quartos\n10. Relatório de receitas geradas\n11. Cadastrar novo usuário\n12. Sair\nEscolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                usuarioAtual = fazerLogin(usuarios, contadorUsuarios);
                break;
            case 2:
                listarQuartos(quartos, MAX_QUARTOS);
                break;
            case 3:
                if (usuarioAtual.nome[0] != '\0') {
                    fazerReserva(reservas, &contadorReservas, usuarioAtual, quartos, MAX_QUARTOS);
                } else {
                    printf("Faça login primeiro!\n");
                }
                break;
            case 4:
                if (usuarioAtual.nome[0] != '\0') {
                    listarReservasUsuario(reservas, contadorReservas, usuarioAtual.cpf);
                } else {
                    printf("Faça login primeiro!\n");
                }
                break;
            case 5:
                if (usuarioAtual.nome[0] != '\0') {
                    realizarPagamento(reservas, contadorReservas, quartos, MAX_QUARTOS);
                } else {
                    printf("Faça login primeiro!\n");
                }
                break;
            case 6:
                if (usuarioAtual.nome[0] != '\0') {
                    alterarReserva(reservas, contadorReservas, usuarioAtual);
                } else {
                    printf("Faça login primeiro!\n");
                }
                break;
            case 7:
                gerarFatura(reservas, contadorReservas, quartos, MAX_QUARTOS);
                break;
            case 8:
                relatorioDesempenhoReservas(reservas, contadorReservas);
                break;
            case 9:
                relatorioOcupacaoQuartos(quartos, MAX_QUARTOS);
                break;
            case 10:
                relatorioReceitasGeradas(reservas, contadorReservas);
                break;
            case 11:
                if (contadorUsuarios < MAX_USUARIOS) {
                    Usuario novoUsuario = cadastrarNovoUsuario(usuarios, &contadorUsuarios);
                    if (strcmp(novoUsuario.nome, "") != 0) {
                        usuarios[contadorUsuarios - 1] = novoUsuario;
                    }
                } else {
                    printf("Limite de usuários atingido. Não foi possível cadastrar o novo usuário.\n");
                }
                break;
            case 12:
                printf("Saindo do sistema. Obrigado!\n");
                return 0;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}
