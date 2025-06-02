
# Biblioteca BitDogLab para Raspberry Pi Pico

O objetivo desta biblioteca é facilitar o uso de alguns periféricos com a placa Raspberry Pi Pico, desenvolvida pelo BitDogLab. Os periféricos suportados incluem **botão**, **joystick** e **buzzer**.

## Estrutura do Projeto

O código está organizado em pastas, onde cada pasta corresponde a um periférico específico, nomeada de acordo com o componente que representa. Dentro de cada pasta, você encontrará a seguinte estrutura:

### Estrutura de Arquivos
```
bitdoglab-raspberry-pico/
├── botao/
│   ├── documentation/
│   │   
│   │   
│   ├── examples/
│   │   
│   │   
│   
│       
│   
├── joystick/
│   ├── documentation/
│ 
│   ├── examples/
│   │  
│   
├── buzzer/
│   ├── documentation/
│   │  
│   ├── examples/
│  
│  
└── projetos01/
    ├── genius/

└── projeto Final/
    ├── Genius/
```


### Descrição das Pastas

- **Documentation**: Contém arquivos PDF associados às bibliotecas utilizadas para a criação desta biblioteca, além de exemplos de uso, estruturas de dados e links importantes para um melhor entendimento.
- **Examples**: Inclui exemplos de uso que variam desde aplicações simples (como acender um LED via interrupção) até projetos mais complexos. Cada periférico possui exemplos específicos, como:
  - **Botão**: Jogo de tempo e reação.
  - **Joystick**: Gráfico da tartaruga (turtle graphics).
  - **Buzzer**: Exemplos de reprodução de sons e melodias.
- **src**: Contém os arquivos fonte (`.c`)  da biblioteca
- **inc**: Contém os arquivos de cabeçalhos (`.h`) da biblioteca.

## Projetos

### Projeto Etapa 1
Na pasta **projetos01**, você encontrará projetos que combinam várias bibliotecas para desenvolver aplicações completas. Um exemplo é o **jogo do Genius** (jogo das cores), que utiliza todas as bibliotecas desenvolvidas até o momento.

### Projeto Etapa Final

Na Pasta **Projeto Final**,você encontrará um projeto final que foi finalizado com base nas bibliotecas desenvolvidas e na implementação das bibliotecas de outros projetistas que foram compartilhadas como parte da atividade

[Link da Biblioteca que Foi Compartilhada](https://github.com/MmonkeyBu/Joystick-Matriz-Painel.git)


## Como Usar

1. **Leia a documentação**: Antes de começar, é importante entender como cada periférico funciona e como a biblioteca foi estruturada.
2. **Clone o repositório**: Faça um `git clone` do projeto para o seu ambiente local.
   ```bash
   git clone https://github.com/Vennominaga/Biblioteca-Raspberry-Pi-Pico
   ```
4. **Inclua os arquivos necessários**: Adicione todos os cabeçalhos (`.h`) e códigos fonte (`.c`) ao seu projeto original.
5. **Declarações e inclusões**: Faça as declarações necessárias no seu código, incluindo os cabeçalhos das bibliotecas.
6. **Configuração do CMake**: Para incluir bibliotecas externas, adicione ao seu `CMakeLists.txt`:
    por exemplo para adicionar a Biblioteca BuzzerPi localizada na pasta src
   ```cmake
   add_executable(play_music_example01 play_music_example01.c src/BuzzerPi.c)
   ```
7. **Compile o projeto**: Após configurar o CMake, compile o projeto e comece a utilizar as bibliotecas.

## Seja Criativo!

Utilize os recursos disponíveis para criar projetos inovadores. A biblioteca foi projetada para ser flexível e permitir a implementação de ideias criativas. Sinta-se à vontade para sugerir novos exemplos ou melhorias.

## Contribuições

Se encontrar erros ou tiver sugestões, por favor, compartilhe! Contribuições são sempre bem-vindas. Abra uma issue ou envie um pull request para ajudar a melhorar a biblioteca.!

---

