%% Pulizia
clc
clear all
close all 

%% Caricamento file .mat (POSIZIONE) --> sono già iddata

load('dc35_vel.mat');
load('dc32_vel.mat');
load('dc31_vel.mat');

load('dcRandom_every_ts_vel.mat');
load('random_sig_m2_vel.mat');

%DETREND:
%Senza detrend ho un fit a malapena sufficiente, tranne per il dc35 che è
%ottimo (ARX)

dc35 = detrend(dc35_vel);
dc32 = detrend(dc32_vel);
dc31 = detrend(dc31_vel);
%Detrendizzare un segnale randomico non è un errore????
dcRandom_every_ts = detrend(dc35_vel);
random_sig_m2 = detrend(dc35_vel);

%una prima visualizzazione dei dati:
figure(10)
plot(dc35)
title('dc35')
figure(11)
plot(dc32)
title('dc32')
figure(12)
plot(dc31)
title('dc31')
figure(13)
plot(dcRandom_every_ts)
title('dcRandom_(every ts)')
figure(14)
plot(random_sig_m2)
title('random_(sig) m2')

%% Identificazione
%
%Dividiamo il dataset:
dc35_training = dc35(1:40000, :);
dc35_validation = dc35(40001:50000, :);

dc32_training = dc32(1:50000, :);
dc32_validation = dc32(50001:60000, :);

dc31_training = dc31(1:40000, :);
dc31_validation = dc31(40001:50000, :);

dcRand_training = dcRandom_every_ts(1:40000, :);
dcRand_validation = dcRandom_every_ts(40001:50000, :); 

rand_training = random_sig_m2(1:20000, :);
rand_validation = random_sig_m2(20001:30000, :);

%% ARX
% %Testiamo ARX:
% arx_dc35 = arx(dc35_training, [40 40 1]);
% armax_dc35 = armax(dc35_training, [40 40 5 1]);
% 
% figure(2)
% compare(dc35_validation, arx_dc35)
% title('ARX_dc35')
% figure(3)
% resid(dc35_validation, arx_dc35)
% title('Resid: ARX_dc35')
% figure(4)
% compare(dc35_validation, armax_dc35)
% title('ARMAX_dc35')
% figure(5)
% resid(dc35_validation, armax_dc35)
% title('Resid: ARMAX_dc35')
%
% Gli altri:

arx_dc35 = arx(dc35_training, [3 3 3 1; 5 5 5 1]);
arx_dc32 = arx(dc32_training, [3 3 3 1; 5 5 5 1]);
arx_dc31 = arx(dc31_training, [3 3 3 1; 5 5 5 1]);
arx_dcRand = arx(dcRand_training, [3 3 3 1; 5 5 5 1]);
arx_rand = arx(rand_training, [3 3 3 1; 5 5 5 1]);

%Verifica del fit ARX:
figure(22)
compare(dc35_validation, arx_dc35);
title('ARX_dc35')
figure(23)
compare(dc32_validation, arx_dc32);
title('ARX_dc32')
figure(24)
compare(dc31_validation, arx_dc31);
title('ARX_dc31')
figure(25)
compare(dcRand_validation, arx_dcRand);
title('ARX_dcRand')
figure(26)
compare(rand_validation, arx_rand);
title('ARX_rand')


%% NLARX
%Identificazione con modelli non lineari
nlarx_dc35 = nlarx(dc35_training, [20 20 1]);
nlhw_dc35 = nlhw(dc35_training, [20 20 1]);

nlarx_dc32 = nlarx(dc32_training, [20 20 1]);
nlhw_dc32 = nlhw(dc32_training, [20 20 1]);

nlarx_dc31 = nlarx(dc31_training, [20 20 1]);
nlhw_dc31 = nlhw(dc31_training, [20 20 1]);

nlarx_dcRand = nlarx(dcRand_training, [20 20 1]);
nlhw_dcRand = nlhw(dcRand_training, [20 20 1]);

nlarx_rand = nlarx(rand_training, [20 20 1]);
nlhw_rand = nlhw(rand_training, [20 20 1]);

%Verifica del fit nlarx:
figure(102)
compare(dc35_validation, nlarx_dc35);
title('NLARX_dc35')
figure(103)
compare(dc32_validation, nlarx_dc32);
title('NLARX_dc32')
figure(104)
compare(dc31_validation, nlarx_dc31);
title('NLARX_dc31')
figure(105)
compare(dcRand_validation, nlarx_dcRand);
title('NLARX_dcRand')
figure(106)
compare(rand_validation, nlarx_rand);
title('NLARX_rand')

%Verifica dei fit nlhw:
figure(202)
compare(dc35_validation, nlhw_dc35);
title('NLHW_dc35')
figure(203)
compare(dc32_validation, nlhw_dc32);
title('NLHW_dc32')
figure(204)
compare(dc31_validation, nlhw_dc31);
title('NLHW_dc31')
figure(205)
compare(dcRand_validation, nlhw_dcRand);
title('NLHW_dcRand')
figure(206)
compare(rand_validation, nlhw_rand);
title('NLHW_rand')

%% Esport identificazione

arxRand_pos = arx_dcRand(1,1);
arxRand_vel = arx_dcRand(2,1);

