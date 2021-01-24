function CINEImageByJump(img, axis, jump)
%% CINEImageByJump Display the frames(time-points) in the image. Jump denotes the
% number of slices to jump for the consecutive slice display

    for n=1:size(img,3)
        img(:,:,n)=imadjust(img(:,:,n));
    end
    i=1;
    axPlot=axis;
    sliceNum = size(img,3);
    nSlices = num2str(sliceNum);
    x = size(img,1)*0.25;
    y = size(img,2)*0.05;
    while sliceNum > 0
        try
            axes(axPlot)
            imshow(img(:,:,i),'Border','tight')
            text(x, y,...
                ['Slice ',num2str(i),'/',nSlices],...
                'FontSize',12,...
                'Color',[1 .2 .4],...
                'FontWeight','bold');
        catch
            continue
        end
        i=i+jump;
        if i>size(img,3)
            i=1;
        end
        sliceNum = sliceNum - jump;
    end
end
