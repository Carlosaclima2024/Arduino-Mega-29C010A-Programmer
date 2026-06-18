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

## Pinagem Completa

<details>
<summary>Clique para ver a tabela de conexões</summary>

| Pino AT29C010A | Nome     | Arduino Mega | Função          |
|----------------|----------|--------------|-----------------|
| 22             | CE#      | 4            | Chip Enable     |
| 24             | OE#      | 3            | Output Enable   |
| 31             | WE#      | 2            | Write Enable    |
| 2              | A16      | 38           | Endereço        |
| 3              | A15      | 37           | Endereço        |
| 4              | A12      | 34           | Endereço        |
| 5              | A7       | 29           | Endereço        |
| ... (ver código) | ...    | ...          | ...             |

**Barramento de Dados (I/O0 a I/O7):** pinos 42 a 49  
**Barramento de Endereço (A0 a A16):** conforme definido no código

</details>

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

