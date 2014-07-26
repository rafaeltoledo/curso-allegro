#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

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
    // Inicializar o áudio
    al_install_audio();
    // Inicializar a parte de codecs de áudio
    al_init_acodec_addon();
    // Reservar samples
    al_reserve_samples(1);

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
    bool sair = false;
    while (sair == false)
    {
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
