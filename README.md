# Programador AT29C010A com Arduino Mega + Cartão SD

**Programador paralelo completo para a Flash EEPROM AT29C010A (128KB)** usando Arduino Mega 2560 e módulo SD Card.

---

## Recursos

- ✅ **Dump completo** da memória (salva como `Dump.bin`)
- ✅ **Gravação** a partir do arquivo `Flash.bin` (com desbloqueio SDP)
- ✅ **Verificação** automática após gravação
- ✅ **Chip Erase** completo (apaga toda a memória)
- ✅ Listagem e exclusão de arquivos no cartão SD
- ✅ Progresso em tempo real
- ✅ Gravação otimizada por setor de 128 bytes
- ✅ Interface simples via Serial Monitor (115200 baud)

---

### Tabela de Conexões - AT29C010A ↔ Arduino Mega 2560

| Pino AT29C010A | Nome   | Conexão Arduino Mega | Observação              |
|----------------|--------|----------------------|-------------------------|
| 1              | NC     | Não ligar            | Não conectar            |
| 2              | A16    | 38                   | Endereço                |
| 3              | A15    | 37                   | Endereço                |
| 4              | A12    | 34                   | Endereço                |
| 5              | A7     | 29                   | Endereço                |
| 6              | A6     | 28                   | Endereço                |
| 7              | A5     | 27                   | Endereço                |
| 8              | A4     | 26                   | Endereço                |
| 9              | A3     | 25                   | Endereço                |
| 10             | A2     | 24                   | Endereço                |
| 11             | A1     | 23                   | Endereço                |
| 12             | A0     | 22                   | Endereço                |
| 13             | I/O0   | 42                   | Dados                   |
| 14             | I/O1   | 43                   | Dados                   |
| 15             | I/O2   | 44                   | Dados                   |
| 16             | GND    | GND                  | Terra                   |
| 17             | I/O3   | 45                   | Dados                   |
| 18             | I/O4   | 46                   | Dados                   |
| 19             | I/O5   | 47                   | Dados                   |
| 20             | I/O6   | 48                   | Dados                   |
| 21             | I/O7   | 49                   | Dados                   |
| 22             | CE#    | 4                    | Chip Enable             |
| 23             | A10    | 32                   | Endereço                |
| 24             | OE#    | 3                    | Output Enable           |
| 25             | A11    | 33                   | Endereço                |
| 26             | A9     | 31                   | Endereço                |
| 27             | A8     | 30                   | Endereço                |
| 28             | A13    | 35                   | Endereço                |
| 29             | A14    | 36                   | Endereço                |
| 30             | NC     | Não ligar            | Não conectar            |
| 31             | WE#    | 2                    | Write Enable            |
| 32             | VCC    | 5V                   | Alimentação (+5V)       |

---

## Como Usar

1. Monte o circuito conforme a pinagem acima.
2. Insira um cartão SD formatado em **FAT16**.
3. Para gravar: coloque o arquivo `Flash.bin` na raiz do cartão SD.
4. Abra o **Serial Monitor** em **115200 baud**.
5. Escolha a opção desejada no menu:
6. Deletar arquivos do SD


---

## Materiais Necessários

- Arduino Mega 2560
- Módulo Micro SD Card (com SPI)
- Flash EEPROM AT29C010A
- Cartão Micro SD
- Protoboard + fios jumper
- Fonte 5V estável

---

## Próximas Melhorias (Roadmap)

- Suporte a outros chips da família (29C020, 29C040, etc.)
- Leitura/escrita de endereços específicos
- Interface com display LCD ou OLED
- Suporte a nomes de arquivos customizados
- Versão com ZIF socket

**Sinta-se à vontade para contribuir!** Pull requests são bem-vindos.

---

## Licença

Este projeto está licenciado sob a **MIT License** — sinta-se livre para usar, modificar e distribuir.

---

**Feito com ❤️ para a comunidade maker e de eletrônica retrô.**

Qualquer dúvida ou sugestão, é só abrir uma Issue!

