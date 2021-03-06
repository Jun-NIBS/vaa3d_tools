function obj = newAxon(obj) 
if (obj.state.display.ch1==1) && isfield(obj.data.ch(1),'imageArray')
                ch=1;
            end
            if (obj.state.display.ch2==1) && isfield(obj.data.ch(2),'imageArray')
                ch=2;
            end
            if isfield(obj.data,'dendrites')
                axonnumber=size(obj.data.dendrites,2)+1;
            else
                obj.data.dendrites=[];
                axonnumber=1;
            end
if (obj.state.display.showprojection) %&& ((obj.state.display.displayfiltered) || (obj.state.display.displayraw))
    axes(get(obj.gh.projectionGUI.Figure,'Children')); 
    [X,Y] = getpts; 
    X=floor(X-1)'; 
    Y=floor(Y-1)'; 

    
    % fill in the intermediate points on the line 
    newvoxels=[]; 
    for ind=1:size(X,2)-1    
        if (abs(X(ind)-X(ind+1))>abs(Y(ind)-Y(ind+1))) 
            linevoxels=[round(((X(ind):sign(X(ind+1)-X(ind)):X(ind+1))-X(ind))*(Y(ind+1)-Y(ind))/(X(ind+1)-X(ind)))+Y(ind);X(ind):sign(X(ind+1)-X(ind)):X(ind+1)];
            
            newvoxels=[newvoxels linevoxels(:,2:end)];   
        else 
            linevoxels=[Y(ind):sign(Y(ind+1)-Y(ind)):Y(ind+1);round(((Y(ind):sign(Y(ind+1)-Y(ind)):Y(ind+1))-Y(ind))*(X(ind+1)-X(ind))/(Y(ind+1)-Y(ind)))+X(ind)];
            
            newvoxels=[newvoxels linevoxels(:,2:end)];   
        end 
    end 
    for i=1:size(newvoxels,2) 
        [maxz,newvoxels(3,i)]=max(obj.data.ch(ch).filteredArray(newvoxels(1,i),newvoxels(2,i),obj.state.ROI.minz:obj.state.ROI.maxz)); 
        newvoxels(3,i)=newvoxels(3,i)+obj.state.ROI.minz-1;
        newvoxels(4,i)=0; 
    end 
    arraysize=size(newvoxels,2); 
    for j=1:size(newvoxels,2) %median filter of data to supress erratic jumps 
        newvoxels(5,:)=newvoxels(3,:); 
        newvoxels(3,j)=uint16(median(double(newvoxels(5,max(1,j-10):min(j+10,arraysize))))); 
    end 
    voxel=[newvoxels(1:4,:)]; 
    obj.data.dendrites(axonnumber).voxel=uint16(voxel); 
    maxx=size(obj.data.ch(ch).filteredArray,2);
    maxy=size(obj.data.ch(ch).filteredArray,1);
    maxz=size(obj.data.ch(ch).filteredArray,3);
    obj.data.dendrites(axonnumber).maxint=double(obj.data.ch(ch).filteredArray(voxel(1,:)+(voxel(2,:)-1)*maxy+(voxel(3,:)-1)*maxy*maxx));
    %        obj.data.dendrites(axonnumber).maxint=[];  
    obj.data.dendrites(axonnumber).maxintcorrected=[];
    obj.data.dendrites(axonnumber).maxintcube=[];
    obj.data.dendrites(axonnumber).medianfiltered=[];
    obj.data.dendrites(axonnumber).meanbacksingle=[];
    obj.data.dendrites(axonnumber).maxintsingle=[];
    obj.data.dendrites(axonnumber).thres=[];
    obj.data.dendrites(axonnumber).meanback=[];
    
    obj=CorrectDendrites(obj);
elseif (obj.state.display.showstack) %& ((obj.state.display.displayfiltered) | (obj.state.display.displayraw) )
    axes(get(obj.gh.imageGUI.Figure,'Children')); 
    [X,Y] = getpts; 
    X=floor(X-1)'; 
    Y=floor(Y-1)'; 
    % fill in the intermediate points on the line 
    newvoxels=[]; 
    for ind=1:size(X,2)-1    
        if (abs(X(ind)-X(ind+1))>abs(Y(ind)-Y(ind+1))) 
            linevoxels=[round(((X(ind):sign(X(ind+1)-X(ind)):X(ind+1))-X(ind))*(Y(ind+1)-Y(ind))/(X(ind+1)-X(ind)))+Y(ind);X(ind):sign(X(ind+1)-X(ind)):X(ind+1)];
            
            newvoxels=[newvoxels linevoxels(:,2:end)];   
        else 
            linevoxels=[Y(ind):sign(Y(ind+1)-Y(ind)):Y(ind+1);round(((Y(ind):sign(Y(ind+1)-Y(ind)):Y(ind+1))-Y(ind))*(X(ind+1)-X(ind))/(Y(ind+1)-Y(ind)))+X(ind)];
            
            newvoxels=[newvoxels linevoxels(:,2:end)];   
        end 
    end 
    for i=1:size(newvoxels,2) 
        newvoxels(3,i)=obj.state.display.currentz;
%        [maxz,newvoxels(3,i)]=max(obj.data.filteredArray(newvoxels(1,i),newvoxels(2,i),:)); 
        newvoxels(4,i)=0; 
    end 
    arraysize=size(newvoxels,2); 
    for j=1:size(newvoxels,2) %median filter of data to supress erratic jumps 
        newvoxels(5,:)=newvoxels(3,:); 
        newvoxels(3,j)=uint16(median(double(newvoxels(5,max(1,j-10):min(j+10,arraysize))))); 
    end 
        voxel=[voxel newvoxels(1:4,:)]; 
    obj.data.dendrites(axonnumber).voxel=uint16(voxel); 
    maxx=size(obj.data.ch(ch).filteredArray,2);
    maxy=size(obj.data.ch(ch).filteredArray,1);
    maxz=size(obj.data.ch(ch).filteredArray,3);
    obj.data.dendrites(axonnumber).maxint=double(obj.data.ch(ch).filteredArray(voxel(1,:)+(voxel(2,:)-1)*maxy+(voxel(3,:)-1)*maxy*maxx));
    %        obj.data.dendrites(axonnumber).maxint=[];  
    obj.data.dendrites(axonnumber).maxintcorrected=[];
    obj.data.dendrites(axonnumber).maxintcube=[];
    obj.data.dendrites(axonnumber).medianfiltered=[];
    obj.data.dendrites(axonnumber).meanbacksingle=[];
    obj.data.dendrites(axonnumber).maxintsingle=[];
    obj.data.dendrites(axonnumber).thres=[];
    obj.data.dendrites(axonnumber).meanback=[];
    
    obj=CorrectDendrites(obj);
end 


