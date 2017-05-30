function [stopIter] = updatePlot(D,corrMat)

global myhandles;
stopIter = myhandles.stopIter;

%corrChannel = reshape(corrMat,8,256)';
corrChannel = reshape(corrMat,256,8);
% axes(myhandles.axes1)
% hold on
%plot(myhandles.axes1,1:2048, corrMat)

% plot(myhandles.axes1,log(myhandles.DelayTime), corrChannel(:,1))
% plot(myhandles.axes2,log(myhandles.DelayTime), corrChannel(:,2))
% plot(myhandles.axes3,log(myhandles.DelayTime), corrChannel(:,3))
% plot(myhandles.axes4,log(myhandles.DelayTime), corrChannel(:,4))

semilogx(myhandles.axes1,myhandles.DelayTime, corrChannel(:,1))
semilogx(myhandles.axes2,myhandles.DelayTime, corrChannel(:,3))
semilogx(myhandles.axes3,myhandles.DelayTime, corrChannel(:,5))
semilogx(myhandles.axes4,myhandles.DelayTime, corrChannel(:,7))



pause
end