/* 
v1.0 (2019/3/1) : retourne les PlusHaut/PlusBas du jour en cours pour l'indice.
v1.1 (2019/3/1) : ajout des DeadZones haute et basse. Transparence des DZ.
v1.2 (2019/3/2) : retourne également les PlusHaut/PlusBas du jour en cours pour le future (RAZ à 18h et 9h30).
v1.2.1 (2019/3/29) : la taille de la DeadZone devient un input. Valeurs usuelles : Nasdaq : 16 ticks = 1.0, 200 ticks = 2.0, 2000 ticks = 4.0, indice 1s = 1.0, indice 20s = 2.0, indice 2min = 4.0. SP500 : 200 ticks = 0.5, indice 1s = 0.25, indice 20s = 0.5, indice 2 min = 1.0.
v1.3.0 (2019/4/18) : ajout d'un indicateur Distance (pour l'indice SP500) qui retourne la distance au plus proche niveau PH ou PB, et projection sur le future NQ100.
v1.3.1 (2019/4/18) : valeur de DZ gérée automatiquement pour les différents indices, futures et UT.
v1.4.0 (2019/4/24) : ajout des indicateur PH/PB dynamiques pour la semaine en cours.
v1.4.1 (2019/4/25) : ajout des indicateur PH/PB dynamiques pour le mois en cours.
v1.4.2 (2019/4/26) : modifs mineures.
v1.4.3 (2019/6/11) : modifs mineures de mise en forme.
v1.4.4 (2019/7/4) : le tracé de "PH/PB PreOpen" est togglé ON/OFF via un bouton. Le statut est géré par la study "Prix typique". La fonction scsf_PHPBDynamiquesForFuture consulte cette study.
v1.4.5 (2019/7/8) : code compatible avec la v1941 de SC.
v1.4.6 (2019/7/8) : les noms des graphes sont raccourcis : mensuel devient M, hebdo devient H, jour devient J, seance devient se, PreOpen devient PO.
v1.4.7 (2019/7/8) : les tracés de PH/PB J sur l'indice sont stoppés à 16h00 (car on veut mieux différencier PH J et PH JP (calculé à 16h02)).
v1.5.0 (2019/10/29) : accélération de l'affichage sur les PH/PB dynamiques de l'indice : utiliser des rectangles horizontaux transparents.
v1.5.1 (2019/10/30) : accélération de l'affichage sur les PH/PB dynamiques de l'indice : pas de recalcul des rectangles tant que FirstVisibleBar et LastVisibleBar ne changent pas.
			Note : le temps de calcul indiqué pour cette study par SC correspond à >90% au temps de calcul des rectangles, et seulement à <10% au temps de calcul des SCSubgraphRef.
v1.5.2 (2019/10/30) : suppression des SCSubgraphRef de la DZ.
v1.6.0 (2019/10/30) : accélération de l'affichage sur les PH/PB dynamiques du future.
v1.6.1 (2020/02/03) : ajout d'un input pour tracer la Distance à PH/PB en ticks ou en points.
v1.6.1 (2020/02/13) : ajout d'un input pour inverser les couleurs rouge/vert dans scsf_PHPBDynamiquesForFuture.
v1.7.0 (2020/05/09) : création de la fonction "[G] PH PB dynamiques de l'indice projetes sur le future".
v1.7.1 (2020/05/11) : on récupère PHJ/PBJ de l'Index NQ100 sur le chart en UT 1s.
v1.7.2 (2020/05/11) : ajout d'un marker sur les niveaux PHJ/PBJ qui correspond à ExternalIndex-3.
v1.7.3 (2020/05/13) : dans les fonctions scsf_DistanceAPHPBForIndex et scsf_OverlayDistanceAPHPBForSP500, on ajoute les SCFloatArrayRef DistanceHaut et DistanceBas (récupérés uniquement pour l'indice SP500, pas le future).
v1.7.4 (2020/06/29) : accélération de SC par ajout de la condition : " if ( sc.ChartWindowIsActive  or  NbMonitors == 2 ) ". Et on ne trace que les niveaux visibles sur l'écran.
v1.7.5 (2020/06/29) : suppression de la gestion de DZ via un input. Dorénavant DZ = RangeVertical/66.



Améliorations à apporter :
	- accélérer le tracé des rectangles en mettant moins de rectangles lorsque le graphique est ultra dense (ie espacement (sc.ChartBarSpacing) des bougies = 0  => tracer un rectangle sur 4 ; espacement des bougies = 1  => tracer un rectangle sur 2)
	
	
SOMMAIRE DES FONCTIONS :
DrawHLine
BooleanHideStudy
scsf_PHPBDynamiquesForIndex
scsf_PHPBDynamiquesForFuture
scsf_DistanceAPHPBForIndex
scsf_DistanceAPHPBForFuture
scsf_OverlayDistanceAPHPBForSP500
 */

#include "sierrachart.h"

SCDLLName("PH PB dynamiques")


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float TopCoordinate ;		// la déclaration de ces deux variables ici permet d'éviter de les repasser en paramètre de toutes les fonctions qui les utilisent
float BottomCoordinate ;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct LigneHorizontale : s_UseTool  // LigneHorizontale crée un objet qui contient mes paramètres choisis pour une ligne horizontale  (hérite de toutes les données public et protected de s_UseTool)
{
	LigneHorizontale(float niveau, int epaisseur, COLORREF couleur, int graphRegion) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
	Clear(); 
	DrawingType = DRAWING_HORIZONTALLINE;
	LineStyle = LINESTYLE_SOLID;
	AddMethod = UTAM_ADD_ALWAYS;	
	BeginValue = niveau;
	LineWidth = epaisseur;
	Color = couleur; 
	Region = graphRegion ;   // 0 par défaut
	} 
};
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawHLine (float niveau, int epaisseur, COLORREF couleur, s_sc* ptrsc, int graphRegion)
{
	//Création d'instance de LigneHorizontale et appel de sc.UseTool
	LigneHorizontale objetLigneHorizontale(niveau, epaisseur, couleur, graphRegion);
	ptrsc->UseTool(objetLigneHorizontale); // c'est sc.UseTool qui est appelé
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int BooleanHideStudy(s_sc* ptrsc)  // détermine le statut d'affichage des niveaux future
{
	int ShowStudy = 1 ;
	int StudyIDPrixTypique = ptrsc->GetStudyIDByName(ptrsc->ChartNumber, "Prix typique", 0);
	if  (StudyIDPrixTypique == 0 )  
	{	SCString message ;  message.Format("!! ERREUR !! Le chart #%d ne contient pas la study ''Prix typique''. Necessaire pour determiner sc.HideStudy.", ptrsc->ChartNumber) ; ptrsc->AddMessageToLog(message,1) ; }
	else
	{	ptrsc->GetChartStudyInputInt(ptrsc->ChartNumber, StudyIDPrixTypique, 3, ShowStudy) ;  }    // int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
	return (1 - ShowStudy) ;    // HideStudy = 1 - ShowStudy
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Rectangle2 : s_UseTool             // Attention à ne pas nommer cette structure Rectangle => erreur de compilation, le terme est probablement réservé pour un autre usage
{
	Rectangle2(int beginIndex, int endIndex, float niveau1, float niveau2, COLORREF maCouleur, s_sc* ptrsc) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		DrawingType = DRAWING_RECTANGLEHIGHLIGHT;
		AddMethod = UTAM_ADD_ALWAYS;	
		BeginIndex = beginIndex ;
		EndIndex = endIndex ;
		BeginValue = niveau1 ;
		EndValue = niveau2 ;
		LineWidth = 0;    // For the Rectangle Drawing types, in order to see the outline it is necessary to set the LineWidth to 1 or greater. Otherwise, the outline will not be visible.
		SecondaryColor = maCouleur;    // Le membre "Color" correspond au contour du rectangle   // SecondaryColor corespond au fill
		TransparencyLevel = 90;
		Region = ptrsc->GraphRegion ; 
	} 
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void creationRectangle (int beginIndex, int endIndex, float niveau1, float niveau2, COLORREF maCouleur, s_sc* ptrsc)			// niveau1 doit être le prix de PHJ / PBJ, et niveau2 doit être le prix de PHJ+DZ / PBJ-DZ
{
	if ( niveau1 > BottomCoordinate  and  niveau1 < TopCoordinate )
	{
		//Création d'instance de Rectangle2 et appel de sc.UseTool
		Rectangle2 objet(beginIndex, endIndex, niveau1, niveau2, maCouleur, ptrsc);
		ptrsc->UseTool(objet); 
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int NumberOfMonitors()		// Number of display monitors on desktop
{
	return GetSystemMetrics(80) ;		// https://docs.microsoft.com/fr-fr/windows/win32/api/winuser/nf-winuser-getsystemmetrics
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_PHPBDynamiquesForIndex(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef PlusHautMensuel = sc.Subgraph[1]; 
	SCSubgraphRef PlusBasMensuel  = sc.Subgraph[2]; 
	SCSubgraphRef PlusHautHebdo = sc.Subgraph[3]; 
	SCSubgraphRef PlusBasHebdo  = sc.Subgraph[4]; 
	SCSubgraphRef PlusHautJour = sc.Subgraph[6];        	// appelé par AMR		// appelé par scsf_PHPBDynamiquesForIndex_Projetes
	SCSubgraphRef PlusBasJour  = sc.Subgraph[7];         	// appelé par AMR		// appelé par scsf_PHPBDynamiquesForIndex_Projetes

	
	SCInputRef TraceVectorielONOFF = sc.Input[0];
	

	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	int& TopCoordinatePrevious = sc.GetPersistentIntFast(3);  


	
	if (sc.SetDefaults)
	{
		sc.GraphName = "PH PB dynamiques de l'indice" ;       // appelé par AMR		// appelé par scsf_PHPBDynamiquesForIndex_Projetes
		sc.GraphRegion = 0;  // 0 = graphe principal
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.AutoLoop = 0;  
		sc.TransparencyLevel = 90 ;  // 90%
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		
		sc.HideStudy = 0 ;  // on veut tracer les PH/PB H et M

		
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		

		
		// paramètres des tracés
		for (int SubgraphIndex = 1; SubgraphIndex <= 4; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineWidth = 1;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;  
		}
		for (int SubgraphIndex = 6; SubgraphIndex <= 7; SubgraphIndex++)
		{
			//sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			//sc.Subgraph[SubgraphIndex].LineWidth = 2;
			//sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;  
		}
	
		
		PlusHautJour.Name = "PH J"; 
		PlusHautJour.PrimaryColor = RGB(250,0,0);
		PlusHautJour.DrawStyle = DRAWSTYLE_HIDDEN ;
		PlusHautJour.LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  |  LL_VALUE_ALIGN_VALUES_SCALE | LL_NAME_ALIGN_ABOVE ;
		PlusBasJour.Name = "PB J"; 
		PlusBasJour.PrimaryColor = RGB(0,200,0);
		PlusBasJour.DrawStyle = DRAWSTYLE_HIDDEN ;
		PlusBasJour.LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  |  LL_VALUE_ALIGN_VALUES_SCALE | LL_NAME_ALIGN_BELOW ;
		PlusHautHebdo.Name = "PH H"; 
		PlusHautHebdo.PrimaryColor = RGB(250,0,0);
		PlusHautHebdo.LineLabel = LL_DISPLAY_NAME | /* LL_DISPLAY_VALUE | */ LL_NAME_ALIGN_FAR_RIGHT  |  LL_VALUE_ALIGN_VALUES_SCALE | LL_NAME_ALIGN_ABOVE ;
		PlusBasHebdo.Name = "PB H"; 
		PlusBasHebdo.PrimaryColor = RGB(0,200,0);
		PlusBasHebdo.LineLabel = LL_DISPLAY_NAME | /* LL_DISPLAY_VALUE | */ LL_NAME_ALIGN_FAR_RIGHT  |  LL_VALUE_ALIGN_VALUES_SCALE | LL_NAME_ALIGN_BELOW ;
		PlusHautMensuel.Name = "PH M /         "; 
		PlusHautMensuel.PrimaryColor = RGB(250,0,0);
		PlusHautMensuel.LineLabel = LL_DISPLAY_NAME | /* LL_DISPLAY_VALUE | */ LL_NAME_ALIGN_FAR_RIGHT  |  LL_VALUE_ALIGN_VALUES_SCALE | LL_NAME_ALIGN_ABOVE ;
		PlusBasMensuel.Name = "PB M /         "; 
		PlusBasMensuel.PrimaryColor = RGB(0,200,0);
		PlusBasMensuel.LineLabel = LL_DISPLAY_NAME | /* LL_DISPLAY_VALUE |  */LL_NAME_ALIGN_FAR_RIGHT  |   LL_VALUE_ALIGN_VALUES_SCALE | LL_NAME_ALIGN_BELOW ;
		

		return ;	
	}  // fin de Set.Defaults
	
	
	if ( sc.UpdateStartIndex == 0 )
	{ 	
		sc.GraphRegion = 0 ;  
		NbMonitors = NumberOfMonitors() ;
		
		PlusHautJour[0] = sc.High[0] ;
		PlusBasJour[0] = sc.Low[0] ;
		PlusHautHebdo[0] = sc.High[0] ;
		PlusBasHebdo[0] = sc.Low[0] ;
		PlusHautMensuel[0] = sc.High[0] ;
		PlusBasMensuel[0] = sc.Low[0] ;
		
		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;
		
		
		// gestion automatique des affichages
		SCString ChartSymbol = sc.GetChartSymbol(sc.ChartNumber) ;
		SCString IUXX = "$IUXX" ;
		SCString SPX = "$SPX" ;
		n_ACSIL :: s_BarPeriod BarPeriod ;
		sc.GetBarPeriodParameters(BarPeriod) ;
		
		if ( ChartSymbol == IUXX and BarPeriod.IntradayChartBarPeriodType == IBPT_DAYS_MINS_SECS  )      // cas du INQ100
		{
			if ( BarPeriod.IntradayChartBarPeriodParameter1 == 1 )  { PlusHautHebdo.Name = "" ;  PlusBasHebdo.Name = "";  PlusHautMensuel.Name = "" ;  PlusBasMensuel.Name = "";  sc.ScaleRangeType = SCALE_AUTO; }
			//else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 15 or BarPeriod.IntradayChartBarPeriodParameter1 == 20 )  { PlusHautMensuel.Name = "" ;  PlusBasMensuel.Name = "";  }
		}
		
		if ( ChartSymbol == SPX and BarPeriod.IntradayChartBarPeriodType == IBPT_DAYS_MINS_SECS  )      // cas du ISP500
		{
			/* if ( BarPeriod.IntradayChartBarPeriodParameter1 == 1 )  */ { PlusHautHebdo.Name = "" ;  PlusBasHebdo.Name = "";  PlusHautMensuel.Name = "" ;  PlusBasMensuel.Name = "";  sc.ScaleRangeType = SCALE_AUTO;  }
			/* else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 15 or BarPeriod.IntradayChartBarPeriodParameter1 == 20 )  { PlusHautMensuel.Name = "" ;  PlusBasMensuel.Name = "";  } */
		}
		
	}
	
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // LOOPING
	{
		
		int CurrentTimeValue = sc.BaseDateTimeIn.TimeAt(index) ;     // TimeAt is a Time Value ( number of seconds since midnight )
		
		if  (   CurrentTimeValue  < sc.BaseDateTimeIn.TimeAt(index-1)  )    // changement de jour              
		{ 
			PlusHautJour[index] = sc.High[index] ;  PlusBasJour[index] = sc.Low[index] ;    
			
			if (   sc.BaseDateTimeIn[index].GetDayOfWeek() <  sc.BaseDateTimeIn[index-1].GetDayOfWeek()   )     // changement de semaine
			{ PlusHautHebdo[index] = sc.High[index] ;  PlusBasHebdo[index] = sc.Low[index] ;  }  
			else
			{ PlusHautHebdo[index] = max(PlusHautHebdo[index-1],sc.High[index]) ;  PlusBasHebdo[index] = min(PlusBasHebdo[index-1],sc.Low[index]) ;  } // premier chandelier du jour sans changement de semaine
		
			 if (  sc.BaseDateTimeIn[index].GetMonth()  !=   sc.BaseDateTimeIn[index-1].GetMonth()  )     // changement de mois
			{ PlusHautMensuel[index] = sc.High[index] ;  PlusBasMensuel[index] = sc.Low[index] ;  }  
			else
			{ PlusHautMensuel[index] = max(PlusHautMensuel[index-1],sc.High[index]) ;  PlusBasMensuel[index] = min(PlusBasMensuel[index-1],sc.Low[index]) ;  } // premier chandelier du jour sans changement de mois
		}
		
		else if ( CurrentTimeValue > 57720 )     // il est après 16h02   // 16*3600+2*60=57720
		{
			PlusHautJour[index] = 0 ; 
			PlusBasJour[index] = 0 ;    
			PlusHautHebdo[index] = PlusHautHebdo[index-1]  ;
			PlusBasHebdo[index] = PlusBasHebdo[index-1] ;  
			PlusHautMensuel[index] = PlusHautMensuel[index-1]  ; 
			PlusBasMensuel[index] = PlusBasMensuel[index-1] ;  
		}
		
		else // tous les chandeliers après le chandelier d'ouverture du jour et avant 16h00
		{
			if ( sc.High[index] > PlusHautJour[index-1] ) 	{ PlusHautJour[index] = sc.High[index] ; }	 	else	{ PlusHautJour[index] = PlusHautJour[index-1]  ; }
			if ( sc.Low[index] < PlusBasJour[index-1] ) 	{ PlusBasJour[index] = sc.Low[index] ; }		else	{ PlusBasJour[index] = PlusBasJour[index-1] ;  }
			if ( sc.High[index] > PlusHautHebdo[index-1] ) 	{ PlusHautHebdo[index] = sc.High[index] ; }	 	else	{ PlusHautHebdo[index] = PlusHautHebdo[index-1]  ; }
			if ( sc.Low[index] < PlusBasHebdo[index-1] ) 	{ PlusBasHebdo[index] = sc.Low[index] ; }		else	{ PlusBasHebdo[index] = PlusBasHebdo[index-1] ;  }
			if ( sc.High[index] > PlusHautMensuel[index-1] ) 	{ PlusHautMensuel[index] = sc.High[index] ; }	 	else	{ PlusHautMensuel[index] = PlusHautMensuel[index-1]  ; }
			if ( sc.Low[index] < PlusBasMensuel[index-1] ) 	{ PlusBasMensuel[index] = sc.Low[index] ; }		else	{ PlusBasMensuel[index] = PlusBasMensuel[index-1] ;  }
		}

	}
	
	
	// Bloc d'affichage       
	if ( TraceVectorielONOFF.GetYesNo() )
	{
		if ( sc.ChartWindowIsActive  or  NbMonitors == 2 )
		{
			TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;
			if (  FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(TopCoordinatePrevious-TopCoordinate) > 1  )          	// Le recalcul des rectangles n'est réalisé que si FirstIndex ou LastVisibleBar ou TopCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
			{	
				FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
				LastVisibleBar = sc.IndexOfLastVisibleBar ;
				BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;
				TopCoordinatePrevious = TopCoordinate ;	
	
				// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
				sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		

				float RangeVertical = TopCoordinate - BottomCoordinate ; 		// c'est la valeur en points entre le top et le bottom du graphique
				const float DZ = RangeVertical / 66 ;
				s_sc* const ptrsc = &sc;
				COLORREF const CouleurRouge = RGB(250,0,0) ;
				COLORREF const CouleurVerte = RGB(0,200,0) ;
				
				// Zone transparente rouge au-dessus de PHJ
				{
					int beginIndex = FirstVisibleBar ;
					float PHJ_IndexMoinsUn = PlusHautJour[FirstVisibleBar] ;
					float PHJ_Index ;
					for ( int index = FirstVisibleBar+1 ; index < LastVisibleBar+1 ; index++ )
					{
						PHJ_Index = PlusHautJour[index] ; 
						// on teste si on fait un nouveau plus haut. Si oui, on trace un rectangle entre beginIndex et index-1. Etc jusqu'au prochain nouveau plus haut.
						if ( PHJ_Index != PHJ_IndexMoinsUn)
						{
							
							if ( PHJ_Index != 0 and PHJ_IndexMoinsUn != 0 )
							{
								creationRectangle (beginIndex, index, PHJ_IndexMoinsUn, PHJ_IndexMoinsUn+DZ, CouleurRouge, ptrsc)    ;      
								creationRectangle (index-1, index, PHJ_IndexMoinsUn+DZ, PHJ_Index+DZ, CouleurRouge, ptrsc)    ;      
								beginIndex = index ;
							}
							else // après 16h02 ou premier rectangle du jour
							{
								creationRectangle (beginIndex, index, PHJ_IndexMoinsUn, PHJ_IndexMoinsUn+DZ, CouleurRouge, ptrsc)    ;     
								beginIndex = index-1 ;
							}
							PHJ_IndexMoinsUn = PHJ_Index ;
						}
					}
					//Dernier rectangle
					creationRectangle (beginIndex, LastVisibleBar, PlusHautJour[LastVisibleBar], PlusHautJour[LastVisibleBar]+DZ, CouleurRouge, ptrsc)    ;    
				}
				
				// Zone transparente rouge en-dessous de PBJ
				{
					int beginIndex = FirstVisibleBar ;
					float PBJ_IndexMoinsUn = PlusBasJour[FirstVisibleBar] ;
					float PBJ_Index ;
					for ( int index = FirstVisibleBar+1 ; index < LastVisibleBar+1 ; index++ )
					{
						PBJ_Index = PlusBasJour[index] ; 
						// on teste si on fait un nouveau plus bas. Si oui, on trace un rectangle entre beginIndex et index-1. Etc jusqu'au prochain nouveau plus haut.
						if ( PBJ_Index != PBJ_IndexMoinsUn)
						{
							
							if ( PBJ_Index != 0 and PBJ_IndexMoinsUn != 0 )
							{
								creationRectangle (beginIndex, index, PBJ_IndexMoinsUn, PBJ_IndexMoinsUn-DZ, CouleurVerte, ptrsc)    ;      
								creationRectangle (index-1, index, PBJ_IndexMoinsUn-DZ, PBJ_Index-DZ, CouleurVerte, ptrsc)    ;      
								beginIndex = index ;
							}
							else // après 16h02 ou premier rectangle du jour
							{
								creationRectangle (beginIndex, index, PBJ_IndexMoinsUn, PBJ_IndexMoinsUn-DZ, CouleurVerte, ptrsc)    ;     
								beginIndex = index-1 ;
							}
							PBJ_IndexMoinsUn = PBJ_Index ;
						}
					}
					//Dernier rectangle
					creationRectangle (beginIndex, LastVisibleBar, PlusBasJour[LastVisibleBar], PlusBasJour[LastVisibleBar]-DZ, CouleurVerte, ptrsc)    ;    
				}

				// pour que le refresh ait lieu immédiatement
				sc.UpdateAlways = 1 ;
			}

			// lorsque je suis déconnecté, pour les backtests :
			// if ( sc.ServerConnectionState != SCS_CONNECTED )  sc.UpdateAlways = 1 ;
			
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_PHPBDynamiquesForFuture(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef PlusHautJour = sc.Subgraph[2];    // appelé par AMR
	SCSubgraphRef PlusBasJour  = sc.Subgraph[3];     // appelé par AMR
	SCSubgraphRef PlusHautHebdo = sc.Subgraph[7]; 
	SCSubgraphRef PlusBasHebdo = sc.Subgraph[8]; 
	SCSubgraphRef PlusHautMensuel = sc.Subgraph[9]; 
	SCSubgraphRef PlusBasMensuel = sc.Subgraph[10]; 
	SCSubgraphRef PlusHautPreOpen = sc.Subgraph[11]; 
	SCSubgraphRef PlusBasPreOpen = sc.Subgraph[12]; 
	
	SCInputRef TraceVectorielONOFF = sc.Input[0];
	SCInputRef InversionRougeVert = sc.Input[1];
	
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	int& TopCoordinatePrevious = sc.GetPersistentIntFast(3);  



	if (sc.SetDefaults)
	{
		sc.GraphName = "PH PB dynamiques du future" ;     // appelé par AMR		//  appelé par "Detecteur de micro-flash-krach sur future (mouvement bref)"
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.AutoLoop = 0;  
		// sc.TransparencyLevel = 90 ;  // 90%
		sc.GraphRegion = 1;  
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		
		sc.HideStudy = 0 ;  // on veut tracer les PH/PB H et M

		
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		InversionRougeVert.Name = "Vert = PH | Rouge = PB";
		InversionRougeVert.SetYesNo(0);

	
		// paramètres des tracés
		for (int SubgraphIndex = 1; SubgraphIndex <= 10; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;  
		}
		PlusHautPreOpen.DrawZeros = true ;  // on le trace égal à zéro pour éviter que le label reste affiché après 18h00.  
		PlusBasPreOpen.DrawZeros = true ;  
		
		
		PlusHautJour.Name = "PH se"; 
		PlusBasJour.Name = "PB se"; 
		PlusHautPreOpen.Name = "PH PO" ;
		PlusBasPreOpen.Name = "PB PO" ;
		PlusHautHebdo.Name = " PH H " ; 
		PlusBasHebdo.Name = " PB H "; 
		PlusHautMensuel.Name = " PH M "; 
		PlusBasMensuel.Name = " PB M "; 
		
		// ne pas afficher les PH/PB du PreOpen pour le FSP500
		SCString ChartSymbol = sc.GetChartSymbol(sc.ChartNumber) ;
		if ( ChartSymbol.Compare("ES",2) == 0 )      // cas du FSP500         // returns 0 if they are the same
		{	PlusHautPreOpen.DrawStyle = DRAWSTYLE_HIDDEN ;  PlusBasPreOpen.DrawStyle = DRAWSTYLE_HIDDEN ; }
		
		
		PlusHautJour.PrimaryColor = RGB(250,0,0);
		PlusBasJour.PrimaryColor = RGB(0,200,0);
		PlusHautPreOpen.PrimaryColor = RGB(250,0,0);
		PlusBasPreOpen.PrimaryColor = RGB(0,200,0);
		PlusHautHebdo.PrimaryColor = RGB(250,0,0);
		PlusBasHebdo.PrimaryColor = RGB(0,200,0);
		PlusHautMensuel.PrimaryColor = RGB(250,0,0);
		PlusBasMensuel.PrimaryColor = RGB(0,200,0);

		PlusHautJour.DrawStyle = DRAWSTYLE_HIDDEN ;
		PlusBasJour.DrawStyle = DRAWSTYLE_HIDDEN ;
		PlusHautPreOpen.DrawStyle = DRAWSTYLE_HIDDEN ;  // DRAWSTYLE_DASH ;
		PlusBasPreOpen.DrawStyle = DRAWSTYLE_HIDDEN ;  // DRAWSTYLE_DASH ;
		PlusHautHebdo.DrawStyle = DRAWSTYLE_HIDDEN ; // DRAWSTYLE_LINE ;
		PlusBasHebdo.DrawStyle = DRAWSTYLE_HIDDEN ;  // DRAWSTYLE_LINE ;
		PlusHautMensuel.DrawStyle = DRAWSTYLE_HIDDEN ; // DRAWSTYLE_LINE ;
		PlusBasMensuel.DrawStyle = DRAWSTYLE_HIDDEN ; // DRAWSTYLE_LINE ;

		PlusHautPreOpen.LineStyle = LINESTYLE_SOLID ;
		PlusBasPreOpen.LineStyle = LINESTYLE_SOLID ;
		PlusHautHebdo.LineStyle = LINESTYLE_SOLID ;
		PlusBasHebdo.LineStyle = LINESTYLE_SOLID ;
		PlusHautMensuel.LineStyle = LINESTYLE_SOLID ;
		PlusBasMensuel.LineStyle = LINESTYLE_SOLID ;
		
		PlusHautJour.LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;
		PlusBasJour.LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;
		PlusHautPreOpen.LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;
		PlusBasPreOpen.LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;
		PlusHautHebdo.LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;
		PlusBasHebdo.LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;
		PlusHautMensuel.LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;
		PlusBasMensuel.LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;
		

		return ;	
	}  // fin de Set.Defaults

	
	if ( sc.UpdateStartIndex == 0  )  
	{ 	
		NbMonitors = NumberOfMonitors() ;
		
		PlusHautJour[0] = sc.High[0] ;  PlusBasJour[0] = sc.Low[0] ;  
		PlusHautPreOpen[0] = 0 ;  PlusBasPreOpen[0] = 0 ; 
		PlusHautHebdo[0] = 0 ;  PlusBasHebdo[0] = 0 ; 
		PlusHautMensuel[0] = 0 ;  PlusBasMensuel[0] = 0 ; 

		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;
		
		
		// gestion automatique des affichages
		SCString ChartSymbol = sc.GetChartSymbol(sc.ChartNumber) ;
		n_ACSIL :: s_BarPeriod BarPeriod ;
		sc.GetBarPeriodParameters(BarPeriod) ;
		// cas du FNQ100
		if (  ChartSymbol.Compare("NQ",2) == 0  and  BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR  )      
		{
			if ( BarPeriod.IntradayChartBarPeriodParameter1 == 16 )  { PlusHautHebdo.Name = "" ;  PlusBasHebdo.Name = "";  PlusHautMensuel.Name = "" ;  PlusBasMensuel.Name = "";  if (sc.ChartNumber == 6) sc.GraphRegion = 1 ;  }  // en 16-ticks, on n'affiche pas les PH/PB H et M
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 200 )  { PlusHautMensuel.Name = "" ;  PlusBasMensuel.Name = "";  /* if (sc.ChartNumber == ??) sc.GraphRegion = 0 ; */  }                       // en 200-ticks, on n'affiche pas les PH/PB M et PO
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 2000 )  { PlusHautPreOpen.Name = "" ;  PlusBasPreOpen.Name = "" ;  /* if (sc.ChartNumber == ??) sc.GraphRegion = 1 ; */  }                  // en 2000-ticks, on n'affiche pas les PH/PB PO
		}
		// cas du FSP500
		if ( ChartSymbol.Compare("ES",2) == 0 )      
		{
			if (  BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR  and BarPeriod.IntradayChartBarPeriodParameter1 == 200  )  { PlusHautHebdo.Name = "" ;  PlusBasHebdo.Name = "";  }
		}
		
		// ne pas afficher les PH/PB du PreOpen pour le FSP500  // tracer dans sc.GraphRegion = 0;  
		if ( ChartSymbol.Compare("ES",2) == 0 )      // cas du FSP500
		{	PlusHautPreOpen.Name = "" ;  PlusBasPreOpen.Name = "" ;  sc.GraphRegion = 0;  }
	}

	
	// Coeur de la study : calcul des SCSubgraphRef
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
		
		if  (    (  sc.BaseDateTimeIn.TimeAt(index) == 64800    and   sc.BaseDateTimeIn.TimeAt(index-1)!= 64800 )  or   index==0  )  // RAZ à la première bougie de 18h00
		{
			PlusHautJour[index] = sc.High[index] ;  PlusBasJour[index] = sc.Low[index] ;  PlusHautPreOpen[0] = 0 ;  PlusBasPreOpen[0] = 0 ;  
		
			if  (  sc.BaseDateTimeIn[index].GetDayOfWeek()   <   sc.BaseDateTimeIn[index-1].GetDayOfWeek()  )  // changement de semaine
				{ PlusHautHebdo[index] = sc.High[index] ;  PlusBasHebdo[index] = sc.Low[index] ;  }   
			else
				{ PlusHautHebdo[index] = max(PlusHautHebdo[index-1],sc.High[index]) ;  PlusBasHebdo[index] = min(PlusBasHebdo[index-1],sc.Low[index]) ;  } // premier chandelier du jour sans changement de semaine
			
			int Year, Month, Day;
			sc.BaseDateTimeIn[index].GetDateYMD(Year, Month, Day);
			if	(	  //  on change de mois :   (  (on est le dernier jour du mois) OU ( le mois a changé entre deux chandeliers successifs càd on est le WE )  )
			(    (  Month==1 and Day==31 )  or (  Month==2 and Day==28 )  or (  Month==3 and Day==31 )  or (  Month==4 and Day==30 )  or (  Month==5 and Day==31 )  or (  Month==6 and Day==30 )  or (  Month==7 and Day==31 )  or 
			(  Month==8 and Day==31 )  or (  Month==9 and Day==30 )  or (  Month==10 and Day==31 )  or (  Month==11 and Day==30 )  or (  Month==1 and Day==31 )  or (  sc.BaseDateTimeIn[index].GetMonth()  !=   sc.BaseDateTimeIn[index-1].GetMonth()  )   )
			)
			{ PlusHautMensuel[index] = sc.High[index] ;  PlusBasMensuel[index] = sc.Low[index] ;  }   
			else
			{ PlusHautMensuel[index] = max(PlusHautMensuel[index-1],sc.High[index]) ;  PlusBasMensuel[index] = min(PlusBasMensuel[index-1],sc.Low[index]) ;  } // premier chandelier du jour sans changement de mois
		}
		
		else if  (    (  sc.BaseDateTimeIn.TimeAt(index) >= 34200    and   sc.BaseDateTimeIn.TimeAt(index-1) < 34200 )   )  // RAZ à la première bougie de 9h30
		{ 	
			PlusHautPreOpen[index] = PlusHautJour[index-1] ;  PlusBasPreOpen[index] = PlusBasJour[index-1] ;
			PlusHautJour[index] = sc.High[index] ;  PlusBasJour[index] = sc.Low[index] ;   
			PlusHautHebdo[index] = max(PlusHautHebdo[index-1],sc.High[index]) ;  PlusBasHebdo[index] = min(PlusBasHebdo[index-1],sc.Low[index]) ;
			PlusHautMensuel[index] = max(PlusHautMensuel[index-1],sc.High[index]) ;  PlusBasMensuel[index] = min(PlusBasMensuel[index-1],sc.Low[index]) ;
		}
		
		else // tous les autres chandeliers
		{
			PlusHautPreOpen[index] = PlusHautPreOpen[index-1] ;  PlusBasPreOpen[index] = PlusBasPreOpen[index-1] ;
			if ( sc.High[index] > PlusHautJour[index-1] ) 	{ PlusHautJour[index] = sc.High[index] ; }	 	else	{ PlusHautJour[index] = PlusHautJour[index-1]  ; }
			if ( sc.Low[index] < PlusBasJour[index-1] ) 	{ PlusBasJour[index] = sc.Low[index] ; }		else	{ PlusBasJour[index] = PlusBasJour[index-1] ;  }
			if ( sc.High[index] > PlusHautHebdo[index-1] ) 	{ PlusHautHebdo[index] = sc.High[index] ; }	 	else	{ PlusHautHebdo[index] = PlusHautHebdo[index-1]  ; }
			if ( sc.Low[index] < PlusBasHebdo[index-1] ) 	{ PlusBasHebdo[index] = sc.Low[index] ; }		else	{ PlusBasHebdo[index] = PlusBasHebdo[index-1] ;  }
			if ( sc.High[index] > PlusHautMensuel[index-1] ) 	{ PlusHautMensuel[index] = sc.High[index] ; }	 	else	{ PlusHautMensuel[index] = PlusHautMensuel[index-1]  ; }
			if ( sc.Low[index] < PlusBasMensuel[index-1] ) 	{ PlusBasMensuel[index] = sc.Low[index] ; }		else	{ PlusBasMensuel[index] = PlusBasMensuel[index-1] ;  }
		}

	}
	
	
	if (sc.MenuEventID == 105)   // Bouton "Niv Fut ON/OFF"    // "PH se" et "PB se" restent toujours affichés
	{
		if ( BooleanHideStudy(&sc) )    // Attention : il y a parfois un bug d'affichage de PHPB du preopen : il est inversé par rapport  au statut de "Niv Fut ON/OFF"   =>  cela provient de l'ordre de calcul de la présente study par rapport à la study prix typique
		{	
			// PlusHautPreOpen.Name = "" ;  PlusBasPreOpen.Name = "";  
			PlusHautPreOpen.DrawStyle = DRAWSTYLE_HIDDEN ;	
			PlusBasPreOpen.DrawStyle = DRAWSTYLE_HIDDEN ;	
		}
		else
		{	
			// PlusHautPreOpen.Name = "PH PO" ;  PlusBasPreOpen.Name = "PB PO" ;  
			PlusHautPreOpen.DrawStyle = DRAWSTYLE_DASH ;	
			PlusBasPreOpen.DrawStyle = DRAWSTYLE_DASH ;	
		}
	}
	
	

	// Bloc d'affichage       
	if ( TraceVectorielONOFF.GetYesNo() )
	{
		if ( sc.ChartWindowIsActive  or  NbMonitors == 2 )
		{
			TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;
			if (  FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(TopCoordinatePrevious-TopCoordinate) > 1  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou TopCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
			{	
				FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
				LastVisibleBar = sc.IndexOfLastVisibleBar ;
				BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;
				TopCoordinatePrevious = TopCoordinate ;

				// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
				sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
				
				float RangeVertical = TopCoordinate - BottomCoordinate ;		// c'est la valeur en points entre le top et le bottom du graphique
				float const DZ = RangeVertical / 66 ;
				s_sc* const ptrsc = &sc ;
				COLORREF CouleurHaute ;
				COLORREF CouleurBasse ;
				
				if ( InversionRougeVert.GetYesNo() ) 	{ CouleurHaute = RGB(0,200,0) ;  CouleurBasse = RGB(250,0,0) ;  }		// suivi de tendance
				else  														{ CouleurHaute = RGB(250,0,0) ;  CouleurBasse = RGB(0,200,0) ;  }     	// contre-tendance (AMR)
				
				// Zone transparente rouge au-dessus de PHJ
				{
					int beginIndex = FirstVisibleBar ;
					float PHJ_IndexMoinsUn = PlusHautJour[FirstVisibleBar] ;
					float PHJ_Index ;
					for ( int index = FirstVisibleBar+1 ; index < LastVisibleBar+1 ; index++ )
					{
						PHJ_Index = PlusHautJour[index] ; 
						// on teste si on fait un nouveau plus haut. Si oui, on trace un rectangle entre beginIndex et index-1. Etc jusqu'au prochain nouveau plus haut.
						if ( PHJ_Index != PHJ_IndexMoinsUn)
						{
							
							if ( PHJ_Index != 0 and PHJ_IndexMoinsUn != 0 )
							{
								creationRectangle (beginIndex, index, PHJ_IndexMoinsUn, PHJ_IndexMoinsUn+DZ, CouleurHaute, ptrsc)    ;      
								creationRectangle (index-1, index, PHJ_IndexMoinsUn+DZ, PHJ_Index+DZ, CouleurHaute, ptrsc)    ;      
								beginIndex = index ;
							}
							else // après 16h02 ou premier rectangle du jour
							{
								creationRectangle (beginIndex, index, PHJ_IndexMoinsUn, PHJ_IndexMoinsUn+DZ, CouleurHaute, ptrsc)    ;     
								beginIndex = index-1 ;
							}
							PHJ_IndexMoinsUn = PHJ_Index ;
						}
					}
					//Dernier rectangle
					creationRectangle (beginIndex, LastVisibleBar, PlusHautJour[LastVisibleBar], PlusHautJour[LastVisibleBar]+DZ, CouleurHaute, ptrsc)    ;    
				}
				
				// Zone transparente rouge en-dessous de PBJ
				{
					int beginIndex = FirstVisibleBar ;
					float PBJ_IndexMoinsUn = PlusBasJour[FirstVisibleBar] ;
					float PBJ_Index ;
					for ( int index = FirstVisibleBar+1 ; index < LastVisibleBar+1 ; index++ )
					{
						PBJ_Index = PlusBasJour[index] ; 
						// on teste si on fait un nouveau plus bas. Si oui, on trace un rectangle entre beginIndex et index-1. Etc jusqu'au prochain nouveau plus haut.
						if ( PBJ_Index != PBJ_IndexMoinsUn)
						{
							
							if ( PBJ_Index != 0 and PBJ_IndexMoinsUn != 0 )
							{
								creationRectangle (beginIndex, index, PBJ_IndexMoinsUn, PBJ_IndexMoinsUn-DZ, CouleurBasse, ptrsc)    ;      
								creationRectangle (index-1, index, PBJ_IndexMoinsUn-DZ, PBJ_Index-DZ, CouleurBasse, ptrsc)    ;      
								beginIndex = index ;
							}
							else // après 16h02 ou premier rectangle du jour
							{
								creationRectangle (beginIndex, index, PBJ_IndexMoinsUn, PBJ_IndexMoinsUn-DZ, CouleurBasse, ptrsc)    ;     
								beginIndex = index-1 ;
							}
							PBJ_IndexMoinsUn = PBJ_Index ;
						}
					}
					//Dernier rectangle
					creationRectangle (beginIndex, LastVisibleBar, PlusBasJour[LastVisibleBar], PlusBasJour[LastVisibleBar]-DZ, CouleurBasse, ptrsc)    ;    
				}
				
				// pour que le refresh ait lieu immédiatement
				sc.UpdateAlways = 1 ;

			}

			// lorsque je suis déconnecté d'IB, pour les backtests :
			// if ( sc.ServerConnectionState != SCS_CONNECTED )  sc.UpdateAlways = 1 ; 
			
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_DistanceAPHPBForIndex(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef Distance = sc.Subgraph[1] ;                       // appelé par scsf_OverlayDistanceAPHPBForSP500
	SCFloatArrayRef PlusHaut = sc.Subgraph[1].Arrays[0] ;  
	SCFloatArrayRef PlusBas = sc.Subgraph[1].Arrays[1] ;  
	SCFloatArrayRef CouleurDeDistance = sc.Subgraph[1].Arrays[2] ;  // rouge = +1, vert = -1               //  appelé par scsf_OverlayDistanceAPHPBForSP500
	SCFloatArrayRef DistanceHaut = sc.Subgraph[1].Arrays[3] ;  
	SCFloatArrayRef DistanceBas = sc.Subgraph[1].Arrays[4] ;  
	

	
	if (sc.SetDefaults)
	{
		sc.GraphName = "Distance aux PH/PB dynamiques de l'indice" ;      // appelé par scsf_OverlayDistanceAPHPBForSP500     
		sc.GraphRegion = 1;  // 0 = graphe principal
		//sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 1;
		sc.AutoLoop = 0;  
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 0;

	
		// paramètres des tracés
		for (int SubgraphIndex = 1; SubgraphIndex <= 1; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineWidth = 1;
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;  
		}
		
		Distance.Name = "Distance (pts) a PH/PB" ; 
		//Distance.PrimaryColor = RGB(0,0,255);

		return ;	
	}  // fin de Set.Defaults
	
	
	if ( sc.UpdateStartIndex == 0 )
	{ 	
		//sc.GraphRegion = 0 ;  
		PlusHaut[0] = sc.High[0] ;
		PlusBas[0] = sc.Low[0] ;
		s_sc* ptrsc = &sc ;  DrawHLine (0, 2, RGB(100,100,100), ptrsc, sc.GraphRegion) ;   // DrawHLine (float niveau, int epaisseur, COLORREF couleur, s_sc* ptrsc, int graphRegion)
	}
	
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
		
		if  (  sc.BaseDateTimeIn.TimeAt(index) < sc.BaseDateTimeIn.TimeAt(index-1)  )  // RAZ d'IntradayBarIndex lors du changement de jour
		{ 	PlusHaut[index] = sc.High[index] ;  PlusBas[index] = sc.Low[index] ; }
		
		else // tous les chandeliers après le chandelier d'ouverture du jour
		{
			if ( sc.High[index] > PlusHaut[index-1] ) 	{ PlusHaut[index] = sc.High[index] ; }	 	else	{ PlusHaut[index] = PlusHaut[index-1]  ; }
			if ( sc.Low[index] < PlusBas[index-1] ) 	{ PlusBas[index] = sc.Low[index] ; }		else	{ PlusBas[index] = PlusBas[index-1] ;  }
		}
		
		DistanceHaut[index] = PlusHaut[index] - sc.High[index] ;
		DistanceBas[index]= sc.Low[index] - PlusBas[index] ;
		Distance[index] = 	fmax ( 0.0001 , min(DistanceHaut[index] , DistanceBas[index] ) ) ;     // on veut éviter d'avoir Distance = 0 car DrawZeros = false
		
		if  (PlusHaut[index] - sc.High[index] > sc.Low[index] - PlusBas[index] )  // on est plus proche de PB que de PH
		{ CouleurDeDistance[index] = -1 ;  Distance.DataColor[index] = RGB(0,200,0) ;  }
		else 
		{ CouleurDeDistance[index] = 1 ;   Distance.DataColor[index] = RGB(255,0,0) ;  }

		
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_DistanceAPHPBForFuture(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef Distance = sc.Subgraph[1] ;                       // appelé par scsf_OverlayDistanceAPHPBForSP500
	SCFloatArrayRef PlusHaut = sc.Subgraph[1].Arrays[0] ;  
	SCFloatArrayRef PlusBas = sc.Subgraph[1].Arrays[1] ;  
	SCFloatArrayRef CouleurDeDistance = sc.Subgraph[1].Arrays[2] ;  // rouge = +1, vert = -1               //  appelé par scsf_OverlayDistanceAPHPBForSP500


	
	if (sc.SetDefaults)
	{
		sc.GraphName = "Distance aux PH/PB dynamiques du future" ;      // appelé par scsf_OverlayDistanceAPHPBForSP500
		sc.GraphRegion = 1;  // 0 = graphe principal
		//sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.AutoLoop = 0;  
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 0;

	
		// paramètres des tracés
		for (int SubgraphIndex = 1; SubgraphIndex <= 1; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineWidth = 1;
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;  
		}
		
		Distance.Name = "Distance (pts) a PH/PB" ; 
		Distance.PrimaryColor = RGB(0,0,255);

		return ;	
	}  // fin de Set.Defaults
	
	
	if ( sc.UpdateStartIndex == 0 )
	{ 	
		sc.GraphRegion = 1 ;  
		PlusHaut[0] = sc.High[0] ;
		PlusBas[0] = sc.Low[0] ;
		s_sc* ptrsc = &sc ;  DrawHLine (0, 2, RGB(100,100,100), ptrsc, sc.GraphRegion) ;   // DrawHLine (float niveau, int epaisseur, COLORREF couleur, s_sc* ptrsc, int graphRegion)
	}
	
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
		
		if  (    (  sc.BaseDateTimeIn.TimeAt(index) == 64800    and   sc.BaseDateTimeIn.TimeAt(index-1)!= 64800 )  or   index==0  )  // RAZ à la première bougie de 18h00
		{ 	PlusHaut[index] = sc.High[index] ;  PlusBas[index] = sc.Low[index] ;  }
		
		else if  (    (  sc.BaseDateTimeIn.TimeAt(index) == 34200    and   sc.BaseDateTimeIn.TimeAt(index-1)!= 34200 )   )  // RAZ à la première bougie de 9h30
		{ 	PlusHaut[index] = sc.High[index] ;  PlusBas[index] = sc.Low[index] ;    }
		
		else // tous les autres chandeliers
		{
			if ( sc.High[index] > PlusHaut[index-1] ) 	{ PlusHaut[index] = sc.High[index] ; }	 	else	{ PlusHaut[index] = PlusHaut[index-1]  ; }
			if ( sc.Low[index] < PlusBas[index-1] ) 	{ PlusBas[index] = sc.Low[index] ; }		else	{ PlusBas[index] = PlusBas[index-1] ;  }
		}
			
		Distance[index] = 	fmax ( 0.0001 , min( PlusHaut[index] - sc.High[index] , sc.Low[index] - PlusBas[index] ) ) ;     // on veut éviter d'avoir Distance = 0 car DrawZeros = false
		if  (PlusHaut[index] - sc.High[index] > sc.Low[index] - PlusBas[index] )  // on est plus proche de PB que de PH
		{ CouleurDeDistance[index] = -1 ;  Distance.DataColor[index] = RGB(0,200,0) ;  }
		else 
		{ CouleurDeDistance[index] = 1 ;   Distance.DataColor[index] = RGB(255,0,0) ;  }
			
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_OverlayDistanceAPHPBForSP500(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef Distance = sc.Subgraph[0]; 
	SCFloatArrayRef DistanceHaut = sc.Subgraph[1].Arrays[3] ;  		// appelé par AMR
	SCFloatArrayRef DistanceBas = sc.Subgraph[1].Arrays[4] ;  		// appelé par AMR
	
	SCInputRef IndiceSP500ChartNumber = sc.Input[0];
	SCInputRef FutureSP500ChartNumber = sc.Input[1];
	SCInputRef Affichage_TicksOuPoints = sc.Input[2]; 
	SCInputRef ChartInputsDistanceIndice = sc.Input[10]; 
	SCInputRef ChartInputsDistanceFuture = sc.Input[11]; 
	
	int& HoraireDebutSeance = sc.GetPersistentInt(1) ;  // time value en secondes
	int& HoraireFinSeance = sc.GetPersistentInt(2) ;
	int& StudyIDIndice = sc.GetPersistentInt(3);  
	int& StudyIDFuture = sc.GetPersistentInt(4);
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "Distance aux PH/PB dynamiques (Indice+Future SP500) pour afficher sur le FNQ100"; 		// appelé par AMR
		sc.GraphRegion = 3;  // 0 = graphe principal
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.AutoLoop = 0;  
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 0;

		
		// Initialisation du ChartNumber pour récupérer l'indice et le future
		IndiceSP500ChartNumber.Name = "ChartNumber de l'indice SP500 en UT 1s";
		IndiceSP500ChartNumber.SetChartNumber(20);
		FutureSP500ChartNumber.Name = "ChartNumber du future SP500 en UT 1s";
		FutureSP500ChartNumber.SetChartNumber(21);
		Affichage_TicksOuPoints.Name = "Affichage de la distance";
		Affichage_TicksOuPoints.SetCustomInputStrings("en ticks;en points");   
		Affichage_TicksOuPoints.SetCustomInputIndex(1);  
		
		if ( Affichage_TicksOuPoints.GetIndex() == 1 ) { Distance.Name = "Distance (points) a PH/PB (Indice SP500)";  sc.ValueFormat = 2; }  else { Distance.Name = "Distance (ticks) a PH/PB (Indice SP500)";  sc.ValueFormat = 1;  }
		Distance.LineWidth = 1;
		Distance.PrimaryColor = RGB(0,0,255);		
		
		for (int SubgraphIndex = 0; SubgraphIndex <= 0; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			//sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
		}
		
	return ;	
	}
	

	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 3;  
		sc.ValueFormat = 1;
		
		if ( Affichage_TicksOuPoints.GetIndex() == 1 ) { Distance.Name = "Distance (points) a PH/PB (Indice SP500)"; }  else { Distance.Name = "Distance (ticks) a PH/PB (Indice SP500)"; }
		
		Distance[0] = 0 ;
		HoraireDebutSeance = HMS_TIME(9,30,00);
		HoraireFinSeance = HMS_TIME(16,00,00);
		
		// Récupération de la studyID de la Distance sur le graphe de l'indice
		StudyIDIndice = sc.GetStudyIDByName(IndiceSP500ChartNumber.GetChartNumber(), "Distance aux PH/PB dynamiques de l'indice", 0) ;
		if  (StudyIDIndice == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart source #%d ne contient pas la distance a PH/PB du ISP500", IndiceSP500ChartNumber.GetChartNumber()) ; sc.AddMessageToLog(message,1) ; }
		else
		{
			// Création des ChartStudySubgraphValues
			ChartInputsDistanceIndice.SetChartStudySubgraphValues(IndiceSP500ChartNumber.GetChartNumber(), StudyIDIndice, 1);   // SCInputRef sc.Input[].SetChartStudySubgraphValues(int ChartNumber, int StudyID, int SubgraphIndex);
		}
		
		// Récupération de la studyID de la Distance sur le graphe du future
		StudyIDFuture = sc.GetStudyIDByName(FutureSP500ChartNumber.GetChartNumber(), "Distance aux PH/PB dynamiques du future", 0) ;
		if  (StudyIDFuture == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart source #%d ne contient pas la distance a PH/PB du FSP500", FutureSP500ChartNumber.GetChartNumber()) ; sc.AddMessageToLog(message,1) ; }
		else
		{
			// Création des ChartStudySubgraphValues
			ChartInputsDistanceFuture.SetChartStudySubgraphValues(FutureSP500ChartNumber.GetChartNumber(), StudyIDFuture, 1);   // SCInputRef sc.Input[].SetChartStudySubgraphValues(int ChartNumber, int StudyID, int SubgraphIndex);
		}
	}
	

	// on récupère l'array de Distance pour l'indice
	SCFloatArray DistanceIndice ;
	sc.GetStudyArrayFromChartUsingID(ChartInputsDistanceIndice.GetChartStudySubgraphValues(), DistanceIndice);   //   void GetStudyArrayFromChartUsingID(const s_ChartStudySubgraphValues& ChartStudySubgraphValues, SCFloatArrayRef SubgraphArray);

	// on récupère l'array de Distance pour le future
	SCFloatArray DistanceFuture ;
	sc.GetStudyArrayFromChartUsingID(ChartInputsDistanceFuture.GetChartStudySubgraphValues(), DistanceFuture);   //   void GetStudyArrayFromChartUsingID(const s_ChartStudySubgraphValues& ChartStudySubgraphValues, SCFloatArrayRef SubgraphArray);
	
	// on récupère l'array de CouleurDeDistance pour l'indice
	SCFloatArray CouleurDeDistanceIndice ;
	sc.GetStudyExtraArrayFromChartUsingID(IndiceSP500ChartNumber.GetChartNumber(), StudyIDIndice, 1, 2, CouleurDeDistanceIndice);   //  int GetStudyExtraArrayFromChartUsingID(int ChartNumber, int StudyID, int SubgraphIndex, int ExtraArrayIndex, SCFloatArrayRef ExtraArrayRef);
	
	// on récupère l'array de CouleurDeDistance pour le future
	SCFloatArray CouleurDeDistanceFuture ;
	sc.GetStudyExtraArrayFromChartUsingID(FutureSP500ChartNumber.GetChartNumber(), StudyIDFuture, 1, 2, CouleurDeDistanceFuture);   //  int GetStudyExtraArrayFromChartUsingID(int ChartNumber, int StudyID, int SubgraphIndex, int ExtraArrayIndex, SCFloatArrayRef ExtraArrayRef);

	// on récupère les arrays de DistanceIndiceHaut et DistanceIndiceBas pour l'indice 
	SCFloatArray DistanceIndiceHaut, DistanceIndiceBas ;
	sc.GetStudyExtraArrayFromChartUsingID(IndiceSP500ChartNumber.GetChartNumber(), StudyIDIndice, 1, 3, DistanceIndiceHaut);   //  int GetStudyExtraArrayFromChartUsingID(int ChartNumber, int StudyID, int SubgraphIndex, int ExtraArrayIndex, SCFloatArrayRef ExtraArrayRef);
	sc.GetStudyExtraArrayFromChartUsingID(IndiceSP500ChartNumber.GetChartNumber(), StudyIDIndice, 1, 4, DistanceIndiceBas);   //  int GetStudyExtraArrayFromChartUsingID(int ChartNumber, int StudyID, int SubgraphIndex, int ExtraArrayIndex, SCFloatArrayRef ExtraArrayRef);

	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                               
	{
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		int PreviousOpenTimeValue = sc.BaseDateTimeIn[index-1].GetTimeInSecondsWithoutMilliseconds()  ; 
		
		
		if ( OpenTimeValue > HoraireDebutSeance and OpenTimeValue < HoraireFinSeance )  // le marché action est ouvert
		{
			if  ( OpenTimeValue  !=  PreviousOpenTimeValue )   // Distance n'est actualisé que s'il y a une nouvelle seconde 
			{ 
				// on récupère l'index sur le chart indice SP500 en UT 1s
				int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(IndiceSP500ChartNumber.GetChartNumber(), index); 
				if ( Affichage_TicksOuPoints.GetIndex() )  	Distance[index] = fmax ( 0.0001 ,    DistanceIndice[ExternalChartIndex] ) ;	 // on veut éviter d'avoir Distance = 0 car DrawZeros = false. 
																		else  Distance[index] = fmax ( 0.0001 , 4*DistanceIndice[ExternalChartIndex] ) ;	 // multiplié par 4 pour passer en ticks
				// couleur du tracé, DistanceIndiceHaut, DistanceIndiceBas
				if (CouleurDeDistanceIndice[ExternalChartIndex] == 1 )  { Distance.DataColor[index] = RGB(255,0,0) ; } else  { Distance.DataColor[index] = RGB(0,200,0) ; }
				DistanceHaut[index] = DistanceIndiceHaut[ExternalChartIndex] ;
				DistanceBas[index] = DistanceIndiceBas[ExternalChartIndex] ;
			}
			else
			{
				Distance[index] = Distance[index-1] ;	
				Distance.DataColor[index] = Distance.DataColor[index-1] ;	
				DistanceHaut[index] = DistanceHaut[index-1] ;
				DistanceBas[index] = DistanceBas[index-1] ;

			}
		}			
		
		else  // le marché action est fermé
		{
			if  ( OpenTimeValue  !=  PreviousOpenTimeValue )   // Distance n'est actualisé que s'il y a une nouvelle seconde 
			{ 
				// on récupère l'index sur le chart future SP500 en UT 1s
				int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(FutureSP500ChartNumber.GetChartNumber(), index); 
				if ( Affichage_TicksOuPoints.GetIndex() )  	Distance[index] = fmax ( 0.0001 ,    DistanceFuture[ExternalChartIndex] ) ;	 // on veut éviter d'avoir Distance = 0 car DrawZeros = false. 
				else																Distance[index] = fmax ( 0.0001 , 4*DistanceFuture[ExternalChartIndex] ) ;	 // multiplié par 4 pour passer en ticks
				// couleur du tracé
				if (CouleurDeDistanceFuture[ExternalChartIndex] == 1 )  { Distance.DataColor[index] = RGB(255,0,0) ; } else  { Distance.DataColor[index] = RGB(0,200,0) ; }
				DistanceHaut[index] = 0 ;		// pas utilisé pour le moment sur future
				DistanceBas[index] = 0 ;
			}
			else
			{
				Distance[index] = Distance[index-1] ;	
				Distance.DataColor[index] = Distance.DataColor[index-1] ;	
				DistanceHaut[index] = 0 ;		// pas utilisé pour le moment sur future
				DistanceBas[index] = 0 ;
			}
		}
		
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int NiveauxIndicesAffiches_Statut(s_sc* ptrsc)    // vaut 0 (OFF), 1 (All) ou 2 (Lean)
{
	int NiveauxIndicesAffiches_Statut = 0 ;
	int StudyIDPrixTypique = ptrsc->GetStudyIDByName(ptrsc->ChartNumber, "Prix typique", 0);
	if  (StudyIDPrixTypique == 0 )  
	{	SCString message ;  message.Format("!! ERREUR !! Le graphique ne contient pas la study ''Prix typique''. Necessaire pour determiner sc.HideStudy.") ; ptrsc->AddMessageToLog(message,1) ; }
	else
	{	ptrsc->GetChartStudyInputInt(ptrsc->ChartNumber, StudyIDPrixTypique, 5, NiveauxIndicesAffiches_Statut) ;  }    // int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
	return NiveauxIndicesAffiches_Statut ;
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
		BeginDateTime = 0;  // 0 = position à gauche ; 150 pour la position à droite
		TextAlignment = DT_VCENTER | DT_LEFT ;
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
			AddMethod = UTAM_ADD_ALWAYS;	
			DrawingType = DRAWING_LINE;
			LineStyle = LINESTYLE_SOLID;
			BeginIndex = beginIndex ;
			EndIndex = endIndex ;
			BeginValue = niveau ;
			EndValue = niveau ;
			LineWidth = epaisseur;
			Color = rgb; 
			Region = ptrsc->GraphRegion ; 
			DrawUnderneathMainGraph = true ;
		} 
	};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
	{
		if ( niveau > BottomCoordinate  and  niveau < TopCoordinate )
		{
			if ( monEpaisseur > 0  and  niveau > 500 )
			{
				//Création d'instance de MaLigneJusquAIndex et appel de sc.UseTool
				MaLigneJusquAIndex objetMaLigne(beginIndex, endIndex, niveau, monEpaisseur, maCouleur, ptrsc);
				ptrsc->UseTool(objetMaLigne); // c'est sc.UseTool qui est appelé
				//Création d'instances de MonTexte et appel de sc.UseTool
				MonTexteString objetMonTexteString(Texte, niveau, monEpaisseur, maCouleur, ptrsc);
				ptrsc->UseTool(objetMonTexteString); 
			}
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Marker : s_UseTool    // crée un Marker pour être affiché en coordonnées (indice,prix)
{											
	Marker (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur) : s_UseTool()    
	{
		Clear(); 
		AddMethod = UTAM_ADD_ALWAYS ;		
		// AddAsUserDrawnDrawing = 1 ;    // This member specifies that a drawing is to be added as a user drawn drawing, which allows the user to interact with the ACSIL added drawing on the chart as if it had been added by the user with one of the drawing tools on the Tools menu.
		// LineNumber = Line_Number ;		
		DrawingType = DRAWING_MARKER;
		MarkerType = marqueur ;
		BeginIndex = index ;
		BeginValue = prix ;
		MarkerSize = tailleObjet ;
		Color = couleur ;
		// ChartNumber = chartNumber ;
		Region = 1 ; 
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawMarker (int marqueur, int index, float niveau, int tailleObjet, COLORREF couleur, s_sc* ptrsc)  
{
	if ( niveau > BottomCoordinate  and  niveau < TopCoordinate )
	{
		//Création d'instance de Marker_UTAMAOA_UDD et appel de sc.UseTool
		Marker objetMarker (marqueur, index, niveau, tailleObjet, couleur) ;
		ptrsc->UseTool(objetMarker); 
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_PHPBDynamiquesForIndex_Projetes(SCStudyInterfaceRef sc)
{
	SCSubgraphRef PHJ_Index = sc.Subgraph[1];        // appelé par Navig_Niveaux
	SCSubgraphRef PBJ_Index = sc.Subgraph[2];	      // appelé par Navig_Niveaux
	
	SCFloatArrayRef PHJ_Index_Projete = sc.Subgraph[1].Arrays[0] ;   	//  PHJ_Index + fSpreadMoyen		 // appelé par Navig_Niveaux
	SCFloatArrayRef PBJ_Index_Projete = sc.Subgraph[2].Arrays[0] ; 	//  PBJ_Index + fSpreadMoyen		 // appelé par Navig_Niveaux
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[1].Arrays[1] ;  



	SCInputRef ChartInputs_PHJ_Index = sc.Input[1];  
	SCInputRef ChartInputs_PBJ_Index = sc.Input[2];  
	SCInputRef TraceVectorielONOFF = sc.Input[3];		// appelé par NavigationNiveaux			// modifié par Tour de controle
	
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	int& TopCoordinatePrevious = sc.GetPersistentIntFast(3);  
	int& HoraireDebutTrace = sc.GetPersistentIntFast(4) ;  // time value en secondes
	int& HoraireFinSeance = sc.GetPersistentIntFast(5) ;
	int& StudyIDOverlayINQ100 = sc.GetPersistentIntFast(6) ;
	int& StudyID_PHPBDynamiquesForIndex = sc.GetPersistentIntFast(7) ;
	

	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.CalculationPrecedence = LOW_PREC_LEVEL  ;  // le SpreadMoyen (Study Overlay INQ100) doit être calculé précédemment
		sc.GraphName = "[G] PHJ PBJ dynamiques de l'indice projetes sur le future";        // appelé par NavigationNiveaux			// appelé par Tour de controle
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;	
		sc.HideStudy = 1 ;

		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		
		
		for (int SubgraphIndex = 1; SubgraphIndex <= 2; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_NAME_REVERSE_COLORS |  LL_NAME_ALIGN_LEFT_EDGE ;  // LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
		}

		PHJ_Index.PrimaryColor = RGB(250,0,0);
		PBJ_Index.PrimaryColor = RGB(0,200,0);
		
		PHJ_Index.Name = "PH J";
		PBJ_Index.Name = "PB J";
		
		PHJ_Index.LineWidth = 2;
		PBJ_Index.LineWidth = 2;
		
		return;
	}  // fin du bloc sc.SetDefaults
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 1 ;
		
		NbMonitors = NumberOfMonitors() ;
	
		//Initialisations des Subgraph 
		PHJ_Index[0] = 0 ;                        
		PBJ_Index[0] = 0 ;                        
		PHJ_Index_Projete[0] = 0 ;         
		PBJ_Index_Projete[0] = 0 ;          
		
		// HoraireDebutTrace = 9h35min
		HoraireDebutTrace = HMS_TIME(9,33,0);   
		HoraireFinSeance = HMS_TIME(16,0,0);
		
		// Récupération de la StudyIDOverlayINQ100 sur le graphe 16-ticks
		StudyIDOverlayINQ100 = sc.GetStudyIDByName(sc.ChartNumber, "Overlay INQ100 sur le future (avec correction du spread)", 0) ;
		if  (StudyIDOverlayINQ100 == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart #6 ne contient pas la study ''Overlay INQ100 sur le future (avec correction du spread)''") ; sc.AddMessageToLog(message,1) ; }
	
		// Récupération de la StudyID_PHPBDynamiquesForIndex sur le chart UT1s 
		const int ChartNumber_Source_PHJPBJ = 4 ;
		StudyID_PHPBDynamiquesForIndex = sc.GetStudyIDByName(ChartNumber_Source_PHJPBJ, "PH PB dynamiques de l'indice", 0) ; 
		if  (StudyID_PHPBDynamiquesForIndex == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart source #%d ne contient pas la study ''PH PB dynamiques de l'indice''", ChartNumber_Source_PHJPBJ) ; sc.AddMessageToLog(message,1) ; }
		else	
		{	
			ChartInputs_PHJ_Index.SetChartStudySubgraphValues(ChartNumber_Source_PHJPBJ, StudyID_PHPBDynamiquesForIndex, 6) ;
			ChartInputs_PBJ_Index.SetChartStudySubgraphValues(ChartNumber_Source_PHJPBJ, StudyID_PHPBDynamiquesForIndex, 7) ; 
		}	
	}
	
	
	// on récupère l'array de SpreadMoyen
	SCFloatArray SpreadMoyenArray ;
	sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDOverlayINQ100, 0, 1, SpreadMoyenArray) ;
	
	// on récupère l'array de PHJ_ExternalChart
	SCFloatArray PHJ_ExternalChart;
	sc.GetStudyArrayFromChartUsingID(ChartInputs_PHJ_Index.GetChartStudySubgraphValues(), PHJ_ExternalChart);   //   void GetStudyArrayFromChartUsingID(const s_ChartStudySubgraphValues& ChartStudySubgraphValues, SCFloatArrayRef SubgraphArray);

	// on récupère l'array de PBJ_ExternalChart
	SCFloatArray PBJ_ExternalChart;
	sc.GetStudyArrayFromChartUsingID(ChartInputs_PBJ_Index.GetChartStudySubgraphValues(), PBJ_ExternalChart);   
	

	// On calcule les valeurs de PHJ_Index et PBJ_Index projetees
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // AutoLoop
	{
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		int PreviousOpenTimeValue = sc.BaseDateTimeIn[index-1].GetTimeInSecondsWithoutMilliseconds()  ; 
		
		if ( OpenTimeValue >= HoraireDebutTrace  and  OpenTimeValue < HoraireFinSeance )  // n'est actualisé que entre HoraireDebutTrace et HoraireFinSeance
		{
			if  ( OpenTimeValue  !=  PreviousOpenTimeValue )   //  n'est actualisé que s'il y a une nouvelle seconde 
			{ 
				// calcul de fSpreadMoyen
				float fSpreadMoyen = SpreadMoyenArray[index] ;
				
				// on récupère l'ExternalIndex sur le chart #4, puis on calcule PHJ_Index et PBJ_Index
				int ExternalIndex = sc.GetContainingIndexForDateTimeIndex(4, index-250); 	// 250 barres = 4000 ticks en arrière
				PHJ_Index[index] = PHJ_ExternalChart[ExternalIndex] ;		
				PBJ_Index[index] = PBJ_ExternalChart[ExternalIndex] ;		
				
				// Calcul des valeurs de PHJ_Index et PBJ_Index projetes dans la première seconde après HoraireDebutTrace
				if (  PHJ_Index_Projete[index-1] == 0  and  OpenTimeValue >=  HoraireDebutTrace  )   // teste si on dépasse HoraireDebutTrace et que PHJ_Index n'ont pas encore été calculés  
				{
					FirstIndex[index] = index ;
					PHJ_Index_Projete[index] = PHJ_Index[index] + fSpreadMoyen ;
					PBJ_Index_Projete[index] = PBJ_Index[index] + fSpreadMoyen ;
				}

				else // on n'est pas dans le premier chandelier du jour, mais on est dans une nouvelle seconde => on rappelle la valeur de chaque niveau et on l'adjuste du spread
				{
					FirstIndex[index] = FirstIndex[index-1] ;
					PHJ_Index_Projete[index] = PHJ_Index[index] + fSpreadMoyen ;
					PBJ_Index_Projete[index] = PBJ_Index[index] + fSpreadMoyen ;
				}
			}
			
			else   // la barre ne débute pas dans une nouvelle seconde => on recopie les valeurs de l'index précédent
			{	
				FirstIndex[index] = FirstIndex[index-1] ;
				PHJ_Index[index] = PHJ_Index[index-1] ;
				PBJ_Index[index] = PBJ_Index[index-1] ;
				PHJ_Index_Projete[index] = PHJ_Index_Projete[index-1] ;
				PBJ_Index_Projete[index] = PBJ_Index_Projete[index-1] ;
			}  
		}
		else
		{
			PHJ_Index_Projete[index] = 0 ;
			PBJ_Index_Projete[index] = 0 ;
		}
		
	} // fin de AutoLoop
	

/* 	if (sc.MenuEventID == 106)   // Bouton "Niv Ind ON/OFF"
	{
		if ( NiveauxIndicesAffiches_Statut(&sc) == 1 )   // All
		{
			TraceVectorielONOFF.SetYesNo(1) ;
		}
		else if ( NiveauxIndicesAffiches_Statut(&sc) == 2 )  // Lean
		{
			TraceVectorielONOFF.SetYesNo(1) ;
		}
		else // OFF
		{
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ; 
			TraceVectorielONOFF.SetYesNo(0) ;
		}
	}	
	 */
	
	
	// Bloc d'affichage       
	if ( TraceVectorielONOFF.GetYesNo() )
	{
		if ( sc.ChartWindowIsActive  or  NbMonitors == 2 )
		{
			TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;
			if (  FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(TopCoordinatePrevious-TopCoordinate) > 1  )          	// Le recalcul des lignes n'est réalisé que si FirstVisibleBar ou LastVisibleBar ou TopCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
			{	
				FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
				LastVisibleBar = sc.IndexOfLastVisibleBar ;
				BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;
				TopCoordinatePrevious = TopCoordinate ;
						
				// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
				sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
				
				s_sc* const ptrsc = &sc;
				int const LastVisibleBar = sc.IndexOfLastVisibleBar ;
				int const firstIndex = FirstIndex[LastVisibleBar] ;
				
				// exécution des fonctions CreationLigneEtTexte
				for (int SubgraphIndex = 1; SubgraphIndex <= 2; SubgraphIndex++)
				{
					float const Niveau = sc.Subgraph[SubgraphIndex].Arrays[0][LastVisibleBar] ;    // la ligne vectorielle est tracée sur une ordonnée prise à LastVisibleBar
					SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
					int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
					COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
					CreationLigneEtTexte(Texte, /* firstIndex */ LastVisibleBar-250, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);    // void CreationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
					DrawMarker (MARKER_DIAMOND, LastVisibleBar-250, Niveau, 4, couleur, ptrsc) ; 		// void DrawMarker (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc)  
				}

				// pour que le refresh ait lieu immédiatement
				sc.UpdateAlways = 1 ;

				// lorsque je suis déconnecté du serveur, pour les backtests :
				// if ( sc.ServerConnectionState != SCS_CONNECTED )  
				// {	sc.UpdateAlways = 1 ;  }
			}
		}
	}

}



