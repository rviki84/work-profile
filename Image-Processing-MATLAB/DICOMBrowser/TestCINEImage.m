function TestCINEImage(img, axis)
% TestCINEImage Display the frames(time-points) in the image continously

    for n=1:size(img,3)
        img(:,:,n)=imadjust(img(:,:,n));
    end
    i=1;
    axPlot=axis;
    imgNum = size(img,3);
    nSlices = num2str(imgNum);
    x = size(img,1)*0.2;
    y = size(img,2)*0.1;
    while imgNum > 0
        try
            axes(axPlot)
            imshow(img(:,:,i))%,'Border','tight')
            text(x, y,...
                ['Slice ',num2str(i),'/',nSlices],...
                'FontSize',8,...
                'Color',[.9 .4 .4],...
                'FontWeight','bold');
            %pause(0.1)
        catch ME
            continue
        end
        i=i+5;
        if i>size(img,3)
            i=1;
        end
        imgNum = imgNum - 5;
%         if imgNum == 0
%             imgNum = size(image,3);
%         end
    end
end