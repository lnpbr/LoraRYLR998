# LoraRYLR998

## Para configurar o chip RYLR998 via NodeMCU (Converter o NodeMCU em um adaptador Serial-USB) ###

No NodeMCU, **_efetuar um Jumper entre "EN" e "G"_**.
Ligar da seguinte forma:

```
RYLR998 <--> NodeMCU
VDD      --  3v3
GND      --  G
TXD      --  TX
RXD      --  RX
```

Comandos para configurar o módulo rádio LoRa:

```
AT+ADDRESS=6 (escolher um endereço diferente para cada rádio)
AT+NETWORKID=10 (deve-se usar o mesmo network parâmetro para todos os rádios)
AT+BAND=915000000 (frequência de operação, consulte as frequências aprovadas)
```

## Projeto FINAL

**Esquema de ligação do dispositivo que RECEBE os dados do sensor**

```
DHT11 <--> NodeMCU
VCC    --  3v3
GND    --  G
Data   --  D2
```
```
RYLR998 <--> NodeMCU
VDD      --  3v3
GND      --  G
TXD      --  D7
RXD      --  D8
```

**Esquema de ligação do dispositivo que RECEBE os dados do sensor**

```
RYLR998 <--> NodeMCU
VDD      --  3v3
GND      --  G
TXD      --  D7
RXD      --  D8
```
