/* 
v1.0 (2019/7/30) : retourne le True Range et sa Moyenne Mobile Simple. Utilisation de sc.MovingAverage() => les résultats obtenus sont faux.
v1.1 (2019/7/30) : les valeurs de MM retournées sont correctes.
v1.2 (2019/7/30) : ajout de Envergure Moyenne Relative = Envergure Moyenne / Prix Moyen . Multiplié par 10000 pour être visible : une valeur de 100 signifie 1%.
v1.3 (2019/7/30) :  ajout du min et du max de Envergure Moyenne.
v1.4 (2019/7/31) : création de "EvergureMoyenne4000Ticks via 2000 ticks"
v1.5 (2019/12/11) : calcul des envergures moyennes sur un nb de jours donné (minimum 24h) car il y a une cyclicité de l'envergure moyenne (elle passe par un max quotidien après l'open du marché action). v1.5 : 1 jour, calcul de 18h à 18h.
v1.6 (2019/12/11) : modifs mineures.
v1.7 (2020/05/15) : ajout du Super True Range (STR) et du STR relatif.
v1.8 (2020/09/11) : création de "VIX moyen via 2000 ticks" par analogie avec "EvergureMoyenne4000Ticks via 2000 ticks".
v1.9 (2020/09/14) : study "TR 4000-ticks moyen (24 heures glissantes, via chart 2000-ticks)" : calcul de l'envergure moyenne sur le chart 2000 ticks sur 24h glissantes.
v2.0 (2020/09/14) : study "TR 4000-ticks moyen (24 heures glissantes, via chart 2000-ticks)" : on exclue la bougie de 18h, et les bougies entre 16h02 et 18h.
v2.1 (2020/09/15) : study "TR 4000-ticks moyen (5 seances glissantes, via chart 2000-ticks)" : tracer courbes sur 2 ou 3+ séances glissantes pour lisser, comparativement à la courbe sur 1 séance glissante.
v2.2 (2020/09/15) : study "TR 4000-ticks moyen (24 heures glissantes, via chart 2000-ticks)" : backgroundcolor en orange sur les jours fériés et les demi-journées de cotation.
v2.3 (2020/09/15) : study "TR 4000-ticks moyen (24 heures glissantes, via chart 2000-ticks)" : après un jour férié, on utilise la valeur de cloture de la séance n-2.
v2.4 (2020/09/15) : les noms des studies du DLL sont préfixées avec un numéro : [XX].
v2.5 (2020/09/16) : le nom du fichier .cpp (Envergure_moyenne_v2.5.cpp) est renommé (Range_TrueRange_v2.5.cpp). Le nom du DLL ("Envergure moyenne (TR Moyen)") est renommé ("Range et True Range").




[00] Range
[01] Range relatif (Range/Open)
[10] True Range
[11] True Range relatif (TR/PreviousClose)
[12] True Range moyen et relatif
[13] True Range (sauf 16h02 a 18hXX)
[14] True Range moyen de la seance precedente
[20] SuperTrueRange
[21] SuperTrueRange relatif (STR/Open)
[30] TR 4000-ticks moyen (SMA, via chart 2000-ticks)
[31] TR 4000-ticks moyen (seance precedente, via chart 2000-ticks)
[32] TR 4000-ticks moyen (1 seance glissante de 24h, via chart 2000-ticks)
[33] TR 4000-ticks moyen (5 seances glissantes, via chart 2000-ticks)
[90] VIX moyen (seance precedente, via chart 2000-ticks)



Améliorations à apporter : 
	- 

 */

#include "sierrachart.h"


SCDLLName("Range et True Range")

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_Range(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef Range = sc.Subgraph[1]; 
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[00] Range"; 
		// sc.GraphRegion = 1;  // 0 = graphe principal
		sc.ValueFormat = 0;
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 1;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		Range.Name = "Range (pts)"; 
		Range.LineWidth = 3;
		Range.PrimaryColor = RGB(0,0,0);   
		Range.DrawStyle = DRAWSTYLE_BAR;
		//Range.LineStyle = LINESTYLE_SOLID ;
		Range.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		Range.DrawZeros = false;

		return ;	
	}
	
	
	
	if ( sc.Index == 0 )
	{
		sc.ValueFormat = 0;
	}
	
	Range[sc.Index] = sc.BaseDataIn[SC_HIGH][sc.Index] - sc.BaseDataIn[SC_LOW][sc.Index] ;
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_RangeRelatif(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef RangeRelatif = sc.Subgraph[1]; 
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[01] Range relatif (Range/Open)"; 
		// sc.GraphRegion = 1;  // 0 = graphe principal
		sc.ValueFormat = 0;
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 1;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		RangeRelatif.Name = "Range relatif (pdm)"; 
		RangeRelatif.LineWidth = 3;
		RangeRelatif.PrimaryColor = RGB(0,0,0);   
		RangeRelatif.DrawStyle = DRAWSTYLE_BAR;
		//RangeRelatif.LineStyle = LINESTYLE_SOLID ;
		RangeRelatif.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		RangeRelatif.DrawZeros = false;

		return ;	
	}
	
	
	
	if ( sc.Index == 0 )
	{
		sc.ValueFormat = 0;
	}
	
	RangeRelatif[sc.Index] = ( sc.BaseDataIn[SC_HIGH][sc.Index] - sc.BaseDataIn[SC_LOW][sc.Index] ) / sc.BaseDataIn[SC_OPEN][sc.Index] *10000 ;  // exprimé en pdm
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_TrueRange(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef TrueRange = sc.Subgraph[1]; 
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[10] True Range"; 
		// sc.GraphRegion = 1;  // 0 = graphe principal
		sc.ValueFormat = 0;
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 1;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		TrueRange.Name = "True Range (pts)"; 
		TrueRange.LineWidth = 3;
		TrueRange.PrimaryColor = RGB(0,0,0);   
		TrueRange.DrawStyle = DRAWSTYLE_BAR;
		//TrueRange.LineStyle = LINESTYLE_SOLID ;
		TrueRange.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		TrueRange.DrawZeros = false;

		return ;	
	}
	
	
	
	if ( sc.Index == 0 )
	{
		sc.ValueFormat = 0;
	}
	
	TrueRange[sc.Index] = sc.GetTrueRange(sc.BaseDataIn) ;
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_TrueRange_Relatif(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef TrueRange = sc.Subgraph[1]; 
	SCSubgraphRef TrueRangeRelatif = sc.Subgraph[2]; 
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[11] True Range relatif (TR/PreviousClose)"; 
		// sc.GraphRegion = 1;  // 0 = graphe principal
		sc.ValueFormat = 0;
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 1;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		//TrueRange.Name = "True Range"; 
		TrueRange.LineWidth = 3;
		TrueRange.PrimaryColor = RGB(0,0,0);   
		TrueRange.DrawStyle = DRAWSTYLE_BAR;
		//TrueRange.LineStyle = LINESTYLE_SOLID ;
		TrueRange.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		TrueRange.DrawZeros = false;

		TrueRangeRelatif.Name = "TR relatif (pdm)"; 
		TrueRangeRelatif.LineWidth = 3;
		TrueRangeRelatif.PrimaryColor = RGB(0,0,255);   
		TrueRangeRelatif.DrawStyle = DRAWSTYLE_BAR;
		// TrueRangeRelatif.LineStyle = LINESTYLE_SOLID ;
		TrueRangeRelatif.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		TrueRangeRelatif.DrawZeros = false;

		return ;	
	}
	
	
	
	if ( sc.Index == 0 )
	{
		sc.ValueFormat = 0;
	}
	
	TrueRange[sc.Index] = sc.GetTrueRange(sc.BaseDataIn) ;
	
	
	// Calcul de TrueRangeRelatif
	if ( sc.Index > 0 )
	{
		TrueRangeRelatif[sc.Index] = TrueRange[sc.Index] / sc.BaseDataIn[SC_LAST][sc.Index-1] *10000 ;  // exprimé en pdm
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_TrueRange_Moyen(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef TrueRange = sc.Subgraph[1]; 
	SCSubgraphRef EnvMoy = sc.Subgraph[2]; 
	SCSubgraphRef EnvMoyRel = sc.Subgraph[3]; 
	SCSubgraphRef EnvMoyMin = sc.Subgraph[4]; 
	SCSubgraphRef EnvMoyMax = sc.Subgraph[5]; 
	
	SCInputRef PeriodeMM = sc.Input[1];  
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[12] True Range moyen et relatif"; 
		sc.GraphRegion = 1;  // 0 = graphe principal
		sc.ValueFormat = 2;
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 1;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		PeriodeMM.Name = "Periode MM";
		PeriodeMM.SetInt(1000);	
		
		
		//TrueRange.Name = "True Range"; 
		TrueRange.LineWidth = 3;
		TrueRange.PrimaryColor = RGB(0,0,0);   
		TrueRange.DrawStyle = DRAWSTYLE_BAR;
		//TrueRange.LineStyle = LINESTYLE_SOLID ;
		TrueRange.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		TrueRange.DrawZeros = false;

		EnvMoy.Name = "TR Moyen"; 
		EnvMoy.LineWidth = 3;
		EnvMoy.PrimaryColor = RGB(255,0,0);   
		EnvMoy.DrawStyle = DRAWSTYLE_LINE;
		EnvMoy.LineStyle = LINESTYLE_SOLID ;
		EnvMoy.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoy.DrawZeros = false;

		EnvMoyRel.Name = "TR Moyen Relatif (pdm)"; 
		EnvMoyRel.LineWidth = 2;
		EnvMoyRel.PrimaryColor = RGB(0,200,0);   
		EnvMoyRel.DrawStyle = DRAWSTYLE_LINE;
		EnvMoyRel.LineStyle = LINESTYLE_SOLID ;
		EnvMoyRel.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoyRel.DrawZeros = false;
		
		// EnvMoyMin.Name = "TR Moyen [Min]"; 
		EnvMoyMin.LineWidth = 1;
		EnvMoyMin.PrimaryColor = RGB(255,0,0);   
		EnvMoyMin.DrawStyle = DRAWSTYLE_LINE;
		EnvMoyMin.LineStyle = LINESTYLE_SOLID ;
		EnvMoyMin.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoyMin.DrawZeros = false;

		// EnvMoyMax.Name = "TR Moyen [Max]"; 
		EnvMoyMax.LineWidth = 1;
		EnvMoyMax.PrimaryColor = RGB(255,0,0);   
		EnvMoyMax.DrawStyle = DRAWSTYLE_LINE;
		EnvMoyMax.LineStyle = LINESTYLE_SOLID ;
		EnvMoyMax.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoyMax.DrawZeros = false;
		
		return ;	
	}
	
	
	TrueRange[sc.Index] = sc.GetTrueRange(sc.BaseDataIn) ;
	int Periode =  PeriodeMM.GetInt() ;
	
	
	// Calcul de la MM simple de EnvMoy
	double Somme = 0 ;
	if ( sc.Index > Periode )
	{
		for ( int i = 0 ; i < Periode ; ++i )
		{
			Somme += TrueRange[sc.Index-i] ;
		}
	}
	EnvMoy[sc.Index] = Somme / Periode ;
	
	// Calcul du PrixMoyen
	Somme = 0 ;
	if ( sc.Index > Periode )
	{
		for ( int i = 0 ; i < Periode ; ++i )
		{
			Somme += sc.BaseDataIn[SC_LAST][sc.Index-i] ;
		}
	}
	float PrixMoyen =  Somme / Periode ;
	
	// Calcul de EnvMoyeRel
	if ( sc.Index > Periode )
	{
		EnvMoyRel[sc.Index] = EnvMoy[sc.Index] / PrixMoyen * 10000 ;   // multiplié par 10000 pour être visible : une valeur de 100 signifie 1%.
	}

	// RAZ de EnvMoyMin et EnvMoyMax
	if ( sc.Index== Periode + 1 )  
	{
		EnvMoyMin[Periode + 1] = EnvMoy[Periode + 1] ;
		EnvMoyMax[Periode + 1] = EnvMoy[Periode + 1] ;
	}
	
	// Calcul de EnvMoyMin et EnvMoyMax
	if ( sc.Index > Periode+1 )
	{
		if ( EnvMoy[sc.Index] > EnvMoyMax [sc.Index - 1] )    EnvMoyMax [sc.Index] = EnvMoy[sc.Index] ;
		else EnvMoyMax [sc.Index] = EnvMoyMax[sc.Index-1] ;
		
		if ( EnvMoy[sc.Index] < EnvMoyMin [sc.Index - 1] )    EnvMoyMin [sc.Index] = EnvMoy[sc.Index] ;
		else EnvMoyMin [sc.Index] = EnvMoyMin[sc.Index-1] ;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_TrueRange_Moyen_1Seance(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef TrueRange = sc.Subgraph[1]; 
	SCSubgraphRef EnvMoy = sc.Subgraph[2]; 
	SCSubgraphRef EnvMoyRel = sc.Subgraph[3]; 
	
	
	
	int& BarIndexRAZ = sc.GetPersistentInt(1);  
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[13] True Range moyen de la seance precedente"; 
		sc.GraphRegion = 1;  // 0 = graphe principal
		sc.ValueFormat = 0;
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 1;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		//TrueRange.Name = "True Range"; 
		TrueRange.LineWidth = 3;
		TrueRange.PrimaryColor = RGB(0,0,0);   
		TrueRange.DrawStyle = DRAWSTYLE_BAR;
		//TrueRange.LineStyle = LINESTYLE_SOLID ;
		TrueRange.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		TrueRange.DrawZeros = false;

		EnvMoy.Name = "TR Moyen"; 
		EnvMoy.LineWidth = 3;
		EnvMoy.PrimaryColor = RGB(255,0,0);   
		EnvMoy.DrawStyle = DRAWSTYLE_LINE;
		EnvMoy.LineStyle = LINESTYLE_SOLID ;
		EnvMoy.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoy.DrawZeros = false;

		EnvMoyRel.Name = "TR Moyen Relatif (pdm)"; 
		EnvMoyRel.LineWidth = 2;
		EnvMoyRel.PrimaryColor = RGB(0,200,0);   
		EnvMoyRel.DrawStyle = DRAWSTYLE_LINE;
		EnvMoyRel.LineStyle = LINESTYLE_SOLID ;
		EnvMoyRel.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoyRel.DrawZeros = false;
		
		return ;	
	}
	
	
	if ( sc.Index == 0 )
	{
		EnvMoy[sc.Index] = 0 ;
		EnvMoyRel[sc.Index] = 0 ;
		BarIndexRAZ = 0 ;
	}	
	
	
	TrueRange[sc.Index] = sc.GetTrueRange(sc.BaseDataIn) ;
	
	
	// RAZ des paramètres lors d'un changement de séance  // 64800 est le nb de secondes à 18h   
	if  (  sc.BaseDateTimeIn.TimeAt(sc.Index) >= 64800    and   sc.BaseDateTimeIn.TimeAt(sc.Index-1)< 64800   )   
	{
		
		// Calcul de la MM simple de EnvMoy
		double Somme = 0 ;
		if ( sc.Index > 0 )
		{
			for ( int i = BarIndexRAZ ; i < sc.Index-1 ; ++i )  // on parcoure les index de la séance précédente (qui débute à BarIndexRAZ et se termine à sc.Index-1)    // on ne somme pas le chandelier à sc.Index-1 car il est incomplet (il manque des ticks)
			{
				Somme += TrueRange[i] ;
			}
		}
		int Periode = sc.Index - 1  - BarIndexRAZ ;
		EnvMoy[sc.Index] = Somme / Periode ;
		
		// Calcul du PrixMoyen
		Somme = 0 ;
		if ( sc.Index > 0 )
		{
			for ( int i = BarIndexRAZ ; i < sc.Index-1 ; ++i )  // on parcoure les index de la séance précédente (qui débute à BarIndexRAZ et se termine à sc.Index-1)
			{
				Somme += sc.BaseDataIn[SC_LAST][i] ;
			}
		}
		float PrixMoyen =  Somme / Periode ;
		
		// Calcul de EnvMoyeRel
		if ( sc.Index > 0 )
		{
			EnvMoyRel[sc.Index] = EnvMoy[sc.Index] / PrixMoyen * 10000 ;   // résultat exprimé en pdm
		}
		
		// RAZ pour la nouvelle séance qui vient de débuter
		BarIndexRAZ = sc.Index ;
	}
	else
	{ 
		EnvMoy[sc.Index] = EnvMoy[sc.Index-1] ;
		EnvMoyRel[sc.Index] = EnvMoyRel[sc.Index-1] ;
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_TrueRange_SaufFinEtDebutSeance(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef TrueRange = sc.Subgraph[1]; 
	
	SCInputRef CourbeOpposee = sc.Input[1];  
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[14] True Range (sauf 16h02 a 18hXX)"; 
		// sc.GraphRegion = 1;  // 0 = graphe principal
		sc.ValueFormat = 0;
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 1;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		TrueRange.Name = "True Range (pts)"; 
		TrueRange.LineWidth = 1;
		TrueRange.PrimaryColor = RGB(0,0,0);   
		TrueRange.DrawStyle = DRAWSTYLE_BAR;
		//TrueRange.LineStyle = LINESTYLE_SOLID ;
		TrueRange.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		TrueRange.DrawZeros = false;
		
		CourbeOpposee.Name = "Courbe opposee (multipliee par -1)" ;
		CourbeOpposee.SetYesNo(1) ;
	
		return ;	
	}
	
	
	
	if ( sc.Index == 0 )
	{
		sc.ValueFormat = 0;
	}


	if ( sc.BaseDateTimeIn.TimeAt(sc.Index) < 57720  or  sc.BaseDateTimeIn.TimeAt(sc.Index) > 64800 )			// exclusion entre 16h02 et première bougie de l'open
	{
		TrueRange[sc.Index] = sc.GetTrueRange(sc.BaseDataIn) ;
		if ( CourbeOpposee.GetYesNo() )  TrueRange[sc.Index] = - TrueRange[sc.Index] ;
	}
	else
	{
		TrueRange[sc.Index] = 0 ;
	}
	
	
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_SuperTrueRange(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef SuperTrueRange = sc.Subgraph[1]; 
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[20] SuperTrueRange"; 
		// sc.GraphRegion = 1;  // 0 = graphe principal
		sc.ValueFormat = 0;
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 1;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		SuperTrueRange.Name = "SuperTrueRange (pts)"; 
		SuperTrueRange.LineWidth = 3;
		SuperTrueRange.PrimaryColor = RGB(0,0,0);   
		SuperTrueRange.DrawStyle = DRAWSTYLE_BAR;
		//SuperTrueRange.LineStyle = LINESTYLE_SOLID ;
		SuperTrueRange.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		SuperTrueRange.DrawZeros = false;

		return ;	
	}
	
	
	if ( sc.Index == 0 )
	{
		sc.ValueFormat = 0;
	}
	
	if ( sc.Index != sc.ArraySize -1 )
	{
		float Range = sc.BaseDataIn[SC_HIGH][sc.Index] - sc.BaseDataIn[SC_LOW][sc.Index] ;
		float Range_GapUp = sc.BaseDataIn[SC_OPEN][sc.Index+1] - sc.BaseDataIn[SC_LOW][sc.Index] ;
		float Range_GapDown = sc.BaseDataIn[SC_HIGH][sc.Index] - sc.BaseDataIn[SC_OPEN][sc.Index+1] ;
		SuperTrueRange[sc.Index] = fmax ( Range , fmax(Range_GapUp, Range_GapDown) ) ;
	}
	else
	{
		SuperTrueRange[sc.Index] = 0 ;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_SuperTrueRangeRelatif(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef 	SuperTrueRangeRelatif 	= sc.Subgraph[1]; 	
	SCFloatArray		SuperTrueRange			= sc.Subgraph[1].Arrays[0]; 	// appelé par Amplitude
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[21] SuperTrueRange relatif (STR/Open)"; 		// appelé par Amplitude
		// sc.GraphRegion = 1;  // 0 = graphe principal
		sc.ValueFormat = 0;
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 1;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		SuperTrueRangeRelatif.Name = "STR relatif (pdm)"; 
		SuperTrueRangeRelatif.LineWidth = 3;
		SuperTrueRangeRelatif.PrimaryColor = RGB(0,0,0);   
		SuperTrueRangeRelatif.DrawStyle = DRAWSTYLE_BAR;
		//SuperTrueRangeRelatif.LineStyle = LINESTYLE_SOLID ;
		SuperTrueRangeRelatif.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		SuperTrueRangeRelatif.DrawZeros = false;

		return ;	
	}
	
	if ( sc.Index == 0 )
	{
		sc.ValueFormat = 0;
	}
	
	if ( sc.Index != sc.ArraySize -1 )
	{
		float Range = sc.BaseDataIn[SC_HIGH][sc.Index] - sc.BaseDataIn[SC_LOW][sc.Index] ;
		float Range_GapUp = sc.BaseDataIn[SC_OPEN][sc.Index+1] - sc.BaseDataIn[SC_LOW][sc.Index] ;
		float Range_GapDown = sc.BaseDataIn[SC_HIGH][sc.Index] - sc.BaseDataIn[SC_OPEN][sc.Index+1] ;
		SuperTrueRange[sc.Index] = fmax ( Range , fmax(Range_GapUp, Range_GapDown) ) ;
		SuperTrueRangeRelatif[sc.Index] = SuperTrueRange[sc.Index] / sc.BaseDataIn[SC_OPEN][sc.Index] *10000 ;  // exprimé en pdm
	}
	else
	{
		SuperTrueRangeRelatif[sc.Index] = 0 ;
	}	
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_TR_Moyen_4000Ticks_SMA_Via_2000Ticks(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef TrueRange = sc.Subgraph[1]; 
	SCSubgraphRef EnvMoy = sc.Subgraph[2]; 
	
	SCInputRef PeriodeMM = sc.Input[1];  
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[30] TR 4000-ticks moyen (SMA, via chart 2000-ticks)"; 
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		PeriodeMM.Name = "Periode MM";
		PeriodeMM.SetInt(1000);	
		

		EnvMoy.Name = "TR moyen 4000-ticks (SMA)"; 
		EnvMoy.LineWidth = 2;
		EnvMoy.PrimaryColor = RGB(0,0,0);   
		EnvMoy.DrawStyle = DRAWSTYLE_LINE;
		EnvMoy.LineStyle = LINESTYLE_SOLID ;
		EnvMoy.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoy.DrawZeros = false;


		return ;	
	}
	
	if ( sc.Index == 0 )
	{
		sc.GraphRegion = 0;
		sc.ValueFormat = 0;
	}
	
	
	TrueRange[sc.Index] = sc.GetTrueRange(sc.BaseDataIn) ;
	int Periode =  PeriodeMM.GetInt() ;
	
	
	// Calcul de la MM simple de EnvMoy
	double Somme = 0 ;
	if ( sc.Index > Periode )
	{
		for ( int i = 0 ; i < Periode ; ++i )
		{
			Somme += TrueRange[sc.Index-i] ;
		}
	}
	EnvMoy[sc.Index] = Somme / Periode * 1.4142 ;  // on multiplie par sqrt(2) pour avoir l'envergure sur le 4000-ticks
	

	

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_TR_Moyen_4000Ticks_Via_2000Ticks_SeancePrecedente(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef TrueRange = sc.Subgraph[1]; 
	SCSubgraphRef EnvMoy = sc.Subgraph[2];      		// appelé par scsf_Sigma     // appelé par scsf_Sigma_x512x256x128x64x32x16
	
	
	
	int& BarIndexRAZ = sc.GetPersistentIntFast(1);  // premier chandelier de la séance
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[31] TR 4000-ticks moyen (seance precedente, via chart 2000-ticks)";     		// appelé par scsf_Sigma     // appelé par scsf_Sigma_x512x256x128x64x32x16
		sc.GraphRegion = 0;  // 0 = graphe principal
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		//TrueRange.Name = "True Range"; 
		TrueRange.LineWidth = 3;
		TrueRange.PrimaryColor = RGB(0,0,0);   
		TrueRange.DrawStyle = DRAWSTYLE_BAR;
		//TrueRange.LineStyle = LINESTYLE_SOLID ;
		TrueRange.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		TrueRange.DrawZeros = false;

		EnvMoy.Name = "TR 4000-ticks moyen de la seance precedente"; 
		EnvMoy.LineWidth = 3;
		EnvMoy.PrimaryColor = RGB(0,0,0);   
		EnvMoy.DrawStyle = DRAWSTYLE_LINE;
		EnvMoy.LineStyle = LINESTYLE_SOLID ;
		EnvMoy.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoy.DrawZeros = false;

		return ;	
	}
	
	
	if ( sc.Index == 0 )
	{
		sc.ValueFormat = 1;
		EnvMoy[sc.Index] = 0 ;
		BarIndexRAZ = 0 ;
	}	
	
	
	TrueRange[sc.Index] = sc.GetTrueRange(sc.BaseDataIn) ;   // The sc.GetTrueRange() function calculates the True Range of a bar at sc.Index (Auto-looping only). The result is returned as a single float value.
	
	
	// RAZ des paramètres lors d'un changement de séance  // 64800 est le nb de secondes à 18h   
	if  (  sc.BaseDateTimeIn.TimeAt(sc.Index) >= 64800    and   sc.BaseDateTimeIn.TimeAt(sc.Index-1)< 64800   )   
	{
		
		// Calcul de la MM simple de TR
		double Somme = 0 ;
		if ( sc.Index > 0 )
		{
			for ( int i = BarIndexRAZ ; i < sc.Index ; ++i )  // on parcoure les index de la séance précédente (qui débute à BarIndexRAZ et se termine à sc.Index-1)    // on ne somme pas le chandelier à sc.Index car il est incomplet (il manque des ticks)
			{
				Somme += TrueRange[i] ;
			}
		}
		int Periode = sc.Index - BarIndexRAZ ;
		EnvMoy[sc.Index] = Somme / Periode * 1.4142 ;  // on multiplie par sqrt(2) pour avoir l'envergure sur le 4000-ticks
		
		
		// RAZ pour la nouvelle séance qui vient de débuter
		BarIndexRAZ = sc.Index ;
	}
	else
	{ 
		EnvMoy[sc.Index] = EnvMoy[sc.Index-1] ;
	}
	
	

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_TR_Moyen_4000Ticks_Via_2000Ticks_SeanceGlissante(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef Background = sc.Subgraph[0] ;  		
	SCSubgraphRef TrueRange = sc.Subgraph[1]; 
	SCSubgraphRef EnvMoy_1SeanceGlissante= sc.Subgraph[2];     	// appelé par scsf_Sigma     // appelé par scsf_Sigma_x512x256x128x64x32x16
	
	int& SeanceApresUneSeanceIncomplete = sc.GetPersistentIntFast(1);  // booléen pour identifier les séances apès les jours fériés et demi-fériés
	int& BarIndexOpenSeance = sc.GetPersistentIntFast(2);  // index du premier chandelier de la séance
	
	
	float& TR_ClotureSeanceComplete = sc.GetPersistentFloatFast(1);  		// valeur (constante) retenue pour TR après une séance incomplète
	
	
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[32] TR 4000-ticks moyen (1 seance glissante de 24h, via chart 2000-ticks)";  			// appelé par scsf_Sigma     // appelé par scsf_Sigma_x512x256x128x64x32x16
		sc.GraphRegion = 0;  // 0 = graphe principal
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		Background.Name = "Subgraph background"; 
		Background.DrawStyle = DRAWSTYLE_BACKGROUND; 
		Background.DisplayNameValueInDataLine = false ;		// display the Subgraph Name and Value in the Region Data Line of the Chart Region the study is displayed in

		
		
		//TrueRange.Name = "True Range"; 
		TrueRange.LineWidth = 3;
		TrueRange.PrimaryColor = RGB(0,0,0);   
		TrueRange.DrawStyle = DRAWSTYLE_BAR;
		//TrueRange.LineStyle = LINESTYLE_SOLID ;
		TrueRange.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		TrueRange.DrawZeros = false;

		EnvMoy_1SeanceGlissante.Name = "TR 4000-ticks moyen (1 seance glissante)"; 
		EnvMoy_1SeanceGlissante.LineWidth = 2;
		EnvMoy_1SeanceGlissante.PrimaryColor = RGB(0,0,0);   
		EnvMoy_1SeanceGlissante.DrawStyle = DRAWSTYLE_LINE;
		EnvMoy_1SeanceGlissante.LineStyle = LINESTYLE_SOLID ;
		EnvMoy_1SeanceGlissante.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoy_1SeanceGlissante.DrawZeros = false;

		return ;	
	}
	
	
	if ( sc.Index == 0 )
	{
		sc.ValueFormat = 1;
		sc.GraphRegion = 0;  
		SeanceApresUneSeanceIncomplete = false ;
		BarIndexOpenSeance = 0 ;
		TR_ClotureSeanceComplete = 0 ;
	}	
	
	
	// Background par défaut
	Background[sc.Index] = 0 ;
	
	// Vérification si la séance écoulée était un jour férié ou une demi-journée de cotation.
	if  (  sc.BaseDateTimeIn.TimeAt(sc.Index) >= 64800  and  sc.BaseDateTimeIn.TimeAt(sc.Index-1)< 64800   )   		 // changement de séance		// 64800 est le nb de secondes à 18h   
	{
		// Note : les jours fériés (marché action fermé), la cotation des futures s'arrête à 13h (EST). Lors des demi-journées de cotation sur marché action (close à 13h), la cotation des futures s'arrête à 13h15.
		// Si le chandelier à sc.Index-1 débute avant 14h, alors il est très probable qu'on avait à faire à une journée de ce type (férié ou demi-journée).
		if ( sc.BaseDateTimeIn.TimeAt(sc.Index-1)< 50400 )	 		// 50400 = 14*3600
		{ 
			SeanceApresUneSeanceIncomplete = true ;			// à sc.Index on vient de débuter une séance après une séance incomplète
			TR_ClotureSeanceComplete = EnvMoy_1SeanceGlissante[BarIndexOpenSeance-1] ; 		// BarIndexOpenSeance est l'index de début de la séance incomplète
			// Gestion de Background pour la séance écoulée
			for ( int i = BarIndexOpenSeance ; i < sc.Index ; ++ i )
			{ Background[i] = true ;  Background.DataColor[i] = RGB(255,153,51) ;  }
		}
		else SeanceApresUneSeanceIncomplete = false ;
		
		// MàJ de BarIndexOpenSeance
		BarIndexOpenSeance = sc.Index ;
	}


	// On récupère TrueRange
	TrueRange[sc.Index] = sc.GetTrueRange(sc.BaseDataIn) ;   // The sc.GetTrueRange() function calculates the True Range of a bar at sc.Index (Auto-looping only). The result is returned as a single float value.

	
	
	if ( SeanceApresUneSeanceIncomplete ) EnvMoy_1SeanceGlissante[sc.Index]  = TR_ClotureSeanceComplete ;
	else
	{
		
		// Trouver BarIndexRAZ (premier chandelier de la séance glissante de 24h)
		int CurrentIndex_Day = sc.BaseDateTimeIn[sc.Index].GetDayOfWeek() ;
		int BarIndexRAZ_Date ;
				if ( CurrentIndex_Day == SUNDAY )  																						{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 3 ;  }		// dimanche soir : le début de la période glissante de 24h débute le jeudi soir
		else	if ( CurrentIndex_Day == MONDAY  and  sc.BaseDateTimeIn.TimeAt(sc.Index) < 64800 )  	{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 3 ;  }  	// lundi avant 18h : le début de la période glissante de 24h débute le vendredi
		else 																																				{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 1 ;  }		// entre lundi 18h et vendredi
		int BarIndexRAZ_Time = sc.BaseDateTimeIn[sc.Index].GetTimeInSeconds() ;
		SCDateTime BarIndexRAZ_SCDateTime = SCDateTime(BarIndexRAZ_Date, BarIndexRAZ_Time) ; 			// SCDateTime(int DateValue, int TimeValue);
		int BarIndexRAZ = sc.GetContainingIndexForSCDateTime(sc.ChartNumber, BarIndexRAZ_SCDateTime);			// int GetContainingIndexForSCDateTime(int ChartNumber, SCDateTime DateTime);

		// if ( sc.Index == sc.ArraySize-2 )  { SCString message ;  message.Format("BarIndexRAZ_2 = %d", BarIndexRAZ_2) ;  sc.AddMessageToLog(message, 0) ;  }
		
		// Calcul de TR moyen sur 24h glissantes
		double Somme = 0 ;
		int NbBougies = 0 ;
		if ( sc.Index > 100 )
		{
			for ( int i = BarIndexRAZ ; i < sc.Index ; ++i )  // on parcoure les index de la séance glissante (qui débute à BarIndexRAZ et se termine à sc.Index-1)    // on ne somme pas le chandelier à sc.Index car il est incomplet (il manque des ticks)
			{
				if ( sc.BaseDateTimeIn.TimeAt(i) < 57720  or  sc.BaseDateTimeIn.TimeAt(i) > 64800 )		// exclusion entre 16h02 et la première bougie de 18h
				{
					Somme += TrueRange[i] ;
					++ NbBougies ;
				}
			}
		}
		EnvMoy_1SeanceGlissante[sc.Index] = Somme / NbBougies * 1.4142 ;  // on multiplie par sqrt(2) pour avoir l'envergure sur le 4000-ticks
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_TR_Moyen_4000Ticks_Via_2000Ticks_CinqSeancesGlissantes(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef TrueRange = sc.Subgraph[1]; 
	SCSubgraphRef EnvMoy_1SeanceGlissante= sc.Subgraph[2];     
	SCSubgraphRef EnvMoy_2SeancesGlissantes= sc.Subgraph[3];     
	SCSubgraphRef EnvMoy_3SeancesGlissantes= sc.Subgraph[4];     
	SCSubgraphRef EnvMoy_4SeancesGlissantes= sc.Subgraph[5];     
	SCSubgraphRef EnvMoy_5SeancesGlissantes= sc.Subgraph[6];     
	SCSubgraphRef EnvMoy_SommeDes5EnvMoy= sc.Subgraph[7];     
	
	
	SCInputRef CourbeOpposee = sc.Input[1];  
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[33] TR 4000-ticks moyen (5 seances glissantes, via chart 2000-ticks)";  
		sc.GraphRegion = 0;  // 0 = graphe principal
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		//TrueRange.Name = "True Range"; 
		TrueRange.LineWidth = 3;
		TrueRange.PrimaryColor = RGB(0,0,0);   
		TrueRange.DrawStyle = DRAWSTYLE_BAR;
		//TrueRange.LineStyle = LINESTYLE_SOLID ;
		TrueRange.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		TrueRange.DrawZeros = false;

		EnvMoy_1SeanceGlissante.Name = "TR 4000-ticks moyen (1 seance glissante)"; 
		EnvMoy_1SeanceGlissante.LineWidth = 2;
		EnvMoy_1SeanceGlissante.PrimaryColor = RGB(0,0,0);   
		EnvMoy_1SeanceGlissante.DrawStyle = DRAWSTYLE_LINE;
		EnvMoy_1SeanceGlissante.LineStyle = LINESTYLE_SOLID ;
		EnvMoy_1SeanceGlissante.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoy_1SeanceGlissante.DrawZeros = false;

		EnvMoy_2SeancesGlissantes.Name = "TR 4000-ticks moyen (2 seances glissantes)"; 
		EnvMoy_2SeancesGlissantes.LineWidth = 2;
		EnvMoy_2SeancesGlissantes.PrimaryColor = RGB(200,0,0);   
		EnvMoy_2SeancesGlissantes.DrawStyle = DRAWSTYLE_LINE;
		EnvMoy_2SeancesGlissantes.LineStyle = LINESTYLE_SOLID ;
		EnvMoy_2SeancesGlissantes.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoy_2SeancesGlissantes.DrawZeros = false;

		EnvMoy_3SeancesGlissantes.Name = "TR 4000-ticks moyen (3 seances glissantes)"; 
		EnvMoy_3SeancesGlissantes.LineWidth = 2;
		EnvMoy_3SeancesGlissantes.PrimaryColor = RGB(0,200,0);   
		EnvMoy_3SeancesGlissantes.DrawStyle = DRAWSTYLE_LINE;
		EnvMoy_3SeancesGlissantes.LineStyle = LINESTYLE_SOLID ;
		EnvMoy_3SeancesGlissantes.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoy_3SeancesGlissantes.DrawZeros = false;

		EnvMoy_4SeancesGlissantes.Name = "TR 4000-ticks moyen (4 seances glissantes)"; 
		EnvMoy_4SeancesGlissantes.LineWidth = 2;
		EnvMoy_4SeancesGlissantes.PrimaryColor = RGB(0,0,200);   
		EnvMoy_4SeancesGlissantes.DrawStyle = DRAWSTYLE_LINE;
		EnvMoy_4SeancesGlissantes.LineStyle = LINESTYLE_SOLID ;
		EnvMoy_4SeancesGlissantes.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoy_4SeancesGlissantes.DrawZeros = false;

		EnvMoy_5SeancesGlissantes.Name = "TR 4000-ticks moyen (5 seances glissantes)"; 
		EnvMoy_5SeancesGlissantes.LineWidth = 2;
		EnvMoy_5SeancesGlissantes.PrimaryColor = RGB(255,0,127);   
		EnvMoy_5SeancesGlissantes.DrawStyle = DRAWSTYLE_LINE;
		EnvMoy_5SeancesGlissantes.LineStyle = LINESTYLE_SOLID ;
		EnvMoy_5SeancesGlissantes.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoy_5SeancesGlissantes.DrawZeros = false;

		EnvMoy_SommeDes5EnvMoy.Name = "TR 4000-ticks moyen (moyenne des 5 moyennes)"; 
		EnvMoy_SommeDes5EnvMoy.LineWidth = 3;
		EnvMoy_SommeDes5EnvMoy.PrimaryColor = RGB(255,128,0);   
		EnvMoy_SommeDes5EnvMoy.DrawStyle = DRAWSTYLE_LINE;
		EnvMoy_SommeDes5EnvMoy.LineStyle = LINESTYLE_SOLID ;
		EnvMoy_SommeDes5EnvMoy.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		EnvMoy_SommeDes5EnvMoy.DrawZeros = false;
		

		CourbeOpposee.Name = "Courbe opposee (multipliee par -1)" ;
		CourbeOpposee.SetYesNo(1) ;
		
		return ;	
	}
	
	
	if ( sc.Index == 0 )
	{
		sc.ValueFormat = 5;
		// sc.GraphRegion = 3;  
	}	
	
	
	
	// Vérification si la séance écoulée était un jour férié	  // 64800 est le nb de secondes à 18h   
	if  (  sc.BaseDateTimeIn.TimeAt(sc.Index) >= 64800  and   sc.BaseDateTimeIn.TimeAt(sc.Index-1)< 64800   )   
	{
		// Vérification : si nb de barres est inférieur à XXX
		
		
		// Vérification qu'on n'a pas à faire à une demi-séance (cloture à 13h)
		
	}

	
	
	// On récupère TrueRange
	TrueRange[sc.Index] = sc.GetTrueRange(sc.BaseDataIn) ;   // The sc.GetTrueRange() function calculates the True Range of a bar at sc.Index (Auto-looping only). The result is returned as a single float value.
	
	
	
	
	// Trouver BarIndexRAZ (premier chandelier de la séance glissante de 24h)
	int CurrentIndex_Day = sc.BaseDateTimeIn[sc.Index].GetDayOfWeek() ;
	int BarIndexRAZ_Date ;
			if ( CurrentIndex_Day == SUNDAY )  																						{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 3 ;  }		// dimanche soir : le début de la période glissante de 24h débute le jeudi soir
	else	if ( CurrentIndex_Day == MONDAY  and  sc.BaseDateTimeIn.TimeAt(sc.Index) < 64800 )  	{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 3 ;  }  	// lundi avant 18h : le début de la période glissante de 24h débute le vendredi
	else 																																				{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 1 ;  }		// entre lundi 18h et vendredi
	int BarIndexRAZ_Time = sc.BaseDateTimeIn[sc.Index].GetTimeInSeconds() ;
	SCDateTime BarIndexRAZ_SCDateTime = SCDateTime(BarIndexRAZ_Date, BarIndexRAZ_Time) ; 			// SCDateTime(int DateValue, int TimeValue);
	int BarIndexRAZ = sc.GetContainingIndexForSCDateTime(sc.ChartNumber, BarIndexRAZ_SCDateTime);			// int GetContainingIndexForSCDateTime(int ChartNumber, SCDateTime DateTime);

	// Trouver BarIndexRAZ_2 (premier chandelier de la séance glissante de 48h)
			if ( CurrentIndex_Day == SUNDAY )  																						{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 4 ;  }		// dimanche soir : le début de la période glissante de 48h débute le mercredi soir
	else	if ( CurrentIndex_Day == MONDAY )  																					{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 4 ;  }  	// lundi : le début de la période glissante de 48h débute le jeudi
	else	if ( CurrentIndex_Day == TUESDAY  and  sc.BaseDateTimeIn.TimeAt(sc.Index) < 64800 ) 	{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 4 ;  }  	// mardi avant 18h : le début de la période glissante de 48h débute le vendredi
	else 																																				{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 2 ;  }		// entre mardi 18h et vendredi
	BarIndexRAZ_SCDateTime = SCDateTime(BarIndexRAZ_Date, BarIndexRAZ_Time) ; 	
	int BarIndexRAZ_2 = sc.GetContainingIndexForSCDateTime(sc.ChartNumber, BarIndexRAZ_SCDateTime);		

	// Trouver BarIndexRAZ_3 (premier chandelier de la séance glissante de 72h)
			if ( CurrentIndex_Day == SUNDAY )  																								{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 5 ;  }		// dimanche soir : le début de la période glissante de 72h débute le mardi soir
	else	if ( CurrentIndex_Day == MONDAY )  																							{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 5 ;  }  	
	else	if ( CurrentIndex_Day == TUESDAY ) 																							{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 5 ;  }  	
	else	if ( CurrentIndex_Day == WEDNESDAY  and  sc.BaseDateTimeIn.TimeAt(sc.Index) < 64800 )	{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 5 ;  }  	// mercredi avant 18h : le début de la période glissante de 72h débute le vendredi
	else 																																						{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 3 ;  }		// entre mercredi 18h et vendredi
	BarIndexRAZ_SCDateTime = SCDateTime(BarIndexRAZ_Date, BarIndexRAZ_Time) ; 	
	int BarIndexRAZ_3 = sc.GetContainingIndexForSCDateTime(sc.ChartNumber, BarIndexRAZ_SCDateTime);		

	// Trouver BarIndexRAZ_4 (premier chandelier de la séance glissante de 96h)
			if ( CurrentIndex_Day == SUNDAY )  																								{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 6 ;  }		// dimanche soir : le début de la période glissante de 72h débute le lundi soir
	else	if ( CurrentIndex_Day == MONDAY )  																							{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 6 ;  }  	
	else	if ( CurrentIndex_Day == TUESDAY ) 																							{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 6 ;  }  	
	else	if ( CurrentIndex_Day == WEDNESDAY   )																						{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 6 ;  }  	
	else	if ( CurrentIndex_Day == THURSDAY  and  sc.BaseDateTimeIn.TimeAt(sc.Index) < 64800 )		{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 6 ;  }  	
	else 																																						{ BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 5 ;  }		// entre jeudi 18h et vendredi
	BarIndexRAZ_SCDateTime = SCDateTime(BarIndexRAZ_Date, BarIndexRAZ_Time) ; 	
	int BarIndexRAZ_4 = sc.GetContainingIndexForSCDateTime(sc.ChartNumber, BarIndexRAZ_SCDateTime);			

	// Trouver BarIndexRAZ_5 (premier chandelier de la séance glissante de 120h (une semaine))
	BarIndexRAZ_Date = sc.BaseDateTimeIn[sc.Index].GetDate() - 7 ;
	BarIndexRAZ_SCDateTime = SCDateTime(BarIndexRAZ_Date, BarIndexRAZ_Time) ; 	
	int BarIndexRAZ_5 = sc.GetContainingIndexForSCDateTime(sc.ChartNumber, BarIndexRAZ_SCDateTime);			


	
	// if ( sc.Index == sc.ArraySize-2 )  { SCString message ;  message.Format("BarIndexRAZ_2 = %d", BarIndexRAZ_2) ;  sc.AddMessageToLog(message, 0) ;  }
	
	
	// Calcul de TR moyen sur 24h glissantes
	double Somme = 0 ;
	int NbBougies = 0 ;
	if ( sc.Index > 100 )
	{
		for ( int i = BarIndexRAZ ; i < sc.Index ; ++i )  // on parcoure les index de la séance glissante (qui débute à BarIndexRAZ et se termine à sc.Index-1)    // on ne somme pas le chandelier à sc.Index car il est incomplet (il manque des ticks)
		{
			if ( sc.BaseDateTimeIn.TimeAt(i) < 57720  or  sc.BaseDateTimeIn.TimeAt(i) > 64800 )		// exclusion entre 16h02 et la première bougie de 18h
			{
				Somme += TrueRange[i] ;
				++ NbBougies ;
			}
		}
	}
	EnvMoy_1SeanceGlissante[sc.Index] = Somme / NbBougies * 1.4142 ;  // on multiplie par sqrt(2) pour avoir l'envergure sur le 4000-ticks
	
	// Calcul de la MM simple de TR sur 48h glissantes : on reprend les datas déjà calculées sur 24h, et on les complète
	if ( sc.Index > 100 )
	{
		for ( int i = BarIndexRAZ_2 ; i < BarIndexRAZ ; ++i )
		{
			if ( sc.BaseDateTimeIn.TimeAt(i) < 57720  or  sc.BaseDateTimeIn.TimeAt(i) > 64800 )
			{
				Somme += TrueRange[i] ;
				++ NbBougies ;
			}
		}
	}
	EnvMoy_2SeancesGlissantes[sc.Index] = Somme / NbBougies * 1.4142 ; 
	

	// Calcul de la MM simple de TR sur 72h glissantes 
	if ( sc.Index > 100 )
	{
		for ( int i = BarIndexRAZ_3 ; i < BarIndexRAZ_2 ; ++i )  
		{
			if ( sc.BaseDateTimeIn.TimeAt(i) < 57720  or  sc.BaseDateTimeIn.TimeAt(i) > 64800 )
			{
				Somme += TrueRange[i] ;
				++ NbBougies ;
			}
		}
	}
	EnvMoy_3SeancesGlissantes[sc.Index] = Somme / NbBougies * 1.4142 ; 

	// Calcul de la MM simple de TR sur 96h glissantes 
	if ( sc.Index > 100 )
	{
		for ( int i = BarIndexRAZ_4 ; i < BarIndexRAZ_3 ; ++i ) 
		{
			if ( sc.BaseDateTimeIn.TimeAt(i) < 57720  or  sc.BaseDateTimeIn.TimeAt(i) > 64800 )
			{
				Somme += TrueRange[i] ;
				++ NbBougies ;
			}
		}
	}
	EnvMoy_4SeancesGlissantes[sc.Index] = Somme / NbBougies * 1.4142 ; 


	// Calcul de la MM simple de TR sur 120h glissantes
	if ( sc.Index > 100 )
	{
		for ( int i = BarIndexRAZ_5 ; i < BarIndexRAZ_4 ; ++i ) 
		{
			if ( sc.BaseDateTimeIn.TimeAt(i) < 57720  or  sc.BaseDateTimeIn.TimeAt(i) > 64800 )
			{
				Somme += TrueRange[i] ;
				++ NbBougies ;
			}
		}
	}
	EnvMoy_5SeancesGlissantes[sc.Index] = Somme / NbBougies * 1.4142 ; 
	
	EnvMoy_SommeDes5EnvMoy[sc.Index] = ( EnvMoy_1SeanceGlissante[sc.Index] + EnvMoy_2SeancesGlissantes[sc.Index] + EnvMoy_3SeancesGlissantes[sc.Index] + EnvMoy_4SeancesGlissantes[sc.Index] + EnvMoy_5SeancesGlissantes[sc.Index] ) / 5 ; 
	

	
	if ( CourbeOpposee.GetYesNo() ) 
	{
		EnvMoy_1SeanceGlissante[sc.Index] 	= - EnvMoy_1SeanceGlissante[sc.Index] ;
		EnvMoy_2SeancesGlissantes[sc.Index]	= - EnvMoy_2SeancesGlissantes[sc.Index] ;
		EnvMoy_3SeancesGlissantes[sc.Index]	= - EnvMoy_3SeancesGlissantes[sc.Index] ;
		EnvMoy_4SeancesGlissantes[sc.Index]	= - EnvMoy_4SeancesGlissantes[sc.Index] ;
		EnvMoy_5SeancesGlissantes[sc.Index]	= - EnvMoy_5SeancesGlissantes[sc.Index] ;
		EnvMoy_SommeDes5EnvMoy[sc.Index] = - EnvMoy_SommeDes5EnvMoy[sc.Index] ;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_VIX_Moyen_Via_2000Ticks_1Seance(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef VIX_Moyen = sc.Subgraph[1]; 
	SCSubgraphRef VIX = sc.Subgraph[2]; 
	
	
	
	int& BarIndexRAZ = sc.GetPersistentIntFast(1);  
	int& StudyID_VIX = sc.GetPersistentIntFast(2);  	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[90] VIX moyen (seance precedente, via chart 2000-ticks)";   
		sc.GraphRegion = 0;  // 0 = graphe principal
		sc.AutoLoop = 1;  	
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		VIX_Moyen.Name = "VIX moyen de la seance precedente"; 
		VIX_Moyen.LineWidth = 2;
		VIX_Moyen.PrimaryColor = RGB(100,100,100);   
		VIX_Moyen.DrawStyle = DRAWSTYLE_LINE;
		VIX_Moyen.LineStyle = LINESTYLE_SOLID ;
		VIX_Moyen.LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
		VIX_Moyen.DrawZeros = false;
		

		// Récupération de la StudyID pour le VIX
		StudyID_VIX = max ( sc.GetStudyIDByName(sc.ChartNumber, "Symbol: $vix", 0), sc.GetStudyIDByName(sc.ChartNumber, "Symbol: $VIX", 0) ) ;
		if ( StudyID_VIX == 0 )   { SCString message ;  message.Format("!! ERREUR !! Le chart #%d ne contient pas la study ''Symbol: $vix''", sc.ChartNumber) ;  sc.AddMessageToLog(message, 1) ;  }

		return ;	
	}
	
	
	if ( sc.Index == 0 )
	{
		VIX_Moyen[sc.Index] = 0 ;
		BarIndexRAZ = 0 ;
		sc.ValueFormat = 1;

	}	

	
	// on récupère le VIX_Array
	SCFloatArray VIX_Array ;
	sc.GetStudyArrayUsingID(StudyID_VIX, 3, VIX_Array) ;
	VIX[sc.Index] = VIX_Array[sc.Index] ;


	// RAZ des paramètres lors d'un changement de séance  // 64800 est le nb de secondes à 18h   
	if  (  sc.BaseDateTimeIn.TimeAt(sc.Index) >= 64800    and   sc.BaseDateTimeIn.TimeAt(sc.Index-1)< 64800   )   
	{
		// Calcul de la MM simple de TR
		double Somme = 0 ;
		if ( sc.Index > 0 )
		{
			for ( int i = BarIndexRAZ ; i < sc.Index-1 ; ++i )  // on parcoure les index de la séance précédente (qui débute à BarIndexRAZ et se termine à sc.Index-1)    // on ne somme pas le chandelier à sc.Index-1 car il est incomplet (il manque des ticks)
			{
				Somme += VIX[i] ;
			}
		}
		int Periode = sc.Index - 1  - BarIndexRAZ ;
		VIX_Moyen[sc.Index] = Somme / Periode ;
		
		
		// RAZ pour la nouvelle séance qui vient de débuter
		BarIndexRAZ = sc.Index ;
	}
	else
	{ 
		VIX_Moyen[sc.Index] = VIX_Moyen[sc.Index-1] ;
	}
	
	

}

