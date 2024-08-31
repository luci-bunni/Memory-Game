/*
  JOGO DA MEMÓRIA
  NEEC IST
*/

#define CHOICE_OFF      0 //Controla os LED's
#define CHOICE_NONE     0 //Check dos Botões
#define CHOICE_2ND_RED  (1 << 0)
#define CHOICE_1ST_RED    (1 << 1)
#define CHOICE_YELLOW (1 << 2)
#define CHOICE_GREEN   (1 << 3)

//Pinos de saída do arduino para os Led's
#define LED_2ND_RED     10
#define LED_1ST_RED   3
#define LED_YELLOW    13
#define LED_GREEN  5

//Pinos de saída do arduino para os botões
#define BUTTON_2ND_RED   9
#define BUTTON_1ST_RED  2
#define BUTTON_YELLLOW   12
#define BUTTON_GREEN 6

// Número de rondas que é preciso fazer com sucesso antes de ganhar.
#define ROUNDS_TO_WIN      13 

// Tempo disponível para pressionar um botão antes que o jogo dê time out. 3000ms = 3 segundos
#define ENTRY_TIME_LIMIT   3000 


#define MODE_MEMORY  0
#define MODE_BATTLE  1

// Variáveis de estado do jogo
byte gameMode = MODE_MEMORY; // Por default, vamos jogar o jogo de memória
byte gameBoard[32]; // Contém a combinação de botões à medida que avançamos
byte gameRound = 0; // Conta o número de rondas bem-sucedidas que o jogador completou

void setup()
{
  // Configuração das entradas/saídas de hardware. Estes pinos estão definidos na função hardware_versions
  // Ativar resistencias de pull-up nas entradas
  pinMode(BUTTON_2ND_RED, INPUT_PULLUP);
  pinMode(BUTTON_1ST_RED, INPUT_PULLUP);
  pinMode(BUTTON_YELLLOW, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);

  pinMode(LED_2ND_RED, OUTPUT);
  pinMode(LED_1ST_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // Verificação do modo de jogo
  gameMode = MODE_MEMORY; // Por default, vamos jogar o jogo de memória

  // Verificar se o botão superior direito está pressionado aquando o arduino é iniciado
  // Se isso acontecer, estamos a colocar o jogo em modo batalha
  if (checkButton() == CHOICE_1ST_RED )
  {
    gameMode = MODE_BATTLE; // Colocar o jogo no modo batalha
    // Ligar o LED superior direito (vermelho)
    setLEDs(CHOICE_1ST_RED);
    led_manegement(CHOICE_1ST_RED, 150);
    setLEDs(CHOICE_2ND_RED | CHOICE_YELLOW | CHOICE_GREEN); // Ligar os outros LEDs até que o botão seja solto

    while(checkButton() != CHOICE_NONE) ; // Aguardar que o jogador pare de pressionar o botão
    // Foi escolhido o modo batalha
  }
  play_winner(); 
}

void loop()
{
  attractMode(); // Piscar as luzes enquanto esperamos o jogador pressionar um botão

  // Indicar o início do jogo
  setLEDs(CHOICE_2ND_RED | CHOICE_1ST_RED | CHOICE_YELLOW | CHOICE_GREEN); // Ligar todos os LEDs
  delay(1000);
  setLEDs(CHOICE_OFF); // Desligar os LEDs
  delay(250);

  if (gameMode == MODE_MEMORY)
  {
    // Jogar o jogo de memória e lidar com o resultado
    if (play_memory() == true) {
      play_winner(); // O jogador venceu, tocar os tons de vitória
      //Serial.print(gameRound, 2);
    }
    else {
      play_loser(); // O jogador perdeu, tocar os tons de derrota
      //Serial.print(gameRound, 2);
    }
  }

  if (gameMode == MODE_BATTLE)
  {
    play_battle(); // Jogar no modo batalha até que alguém perca
    play_loser(); // O jogador perdeu, tocar os tons de derrota
    //Serial.print(gameRound, 2);
  }
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//As funções a seguir estão relacionadas apenas ao jogo


// --------------MODO JOGO DA MEMÓRIA DEFAULT------------------
// Retorna 0 se o jogador perder, ou 1 se o jogador vencer
boolean play_memory(void)
{
  randomSeed(millis()); //função random a funcionar em milissegundos

  gameRound = 0; // Reset do jogo para o início

  while (gameRound < ROUNDS_TO_WIN) 
  {
    add_to_moves(); // Adiciona um botão aos movimentos atuais e reproduz
    playMoves(); // Reproduz a sequência do jogo atual

    // Em seguida, exige que o jogador repita a sequência.
    for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++)
    {
      byte choice = wait_for_button(); // Verifica qual botão foi pressionado
      if (choice == 0) return false; // Se o tempo de espera acabar, o jogador perde
      if (choice != gameBoard[currentMove]) return false; // Se a escolha estiver incorreta, o jogador perde
    }
    delay(1000); // O jogador acertou, aguarda antes de reproduzir os movimentos
  }
  return true; // O jogador passou por todas as rondas e venceu!
}

//--------------MODO JOGO DA MEMÓRIA BATALHA------------------
// Jogar o modo batalha especial para 2 jogadores
// Um jogador começa pressionando um botão e passando-o para o outro jogador
// Esse jogador repete o botão e adiciona um e devolve ao outro.

// Esta função dá return quando alguém perde
boolean play_battle(void)
{
  gameRound = 0; // Reset do jogo para a primeira ronda

  while (1) // Loop até que alguém falhe
  {
    byte newButton = wait_for_button(); // Aguarda o jogador realize o próximo movimento
    gameBoard[gameRound++] = newButton; // Adiciona este novo botão ao array do jogo

    // Em seguida, exige que o jogador repita a sequência.
    for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++)
    {
      byte choice = wait_for_button();
      if (choice == 0) return false; // Se o tempo de espera acabar, o jogador perde.
      if (choice != gameBoard[currentMove]) return false; // Se a escolha estiver incorreta, o jogador perde.
    }
    delay(100); // Dá ao jogador 100ms extras para passar o jogo para o outro jogador
  }
  return true; // Nunca devemos chegar aqui porque não existe limite de jogadas, apenas acaba quando alguém perder
}

// Reproduz o conteúdo atual dos movimentos do jogo
void playMoves(void)
{
  for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++) 
  {
    led_manegement(gameBoard[currentMove], 150);
    // Aguarda algum tempo entre as reproduções dos botões
    // Se quiseres tornar o jogo mais difícil, diminui o tempo
    delay(150); // 150 funciona bem. 75 fica rápido.
  }
}

// Adiciona um novo botão aleatório à sequência do jogo
void add_to_moves(void)
{
  // Random de números de 0 a 3
  byte newButton = random(0, 4); //min (incluído), max (excluído)

  // Temos que converter esse número, de 0 a 3, para CHOICEs
  if(newButton == 0) newButton = CHOICE_2ND_RED;
  else if(newButton == 1) newButton = CHOICE_1ST_RED;
  else if(newButton == 2) newButton = CHOICE_YELLOW;
  else if(newButton == 3) newButton = CHOICE_GREEN;
  gameBoard[gameRound++] = newButton; // Adiciona este novo botão ao array do jogo
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// As funções a seguir controlam o hardware
// Acende os LEDs específicos
// Recebe um byte composto por CHOICE_2ND_RED, CHOICE_GREEN, etc.
void setLEDs(byte leds)
{
  if ((leds & CHOICE_2ND_RED) != 0)
    digitalWrite(LED_2ND_RED, HIGH);
  else
    digitalWrite(LED_2ND_RED, LOW);
  if ((leds & CHOICE_1ST_RED) != 0)
    digitalWrite(LED_1ST_RED, HIGH);
  else
    digitalWrite(LED_1ST_RED, LOW);
  if ((leds & CHOICE_YELLOW) != 0)
    digitalWrite(LED_YELLOW, HIGH);
  else
    digitalWrite(LED_YELLOW, LOW);
  if ((leds & CHOICE_GREEN) != 0)
    digitalWrite(LED_GREEN, HIGH);
  else
    digitalWrite(LED_GREEN, LOW);
}

// Aguarda até que um botão seja pressionado.
// Return de uma das cores dos LEDs (LED_2ND_RED, etc.) se bem-sucedido, '0' se o tempo esgotar
byte wait_for_button(void)
{
  long startTime = millis(); // Armazena o tempo em que iniciamos este loop

  while ( (millis() - startTime) < ENTRY_TIME_LIMIT) // Loop até que muito tempo tenha passado
  {
    byte button = checkButton();
    if (button != CHOICE_NONE)
    { 
      led_manegement(button, 150);
      while(checkButton() != CHOICE_NONE) ;  // Agora espera o jogador soltar o botão
      delay(10); // Isto ajuda a evitar o bounce e toques duplos acidentais
      return button;
    }
  }
  return CHOICE_NONE; // Se chegarmos aqui, o tempo acabou!
}

// Função que liga os led's
void led_manegement(byte which, int length_ms)
{
  setLEDs(which); //Mostra o led dado

  switch(which) 
  {
  case CHOICE_2ND_RED:
    led_timer(length_ms, 1136); 
    break;
  case CHOICE_1ST_RED:
    led_timer(length_ms, 568); 
    break;
  case CHOICE_YELLOW:
    led_timer(length_ms, 851); 
    break;
  case CHOICE_GREEN:
    led_timer(length_ms, 638); 
    break;
  }

  setLEDs(CHOICE_OFF); //Desliga todos os led's
}

// Gere o tempo de mostragem dos led's
void led_timer(int length_ms, int buzz_delay_us)
{
  // converter milissegundos para microssegundos
  long length_us = length_ms * (long)1000;

  // Loop que vai diminuindo o tempo até ser menos que um delay_us
  while (length_us > (buzz_delay_us * 2))
  {
    length_us -= buzz_delay_us * 2; //Diminui o tempo de jogo

    delayMicroseconds(buzz_delay_us);
    delayMicroseconds(buzz_delay_us);
  }
}

// Retorna um bit '1' na posição correspondente a CHOICE_2ND_RED, CHOICE_1ST_RED, etc.
byte checkButton(void)
{
  if (digitalRead(BUTTON_2ND_RED) == 0) return(CHOICE_2ND_RED); 
  else if (digitalRead(BUTTON_1ST_RED) == 0) return(CHOICE_1ST_RED); 
  else if (digitalRead(BUTTON_YELLLOW) == 0) return(CHOICE_YELLOW); 
  else if (digitalRead(BUTTON_GREEN) == 0) return(CHOICE_GREEN);

  return(CHOICE_NONE); // Se nenhum botão for pressionado, retorna CHOICE_NONE
}

// Acende as luzes do vencedor
void play_winner(void)
{
  setLEDs(CHOICE_1ST_RED | CHOICE_YELLOW);
  setLEDs(CHOICE_2ND_RED | CHOICE_GREEN);
  setLEDs(CHOICE_1ST_RED | CHOICE_YELLOW);
  setLEDs(CHOICE_2ND_RED | CHOICE_GREEN);
}

// Mostra as luzes do perdedor
void play_loser(void)
{
  setLEDs(CHOICE_2ND_RED | CHOICE_1ST_RED);
  led_timer(255, 1500);
  setLEDs(CHOICE_YELLOW | CHOICE_GREEN);
  led_timer(255, 1500);
  setLEDs(CHOICE_2ND_RED | CHOICE_1ST_RED);
  led_timer(255, 1500); 
  setLEDs(CHOICE_YELLOW | CHOICE_GREEN);
  led_timer(255, 1500);
}

// Mostra uma exibição do "modo de atração" enquanto espera que o jogador pressione um botão.
void attractMode(void)
{
  while(1) 
  {
    setLEDs(CHOICE_2ND_RED);
    delay(100);
    if (checkButton() != CHOICE_NONE) return;

    setLEDs(CHOICE_YELLOW);
    delay(100);
    if (checkButton() != CHOICE_NONE) return;

    setLEDs(CHOICE_1ST_RED);
    delay(100);
    if (checkButton() != CHOICE_NONE) return;

    setLEDs(CHOICE_GREEN);
    delay(100);
    if (checkButton() != CHOICE_NONE) return;
  }
}
