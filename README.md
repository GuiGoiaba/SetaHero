<h1>Seta-Hero</h1>
Jogo simples de ritmo

## Descrição

Nesse jogo a tela se comportar como uma esteira onde serão enfileirados símbolos aleatórios correspondentes as setas do teclado (para cima, para baixo, para direita e para esquerda).

---
# Ferramenta

Linguagem C em conjunto à biblioteca Allegro5

Mais informações acerca em [Allegro 5 (A game programming library)](http://liballeg.org/)
## Compilação

Se você estiver no Linux ou Mac e tiver o Allegro5 e o gcc instalados, execute compile.sh para compilar este código. Não tenho ideia de como isso funcionaria no Windows, embora o Allegro ofereça suporte ao mesmo, então você pode tentar.

```
gcc -c funcoes.c
gcc -o SetaHero main.c funcoes.o -lallegro -lallegro_primitives -lallegro_image -lallegro_ttf -lallegro_font
```

---
# Sobre a biblioteca utilizada

```
ALLEGRO_TIMER *timer;
```
Essa parte do código representa um tipo abstrato de temporizador para o software.

```
ALLEGRO_DISPLAY *janela;
```
Cria uma tela ou janela com as dimensões especificadas.
```
ALLEGRO_EVENT_QUEUE *fila_eventos;
```
Uma fila de eventos realiza eventos que foram gerados por fontes de eventos que estão registrados na fila. Os eventos são armazenados na ordem em que são gerados. O acesso está em um FIFO estritamente.

---
```
ALLEGRO_BITMAP *seta_cima, *seta_baixo, *seta_direita, *seta_esquerda;
```
Basicamente um tipo abstrado (imagem em 2D).

---

A função ```inicializa_allegro();``` todos os addons necessários para o funcionamento da biblioteca como:
```
al_install_keyboard();

al_init_primitives_addon();

al_init_image_addon();

al_init_font_addon();

al_init_ttf_addon();
```
Por outro lado a função ```inicializa_jogo();```  inicializa com valores todas as  os objetos presente na função anterior [[INICIALIZAÇÃO DOS RECURSOS]], tal como:
```
*janela = cria_tela();

*fila_eventos = al_create_event_queue();

*timer = al_create_timer(1.0 / FPS);

inicializa_allegro();

inicializa_recursos(seta_cima, seta_baixo, seta_direita, seta_esquerda);

configura_eventos(*fila_eventos, *janela, *timer);
```


---
As setas são criadas a partir desta estrutura:
```
typedef struct seta
{
int x, y;
int direcao;
} Seta;
```
Onde o "x" e o "y" são as posições do objeto "Seta" no plano cartesiano com base nas dimensões criadas na [[INICIALIZAÇÃO DO JOGO]]. Enquanto a fila é criada com base no tipo "Fila com lista", com a principais funcionalidades(criar, inserir e remover elementos, liberar lista)
```
typedef struct lista
{
Seta *seta;
struct lista *proximo;
} Lista;

typedef struct fila
{
Lista *inicio;
Lista *fim;
} Fila;
```

---
A função ```criar_seta(int x, int y);``` aloca memória para a criação dos inimigos do jogo durante a compilação e inicializa todas as variáveis.

```
Seta *seta = (Seta *)malloc(sizeof(Seta));
if (!seta)
{
printf("erro ao alocar memória para a fila :/\n");
exit(EXIT_FAILURE);
}
seta->x = x;
seta->y = y;
seta->direcao = rand() % 4;
return seta;
```


---
# Loop Principal
```
srand(time(NULL));

bool redraw = true;

bool fechar = false;

bool dentro_menu = true;
```
Quando redraw está ativo isso significa que a cada um segundo um quadro sera renderizado.

---
# Menu
```if (ev.type == ALLEGRO_EVENT_KEY_DOWN)```

Esse if é para saber se alguma tecla foi pressionada.
## Escolha das opções
```
if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN)
{
opcao = (opcao + 1) % 2;
}
else if (ev.keyboard.keycode == ALLEGRO_KEY_UP)
{
opcao = (opcao - 1 + 2) % 2;
}
```
Essa operação garante que opcão faça ciclos entre 0 e 1.

---
# CONFIRMAÇÃO DA OPÇÃO
```
if (opcao == 0)
{
// reinicialização do jogo
dentro_menu = false;
insere_nome(nome_jogador);
pontuacao = 0;
vidas = 3;
vel_inim = 0;
for (int i = 0; i < MAX_INIMIGOS; i++)
{
setas[i]->x = LARGURA + i * espaco_inicial;
}
}
else
{
// saída do jogo
dentro_menu = false;
fechar = true;
}
```
Já essa parte do código manipula a confirmação de uma opção de menu quando a tecla "Enter" é pressionada e, se a opção selecionada for 0, ele reinicia o jogo redefinindo as variáveis ​​de estado e posicionando os inimigos para uma nova sessão de jogo. Caso não o menu é fechado dando espaço para a nova sessão, isto é ```insere_nome```.

---
# Inserir nome
```
void insere_nome(char *nome_jogador)
{
ALLEGRO_FONT *fonte;
fonte = al_create_builtin_font();
al_clear_to_color(al_map_rgb(192, 192, 192));

al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 3, ALLEGRO_ALIGN_CENTRE, "Digite seu nome:");
al_flip_display();
ALLEGRO_EVENT_QUEUE *input_teclado;
input_teclado = al_create_event_queue();
al_register_event_source(input_teclado, al_get_keyboard_event_source());
int inicio = 0;
bool cabou = false;
while (!cabou)
{
// captura de eventos
ALLEGRO_EVENT ev;
al_wait_for_event(input_teclado, &ev);
if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
{
cabou = true;
}
if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
{
if (ev.keyboard.unichar >= 32 && ev.keyboard.unichar <= 126 && inicio < MAX_NOME - 1)
{
nome_jogador[inicio++] = (char)ev.keyboard.unichar;
nome_jogador[inicio] = '\0';
}
else if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && inicio > 0)
{
nome_jogador[--inicio] = '\0';
}
else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
{
cabou = true;
}
al_clear_to_color(al_map_rgb(192, 192, 192));
al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 3, ALLEGRO_ALIGN_CENTRE, "Digite seu nome:");
al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 2, ALLEGRO_ALIGN_CENTRE, nome_jogador);
al_flip_display();
}
}
al_destroy_event_queue(input_teclado);
}
```

Este trecho de código garante que apenas caracteres válidos e imprimíveis sejam adicionados ao nome do jogador e que o nome não exceda o comprimento máximo permitido.

---
# Movimento da esteira
```
for (int i = 0; i < MAX_INIMIGOS; i++)
{
setas[i]->x -= (int)vel_inim;

if (setas[i]->x + TAM_SETA < 0)
{
vidas--;
free(setas[i]);
setas[i] = desenfileirar(fila_setas);
setas[i] = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
}
}
```

O trecho do código manipula o movimento dos inimigos pela tela, verifica se eles saíram da tela, diminui as vidas do jogador se tiverem saído e reposiciona os inimigos para entrar novamente no jogo pelo lado direito. Isso garante um fluxo contínuo de inimigos no jogo.

---
## Atualização dos segundos 
```
seg++;
if (seg > 300)
{
vel_inim += VELOCIDADE;
seg = 0;
if (vel_inim > 12)
fechar = true;
}
```

Já neste a cada cinco segundos haverá um incremento na velocidade da esteira, porem se passar de doze segundos há um estouro de memória.

---
# Movimento do Jogador
```
switch (ev.keyboard.keycode)
{
case (ALLEGRO_KEY_ESCAPE):
fechar = true;
break;
case (ALLEGRO_KEY_UP):
direcao = cima;
break;
case (ALLEGRO_KEY_DOWN):
direcao = baixo;
break;
case (ALLEGRO_KEY_RIGHT):
direcao = direita;
break;
case (ALLEGRO_KEY_LEFT):
direcao = esquerda;
break;
}
```
Isto basicamente muda os sprites do jogador caso as teclas cima, baixo, direita ou esquerda forem pressionada.

---
# Colisão
```
if (setas[i]->x < LARGURA / 2 + TAM_SETA / 2 &&
setas[i]->x > LARGURA / 2 - TAM_SETA / 2 &&
setas[i]->direcao == direcao)
{
pontuacao += 50;
free(setas[i]);
setas[i] = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
}
else if (setas[i]->x < LARGURA / 2 + TAM_SETA / 2 &&
setas[i]->x > LARGURA / 2 - TAM_SETA / 2 &&
setas[i]->direcao != direcao)
{
vidas--;
free(setas[i]);
setas[i] = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
}
```
Este trecho de código manipula a detecção de colisão entre o jogador e os inimigos, se a seta estiver na área de colisão do jogador ela vai ser desalocada.

---
# Tela final
```
{
// tela final do jogo
game_over(pontuacao, nome_jogador);
ult_pont = pontuacao;
liberar_setas(setas, MAX_INIMIGOS);

// Reiniciar o jogo
for (int i = 0; i < MAX_INIMIGOS; i++)
{
setas[i] = criar_seta(LARGURA, ALTURA / 2 - TAM_SETA / 2);
}
```
Aqui manipula a transição da tela de game over para o menu principal, além de reiniciar as variáveis principais.

---
# O Jogo
```
if (redraw && al_is_event_queue_empty(fila_eventos))
{
redraw = false;
desenha_jogo(setas, jogador, direcao, pontuacao, vidas, vel_inim, seta_cima, seta_baixo, seta_direita, seta_esquerda);
}
```
Por ultimo, ele redefine o flag redraw e chama a função ```desenha_jogo``` para atualizar a janela com o estado atual do jogo.

---
# Liberar memória
```
liberar_fila(fila_setas);

liberar_setas(setas, MAX_INIMIGOS);

finaliza_recursos(janela, fila_eventos, timer, seta_cima, seta_baixo, seta_direita, seta_esquerda);
```
