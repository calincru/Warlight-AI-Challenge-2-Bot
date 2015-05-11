# TODO List

* Fără fugărire (reverse nu tocmai ok?) - mai poate fi amanata, nu e o problema
foarte mare.
* Faza de pick. Am pierdut cateva meciuri doar pt ca am avut pick prost.
Superregiunile sunt OK, insa trebuie tinut cont si de adversar si de regiunile
noastre pe care le-am ales deja. Am putea face funcii de scor pentru fiecare
criteriu si apoi sa facem o medie ponderata. Partea grea e la gasirea
ponderilor, insa cred ca ar merge. Gen o functie care da scorul bazat pe
reward raportat la numarul de subregiuni. O functie care da scorul bazat pe
suma distantelor pana la celelalte regiuni deja alese de noi. O functie care da
scorul bazat pe suma distantelor pana la adversar.
* Adaugat faza de defense. Eventual combinata cu faza de spoil, pentru ca
se aseamana conditiile.
* Faza de expand imbunatatita astfel incat sa tina si ea cont de adversar - 
vedeti meciul asta
http://theaigames.com/competitions/warlight-ai-challenge-2/games/5550347135ec1d06ec727f5d
Incepe sa cucereasca superregiune aia in prima runda chiar daca vede ca e
inconjurat de 2 inamici. Eventual sa modificam functia de scor sa tina cont de
inamici / distanta pana la alte regiuni ale noastre.



Strategie:

Alegere:
Initial ma gandesc sa aleg superreg init bune.
Superreg ai sa o cuc cat mai rapid.
Criterii:
* bonus
* nr reg
* topologie superreg: cat mai  complet
* sa fie vecin cu o superreg buna
* in 2 clustere

Algoritm:



#Desfasurare:

Te concentrezi pe cea mai buna superreg:
* oricand in afara de:
adversaru are langa o superreg pe care o detii complet, o reg dea lui cu care
ar putea sati ia o reg  dea  ta la tura urmatoatoare cu exceptia cand  tio ia
sigur.

Joc vs Jucator:
Folosire aprox income
income ca al tau:
    nu ataci
income > al tai || outnumbered gtfo, cu excpetia  unei regiuni (criterii
apropiere tine departare adveersar)
income < al tai
    plusezi || ataci cand iei 

cand dteai decis sa te bati:
    ataci cu mult spre putin, atac intarziat (spoil)
    cucerire: alte locuri pe harta unde e bine sa apere adv

Expand:
    Ei superreg cea mai buna dupa ac crit ca la alegere
    Cea mai buna deschidere spre harta neocupatta
    Departe de adversar
