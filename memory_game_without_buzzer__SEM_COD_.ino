/*
  JOGO DA MEMÓRIA
  NEEC IST
*/

?  ?   ? //Controla os LED's
?  ?   ?//Check dos Botões
?  ?   ?
#define ?   ?
#define ?   ?
#define ?   ?

//Pinos de saída do arduino para os Led's
?  ?  ?
?  ?  ?
?  ?  ?
#define ?  ?

//Pinos de saída do arduino para os botões
?  ?   ?
?  ?   ?
?  ?   ?
#define ?   ? 

// Número de rondas que é preciso fazer com sucesso antes de ganhar.
?    ?     ? 

// Tempo disponível para pressionar um botão antes que o jogo dê time out. 3000ms = 3 segundos
?  ?   ? 


#define ?  ?
#define ?  ?

// Variáveis de estado do jogo
?   ?   ?  // Por default, vamos jogar o jogo de memória
?   ?   ? // Contém a combinação de botões à medida que avançamos
?   ?   ? // Conta o número de rondas bem-sucedidas que o jogador completou

void setup()
{
  // Configuração das entradas/saídas de hardware. Estes pinos estão definidos na função hardware_versions
  // Ativar resistencias de pull-up nas entradas
?
?
?
?
?
?
?
?
  // Verificação do modo de jogo
 ?  ?  ? // Por default, vamos jogar o jogo de memória

  // Verificar se o botão superior direito está pressionado aquando o arduino é iniciado
  // Se isso acontecer, estamos a colocar o jogo em modo batalha
  ? ? ?
  
    ?   ?   ? // Colocar o jogo no modo batalha
    // Ligar o LED superior direito (vermelho)
    ?
    ?
    ? // Ligar os outros LEDs até que o botão seja solto

    ?   ?   ? // Aguardar que o jogador pare de pressionar o botão
    // Foi escolhido o modo batalha
  
 ?
}

void loop()
{
 ? // Piscar as luzes enquanto esperamos o jogador pressionar um botão

  // Indicar o início do jogo
  ? // Ligar todos os LEDs
  ?
  ? // Desligar os LEDs
  ?

  ?   ?   
    
    // Jogar o jogo de memória e lidar com o resultado
    ?
      ? // O jogador venceu, tocar os tons de vitória
      ?
    
    ?
     ? // O jogador perdeu, tocar os tons de derrota
      //?
  

  ?
  
   ? // Jogar no modo batalha até que alguém perca
    ? // O jogador perdeu, tocar os tons de derrota
    //?
  
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//As funções a seguir estão relacionadas apenas ao jogo


// --------------MODO JOGO DA MEMÓRIA DEFAULT------------------
// Retorna 0 se o jogador perder, ou 1 se o jogador vencer


? ?
{
  ? //função random a funcionar em milissegundos

  ?   ? // Reset do jogo para o início

  ?   ?
  {
   ? // Adiciona um botão aos movimentos atuais e reproduz
    ? // Reproduz a sequência do jogo atual

    // Em seguida, exige que o jogador repita a sequência.
    ?   ?   ?   ?   ?
    {
      ?   ?   ?// Verifica qual botão foi pressionado
      ?   ?   ?   ? // Se o tempo de espera acabar, o jogador perde
      ?   ?   ?   ?// Se a escolha estiver incorreta, o jogador perde
    }
    ? // O jogador acertou, aguarda antes de reproduzir os movimentos
  }
  ?   ? // O jogador passou por todas as rondas e venceu!
}

//--------------MODO JOGO DA MEMÓRIA BATALHA------------------
// Jogar o modo batalha especial para 2 jogadores
// Um jogador começa pressionando um botão e passando-o para o outro jogador
// Esse jogador repete o botão e adiciona um e devolve ao outro.

// Esta função dá return quando alguém perde
?   ?
{
  ?   ? // Reset do jogo para a primeira ronda

  ?   ? // Loop até que alguém falhe
  {
    ?   ?   ? // Aguarda o jogador realize o próximo movimento
   ?    ? // Adiciona este novo botão ao array do jogo

    // Em seguida, exige que o jogador repita a sequência.
   ?    ?   ?   ?
    {
      ?   ?   ?
      ?   ?   ?   ? // Se o tempo de espera acabar, o jogador perde.
      ?   ?   ?   ? // Se a escolha estiver incorreta, o jogador perde.
    }
    ? // Dá ao jogador 100ms extras para passar o jogo para o outro jogador
  }
  ?   ? // Nunca devemos chegar aqui porque não existe limite de jogadas, apenas acaba quando alguém perder
}

// Reproduz o conteúdo atual dos movimentos do jogo
?   ?
{
  ?   ?   ?   ? 
  {
    ?
    // Aguarda algum tempo entre as reproduções dos botões
    // Se quiseres tornar o jogo mais difícil, diminui o tempo
    ? // 150 funciona bem. 75 fica rápido.
  }
}

// Adiciona um novo botão aleatório à sequência do jogo
?   ?
{
  // Random de números de 0 a 3
  ?   ?   ? //min (incluído), max (excluído)

  // Temos que converter esse número, de 0 a 3, para CHOICEs
  ?     ?
  ?     ?     ?
  ?     ?     ?
  ?     ?     ?
  ?     ? // Adiciona este novo botão ao array do jogo
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// As funções a seguir controlam o hardware
// Acende os LEDs específicos
// Recebe um byte composto por CHOICE_2ND_RED, CHOICE_GREEN, etc.
?  ?  ?  ?
{
  ? ?
    ? ?
  ?
    ? ?
  ?  
    ? ?
  ?
    ? ?
  ?
    ? ?
  ?
    ?
  ?  ?
    ?
  ?
    ?
}

// Aguarda até que um botão seja pressionado.
// Return de uma das cores dos LEDs (LED_2ND_RED, etc.) se bem-sucedido, '0' se o tempo esgotar

?   ?
{
  ?   ? // Armazena o tempo em que iniciamos este loop

  ?   ? // Loop até que muito tempo tenha passado
  
    ?   ?
    ?   ?
      ?
     ?  ? // Agora espera o jogador soltar o botão
      ? // Isto ajuda a evitar o bounce e toques duplos acidentais
      ?   ?
  
  ?   ? // Se chegarmos aqui, o tempo acabou!
}


// Função que liga os led's

?   ?
{
  ? //Mostra o led dado

  ?   ? 
  
  ?   ?
    ?
    ?
 ?    ?
    ?
    ?
 ?    ?
   ?
   ?
  ?   ?
    ?
    ?

 ? //Desliga todos os led's
}

// Gere o tempo de mostragem dos led's
?   ?
{
  // converter milissegundos para microssegundos
 ?    ?

  // Loop que vai diminuindo o tempo até ser menos que um delay_us
  ?   ?
    ? //Diminui o tempo de jogo

    ?
    ?
}

// Retorna um bit '1' na posição correspondente a CHOICE_2ND_RED, CHOICE_1ST_RED, etc.
?   ?
{
  ?   ?   ?   ?
  ?   ?   ?   ?  
  ?    ?   ?   ?
  ?   ?   ?   ?
  ?   ? // Se nenhum botão for pressionado, retorna CHOICE_NONE
}


// Acende as luzes do vencedor

?   ?
{
  ?
  ?
  ?
  ?
}

// Mostra as luzes do perdedor
?   ?
{
 ?
 ?
 ?
 ?
 ?
 ?
 ?
 ?
}

// Mostra uma exibição do "modo de atração" enquanto espera que o jogador pressione um botão.
?   ?
{
  ?
  
    ?
    ?
    ?   ?   ?

    ?
    ?
    ?   ?   ?

    ?
    ?
    ?   ?   ?

    ?
    ?
    ?   ?
  
}
