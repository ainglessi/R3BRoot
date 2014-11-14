// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        R3BStarTrackUnpack                         -----
// -----                           Version 0.1                             -----
// -----                    Created 28.05.2014 by M. Labiche               -----
// -----                                                                   -----
// -----------------------------------------------------------------------------


//ROOT headers
#include "TClonesArray.h"
#include "TH1F.h"
#include "TMath.h"

//Fair headers
#include "FairRootManager.h"
#include "FairRunOnline.h"
#include "FairLogger.h"

//STaRTracker headers
#include "R3BStarTrackRawHit.h"
#include "R3BStarTrackUnpack.h"
//#include "R3BSTaRTraHit.h"

//#define NUMCHANNELS 1500
//#define NUMMODULES 1500

#include <iostream>
using namespace std;


//R3BStarTrackUnpack: Constructor
R3BStarTrackUnpack::R3BStarTrackUnpack(char *strTraDir,
                             Short_t type, Short_t subType,
                             Short_t procId,
                             Short_t subCrate, Short_t control)
: FairUnpack(type, subType, procId, subCrate, control),
fRawData(new TClonesArray("R3BStarTrackRawHit")),
fNHits(0)
{
}



//Virtual R3BStarTrackUnpack: Public method
R3BStarTrackUnpack::~R3BStarTrackUnpack()
{
  LOG(INFO) << "R3BStarTrackUnpack: Delete instance" << FairLogger::endl;
  delete fRawData;
}



//Init: Public method
Bool_t R3BStarTrackUnpack::Init()
{
  Register();
  LOG(INFO) << "Registration Completed" << FairLogger::endl;
  return kTRUE;
}



//Register: Protected method
void R3BStarTrackUnpack::Register()
{
  LOG(INFO) << "Registration of StarTrack Unpacker" << FairLogger::endl;
 FairRootManager *fMan = FairRootManager::Instance();
  if(! fMan) {
    return;
  }
  fMan->Register("StarTrackRawHit", "Raw data from R3B Si Tracker", fRawData, kTRUE);

  }



//DoUnpack: Public method
Bool_t R3BStarTrackUnpack::DoUnpack(Int_t *data, Int_t size)  // used for Mbs format datafile 
{
   
  // TODO: adapt it for Tracker when  data format is known for tracker
  
  LOG(INFO) << "R3BSTaRTrackUnpack : Unpacking... size = "  << size << FairLogger::endl;

  UInt_t l_s = 0;
 
  UInt_t* pl_data = (UInt_t*)(data + l_s); // let's find 0x00000200 
  
    l_s++;
    UInt_t* wr1 = (UInt_t*)(data + l_s); // MBS/WR sent by us to MBS
    l_s++;
    UInt_t* wr2 = (UInt_t*)(data + l_s); // MBS/WR sent by us to MBS
    l_s++;
    UInt_t* wr3 = (UInt_t*)(data + l_s); // MBS/WR sent by us to MBS
    l_s++;
    UInt_t* wr4 = (UInt_t*)(data + l_s); // MBS/WR sent by us to MBS
    l_s++;    

    //  WRhb1: WR high bits time stamp in header
    //  WRhb2: WR high bits time stamp in header
    //  WRlb:  Wr low bits time stamp in header
    //  WR:    WR time stamp in header
    //WRhb = UInt_t( ((wr4[0] & 0x00000fff) << 16) + ((wr3[0] & 0x0000ffff) << 4) + ((wr2[0] & 0x0000f000) >> 12) ); // bits: 28-59
    UInt_t WRhb1 = UInt_t((wr4[0] & 0x0000ffff)); // bits: 48-63
    UInt_t WRhb2 = UInt_t( ((wr3[0] & 0x0000ffff) << 4) + ((wr2[0] & 0x0000f000) >> 12) ); // 20 bits: 28-47
    UInt_t WRlb = UInt_t(( ((wr2[0] & 0x00000fff) << 16) + (wr1[0] & 0x0000ffff) )); // 28 bits: 0-27
    // + wr3[0] << 32 + wr2[0] << 16 + wr1[0];

    long long WR = WRhb1 << (48 + WRhb2) << (28 + WRlb);
    
    // initialisation of ts_hb1 and 2 as this is not done in the Si Tracker DAQ unless word 10 appear with 4&5 or 7&8 info codes.

    // ts_hb1; // 63:48 in Si (16 bits)
    // ts_hb2; // 47:28 in Si (20 bits)
    // ts_hb=0 ; // in Si
    // ts_lb=0 ; // in Si
    // long long ts=0 ; // 27:0 in Si (28 bits)	   
 
    //UInt_t ts_hb1=WRhb1; // high bits 48:63
    //UInt_t ts_hb2=WRhb2; // high bits 28:47
    //UInt_t ts_hb=WRhb2;  // (WRhb1 << 20)+WRhb2  // full high bit


    //if( pl_data[0]==0x00000200)
    //{

	//LOG(INFO) << "R3BSTaRTrackUnpack :   pl_data[0]:" << pl_data[0] << FairLogger::endl;
	//LOG(INFO) << "R3BSTaRTrackUnpack :   wr1[0]:" << wr1[0] << FairLogger::endl;
	//LOG(INFO) << "R3BSTaRTrackUnpack :   wr2[0]:" << wr2[0] << FairLogger::endl;
	//LOG(INFO) << "R3BSTaRTrackUnpack :   wr3[0]:" <<  wr3[0] << FairLogger::endl;
	//LOG(INFO) << "R3BSTaRTrackUnpack :   wr4[0]:" <<  wr4[0] << FairLogger::endl;
	//LOG(INFO) << "R3BSTaRTrackUnpack :   WRlb:" <<  WRlb << FairLogger::endl;
	//LOG(INFO) << "R3BSTaRTrackUnpack :   WRhb:" <<  WRhb << FairLogger::endl;
	//LOG(INFO) << "R3BSTaRTrackUnpack :   size:" << l_da_siz << FairLogger::endl;	
    //}


    if( pl_data[0]==0x00000200)
      {

	while(l_s < size) { 
	  	  
	  //pl_data = (UInt_t*)(data + l_s);
	  
	  UInt_t wordtype;  // 
	  UInt_t info_field;
	  UInt_t info_code;
	  
	  UInt_t ts_hb1;
	  UInt_t ts_hb2;
	  UInt_t ts_hb;
	  UInt_t ts_lb;
	  long long ts;
	  
	  // Time information from an external input (ie: other than Si tracker)
	  UInt_t tsExt_hb1; // 63:48 in Si (16 bits)
	  UInt_t tsExt_hb2; // 47:28 in Si (20 bits)
	  UInt_t tsExt_hb ; // in Si
	  UInt_t tsExt_lb; // 27:0 in Si (28 bits)	   
	  long long tsExt; //	   
	  
	  UInt_t hitbit;
	  UInt_t module_id;  // module id
	  UInt_t side;
	  UInt_t asic_id;    // Chip id
	  UInt_t strip_id;   // strip id 
	  UInt_t adcData;  // adc value for energy loss in Si
	  
	  for (Int_t i1 = 0; i1 < 2; i1++)   // 2 words to read.
	    { 

	      if( ( ( (pl_data[l_s] >> 30) & 0x3) == 0x3) && ( ((pl_data[l_s] & 0xFFFFFFFF) != 0xFFFFFFFF)))  
		{ 
		  
		  //LOG(INFO) << " pl_data: " <<  ((pl_data[l_s] >> 30) & 0x3)  << FairLogger::endl;
		  
		  //LOG(INFO) << "R3BSTaRTrackUnpack :   wordB :" <<  (pl_data[l_s] & 0xC0000000) << FairLogger::endl;
		  
		  // wordtype=11;
		  wordtype = (pl_data[l_s] >> 30) & 0x3; // bit 31:30
		  if(wordtype!=3) cout << wordtype << endl;
		  
		  hitbit= (pl_data[l_s] >> 29) & 0x01;
		  
		  //LOG(INFO) << "R3BSTaRTrackUnpack :   hitbit :" <<  hitbit << FairLogger::endl;
		  
		  adcData = (pl_data[l_s] & 0x00000FFF);
		  
		  UInt_t ADCchanIdent=  (pl_data[l_s] >> 12) & 0x0001FFFF; //17 bits after a shift of 12 bit
		  
		  module_id = (ADCchanIdent >> 12) & 0x0000001F;
		  
		  side = (ADCchanIdent >> 11) & 0x00000001;
		  
		  asic_id = (ADCchanIdent >> 7) & 0x0000000F;
		  
		  strip_id = ADCchanIdent & 0x0000007F;
		  
		  ts_lb= pl_data[l_s+1] & 0x0FFFFFFF;  // low bit time stamp in Silicon
		  
		  l_s +=2;	  
		  
		  //LOG(DEBUG) << "R3BStartrackerUnpack : Strip_ID IS " << strip_id << ",  Chip ID IS " << asic_id << " , Ladder ID IS " << module_id << " , ADC Data IS " << adcData << FairLogger::endl;
		  //new ((*fRawData)[fNHits]) R3BStarTrackRawHit(module_id, side, asic_id, strip_id, adcData, lclock);
		  
		  //new ((*fRawData)[fNHits]) R3BStarTrackRawHit(wordtype, hitbit, module_id, side, asic_id, strip_id, adcData, lclock);
		  //new ((*fRawData)[fNHits]) R3BStarTrackRawHit(wordtype, hitbit, module_id, side, asic_id, strip_id, adcData, ts2);
		  //new ((*fRawData)[fNHits]) R3BStarTrackRawHit(WRhb2,WRlb, wordtype, hitbit, module_id, side, asic_id, strip_id, adcData, ts_hb, ts_lb,tsExt_hb , tsExt_lb,info_field, info_code);
		  new ((*fRawData)[fNHits]) R3BStarTrackRawHit(WR,WRlb, wordtype, hitbit, module_id, side, asic_id, strip_id, adcData, ts, ts_lb, tsExt, tsExt_lb,info_field, info_code);
		  fNHits++;
		  
		}else if(((pl_data[l_s] >> 30) & 0x3) == 0x2 && ( (pl_data[l_s] & 0xFFFFFFFF) != 0xFFFFFFFF ))
		{
		  wordtype = (pl_data[l_s] >> 30) & 0x3; // bit 31:30
		  
		  //info_code = (pl_data[l_s] >> 20) & 0x0000000F; //bits 20:23
		  
		  info_code = (pl_data[l_s] & 0x00F00000) >> 20; //bits 20:23
		  
		  info_field =  (pl_data[l_s]) & 0x000FFFFF; //20 bits: 0-19;
		  
		  tsExt_lb = (pl_data[(l_s+1)] & 0x0FFFFFFF); // low bit timestamp from master trigger  (To be checked)
		  
		  if(info_code == 4 || info_code == 7) ts_hb2=info_field; 
		  if(info_code == 5 || info_code == 8) ts_hb1=info_field;
		  //if(ts_hb1 && ts_hb2)
		    ts=ts_hb1 << (48 + ts_hb2) << (28 + ts_lb);
		  

		  if(info_code == 14) tsExt_hb2=info_field; // high bit (47:28) time from the external Master
		  if(info_code == 15) tsExt_hb1=info_field; // high bit (63:48)time from the external Master
		  if(info_code == 14 || info_code == 15){
		    tsExt_lb = (pl_data[(l_s+1)] & 0x0FFFFFFF); // low bit timestamp from master trigger  (To be checked)
		    //if(tsExt_hb1 && tsExt_hb2) 
		      tsExt=tsExt_hb1 << (48 + tsExt_hb2) << (28 + tsExt_lb);  // full timestamp from Master trigger
		  }

		  ts_hb= (ts_hb1 << 20) + ts_hb2;
		  //tsExt_hb=tsExt_hb2;
		  //tsExt=tsExt_hb1 << 48 + tsExt_hb2 << 28;

		  l_s +=2;
		} else
		{
		  LOG(INFO) << " Warning:  Word not recognised) :";
		  l_s++;	  
		}
	      
	     	      
	      
	      
	      
	    }	   
	  
	  
	}	
	
      }
    
    LOG(INFO) << "R3BSTaRTrackUnpack : Number of hits in STarTracker: " << fNHits << FairLogger::endl;
    return kTRUE;    
}


//DoUnpack2: Public method  (use for R3B Si tracker and data file from DL)
// Each block_main should contain a pair of words: 

/*
Bool_t R3BStarTrackUnpack::DoUnpack2(Int_t *data_word0, Int_t *data_word1, Int_t size)   
{
  
  LOG(INFO) << "R3BSTaRTrackUnpack2 : Unpacking... size = " << size << FairLogger::endl;

  UInt_t l_s = 0;

  UInt_t *pl_data_word0 = (UInt_t*) data_word0;   
  UInt_t *pl_data_word1 = (UInt_t*) data_word1;   
 
  UInt_t wordtype;  // 11 or 10
  //UInt_t evsize; 
  UInt_t info_field=0;
  UInt_t info_code=0;
  UInt_t timestamp; 
  UInt_t energy=0;  // energy loss in Si
  UInt_t side=0;  // module side
  UInt_t channel_id=0;  // Channel id
  UInt_t asic_id=0;  // Chip id
  UInt_t module_id=0;  // module id
  UInt_t hitbit=0;  // 0 or 1

 
  
  //cout << "size=" << size << endl; 
  //cout << "data_word0=" << *data_word0 << endl; 
  //cout << "data_word1=" << *data_word1 << endl; 
  //cout << "data_word0=" << *pl_data_word0 << endl; 
  //cout << "data_word1=" << *pl_data_word1 << endl; 
  

  //LOG(INFO) << "Unpacking Startracker data" << FairLogger::endl;
  LOG(DEBUG) << "Unpacking Startracker data" << FairLogger::endl;
 

  wordtype = (*data_word0 >> 30) & 0x3; // bit 31:30

  if(wordtype==2) LOG(INFO) << "Words type 2(10) or 3(11)=" << wordtype << FairLogger::endl;

  // Check if word_0 begins with:
  // - 10 then is type A word. 
  // - 11 then is type B word.

  //while(l_s < size) {
  //  l_s++;

      // A (10):
      if ( (*data_word0 & 0xC0000000)==0x80000000 && ( ((*data_word0 & 0xFFFFFFFF) != 0xFFFFFFFF) && ((*data_word1 & 0xFFFFFFFF) != 0xFFFFFFFF)) ){
	//cout << "Words type A (msb=10)." << std::endl;
	LOG(DEBUG) << "Words type A (msb=10)." << FairLogger::endl;
	word_0A=*pl_data_word0;
	word_1A=*pl_data_word1;
	
	while(l_s < size) {
	l_s++;
	
	
	// Check the trailer: reject or keep the block.
	//if( (*data_word0 & 0xFFFFFFFF) == 0xFFFFFFFF ||
	//  (*data_word1 & 0xFFFFFFFF) == 0xFFFFFFFF) {
	//  // this marks the point after which there is no more good data in the block_main
	//  // log_file << "End of block " << itr_1 << std::endl; 
	//  //LOG(INFO) << "End of block " <<  FairLogger::endl; 
	//  //flag_terminator=true;
	//  break;
	//  }
	//
	
	info_field = word_0A & 0x000FFFFF; //bits 0:19
	info_code = (word_0A >> 20) & 0x0000000F; //bits 20:23
	module_id = (word_0A >> 24) & 0x0000003F; //bits 24:29   
	
	// Extract time stamp.
	timestamp =  (unsigned long) (word_1A & 0x0FFFFFFF);
	
	// LOG(INFO) << "Info_field " << info_field <<  FairLogger::endl; 
	
	}  // end of while(l_s<size)

      } //end of A word case

      // B (11):
      if ( (*data_word0 & 0xC0000000)==0xC0000000 && ( ((*data_word0 & 0xFFFFFFFF) != 0xFFFFFFFF) && ((*data_word1 & 0xFFFFFFFF) != 0xFFFFFFFF)) ){
	//cout << "Words type B (msb=11)."<< std::endl;
	LOG(DEBUG) << "Words type B (msb=11)."<< std::endl;
	word_0B=*pl_data_word0;
	word_1B=*pl_data_word1;
	
 
	while(l_s < size) {
	l_s++;
      
	// Check the trailer: reject or keep the block.
	
	//  if( (*data_word0 & 0xFFFFFFFF) == 0xFFFFFFFF ||
	//  (*data_word1 & 0xFFFFFFFF) == 0xFFFFFFFF) {
	//  // this marks the point after which there is no more good data in the block_main
	//  // log_file << "End of block " << itr_1 << std::endl; 
	//  //LOG(INFO) << "End of block " <<  FairLogger::endl; 
	//  //flag_terminator=true;
	//  break;
	//  }
	
	
	LOG(DEBUG) << "At GOSIP memory" << FairLogger::endl;
        
	// Real R3B Si Tracker data channel
	
	// Extract time stamp.
	// Msb from word_0A bits 0-19.
	// Lsb from word_0B bits 0-27.
	//timestamp =  (unsigned long long int) (word_0A & 0x000FFFFF) << 28 | (word_1B & 0x0FFFFFFF);
	timestamp =  (unsigned long) (word_1B & 0x0FFFFFFF);
	//timestamp =  (unsigned long long int) (word_1A & 0x000FFFFF) << 28 | (word_1B & 0x0FFFFFFF);
	
	// Extract Energy (ADC counts).
	// From word_2: 12 bits (0-11) = ADC_data
	energy = (word_0B & 0x00000FFF); 
	
	// Extract channel id.
	// From word_2: 7 bits (12-18). 
	channel_id = (word_0B & 0x0007F000) >> 12;
	
	// Extract ASIC id.
	// From word_2: 4bits (19-22).
	asic_id = (word_0B & 0x00780000) >> 19;
	
	// Extract Side.
	// From word_2: 1 bit (23).
	side = (word_0B & 0x00800000) >> 23;
	
	// Extract module id.
	// From word_2 bits 23-28.
	//module_id = (word_0B & 0x1F800000) >> 23;
	// From word_2: 5 bits (24-28).
	module_id = (word_0B & 0x1F000000) >> 24;
	
	// Extract hit bit .
	// From word_2: bit 29.
	hitbit = (word_0B & 0x20000000) >> 29;
	
	} // end of while(l_s<size) for B

 
      } // end of B word case
   
	    //LOG(INFO) << " --------- event " << FairLogger::endl
    LOG(DEBUG) << " --------- event " << FairLogger::endl
    << "        hitbit " << hitbit << FairLogger::endl
    << "        Channel_id " << channel_id << FairLogger::endl
    << "        ASIC_id " << asic_id << FairLogger::endl
    << "        Side " << side << FairLogger::endl
    << "        Module_id " << module_id << FairLogger::endl
    << "        energy " << energy << FairLogger::endl
    << "        timestamp " << timestamp << FairLogger::endl
    << "=================================" << FairLogger::endl;
    
    //new ((*fRawData)[fNHits]) R3BStarTrackRawHit(channel_id, asic_id, module_id, energy, timestamp);
    //new ((*fRawData)[fNHits]) R3BStarTrackRawHit(wordtype, hitbit, module_id, side, asic_id, channel_id, energy, timestamp);
    //new ((*fRawData)[fNHits]) R3BStarTrackRawHit(wordtype, hitbit, module_id, side, asic_id, channel_id, energy, timestamp, info_field, info_code);
    new ((*fRawData)[fNHits]) R3BStarTrackRawHit(timestamp, timestamp, wordtype, hitbit, module_id, side, asic_id, channel_id, energy, timestamp, timestamp,timestamp, timestamp, info_field, info_code);
    fNHits++;
 
  
  
  LOG(DEBUG) << "End of memory" << FairLogger::endl;
  LOG(DEBUG) << "R3BStarTrackUnpack: Number of Si Tracker raw hits: " << fNHits << FairLogger::endl;
  //LOG(INFO) << "R3BStarTrackUnpack: Number of Si Tracker raw hits: " << fNHits << FairLogger::endl;
  
  
  return kTRUE;
}
*/


//Reset: Public method
void R3BStarTrackUnpack::Reset()
{
  LOG(DEBUG) << "Clearing Data Structure" << FairLogger::endl;
  fRawData->Clear();
  fNHits = 0;
}



ClassImp(R3BStarTrackUnpack)
