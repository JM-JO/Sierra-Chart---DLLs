/*
v1.0 (2019/02/02) : on part de niveaux_v2.3. On trace les niveaux 6000 à 8000 pour le future (centaines et 250/750).
v1.1 (2019/04/18) : on trace tous les niveaux du manuel qualité.
v2.0 (2019/7/4) : le tracé de tous les niveaux futures est togglé ON/OFF via un bouton. Le statut géré par la study "Prix typique". Les fonctions du DLL study Niveaux_psy_FNQ100 consultent ce statut pour gérer la variable sc.HideStudy.
v2.1 (2019/10/26) : les niveaux en tracé vectoriel ne sont tracés que jusqu'à LastIndex. Les UseTool doivent donc être recalculés à chaque appel du DLL (car LastIndex peut changer). 





Note : les LineNumber ne servent plus à rien, mais on les garde tout de même au cas où on en aurait rebesoin ultérieurement.




Manuel qualité du traçage de niveau NQ (màj 14/6/2018, idem PRT) :
- niveaux (50, xZZZZ) = F1.5 (line,1).
- niveaux (100, 333.0, 666.0) = F2.
- niveaux (250,750) = F2.5 (line,2).
- niveaux (500) = F3 (line,3).
- niveaux (1000) = F3.5 (line,3).




SOMMAIRE DES STRUCTURES ET FONCTIONS :
MaLigne
MonTexte
creationLigneEtTexte
BooleanHideStudy
scsf_Niveaux



*/

#include "sierrachart.h"  // When including in your source code file additional header files, they need to be placed above the #include "sierrachart.h" line. 

SCDLLName("Niveaux psys du future NQ100")


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MaLigne : s_UseTool  // MaLigne crée un objet qui contient mes paramètres choisis pour une ligne horizontale  (hérite de toutes les données public et protected de s_UseTool)
	{
		MaLigne(float niveau, int epaisseur, COLORREF rgb, int Graphregion, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
		{
			Clear(); 
			AddMethod = UTAM_ADD_OR_ADJUST ;		
			LineNumber = Line_Number ;		
			DrawingType = DRAWING_HORIZONTALLINE;
			LineStyle = LINESTYLE_SOLID;
			BeginValue = niveau;
			LineWidth = epaisseur;
			Color = rgb; 
			Region = Graphregion ;   // 0 par défaut		
		} 
	};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MaLigneJusquAIndex : s_UseTool  // MaLigne crée un objet qui contient mes paramètres choisis pour une ligne horizontale  (hérite de toutes les données public et protected de s_UseTool)
	{
		MaLigneJusquAIndex(float niveau, int index, int epaisseur, COLORREF rgb, int Graphregion, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
		{
			Clear(); 
			AddMethod = UTAM_ADD_OR_ADJUST ;		
			LineNumber = Line_Number ;		
			DrawingType = DRAWING_LINE;
			LineStyle = LINESTYLE_SOLID;
			BeginIndex = 0 ;
			BeginValue = niveau ;
			EndIndex = index ;
			EndValue = niveau ;
			LineWidth = epaisseur;
			Color = rgb; 
			Region = Graphregion ;   
		} 
	};
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
struct MonTexte : s_UseTool // MonTexte crée du texte avec mes paramètres choisis  (hérite de toutes les données public et protected de s_UseTool)
{											
	MonTexte(float niveau, int epaisseur, COLORREF rgb, int Graphregion, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;		
		LineNumber = Line_Number ;			
		DrawingType = DRAWING_TEXT;
		BeginDateTime = 150;
		//FontBold = 1;
		Color = RGB(220, 220, 220); //background gris du graphique
		TextAlignment = DT_VCENTER | DT_RIGHT;
		BeginValue = niveau;
		FontSize = epaisseur+6;
		FontBackColor = rgb;
		Region = Graphregion ;   // 0 par défaut		
		//Formatage du texte
		SCString texte ;
		if ( niveau-floor(niveau) == 0 ) { int niveauint = (int)niveau ; texte.Format("%d", niveauint);}  else { texte.Format("%.2f", niveau); }
		Text = texte;			
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void creationLigneEtTexte (float monNiveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc, int Line_Number)     // Line_Number est pair
	{
		//Création d'instance de MaLigne et appel de sc.UseTool
		MaLigne objetMaLigne(monNiveau, monEpaisseur, maCouleur, ptrsc->GraphRegion, Line_Number+0);
		ptrsc->UseTool(objetMaLigne); // c'est sc.UseTool qui est appelé
		//Création d'instances de MonTexte et appel de sc.UseTool
		MonTexte objetMonTexte(monNiveau, monEpaisseur, maCouleur, ptrsc->GraphRegion, Line_Number+1);
		ptrsc->UseTool(objetMonTexte); 
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void creationLigneJusquAIndexEtTexte (int index, float monNiveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc, int Line_Number)     // Line_Number est pair
	{
		//Création d'instance de MaLigneJusquAIndex et appel de sc.UseTool
		MaLigneJusquAIndex objetMaLigne(monNiveau, index, monEpaisseur, maCouleur, ptrsc->GraphRegion, Line_Number+0);
		ptrsc->UseTool(objetMaLigne); // c'est sc.UseTool qui est appelé
		//Création d'instances de MonTexte et appel de sc.UseTool
		MonTexte objetMonTexte(monNiveau, monEpaisseur, maCouleur, ptrsc->GraphRegion, Line_Number+1);
		ptrsc->UseTool(objetMonTexte); 
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

SCSFExport scsf_Niveaux(SCStudyInterfaceRef sc)
{
	
	
	s_sc* const ptrsc = &sc;
	COLORREF const couleur = RGB(0, 0, 255) ;
	int const ThisStudyID = sc.StudyGraphInstanceID ;  // StudyID de l'instance
	int const LastIndex = sc.ArraySize - 1 ;

	
	if (sc.SetDefaults)
	{
		sc.GraphName = "Niveaux psys future (vectoriel)";		
		sc.GraphRegion = 1;
		sc.AutoLoop = 0; 
		sc.DisplayStudyName = 0;
		return;
	}

	
	// on efface les lignes
	sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;     // all non user drawn Chart Drawings on the specified ChartNumber will be deleted.
	
	
	// détermination du XTicks (ne fonctionne pas si placé dans sc.SetDefaults)
	n_ACSIL :: s_BarPeriod BarPeriod;
	sc.GetBarPeriodParameters(BarPeriod);
	
	
	// cas du graphe en 16-ticks
	if ( BarPeriod.IntradayChartBarPeriodParameter1 == 16 )  
	{
		sc.GraphRegion = 1; 
		
		if ( BooleanHideStudy(ptrsc) == 0 )   // on trace les lignes
		{
		
			//Tracé des centaines :
			for (float niveau=6650 ; niveau<=8700 ; niveau+=50)   
			{	creationLigneJusquAIndexEtTexte(LastIndex, niveau, 1, couleur, ptrsc, 10000+1000*ThisStudyID+niveau/25);  }       
		
			//Tracé des 333/666 :
			creationLigneJusquAIndexEtTexte(LastIndex, 8666, 1, couleur, ptrsc, 0);		
			creationLigneJusquAIndexEtTexte(LastIndex, 8333, 1, couleur, ptrsc, 2);
			creationLigneJusquAIndexEtTexte(LastIndex, 7666, 1, couleur, ptrsc, 4);
			creationLigneJusquAIndexEtTexte(LastIndex, 7333, 1, couleur, ptrsc, 6);
			creationLigneJusquAIndexEtTexte(LastIndex, 6666, 1, couleur, ptrsc, 8);		
		}
		
		
		if (sc.MenuEventID == 105) 
		{	sc.UpdateAlways = 1;  }  // pour que la study soit recalculée immédiatement (sinon il faut attendre qu'une condition d'appel se produise, il n'y a jamais d'appel si le marché est fermé)
		else { sc.UpdateAlways = 0; }
	}
	
	// cas des graphes en 200 et 2000 ticks : les lignes sont toujours tracées
	else
	{
		if 			( BarPeriod.IntradayChartBarPeriodParameter1 == 200 )  sc.GraphRegion = 0; 
		else if 	( BarPeriod.IntradayChartBarPeriodParameter1 == 2000 )  sc.GraphRegion = 1; 
		
		
		//Tracé des centaines :
		for (float niveau=6650 ; niveau<=8700 ; niveau+=50)   
		{	creationLigneJusquAIndexEtTexte(LastIndex, niveau, 1, couleur, ptrsc, 10000+1000*ThisStudyID+niveau/25);  }    
	
		//Tracé des 333/666 :
		creationLigneJusquAIndexEtTexte(LastIndex, 8666, 1, couleur, ptrsc, 0);		
		creationLigneJusquAIndexEtTexte(LastIndex, 8333, 1, couleur, ptrsc, 2);
		creationLigneJusquAIndexEtTexte(LastIndex, 7666, 1, couleur, ptrsc, 4);
		creationLigneJusquAIndexEtTexte(LastIndex, 7333, 1, couleur, ptrsc, 6);
		creationLigneJusquAIndexEtTexte(LastIndex, 6666, 1, couleur, ptrsc, 8);		
	}
	
}

