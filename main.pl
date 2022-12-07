%%%	Gestiunea populatiei care trece vama


  %%% Fapte

  % punct_vama(Denumire)
  punct_vama('Giurgiu').
  punct_vama('Tulcea').
  punct_vama('Zimnicea').
  punct_vama('Oltenita').
  punct_vama('Portile de Fier 1').
  punct_vama('Portile de Fier 2').
  punct_vama('Mangalia').
  punct_vama('Aeroportul International Henry Coanda').
  punct_vama('Aeroportul International Craiova').
  punct_vama('Aeroportul International Aurel Vlaicu').
  punct_vama('Aeroportul International Bacau').
  punct_vama('Braila').

  control_sporit('Giurgiu').
  control_sporit('Mangalia').
  control_sporit('Bulgaria').
  control_sporit('Romania').

  % persoana(Prenume,Nume,CNP)
  persoana('Mihai',	'Albastru', '5001020180040',54).
  persoana('Geore', 	'Popescu',	'5001020180041',34).
  persoana('Grigore',	'Moisil',	'5001020180042',34).
  persoana('Mircea',	'Diaconu',	'5001020180043',10).
  persoana('Bianca',	'Brenciu',	'5001020180044',32).
  persoana('Mihalea',	'Amalgam',	'5001020180045',21).

  % buletin(CNP, Serie, Numar, Adresa, Gen, Cetatenie, Data_Expirare)
  buletin('5001020180040', 'GZ', '712157', 'Bucuresti, Theodor Aman 5', 'M', 'ROU', '20.10.2030').
  buletin('5001020180041', 'GZ', '712158', 'Bucuresti, Splaiul Independentei 200', 'M', 'ROU', '21.10.2030').
  buletin('5001020180042', 'CZ', '712234', 'Gorj, Theodor Aman', 'M', 'CZH', '22.10.2030').
  buletin('5001020180043', 'AC', '144234', 'Bucuresti, Eroilor', 'M', 'ROU', '23.10.2030').
  buletin('5001020180044', 'GZ', '512413', 'Ploiesti, Calea Verde', 'F', 'ROU', '24.10.2030').
  buletin('5001020180045', 'CZ', '993233', 'Teleorman, Repelu', 'M', 'UNG', '25.10.2030').

  % trece_vama(CNP, punct, sens, data, ora); sens -> 1 = vine in tara, 0 = pleaca din tara
  trece_vama('5001020180040','Oltenita','1','23.12.2021','10:00').
  trece_vama('5001020180043','Mangalia','1','13.01.2021','13:23').
  trece_vama('5001020180045','Braila'  ,'0','20.10.2022','1:37').
  trece_vama('5001020180044','Aeroportul International Aurel Vlaicu','0','20.10.2022','10:00').
  trece_vama('5001020180040','Oltenita','0','24.12.2021','11:00').

  % cautat_politie(CNP).
  cautat_politie('5001020180040').
  masina_cautata('5001020180040').

  % detine_pasaport(CNP, raspuns) -> raspuns: 1 = detine pasaport, 0 = nu detine pasaport  
  detine_pasaport('5001020180040').
  detine_pasaport('5001020180043').
  detine_pasaport('5001020180045').
  detine_pasaport('5001020180044').
  detine_pasaport('5001020180040').

  % verificare apartinere lista
  apartine(X,[X|_]):-!.
  apartine(X,[_|Y]):-apartine(X,Y).

  %trece_cu_animale(CNP)
  trece_cu_animale('5001020180040').
  trece_cu_animale('5001020180041').
  %detine_pasaport_animal_companie(CNP)
  detine_pasaport_animal_companie('5001020180041').

  %certificat_covid(CNP)
  certificat_covid('5001020180040').
  certificat_covid('5001020180041').
  certificat_covid('5001020180042').
  certificat_covid('5001020180043').
 
  %detine_electronice_mari(CNP).
  detine_electronice_mari('5001020180040').
  detine_electronice_mari('5001020180042').

  %detine_electronice_mari(CNP).
  detine_lichide_speciale('5001020180043').
  detine_lichide_speciale('5001020180042').

  %isi_cunoaste_bagajul(CNP).
  isi_cunoaste_bagajul('5001020180040').
  isi_cunoaste_bagajul('5001020180041').
  isi_cunoaste_bagajul('5001020180042').

  %bani_cash(CNP, suma)
  bani_cash('5001020180040',100).
  bani_cash('5001020180041',32).
  bani_cash('5001020180042',2342).
  bani_cash('5001020180045',10001).


  % are_copil(CNP_parinte,nume_copil)
  are_copil('5001020180040', 'Darius').
  are_copil('5001020180040', 'Doru').
  are_copil('5001020180042', 'Mihai').
  
  % Reguli

  are_mai_multi_copii(CNP):-
    are_copil(CNP,_),
    numar_copii(CNP,Count),
    Count > 1.
	
  verificare_acte_copil(CNP):-
    are_copil(CNP,_).
    
  verificare_acte_masina(CNP,Data,Ora):-
    trece_la_sol(CNP,Data,Ora).

  punct_vamal_in_control_sporit(Punct):-
      punct_vama(Punct),
      control_sporit(Punct).

  verificare_obiecte_interzise(CNP):-
      prea_multi_bani_cash(CNP);
      not(isi_cunoaste_bagajul(CNP)).

  arestat_la_vama(CNP):-
      masina_cautata(CNP);
      cautat_politie(CNP).

  prea_multi_bani_cash(CNP):-
      bani_cash(CNP,Suma_Bani),
      Suma_Bani > 10000.

  persoana_suspecta(CNP):-
      persoana(_,_,CNP,_),
      (   
        detine_electronice_mari(CNP);
        detine_lichide_speciale(CNP);
        minor(CNP);
        verificare_obiecte_interzise(CNP)
      ).

  minor(CNP):-
      persoana(_,_,CNP,Varsta),
      Varsta < 18.


  eligibil_trecere_vama(CNP):-
      not(cautat_politie(CNP)),
      not(masina_cautata(CNP)),
      (   buletin(CNP,_,_,_,_,_,_);
          detine_pasaport(CNP)
      ),
      (   trece_cu_animale(CNP)->  detine_pasaport_animal_companie(CNP) ; true).

  trece_cu_certificat(CNP):-
      trece_vama(CNP,_,_,_,_),
      certificat_covid(CNP).

  numar_copii(CNP,Count):-
      findall(CNP,are_copil(CNP,_),List),
      length(List,Count).
  numar_puncte_vamale(Count):-
      findall(Punct,punct_vama(Punct),List),
      length(List,Count).

  numar_persoana_care_detin_pasaport(Count):-
      findall(CNP,detine_pasaport(CNP),List),
      length(List,Count).

  calator_frecvent(CNP):-
      findall(CNP,trece_vama(CNP,_,_,_,_),List),
      length(List,Count),
      Count > 1.

  persoane_care_detin_pasaport(CNP):-
      detine_pasaport(CNP).

  afis_lista([]):-!.
  afis_lista([H|X]):-write(H), nl, afis_lista(X).
	
  afiseaza_trecerile_vamale_persoana(CNP):-
    trece_vama(CNP,_,_,_,_),
    findall([CNP, Punct,Sens,Data,Ora],trece_vama(CNP,Punct,Sens,Data,Ora),List),
    afis_lista(List).
        
  trecut_aerian(CNP,Data,Ora):-
      trece_vama(CNP,Punct,_,Data,Ora),
      apartine(Punct, ['Aeroportul International Bacau',
                       'Aeroportul International Craiova',
                       'Aeroportul International Bacau',
                       'Aeroportul International Aurel Vlaicu']).

  trece_la_sol(CNP,Data,Ora):-
      trece_vama(CNP,Punct,_,Data,Ora),
      apartine(Punct,['Mangalia',
                       'Tulcea',
                       'Zimnicea',
                       'Oltenita',
                       'Portile de Fier 1',
                       'Portile de Fier 2',
                       'Mangalia',
                       'Braila']).
