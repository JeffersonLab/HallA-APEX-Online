// A very simple script to get map between ADC channels and PMTs
//  -- Zhihong Ye, 02/16/2011
{  
  ifstream file("easy.dat");
  ofstream ofile("map.dat");

  int roc[20], slot[20], start[20],end[20];
  
  int pmt;

  int i=0;
  while(!file.eof()){
    file >> roc[i] >> slot[i] >> start[i] >> end[i];
    i++;
  }
  int N=i;
  int k[20];
  
  cerr  <<"Counter#"<<"   "<<"ROC#"<<"   "<<"Slot#"<<"   "<<"Channel#"<<endl;;
  ofile <<"Counter#"<<"   "<<"ROC#"<<"   "<<"Slot#"<<"   "<<"Channel#"<<endl;;
  
  
  for(int j=0;j<N;j++)
    {
      for(k[j]=start[j];k[j]<end[j]+1;k[j]++)
	{
	  pmt+=1;
	  if(pmt<10){
	    cerr  <<pmt<<"          " <<roc[j]<<"       "<<slot[j]<<"       "<<k[j]<<endl;;
	    ofile <<pmt<<"          " <<roc[j]<<"       "<<slot[j]<<"       "<<k[j]<<endl;;
	  }
	  else{
	    cerr  <<pmt<<"         " <<roc[j]<<"       "<<slot[j]<<"       "<<k[j]<<endl;;
	    ofile <<pmt<<"         " <<roc[j]<<"       "<<slot[j]<<"       "<<k[j]<<endl;;
	  }
	}
    }
}
