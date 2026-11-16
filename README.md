# doorBell

Campainha ESP32-S2 baseada no Light1Canal (base `86cb112`). Mantém Wi-Fi, portal, pareamento Matter e atualização OTA.

## Ligações e funcionamento

- Entrada: contato seco normalmente aberto entre **GPIO 33 e GND**, com pull-up interno habilitado por software. Aberto = HIGH, pressionado = LOW. Não aplicar tensão externa na entrada.
- Saída: **GPIO 35**, LED ativo em HIGH, com resistor limitador adequado; retorno ao GND.
- Cada pressionamento estável por 30 ms inicia o LED imediatamente e pisca a **1 Hz** (500 ms ligado / 500 ms desligado) durante **10 segundos**, terminando em LOW.
- Soltar o botão não aciona. Manter pressionado não repete. Um novo pressionamento reinicia os 10 segundos. Botão fechado ao ligar é reconhecido após o debounce.
- Funciona sem rede e antes do início do Matter. Reiniciar deixa o LED desligado; estados salvos não reacendem a saída.
- O endpoint Matter herdado informa acionamento por OnOff=true durante os 10 segundos e false ao terminar. As piscadas não alternam o atributo. Comandos remotos de liga/desliga/brilho não acionam o LED, destinado exclusivamente à confirmação do botão local. O endpoint continua compatível com luz regulável da base; não anuncia eventos Matter de campainha.

## Compilação e atualização

O push em `main` executa `.github/workflows/build.yml`: testes e compilação ESP-Matter para ESP32-S2. O artefato `doorbell-firmware` contém os binários e os argumentos para gravação. A release publica `light.bin`, nome mantido por compatibilidade com o portal OTA. Use somente firmware deste repositório.

O projeto ESP-IDF permanece em `light/`. O portal usa o AP `doorBell-XXXXXX`; configure Wi-Fi e pareamento como na base. ESP32-S2 não possui Bluetooth; Matter usa comissionamento pela rede IP.

Testes de lógica: `g++ -std=c++17 -Wall -Wextra -Werror light/tests/doorbell_test.cpp -o doorbell_test` e execução do binário; `python light/tests/relay_local_test.py`; `node light/tests/portal_ui_test.js`.
