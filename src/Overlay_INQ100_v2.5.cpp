/* 
v1.0 (2019/2/22) : trace l'indice NQ100 sur le future.
v1.1 (2019/2/22) : accélération du code : autoloop + n'exécuter sc.GetContainingIndexForDateTimeIndex que s'il y a une nouvelle seconde + n'exécuter sc.GetContainingIndexForDateTimeIndex que durant la séance (9h30-16h10).
v1.2 (2019/2/26) : trace l'indice NQ100 sur le future avec correction du spread.
v1.3 (2019/3/15) : à la study "Overlay INQ100 sur le future (avec correction du spread)" sont rajoutés les SCFloatArrayRef : INQ100SansCorrection et SpreadMoyen.
v1.3.1 (2019/3/28) : pour la study "Overlay INQ100 sur le future (avec correction du spread)", OverlayINQ100 est retourné dès 9h30m14s, tandis que SpreadMoyen est retourné à partir de 9h30m24s.
v1.3.2 (2019/9/28) : pour la study "Overlay INQ100 sur le future (avec correction du spread)", SpreadMoyen est retourné dès 8h55m00s (pour pouvoir tracer les PP projetés à partir de 9h00), tandis que OverlayINQ100 est retourné à partir de 9h30m24s.
v1.3.3 (2019/10/6) : projeter le dernier prix du future (corrigé du spread) sur le graphe indice. 
v1.3.4 (2019/11/15) : INQ100SansCorrection est défini avant 9h30m16s par la formule : FNQ100 - SpreadMoyen.    HoraireDebutTrace_OverlayINQ100 passe de 9h30m24s à 9h30m16s.
v2.0 (2020/1/10) : le SpreadMoyen est calculé sur 6000 ticks (SMA).
v2.1 (2020/1/29) : nettoyage du code. Suppression de tout ce qui a lien avec le calcul d'une MM 300 secondes.
v2.2 (2020/1/29) : le SpreadMoyen est calculé sur 3000 ticks (EMA).    !! Attention la formule utilisee est fausse !!
v2.3 (2020/1/29) : le SpreadMoyen est calculé sur 3000 ticks (EMA). Formule de calcul correcte.
v2.4 (2020/03/05) : création d'un input qui permette de choisir entre une study avec une MMA temporelle (tracé en rose + input de choix de période temporelle) et une MME volumique (tracé en bleu + input de période).
v2.5 (2020/03/06) : création de la study hybride entre SMA X-secondes et EMA X-ticks. Ajout d'un input pour choisir l'horaire de bascule.




Améliorations à apporter :
	- s'assurer que la source est bien en UT 1s : The sc.GetChartName() function, gets the name of the chart specified by the ChartNumber parameter. The name contains the symbol of the chart as well as the timeframe per bar and the chart number. 
	

SOMMAIRE DES STRUCTURES ET FONCTIONS :
Marker_UTAMAOA_UDD
DrawMarker_UTAMAOA_UDD_SurGrapheExterne
scsf_OverlayINQ100PlusSpread
	
	
 */

 
 
#include "sierrachart.h"  // tous les autres headers doivent être placés avant sierrachart.h
SCDLLName("Overlay indice NQ100 UT 1s")


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Marker_UTAMAOA_UDD : s_UseTool    // crée un Marker pour être affiché en coordonnées (indice,prix)
{											
	Marker_UTAMAOA_UDD (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, int chartNumber, int graphRegion, int Line_Number) : s_UseTool()    
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;		
		AddAsUserDrawnDrawing = 1 ;    // This member specifies that a drawing is to be added as a user drawn drawing, which allows the user to interact with the ACSIL added drawing on the chart as if it had been added by the user with one of the drawing tools on the Tools menu.
		LineNumber = Line_Number ;		
		DrawingType = DRAWING_MARKER;
		MarkerType = marqueur ;
		BeginIndex = index ;
		BeginValue = prix ;
		MarkerSize = tailleObjet ;
		Color = couleur ;
		ChartNumber = chartNumber ;
		Region = graphRegion ; 
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawMarker_UTAMAOA_UDD_SurGrapheExterne (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc, int Line_Number, int chartNumber, int graphRegion)      // AddAsUserDrawnDrawing (UDD) est obligatoire pour faire un tracé sur un graphe externe
{
	//Création d'instance de Marker_UTAMAOA_UDD et appel de sc.UseTool
	Marker_UTAMAOA_UDD objetMarker (marqueur, index, prix, tailleObjet, couleur, chartNumber, graphRegion, Line_Number) ;
	ptrsc->UseTool(objetMarker); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_OverlayINQ100PlusSpread(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef OverlayINQ100 = sc.Subgraph[0]; 
	SCFloatArrayRef INQ100SansCorrection = sc.Subgraph[0].Arrays[0] ;  			// appelé par AMR
	SCFloatArrayRef SpreadMoyen_PourAMR = sc.Subgraph[0].Arrays[1] ;  			// appelé par AMR 				// SpreadMoyen_PourAMR = SpreadMoyen   		  // ce SCFloatArrayRef ne sert qu'à être raccord avec la référence appelée par AMR
	SCSubgraphRef Spread = sc.Subgraph[1]; 														// c'est le spread instantanné 
	SCSubgraphRef SpreadMoyen = sc.Subgraph[2]; 											// c'est la moyenne sur PERIODE ticks

	
	SCInputRef TypeDeMM = sc.Input[0];
	SCInputRef ExternalChartNumber = sc.Input[1];
	SCInputRef Input_PeriodeSMA = sc.Input[2];
	SCInputRef Input_PeriodeEMA = sc.Input[3];
	SCInputRef ChartInputsSpreadMoyen_SMA = sc.Input[4]; 
	SCInputRef ChartInputsSpread_PourEMA = sc.Input[5]; 
	SCInputRef Input_DureeEMA_MMHybride = sc.Input[6]; 
	
	
	
	int& HoraireDebutTrace_OverlayINQ100 = sc.GetPersistentInt(1) ;  // time value en secondes
	int& HoraireFinSeance = sc.GetPersistentInt(2) ;
	int& HoraireDebutSpread = sc.GetPersistentInt(3) ; 
	int& HoraireBasculeHybride = sc.GetPersistentInt(4) ; 

	float& alphaEMA = sc.GetPersistentFloat(1);  
	float& betaEMA = sc.GetPersistentFloat(2);  
	
	float PrixFutureProjeteSurGrapheIndice ;
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "Overlay INQ100 sur le future (avec correction du spread)" ; 
		sc.GraphRegion = 1 ;  
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.AutoLoop = 0;  
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 1;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;	
		
		
		//Inputs
		TypeDeMM.Name = "Type De MM";
		TypeDeMM.SetCustomInputStrings("SMA;EMA;MM hybride");
		TypeDeMM.SetCustomInputIndex(2);
		// ExternalChartNumber.Name = "ChartNumber du INQ100 en UT 1s";
		ExternalChartNumber.SetChartNumber(4);
		Input_PeriodeSMA.Name = "Periode de la SMA (secondes)";
		Input_PeriodeSMA.SetInt(300);  // cette valeur est figée à 300 (seule la study du chart #4 est autorisée à modifier cette valeur)
		Input_PeriodeEMA.Name = "Periode de l'EMA (ticks)";
		Input_PeriodeEMA.SetInt(12000);
		Input_DureeEMA_MMHybride.Name = "Duree EMA lorsque MM hybride (secondes, t+9h30)";
		Input_DureeEMA_MMHybride.SetInt(600);  // 600 signifie 10 minutes, donc la bascule se fera à 9h40

		// Subgraphs parameters
		for (int SubgraphIndex = 0; SubgraphIndex <= 2; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			//sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
		}
		
		OverlayINQ100.Name = "Indice+Spread"; 
		OverlayINQ100.LineWidth = 1;
		// OverlayINQ100.PrimaryColor = RGB(255,127,255);		// rose
		// OverlayINQ100.PrimaryColor = RGB(176,176,0);			// beige
		OverlayINQ100.PrimaryColor = RGB(0,128,255);				// bleu
		
		Spread.Name = "Spread"; 
		Spread.LineWidth = 1;
		Spread.PrimaryColor = RGB(0, 200, 0);
		Spread.DrawStyle = DRAWSTYLE_IGNORE;
		Spread.DrawZeros = true ;
		
		SpreadMoyen.Name = "SpreadMoyen"; 
		SpreadMoyen.LineWidth = 1;
		SpreadMoyen.PrimaryColor = RGB(200, 0, 0);
		SpreadMoyen.DrawStyle = DRAWSTYLE_IGNORE;
		SpreadMoyen.DrawZeros = true ;
		
		return ;	
	}
	
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		if ( sc.ChartNumber == 6 )  sc.GraphRegion = 1;  
		OverlayINQ100[0] = 0 ;
		INQ100SansCorrection[0] = 0 ;
		HoraireDebutSpread = HMS_TIME(8,55,0);  // cette valeur doit être inférieure à 9h00 qui est l'horaire de début de calcul des PP projetés
		HoraireDebutTrace_OverlayINQ100 = HMS_TIME(9,30,15);
		HoraireBasculeHybride = HMS_TIME(9,40,0);
		HoraireFinSeance = HMS_TIME(16,0,0);
		Input_PeriodeSMA.SetInt(300);  // cette valeur est figée à 300 (seule la study du chart #4 est autorisée à modifier cette valeur)
		
		// Récupération du X dans le X-Ticks
		int xTicks=1 ;
		n_ACSIL::s_BarPeriod BarPeriod;
		sc.GetBarPeriodParameters(BarPeriod);
		if ( BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR )   // on vérifie que le graphe est bien en XTicks
		{	xTicks = BarPeriod.IntradayChartBarPeriodParameter1; 	}  
		else // message d'erreur dans le log
		{  SCString message ;   message.Format("!! ERREUR !! Le chart n'est pas en ticks, impossible de tracer l'indicateur ''Overlay INQ100 sur le future (avec correction du spread)''") ;    sc.AddMessageToLog(message,1) ; }

		// Récupération de la studyID du Spread sur le graphe de l'indice
		int StudyID = sc.GetStudyIDByName(ExternalChartNumber.GetChartNumber(), "Spread Future-Indice pour tracer sous l'indice", 0) ;
		if  (StudyID == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart source #%d ne contient pas le spread Future-Indice", ExternalChartNumber.GetChartNumber()) ; sc.AddMessageToLog(message,1) ; }
		
		// Création des ChartInputsSpreadMoyen_SMA
		if  (StudyID != 0 )  { ChartInputsSpreadMoyen_SMA.SetChartStudySubgraphValues(ExternalChartNumber.GetChartNumber(), StudyID, 5);  }
		
		// Création des ChartInputsSpread_PourEMA
		if  (StudyID != 0 )  { ChartInputsSpread_PourEMA.SetChartStudySubgraphValues(ExternalChartNumber.GetChartNumber(), StudyID, 1);  }
		
		// Calcul de alpha et beta de l'EMA
		float periode = float( Input_PeriodeEMA.GetInt() ) / xTicks ;    
		alphaEMA = 2 / (periode+1) ;
		betaEMA = 1 - alphaEMA ;

		// paramètres du tracé
		if ( TypeDeMM.GetIndex() == 0 )  // SMA
		{
			OverlayINQ100.PrimaryColor = RGB(255,127,255);			// rose
		}
		else if ( TypeDeMM.GetIndex() == 1 )  // EMA
		{
			OverlayINQ100.PrimaryColor = RGB(0,128,255);				// bleu
		}
		else if ( TypeDeMM.GetIndex() == 2 )  // hybride
		{
			OverlayINQ100.PrimaryColor = RGB(197,138,244);				// violet
		}
	}
	
	
	// on récupère le cours du INQ100 dans ExternalChartBaseData
	SCGraphData ExternalChartBaseData;
	sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartBaseData);
	
	// on récupère l'array de SpreadMoyenExternalArray   (c'est le SpreadMoyen non arrondi : utilisé par SMA)
	SCFloatArray SpreadMoyenExternalArray;
	sc.GetStudyArrayFromChartUsingID(ChartInputsSpreadMoyen_SMA.GetChartStudySubgraphValues(), SpreadMoyenExternalArray);   //   void GetStudyArrayFromChartUsingID(const s_ChartStudySubgraphValues& ChartStudySubgraphValues, SCFloatArrayRef SubgraphArray);

	// on récupère l'array de SpreadExternalArray   (c'est le Spread instantanné : utilisé par EMA)
	SCFloatArray SpreadExternalArray;
	sc.GetStudyArrayFromChartUsingID(ChartInputsSpread_PourEMA.GetChartStudySubgraphValues(), SpreadExternalArray);   //   void GetStudyArrayFromChartUsingID(const s_ChartStudySubgraphValues& ChartStudySubgraphValues, SCFloatArrayRef SubgraphArray);



	if ( TypeDeMM.GetIndex() == 0 )  // SMA
	{
		for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)   // début autoloop          
		{
			int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
			int PreviousOpenTimeValue = sc.BaseDateTimeIn[index-1].GetTimeInSecondsWithoutMilliseconds()  ; 
			
			
			if ( OpenTimeValue >  HoraireDebutSpread  and  OpenTimeValue < HoraireFinSeance )  // entre 8h55 et 16h00
			{
				
				if  ( OpenTimeValue  !=  PreviousOpenTimeValue )   // l'OverlayINQ100 n'est actualisé que s'il y a une nouvelle seconde 
				{ 
					// on récupère l'index sur le chart INQ100 en UT 1s
					int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); 
					
					if ( OpenTimeValue >= HoraireDebutTrace_OverlayINQ100 )
					{	
						INQ100SansCorrection[index] = ExternalChartBaseData[SC_OPEN][ExternalChartIndex] ;	
						SpreadMoyen[index] = SpreadMoyenExternalArray[ExternalChartIndex] ;	
						OverlayINQ100[index] = INQ100SansCorrection[index] + SpreadMoyen[index] ;  
					}
					else // entre 9h00m00s et 9h30m15s : INQ100SansCorrection est défini par la formule : FNQ100 - SpreadMoyen.
					{	
						SpreadMoyen[index] = SpreadMoyenExternalArray[ExternalChartIndex-120] ;	
						INQ100SansCorrection[index] = sc.BaseDataIn[SC_LAST][index] - SpreadMoyen[index] ;
						OverlayINQ100[index] = 0 ;  
					}
				
				}
				else   // pas de changement de seconde
				{
					INQ100SansCorrection[index] = INQ100SansCorrection[index-1] ;
					SpreadMoyen[index] = SpreadMoyen[index-1] ;   
					OverlayINQ100[index] = OverlayINQ100[index-1] ;
				} 
				
				// PrixFutureProjeteSurGrapheIndice est recalculé à chaque appel du DLL durant la séance
				if ( index == sc.ArraySize-1 )  { PrixFutureProjeteSurGrapheIndice = sc.Close[index] - SpreadMoyen[index] ;  }
			}		
			
			else    // avant 8h55 ou après 16h00
			{	
				INQ100SansCorrection[index] = 0 ;
				SpreadMoyen[index] = 0 ;  
				OverlayINQ100[index] = 0 ;  
				PrixFutureProjeteSurGrapheIndice = 0 ;
			} 
			
			SpreadMoyen_PourAMR[index] = SpreadMoyen[index] ;
			
		} // fin autoloop
	}
	else if ( TypeDeMM.GetIndex() == 1 )  // EMA
	{
		for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)   // début autoloop          
		{
			int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
			int PreviousOpenTimeValue = sc.BaseDateTimeIn[index-1].GetTimeInSecondsWithoutMilliseconds()  ; 
			
			
			if ( OpenTimeValue >  HoraireDebutSpread and OpenTimeValue < HoraireFinSeance )  // entre 8h55 et 16h00
			{
				
				if  ( OpenTimeValue  !=  PreviousOpenTimeValue )   // l'OverlayINQ100 n'est actualisé que s'il y a une nouvelle seconde 
				{ 
					// on récupère l'index sur le chart INQ100 en UT 1s
					int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); 
					
					if ( OpenTimeValue >= HoraireDebutTrace_OverlayINQ100 )
					{	
						INQ100SansCorrection[index] = ExternalChartBaseData[SC_OPEN][ExternalChartIndex] ;	
						Spread[index] = SpreadExternalArray[ExternalChartIndex] ;	
						SpreadMoyen[index] = betaEMA*SpreadMoyen[index-1] + alphaEMA*Spread[index] ;
						OverlayINQ100[index] = INQ100SansCorrection[index] + SpreadMoyen[index] ;  
					}
					else // entre 9h00m00s et 9h30m15s : INQ100SansCorrection est défini par la formule : FNQ100 - SpreadMoyen
					{	
						SpreadMoyen[index] = SpreadMoyen[index-1] ;   
						Spread[index] = SpreadMoyen[index] ;
						INQ100SansCorrection[index] = sc.BaseDataIn[SC_LAST][index] - SpreadMoyen[index] ;
						OverlayINQ100[index] = 0 ;  
					}
				
				}
				else   // pas de changement de seconde
				{
					Spread[index] = Spread[index-1] ;   
					SpreadMoyen[index] = betaEMA*SpreadMoyen[index-1] + alphaEMA*Spread[index] ;     // on considère Spread constant pour les ticks à l'intérieur de la seconde.
					OverlayINQ100[index] = OverlayINQ100[index-1] ;
					INQ100SansCorrection[index] = INQ100SansCorrection[index-1] ;
				} 
				
				// PrixFutureProjeteSurGrapheIndice est recalculé à chaque appel du DLL durant la séance
				if ( index == sc.ArraySize-1 )  { PrixFutureProjeteSurGrapheIndice = sc.Close[index] - SpreadMoyen[index] ;  }
			}		
			
			else    // avant 8h55 ou après 16h00
			{	
				Spread[index] = Spread[index-1] ;
				SpreadMoyen[index] = SpreadMoyen[index-1] ;
				INQ100SansCorrection[index] = 0 ;
				OverlayINQ100[index] = 0 ;  
				PrixFutureProjeteSurGrapheIndice = 0 ;
			} 
			
			
			SpreadMoyen_PourAMR[index] = SpreadMoyen[index] ;
			
		} // fin autoloop
	}
	else if ( TypeDeMM.GetIndex() == 2 )  // Hybride
	{
		for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)   // début autoloop          
		{
			int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
			int PreviousOpenTimeValue = sc.BaseDateTimeIn[index-1].GetTimeInSecondsWithoutMilliseconds()  ; 
			
			
			if ( OpenTimeValue >  HoraireDebutSpread  and  OpenTimeValue < HoraireFinSeance )  // entre 8h55 et 16h00
			{
				
				if  ( OpenTimeValue  !=  PreviousOpenTimeValue )   // l'OverlayINQ100 n'est actualisé que s'il y a une nouvelle seconde 
				{ 
					// on récupère l'index sur le chart INQ100 en UT 1s
					int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); 
					
					if ( OpenTimeValue >= HoraireDebutTrace_OverlayINQ100  and  OpenTimeValue < HoraireBasculeHybride )  // période où la MM hybride est l'EMA
					{	
						INQ100SansCorrection[index] = ExternalChartBaseData[SC_OPEN][ExternalChartIndex] ;	
						Spread[index] = SpreadExternalArray[ExternalChartIndex] ;	
						SpreadMoyen[index] = betaEMA*SpreadMoyen[index-1] + alphaEMA*Spread[index] ;
						OverlayINQ100[index] = INQ100SansCorrection[index] + SpreadMoyen[index] ;  
					}
					else if ( OpenTimeValue >= HoraireBasculeHybride )  // période où la MM hybride est la SMA
					{
						INQ100SansCorrection[index] = ExternalChartBaseData[SC_OPEN][ExternalChartIndex] ;	
						SpreadMoyen[index] = SpreadMoyenExternalArray[ExternalChartIndex] ;	
						OverlayINQ100[index] = INQ100SansCorrection[index] + SpreadMoyen[index] ;  
						Spread[index] = SpreadMoyen[index] ;   // il est nécessaire d'avoir un spread reporté pour un calcul correct de Spread via l'EMA à l'open
					}
					else // entre 9h00m00s et 9h30m15s : INQ100SansCorrection est défini par la formule : FNQ100 - SpreadMoyen
					{	
						SpreadMoyen[index] = SpreadMoyen[index-1] ;   
						Spread[index] = SpreadMoyen[index] ;
						INQ100SansCorrection[index] = sc.BaseDataIn[SC_LAST][index] - SpreadMoyen[index] ;
						OverlayINQ100[index] = 0 ;  
					}
				
				}
				else   // pas de changement de seconde
				{
					OverlayINQ100[index] = OverlayINQ100[index-1] ;
					INQ100SansCorrection[index] = INQ100SansCorrection[index-1] ;
					Spread[index] = Spread[index-1] ;   
					if ( /* OpenTimeValue >= HoraireDebutTrace_OverlayINQ100  and  */ OpenTimeValue < HoraireBasculeHybride )  // période où la MM hybride est l'EMA
					{	
						SpreadMoyen[index] = betaEMA*SpreadMoyen[index-1] + alphaEMA*Spread[index] ;     // on considère Spread constant pour les ticks à l'intérieur de la seconde.
					}
					else if ( OpenTimeValue >= HoraireBasculeHybride )  // période où la MM hybride est la SMA
					{
						SpreadMoyen[index] = SpreadMoyen[index-1] ;   
					}
				} 
				
				// PrixFutureProjeteSurGrapheIndice est recalculé à chaque appel du DLL durant la séance
				if ( index == sc.ArraySize-1 )  { PrixFutureProjeteSurGrapheIndice = sc.Close[index] - SpreadMoyen[index] ;  }
			}		
			
			else    // avant 8h55 ou après 16h00
			{	
				Spread[index] = Spread[index-1] ;
				SpreadMoyen[index] = SpreadMoyen[index-1] ;
				INQ100SansCorrection[index] = 0 ;
				OverlayINQ100[index] = 0 ;  
				PrixFutureProjeteSurGrapheIndice = 0 ;
			} 
			
			
			SpreadMoyen_PourAMR[index] = SpreadMoyen[index] ;
			
		} // fin autoloop
	}
	
	
	
	// Affichage sur les graphes indice de PrixFutureProjeteSurGrapheIndice
	if ( sc.ChartNumber == 6 )
	{
		int ExternalChartIndex_UT20s = sc.GetContainingIndexForDateTimeIndex(15, sc.ArraySize-1) ;    // graphe #15
		int ExternalChartIndex_UT2min = sc.GetContainingIndexForDateTimeIndex(19, sc.ArraySize-1) ;    // graphe #19
		s_sc* const ptrsc = &sc;   
		int const ThisStudyID = sc.StudyGraphInstanceID ;  
		sc.DeleteUserDrawnACSDrawing(15, 10001+10000*ThisStudyID) ;
		sc.DeleteUserDrawnACSDrawing(19, 10001+10000*ThisStudyID) ;
		DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_DIAMOND, ExternalChartIndex_UT20s+2, PrixFutureProjeteSurGrapheIndice, 4, RGB(0,0,0), ptrsc, 10001+10000*ThisStudyID, 15, 0) ;    // void DrawMarker_UTAMAOA_UDD_SurGrapheExterne (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc, int Line_Number, int chartNumber, int graphRegion)   
		DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_DIAMOND, ExternalChartIndex_UT2min+2, PrixFutureProjeteSurGrapheIndice, 4, RGB(0,0,0), ptrsc, 10001+10000*ThisStudyID, 19, 0) ; 
	}
	
}
