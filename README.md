# Prometheus 1 - Aviônica

Projeto universitario de sistemas embarcados, desenvolvido para fins academicos e de pesquisa com um microcontrolador STM32.

O projeto reune firmware, integracao com sensores e comunicacao para apoiar atividades praticas de desenvolvimento embarcado.

## Estrutura do codigo

O codigo principal do projeto esta organizado dentro da pasta `Core`:

```text
Core/
├── Inc/  # Headers e bibliotecas utilizadas pelo firmware
└── Src/  # Implementacoes, incluindo o main e as bibliotecas do programa
```

### `Core/Inc`

A pasta `Inc` contem os arquivos de cabecalho (`.h`) e as declaracoes das bibliotecas utilizadas pelo projeto. Esses arquivos definem interfaces, tipos, configuracoes e funcoes compartilhadas pelo firmware.

### `Core/Src`

A pasta `Src` contem os arquivos-fonte (`.c`) do firmware. Nela esta o `main.c`, que inicializa o sistema e coordena a aplicacao, junto das implementacoes das bibliotecas utilizadas pelo `main`, como drivers, sensores, comunicacao e maquina de estados.

## Creditos

Este projeto foi desenvolvido no contexto das atividades da LAPESP, liga academica a qual o autor pertence.

## Tecnologias

- STM32CubeIDE
- STM32F401CEU6
- STM32 HAL
- FreeRTOS
- C

## Licenca

Este repositorio tem finalidade academica.
