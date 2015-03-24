# Proiect PA - Warlight AI Challenge 2 - Etapa 1 24.03.2015

## Echipa AproapeBine (Cruceru Călin, Dumitru Mihai, Oancea Radu, Ursu Vlad)


Pentru această etapă a proiectului am ales o abordare Greedy. Strategia gândită
de noi se împarte în două părți principale:
* Faza de alegere a regiunilor inițiale
* Faza de alegere a deployment-urilor și atacurilor din timpul fiecărei runde.

Am separat aceste 2 părți din strategia botului nostru deoarece în primul rând
am găsit importantă posibilitatea schimbării ușoare a celor 2 strategii în mod
independent și, în al doilea rând, ele pot influența într-o mare măsură
parcursul ulterior al jocului.

Pentru prima fază, cea de alegere a regiunilor inițiale, am ales să ținem cont
în principal de numărul de wasteland-uri din regiunile din care putem alege și
să le favorizăm pe acelea care au cât mai puține. De asemenea, în caz de
egalitate, botul nostru se bazează pe raportul dintre reward-ul oferit de super
regiunea din care face parte regiunea curenta și numărul de regiuni din aceași
super regiune.

În a doua fază, cea de alegere a deployment-urilor și a atacurilor, strategia
noastră selectează acele regiuni deținute de adversari (incluzând oponentul și
regiunile neutre) care fac parte din super regiunile care sunt primele după
scorurile date de noi. Acest sistem de ranking încearcă să "avantajeze" super
regiunile în care potențialul nostru de atac este mai mare, precum și reward-ul
primit după cucerirea acesteia. Apoi, în același mod Greedy, încercă să cucerim
cât mai multe dintre regiunile menționate mai devreme. În cele din urmă,
strategia noastră ține cont și de situația în care avem armate rămase pe regiuni
izolate de adversari și încearcă să le "migreze" către zone cu adversari,
calculând distanța minimă până la primul adversar, folosind Breadth First
Search.
