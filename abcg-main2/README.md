Projeto 1 - Computação Gráfica - PONG
======
### Autores
----
- Fellip da Silva Ribeiro - RA: 11201921507
- Verônica Agatha Gonçalves Isobe - RA: 11201920292

### Descrição
A aplicação consiste no clássico jogo PONG. Duas barras se movimentam pelo eixo Y (uma ao lado esquerdo da tela e outro do lado direito) rebatendo uma bola de uma lado para o outro. Quando uma barrinha não conseguir rebater a bolinha, o outro jogador ganhara um ponto e aquele que fizer 5 pontos primeiro vence.

### Modos de jogo
O jogo possui 3 modos, apresentados no menu inicial:
- O modo "Player 1 - Right", onde a barrinha da esquerda se torna uma AI. 
- O modo "Player 2 - Left", onde a barrinha da direita se torna uma AI. 
- O modo "Two Playes", onde ambas as barrinhas são controladas pelos jogadores.

### Controles
Tecla ESC - Encerra o programa
Seta para cima - movimenta a barrinha direita para cima
Seta para baixo -  movimenta a barrinha direita para baixo.
Tecla W - movimenta a barrinha esquerda para cima.
Tecla S - movimenta a barrinha esquerda para baixo.

As controles são validas para todos os modos, com a AI assumindo o controle de uma das barrinhas no caso dos modos para um jogador.
As teclas para controle dos players estarão sempre visiveis no topo da janela durante o jogo.

### Código Fonte
O código fonte está dentro da pasta Examples, dentro do diretório ABCg, como "PONG".

### Observação
O compilador WebAssembly não compilou da forma esperada e não redenriza as barrinhas, não consegui corrigir o problema e fui orientado pelo professor a disponibilizar o WebAssembly e o PONG.exe para testes e deixar uma observação no relatório. 
O PON.exe pode ser encontrado em:  abcg / build2 / bin / PONG / PONG.exe

### Link do Github Pages
https://fellip13.github.io/PONG/
