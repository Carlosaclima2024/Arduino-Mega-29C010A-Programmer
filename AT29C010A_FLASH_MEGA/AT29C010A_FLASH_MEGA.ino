// =============================================
//  AT29C010A 128KB Programmer - VERSÃO FINAL
// Arduino Mega
//  Pinagem conforme tabela 
// =============================================
/* 
Pino AT29C010A    Nome   Conexão no Arduino Mega  Observação
1                  NC           Não ligar
2                  A16          38                Endereço
3                  A15          37                Endereço
4                  A12          34                Endereço
5                  A7           29                Endereço
6                  A6           28                Endereço
7                  A5           27                Endereço
8                  A4           26                Endereço
9                  A3           25                Endereço
10                 A2           24                Endereço
11                 A1           23                Endereço
12                 A0           22                Endereço
13                 I/O0         42                DADOS
14                 I/O1         43                DADOS
15                 I/O2         44                DADOS
16                 GND          GND               Terra
17                 I/O3         45                Dados
18                 I/O4         46                Dados
19                 I/O5         47                Dados
20                 I/O6         48                Dados
21                 I/O7         49                Dados
22                 CE#          4                CHIP ENABLE
23                 A10          32                ENDEREÇO
24                 OE#           3                Output Enable
25                 A11          33                ENDEREÇO
26                 A9           31                Endereço
27                 A8           30                Endereço
28                 A13          35                Endereço
29                 A14          36               Endereço
30                 NC
31                 WE#          2                 Output Enable
32                 VCC          5V                Alimentação
*/

#include <SPI.h>
#include <SD.h>

// Pino CS para o módulo de Cartão SD no Arduino Mega
const int SD_CS_PIN = 53; 

// Pinos de Controle da AT29C010A
const int WE_PIN = 2;   // Pino 31 da EEPROM
const int OE_PIN = 3;   // Pino 24 da EEPROM
const int CE_PIN = 4;   // Pino 22 da EEPROM

// Total de memória da AT29C010A: 1 Megabit = 131.072 Bytes (128 KB)
const unsigned long CHIP_SIZE = 131072; 
const int SECTOR_SIZE = 128; // Tamanho do setor nativo da Flash

// Mapeamento do seu barramento de Endereço (A0 até A16)
const int addrPins[] = {
  22, 23, 24, 25, 26, 27, 28, 29, // A0 - A7
  30, 31, 32, 33, 34, 35, 36, 37, // A8 - A15
  38                              // A16
};

// Mapeamento do seu barramento de Dados (I/O0 até I/O7)
const int dataPins[] = {
  42, 43, 44, 45, 46, 47, 48, 49
};

// Buffer global temporário para 1 setor (128 bytes)
byte sectorBuffer[SECTOR_SIZE];

// === DECLARAÇÃO DOS PROTÓTIPOS DAS FUNÇÕES ===
void exibirMenu();
void fazerDumpParaSD();
void gravarFlashDoSD();
bool verificarFlashComSD();
void apagarMemoriaCompleta();
void listarDiretorioSD();
void menuDeletarArquivos();
void setAddress(unsigned long address);
void setDataDirection(int mode);
byte readByte(unsigned long address);
void writeByteRaw(unsigned long address, byte data);

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; } 
  
  pinMode(WE_PIN, OUTPUT);
  pinMode(OE_PIN, OUTPUT);
  pinMode(CE_PIN, OUTPUT);
  
  digitalWrite(WE_PIN, HIGH);
  digitalWrite(OE_PIN, HIGH);
  digitalWrite(CE_PIN, HIGH);
  
  for(int i = 0; i < 17; i++) {
    pinMode(addrPins[i], OUTPUT);
  }

  Serial.print(F("Inicializando cartao SD... "));
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println(F("ERRO: Falha no SD ou cartao nao inserido!"));
    while(1); 
  }
  Serial.println(F("SD Pronto!"));
  
  exibirMenu();
}

void loop() {
  if (Serial.available() > 0) {
    char escolha = Serial.read();
    
    while(Serial.available() > 0) { Serial.read(); }

    if (escolha == '1') {
      fazerDumpParaSD();
      exibirMenu();
    } 
    else if (escolha == '2') {
      gravarFlashDoSD();
      exibirMenu();
    }
    else if (escolha == '3') {
      verificarFlashComSD();
      exibirMenu();
    }
    else if (escolha == '4') {
      apagarMemoriaCompleta();
      exibirMenu();
    }
    else if (escolha == '5') {
      listarDiretorioSD();
      exibirMenu();
    }
    else if (escolha == '6') {
      menuDeletarArquivos();
      exibirMenu();
    }
  }
}

void exibirMenu() {
  Serial.println(F("\n====================================="));
  Serial.println(F("     PROGRAMADOR AT29C010A + SD      "));
  Serial.println(F("====================================="));
  Serial.println(F(" 1. Ler Memoria e salvar como (Dump.bin)"));
  Serial.println(F(" 2. Gravar Memoria a partir de (Flash.bin)"));
  Serial.println(F(" 3. Verificar apenas (Comparar Chip vs Flash.bin)"));
  Serial.println(F(" 4. Apagar Memoria Completa (Chip Erase)"));
  Serial.println(F(" 5. Listar arquivos do SD (DIR)"));
  Serial.println(F(" 6. Deletar arquivos do SD (Delete)"));
  Serial.println(F("Digite a opcao desejada: "));
}
// === FUNÇÕES AUXILIARES DE HARDWARE ===

void setAddress(unsigned long address) {
  for(int i = 0; i < 17; i++) {
    digitalWrite(addrPins[i], (address >> i) & 0x01);
  }
}

void setDataDirection(int mode) {
  for(int i = 0; i < 8; i++) {
    pinMode(dataPins[i], mode);
  }
}

byte readByte(unsigned long address) {
  setDataDirection(INPUT);
  setAddress(address);
  
  digitalWrite(CE_PIN, LOW);
  digitalWrite(OE_PIN, LOW);
  delayMicroseconds(1);
  
  byte data = 0;
  for(int i = 0; i < 8; i++) {
    data |= (digitalRead(dataPins[i]) << i);
  }
  
  digitalWrite(OE_PIN, HIGH);
  digitalWrite(CE_PIN, HIGH);
  return data;
}

void writeByteRaw(unsigned long address, byte data) {
  setDataDirection(OUTPUT);
  setAddress(address);
  
  for(int i = 0; i < 8; i++) {
    digitalWrite(dataPins[i], (data >> i) & 0x01);
  }
  
  digitalWrite(CE_PIN, LOW);
  digitalWrite(WE_PIN, LOW);
  delayMicroseconds(1); 
  digitalWrite(WE_PIN, HIGH);
  digitalWrite(CE_PIN, HIGH);
}

// === OPERAÇÃO 1: DUMP (LEITURA COMPLETA) ===
void fazerDumpParaSD() {
  Serial.println(F("\nIniciando DUMP da memoria..."));
  
  if (SD.exists("Dump.bin")) {
    SD.remove("Dump.bin");
  }
  
  File dumpFile = SD.open("Dump.bin", FILE_WRITE);
  if (!dumpFile) {
    Serial.println(F("ERRO: Nao foi possivel criar o arquivo Dump.bin no SD."));
    return;
  }
  
  unsigned long progressoAntigo = 0;
  
  for (unsigned long addr = 0; addr < CHIP_SIZE; addr++) {
    byte dado = readByte(addr);
    dumpFile.write(dado);
    
    unsigned long progresso = (addr * 100) / CHIP_SIZE;
    if (progresso % 10 == 0 && progresso != progressoAntigo) {
      Serial.print(progresso);
      Serial.println(F("% concluido..."));
      progressoAntigo = progresso;
    }
  }
  
  dumpFile.close();
  Serial.println(F("SUCESSO: Total de 128KB salvos em 'Dump.bin'!"));
}

// === OPERAÇÃO 2: FLASH (GRAVAÇÃO POR SETOR - CORRIGIDA) ===
void gravarFlashDoSD() {
  if (!SD.exists("Flash.bin")) {
    Serial.println(F("\nERRO: O arquivo 'Flash.bin' nao foi encontrado na raiz do SD!"));
    return;
  }
  
  File flashFile = SD.open("Flash.bin", FILE_READ);
  if (!flashFile) {
    Serial.println(F("ERRO: Falha ao abrir o arquivo Flash.bin para leitura."));
    return;
  }
  
  unsigned long tamanhoArquivo = flashFile.size();
  Serial.print(F("\nArquivo carregado. Tamanho: "));
  Serial.print(tamanhoArquivo);
  Serial.println(F(" bytes."));
  
  Serial.println(F("Gravando na AT29C010A... Por favor, aguarde."));
  
  unsigned long baseAddress = 0;
  int totalSetores = CHIP_SIZE / SECTOR_SIZE; 
  
  digitalWrite(OE_PIN, HIGH); 

  for (int setor = 0; setor < totalSetores; setor++) {
    baseAddress = (unsigned long)setor * SECTOR_SIZE;
    
    for (int i = 0; i < SECTOR_SIZE; i++) {
      if (flashFile.available()) {
        sectorBuffer[i] = flashFile.read();
      } else {
        sectorBuffer[i] = 0xFF; 
      }
    }
    
    // Desbloqueio SDP (Software Data Protection)
    writeByteRaw(0x5555, 0xAA);
    writeByteRaw(0x2AAA, 0x55);
    writeByteRaw(0x5555, 0xA0);
    
    // Grava setor contínuo
    for (int i = 0; i < SECTOR_SIZE; i++) {
      writeByteRaw(baseAddress + i, sectorBuffer[i]);
    }
    
    delay(12); // Tempo tWC do chip
    
    // EXIBIÇÃO DE PROGRESSO CORRIGIDA SEM ESTOURO (Formatado para 32-bits com 'UL')
    if (setor % 128 == 0) {
      Serial.print(F("Progresso Gravacao: "));
      unsigned long progresso = ((unsigned long)setor * 100UL) / totalSetores;
      Serial.print(progresso);
      Serial.println(F("%"));
    }
  }
  
  flashFile.close();
  Serial.println(F("SUCESSO: Processo de gravacao concluido!"));
  
  verificarFlashComSD();
}


// === OPERAÇÃO 3: VERIFY ===
bool verificarFlashComSD() {
  Serial.println(F("\nIniciando VERIFICACAO de dados..."));
  
  if (!SD.exists("Flash.bin")) {
    Serial.println(F("ERRO: Arquivo 'Flash.bin' nao encontrado para comparacao."));
    return false;
  }
  
  File flashFile = SD.open("Flash.bin", FILE_READ);
  if (!flashFile) {
    Serial.println(F("ERRO: Falha ao abrir o arquivo Flash.bin para verificacao."));
    return false;
  }
  
  unsigned long errosEncontrados = 0;
  unsigned long progressoAntigo = 0;
  
  for (unsigned long addr = 0; addr < CHIP_SIZE; addr++) {
    byte byteEsperado = 0xFF; 
    if (flashFile.available()) {
      byteEsperado = flashFile.read();
    }
    
    byte byteLidoDoChip = readByte(addr);
    
    if (byteLidoDoChip != byteEsperado) {
      errosEncontrados++;
      if (errosEncontrados <= 10) {
        Serial.print(F("ERRO no Endereco 0x"));
        Serial.print(addr, HEX);
        Serial.print(F(" -> Esperado: 0x"));
        Serial.print(byteEsperado, HEX);
        Serial.print(F(" | Lido: 0x"));
        Serial.println(byteLidoDoChip, HEX);
      }
    }
    
    unsigned long progresso = (addr * 100) / CHIP_SIZE;
    if (progresso % 20 == 0 && progresso != progressoAntigo) {
      Serial.print(F("Verificando: "));
      Serial.print(progresso);
      Serial.println(F("%"));
      progressoAntigo = progresso;
    }
  }
  
  flashFile.close();
  
  if (errosEncontrados == 0) {
    Serial.println(F("⭐⭐⭐⭐⭐ VERIFICACAO OK! 100% IDENTICO! ⭐⭐⭐⭐⭐"));
    return true;
  } else {
    Serial.println(F("❌❌❌❌❌ FALHA NA VERIFICACAO! ❌❌❌❌❌"));
    Serial.print(F("Total de bytes corrompidos/divergentes: "));
    Serial.println(errosEncontrados);
    return false;
  }
}

// === OPERAÇÃO 4: CHIP ERASE ===
void apagarMemoriaCompleta() {
  Serial.println(F("\nApagando toda a memoria (Chip Erase)... Por favor, aguarde."));
  
  digitalWrite(OE_PIN, HIGH); 
  
  writeByteRaw(0x5555, 0xAA);
  writeByteRaw(0x2AAA, 0x55);
  writeByteRaw(0x5555, 0x80);
  writeByteRaw(0x5555, 0xAA);
  writeByteRaw(0x2AAA, 0x55);
  writeByteRaw(0x5555, 0x10);
  
  delay(50); 
  
  Serial.println(F("Verificando se o apagamento foi bem-sucedido..."));
  unsigned long errosDeApagamento = 0;
  
  for (unsigned long addr = 0; addr < CHIP_SIZE; addr++) {
    if (readByte(addr) != 0xFF) {
      errosDeApagamento++;
      if (errosDeApagamento <= 5) {
        Serial.print(F("Falha ao apagar no endereco 0x"));
        Serial.println(addr, HEX);
      }
    }
  }
  
  if (errosDeApagamento == 0) {
    Serial.println(F("⭐⭐⭐⭐⭐ CHIP COMPLETAMENTE APAGADO (TUDO 0xFF)! ⭐⭐⭐⭐⭐"));
  } else {
    Serial.println(F("❌❌❌❌❌ ERRO: Alguns setores nao foram apagados corretamente. ❌❌❌❌❌"));
    Serial.print(F("Total de bytes nao apagados: "));
    Serial.println(errosDeApagamento);
  }
}

// === OPERAÇÃO 5: LISTAR DIRETÓRIO DO SD (DIR) ===
void listarDiretorioSD() {
  Serial.println(F("\n--- ARQUIVOS ENCONTRADOS NO CARTÃO SD ---"));
  
  File raiz = SD.open("/");
  if (!raiz) {
    Serial.println(F("ERRO: Nao foi possivel abrir o diretorio raiz."));
    return;
  }
  
  int contadorArquivos = 0;
  
  while (true) {
    File arquivoAtual = raiz.openNextFile();
    if (!arquivoAtual) {
      break; 
    }
    
    if (!arquivoAtual.isDirectory()) {
      contadorArquivos++;
      Serial.print(F("📄 "));
      Serial.print(arquivoAtual.name());
      Serial.print(F("\t\tTamanho: "));
      Serial.print(arquivoAtual.size());
      Serial.println(F(" bytes"));
    }
    arquivoAtual.close();
  }
  raiz.close();
  
  if (contadorArquivos == 0) {
    Serial.println(F("(O cartao SD esta vazio ou nao contem arquivos validos)"));
  }
  Serial.println(F("-----------------------------------------"));
}

// === OPERAÇÃO 6: SUB-MENU PARA DELETAR ARQUIVOS ===
void menuDeletarArquivos() {
  // LIMPEZA COMPREENSIVA DO BUFFER ANTES DE EXIBIR O TEXTO
  // Isso descarta o 'Enter' herdado da escolha do menu principal
  delay(50); 
  while (Serial.available() > 0) { 
    Serial.read(); 
  }

  Serial.println(F("\n--- SELECIONE O ARQUIVO PARA DELETAR ---"));
  Serial.println(F(" A. Deletar Flash.bin"));
  Serial.println(F(" B. Deletar Dump.bin"));
  Serial.println(F(" V. Voltar ao menu principal"));
  Serial.println(F("Digite a sua opcao (A, B ou V): "));
  
  // Agora sim aguarda pacientemente até que o usuário digite a nova opção
  while (Serial.available() == 0) { 
    delay(10); 
  }
  
  // Lê toda a string enviada até a quebra de linha do Enter
  String entrada = Serial.readStringUntil('\n');
  entrada.trim(); // Remove espaços ocultos e o caractere \r
  
  // Valida se a entrada não veio vazia
  if (entrada.length() == 0) {
    Serial.println(F("Opcao vazia! Operacao cancelada."));
    return;
  }
  
  // Isola a primeira letra e converte para maiúscula
  char subEscolha = toupper(entrada.charAt(0));
  
  if (subEscolha == 'A') {
    if (SD.exists("Flash.bin")) {
      SD.remove("Flash.bin");
      Serial.println(F("🗑️ SUCESSO: O arquivo 'Flash.bin' foi deletado!"));
    } else {
      Serial.println(F("❌ ERRO: 'Flash.bin' nao existe no cartao SD."));
    }
  } 
  else if (subEscolha == 'B') {
    if (SD.exists("Dump.bin")) {
      SD.remove("Dump.bin");
      Serial.println(F("🗑️ SUCESSO: O arquivo 'Dump.bin' foi deletado!"));
    } else {
      Serial.println(F("❌ ERRO: 'Dump.bin' nao existe no cartao SD."));
    }
  } 
  else if (subEscolha == 'V') {
    Serial.println(F("Voltando ao menu principal..."));
  } 
  else {
    Serial.print(F("Opcao '"));
    Serial.print(subEscolha);
    Serial.println(F("' invalida! Operacao cancelada."));
  }
}
