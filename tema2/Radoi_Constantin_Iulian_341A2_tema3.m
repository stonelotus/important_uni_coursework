%%%%% Nume_student: Radoi Constantin-Iulian
%%%%% Grupa: 341A2
%%%%% Laborator 3 - 2022
%%%%% Modele Parametrice - Predictii ARX, ARMAX, SS

function [ hair_dryer_stat ] = hair_dryer()

   % 1. Incarcati datele experimentale: hair_dryer.mat.
   hair_dryer_data = load('hair_dryer.mat');

   % 2. Reprezentati grafic intrarea si iesirea systemului. Fiecare pe cate un grafic, iar in final, intr-un grafic trasatile si impreuna. Afisati toate aceste grafice in aceeasi figura
   % 3. Completati Figura/graficele cu titlu, etichete pentru axe si legenda (unde este cazul). Intrarea este reprezentata de o serie de tensiuni, pe cand iesirea este o temperatura.
   
   figure;
   %tiledlayout(3,2)
   subplot(3,2,1)
   plot(hair_dryer_data.u);
   title('Input');
   xlabel('Time(s)')
   ylabel('Voltage(V)')
   
   subplot(3,2,3);
   plot(hair_dryer_data.y);
   title('Output');
   xlabel('Time (s)')
   ylabel('Temperature (C)')

   subplot(3,2,5);
   plot(hair_dryer_data.u);
   hold;
   plot(hair_dryer_data.y);
   title('Both Output And Input');
   xlabel('Time (s)');
   legend('Input', 'Output');
   
   % 4. Eliminati componenta continua din semnalele de intrare si iesire (functia detrend).
   y_stat = detrend(hair_dryer_data.y, 'constant');
   u_stat = detrend(hair_dryer_data.u, 'constant');

   % 5. Afisati grafic noile semnale obtinute dupa eliminarea componentei continue (pe acelasi grafic cu semnalele corespunzatoare inainte de eliminarea componentei continue).
   subplot(3,2,2)
   plot(u_stat);
   title('Detrend Constant INPUT')
   xlabel('Time (s)');
   ylabel('Voltage (V)');

   subplot(3,2,4)
   plot(y_stat);
   title('Detrend Constant OUTPUT')
   xlabel('Time (s)');
   ylabel('Temperature (V)');

   subplot(3,2,6)
   plot(u_stat);
   hold
   plot(y_stat);
   xlabel('Time (s)');
   legend('Input','Output')
   title('Both Detrend Constant I/O')

   % 8. Pe baza datelor experimentale, determinati timpul mort (functie delayest).
   hair_dryer_stat = iddata(y_stat,u_stat);
   nk = delayest(hair_dryer_stat);
   
   % 9. Identificati modelele parametrice folosind datele experimentale fara componenta continua, si anume modelele: armax, arx si spatiul starilor (m4sid).   
   na = 1;
   nb = 2;
   nc = 10;
   nr_stari = 4;

   M1 = arx(hair_dryer_stat,[na nb nk]);
   M2 = armax(hair_dryer_stat,[na nb nc nk]);
   M3 = idpoly(n4sid(hair_dryer_stat,nr_stari));
   
  
   % 11. Calculati si afisati grafic iesirea pentru fiecare model estimat (functie predict).
   y_sim1 = predict(M1, hair_dryer_stat);
   y_sim2 = predict(M2, hair_dryer_stat);
   y_sim3 = predict(M3, hair_dryer_stat);
    
   figure
   subplot(3,1,1);
   plot(y_sim1);
   title('ARX Prediction')

   subplot(3,1,2);
   plot(y_sim2);
   title('ARMAX Prediction')

   subplot(3,1,3);
   plot(y_sim3);
   title('State-Space Prediction')
   

   % 12 Calculati eroarea intre iesirea modelului estimat si iesirea reala, pentru fiecare model estimat. Afisati rezultatele grafic

   er1_dif = y_stat - y_sim1.y;
   er2_dif = y_stat - y_sim2.y;
   er3_dif = y_stat - y_sim3.y;

   er1_pe = pe(M1,hair_dryer_stat);
   er2_pe = pe(M2,hair_dryer_stat);
   er3_pe = pe(M3,hair_dryer_stat);

   % 13. Concluzionati ceea ce ati lucrat in timpul laboratorului.
   figure
   subplot(3,2,1);
   plot(er1_dif);
   title('ARX Diff Error');
   subplot(3,2,2);
   plot(er1_pe);
   title('ARX PE Error');

   subplot(3,2,3);
   plot(er2_dif);
   title('ARMAX Diff Error')
   subplot(3,2,4);
   plot(er2_pe);
   title('ARMAX PE Error');

   subplot(3,2,5);
   plot(er3_dif);
   title('State-Space Diff Error')
   subplot(3,2,6);
   plot(er3_pe);
   title('State-Space PE Error');

   M1.noiseVariance % 0.0056
   M2.noiseVariance % 0.0052
   M3.noiseVariance % 0.0026

   %%% State Space prediction has a much smaller noise variance than ARMAX
   %%% and ARX predictions. ARMAX and ARX predictions' noise variances are
   %%% close to each other.
 
end
