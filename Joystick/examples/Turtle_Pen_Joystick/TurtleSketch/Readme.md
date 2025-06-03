# 📌 Visão Geral  

**TurtleSketch** é um programa que simula uma *tartaruga gráfica* controlada por um joystick. Ele permite que o usuário mova um cursor (representando uma "tartaruga") em uma grade 10×10 e decida quando deixar um rastro no caminho, criando desenhos simples.  

##  Funcionalidades Principais 

✅ **Movimento Controlado por Joystick**  
   - **Eixo X (Horizontal)**: Move para **esquerda** ou **direita**.  
   - **Eixo Y (Vertical)**: Move para **cima** ou **baixo**.  
   - **Botão do Joystick**: Liga/desliga a **caneta** (se está desenhando ou não).  

✅ **Sistema de Desenho**  
   - Quando a caneta está ativa (`pen_down = true`), a tartaruga deixa um rastro (`*`) na matriz.  
   - Quando levantada (`pen_down = false`), ela se move sem desenhar.  

✅ **Visualização em Tempo Real**  
   - A matriz é exibida com **bordas estilizadas** (usando caracteres Unicode como `┌`, `─`, `│`, etc.).  
   - Mostra a **posição atual** da tartaruga e o **estado da caneta**.  

 
### 🖥️ Saída no Terminal  
O programa limpa e atualiza a tela constantemente, mostrando:  
- A **grade 10×10** com o desenho atual.  
- A **posição (X, Y)** da tartaruga.  
- Se a caneta está **ativa ou não**.  
