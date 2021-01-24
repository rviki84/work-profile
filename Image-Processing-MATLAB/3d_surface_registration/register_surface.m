% REGISTER_SURFACE  Registers surfaces by minimising the RMS distances. 
%
%   Registers a surface to a fixed surface by applying the initial
%   transformation Tinit and subsequently refines (min. RMS distance
%   computation) the registration using fminsearch on the 6 degrees of
%   freedom (dof6): 3 Euler angles for rotation + 3 translation parameters.
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
% [v_fixed, f_fixed, ~, ~] = stlReadAscii('surfaces_stl_input/1_LCAcorrected.stl');
% s_fixed = struct('vertices', v_fixed, 'faces', f_fixed); % fixed surface
% display_surface(s_fixed, [0.8 0.8 1.0]); % mild blue

% Import a STL mesh in binary form, returning a PATCH-compatible face-vertex structure
% s_move = stlread('surfaces_stl_input/1_LCAcorrected.stl'); % moving surface
% display_surface(s_move, [0.8 1.0 0.5]); % green
% v_move = s_move.vertices;

% Import a STL mesh in ascii form, returning its vertices and faces
[v_move, f_move, ~, ~] = stlReadAscii('surfaces_stl_input/RUN1-3.stl');
s_move = struct('vertices', v_move, 'faces', f_move); % moving surface
display_surface(s_move, [0.8 1.0 0.5]); % green
%


%% Apply transformation and minimise the RMS distance
% Transformation matrix to apply in order to register the intended surface
% to the fixed one
Tinit = [-0.21,	-0.53,	-0.24,	5.78;...
0.16,	0.2,	-0.57,	8.79;...
0.56,	-0.26,	0.07,	14.7;...
0.0,	0.0,	0.0,	1.0];

%% Extract Euler angles and scaling factors if 'ON'
scalingSwitch = 'ON'; % compute scaling - set 'ON' or 'OFF'
[eulXYZ, scaleXYZ] = Tm4x4ToEulerXYZ(Tinit, scalingSwitch);
% Initial 6 DOFs
dof6init = [eulXYZ Tinit(1,4) Tinit(2,4) Tinit(3,4)];

%% Run optimisation on 6 DOFs
options = optimset('Display','iter',...
            'MaxFunEvals',50*length(dof6init),...
            'TolFun',1e-6,...
            'TolX',1e-6);
tic
[dof6, ~, exitflag, ~] = fminsearch(@(dof6) rmsDistSurfaces_v2( dof6, scaleXYZ, ...
    v_move, s_fixed, 0 ), dof6init, options);
disp(['exitStatus: ', num2str(exitflag)]);
[ rmsDist, minDist, maxDist, meanDist, TvertMove, ...
    minDistVec, Tfinal] = rmsDistSurfaces_v2( dof6, scaleXYZ, v_move, s_fixed, 1 );
toc

% Display the final and finely registered surface
s_move.vertices = TvertMove;
display_surface(s_move, [0.8 0.6 0.5]);  % gold

% Display the legend for the current figure
legend('fixed surface', 'moving surface', 'registered surface');

% Save the registered surface as 'stl' mesh
stlwrite('surfaces_stl_output/regtdSurface_1_LCAcorrected_RUN1-3.stl', s_move, 'mode', 'ascii');

% END OF THE SCRIPT

