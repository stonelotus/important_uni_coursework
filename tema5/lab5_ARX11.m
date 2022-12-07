%% Radoi Constantin-Iulian
%% 341A2
%% Tema 5 IS - MCMMP


%% ARX 11
function [A_med, B_med, lambda] = lab5_ARX11(A,B,N,nr)
    % 1. Verificarea corectitudinii parametrilor de apel ai rutinei
    if (nargin < 4 || isempty(nr))
        nr = 100;
    end
    if (nargin < 3 || isempty(N))
        N = 100;
    end
    if (nargin < 2 || isempty(B))
        B = 0.9;
    end
    if (nargin < 1 || isempty(A))
        A = 0.3;
    end
    
    B = [0 B];
    A = [1 A];
    
    %TODO check A, B stability
       
    % constante si variabile
    lambdaVar = 1;
    A_salvat = zeros(nr,1);
    B_salvat = zeros(nr,1);
    lambda = zeros(nr,1);
    
    % 2. Generare de date
    U = sign(randn(N,nr));              % matrice de semnale de intrare spab ( cu valori 1 si -1) de dimensiune N x nr
    E = lambdaVar * randn(N,nr);        % matrice de semnale de perturbatii cu varianta unitara, de dimensiune N x nr
    Y = filter(B,A,U) + filter(1,A,E);  % matrice de semnale de iesire de dimensiune N x nr (aplicarea functiei filter ) = Yi + v
    
    % 3. Implementare MCMMP pentru ARX[1,1] – se aplica functia xcov
    %   a. Estimarea parametrilor, folosind nr realizari (prin mediere)
    
    for i = 1 : nr
       [ry,K] = xcov(Y(:,i),'biased');  % calculam covarianta ry si pivotului K, iar 'biased' calculeaza (1/N) * SUMA(...)
       ry = ry(K>=0);                   % luam doar elementele mai mari ca 0
       
       [ru,K] = xcov(U(:,i),'biased');  % calculam covarianta ru si pivotului K, iar 'biased' calculeaza (1/N) * SUMA(...)
       ru = ru(K>=0);                   % luam doar elementele mai mari ca 0
       
       [ryu,K] = xcov(Y(:,i),U(:,i),'biased');
       ryu = ryu(K>=0);

       R = [ry(1) -ryu(1); -ryu(1) ru(1)];  % construire matrice realizari
       r = [-ry(2); ryu(2)];                % construire vector liber
       teta = R\r;                          % inv(R) * r
     
       A_salvat(i) = teta(1); % salvare pentru mediere in afara forului
       B_salvat(i) = teta(2);
       
       % b. Estimarea zgomotului alb si a dispersiei.

       eroare = Y(:,i) - [0 0; -Y(1:(N-1),i) U(1:(N-1),i)] * teta;  % zgomot
       lambda(i) = norm(eroare) / sqrt(N-2);                        % dispersia
     
    end
    A_med = mean(A_salvat); % medierea estimarilor
    B_med = mean(B_salvat);
    
    % c. Afisare pe acelasi grafic: parametrii adevarati, parametrii estimimati, dispersia z.a. la fiecare realizare.
    figure;
    plot(lambda.^2, '--b'); % dispersia la fiecare realizare
    hold on;
    plot(ones(100),'r');    
    hold off;
    text(2,0.7,['Parametri adevarati: ' sprintf('%9.3f', [A(2) B(2)])]);     % parametri adevarati
    text(2,0.72,['Parametri estimati: ' sprintf('%9.3f', [A_med B_med])]);   % parametri adevarati
    title('Estimarea unui model ARX[1,1] prin Metoda Celor Mai Mici Patrate');
    xlabel('Index realizare');
    ylabel('Varianta zgomot');
    
end
