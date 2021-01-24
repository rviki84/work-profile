function clsavevtk(array, filename)
%  savevtk Save a 3-D scalar array in VTK format.
%  savevtk(array, filename) saves a 2-D array (3xn) of any size to
%  filename in VTK format.
    [dd, N] = size(array);
    fid = fopen(filename, 'wt');
    fprintf(fid, '# vtk DataFile Version 3.0\n');
    fprintf(fid, 'Centreline data\n');
    fprintf(fid, 'ASCII\n');
    fprintf(fid, '\n');
    fprintf(fid, 'DATASET POLYDATA\n');
    fprintf(fid, 'POINTS %d float\n', N);
    for i=1:N
        for j=1:dd
            fprintf(fid, '%d ', array(j,i));
        end;
        fprintf(fid, '\n');
    end;
    fprintf(fid, '\n');
    fprintf(fid, 'LINES 1 %d\n', N+1);
    fprintf(fid, '%d ', N);
    for i=1:N
        fprintf(fid, '%d ', i-1);
    end;
    fprintf(fid, '\n');
    fprintf(fid, '\n');
    fprintf(fid, 'POINT_DATA %d\n', N);
    fprintf(fid, 'SCALARS scalars float\n');
    fprintf(fid, 'LOOKUP_TABLE default');
    fprintf(fid, '\n');
    for i=1:N
        fprintf(fid, '%d ', i-1);
    end;
    fprintf(fid, '\n');
    fclose(fid);
return

