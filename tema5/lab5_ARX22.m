%% Radoi Constantin-Iulian
%% 341A2
%% Tema 5 IS - MCMMP


%% ARX 22
%% Implementare MCMMP pentru ARX[2,2] – se aplica functia xcov
function [A_med, B_med, lambda] = lab5_ARX22(A,B,N,nr)
    % Verificarea corectitudinii parametrilor de apel ai rutinei
    if (nargin < 4 || isempty(nr))
        nr = 100;
    end
    if (nargin < 3 || isempty(N))
        N = 100;
    end
    if (nargin < 2 || isempty(B))
        B = [0.5 0.25];
    end
    if (nargin < 1 || isempty(A))
        A = [-0.4 0.2];
    end
    
    B = [0 B];
    A = [1 A];
    
    %TODO check A, B stability
       
    % constante si variabile
    lambdaVar = 1;
%     A_salvat = zeros(nr,2);
%     B_salvat = zeros(nr,2);
%     lambda = zeros(nr,2);
%     
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
       ryu = ryu(K>=-1);
        
       
       R = [ry(1) ry(2) -ryu(2) -ryu(3);    % matricea realizarilor
            ry(2) ry(1) -ryu(1) -ryu(2);
            -ryu(2) -ryu(1) ru(1) ru(2);    % ryu(2) = ruy(0); ryu(1) = ruy(1); ryu(3) = ruy(-1)
            -ryu(3) -ryu(2) ru(2) ru(1)];

       r = [-ry(2); -ry(3); ryu(3); ryu(4)];  % construire vector liber
       teta = R\r;                            % inv(R) * r
     
       A_salvat(i,:) = teta(1:2); % salvare pentru mediere in afara forului
       B_salvat(i,:) = teta(3:4);
       
       % b. Estimarea zgomotului alb si a dispersiei.

       eroare = Y(:,i) - [0 0 0 0; 
                          -Y(1,i) 0 U(1,i) 0;
                          -Y(2:(N-1),i) -Y(1:(N-2),i) U(2:(N-1),i) U(1:(N-2),i)] * teta;  % zgomot

       lambda(i) = norm(eroare) / sqrt(N-4);          
        
       
    end
    A_med = mean(A_salvat); % medierea estimarilor
    B_med = mean(B_salvat);
    
    % c. Afisare pe acelasi grafic: parametrii adevarati, parametrii estimimati, dispersia z.a. la fiecare realizare.
    figure;
    plot(lambda.^2, '--b'); % dispersia la fiecare realizare
    hold on;
    plot(ones(100),'r');    
    hold off;
    text(2,0.7,['Parametri adevarati: ' sprintf('%9.3f', [A(2) A(3) B(2) B(3)])]);     % parametri adevarati
    text(2,0.72,['Parametri estimati: ' sprintf('%9.3f', [A_med B_med])]);   % parametri adevarati
    title('Estimarea unui model ARX[2,2] prin Metoda Celor Mai Mici Patrate');
    xlabel('Index realizare');
    ylabel('Varianta zgomot');
    
end
