# Genius Game

## 📌 Visão Geral
O **Genius Game** é um desafio interativo de memória que combina estímulos visuais e sonoros para testar suas habilidades cognitivas. Inspirado no clássico jogo de sequências, esta versão moderna utiliza tecnologia embarcada para criar uma experiência imersiva, onde você deve:

- Memorizar padrões luminosos (através de matriz RGB)
- Reconhecer sequências sonoras (via buzzer)
- Reproduzir com precisão as combinações apresentadas

## 🎮 Como Funciona
### Componentes Integrados:
- **Matriz LED RGB**: Gera padrões coloridos dinâmicos
- **Buzzer**: Emite tons musicais sincronizados
- **Display OLED**: Exibe pontuação e feedback visual
- **Joystick/Controles**: Para interação do jogador

### Mecânica Principal:
1. O sistema gera uma sequência aleatória (luzes + sons)
2. O jogador deve reproduzir exatamente a mesma ordem
3. A cada rodada bem-sucedida:
   - A sequência aumenta em 1 item
   - A velocidade de exibição acelera progressivamente
4. O jogo continua até ocorrer um erro, registrando o recorde

## ⚙️ Bibliotecas Desenvolvidas
As seguintes bibliotecas foram **modificadas e otimizadas** a partir de bases existentes:



### Melhorias Implementadas:
✅ Tratamento de eventos simultâneos (luz+som)  
✅ Calibração automática de temporização  
✅ Sistema para detectar inatividade

