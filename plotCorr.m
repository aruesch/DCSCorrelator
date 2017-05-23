
function [D] = plotCorr(corrMat,delayTime)

corrChannel = reshape(corrMat,8,256)';

for i = 1:8
    figure(i);
    semilogx(delayTime,corr(:,i));
    hold on
end

% 
% fsave = sprintf('F:\\Results\\Amat%0.1d.mat',iter);
% save(fsave,'Amat')
D = 1;

%saveas(gcf, 'F:\Results\cVar.png');
end