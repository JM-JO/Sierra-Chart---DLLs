/*
v1.0 (2019/02/05) : création de "Points_pivots_FNQ100_v1.0" en copiant le code source de "Points_pivots_INQ100_v2.5". Les PP J/H/M sont calculés à partir des graphes externes J/H/M.
v1.1 (2019/02/05) : les PP 1h/4h sont calculés à partir des graphes externes 1h/4h.
v1.2 (2019/4/3) : les PP mensuels sont calculés de manière correcte à 18h, le dernier jour du mois.
v2.0 (2019/7/4) : le tracé de tous les niveaux futures est togglé ON/OFF via un bouton. Le statut est géré par la study "Prix typique". Les fonctions du DLL study Points_pivots_FNQ100 consultent ce statut pour gérer la variable sc.HideStudy.
v2.1 (2019/7/6) : gestion automatique de sc.GraphRegion.
v2.2 (2019/10/27) : tracé des PP en vectoriel. Le niveau retenu est celui de index = sc.LastVisibleBar.
v2.3 (2019/10/27) : le tracé de tous les niveaux futures est togglé ON/OFF via l'input TraceVectorielONOFF (vs la variable sc.HideStudy précédemment).
	
	
	
	
Améliorations possibles : 
	- créer une fonction unique pour corriger les bugs de flux (commune à tous les indicateurs) : BugDeFlux (char UT [J/H/M], int Year, int Month, int Day, &HighCorrige, &LowCorrige, &CloseCorrige)
	- ajouter un test pour valider que le ExternalChartNumber retenu est le bon (la bonne UT et le bon instrument).
	
	
	
SOMMAIRE DES FONCTIONS :
CalculatePPs
BooleanHideStudy
scsf_MesPointsPivotsMensuels
scsf_MesPointsPivotsHebdo
scsf_MesPointsPivotsJournaliers
scsf_MesPointsPivotsQuatreHeures
scsf_MesPointsPivotsUneHeure

	
	
*/

#include "sierrachart.h"  // When including in your source code file additional header files, they need to be placed above the #include "sierrachart.h" line. 

SCDLLName("Points pivots du future NQ100")



// Calcul des niveaux Full/Mid/Quart ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CalculatePPs ( 	float PriorHigh, float PriorLow, float PriorClose, float& PP, float& R1, float& R2, float& R3, float& R4, float& S1, float& S2, float& S3, float& S4, float& mR1, float& mR2, float& mR3, float& mR4, float& mS1, float& mS2, float& mS3, float& mS4, 
							float& RQ1, float& RQ2, float& RQ3, float& RQ4, float& RQ5, float& RQ6, float& RQ7, float& RQ8, float& SQ1, float& SQ2, float& SQ3, float& SQ4, float& SQ5, float& SQ6, float& SQ7, float& SQ8)
	{
		PP = (PriorHigh + PriorLow + PriorClose) / 3;
		R1 = (PP * 2) - PriorLow;
		R2 = PP + (PriorHigh - PriorLow);
		R3 = 2 * PP + (PriorHigh - 2*PriorLow); 
		R4 = R3 + PriorHigh - PriorLow;
		//R4bis = 3 * PP + (PriorHigh - 3 * PriorLow);
		S1 = (2 * PP) - PriorHigh;
		S2 = PP - (PriorHigh - PriorLow);
		S3 = 2 * PP - (2 * PriorHigh - PriorLow);
		S4 = S3 - PriorHigh + PriorLow;
		//S4bis = 3 * PP - (3 * PriorHigh - PriorLow);
		mR1 = (PP + R1) / 2;
		mR2 = (R1 + R2) / 2;
		mR3 = (R2 + R3) / 2;
		mR4 = (R3 + R4) / 2;
		mS1 = (PP + S1) / 2;
		mS2 = (S1 + S2) / 2;
		mS3 = (S2 + S3) / 2;
		mS4 = (S3 + S4) / 2;
		
		// - calcul des R/S quarts :
		RQ1 = (PP+mR1)/2;
		RQ2 = (mR1+R1)/2;
		RQ3 = (R1+mR2)/2;
		RQ4 = (mR2+R2)/2;
		RQ5 = (R2+mR3)/2;
		RQ6 = (mR3+R3)/2;
		RQ7 = (R3+mR4)/2;
		RQ8 = (mR4+R4)/2;
		SQ1 = (PP+mS1)/2;
		SQ2 = (mS1+S1)/2;
		SQ3 = (S1+mS2)/2;
		SQ4 = (mS2+S2)/2;
		SQ5 = (S2+mS3)/2;
		SQ6 = (mS3+S3)/2;
		SQ7 = (S3+mS4)/2;
		SQ8 = (mS4+S4)/2;
	}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int BooleanHideStudy(s_sc* ptrsc)
{
	int ShowStudy = 1 ;
	int StudyIDPrixTypique = ptrsc->GetStudyIDByName(ptrsc->ChartNumber, "Prix typique", 0);
	if  (StudyIDPrixTypique == 0 )  
	{	SCString message ;  message.Format("!! ERREUR !! Le graphique ne contient pas la study ''Prix typique''. Necessaire pour determiner sc.HideStudy.") ; ptrsc->AddMessageToLog(message,1) ; }
	else
	{	ptrsc->GetChartStudyInputInt(ptrsc->ChartNumber, StudyIDPrixTypique, 3, ShowStudy) ;  }    // int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
	return (1 - ShowStudy) ;    // HideStudy = 1 - ShowStudy
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
struct MonTexteString : s_UseTool // MonTexteString crée du texte avec un string
{											
	MonTexteString(SCString Texte, float niveau, int epaisseur, COLORREF rgb, s_sc* ptrsc) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_ALWAYS;			
		DrawingType = DRAWING_TEXT;
		//BeginDateTime = 0;  // 0 = position à gauche ; 150 pour la position à droite
		//TextAlignment = DT_VCENTER | DT_LEFT ;
		BeginDateTime = 150;  // 0 = position à gauche ; 150 pour la position à droite
		TextAlignment = DT_VCENTER | DT_RIGHT ;
		//FontBold = 1;
		Color = RGB(220, 220, 220); //background gris du graphique
		BeginValue = niveau;
		FontSize = epaisseur+6;
		FontBackColor = rgb;
		Text = Texte;			
		Region = ptrsc->GraphRegion ; 
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MaLigneJusquAIndex : s_UseTool  // MaLigneJusquALastIndex crée une ligne entre index=beginIndex et index=endIndex
	{
		MaLigneJusquAIndex(int beginIndex, int endIndex, float niveau, int epaisseur, COLORREF rgb, s_sc* ptrsc) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
		{
			Clear(); 
			DrawingType = DRAWING_LINE;
			LineStyle = LINESTYLE_SOLID;
			AddMethod = UTAM_ADD_ALWAYS;	
			BeginIndex = beginIndex ;
			EndIndex = endIndex ;
			BeginValue = niveau ;
			EndValue = niveau ;
			LineWidth = epaisseur;
			Color = rgb; 
			Region = ptrsc->GraphRegion ; 
		} 
	};
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
	{
		if ( monEpaisseur > 0 )
		{
			//Création d'instance de MaLigneJusquAIndex et appel de sc.UseTool
			MaLigneJusquAIndex objetMaLigne(beginIndex, endIndex, niveau, monEpaisseur, maCouleur, ptrsc);
			ptrsc->UseTool(objetMaLigne); // c'est sc.UseTool qui est appelé
			//Création d'instances de MonTexte et appel de sc.UseTool
			MonTexteString objetMonTexteString(Texte, niveau, monEpaisseur, maCouleur, ptrsc);
			ptrsc->UseTool(objetMonTexteString); 
		}
	}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsMensuels(SCStudyInterfaceRef sc)
{
	SCSubgraphRef PHPrior = sc.Subgraph[0];   // appelé par AMR
	SCSubgraphRef PBPrior = sc.Subgraph[1];	   // appelé par AMR
	SCSubgraphRef SettlePrior = sc.Subgraph[2];
	SCSubgraphRef PP = sc.Subgraph[3];   // appelé par AMR
	SCSubgraphRef R1 = sc.Subgraph[4];   // appelé par AMR
	SCSubgraphRef R2 = sc.Subgraph[5];   // appelé par AMR
	SCSubgraphRef R3 = sc.Subgraph[6];   // ...
	SCSubgraphRef R4 = sc.Subgraph[7];
	SCSubgraphRef S1 = sc.Subgraph[8];
	SCSubgraphRef S2 = sc.Subgraph[9];
	SCSubgraphRef S3 = sc.Subgraph[10];
	SCSubgraphRef S4 = sc.Subgraph[11];
	SCSubgraphRef mR1 = sc.Subgraph[12];
	SCSubgraphRef mR2 = sc.Subgraph[13];
	SCSubgraphRef mR3 = sc.Subgraph[14];
	SCSubgraphRef mR4 = sc.Subgraph[15];
	SCSubgraphRef mS1 = sc.Subgraph[16];
	SCSubgraphRef mS2 = sc.Subgraph[17];
	SCSubgraphRef mS3 = sc.Subgraph[18];
	SCSubgraphRef mS4 = sc.Subgraph[19];
	SCSubgraphRef RQ1 = sc.Subgraph[20];
	SCSubgraphRef RQ2 = sc.Subgraph[21];
	SCSubgraphRef RQ3 = sc.Subgraph[22];
	SCSubgraphRef RQ4 = sc.Subgraph[23];
	SCSubgraphRef RQ5 = sc.Subgraph[24];
	SCSubgraphRef RQ6 = sc.Subgraph[25];
	SCSubgraphRef RQ7 = sc.Subgraph[26];
	SCSubgraphRef RQ8 = sc.Subgraph[27];
	SCSubgraphRef SQ1 = sc.Subgraph[28];
	SCSubgraphRef SQ2 = sc.Subgraph[29];
	SCSubgraphRef SQ3 = sc.Subgraph[30];
	SCSubgraphRef SQ4 = sc.Subgraph[31];
	SCSubgraphRef SQ5 = sc.Subgraph[32];
	SCSubgraphRef SQ6 = sc.Subgraph[33];
	SCSubgraphRef SQ7 = sc.Subgraph[34];
	SCSubgraphRef SQ8 = sc.Subgraph[35];
	const int NUMBER_OF_STUDY_SUBGRAPHS = 36;  //35+1
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[0] ;  

	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[2];
	
	


	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "[1] Pivots Fut M (Full/Mid/Quart)";               // appelé par AMR
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = VALUEFORMAT_INHERITED;
		sc.GraphRegion = 2;
		sc.DisplayStudyName = 0;
		sc.DisplayStudyInputValues = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue = 0 ;
		
		sc.HideStudy = 1 ;

		
		ExternalChartNumber.Name = "Monthly Chart Number";
		ExternalChartNumber.SetChartNumber(3);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		

		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(0,0,0);
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineWidth = 1;
		}

		SettlePrior.DrawStyle = DRAWSTYLE_HIDDEN;
		
		PHPrior.PrimaryColor = RGB(255, 0, 0);
		PBPrior.PrimaryColor = RGB(0,168,0);
		SettlePrior.PrimaryColor = RGB(210,0,210);
		
		
		PHPrior.Name = "   PH MP";
		PBPrior.Name = "   PB MP";
		SettlePrior.Name = "Settle MP";  SettlePrior.LineWidth = 0;
		PP.Name = "      PP M";
		R1.Name = "      R1 M";
		R2.Name = "      R2 M";
		R3.Name = "      R3 M";
		R4.Name = "      R4 M";
		S1.Name = "      S1 M";
		S2.Name = "      S2 M";
		S3.Name = "      S3 M";
		S4.Name = "      S4 M";
		mR1.Name = "    mR1 M";
		mR2.Name = "    mR2 M";
		mR3.Name = "    mR3 M";
		mR4.Name = "    mR4 M";
		mS1.Name = "    mS1 M";
		mS2.Name = "    mS2 M";
		mS3.Name = "    mS3 M";
		mS4.Name = "    mS4 M";
		RQ1.Name = "   RQ1 M";
		RQ2.Name = "   RQ2 M";
		RQ3.Name = "   RQ3 M";
		RQ4.Name = "   RQ4 M";
		RQ5.Name = "   RQ5 M";
		RQ6.Name = "   RQ6 M";
		RQ7.Name = "   RQ7 M";
		RQ8.Name = "   RQ8 M";
		SQ1.Name = "   SQ1 M";
		SQ2.Name = "   SQ2 M";
		SQ3.Name = "   SQ3 M";
		SQ4.Name = "   SQ4 M";
		SQ5.Name = "   SQ5 M";
		SQ6.Name = "   SQ6 M";
		SQ7.Name = "   SQ7 M";
		SQ8.Name = "   SQ8 M";
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop   // REMPLACER LES index par index
	{
		if 	(  
				sc.BaseDateTimeIn.TimeAt(index) == 64800    and   sc.BaseDateTimeIn.TimeAt(index-1)!= 64800    or index == 0        // condition 1 : il est 18h00m00s, index est le premier chandelier de la séance
				/* and   (  not  (   ( Year==2019 and Month==2 and Day==18)  or  ( Year==2019 and Month==1 and Day==21)   )   )  */     // condition 2 : exclusion des journées sans settlement
			)
		{	
			// Year, Month, Day
			int Year, Month, Day;
			sc.BaseDateTimeIn[index].GetDateYMD(Year, Month, Day);     
			
			if				  // condition 3 : on change de mois :   (  (on est le dernier jour du mois) OU ( le mois a changé entre deux chandeliers successifs càd l'open est un dimanche ou un jour férié )  )
				(    (  Month==1 and Day==31 )  or (  Month==2 and Day==28 )  or (  Month==3 and Day==31 )  or (  Month==4 and Day==30 )  or (  Month==5 and Day==31 )  or (  Month==6 and Day==30 )  or (  Month==7 and Day==31 )  or 
				(  Month==8 and Day==31 )  or (  Month==9 and Day==30 )  or (  Month==10 and Day==31 )  or (  Month==11 and Day==30 )  or (  Month==1 and Day==31 )  or (  sc.BaseDateTimeIn[index].GetMonth()  !=   sc.BaseDateTimeIn[index-1].GetMonth()  )   )    
			{
				FirstIndex[index] = index ;
				
				SCGraphData ExternalChartData;
				sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
				int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // avant minuit, ExternalChartIndex retourne l'index du mois qui se termine, bien que la bougie du nouveau mois ait déjà débuté (elle correspond à ExternalChartIndex+1)
				
				// déclarations
				float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fPP = 0 ;
				float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
				float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
				float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
				float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;
				
				// on récupère HLC
				fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex];
				fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex];
				fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex];
				
				// correction des bugs de flux : à coder
			
				//on calcule les PP 
				CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
				
				// les valeurs sont entrées dans les sc.Subgraph[]
				PHPrior[index] = fPHPrior;
				PBPrior[index] = fPBPrior;
				SettlePrior[index] = fSettlePrior;
				PP[index] = fPP;
				R1[index] = fR1;
				R2[index] = fR2;
				R3[index] = fR3;
				R4[index] = fR4;
				S1[index] = fS1;
				S2[index] = fS2;
				S3[index] = fS3;
				S4[index] = fS4;
				mR1[index] = fmR1;
				mR2[index] = fmR2;
				mR3[index] = fmR3;
				mR4[index] = fmR4;
				mS1[index] = fmS1;
				mS2[index] = fmS2;
				mS3[index] = fmS3;
				mS4[index] = fmS4;
				RQ1[index] = fRQ1;
				RQ2[index] = fRQ2;
				RQ3[index] = fRQ3;
				RQ4[index] = fRQ4;
				RQ5[index] = fRQ5;
				RQ6[index] = fRQ6;
				RQ7[index] = fRQ7;
				RQ8[index] = fRQ8;
				SQ1[index] = fSQ1;
				SQ2[index] = fSQ2;
				SQ3[index] = fSQ3;
				SQ4[index] = fSQ4;
				SQ5[index] = fSQ5;
				SQ6[index] = fSQ6;
				SQ7[index] = fSQ7;
				SQ8[index] = fSQ8;		
			
			}
			
			else // on a changé de séance mais pas de mois
			{
				if ( index == 0 )   // les PP n'ont encore jamais été calculés => il faut les calculer
				{
					FirstIndex[index] = index ;
				
					SCGraphData ExternalChartData;
					sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
					int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // ExternalChartIndex retourne l'index du mois en cours, débuté lors d'une séance précédente
					
					// déclarations
					float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fPP = 0 ;
					float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
					float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
					float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
					float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;
					
					// on récupère HLC
					fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex-1];
					fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex-1];
					fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex-1];
					
					// correction des bugs de flux : à coder
				
					//on calcule les PP 
					CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
					
					// les valeurs sont entrées dans les sc.Subgraph[]
					PHPrior[index] = fPHPrior;
					PBPrior[index] = fPBPrior;
					SettlePrior[index] = fSettlePrior;
					PP[index] = fPP;
					R1[index] = fR1;
					R2[index] = fR2;
					R3[index] = fR3;
					R4[index] = fR4;
					S1[index] = fS1;
					S2[index] = fS2;
					S3[index] = fS3;
					S4[index] = fS4;
					mR1[index] = fmR1;
					mR2[index] = fmR2;
					mR3[index] = fmR3;
					mR4[index] = fmR4;
					mS1[index] = fmS1;
					mS2[index] = fmS2;
					mS3[index] = fmS3;
					mS4[index] = fmS4;
					RQ1[index] = fRQ1;
					RQ2[index] = fRQ2;
					RQ3[index] = fRQ3;
					RQ4[index] = fRQ4;
					RQ5[index] = fRQ5;
					RQ6[index] = fRQ6;
					RQ7[index] = fRQ7;
					RQ8[index] = fRQ8;
					SQ1[index] = fSQ1;
					SQ2[index] = fSQ2;
					SQ3[index] = fSQ3;
					SQ4[index] = fSQ4;
					SQ5[index] = fSQ5;
					SQ6[index] = fSQ6;
					SQ7[index] = fSQ7;
					SQ8[index] = fSQ8;		
				}
				
				else // les PP ont déjà été calculés précédemment ce mois-ci
				{
					FirstIndex[index] = FirstIndex[index-1] ;
					PHPrior[index] = PHPrior[index-1] ;
					PBPrior[index] = PBPrior[index-1] ;
					SettlePrior[index] = SettlePrior[index-1] ;
					PP[index] = PP[index-1];
					R1[index] = R1[index-1];
					R2[index] = R2[index-1];
					R3[index] = R3[index-1];
					R4[index] = R4[index-1];
					S1[index] = S1[index-1];
					S2[index] = S2[index-1];
					S3[index] = S3[index-1];
					S4[index] = S4[index-1];
					mR1[index] = mR1[index-1];
					mR2[index] = mR2[index-1];
					mR3[index] = mR3[index-1];
					mR4[index] = mR4[index-1];
					mS1[index] = mS1[index-1];
					mS2[index] = mS2[index-1];
					mS3[index] = mS3[index-1];
					mS4[index] = mS4[index-1];
					RQ1[index] = RQ1[index-1];
					RQ2[index] = RQ2[index-1];
					RQ3[index] = RQ3[index-1];
					RQ4[index] = RQ4[index-1];
					RQ5[index] = RQ5[index-1];
					RQ6[index] = RQ6[index-1];
					RQ7[index] = RQ7[index-1];
					RQ8[index] = RQ8[index-1];
					SQ1[index] = SQ1[index-1];
					SQ2[index] = SQ2[index-1];
					SQ3[index] = SQ3[index-1];
					SQ4[index] = SQ4[index-1];
					SQ5[index] = SQ5[index-1];
					SQ6[index] = SQ6[index-1];
					SQ7[index] = SQ7[index-1];
					SQ8[index] = SQ8[index-1];
				}
			}
			
		}

		else // on n'est pas dans le premier chandelier du jour
		{
			FirstIndex[index] = FirstIndex[index-1] ;
			PHPrior[index] = PHPrior[index-1] ;
			PBPrior[index] = PBPrior[index-1] ;
			SettlePrior[index] = SettlePrior[index-1] ;
			PP[index] = PP[index-1];
			R1[index] = R1[index-1];
			R2[index] = R2[index-1];
			R3[index] = R3[index-1];
			R4[index] = R4[index-1];
			S1[index] = S1[index-1];
			S2[index] = S2[index-1];
			S3[index] = S3[index-1];
			S4[index] = S4[index-1];
			mR1[index] = mR1[index-1];
			mR2[index] = mR2[index-1];
			mR3[index] = mR3[index-1];
			mR4[index] = mR4[index-1];
			mS1[index] = mS1[index-1];
			mS2[index] = mS2[index-1];
			mS3[index] = mS3[index-1];
			mS4[index] = mS4[index-1];
			RQ1[index] = RQ1[index-1];
			RQ2[index] = RQ2[index-1];
			RQ3[index] = RQ3[index-1];
			RQ4[index] = RQ4[index-1];
			RQ5[index] = RQ5[index-1];
			RQ6[index] = RQ6[index-1];
			RQ7[index] = RQ7[index-1];
			RQ8[index] = RQ8[index-1];
			SQ1[index] = SQ1[index-1];
			SQ2[index] = SQ2[index-1];
			SQ3[index] = SQ3[index-1];
			SQ4[index] = SQ4[index-1];
			SQ5[index] = SQ5[index-1];
			SQ6[index] = SQ6[index-1];
			SQ7[index] = SQ7[index-1];
			SQ8[index] = SQ8[index-1];
		}
	 
	}


	// gestion automatique de sc.GrapRegion
	if ( sc.UpdateStartIndex == 0 )
	{
		n_ACSIL :: s_BarPeriod BarPeriod ;
		sc.GetBarPeriodParameters(BarPeriod) ;
		
		if ( BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR )     // XTicks
		{
			if ( BarPeriod.IntradayChartBarPeriodParameter1 == 16 )  { sc.GraphRegion = 1 ;  }
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 200 )  { sc.GraphRegion = 0 ;  }
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 2000 )  { sc.GraphRegion = 1 ;  }
		}
	}
	
	
	if (sc.MenuEventID == 105)   // Bouton "Niv Fut ON/OFF"
	{
		if ( BooleanHideStudy(&sc) )   // on veut cacher la study
		{
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ; 
			TraceVectorielONOFF.SetYesNo(0) ;
		}
		else
		{
			TraceVectorielONOFF.SetYesNo(1) ;
		}
	}
	
	
	// Tracé vectoriel des sc.Subgraph
	if ( TraceVectorielONOFF.GetYesNo() )
	{	
		// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
		
		int const LastIndex = sc.IndexOfLastVisibleBar ;
		s_sc* const ptrsc = &sc;
		int const firstIndex = FirstIndex[LastIndex] ;  // index de la première bougie du mois : début du tracé de la ligne  // FirstIndex est un array qui contient le firstIndex
		
		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			float const Niveau = sc.Subgraph[SubgraphIndex][LastIndex] ;
			SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
			int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
			COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
			creationLigneEtTexte(Texte, firstIndex, LastIndex, Niveau, epaisseur, couleur, ptrsc);
		}
		
		// lorsque je suis déconnecté d'IB, pour les backtests :
		if ( sc.ServerConnectionState != SCS_CONNECTED )  
		{	sc.UpdateAlways = 1 ;  }
	}	
	

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsHebdo(SCStudyInterfaceRef sc)
{
	SCSubgraphRef PHPrior = sc.Subgraph[0];    // appelé par AMR
	SCSubgraphRef PBPrior = sc.Subgraph[1];	   // appelé par AMR
	SCSubgraphRef SettlePrior = sc.Subgraph[2];
	SCSubgraphRef PP = sc.Subgraph[3];   // appelé par AMR
	SCSubgraphRef R1 = sc.Subgraph[4];   // appelé par AMR
	SCSubgraphRef R2 = sc.Subgraph[5];   // appelé par AMR
	SCSubgraphRef R3 = sc.Subgraph[6];   // appelé par AMR
	SCSubgraphRef R4 = sc.Subgraph[7];   // appelé par AMR
	SCSubgraphRef S1 = sc.Subgraph[8];   // appelé par AMR
	SCSubgraphRef S2 = sc.Subgraph[9];   // appelé par AMR
	SCSubgraphRef S3 = sc.Subgraph[10];   // appelé par AMR
	SCSubgraphRef S4 = sc.Subgraph[11];   // appelé par AMR
	SCSubgraphRef mR1 = sc.Subgraph[12];   // appelé par AMR
	SCSubgraphRef mR2 = sc.Subgraph[13];   // appelé par AMR
	SCSubgraphRef mR3 = sc.Subgraph[14];   // appelé par AMR
	SCSubgraphRef mR4 = sc.Subgraph[15];   // appelé par AMR
	SCSubgraphRef mS1 = sc.Subgraph[16];   // appelé par AMR
	SCSubgraphRef mS2 = sc.Subgraph[17];   // appelé par AMR
	SCSubgraphRef mS3 = sc.Subgraph[18];   // etc..
	SCSubgraphRef mS4 = sc.Subgraph[19];
	SCSubgraphRef RQ1 = sc.Subgraph[20];
	SCSubgraphRef RQ2 = sc.Subgraph[21];
	SCSubgraphRef RQ3 = sc.Subgraph[22];
	SCSubgraphRef RQ4 = sc.Subgraph[23];
	SCSubgraphRef RQ5 = sc.Subgraph[24];
	SCSubgraphRef RQ6 = sc.Subgraph[25];
	SCSubgraphRef RQ7 = sc.Subgraph[26];
	SCSubgraphRef RQ8 = sc.Subgraph[27];
	SCSubgraphRef SQ1 = sc.Subgraph[28];
	SCSubgraphRef SQ2 = sc.Subgraph[29];
	SCSubgraphRef SQ3 = sc.Subgraph[30];
	SCSubgraphRef SQ4 = sc.Subgraph[31];
	SCSubgraphRef SQ5 = sc.Subgraph[32];
	SCSubgraphRef SQ6 = sc.Subgraph[33];
	SCSubgraphRef SQ7 = sc.Subgraph[34];    // ...
	SCSubgraphRef SQ8 = sc.Subgraph[35];    // appelé par AMR
	const int NUMBER_OF_STUDY_SUBGRAPHS = 36;  //35+1
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[0] ;  

	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[2];
	SCInputRef AffichageDesQuartsONOFF = sc.Input[3];
	


	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "[2] Pivots Fut H (Full/Mid/Quart)";           // appelé par AMR
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = VALUEFORMAT_INHERITED;
		sc.GraphRegion = 2;
		sc.DisplayStudyName = 0;
		sc.DisplayStudyInputValues = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue = 0 ;
		
		sc.HideStudy = 1 ;

		ExternalChartNumber.Name = "Weekly Chart Number";
		ExternalChartNumber.SetChartNumber(10);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		AffichageDesQuartsONOFF.Name = "Affichage des Quarts";
		AffichageDesQuartsONOFF.SetYesNo(0);

		

		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(130,130,130);
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineWidth = 1;
		}

		SettlePrior.DrawStyle = DRAWSTYLE_HIDDEN;
		
		PHPrior.PrimaryColor = RGB(255, 0, 0);
		PBPrior.PrimaryColor = RGB(0,168,0);
		SettlePrior.PrimaryColor = RGB(210,0,210);
		
		
		PHPrior.Name = "  PH HP";
		PBPrior.Name = "  PB HP";
		SettlePrior.Name = "Settle HP";  SettlePrior.LineWidth = 0;
		PP.Name = "     PP H";
		R1.Name = "     R1 H";
		R2.Name = "     R2 H";
		R3.Name = "     R3 H";
		R4.Name = "     R4 H";
		S1.Name = "     S1 H";
		S2.Name = "     S2 H";
		S3.Name = "     S3 H";
		S4.Name = "     S4 H";
		mR1.Name = "  mR1 H";
		mR2.Name = "  mR2 H";
		mR3.Name = "  mR3 H";
		mR4.Name = "  mR4 H";
		mS1.Name = "  mS1 H";
		mS2.Name = "  mS2 H";
		mS3.Name = "  mS3 H";
		mS4.Name = "  mS4 H";
		RQ1.Name = "  RQ1 H";
		RQ2.Name = "  RQ2 H";
		RQ3.Name = "  RQ3 H";
		RQ4.Name = "  RQ4 H";
		RQ5.Name = "  RQ5 H";
		RQ6.Name = "  RQ6 H";
		RQ7.Name = "  RQ7 H";
		RQ8.Name = "  RQ8 H";
		SQ1.Name = "  SQ1 H";
		SQ2.Name = "  SQ2 H";
		SQ3.Name = "  SQ3 H";
		SQ4.Name = "  SQ4 H";
		SQ5.Name = "  SQ5 H";
		SQ6.Name = "  SQ6 H";
		SQ7.Name = "  SQ7 H";
		SQ8.Name = "  SQ8 H";
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)             
	{
		
		if (   (  sc.BaseDateTimeIn[index].GetDayOfWeek()   <   sc.BaseDateTimeIn[index-1].GetDayOfWeek()   )   or   index==0    )   // ie on change de semaine 
		{
			FirstIndex[index] = index ;
			
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du nouveau chandelier sur le Weekly chart

			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fPP = 0 ;
			float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
			float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
			float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
			float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;

			// on récupère HLC : 
			if (  sc.BaseDateTimeIn[index].GetDayOfWeek() == 0  )  // c'est dimanche, et donc on va cherher les HLC dans la semaine en cours
			{	
				fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex];
				fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex];
				fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex];
			}
			else // ce n'est pas dimanche
			{
				fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex-1];
				fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex-1];
				fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex-1];
			}
			
			// correction des bugs de flux
			SCDateTimeArray ExternalChartDateTime;
			sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartDateTime);
			int Year, Month, Day;
			ExternalChartDateTime[ExternalChartIndex].GetDateYMD(Year, Month, Day);
		/* 	if (Year==2019 and Month==1 and Day==31)  // premier jour de la semaine pour lequel HLC est faux
			{
				//fPHPrior = nd;
				fPBPrior = nd;
				//fSettlePrior = nd;
			} */
			
			//on calcule les PP 
			CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
			
			// les valeurs sont entrées dans les sc.Subgraph[]
			PHPrior[index] = fPHPrior;
			PBPrior[index] = fPBPrior;
			SettlePrior[index] = fSettlePrior;
			PP[index] = fPP;
			R1[index] = fR1;
			R2[index] = fR2;
			R3[index] = fR3;
			R4[index] = fR4;
			S1[index] = fS1;
			S2[index] = fS2;
			S3[index] = fS3;
			S4[index] = fS4;
			mR1[index] = fmR1;
			mR2[index] = fmR2;
			mR3[index] = fmR3;
			mR4[index] = fmR4;
			mS1[index] = fmS1;
			mS2[index] = fmS2;
			mS3[index] = fmS3;
			mS4[index] = fmS4;
			RQ1[index] = fRQ1;
			RQ2[index] = fRQ2;
			RQ3[index] = fRQ3;
			RQ4[index] = fRQ4;
			RQ5[index] = fRQ5;
			RQ6[index] = fRQ6;
			RQ7[index] = fRQ7;
			RQ8[index] = fRQ8;
			SQ1[index] = fSQ1;
			SQ2[index] = fSQ2;
			SQ3[index] = fSQ3;
			SQ4[index] = fSQ4;
			SQ5[index] = fSQ5;
			SQ6[index] = fSQ6;
			SQ7[index] = fSQ7;
			SQ8[index] = fSQ8;
		}
	
		else // on n'est pas dans le premier chandelier du jour
		{
			FirstIndex[index] = FirstIndex[index-1] ;
			PHPrior[index] = PHPrior[index-1] ;
			PBPrior[index] = PBPrior[index-1] ;
			SettlePrior[index] = SettlePrior[index-1] ;
			PP[index] = PP[index-1];
			R1[index] = R1[index-1];
			R2[index] = R2[index-1];
			R3[index] = R3[index-1];
			R4[index] = R4[index-1];
			S1[index] = S1[index-1];
			S2[index] = S2[index-1];
			S3[index] = S3[index-1];
			S4[index] = S4[index-1];
			mR1[index] = mR1[index-1];
			mR2[index] = mR2[index-1];
			mR3[index] = mR3[index-1];
			mR4[index] = mR4[index-1];
			mS1[index] = mS1[index-1];
			mS2[index] = mS2[index-1];
			mS3[index] = mS3[index-1];
			mS4[index] = mS4[index-1];
			RQ1[index] = RQ1[index-1];
			RQ2[index] = RQ2[index-1];
			RQ3[index] = RQ3[index-1];
			RQ4[index] = RQ4[index-1];
			RQ5[index] = RQ5[index-1];
			RQ6[index] = RQ6[index-1];
			RQ7[index] = RQ7[index-1];
			RQ8[index] = RQ8[index-1];
			SQ1[index] = SQ1[index-1];
			SQ2[index] = SQ2[index-1];
			SQ3[index] = SQ3[index-1];
			SQ4[index] = SQ4[index-1];
			SQ5[index] = SQ5[index-1];
			SQ6[index] = SQ6[index-1];
			SQ7[index] = SQ7[index-1];
			SQ8[index] = SQ8[index-1];
		}
	}	
	

	
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		// gestion automatique de sc.GrapRegion
		n_ACSIL :: s_BarPeriod BarPeriod ;
		sc.GetBarPeriodParameters(BarPeriod) ;
		if ( BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR )     // XTicks
		{
			if ( BarPeriod.IntradayChartBarPeriodParameter1 == 16 )  { sc.GraphRegion = 1 ;  }
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 200 )  { sc.GraphRegion = 0 ;  }
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 2000 )  { sc.GraphRegion = 1 ;  }
		}
		
		if ( AffichageDesQuartsONOFF.GetYesNo() )
		{
			RQ1.LineWidth = 1 ;
			RQ2.LineWidth = 1 ;
			RQ3.LineWidth = 1 ;
			RQ4.LineWidth = 1 ;
			RQ5.LineWidth = 1 ;
			RQ6.LineWidth = 1 ;
			RQ7.LineWidth = 1 ;
			RQ8.LineWidth = 1 ;
			SQ1.LineWidth = 1 ;
			SQ2.LineWidth = 1 ;
			SQ3.LineWidth = 1 ;
			SQ4.LineWidth = 1 ;
			SQ5.LineWidth = 1 ;
			SQ6.LineWidth = 1 ;
			SQ7.LineWidth = 1 ;
			SQ8.LineWidth = 1 ;
		}
		else
		{
			//sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;
			RQ1.LineWidth = 0;
			RQ2.LineWidth = 0;
			RQ3.LineWidth = 0;
			RQ4.LineWidth = 0;
			RQ5.LineWidth = 0;
			RQ6.LineWidth = 0;
			RQ7.LineWidth = 0;
			RQ8.LineWidth = 0;
			SQ1.LineWidth = 0;
			SQ2.LineWidth = 0;
			SQ3.LineWidth = 0;
			SQ4.LineWidth = 0;
			SQ5.LineWidth = 0;
			SQ6.LineWidth = 0;
			SQ7.LineWidth = 0;
			SQ8.LineWidth = 0;
		}
	}


	if (sc.MenuEventID == 105)   // Bouton "Niv Fut ON/OFF"
	{
		if ( BooleanHideStudy(&sc) )   // on veut cacher la study
		{
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ; 
			TraceVectorielONOFF.SetYesNo(0) ;
		}
		else
		{
			TraceVectorielONOFF.SetYesNo(1) ;
		}
	}

	

	// Tracé vectoriel des sc.Subgraph
	if ( TraceVectorielONOFF.GetYesNo() )
	{	
		// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
		
		int const LastIndex = sc.IndexOfLastVisibleBar ;
		s_sc* const ptrsc = &sc;
		int const firstIndex = FirstIndex[LastIndex] ;  // index de la première bougie du mois : début du tracé de la ligne  // FirstIndex est un array qui contient le firstIndex
		
		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			float const Niveau = sc.Subgraph[SubgraphIndex][LastIndex] ;
			SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
			int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
			COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
			creationLigneEtTexte(Texte, firstIndex, LastIndex, Niveau, epaisseur, couleur, ptrsc);
		}
		
		// lorsque je suis déconnecté d'IB, pour les backtests :
		if ( sc.ServerConnectionState != SCS_CONNECTED )  
		{	sc.UpdateAlways = 1 ;  }
	}		
	

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsJournaliers(SCStudyInterfaceRef sc)
{
	SCSubgraphRef PHPrior = sc.Subgraph[0];   // appelé par AMR
	SCSubgraphRef PBPrior = sc.Subgraph[1];	   // appelé par AMR
	SCSubgraphRef SettlePrior = sc.Subgraph[2];   // appelé par AMR
	SCSubgraphRef PP = sc.Subgraph[3];   // appelé par AMR
	SCSubgraphRef R1 = sc.Subgraph[4];   // appelé par AMR
	SCSubgraphRef R2 = sc.Subgraph[5];   // appelé par AMR
	SCSubgraphRef R3 = sc.Subgraph[6];   // appelé par AMR
	SCSubgraphRef R4 = sc.Subgraph[7];   // appelé par AMR
	SCSubgraphRef S1 = sc.Subgraph[8];   // appelé par AMR
	SCSubgraphRef S2 = sc.Subgraph[9];   // appelé par AMR
	SCSubgraphRef S3 = sc.Subgraph[10];   // appelé par AMR
	SCSubgraphRef S4 = sc.Subgraph[11];   // appelé par AMR
	SCSubgraphRef mR1 = sc.Subgraph[12];   // appelé par AMR
	SCSubgraphRef mR2 = sc.Subgraph[13];   // appelé par AMR
	SCSubgraphRef mR3 = sc.Subgraph[14];   // appelé par AMR
	SCSubgraphRef mR4 = sc.Subgraph[15];   // appelé par AMR
	SCSubgraphRef mS1 = sc.Subgraph[16];   // appelé par AMR
	SCSubgraphRef mS2 = sc.Subgraph[17];   // appelé par AMR
	SCSubgraphRef mS3 = sc.Subgraph[18];   // appelé par AMR
	SCSubgraphRef mS4 = sc.Subgraph[19];   // appelé par AMR
	const int NUMBER_OF_STUDY_SUBGRAPHS = 20;  
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[0] ;  

	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[2];
	

	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "[3] Pivots Fut J (Full/Mid)";               // appelé par AMR
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.GraphRegion = 2;
		sc.ValueFormat = 2;
		sc.DisplayStudyName = 0;
		sc.DisplayStudyInputValues = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue = 0 ;
		
		sc.HideStudy = 1 ;
		
		ExternalChartNumber.Name = "Daily Chart Number";
		ExternalChartNumber.SetChartNumber(9);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);



		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(176,176,0);
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineWidth = 1;
		}


		PHPrior.PrimaryColor = RGB(255, 0, 0);
		PBPrior.PrimaryColor = RGB(0,168,0);
		SettlePrior.PrimaryColor = RGB(210,0,210);
		
		
		PHPrior.Name = " PH JP";
		PBPrior.Name = " PB JP";
		SettlePrior.Name = "Settle JP";
		PP.Name = "    PP J";
		R1.Name = "    R1 J";
		R2.Name = "    R2 J";
		R3.Name = "    R3 J";
		R4.Name = "    R4 J";
		S1.Name = "    S1 J";
		S2.Name = "    S2 J";
		S3.Name = "    S3 J";
		S4.Name = "     S4 J";
		mR1.Name = " mR1 J";
		mR2.Name = " mR2 J";
		mR3.Name = " mR3 J";
		mR4.Name = " mR4 J";
		mS1.Name = " mS1 J";
		mS2.Name = " mS2 J";
		mS3.Name = " mS3 J";
		mS4.Name = " mS4 J";
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop   // REMPLACER LES index par index
	{
		if (     (  sc.BaseDateTimeIn.TimeAt(index) == 64800    and   sc.BaseDateTimeIn.TimeAt(index-1)!= 64800   )    or index == 0  )   //64800 est le nb de secondes à 18h // on change de séance
		{
			FirstIndex[index] = index ;
			
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du nouveau chandelier sur le daily chart

			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fPP = 0 ;
			float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
			float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
			float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
			float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;
				
			// on récupère HLC
			fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex];
			fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex];
			fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex];
			
			// correction des bugs de flux
			SCDateTimeArray ExternalChartDateTime;
			sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartDateTime);
			int Year, Month, Day;
			ExternalChartDateTime[ExternalChartIndex].GetDateYMD(Year, Month, Day);
		/* 	if (Year==2019 and Month==1 and Day==31)  // jour pour lequel HLC est faux
			{
				//fPHPrior = nd;
				fPBPrior = nd;
				//fSettlePrior = nd;
			} */
						
			//on calcule les PP 
			CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
			
			// les valeurs sont entrées dans les sc.Subgraph[]
			PHPrior[index] = fPHPrior;
			PBPrior[index] = fPBPrior;
			SettlePrior[index] = fSettlePrior;
			PP[index] = fPP;
			R1[index] = fR1;
			R2[index] = fR2;
			R3[index] = fR3;
			R4[index] = fR4;
			S1[index] = fS1;
			S2[index] = fS2;
			S3[index] = fS3;
			S4[index] = fS4;
			mR1[index] = fmR1;
			mR2[index] = fmR2;
			mR3[index] = fmR3;
			mR4[index] = fmR4;
			mS1[index] = fmS1;
			mS2[index] = fmS2;
			mS3[index] = fmS3;
			mS4[index] = fmS4;
		}
	 
		else // on n'est pas dans le premier chandelier du jour
		{
			FirstIndex[index] = FirstIndex[index-1] ;
			PHPrior[index] = PHPrior[index-1] ;
			PBPrior[index] = PBPrior[index-1] ;
			SettlePrior[index] = SettlePrior[index-1] ;
			PP[index] = PP[index-1];
			R1[index] = R1[index-1];
			R2[index] = R2[index-1];
			R3[index] = R3[index-1];
			R4[index] = R4[index-1];
			S1[index] = S1[index-1];
			S2[index] = S2[index-1];
			S3[index] = S3[index-1];
			S4[index] = S4[index-1];
			mR1[index] = mR1[index-1];
			mR2[index] = mR2[index-1];
			mR3[index] = mR3[index-1];
			mR4[index] = mR4[index-1];
			mS1[index] = mS1[index-1];
			mS2[index] = mS2[index-1];
			mS3[index] = mS3[index-1];
			mS4[index] = mS4[index-1];
		}
	
	}
	
	

	
	// gestion automatique de sc.GrapRegion
	if ( sc.UpdateStartIndex == 0 )
	{
		n_ACSIL :: s_BarPeriod BarPeriod ;
		sc.GetBarPeriodParameters(BarPeriod) ;
		
		if ( BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR )     // XTicks
		{
			if ( BarPeriod.IntradayChartBarPeriodParameter1 == 16 )  { sc.GraphRegion = 1 ;  }
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 200 )  { sc.GraphRegion = 0 ;  }
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 2000 )  { sc.GraphRegion = 1 ;  }
		}
	}
	

	if (sc.MenuEventID == 105)   // Bouton "Niv Fut ON/OFF"
	{
		if ( BooleanHideStudy(&sc) )   // on veut cacher la study
		{
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ; 
			TraceVectorielONOFF.SetYesNo(0) ;
		}
		else
		{
			TraceVectorielONOFF.SetYesNo(1) ;
		}
	}
	
	
	// Tracé vectoriel des sc.Subgraph
	if ( TraceVectorielONOFF.GetYesNo() )
	{	
		// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
		
		int const LastIndex = sc.IndexOfLastVisibleBar ;
		s_sc* const ptrsc = &sc;
		int const firstIndex = FirstIndex[LastIndex] ;  // index de la première bougie du mois : début du tracé de la ligne  // FirstIndex est un array qui contient le firstIndex
		
		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			float const Niveau = sc.Subgraph[SubgraphIndex][LastIndex] ;
			SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
			int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
			COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
			creationLigneEtTexte(Texte, firstIndex, LastIndex, Niveau, epaisseur, couleur, ptrsc);
		}
		
		// lorsque je suis déconnecté d'IB, pour les backtests :
		if ( sc.ServerConnectionState != SCS_CONNECTED )  
		{	sc.UpdateAlways = 1 ;  }
	}			

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsQuatreHeures(SCStudyInterfaceRef sc)
{
	SCSubgraphRef PP = sc.Subgraph[0];   // appelé par AMR    // appelé par Navig_Niveaux
	SCSubgraphRef R1 = sc.Subgraph[1];   // appelé par AMR    // appelé par Navig_Niveaux
	SCSubgraphRef R2 = sc.Subgraph[2];   // appelé par AMR    // appelé par Navig_Niveaux
	SCSubgraphRef R3 = sc.Subgraph[3];   // appelé par AMR    // appelé par Navig_Niveaux
	SCSubgraphRef R4 = sc.Subgraph[4];   // appelé par AMR    // appelé par Navig_Niveaux
	SCSubgraphRef S1 = sc.Subgraph[5];   // appelé par AMR    // appelé par Navig_Niveaux
	SCSubgraphRef S2 = sc.Subgraph[6];   // appelé par AMR    // appelé par Navig_Niveaux
	SCSubgraphRef S3 = sc.Subgraph[7];   // appelé par AMR    // appelé par Navig_Niveaux
	SCSubgraphRef S4 = sc.Subgraph[8];   // appelé par AMR    // appelé par Navig_Niveaux
	const int NUMBER_OF_STUDY_SUBGRAPHS = 9;  
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[0] ;  

	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[2];

	
	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "[4] Pivots Fut 4h (Full)";        // appelé par AMR    // appelé par Navig_Niveaux
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.GraphRegion = 1;
		sc.ValueFormat = VALUEFORMAT_INHERITED;
		sc.DisplayStudyName = 0;
		sc.DisplayStudyInputValues = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue = 0 ;
		
		sc.HideStudy = 1 ;

		
		ExternalChartNumber.Name = "240 Min Chart Number";
		ExternalChartNumber.SetChartNumber(8);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);


		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(255,128,0);
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineWidth = 1;
		}
	
		PP.Name = " PP 4h" ;
		R1.Name = " R1 4h" ;
		R2.Name = " R2 4h" ;
		R3.Name = " R3 4h" ;
		R4.Name = " R4 4h" ;
		S1.Name = " S1 4h" ;
		S2.Name = " S2 4h" ;
		S3.Name = " S3 4h" ;
		S4.Name = " S4 4h" ;
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop   // REMPLACER LES index par index
	{
		
		int heure = sc.BaseDateTimeIn[index].GetHour() ;
		
		if (    heure != sc.BaseDateTimeIn[index-1].GetHour()    and ( heure==9 or heure==13 or heure==18 or heure==21 or heure==1 or heure==5  )    )   // on change d'heure 
		{
			FirstIndex[index] = index ;
			
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du nouveau chandelier sur l'external chart

			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fPP = 0 ;
			float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
			float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
			float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
			float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;

			// on récupère HLC
			fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex-1];
			fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex-1];
			fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex-1];

			//on calcule les PP 
			CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
			
			// les valeurs sont entrées dans les sc.Subgraph[]
			PP[index] = fPP;
			R1[index] = fR1;
			R2[index] = fR2;
			R3[index] = fR3;
			R4[index] = fR4;
			S1[index] = fS1;
			S2[index] = fS2;
			S3[index] = fS3;
			S4[index] = fS4;
		}
		
		else // on n'est pas dans le premier chandelier du jour
		{
			FirstIndex[index] = FirstIndex[index-1] ;
			PP[index] = PP[index-1];
			R1[index] = R1[index-1];
			R2[index] = R2[index-1];
			R3[index] = R3[index-1];
			R4[index] = R4[index-1];
			S1[index] = S1[index-1];
			S2[index] = S2[index-1];
			S3[index] = S3[index-1];
			S4[index] = S4[index-1];
		}
	}

	
	// gestion automatique de sc.GrapRegion
	if ( sc.UpdateStartIndex == 0 )
	{
		n_ACSIL :: s_BarPeriod BarPeriod ;
		sc.GetBarPeriodParameters(BarPeriod) ;
		
		if ( BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR )     // XTicks
		{
			if ( BarPeriod.IntradayChartBarPeriodParameter1 == 16 )  { sc.GraphRegion = 1 ;  }
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 200 )  { sc.GraphRegion = 0 ;  }
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 2000 )  { sc.GraphRegion = 1 ;  }
		}
	}
	

	if (sc.MenuEventID == 105)   // Bouton "Niv Fut ON/OFF"
	{
		if ( BooleanHideStudy(&sc) )   // on veut cacher la study
		{
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ; 
			TraceVectorielONOFF.SetYesNo(0) ;
		}
		else
		{
			TraceVectorielONOFF.SetYesNo(1) ;
		}
	}	
	
	
	// Tracé vectoriel des sc.Subgraph
	if ( TraceVectorielONOFF.GetYesNo() )
	{	
		// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
		
		int const LastIndex = sc.IndexOfLastVisibleBar ;
		s_sc* const ptrsc = &sc;
		int const firstIndex = FirstIndex[LastIndex] ;  // index de la première bougie du mois : début du tracé de la ligne  // FirstIndex est un array qui contient le firstIndex
		
		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			float const Niveau = sc.Subgraph[SubgraphIndex][LastIndex] ;
			SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
			int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
			COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
			creationLigneEtTexte(Texte, firstIndex, LastIndex, Niveau, epaisseur, couleur, ptrsc);
		}
		
		// lorsque je suis déconnecté d'IB, pour les backtests :
		if ( sc.ServerConnectionState != SCS_CONNECTED )  
		{	sc.UpdateAlways = 1 ;  }
	}			
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsUneHeure(SCStudyInterfaceRef sc)
{
	SCSubgraphRef PP = sc.Subgraph[0];   // appelé par AMR         // appelé par Navig_Niveaux
	SCSubgraphRef R1 = sc.Subgraph[1];   // appelé par AMR         // appelé par Navig_Niveaux
	SCSubgraphRef R2 = sc.Subgraph[2];   // appelé par AMR         // appelé par Navig_Niveaux
	SCSubgraphRef R3 = sc.Subgraph[3];   // appelé par AMR         // appelé par Navig_Niveaux
	SCSubgraphRef R4 = sc.Subgraph[4];   // appelé par AMR         // appelé par Navig_Niveaux
	SCSubgraphRef S1 = sc.Subgraph[5];   // appelé par AMR         // appelé par Navig_Niveaux
	SCSubgraphRef S2 = sc.Subgraph[6];   // appelé par AMR         // appelé par Navig_Niveaux
	SCSubgraphRef S3 = sc.Subgraph[7];   // appelé par AMR         // appelé par Navig_Niveaux
	SCSubgraphRef S4 = sc.Subgraph[8];   // appelé par AMR         // appelé par Navig_Niveaux
	const int NUMBER_OF_STUDY_SUBGRAPHS = 9;  
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[0] ;  
	
	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[2];          // appelé par Navig_Niveaux
	

	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "[5] Pivots Fut 1h (Full)";                // appelé par AMR       // appelé par Navig_Niveaux
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.GraphRegion = 1;
		sc.ValueFormat = VALUEFORMAT_INHERITED;
		sc.DisplayStudyName = 0;
		sc.DisplayStudyInputValues = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue = 0 ;
		
		sc.HideStudy = 1 ;
		
		ExternalChartNumber.Name = "60 Min Chart Number";
		ExternalChartNumber.SetChartNumber(7);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);

		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(182,108,255);  // RGB(152,49,255);
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineWidth = 1;
		}
	
		PP.Name = "PP 1h";
		R1.Name = "R1 1h";
		R2.Name = "R2 1h";
		R3.Name = "R3 1h";
		R4.Name = "R4 1h";
		S1.Name = "S1 1h";
		S2.Name = "S2 1h";
		S3.Name = "S3 1h";
		S4.Name = "S4 1h";
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
		if (    sc.BaseDateTimeIn[index].GetHour() != sc.BaseDateTimeIn[index-1].GetHour()    )    // on change d'heure
		{
			FirstIndex[index] = index ;
			
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du nouveau chandelier sur l'external chart

			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fPP = 0 ;
			float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
			float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
			float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
			float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;
			
			// on récupère HLC
			fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex-1];
			fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex-1];
			fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex-1];
			
			//on calcule les PP 
			CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
			
			// les valeurs sont entrées dans les sc.Subgraph[]
			PP[index] = fPP;
			R1[index] = fR1;
			R2[index] = fR2;
			R3[index] = fR3;
			R4[index] = fR4;
			S1[index] = fS1;
			S2[index] = fS2;
			S3[index] = fS3;
			S4[index] = fS4;
		}
		
		else // on n'est pas dans le premier chandelier du jour
		{
			FirstIndex[index] = FirstIndex[index-1] ;
			PP[index] = PP[index-1];
			R1[index] = R1[index-1];
			R2[index] = R2[index-1];
			R3[index] = R3[index-1];
			R4[index] = R4[index-1];
			S1[index] = S1[index-1];
			S2[index] = S2[index-1];
			S3[index] = S3[index-1];
			S4[index] = S4[index-1];
		}
		
	}
	
	
	
	// gestion automatique de sc.GrapRegion
	if ( sc.UpdateStartIndex == 0 )
	{
		n_ACSIL :: s_BarPeriod BarPeriod ;
		sc.GetBarPeriodParameters(BarPeriod) ;
		
		if ( BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR )     // XTicks
		{
			if ( BarPeriod.IntradayChartBarPeriodParameter1 == 16 )  { sc.GraphRegion = 1 ;  }
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 200 )  { sc.GraphRegion = 0 ;  }
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 2000 )  { sc.GraphRegion = 1 ;  }
		}
	}	
	
	
	if (sc.MenuEventID == 105)   // Bouton "Niv Fut ON/OFF"
	{
		//sc.UpdateAlways = 1;   // pour que la study soit recalculée immédiatement (sinon il faut attendre qu'une condition d'appel se produise, il n'y a jamais d'appel si le marché est fermé)
		//sc.SetCustomStudyControlBarButtonEnable(sc.MenuEventID,FALSE); // le bouton cliqué ne prend pas la position enfoncée
		
		// sc.HideStudy = BooleanHideStudy(&sc) ;   // désuet pour le tracé vectoriel
		
		if ( BooleanHideStudy(&sc) )   // on veut cacher la study
		{
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ; 
			TraceVectorielONOFF.SetYesNo(0) ;
		}
		else
		{
			TraceVectorielONOFF.SetYesNo(1) ;
		}
		
	}
	//else { sc.UpdateAlways = 0; }
	

	
	// Tracé vectoriel des sc.Subgraph
	if ( TraceVectorielONOFF.GetYesNo() )
	{	
		// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
		
		int const LastIndex = sc.IndexOfLastVisibleBar ;
		s_sc* const ptrsc = &sc;
		int const firstIndex = FirstIndex[LastIndex] ;  // index de la première bougie du mois : début du tracé de la ligne  // FirstIndex est un array qui contient le firstIndex
		
		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			float const Niveau = sc.Subgraph[SubgraphIndex][LastIndex] ;
			SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
			int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
			COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
			creationLigneEtTexte(Texte, firstIndex, LastIndex, Niveau, epaisseur, couleur, ptrsc);
		}
		
		// lorsque je suis déconnecté d'IB, pour les backtests :
		if ( sc.ServerConnectionState != SCS_CONNECTED )  
		{	sc.UpdateAlways = 1 ;  }
	}		
	
	
	
	
}





