/* 
v1.0 (2019/2/20) : cet indicateur retourne la différence entre le future (projeté sur l'index en UT 1s avec la study OverlayBar) et l'index (en UT 1s). La différence est calculée sur l'open des bougies.
v1.1 (2019/2/20) : cet indicateur retourne la différence entre le future (prélevé depuis le chart du future en UT 1s) et l'index (en UT 1s). La différence est calculée sur l'open des bougies.
v1.2 (2019/2/20) : on ajoute les éléments de l'indicateur "Outils SpreadCF NQ v2.0.3" de PRT.
v1.3 (2019/2/20) : on trace le SpreadMoyen et le SpreadMoyenArrondi sous le future.
v1.4 (2019/3/18) : pas de calcul, ni de tracé après 16h00.
v1.5 (2019/9/28) : à la cloture de l'indice (16h00) on affiche le dernier SpreadMoyenArrondi. La valeur de DernierSpreadMoyen est reprise pour remplacer les MM10/MM60/MM300 à 9h30m15s et tant qu'elles n'ont pas été calculées.
v1.6 (2019/10/05) : la fonction scsf_SpreadFutureIndexForIndex est renommée scsf_SpreadFutureIndexForIndex_MM10MM60MM300. On crée une fonction scsf_SpreadFutureIndexForIndex_MM300 qui ne retourne que la MM300.
                             Attention : les deux fonctions ont le même sc.GraphName.
v1.7 (2019/10/05) : mise en place du manual looping.


Améliorations à apporter :
	- que se passe-t-il si le future n'a pas encore fait de tick dans la nouvelle seconde et qu'on calcule le spread dans la nouvelle seconde ? Dans ce cas, à quoi correspond ExternalChartData[SC_OPEN][ExternalChartIndex] ? Question miroir pour versions > v2.0 (pas de tick indice).
	- tester "Add Additional Symbol" pour tracer l'indice et le future sur le même chart. https://www.sierrachart.com/index.php?page=doc/StudiesReference.php&ID=400&Name=Add_Additional_Symbol

	
	
SOMMAIRE DES STRUCTURES ET FONCTIONS :
SegmentVertical
DrawVLine
MoyenneMobileSimple
MonTexte
Drawtext
scsf_SpreadFutureIndexForIndex
scsf_SpreadFutureIndexForFuture
	
 */
 
#include "sierrachart.h"

SCDLLName("Spread Future-Index")

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct SegmentVertical : s_UseTool  // SegmentVertical crée un objet qui contient mes paramètres choisis pour une ligne Verticale  (hérite de toutes les données public et protected de s_UseTool)
	{
		SegmentVertical(int index, int epaisseur, COLORREF maCouleur, int Graphregion) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
		{
		Clear(); 
		DrawingType = DRAWING_LINE;
		LineStyle = LINESTYLE_SOLID;
		AddMethod = UTAM_ADD_ALWAYS;	
		BeginIndex = index;
		BeginValue = -1000 ;
		EndIndex = index ;
		EndValue = 1000 ;
		LineWidth = epaisseur;
		Color = maCouleur; 
		Region = Graphregion ;   // 0 par défaut
		} 
	};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawVLine (int index, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc, int Graphregion)
{
	//Création d'instance de LigneHorizontale et appel de sc.UseTool
	SegmentVertical objetSegmentVertical(index, monEpaisseur, maCouleur, Graphregion);
	ptrsc->UseTool(objetSegmentVertical); // c'est sc.UseTool qui est appelé
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float MoyenneMobileSimple (int periode, SCSubgraphRef Data, int index)   // periode doit être strictement positif
{
	float DataCumule = 0 ;
	
	for (int i = index ; i > index - periode ; i-- )
	{ DataCumule += Data[i] ; }
	
	return DataCumule/periode ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MonTexte : s_UseTool // MonTexte crée du texte avec mes paramètres choisis  (hérite de toutes les données public et protected de s_UseTool)
{											
	MonTexte(SCString texte, int IndexDebut, float y, COLORREF maCouleur, int tailleTexte, int Graphregion) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_ALWAYS;			
		DrawingType = DRAWING_TEXT;
		Text = texte;	
		BeginIndex = IndexDebut ;    // alternative : BeginDateTime
		BeginValue = y;
		Color = maCouleur ;		
		FontSize = tailleTexte ;  // 10 est bien
		Region = Graphregion ;   // 0 par défaut
		FontBold = 1;
		TextAlignment = DT_VCENTER | DT_CENTER ;
	} 
};
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Drawtext (SCString texte, int IndexDebut, float y, COLORREF maCouleur, int tailleTexte, s_sc* ptrsc, int Graphregion)
{
	//Création d'instances de MonTexte et appel de sc.UseTool
	MonTexte objetMonTexte(texte, IndexDebut, y, maCouleur, tailleTexte, Graphregion);
	ptrsc->UseTool(objetMonTexte); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_SpreadFutureIndexForIndex_MM10MM60MM300(SCStudyInterfaceRef sc)   // fonction désuète avec 3 MM simultaneés  // le SpreadMoyen est une moyenne pondérée des MM10, MM60 et MM 300.
{
	
	SCSubgraphRef Spread = sc.Subgraph[1]; 
	SCSubgraphRef MM10 = sc.Subgraph[2]; 
	SCSubgraphRef MM60 = sc.Subgraph[3]; 
	SCSubgraphRef MM300 = sc.Subgraph[4]; 
	SCSubgraphRef SpreadMoyen = sc.Subgraph[5];                  	 // !! GetStudyIDByName récupère ce SubgraphIndex pour : scsf_SpreadFutureIndexForFuture et scsf_OverlayINQ100PlusSpread
	SCSubgraphRef SpreadMoyenArrondi = sc.Subgraph[6]; 
	
	SCInputRef ExternalChartNumber = sc.Input[0];
	
	int& IntradayBarIndexRAZ = sc.GetPersistentInt(1);  
	int& HoraireFinSeance = sc.GetPersistentInt(2) ;
	
	float& DernierSpreadMoyen = sc.GetPersistentFloat(1);  
	
	


	if (sc.SetDefaults)
	{
		sc.GraphName = "Spread Future-Indice pour tracer sous l'indice MM10MM60MM300" ; 	  // !! GetStudyIDByName récupère ce Name pour : scsf_SpreadFutureIndexForFuture et scsf_OverlayINQ100PlusSpread
		//sc.GraphRegion = 0;  // 0 = graphe principal
		//sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.AutoLoop = 1;  
		
		
		// Initialisation du ChartNumber pour récupérer le future
		ExternalChartNumber.Name = "ChartNumber du future en UT 1s";
		ExternalChartNumber.SetChartNumber(14);

		
		// paramètres des tracés
		for (int SubgraphIndex = 1; SubgraphIndex <= 6; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			//sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			//sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;  // attention, il faudra convertir les "vrais zéros" en 0.000001
		}
		Spread.Name = "Spread"; 
		Spread.LineWidth = 1;
		Spread.PrimaryColor = RGB(0, 200, 0);
		Spread.DrawStyle = DRAWSTYLE_HIDDEN;
		MM10.Name = "MM10"; 
		MM10.LineWidth = 1;
		MM10.PrimaryColor = RGB(150, 150, 0);
		MM60.Name = "MM60"; 
		MM60.LineWidth = 1;
		MM60.PrimaryColor = RGB(255, 0, 255);
		MM300.Name = "MM300"; 
		MM300.LineWidth = 1;
		MM300.PrimaryColor = RGB(0, 0, 255);
		SpreadMoyen.Name = "SpreadMoyen"; 
		SpreadMoyen.LineWidth = 1;
		SpreadMoyen.PrimaryColor = RGB(0, 0, 0);
		//SpreadMoyen.DrawStyle = DRAWSTYLE_HIDDEN;
		SpreadMoyenArrondi.Name = "SpreadMoyenArrondi"; 
		SpreadMoyenArrondi.LineWidth = 2;
		SpreadMoyenArrondi.PrimaryColor = RGB(0, 0, 0);
		SpreadMoyenArrondi.LineLabel =  LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER |  LL_VALUE_ALIGN_VALUES_SCALE; 

		return ;	
	}  // fin de Set.Defaults
	
	
	if ( sc.Index == 0 ) // on vérifie que le graphe de l'index et le graphe du future sont bien en UT 1s
	{	
		// vérification de l'UT de l'index
		n_ACSIL::s_BarPeriod BarPeriod;
		sc.GetBarPeriodParameters(BarPeriod);
		if ( BarPeriod.IntradayChartBarPeriodType != IBPT_DAYS_MINS_SECS   or  BarPeriod.IntradayChartBarPeriodParameter1 != 1  )   // on vérifie que le graphe est bien en UT 1s
		{  SCString message ;   message.Format("!! Erreur !! Le graphe #%d n'est pas en UT 1s", sc.ChartNumber) ;    sc.AddMessageToLog(message,1) ; }
		// vérification de l'UT du future : il n'y a pas de fonction similaire à sc.GetBarPeriodParameters() pour récupérer l'UT d'un autre graphe.
		
		IntradayBarIndexRAZ = 0 ;
		
		HoraireFinSeance = HMS_TIME(16,00,01);
		
		DernierSpreadMoyen = 0;
	}
	
	
	
	// RAZ d'IntradayBarIndex lors du changement de jour
	if  (  sc.BaseDateTimeIn.TimeAt(sc.Index) < sc.BaseDateTimeIn.TimeAt(sc.Index-1)   )  
	{ IntradayBarIndexRAZ = sc.Index ; }
	// Tracé de lignes verticales à t0, 10s, 1min, 5min
	if ( sc.Index-IntradayBarIndexRAZ==0 or sc.Index-IntradayBarIndexRAZ==9 or sc.Index-IntradayBarIndexRAZ==59 or sc.Index-IntradayBarIndexRAZ==299 )
	{ s_sc* ptrsc = &sc ;   DrawVLine(sc.Index, 2, RGB(0,0,0), ptrsc, sc.GraphRegion) ; }  
	
	
	// OpenTimeValue du chandelier actif
	int OpenTimeValue = sc.BaseDateTimeIn[sc.Index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value

	
	// récupération des data du future et calcul du spread
	SCGraphData ExternalChartData; 
	sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
	int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), sc.Index); // on récupère l'index dans le graphe du future
	
	if ( ExternalChartIndex != 0  and  OpenTimeValue < HoraireFinSeance )
	{ Spread[sc.Index] = ExternalChartData[SC_OPEN][ExternalChartIndex] - sc.Open[sc.Index] ;  }
	
	
	

	if ( OpenTimeValue < HoraireFinSeance )  // n'est actualisé que pendant l'ouverture du marché action
	{
		if  ( sc.Index-IntradayBarIndexRAZ > 298 )
		{
			MM10[sc.Index]   = MoyenneMobileSimple(10, Spread, sc.Index) ;
			MM60[sc.Index]   = MoyenneMobileSimple(60, Spread, sc.Index) ;
			MM300[sc.Index] = MoyenneMobileSimple(300, Spread, sc.Index) ;
			SpreadMoyen[sc.Index] = ( MM10[sc.Index]  + 2*MM60[sc.Index]  + 4*MM300[sc.Index] ) /7 ;
			MM10[sc.Index]   = round( 2* MM10[sc.Index] )/2 ;
			MM60[sc.Index]   = round( 2* MM60[sc.Index] )/2 ;
			MM300[sc.Index] = round( 2* MM300[sc.Index] )/2 ;
			
			if  ( SpreadMoyen[sc.Index] > SpreadMoyenArrondi[sc.Index-1] + 0.5 )
			{	
				SpreadMoyenArrondi[sc.Index] = SpreadMoyenArrondi[sc.Index-1] + 0.5 ;
				s_sc* ptrsc = &sc ;  SCString texte ;  texte.Format("%.1f", SpreadMoyenArrondi[sc.Index] ) ; Drawtext(texte, sc.Index, SpreadMoyenArrondi[sc.Index]+0.6, RGB(0,0,0), 12, ptrsc, sc.GraphRegion) ;   
			}
			else if ( SpreadMoyen[sc.Index] < SpreadMoyenArrondi[sc.Index-1] - 0.5 )
			{	
				SpreadMoyenArrondi[sc.Index] = SpreadMoyenArrondi[sc.Index-1] - 0.5 ;
				s_sc* ptrsc = &sc ;  SCString texte ;  texte.Format("%.1f", SpreadMoyenArrondi[sc.Index] ) ; Drawtext(texte, sc.Index, SpreadMoyenArrondi[sc.Index]-0.6, RGB(0,0,0), 12, ptrsc, sc.GraphRegion) ;   
			}
			else
			{ 	SpreadMoyenArrondi[sc.Index] = SpreadMoyenArrondi[sc.Index-1] ;   }		
		}
		
		
		else if ( sc.Index-IntradayBarIndexRAZ >= 59 and sc.Index-IntradayBarIndexRAZ < 299) 
		{
			MM10[sc.Index] = MoyenneMobileSimple(10, Spread, sc.Index) ;
			MM60[sc.Index] = MoyenneMobileSimple(60, Spread, sc.Index) ;
			SpreadMoyen[sc.Index] = ( MM10[sc.Index]  + 2*MM60[sc.Index] ) /3 ;

			if  ( SpreadMoyen[sc.Index] > SpreadMoyenArrondi[sc.Index-1] + 0.5 )
			{
				SpreadMoyenArrondi[sc.Index] = SpreadMoyenArrondi[sc.Index-1] + 0.5 ; 
				s_sc* ptrsc = &sc ;  SCString texte ;  texte.Format("%.1f", SpreadMoyenArrondi[sc.Index] ) ; Drawtext(texte, sc.Index, SpreadMoyenArrondi[sc.Index]+0.6, RGB(0,0,0), 12, ptrsc, sc.GraphRegion) ;   
			}
			else if ( SpreadMoyen[sc.Index] < SpreadMoyenArrondi[sc.Index-1] - 0.5 )
			{ 
				SpreadMoyenArrondi[sc.Index] = SpreadMoyenArrondi[sc.Index-1] - 0.5 ;
				s_sc* ptrsc = &sc ;  SCString texte ;  texte.Format("%.1f", SpreadMoyenArrondi[sc.Index] ) ; Drawtext(texte, sc.Index, SpreadMoyenArrondi[sc.Index]-0.6, RGB(0,0,0), 12, ptrsc, sc.GraphRegion) ;   
			}
			else
			{ 	SpreadMoyenArrondi[sc.Index] = SpreadMoyenArrondi[sc.Index-1] ; }
			
			MM10[sc.Index] = round( 2* MM10[sc.Index] )/2 ;
			MM60[sc.Index] = round( 2* MM60[sc.Index] )/2 ;
			MM300[sc.Index] = MM300[sc.Index-1] ;
		}
		
		
		else if ( sc.Index-IntradayBarIndexRAZ > 9 and sc.Index-IntradayBarIndexRAZ < 59) 
		{
			MM10[sc.Index] = MoyenneMobileSimple(10, Spread, sc.Index) ;
			SpreadMoyen[sc.Index] = MM10[sc.Index] ;
			
			//SpreadMoyenArrondi[sc.Index] = SpreadMoyenArrondi[sc.Index-1] ;
			if  ( SpreadMoyen[sc.Index] > SpreadMoyenArrondi[sc.Index-1] + 0.5 )
			{
				SpreadMoyenArrondi[sc.Index] = SpreadMoyenArrondi[sc.Index-1] + 0.5 ; 
				s_sc* ptrsc = &sc ;  SCString texte ;  texte.Format("%.1f", SpreadMoyenArrondi[sc.Index] ) ; Drawtext(texte, sc.Index, SpreadMoyenArrondi[sc.Index]+0.6, RGB(0,0,0), 12, ptrsc, sc.GraphRegion) ;   
			}
			else if ( SpreadMoyen[sc.Index] < SpreadMoyenArrondi[sc.Index-1] - 0.5 )
			{ 
				SpreadMoyenArrondi[sc.Index] = SpreadMoyenArrondi[sc.Index-1] - 0.5 ;
				s_sc* ptrsc = &sc ;  SCString texte ;  texte.Format("%.1f", SpreadMoyenArrondi[sc.Index] ) ; Drawtext(texte, sc.Index, SpreadMoyenArrondi[sc.Index]-0.6, RGB(0,0,0), 12, ptrsc, sc.GraphRegion) ;   
			}
			else
			{ 	SpreadMoyenArrondi[sc.Index] = SpreadMoyenArrondi[sc.Index-1] ; }

			MM10[sc.Index] = round( 2* MoyenneMobileSimple(10, Spread, sc.Index) )/2 ;
			MM60[sc.Index] = MM60[sc.Index-1] ;
			MM300[sc.Index] = MM300[sc.Index-1] ;
		}
		
		
		else if ( sc.Index-IntradayBarIndexRAZ == 9 ) 
		{
			MM10[sc.Index] = MoyenneMobileSimple(10, Spread, sc.Index) ;
			SpreadMoyen[sc.Index] = MM10[sc.Index] ;
			
			SpreadMoyenArrondi[sc.Index] = round ( 2*SpreadMoyen[sc.Index] ) /2 ;
			{   s_sc* ptrsc = &sc ;  SCString texte ;  texte.Format("%.1f", SpreadMoyenArrondi[sc.Index] ) ; Drawtext(texte, sc.Index, SpreadMoyenArrondi[sc.Index]-0.6, RGB(0,0,0), 12, ptrsc, sc.GraphRegion) ;   }

			MM10[sc.Index] = round( 2* MoyenneMobileSimple(10, Spread, sc.Index) )/2 ;
			MM60[sc.Index] = MM60[sc.Index-1] ;
			MM300[sc.Index] = MM300[sc.Index-1] ;
		}
		
		
		else if ( sc.Index-IntradayBarIndexRAZ < 9 ) 
		{
			MM10[sc.Index] = DernierSpreadMoyen ;
			MM60[sc.Index] = DernierSpreadMoyen ;
			MM300[sc.Index] = DernierSpreadMoyen ;
			SpreadMoyen[sc.Index] = DernierSpreadMoyen ;
			//SpreadMoyenArrondi[sc.Index] = SpreadMoyenArrondi[sc.Index-1] ;
		}
		
		
		
		if ( OpenTimeValue == HoraireFinSeance - 2 )  // dernier calcul de Spread
		{   
			s_sc* ptrsc = &sc ;  SCString texte ;  texte.Format("%.1f", SpreadMoyenArrondi[sc.Index] ) ; Drawtext(texte, sc.Index, SpreadMoyenArrondi[sc.Index]+0.6, RGB(0,0,0), 12, ptrsc, sc.GraphRegion) ;   
			DernierSpreadMoyen = SpreadMoyen[sc.Index] ; 
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_SpreadFutureIndexForIndex_MM300(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef Spread = sc.Subgraph[1]; 							// appelé par scsf_OverlayINQ100PlusSpread
	SCSubgraphRef SpreadMoyen = sc.Subgraph[5];                  	// appelé par scsf_SpreadFutureIndexForFuture et scsf_OverlayINQ100PlusSpread
	SCSubgraphRef SpreadMoyenArrondi = sc.Subgraph[6]; 
	
	SCInputRef ExternalChartNumber = sc.Input[0];
	
	int& IntradayBarIndexRAZ = sc.GetPersistentInt(1);  
	int& HoraireFinSeance = sc.GetPersistentInt(2) ;
	int& DernierIndex = sc.GetPersistentInt(3) ;


	if (sc.SetDefaults)
	{
		sc.GraphName = "Spread Future-Indice pour tracer sous l'indice" ; 	  // appelé par scsf_SpreadFutureIndexForFuture et scsf_OverlayINQ100PlusSpread
		//sc.GraphRegion = 0;  // 0 = graphe principal
		//sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 1;
		sc.AutoLoop = 0;  
		
		
		// Initialisation du ChartNumber pour récupérer le future
		ExternalChartNumber.Name = "ChartNumber du future en UT 1s";
		ExternalChartNumber.SetChartNumber(14);

		
		// paramètres des tracés
		for (int SubgraphIndex = 1; SubgraphIndex <= 6; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			//sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			//sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;  // attention, il faudra convertir les "vrais zéros" en 0.000001
		}
		
		Spread.Name = "Spread"; 
		Spread.LineWidth = 1;
		Spread.PrimaryColor = RGB(0, 200, 0);
		// Spread.DrawStyle = DRAWSTYLE_HIDDEN;
		
		SpreadMoyen.Name = "SpreadMoyen"; 
		SpreadMoyen.LineWidth = 1;
		SpreadMoyen.PrimaryColor = RGB(0, 0, 0);
		//SpreadMoyen.DrawStyle = DRAWSTYLE_HIDDEN;
		
		SpreadMoyenArrondi.Name = "SpreadMoyenArrondi"; 
		SpreadMoyenArrondi.LineWidth = 2;
		SpreadMoyenArrondi.PrimaryColor = RGB(0, 0, 0);
		SpreadMoyenArrondi.LineLabel =  LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER |  LL_VALUE_ALIGN_VALUES_SCALE; 
		// SpreadMoyenArrondi.DrawZeros = true;

		return ;	
	}  // fin de Set.Defaults
	
	
	if ( sc.UpdateStartIndex == 0 ) 
	{	
		// on vérifie que le graphe de l'index et le graphe du future sont bien en UT 1s
		n_ACSIL::s_BarPeriod BarPeriod;
		sc.GetBarPeriodParameters(BarPeriod);
		if ( BarPeriod.IntradayChartBarPeriodType != IBPT_DAYS_MINS_SECS   or  BarPeriod.IntradayChartBarPeriodParameter1 != 1  )   // on vérifie que le graphe est bien en UT 1s
		{  SCString message ;   message.Format("!! Erreur !! Le graphe #%d n'est pas en UT 1s", sc.ChartNumber) ;    sc.AddMessageToLog(message,1) ; }
		// vérification de l'UT du future : il n'y a pas de fonction similaire à sc.GetBarPeriodParameters() pour récupérer l'UT d'un autre graphe.
		
		IntradayBarIndexRAZ = 0 ;
		HoraireFinSeance = HMS_TIME(16,00,01);
	}
	
	
	// récupération des data du future et calcul du spread
	SCGraphData ExternalChartData; 
	sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);

	
	// bloc de looping manuel
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)      
	{
		// RAZ d'IntradayBarIndex lors du changement de jour
		if  (  sc.BaseDateTimeIn.TimeAt(index) < sc.BaseDateTimeIn.TimeAt(index-1)   )  
		{
			IntradayBarIndexRAZ = index ;
			SpreadMoyenArrondi[index-1] = SpreadMoyenArrondi[DernierIndex] ;
		}
		
		// Tracé de lignes verticales à t0 et 5min
		if ( index-IntradayBarIndexRAZ==0  or  index-IntradayBarIndexRAZ==299 )
		{ s_sc* ptrsc = &sc ;   DrawVLine(index, 2, RGB(0,0,0), ptrsc, sc.GraphRegion) ; }  

		// OpenTimeValue du chandelier actif
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		
		// Calcul du spread instantanné
		int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index dans le graphe du future    // int GetContainingIndexForDateTimeIndex(int ChartNumber, int DateTimeIndex); If the Date-Time at DateTimeIndex is after any Date-Time in the chart specified with ChartNumber , then the index of the last element is given which will be sc.ArraySize - 1 => ça veut dire que si le future n'a pas encore fait de tick dans la nouvelle seconde, au moment de calculer mon spread la valeur utilisée est le dernier tick connu du future => ce qui est parfait pour ma study "Spread Future-Indice"
		if ( ExternalChartIndex != 0  and  OpenTimeValue < HoraireFinSeance )
		{ Spread[index] = ExternalChartData[SC_OPEN][ExternalChartIndex] - sc.Open[index] ;  }
		
		// bloc de calcul de SpreadMoyen et SpreadMoyenArrondi
		if ( OpenTimeValue < HoraireFinSeance )  // n'est actualisé que pendant l'ouverture du marché action
		{
			int IntradayBarIndex = index-IntradayBarIndexRAZ + 1 ;    // IntradayBarIndex is a 1-based index
			
			// calcul de SpreadMoyen
			if  ( IntradayBarIndex >= 300 )
			{
				SpreadMoyen[index] = MoyenneMobileSimple(300, Spread, index) ;
			}
			else // on fait une pondération entre le dernier Spread connu (15h59m59s) et la MMx de période IntradayBarIndex : ça revient à calculer la MM300 en continu sans tenir compte des valeurs entre 16h00 et 9h30m15s.
			{
				float MMx = MoyenneMobileSimple(IntradayBarIndex, Spread, index) ;
				SpreadMoyen[index] = ( IntradayBarIndex * MMx  +  (300-IntradayBarIndex) * SpreadMoyen[DernierIndex] ) / 300 ;
			}
			
			// calcul de SpreadMoyenArrondi
			if  ( SpreadMoyen[index] > SpreadMoyenArrondi[index-1] + 0.5 )
			{	
				SpreadMoyenArrondi[index] = SpreadMoyenArrondi[index-1] + 0.5 ;
				s_sc* ptrsc = &sc ;  SCString texte ;  texte.Format("%.1f", SpreadMoyenArrondi[index] ) ; Drawtext(texte, index, SpreadMoyenArrondi[index]+0.6, RGB(0,0,0), 12, ptrsc, sc.GraphRegion) ;   
			}
			else if ( SpreadMoyen[index] < SpreadMoyenArrondi[index-1] - 0.5 )
			{	
				SpreadMoyenArrondi[index] = SpreadMoyenArrondi[index-1] - 0.5 ;
				s_sc* ptrsc = &sc ;  SCString texte ;  texte.Format("%.1f", SpreadMoyenArrondi[index] ) ; Drawtext(texte, index, SpreadMoyenArrondi[index]-0.6, RGB(0,0,0), 12, ptrsc, sc.GraphRegion) ;   
			}
			else
			{ 	SpreadMoyenArrondi[index] = SpreadMoyenArrondi[index-1] ;   }		
			
			// affichage du dernier SpreadMoyenArrondi à 15h59m59s et relevé du DernierIndex
			if ( OpenTimeValue == HoraireFinSeance - 2 )  
			{   
				s_sc* ptrsc = &sc ;  SCString texte ;  texte.Format("%.1f", SpreadMoyenArrondi[index] ) ; Drawtext(texte, index + 30, SpreadMoyenArrondi[index]+0.0, RGB(200,0,0), 16, ptrsc, sc.GraphRegion) ;   
				DernierIndex = index ;
			}
			

			if  ( SpreadMoyenArrondi[index] == 0 )  SpreadMoyenArrondi[index] = 0.00000000000000000000000000000001 ;			// car SpreadMoyenArrondi.DrawZeros = false;

			
		}
		
	} // fin manual looping

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_SpreadFutureIndexForFuture(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef Spread = sc.Subgraph[1]; 
	SCSubgraphRef SpreadMoyen = sc.Subgraph[5]; 
	SCSubgraphRef SpreadMoyenArrondi = sc.Subgraph[6]; 
	
	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef ChartInputsSpread = sc.Input[1]; 
	SCInputRef ChartInputsSpreadMoyen = sc.Input[5]; 
	SCInputRef ChartInputsSpreadMoyenArrondi = sc.Input[6]; 
	
	int& HoraireDebutSeance = sc.GetPersistentInt(1) ;  // time value en secondes
	int& HoraireFinSeance = sc.GetPersistentInt(2) ;

	if (sc.SetDefaults)
	{
		sc.GraphName = "Spread Future-Indice pour tracer sous le future"; 
		//sc.GraphRegion = 0;  // 0 = graphe principal
		//sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2 ;
		sc.AutoLoop = 0;  
		sc.AutoScalePaddingPercentage = 0.2; // A value of 0.4 means that padding takes up 40% of the chart, and the graph uses the rest of the 60%. 
		
		// Initialisation du ChartNumber pour récupérer le future
		ExternalChartNumber.Name = "ChartNumber du INQ100 en UT 1s";
		ExternalChartNumber.SetChartNumber(4);
		
		// Subgraphs parameters
		for (int SubgraphIndex = 5; SubgraphIndex <= 6; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			//sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = true ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
		}		
		
		Spread.Name = "Spread"; 
		Spread.DrawStyle = DRAWSTYLE_LINE ;
		Spread.LineWidth = 1;
		Spread.PrimaryColor = RGB(0,0,0);		
		SpreadMoyen.Name = "SpreadMoyen"; 
		SpreadMoyen.DrawStyle = DRAWSTYLE_LINE ;
		SpreadMoyen.LineWidth = 2;
		SpreadMoyen.PrimaryColor = RGB(0,0,255);		
		SpreadMoyen.LineLabel =  LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER |  LL_VALUE_ALIGN_VALUES_SCALE; 
		SpreadMoyenArrondi.Name = "SpreadMoyenArrondi"; 
		SpreadMoyenArrondi.DrawStyle = DRAWSTYLE_IGNORE ;
		SpreadMoyenArrondi.LineWidth = 2;
		SpreadMoyenArrondi.PrimaryColor = RGB(0,0,0);		


		
	return ;	
	}
	
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		SpreadMoyen[0] = 0 ;
		SpreadMoyenArrondi[0] = 0 ;
		HoraireDebutSeance = HMS_TIME(9,31,14);   // SpreadMoyenArrondi n'est pas retourné avant la première minute de cotation du NQ
		HoraireFinSeance = HMS_TIME(16,01,00);
		
		// Récupération de la studyID sur le graphe de l'indice
		int StudyID = sc.GetStudyIDByName(ExternalChartNumber.GetChartNumber(), "Spread Future-Indice pour tracer sous l'indice", 0) ;
		if  (StudyID == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le graphique source ne contient pas le spread Future-Indice") ; sc.AddMessageToLog(message,1) ; }
		else
		{
			// Création des ChartStudySubgraphValues pour Spread
			ChartInputsSpread.SetChartStudySubgraphValues(ExternalChartNumber.GetChartNumber(), StudyID, 1);
			// Création des ChartStudySubgraphValues pour SpreadMoyen
			ChartInputsSpreadMoyen.SetChartStudySubgraphValues(ExternalChartNumber.GetChartNumber(), StudyID, 5);
			// Création des ChartStudySubgraphValues pour SpreadMoyenArrondi
			ChartInputsSpreadMoyenArrondi.SetChartStudySubgraphValues(ExternalChartNumber.GetChartNumber(), StudyID, 6);
		}
	}
	

	
	// on récupère l'array de SpreadMoyen et de SpreadMoyenArrondi
	SCFloatArray SpreadExternalArray;
	sc.GetStudyArrayFromChartUsingID(ChartInputsSpread.GetChartStudySubgraphValues(), SpreadExternalArray);   //   void GetStudyArrayFromChartUsingID(const s_ChartStudySubgraphValues& ChartStudySubgraphValues, SCFloatArrayRef SubgraphArray);
	SCFloatArray SpreadMoyenExternalArray;
	sc.GetStudyArrayFromChartUsingID(ChartInputsSpreadMoyen.GetChartStudySubgraphValues(), SpreadMoyenExternalArray);   
	SCFloatArray SpreadMoyenArrondiExternalArray;
	sc.GetStudyArrayFromChartUsingID(ChartInputsSpreadMoyenArrondi.GetChartStudySubgraphValues(), SpreadMoyenArrondiExternalArray);  
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                               
	{
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		int PreviousOpenTimeValue = sc.BaseDateTimeIn[index-1].GetTimeInSecondsWithoutMilliseconds()  ; 
		
		if ( OpenTimeValue > HoraireDebutSeance and OpenTimeValue < HoraireFinSeance )  // n'est actualisé que pendant l'ouverture du marché action
		{
			if  ( OpenTimeValue  !=  PreviousOpenTimeValue )   //  n'est actualisé que s'il y a une nouvelle seconde 
			{ 
			// on récupère l'index sur le chart INQ100 en UT 1s
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); 
			Spread[index]            			= SpreadExternalArray[ExternalChartIndex] ;	
			SpreadMoyen[index]            	= SpreadMoyenExternalArray[ExternalChartIndex] ;	
			SpreadMoyenArrondi[index] 	= SpreadMoyenArrondiExternalArray[ExternalChartIndex] ;	
			}
			else
			{
			Spread[index] 						= Spread[index-1] ;	
			SpreadMoyen[index] 				= SpreadMoyen[index-1] ;	
			SpreadMoyenArrondi[index]	= SpreadMoyenArrondi[index-1] ;	
			}
		}			
		else
		{
			Spread[index] = 0 ;
			SpreadMoyen[index] = 0 ;
			SpreadMoyenArrondi[index] = 0 ;
		}
		
	}
	
}

