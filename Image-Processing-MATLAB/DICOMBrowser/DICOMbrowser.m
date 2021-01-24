function varargout = DICOMbrowser(varargin)
% DICOMBROWSER MATLAB code for DICOMbrowser.fig
%      DICOMBROWSER, by itself, creates a new DICOMBROWSER or raises the existing
%      singleton*.
%
%      H = DICOMBROWSER returns the handle to a new DICOMBROWSER or the handle to
%      the existing singleton*.
%
%      DICOMBROWSER('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in DICOMBROWSER.M with the given input arguments.
%
%      DICOMBROWSER('Property','Value',...) creates a new DICOMBROWSER or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before DICOMbrowser_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to DICOMbrowser_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help DICOMbrowser

% Last Modified by GUIDE v2.5 09-Jan-2019 18:07:49

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @DICOMbrowser_OpeningFcn, ...
                   'gui_OutputFcn',  @DICOMbrowser_OutputFcn, ...
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


% --- Executes just before DICOMbrowser is made visible.
function DICOMbrowser_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to DICOMbrowser (see VARARGIN)

global imageDir

imageDir = '';

% Choose default command line output for DICOMbrowser
handles.output = hObject;

% Place the DICOMbrowser GUI at center
movegui(hObject, 'center');

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes DICOMbrowser wait for user response (see UIRESUME)
% uiwait(handles.dicomBrowser);


% --- Outputs from this function are returned to the command line.
function varargout = DICOMbrowser_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in btnDicomFolder.
function btnDicomFolder_Callback(hObject, eventdata, handles)
% hObject    handle to btnDicomFolder (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

global imageDir imagePaths images selectedImgSeq

copyDir = '';
updateWarning('None', handles.txtMessage)

if isempty(imageDir)
    filepath = fileparts(mfilename('fullpath'));
    imageDir = uigetdir(filepath,'Select DICOM Image Directory');
else
    copyDir = imageDir;
    imageDir = uigetdir(imageDir,'Select DICOM Image Directory');
end

if ~ischar(imageDir)
    disp('No valid directory selected!')
    imageDir = copyDir;
    return;
end
handles.dicomDir = imageDir;

% get all files in the imageDir
dicomFiles = dir(imageDir);
dicomFiles = dicomFiles(3:end);   % avoid . and ..
nfiles = length(dicomFiles);
if nfiles<1
    disp('No files available!')
    return;
end

% update the 'Chosen Folder' text display
set(handles.txtFolder,'String',imageDir)
% erase all previously loaded images
delete(allchild(handles.panelDicom))
unselectImage([], handles)
% initialise image variables
imagePaths = cell(nfiles,1);
images = cell(nfiles,1);
selectedImgSeq = [0 0];
% initiate progress bar
hwait = waitbar(0,'Scanning images...',...
                'WindowStyle','modal');

% read image(DICOM) files 
for n=1:nfiles
    fname = dicomFiles(n).name;
    try
       metadata = dicominfo([imageDir '\' fname]);
       LAORAO = metadata.PositionerPrimaryAngle;
    catch
%        disp ('Not a DICOM file!');
       continue
    end

    imagePaths{n}=[imageDir '\' fname];
    images{n} = dicomread(imagePaths{n});
    images{n} = squeeze(images{n});
end

% remove empty fields
imagePaths = imagePaths(~cellfun('isempty',imagePaths));
images = images(~cellfun('isempty',images));
% if no image files found, display warning and return
if isempty(imagePaths)
    waitbar(1,hwait,'No angiograms spotted!!!');
    txt = {'No angiograms available in the chosen folder!'};
    updateWarning(txt, handles.txtMessage)
    close(hwait)
    return
end

%% Prepare for loading the images
try
    waitbar(.2,hwait,'Preparing images...');
catch
    txt = {'Loading of images cancelled!'};
    updateWarning(txt, handles.txtMessage)
    return
end
handles.hAxes = gobjects(length(imagePaths),1);
nrows = ceil(length(imagePaths)/5);
yfactor = 1/nrows;
ystart = yfactor*(nrows-1);
height = yfactor;

hPanel = handles.panelDicom;
panelScroll = uipanel('Parent',hPanel,'Units','normalized',...
                    'BackgroundColor',get(hPanel,'BackgroundColor'));
set(panelScroll,'Position',[0 -1 1 2]);

hSlider = uicontrol('Style','Slider','Parent',hPanel,...
      'Units','normalized','Position',[0.98 0 0.02 1],...
      'SliderStep',[0.1, 0.2],'Min', 0, 'Max', 1,...
      'Value',1,'Callback',{@slider_callback,panelScroll});
 
for i=1:length(imagePaths)   
    m = mod(i,5);
    q = fix(i/5);
    if m ~= 0
        pos = [(m-1)*0.195 ystart-q*yfactor 0.19 height];
    else
        pos = [4*0.195 ystart-(q-1)*yfactor 0.19 height];
    end
    handles.hAxes(i) = axes('Color',[.8 .8 .8],...
                 'Position',pos,...
                 'Parent',panelScroll,...
                 'XTick',[],...
                 'YTick',[]);
end
%
%% Load the images
for j=1:length(imagePaths)
    CINEImageByJump(images{j}, handles.hAxes(j), 5);
    len = 0.4 + 0.6*j/length(imagePaths);
    set(handles.hAxes(j),'UserData',struct('alpha',1))
    try
        waitbar(len,hwait,'Loading images...');
    catch
        txt = {'Loading of images cancelled!'};
        updateWarning(txt, handles.txtMessage)
        return;
    end
end

close(hwait)
guidata(hObject, handles);
%

%% --- Executes on change in the slider(hSlider) position
function slider_callback(src,eventdata,obj)
val = get(src,'Value');
set(obj,'Position',[0 -val 1 2])
%

%% --- Executes on mouse press over figure background, over a disabled or
% --- inactive control, or over an axes background.
function dicomBrowser_WindowButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to dicomBrowser (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

global images imagePaths selectedImgSeq

updateWarning('None', handles.txtMessage)
% if the click is NOT on any of the image, RETURN
if ~strcmp(eventdata.Source.CurrentObject.Type,'image')
    return
end
% if there is no axes handle field, RETURN
if ~isfield(handles,'hAxes')
    return
end

% get the image the user clicked on
ax = gca;
imgNum = 0;
for n=1:length(handles.hAxes)
    if ax == handles.hAxes(n)
        imgNum = n;
        break
    end
end
if imgNum == 0
    return
end

%% Based on the button clicked, perform the action
bClicked = get(hObject,'SelectionType');
alphaVal = ax.UserData.alpha;
if strcmp(bClicked,'normal') % left mouse button
    CINEImageByJump(images{imgNum}, ax, 1);
    ax.UserData.alpha = alphaVal;
    alpha(ax,alphaVal)
    if alphaVal == 1
        colormap(ax,gray);
    else
        colormap(ax,jet);
    end
    
elseif strcmp(bClicked,'alt') || strcmp(bClicked,'extend') % right mouse button     
    if alphaVal == 1
        if selectedImgSeq(1) ~= 0 && selectedImgSeq(2) ~= 0
            txt = {'2 images already selected!' 'Please unselect any one.'};
            updateWarning(txt, handles.txtMessage) 
            return;
        end
        alpha(ax,'.25')
%         alpha(ax,'color')
        colormap(ax,jet);
        ax.UserData.alpha = .25;
        updateSelectedImageData(imagePaths{imgNum}, handles)
    else
        alpha(ax,'1')
        ax.UserData.alpha = 1;
        colormap(ax,gray);
        unselectImage(imagePaths{imgNum}, handles)
    end
end
%

%% --- Updates selected image's rotation and distance information
function updateSelectedImageData(imgFilepath, handles)
% imgFilepath   image file path to fetch DICOM header information
% handles       structure with handles and user data (see GUIDATA)

global selectedImgSeq

imgHeader = dicominfo(imgFilepath);
Run = imgHeader.SeriesNumber;
if isempty(Run)
    Run = imgHeader.InstanceNumber;
end
LAORAO = imgHeader.PositionerPrimaryAngle;
CauCra = imgHeader.PositionerSecondaryAngle;
distSD = imgHeader.DistanceSourceToDetector;
if selectedImgSeq(1) == 0
    set(handles.txtLAOLeft,'String',num2str(LAORAO))
    set(handles.txtCranLeft,'String',num2str(CauCra))
    set(handles.txtDistLeft,'String',num2str(distSD))
    selectedImgSeq(1) = Run;
    
elseif selectedImgSeq(2) == 0
    set(handles.txtLAORight,'String',num2str(LAORAO))
    set(handles.txtCranRight,'String',num2str(CauCra))
    set(handles.txtDistRight,'String',num2str(distSD))
    selectedImgSeq(2) = Run;
end
%

%% --- Clears selected image's rotation and distance information
function unselectImage(imgFilepath, handles)
% imgFilepath   image file path to fetch DICOM header information
% handles       structure with handles and user data (see GUIDATA)

global selectedImgSeq

if isempty(imgFilepath)
    set(handles.txtLAOLeft,'String','-')
    set(handles.txtCranLeft,'String','-')
    set(handles.txtDistLeft,'String','-')
    set(handles.txtLAORight,'String','-')
    set(handles.txtCranRight,'String','-')
    set(handles.txtDistRight,'String','-')
    return
end

imgHeader = dicominfo(imgFilepath);
Run = imgHeader.SeriesNumber;
if isempty(Run)
    Run = imgHeader.InstanceNumber;
end
if selectedImgSeq(1) == Run
    set(handles.txtLAOLeft,'String','-')
    set(handles.txtCranLeft,'String','-')
    set(handles.txtDistLeft,'String','-')
    selectedImgSeq(1) = 0;
    
elseif selectedImgSeq(2) == Run
    set(handles.txtLAORight,'String','-')
    set(handles.txtCranRight,'String','-')
    set(handles.txtDistRight,'String','-')
    selectedImgSeq(2) = 0;
end
%

%% --- Updates warning text messsage
function updateWarning(text, hText)
% text  text to display in the warning space
% hText handle to the warning text UI

if strcmp(text,'None')
    set(hText,'String',text)
    return
end

actualColor = get(hText,'ForegroundColor');
set(hText,'String',text)
% darkRed = [0.64 0.08 0.18];
for i=1:10
    set(hText,'ForegroundColor',[0 0 0]) % black color
    pause(0.1)
    set(hText,'ForegroundColor',actualColor)
    pause(0.1)
end
%

%% --- Executes on button press in btnLoadImages.
function btnLoadImages_Callback(hObject, eventdata, handles)
% hObject    handle to btnLoadImages (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
