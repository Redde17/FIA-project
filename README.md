# MazeAI

Questo progetto mira a esplorare algoritmi di ricerca tramite la generazione e la ricerca di un percorso di un labirinto analizzando le performance temporali degli algoritmi utilizzati.

## Contenuti
La repository contiene tutto il necessario per mettere su un ambiente di sviluppo e compilare le build necessarie. In particolare, è presente la libreria SFML utilizzata per lo sviluppo.

## Installazione
Il programma, se preso tramite release, non necessita di nessuna installazione.

## Come creare una build
Per generare una build, una volta clonata la repo è possibile generare una build che sia debug o release, prima di avviare però le build necessitano dei file `.dll` della libreria SFML. 
Questi file sono collocati nella cartella `lib\SFML-2.6.1\bin` è consigliabile usare i seguenti `.dll` a seconda di che tipo di build si sta cercando di creare:  

Debug: 
```
    sfml-graphics-d-2.dll
    sfml-window-d-2.dll
    sfml-system-d-2.dll
```
Release:
```
    sfml-graphics-2.dll
    sfml-window-2.dll
    sfml-system-2.dll
```

Copiare questi `.dll` nella cartella di build che si sta cercando di creare.

## Utilizzo
I comandi da tastiera per interagire con il programma sono i seguenti:
```
Space:     Genera e risolve un nuovo labirinto
G:         Genera un nuovo labirinto
S:         Risolve l'attuale labirinto
```

## Altro
Per eventuali problemi di linking con la libreria SFML, consultare il tutorial d'introduzione della libreria: <br/>
`https://www.sfml-dev.org/tutorials/2.6/start-vc.php`
