% TEST_TRANSFORM  To test the transformation matrix used for registration of surfaces. 
%
%   USAGE:
%   1. Replace the input STL filenames in the functions 'stlread' and
%   'stlReadAscii' as appropriate.
%   2. Change the 'Tinit' and 'scalingSwitch' variables with appropriate
%   values.
%   3. Finally in the last line, in the function 'stlwrite' replace the
%   output STL filename to be saved.
%
%
%   Author: Vignesh Rammohan, 06-February-2018
%
%   Mathematical Modelling in Medicine Group
%   Department of Infection, Immunity and Cardiovascular Disease
%   The University of Sheffield, Sheffield, United Kingdom.


%% Import surface meshes
% Import a STL mesh in binary form, returning a PATCH-compatible face-vertex structure
s_fixed = stlread('surfaces_stl_input/1_LCAcorrected.stl'); % fixed surface
display_surface(s_fixed, [0.8 0.8 1.0]); % mild blue
v_fixed = s_fixed.vertices;

% Import a STL mesh in ascii form, returning its vertices and faces
% [v_fixed, f_fixed, ~, ~] = stlReadAscii('surfaces_stl_input/LCA1_RUN1-3.stl');
% s_fixed = struct('vertices', v_fixed, 'faces', f_fixed); % fixed surface
% display_surface(s_fixed, [0.8 0.8 1.0]); % mild blue

% Import a STL mesh in binary form, returning a PATCH-compatible face-vertex structure
% s_move = stlread('surfaces_stl_input/LCA1_RUN1-4.stl'); % moving surface
% display_surface(s_move, [0.8 1.0 0.5]); % green
% v_move = s_move.vertices;

% Import a STL mesh in ascii form, returning its vertices and faces
[v_move, f_move, ~, ~] = stlReadAscii('surfaces_stl_input/LCA1_RUN1-24.stl');
s_move = struct('vertices', v_move, 'faces', f_move); % moving surface
display_surface(s_move, [0.8 1.0 0.5]); % green
%

%% Apply transformation and calculate the RMS distance
% Transformation matrix to apply in order to register the intended surface
% to the fixed one
Tinit = [0.3, -0.37, 0.58, 11.39;...
    -0.23, 0.54, 0.47, -40.32;...
    -0.65, -0.36, 0.11, 3.72;...
    0, 0, 0, 1];

%%%%%%%%%% FOR TESTING ABOVE TRANSFORMATION DIRECTLY %%%%%%%%%%%%%
    vertMov = v_move;
    vertMov(:,4) = ones(size(vertMov,1),1);
    T_vmov = Tinit*vertMov';
    TvertMove = T_vmov(1:3,:)'; 
    s_move.vertices = TvertMove;
    display_surface(s_move, [0.8 0.2 0.5]); % maroon
%%%%%%%%%% FOR TESTING ABOVE TRANSFORMATION DIRECTLY %%%%%%%%%%%%%

%% Extract Euler angles and scaling factors if 'ON'
scalingSwitch = 'ON'; % compute scaling - set 'ON' or 'OFF'
[eulXYZ, scaleXYZ] = Tm4x4ToEulerXYZ(Tinit, scalingSwitch);
% Initial 6 DOFs
dof6init = [eulXYZ Tinit(1,4) Tinit(2,4) Tinit(3,4)];

% Get the RMS distance resulting from the registration
tic
[ rmsDist, minDist, maxDist, meanDist, TvertMove, ...
    minDistVec, Tfinal ] = rmsDistSurfaces( dof6init, scaleXYZ, v_move, v_fixed, 1 );
toc

% Display the registered surface
s_move.vertices = TvertMove;
display_surface(s_move, [0.8 0.6 0.5]); % gold
%
% Save the registered surface as 'stl' mesh
stlwrite('surfaces_stl_output/regtdSurface(init)_1_LCAcorrected_RUN1-24.stl', s_move, 'mode', 'ascii');
%

%% Display the legend for the current figure
legend('fixed surface', 'moving surface', 'regtd-Tinit', 'regtd-Euler');

%%
%     eulZYX = tform2eul(T, 'ZYX'); % The default order for Euler angle rotations is 'ZYX'.
%     rotmZYX = eul2rotm(flip(eulXYZ), 'ZYX');

% END OF THE SCRIPT

