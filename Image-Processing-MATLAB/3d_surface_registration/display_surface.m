function display_surface( surface, facecolor )
% DISPLAY_SURFACE   Displays a surface with the input facecolor
%
%   Author: Vignesh Rammohan, 06-February-2018
%
%   Mathematical Modelling in Medicine Group
%   Department of Infection, Immunity and Cardiovascular Disease
%   The University of Sheffield, Sheffield, United Kingdom.


    patch(surface,'FaceColor', facecolor, ...
         'EdgeColor', 'none',        ...
         'FaceLighting', 'gouraud',     ...
         'AmbientStrength', 0.15); % earlier 0.15
    % Add a camera light and a surface material
    camlight(45,60); % earlier 45 60
    material('metal'); % options: 'dull', 'shiny'
    % Fix the axes scaling, and set a nice view angle
    axis('vis3d');
    view([45 60]);
    
end

