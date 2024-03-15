# MazeAI

Questo progetto mira ad esplorare algoritmi di ricerca tramite la generazione e la ricerca di un percorso di un labirinto analizzando le perfomance temporali degli algoritmi utilizzati.

## Contenuti
La repository contiene tutto il necessario per mettere su un ambiente di sviluppo e compilare le build necessarie. In particolare é presente la libreria SFML utilizzata per lo sviluppo.

## Installazione
Il programma se preso tramite release non necessità di nessuna installazione.

## Come creare una build
Per generare una build, una volta clonata la repo é possibile generare una build che sia debug o release, prima di avviare peró le build necessitano dei file `.dll` della libreria SFML. 
Questi file sono collecati nella cartella `lib\SFML-2.6.1\bin` é consigliabile usare i seguenti `.dll` a seconda di che tipo di build si sta cercando di creare:  

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

Copiare questi .dll nella cartella di build che si sta cercando di creare.


## Altro
Per eventuali problemi di linking con la libreria SFML consultare il tutorial di introduzione della libreria: <br/>
`https://www.sfml-dev.org/tutorials/2.6/start-vc.php`
