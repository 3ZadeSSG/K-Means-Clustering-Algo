import java.io.File;
import java.io.FileNotFoundException;
import java.util.Random;
import java.util.Scanner;
public class KMean {
	private static double diss,sim;
	private static class K_mean{
		double data[][];
		int cluster[][];
		double centroid[][];
		double L[];
		double U[];
		int attributes,total_clusters,total_data;
		public void display_data() {
			for(int i=0;i<total_data;i++) {
				System.out.print("\nLine "+i+" : ");
				for(int j=0;j<attributes;j++) {
					System.out.format(" .3f ",data[i][j]);
				}
			}
			
		}
		public void display_clusters() {
			for(int i=0;i<total_clusters;i++) {
				System.out.print("\nCluster "+i+"= "+cluster[i][0]+" :");
				for(int j=1;j<cluster[i][0];j++) {
					System.out.print(cluster[i][j]+" ");
				}
			}
		}
		public void display_centroid() {
			for(int i=0;i<total_clusters;i++) {
				System.out.print("\nCentroid "+i+" : ");
				for(int j=0;j<attributes;j++) {
					System.out.format("%.3f ",centroid[i][j]);
				}
			}
		}
		public double eu_distance(double v1[],double v2[]) {
			double sum=0;
			for(int i=0;i<v1.length;i++) {
				sum+=Math.pow(v1[i]-v2[i], 2);
			}
			return Math.sqrt(sum);
		}
		public void create_cluster() {
			diss=sim=0;
			for(int i=0;i<total_clusters;i++) {
				cluster[i][0]=0;
			}
			for(int i=0;i<total_data;i++) {
				double min=eu_distance(centroid[0],data[i]);
				double max=min;
				int index=0;
				for(int j=0;j<total_clusters;j++) {
					double d=eu_distance(centroid[j],data[i]);
					if(d<min) {
						min=d;
						index=j;
					}
					if(d>max) {
						max=d;
					}
				}
				cluster[index][0]++;
				cluster[index][cluster[index][0]]=i;
				sim+=min;
				diss+=max;
			}
		}
		public void update_centroid() {
			for(int i=0;i<total_clusters;i++) {
				for(int j=0;j<attributes;j++) {
					double sum=0;
					if(cluster[i][0]!=0) {
						for(int k=1;k<cluster[i][0];k++) {
							sum+=data[cluster[i][k]][j];
						}
					}
					centroid[i][j]=sum/cluster[i][0];
				}
			}
		}
		public void create_centroid() {
			Random r=new Random();
			for(int i=0;i<total_clusters;i++) {
				for(int j=0;j<attributes;j++) {
					centroid[i][j]=L[j]+(U[j]-L[j])*r.nextDouble();
				}
			}
		}
		public void read_data() {
			try {
				Scanner sc=new Scanner(new File("G:\\biodata.csv"));
				String[] Line=new String[3];
	            Line=sc.nextLine().split(",");
	            total_data=Integer.parseUnsignedInt(Line[0]);
	            attributes=Integer.parseUnsignedInt(Line[1]);
	            total_clusters=Integer.parseUnsignedInt(Line[2]);
	            
	            data=new double[total_data][attributes];
	            cluster=new int[total_clusters][total_data+1];
	            centroid=new double[total_clusters][attributes];
	            L=new double[attributes];
	            U=new double[attributes];
	            
	            String[] data_line=new String[attributes+1];// Including actual cluster number in 1st column
	       	 	data_line=sc.nextLine().split(",");
		       	 for(int j=1;j<(attributes+1);j++){
	         		double val;
	         		val=Double.parseDouble(data_line[j-1]);
	         		data[0][j-1]=val;
	         		L[j-1]=val;
	         		U[j-1]=val;
	         	}
		       int 	index=1;
	            while(index<total_data)
	            {
	            	data_line=sc.nextLine().split(",");
		            for(int j=1;j<(attributes);j++)
		            {
		            	double Val;
		            	Val=Double.parseDouble(data_line[j]);
		            	data[index][j-1]=Val;
		            	if(L[j-1]>Val)L[j-1]=Val;
		            	if(U[j-1]<Val)U[j-1]=Val;
		            }
		            index++;
	            }       
			} catch (FileNotFoundException e) {
				e.printStackTrace();
				System.exit(0);
			}	
		}
		public void execute() {
			double tempD,tempS;
			System.out.println("\n===========Initial Centroids and Clusters==========\n");
			create_centroid();
			display_centroid();
			create_cluster();
			display_clusters();
			System.out.format("\nSimilairty: %.3f \tDisimilarity: %.3f",sim,diss);
			tempD=diss;
			tempS=sim;
			System.out.println("\n\n===========Final Centroids and Clusters==========\n");
			for(int i=0;i<99;i++) {
//				System.out.println("\nIteration "+i+"\n");
				update_centroid();
				create_cluster();
				if(tempD==diss && tempS==sim) {
					System.out.print("\nIteration: "+i);
					break;
				}
				tempD=diss;
				tempS=sim;
//				display_clusters();
			}
			display_centroid();
			display_clusters();
			System.out.format("\nSimilairty: %.3f \tDisimilarity: %.3f",sim,diss);
		}
	}
	public static void main(String[] args) {
		K_mean k=new K_mean();
		k.read_data();
		k.execute();
	}
}