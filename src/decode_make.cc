#include "decode_make.hh"


// using namespace std;
// struct Header_t {
//   char           event_header[4];
//   unsigned int   serial_number;
//   unsigned short year;
//   unsigned short month;
//   unsigned short day;
//   unsigned short hour;
//   unsigned short minute;
//   unsigned short second;
//   unsigned short millisecond;
//   unsigned short reserved1;
//   float time[1024];
// };
// 
// struct Waveform_t {
//   char           chn1_header[4];
//   unsigned short chn1[1024];
//   char           chn2_header[4];
//   unsigned short chn2[1024];
//   char           chn3_header[4];
//   unsigned short chn3[1024];
// //  char           chn4_header[4];
// //  unsigned short chn4[1024];
// };
// 

Int_t Get1stTriggerTime(Float_t trigger_level,Double_t chn_trg[],Int_t &nTrigs) {
  vector<Int_t> vecTrigTimes;
  nTrigs = 0;
  Int_t trigger_on = 0;
  Int_t i_on = -1;
  //vector<Int_t> trigger_time  = -1;
  Int_t min_trigger_length = 7; // 7  = 10 ns at 0.7 GSamples/s
  Int_t first_trigger_time  = -1;
  for (Int_t i=0;i<1024;i++){
    //Turn the trigger status ON if signal goes below trigger level (Usually -200 mV)
    if (chn_trg[i]<trigger_level && trigger_on == 0) {
      trigger_on = 1;
      i_on=i;
    }
    //Turn trigger status to 0 (OFF) if the singal goes back above the trigger level
    if (chn_trg[i]>trigger_level and trigger_on == 1) {
      trigger_on = 0;
      // If the trigger was longer than the minimum trigger length count the trigger
      if(i_on+min_trigger_length<i) {
        //trigger_time.push_back(i_on);
        nTrigs++;
        vecTrigTimes.push_back(i_on);
      }
      if(nTrigs == 1) first_trigger_time = i_on;
    }
  }
  //cout<<"\nGet1stTriggerTime"<<endl;
  //cout<<first_trigger_time<<" "<<nTrigs<<endl;
  return first_trigger_time;
}


void decode(TString filename) {

  cout << "this is the stopwatch at start\n===============" << std::endl;
  TStopwatch  stopwatch;

  Header_t header;
  Waveform_t waveform;
  Int_t year, month, day;
  Double_t  time_stamp;
  Double_t t[1024], chn1[1024], chn2[1024], chn3[1024], chn4[1024];
  int calibflag;
  Int_t n_channels = 4;
  Int_t n;
  Int_t k =0;
  //  cout << "0" << endl;
  
  // open the binary waveform file
  FILE *f;
  if (filename.EndsWith(".dat"))
    filename.ReplaceAll(".dat","");
  cout <<"Filename: "<<filename<<endl;
  f = fopen64(filename+".dat", "r");
  cout<<f<<endl;
  if (!f){
    cout<<"Cannot read file \""<<filename<<"\". Please take a valid file."<<endl;
    return;
  }
  //  cout << "1" << endl;
  
  //open the root file
  TFile *outfile = new TFile(filename+".root", "RECREATE");
  
  // define the output tree
  TTree *rec = new TTree("rec","rec");
  rec->SetAutoSave(10000000000); // prevent the tree from writing itself multiple times into the file. currently set to 10 GB
  rec->Branch("year", &year ,"year/I");
  rec->Branch("month", &month ,"month/I");
  rec->Branch("day", &day ,"day/I");
  rec->Branch("time_stamp", &time_stamp ,"time_stamp/D");
  rec->Branch("t", &t   ,"t[1024]/D");  
  rec->Branch("n", &k   ,"n/I");  
  // Create branches for the channels
  rec->Branch("chn1", &chn1 ,"chn1[1024]/D");
  rec->Branch("chn2", &chn2 ,"chn2[1024]/D");
  rec->Branch("calibflag", &calibflag ,"calibflag/I");
  Int_t nTrigs;
  Int_t trigTime;
  rec->Branch("nTrigs",&nTrigs,"nTrigs/I");
  rec->Branch("trigTime",&trigTime,"trigTime/I");
//  rec->Branch("chn3", &chn3 ,"chn3[1024]/D");
//  rec->Branch("chn4", &chn4 ,"chn4[1024]/D");
  //  cout << "21" << endl;
  TriggerDelay dt_data(128);
  dt_data.SetName("DATA");
  TriggerDelay dt_cali(16);
  dt_cali.SetName("CAL");

  
  // loop over all events in data file
  for (n=0 ; fread(&header, sizeof(header), 1, f) > 0; n++) {
    //  cout << "30" << endl;
    if (n%1000 == 0) cout << n << std::endl;
    // decode time     
    if( n == 0) cout << "event header  " << header.event_header << std::endl;

    // get the year/month/date
    year = header.year;
    month = header.month;
    day = header.day;

    // print this info for every 10kth event
    if (n%10000 == 0) cout << "year "<< year << ": month " << month << ": day " << day << " : hour " << header.hour << " : minute " << header.minute << std::endl; 
    // get the timestamp
    time_stamp = header.hour*3600+header.minute*60+header.second+header.millisecond/1000.; 
    if (n == 0) cout << "millisecond " << header.millisecond << std::endl;

// marc speedup    for (Int_t i=0; i<1024; i++)
// marc speedup      t[i] = (Double_t) header.time[i];

    //  cout << "3" << endl;
    if(n == 0) cout << "waveform size : " <<    sizeof(waveform)/4*n_channels << endl;

    // read the waveform
    float sizeofwaveform = fread(&waveform, sizeof(waveform), 1, f);
    // cout << "sizeofwaveform: " << sizeofwaveform << std::endl;
    
    // decode amplitudes in mV
    float chn3_int = 0.;
    for (Int_t i=0; i<1024; i++) {
      chn1[i] = (Double_t) ((waveform.chn1[i]) / 65535. - 0.5) * 1000;   	
      chn2[i] = (Double_t) ((waveform.chn2[i]) / 65535. - 0.5) * 1000;   
      chn3_int += (Double_t) ((waveform.chn3[i]) / 65535. - 0.5) * 1000;
      t[i] = (Double_t) header.time[i]; // marc speedup
//      chn3[i] = (Double_t) ((waveform.chn3[i]) / 65535. - 0.5) * 1000;   
//      chn4[i] = (Double_t) ((waveform.chn4[i]) / 65535. - 0.5) * 1000;   
    }

    calibflag = (fabs(chn3_int) > 100000.) ? 1:0;
    trigTime = Get1stTriggerTime(-200,chn2,nTrigs);
    if(calibflag)
      dt_cali.AddWaveform(chn1,1024);
    else
      dt_data.AddWaveform(chn1,1024);


    //cout << "5" << endl;
    rec->Fill();
    k++;
  }
      
  // print number of events
  //  cout<<TString::Format("\r%4d/%4d \t%5.1f",ientry,nentries,(Float_t)ientry/(Float_t)(nentries)*100)<<flush;
  cout<<n<<" events processed"<<endl;
  cout<<"\""<< filename+".root" <<"\" written"<<endl;
  
  // save and close root file
  outfile->cd();
  rec->Write();
  outfile->Close();

  cout << "this is the stopwatch at end\n===============" << std::endl;
  stopwatch.Stop();
  cout << "cpu time:  " << stopwatch.CpuTime() << std::endl;
  cout << "real time: " << stopwatch.RealTime() << std::endl;
  stopwatch.Reset();


}

void usage(){
  cout<< "usage:\n ./decode -i <filename>" << std::endl;
}

int main(int argc, char* argv[]){

  TString infile;
  // Parse options
  char ch;
  while ((ch = getopt(argc, argv, "i:h")) != -1 ) {
    switch (ch) {
      case 'i': infile  = TString(optarg);  break;
      case 'h': usage(); break;
      default:
      cerr << "*** Error: unknown option " << optarg << std::endl;
      usage();
    }
  }
  
  // Check arguments
  if( argc<1 ) {
    usage();
  }

  cout<<"calling decode on file " << infile << ".dat" << std::endl;
  decode(infile);
  cout<<"done. exiting..."<<std::endl;

}


