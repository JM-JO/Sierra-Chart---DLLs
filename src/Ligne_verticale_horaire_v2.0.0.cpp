/* 
v1.0 (2019/2/11) : l'indicateur "Ligne verticale horaire" trace une barre verticale à Horaire.
v1.1 (2019/2/11) : l'indicateur "Zone verticale horaire" grise le background entre Horaire1 et Horaire2.
v1.2 (2019/3/1) : ajout d'un package d'indicateurs pour le future : indicateur all-in-one qui rassemble les différents horaires-clés sur future : 18h00, 9h30, 16h00, 16h30, zone 15h54-15h55.
v1.3 (2019/4/2) : ajout d'un package pour le 200-ticks (9h30 et 18h00) et le 2000-ticks (1er jour de la semaine et 1er jour du mois). Ajout d'un package pour l'indice en 15s (9h30) et en 2 min (1er jour de la semaine et 1er jour du mois).
v1.3.1 (2019/4/4) : ajout de lignes jour sur tous les graphes.      (2019/9/28) : ajout de sc.UpdateAlways quand le marché est fermé pour les graphes 200/2000 ticks.
v1.4.0 (2019/10/7) : création d'un indicateur qui trace une ligne verticale si la barre fait partie d'une nouvelle seconde (utile pour le 1-tick).
v2.0.0 (2019/10/7) : les lignes verticales tracées sur indice deviennent des tracés vectoriels.



Améliorations à apporter :
	- ne pas chercher à améliorer le Chart Drawing Time de scsf_PackageHorairesFuture16ticks en supprimant le subgraph background : le gain ne serait que de 1ms.
	
	
SOMMAIRE DES STRUCTURES ET FONCTIONS :
SegmentVertical
DrawVLine
scsf_LigneVerticale
scsf_ZoneVerticale
scsf_PackageHorairesFuture16ticks
scsf_PackageHorairesFuture200ticks
scsf_PackageHorairesFuture2000ticks
scsf_PackageHorairesIndiceLigneJour
scsf_PackageHorairesIndiceLignesJourHebdoEtMensuelle
scsf_LigneVerticaleAChaqueSeconde
 */

 
 
#include "sierrachart.h"


SCDLLName("Lignes/zones verticales horaire-dependant")

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
			BeginValue = -100000 ;
			EndIndex = index ;
			EndValue = 100000 ;
			LineWidth = epaisseur;
			Color = maCouleur; 
			Region = Graphregion ;   // 0 par défaut
			DrawUnderneathMainGraph = true ;
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

struct SegmentVerticalPointille : s_UseTool 
	{
		SegmentVerticalPointille(int index, int epaisseur, COLORREF maCouleur, int Graphregion) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
		{
		Clear(); 
		DrawingType = DRAWING_VERTICALLINE;             // DRAWING_VERTICALLINE est aussi rapide que DRAWING_LINE pour le Chart Drawing Time    // Par contre DRAWING_VERTICALLINE déborde sur les autres graphes.
		LineStyle = LINESTYLE_DOT;                                 // l'ajout de " LineStyle = LINESTYLE_DOT " rend le Chart Drawing Time bcp plus lent
		AddMethod = UTAM_ADD_ALWAYS;	                      
		BeginIndex = index;
		LineWidth = epaisseur;
		Color = maCouleur; 
		Region = Graphregion ;   // 0 par défaut
		} 
	};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawSegmentVerticalPointille (int index, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc, int Graphregion)
{
	//Création d'instance de SegmentVerticalPointille et appel de sc.UseTool
	SegmentVerticalPointille objetSegmentVertical(index, monEpaisseur, maCouleur, Graphregion);
	ptrsc->UseTool(objetSegmentVertical); // c'est sc.UseTool qui est appelé
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_LigneVerticale(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef LigneHoraire = sc.Subgraph[0]; 
	
	SCInputRef Heure = sc.Input[0];  
	SCInputRef Minute = sc.Input[1];  
	SCInputRef Seconde = sc.Input[2]; 


	if (sc.SetDefaults)
	{
		sc.GraphName = "Ligne verticale horaire"; 
		sc.GraphRegion = 0;  // 0 = graphe principal
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		//sc.ValueFormat = 2;
		sc.AutoLoop = 1;  
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;


		
		Heure.Name = "Heure";
		Heure.SetInt(16);
		Minute.Name = "Minute";
		Minute.SetInt(15);
		Seconde.Name = "Seconde";
		Seconde.SetInt(0);
		
		LigneHoraire.Name = "Ligne verticale"; 
		LigneHoraire.LineWidth = 2;
		LigneHoraire.PrimaryColor = RGB(0,0,0);		
				
		for (int SubgraphIndex = 0; SubgraphIndex <= 0; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_DOT ;
			//sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			//sc.Subgraph[SubgraphIndex].DrawZeros = true ;
		}
		
	return ;	
	}
	
	//Horaire.SetTimeHMS(iHeure, iMinute, iSeconde);  // Horaire au format SCDateTime
	int Horaire =  HMS_TIME(Heure.GetInt(), Minute.GetInt(), Seconde.GetInt()) ;   // HMS_TIME() constructs a Time Value from the given Hour, Minute, and Second components.

	if ( sc.Index== 0 )  
	{
		LigneHoraire[sc.Index] = 100000 ;
	}

	// Premier chandelier après Horaire
	if  (   ( sc.BaseDateTimeIn[sc.Index].GetTimeInSeconds()  >=  Horaire  )   and   (   sc.BaseDateTimeIn[sc.Index-1].GetTimeInSeconds()  <  Horaire    )   )   
	{	LigneHoraire[sc.Index] = - LigneHoraire[sc.Index-1] ; }
	else  
	{ 	LigneHoraire[sc.Index] = LigneHoraire[sc.Index-1] ;}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_ZoneVerticale(SCStudyInterfaceRef sc) 
{
	
	SCInputRef Heure1 = sc.Input[0];  
	SCInputRef Minute1 = sc.Input[1];  
	SCInputRef Seconde1 = sc.Input[2]; 
	SCInputRef Heure2 = sc.Input[3];  
	SCInputRef Minute2 = sc.Input[4];  
	SCInputRef Seconde2 = sc.Input[5]; 
	

	if (sc.SetDefaults)
	{
		sc.GraphName = "Zone verticale horaire"; 
		sc.GraphRegion = 0;  // 0 = graphe principal
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		//sc.ValueFormat = 2;
		sc.AutoLoop = 1;  
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;



		
		Heure1.Name = "Heure1";
		Heure1.SetInt(15);
		Minute1.Name = "Minute1";
		Minute1.SetInt(54);
		Seconde1.Name = "Seconde1";
		Seconde1.SetInt(0);
		Heure2.Name = "Heure2";
		Heure2.SetInt(15);
		Minute2.Name = "Minute2";
		Minute2.SetInt(55);
		Seconde2.Name = "Seconde2";
		Seconde2.SetInt(2);
		
		sc.Subgraph[0].Name = "Subgraph background"; 
		sc.Subgraph[0].DrawStyle = DRAWSTYLE_BACKGROUND; 
		sc.Subgraph[0].PrimaryColor = RGB(255,0,255);
		sc.Subgraph[0].DisplayNameValueInWindowsFlags = false ;
		
	return ;	
	}
	
	int Horaire1 =  HMS_TIME(Heure1.GetInt(), Minute1.GetInt(), Seconde1.GetInt()) ;   // HMS_TIME() constructs a Time Value from the given Hour, Minute1, and Second components.
	int Horaire2 =  HMS_TIME(Heure2.GetInt(), Minute2.GetInt(), Seconde2.GetInt()) ;   
	
	// Premier chandelier après Horaire
	if  (   ( sc.BaseDateTimeIn[sc.Index].GetTimeInSeconds()  >=  Horaire1  )   and   (   sc.BaseDateTimeIn[sc.Index].GetTimeInSeconds()  <  Horaire2    )   )      // TIME_PART() returns the time part of a SCDateTime value given by SCDateTimeVariable. The value returned is a Time Value.
	{	
		sc.Subgraph[0][sc.Index] = 1 ; 
		sc.Subgraph[0].DataColor[sc.Index]  = RGB(210,210,210) ;
	}
	else  
	{
		sc.Subgraph[0][sc.Index] = 0 ; 
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_PackageHorairesFuture16ticks(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef Background = sc.Subgraph[0]; 
	SCSubgraphRef TraceLigne = sc.Subgraph[1]; 
	
	int& Horaire180000 = sc.GetPersistentInt(1) ;  // time value en secondes
	int& Horaire082500 = sc.GetPersistentInt(2) ;  // time value en secondes
	int& Horaire083010 = sc.GetPersistentInt(3) ;  // time value en secondes
	int& Horaire093000 = sc.GetPersistentInt(4) ;  // time value en secondes
	int& Horaire160000 = sc.GetPersistentInt(5) ;  // time value en secondes
	int& Horaire161500 = sc.GetPersistentInt(6) ;  // time value en secondes
	int& Horaire155400 = sc.GetPersistentInt(7) ;  // time value en secondes
	int& Horaire155502 = sc.GetPersistentInt(8) ;  // time value en secondes
	int& FirstIndex = sc.GetPersistentInt(9);  
	int& LastIndex = sc.GetPersistentInt(10);  
	
	s_sc* ptrsc = &sc ; 
	
	COLORREF CouleurNoire = RGB(0,0,0) ;
	

	if (sc.SetDefaults)
	{
		sc.GraphName = "Package d'horaires pour le future en 16-ticks"; 
		sc.GraphRegion = 1;
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		//sc.ValueFormat = 2;
		sc.AutoLoop = 0;  
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;

		Background.Name = "Subgraph background"; 
		Background.DrawStyle = DRAWSTYLE_BACKGROUND; 
		Background.PrimaryColor = RGB(255,0,255);
		Background.DisplayNameValueInWindowsFlags = false ;

		TraceLigne.DrawStyle = DRAWSTYLE_HIDDEN ;
		TraceLigne.DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre

		return ;	
	}
	
	
	if (sc.UpdateStartIndex == 0)  
	{
		sc.GraphRegion = 1;
		
		Horaire180000 = HMS_TIME(18, 0, 0) ;   // HMS_TIME() constructs a Time Value from the given Hour, Minute, and Second components.
		Horaire082500 = HMS_TIME( 8,25, 0) ;   
		Horaire083010 = HMS_TIME( 8,30,10) ;   
		Horaire093000 = HMS_TIME( 9,30, 0) ;   
		Horaire160000 = HMS_TIME(16, 0, 0) ;   
		Horaire161500 = HMS_TIME(16,15, 0) ;   
		// Horaire155400 = HMS_TIME(15,54, 0) ;   
		// Horaire155502 = HMS_TIME(15,55, 2) ;   
		Horaire155400 = HMS_TIME(15,44,50) ;   
		Horaire155502 = HMS_TIME(16, 0, 0) ;   
		FirstIndex = 0 ;  LastIndex = 0 ;  // RAZ nécessaire pour que les tracés soient réaffichés après leur effacement lors d'une FullRecalculation
	}
	
	
	// Manual Looping
	for ( int index = sc.UpdateStartIndex ;  index < sc.ArraySize ;  index++ )                 // début looping manuel
	{
		int Horaire = sc.BaseDateTimeIn[index].GetTimeInSeconds() ;
		int HorairePrior = sc.BaseDateTimeIn[index-1].GetTimeInSeconds() ;
		
		// Zones grises
		if  (        ( Horaire  >=  Horaire155400  )   and   (   Horaire  <  Horaire155502    )  
			   or   ( Horaire  >=  Horaire082500  )   and   (   Horaire  <  Horaire083010    )   )      // Premier chandelier après Horaire  // TIME_PART() returns the time part of a SCDateTime value given by SCDateTimeVariable. The value returned is a Time Value.
		{	
			Background[index] = 1 ; 
			Background.DataColor[index]  = RGB(210,210,210) ;
		}
		else  
		{
			Background[index] = 0 ; 
		}

		// Lignes verticales
		if  (			( Horaire  >=  Horaire180000 )   and   ( HorairePrior  <  Horaire180000 )
				or 	( Horaire  >=  Horaire093000 )   and   ( HorairePrior  <  Horaire093000 )
				or 	( Horaire  >=  Horaire160000 )   and   ( HorairePrior  <  Horaire160000 )
				or 	( Horaire  >=  Horaire161500 )   and   ( HorairePrior  <  Horaire161500 )     )  
		{	TraceLigne[index] = 2 ; }
		else  
		{ 	TraceLigne[index] = 0 ;}
	}
	
	
	// Bloc d'affichage 
	if ( FirstIndex != sc.IndexOfFirstVisibleBar  or  LastIndex != sc.IndexOfLastVisibleBar )
	{
		FirstIndex = sc.IndexOfFirstVisibleBar ;
		LastIndex = sc.IndexOfLastVisibleBar ;
		
		// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
		
		// tracés de traits verticaux sur la fenêtre
		int NbIndexVisibles = LastIndex - FirstIndex + 1 ;
		for ( int index = FirstIndex - NbIndexVisibles ; index < min(LastIndex + NbIndexVisibles, sc.ArraySize-1) ; index++ )         // std::max ne fonctionne pas (ne fonctionne que sur des float) ; sans doute lié à une surcharge spécifique à Sierra
		{
			if ( TraceLigne[index] != 0 )
			{
				DrawSegmentVerticalPointille(index, TraceLigne[index], CouleurNoire, ptrsc, sc.GraphRegion) ; 
			}
		} // fin manual looping
		
		// lorsque je suis déconnecté d'IB, pour les backtests :
		if ( sc.ServerConnectionState != SCS_CONNECTED )  
		{	sc.UpdateAlways = 1 ;  }

	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_PackageHorairesFuture200ticks(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef TraceLigne = sc.Subgraph[0]; 
	
	s_sc* ptrsc = &sc ; 
	
	COLORREF CouleurNoire = RGB(0,0,0) ;
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "Package d'horaires pour le future en 200-ticks"; 
		sc.GraphRegion = 0;
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		//sc.ValueFormat = 2;
		sc.AutoLoop = 0;  
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;

		sc.HideStudy = 1 ;

		TraceLigne.DrawStyle = DRAWSTYLE_HIDDEN ;
		TraceLigne.DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
		
		return ;	
	}
	
	

	// AutoLoop
	for ( int index = sc.UpdateStartIndex ;  index < sc.ArraySize ;  index++ )                 // début looping manuel
	{
		if  (			( sc.BaseDateTimeIn[index].GetTimeInSeconds()  >=  64800 )   and   ( sc.BaseDateTimeIn[index-1].GetTimeInSeconds()  <  64800 )
				or 	( sc.BaseDateTimeIn[index].GetTimeInSeconds()  >=  34200 )   and   ( sc.BaseDateTimeIn[index-1].GetTimeInSeconds()  < 34200 )      )         // 34200 = 9h30
		{	TraceLigne[index-1] = 1 ; }
		else  
		{ 	TraceLigne[index-1] = 0 ;  }
	}
	
	
	// Bloc d'affichage 
	if ( sc.ChartWindowIsActive )
	{
		// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
		
		// tracés de traits verticaux sur la fenêtre
		int NbIndexVisibles = sc.IndexOfLastVisibleBar - sc.IndexOfFirstVisibleBar + 1 ;
		for ( int index = sc.IndexOfFirstVisibleBar - NbIndexVisibles ; index < min(sc.IndexOfLastVisibleBar + NbIndexVisibles, sc.ArraySize-1) ; index++ )         // std::max ne fonctionne pas (ne fonctionne que sur des float) ; sans doute lié à une surcharge spécifique à Sierra
		{
			if ( TraceLigne[index] != 0 )
			{
				DrawSegmentVerticalPointille(index, TraceLigne[index], CouleurNoire, ptrsc, sc.GraphRegion) ; 
			}
		} // fin manual looping
		
		// lorsque je suis déconnecté d'IB, pour les backtests :
		if ( sc.ServerConnectionState != SCS_CONNECTED )  
		{	sc.UpdateAlways = 1 ;  }

	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_PackageHorairesFuture2000ticks(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef TraceLigne = sc.Subgraph[0]; // vaut 0 si pas de ligne tracée, 2 pour une ligne jour, 4 pour hebdo, 6 pour mensuelle (c'est l'épaisseur de la ligne)
	
	s_sc* ptrsc = &sc ; 
	
	COLORREF CouleurNoire = RGB(0,0,0) ;
	

	if (sc.SetDefaults)
	{
		sc.GraphName = "Package d'horaires pour le future en 2000-ticks"; 
		sc.GraphRegion = 1;
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.AutoLoop = 0;  
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;

		sc.HideStudy = 1 ;

		TraceLigne.DrawStyle = DRAWSTYLE_HIDDEN ;
		TraceLigne.DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre

		return ;	
	}
	
	
	
	// AutoLoop
	for ( int index = sc.UpdateStartIndex ;  index < sc.ArraySize ;  index++ )                 // début looping manuel
	{
		// Changement de séance (indispensable pour le changement de mois)
		if (  sc.BaseDateTimeIn.TimeAt(index) == 64800    and   sc.BaseDateTimeIn.TimeAt(index-1)!= 64800      // 64800 est le nb de secondes à 18h
				/* and   (  not  (   ( Year==2019 and Month==2 and Day==18)  or  ( Year==2019 and Month==1 and Day==21)   )   )  */  )   // exclusion des journées sans settlement
		{
			// Changement de séance
			TraceLigne[index-1] = 1 ;
			
			// Changement de semaine 
			if (  sc.BaseDateTimeIn[index].GetDayOfWeek()   <   sc.BaseDateTimeIn[index-1].GetDayOfWeek()  )  
			{	TraceLigne[index-1] = 4 ;  }
			
			// Changement de mois :   ( on change de séance càd il est 18h)  ET   (  (on est le dernier jour du mois) OU ( le mois a changé entre deux chandeliers successifs càd on est le WE )  )
			int Year, Month, Day;
			sc.BaseDateTimeIn[index].GetDateYMD(Year, Month, Day);
			if (    (  Month==1 and Day==31 )  or (  Month==2 and Day==28 )  or (  Month==3 and Day==31 )  or (  Month==4 and Day==30 )  or (  Month==5 and Day==31 )  or (  Month==6 and Day==30 )  or (  Month==7 and Day==31 )  or 
			(  Month==8 and Day==31 )  or (  Month==9 and Day==30 )  or (  Month==10 and Day==31 )  or (  Month==11 and Day==30 )  or (  Month==1 and Day==31 )  or (  sc.BaseDateTimeIn[index].GetMonth()  !=   sc.BaseDateTimeIn[index-1].GetMonth()  )   )
			{	TraceLigne[index-1] = 6 ; }
		}
		else  // on ne change pas de séance
		{	TraceLigne[index-1] = 0 ;  }		
		
	}
	
	
	// Bloc d'affichage 
	if ( sc.ChartWindowIsActive )
	{
		// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
		
		// tracés de traits verticaux sur la fenêtre
		int NbIndexVisibles = sc.IndexOfLastVisibleBar - sc.IndexOfFirstVisibleBar + 1 ;
		for ( int index = sc.IndexOfFirstVisibleBar - NbIndexVisibles ; index < min(sc.IndexOfLastVisibleBar + NbIndexVisibles, sc.ArraySize-1) ; index++ )         // std::max ne fonctionne pas (ne fonctionne que sur des float) ; sans doute lié à une surcharge spécifique à Sierra
		{
			if ( TraceLigne[index] != 0 )
			{
				DrawSegmentVerticalPointille(index, TraceLigne[index], CouleurNoire, ptrsc, sc.GraphRegion) ; 
			}
		} // fin manual looping
		
		// lorsque je suis déconnecté d'IB, pour les backtests :
		if ( sc.ServerConnectionState != SCS_CONNECTED )  
		{	sc.UpdateAlways = 1 ;  }

	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_PackageHorairesIndiceLignesJourHebdoEtMensuelle(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef TraceLigne = sc.Subgraph[0];    // vaut 0 si pas de ligne tracée, 2 pour une ligne jour, 4 pour hebdo, 6 pour mensuelle (c'est l'épaisseur de la ligne)

	int& FirstIndex = sc.GetPersistentInt(0);  
	int& LastIndex = sc.GetPersistentInt(1);  
	
	int Year, Month, Day, MonthPriorIndex, DayPriorIndex ;
	
	s_sc* ptrsc = &sc ; 
	COLORREF CouleurNoire = RGB(0,0,0) ;
	

	if (sc.SetDefaults)
	{
		sc.GraphName = "Package d'horaires pour l'indice (lignes jour, hebdo et mensuelle)"; 
		sc.GraphRegion = 0;
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		//sc.ValueFormat = 2;
		sc.AutoLoop = 0; 
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		sc.HideStudy = 1 ;

		TraceLigne.DrawStyle = DRAWSTYLE_HIDDEN ;
		TraceLigne.DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre

		return ;	
	}
	
	
	if (sc.UpdateStartIndex == 0)  
	{
		FirstIndex = 0 ;  LastIndex = 0 ;  // RAZ nécessaire pour que les tracés soient réaffichés après leur effacement lors d'une FullRecalculation
	}	

	
	// AutoLoop
	for ( int index = sc.UpdateStartIndex ;  index < sc.ArraySize ;  index++ )                 // début looping manuel
	{
		// Changement de jour ou de mois
		sc.BaseDateTimeIn[index].GetDateYMD(Year, Month, Day);
		sc.BaseDateTimeIn[index-1].GetDateYMD(Year, MonthPriorIndex, DayPriorIndex);
		
		if ( Day != DayPriorIndex )    // On cherche d'abord un changement de jour, puis de semaine, puis de mois.
		{	
			// Changement de jour
			TraceLigne[index-1] = 2 ;  
			// Changement de semaine 
			if (  sc.BaseDateTimeIn[index].GetDayOfWeek()   <   sc.BaseDateTimeIn[index-1].GetDayOfWeek()  )  {	TraceLigne[index-1] = 4 ; }
			// Changement de mois
			if ( Month != MonthPriorIndex )  {	TraceLigne[index-1] = 6 ;  }
		}
		else  // on ne change pas de séance
		{	TraceLigne[index-1] = 0 ;  }		
	}
	
	// Bloc d'affichage 
	if ( FirstIndex != sc.IndexOfFirstVisibleBar  or  LastIndex != sc.IndexOfLastVisibleBar )
	{
		FirstIndex = sc.IndexOfFirstVisibleBar ;
		LastIndex = sc.IndexOfLastVisibleBar ;
		
		// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
		
		// tracés de traits verticaux sur la fenêtre
		int NbIndexVisibles = LastIndex - FirstIndex + 1 ;
		for ( int index = FirstIndex - NbIndexVisibles ; index < min(LastIndex + NbIndexVisibles, sc.ArraySize-1) ; index++ )         // std::max ne fonctionne pas (ne fonctionne que sur des float) ; sans doute lié à une surcharge spécifique à Sierra
		{
			if ( TraceLigne[index] != 0 )
			{
				DrawSegmentVerticalPointille(index, TraceLigne[index], CouleurNoire, ptrsc, sc.GraphRegion) ; 
			}
		} // fin manual looping
		
		// lorsque je suis déconnecté d'IB, pour les backtests :
		if ( sc.ServerConnectionState != SCS_CONNECTED )  
		{	sc.UpdateAlways = 1 ;  }

	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_LigneVerticaleAChaqueSeconde(SCStudyInterfaceRef sc) 
{
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "Ligne verticale a chaque seconde" ;
		sc.GraphRegion = 0 ;
		//sc.ScaleRangeType = SCALE_SAMEASREGION ;
		// sc.ValueFormat = 6 ;
		sc.AutoLoop = 0 ;  		
		sc.DisplayStudyName = 0;
		sc.DisplayStudyInputValues = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		return ;	
	}
	
	
	// Bloc d'affichage 
	if ( sc.ChartWindowIsActive )  // sc.ChartWindowIsActive == 1 : laisser cette condition pour éviter d'alourdir les temps de calcul quand ce chart n'est pas affiché
	{
		// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
		
		// gestion des boutons et sc.HideStudy
		if ( sc.MenuEventID != 0 )
		{
			sc.SetCustomStudyControlBarButtonEnable(sc.MenuEventID,FALSE); // le bouton cliqué ne prend pas la position enfoncée		// cette ligne est nécessaire pour vider le buffer (on crée un doublon si on clique sur un autre bouton)
			if (sc.MenuEventID == ACS_BUTTON_49 )  
			{ sc.HideStudy = 1 - sc.HideStudy ;  }
		}
		
		// bloc de tracé des traits
		if (sc.HideStudy == 0 )  // cocher "Hide"  pour supprimer l'affichage
		{
			COLORREF Couleur = RGB(150,150,250) ;
			
			// tracés de traits verticaux sur 3 écrans consécutifs
			int NbIndexVisibles = sc.IndexOfLastVisibleBar - sc.IndexOfFirstVisibleBar + 1 ;
			for ( int index = sc.IndexOfFirstVisibleBar - NbIndexVisibles ; index < min(sc.IndexOfLastVisibleBar + NbIndexVisibles, sc.ArraySize-1) ; index++ )                 // début manual looping      // std::max ne fonctionne pas (ne fonctionne que sur des float) ; sans doute lié à une surcharge spécifique à Sierra
			{
				
				// Récupération de l'horaire de la barre active et de la précédente.
				SCDateTime Horaire_BarreActive = sc.BaseDateTimeIn[index] ;
				SCDateTime Horaire_BarrePrecedente = sc.BaseDateTimeIn[index-1] ;
				
				// Arrondi au début de la seconde
				Horaire_BarreActive.RoundDateTimeDownToSecond() ;
				Horaire_BarrePrecedente.RoundDateTimeDownToSecond() ;
				
				// Tracé d'une barre verticale si on a pénétré une nouvelle seconde
				if ( Horaire_BarreActive != Horaire_BarrePrecedente )
				{
					DrawVLine(index, 1, Couleur, &sc, sc.GraphRegion) ; 
				}
			} // fin manual looping
		}
		
		// lorsque je suis déconnecté d'IB, pour les backtests :
		if ( sc.ServerConnectionState != SCS_CONNECTED )  
		{	sc.UpdateAlways = 1 ;  }
	}
	
}



