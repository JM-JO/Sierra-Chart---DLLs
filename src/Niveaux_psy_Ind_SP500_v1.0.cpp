/*
v1.0 (2019/06/02) : on trace les niveaux 2590 à 3010 sur l'indice avec UseTool.


Manuel qualité du traçage de niveaux indice SP500 (màj 29/9/2018 (idem PRT)) :
- niveaux F1.5 = 142.8, 285.7, 428.5, 571.4, 714.2, 857.1 (line,1).
- niveaux F2.5 = dizaines, 25,75 (line,2).
- niveaux F3 =  125, 375, 625, 875 (line,3).
- niveaux F3.5 = 50, 333.0, 666.0, (line,3).
- niveaux F4 = 100 (line,4)
- niveaux F4.5 = 250 (line,4)
- niveaux F5 = 500 (line,5)
- niveaux F5.5 = 500 (line,5)
- niveaux F6 = 1000 (line,5)
- niveaux xZZZ, 166 et 833 pour observation (line,1).


SOMMAIRE DES STRUCTURES ET FONCTIONS :
MaLigne
MonTexte
creationLigneEtTexte
scsf_NiveauxINQ100



*/

#include "sierrachart.h"  // When including in your source code file additional header files, they need to be placed above the #include "sierrachart.h" line. 

SCDLLName("Niveaux psys de l'indice SP500")

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MaLigne : s_UseTool  // MaLigne crée un objet qui contient mes paramètres choisis pour une ligne horizontale  (hérite de toutes les données public et protected de s_UseTool)
	{
		MaLigne(float niveau, int epaisseur, COLORREF rgb) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
		{
		Clear(); 
		DrawingType = DRAWING_HORIZONTALLINE;
		LineStyle = LINESTYLE_SOLID;
		AddMethod = UTAM_ADD_ALWAYS;	
		BeginValue = niveau;
		LineWidth = epaisseur;
		Color = rgb; 
		} 
	};

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
		BeginDateTime = 150;  // 0 = position à gauche ; 150 pour la position à droite
		TextAlignment = DT_VCENTER | DT_RIGHT ;
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

void creationLigneEtTexte (float monNiveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
	{
		//Création d'instance de MaLigne et appel de sc.UseTool
		MaLigne objetMaLigne(monNiveau, monEpaisseur, maCouleur);
		ptrsc->UseTool(objetMaLigne); // c'est sc.UseTool qui est appelé
		//Création d'instances de MonTexte et appel de sc.UseTool
		MonTexte objetMonTexte(monNiveau, monEpaisseur, maCouleur);
		ptrsc->UseTool(objetMonTexte); 
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_NiveauxISP500(SCStudyInterfaceRef sc)
{
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "Niveaux psys de l'indice (vectoriel)";		
		sc.GraphRegion = 0; // 1 par défaut
		sc.AutoLoop = 0; 
		return;
	}
		
		// if (sc.IsFullRecalculation)
	if (sc.UpdateStartIndex == 0)
	{
		s_sc* ptrsc = &sc;
		COLORREF couleur = RGB(0, 0, 255) ;
	
		creationLigneEtTexte(3010, 10, couleur, ptrsc);
		creationLigneEtTexte(3000, 6, couleur, ptrsc);
		creationLigneEtTexte(2990, 2, couleur, ptrsc);
		creationLigneEtTexte(2980, 2, couleur, ptrsc);
		creationLigneEtTexte(2975, 2, couleur, ptrsc);
		creationLigneEtTexte(2970, 2, couleur, ptrsc);
		creationLigneEtTexte(2960, 2, couleur, ptrsc);
		creationLigneEtTexte(2950, 3, couleur, ptrsc);
		creationLigneEtTexte(2940, 2, couleur, ptrsc);
		creationLigneEtTexte(2930, 2, couleur, ptrsc);
		creationLigneEtTexte(2925, 2, couleur, ptrsc);
		creationLigneEtTexte(2920, 2, couleur, ptrsc);
		creationLigneEtTexte(2910, 2, couleur, ptrsc);
		creationLigneEtTexte(2900, 4, couleur, ptrsc);
		creationLigneEtTexte(2890, 2, couleur, ptrsc);
		creationLigneEtTexte(2888, 1, couleur, ptrsc);
		creationLigneEtTexte(2880, 2, couleur, ptrsc);
		creationLigneEtTexte(2875, 3, couleur, ptrsc);
		creationLigneEtTexte(2870, 2, couleur, ptrsc);
		creationLigneEtTexte(2860, 2, couleur, ptrsc);
		creationLigneEtTexte(2857.1, 1, couleur, ptrsc);
		creationLigneEtTexte(2850, 3, couleur, ptrsc);
		creationLigneEtTexte(2840, 2, couleur, ptrsc);
		creationLigneEtTexte(2833, 1, couleur, ptrsc);
		creationLigneEtTexte(2830, 2, couleur, ptrsc);
		creationLigneEtTexte(2825, 2, couleur, ptrsc);
		creationLigneEtTexte(2820, 2, couleur, ptrsc);
		creationLigneEtTexte(2810, 2, couleur, ptrsc);
		creationLigneEtTexte(2800, 4, couleur, ptrsc);
		creationLigneEtTexte(2790, 2, couleur, ptrsc);
		creationLigneEtTexte(2780, 2, couleur, ptrsc);
		creationLigneEtTexte(2777, 1, couleur, ptrsc);
		creationLigneEtTexte(2775, 2, couleur, ptrsc);
		creationLigneEtTexte(2770, 2, couleur, ptrsc);
		creationLigneEtTexte(2760, 2, couleur, ptrsc);
		creationLigneEtTexte(2750, 4, couleur, ptrsc);
		creationLigneEtTexte(2740, 2, couleur, ptrsc);
		creationLigneEtTexte(2730, 2, couleur, ptrsc);
		creationLigneEtTexte(2725, 2, couleur, ptrsc);
		creationLigneEtTexte(2720, 2, couleur, ptrsc);
		creationLigneEtTexte(2714.2, 1, couleur, ptrsc);
		creationLigneEtTexte(2710, 2, couleur, ptrsc);
		creationLigneEtTexte(2700, 4, couleur, ptrsc);
		creationLigneEtTexte(2690, 2, couleur, ptrsc);
		creationLigneEtTexte(2680, 2, couleur, ptrsc);
		creationLigneEtTexte(2675, 2, couleur, ptrsc);
		creationLigneEtTexte(2670, 2, couleur, ptrsc);
		creationLigneEtTexte(2666.66, 2, couleur, ptrsc);
		creationLigneEtTexte(2666, 3, couleur, ptrsc);
		creationLigneEtTexte(2660, 2, couleur, ptrsc);
		creationLigneEtTexte(2650, 3, couleur, ptrsc);
		creationLigneEtTexte(2640, 2, couleur, ptrsc);
		creationLigneEtTexte(2630, 2, couleur, ptrsc);
		creationLigneEtTexte(2625, 2, couleur, ptrsc);
		creationLigneEtTexte(2620, 2, couleur, ptrsc);
		creationLigneEtTexte(2610, 2, couleur, ptrsc);
		creationLigneEtTexte(2600, 4, couleur, ptrsc);
		creationLigneEtTexte(2590, 10, couleur, ptrsc);

	}
}



