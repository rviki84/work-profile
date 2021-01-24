%% Parses specified field in a file and plots the data
%

filetext = fileread('log.trn');
expr = '[^\n]*Windkessel [1-2][^\n]*';
matches = regexp(filetext,expr,'match');

iter = length(matches);
wkQP = zeros(iter/2,4);
for i=1:2:iter
    wkQP((i+1)/2,1) = str2double(matches{i}(:,20:32));
    wkQP((i+1)/2,2) = str2double(matches{i}(:,44:56));
end

for i=2:2:iter
    wkQP(i/2,3) = str2double(matches{i}(:,20:32));
    wkQP(i/2,4) = str2double(matches{i}(:,44:56));
end

subplot(2,1,1);
plot(wkQP(:,1),'b-.')
hold on
plot(wkQP(:,3),'m-.')
title('Windkessel-Q')
legend('outlet-main','outlet-branch')

subplot(2,1,2);
plot(wkQP(:,2),'-.')
hold on
plot(wkQP(:,4),'-.')
title('Windkessel-P')
legend('outlet-main','outlet-branch')

saveas(gcf,'windkessel-QP-1.4.fig')
saveas(gcf,'windkessel-QP-1.4.png')

% ---------- END ------------ %