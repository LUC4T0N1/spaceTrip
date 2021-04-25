O código do projeto foi desenvolvido a partir da ABCg, portanto a base da estrutura foi obtida a partir de https://github.com/hbatagelo/abcg.git
O código referente apenas ao desenvolvimento do projeto pode ser encontrado na pasta examples. Todo o código desenvolvido está na pasta atividade_3.

O projeto representa um jogo 3D desenvolvido com ABCg, cujos gráficos são renderizados com o pipeline do OpenGL usando shaders.
O jogo é em primeiro pessoa, e simboliza uma nave que está passando por túneis com diversos obstáculos, cujo piloto deve desviar para avançar pelos túneis.
O jogo possui um total de 4 fases, sendo que cada fase é representada por um túnel. Quando chegar ao final de uma fase, o jogador é automaticamente levado para a próxima fase.
Cada túnel possui uma coloração, e conforme o jogo vai progredindo,  os túneis ficam maiores e com mais obstáculos, além da velocidade da nave aumentar consideravelmente.
Caso o jogador colida com algum obstáculo, o jogo reinicia.
Caso o jogador finalize as 4 fases o jogo reinicia.
O jogo é inteiramente controlado pelo teclado.

Comandos:
  - P : a tecla P pausa o jogo. Caso ele esteja pausado, a tecla P irá retomar o jogo de onde ele parou.
  - R : a tecla R reinicia o jogo, voltando a nae para o estado inicial.
  - W ou Seta Para Cima:  a tecla W ou a seta para cima movem a nave para cima.
  - S ou Seta Para Baixo:  a tecla S ou a seta para baixo movem a nave para baixo.
  - A ou Seta Para Esquerda:  a tecla a ou a seta para esquerda movem a nave para a esquerda.
  - D ou Seta Para Direita:  a tecla D ou a seta para direita movem a nave para a direita.





OBS: Caso o computador do jogador não consiga rodar o jogo com boa performance, o jogo pode apresentar alguns bugs de colisão com as paredes dos túneis.
     A textura utilizada nas paredes encontra-se em: https://i.pinimg.com/originals/cf/8e/f0/cf8ef0e8ea9e40cc32fbfc3c83b8e4c1.jpg
     
     
     
Nome do Desenvolvedor: Lucas Moniz de Arruda.
RA: 11201721285
