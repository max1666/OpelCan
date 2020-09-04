# OpelCan
Opel CAN sniffer

Проект поддерживает работу со снифеером CAN - CANHacker (v2.00.01)

В проекте используется отладочная плата "BluePill" STM32F103C8T6B

## Описание периферии
### CAN

В проекте используется трансивер MCP2515.

***Трансивер необходимо запитывать от 5V.***

Поддержка скоростей шин:
- HS-Can (High speed) (HICAN) - 500 kbit/s
- MS-Can (Medium speed) (MSCAN) - 95 kbit/s
- LS-Can (Low speed) (SWCAN) - 33.3 kbit/s

#### Пины
- Can Rx - PB8
- Can Tx - PB9

### USART1

USART1 является интерфейсом вывода пользовательской информации

Настройки соединения:
- 115200 8 N 1

#### Пины
- USART1 Rx - PA10
- USART1 Tx - PA9

### USART3

USART3 является интерфейсом для взаимодействия с программой CANHacker

Настройки соединения:
- 115200 8 N 1

#### Пины
- USART3 Rx - PB11
- USART3 Tx - PB10

#### Поддержка настройки скорости шины

С помощью программы CANHacker можно "на лету" менять частоту CAN-шины

В настройках программы опция CAN Baudrate:
- 500 Kbit/s = 500 Kbit/s (HS-Can)
- 100 Kbit/s = 95 Kbit/s (MS-Can)
- 10 Kbit/s = 33.3 Kbit/s (LS-Can)