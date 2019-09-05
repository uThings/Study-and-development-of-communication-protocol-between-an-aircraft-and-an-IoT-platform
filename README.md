# Studio e sviluppo di un protocollo di comunicazione tra drone e dispositivo IoT

Questa è la repository del codice sviluppato per la laurea di Giacomo Andrioli in Ingegneria Elettronica all'Università degli Studi di Udine.
Per informazioni sul progetto e sul codice si veda il documento di tesi.

## Requisiti

Il codice è stato testato sui seguenti OS:
  - Linux Ubuntu 14.04
  - Linux Ubuntu 16.04
  - Linux Ubuntu 18.04

## Struttura della repository

Nella repository sono presenti:
  - Applicativo client dimostrativo;
  - Applicativo server dimostrativo;
  - Esempi di messaggi JSON;
  - Codice per la misurazione dei ritardi di elaborazione.


## Compilazione

In ogni cartella di un applicativo è presente una cartella `build` con all'interno un makefile.

Per compilare è sufficiente utilizzare il comando `make` .
