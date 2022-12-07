%%%%% Nume_student: Radoi Constantin-Iulian
%%%%% Grupa: 341A2
%%%%% Laborator 4 (7) - 2022
%%%%% Identificarea sistemelor multivariabile - Identificarea instalatiei ASTANK2

% Load data
load('data_ASTANK2.mat','Did');

% select outputs 
y1 = Did.y(:,1);
y2 = Did.y(:,2);

% select inputs
u1 = Did.u(:, 1);
u2 = Did.u(:, 2);

% remove continous component from output to compute snr later on
y1_detrend = detrend(y1, 'constant');
y2_detrend = detrend(y2, 'constant');

% make idata object for both outputs
D1 = iddata(y1, [Did.u(:,1) Did.u(:,2)]);
D2 = iddata(y2, [Did.u(:,1) Did.u(:,2)]);

% declare output arx models
y1_arx = zeros(length(y1),1);
y2_arx = zeros(length(y2),1);

% initialize awesome variables and will insert 1 polynom for each entry
Marx = cell(20,2);
v1 = zeros(length(y1),1);
v2 = zeros(length(y2),1);

y1_snr = zeros(20,1);
y2_snr = zeros(20,1);


% generate random matrix for na, nb1, nb2 
random_matrix = randi([1 30],20,3);


for i = 1 : 20
    %% exercise 1. Identificarea unui model util din datele masurate, pe baza unui model ARX multivariabil
    % extract random na, nb1, nb2 from random matrix
    na  = random_matrix(i,1);
    nb1 = random_matrix(i,2);
    nb2 = random_matrix(i,3);

    % get ARX model and save it in in 20:2 cell matrix
    Marx{i}{1} = arx(D1, [na, [nb1 nb2],  [1 1]]);
    Marx{i}{2} = arx(D2, [na, [nb1 nb2],  [1 1]]);

    % arx y1 and y2 initializing (from lab instructions)
    for n = 1 : max([na, nb1, nb2])
        y1_arx(n) = y1(n);
        y2_arx(n) = y2(n);
    end
    
    %% exercise 2. Simularea modelului util, cu obtinerea iesirii simulate ARX y, pe baza ecuatiei 

    % start from max + 1 to complete vector (to not have a case where you
    % access vector[3-na], where na is 10 (example given)
    for n = (max([na, nb1, nb2]) + 1) : length(y1)    
        % apply formula to compute y_arx model 
         for k = 1 : na
            y1_arx(n) = y1_arx(n) - y1_arx(n-k) * Marx{i}{1}.A(k+1);    % we take A(k+1) because we don't want the free coeficient (we start form a1 not a0)
            y2_arx(n) = y2_arx(n) - y2_arx(n-k) * Marx{i}{2}.A(k+1);
         end

         for k = 1 : nb1
            y1_arx(n) = y1_arx(n) + u1(n-k) * Marx{i}{1}.B{1}(k+1);
            y2_arx(n) = y2_arx(n) + u1(n-k) * Marx{i}{2}.B{1}(k+1);
         end

         for k = 1 : nb2
            y1_arx(n) = y1_arx(n) + u2(n-k) * Marx{i}{1}.B{2}(k+1);
            y2_arx(n) = y2_arx(n) + u2(n-k) * Marx{i}{2}.B{2}(k+1);
         end

    end
    
    %% exercise 3. Estimarea zgomotului rezultat, pe baza ecuatiei + SNR
    for n = 1 : length(y1)
        v1(n) = y1(n) - y1_arx(n);
        v2(n) = y2(n) - y2_arx(n);
    end

    % compute snr for each model
    y1_snr(i) = 20 * log(std(y1_detrend)/std(v1));
    y2_snr(i) = 20 * log(std(y2_detrend)/std(v2));

    % save y_arx and v for later usage
    y1_arx_all{i} = y1_arx;
    y2_arx_all{i} = y2_arx;

    v1_all{i} = v1;
    v2_all{i} = v2;
        
end

% compute optimal SNR by extracting max snr and its position
[y1_snr_arx_optimal, y1_snr_arx_optimal_index] = max(y1_snr);
[y2_snr_arx_optimal, y2_snr_arx_optimal_index] = max(y2_snr);

%use optimal snr position to extract optimal arx from array
y1_arx_optimal = y1_arx_all{y1_snr_arx_optimal_index};
y2_arx_optimal = y2_arx_all{y2_snr_arx_optimal_index};

% plot measurements vs simulations
%% ARX 1 

figure
plot(y1);
hold on;
plot(y1_arx_optimal);

title('y1 Measured vs y1 ARX')
legend('Measured y1', 'Predicted y1')

hold off;

%% ARX 2

figure

plot(y2);
hold on;
plot(y2_arx_optimal);

title('y2 Measured vs y2 ARX')
legend('Measured y2', 'Predicted y2')
hold off;


%% exercise 4. Dupa gasirea modelului optim ARX, estimati 10 modele ARMA pentru perturbatia v, folosind functia armax, cu indici variind in intervalul [50, 90].
random_matrix = randi([51 62],10,2);
Marma = cell(10,2);

v1_arma_snr = zeros(10,1);
v2_arma_snr = zeros(10,1);
D1_arma = iddata(v1_all{y1_snr_arx_optimal_index});
D2_arma = iddata(v2_all{y2_snr_arx_optimal_index});

for i = 1 : 10
    na = random_matrix(i,1);
    nc = random_matrix(i,2);

    Marma{i}{1} = armax(D1_arma, [na nc]);
    Marma{i}{2} = armax(D2_arma, [na nc]);
    
 %% exercise 5. Calculati iesirea simulata a modelului ARMA vArma, cu functia predict
    v1_arma = predict(Marma{i}{1}, D1_arma);
    v2_arma = predict(Marma{i}{2}, D2_arma);
   
 %% exercise 6. Calculati estimatia zgomotului alb

    eps1{i} = v1_all{y1_snr_arx_optimal_index} - v1_arma.y;
    eps2{i} = v2_all{y2_snr_arx_optimal_index} - v2_arma.y;

    v1_arma_snr(i) = 20 * log(std(v1_arma.y)/std(eps1{i}));
    v2_arma_snr(i) = 20 * log(std(v2_arma.y)/std(eps2{i}));
    
    v1_arma_all{i} = v1_arma;
    v2_arma_all{i} = v2_arma;
end

% compute the optimal snr
[v1_arma_snr_optimal, v1_arma_snr_optimal_index] =  max(v1_arma_snr);
[v2_arma_snr_optimal, v2_arma_snr_optimal_index] =  max(v2_arma_snr);

%% exercise 7. Calculati iesirea simulata totala s ARX = + ARMA y y v si comparati-o cu datele masurate, trasand-o pe acelasi grafic.
ys1 = y1_arx_optimal + v1_arma_all{v1_arma_snr_optimal_index}.y;
ys2 = y2_arx_optimal + v2_arma_all{v2_arma_snr_optimal_index}.y;

figure;
plot(ys1);
hold on 
plot(y1);
hold off;

title('y1 Total vs y1 Measured');
legend('y1 Total', 'y1 Measured')

figure;
plot(ys2);
hold on 
plot(y2);
hold off;

title('y2 Total vs y2 Measured');
legend('y2 Total', 'y2 Measured')

%% exercise 8. Separat, trasati graficul esitmatiei zgomotului alb rezultat. 

eps_1 = v1_all{y1_snr_arx_optimal_index} - v1_arma_all{v1_arma_snr_optimal_index}.y;
figure;
plot(eps_1);
title('White Noise Prediction 1');

eps_2 = v2_all{y2_snr_arx_optimal_index} - v2_arma_all{v2_arma_snr_optimal_index}.y;

figure;
plot(eps_2);
title('White Noise Prediction 2');


% compute y prediction error by substracting ystotal from y 
y1_prediction_error = y1 - ys1;
y2_prediction_error = y2 - ys2;

% compute variance 1 and 2
lambda1 = var(y1_prediction_error);
lambda2 = var(y2_prediction_error);

figure;
plot(y1_prediction_error);
title('Prediction error 1');

figure;
plot(y2_prediction_error);
title('Prediction error 2');


