#include<bits/stdc++.h> 
using namespace std;

struct PIXEL{
	int r=0,g=0,b=0;
};struct image{
	string type="P3";
	int mode=255;
	PIXEL p[3000][3000];
	 
};
//csc-cot
int row,col,msk[3000][3000]={{0}},mini=3000,maxi=0,minj=3000,maxj=0;
image destin,source,mask,output;
PIXEL black,white;
fstream fin,fout;
pair<int,int> bd[10000],in[1000000];int bds=0,ins=0;//boundary,inside
string destinfile="destin2.ppm",sourcefile="source2.ppm",maskfile="mask2.ppm";
//前處理 
void opendestin(){
    fin.open(destinfile, ios::in); //open input file   ex: original.ppm, lena.ppm,.....
	fin >> destin.type >> col >> row >> destin.mode;
    for (int i=0; i<row; i++)
        for (int j=0; j<col; j++)
            fin >> destin.p[i][j].r >> destin.p[i][j].g >> destin.p[i][j].b ;
    fin.close();
}void opensource(){
    fin.open(sourcefile, ios::in); //open input file   ex: original.ppm, lena.ppm,.....
	fin >> source.type >> col >> row >> source.mode;
    for (int i=0; i<row; i++)
        for (int j=0; j<col; j++)
            fin >> source.p[i][j].r >> source.p[i][j].g >> source.p[i][j].b ;
    fin.close();
}bool bfs(int a,int b){
	int dx[4]={0,1,0,-1},dy[4]={1,0,-1,0};
	queue<pair<int,int>>q;
	q.push({a,b});
	while(!q.empty()){
		int i=q.front().first,j=q.front().second;q.pop();
		int f=0;
		for(int d=0;d<4;d++){
			int x=i+dx[d],y=j+dy[d];
			if(x>=0)if(y>=0)if(x<=row)if(y<=col)if(msk[x][y]){
				f++;
				if(msk[x][y]==3)msk[x][y]=2,q.push({x,y});
			}
		}
		if(f==4)msk[i][j]=2,in[ins]={i,j},ins++;
		else msk[i][j]=1;
	}
}bool dfs2(int i,int j,int d){
	if(msk[i][j]==1){
		bd[bds]={i,j},bds++;
		int dx[8]={0,-1,-1,-1,0,1,1,1},dy[8]={-1,-1,0,1,1,1,0,-1};
		msk[i][j]=4;
		for(int dd=0;dd<8;dd++){
			if(dfs2(i+dx[(d+dd)%8],j+dy[(d+dd)%8],(4+d+dd)%8))return 1;
		}
	}
	return 0;
}void openmask(){
    fin.open(maskfile, ios::in); //open input file   ex: original.ppm, lena.ppm,.....
	fin >> mask.type >> col >> row >> mask.mode;
	int a=3000,b=3000;
    for(int i=0; i<row; i++)for(int j=0; j<col; j++){
    	fin >> mask.p[i][j].r >> mask.p[i][j].g >> mask.p[i][j].b ;
    	if(mask.p[i][j].r+mask.p[i][j].g+mask.p[i][j].b>200){
    		if(a==3000)a=i,b=j;
    		msk[i][j]=3,mini=min(i,mini),maxi=max(i,maxi),minj=min(j,minj),maxj=max(j,maxj);
		}
	}
    fin.close();
	bfs(a,b);
	dfs2(a,b,0);
	cout<<"Read Images successfully\n";
}void writeimage(){
    string outfile;
    cout << "Input your  Output filename:";
    cin >> outfile;             // ex: original_gray.ppm, lena_gray.ppm
    int s=outfile.length();if(!(s>4&&outfile[s-4]=='.'&&outfile[s-3]=='p'&&outfile[s-2]=='p'&&outfile[s-1]=='m'))outfile+=".ppm";
    fout.open(outfile, ios::out); //open output file
    fout << output.type << endl;
    fout << col << " " <<row << endl;
    fout << output.mode << endl;
    for (int i=0; i<row; i++){
        for (int j=0; j<col; j++)
            fout << output.p[i][j].r << " " <<output.p[i][j].g << " " << output.p[i][j].b << " ";
        fout << endl;
    }
    cout << "Write Image Successfully!!"  << endl;
    fout.close();
}
//就只是貼上去 
void basic(){
	output=destin;
	for(int i=0;i<ins;i++){
		int x=in[i].first,y=in[i].second;
		output.p[x][y]=source.p[x][y];
	}
	for(int i=0;i<bds;i++){
		int x=bd[i].first,y=bd[i].second;
		output.p[x][y].r+=source.p[x][y].r,output.p[x][y].g+=source.p[x][y].g,output.p[x][y].b+=source.p[x][y].b;
		output.p[x][y].r/=2,output.p[x][y].g/=2,output.p[x][y].b/=2;
	}
}
//
double weight[10000]={0};
double difr[10000],difg[10000],difb[10000];
void caldif(){
	for(int k=0;k<bds;k++){
		int x=bd[k].first,y=bd[k].second;
		difr[k]=destin.p[x][y].r-source.p[x][y].r;
		difg[k]=destin.p[x][y].g-source.p[x][y].g;
		difb[k]=destin.p[x][y].b-source.p[x][y].b;
	}
}
void calweight1(int i,int j){
	output.p[i][j]=source.p[i][j];
	long double sum=0,tan[10000]={0},d[10000]={0};
	for(int k=0;k<bds;k++){
		int x=bd[k].first-i,y=bd[k].second-j;
		tan[k]=1.0*y/(x+0.5);
		d[k]=sqrt(x*x+y*y);
	}
	long double dr=0,dg=0,db=0;
	for(int k=0;k<bds;k++){
		weight[k]=(tan[k]-tan[(k+bds-1)%bds])/(1+tan[k]*tan[(k+bds-1)%bds])+(tan[(k+1)%bds]-tan[k])/(1+tan[(k+1)%bds]*tan[k]); //t2-t1/1+t1t2
		weight[k]/=d[k];
		sum+=weight[k];
		dr+=weight[k]*difr[k],dg+=weight[k]*difg[k],db+=weight[k]*difb[k];
	}
	output.p[i][j].r+=dr/sum,output.p[i][j].g+=dg/sum,output.p[i][j].b+=db/sum;
	if(output.p[i][j].r>255)output.p[i][j].r=255;if(output.p[i][j].r<0)output.p[i][j].r=0;
	if(output.p[i][j].g>255)output.p[i][j].g=255;if(output.p[i][j].g<0)output.p[i][j].g=0;
	if(output.p[i][j].b>255)output.p[i][j].b=255;if(output.p[i][j].b<0)output.p[i][j].b=0;
}void mvc1(){
	for(int k=0;k<ins;k++){
		calweight1(in[k].first,in[k].second);
	}
}
void calweight2(int i,int j){
	double sum=0,tan[10000]={0},d[10000]={0};
	for(int k=0;k<bds;k++){
		int x1=bd[(k+bds-1)%bds].first-i,y1=bd[(k+bds-1)%bds].second-j,x2=bd[k].first-i,y2=bd[k].second-j,x3=x1-x2,y3=y1-y2;
		double d1=x1*x1+y1*y1,d2=x2*x2+y2*y2,d3=x3*x3+y3*y3;
		double cos=(d1+d2-d3)/(2.0*sqrt(d1*d2));
		tan[k]=sqrt((1-cos)/(1+cos));
		d[k]=sqrt(d1);
	}
	output.p[i][j]=source.p[i][j];
	long double dr=0,dg=0,db=0;
	for(int k=0;k<bds-1;k++){
		weight[k]=(tan[k]+tan[(k+1)%bds])/d[k];
		sum+=weight[k];
		dr+=weight[k]*difr[k],dg+=weight[k]*difg[k],db+=weight[k]*difb[k];
	}
	output.p[i][j].r+=dr/sum,output.p[i][j].g+=dg/sum,output.p[i][j].b+=db/sum;
	if(output.p[i][j].r>255)output.p[i][j].r=255;if(output.p[i][j].r<0)output.p[i][j].r=0;
	if(output.p[i][j].g>255)output.p[i][j].g=255;if(output.p[i][j].g<0)output.p[i][j].g=0;
	if(output.p[i][j].b>255)output.p[i][j].b=255;if(output.p[i][j].b<0)output.p[i][j].b=0;
}void mvc2(){
	for(int k=0;k<ins;k++){
		calweight2(in[k].first,in[k].second);
	}
}
void calweight3(int i,int j){
	output.p[i][j]=source.p[i][j];
	long double sum=0;
	long double dr=0,dg=0,db=0;
	for(int k=0;k<bds;k++){
		int x=bd[k].first-i,y=bd[k].second-j;
		double d=sqrt(x*x+y*y);
		weight[k]=1.0/d;
		sum+=weight[k];
		dr+=weight[k]*difr[k],dg+=weight[k]*difg[k],db+=weight[k]*difb[k];
	}
	output.p[i][j].r+=dr/sum,output.p[i][j].g+=dg/sum,output.p[i][j].b+=db/sum;
	if(output.p[i][j].r>255)output.p[i][j].r=255;if(output.p[i][j].r<0)output.p[i][j].r=0;
	if(output.p[i][j].g>255)output.p[i][j].g=255;if(output.p[i][j].g<0)output.p[i][j].g=0;
	if(output.p[i][j].b>255)output.p[i][j].b=255;if(output.p[i][j].b<0)output.p[i][j].b=0;
}void mvc3(){
	for(int k=0;k<ins;k++){
		calweight3(in[k].first,in[k].second);
	}
}
void calweight4(int i,int j){
	output.p[i][j]=source.p[i][j];
	long double sum=0;
	long double dr=0,dg=0,db=0;
	for(int k=0;k<bds;k++){
		int x=bd[k].first-i,y=bd[k].second-j;
		int d=x*x+y*y;
		weight[k]=1.0/d;
		sum+=weight[k];
		dr+=weight[k]*difr[k],dg+=weight[k]*difg[k],db+=weight[k]*difb[k];
	}
	output.p[i][j].r+=dr/sum,output.p[i][j].g+=dg/sum,output.p[i][j].b+=db/sum;
	if(output.p[i][j].r>255)output.p[i][j].r=255;if(output.p[i][j].r<0)output.p[i][j].r=0;
	if(output.p[i][j].g>255)output.p[i][j].g=255;if(output.p[i][j].g<0)output.p[i][j].g=0;
	if(output.p[i][j].b>255)output.p[i][j].b=255;if(output.p[i][j].b<0)output.p[i][j].b=0;
}void mvc4(){
	for(int k=0;k<ins;k++){
		calweight4(in[k].first,in[k].second);
	}
}
void calweight5(int i,int j){
	double sum=0,tan[10000]={0},d[10000]={0};
	for(int k=0;k<bds;k++){
		int x1=bd[(k+bds-1)%bds].first-i,y1=bd[(k+bds-1)%bds].second-j,x2=bd[k].first-i,y2=bd[k].second-j,x3=x1-x2,y3=y1-y2;
		double d1=x1*x1+y1*y1,d2=x2*x2+y2*y2,d3=x3*x3+y3*y3;
		double cos=(d1+d2-d3)/(2.0*sqrt(d1*d2));
		tan[k]=sqrt((1-cos)/(1+cos));
		d[k]=(d1);
	}
	output.p[i][j]=source.p[i][j];
	long double dr=0,dg=0,db=0;
	for(int k=0;k<bds-1;k++){
		weight[k]=(tan[k]+tan[(k+1)%bds])/d[k];
		sum+=weight[k];
		dr+=weight[k]*difr[k],dg+=weight[k]*difg[k],db+=weight[k]*difb[k];
	}
	output.p[i][j].r+=dr/sum,output.p[i][j].g+=dg/sum,output.p[i][j].b+=db/sum;
	if(output.p[i][j].r>255)output.p[i][j].r=255;if(output.p[i][j].r<0)output.p[i][j].r=0;
	if(output.p[i][j].g>255)output.p[i][j].g=255;if(output.p[i][j].g<0)output.p[i][j].g=0;
	if(output.p[i][j].b>255)output.p[i][j].b=255;if(output.p[i][j].b<0)output.p[i][j].b=0;
}void mvc5(){
	for(int k=0;k<ins;k++){
		calweight5(in[k].first,in[k].second);
	}
}
void start(){
	white.r=255,white.g=255,white.b=255;
	opendestin();//sky
	opensource();//plane
	openmask();
	caldif();
	output=destin;
}

int main(){
	start();
	cout<<"choose:\n0:basic\n1:mvc1 w=(tana+tanb)/d\n2:mvc2(true) w=(tana/2+tanb/2)/d\n3:mvc3 w=1/d\n4:mvc4 w=1/d^2\n5:mvc5 w=(tana/2+tanb/2)/d\n";
	int n;
	while(cin>>n){
		output=destin;
		if(n==0)basic();
		else if(n==1)mvc1();
		else if(n==2)mvc2();
		else if(n==3)mvc3();
		else if(n==4)mvc4();
		else if(n==5)mvc5();
		cout<<"done\n";
		writeimage();
	}
		
    return 0;
}


