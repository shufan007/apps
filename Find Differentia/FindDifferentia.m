% function ZhaoCha(str,t1,t2)
% 彩色图处理；聚类框图
% if nargin==1
    t1=0.2;t2=0.4;
% elseif nargin==2
  %  t2=0.35;
% end 
str=input('input the photo: ','s');
name=[str,'.jpg'];
photo=double(imread(name))/255;
A0=rgb2gray(photo);
 %imshow(A0);
 %pause
[m,n]=size(A0);
AL=[A0(:,2:n),A0(:,n)];
AH=[A0(2:m,:);A0(m,:)];
p_L=abs(A0-AL);    % 横向做差
p_H=abs(A0-AH);    % 纵向做差
%---------------------------
% 阈值化
for i=1:m
    for j=1:n
        if p_L(i,j)>t1
           p_L(i,j)=1;
        else
            p_L(i,j)=0;
        end
        if p_H(i,j)>t1
           p_H(i,j)=1;
        else
            p_H(i,j)=0;
        end
    end
end 
%——————————————————————————————————
% 寻找图像边界
%———————————————————————————————————
% %——————————找左右边界—————————
s1=sum(p_L);
kk1=fix(n/20);
k1=fix(n/2-n/10);
Ind1=find(s1>t2*m);
n1=length(Ind1);
for i=kk1:-1:1
    if s1(i)>t2*m
       f1=i;
       break;
    else
        f1=0;
    end
end
 flag1=0;       
for i=1:n1
    if Ind1(i)>k1 && Ind1(i)<n/2 
        if f1 && Ind1(i-1)==f1
           ind1=Ind1(i-1:i+2);
           break;
        
        else
            ind1(2)=Ind1(i);
            flag1=2;
            break;
        end
      elseif f1
            ind1(1)=f1;
            flag1=1;
            break;
    end
end
% 查漏补缺
if flag1==1 || ind1(3)-ind1(2)~=10
    ind1(2)=ind1(1)+498;
    ind1(3)=ind1(2)+10;
    ind1(4)=ind1(3)+498;
elseif flag1==2 
    ind1(1)=ind1(2)-498;
    ind1(3)=ind1(2)+10;
    ind1(4)=ind1(3)+498;
end
%———————————————————
%————————找上下边界——————
s2=sum(p_H');
k2=fix(m/2-m/10);
Ind2=find(s2>t2*n);
n2=length(Ind2);
kk2=fix(m/20);
for i=kk2:-1:1
    if s2(i)>t2*n
       f2=i;
       break;
    else
        f2=0;
    end
end
flag2=0;
for i=1:n2
    if Ind2(i)>k2

          if f2 && Ind2(i-1)==f2
            ind2=Ind2(i-1:i);
            break;
        elseif f2
            ind2(1)=f2;
            flag2=1;
            break;
        else
            ind2(2)=Ind2(i);
            flag2=2;
            break;
        end       
    end
end
if flag2==1
    ind2(2)=ind2(1)+450;
elseif flag2==2
    ind2(1)=ind2(2)-450;
end
%--------------------------------
v=1;  % 平移量
cut=2;
%p1=A0(ind2(1)+cut:ind2(2)-cut,ind1(1)+cut:ind1(2)-cut);
%p2=A0(ind2(1)+cut:ind2(2)-cut,ind1(3)+v+cut:ind1(4)+v-cut);
%========================================
pp1=photo(ind2(1)+cut:ind2(2)-cut,ind1(1)+cut:ind1(2)-cut,:);
pp2=photo(ind2(1)+cut:ind2(2)-cut,ind1(3)+v+cut:ind1(4)+v-cut,:);
photo1=abs(pp1-pp2);
%p1=A0(:,ind1(1):ind1(2));
%p2=A0(:,ind1(3)+v:ind1(4)+v);
%p=abs(p1-p2);
%figure(1);
%imshow(1-p);
% subplot(2,1,1);
% imshow(1-photo1);

%------------------------------------------------------------------------
pp=sum(photo1,3);
[x,y]=find(pp>0.4);
L=length(x);
X=[x,y];
nk=5;  % 聚类个数
IDX = kmeans(X,nk); % 聚类
cx=zeros(nk,2);
cy=zeros(nk,2);
for i=1:nk
    idx=find(IDX==i);  % 聚类索引
    cx(i,:)=[min(x(idx)),max(x(idx))];
    cy(i,:)=[min(y(idx)),max(y(idx))];
end   
% 校正坐标
cx=cx+ind2(1)+cut;
cy=cy+ind1(1)+cut;
d=ind1(2)+10+v;
w=3;  % 边框的宽度
% photo1=photo(1:ind1(2)+1,1:ind2(2)+1,:);
% subplot(2,1,2);
% imshow(photo);
%——————————————框出不同区域———————————
for q=1:nk
    for i=cx(q,1)-w:cx(q,2)+w
         dd=cy(q,2)-cy(q,1)+w;
        for j=cy(q,1)-w:cy(q,1)-1
            photo(i,j,1)=255;  photo(i,j+dd,1)=255;
            photo(i,j,2)=0;    photo(i,j+dd,2)=0;
            photo(i,j,3)=0;    photo(i,j+dd,3)=0;
            photo(i,j+d,1)=255;photo(i,j+d+dd,1)=255;
            photo(i,j+d,2)=0;  photo(i,j+d+dd,2)=0;
            photo(i,j+d,3)=0;  photo(i,j+d+dd,3)=0;
        end
    end
    for j=cy(q,1):cy(q,2)
        dd=cx(q,2)-cx(q,1)+w+1;
        for i=cx(q,1)-w:cx(q,1)-1
            photo(i,j,1)=1;  photo(i+dd,j,1)=1;
            photo(i,j,2)=0;    photo(i+dd,j,2)=0;
            photo(i,j,3)=0;    photo(i+dd,j,3)=0;
            photo(i,j+d,1)=1;photo(i+dd,j+d,1)=1;
            photo(i,j+d,2)=0;  photo(i+dd,j+d,2)=0;
            photo(i,j+d,3)=0;  photo(i+dd,j+d,3)=0;
        end
    end
    pause(0.2);
    imshow(photo);
end

