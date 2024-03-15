clearvars;
close all;

%% AUDISEN music player Matlab simulator
%%
%% (C) ISEN Ouest 2024 PJB


fileName = 'fichiers_musicaux/Playlist.frm';% change here the name of file *.frm
%% parameters
FRAME_SIZE=21; % data frame size
Fs = 44100;% sampling frequency

freqTab=[0.0,...
65.407,...
69.296,...
73.417,...
77.782,...
82.407,...
87.308,...
92.499,...
97.999,...
103.827,...
110,...
116.541,...
123.471,...
130.813,...
138.592,...
146.833,...
155.564,...
164.814,...
174.615,...
184.998,...
195.998,...
207.653,...
220,...
233.082,...
246.942,...
261.626,...
277.183,...
293.665,...
311.127,...
329.628,...
349.229,...
369.995,...
391.996,...
415.305,...
440,...
466.164,...
493.884,...
523.252,...
554.366,...
587.33,...
622.254,...
659.256,...
698.457,...
739.989,...
783.991,...
830.61,...
880,...
932.328,...
987.767,...
1046.503,...
1108.731,...
1174.66,...
1244.508,...
1318.511,...
1396.913,...
1479.978,...
1567.982,...
1661.219,...
1760,...
1864.656,...
1975.534];%% frequencies tab of notes (0->60)


notesTab = {
'0',...
'C1',...
'C#1',...
'D1',...
'D#1',...
'E1',...
'F1',...
'F#1',...
'G1',...
'G#1',...
'A1',...
'A#1',...
'B1',...
'C2',...
'C#2',...
'D2',...
'D#2',...
'E2',...
'F2',...
'F#2',...
'G2',...
'G#2',...
'A2',...
'A#2',...
'B2',...
'C3',...
'C#3',...
'D3',...
'D#3',...
'E3',...
'F3',...
'F#3',...
'G3',...
'G#3',...
'A3',...
'A#3',...
'B3',...
'C4',...
'C#4',...
'D4',...
'D#4',...
'E4',...
'F4',...
'F#4',...
'G4',...
'G#4',...
'A4',...
'A#4',...
'B4',...
'C5',...
'C#5',...
'D5',...
'D#5',...
'E5',...
'F5',...
'F#5',...
'G5',...
'G#5',...
'A5',...
'A#5',...
'B5'};


%% load playlist

fileID = fopen(fileName);
data = fread(fileID);
fclose(fileID);
idf=find(data(1:end-1)==13 & data(2:end)==10);% find LF and CR
if(isempty(idf))
    errordlg('Erroneous file');
    error('Erroneous file');
end
NTab=[idf(1);idf(2:end)-idf(1:end-1)];
nSongs=sum(NTab>FRAME_SIZE);% get number of songs by detecting init frames
if(nSongs==0)
    errordlg('No song detected');
    error('No song detected');
end
idf = [-1;idf];
note = zeros(1,4);

cpt=1;
for k=1:nSongs
    s=[];
    try
        frame=data((idf(cpt)+2):(idf(cpt+1)-1));
    catch
        errordlg('Frame cannot be extracted');
        error('Frame cannot be extracted');
    end

    idfc=find(frame==',');
    idfs=find(frame=='*');
    title=char(frame(2:idfc(1)-1)');
    tpm = str2double(char(transpose(frame((idfc(1)+1):(idfc(2)-1)))));
    nTicks=str2double(char(transpose(frame((idfc(2)+1):(idfs(1)-1)))));
   
    if(checkFrame(frame)~=1)
        errordlg('Erroneous checksum !');
        error('Erroneous checksum !');
    end
    fprintf('Start playing %s at %d tpm for %d ticks\n',title,tpm,nTicks);
    cpt=cpt+1;
    T = 60/tpm;
    t = (0:1/Fs:T)';
    accent =0.5* (T-t)/T;
    for n=1:nTicks
        try
            frame = data(idf(cpt)+2:idf(cpt+1)-1);
        catch
            errordlg('Frame cannot be extracted');
            error('Frame cannot be extracted');
        end
        if(checkFrame(frame)~=1)
            errordlg('Erroneous checksum !');
            error('Erroneous checksum !');
        end
        idfc=find(frame==',');
        idfs=find(frame=='*');
        accentFlag=str2double(char(transpose(frame((idfc(1)+1):(idfc(2)-1)))));
        note(1)=str2double(char(transpose(frame((idfc(2)+1):(idfc(3)-1)))));
        note(2)=str2double(char(transpose(frame((idfc(3)+1):(idfc(4)-1)))));
        note(3)=str2double(char(transpose(frame((idfc(4)+1):(idfc(5)-1)))));
        note(4)=str2double(char(transpose(frame((idfc(5)+1):(idfs(1)-1)))));
        s=[s;sum(sin(repmat(2*pi*freqTab(note+1),length(t),1).*repmat(t,1,length(note))),2).*(0.5+accent*accentFlag)/sum(note>0)];
        cpt = cpt+1;
    end
    playerObj=audioplayer(s,Fs);
    playblocking(playerObj);
    fprintf('Stop playing %s [%f s]\n',title,length(s)/Fs)
end


function flag=checkFrame(frame)

    checksum=char(frame(end-1:end))';
    ref=getChecksum(char(frame(2:end-3))');
    flag = (ref==hex2dec(checksum));

end

function xor=getChecksum(str)
    code = double(str);
    xor=code(1);
    for k=2:length(code)
        xor=bitxor(xor,code(k));
    end
end

