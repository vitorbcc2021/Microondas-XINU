#include <xinu.h>

// Declaração das funções
int lerInteiro(void);
void controlarLuz(int ligar);
void tracaoPrato(int girar);

typedef struct {
    int tempo;         // Tempo de cozimento em segundos
    int potencia;      // Potência do Klystron (0-100%)
    int portaAberta;   // Status da porta
    int luzLigada;     // Status da luz interna
    int pratoGirando;  // Status do prato giratório
} Microondas;

Microondas microondas;

void inicializarMicroondas() {
    microondas.tempo = 0;
    microondas.potencia = 0;
    microondas.portaAberta = 0;
    microondas.luzLigada = 0;
    microondas.pratoGirando = 0;
}

void controlarKlystron(int potencia) {
    kprintf("Klystron ajustado para %d%% de potência.\n", potencia);
    microondas.potencia = potencia;
}

void especificarCiclo(int tempo, int potencia) {
    microondas.tempo = tempo;
    controlarKlystron(potencia);
    kprintf("Ciclo especificado: Tempo = %d segundos, Potência = %d%%.\n", tempo, potencia);
}

void iniciarCozimento() {
    if (microondas.portaAberta) {
        kprintf("Erro: Porta aberta. Feche a porta para iniciar o cozimento.\n");
        return;
    }

    kprintf("Iniciando cozimento: Tempo = %d segundos, Potência = %d%%.\n",
           microondas.tempo, microondas.potencia);

    controlarLuz(1);       // Liga a luz interna
    tracaoPrato(1);        // Inicia o prato giratório

    int i;
    for (i = 0; i < microondas.tempo; i++) {
        kprintf("Cozinhando... (%d/%d segundos)\n", i + 1, microondas.tempo);
        sleepms(1000);  // Substituindo sleep(1) por sleepms(1000)
    }

    controlarLuz(0);       // Desliga a luz interna
    tracaoPrato(0);        // Para o prato giratório
    kprintf("Cozimento concluído!\n");

    microondas.tempo = 0;  // Reset após o ciclo
}

void programaCozimento() {
    kprintf("Escolha o programa de cozimento:\n");
    kprintf("1. Carne\n");
    kprintf("2. Peixe\n");
    kprintf("3. Frango\n");
    kprintf("4. Lasanha\n");
    kprintf("5. Pipoca\n");
    kprintf("6. Manual\n");
    kprintf("Digite sua escolha: ");
    
    int escolha, tempo, potencia;
    escolha = lerInteiro();

    switch (escolha) {
        case 1: // Carne
            especificarCiclo(300, 80);
            break;
        case 2: // Peixe
            especificarCiclo(200, 70);
            break;
        case 3: // Frango
            especificarCiclo(250, 75);
            break;
        case 4: // Lasanha
            especificarCiclo(400, 90);
            break;
        case 5: // Pipoca
            especificarCiclo(120, 100);
            break;
        case 6: // Manual
            kprintf("Digite o tempo de cozimento (segundos): ");
            tempo = lerInteiro();
            kprintf("Digite a potência do Klystron (0-100%%): ");
            potencia = lerInteiro();
            especificarCiclo(tempo, potencia);
            break;
        default:
            kprintf("Escolha inválida.\n");
            return;
    }

    iniciarCozimento();
}

void controlarLuz(int ligar) {
    microondas.luzLigada = ligar;
    if (ligar) {
        kprintf("Luz interna ligada.\n");
    } else {
        kprintf("Luz interna desligada.\n");
    }
}

void tracaoPrato(int girar) {
    microondas.pratoGirando = girar;
    if (girar) {
        kprintf("Prato girando...\n");
    } else {
        kprintf("Prato parado.\n");
    }
}

int lerInteiro() {
    char c;
    int valor = 0;
    int sinal = 1;

    // Lê o caractere e verifica se é um sinal de negativo
    c = getc(stdin);
    if (c == '-') {
        sinal = -1;
        c = getc(stdin); // Pega o próximo caractere
    }

    // Lê os caracteres numéricos e monta o número
    while (c >= '0' && c <= '9') {
        valor = valor * 10 + (c - '0');
        c = getc(stdin); // Lê o próximo caractere
    }

    return valor * sinal; // Retorna o número com o sinal correto
}

process main() {
    inicializarMicroondas();
    kprintf("Sistema de Microondas Iniciado.\n");

    while (1) {
        programaCozimento(); // Menu interativo
        kprintf("Deseja realizar outro ciclo? (1 para sim, 0 para não): ");
        int continuar = lerInteiro();  // Lê a escolha do usuário
        if (continuar == 0) {
            break;
        } else if (continuar != 1) {
            kprintf("Escolha inválida. Encerrando...\n");
            break;
        }
    }

    kprintf("Sistema de Microondas Encerrado.\n");

    return OK;
}
