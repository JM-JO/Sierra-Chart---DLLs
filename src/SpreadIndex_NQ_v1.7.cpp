/* 
INDICATEUR POUR LE X-TICK
PeriodeEMA = nb de variations incluses dans l'EMA : 400 pour le 8-ticks, 200 pour le 16-ticks.
v1.0 (2019/2/11) : indicateur identique à "Spread Index NQ v2.1.1" de PRT.
v1.1 (2019/2/12) : sc.SetDefaults calcule automatiquement PeriodeEMA = 3200/XTicks.
v1.2 (2019/3/3) : la study "affichage" récupère la StudyID et le SubGraph de la study "calcul" source par le biais du Name du SubGraph. Manual looping.
v1.3 (2020/01/06) : la study "affichage" est tracée vectoriellement sur sc.GraphRegion = 2 et avec DRAWSTYLE_IGNORE.
v1.3.1 : le SpreadIndex n'est actualisé que toutes les X barres, pour accélérer le calcul de la study. Le 28/2/2020, j'ai repassé la valeur de X de 20 à 1 pour éviter les ondulations du graphe de RatioAUC car SI était très élevé (SI=4) ; le tps de calcul de la study est passé de 5 à 33ms.
v1.4 (2020/03/31) : création de la study "SI moyen de la seance precedente".
v1.5 (2020/04/01) : la study "SI moyen de la seance precedente" retourne la distribution de variation de pas de cotation sur la séance précédente.
v1.6 (2020/04/01) : création de la study "SI moyen de X ticks" : tous les X ticks, on calcule le SI moyen des derniers X ticks.
v1.7 (2020/10/22) : ajout d'un warning dans scsf_SpreadIndexAffichage si la valeur de SI est constante (càd que le date range sur chart 1-tick est insuffisant).


Améliorations à apporter :
	- pour scsf_SpreadIndex_Moyen_XTicks : accélérer la study grace à : 1°) looping manuel,  2°) myStream utilisé dans le looping doit être déclaré en dehors de la boucle de looping (les nombreuses écritures sur SSD sont longues)
	- pour scsf_SpreadIndex_Moyen_XTicks : ne garder que les bougies entre 9h30 et 15h50.
	- pour scsf_SpreadIndex_Moyen_XTicks : doubler (voire quadrupler, ou même faire x16) l'échantillonnage de bougies de 4000 ticks en prenant des bougies de 4000 ticks décalées de 2000 ( ou 1000 ticks pour quadrupler).
	- pour le SP500, faire les calculs en double float, car il converge vers 1.00014 en cas de forte liquidité.

SOMMAIRE DES STRUCTURES ET FONCTIONS :
LigneHorizontale
DrawLine
scsf_SpreadIndexCalcul
TexteAligneDroit_UTAMAOA
scsf_SpreadIndexAffichage
scsf_SpreadIndex_Moyen_1Seance
Highest
Lowest
scsf_SpreadIndex_Moyen_XTicks

	
	
 */

 
#include "sierrachart.h"  // tous les autres headers doivent être placés avant sierrachart.h


SCDLLName("SpreadIndex")

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct LigneHorizontale : s_UseTool  // LigneHorizontale crée un objet qui contient mes paramètres choisis pour une ligne horizontale  (hérite de toutes les données public et protected de s_UseTool)
	{
		LigneHorizontale(float niveau, int epaisseur, COLORREF maCouleur, int Graphregion) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
		{
			Clear(); 
			DrawingType = DRAWING_HORIZONTALLINE;
			LineStyle = LINESTYLE_SOLID;
			AddMethod = UTAM_ADD_ALWAYS;	
			BeginValue = niveau;
			LineWidth = epaisseur;
			Color = maCouleur; 
			Region = Graphregion ;   // 0 par défaut
		} 
	};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
void DrawLine (float monNiveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc, int Graphregion)
{
	//Création d'instance de LigneHorizontale et appel de sc.UseTool
	LigneHorizontale objetLigneHorizontale(monNiveau, monEpaisseur, maCouleur, Graphregion);
	ptrsc->UseTool(objetLigneHorizontale); // c'est sc.UseTool qui est appelé
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_SpreadIndexCalcul(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef SpreadIndex = sc.Subgraph[0];   // !! SetChartStudySubgraphValues utilise [0] pour : scsf_SpreadIndexAffichage et Detecteur de micro-flash-krach sur future
	// SCFloatArrayRef SpreadIndexNonArrondi = sc.Subgraph[0].Arrays[0];		
	
	
	SCInputRef PeriodeEMA = sc.Input[0];  

	float& alpha = sc.GetPersistentFloatFast(1); 
	float& beta = sc.GetPersistentFloatFast(2); 
	float& TailleTick = sc.GetPersistentFloatFast(3);  // c'est pertinent de le passer par variable persistante si on a différents futures avec des TailleTick différentes
	
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "SpreadIndexCalcul";   // !! GetStudyIDByName récupère ce GraphName pour : scsf_SpreadIndexAffichage et scsf_Sigmax512x256x128x64x32x16
		sc.GraphRegion = 1;
		//sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.AutoLoop = 0;  
		sc.AutoScalePaddingPercentage = 0.2; // A value of 0.4 means that padding takes up 40% of the chart, and the graph uses the rest of the 60%. 
		sc.DisplayStudyName = 0;
		sc.DisplayStudyInputValues = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		PeriodeEMA.Name = "PeriodeEMA";
		
		SpreadIndex.Name = "Spread Index"; 
		SpreadIndex.LineWidth = 1;
		SpreadIndex.PrimaryColor = RGB(0,0,255);		

		for (int SubgraphIndex = 0; SubgraphIndex <= 0; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = true ;
		}
		
		return ;	
	}
	
	
	if ( sc.UpdateStartIndex== 0 )  
	{
		// CALCUL DE PeriodeEMA
		// détermination du XTicks (ne fonctionne pas si placé dans sc.SetDefaults)
		n_ACSIL :: s_BarPeriod BarPeriod ;
		sc.GetBarPeriodParameters(BarPeriod) ;
		if ( BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR )   // on vérifie que le graphe est bien en XTicks
		{	
			int xTicks = BarPeriod.IntradayChartBarPeriodParameter1 ; 	
			PeriodeEMA.SetInt(round(3200.0f/xTicks)) ;			
		}  
		else // message d'erreur dans le log
		{
			SCString message ;   
			message.Format("!! ERREUR !! Le graphique n'est pas en ticks, impossible de tracer l'indicateur SpreadIndexCalcul") ;    
			sc.AddMessageToLog(message,1) ;
		}
		
		SpreadIndex[0] = 1.1 ;
		float fPeriodeEMA = (float)PeriodeEMA.GetInt() ;
		alpha = 2 / ( fPeriodeEMA + 1 ) ;
		beta = 1 - alpha ; 
		TailleTick = 0.25 ;	
	}
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
		if (  sc.Open[index] != sc.Close[index-1]  )
		{
			float ecart = fabs( sc.Open[index] - sc.Close[index-1] ) ;
			SpreadIndex[index] = beta * SpreadIndex[index-1]  + alpha * ecart / TailleTick ;
		}
		else
		{
			SpreadIndex[index]  = SpreadIndex[index-1] ;
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TexteAligneDroit_UTAMAOA : s_UseTool  
{											
	TexteAligneDroit_UTAMAOA (SCString texte, int tailleTexte,  COLORREF couleur, int graphRegion, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;			
		LineNumber = Line_Number ;
		DrawingType = DRAWING_TEXT;
		Text = texte ;		
		BeginDateTime = 148 ;     // To use a value relative to the left side of the chart for BeginDateTime rather than an absolute Date Time value, specify an integer value from 1 to 150
		UseRelativeVerticalValues = true ;   // If UseRelativeVerticalValues is set to 1 or a nonzero value, then BeginValue is a value relative to the Chart Region itself. It is a percentage. Where 1 = 1%.
		BeginValue = 50 ;   
		FontSize = tailleTexte ; 
		Color = couleur ;
		Region = graphRegion ; 
		FontBold = 1 ;
		TextAlignment = DT_VCENTER | DT_RIGHT;  // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TexteCentre_UTAMAOA : s_UseTool  
{											
	TexteCentre_UTAMAOA (SCString texte, int tailleTexte,  COLORREF couleur, int graphRegion, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;			
		LineNumber = Line_Number ;
		DrawingType = DRAWING_TEXT;
		Text = texte ;		
		BeginDateTime = 75 ;     // To use a value relative to the left side of the chart for BeginDateTime rather than an absolute Date Time value, specify an integer value from 1 to 150
		UseRelativeVerticalValues = true ;   // If UseRelativeVerticalValues is set to 1 or a nonzero value, then BeginValue is a value relative to the Chart Region itself. It is a percentage. Where 1 = 1%.
		BeginValue = 50 ;   
		FontSize = tailleTexte ; 
		Color = couleur ;
		Region = graphRegion ; 
		FontBold = 1 ;
		TextAlignment = DT_VCENTER | DT_CENTER;  // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_SpreadIndexAffichage(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef SpreadIndexAffichage = sc.Subgraph[0]; 
	
	SCInputRef ChartNumberSpreadIndexCalcul = sc.Input[1]; 
	SCInputRef ChartInputsSpreadIndexCalcul = sc.Input[2];  
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "SpreadIndexAffichage" ; 
		sc.GraphRegion = 2;
		//sc.ScaleRangeType = SCALE_SAMEASREGION ;
		sc.ValueFormat = 2 ;
		sc.AutoLoop = 0 ;  
		sc.DisplayStudyName = 0;
		sc.DisplayStudyInputValues = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;

		
		// Inputs
		ChartNumberSpreadIndexCalcul.Name = "ChartNumber du calcul du SpreadIndex";
		ChartNumberSpreadIndexCalcul.SetChartNumber(13);
		
		
		// Subgraphs parameters
		for (int SubgraphIndex = 0; SubgraphIndex <= 0; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = true ;
		}
		SpreadIndexAffichage.Name = "SI"; 
		SpreadIndexAffichage.LineWidth = 1;
		SpreadIndexAffichage.PrimaryColor = RGB(0,0,255);		

		return ;	
	}
	
	
	if ( sc.UpdateStartIndex == 0 )  
	{
		sc.GraphRegion = 2 ;
		// Récupération de la studyID
		int StudyID = sc.GetStudyIDByName(ChartNumberSpreadIndexCalcul.GetChartNumber(), "SpreadIndexCalcul", 0) ;
		// on crée ChartStudySubgraphValues
		if  (StudyID == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart source #%d ne contient pas de calcul du SpreadIndex", ChartNumberSpreadIndexCalcul.GetChartNumber()) ; sc.AddMessageToLog(message,1) ; }
		else	{	ChartInputsSpreadIndexCalcul.SetChartStudySubgraphValues(ChartNumberSpreadIndexCalcul.GetChartNumber(),StudyID,0);	}	
		
		if (sc.ChartNumber == 6 )  sc.Subgraph[0].DrawStyle = DRAWSTYLE_IGNORE ;
		
		if (sc.ChartNumber == 12 ) 		// chart 2000-ticks
		{
			// Ajout de lignes horizontales
			COLORREF Gris = RGB(200, 200, 200) ;
			DrawLine(1, 3, Gris, &sc, sc.GraphRegion) ;			// void DrawLine (float monNiveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc, int Graphregion)
			DrawLine(1.125, 1, Gris, &sc, sc.GraphRegion) ;
			DrawLine(1.25, 1, Gris, &sc, sc.GraphRegion) ;
			DrawLine(1.5, 2, Gris, &sc, sc.GraphRegion) ;
			DrawLine(1.75, 1, Gris, &sc, sc.GraphRegion) ;
			DrawLine(2, 3, Gris, &sc, sc.GraphRegion) ;
			DrawLine(2.5, 2, Gris, &sc, sc.GraphRegion) ;
			DrawLine(3, 3, Gris, &sc, sc.GraphRegion) ;
			DrawLine(3.5, 2, Gris, &sc, sc.GraphRegion) ;
			DrawLine(4, 3, Gris, &sc, sc.GraphRegion) ;
			DrawLine(5, 3, Gris, &sc, sc.GraphRegion) ;
			DrawLine(6, 3, Gris, &sc, sc.GraphRegion) ;
		}
	}
	
	
	// on récupère la valeur du SpreadIndex dans le Subgraph adéquat
	SCFloatArray DataGrapheCalculs;
	sc.GetStudyArrayFromChartUsingID(ChartInputsSpreadIndexCalcul.GetChartStudySubgraphValues(), DataGrapheCalculs);   //   void GetStudyArrayFromChartUsingID(const s_ChartStudySubgraphValues& ChartStudySubgraphValues, SCFloatArrayRef SubgraphArray);
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
	
		// on récupère l'index du nouveau chandelier sur le chart d'où provient le calcul (UT 1-tick)
		int ChartCalculsIndex = sc.GetContainingIndexForDateTimeIndex(ChartNumberSpreadIndexCalcul.GetChartNumber(), index); 
		SpreadIndexAffichage[index] = DataGrapheCalculs[ChartCalculsIndex] ;
		
		// toutes les 1000 barres, on s'assure que SpreadIndexAffichage n'est pas constant
		if ( index %1000 == 0  and  index > 0  and  sc.ChartNumber == 6 )  
		{
			const float epsilon = 0.0000001 ;
			if ( fabs(SpreadIndexAffichage[index] - SpreadIndexAffichage[index-300]) < epsilon )
			{
				if (fabs(SpreadIndexAffichage[index] - SpreadIndexAffichage[index-600]) < epsilon )
				{
					// Création d'instance de TexteBasCentre_UTAMAOA et appel de sc.UseTool
					SCString Texte ;
					Texte.Format("!! WARNING !! SpreadIndexAffichage est constant car le date range sur chart 1-tick est insuffisant") ;
					TexteCentre_UTAMAOA objetTexte(Texte, 14, RGB(255,0,0), 2, 10002+10000*sc.StudyGraphInstanceID);               
					sc.UseTool(objetTexte); 
					sc.AddMessageToLog(Texte, 0) ;
				}
			}
		}
		
	}

	
	// Création d'instance de TexteBasCentre_UTAMAOA et appel de sc.UseTool  (à chaque appel du DLL)
	SCString Texte ;
	float SI = SpreadIndexAffichage[sc.IndexOfLastVisibleBar] ;
	if ( SI < 1.1 )  Texte.Format("SI = %.3f", SI) ;  else if ( SI < 3 )  Texte.Format("SI = %.2f", SI) ;  else  Texte.Format("SI = %.1f", SI) ;
	TexteAligneDroit_UTAMAOA objetTexte(Texte, 8, RGB(0,0,255), 2, 10001+10000*sc.StudyGraphInstanceID);               
	sc.UseTool(objetTexte); 
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_SpreadIndex_Moyen_1Seance_Et_Distribution_Prix(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef SI_Moyen = sc.Subgraph[2];       
	
	SCInputRef SI_Moyen_DansMessageLog = sc.Input[0]; 
	SCInputRef DistributionDesVariations_DansMessageLog = sc.Input[1]; 
	
	int& BarIndexRAZ = sc.GetPersistentInt(1);  
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "SI moyen de la seance precedente et distribution des variations de prix (pour chart 1-tick)";    
		// sc.GraphRegion = 0;  // 0 = graphe principal
		sc.ValueFormat = 2;
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		SI_Moyen_DansMessageLog.Name = "SI moyen de la seance precedente (dans MessageLog)" ;
		SI_Moyen_DansMessageLog.SetYesNo(0) ;
		DistributionDesVariations_DansMessageLog.Name = "Distribution des variations de prix (dans MessageLog)" ;
		DistributionDesVariations_DansMessageLog.SetYesNo(0) ;
		

		SI_Moyen.Name = "SI moyen de la seance precedente"; 
		SI_Moyen.LineWidth = 3;
		SI_Moyen.PrimaryColor = RGB(0,0,0);   
		SI_Moyen.DrawStyle = DRAWSTYLE_LINE;
		SI_Moyen.LineStyle = LINESTYLE_SOLID ;
		SI_Moyen.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		SI_Moyen.DrawZeros = false;

		return ;	
	}
	
	
	if ( sc.Index == 0 )
	{
		SI_Moyen[sc.Index] = 0 ;
		BarIndexRAZ = 0 ;
		
		// Titre de la série de messages dans le log
		if ( SI_Moyen_DansMessageLog.GetYesNo()  and  DistributionDesVariations_DansMessageLog.GetYesNo() )
		{
			SCString Texte ;  
			Texte.Format("XXXXXXXXXX          SI moyen de la seance precedente et distribution des variations de prix          XXXXXXXXXX") ;
			sc.AddMessageToLog(Texte,0) ;
		}
		else if ( SI_Moyen_DansMessageLog.GetYesNo() )
		{
			SCString Texte ;  
			Texte.Format("XXXXXXXXXX          SI moyen de la seance precedente          XXXXXXXXXX") ;
			sc.AddMessageToLog(Texte,0) ;
		}
		if ( DistributionDesVariations_DansMessageLog.GetYesNo() )
		{
			SCString Texte ;  
			Texte.Format("XXXXXXXXXX          Distribution des variations de prix          XXXXXXXXXX") ;
			sc.AddMessageToLog(Texte,0) ;
		}
	}	
	
	
	
	
	// RAZ des paramètres lors d'un changement de séance  // 64800 est le nb de secondes à 18h   
	if  (  sc.BaseDateTimeIn.TimeAt(sc.Index) >= 64800    and   sc.BaseDateTimeIn.TimeAt(sc.Index-1)< 64800   )   
	{
		
		// Calcul de la somme des changements de prix
		double Somme_ChangementPrix = 0 ;
		int Nb_ChangementPrix = 0 ;
		int Ecarts[101] = {0} ;  // Ecart[0] correspond à 0 variation, Ecart[1] correspond à une variation de 1 pas (0.25pt),  etc...
		

		
		if ( sc.Index > 0 )
		{
			for ( int i = BarIndexRAZ+1 ; i < sc.Index ; ++i )  // on parcoure les index de la séance précédente (qui débute à BarIndexRAZ et se termine à sc.Index-1)   
			{
				if (  sc.Open[i] != sc.Close[i-1]  )
				{
					float ecart = fabs( sc.Open[i] - sc.Close[i-1] ) ;
					Nb_ChangementPrix ++ ;
					Somme_ChangementPrix += ecart ;
					
					// Calcul de la distribution
					if (ecart < 25.0)  Ecarts[int(ecart*4)] = Ecarts[int(ecart*4)] + 1 ;   
					else Ecarts[100] = Ecarts[100] +1 ;  // si l'écart est supérieur à 25 pts on met tous les changements dans Ecart[100]
				}
				else
				{
					Ecarts[0] = Ecarts[0] +1 ;
				}
			}
		}
		
		// Calcul de SI_Moyen de la séance précédente
		float PasDeCotation = 0.25 ;	
		if ( Nb_ChangementPrix != 0 )  { SI_Moyen[sc.Index] = Somme_ChangementPrix / Nb_ChangementPrix / PasDeCotation  ;  }
		else SI_Moyen[sc.Index] = 0 ;
		

		
		// Messages dans Log
		if ( SI_Moyen_DansMessageLog.GetYesNo()  or  DistributionDesVariations_DansMessageLog.GetYesNo() )
		{
			int Year, Month, Day ;  sc.BaseDateTimeIn[sc.Index-1].GetDateYMD(Year, Month, Day) ;
			int NbEcartsCollectes = (sc.Index-1) - (BarIndexRAZ+1) ;
			
			// SI_Moyen de la séance
			if ( SI_Moyen_DansMessageLog.GetYesNo() )
			{
				SCString Texte ;  
				Texte.Format("Seance du %04d-%02d-%02d | SpreaIndexMoyen de la seance =  %.2f   ", Year, Month, Day, SI_Moyen[sc.Index]) ;
				sc.AddMessageToLog(Texte,0) ;
			}
			
			// Distribution des changements de prix
			if ( DistributionDesVariations_DansMessageLog.GetYesNo() )
			{
				for ( int i = 0 ; i < 100 ; i++)
				{
					if ( Ecarts[i] != 0 )
					{			
						float Distribution = float(Ecarts[i]) / NbEcartsCollectes ;
						SCString Texte ;  
						Texte.Format("Seance du %04d-%02d-%02d | Distribution de %d pas : %8.4f%%    ", Year, Month, Day, i, Distribution*100) ;
						sc.AddMessageToLog(Texte,0) ;
					}
				}
				// Cas i==100
				if ( Ecarts[100] != 0 )
				{			
					float Distribution = float(Ecarts[100]) / NbEcartsCollectes ;
					SCString Texte ;  
					Texte.Format("Seance du %04d-%02d-%02d | Distribution de 100+ pas : %8.4f%%    ", Year, Month, Day, Distribution*100) ;
					sc.AddMessageToLog(Texte,0) ;
				}
			}
		}
		
		
		
		// RAZ pour la nouvelle séance qui vient de débuter
		BarIndexRAZ = sc.Index ;
	}
	else
	{ 
		SI_Moyen[sc.Index] = SI_Moyen[sc.Index-1] ;
	}
	
	

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Highest (int IndexDebut, int NbChandeliersEnArriere, s_sc* ptrsc)  		
{
	// mettre erreur dans Log si NbChandeliers <1	 // note : en l'état si NbChandeliers <1, Highest retourne -FLT_MAX	// NbChandeliersEnArriere = 1  ne retourne que le High de IndexDebut
	float highest = -FLT_MAX  ;
	for (int Index = IndexDebut ; Index > IndexDebut - NbChandeliersEnArriere ; Index--)    // Highest et Lowest sont construits de la même manière que les fonctions du même nom de PRT
	{
		if ( ptrsc->BaseData[SC_HIGH][Index] > highest)  highest = ptrsc->BaseData[SC_HIGH][Index] ;
	}
	return highest ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Lowest (int IndexDebut, int NbChandeliersEnArriere, s_sc* ptrsc)
{
	// mettre erreur dans Log si NbChandeliers <1 // note : en l'état si NbChandeliers <1, Highest retourne FLT_MAX
	float lowest = FLT_MAX  ;
	for (int Index = IndexDebut ; Index > IndexDebut - NbChandeliersEnArriere ; Index--)
	{
		if ( ptrsc->BaseData[SC_LOW][Index] < lowest)  lowest = ptrsc->BaseData[SC_LOW][Index] ;
	}
	return lowest ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string ConversionFloat(double number, int Precision)   	// fonction qui convertit un float en string avec un séparateur décimal "," pour le Excel français.	// Precision est le nb de chiffres après la virgule
{
	std::string MyString ;
	std::ostringstream oss;  // ostringstream = output stream class to operate on strings
	
	// Conversion of the float number into a string
	oss << std::fixed;														// std::fixed garantit que les chiffres avant la virgule ne sont pas tronqués.   
	oss << std::setprecision(Precision) << number;     // std::setprecision donne le nb de chiffres après la virgule.
	MyString = oss.str() ;
	
	// Replacement of '.' by ','    
	std::replace( MyString.begin(), MyString.end(), '.', ',');
	
	// Cas particuliers : +inf, -inf
	if ( number < -999999 )  MyString = "infini" ;
	if ( number > 999999 )  MyString = "infini" ;

	return MyString ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_SpreadIndex_Moyen_XTicks(SCStudyInterfaceRef sc) 
{
	SCInputRef X_Ticks_Input = sc.Input[1]; 
	
	SCSubgraphRef SI_Moyen = sc.Subgraph[1];       
	SCSubgraphRef Range = sc.Subgraph[2];       
	
	int& BarIndexRAZ = sc.GetPersistentInt(1);  
	int& X_Ticks = sc.GetPersistentInt(2);  
	
	SCString& NameLogFile = sc.GetPersistentSCString(1);
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "SI moyen et range de X ticks (pour chart 1-tick)";    
		sc.GraphRegion = 1;
		sc.ValueFormat = 2;
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		X_Ticks_Input.Name = "X Ticks par bougie" ;
		X_Ticks_Input.SetInt(4000) ;
		
		SI_Moyen.Name = "SI moyen et range de X ticks"; 
		SI_Moyen.LineWidth = 3;
		SI_Moyen.PrimaryColor = RGB(0,0,0);   
		SI_Moyen.DrawStyle = DRAWSTYLE_LINE;
		SI_Moyen.LineStyle = LINESTYLE_SOLID ;
		SI_Moyen.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		SI_Moyen.DrawZeros = false;
		Range.Name = "Range de X ticks"; 
		Range.LineWidth = 3;
		Range.PrimaryColor = RGB(0,0,0);   
		Range.DrawStyle = DRAWSTYLE_LINE;
		Range.LineStyle = LINESTYLE_SOLID ;
		Range.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur

		return ;	
	}
	
	
	if ( sc.Index == 0 )
	{
		sc.GraphRegion = 1;
		
		SI_Moyen[sc.Index] = 0 ;
		BarIndexRAZ = 0 ;
		X_Ticks = X_Ticks_Input.GetInt() ;
		
		// Création du nom du fichier .csv
		{
			// Récupération date première séance et date dernière séance
			int Year_Debut, Month_Debut, Day_Debut ;
			SCDateTime SeancePlusUn = sc.BaseDateTimeIn[0] + SCDateTime::DAYS(1) ;					// le +1 est pour avoir la date de la séance car sc.Index==0 est le tick de début de séance à 18h00
			SeancePlusUn.GetDateYMD(Year_Debut, Month_Debut, Day_Debut) ;		
			int Year_Fin, Month_Fin, Day_Fin ;  
			sc.BaseDateTimeIn[sc.ArraySize-1].GetDateYMD(Year_Fin, Month_Fin, Day_Fin) ;		
			
			
			// Nom de fichier
			NameLogFile.Format("Range et SI_Moyen en UT %d ticks - %04d-%02d-%02d au %04d-%02d-%02d.csv", X_Ticks, Year_Debut, Month_Debut, Day_Debut, Year_Fin, Month_Fin, Day_Fin) ;
			
			// Envoi de ce nom dans le log
			SCString message ;
			message.Format("Ecriture en cours dans le fichier ''%s''", NameLogFile.GetChars() ) ;    // the GetChars() function on the SCString to be able to access the internal C++ character pointer which is necessary when using this class with the SCString Format() and AppendFormat() functions.
			sc.AddMessageToLog(message,0) ;
			
		}
		
		// Création des noms de colonne s'il s'agit de la première ouverture du fichier
		std::string string_NomFichierLog = NameLogFile.GetChars(); // conversion SCString en string
		remove(NameLogFile.GetChars()) ;		// effacement du fichier au cas où il existerait déjà  		// remove() exige un const char* en argument et ne compile pas avec un string
		{ 
			// Déclaration du flux d'écriture
			std::ofstream myStream ;
			myStream.open(string_NomFichierLog, std::ofstream::out | std::ofstream::app) ;        
			if ( myStream.is_open() )
			{	 
				// remplissage de la première ligne du fichier .csv
				myStream << "Date" << ";" << "Horaire" << ";" << "SI_Moyen (" << X_Ticks << "-ticks)" << ";" << "Range (" << X_Ticks <<  "-ticks)" <<  "\n" ; 
			}  
			else {	SCString message ;  message.Format("Ouverture du fichier a la creation : erreur") ;  sc.AddMessageToLog(message, 1) ;  }
		}

	}	
	
	
	
	
	// Calcul de SI si on fait X ticks
	if ( (sc.Index-BarIndexRAZ)%X_Ticks == 0 )
	{
		if ( sc.Index > 0 )
		{
			// Calcul de la somme des changements de prix
			double Somme_ChangementPrix = 0 ;
			int Nb_ChangementPrix = 0 ;
			for ( int i = BarIndexRAZ+1 ; i < sc.Index ; ++i )  // on parcoure les index de la barre de X ticks (qui débute à BarIndexRAZ et se termine à sc.Index-1)   
			{
				if (  sc.Open[i] != sc.Close[i-1]  )
				{
					float ecart = fabs( sc.Open[i] - sc.Close[i-1] ) ;
					Nb_ChangementPrix ++ ;
					Somme_ChangementPrix += ecart ;
				}
			}
			
			// Calcul de SI_Moyen de la barre de X-ticks précédente
			float PasDeCotation = 0.25 ;	
			if ( Nb_ChangementPrix != 0 )  { SI_Moyen[sc.Index] = Somme_ChangementPrix / Nb_ChangementPrix / PasDeCotation  ;  }
			else SI_Moyen[sc.Index] = 0.01 ;
			
			// Calcul du Range de la barre de X-ticks précédente
			s_sc* const ptrsc = &sc;  
			float highest = Highest (sc.Index-1, X_Ticks, ptrsc) ;  		 			// float Highest (int IndexDebut, int NbChandeliersEnArriere, s_sc* ptrsc)  		
			float lowest = Lowest (sc.Index-1, X_Ticks, ptrsc) ;
			Range[sc.Index] = highest - lowest ;
			
			
			
			// Récupération date et heure
			int Year, Month, Day ;  sc.BaseDateTimeIn[sc.Index-1].GetDateYMD(Year, Month, Day) ;
			int Hour, Minute, Second ;  TIME_TO_HMS(sc.BaseDateTimeIn[sc.Index-1].GetTimeInSecondsWithoutMilliseconds(), Hour, Minute, Second);

			
			// Ecriture dans console Sierra
			{
				SCString Texte ;
				Texte.Format("Seance du %04d-%02d-%02d | %02dh%02dm%02ds | Range = %3.2f | SI_Moyen = %1.2f ", Year, Month, Day, Hour, Minute, Second, Range[sc.Index], SI_Moyen[sc.Index]) ;     	
				sc.AddMessageToLog(Texte,0) ;
			}
			
			// Ecriture dans fichier .txt
			{
				// Déclaration du flux d'écriture
				std::ofstream myStream ;
				myStream.open(NameLogFile, std::ofstream::out | std::ofstream::app) ;
				if ( myStream.is_open() ) 	{	/* SCString message ;  message.Format("Ouverture du fichier : OK") ;  sc.AddMessageToLog(message, 0) ; */  }  else {	SCString message ;  message.Format("Ouverture du fichier : erreur") ;  sc.AddMessageToLog(message, 0) ;  }
				
				// remplissage du fichier .csv 
				myStream  << Year << "-" << Month << "-" << Day << ";" << Hour<< "h" << Minute << "m" << Second << "s" << ";" << ConversionFloat(SI_Moyen[sc.Index],5) << ";" << ConversionFloat(Range[sc.Index],2) << "\n" ;
				
			}
			
		}
		
		// RAZ pour la nouvelle barre qui vient de débuter
		BarIndexRAZ = sc.Index ;
		
	}
	else
	{ 
		SI_Moyen[sc.Index] = SI_Moyen[sc.Index-1] ;
		Range[sc.Index] = Range[sc.Index-1] ;
	}

	
	// RAZ de BarIndexRAZ lors d'un changement de séance  // 64800 est le nb de secondes à 18h   
	if  (  sc.BaseDateTimeIn.TimeAt(sc.Index) >= 64800    and   sc.BaseDateTimeIn.TimeAt(sc.Index-1)< 64800   )   
	{
		// RAZ pour la nouvelle séance qui vient de débuter
		BarIndexRAZ = sc.Index ;
	}

}



