// -------------------------------------------------------------------------
// -----                        R3BCalo header file                    -----
// -----                  Created 26/03/09  by D.Bertini               -----
// -----	     Last modification 28/05/12 by P.Cabanelas         -----
// -------------------------------------------------------------------------

/**  R3BCalo.h
 **/


#ifndef R3BCALO_H
#define R3BCALO_H

#include "R3BDetector.h"

#include "TLorentzVector.h"

class TClonesArray;
class R3BCaloPoint;
class R3BCaloCrystalHit;
class R3BCaloCrystalHitSim;
class FairVolume;
class TGeoRotation;


class R3BCalo : public R3BDetector
{

 public:

  /** Default constructor **/
  R3BCalo();


  /** Standard constructor.
   *@param name    detetcor name
   *@param active  sensitivity flag
   **/
  R3BCalo(const char* name, Bool_t active);


  /** Destructor **/
  virtual ~R3BCalo();


  /** Virtual method ProcessHits
   **
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a R3BCaloPoint and adds it to the
   ** collection.
   *@param vol  Pointer to the active volume
   **/
  virtual Bool_t ProcessHits(FairVolume* vol = 0);


  /** Virtual method BeginEvent
   **
   ** If verbosity level is set, print hit collection at the
   ** end of the event and resets it afterwards.
   **/
  virtual void BeginEvent();

	
  /** Virtual method EndOfEvent
   **
   ** Added support for R3BCaloCrystalHit
   ** If verbosity level is set, print hit collection at the
   ** end of the event and resets it afterwards.
   **/
  virtual void EndOfEvent();


  /** Virtual method Register
   **
   ** Registers the hit collection in the ROOT manager.
   **/
  virtual void Register();


  /** Accessor to the hit collection **/
  virtual TClonesArray* GetCollection(Int_t iColl) const;


  /** Virtual method Print
   **
   ** Screen output of hit collection.
   **/
  virtual void Print() const;


  /** Virtual method Reset
   **
   ** Clears the hit collection
   **/
  virtual void Reset();


  /** Virtual method CopyClones
   **
   ** Copies the hit collection with a given track index offset
   *@param cl1     Origin
   *@param cl2     Target
   *@param offset  Index offset
   **/
  virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2,
			  Int_t offset);


  /** Virtual method Construct geometry
   **
   ** Constructs the STS geometry
   **/
  virtual void ConstructGeometry();
	
	
	/** Public method ConstructOldGeometry
	 **
	 ** Defines the CALIFA v5 geometry (from a R3BSim translation, never tested)
	 **/
  void ConstructOldGeometry();

	
	/** Public method ConstructUserDefinedGeometry
	 **
	 ** Defines a CALIFA geometry defined by the user from the perl scripts output
	 **/
  void ConstructUserDefinedGeometry();
	
	
	/** Public method SelectGeometryVersion
	 **
	 ** Defines the geometry 
	 *@param version  Integer parameter used to select the geometry: 
	 **	0- CALIFA 5.0, including BARREL and ENDCAP.
	 **	1- CALIFA 7.05, only BARREL
	 ** 2- CALIFA 7.07, only BARREL
	 ** 3- CALIFA 7.09, only BARREL 
	 ** 4- CALIFA 7.17, only ENDCAP (in CsI[Tl])
	 ++    4.1 Phoswich endcap (CLF717_Geometry_PhoswichEndcap.geo): 
	 ** 5- CALIFA 7.07+7.17, 
	 ** 6- CALIFA 7.09+7.17,
	 ** 10- CALIFA 8.11, only BARREL 
	 ** ...
	 **/
  void SelectGeometryVersion(Int_t version);
	
	
	/** Public method SetNonUniformity
	 **
	 ** Defines the fNonUniformity parameter in % deviation from the central value 
	 *@param nonU  Double parameter setting the maximum non-uniformity allowed 
	 **/	
  void SetNonUniformity( Double_t nonU );

	
  virtual void Initialize();
  virtual void SetSpecialPhysicsCuts();
  void SetEnergyCutOff( Double_t cutE ){fCutE = cutE;}
  Double_t  GetEnergyCutOff ( ) {return fCutE;}


//  void SaveGeoParams();

  private:

    /** Track information to be stored until the track leaves the
	active volume. **/
    Int_t          fTrackID;           //!  track index
    Int_t          fTrackPID;          //!  particle identification
    Int_t          fVolumeID;          //!  volume id
    Int_t          fParentTrackID;     //!  parent track index
    Int_t          fUniqueID;          //!  particle unique id (e.g. if Delta electron, fUniqueID=9)
    TLorentzVector fPosIn, fPosOut;    //!  position
    TLorentzVector fMomIn, fMomOut;    //!  momentum
    Double32_t     fTime;              //!  time
    Double32_t     fLength;            //!  length
    Double32_t     fELoss;             //!  energy loss
    Double32_t     fCutE;              //!  energy loss
    Int_t          fPosIndex;          //!
    Int_t          fNSteps;            //!  Number of steps in the active volume
    Double32_t     fEinc;              //!  Total incident energy
    Bool_t         kGeoSaved;          //!
    TList*         flGeoPar;	       //!

    TClonesArray*  fCaloCollection;              //!  The hit collection
    TClonesArray*  fCaloCrystalHitCollection;    //!  The crystal hit collection

    Int_t fCrystalType[30];
    Int_t fAlveolusType[32];
    Int_t fAlveolusECType[3]; //For phoswich endcap (CLF717_Geometry_PhoswichEndcap.geo):Int_t fAlveolusECType[16];

	
    // Selecting the geometry of the CALIFA calorimeter
    Int_t fGeometryVersion;
    // Adding some non-uniformity preliminary description
    Double_t  fNonUniformity;
	
    /** Private method AddHit
     **
     ** Adds a CaloPoint to the HitCollection
     **/
    R3BCaloPoint* AddHit(Int_t trackID, Int_t detID, Int_t volid, Int_t copy, Int_t ident,
			TVector3 posIn,
			TVector3 pos_out, TVector3 momIn, 
			TVector3 momOut, Double_t time, 
			Double_t length, Double_t eLoss);

	
    /** Private method AddCrystalHit
     **
     ** Adds a CaloCrystalHitSim to the HitCollection
     **/
    R3BCaloCrystalHitSim* AddCrystalHit(Int_t type, Int_t copy, Int_t ident,
					Double_t energy, Double_t tof,
					Int_t steps, Double_t einc,
					Int_t trackid, Int_t volid, Int_t partrackid,
					Int_t pdgid, Int_t uniqueid);
	
	
    /** Private method NUSmearing
     **
     ** Smears the energy according to some non-uniformity distribution	
     ** Very simple preliminary scheme where the NU is introduced as a flat random
     ** distribution with limits fNonUniformity (%) of the energy value.
     **/
    Double_t NUSmearing(Double_t inputEnergy);
	
	
    /** Private method ResetParameters
     **
     ** Resets the private members for the track parameters
     **/
    void ResetParameters();
	
    TGeoRotation* createMatrix( Double_t phi, Double_t theta, Double_t psi);

    Int_t  GetCrystalType(Int_t volID);
    Int_t  GetAlveolusType(Int_t volID);
    Int_t  GetAlveolusECType(Int_t volID);

    ClassDef(R3BCalo,1);
};

inline Int_t R3BCalo::GetCrystalType(Int_t volID) {
Int_t type=-1;

for (Int_t i=0;i<30;i++ ){
    if (volID==fCrystalType[i]) {
	type=i+1; //
	return (type);
    }
}
return type;
}

inline Int_t R3BCalo::GetAlveolusType(Int_t volID) {
	Int_t type=-1;
	
	for (Int_t i=0;i<32;i++ ){
		if (volID==fAlveolusType[i]) {
			type=i+1; //
			return (type);
		}
	}
	return type;
}

inline Int_t R3BCalo::GetAlveolusECType(Int_t volID) {
	Int_t type=-1;
	
	for (Int_t i=0;i<3;i++ ){ //For phoswich endcap IEM ((CLF717_Geometry_PhoswichEndcap.geo): (Int_t i=0;i<15;i++ )
		if (volID==fAlveolusECType[i]) {
			type=i+1; //
			return (type);
		}
	}
	return type;
}

inline void R3BCalo::ResetParameters() {
  fTrackID = fVolumeID = fParentTrackID = fTrackPID = fUniqueID = 0;
  fPosIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fPosOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fTime = fLength = fELoss = fEinc = 0;
  fPosIndex = 0;
  fNSteps = 0;
};

#endif 
