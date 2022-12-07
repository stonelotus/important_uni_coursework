%% Radoi Constantin-Iulian
%% 341A2
%% Tema 5 IS - MCMMP


%% OE22
%% Verificarea aplicarii MCMMP pentru identificarea parametrica a modelelor OE[2,2]

function [A_med, B_med, lambda] = lab5_OE22(A,B,N,nr)
    % Verificarea corectitudinii parametrilor de apel ai rutinei
    if (nargin < 4 || isempty(nr))
        nr = 100;
    end
    if (nargin < 3 || isempty(N))
        N = 100;
    end
    if (nargin < 2 || isempty(B))
        B = [0.27 0.1];
    end
    if (nargin < 1 || isempty(A))
        A = [-0.4 -0.4];
    end
    
    B = [0 B];
    A = [1 A];
    
    %TODO check A, B stability
       
    % constante si variabile
    lambdaVar = 1;
%     A_salvat = zeros(nr,1);
%     B_salvat = zeros(nr,1);
%     lambda = zeros(nr,1);
%     
    % 2. Generare de date
    U = sign(randn(N,nr));              % matrice de semnale de intrare spab ( cu valori 1 si -1) de dimensiune N x nr
    E1 = lambdaVar * randn(N,nr);        % matrice de semnale de perturbatii cu varianta unitara, de dimensiune N x nr
    Y = filter(B,A,U) + filter(1,1,E1);  % matrice de semnale de iesire de dimensiune N x nr (aplicarea functiei filter ) = Yi + v
    Y2 = E1 - Y;
    % 3. Implementare MCMMP pentru ARX[1,1] – se aplica functia xcov
    %   a. Estimarea parametrilor, folosind nr realizari (prin mediere)
    
    for i = 1 : nr
       [ry2,K] = xcov(Y2(:,i),'biased');  % calculam covarianta ry si pivotului K, iar 'biased' calculeaza (1/N) * SUMA(...)
       ry2 = ry2(K>=0);                   % luam doar elementele mai mari ca 0
       
       [ru,K] = xcov(U(:,i),'biased');  % calculam covarianta ru si pivotului K, iar 'biased' calculeaza (1/N) * SUMA(...)
       ru = ru(K>=0);                   % luam doar elementele mai mari ca 0
       
       [ry2u,K] = xcov(Y2(:,i),U(:,i),'biased');
       ry2u = ry2u(K>=-1);
 
       [ruy,K] = xcov(U(:,i),Y(:,i),'biased');
       ruy = ruy(K>=-2);
        
       [ry2y,K] = xcov(Y2(:,i),Y(:,i),'biased');
       ry2y = ry2y(K>=-2);
       
       [ruy2,K] = xcov(U(:,i),Y2(:,i),'biased');
       ruy2 = ruy2(K>=-1);

       R = [ry2(1) ry2(2) ry2u(2) ry2u(3);
            ry2(2) ry2(1) ry2u(1) ry2u(2);
            ruy2(2) ruy2(3) ru(1) ru(2);
            ruy2(1) ruy2(2) ru(2) ru(1)];

       r = [ry2y(2); ry2y(1); ruy(2); ruy(1)];


       teta = R\r;                            % inv(R) * r
     
       A_salvat(i,:) = teta(1:2); % salvare pentru mediere in afara forului
       B_salvat(i,:) = teta(3:4);
       
       % b. Estimarea zgomotului alb si a dispersiei.

       eroare = Y2(:,i) - [0 0 0 0;
                           Y2(1,i) 0 U(1,i) 0;
                           Y2(2:(N-1),i) Y2(1:(N-2),i) U(2:(N-1),i) U(1:(N-2),i)] * teta;

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
    text(2,0.76,['Parametri estimati: ' sprintf('%9.3f', [A_med B_med])]);   % parametri adevarati
    title('Estimarea unui model OE[2,2] prin Metoda Celor Mai Mici Patrate');
    xlabel('Index realizare');
    ylabel('Varianta zgomot');
    
end
