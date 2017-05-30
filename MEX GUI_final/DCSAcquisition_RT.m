function varargout = DCSAcquisition_RT(varargin)
% DCSACQUISITION_RT MATLAB code for DCSAcquisition_RT.fig
%      DCSACQUISITION_RT, by itself, creates a new DCSACQUISITION_RT or raises the existing
%      singleton*.
%
%      H = DCSACQUISITION_RT returns the handle to a new DCSACQUISITION_RT or the handle to
%      the existing singleton*.
%
%      DCSACQUISITION_RT('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in DCSACQUISITION_RT.M with the given input arguments.
%
%      DCSACQUISITION_RT('Property','Value',...) creates a new DCSACQUISITION_RT or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before DCSAcquisition_RT_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to DCSAcquisition_RT_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help DCSAcquisition_RT

% Last Modified by GUIDE v2.5 25-May-2017 21:39:42

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @DCSAcquisition_RT_OpeningFcn, ...
                   'gui_OutputFcn',  @DCSAcquisition_RT_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before DCSAcquisition_RT is made visible.
function DCSAcquisition_RT_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to DCSAcquisition_RT (see VARARGIN)

% Choose default command line output for DCSAcquisition_RT
handles.output = hObject;

mex -g DCScorrelator_read.cpp

FIRSTDELAY = 1e-7;

DelayTime = (1:16)*FIRSTDELAY;
for j = 1:30
    for i = 1:8
        DelayTime(i + (j - 1) * 8 + 16) = DelayTime((j - 1) * 8 + 16 + i - 1) + FIRSTDELAY*2^j;
    end
end

handles.DelayTime = DelayTime;

% Update handles structure
guidata(hObject, handles);

global myhandles 
myhandles = handles;

% updateFunction = @update;
% plotRealTime = @update_v2;
% % UIWAIT makes DCSAcquisition_RT wait for user response (see UIRESUME)
% % uiwait(handles.figure1);
% 
% function update_v2(testVar)
% msgbox('inside update_v2')
% global myhandles;
% plot(myhandles.axes1, 1:18, testVar)
% 
% function update(delayTime, corrMat, Legend)
% global myhandles;
% corrChannel = reshape(corrMat,8,256)';
% msgbox('inside updatev2')
% plot(myhandles.axes1,delayTime,corrChannel(:,2));
% plot(myhandles.axes2,delayTime,corrChannel(:,3));
% plot(myhandles.axes3,delayTime,corrChannel(:,4));
%legend(Legend);
%hold on

% for i = 2:5
%     figure(i);
%     semilogx(delayTime,corrChannel(:,i));
%     legend(Legend);
%     hold on
% end


% function update(Amat)
%   msgbox('Inside update')  
%   global myhandles;
%   plot(myhandles.axes2,1:18,Amat);
%   
%   %axes(myhandles.axes2);
%   %plot(1:2,1:2);

% --- Outputs from this function are returned to the command line.
function varargout = DCSAcquisition_RT_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

% --- Executes on button press in startDCS.
function startDCS_Callback(hObject, eventdata, handles)
% hObject    handle to startDCS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.stopIter = 0;
%disp(handles.numIter)
guidata(hObject, handles);
global myhandles
myhandles = handles;

a = DCScorrelator_read(1, rand(2048,1)');
% for i = 1:handles.numIter
%     %corrVal = trial(1,rand(2048,1)');
%     corrVal = trial(i, rand(5,1)')
%     axes(handles.axes1)
%     plot(1:5, corrVal)
%     hold on
%     x = input('prompt');
% end



% --- Executes on button press in stopDCS.
function stopDCS_Callback(hObject, eventdata, handles)
% hObject    handle to stopDCS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

disp('STOP DCS pressed')
handles.stopIter = 1;
guidata(hObject,handles);
global myhandles
myhandles = handles;
msgbox('Press space bar twice to quit and save file')

function edit2_Callback(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit2 as text
%        str2double(get(hObject,'String')) returns contents of edit2 as a double

numIter = get(hObject, 'String');
handles.numIter = str2num(numIter)
guidata(hObject, handles);

% --- Executes during object creation, after setting all properties.
function edit2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
