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

% Last Modified by GUIDE v2.5 19-May-2017 13:03:28

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

% Update handles structure
guidata(hObject, handles);

%save('handles.mat','handles');

global myhandles updateFunction
myhandles = handles;
updateFunction = @update;

% UIWAIT makes DCSAcquisition_RT wait for user response (see UIRESUME)
% uiwait(handles.figure1);

function update(Amat)
  msgbox('Inside update')  
  global myhandles;
  plot(myhandles.axes2,1:18,Amat);
  
  %axes(myhandles.axes2);
  %plot(1:2,1:2);

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


%Call C++ .exe file to collect data
system('ConsoleApplication2.exe');




%Obtain data from .cpp file (via streaming or via files)





% --- Executes on button press in stopDCS.
function stopDCS_Callback(hObject, eventdata, handles)
% hObject    handle to stopDCS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
