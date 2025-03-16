# README - Biblioteca de Periféricos para Raspberry Pi Pico

Este repositório contém uma coleção de bibliotecas desenvolvidas para facilitar a integração e o controle de periféricos comuns no Raspberry Pi Pico. As bibliotecas foram projetadas para serem simples de usar, eficientes e modulares, permitindo que desenvolvedores foquem na lógica de seus projetos sem se preocupar com detalhes de baixo nível da placa.

## 📦 Bibliotecas Disponíveis

1. **ButtonPi**: Para gerenciamento de botões com suporte a interrupções e debounce.
2. **gpio_irq_manager**: Para gerenciamento modular de interrupções de GPIO.
3. **JoystickPi**: Para leitura de joysticks analógicos e botões.
4. **BuzzerPi**: Para controle de buzzers e geração de tons e melodias.

Abaixo está uma visão geral de cada biblioteca, com links para a documentação detalhada e exemplos de uso.

---

## 1. ButtonPi

A biblioteca **ButtonPi** facilita o gerenciamento de botões no Raspberry Pi Pico. Ela oferece funções para inicializar botões, ler seu estado e configurar callbacks que são executados quando o botão é pressionado. A biblioteca também inclui suporte a debounce para evitar leituras falsas.

### Funcionalidades Principais:
- Inicialização de botões com pull-up interno.
- Leitura do estado do botão.
- Registro de callbacks para eventos de pressionamento.
- Debounce integrado para evitar ruídos.

### Documentação:
- [Documentação detalhada da ButtonPi](https://github.com/Vennominaga/Biblioteca-Raspberry-Pi-Pico/tree/master/Button/Documentation)

### Exemplo de Uso:
```c
#include "ButtonPi.h"

void on_button_press() {
    printf("Botão pressionado!\n");
}

int main() {
    ButtonPi myButton;
    ButtonPi_init(&myButton, 5); // Inicializa o botão no pino 5
    ButtonPi_attach_callback(&myButton, on_button_press); // Registra o callback

    while (1) {
        // Loop principal
    }
}
```

---

## 2. gpio_irq_manager

A biblioteca **gpio_irq_manager** foi criada para encapsular e modularizar o tratamento de interrupções de GPIO no Raspberry Pi Pico. Ela permite registrar e remover callbacks para interrupções em pinos GPIO, com suporte a debounce para evitar leituras falsas.

### Funcionalidades Principais:
- Registro e remoção de callbacks para interrupções de GPIO.
- Debounce integrado para evitar ruídos.
- Inicialização simplificada do sistema de interrupções.



### Exemplo de Uso:
```c
#include "gpio_irq_manager.h"

void on_interrupt() {
    printf("Interrupção detectada!\n");
}

int main() {
    gpio_irq_manager_init(); // Inicializa o gerenciador de interrupções
    register_gpio_callback(5, on_interrupt, GPIO_IRQ_EDGE_FALL); // Registra o callback

    while (1) {
        // Loop principal
    }
}
```

---

## 3. JoystickPi

A biblioteca **JoystickPi** simplifica a leitura de joysticks analógicos e botões no Raspberry Pi Pico. Ela oferece funções para ler os eixos X e Y, detectar o estado do botão e mapear valores analógicos para intervalos personalizados.

### Funcionalidades Principais:
- Inicialização do joystick e configuração dos pinos analógicos e digitais.
- Leitura dos eixos X e Y e do botão.
- Mapeamento de valores analógicos para intervalos personalizados.

  ### Documentação:
- [Documentação detalhada da JoystickPi](https://github.com/Vennominaga/Biblioteca-Raspberry-Pi-Pico/tree/master/Joystick/Documentation)




### Exemplo de Uso:
```c
#include "JoystickPi.h"

int main() {
    joystickPi_init(); // Inicializa o joystick

    while (1) {
        joystick_state_t state = joystickPi_read(); // Lê o estado do joystick
        printf("X: %d, Y: %d, Botão: %d\n", state.x, state.y, state.button);
    }
}
```

---

## 4. BuzzerPi

A biblioteca **BuzzerPi** facilita o controle de buzzers e a geração de tons e melodias no Raspberry Pi Pico. Ela utiliza o módulo PWM para gerar tons em diferentes frequências e permite tocar melodias completas com controle sobre a duração de cada nota.

### Funcionalidades Principais:
- Inicialização do PWM para geração de tons.
- Reprodução de tons individuais com controle de frequência e duração.
- Reprodução de melodias completas.
- Efeitos sonoros como "beeps" repetidos.


 ### Documentação:
- [Documentação detalhada da BuzzerPi](https://github.com/Vennominaga/Biblioteca-Raspberry-Pi-Pico/tree/master/Buzzer/Documentation)



### Exemplo de Uso:
```c
#include "BuzzerPi.h"

int main() {
    initialize_pwm(15); // Configura o pino 15 como saída PWM

    int melody[] = {440, 494, 523}; // Notas Lá, Si, Dó
    int durations[] = {500, 500, 500}; // Durações de 500 ms cada
    play_melody(15, melody, durations, 1.0, 3); // Toca a melodia no pino 15

    return 0;
}
```

---

## Como Usar

1. **Clone o Repositório:**
   ```bash
   git clone https://github.com/Vennominaga/Biblioteca-Raspberry-Pi-Pico
   ```

2. **Inclua as Bibliotecas no Seu Projeto:**
   - Adicione os arquivos `.h` e `.c` das bibliotecas ao seu projeto.
   - Inclua os cabeçalhos necessários no seu código.

3. **Compile e Execute:**
   - Utilize o `CMake` ou `Makefile` para compilar seu projeto com as bibliotecas.

---

## Contribuição

Contribuições são bem-vindas! Se você encontrar algum problema ou tiver sugestões de melhorias, sinta-se à vontade para abrir uma *issue* ou enviar um *pull request*.

---

