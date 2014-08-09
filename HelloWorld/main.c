#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

const int FPS = 60;

struct cano {
    int x;
    int y;
};

int atualizar_posicao(int aceleracao, int posicao) {
    return posicao - aceleracao;
}

void desenha_cano(struct cano c)
{
    al_draw_filled_rectangle(c.x, 0, c.x + 80, c.y,
        al_map_rgb(0, 0xFF, 0));

    al_draw_filled_rectangle(c.x, c.y + 120, c.x + 80, 480,
        al_map_rgb(0, 0xFF, 0));
}

void mostra_jogo()
{
    ALLEGRO_EVENT_QUEUE *fila_eventos =
        al_create_event_queue();
    al_register_event_source(fila_eventos,
        al_get_keyboard_event_source());
    al_register_event_source(fila_eventos,
        al_get_mouse_event_source());

    int y = 240;
    float aceleracao = 0;
    double tempo_inicial, tempo_final;
    bool sair = false;
    int raio = 20;

    struct cano canos[4];

    // inicialização dos canos
    canos[0].x = 750;
    canos[0].y = 180;

    canos[1].x = 750 + (160 * 1);
    canos[1].y = 180;

    canos[2].x = 750 + (160 * 2);
    canos[2].y = 180;

    canos[3].x = 750 + (160 * 3);
    canos[3].y = 180;

    while (sair == false) {
        tempo_inicial = al_get_time();

        al_clear_to_color(al_map_rgb(0, 0, 0));

        int i = 0;
        for (i = 0; i < 4; i++) {
            desenha_cano(canos[i]);
        }

        al_draw_filled_circle(70, y, raio,
            al_map_rgb(0xFF, 0, 0));
        al_flip_display();

        y = atualizar_posicao(aceleracao, y);
        aceleracao -= 0.25;

        if (y > 480 - raio) {
            y = 480 - raio;
            aceleracao = 0;
        }

        if (y < raio) {
            y = raio;
            aceleracao = 0;
        }

        while (!al_is_event_queue_empty(fila_eventos)) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (evento.keyboard.keycode ==
                                    ALLEGRO_KEY_ESCAPE)
                {
                    sair = true;
                }
            }
            else if (evento.type ==
                    ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                aceleracao = 5;
            }
        }

        tempo_final = al_get_time() - tempo_inicial;
        if (tempo_final < 1.0 / FPS) {
            al_rest(1.0 / FPS - tempo_final);
        }
    }

    al_destroy_event_queue(fila_eventos);
}

void monta_menu(int opcao_selecionada)
{
    ALLEGRO_BITMAP *config =
        al_load_bitmap("Recursos/config.png");
    ALLEGRO_BITMAP *config_pressed =
        al_load_bitmap("Recursos/config_pressed.png");
    ALLEGRO_BITMAP *iniciar =
        al_load_bitmap("Recursos/iniciar.png");
    ALLEGRO_BITMAP *iniciar_pressed =
        al_load_bitmap("Recursos/iniciar_pressed.png");
    ALLEGRO_BITMAP *sair =
        al_load_bitmap("Recursos/sair.png");
    ALLEGRO_BITMAP *sair_pressed =
        al_load_bitmap("Recursos/sair_pressed.png");

    // Primeira opção do menu (iniciar)
    if (opcao_selecionada == 1) {
        al_draw_bitmap(iniciar_pressed, 320 -
            al_get_bitmap_width(iniciar_pressed) / 2,
            240 - 10 -
            al_get_bitmap_height(iniciar_pressed) -
            al_get_bitmap_height(config) / 2,
            0);
    } else {
        al_draw_bitmap(iniciar, 320 -
            al_get_bitmap_width(iniciar) / 2,
            240 - 10 -
            al_get_bitmap_height(iniciar) -
            al_get_bitmap_height(config) / 2,
            0);
    }

    // Segundo botão do menu (config)
    if (opcao_selecionada == 2) {
        al_draw_bitmap(config_pressed, 320 -
            al_get_bitmap_width(config_pressed) / 2,
            240 - al_get_bitmap_height(config_pressed) / 2,
            0);
    } else {
        al_draw_bitmap(config, 320 -
            al_get_bitmap_width(config) / 2,
            240 - al_get_bitmap_height(config) / 2,
            0);
    }

    // Terceiro botão do menu (sair)
    if (opcao_selecionada == 3) {
        al_draw_bitmap(sair_pressed, 320 -
            al_get_bitmap_width(sair_pressed) / 2,
            240 + 10 + al_get_bitmap_height(config) / 2,
            0);
    } else {
        al_draw_bitmap(sair, 320 -
            al_get_bitmap_width(sair_pressed) / 2,
            240 + 10 + al_get_bitmap_height(config) / 2,
            0);
    }

    al_destroy_bitmap(config);
    al_destroy_bitmap(config_pressed);
    al_destroy_bitmap(iniciar);
    al_destroy_bitmap(iniciar_pressed);
    al_destroy_bitmap(sair);
    al_destroy_bitmap(sair_pressed);
}

int main()
{
    // Declara a janela do aplicativo
    ALLEGRO_DISPLAY *janela;
    // Variável para controlar os eventos
    ALLEGRO_EVENT_QUEUE *fila_eventos;
    // Declara a imagem de fundo
    ALLEGRO_BITMAP *fundo;
    // Variável que representa a música de fundo
    ALLEGRO_AUDIO_STREAM *musica;

    // Inicializa a biblioteca
    al_init();
    // Inicializa o módulo de imagens
    al_init_image_addon();
    // Inicializando o teclado
    al_install_keyboard();
    // Inicializando o mouse
    al_install_mouse();
    // Inicializar o áudio
    al_install_audio();
    // Inicializar a parte de codecs de áudio
    al_init_acodec_addon();
    // Reservar samples
    al_reserve_samples(1);
    // Inicializar o módulo de primitivas gráficas
    al_init_primitives_addon();

    // Cria a fila de eventos
    fila_eventos = al_create_event_queue();
    // Carregar a imagem de fundo
    fundo = al_load_bitmap("Recursos/bg.jpg");

    // Cria uma janela de 640x480
    janela = al_create_display(640, 480);
    al_set_window_title(janela, "Super Projeto");
    // Pinta a tela de vermelho
    //al_clear_to_color(al_map_rgb(0xFF, 0x00, 0x00));

    // Desenhar a imagem na tela
    al_draw_bitmap(fundo, 0, 0, 0);

    // Aplica as alterações na tela
    al_flip_display();

    // Carregar a música
    musica = al_load_audio_stream("Recursos/music.ogg",
        4, 1024);
    al_attach_audio_stream_to_mixer(musica,
        al_get_default_mixer());

    // A fila de eventos receberá os eventos da janela
    al_register_event_source(fila_eventos,
        al_get_display_event_source(janela));
    al_register_event_source(fila_eventos,
        al_get_keyboard_event_source());

    // Música, comece a tocar!
    al_set_audio_stream_playing(musica, true);

    // Espera 5 segundos
    //al_rest(5.0);
    int opcao_menu = 1;
    bool sair = false;
    while (sair == false)
    {
        al_draw_bitmap(fundo, 0, 0, 0);
        monta_menu(opcao_menu);
        al_flip_display();

        while (!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                sair = true;
            }
            else if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if (evento.keyboard.keycode ==
                                    ALLEGRO_KEY_ESCAPE)
                {
                    sair = true;
                }
                else if (evento.keyboard.keycode ==
                                    ALLEGRO_KEY_DOWN)
                {
                    opcao_menu++;
                    if (opcao_menu > 3) {
                        opcao_menu = 1;
                    }
                }
                else if (evento.keyboard.keycode ==
                                    ALLEGRO_KEY_UP)
                {
                    opcao_menu--;
                    if (opcao_menu < 1) {
                        opcao_menu = 3;
                    }
                }
                else if (evento.keyboard.keycode ==
                                    ALLEGRO_KEY_ENTER)
                {
                    if (opcao_menu == 3) {
                        sair = true;
                    }
                    else if (opcao_menu == 1) {
                        al_unregister_event_source(
                            fila_eventos,
                            al_get_keyboard_event_source());
                        mostra_jogo();
                        al_register_event_source(
                            fila_eventos,
                            al_get_keyboard_event_source());
                    }
                }
            }
        }
    }

    // Música, pode parar!
    al_set_audio_stream_playing(musica, false);

    // Libera a memória usada pela música
    al_destroy_audio_stream(musica);
    // Libera a memória usada pela imagem de fundo
    al_destroy_bitmap(fundo);
    // Libera a memória usada pela janela
    al_destroy_display(janela);

    return 0;
}
