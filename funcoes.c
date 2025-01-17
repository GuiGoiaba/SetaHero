#include "header.h"

Fila *criar_fila()
{
    Fila *fila = (Fila *)malloc(sizeof(Fila));
    if (!fila)
    {
        printf("erro ao alocar memória para a fila :/\n");
        exit(EXIT_FAILURE);
    }
    fila->inicio = fila->fim = NULL;
    return fila;
}

void enfileirar(Fila *fila, Seta *seta)
{
    Lista *novo = (Lista *)malloc(sizeof(Lista));
    if (!novo)
    {
        printf("erro ao alocar memória para a fila :/\n");
        exit(EXIT_FAILURE);
    }
    novo->seta = seta;
    novo->proximo = NULL;

    if (fila->fim != NULL)
    {
        fila->fim->proximo = novo;
    }
    else
    {
        fila->inicio = novo;
    }
    fila->fim = novo;
}

Seta *desenfileirar(Fila *fila)
{
    Lista *temp = fila->inicio;
    Seta *seta = temp->seta;
    fila->inicio = fila->inicio->proximo;

    if (fila->inicio == NULL)
    {
        fila->fim = NULL;
    }
    free(temp);
    return seta;
}

void liberar_fila(Fila *fila)
{
    while (!fila)
    {
        Seta *seta = desenfileirar(fila);
        free(seta);
    }
    free(fila);
}

Seta *criar_seta(int x, int y)
{
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
}

Seta *criar_jogador(int x, int y, int direcao)
{
    Seta *seta_jogador = (Seta *)malloc(sizeof(Seta));
    if (!seta_jogador)
    {
        printf("erro ao alocar memória para a fila :/\n");
        exit(EXIT_FAILURE);
    }
    seta_jogador->x = x;
    seta_jogador->y = y;
    seta_jogador->direcao = direcao;
    return seta_jogador;
}

void liberar_setas(Seta **setas, int max_inimigos)
{
    for (int i = 0; i < max_inimigos; i++)
    {
        if (setas[i] != NULL)
        {
            free(setas[i]);
            setas[i] = NULL;
        }
    }
}

// inicialização da janela
ALLEGRO_DISPLAY *cria_tela()
{
    ALLEGRO_DISPLAY *janela = al_create_display(LARGURA, ALTURA);
    if (!janela)
    {
        printf("erro ao alocar memória para a fila :/\n");
        exit(EXIT_FAILURE);
    }
    return janela;
}

// funções de inicialização
void inicializa_allegro()
{
    if (!al_init())
    {
        printf("erro ao alocar memória para a fila :/\n");
        exit(EXIT_FAILURE);
    }
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
}

// carregamento dos bitmaps
void inicializa_recursos(ALLEGRO_BITMAP **seta_cima, ALLEGRO_BITMAP **seta_baixo, ALLEGRO_BITMAP **seta_direita, ALLEGRO_BITMAP **seta_esquerda)
{
    *seta_cima = al_load_bitmap("Assets/seta_cima.png");
    *seta_baixo = al_load_bitmap("Assets/seta_baixo.png");
    *seta_direita = al_load_bitmap("Assets/seta_direita.png");
    *seta_esquerda = al_load_bitmap("Assets/seta_esquerda.png");
    if (!*seta_cima || !*seta_baixo || !*seta_direita || !*seta_esquerda)
    {
        printf("Erro ao carregar os bitmaps.\n");
        exit(EXIT_FAILURE);
    }
}

// configuração dos eventos
void configura_eventos(ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_DISPLAY *janela, ALLEGRO_TIMER *timer)
{
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_start_timer(timer);
}

// inicialização do jogo
void inicializa_jogo(ALLEGRO_DISPLAY **janela, ALLEGRO_BITMAP **seta_cima, ALLEGRO_BITMAP **seta_baixo, ALLEGRO_BITMAP **seta_direita, ALLEGRO_BITMAP **seta_esquerda, ALLEGRO_EVENT_QUEUE **fila_eventos, ALLEGRO_TIMER **timer)
{
    *janela = cria_tela();
    *fila_eventos = al_create_event_queue();
    *timer = al_create_timer(1.0 / FPS);
    inicializa_allegro();
    inicializa_recursos(seta_cima, seta_baixo, seta_direita, seta_esquerda);
    configura_eventos(*fila_eventos, *janela, *timer);
    printf("espero que goste :D\n");
}

// desenho do menu
void desenha_menu(int escolha, const char *nome_jogador, int ult_pont)
{
    ALLEGRO_FONT *fonte = al_create_builtin_font();
    al_clear_to_color(al_map_rgb(192, 192, 192));

    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 4, ALLEGRO_ALIGN_CENTRE, "SETA HERO");

    if (nome_jogador[0] != '\0')
    {
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 4 + 40, ALLEGRO_ALIGN_CENTRE,
                      "Jogador: %s", nome_jogador);
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 4 + 80, ALLEGRO_ALIGN_CENTRE,
                      "Ultima Pontuacao: %d", ult_pont);
    }

    const char *opcoes[] = {"Iniciar Jogo", "Sair"};
    for (int i = 0; i < 2; i++)
    {
        if (i == escolha)
        {
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 2 + i * 40, ALLEGRO_ALIGN_CENTRE, opcoes[i]);
        }
        else
        {
            al_draw_text(fonte, al_map_rgb(128, 128, 128), LARGURA / 2, ALTURA / 2 + i * 40, ALLEGRO_ALIGN_CENTRE, opcoes[i]);
        }
    }
    al_flip_display();
}

// inserção do nome do jogador
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

void desenha_jogo(Seta *setas[], Seta *jogador, int direcao, int pontuacao, int vidas, float vel_inim, ALLEGRO_BITMAP *seta_cima, ALLEGRO_BITMAP *seta_baixo, ALLEGRO_BITMAP *seta_direita, ALLEGRO_BITMAP *seta_esquerda)
{
    jogador = criar_jogador(LARGURA / 2 - TAM_SETA / 2, (ALTURA / 2) - TAM_SETA / 2, direcao);
    desenha_seta(jogador, seta_cima, seta_baixo, seta_direita, seta_esquerda);
    for (int i = 0; i < MAX_INIMIGOS; i++)
    {
        desenha_seta(setas[i], seta_cima, seta_baixo, seta_direita, seta_esquerda);
    }
    al_flip_display();
    al_clear_to_color(al_map_rgb(192, 192, 192));
    al_draw_filled_rectangle(0, 355, LARGURA, 270, al_map_rgb(128, 128, 128));
    al_draw_textf(al_create_builtin_font(), al_map_rgb(0, 0, 0), 10, 10, 0, "Pontuação: %d", pontuacao);
    al_draw_textf(al_create_builtin_font(), al_map_rgb(0, 0, 0), LARGURA - 150, 10, 0, "Vidas: %d", vidas);
    al_draw_textf(al_create_builtin_font(), al_map_rgb(0, 0, 0), LARGURA - 150, 30, 0, "Velocidade: %.2f", vel_inim);
}

void desenha_seta(Seta *seta, ALLEGRO_BITMAP *seta_cima, ALLEGRO_BITMAP *seta_baixo, ALLEGRO_BITMAP *seta_direita, ALLEGRO_BITMAP *seta_esquerda)
{
    switch (seta->direcao)
    {
    case cima: // Cima
        al_draw_bitmap(seta_cima, seta->x, seta->y, 0);
        break;
    case baixo: // Baixo
        al_draw_bitmap(seta_baixo, seta->x, seta->y, 0);
        break;
    case direita: // Direita
        al_draw_bitmap(seta_direita, seta->x, seta->y, 0);
        break;
    case esquerda: // Esquerda
        al_draw_bitmap(seta_esquerda, seta->x, seta->y, 0);
        break;
    }
}

void game_over(int pontuacao, char *nome_jogador)
{
    ALLEGRO_FONT *fonte = al_create_builtin_font();
    al_clear_to_color(al_map_rgb(192, 192, 192));

    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 3, ALLEGRO_ALIGN_CENTRE, "Fim de Jogo!");
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 2, ALLEGRO_ALIGN_CENTRE, "Jogador: %s", nome_jogador);
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 2 + 40, ALLEGRO_ALIGN_CENTRE, "Pontuação Final: %d", pontuacao);
    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA / 2, ALTURA / 2 + 80, ALLEGRO_ALIGN_CENTRE, "Pressione Enter para voltar ao Menu");

    al_flip_display();
}

void finaliza_recursos(ALLEGRO_DISPLAY *janela, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_TIMER *timer, ALLEGRO_BITMAP *seta_cima, ALLEGRO_BITMAP *seta_baixo, ALLEGRO_BITMAP *seta_direita, ALLEGRO_BITMAP *seta_esquerda)
{
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_destroy_bitmap(seta_cima);
    al_destroy_bitmap(seta_baixo);
    al_destroy_bitmap(seta_direita);
    al_destroy_bitmap(seta_esquerda);
}