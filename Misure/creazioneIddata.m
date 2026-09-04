clc
clear
close all
directory = pwd;

% Ottieni la lista di tutti i file nella directory
files = dir(fullfile(directory, '*.csv'));

% Itera su ciascun file CSV
for i = 1:length(files)
   name = erase(files(i).name,'.csv');
   csv_name = files(i).name;
   mat_name_pos = strcat(name, '.mat');
   mat_name_pos_vel = strcat(strcat(name,'_vel'), '.mat');
   
   load(csv_name)

   direzione = eval(sprintf("%s(:,1)",name));
   dc = eval(sprintf("%s(:,2)",name)).*direzione;
   angpos = eval(sprintf("%s(:,3)",name));
   t = eval(sprintf("%s(:,4)",name))/1000;
   vel = [];
   for j = 1:numel(angpos)-1
       vel(j) = (angpos(j+1)-angpos(j))/(t(j+1)-t(j));
   end
   sampling_time = [];
   for j = 1:numel(t)-1
        sampling_time(j) = t(j+1)-t(j);
   end
   vel = [0 vel]';
   ts = mean(sampling_time);
   eval(sprintf("%s = iddata(angpos,dc,ts);",name))
   eval(sprintf("%s_vel = iddata([angpos vel],dc,ts);",name))
   save(mat_name_pos,name);
   save(mat_name_pos_vel,strcat(name,'_vel'));
end
