#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<math.h>
#include<fstream>
#include<vector>
using namespace std;
class KMean{
	vector<vector<double> >data; 
	vector<vector<double> >centroid; 
	vector<vector<int> >cluster;
	vector<double>L;
	vector<double>U;
	int attributes,total_clusters,total_data;
	double similar;
	double dissimilar;
public:
	void display_clusters();
	void display_centroids();
	void display_data();
	void get_data(); //read data from file and resize corresponding data,centroid and cluster space
	void execute(); //create initial centroids and start iterating until optimal clusters found
	void create_cluster();
	void create_centroid(); //create initial centroids randomly
	void update_centroid();
	double manhattan_distance(vector<double>,vector<double>); 
	double eu_distance(vector<double>,vector<double>);
};
void KMean::display_data(){
	for(int i=0;i<total_data;i++){
		cout<<"\nLine "<<i<<": ";
		for(int j=0;j<attributes;j++){
			cout<<data[i][j]<<" ";
		}
	}
}
void KMean::display_clusters(){
	for(int i=0;i<total_clusters;i++){
		cout<<"\nCluster "<<i<<" ["<<cluster[i].size()<<"]: ";
		for(int j=0;j<cluster[i].size();j++){
			cout<<cluster[i][j]<<" ";
		}
	}
}
void KMean::display_centroids(){
	for(int i=0;i<total_clusters;i++){
		cout<<"\nCentroid "<<i<<" : ";
		for(int j=0;j<attributes;j++){
			cout<<setprecision(2)<<centroid[i][j]<<" ";
		}
	}
}
double KMean::manhattan_distance(vector<double>v1,vector<double>v2){
	double sum=0;
	for(int i=0;i<v1.size();i++){
		sum+=abs(v1[i]-v2[i]);
	}
	return sum;
}
double KMean::eu_distance(vector<double>v1,vector<double>v2){
	double sum=0;
	for(int i=0;i<v1.size();i++){
		sum+=pow((v1[i]-v2[i]),2);
	}
	return pow(sum,1/2.);
}
void KMean::create_cluster(){
	double similarity=0;
	double dis=0;
	for(int i=0;i<total_clusters;i++){
		cluster[i].resize(0);		//for all clusters resize their size to zero hence eventually emptying them
	}
	for(int i=0;i<total_data;i++){
		int index=0;
		double min=eu_distance(centroid[0],data[i]);
		double max=min;
		for(int j=0;j<total_clusters;j++){
			double d=eu_distance(centroid[j],data[i]);
			if(d<min){
				min=d;
				index=j;
			}
			if(d>max){
				d=max;
			}
		}
		similarity+=min;
		dis+=max;
		cluster[index].push_back(i); //push the index at rear of the cluster number "index" hence expanding its size as new data added
	}
	similar=similarity;
	dissimilar=dis;
//	cout<<"\n===================\nSimilarityMeasure: "<<setprecision(8)<<similarity<<endl;
	//cout<<"DismilarityMeasure: "<<setprecision(8)<<disimilarity<<endl;
}
void KMean::update_centroid(){
	for(int i=0;i<total_clusters;i++){
		for(int j=0;j<attributes;j++){
			if(cluster[i].size()!=0){ //if cluster i is not empty then update corresponding centroid
				double sum=0;
				for(int k=0;k<cluster[i].size();k++){ //iterate till the end of current cluster
					sum+=data[k][j];
				}
				centroid[i][j]=sum/cluster[i].size();	//find out average for corresponding cluster X corresponding attribute and insert into centroid
			}
		}
	}
}
void KMean::create_centroid(){
	for(int i=0;i<total_clusters;i++){
		for(int j=0;j<attributes;j++){
			centroid[i][j]=L[j]+(U[j]-L[j])*( ((double) rand() / (RAND_MAX)));
		}
	}
}
void KMean::get_data(){
	ifstream ptr("G:\\biodata.csv");
	string s;
	getline(ptr,s);
	int count=0;
	int index=-1;
	vector<int>v;
	while(count!=3){
		string element;
		index++;
		while(s[index]!=','){
			element+=s[index++];
		}
		v.push_back(atoi(element.c_str()));
		count++;
	}
	total_data=v[0];
	attributes=v[1];
	total_clusters=v[2];
	//total_clusters=2;
	data.resize(total_data,vector<double>(attributes,0)); //resize data matrix into "total_data X attributes" size
	centroid.resize(total_clusters,vector<double>(attributes,0)); //resize the centroid into "total_cluster X attributes"  size
	cluster.resize(total_clusters); //resize the cluster into "total_cluster" size i.e., only rows are initialized, the columns i.e., data can be added using "push_back"
	for(int i=0;i<total_data;i++){
   		getline(ptr,s);
		 count=0;
		 index=-1;
		 while(count!=attributes){
		 	string temp;
		 	index++;
		 	while(s[index]!=','){
		 		temp+=s[index++];
			 }
			 data[i][count]=atof(temp.c_str());
			 count++;
		 }
	}
	for(int i=0;i<attributes;i++){
		L.push_back(data[0][i]);
		U.push_back(data[0][i]);
	}
	for(int j=0;j<attributes;j++){
		for(int i=0;i<total_data;i++){
			if(L[j]>data[i][j]){
				L[j]=data[i][j];
			}
			if(U[j]<data[i][j]){
				U[j]=data[i][j];
			}
		}
	}
	//display_data();	//display inserted data set
}
void KMean::execute(){
	cout<<"\n====================Initial centroids and Clusters=============\n\n";
	create_centroid();
	display_centroids();
	create_cluster();
	display_clusters();
	cout<<"\nSimilarity: "<<fixed<<setprecision(4)<<similar<<"\t Dissimilarity: "<<setprecision(4)<<dissimilar<<endl;
	double tempS,tempD;
	tempS=similar;
	tempD=dissimilar;
	cout<<"\n\n====================Final centroids and Clusters=============\n\n";
	for(int i=0;i<100;i++){
		update_centroid();
		create_cluster();
		if(tempS==similar || tempD==dissimilar){
			cout<<"\nIteration : "<<i<<endl;
			break;
		}
		tempS=similar;
		tempD=dissimilar;
	//	cout<<endl<<"Iteration: "<<i+1<<endl; //for each iteration print the centroids and clusters
	//	display_centroids();
	//	display_clusters();
	}
	display_centroids();
	display_clusters();
	cout<<"\nSimilarity: "<<fixed<<setprecision(4)<<similar<<"\t Dissimilarity: "<<setprecision(4)<<dissimilar<<endl;
}
int main(){
	KMean K;
	K.get_data();
	K.execute();
	return 0;
}
