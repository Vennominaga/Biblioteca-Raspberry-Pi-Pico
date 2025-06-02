# 📌 Visão Geral

Este exemplo implementa um jogo de teste de reflexo desenvolvida para a placa bitdoglab,o objetivo principal é medir o tempo de reação do jogador quando um led acende,proporcionando estatísticas de
desempenho ao final de cada rodada.

# 🎮 Funcionamento do Jogo
o Jogo segue a seguinte lógica :

- A inicialização aleatória do led,o led verde acende em um intervalo aleatório dentre (1~4)segundos
  
- Reação do Jogador,o jogador deve pressionar imediamente após o led acender

- Mecânicas do jogo:

✅ Sucesso: Botão pressionado após LED acender (tempo registrado)

⚠️ Falsa partida: Botão pressionado antes do LED acender

❌ Tempo esgotado: Nenhuma ação em 5 segundos após LED acender

⏹ Interrupção: Botão B para encerrar o jogo a qualquer momento



