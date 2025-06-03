# 📌 Visão Geral

Este exemplo consiste em um loop infinito que:
1. Lê continuamente o valor analógico do joystick (valores de 0 a 4096)
2. Verifica o estado do botão do joystick (0 ou 1, indicando não pressionado ou pressionado)
3. Envia uma mensagem via UART contendo:
   - O valor analógico em bits (faixa de 0 a 4096)
   - O estado do botão (0 ou 1)
