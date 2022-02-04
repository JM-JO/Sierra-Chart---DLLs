/*
v1.0 (2019/06/02) : on trace les niveaux 6000 à 8000 sur l'indice avec UseTool.
v1.1 (2019/2/27) : on projette les niveaux 7300 à 6700 du INQ100 sur le future.  
v1.2 (2019/4/24) : on projette les niveaux 7300 à 8200 du INQ100 sur le future.  
v2.0 (2019/7/6) : le tracé de tous les niveaux indice est togglé ON/OFF via un bouton. Le statut géré par la study "Prix typique".  Les fonctions du DLL study Niveaux_psy_INQ100 consultent ce statut pour gérer la variable sc.HideStudy. Compatible v1941.
v2.1 (2019/9/30) : les niveaux indice projetés sont tracés à partir de 9h00 (le SpreadFutureIndice correspond à sa valeur à la cloture de la veille) pour naviguer sur les niveaux avant 9h30m25s. SpreadMoyenArray est dorénavant récupéré à partir de la study "Overlay INQ100 sur le future".
v2.2 (2019/10/26) : les niveaux en tracé vectoriel ne sont tracés que jusqu'à LastVisibleBar. Les UseTool doivent donc être recalculés à chaque appel du DLL (car LastVisibleBar peut changer).
v2.3 (2019/10/28) : les niveaux projetés sont tracés en vectoriel. Le niveau retenu est celui de index = sc.LastVisibleBar. Les niveaux qui valent <500 ne sont pas tracés.
v2.4 (2019/11/7) : les niveaux 10 à moins de 20 pts du dernier prix sont tracés sur le graphe indice UT 20s, et projetés sur le graphe UT 16ticks.
v2.5 (2019/11/14) : les niveaux 25 à moins de xxx pts du dernier prix sont tracés sur le graphe indice UT 20s, et projetés sur le graphe UT 16ticks.
v2.6 (2019/11/15) : "BooleanHideStudy" devient "NiveauxIndicesAffiches_Statut" (vaut 0 (OFF), 1 (All) ou 2 (Lean)).
v2.7 (2019/11/20) : sur INQ100, ajout d'inputs pour supprimer les niveaux 10 et 25. Ajout d'un input pour supprimer les niveaux faibles automatiquement sur l'INQ100 en UT 4 min.
v2.8 (2020/1/9) : modifications des niveaux indices projetés.
v3.0 (2020/3/17) : suppression des SCSubgraphRef utilisés pour les tracés des niveaux projetés, et remplacement simplement par un tracé de " NiveauPsy + fSpreadMoyen".
v3.1 (2020/6/27) : on remplace GetTimeWithoutMilliseconds par GetTimeInSecondsWithoutMilliseconds.
v3.2 (2020/6/27) : accélération de SC par ajout de la condition : " if ( sc.ChartWindowIsActive  or  NbMonitors == 2 ) ".
v3.3 (2020/6/27) : accélération de SC : on ne trace que les niveaux visibles sur l'écran.
v3.4 (2020/7/23) : dans les niveaux projetés, ajout des inputs : Niveaux50 et NiveauxMoyens.
v3.5 (2020/09/08) : ajout d'un input "Trace Vectoriel ON/OFF" pour les niveaux non projetés.



Améliorations à apporter :
- 


*/

#include "sierrachart.h"  // When including in your source code file additional header files, they need to be placed above the #include "sierrachart.h" line. 

SCDLLName("Niveaux psys de l'indice NQ100")

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MonTexte : s_UseTool // MonTexte crée du texte avec mes paramètres choisis  (hérite de toutes les données public et protected de s_UseTool)
{											
	MonTexte(float niveau, int epaisseur, COLORREF rgb) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
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
		//Formatage du texte
		SCString texte ;
		if ( niveau-floor(niveau) == 0 ) { int niveauint = (int)niveau ; texte.Format("%d", niveauint);}  else { texte.Format("%.2f", niveau); }
		Text = texte;			
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MaLigneJusquAIndex : s_UseTool  // MaLigneJusquALastIndex crée une ligne entre index = 0 et index
	{
		MaLigneJusquAIndex(float niveau, int beginIndex, int endIndex, int epaisseur, COLORREF rgb, s_sc* ptrsc) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
		{
			Clear(); 
			DrawingType = DRAWING_LINE;
			LineStyle = LINESTYLE_SOLID;
			AddMethod = UTAM_ADD_ALWAYS;	
			BeginIndex = beginIndex ;
			BeginValue = niveau ;
			EndIndex = endIndex ;
			EndValue = niveau ;
			LineWidth = epaisseur;
			Color = rgb; 
			Region = ptrsc->GraphRegion ; 
			DrawUnderneathMainGraph = true ;
		} 
	};
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float TopCoordinate ;		// la déclaration de ces deux variables ici permet d'éviter de les repasser en paramètre de toutes les fonctions creationLigneJusquAIndexEtTexte
	float BottomCoordinate ;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void creationLigneJusquAIndexEtTexte (int endIndex, float monNiveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)       // fonction pour créer ligne sur graphiques indice UT 15s et UT 4min
	{
		if ( monNiveau > BottomCoordinate  and  monNiveau < TopCoordinate )
		{
			//Création d'instance de MaLigneJusquAIndex et appel de sc.UseTool
			MaLigneJusquAIndex objetMaLigne(monNiveau, 0, endIndex, monEpaisseur, maCouleur, ptrsc);
			ptrsc->UseTool(objetMaLigne); // c'est sc.UseTool qui est appelé
			//Création d'instances de MonTexte et appel de sc.UseTool
			MonTexte objetMonTexte(monNiveau, monEpaisseur, maCouleur);
			ptrsc->UseTool(objetMonTexte); 
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)       // fonction pour créer ligne sur graphique 16-ticks
	{
		if ( niveau > BottomCoordinate  and  niveau < TopCoordinate )
		{
			if ( /* monEpaisseur > 0  and */  niveau > 500 )
			{
				//Création d'instance de MaLigneJusquAIndex et appel de sc.UseTool
				MaLigneJusquAIndex objetMaLigne(niveau, beginIndex, endIndex, monEpaisseur, maCouleur, ptrsc);
				ptrsc->UseTool(objetMaLigne); // c'est sc.UseTool qui est appelé
				//Création d'instances de MonTexte et appel de sc.UseTool
				MonTexteString objetMonTexteString(Texte, niveau, monEpaisseur, maCouleur, ptrsc);
				ptrsc->UseTool(objetMonTexteString); 
			}
		}
	}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int NumberOfMonitors()		// Number of display monitors on desktop
{
	return GetSystemMetrics(80) ;		// https://docs.microsoft.com/fr-fr/windows/win32/api/winuser/nf-winuser-getsystemmetrics
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_NiveauxINQ100(SCStudyInterfaceRef sc)
{
	SCInputRef TraceVectorielONOFF = sc.Input[1]; 		// modifié par Prix_typique du chart #6
	SCInputRef TraceDesDizaines = sc.Input[2];				// modifié par Prix_typique du chart #6
	SCInputRef TraceDes25 = sc.Input[3];						// modifié par Prix_typique du chart #6
	SCInputRef TraceDes50 = sc.Input[4];						// modifié par Prix_typique du chart #6
	SCInputRef TraceDesNiveauxMoyens = sc.Input[5];	// modifié par Prix_typique du chart #6
	
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  

	float& BottomCoordinatePrevious = sc.GetPersistentFloatFast(0);  
	float& TopCoordinatePrevious = sc.GetPersistentFloatFast(1);  


	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[0] Niveaux psys de l'indice traces sur l'indice";		// modifié par Prix_typique du chart #6
		sc.AutoLoop = 0; 
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;		
		sc.HideStudy = 1 ;
		
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		TraceDesDizaines.Name = "TraceDesDizaines" ;
		TraceDes25.Name = "TraceDes25" ;
		TraceDes50.Name = "TraceDes50" ;
		TraceDesNiveauxMoyens.Name = "TraceDesNiveauxMoyens" ;
		
		// Détermination de l'UT pour déterminer TraceDesDizaines, TraceDes25, TraceDesNiveauxMoyens
		n_ACSIL :: s_BarPeriod BarPeriod ;
		sc.GetBarPeriodParameters(BarPeriod) ;
		if ( BarPeriod.IntradayChartBarPeriodType == IBPT_DAYS_MINS_SECS  and  (  BarPeriod.IntradayChartBarPeriodParameter1 >= 1  and  BarPeriod.IntradayChartBarPeriodParameter1 <= 60 )  )    // UT comprise entre 1s et 60s
		{ TraceDesDizaines.SetYesNo(0) ;  TraceDes25.SetYesNo(0) ;  TraceDes50.SetYesNo(1) ;  TraceDesNiveauxMoyens.SetYesNo(1) ;  }
		else
		{ TraceDesDizaines.SetYesNo(0) ;  TraceDes25.SetYesNo(0) ;  TraceDes50.SetYesNo(0) ;  TraceDesNiveauxMoyens.SetYesNo(0) ;  }
		
		return;
	}
	
	
	if ( sc.UpdateStartIndex== 0 )  
	{
		sc.GraphRegion = 0; 
		NbMonitors = NumberOfMonitors() ;

		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;
	}
	
	
	
	// Bloc d'affichage       
	if ( sc.ChartWindowIsActive  or  NbMonitors == 2  or  sc.IsFullRecalculation )
	{
		BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;		// la variable est déclarée en dehors de la fct scsf
		TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;				// la variable est déclarée en dehors de la fct scsf
		if (  TraceVectorielONOFF.GetYesNo()  and  ( FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(BottomCoordinatePrevious-BottomCoordinate) > 0.02  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02 )  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou BottomCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)		// Note : Sierra a un bug interne pour le premier calcul de BottomCoordinate (il vaut la même valeur que TopCoordinate)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
		{	
			FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
			LastVisibleBar = sc.IndexOfLastVisibleBar ;
			BottomCoordinatePrevious = BottomCoordinate ;
			TopCoordinatePrevious = TopCoordinate ;

			// les useTool doivent être effacés à chaque appel du bloc d'affichage pour éviter une fuite de mémoire
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		

			s_sc* const ptrsc = &sc;
			COLORREF couleur = RGB(0, 0, 255) ;
			
			
			// tracés des dizaines avant/après le dernier prix affiché  // tracer ces lignes+textes avant les tracés des niveaux plus importants car ils vont les écraser
			if ( TraceDesDizaines.GetYesNo() )
			{
				float DizaineFloor = floor(sc.BaseDataIn[SC_LAST][LastVisibleBar]/10)*10 ;    // arrondi à la dizaine sous le dernier prix affiché
				creationLigneJusquAIndexEtTexte(LastVisibleBar, DizaineFloor+20, 1, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, DizaineFloor+10, 1, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, DizaineFloor     , 1, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, DizaineFloor-10, 1, couleur, ptrsc);
			}
			
			// tracés des 25 avant/après le dernier prix affiché  // tracer ces lignes+textes avant les tracés des niveaux plus importants car ils vont les écraser
			if ( TraceDes25.GetYesNo() )
			{
				float f25Floor = floor(sc.BaseDataIn[SC_LAST][LastVisibleBar]/25)*25 ;    // arrondi à 25 sous le dernier prix affiché
				creationLigneJusquAIndexEtTexte(LastVisibleBar, f25Floor+50, 1, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, f25Floor+25, 1, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, f25Floor     , 1, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, f25Floor-25, 1, couleur, ptrsc);
			}

			// tracé des niveaux forts : grosso modo les multiples de 100 et 250
			{
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 14000, 5, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13900, 3, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13800, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13750, 4, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13700, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13600, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13500, 4, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13400, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13300, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13250, 4, couleur, ptrsc);		
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13200, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13100, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13000, 5, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12900, 3, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12800, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12750, 4, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12700, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12600, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12500, 5, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12400, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12300, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12250, 4, couleur, ptrsc);		
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12200, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12100, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12000, 5, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11900, 3, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11800, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11750, 4, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11700, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11600, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11500, 4, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11400, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11300, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11250, 4, couleur, ptrsc);		
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11200, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11100, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11000, 5, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10900, 3, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10800, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10750, 4, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10700, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10600, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10500, 4, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10400, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10300, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10250, 4, couleur, ptrsc);		
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10200, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10100, 3, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10000, 6, couleur, ptrsc);   // spécial
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9900, 3, couleur, ptrsc);  
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9800, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9750, 4, couleur, ptrsc);  
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9700, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9600, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9500, 4, couleur, ptrsc);  
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9400, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9300, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9250, 4, couleur, ptrsc);		
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9200, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9100, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9000, 5, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8900, 3, couleur, ptrsc);  
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8800, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8750, 4, couleur, ptrsc);  
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8700, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8600, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8500, 4, couleur, ptrsc);  
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8400, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8300, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8250, 4, couleur, ptrsc);		
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8200, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8100, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8000, 5, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7900, 3, couleur, ptrsc);  // FontSize = 6 + 3 = 9 (niveau 100)
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7800, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7750, 4, couleur, ptrsc);  // FontSize = 6 + 4 = 10 (niveau 250)
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7700, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7600, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7500, 5, couleur, ptrsc);  // spécial
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7400, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7300, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7250, 4, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7200, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7100, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7000, 5, couleur, ptrsc);  // FontSize = 6 + 5 = 11 (niveau 1000)
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6900, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6800, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6750, 4, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6700, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6666, 3, couleur, ptrsc);  // spécial
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6600, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6500, 4, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6400, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6300, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6250, 4, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6200, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6100, 3, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6000, 5, couleur, ptrsc);
			}
			
			// tracés des niveaux 50
			if ( TraceDes50.GetYesNo() )
			{
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13950, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13850, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13650, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13550, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13450, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13350, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13150, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13050, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12950, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12850, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12650, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12550, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12450, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12350, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12150, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12050, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11950, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11850, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11650, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11550, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11450, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11350, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11150, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11050, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10950, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10850, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10650, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10550, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10450, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10350, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10150, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10050, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9950, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9850, 2, couleur, ptrsc);  
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9650, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9550, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9450, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9350, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9150, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9050, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8950, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8850, 2, couleur, ptrsc);  
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8650, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8550, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8450, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8350, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8150, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8050, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7950, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7850, 2, couleur, ptrsc);  // FontSize = 6 + 2 = 8 (niveau 50)
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7650, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7550, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7450, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7350, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7150, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7050, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6950, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6850, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6650, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6550, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6450, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6350, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6150, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6050, 2, couleur, ptrsc);
			}

			// tracés des autres niveaux moyens (125, 333, ...) : grosso modo tout ce qui n'est pas un multiple de 50
			if ( TraceDesNiveauxMoyens.GetYesNo() )  // niveaux non tracés sur le graphe en UT=4min
			{
				
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13875, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13666, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13625, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13375, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13333, 3, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 13125, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12875, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12857.14, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12666, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12625, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12375, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12333, 2, couleur, ptrsc);  
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 12222, 2, couleur, ptrsc);  
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 12125, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11875, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11666, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11625, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 11428.57, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11375, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11333, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 11125, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 11111, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10875, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10666, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10625, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10375, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10333, 2, couleur, ptrsc);
				creationLigneJusquAIndexEtTexte(LastVisibleBar, 10125, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9875, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9666, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9625, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9375, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9333, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 9125, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8888, 2, couleur, ptrsc); 
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8875, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8666, 2, couleur, ptrsc);  
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8625, 2, couleur, ptrsc);  // FontSize = 6 + 1 = 7 (niveau 125)
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8571.43, 2, couleur, ptrsc); 
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8375, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8333, 2, couleur, ptrsc);  // spécial ? sixième de 10000
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 8125, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7875, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7777, 2, couleur, ptrsc);  
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7666, 2, couleur, ptrsc);  
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7625, 2, couleur, ptrsc);  
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7375, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7333, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7142.86, 2, couleur, ptrsc); 
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 7125, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6875, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6625, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6375, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6333, 2, couleur, ptrsc);
				// creationLigneJusquAIndexEtTexte(LastVisibleBar, 6125, 2, couleur, ptrsc);
			}
			
			// recalcul immédiat
			sc.UpdateAlways = 1 ;
		}
	}
	if ( TraceVectorielONOFF.GetYesNo() == 0 )  sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   
	
	// lorsque je suis déconnecté d'IB, pour les backtests :
	if ( sc.ServerConnectionState != SCS_CONNECTED )  
	{	sc.UpdateAlways = 1 ;  }
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_NiveauxINQ100Projetes(SCStudyInterfaceRef sc) 
{
	SCFloatArrayRef FirstIndex = sc.Subgraph[1].Arrays[0] ;  
	
	
	SCInputRef TraceVectorielONOFF = sc.Input[1];      	// modifié par Prix_typique 		// appelé par NavigNiveaux
	SCInputRef TraceDesDizaines = sc.Input[2];				// modifié par Prix_typique 		// appelé par NavigNiveaux
	SCInputRef TraceDes25 = sc.Input[3];						// modifié par Prix_typique 		// appelé par NavigNiveaux
	SCInputRef TraceDes50 = sc.Input[4];						// modifié par Prix_typique 		// appelé par NavigNiveaux
	SCInputRef TraceDesNiveauxMoyens = sc.Input[5];	// modifié par Prix_typique 		// appelé par NavigNiveaux
	
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& HoraireDebutSeance = sc.GetPersistentIntFast(1) ;  // time value en secondes
	int& HoraireFinSeance = sc.GetPersistentIntFast(2) ;
	int& StudyIDOverlayINQ100 = sc.GetPersistentIntFast(3) ;
	int& FirstVisibleBar = sc.GetPersistentIntFast(4);  
	int& LastVisibleBar = sc.GetPersistentIntFast(5);  
	int& TopCoordinatePrevious = sc.GetPersistentIntFast(6);  
	int& BottomCoordinatePrevious = sc.GetPersistentIntFast(7);  
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "[Z] Niveaux psys de l'indice projetes sur le future";   // appelé par Navig_Niveaux		// appelé par Prix_typique 
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.AutoLoop = 0;  
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		sc.GraphRegion = 1; 
		sc.HideStudy = 1 ;

		
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		TraceDesDizaines.Name = "Trace des dizaines";
		TraceDesDizaines.SetYesNo(1);
		TraceDes25.Name = "Trace des 25/75";
		TraceDes25.SetYesNo(1);
		TraceDes50.Name = "TraceDes50" ;
		TraceDes50.SetYesNo(1);
		TraceDesNiveauxMoyens.Name = "TraceDesNiveauxMoyens" ;
		TraceDesNiveauxMoyens.SetYesNo(1);
		
		return ;	
	}
	
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 1;  
		
		NbMonitors = NumberOfMonitors() ;

		HoraireDebutSeance = HMS_TIME(9,0,0); 
		HoraireFinSeance = HMS_TIME(16,0,0);
		
		// Récupération de la studyID sur le graphe 16-ticks
		StudyIDOverlayINQ100 = sc.GetStudyIDByName(sc.ChartNumber, "Overlay INQ100 sur le future (avec correction du spread)", 0) ;
		if  (StudyIDOverlayINQ100 == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le spread Future-Indice n'est pas disponible") ; sc.AddMessageToLog(message,1) ; }
	}
	

	// on récupère l'array de SpreadMoyen
	SCFloatArray SpreadMoyenArray ;
	sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDOverlayINQ100, 0, 1, SpreadMoyenArray) ;

	
	// Manual Looping pour récupérer FirstIndex
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                               
	{
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		
		if ( OpenTimeValue > HoraireDebutSeance and OpenTimeValue < HoraireFinSeance )  // n'est actualisé que pendant l'ouverture du marché action
		{
			// Gestion de FistIndex
			if ( FirstIndex[index-1] == 0 )    // premier index de la séance action
			{ FirstIndex[index] = index ;  }
			else
			{ FirstIndex[index] = FirstIndex[index-1] ;  }
		}			
		
		else  // le marché action est fermé
		{  	
			FirstIndex[index] = 0 ;
		}  
	}
	
	
	
	// Bloc d'affichage       
	if ( sc.ChartWindowIsActive  or  NbMonitors == 2 )
	{
		BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;
		TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;
		if ( TraceVectorielONOFF.GetYesNo()  and  ( FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02  or  fabs(BottomCoordinatePrevious-BottomCoordinate) > 0.02 ) )		// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar varie   // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
		{	
			FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
			LastVisibleBar = sc.IndexOfLastVisibleBar ;
			BottomCoordinatePrevious = BottomCoordinate ;
			TopCoordinatePrevious = TopCoordinate ;
			
			// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
			
			s_sc* const ptrsc = &sc;
			SCString Texte ;
			int const firstIndex = FirstIndex[LastVisibleBar] ;
			COLORREF const couleur = RGB(0,0,255);
			float fSpreadMoyen = SpreadMoyenArray[LastVisibleBar] ;

			
			if ( firstIndex !=0 )
			{
				// tracés des dizaines avant/après le dernier prix affiché  // tracer ces lignes+textes avant les tracés des niveaux plus importants car ils vont les écraser
				if ( TraceDesDizaines.GetYesNo() )
				{
					float DizaineFloor = floor((sc.BaseDataIn[SC_LAST][LastVisibleBar]-fSpreadMoyen)/10)*10 ;    // arrondi à la dizaine inférieur du prix de l'indice
					int const epaisseur = 1 ;
					
					Texte.Format("%.0f", DizaineFloor+20) ;			creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, DizaineFloor+20+fSpreadMoyen, epaisseur, couleur, ptrsc);
					Texte.Format("%.0f", DizaineFloor+10) ;			creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, DizaineFloor+10+fSpreadMoyen, epaisseur, couleur, ptrsc);
					Texte.Format("%.0f", DizaineFloor) ;				creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, DizaineFloor     +fSpreadMoyen, epaisseur, couleur, ptrsc);
					Texte.Format("%.0f", DizaineFloor-10) ;			creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, DizaineFloor-10+fSpreadMoyen, epaisseur, couleur, ptrsc);
				}
				
				// tracés des 25 avant/après le dernier prix affiché  // tracer ces lignes+textes avant les tracés des niveaux plus importants car ils vont les écraser
				if ( TraceDes25.GetYesNo() )
				{
					float f25Floor = floor((sc.BaseDataIn[SC_LAST][LastVisibleBar]-fSpreadMoyen)/25)*25 ;    // arrondi au 25 inférieur du prix de l'indice
					int const epaisseur = 1 ;
					
					Texte.Format("%.0f", f25Floor+50) ;			creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, f25Floor+50+fSpreadMoyen, epaisseur, couleur, ptrsc);
					Texte.Format("%.0f", f25Floor+25) ;			creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, f25Floor+25+fSpreadMoyen, epaisseur, couleur, ptrsc);
					Texte.Format("%.0f", f25Floor) ;				creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, f25Floor     +fSpreadMoyen, epaisseur, couleur, ptrsc);
					Texte.Format("%.0f", f25Floor-25) ;			creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, f25Floor-25+fSpreadMoyen, epaisseur, couleur, ptrsc);
				}
				
				float NiveauPsy ;				
				
				// tracés des niveaux forts
				{
					NiveauPsy = 14000 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 5, couleur, ptrsc);
					NiveauPsy = 13900 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 13800 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 13750 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					NiveauPsy = 13700 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 13600 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 13500 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					NiveauPsy = 13400 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 13300 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 13250 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					NiveauPsy = 13200 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 13100 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 13000 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 5, couleur, ptrsc);
					NiveauPsy = 12900 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 12800 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 12750 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					NiveauPsy = 12700 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 12600 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 12500 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 5, couleur, ptrsc);
					NiveauPsy = 12400 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 12300 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 12250 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					NiveauPsy = 12200 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 12100 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 12000 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 5, couleur, ptrsc);
					NiveauPsy = 11900 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 11800 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 11750 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					NiveauPsy = 11700 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 11600 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 11500 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					NiveauPsy = 11400 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 11300 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 11250 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					NiveauPsy = 11200 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 11100 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 11000 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 5, couleur, ptrsc);
					NiveauPsy = 10900 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 10800 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 10750 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					NiveauPsy = 10700 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 10600 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 10500 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					NiveauPsy = 10400 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 10300 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 10250 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					NiveauPsy = 10200 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 10100 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					NiveauPsy = 10000 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 6, couleur, ptrsc);
					// NiveauPsy = 9900 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 9800 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 9750 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					// NiveauPsy = 9700 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 9600 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 9500 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					// NiveauPsy = 9400 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 9300 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 9250 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					// NiveauPsy = 9200 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 9100 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 9000 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 5, couleur, ptrsc);
					// NiveauPsy = 8900 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 8800 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 8750 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					// NiveauPsy = 8700 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 8600 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 8500 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					// NiveauPsy = 8400 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 8300 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 8250 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					// NiveauPsy = 8200 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 8100 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 8000 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 5, couleur, ptrsc);
					// NiveauPsy = 7900 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 7800 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 7750 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					// NiveauPsy = 7700 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 7600 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 7500 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 5, couleur, ptrsc);
					// NiveauPsy = 7400 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 7300 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 7250 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					// NiveauPsy = 7200 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 7100 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 7000 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 5, couleur, ptrsc);
					// NiveauPsy = 6900 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 6800 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 6750 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					// NiveauPsy = 6700 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 6600 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 6500 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					// NiveauPsy = 6400 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 6300 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
					// NiveauPsy = 6250 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 4, couleur, ptrsc);
					// NiveauPsy = 6200 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6100 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 3, couleur, ptrsc);
				}
				
				// tracé des 50
				if ( TraceDes50.GetYesNo() )
				{
					NiveauPsy = 13950 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 13850 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 13650 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 13550 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 13450 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 13350 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 13150 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 13050 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12950 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12850 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12650 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12550 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12450 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12350 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12150 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12050 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);			
					NiveauPsy = 11950 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 11850 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 11650 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 11550 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 11450 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 11350 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 11150 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 11050 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 10950 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 10850 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 10650 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 10550 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 10450 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 10350 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 10150 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 10050 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);			
					// NiveauPsy = 9950 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 9850 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 9650 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 9550 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 9450 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 9350 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 9150 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 9050 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8950 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8850 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8650 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8550 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8450 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8350 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8150 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8050 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7950 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7850 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7650 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7550 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7450 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7350 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7150 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7050 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6950 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6850 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6650 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6550 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6450 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6350 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6150 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6050 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
				}

				// tracé des niveaux moyens
				if ( TraceDesNiveauxMoyens.GetYesNo() )
				{
					NiveauPsy = 13875 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 13666 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 13625 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 13375 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 13333 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 13125 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12875 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12857.14 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12666 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12625 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12375 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12333 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 12222 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 12125 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 11875 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 11666 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 11625 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 11428.57 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 11375 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 11333 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 11125 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 11111 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 10875 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 10666 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 10625 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 10375 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 10333 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					NiveauPsy = 10125 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 9875 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 9666 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 9625 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 9375 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 9333 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 9125 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8888 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8875 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8666 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8625 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8571.43 ;  Texte.Format("%.2f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8375 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8333 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 8125 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7875 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7777 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7666 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7625 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7375 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7333 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7142.86 ;  Texte.Format("%.2f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 7125 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6875 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6666 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6625 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6375 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6333 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
					// NiveauPsy = 6125 ;  Texte.Format("%.0f", NiveauPsy) ;  creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, NiveauPsy + fSpreadMoyen, 2, couleur, ptrsc);
				}
				
			}
			
			// recalcul immédiat
			sc.UpdateAlways = 1 ;
		}
	}
	
	if ( TraceVectorielONOFF.GetYesNo() == 0 )  sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   

	// lorsque je suis déconnecté d'IB, pour les backtests :
	if ( sc.ServerConnectionState != SCS_CONNECTED )  
	{	sc.UpdateAlways = 1 ;  }
	
}


