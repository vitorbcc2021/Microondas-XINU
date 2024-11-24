#include <xinu.h>

int lerInteiro(void);
void controlarLuz(int ligar);
void tracaoPrato(int girar);

typedef struct {
    int tempo;
    int potencia;
    int portaAberta;
    int luzLigada;
    int pratoGirando;
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

    kprintf("Iniciando cozimento: Tempo = %d segundos, Potência = %d%%.\n", microondas.tempo, microondas.potencia);

    controlarLuz(1);
    tracaoPrato(1);

    int i;
    for (i = 0; i < microondas.tempo; i++) {
        kprintf("Cozinhando... (%d/%d segundos)\n", i + 1, microondas.tempo);
        sleepms(1000);
    }

    controlarLuz(0);
    tracaoPrato(0);
    kprintf("Cozimento concluído!\n");

    microondas.tempo = 0;
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
        case 1:
            especificarCiclo(300, 80);
            break;
        case 2:
            especificarCiclo(200, 70);
            break;
        case 3:
            especificarCiclo(250, 75);
            break;
        case 4:
            especificarCiclo(400, 90);
            break;
        case 5:
            especificarCiclo(120, 100);
            break;
        case 6:
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

    c = getc(stdin);
    if (c == '-') {
        sinal = -1;
        c = getc(stdin);
    }

    while (c >= '0' && c <= '9') {
        valor = valor * 10 + (c - '0');
        c = getc(stdin);
    }

    return valor * sinal;
}

process main() {
    inicializarMicroondas();
    kprintf("Sistema de Microondas Iniciado.\n");

    while (1) {
        programaCozimento();
        kprintf("Deseja realizar outro ciclo? (1 para sim, 0 para não): ");
        int continuar = lerInteiro();
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
