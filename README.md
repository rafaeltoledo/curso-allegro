curso-allegro
=============

Exemplos desenvolvidos no curso sobre a biblioteca Allegro.

## Para baixar a biblioteca Allegro (Windows)
[Zip](http://cdn.allegro.cc/file/library/allegro/5.0.10/allegro-5.0.10-mingw-4.7.0.zip)
ou
[7z](http://cdn.allegro.cc/file/library/allegro/5.0.10/allegro-5.0.10-mingw-4.7.0.7z)

- Copiar as pastas **bin**, **include** e **lib** para C:\Program Files (x86)\CodeBlocks\MinGW

Para compilar a biblioteca Allegro (Ubuntu):
[Tutorial](http://www.rafaeltoledo.net/compilando-e-instalando-a-biblioteca-allegro-5-no-ubuntu/)

Para baixar o Code::Blocks (Windows):
[Download](http://sourceforge.net/projects/codeblocks/files/Binaries/13.12/Windows/codeblocks-13.12mingw-setup.exe)

Para instalar o Code::Blocks (Linux):
```
  sudo apt-get install codeblocks
```

Opções do linker
================

(Windows)
```
  -lallegro-5.0.10-mt
  -lallegro_image-5.0.10-mt
  -lallegro_audio-5.0.10-mt
  -lallegro_acodec-5.0.10-mt
  -lallegro_font-5.0.10-mt
  -lallegro_ttf-5.0.10-mt
  -lallegro_dialog-5.0.10-mt
  -lallegro_primitives-5.0.10-mt
```

(Ubuntu)
```
  -lallegro
  -lallegro_image
  -lallegro_audio
  -lallegro_acodec
  -lallegro_font
  -lallegro_ttf
  -lallegro_dialog
  -lallegro_primitives
```
